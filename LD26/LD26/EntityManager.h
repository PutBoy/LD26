#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <memory>
#include <vector>
#include <SFML\System\NonCopyable.hpp>
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Rect.hpp>

class Canvas;
class Entity;


class EntityManager : public sf::NonCopyable
{
public:
	static EntityManager* getInstance();

	void removeEntity(std::shared_ptr<Entity> entity);
	void addEntity(std::shared_ptr<Entity> entity);
	void update();
	void render(Canvas& canvas);

	std::vector<std::shared_ptr<Entity>> getEntitiesAt(sf::Vector2f position);
	std::vector<std::shared_ptr<Entity>> getIntersectingEntities(sf::FloatRect rect);

	void clearAll();

private:
	EntityManager();
	void internalUpdate();
	void internalCullDead();
	void internalRender(Canvas& canvas);
	void internalHandleCollision();
	void internalAddEntities();
	void internalRemoveEntities();
	void internalUpdateCollisionGrid();

	std::vector<std::shared_ptr<Entity>> mEntities;
	std::vector<std::shared_ptr<Entity>> mToAdd;
	std::vector<std::shared_ptr<Entity>> mToRemove;
	

	std::vector<std::vector<std::shared_ptr<Entity>>> mCollisionGrid;
	const sf::Vector2u mGrids;
	const size_t mGridSize;
	int collisionCheckTimer;
};

#endif