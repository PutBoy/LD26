#ifndef ITEM_H
#define ITEM_H

#include "Entity.h"
#include <memory>

class Item : public Entity
{
public:

	virtual void update() = 0;
	virtual void render(Canvas& canvas) = 0;
	virtual sf::FloatRect getBounds() = 0;
	virtual void handleCollision(std::shared_ptr<Entity>) = 0;
	virtual void setAblaze() = 0;
	virtual bool isAblaze() = 0;
	virtual sf::Vector2f getPosition() = 0;

	virtual void pickUp(std::shared_ptr<Entity>) = 0;
	virtual bool useOn(std::shared_ptr<Entity>, float strength) = 0;
protected:
	std::shared_ptr<Entity> getUser() 
	{
		if (!mUser.expired())
			return mUser.lock();
		else
		{
			mUser = std::weak_ptr<Entity>();
			return nullptr;
		}
	};

	void setUser(std::shared_ptr<Entity> user) {mUser = user;};
private:
	
	std::weak_ptr<Entity> mUser;
};

#endif