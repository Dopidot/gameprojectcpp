#pragma once
#include <SFML/Graphics.hpp>

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();
public:
	bool isCollision(sf::Sprite const& mainCharacter , sf::Sprite const& objcetc);

};

