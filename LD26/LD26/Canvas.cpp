#include "Canvas.h"
#include <SFML\Graphics\RenderTarget.hpp>

Canvas::Canvas(const std::string& file)
{
	mCanvasLayers.resize(10);

	sf::Image image;
	image.loadFromFile(file);
	image.createMaskFromColor(sf::Color(255, 0, 255));
	mSpriteSheet.loadFromImage(image);
}

void Canvas::addSprite(float x, float y, int layer, size_t sheetRow, size_t sheetCol, sf::Color color, float scale)
{
	if (layer < 0)
		layer = 0;
	if (layer >= 10)
		layer = 9;

	sf::Sprite sprite(mSpriteSheet);
	sprite.setOrigin(8, 8);
	sprite.setPosition(x * 2, y * 2);
	sprite.setScale(2.0 * scale, 2.0 * scale);
	sprite.setTextureRect(sf::IntRect(16 * sheetRow, 16 * sheetCol, 16, 16));
	sprite.setColor(color);


	mCanvasLayers[layer].push_back(sprite);
}

void Canvas::render(sf::RenderTarget& target)
{
	for(auto& layer : mCanvasLayers)
	{
		for (auto& sprite : layer)
		{
			target.draw(sprite);
			
		}

		layer.clear();
	}

}

void Canvas::addText(const std::string& text, float x, float y, size_t fontSize, int layer, sf::Color color)
{
	if (layer < 0)
		layer = 0;
	if (layer >= 10)
		layer = 9;

	float scale = fontSize / 16.f;

	std::vector<sf::Vector2i> charcoords;
	//first parse
	for(auto character : text)
	{
		char charval = static_cast<unsigned char>(character);
		if (character >= 'A' && character <= 'Z')
			charcoords.emplace_back(charval - 65, 10);
		else if (character >= 'a' && character <= 'z')
			charcoords.emplace_back(charval - 97, 10);
		else if (character >= '0' && character <= '9')
			charcoords.emplace_back(charval - 48, 11);
		else if (character == ' ')
			charcoords.emplace_back(-1, 0);
		else if (character == '#')
			charcoords.emplace_back(11, 11);
		else if (character == '!')
			charcoords.emplace_back(12, 11);
		else if (character == '<')
			charcoords.emplace_back(13, 11);
		else if (character == '>')
			charcoords.emplace_back(14, 11);
		else if (character == '?')
			charcoords.emplace_back(15, 11);
		else if (character == '.')
			charcoords.emplace_back(16, 11);
		else if (character == '\'')
			charcoords.emplace_back(17, 11);

		else if (character == '\n')
			charcoords.emplace_back(-2, 0);

		else
			charcoords.emplace_back(10, 11);
	}

	int xoffset = -4 * scale;
	int yoffset = 0;

	for (auto& coord: charcoords)
	{
		if (coord.x >= 0)
			addSprite(x + xoffset, y + yoffset, layer, coord.x, coord.y, color, scale);
		
		xoffset += fontSize - 4 * scale;

		if (coord.x == -2)
		{
			xoffset = -4 * scale;
			yoffset += fontSize;
		}

	}
}
