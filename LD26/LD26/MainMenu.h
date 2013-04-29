#ifndef MAINMENU_H
#define MAINMENU_H

#include "State.h"
#include <functional>
#include <vector>

class StateManager;
class Canvas;

class MainMenu: public State
{
public:
	class MainMenu(StateManager& stateManager);

	virtual void render(Canvas&);
	virtual void update();
	virtual bool isAlive();
private:
	bool mIsAlive;
	std::vector<std::pair<std::string, std::function<void()>>> mMenuItems;

	int mMenuSelection;
	int mSelectionTimer;
	int mEnterTimer;
};

#endif