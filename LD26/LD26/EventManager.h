#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <functional>
#include <SFML\System\NonCopyable.hpp>
#include <SFML\Window\Event.hpp>
#include <map>

class EventHandler;

class EventManager: public sf::NonCopyable
{
public:
	static EventManager* getInstance();

	void newEvent(sf::Event&);
	void registerEventHandler(EventHandler*, sf::Event::EventType, std::function<void (sf::Event&)>);
	void unregisterEventHandler(EventHandler*);
private:
	EventManager();

	std::map<sf::Event::EventType, std::map<EventHandler*, std::function<void (sf::Event&)>>> mEventHandlers;
};

#endif