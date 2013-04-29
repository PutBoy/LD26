#ifndef FIREFIGTHER_H
#define FIREFIGTHER_H

#include <memory>
#include "Entity.h"
#include "Axe.h"
#include "FireHose.h"

class Tree;

class FireFighter : public Entity
{
public:
	FireFighter(float x, float y, std::shared_ptr<Entity> destination);

	virtual void update();
	virtual void render(Canvas& canvas);
	virtual sf::FloatRect getBounds();
	virtual void handleCollision(std::shared_ptr<Entity>);
	virtual void setAblaze();
	virtual bool isAblaze();
	virtual sf::Vector2f getPosition();
	virtual bool isSolid() {return true;};

protected:
	virtual void onDeath();
private:
	void extinguishFire();
	void dieByFire();

	std::weak_ptr<Entity> mDestination;
	std::shared_ptr<Axe> mFireAxe;
	std::shared_ptr<FireHose> mFireHose;

	sf::Vector2f mPosition;
	sf::Vector2f mHomePosition;
	
	bool mStahp;
	size_t mFrameCounter;
	int mCurrentAnim;
	sf::Vector2f mDirection;

	std::weak_ptr<Tree> mCurrentChop;
	
	bool mIsAblaze;
	bool mReturnHome;
	bool mIsDoingHisJob;

};

#endif