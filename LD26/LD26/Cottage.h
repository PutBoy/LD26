#ifndef COTTAGE_H
#define COTTAGE_H

#include "Entity.h"
#include <SFML\System\Vector2.hpp>

class Cottage : public Entity
{
public:
	Cottage(float x, float y, int type);

	virtual void update();
	virtual void render(Canvas& canvas);
	virtual sf::FloatRect getBounds();
	virtual void handleCollision(std::shared_ptr<Entity>);
	virtual void setAblaze();
	virtual bool isAblaze();
	virtual sf::Vector2f getPosition();

	virtual bool isSolid() {return true;};
	virtual bool needsCollision() {return false;};


private:
	sf::Vector2f mPosition;
	int mType;
	sf::FloatRect mBounds;
};

#endif
