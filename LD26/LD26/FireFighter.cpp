#include "FireFighter.h"
#include "Utility.h"
#include "Tree.h"
#include "Fire.h"
#include "FireFighter.h"
#include "EntityManager.h"
#include "Simulator.h"
#include "KillEvent.h"
#include "Debugger.h"
#include "StillImage.h"
#include "FireHose.h"

FireFighter::FireFighter(float x, float y, std::shared_ptr<Entity> destination)
	:mPosition(x, y)
	,mHomePosition(x, y)
	,mDestination(destination)
	,mIsAblaze(false)
	,mReturnHome(false)
	,mIsDoingHisJob(false)
{
	mCurrentAnim = 1;
}

void FireFighter::update()
{
	if (!mFireAxe)
	{
		mFireAxe = std::make_shared<Axe>(mPosition.x, mPosition.y);
		mFireAxe->pickUp(getThis());
	}
	mFireAxe->update();
	if (!mFireHose)
	{
		mFireHose = std::make_shared<FireHose>(mPosition.x, mPosition.y);
		mFireHose->pickUp(getThis());
	}
	mFireHose->update();

	++mFrameCounter;
	
	//have to do this first so the collision also goes in
	if (!mIsAblaze)
		mPosition += mDirection * .4f;
	else
	{
		mStahp = true;
		return;
	}

	if (mDestination.expired() || !mDestination.lock() || mReturnHome)
	{
		mIsDoingHisJob = false;
		mDirection = mHomePosition - mPosition;
		if (Util::length(mDirection) < 16.f)
			kill();
	}
	else
	{
		mDirection = mDestination.lock()->getPosition() - mPosition;
		if (Util::length(mDirection) < 28.f)
		{
			mDirection = sf::Vector2f();
			extinguishFire();
		}
	}

	Util::normalize(mDirection);

	if (mDirection.x < -0.5f)
		mDirection.x = -1.f;
	else if (mDirection.x > 0.5f)
		mDirection.x = 1.f;
	else
		mDirection.x = 0.f;

	if (mDirection.y < -0.5f)
	{
		mCurrentAnim = 0;
		mDirection.y = -1.f;
	}
	else if (mDirection.y > 0.5f)
	{
		mCurrentAnim = 1;
		mDirection.y = 1.f;
	}
	else
		mDirection.y = 0.f;

	Util::normalize(mDirection);

	if (Util::length(mDirection) < 0.1f)
		mStahp = true;
	else 
		mStahp = false;
	
	if (mCurrentChop.lock() && mCurrentChop.lock()->isDead())
		mCurrentChop = std::weak_ptr<Tree>();

	if (mCurrentChop.lock() && !mCurrentChop.lock()->isDead() && !mFireAxe->isDead())
	{
		mFireAxe->useOn(mCurrentChop.lock(), 1.f);
		mStahp = true;
		mDirection = sf::Vector2f();
	}

	if (mFireAxe->isDead() || mFireHose->isDead())
	{
		mReturnHome = true;
	}

}

void FireFighter::render(Canvas& canvas)
{
	int frame;
	if (mStahp)
	{
		frame = 0;
	}
	else
	{
		frame = (mFrameCounter / 45) % 2 + 1;
	}
	
	frame += mCurrentAnim * 3;
	canvas.addSprite(mPosition.x, mPosition.y, 1, frame, 4);
	
	if (mIsDoingHisJob && mFireHose)
		mFireHose->render(canvas);
	else if (mFireAxe)
		mFireAxe->render(canvas);
}

sf::FloatRect FireFighter::getBounds()
{
	return sf::FloatRect(mPosition.x - 6, mPosition.y - 6, 12, 12);
}

void FireFighter::handleCollision(std::shared_ptr<Entity> other)
{
	std::shared_ptr<Tree> tree = std::dynamic_pointer_cast<Tree>(other);
	std::shared_ptr<FireFighter> fireFighter = std::dynamic_pointer_cast<FireFighter>(other);

	std::shared_ptr<Fire> fire = std::dynamic_pointer_cast<Fire>(other);

	if (fire && other != mDestination.lock())
	{
		dieByFire();
	}

	if (other->isSolid())
	{
		sf::FloatRect otherBox = other->getBounds();
		sf::FloatRect myBox = getBounds();
		
		// I'm pretty sure this only works because our boxes are all square and have the same size, idc doe!

		sf::Vector2f distance = Util::normalize(Util::getRectDiffRatio(otherBox, myBox));

		sf::Vector2f major = Util::getMajorVector(distance);
		
		float shortestRouteMultlipler = 2.f;

		if (major.x < -.5)
		{
			distance.x = 0;
			mDirection.x = 0;
			//this makes the fighter chose the shortest route around the object
			mDirection.y = Util::getNormalized(distance).y * shortestRouteMultlipler;
			mPosition.x = otherBox.left - myBox.width / 2;
		}
		else if (major.x > .5)
		{
			distance.x = 0;
			mDirection.x = 0;
			mDirection.y = Util::getNormalized(distance).y * shortestRouteMultlipler;
			mPosition.x = otherBox.left + otherBox.width + myBox.width / 2;
		}
		else if (major.y < -.5)
		{
			distance.y = 0;
			mDirection.y = 0;
			mDirection.x = Util::getNormalized(distance).x * shortestRouteMultlipler;
			mPosition.y = otherBox.top - myBox.height / 2;
		}
		else if (major.y > .5)
		{
			distance.y = 0;
			mDirection.y = 0;
			mDirection.x = Util::getNormalized(distance).x * shortestRouteMultlipler;
			mPosition.y = otherBox.top + otherBox.height + myBox.height / 2;
		}
	}
	
	//actually fuck trees, CHOP EM DOWN!
	if (tree && !mIsDoingHisJob)
	{
		if (mFireAxe->isDead() || mFireHose->isDead())
		{
			mReturnHome = true;
		}
		else
		{
			mDirection.x = 0;
			mDirection.y = 0;

			mCurrentChop = tree;
		}
	}
}

void FireFighter::setAblaze()
{

}

bool FireFighter::isAblaze()
{
	return mIsAblaze;
}

sf::Vector2f FireFighter::getPosition()
{
	return mPosition;
}

void FireFighter::extinguishFire()
{
	mIsDoingHisJob = true;

	if (!mDestination.expired() && mDestination.lock())
	{
		mFireHose->useOn(mDestination.lock(), 5.f);
	}
}

void FireFighter::dieByFire()
{
	if (!mIsAblaze)
	{
		mIsAblaze = true;
		Simulator* sim = Simulator::getInstance();
		EntityManager* manager = EntityManager::getInstance();

		float time = sim->getNow() + 1.5f;
		sim->addEvent(time, std::make_shared<KillEvent>(getThis()));

		manager->addEntity(std::make_shared<Fire>(getThis(), time));
		manager->addEntity(mFireAxe);
		manager->addEntity(mFireHose);
		mFireHose->pickUp(nullptr);
		mFireAxe->pickUp(nullptr);
	}
}

void FireFighter::onDeath()
{
	if (mIsAblaze)
	{
		EntityManager* manager = EntityManager::getInstance();
		manager->addEntity(std::make_shared<StillImage>(mPosition.x, mPosition.y, 6, 4, 0));
	}
}