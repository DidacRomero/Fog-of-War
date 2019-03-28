#ifndef __FOWMANAGER_H__
#define __FOWMANAGER_H__

#include "j1Module.h"
#include "p2Defs.h"
#include "p2Point.h"

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

public: // Variables


private: //Variables
	uint width, height;
	int8_t* visibility_map = nullptr;

};


#endif 

