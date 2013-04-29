#ifndef KILLEVENT_H
#define KILLEVENT_H

#include "SimulationEvent.h"
#include "Entity.h"
#include <memory>

class KillEvent :public SimulationEvent
{
public:
	KillEvent(std::shared_ptr<Entity>);
	virtual void activate();
private:
	std::weak_ptr<Entity> mEntity;
};

#endif