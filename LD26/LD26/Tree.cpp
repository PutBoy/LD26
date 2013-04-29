#include "Tree.h"
#include "Girl.h"
#include "Simulator.h"
#include "EntityManager.h"
#include "Fire.h"
#include "Utility.h"
#include "KillEvent.h"
#include "BurnedLog.h"

Tree::Tree(float x, float y)
	:mIsAblaze(false)
	,mShake(0)
	,mHealth(100)
{
	mPosition.x = x;
	mPosition.y = y;
}

void Tree::chop(float strength)
{
	mShake = 25;
	mHealth -= strength;
	if (mHealth < 0.f)
		kill();
}

void Tree::update()
{
	--mShake;
	if (mShake < 0)
		mShake = 0;


}


void Tree::render(Canvas& canvas)
{
	//has to be done here or the shake animation doesn't show
	mShakePosition += mShakePositionDirection;
	if (mShakePosition > 1.f)
	{
		mShakePositionDirection = -0.33f;
		mShakePosition = 1.f;
	}
	else if(mShakePosition < -1.f)
	{
		mShakePositionDirection = 0.33f;
		mShakePosition = -1.f;
	}

	float positionOffset = 0;
	int color = 255;
	if (mShake > 0)
	{
		positionOffset = mShakePosition;
		color = ((2.f / (mShakePosition + 1.f)) * 50.f) + 205;
		if (color > 255)
			color = 255;
		else if (color < 127)
			color = 127;
	}


	canvas.addSprite(mPosition.x + positionOffset, mPosition.y, 1, 1, 2, sf::Color(color, color, color));
	canvas.addSprite(mPosition.x + positionOffset, mPosition.y, 2, 0, 2, sf::Color(color, color, color));
}

sf::FloatRect Tree::getBounds()
{
	return sf::FloatRect(mPosition.x + 2 - 8, mPosition.y + 2 - 8, 12, 12);
}

void Tree::handleCollision(std::shared_ptr<Entity> other)
{

}

void Tree::setAblaze()
{
	EntityManager* entities = EntityManager::getInstance();
	Simulator* sim = Simulator::getInstance();

	float time = Util::getRandom(24.f, 48.f);
	sim->addEvent(sim->getNow() + time, std::make_shared<KillEvent>(getThis()));
	entities->addEntity(std::make_shared<Fire>(getThis(), sim->getNow() + time));
	
	mIsAblaze = true;
}

bool Tree::isAblaze()
{
	return mIsAblaze;
}

sf::Vector2f Tree::getPosition()
{
	return mPosition;
}

void Tree::onDeath()
{
	EntityManager* manager = EntityManager::getInstance();
	if (mIsAblaze)
		manager->addEntity(std::make_shared<BurnedLog>(mPosition.x, mPosition.y));
	//else
		//manager->addEntity(std::make_shared<RegularLog>(mPosition.x, mPosition.y));
}