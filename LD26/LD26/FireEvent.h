#ifndef FIREEVENT_H
#define FIREEVENT_H

#include "SimulationEvent.h"
#include "Entity.h"
#include <memory>
#include <vector>

class FireEvent :public SimulationEvent
{
public:
	FireEvent(std::shared_ptr<Entity>);
	virtual void activate();
private:
	std::weak_ptr<Entity> mEntity;
	static std::vector<std::shared_ptr<Entity>> mFireSpread;
};

#endif