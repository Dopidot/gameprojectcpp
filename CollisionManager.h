#pragma once
#include <SFML/Graphics.hpp>

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();
public:
	bool isCollision(sf::Sprite const& , sf::Sprite const& , int topGap = 0, int sideGap = 0);
};

