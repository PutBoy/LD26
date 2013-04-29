#ifndef ENTITY_H
#define ENTITY_H

#include "Canvas.h"
#include <SFML\Graphics\Rect.hpp>
#include <memory>

class Entity : public std::enable_shared_from_this<Entity>
{
public:
	Entity();
	virtual ~Entity() ;

	virtual void update() = 0;
	virtual void render(Canvas& canvas) = 0;
	virtual sf::FloatRect getBounds() = 0;
	virtual void handleCollision(std::shared_ptr<Entity>) = 0;
	virtual void setAblaze() = 0;
	virtual bool isAblaze() = 0;
	virtual sf::Vector2f getPosition() = 0;

	virtual bool isSolid() {return false;};
	virtual bool needsCollision() {return true;};
	virtual bool nonCollidable() {return false;};

	bool isDead() {return mIsDead;};
	void kill() {
		if (!mIsDead)	
			onDeath();
		mIsDead = true; 
	};

protected:
	//this is a function that gets called whenever kill() is called. This can be implemented
	//in a subclass that needs to do something when it dies
	virtual void onDeath() {};

	std::shared_ptr<Entity> getThis()
    {
        return shared_from_this();
    };

private:
	bool mIsDead;

	static int mEntityCount;
};	

#endif