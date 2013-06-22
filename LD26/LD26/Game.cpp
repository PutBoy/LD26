#include "Game.h"
#include "Girl.h"
#include <cmath>
#include "Tree.h"
#include "Simulator.h"
#include "FireEvent.h"
#include "KillEvent.h"
#include "Utility.h"
#include <time.h>
#include "Globals.h"
#include "FireFighter.h"
#include "Cottage.h"
#include "Goal.h"

#include "Guy.h"
#include "FireHose.h"
#include "FireFighterEvent.h"


Game::Game(StateManager& stateManager)
	:State(stateManager)
	,entities(EntityManager::getInstance())
	,mCellSize((Globals::screenSize.x / 2) / 8 - 1, (Globals::screenSize.y / 2) / 8 - 1)
	,mIsAlive(true)
{
	mOccupiedCells.resize(mCellSize.x * mCellSize.y);
	std::fill(mOccupiedCells.begin(), mOccupiedCells.end(), false);


	srand (time(NULL));

	placeStartingPad(2,2);

	//snap so that we fit in the maze!
	int cellX = ((mCellSize.x / 4) * 4 - 2);
	int cellY = ((mCellSize.y / 4) * 4 - 2);

	mGuy = std::make_shared<Guy>(cellX * 8 + 8, cellY * 8 + 8);
	entities->addEntity(mGuy);

	for (int i = cellX; i < cellX + 2; ++i)
		for (int j = cellY; j < cellY + 2; ++j)
			occupy(i, j);

	generateMaze();
	spawnForest();

}

Game::~Game()
{
	entities->clearAll();
	Simulator::getInstance()->clearAll();
}

void Game::render(Canvas& canvas)
{
	entities->render(canvas);
}

void Game::update()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		mIsAlive = false;

	entities->update();
}

bool Game::isAlive()
{
	if (mGoal)
		return !mGoal->isWon() && mIsAlive && !mGirl->isDead() && !mGuy->isDead();
	else
		//assume if there is no goal we are debugging or something
		return mIsAlive;
}

void Game::spawnForest()
{
	std::shared_ptr<Tree> tree;

	EntityManager* manager = EntityManager::getInstance();
	Simulator* sim = Simulator::getInstance();

	for (int i = 0; i < 80; ++i)
	{

		tree = placeTree(rand() % mCellSize.x, rand() % mCellSize.y);

		if (i % 10 == 0 && tree)
		{
			float time = sim->getNow() + Util::getRandom(5.0f, 60.0f);
			if (rand() % 2 == 0)
				sim->addEvent(time + Util::getRandom(5.0f, 15.0f), std::make_shared<FireFighterEvent>(tree));
			
			sim->addEvent(time, std::make_shared<FireEvent>(tree));
		}
	}

}

void Game::generateMaze()
{
	/*
	Make the initial cell the current cell and mark it as visited
		While there are unvisited cells
			If the current cell has any neighbours which have not been visited
				Choose randomly one of the unvisited neighbours
				Push the current cell to the stack
				Remove the wall between the current cell and the chosen cell
				Make the chosen cell the current cell and mark it as visited
			Else if stack is not empty
				Pop a cell from the stack
				Make it the current cell
			Else
				Pick a random cell, make it the current cell and mark it as visit
	*/

	
	size_t w = mCellSize.x / 4;// - (mCellSize.x % 4) - 1;
	size_t h = mCellSize.y / 4;// - (mCellSize.y % 4) - 1;

	std::vector<sf::Vector2i> cellStack;
	std::vector<bool> visited(w * h);
	std::fill(visited.begin(), visited.end(), false);
	std::vector<bool> walls((w * 2) * (h * 2));
	std::fill(walls.begin(), walls.end(), true);

	// Make the initial cell the current cell and mark it as visited
	sf::Vector2i currentCell(0, 0);
	visited[0] = true;

	// While there are unvisited cells
	while (std::find(visited.begin(), visited.end(), false) != visited.end())
	{
		//Determine unvisited neighbourse
		std::vector<sf::Vector2i> neighbours;
		neighbours.emplace_back(currentCell.x + 1, currentCell.y);
		neighbours.emplace_back(currentCell.x - 1, currentCell.y);
		neighbours.emplace_back(currentCell.x, currentCell.y - 1);
		neighbours.emplace_back(currentCell.x, currentCell.y + 1);

		//remove all out-of-bounds neighbours and all visited neighbours
		neighbours.erase(std::remove_if(neighbours.begin(), neighbours.end(), [h, w, &visited](sf::Vector2i& cell)
			{
				return (cell.x < 0 || cell.x >= w || cell.y < 0 || cell.y >= h
					|| visited[cell.x + cell.y * w] == true);
			}), neighbours.end());
		//If the current cell has any neighbours which have not been visited
		if (!neighbours.empty())
		{
			//Choose randomly one of the unvisited neighbours
			size_t index;
			if (neighbours.size() == 1)
				index = 0;
			else
				index = rand() % neighbours.size();

			//Push the current cell to the stack
			cellStack.push_back(neighbours[index]);

			//Remove the wall between the current cell and the chosen cell
			//determine direction
			sf::Vector2i direction = neighbours[index] - currentCell;
			sf::Vector2i currentCellCell = currentCell * 2 + sf::Vector2i(1, 1);
			sf::Vector2i wallCell = currentCellCell + direction;

			//determine currentCell's index in the walls-vector, add direction
			size_t wallIndexCurrent = (currentCellCell.x) + (currentCellCell.y) * w * 2;

			size_t wallIndexWall =  (wallCell.x) + (wallCell.y) * w * 2;;
			//set that wall to false
			walls[wallIndexCurrent] = false;
			walls[wallIndexWall] = false;
			//BUT HOW!=!=!??½?

			//Make the chosen cell the current cell and mark it as visited
			currentCell = neighbours[index];
			visited[currentCell.x + currentCell.y * w] = true;
		}
		//Else if stack is not empty
		else if (!cellStack.empty())
		{
			//Pop a cell from the stack
			//Make it the current cell
			currentCell = cellStack.back();
			cellStack.pop_back();
		}
		//Else
		else
		{
			//Pick a random cell, make it the current cell and mark it as visit
			std::vector<sf::Vector2i> unvisitedCells;

			for (size_t i = 0; i < visited.size(); ++i)
			{
				unvisitedCells.emplace_back(i % w, i / w);
			}

			size_t randomIndex = rand() % unvisitedCells.size();;

			currentCell = unvisitedCells[randomIndex];
			visited[currentCell.x + currentCell.y * w] = true;
		}
			
	}

	//create trees for the maze
	for (size_t i = 0; i < walls.size(); ++i)
	{
		if (walls[i] == true)
		{
			placeTree(i % (w*2) * 2, i / (w*2) * 2);
		}
	}

	//I don't even know why I can't get the bottom-most and right-most border
	//to fill without this, sry bro!
	//AND ITS A FUKCING MESS!
	for (size_t i = 0; i < w * 4 + 2; i +=2)
	{
		placeTree(i, h * 4);
	}

	for (size_t i = 0; i < h * 4 + 2; i += 2)
	{
		placeTree(w * 4, i);
	}
}

std::shared_ptr<Tree> Game::placeTree(int cellX, int cellY)
{
	if (!isOccupied(cellX, cellY))
	{
		occupy(cellX, cellY);
		EntityManager* manager = EntityManager::getInstance();

		std::shared_ptr<Tree>(tree) = std::make_shared<Tree>(cellX * 8 + 8, cellY * 8 + 8);
		entities->addEntity(tree);
		return tree;
	}
	else
		return nullptr;

}

std::shared_ptr<Cottage> Game::placeHouse(int cellX, int cellY)
{
	bool occupied = false;

	for (int i = cellX; i < cellX + 6; ++i)
		for (int j = cellY; j < cellY + 4; ++j)
			if (isOccupied(i, j))
				occupied = true;

	if (!occupied)
	{
		for (int i = cellX; i < cellX + 6; ++i)
			for (int j = cellY; j < cellY + 4; ++j)
				occupy(i, j);

		std::shared_ptr<Cottage> cottage = std::make_shared<Cottage>(cellX * 8 + 8, cellY * 8 + 8, 0);
		entities->addEntity(cottage);
		return cottage;
	}
	else
		return nullptr;
}

bool Game::isOccupied(int cellX, int cellY)
{
	if (cellX < 0 || cellX >= mCellSize.x || cellY < 0 || cellY >= mCellSize.y)
		return true;
	else
		return mOccupiedCells[cellX + cellY * mCellSize.x];
}

void Game::occupy(int cellX, int cellY)
{
	if (cellX < 0 || cellX >= mCellSize.x || cellY < 0 || cellY >= mCellSize.y)
		return;
	else
		mOccupiedCells[cellX + cellY * mCellSize.x] = true;
}

void Game::placeStartingPad(int cellX, int cellY)
{
	placeHouse(2,2);

	mGoal = std::make_shared<Goal>(cellX + 6 * 8 + 8, cellY + 6 * 8 + 8);
	entities->addEntity(mGoal);

	mGirl = std::make_shared<Girl>(cellX * 8 + 8, cellY + 6 * 8 + 8);
	entities->addEntity(mGirl);

	auto axe = std::make_shared<Axe>(0, 0);
	mGirl->pickUpItem(axe);

	auto hose = std::make_shared<FireHose>(0, 0);
	mGirl->pickUpItem(hose);

	for (int i = cellX - 1; i < cellX + 8; ++i)
		for (int j = cellY - 1; j < cellY + 6; ++j)
			occupy(i, j);

}