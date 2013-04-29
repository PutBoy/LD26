#include "FireFighterEvent.h"
#include "EntityManager.h"
#include "FireFighter.h"
#include "Utility.h"
#include "Debugger.h"
#include "Globals.h"

FireFighterEvent::FireFighterEvent(std::shared_ptr<Entity> entity)
	:mEntity(entity)
{
}
void FireFighterEvent::activate()
{
	EntityManager* manager = EntityManager::getInstance();
	sf::Vector2f position = sf::Vector2f(Util::getRandom(0.f, static_cast<float>(Globals::screenSize.x / 2)), 
		Util::getRandom(0.f,static_cast<float>(Globals::screenSize.y / 2)));
	sf::Vector2f major = sf::Vector2f(Util::getRandom(0.f, 1.f), Util::getRandom(0.f, 1.f));
	major = Util::getMajorVector(major);
	Util::normalize(major);

	position.x *= major.x;
	position.y *= major.y;

	if (rand() % 2 == 0)
	{
		position.x += major.y * Globals::screenSize.x / 2;
		position.y += major.x * Globals::screenSize.y / 2;
	}

	manager->addEntity(std::make_shared<FireFighter>(position.x, position.y, mEntity.lock()));
}
