#include "Girl.h"
#include <functional>
#include <SFML\Window\Keyboard.hpp>
#include "Utility.h"
#include <memory>
#include "Tree.h"
#include "EntityManager.h"
#include "Item.h"
#include "Cottage.h"
#include "Guy.h"
#include "Item.h"
#include "Simulator.h"
#include "Fire.h"
#include "KillEvent.h"

//HI GURL!
Girl::Girl(float x, float y)
	:mCurrentSelectedItemIndex(-1)
	,mSelectionTimer(0)
	,mHeat(0.f)
{
	mPosition.x = x;
	mPosition.y = y;

	evHandler.addEventListener(sf::Event::KeyPressed, std::bind(&Girl::keyPressed, this, std::placeholders::_1));
	mCurrentAnim = AnimDown;
}

void Girl::update()
{
	mHeat -= 0.2f;

	if (mHeat < 0.f)
		mHeat = 0.f;
	if (mHeat > 100.f)
	{
		EntityManager::getInstance()->addEntity(std::make_shared<Fire>(getThis(), 2.f));
		Simulator::getInstance()->addEvent(Simulator::getInstance()->getNow() + 2.f, std::make_shared<KillEvent>(getThis())); 
	}

	for (auto item : mItems)
	{
		item->update();
	}

	if (mSelectedItem)
	{
		if (mSelectedItem->isDead())
		{
			mItems.erase(std::remove(mItems.begin(), mItems.end(), mSelectedItem), mItems.end());
			if (mCurrentSelectedItemIndex >= mItems.size())
				mCurrentSelectedItemIndex = 0;

			if (mItems.size() != 0)
				mSelectedItem = mItems[mCurrentSelectedItemIndex];
			else
				mSelectedItem = nullptr;
		}
	}

	updateKeyState();
	
	mStahp = true;

	++mFrameCounter;
	--mSelectionTimer;
	if (mSelectionTimer < 0)
		mSelectionTimer = 0;

	if (keyState & UP)
	{
		mDirection.y -= 1.f;
		mCurrentAnim = AnimUp;
		mStahp = false;
	}
	else if (keyState & DOWN)
	{
		mDirection.y += 1.f;
		mCurrentAnim = AnimDown;
		mStahp = false;
	}
	else
		mDirection.y = 0;

	if (keyState & LEFT)
	{
		mDirection.x -= 1.f;
		mCurrentAnim = AnimLeft;
		mStahp = false;
	}
	else if (keyState & RIGHT)
	{
		mDirection.x += 1.f;
		mCurrentAnim = AnimRight;
		mStahp = false;
	}
	else
	{
		mDirection.x = 0;
	}

	if (keyState & USE)
	{
		if (mSelectedItem)
			useItem();
	}	

	if (keyState & NEXTITEM)
	{
		if (mSelectionTimer <= 0)
		{
			int thisItemType = mSelectedItem->itemType();


			int find0 = -1;
			for (int i = mCurrentSelectedItemIndex; i < mItems.size(); ++i)
			{
				if (mItems[i]->itemType() != thisItemType)
				{
					find0 = i;
					break;
				}
			}

			int find1 = -1;
			for (int i = 0; i < mCurrentSelectedItemIndex; ++i)
			{
				if (mItems[i]->itemType() != thisItemType)
				{
					find1 = i;
					break;
				}
			}

			if (find0 >= 0)
				mCurrentSelectedItemIndex = find0;
			else if (find1 >= 0)
				mCurrentSelectedItemIndex = find1;
			else
			{

				++mCurrentSelectedItemIndex;
				if (mCurrentSelectedItemIndex >= mItems.size())
					mCurrentSelectedItemIndex = 0;
			}

			if (mItems.size() != 0)
				mSelectedItem = mItems[mCurrentSelectedItemIndex];
			else
				mSelectedItem = nullptr;

			mSelectionTimer = 25;
		}
	}

	if (keyState & DROPITEM)
	{
		if (mSelectionTimer <= 0 && mSelectedItem)
		{
			//dropping an item crashes the game, disable for now
			EntityManager::getInstance()->addEntity(mSelectedItem);
			mSelectedItem->pickUp(nullptr);

			mItems.erase(std::remove(mItems.begin(), mItems.end(), mSelectedItem), mItems.end());

			if (mCurrentSelectedItemIndex >= mItems.size())
				mCurrentSelectedItemIndex = 0;

			if (mItems.size() != 0)
				mSelectedItem = mItems[mCurrentSelectedItemIndex];
			else
				mSelectedItem = nullptr;

			mSelectionTimer = 50;
		}
		
	}

	Util::normalize(mDirection);

	mPosition += mDirection * 1.5f;

}

void Girl::updateKeyState()
{
	keyState = 0;
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		keyState |= UP;
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		keyState |= DOWN;
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		keyState |= LEFT;
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		keyState |= RIGHT;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		keyState |= USE;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
		keyState |= NEXTITEM;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		keyState |= DROPITEM;

}

void Girl::render(Canvas& canvas)
{
	if (mSelectedItem)
		mSelectedItem->render(canvas);

	int frame;
	if (mStahp)
	{
		frame = 0;
	}
	else
	{
		frame = (mFrameCounter / 30) % 2 + 1;
	}
	
	frame += mCurrentAnim * 3;
	sf::Color fireColor(255, 255 - mHeat * 2, 255 - mHeat * 2, 255);

	canvas.addSprite(mPosition.x, mPosition.y, 2, frame, 0, fireColor);
}

sf::FloatRect Girl::getBounds()
{
	return sf::FloatRect(mPosition.x - 5, mPosition.y - 5, 10, 10);
}

void Girl::handleCollision(std::shared_ptr<Entity> other)
{
	auto item = std::dynamic_pointer_cast<Item>(other);
	auto guy = std::dynamic_pointer_cast<Guy>(other);
	auto fire = std::dynamic_pointer_cast<Fire>(other);

	if (fire)
	{
		mHeat += fire->getHeat() / 75.f;
	}

	if (item)
	{
		pickUpItem(item);
	}

	if (guy)
	{
		sf::Vector2f direction = guy->getPosition() - mPosition;
		direction = Util::getMajorVector(Util::getNormalized(direction));
		guy->push(direction);
	}

	if (other->isSolid())
	{
		sf::FloatRect otherBox = other->getBounds();
		sf::FloatRect myBox = getBounds();
		
		// I'm pretty sure this only works because our boxes are all square and have the same size, idc doe!
		sf::Vector2f major = Util::getMajorVector(Util::getNormalized(Util::getRectDiffRatio(otherBox, myBox)));

		if (major.x < -.5)
		{
			mDirection.x = 0;
			mPosition.x = otherBox.left - myBox.width / 2;
		}
		else if (major.x > .5)
		{
			mDirection.x = 0;
			mPosition.x = otherBox.left + otherBox.width + myBox.width / 2;
		}
		else if (major.y < -.5)
		{
			mDirection.y = 0;
			mPosition.y = otherBox.top - myBox.height / 2;
		}
		else if (major.y > .5)
		{
			mDirection.y = 0;
			mPosition.y = otherBox.top + otherBox.height + myBox.height / 2;
		}
	}
}

void Girl::setAblaze()
{

}

bool Girl::isAblaze()
{
	return true;
}

void Girl::keyPressed(sf::Event& ev)
{
	
}

sf::Vector2f Girl::getPosition()
{
	return mPosition;
}

void Girl::pickUpItem(std::shared_ptr<Item> item)
{
	if (mSelectionTimer <= 0)
	{
		mItems.push_back(item);
		mSelectedItem = mItems.back();
		mCurrentSelectedItemIndex = mItems.size() - 1;
		EntityManager* manager = EntityManager::getInstance();
		item->pickUp(getThis());

		manager->removeEntity(item);
	}
}

void Girl::useItem()
{
	if (!mSelectedItem)
		return;

	//find some entity
	sf::Vector2f useDirection;

	if (mCurrentAnim == AnimUp)
		useDirection = sf::Vector2f(0.f, -1.f);
	else if (mCurrentAnim == AnimDown)
		useDirection = sf::Vector2f(0.f, 1.f);
	else if (mCurrentAnim == AnimLeft)
		useDirection = sf::Vector2f(-1.f, 0);
	else if (mCurrentAnim == AnimRight)
		useDirection = sf::Vector2f(1.f, 0);

	useDirection *= 12.f;
	sf::Vector2f usePosition = mPosition + useDirection;

	sf::FloatRect useRect(usePosition.x - 4, usePosition.y - 4, 8, 8);

	EntityManager* manager = EntityManager::getInstance();
	auto entities = manager->getIntersectingEntities(useRect);
	entities.erase(std::remove(entities.begin(), entities.end(), getThis()), entities.end());
	
	//try and always use the item because it's more fun that way!

	entities.push_back(nullptr);
	for (auto& entity : entities)
	{
 		if (mSelectedItem->useOn(entity, 2.f))
			break;
	}
}