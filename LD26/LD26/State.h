#ifndef STATE_H
#define STATE_H

#include <SFML\System\NonCopyable.hpp>
class StateManager;
class Canvas;

class State: public sf::NonCopyable
{
public:
	class State(StateManager& stateManager) :mStateManager(stateManager){};
	virtual ~State() {};
	virtual void render(Canvas&) = 0;
	virtual void update() = 0;
	virtual bool isAlive() = 0;

	StateManager& getStateManager() {return mStateManager;};
private:
	StateManager& mStateManager;
};

#endif