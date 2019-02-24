#include "pch.h"
#include "StringHelpers.h"
#include "Game.h"
#include "EntityManager.h"
#include "CollisionManager.h"
#include "Player.h"

const float Game::PlayerSpeed = 100.f;
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);
const sf::Vector2i screenResolution(840, 600);

CollisionManager collision;
std::default_random_engine eng;
int gameMode = GameMode::initializeMenu;
int cursorSelection = ActionMenu::newGame;
bool isVictory = false;

JumpAction jumpMario;
int positionYBlocks[BLOCK_COUNT_Y];

int healthPoints = HEALTH_POINTS;
int godTimeInSec = 0;
int score;

bool isClimbingLadder = false;

sf::Clock clockAnimation;
sf::Clock clockCursor;
sf::Clock clockEndGame;



float getNearestFloor(float actualPosY, int sizeYMario)
{
	for (int i = 0; i < BLOCK_COUNT_Y; i++)
	{
		if (positionYBlocks[i] - sizeYMario >= actualPosY)
		{
			return positionYBlocks[i] - sizeYMario;
		}
	}

	return positionYBlocks[BLOCK_COUNT_Y - 1] - sizeYMario;
}

void changeOrientation(std::shared_ptr<Entity> entity, bool isLeftSide = false)
{
	if (isLeftSide)
	{
		if (entity->m_sprite.getScale().x == 1)
		{
			entity->m_sprite.setOrigin(entity->m_sprite.getLocalBounds().width, 0);
			entity->m_sprite.setScale(-1, 1);
		}
	}
	else if (entity->m_sprite.getScale().x == -1)
	{
		entity->m_sprite.setOrigin(0, 0);
		entity->m_sprite.setScale(1, 1);
	}
}

void startAnimation(bool isLeftSide = false)
{
	std::shared_ptr<Entity> player;
	player = EntityManager::GetPlayer();

	sf::IntRect rectSourceSprite;

	if (player->m_sprite.getTextureRect().left == 35)
	{
		rectSourceSprite = sf::IntRect(77, 0, 28, 40);
	}
	else
	{
		rectSourceSprite = sf::IntRect(35, 0, 38, 40);
	}

	player->m_sprite.setTextureRect(rectSourceSprite);

	changeOrientation(player, isLeftSide);

	clockAnimation.restart();
}

void stopAnimation()
{
	std::shared_ptr<Entity> player;
	player = EntityManager::GetPlayer();

	sf::IntRect rectSourceSprite(0, 0, 31, 40);
	player->m_sprite.setTextureRect(rectSourceSprite);

	clockAnimation.restart();
}

void moveCursorMenu()
{
	std::shared_ptr<Entity> cursor;
	cursor = EntityManager::GetCursor();

	for (auto entity : EntityManager::m_Entities)
	{
		if (entity->m_type != EntityType::button)
		{
			continue;
		}

		if (entity->m_enabled)
		{
			float currentPosX = cursor->m_sprite.getPosition().x;
			float newPosY = entity->m_sprite.getPosition().y + (cursor->m_sprite.getTexture()->getSize().y / 3);

			if (cursor->m_sprite.getPosition().y != newPosY)
			{
				cursor->m_sprite.setPosition(currentPosX, newPosY);

				if (cursorSelection == ActionMenu::newGame)
				{
					cursorSelection = ActionMenu::exitGame;
				}
				else
				{
					cursorSelection = ActionMenu::newGame;
				}

				break;
			}
			
		}
	}
}


Game::Game()
	: mWindow(sf::VideoMode(screenResolution.x, screenResolution.y), "Donkey Kong 1981", sf::Style::Close)
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
	, mIsSpaceBar(false)
{
	mWindow.setFramerateLimit(160);
	eng.seed(time(0));
	


	// Draw Statistic Font 

	mFont.loadFromFile("Media/Sansation.ttf");
	mStatisticsText.setString("Welcome to Donkey Kong 1981");
	mStatisticsText.setFont(mFont);
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10);
}


void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (mWindow.isOpen())
	{
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
	if (gameMode == GameMode::initializeMenu)
	{
		// Draw Menu

		_TextureLogo.loadFromFile("Media/Textures/Donkey_kong.png");
		_sizeLogo = _TextureLogo.getSize();
		_Logo.setTexture(_TextureLogo);

		int menuPosX = (screenResolution.x / 2) - _sizeLogo.x / 2;
		int menuPosY = (screenResolution.y / 4) - _sizeLogo.y / 2;

		_Logo.setPosition(menuPosX, menuPosY);

		std::shared_ptr<Entity> logo = std::make_shared<Entity>();
		logo->m_sprite = _Logo;
		logo->m_type = EntityType::logo;
		logo->m_size = _TextureLogo.getSize();
		logo->m_position = _Logo.getPosition();

		EntityManager::m_Entities.push_back(logo);



		// Draw Title 

		mFontCustom.loadFromFile("Media/College.ttf");
		mTitle.setString("Welcome to the New Version of Donkey Kong 1981");
		mTitle.setFont(mFontCustom);
		mTitle.setCharacterSize(32);

		int titlePosX = (screenResolution.x / 2) - mTitle.getLocalBounds().width / 2;
		int titlePosY = (screenResolution.y / 2) - mTitle.getLocalBounds().height / 2;
		mTitle.setPosition(titlePosX, titlePosY);



		// Draw New Game

		_TextureButtonNew.loadFromFile("Media/Textures/Button_newgame_small.png");
		_sizeButtonNew = _TextureButtonNew.getSize();
		_ButtonNew.setTexture(_TextureButtonNew);

		int newGamePosX = (screenResolution.x / 2) - _sizeButtonNew.x / 2;
		int newGamePosY = titlePosY + (mTitle.getPosition().y / 3);

		_ButtonNew.setPosition(newGamePosX, newGamePosY);

		std::shared_ptr<Entity> buttonNew = std::make_shared<Entity>();
		buttonNew->m_sprite = _ButtonNew;
		buttonNew->m_type = EntityType::button;
		buttonNew->m_size = _TextureButtonNew.getSize();
		buttonNew->m_position = _ButtonNew.getPosition();

		EntityManager::m_Entities.push_back(buttonNew);



		// Draw Exit

		_TextureButtonExit.loadFromFile("Media/Textures/Button_exit_small.png");
		_sizeButtonExit = _TextureButtonExit.getSize();
		_ButtonExit.setTexture(_TextureButtonExit);

		int exitPosX = (screenResolution.x / 2) - _sizeButtonExit.x / 2;
		int exitPosY = newGamePosY + (mTitle.getPosition().y / 4);

		_ButtonExit.setPosition(exitPosX, exitPosY);

		std::shared_ptr<Entity> buttonExit = std::make_shared<Entity>();
		buttonExit->m_sprite = _ButtonExit;
		buttonExit->m_type = EntityType::button;
		buttonExit->m_size = _TextureButtonExit.getSize();
		buttonExit->m_position = _ButtonExit.getPosition();

		EntityManager::m_Entities.push_back(buttonExit);



		// Draw Cursor

		_TextureCursor.loadFromFile("Media/Textures/Cursor_small.png");
		_sizeCursor = _TextureCursor.getSize();
		_Cursor.setTexture(_TextureCursor);

		int cursorPosX = _sizeButtonNew.x + _sizeCursor.x;
		int cursorPosY = newGamePosY + (_sizeCursor.y / 3);

		_Cursor.setPosition(cursorPosX, cursorPosY);

		std::shared_ptr<Entity> cursor = std::make_shared<Entity>();
		cursor->m_sprite = _Cursor;
		cursor->m_type = EntityType::cursor;
		cursor->m_size = _TextureCursor.getSize();
		cursor->m_position = _Cursor.getPosition();

		EntityManager::m_Entities.push_back(cursor);



		// Draw info text

		mFont.loadFromFile("Media/Sansation.ttf");
		mInfo.setString("Press the space key to validate your choice.");
		mInfo.setFont(mFont);
		mInfo.setCharacterSize(16);

		int infoPosX = (screenResolution.x / 2) - mInfo.getLocalBounds().width / 2;
		int infoPosY = screenResolution.y - (mInfo.getLocalBounds().height * 2);
		mInfo.setPosition(infoPosX, infoPosY);

		gameMode = GameMode::menu;
	}
	else if (gameMode == GameMode::initializeGame)
	{
		healthPoints = HEALTH_POINTS;

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

		// Draw Ladders

		_TextureLadder.loadFromFile("Media/Textures/Ladder_small.png");

		int minX;
		int maxX = 700;
		int randX = 0;

		for (int i = 0; i < LADDER_COUNT; i++)
		{
			int oldPosX = randX;

			if (i == 3)
			{
				minX = 270; // for the bottom floor, move away the ladder from mario
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

			_Ladder[i].setTexture(_TextureLadder);
			_Ladder[i].setPosition(randX, 0.f + BLOCK_SPACE * (i + 1) + _sizeBlock.y);

			std::shared_ptr<Entity> ladder = std::make_shared<Entity>();
			ladder->m_sprite = _Ladder[i];
			ladder->m_type = EntityType::ladder;
			ladder->m_size = _TextureLadder.getSize();
			ladder->m_position = _Ladder[i].getPosition();
			EntityManager::m_Entities.push_back(ladder);
		}



		// Draw Pieces

		_TexturePiece.loadFromFile("Media/Textures/Piece_small.png");
		_sizePiece = _TexturePiece.getSize();

		for (int i = 0; i < PIECE_COUNT; i++)
		{
			_Piece[i].setTexture(_TexturePiece);

			sf::Vector2f posPiece;

			posPiece.x = 200.f;

			posPiece.y = BLOCK_SPACE * (i / 2 + 2) - (_sizePiece.y + _sizePiece.y / 2);

			if (i % 2 == 0)
			{
				posPiece.x += 470.f;
			}

			_Piece[i].setPosition(posPiece);

			std::shared_ptr<Entity> piece = std::make_shared<Entity>();
			piece->m_sprite = _Piece[i];
			piece->m_type = EntityType::piece;
			piece->m_size = _TexturePiece.getSize();
			piece->m_position = _Piece[i].getPosition();

			EntityManager::m_Entities.push_back(piece);
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


		// Draw Peach

		_TexturePeach.loadFromFile("Media/Textures/Peach_small.png");
		_sizePeach = _TexturePeach.getSize();
		_Peach.setTexture(_TexturePeach);

		sf::Vector2f posPeach;
		posPeach.x = eng() % (700 - 170) + 170;
		posPeach.y = BLOCK_SPACE - _sizePeach.y;
		_Peach.setPosition(posPeach);

		std::shared_ptr<Entity> peach = std::make_shared<Entity>();
		peach->m_sprite = _Peach;
		peach->m_type = EntityType::peach;
		peach->m_size = _TexturePeach.getSize();
		peach->m_position = _Peach.getPosition();

		EntityManager::m_Entities.push_back(peach);



		// Draw Mario

		mTexture.loadFromFile("Media/Textures/Mario_animation_small.png");
		sf::IntRect rectSourceSprite(0, 0, 31, 40);

		mPlayer.setTextureRect(rectSourceSprite);
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

		EntityManager::m_Entities.push_back(player);


		// Draw Heart

		_TextureHeart.loadFromFile("Media/Textures/Heart_small.png");
		_sizeHeart = _TextureHeart.getSize();

		for (int i = 0; i < healthPoints; i++)
		{
			_Heart[i].setTexture(_TextureHeart);
			_Heart[i].setPosition(screenResolution.x - ((i + 1) * _sizeHeart.x), 0.f);

			std::shared_ptr<Entity> heart = std::make_shared<Entity>();
			heart->m_sprite = _Heart[i];
			heart->m_type = EntityType::heart;
			heart->m_size = _TextureHeart.getSize();
			heart->m_position = _Heart[i].getPosition();
			EntityManager::m_Entities.push_back(heart);
		}



		// Draw Score 
		
		score = 0;
		mFontCustom.loadFromFile("Media/College.ttf");
		mScore.setString("Score : " + toString(score));
		mScore.setFont(mFontCustom);
		mScore.setCharacterSize(24);

		int scorePosX = (screenResolution.x / 2) - mScore.getLocalBounds().width / 2;
		mScore.setPosition(scorePosX, 0.f);


		mIsSpaceBar = false;

		gameMode = GameMode::playing;
	}
	else if (gameMode == GameMode::playing)
	{
		std::shared_ptr<Entity> player;
		player = EntityManager::GetPlayer();

		int delta_x = abs(player->m_sprite.getPosition().x - _Ladder[3].getPosition().x);
		int delta_y = abs(player->m_sprite.getPosition().y - _Ladder[3].getPosition().y);
		int pos_player_x = player->m_sprite.getPosition().x;
		int pos_player_y = player->m_sprite.getPosition().y;

		sf::Vector2f movement(0.f, 0.f);
		sf::Vector2f movementEnnemi[ENNEMI_COUNT];

		// manual imput
		if (mIsMovingUp) 
		{
			if (jumpMario.state == JumpState::notActivated)
			{
				bool isClimbingLadderTemp = false;

				for (int i = 0; i < LADDER_COUNT; i++) 
				{
					if (collision.isCollision(player->m_sprite, _Ladder[i], _sizeBlock.y, _Ladder->getLocalBounds().width / 2))
					{
						movement.y -= PlayerSpeed;
						isClimbingLadderTemp = true;
					}
				}

				isClimbingLadder = isClimbingLadderTemp;
			}
			
		}

		if (mIsMovingDown) 
		{
			if (jumpMario.state == JumpState::notActivated)
			{
				bool isClimbingLadderTemp = false;
				int getNearestFloorPosY = getNearestFloor(pos_player_y, _sizeMario.y);

				for (int i = 0; i < LADDER_COUNT; i++)
				{
					if (collision.isCollision(player->m_sprite, _Ladder[i], _sizeBlock.y, _Ladder->getLocalBounds().width / 2))
					{
						if (pos_player_y < getNearestFloorPosY)
						{
							movement.y += PlayerSpeed;
							isClimbingLadderTemp = true;
						}
					}
				}

				isClimbingLadder = isClimbingLadderTemp;
			}
		}

		if (mIsMovingLeft)
		{
			if (pos_player_x > 170 && (!isClimbingLadder || pos_player_y == getNearestFloor(pos_player_y, _sizeMario.y)))
			{
				movement.x -= PlayerSpeed;
				isClimbingLadder = false;

				if (jumpMario.state != JumpState::notActivated)
				{
					changeOrientation(player, true);
				}
				else if (clockAnimation.getElapsedTime().asSeconds() >= 0.15f)
				{
					startAnimation(true);
				}

			}
		}

		if (mIsMovingRight)
		{
			if (pos_player_x < 685 && (!isClimbingLadder || pos_player_y == getNearestFloor(pos_player_y, _sizeMario.y)))
			{
				movement.x += PlayerSpeed;
				isClimbingLadder = false;

				if (jumpMario.state != JumpState::notActivated)
				{
					changeOrientation(player);
				}
				else if (clockAnimation.getElapsedTime().asSeconds() >= 0.15f)
				{
					startAnimation();
				}
			}
		}

		if (mIsSpaceBar)
		{
			if (jumpMario.state == JumpState::notActivated)
			{
				jumpMario.state = JumpState::toTheTop;
				jumpMario.initialPosY = getNearestFloor(pos_player_y, _sizeMario.y);
			}
		}


		if ((!mIsMovingUp && !mIsMovingDown && !mIsMovingLeft && !mIsMovingRight) || jumpMario.state != JumpState::notActivated || isClimbingLadder)
		{
			stopAnimation();
		}

		int ennemiIndex = 0;

		for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
		{
			if (entity->m_enabled == false)
			{
				continue;
			}

			if (entity->m_type != EntityType::player && entity->m_type != EntityType::ennemi && entity->m_type != EntityType::peach && entity->m_type != EntityType::piece)
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
					if (collision.isCollision(player->m_sprite, entity->m_sprite, 0, 5))
					{
						if (jumpMario.state == JumpState::toTheBottom)
						{
							entity->m_enabled = false;

							score += 50;
							mScore.setString("Score : " + toString(score));

							jumpMario.state = JumpState::toTheTop;
						}
						else if (healthPoints > 0)
						{
							healthPoints--;
							godTimeInSec = GOD_TIME_IN_SEC;

							std::cout << "Hit ! Health remaining : " << healthPoints << std::endl;

							EntityManager::DisableOneHeart(healthPoints);

							if (score > 0)
							{
								score -= 50;
								mScore.setString("Score : " + toString(score));
							}

							if (healthPoints == 0)
							{
								gameMode = GameMode::ending;

								std::cout << "Game Over !" << std::endl;
								stopAnimation();

								// Draw Game Over

								_TextureGameOver.loadFromFile("Media/Textures/Game_over_small.png");
								_sizeGameOver = _TextureGameOver.getSize();
								_GameOver.setTexture(_TextureGameOver);

								_GameOver.setPosition((screenResolution.x / 2) - (_sizeGameOver.x / 2), (screenResolution.y / 2) - (_sizeGameOver.y / 2));

								std::shared_ptr<Entity> gameOver = std::make_shared<Entity>();
								gameOver->m_sprite = _GameOver;
								gameOver->m_type = EntityType::endScreen;
								gameOver->m_size = _TextureGameOver.getSize();
								gameOver->m_position = _GameOver.getPosition();
								EntityManager::m_Entities.push_back(gameOver);

								clockEndGame.restart();
							}
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

			if (entity->m_type == EntityType::peach)
			{
				if (!isVictory && collision.isCollision(player->m_sprite, entity->m_sprite))
				{
					if (clockEndGame.getElapsedTime().asSeconds() >= 3.0f)
					{
						std::cout << "You Win !" << std::endl;
						isVictory = true;
						gameMode = GameMode::ending;

						if (player->m_sprite.getPosition().x < entity->m_sprite.getPosition().x)
						{
							changeOrientation(entity, true);
						}
						else
						{
							changeOrientation(entity);
						}

						stopAnimation();

						// Draw Victory

						_TextureVictory.loadFromFile("Media/Textures/Victory_small.png");
						_sizeVictory = _TextureVictory.getSize();
						_Victory.setTexture(_TextureVictory);

						_Victory.setPosition((screenResolution.x / 2) - (_sizeVictory.x / 2), (screenResolution.y / 2) - (_sizeVictory.y / 2));

						std::shared_ptr<Entity> victory = std::make_shared<Entity>();
						victory->m_sprite = _Victory;
						victory->m_type = EntityType::endScreen;
						victory->m_size = _TextureVictory.getSize();
						victory->m_position = _Victory.getPosition();
						EntityManager::m_Entities.push_back(victory);



						// Draw Love

						_Love.setTexture(_TextureHeart);
						_Love.setPosition(entity->m_sprite.getPosition().x, _sizeHeart.y * 1);

						std::shared_ptr<Entity> love = std::make_shared<Entity>();
						love->m_sprite = _Love;
						love->m_type = EntityType::endScreen;
						love->m_size = _TextureHeart.getSize();
						love->m_position = _Love.getPosition();
						EntityManager::m_Entities.push_back(love);

						
						clockEndGame.restart();
					}
				}
			}

			if (entity->m_type == EntityType::piece)
			{
				if (collision.isCollision(player->m_sprite, entity->m_sprite))
				{
					entity->m_enabled = false;

					score += 100;
					mScore.setString("Score : " + toString(score));
				}
			}

		}

	}
	else if (gameMode == GameMode::ending)
	{
		if (clockEndGame.getElapsedTime().asSeconds() >= 5.0f)
		{
			EntityManager::m_Entities.clear();
			gameMode = GameMode::initializeMenu;
			clockEndGame.restart();
			isVictory = false;
		}
	}
	else // menu
	{
		// manual imput
		if (mIsMovingUp) 
		{
			if (clockCursor.getElapsedTime().asSeconds() >= 0.2f)
			{
				moveCursorMenu();
				clockCursor.restart();
			}
		}

		if (mIsMovingDown) 
		{
			if (clockCursor.getElapsedTime().asSeconds() >= 0.2f)
			{
				moveCursorMenu();
				clockCursor.restart();
			}
		}

		if (mIsSpaceBar)
		{
			if (cursorSelection == ActionMenu::newGame)
			{
				EntityManager::m_Entities.clear();
				gameMode = GameMode::initializeGame;
			}
			else
			{
				mWindow.close();
			}
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

	if (gameMode == GameMode::menu)
	{
		mWindow.draw(mTitle);
		mWindow.draw(mInfo);
	}
	else if (gameMode == GameMode::playing)
	{
		mWindow.draw(mScore);
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
		mIsSpaceBar = isPressed;
	}
}

