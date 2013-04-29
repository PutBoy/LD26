#include "Guy.h"
#include "Girl.h"
#include "Utility.h"
#include "EntityManager.h"
#include "Simulator.h"
#include "Fire.h"
#include "KillEvent.h"

Guy::Guy(float x, float y)
	:mPosition(x, y)
	,mCurrentAnim(AnimDown)
	,mHeat(0.f)
{
}

void Guy::update()
{
	++mFrameCounter;

	mHeat -= 0.2f;

	if (mHeat < 0.f)
		mHeat = 0.f;
	if (mHeat > 100.f)
	{
		EntityManager::getInstance()->addEntity(std::make_shared<Fire>(getThis(), 2.f));
		Simulator::getInstance()->addEvent(Simulator::getInstance()->getNow() + 2.f, std::make_shared<KillEvent>(getThis())); 
	}


	std::shared_ptr<Girl> girl = maGurl.lock();
	if (girl)
	{
									
		mPosition += mDirection * 1.2f;

		mDirection = girl->getPosition() - mPosition;

		if (Util::length(mDirection) < 12.f)
		{
			mStahp = true;
			mDirection = sf::Vector2f();
		}
		else
		{
			mStahp = false;

			Util::normalize(mDirection);

			if (mDirection.x < -0.5f)
			{
				mCurrentAnim = AnimLeft;
				//mDirection.x = -1.f;
			}
			else if (mDirection.x > 0.5f)
			{
				mCurrentAnim = AnimRight;
				//mDirection.x = 1.f;
			}
			else
				//mDirection.x = 0.f;

			if (mDirection.y < -0.5f)
			{
				mCurrentAnim = AnimUp;
				//mDirection.y = -1.f;
			}
			else if (mDirection.y > 0.5f)
			{
				mCurrentAnim = AnimDown;
				//mDirection.y = 1.f;
			}
			else
				//mDirection.y = 0.f;

			Util::normalize(mDirection);

		}
	}
	else
	{
		mStahp = true;
		//TRY TO FIND MA GURL
		EntityManager* manager = EntityManager::getInstance();
		sf::FloatRect searchBox(mPosition.x - 16.f, mPosition.y - 16.f, 32.f, 32.f);
		auto entities = manager->getIntersectingEntities(searchBox);
		
		for (auto& entity : entities)
		{
			auto girl = std::dynamic_pointer_cast<Girl>(entity);
			if (girl)
				maGurl = girl;
		}
	}
}

void Guy::render(Canvas& canvas)
{	
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

	canvas.addSprite(mPosition.x, mPosition.y, 2, frame, 1, fireColor);
}

sf::FloatRect Guy::getBounds()
{
	return sf::FloatRect(mPosition.x - 5, mPosition.y - 5, 10, 10);
}

void Guy::handleCollision(std::shared_ptr<Entity> other)
{
	auto fire = std::dynamic_pointer_cast<Fire>(other);

	if (fire)
	{
		mHeat += fire->getHeat() / 75.f;
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

void Guy::setAblaze()
{
}

bool Guy::isAblaze()
{
	return true;
}

sf::Vector2f Guy::getPosition()
{
	return mPosition;
}

void Guy::push(sf::Vector2f direction)
{
	mPosition += direction;
}