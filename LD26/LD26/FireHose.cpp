#include "FireHose.h"
#include "Tree.h"
#include "Fire.h"
#include "WaterSplash.h"
#include "EntityManager.h"
#include "Tree.h"

FireHose::FireHose(float x, float y)
	:mPosition(x, y)
	,mUses(0.f)
	,mSplashRecharge(0)
{
}

void FireHose::update()
{
	--mSplashRecharge;
	if (mSplashRecharge < 0)
		mSplashRecharge = 0;

	if (getUser() && !isDead())
		mPosition = getUser()->getPosition();
}

void FireHose::render(Canvas& canvas)
{
	if (isDead())
		return;

	if (!getUser())
	{
		canvas.addSprite(mPosition.x, mPosition.y, 1, 1, 5);
	}
	else
	{
		canvas.addSprite(mPosition.x - 4, mPosition.y + 2, 4, 1, 5, sf::Color::White, 0.75f);
	}
}

sf::FloatRect FireHose::getBounds()
{
	if (!getUser())
		return sf::FloatRect(mPosition.x - 6, mPosition.y - 6, 12, 12);
	else
		return sf::FloatRect();
}

void FireHose::handleCollision(std::shared_ptr<Entity>)
{
	
}

void FireHose::setAblaze()
{

}

bool FireHose::isAblaze()
{
	return false;
}

sf::Vector2f FireHose::getPosition()
{
	return mPosition;
}

void FireHose::pickUp(std::shared_ptr<Entity> other)
{
	setUser(other);
}

bool FireHose::useOn(std::shared_ptr<Entity> other, float strength)
{
	bool success = false;
	
	if (isDead())
 		return false;

	auto fire = std::dynamic_pointer_cast<Fire>(other);
	auto tree = std::dynamic_pointer_cast<Tree>(other);
	sf::Vector2f splashPosition;

	if (fire)
	{
		fire->extinguish(strength);

		success = true;
		
		//there are 100hp on 1 fire
		mUses+=strength;
		splashPosition = fire->getPosition();
	}
	if (tree)
	{
		success = true;
		
		//there are 100hp on 1 fire
		mUses+=strength;
		splashPosition = tree->getPosition();
	}
	else 
		splashPosition = mPosition;
	

	if (mUses > 500.f)
		kill();
	
	if (mSplashRecharge <= 0)
	{
		auto waterSplash = std::make_shared<WaterSplash>(splashPosition.x, splashPosition.y);
		EntityManager::getInstance()->addEntity(waterSplash);
		mSplashRecharge = 20;
	}

	return success;
}