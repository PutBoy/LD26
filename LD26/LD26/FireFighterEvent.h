#ifndef FIREFIGHTEREVENT_H
#define FIREFIGHTEREVENT_H

#include "SimulationEvent.h"
#include "Entity.h"
#include <memory>

class FireFighterEvent :public SimulationEvent
{
public:
	FireFighterEvent(std::shared_ptr<Entity>);
	virtual void activate();
private:
	std::weak_ptr<Entity> mEntity;
};

#endif