#pragma once

#include "EntityManager.h"

enum JumpState
{
	notActivated,
	toTheTop,
	toTheBottom
};

struct JumpAction
{
	int state;
	float initialPosY;
};

class Player
{
public:
	Player();
	~Player();

public:
	std::shared_ptr<Entity> entity;
};

