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
	VISIBLE = 255,
	FOGGED = 1,

	//States for edge smoothing (SMTH)
	// Black
	UNVISITED_SMTH_TOP,
	UNVISITED_SMTH_DOWN,
	UNVISITED_SMTH_RIGHT,
	UNVISITED_SMTH_LEFT,

	UNVISITED_SMTH_TRIGHT_CORNER, // Top right Corner
	UNVISITED_SMTH_TLEFT_CORNER, // Top left Corner
	UNVISITED_SMTH_DRIGHT_CORNER, // Down right Corner
	UNVISITED_SMTH_DLEFT_CORNER, // Down left Corner

	UNVISITED_SMTH_DEAD_END_TOP,
	UNVISITED_SMTH_DEAD_END_BOTTOM,
	UNVISITED_SMTH_DEAD_END_RIGHT,
	UNVISITED_SMTH_DEAD_END_LEFT,

	UNVISITED_SMTH_TLEFT_OUT_CORNER,
	UNVISITED_SMTH_DLEFT_OUT_CORNER,
	UNVISITED_SMTH_DRIGHT_OUT_CORNER,
	UNVISITED_SMTH_TRIGHT_OUT_CORNER,

	// Shroud
	FOGGED_SMTH_TOP,
	FOGGED_SMTH_DOWN,
	FOGGED_SMTH_RIGHT,
	FOGGED_SMTH_LEFT,

	FOGGED_SMTH_TRIGHT_CORNER, // Top right Corner
	FOGGED_SMTH_TLEFT_CORNER, // Top left Corner
	FOGGED_SMTH_DRIGHT_CORNER, // Down right Corner
	FOGGED_SMTH_DLEFT_CORNER, // Down left Corner

	FOGGED_SMTH_DEAD_END_TOP,
	FOGGED_SMTH_DEAD_END_BOTTOM,
	FOGGED_SMTH_DEAD_END_RIGHT,
	FOGGED_SMTH_DEAD_END_LEFT,

	FOGGED_SMTH_TLEFT_OUT_CORNER,
	FOGGED_SMTH_DLEFT_OUT_CORNER,
	FOGGED_SMTH_DRIGHT_OUT_CORNER,
	FOGGED_SMTH_TRIGHT_OUT_CORNER,

	// Black to Shroud
	UTOF_SMTH_TOP,
	UTOF_SMTH_DOWN,
	UTOF_SMTH_RIGHT,
	UTOF_SMTH_LEFT,

	UTOF_SMTH_TRIGHT_CORNER, // Top right Corner Black to shroud
	UTOF_SMTH_TLEFT_CORNER, // Top left Corner Black to shroud
	UTOF_SMTH_DRIGHT_CORNER, // Down right Corner Black to shroud
	UTOF_SMTH_DLEFT_CORNER, // Down left Corner Black to shroud

	UTOF_SMTH_DEAD_END_TOP,
	UTOF_SMTH_DEAD_END_BOTTOM,
	UTOF_SMTH_DEAD_END_RIGHT,
	UTOF_SMTH_DEAD_END_LEFT,

	UTOF_SMTH_TLEFT_OUT_CORNER,
	UTOF_SMTH_DLEFT_OUT_CORNER,
	UTOF_SMTH_DRIGHT_OUT_CORNER,
	UTOF_SMTH_TRIGHT_OUT_CORNER,

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

	std::list<iPoint> LOS;

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



	// Create a Fog of War Entity (Abstraction of an entity that will be managed
	// by the FOWManager)
	FOW_Entity* CreateFOWEntity(iPoint position, bool provides_visibility);

	//Destroy a Fog of War Entity and take it out of the list
	bool DestroyFOWEntity(FOW_Entity* to_destroy);



	// Set Visibility Map
	void SetVisibilityMap(uint width, uint height);
	// Reset Visibility Map
	void ResetVisibilityMap();

	//Get the value of a tile within the Visibility Map
	int8_t GetVisibilityTileAt(const iPoint& pos) const;

	//Get the Rect to blit the correspondant tile state
	SDL_Rect& GetFOWMetaRect(FOW_TileState state);



	//This function will call do the proper frontier managing and calling the functions 
	//that will actually smooth the edges
	void SmoothEdges(FOW_Entity* fow_entity);

	void SmoothEntitiesInnerEdges(std::list<iPoint> frontier);
	void SmoothEntitiesOuterEdges(std::list<iPoint> frontier);

	// Set a squared frontier depending of a radius
	std::list<iPoint> CreateFrontierSquare(uint radius, iPoint center);

	// Fill a given Frontier to return a LOS (Line of Sight)
	std::list<iPoint> FillFrontier(const std::list<iPoint>& frontier);


private: // Functions

	// Set the state of a tile in the visibility map
	void SetVisibilityTile(iPoint pos, FOW_TileState state);

	// Manage visibility of entities
	void ManageEntitiesVisibility();

	// Manage Entities FOW manipulation
	void ManageEntitiesFOWManipulation();

	// Returns true if a tile is inside a given list (if an iPoint is inside an iPoint list)
	bool TileInsideList(iPoint tile, const std::list<iPoint>& list_checked) const;

	// Check boundaries of the Visibility map
	bool CheckBoundaries(const iPoint& pos) const;

	// Set a frontier with the rectangle size
	std::list<iPoint> CreateFrontierRect(uint width, uint height, iPoint center);

public: // Variables

	bool debug = false;
	bool scouting_trail = true; // Leave a trail of Fogged tiles when leaving an area

	//Texture for the fog of war tiles
	SDL_Texture* meta_FOW = nullptr;

private: //Variables

	uint width, height;

	int8_t* visibility_map = nullptr;

	// List of all FOW Entities
	std::list<FOW_Entity*> fow_entities;

	//---- This 2 pointers are for debug purpose only
	int8_t* debug_map = nullptr;
	int8_t* visibility_debug_holder = nullptr;
	//----

};
#endif 