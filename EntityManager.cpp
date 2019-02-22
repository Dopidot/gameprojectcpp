#include "pch.h"
#include "EntityManager.h"

std::vector<std::shared_ptr<Entity>> EntityManager::m_Entities;

EntityManager::EntityManager()
{
}


EntityManager::~EntityManager()
{
}

std::shared_ptr<Entity> EntityManager::GetPlayer()
{
	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_enabled == false)
		{
			continue;
		}

		if (entity->m_type == EntityType::player)
		{
			return entity;
		}
	}

	return nullptr;
}

void EntityManager::DisableOneHeart(int healthPoints)
{
	int indexOfHeart = -1;
	int heartsVisible = 0;

	for (int i = 0; i < EntityManager::m_Entities.size(); i++)
	{
		if (EntityManager::m_Entities[i]->m_type != EntityType::heart)
		{
			continue;
		}

		if (EntityManager::m_Entities[i]->m_enabled)
		{
			if (indexOfHeart == -1)
			{
				indexOfHeart = i;
			}

			heartsVisible++;
		}
	}

	if (heartsVisible > healthPoints && indexOfHeart != -1)
	{
		EntityManager::m_Entities[indexOfHeart]->m_enabled = false;
	}
}

std::shared_ptr<Entity> EntityManager::GetCursor()
{
	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_enabled == false)
		{
			continue;
		}

		if (entity->m_type == EntityType::cursor)
		{
			return entity;
		}
	}

	return nullptr;
}
