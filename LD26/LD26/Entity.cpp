#include "Entity.h"
#include "Debugger.h"

int Entity::mEntityCount = 0;


Entity::Entity(): mIsDead(false) 
{ 
	if (mEntityCount == 0)
		DEBUG("First Entity created!");
	mEntityCount++;
}

Entity::~Entity()
{
	mEntityCount--;
	if (mEntityCount == 0)
		DEBUG("All entities destroyed!");
}