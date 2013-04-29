#include "Simulator.h"
#include "Display.h"
#include <memory>
#include "DummyEvent.h"
#include "Canvas.h"
#include "StateManager.h"
#include "Game.h"
#include "FireEvent.h"
#include "Debugger.h"
#include "Globals.h"

#include <sstream>
#include <fstream>
#include <iomanip>
#include "MainMenu.h"

std::string debugFileName;

#define OVERRIDEDEBBUGGER 1
void printDebugToFile(const char* messenger, const char* message, unsigned int lineNumber)
{
	static sf::Clock time;
	static std::string debugMessage;

	sf::Int32 milliSec = time.restart().asMilliseconds();

	std::stringstream ss;
	if (!debugMessage.empty())
		ss << "\n";
	
	ss << messenger << ": " << message << " [line " << lineNumber << "]" << " [" << milliSec << "ms]";
		
	debugMessage.append(ss.str());
	

	std::ofstream logfile(debugFileName);
	logfile << debugMessage << std::flush;
	logfile.close();

	std::ofstream file("debug.txt");
	file << debugMessage << std::flush;
	file.close();
	

	//this will disregard the time it took to print the debug message to the file.
	//Should we do that?
	time.restart();
}


int main()
{
	std::time_t t = std::time(NULL);
    std::tm tm = *std::localtime(&t);

	std::stringstream ss;
	ss << "logs/debug" << std::put_time(&tm, "%y%m%d-%H%M") << ".txt";
	debugFileName = ss.str();

	Debugger::setPrintFunction(&printDebugToFile);

	Globals::screenSize.x = 930;
	Globals::screenSize.y = 930;
	Globals::timeSpeed = 1.0f;

	Display display(Globals::screenSize.x, Globals::screenSize.y, "I WANNA SAVE THE BOY!");
	Canvas canvas("spritesheet.png");
	StateManager states;
	states.pushState(std::make_shared<MainMenu>(states));
	Simulator* sim = Simulator::getInstance();

	while (display.isOpen() && !states.empty())
	{
		display.pollEvents();

		states.update();
		sim->update();

		display.clear();
		states.render(canvas);

		canvas.render(display.getTarget());		

		display.flip();
		
	}

}