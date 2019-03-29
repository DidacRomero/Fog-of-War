#ifndef __FOWMANAGER_H__
#define __FOWMANAGER_H__

#include "j1Module.h"
#include "p2Defs.h"
#include "p2Point.h"
#include <list>

class FowManager : public j1Module
{
public: // Functions

	FowManager();

	// Destructor
	virtual ~FowManager();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//Load
	bool Load(pugi::xml_node&);
	//Save
	bool Save(pugi::xml_node&) const;

	// Set Visibility Map
	void SetVisibilityMap(uint width, uint height);

	int8_t GetVisibilityTileAt(const iPoint& pos) const;

private: // Functions

	// Set the value of a 
	void SetVisibilityTile(iPoint pos, int8_t value);

	// Returns a list of iPoints that are contained in width and height
	std::list<iPoint> GetRectFrontier(uint width, uint height, iPoint pos);

	// Manage visibility of entities
	void ManageEntitiesVisibility();

	//Update Entities positions
	void UpdateEntitiesPositions();

public: // Variables

	bool debug = false;

private: //Variables
	uint width, height;
	int8_t* visibility_map = nullptr;

	//---- This 2 pointers are for debug purpose only
	int8_t* debug_map = nullptr;
	int8_t* visibility_debug_holder;
	//----


	// This list contains the position in MAP COORDINATES of all entities
	std::list<iPoint> entities_pos;

};


#endif 

