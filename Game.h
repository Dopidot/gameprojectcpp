#pragma once

#define LADDER_COUNT 4
#define ENNEMI_COUNT 6
#define BLOCK_COUNT_X 8
#define BLOCK_COUNT_Y 5
#define BLOCK_SPACE 110.f
#define JUMP_HEIGHT 50
#define GOD_TIME_IN_SEC 3
#define HEALTH_POINTS 5
#define PIECE_COUNT 7

enum GameMode
{
	initializeMenu,
	menu,
	initializeGame,
	playing,
	ending
};

enum ActionMenu
{
	newGame,
	exitGame
};

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
	sf::Font	mFontCustom;
	sf::Text	mStatisticsText;
	sf::Time	mStatisticsUpdateTime;
	sf::Text	mTitle;
	sf::Text	mInfo;
	sf::Text	mScore;

	std::size_t	mStatisticsNumFrames;
	bool mIsMovingUp;
	bool mIsMovingDown;
	bool mIsMovingRight;
	bool mIsMovingLeft;
	bool mIsSpaceBar;
	bool dirEnnemi[ENNEMI_COUNT];

	sf::Texture	_TextureLadder;
	sf::Sprite	_Ladder[LADDER_COUNT];
	sf::Texture	_TextureBlock;
	sf::Sprite	_Ennemi[ENNEMI_COUNT];
	sf::Texture	_TextureEnnemi;
	sf::Sprite	_Block[BLOCK_COUNT_X][BLOCK_COUNT_Y];
	sf::Texture	_TextureWeapon;
	sf::Sprite	_Weapon;
	sf::Texture	_TexturePeach;
	sf::Sprite	_Peach;
	sf::Texture	_TextureHeart;
	sf::Sprite	_Heart[HEALTH_POINTS];
	sf::Texture	_TextureLogo;
	sf::Sprite	_Logo;
	sf::Texture	_TextureButtonNew;
	sf::Sprite	_ButtonNew;
	sf::Texture	_TextureButtonExit;
	sf::Sprite	_ButtonExit;
	sf::Texture	_TextureCursor;
	sf::Sprite	_Cursor;
	sf::Texture	_TextureVictory;
	sf::Sprite	_Victory;
	sf::Texture	_TextureGameOver;
	sf::Sprite	_GameOver;
	sf::Sprite	_Love;
	sf::Texture	_TexturePiece;
	sf::Sprite	_Piece[PIECE_COUNT];

	sf::Vector2u _sizeBlock;
	sf::Vector2u _sizeEnnemi;
	sf::Vector2u _sizeMario;
	sf::Vector2u _sizePeach;
	sf::Vector2u _sizeHeart;
	sf::Vector2u _sizeLogo;
	sf::Vector2u _sizeButtonNew;
	sf::Vector2u _sizeButtonExit;
	sf::Vector2u _sizeCursor;
	sf::Vector2u _sizeVictory;
	sf::Vector2u _sizeGameOver;
	sf::Vector2u _sizePiece;
};

