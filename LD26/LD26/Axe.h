#ifndef ANDYOUHAVEMYAXE_H
#define ANDYOUHAVEMYAXE_H

#include "Item.h"
#include <SFML\System\Vector2.hpp>

class Axe : public Item
{
public:
	Axe(float x, float y);
	
	virtual void update();
	virtual void render(Canvas& canvas);
	virtual sf::FloatRect getBounds();
	virtual void handleCollision(std::shared_ptr<Entity>);
	virtual void setAblaze();
	virtual bool isAblaze();
	virtual sf::Vector2f getPosition();

	virtual void pickUp(std::shared_ptr<Entity>);
	virtual bool useOn(std::shared_ptr<Entity>, float strength);
private:
	sf::Vector2f mPosition;
	float uses;

};

#endif
