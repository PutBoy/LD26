#ifndef FIRE_H
#define FIRE_H

#include "Entity.h"
#include <SFML\System\Vector2.hpp>

class Fire : public Entity
{
public:
	Fire(std::shared_ptr<Entity> torchWood, float time);

	virtual void update();
	virtual void render(Canvas& canvas);
	virtual sf::FloatRect getBounds();
	virtual void handleCollision(std::shared_ptr<Entity>);
	virtual void setAblaze();
	virtual bool isAblaze();
	virtual sf::Vector2f getPosition();

	virtual bool needsCollision() {return false;};

	void extinguish(float);
	float getHeat() {return mHeat;};

protected:
	virtual void onDeath();

private:
	sf::Vector2f mPosition;
	bool mInitialized;
	float mTime;
	size_t mFrameCount;

	std::weak_ptr<Entity> mTorchWood;
	float mHeat;
};

#endif