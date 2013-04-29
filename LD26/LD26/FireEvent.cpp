#include "FireEvent.h"
#include "EntityManager.h"
#include <algorithm>
#include "Simulator.h"
#include "Tree.h"
#include "Utility.h"
#include "FireFighterEvent.h"
#include <cmath>
#include "Debugger.h"
std::vector<std::shared_ptr<Entity>> FireEvent::mFireSpread;

FireEvent::FireEvent(std::shared_ptr<Entity> entity)
	:mEntity(entity)
{

}

void FireEvent::activate()
{
	if (mEntity.expired())
		return;

	auto entityManager = EntityManager::getInstance();
	auto sim = Simulator::getInstance();
	auto thisEntity = mEntity.lock();

	thisEntity->setAblaze();
	//sometimes call the firedepartment
	if (Util::getRandom(0.f, 1.f) < 0.1f)
	{
		sim->addEvent(sim->getNow() + 4.f, std::make_shared<FireFighterEvent>(thisEntity));
	}

	sf::FloatRect eventBox = thisEntity->getBounds();

	sf::Vector2f entityPos(eventBox.left + eventBox.width / 2, eventBox.top + eventBox.left / 2);
	sf::Vector2f ignitePosition(entityPos + sf::Vector2f(Util::getRandom(-32.0f, 32.0f), Util::getRandom(-32.0f, 32.0f)));

	sf::FloatRect igniteRect(eventBox.left - 32 , eventBox.top - 32, eventBox.width + 64, eventBox.height + 64);

	auto entities = entityManager->getIntersectingEntities(igniteRect);
	
	//don't set fire to yourself doe
	entities.erase(std::remove(entities.begin(), entities.end(), thisEntity), entities.end());
	entities.erase(std::remove_if(entities.begin(), entities.end(), [](std::shared_ptr<Entity> entity){return entity->isAblaze();}), entities.end());

	//we keep track of which objects have been added to a fire even so we don't fire off multiple
	//events on one object.
	for (auto& fired : mFireSpread)
	{
		entities.erase(std::remove(entities.begin(), entities.end(), fired), entities.end());
	}
	
	if (!entities.empty())
	{
		//create more than one fire like once in never.
		size_t amount = 1;

		if (Util::getRandom(.0f, 1.f) > .85f)
			amount = 2;

		if (Util::getRandom(.0f, 1.f) > .98f)
			amount = 3;

		if (entities.size() < 10)
			amount = 1;

		for (int i = 0; i < amount; ++i)
		{
			size_t setFireTo = rand() % entities.size();
			auto& entity = entities[setFireTo];
			entities.erase(entities.begin() + setFireTo);

			sim->addEvent(sim->getNow() + Util::getRandom(2.0f, 10.0f), std::make_shared<FireEvent>(entity));
			mFireSpread.push_back(entity);

		}
	}

}
