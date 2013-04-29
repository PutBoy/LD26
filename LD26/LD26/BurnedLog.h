#ifndef BURNEDLOG_H
#define BURNEDLOG_H

#include "Entity.h"
#include <SFML\System\Vector2.hpp>

class BurnedLog : public Entity
{
public:
	BurnedLog(float x, float y);

	virtual void update();
	virtual void render(Canvas& canvas);
	virtual sf::FloatRect getBounds();
	virtual void handleCollision(std::shared_ptr<Entity>);
	virtual void setAblaze();
	virtual bool isAblaze();
	virtual sf::Vector2f getPosition();

	virtual bool needsCollision() {return false;};

private:
	sf::Vector2f mPosition;
};

#endif