#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <SFML\Graphics\Rect.hpp>

#include <SFML\System\Vector2.hpp>
#include <cmath>
#include <limits>
#include <sstream>

namespace Util
{
	template <typename T, typename stringT>
	T stringTo(const stringT& str)
	{
		std::istringstream ss(str);
		T ret;

		return (ss >> ret) ? ret : throw "fromString: invalid string";
	};

	template <typename T>
	std::string toString(const T& value)
	{
		std::ostringstream ss("");
		
		return (ss << value) ? ss.str() : throw "toString: invalid value";
	};

	template<typename floatType>
	bool compare(floatType f0, floatType f1, floatType epsilon = std::numeric_limits<floatType>::epsilon())
	{
		return abs(f0 - f1) < epsilon;
	};

	template<typename T>
	T length(const sf::Vector2<T>& vec)
	{
		return static_cast<T>(std::sqrt(vec.x * vec.x + vec.y * vec.y));
	};

	template<typename T>
	sf::Vector2<T> normalize(sf::Vector2<T>& vec)
	{
		T len = length(vec);
		if (compare(len, T()))
			return vec;
		
		vec *= 1 / len;
		return vec;
	};
	
	template<typename T>
	sf::Vector2<T> getNormalized(const sf::Vector2<T>& vec)
	{
		T len = length(vec);
		if (compare(len, T()))
			return vec;
		
		return vec * (1 / len);
	};

	template<typename T>
	sf::Vector2<T> getMajorVector(const sf::Vector2<T>& vec)
	{
		if (std::abs(vec.x) > std::abs(vec.y))
		{
			return sf::Vector2<T>(vec.x, T());
		}
		else
		{
			return sf::Vector2<T>(T(), vec.y);
		}
	};

	template<typename T>
	sf::Vector2<T> getRectDiffRatio(sf::Rect<T> r0, sf::Rect<T> r1)
	{
		//calculate the two centers
		sf::Vector2<T> c0(r0.left + r0.width / 2, r0.top + r0.height / 2);
		sf::Vector2<T> c1(r1.left + r1.width / 2, r1.top + r1.height / 2);
		//calculate the diffence between the two centers.
		sf::Vector2<T> diff = c1 - c0;

		//scale the distance to take account for the width and height of the objects. THIS IS NOT PERFECT (Not even valid lol).
		diff.x = diff.x / (r0.width / 2 + r1.width / 2);
		diff.y = diff.y / (r0.height / 2 + r1.height / 2);
	
		return diff;
	}

	template <typename T>
	T getRandom(T min, T max)
	{
		return min + static_cast<T>(rand()) / ( static_cast<T>(RAND_MAX / (max-min)));
	};

	template <typename valueType, typename gridType>
	valueType snapToGrid(valueType value, gridType gridSize)
	{
		return (value / gridSize * gridSize) + (gridSize / 2);
	}
};

#endif