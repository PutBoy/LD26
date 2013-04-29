#include "WaterSplash.h"
#include "Fire.h"

WaterSplash::WaterSplash(float x, float y)
	:mPosition(x, y)
	,mFrameCount(0)
{

}

void WaterSplash::update()
{
	++mFrameCount;
	if (mFrameCount > 32)
	{
		kill();
		mFrameCount = 32;
	}
}

void WaterSplash::render(Canvas& canvas)
{
	if (!isDead())
	{
		int frame = (mFrameCount / 8);
		canvas.addSprite(mPosition.x, mPosition.y, 3, frame + 3, 3);
	}
}

sf::FloatRect WaterSplash::getBounds()
{
	return sf::FloatRect(mPosition.x - 8, mPosition.y - 8, 16, 16);
}

void WaterSplash::handleCollision(std::shared_ptr<Entity> other)
{
	auto fire = std::dynamic_pointer_cast<Fire>(other);

	if (fire)
		fire->extinguish(1.f);
}

void WaterSplash::setAblaze()
{

}

bool WaterSplash::isAblaze()
{
	return true;
}

sf::Vector2f WaterSplash::getPosition()
{
	return mPosition;
}
