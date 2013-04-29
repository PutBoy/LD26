#ifndef GAME_H
#define GAME_H

#include "State.h"
#include "EntityManager.h"

class Cottage;
class Simulator;
class Tree;
class Goal;
class Girl;
class Guy;

class Game: public State
{
public:
	Game(StateManager& stateManager);
	~Game();

	virtual void render(Canvas&);
	virtual void update();
	virtual bool isAlive();
private:
	void spawnForest();
	void generateMaze();
	void placeStartingPad(int cellX, int cellY);

	std::shared_ptr<Tree> placeTree(int cellX, int cellY);
	std::shared_ptr<Cottage> placeHouse(int cellX, int cellY);

	bool isOccupied(int cellX, int cellY);
	void occupy(int cellX, int cellY);

	std::vector<bool> mOccupiedCells;
	sf::Vector2u mCellSize;

	EntityManager* entities;

	std::shared_ptr<Goal> mGoal;
	bool mIsAlive;

	std::shared_ptr<Girl> mGirl;
	std::shared_ptr<Guy> mGuy;

};

#endif