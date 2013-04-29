#include "Fire.h"
#include "Simulator.h"
#include "KillEvent.h"
#include "Utility.h"
#include "EntityManager.h"
#include "BurnedLog.h"
#include "Debugger.h"

Fire::Fire(std::shared_ptr<Entity> torchWood, float time)
	:mInitialized(false)
	,mTime(time)
	,mHeat(1.f)
	,mTorchWood(torchWood)
{
	mPosition = torchWood->getPosition();
	mPosition.y += 6;
}

void Fire::update()
{
	if (!mTorchWood.expired())
	{
		mPosition = mTorchWood.lock()->getPosition();
		mPosition.y -= 6;
	}

	++mFrameCount;
	mHeat += 0.1f;
	if (mHeat > 100.f)
		mHeat = 100.f;
	else if (mHeat < 0.f)
	{
		kill();
	}

	//THE HACKS DOE!
	if (!mInitialized)
	{
		Simulator* sim = Simulator::getInstance();
		sim->addEvent(mTime, std::make_shared<KillEvent>(getThis()));
		mInitialized = true;
	}

}

void Fire::render(Canvas& canvas)
{
	int frame = (mFrameCount / 20) % 3;
	float scale = (mHeat / 100.f) * 1.5f;
	if (scale < 0.5f)
		scale = 0.5f;

	canvas.addSprite(mPosition.x, mPosition.y, 2, 0 + frame, 3, sf::Color::White, scale);
}

sf::FloatRect Fire::getBounds()
{

	return sf::FloatRect(mPosition.x - 10, mPosition.y - 10, 20, 20);
}

void Fire::handleCollision(std::shared_ptr<Entity>)
{

}

void Fire::setAblaze()
{

}

bool Fire::isAblaze()
{
	return true;
}

sf::Vector2f Fire::getPosition()
{
	return mPosition;
}

void Fire::extinguish(float strength)
{
	mHeat -= strength * 0.5f;
	if (mHeat < 0.f)
		kill();
}

void Fire::onDeath()
{

}