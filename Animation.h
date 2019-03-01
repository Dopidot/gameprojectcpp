#pragma once
class Animation
{
public:
	Animation();
	~Animation();
	void changeOrientation(sf::Sprite &sprite, bool isLeftSide = false);
	void startAnimation(sf::Sprite &sprite, sf::Clock *clock, bool isLeftSide = false);
	void stopAnimation(sf::Sprite &sprite, sf::Clock *clock);

};

