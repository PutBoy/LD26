#include "Axe.h"

#include "Tree.h"

Axe::Axe(float x, float y)
	:mPosition(x, y)
	,uses(0.f)
{
}

void Axe::update()
{
	if (getUser() && !isDead())
		mPosition = getUser()->getPosition();
}

void Axe::render(Canvas& canvas)
{
	if (isDead())
		return;

	if (!getUser())
	{
		canvas.addSprite(mPosition.x, mPosition.y, 1, 0, 5);
	}
	else
	{
		canvas.addSprite(mPosition.x - 4, mPosition.y + 2, 4, 0, 5, sf::Color::White, 0.75f);
	}
}

sf::FloatRect Axe::getBounds()
{
	if (!getUser())
		return sf::FloatRect(mPosition.x - 6, mPosition.y - 6, 12, 12);
	else
		return sf::FloatRect();
}

void Axe::handleCollision(std::shared_ptr<Entity>)
{
	
}

void Axe::setAblaze()
{

}

bool Axe::isAblaze()
{
	return false;
}

sf::Vector2f Axe::getPosition()
{
	return mPosition;
}

void Axe::pickUp(std::shared_ptr<Entity> other)
{
	setUser(other);
}

bool Axe::useOn(std::shared_ptr<Entity> other, float strength)
{
	bool success = false;
	if (isDead())
		return false;

	std::shared_ptr<Tree> tree = std::dynamic_pointer_cast<Tree>(other);

	if (tree)
	{
		tree->chop(strength);
		success = true;
		
		uses+=strength;

	}
	//there are 100hp on 1 tree
	if (uses > 300.f)
		kill();

	return success;
}