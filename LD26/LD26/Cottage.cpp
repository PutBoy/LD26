#include "Cottage.h"

Cottage::Cottage(float x, float y, int type)
	:mPosition(x, y)
	,mType(type)
	,mBounds(x - 5, y - 5, 44, 28)
{
}

void Cottage::update()
{

}

void Cottage::render(Canvas& canvas)
{
	canvas.addSprite(mPosition.x, mPosition.y, 2, 0, 6);
	canvas.addSprite(mPosition.x + 16, mPosition.y, 2, 1, 6);
	canvas.addSprite(mPosition.x + 32, mPosition.y, 2, 2, 6);

	canvas.addSprite(mPosition.x, mPosition.y + 16, 2, 0, 7);
	canvas.addSprite(mPosition.x + 16, mPosition.y + 16, 2, 1, 7);
	canvas.addSprite(mPosition.x + 32, mPosition.y + 16, 2, 2, 7);

}

sf::FloatRect Cottage::getBounds()
{
	return mBounds;
}

void Cottage::handleCollision(std::shared_ptr<Entity>)
{

}

void Cottage::setAblaze()
{
}

bool Cottage::isAblaze()
{
	return false;
}

sf::Vector2f Cottage::getPosition()
{
	return mPosition;
}