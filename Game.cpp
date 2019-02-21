#include "pch.h"
#include "StringHelpers.h"
#include "Game.h"
#include "EntityManager.h"
#include "CollisionManager.h"
#include "Player.h"

const float Game::PlayerSpeed = 100.f;
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);
CollisionManager collision;
std::default_random_engine eng;
int gameMode = 0;

JumpAction jumpMario;
int positionYBlocks[BLOCK_COUNT_Y];

int healthPoints = HEALTH_POINTS;
int godTimeInSec = 0;

float getNearestFloor(float actualPosY, int *positionYBlocks, int sizeYMario)
{
	for (int i = 0; i < BLOCK_COUNT_Y; i++)
	{
		if (positionYBlocks[i] - sizeYMario >= actualPosY)
		{
			return positionYBlocks[i] - sizeYMario;
		}
	}

	return actualPosY;
}

Game::Game()
	: mWindow(sf::VideoMode(840, 600), "Donkey Kong 1981", sf::Style::Close)
	, mTexture()
	, mPlayer()
	, mFont()
	, mStatisticsText()
	, mStatisticsUpdateTime()
	, mStatisticsNumFrames(0)
	, mIsMovingUp(false)
	, mIsMovingDown(false)
	, mIsMovingRight(false)
	, mIsMovingLeft(false)
	,jump(false)
{
	mWindow.setFramerateLimit(160);
	eng.seed(time(0));
	
	if (gameMode == 0) // menu
	{

	}
	else if (gameMode == 1) // jeu
	{

	}
	else if (gameMode == 2) // ecran fin
	{

	}
	// Draw blocks

	_TextureBlock.loadFromFile("Media/Textures/Block.png");
	_sizeBlock = _TextureBlock.getSize();

	for (int i = 0; i < BLOCK_COUNT_X; i++)
	{
		for (int j = 0; j < BLOCK_COUNT_Y; j++)
		{
			_Block[i][j].setTexture(_TextureBlock);
			_Block[i][j].setPosition(100.f + 70.f * (i + 1), 0.f + BLOCK_SPACE * (j + 1));

			positionYBlocks[j] = _Block[i][j].getPosition().y;

			std::shared_ptr<Entity> se = std::make_shared<Entity>();
			se->m_sprite = _Block[i][j];
			se->m_type = EntityType::block;
			se->m_size = _TextureBlock.getSize();
			se->m_position = _Block[i][j].getPosition();
			EntityManager::m_Entities.push_back(se);
		}
	}

	// Draw Echelles

	_TextureEchelle.loadFromFile("Media/Textures/Echelle.png");

	int minX;
	int maxX = 700;
	int randX = 0;

	for (int i = 0; i < ECHELLE_COUNT; i++)
	{
		int oldPosX = randX;

		if (i == 3)
		{
			minX = 270; // for the first floor, move away the ladder from mario
		}
		else
		{
			minX = 170;
		}

		randX = eng() % (maxX - minX) + minX;

		if (std::abs(randX - oldPosX) < 100) // if two ladders are close
		{
			randX = randX + std::abs(randX - oldPosX) + 100;
			randX = randX >= maxX ? minX : randX;
		}

		_Echelle[i].setTexture(_TextureEchelle);
		_Echelle[i].setPosition(randX, 0.f + BLOCK_SPACE * (i + 1) + _sizeBlock.y);

		std::shared_ptr<Entity> se = std::make_shared<Entity>();
		se->m_sprite = _Echelle[i];
		se->m_type = EntityType::echelle;
		se->m_size = _TextureEchelle.getSize();
		se->m_position = _Echelle[i].getPosition();
		EntityManager::m_Entities.push_back(se);
	}

	// Draw Ennemis

	_TextureEnnemi.loadFromFile("Media/Textures/Goomba_small.png");
	_sizeEnnemi = _TextureEnnemi.getSize();
	for (int i = 0; i < ENNEMI_COUNT; i++)
	{
		_Ennemi[i].setTexture(_TextureEnnemi);

		int initX = 100.f + 70.f;

		int initY = BLOCK_SPACE * (i / 2 + 2) - _sizeEnnemi.y;
		if (i % 2) {
			initX += 530.f;
		}

		_Ennemi[i].setPosition(initX, initY);

		std::shared_ptr<Entity> enn = std::make_shared<Entity>();
		enn->m_sprite = _Ennemi[i];
		enn->m_type = EntityType::ennemi;
		enn->m_size = _TextureEnnemi.getSize();
		enn->m_position = _Ennemi[i].getPosition();
		EntityManager::m_Entities.push_back(enn);
	}

	// Draw Flag

	_TextureFlag.loadFromFile("Media/Textures/drapeau_small.png");
	_sizeFlag = _TextureFlag.getSize();
	_Flag.setTexture(_TextureFlag);

	sf::Vector2f posFlag;
	posFlag.x = eng() % (700 - 170) + 170;
	posFlag.y = BLOCK_SPACE - _sizeFlag.y;
	_Flag.setPosition(posFlag);

	std::shared_ptr<Entity> flag = std::make_shared<Entity>();
	flag->m_sprite = _Flag;
	flag->m_type = EntityType::drapeau;
	flag->m_size = _TextureFlag.getSize();
	flag->m_position = _Flag.getPosition();

	EntityManager::m_Entities.push_back(flag);


	// Draw Mario


	//sf::IntRect rectSourceSprite(10, 10, 50, 50);
	//sf::Sprite sprite(texture, rectSourceSprite);

	mTexture.loadFromFile("Media/Textures/Mario_small_transparent.png"); // Mario_small.png");
	//mTexture.loadFromFile("Media/Textures/Mario_animation.png"); // Mario_small.png");
	//sf::IntRect rectSourceSpriteTemp(0, 0, 160, 140);
	//sf::Sprite sprite(mTexture, rectSourceSpriteTemp);
	//mPlayer = sprite;
	//rectSourceSprite = rectSourceSpriteTemp;
																 //_sizeMario = mTexture.getSize();
	_sizeMario = mTexture.getSize();
	mPlayer.setTexture(mTexture);
	sf::Vector2f posMario;
	posMario.x = 100.f + 70.f;
	posMario.y = BLOCK_SPACE * 5 - _sizeMario.y;

	mPlayer.setPosition(posMario);

	std::shared_ptr<Entity> player = std::make_shared<Entity>();
	player->m_sprite = mPlayer;
	player->m_type = EntityType::player;
	player->m_size = mTexture.getSize();
	player->m_position = mPlayer.getPosition();

	//std::cout << player->m_position.y << std::endl;
	EntityManager::m_Entities.push_back(player);

	// Draw Statistic Font 

	mFont.loadFromFile("Media/Sansation.ttf");
	mStatisticsText.setString("Welcome to Donkey Kong 1981");
	mStatisticsText.setFont(mFont);
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10);
}


void Game::run()
{
	//sf::Clock clockTest;
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (mWindow.isOpen())
	{
		/*if (clockTest.getElapsedTime().asSeconds() > 1.0f)
		{
			if (rectSourceSprite.left == 300)
			{
				rectSourceSprite.left = 0;
			}
			else
			{
				rectSourceSprite.left += 150;
			}

			mPlayer.setTextureRect(rectSourceSprite);
			clockTest.restart();
		}*/


		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processEvents();
			update(TimePerFrame);
		}

		updateStatistics(elapsedTime);
		render();
	}
}

void Game::processEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::KeyPressed:
			handlePlayerInput(event.key.code, true);
			break;

		case sf::Event::KeyReleased:
			handlePlayerInput(event.key.code, false);
			break;

		case sf::Event::Closed:
			mWindow.close();
			break;
		}
	}
}

void Game::update(sf::Time elapsedTime)
{
	std::shared_ptr<Entity> player;
	EntityManager em;
	player = em.GetPlayer();

	int delta_x = abs(player->m_sprite.getPosition().x - _Echelle[3].getPosition().x);
	int delta_y = abs(player->m_sprite.getPosition().y - _Echelle[3].getPosition().y);
	int pos_player_x = player->m_sprite.getPosition().x;
	int pos_player_y = player->m_sprite.getPosition().y;

	//std::cout << delta_y << std::endl;
	
	sf::Vector2f movement(0.f, 0.f);
	sf::Vector2f movementEnnemi[ENNEMI_COUNT];
	
	// manual imput
	if (mIsMovingUp) {
		for (int i = 0; i < ECHELLE_COUNT; i++) {
			if (collision.isCollision(player->m_sprite, _Echelle[i], _sizeBlock.y)) {
				movement.y -= PlayerSpeed;
			}
		}
	}
	if (mIsMovingDown) {
		for (int i = 0; i < ECHELLE_COUNT; i++) {
			if (collision.isCollision(player->m_sprite, _Echelle[i], _sizeBlock.y)) {
				movement.y += PlayerSpeed;
			}
		}
	}
	if (mIsMovingLeft) 
	{
		if (pos_player_x > 170)
		{
			movement.x -= PlayerSpeed;
		}
	}
	if (mIsMovingRight) 
	{
		if (pos_player_x < 685)
		{
			movement.x += PlayerSpeed;
		}
	}

	if (jump) 
	{
		if (jumpMario.state == JumpState::notActivated)
		{
			jumpMario.state = JumpState::toTheTop;
			jumpMario.initialPosY = getNearestFloor(pos_player_y, positionYBlocks, _sizeMario.y);
		}
	}


	int ennemiIndex = 0;

	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_enabled == false)
		{
			continue;
		}

		if (entity->m_type != EntityType::player && entity->m_type != EntityType::ennemi)
		{
			continue;
		}
		if (entity->m_type == EntityType::player) 
		{
			if (jumpMario.state == JumpState::notActivated)
			{
				entity->m_sprite.move(movement * elapsedTime.asSeconds());
			}
			else
			{
				if (jumpMario.state == JumpState::toTheTop)
				{
					if (entity->m_sprite.getPosition().y > (jumpMario.initialPosY - JUMP_HEIGHT))
					{
						movement.y -= PlayerSpeed;
						entity->m_sprite.move(movement * elapsedTime.asSeconds());
					}
					else
					{
						jumpMario.state = JumpState::toTheBottom;
					}
				}
				else
				{
					if (entity->m_sprite.getPosition().y < jumpMario.initialPosY)
					{
						movement.y += PlayerSpeed;
						entity->m_sprite.move(movement * elapsedTime.asSeconds());
					}
					else
					{
						jumpMario.state = JumpState::notActivated;
					}
				}
				
			}
			
		}

		if (entity->m_type == EntityType::ennemi) 
		{
			if (godTimeInSec == 0)
			{
				if (collision.isCollision(player->m_sprite, entity->m_sprite))
				{
					healthPoints--;
					godTimeInSec = GOD_TIME_IN_SEC;

					std::cout << "Hit ! Health remaining : " << healthPoints << std::endl;

					if (healthPoints == 0)
					{
						std::cout << "Game Over !" << std::endl;
						healthPoints = HEALTH_POINTS;
					}
				}
			}


			if (entity->m_sprite.getPosition().x >= 660.f + _sizeBlock.x - _sizeEnnemi.x)
				dirEnnemi[ennemiIndex] = true;
			if (entity->m_sprite.getPosition().x <= 170.f)
				dirEnnemi[ennemiIndex] = false;


			if (dirEnnemi[ennemiIndex]) { // go to left
				movementEnnemi[ennemiIndex].x -= 90;
			}
			else { // go to right
				movementEnnemi[ennemiIndex].x += 90;
			}

			entity->m_sprite.move(movementEnnemi[ennemiIndex] * elapsedTime.asSeconds());
			ennemiIndex++;
		}
		
	}
}


void Game::render()
{
	mWindow.clear();

	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_enabled == false)
		{
			continue;
		}

		mWindow.draw(entity->m_sprite);
	}

	mWindow.draw(mStatisticsText);
	mWindow.display();
}

void Game::updateStatistics(sf::Time elapsedTime)
{
	mStatisticsUpdateTime += elapsedTime;
	mStatisticsNumFrames += 1;

	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		mStatisticsText.setString(
			"Frames / Second = " + toString(mStatisticsNumFrames) + "\n" +
			"Time / Update = " + toString(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us");

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;

		if (godTimeInSec > 0)
		{
			godTimeInSec--;
		}
	}

	//
	// Handle collision
	//


	if (mStatisticsUpdateTime >= sf::seconds(0.050f))
	{
		// Handle collision weapon enemies
	}
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
	if (key == sf::Keyboard::Up) {
		mIsMovingUp = isPressed;
	}
	else if (key == sf::Keyboard::Down) {
		mIsMovingDown = isPressed;
	}
	else if (key == sf::Keyboard::Left) {
		mIsMovingLeft = isPressed;
	}
	else if (key == sf::Keyboard::Right) {
		mIsMovingRight = isPressed;
	}
	if (key == sf::Keyboard::Space)
	{
		jump = isPressed;
	}
}


