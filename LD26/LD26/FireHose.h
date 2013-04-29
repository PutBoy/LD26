#ifndef FIREHOSE_H
#define FIREHOSE_H

#include "Item.h"
#include <SFML\System\Vector2.hpp>

class FireHose : public Item
{
public:
	FireHose(float x, float y);
	
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
	float mUses;
	int mSplashRecharge;

};

#endif
