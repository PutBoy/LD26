#include "What.h"
#include "Canvas.h"
#include "Utility.h"
#include <SFML\Window\Keyboard.hpp>

What::What(StateManager& stateManager)
	:State(stateManager)
	,mCurrentScreen(0)
	,mDelayTimer(50)
	,mIsAlive(true)
{
	mHelpScreens.push_back(std::bind(&What::screen0, this, std::placeholders::_1));
	mHelpScreens.push_back(std::bind(&What::screen1, this, std::placeholders::_1));
	mHelpScreens.push_back(std::bind(&What::screen2, this, std::placeholders::_1));
}

void What::render(Canvas& canvas)
{
	if (mCurrentScreen < mHelpScreens.size())
		mHelpScreens[mCurrentScreen](canvas);

	canvas.addText("What?", 200, 50, 32, 0, sf::Color::Red);

	canvas.addText("Press Enter to Continue <" + Util::toString(mCurrentScreen + 1) + " of " + Util::toString(mHelpScreens.size()) + ">", 
		250, 450, 8, 0, sf::Color::Red);

}

void What::update()
{
	mDelayTimer--;
	if (mDelayTimer < 0)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
		{
			mCurrentScreen++;
			mDelayTimer = 50;
		}
	}
	
	if (mCurrentScreen >= mHelpScreens.size())
		mIsAlive = false;
}

bool What::isAlive()
{
	return mIsAlive;
}

void What::screen0(Canvas& canvas)
{
	std::string text = "You are the girl!\n\n"
		"You were spending time at your friend's \n"
		"cottage while a fire broke out!\n\n"
		"Unfortunately your friend got lost in\n"
		"the woods. Can you find him and bring \n"
		"him home?";

	canvas.addText(text, 50, 100, 12, 0, sf::Color::Black);
}

void What::screen1(Canvas& canvas)
{
	std::string text = "W A S D to move.\n\n"
		"Use items with space.\n"
		"Cycle through items with E.\n"
		"Drop items with Q.\n\n"
		"Avoid fire! Firefighters are not\n"
		"too bright. Don't rely on them!";

	canvas.addText(text, 50, 100, 12, 0, sf::Color::Black);
}

void What::screen2(Canvas& canvas)
{
	std::string text = "Developed by Sebastian Zander";

	canvas.addText(text, 50, 100, 16, 0, sf::Color::Red);

	text = "This game was developed during 48 hours\n"
		"for the 26th Ludum Dare!\n";


	canvas.addText(text, 50, 164, 12, 0, sf::Color::Black);
}

