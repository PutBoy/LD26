#include "EventHandler.h"
#include "EventManager.h"

EventHandler::EventHandler(sf::Event::EventType eventType, std::function<void (sf::Event&)> eventFunction)
{
	addEventListener(eventType, eventFunction);
}

EventHandler::EventHandler()
{

}

EventHandler::~EventHandler()
{
	EventManager::getInstance()->unregisterEventHandler(this);
}

void EventHandler::addEventListener(sf::Event::EventType eventType, std::function<void (sf::Event&)> eventFunction)
{
	EventManager::getInstance()->registerEventHandler(this, eventType, eventFunction);
}