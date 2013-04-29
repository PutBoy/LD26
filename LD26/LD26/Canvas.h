#ifndef CANVAS_H
#define CANVAS_H

#include <string>
#include <SFML\Graphics\Texture.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include <vector>

class Canvas
{
public:
	Canvas(const std::string& file);

	void addSprite(float x, float y, int layer, size_t sheetRow, size_t sheetCol, sf::Color color = sf::Color::White, float scale = 1.0f);

	void render(sf::RenderTarget&);
	void addText(const std::string& text, float x, float y, size_t fontSize, int layer, sf::Color color = sf::Color::Black);

private:
	sf::Texture mSpriteSheet;

	std::vector<std::vector<sf::Sprite>> mCanvasLayers;
	
};

#endif
