#include "Display.h"
#include <functional>

#include <SFML/Graphics.hpp>

Display::Display(size_t w, size_t h, std::string title)
	:sfWindow(sf::VideoMode(w, h), title)
	,eventManager(EventManager::getInstance())
{
	addEventListener(sf::Event::EventType::Closed, std::bind(&Display::closeEvent, this, std::placeholders::_1));
	sfWindow.setFramerateLimit(60);
}

void Display::pollEvents()
{
    sf::Event ev;
    while (sfWindow.pollEvent(ev))
    {
		eventManager->newEvent(ev);
    }
}

void Display::flip()
{
	sfWindow.display();
}

void Display::clear()
{
	sfWindow.clear(sf::Color::White);
}

bool Display::isOpen()
{
	return sfWindow.isOpen();
}

void Display::close()
{
	sfWindow.close();
}

void Display::closeEvent(sf::Event&)
{
	sfWindow.close();
}

sf::RenderTarget& Display::getTarget()
{
	return sfWindow;
}