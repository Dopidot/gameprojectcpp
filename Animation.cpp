#include "pch.h"
#include "Animation.h"
#include "Entity.h"
#include "EntityManager.h"


Animation::Animation()
{
}


Animation::~Animation()
{
}

void Animation::changeOrientation(sf::Sprite &sprite, bool isLeftSide)
{
	if (isLeftSide)
	{
		if (sprite.getScale().x == 1)
		{
			sprite.setOrigin(sprite.getLocalBounds().width, 0);
			sprite.setScale(-1, 1);
		}
	}
	else if (sprite.getScale().x == -1)
	{
		sprite.setOrigin(0, 0);
		sprite.setScale(1, 1);
	}
}

void Animation::startAnimation(sf::Sprite &sprite, sf::Clock *clock, bool isLeftSide)
{
	sf::IntRect rectSourceSprite;

	if (sprite.getTextureRect().left == 35)
	{
		rectSourceSprite = sf::IntRect(77, 0, 28, 40);
	}
	else
	{
		rectSourceSprite = sf::IntRect(35, 0, 38, 40);
	}

	sprite.setTextureRect(rectSourceSprite);

	changeOrientation(sprite, isLeftSide);

	clock->restart();
}

void Animation::stopAnimation(sf::Sprite &sprite, sf::Clock *clock)
{
	sf::IntRect rectSourceSprite(0, 0, 31, 40);
	sprite.setTextureRect(rectSourceSprite);

	clock->restart();
}
