#include "EntityManager.h"
#include "Entity.h"
#include <algorithm>
#include "Canvas.h"
#include "Utility.h"
#include "Globals.h"
#include "Debugger.h"

EntityManager::EntityManager()
	:mGridSize(16)
	,mGrids(Globals::screenSize.x / 2 / 16 + 1, Globals::screenSize.y / 2 / 16 + 1)
{
	mCollisionGrid.resize(mGrids.x * mGrids.y);
}

EntityManager* EntityManager::getInstance()
{
	static EntityManager thisIsAnInstanceOfSomeDiscription;
	return &thisIsAnInstanceOfSomeDiscription;
}

void EntityManager::addEntity(std::shared_ptr<Entity> entity)
{
	mToAdd.push_back(entity);
}

void EntityManager::internalAddEntities()
{
	for (size_t i = 0; i < mToAdd.size(); ++i)
	{
		mEntities.push_back(mToAdd[i]);
	}
	mToAdd.clear();
}

void EntityManager::update()
{
	internalUpdate();
	
	internalHandleCollision();
	//to increase performance we only update collisiongrids every so often
	++collisionCheckTimer;
	if (collisionCheckTimer % 4 == 0)
	{
		
		internalUpdateCollisionGrid();
	}
	internalCullDead();
	internalAddEntities();
}

void EntityManager::internalHandleCollision()
{
	int tests = 0;

	for (auto& grid : mCollisionGrid)
	{
		for (auto iter0 = grid.begin(); iter0 != grid.end(); ++iter0)
		{
			if ((*iter0)->needsCollision())
			{
				sf::FloatRect iter0bounds = (*iter0)->getBounds();
				for (auto iter1 = grid.begin(); iter1 != grid.end(); ++iter1)
				{
					if (*iter0 != *iter1)
					{
						++tests;
						if (iter0bounds.intersects((*iter1)->getBounds()))
						{
							(*iter0)->handleCollision(*iter1);
							(*iter1)->handleCollision(*iter0);
						}
					}
				}
			}
		}
	}
	DEBUG(("trying collision " + Util::toString(tests) + " times").c_str());
	tests = tests;
}

void EntityManager::render(Canvas& canvas)
{
	internalRender(canvas);
}
void EntityManager::internalUpdateCollisionGrid()
{
	for (auto& grid : mCollisionGrid)
	{
		grid.clear();
	}

	for (auto& entity : mEntities)
	{
		sf::FloatRect rect = entity->getBounds();

		if (Util::compare(rect.width, 0.f) && Util::compare(rect.height, 0.f)
			|| entity->nonCollidable())
			continue;

		sf::Vector2f p[] =
		{
			sf::Vector2f(rect.left, rect.top),
			sf::Vector2f(rect.left + rect.width, rect.top),
			sf::Vector2f(rect.left, rect.top + rect.height),
		};

		std::vector<sf::Vector2i> gridIndices;
		for (int i = 0; i < 3; ++i)
		{
			sf::Vector2i gridIndex(p[i].x / mGridSize, p[i].y / mGridSize);

			if (gridIndex.x < 0)
				gridIndex.x = 0;
			else if (gridIndex.x > mGrids.x - 1)
				gridIndex.x = mGrids.x -1;
			if (gridIndex.y < 0)
				gridIndex.y = 0;
			else if (gridIndex.y > mGrids.y - 1)
				gridIndex.y = mGrids.y -1;

			gridIndices.push_back(gridIndex);
		}

		for (int i = gridIndices[0].x; i <= gridIndices[1].x; ++i)
		{
			for (int j = gridIndices[0].y; j <= gridIndices[2].y; ++j)
			{
				sf::Vector2i gridIndex(i, j);
				auto& grid = mCollisionGrid[gridIndex.x + gridIndex.y * mGrids.x];

				if (grid.empty())
				{
					mCollisionGrid[gridIndex.x + gridIndex.y * mGrids.x].push_back(entity);
				}
				//don't do it twice lol
				else if (grid.back() != entity)
				{
					mCollisionGrid[gridIndex.x + gridIndex.y * mGrids.x].push_back(entity);
				}
			}
		}
	}
}
void EntityManager::internalUpdate()
{
	for (auto& entity : mEntities)
	{
		entity->update();
	}

	
}

void EntityManager::internalCullDead()
{
	mEntities.erase(
		std::remove_if(mEntities.begin(), mEntities.end(), [](std::shared_ptr<Entity> entity) {return entity->isDead();}),
		mEntities.end());
}

void EntityManager::internalRender(Canvas& canvas)
{
	for (auto& entity : mEntities)
	{
		entity->render(canvas);
	}
}

std::vector<std::shared_ptr<Entity>> EntityManager::getEntitiesAt(sf::Vector2f position)
{
	sf::Vector2i gridIndex(position.x / mGridSize, position.y / mGridSize);

	if (gridIndex.x < 0)
		gridIndex.x = 0;
	else if (gridIndex.x > mGrids.x - 1)
		gridIndex.x = mGrids.x -1;
	if (gridIndex.y < 0)
		gridIndex.y = 0;
	else if (gridIndex.y > mGrids.y - 1)
		gridIndex.y = mGrids.y -1;

	
	std::vector<std::shared_ptr<Entity>> ret;

	for (auto& entity : mCollisionGrid[gridIndex.x + gridIndex.y * mGrids.x])
	{
		if (entity->getBounds().contains(position))
			ret.push_back(entity); 
	}

	return ret;
}

std::vector<std::shared_ptr<Entity>> EntityManager::getIntersectingEntities(sf::FloatRect rect)
{
	
	std::vector<std::shared_ptr<Entity>> ret;

	sf::Vector2f p[] =
	{
		sf::Vector2f(rect.left, rect.top),
		sf::Vector2f(rect.left + rect.width, rect.top),
		sf::Vector2f(rect.left, rect.top + rect.height),
	};

	std::vector<sf::Vector2i> gridIndices;
	for (int i = 0; i < 3; ++i)
	{
		sf::Vector2i gridIndex(p[i].x / mGridSize, p[i].y / mGridSize);

		if (gridIndex.x < 0)
			gridIndex.x = 0;
		else if (gridIndex.x > mGrids.x - 1)
			gridIndex.x = mGrids.x -1;
		if (gridIndex.y < 0)
			gridIndex.y = 0;
		else if (gridIndex.y > mGrids.y - 1)
			gridIndex.y = mGrids.y -1;

		gridIndices.push_back(gridIndex);
	}

	for (int i = gridIndices[0].x; i <= gridIndices[1].x; ++i)
	{
		for (int j = gridIndices[0].y; j <= gridIndices[2].y; ++j)
		{
			sf::Vector2i gridIndex(i, j);
			auto& grid = mCollisionGrid[gridIndex.x + gridIndex.y * mGrids.x];

			for (auto& entity : grid)
			{
				if (entity->getBounds().intersects(rect))
					ret.push_back(entity); 
			}
		}
	}


	return ret;
}

void EntityManager::removeEntity(std::shared_ptr<Entity> entity)
{
	mToRemove.push_back(entity);
}

void EntityManager::internalRemoveEntities()
{
	for (auto &entity : mToRemove)
	{
		mEntities.erase(std::remove(mEntities.begin(), mEntities.end(), entity), mEntities.end());
	}
}

void EntityManager::clearAll()
{
	mEntities.clear();
	mToAdd.clear();
	mToRemove.clear();
	
	mCollisionGrid.clear();
	mCollisionGrid.resize(mGrids.x * mGrids.y);
}