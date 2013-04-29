#include "BurnedLog.h"

BurnedLog::BurnedLog(float x, float y)
	:mPosition(x, y)
{
}

void BurnedLog::update()
{

}

void BurnedLog::render(Canvas& canvas)
{
	canvas.addSprite(mPosition.x, mPosition.y, 0, 2, 2);
}

sf::FloatRect BurnedLog::getBounds()
{
	return sf::FloatRect();	
}

void BurnedLog::handleCollision(std::shared_ptr<Entity>)
{
}

void BurnedLog::setAblaze()
{
	
}

bool BurnedLog::isAblaze()
{
	return true;
}

sf::Vector2f BurnedLog::getPosition()
{
	return mPosition;
}