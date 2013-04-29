#ifndef GOAL_H
#define GOAL_H

#include "StillImage.h"

class Goal : public StillImage
{
public:
	Goal(float x, float y);

	virtual void handleCollision(std::shared_ptr<Entity>);
	virtual sf::FloatRect getBounds();

	bool isWon() {return mHasWon;};

private:
	void winningEpicWinning();
	bool mHasWon;
};

#endif