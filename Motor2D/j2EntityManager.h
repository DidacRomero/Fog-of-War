#ifndef __J2ENTITYMANAGER_H__
#define __J2ENTITYMANAGER_H__

#include "j1Module.h"
#include "PugiXml\src\pugixml.hpp"
#include <list>

class j2Entity;
enum class ENTITY_TYPE
{
	UNKNOWN
};

class j2EntityManager : public j1Module
{
public:

	//Constructor
	j2EntityManager();
	//Destructor
	virtual ~j2EntityManager();

	// Awake: Called before render is available
	bool Awake(pugi::xml_node& config);

	//Start
	bool Start();
	//PreUpdate
	bool PreUpdate();
	//Update
	bool Update(float dt);

	//PostUpdate
	bool PostUpdate();

	bool CleanUp();

	//Load
	bool Load(pugi::xml_node&);
	//Save
	bool Save(pugi::xml_node&) const;

public:
	j2Entity* CreateEntity(ENTITY_TYPE type);
	void DestroyEntity(j2Entity* entity_to_destroy);

	std::list<j2Entity*> entities;

private:

	bool	do_logic;
	int		logic_updates_per_second;
	float	update_ms_cycle;
	float   accumulated_time;

};
#endif
