#ifndef WHAT_H
#define WHAT_H

#include "State.h"
#include <functional>
#include <vector>

class StateManager;
class Canvas;

class What: public State
{
public:
	What(StateManager& stateManager);

	virtual void render(Canvas&);
	virtual void update();
	virtual bool isAlive();
private:
	bool mIsAlive;
	std::vector<std::function<void(Canvas&)>> mHelpScreens;

	void screen0(Canvas& canvas);
	void screen1(Canvas& canvas);
	void screen2(Canvas& canvas);

	size_t mCurrentScreen;
	int mDelayTimer;
};

#endif