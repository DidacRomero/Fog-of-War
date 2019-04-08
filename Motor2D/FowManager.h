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

	BLACK_SMTH_DEAD_END_TOP,
	BLACK_SMTH_DEAD_END_BOTTOM,
	BLACK_SMTH_DEAD_END_RIGHT,
	BLACK_SMTH_DEAD_END_LEFT,

	BLACK_SMTH_TLEFT_OUT_CORNER,
	BLACK_SMTH_DLEFT_OUT_CORNER,
	BLACK_SMTH_DRIGHT_OUT_CORNER,
	BLACK_SMTH_TRIGHT_OUT_CORNER,

	// Shroud
	SHROUD_SMTH_TOP,
	SHROUD_SMTH_DOWN,
	SHROUD_SMTH_RIGHT,
	SHROUD_SMTH_LEFT,

	SHROUD_SMTH_TRIGHT_CORNER, // Top right Corner
	SHROUD_SMTH_TLEFT_CORNER, // Top left Corner
	SHROUD_SMTH_DRIGHT_CORNER, // Down right Corner
	SHROUD_SMTH_DLEFT_CORNER, // Down left Corner

	SHROUD_SMTH_DEAD_END_TOP,
	SHROUD_SMTH_DEAD_END_BOTTOM,
	SHROUD_SMTH_DEAD_END_RIGHT,
	SHROUD_SMTH_DEAD_END_LEFT,

	SHROUD_SMTH_TLEFT_OUT_CORNER,
	SHROUD_SMTH_DLEFT_OUT_CORNER,
	SHROUD_SMTH_DRIGHT_OUT_CORNER,
	SHROUD_SMTH_TRIGHT_OUT_CORNER,

	// Black to Shroud
	BTOS_SMTH_TOP,
	BTOS_SMTH_DOWN,
	BTOS_SMTH_RIGHT,
	BTOS_SMTH_LEFT,

	BTOS_SMTH_TRIGHT_CORNER, // Top right Corner Black to shroud
	BTOS_SMTH_TLEFT_CORNER, // Top left Corner Black to shroud
	BTOS_SMTH_DRIGHT_CORNER, // Down right Corner Black to shroud
	BTOS_SMTH_DLEFT_CORNER, // Down left Corner Black to shroud

	BTOS_SMTH_DEAD_END_TOP,
	BTOS_SMTH_DEAD_END_BOTTOM,
	BTOS_SMTH_DEAD_END_RIGHT,
	BTOS_SMTH_DEAD_END_LEFT,

	BTOS_SMTH_TLEFT_OUT_CORNER,
	BTOS_SMTH_DLEFT_OUT_CORNER,
	BTOS_SMTH_DRIGHT_OUT_CORNER,
	BTOS_SMTH_TRIGHT_OUT_CORNER,

	MAX_TILESTATE

};

struct FOW_Entity
{
	// Vars for position and movement
	iPoint position = {0,0};
	iPoint motion = { 0,0 };

	// Bools for checks
	bool is_visible;
	bool provides_visibility;
	bool moved_in_map = false; // This boolean stipulates if we moved in map coordinates

	// Lists containing tiles 
	std::list<iPoint> frontier;
	std::list<iPoint> last_frontier;

	std::list<iPoint> LOS;
	std::list<iPoint> last_LOS;

	// Default Constructor
	FOW_Entity() {}
	// Constructor
	FOW_Entity(iPoint position, bool provides_visibility);

	// Pass the position in world coordinates, the function will automatically
	// change the coordinates to map coordinates
	void SetPos(iPoint new_pos);

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
	// Called each loop iteration
	bool Update(float dt);
	// Called before quitting
	bool CleanUp();

	//Load
	bool Load(pugi::xml_node&);
	//Save
	bool Save(pugi::xml_node&) const;

	// Create a Fog of War Entity (Abstraction of an entity that will be managed
	// by the FOWManager)
	FOW_Entity* CreateFOWEntity(iPoint position, bool provides_visibility);

	//Destroy a Fog of War Entity and take it out of the list
	bool DestroyFOWEntity(FOW_Entity* to_destroy);

	// Set Visibility Map
	void SetVisibilityMap(uint width, uint height);
	// Reset Visibility Map
	void ResetFOWVisibility();

	int8_t GetVisibilityTileAt(const iPoint& pos) const;

	SDL_Rect& GetFOWMetaRect(FOW_TileState state);

	void SmoothEdges(FOW_Entity* fow_entity);

	void SmoothEntitiesInnerEdges(std::list<iPoint> frontier);

	
	// Set a squared frontier depending of a radius
	std::list<iPoint> CreateFrontierSquare(uint radius, iPoint center);

	// Fill a given Frontier to return a LOS (Line of Sight)
	std::list<iPoint> FillFrontier(const std::list<iPoint>& frontier);
private: // Functions

	// Set the state of a tile in the visibility map
	void SetVisibilityTile(iPoint pos, FOW_TileState state);

	// Returns a list of iPoints that are contained in width and height
	std::list<iPoint> GetRectFrontier(uint width, uint height, iPoint pos);

	// Manage visibility of entities
	void ManageEntitiesVisibility();

	//Update Entities positions
	bool UpdateEntitiesPositions();

	// Tile inside a Frontier
	bool TileInsideList(iPoint tile, const std::list<iPoint>& list_checked) const;

	// Check boundaries
	bool CheckBoundaries(const iPoint& pos) const;

	// Set a frontier with the rectangle size
	std::list<iPoint> CreateFrontierRect(uint width, uint height, iPoint center);

public: // Variables

	bool debug = false;

	bool scouting_trail = true;

	SDL_Texture* meta_FOW = nullptr;

private: //Variables

	uint width, height;


	int8_t* visibility_map = nullptr;

	// Testing the FOW_Entity 
	FOW_Entity player;

	// List of all FOW Entities
	std::list<FOW_Entity*> fow_entities;

	// This list contains the position in MAP COORDINATES of all entities
	std::list<iPoint> entities_pos;


	//---- This 2 pointers are for debug purpose only
	int8_t* debug_map = nullptr;
	int8_t* visibility_debug_holder = nullptr;
	//----

};
#endif 