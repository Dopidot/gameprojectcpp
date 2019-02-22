#pragma once

#define ECHELLE_COUNT 4
#define ENNEMI_COUNT 6
#define BLOCK_COUNT_X 8
#define BLOCK_COUNT_Y 5
#define BLOCK_SPACE 110.f
#define JUMP_HEIGHT 50
#define GOD_TIME_IN_SEC 3
#define HEALTH_POINTS 5

class Game
{
public:
	Game();
	~Game() { };
	void run();

private:
	void processEvents();
	void update(sf::Time elapsedTime);
	void render();
	//void col(std::shared_ptr<Entity> entity);

	void updateStatistics(sf::Time elapsedTime);
	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);

private:
	static const float		PlayerSpeed;
	static const sf::Time	TimePerFrame;

	sf::RenderWindow		mWindow;
	sf::Texture	mTexture;
	sf::Sprite	mPlayer;
	sf::Font	mFont;
	sf::Text	mStatisticsText;
	sf::Time	mStatisticsUpdateTime;

	std::size_t	mStatisticsNumFrames;
	bool mIsMovingUp;
	bool mIsMovingDown;
	bool mIsMovingRight;
	bool mIsMovingLeft;
	bool jump;
	bool dirEnnemi[ENNEMI_COUNT];

	sf::Texture	_TextureEchelle;
	sf::Sprite	_Echelle[ECHELLE_COUNT];
	sf::Texture	_TextureBlock;
	sf::Sprite	_Ennemi[ENNEMI_COUNT];
	sf::Texture	_TextureEnnemi;
	sf::Sprite	_Block[BLOCK_COUNT_X][BLOCK_COUNT_Y];
	sf::Texture	_TextureWeapon;
	sf::Sprite	_Weapon;
	sf::Texture	_TextureFlag;
	sf::Sprite	_Flag;
	sf::Texture	_TextureHeart;
	sf::Sprite	_Heart[HEALTH_POINTS];

	sf::Vector2u _sizeBlock;
	sf::Vector2u _sizeEnnemi;
	sf::Vector2u _sizeMario;
	sf::Vector2u _sizeFlag;
	sf::Vector2u _sizeHeart;
};

