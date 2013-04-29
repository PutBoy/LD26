#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <deque>
#include <memory>
#include "SimulationEvent.h"
#include <SFML\System\NonCopyable.hpp>

class Simulator: public sf::NonCopyable
{
public:
	typedef std::pair<float, std::shared_ptr<SimulationEvent>> PQtype;

	static Simulator* getInstance();

	void update();
	float getNow();

	void addEvent(float time, std::shared_ptr<SimulationEvent> ev);
	
	void clearAll();
private:

	std::deque<PQtype> eventQueue;
	Simulator();
	sf::Clock mClock;
	bool mDoSort;
};

#endif