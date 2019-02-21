#pragma once

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
};

