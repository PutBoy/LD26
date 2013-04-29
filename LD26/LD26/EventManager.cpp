#include "EventManager.h"
#include <assert.h>

EventManager::EventManager()
{

}

EventManager* EventManager::getInstance()
{
	static EventManager mThisIsAnInstanceFucker;
	return &mThisIsAnInstanceFucker;
}

/*	This calls every function associated with the type of ev
 */
void EventManager::newEvent(sf::Event& ev)
{
	if (mEventHandlers.count(ev.type) != 0)
	{
		for (auto iter = mEventHandlers.at(ev.type).begin(); iter != mEventHandlers.at(ev.type).end(); ++iter)
		{
			(*iter).second(ev);
		}
	}
}

/*	This function adds an eventhandler which responds to a specific type of event.
 */
void EventManager::registerEventHandler(EventHandler* handler, sf::Event::EventType eventType, std::function<void (sf::Event&)> function)
{
	assert(mEventHandlers[eventType].count(handler) == 0);
	mEventHandlers[eventType][handler] = function;
}

/*	This function deletes every function associated with a specific EventHandler-pointer.
 */
void EventManager::unregisterEventHandler(EventHandler* handler)
{
	for (auto iter = mEventHandlers.begin(); iter != mEventHandlers.end(); ++iter)
	{
		(*iter).second.erase(handler);
	}
}
