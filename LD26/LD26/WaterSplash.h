#ifndef WATERSPLASH_H
#define WATERSPLASH_H

#include "Entity.h"
#include <SFML\System\Vector2.hpp>

class WaterSplash : public Entity
{
public:
	WaterSplash(float x, float y);

	virtual void update();
	virtual void render(Canvas& canvas);
	virtual sf::FloatRect getBounds();
	virtual void handleCollision(std::shared_ptr<Entity>);
	virtual void setAblaze();
	virtual bool isAblaze();
	virtual sf::Vector2f getPosition();

	virtual bool isSolid() {return false;};
	
private:
	int mFrameCount;
	sf::Vector2f mPosition;
};

#endif