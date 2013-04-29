#include "Simulator.h"
#include <algorithm>

#include "Debugger.h"
#include "Globals.h"
#include "Utility.h"

Simulator::Simulator()
	:mDoSort(false)
{
}

void Simulator::update()
{

	if (eventQueue.empty())
		return;
	
	float time = getNow() * Globals::timeSpeed;

	if (mDoSort)
		std::sort(eventQueue.begin(), eventQueue.end(), [](PQtype& e0, PQtype& e1){return e0.first < e1.first;});

	float thisTime = eventQueue.front().first;


	while (!eventQueue.empty() && (eventQueue.front().first < time))
	{

		float thisTime = eventQueue.front().first;
		eventQueue.front().second->activate();
		eventQueue.pop_front();

		if (mDoSort)
			std::sort(eventQueue.begin(), eventQueue.end(), [](PQtype& e0, PQtype& e1){return e0.first < e1.first;});

	}

	mDoSort = false;

}

void Simulator::addEvent(float time, std::shared_ptr<SimulationEvent> ev)
{
	eventQueue.push_back(std::make_pair(time, ev));
	mDoSort = true;
}

float Simulator::getNow()
{
	return mClock.getElapsedTime().asSeconds();
}

Simulator* Simulator::getInstance()
{
	static Simulator thisIsASimulatorInstanceIfYouKnowWhatIMean;
	return &thisIsASimulatorInstanceIfYouKnowWhatIMean;
}

void Simulator::clearAll()
{
	eventQueue.clear();
	mClock.restart();
}