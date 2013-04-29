#ifndef TREE_H
#define TREE_H

#include "Entity.h"
#include <SFML\System\Vector2.hpp>

class Canvas;

class Tree : public Entity
{
public:
	Tree(float x, float y);

	virtual void update();
	virtual void render(Canvas& canvas);
	virtual void handleCollision(std::shared_ptr<Entity>);
	virtual void setAblaze();
	virtual bool isAblaze();
	virtual sf::Vector2f getPosition();
	virtual sf::FloatRect getBounds();
	virtual bool isSolid() {return true;};
	virtual bool needsCollision() {return false;};

	void chop(float strength);
protected:
	virtual void onDeath();
private:
	sf::Vector2f mPosition;
	bool mIsAblaze;

	float mHealth;
	int mShake;
	float mShakePosition;
	float mShakePositionDirection;
};

#endif