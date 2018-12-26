#include "pch.h"
#include "CollisionManger.h"



CollisionManger::CollisionManger()
{
}


CollisionManger::~CollisionManger()
{
}

bool CollisionManger::isCollision(sf::Sprite const& mainCharacter, sf::Sprite const& object)
{
	//Character 
	int position_x_character = mainCharacter.getPosition().x;
	int position_y_character = mainCharacter.getPosition().y;

	int width_character = mainCharacter.getLocalBounds().width;
	int heigth_character = mainCharacter.getGlobalBounds().height;


	//Object
	int position_x_object = object.getPosition().x;
	int position_y_object = object.getPosition().y;

	int width_object = object.getLocalBounds().width;
	int heigth_object = object.getGlobalBounds().height;


	if (
		position_x_character >= (position_x_object + width_character) ||
		(position_x_character + width_character) <= position_x_object ||
		position_y_character >= (position_y_object + heigth_object) ||
		(position_y_character + heigth_character) <= heigth_object)
	{
		
		return false;
	}
	else {
		return true;
	}

}



