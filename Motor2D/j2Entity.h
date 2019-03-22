#ifndef __J2ENTITIES_H__
#define __J2ENTITIES_H__

#include "p2Point.h"
#include "SDL_image/include/SDL_image.h"
#include "PugiXml/src/pugixml.hpp"
#include "SDL_image/include/SDL_image.h"
#include <string>

class j2EntityManager;

enum class ENTITY_TYPE
{
	ENEMY,
	PLAYER,
	UNKNOWN
};


class j2Entity
{
public:

	//Constructor

	j2Entity(ENTITY_TYPE type) : name("Unnamed"), manager(NULL), type(type), entity_enabled(true) {}

	//Destructor
	virtual ~j2Entity() {}

	virtual bool Awake(pugi::xml_node& config) { return true; }
	//Start
	virtual bool Start() { return true; }
	//PreUpdate		Called each loop iteration
	virtual bool PreUpdate() { return true; }
	//Update		Called each loop iteration
	virtual bool Update(float dt, bool do_logic) { return true; }
	//PostUpdate	Called each loop iteration
	virtual bool PostUpdate() { return true; }
	//CleanUp
	virtual bool CleanUp() { return true; }

	//Draw Entity
	virtual void Draw() {};

	//Load
	virtual bool Load(pugi::xml_node&) { return true; }
	//Save
	virtual bool Save(pugi::xml_node&) const { return true; }

public:
	iPoint position = {0,0};
	SDL_Rect entity_rect;
	bool active = false;
	bool entity_enabled = false;

	SDL_Texture* entity_tex = nullptr;

	ENTITY_TYPE type;

	std::string name;

private:
	j2EntityManager* manager;
};

#endif
