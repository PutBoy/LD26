#ifndef DISPLAY_H
#define DISPLAY_H

#include <SFML\Graphics\RenderWindow.hpp>
#include <string>
#include "EventManager.h"
#include "EventHandler.h"


class Canvas;

class Display: public EventHandler
{
public:

	Display(size_t w, size_t h, std::string title);
	
	void renderCanvas(Canvas* canvas);
	void flip();
	void clear();
	void pollEvents();
	bool isOpen();

	void close();

	sf::RenderTarget& getTarget();

private:
	void closeEvent(sf::Event&);

	EventManager* eventManager;
	sf::RenderWindow sfWindow;
};

#endif
