#include "MainMenu.h"
#include "Canvas.h"
#include "StateManager.h"
#include "Game.h"
#include <memory>
#include <SFML\Graphics\Color.hpp>
#include <SFML\Window\Keyboard.hpp>
#include "Globals.h"
#include "What.h"

MainMenu::MainMenu(StateManager& stateManager)
	:State(stateManager)
	,mIsAlive(true)
	,mSelectionTimer(-1)
	,mMenuSelection(0)
	,mEnterTimer(0)
{
	mMenuItems.push_back(std::make_pair("Start!",
	[&]()
	{
		getStateManager().pushState(std::make_shared<Game>(getStateManager()));
	}));

	mMenuItems.push_back(std::make_pair("What?",
	[&]()
	{
		getStateManager().pushState(std::make_shared<What>(getStateManager()));
	}));

	mMenuItems.push_back(std::make_pair("Exit!",
	[&]()
	{
		mIsAlive = false;
	}));
}

void MainMenu::render(Canvas& canvas)
{
	canvas.addText("  I Want to\nSave The Boy!", 95, 50, 32, 0,sf::Color::Red);

	for (size_t i = 0; i < mMenuItems.size(); ++i)
	{
		//Some magic too center the texts!
		float offset = (mMenuItems[i].first.length() * (32 - 8.f)) / 2.f;

		if (mMenuSelection == i)
			canvas.addText("<" + mMenuItems[i].first + ">", (Globals::screenSize.x / 4) - offset, 200 + i * 32, 32, 0,sf::Color::Red);
		else
			canvas.addText(" " + mMenuItems[i].first, (Globals::screenSize.x / 4) - offset, 200 + i * 32, 32, 0,sf::Color::Red);
	}
}

void MainMenu::update()
{
	mSelectionTimer--;
	mEnterTimer--;
	if (mSelectionTimer < 0)
	{
		mSelectionTimer = -1;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			mSelectionTimer = 20;
			mMenuSelection++;
			if (mMenuSelection >= mMenuItems.size())
				mMenuSelection = 0;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			mSelectionTimer = 20;
			mMenuSelection--;
			if (mMenuSelection < 0)
				mMenuSelection = mMenuItems.size() - 1;
		}

	}
	if (mEnterTimer < 0)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
		{
			mMenuItems[mMenuSelection].second();
			mEnterTimer = 20;
		}
	}

}

bool MainMenu::isAlive()
{
	return mIsAlive;
}

