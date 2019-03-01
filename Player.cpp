#include "pch.h"
#include "Player.h"
#include "Entity.h"


Player::Player()
{
	entity = EntityManager::GetPlayer();
}


Player::~Player()
{
}


