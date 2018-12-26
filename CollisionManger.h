#pragma once
#include <SFML/Graphics.hpp>

class CollisionManger
{
public:
	CollisionManger();
	~CollisionManger();
public:
	bool isCollision(sf::Sprite const& mainCharacter , sf::Sprite const& objcetc);

};

