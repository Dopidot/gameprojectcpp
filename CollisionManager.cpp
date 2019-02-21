#include "pch.h"
#include "CollisionManager.h"



CollisionManager::CollisionManager()
{
}


CollisionManager::~CollisionManager()
{
}

bool CollisionManager::isCollision(sf::Sprite const& mainCharacter, sf::Sprite const& object, int stairGap)
{
	//Character 
	int position_x1_character = mainCharacter.getPosition().x;
	int position_y1_character = mainCharacter.getPosition().y;

	int position_x2_character = position_x1_character + mainCharacter.getLocalBounds().width;
	int position_y2_character = position_y1_character + mainCharacter.getLocalBounds().height;

	//Object
	int position_x1_object = object.getPosition().x;
	int position_y1_object = object.getPosition().y;

	int position_x2_object = position_x1_object + object.getLocalBounds().width;
	int position_y2_object = position_y1_object + object.getLocalBounds().height;

	if (
		position_x1_character >= position_x2_object ||
		position_x2_character <= position_x1_object ||
		position_y1_character >= position_y2_object  ||
		position_y2_character <= position_y1_object - stairGap
		)
	{
		
		return false;
	}
	else {
		return true;
	}

}



