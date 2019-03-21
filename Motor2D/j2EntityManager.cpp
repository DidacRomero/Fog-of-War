#include "j2EntityManager.h"
#include "j2Entity.h"
#include "j1App.h"


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
	active = false;

	bool ret = true;

	for (std::list<j2Entity*>::iterator item = entities.begin(); item != entities.end(); item++)
	{
		ret = (*item)->Awake(config);
		if (!ret)
			break;
	}

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
		if ((*item)->EntitiesEnable == true)
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
		if ((*item)->EntitiesEnable == true)
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
	for (std::list<j2Entity*>::iterator item; item != entities.end(); item++)
	{
		if ((*item)->EntitiesEnable == true)
		{
			ret = (*item)->PostUpdate();
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
	static_assert(ENTITY_TYPE::UNKNOWN == ENTITY_TYPE(0), "code needs update");
	j2Entity* ret = nullptr;
	//switch (type) {

	//*case ENTITY_TYPE::FLYING_ENEMY: ret = new j2FlyingEnemy(); break;
	//case ENTITY_TYPE::GROUND_ENEMY: ret = new j2GroundEnemy(); break;
	//case ENTITY_TYPE::PLAYER: ret = new j2Player(); break;
	//case ENTITY_TYPE::LIFE_ITEM: ret = new j2LifeItem(); break;
	//case ENTITY_TYPE::COIN: ret = new j2Coin(); break;*/
	//}
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
