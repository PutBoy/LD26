#include "KillEvent.h"
#include "EntityManager.h"
#include <algorithm>
#include "Debugger.h"

KillEvent::KillEvent(std::shared_ptr<Entity> entity)
	:mEntity(entity)
{

}

void KillEvent::activate()
{
	if (!mEntity.expired() && mEntity.lock())
	{
		mEntity.lock()->kill();
	}
}
