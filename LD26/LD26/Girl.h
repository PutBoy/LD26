#ifndef GIRL_H
#define GIRL_H

#include "EventHandler.h"
#include "Entity.h"
class Item;

class Girl : public Entity
{
public:
	Girl(float x, float y);
	virtual void update();
	virtual void render(Canvas& canvas);
	virtual void handleCollision(std::shared_ptr<Entity>);
	virtual void setAblaze();
	virtual bool isAblaze();
	virtual sf::Vector2f getPosition();
	virtual sf::FloatRect getBounds();
	virtual bool isSolid() {return true;};


	void pickUpItem(std::shared_ptr<Item> item);

private:
	
	void keyPressed(sf::Event& );
	void updateKeyState();

	void useItem();

	enum Key{	UP = 0x0001,
				DOWN = 0x0002,
				LEFT = 0x0004,
				RIGHT = 0x0008,
				USE =  0x10,
				NEXTITEM = 0x20,
				DROPITEM = 0x40};

	enum Animation{AnimUp, AnimDown, AnimLeft, AnimRight};

	Animation mCurrentAnim;
	bool mStahp;
	size_t mFrameCounter;

	int keyState;


	sf::Vector2f mDirection;
	sf::Vector2f mPosition;

	EventHandler evHandler;

	std::vector<std::shared_ptr<Item>> mItems;
	std::shared_ptr<Item> mSelectedItem;
	int mCurrentSelectedItemIndex;
	int mSelectionTimer;
	float mHeat;

};

#endif