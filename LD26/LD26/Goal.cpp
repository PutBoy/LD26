#include "Goal.h"
#include "Guy.h"

Goal::Goal(float x, float y)
	:StillImage(x, y, 0, 8, 1)
	,mHasWon(false)
{
}

void Goal::handleCollision(std::shared_ptr<Entity> other)
{
	auto guy = std::dynamic_pointer_cast<Guy>(other);
	if (guy)
	{
		winningEpicWinning();
	}
}

void Goal::winningEpicWinning()
{
	mHasWon = true;
}

sf::FloatRect Goal::getBounds()
{
	return sf::FloatRect(getPosition().x - 5, getPosition().y - 5, 10, 10);
}