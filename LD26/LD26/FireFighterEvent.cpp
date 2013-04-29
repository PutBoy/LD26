#include "FireFighterEvent.h"
#include "EntityManager.h"
#include "FireFighter.h"
#include "Utility.h"
#include "Debugger.h"

FireFighterEvent::FireFighterEvent(std::shared_ptr<Entity> entity)
	:mEntity(entity)
{
}
void FireFighterEvent::activate()
{
	EntityManager* manager = EntityManager::getInstance();
	sf::Vector2f position = sf::Vector2f(Util::getRandom(0.f, 600.f), Util::getRandom(0.f, 400.f));
	sf::Vector2f major = sf::Vector2f(Util::getRandom(0.f, 1.f), Util::getRandom(0.f, 1.f));
	major = Util::getMajorVector(major);
	position.x *= major.x;
	position.y *= major.y;

	manager->addEntity(std::make_shared<FireFighter>(position.x, position.y, mEntity.lock()));
}
