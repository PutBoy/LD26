#ifndef GUY_H
#define GUY_H

#include "Entity.h"
#include <SFML\System\Vector2.hpp>
#include <deque>

class Girl;

class Guy : public Entity
{
public:
	Guy(float x, float y);

	virtual void update();
	virtual void render(Canvas& canvas);
	virtual sf::FloatRect getBounds();
	virtual void handleCollision(std::shared_ptr<Entity>);
	virtual void setAblaze();
	virtual bool isAblaze();
	virtual sf::Vector2f getPosition();

	virtual bool isSolid() {return true;};

	void push(sf::Vector2f direction);
private:
	sf::Vector2f mPosition;
	sf::Vector2f mDirection;

	enum Animation{AnimUp, AnimDown, AnimLeft, AnimRight};

	Animation mCurrentAnim;
	bool mStahp;

	size_t mFrameCounter;

	std::weak_ptr<Girl> maGurl;
	std::deque<sf::Vector2f> mFollowQueue;
	float mHeat;

};
#endif