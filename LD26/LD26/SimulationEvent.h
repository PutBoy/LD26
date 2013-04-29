#ifndef SIMULATIONEVENT_H
#define SIMULATIONEVENT_H

#include <memory>
#include <SFML\System\Clock.hpp>

class Simulator;

class SimulationEvent
{
public:
	SimulationEvent();

	virtual ~SimulationEvent() {};

	virtual void activate() = 0;
protected:
	void addNewEvent(float time, std::shared_ptr<SimulationEvent> ev);
	float getNow();

private:
	Simulator* simulator;


};

#endif