#include "j2EntityManager.h"
#include "j2Entity.h"
#include "j1App.h"

//Entities
#include "DummyEnemy.h"
#include "Player.h"


j2EntityManager::j2EntityManager() : j1Module()
{
	name="entities";

	update_ms_cycle = 0.200f; 
}

j2EntityManager::~j2EntityManager()
{
}

bool j2EntityManager::Awake(pugi::xml_node & config)
{
	bool ret = true;

	for (std::list<j2Entity*>::iterator item = entities.begin(); item != entities.end(); item++)
	{
		ret = (*item)->Awake(config);
		if (!ret)
			break;
	}

	CreateEntity(ENTITY_TYPE::PLAYER);
	SetLastEntityPos(50, 900);

	CreateEntity(ENTITY_TYPE::ENEMY);
	SetLastEntityPos(10, 100);

	CreateEntity(ENTITY_TYPE::ENEMY);
	SetLastEntityPos(500, 450);

	CreateEntity(ENTITY_TYPE::ENEMY);
	SetLastEntityPos(850, 732);

	CreateEntity(ENTITY_TYPE::ENEMY);
	SetLastEntityPos(-400, 600);

	CreateEntity(ENTITY_TYPE::ENEMY);
	SetLastEntityPos(300, 1200);

	return ret;

}

bool j2EntityManager::Start()
{
	bool ret = true;

	for (std::list<j2Entity*>::iterator item = entities.begin(); item != entities.end(); item++)
	{
		ret = (*item)->Start();
		if (!ret)
			break;
	}

	return ret;
}

bool j2EntityManager::PreUpdate()
{
	bool ret = true;

	for (std::list<j2Entity*>::iterator item = entities.begin(); item != entities.end(); item++)
	{
		if ((*item)->entity_enabled == true)
		{
			ret = (*item)->PreUpdate();
			if (!ret)
				break;
		}
	}

	return ret;
}

bool j2EntityManager::Update(float dt)
{
	bool ret = true;

	accumulated_time += dt;
	if (accumulated_time >= update_ms_cycle)
		do_logic = true;

	for (std::list<j2Entity*>::iterator item = entities.begin(); item != entities.end(); item++)
	{
		if ((*item)->entity_enabled == true)
		{
			ret = (*item)->Update(dt,do_logic);
			if (!ret)
				break;
		}
	}

	if (do_logic == true) {
		accumulated_time = 0.0f;
		do_logic = false;
	}

	return ret;
}

bool j2EntityManager::PostUpdate()
{
	bool ret = true;
	for (std::list<j2Entity*>::iterator item = entities.begin(); item != entities.end(); item++)
	{
		if ((*item)->visible == true)
		{
			ret = (*item)->PostUpdate();
			(*item)->Draw();
			if (!ret)
				break;
		}
	}

	return ret;
}

bool j2EntityManager::CleanUp()
{
	bool ret = true;
	for (std::list<j2Entity*>::iterator item = entities.begin(); item != entities.end(); item++)
	{
		ret = (*item)->CleanUp();
		if (!ret)
			break;
	}

	return ret;
}

bool j2EntityManager::Load(pugi::xml_node &save_game_manager)
{
	bool ret = true;

	for (std::list<j2Entity*>::iterator item = entities.begin(); item != entities.end(); item = item++)
	{
		ret = (*item)->Load(save_game_manager);
		if (!ret)
			break;
	}

	return ret;
}

bool j2EntityManager::Save(pugi::xml_node &save_game_manager) const
{
	bool ret = true;

	for (std::list<j2Entity*>::const_iterator item = entities.cbegin(); item != entities.end(); item = item++)
	{
		ret = (*item)->Save(save_game_manager);
		if (!ret)
			break;
	}

	return ret;
}

j2Entity* j2EntityManager::CreateEntity(ENTITY_TYPE type)
{
	static_assert(ENTITY_TYPE::UNKNOWN == ENTITY_TYPE(2), "code needs update");
	j2Entity* ret = nullptr;
	switch (type) {

	case ENTITY_TYPE::ENEMY: ret = new DummyEnemy(); break;
	case ENTITY_TYPE::PLAYER: ret = new Player(); break;
	}
	if (ret != nullptr)
		entities.push_back(ret);
	return ret;
}

void j2EntityManager::DestroyEntity(j2Entity* entity_to_destroy)
{
	for (std::list<j2Entity*>::iterator item = entities.begin(); item != entities.end(); item++)
	{
		if ((*item) == entity_to_destroy)
		{
			entities.remove(*item);
			RELEASE(*item);
			break;
		}
	}
}

const std::list<j2Entity*> j2EntityManager::GetEntitiesInfo() const
{
	return entities;
}

void j2EntityManager::SetLastEntityPos(int x, int y)
{
	std::list<j2Entity*>::iterator item = --entities.end();
	(*item)->SetPos(x, y);
}
