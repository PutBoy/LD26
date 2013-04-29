#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <SFML\System\NonCopyable.hpp>
#include <functional>
#include <SFML\Window\Event.hpp>

class EventHandler : public sf::NonCopyable
{
public:
	EventHandler(sf::Event::EventType, std::function<void (sf::Event&)> eventFunction);
	EventHandler();

	void addEventListener(sf::Event::EventType, std::function<void (sf::Event&)> eventFunction);

	~EventHandler();
};
#endif