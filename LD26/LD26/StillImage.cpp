#include "StillImage.h"

StillImage::StillImage(float x, float y, size_t row, size_t col, int layer, sf::Color color, float scale)
	:mColor(color)
	,mScale(scale)
	,mSheetPos(row, col)
	,mPosition(x, y)
	,mLayer(layer)
{
}
	
void StillImage::update()
{
}

void StillImage::render(Canvas& canvas)
{
	canvas.addSprite(mPosition.x, mPosition.y, mLayer, mSheetPos.x, mSheetPos.y, mColor, mScale);
}

sf::FloatRect StillImage::getBounds()
{
	return sf::FloatRect();
}

void StillImage::handleCollision(std::shared_ptr<Entity>)
{
}

void StillImage::setAblaze()
{
}

bool StillImage::isAblaze()
{
	return true;
}

sf::Vector2f StillImage::getPosition()
{
	return mPosition;
}
