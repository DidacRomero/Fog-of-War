#ifndef __FOWMANAGER_H__
#define __FOWMANAGER_H__

#include "j1Module.h"
#include "p2Defs.h"
#include "p2Point.h"
#include <list>

struct SDL_Texture;
struct SDL_Rect;

enum class FOW_TileState
{
	//Basic states
	UNVISITED = 0,
	VISIBLE = 1,
	SHROUDED,

	//States for edge smoothing (SMTH)
	// Black
	BLACK_SMTH_TOP,
	BLACK_SMTH_DOWN,
	BLACK_SMTH_RIGHT,
	BLACK_SMTH_LEFT,

	BLACK_SMTH_TRIGHT_CORNER, // Top right Corner
	BLACK_SMTH_TLEFT_CORNER, // Top left Corner
	BLACK_SMTH_DRIGHT_CORNER, // Down right Corner
	BLACK_SMTH_DLEFT_CORNER, // Down left Corner

	// Shroud
	SHROUD_SMTH_TOP,
	SHROUD_SMTH_DOWN,
	SHROUD_SMTH_RIGHT,
	SHROUD_SMTH_LEFT,

	SHROUD_SMTH_TRIGHT_CORNER, // Top right Corner
	SHROUD_SMTH_TLEFT_CORNER, // Top left Corner
	SHROUD_SMTH_DRIGHT_CORNER, // Down right Corner
	SHROUD_SMTH_DLEFT_CORNER, // Down left Corner


	BTOS_SMTH_TRIGHT_CORNER, // Top right Corner Black to shroud
	BTOS_SMTH_TLEFT_CORNER, // Top left Corner Black to shroud
	BTOS_SMTH_DRIGHT_CORNER, // Down right Corner Black to shroud
	BTOS_SMTH_DLEFT_CORNER, // Down left Corner Black to shroud


};

struct FOW_Entity
{
	// List containing the tiles 
	std::list<iPoint> frontier;
	std::list<iPoint> last_frontier;

	iPoint position;
};

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
	
	// Reset Visibility Map
	void ResetFOWVisibility();

	int8_t GetVisibilityTileAt(const iPoint& pos) const;

	SDL_Rect& GetFOWMetaRect(FOW_TileState state);

private: // Functions

	// Set the state of a tile in the visibility map
	void SetVisibilityTile(iPoint pos, FOW_TileState state);

	// Returns a list of iPoints that are contained in width and height
	std::list<iPoint> GetRectFrontier(uint width, uint height, iPoint pos);

	// Manage visibility of entities
	void ManageEntitiesVisibility();

	//Update Entities positions
	void UpdateEntitiesPositions();

	// Tile inside a Frontier
	int8_t TileInsideFrontier(iPoint tile, const std::list<iPoint>& frontier_checked) const;

	// Check boundaries
	bool CheckBoundaries(const iPoint& pos) const;

	// Fill a given Frontier to return a LOS (Line of Sight)
	std::list<iPoint> FillFrontier(const std::list<iPoint>& frontier);

	// Set a frontier with the rectangle size
	std::list<iPoint> CreateFrontierRect(uint width, uint height);

public: // Variables

	bool debug = false;

	SDL_Texture* meta_FOW = nullptr;

private: //Variables

	uint width, height;
	int8_t* visibility_map = nullptr;

	//---- This 2 pointers are for debug purpose only
	int8_t* debug_map = nullptr;
	int8_t* visibility_debug_holder = nullptr;
	//----

	// Testing the FOW_Entity 
	FOW_Entity player;

	// This list contains the position in MAP COORDINATES of all entities
	std::list<iPoint> entities_pos;

};


#endif 

