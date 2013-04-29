#include "SimulationEvent.h"
#include "Simulator.h"

SimulationEvent::SimulationEvent()
	:simulator(Simulator::getInstance())
{

}

void SimulationEvent::addNewEvent(float time, std::shared_ptr<SimulationEvent> ev)
{
	simulator->addEvent(time, ev);
}

float SimulationEvent::getNow()
{
	return simulator->getNow();
}