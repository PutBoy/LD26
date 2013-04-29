#ifndef STILLIMAGE_H
#define STILLIMAGE_H

#include "Entity.h"
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Color.hpp>

class StillImage : public Entity
{
public:
	StillImage(float x, float y, size_t row, size_t col, int layer, sf::Color color = sf::Color::White, float scale = 1.f);
	
	virtual void update();
	virtual void render(Canvas& canvas);
	virtual sf::FloatRect getBounds();
	virtual void handleCollision(std::shared_ptr<Entity>);
	virtual void setAblaze();
	virtual bool isAblaze();
	virtual sf::Vector2f getPosition();

	virtual bool needsCollision() {return false;};

private:
	sf::Color mColor;
	float mScale;
	int mLayer;
	sf::Vector2u mSheetPos;
	sf::Vector2f mPosition;
};

#endif