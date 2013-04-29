#include "StateManager.h"
#include <algorithm>
#include "Display.h"
#include "Globals.h"


StateManager::StateManager()
	:msElapsed(0.f)
	,msUpdateRate((1000.f / 60.f) / Globals::timeSpeed)
{
	clock.restart();
}

void StateManager::pushState(std::shared_ptr<State> state)
{
	//when pushing a state we must retart our clock
	mStateStack.push_back(state);
	clock.restart();
}

void StateManager::update()
{
	//remove dead states
	auto pred = [](std::shared_ptr<State> state) {return !state->isAlive();};
	mStateStack.erase(
			std::remove_if(mStateStack.begin(), mStateStack.end(), pred),
			mStateStack.end());

	//If empty do nothing
	if (mStateStack.empty())
		return;

	//update state using a defined timestep
	for (msElapsed += clock.restart().asMilliseconds(); msElapsed > msUpdateRate; msElapsed -= msUpdateRate)
	{
		mStateStack.back()->update();
	}
}

void StateManager::render(Canvas& canvas)
{
	if (!mStateStack.empty())
		mStateStack.back()->render(canvas);
}

bool StateManager::empty()
{
	return mStateStack.empty();
}