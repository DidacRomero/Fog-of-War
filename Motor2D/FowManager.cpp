#include "FowManager.h"
#include "j1App.h"
#include "j2EntityManager.h"
#include "j2Entity.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1Textures.h"

FowManager::FowManager()
{
}

FowManager::~FowManager()
{
	//Delete all 2D Fog data containers
	if (visibility_map != nullptr && visibility_map != debug_map)
	{
		delete[] visibility_map;
		visibility_debug_holder = nullptr;
	}
	else if (visibility_debug_holder != nullptr)
		delete[] visibility_debug_holder;
	
	
	if (debug_map != nullptr)
		delete[] debug_map;
}

bool FowManager::Awake()
{
	return true;
}

bool FowManager::Start()
{
	meta_FOW = App->tex->Load("maps/FOW_meta_sheet.png");

	return true;
}

bool FowManager::Update(float dt)
{
	ManageEntitiesVisibility();
	//Update Entities positions. 

	if (debug == false)
	{
		for (std::list<FOW_Entity*>::iterator item = fow_entities.begin(); item != fow_entities.end(); ++item)
		{
			if ((*item)->provides_visibility == true && (*item)->moved_in_map == true)
			{
				//We store the LOS of the current entity, since the LOS will change this will be our previous LOS
				std::list<iPoint> prev_LOS = (*item)->LOS;
				//If the Entity moved, we update the LOS position 
				for (std::list<iPoint>::iterator tile = (*item)->LOS.begin(); tile != (*item)->LOS.end(); tile++)
				{
					(*tile).x += (*item)->motion.x;
					(*tile).y += (*item)->motion.y;

					SetVisibilityTile((*tile), FOW_TileState::VISIBLE);
				}

				(*item)->moved_in_map = false;


				for (std::list<iPoint>::const_iterator lf_item = prev_LOS.cbegin(); lf_item != prev_LOS.end(); lf_item++)
				{
					if (TileInsideList((*lf_item), (*item)->LOS) == false)
					{
						if (scouting_trail == true)
							SetVisibilityTile((*lf_item), FOW_TileState::SHROUDED);
						else
							SetVisibilityTile((*lf_item), FOW_TileState::UNVISITED);
					}
				}

				SmoothEdges((*item));
			}
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		debug = !debug;

		// If we enter debug mode, our visibility map should be clear.
		// We will point to a clear visibility map (debug_map) so all calls depending on visibility_map don't
		// need further management. But before we store our current visibility map!
		if (debug == true)
		{
			//To keep the pointer to the visibility map we use our debug_holder;
			visibility_debug_holder = visibility_map;
			visibility_map = debug_map;
		}
		else // Debug == false
		{
			visibility_map = visibility_debug_holder;
		}
	}

	return true;
}

bool FowManager::CleanUp()
{
	App->tex->UnLoad(meta_FOW);
	meta_FOW = nullptr;

	for (std::list<FOW_Entity*>::iterator item = fow_entities.begin(); item != fow_entities.end(); ++item)
	{
		if((*item)!=nullptr)
		delete (*item);
	}

	fow_entities.clear();
	return true;
}

bool FowManager::Load(pugi::xml_node &)
{
	return true;
}

bool FowManager::Save(pugi::xml_node &) const
{
	return true;
}

FOW_Entity * FowManager::CreateFOWEntity(iPoint position, bool provides_visibility)
{
	FOW_Entity* ret = nullptr;

	ret = new FOW_Entity(position, provides_visibility);

	if(ret != nullptr)
	fow_entities.push_back(ret);

	return ret;
}

bool FowManager::DestroyFOWEntity(FOW_Entity* to_destroy)
{
	bool ret = false;

	for (std::list<FOW_Entity*>::iterator item = fow_entities.begin(); item != fow_entities.end(); ++item)
	{
		if (to_destroy == (*item))
		{
			delete (*item);
			fow_entities.remove(*item);
			ret = true;
			break;
		}
	}

	return ret;
}

void FowManager::SetVisibilityMap(uint w, uint h)
{
	if (visibility_map != nullptr)
	{
		if (visibility_debug_holder == visibility_map)
		{
			visibility_debug_holder = nullptr;
		}
		else
		{
			delete[] visibility_debug_holder;
			visibility_debug_holder = nullptr;
		}

		if (visibility_map != debug_map)
		{
			delete[] visibility_map;
			visibility_map = nullptr;
		}
	}

	if (debug_map != nullptr)
	{
		delete[] debug_map;
		debug_map = nullptr;
	}

	width = w;
	height = h;

	//Visibility map will contain the 3 basic states of logic in FOW
	visibility_map = new int8_t [width*height];
	memset(visibility_map, 0,width*height);

	// Keep a totally clear map for debug purposes
	debug_map = new int8_t[width*height];
	memset(debug_map, 1, width*height);
}

int8_t FowManager::GetVisibilityTileAt(const iPoint& pos) const
{
	// Utility: return the visibility value of a tile
	if (CheckBoundaries(pos)) // Since both maps will have the same size we can check with the main one
			return visibility_map[(pos.y * width) + pos.x];
	else
		return 0;
}

SDL_Rect& FowManager::GetFOWMetaRect(FOW_TileState state)
{
	SDL_Rect ret;
	uint real_sprite_pos = uint(state) - uint(FOW_TileState::SHROUDED); // tile inside the FOWmeta spritesheet

	ret.w = 64; // 64 is the width between different tiles in the spritesheet
	ret.h = 64;
	ret.x = real_sprite_pos * ret.w; 
	ret.y = 0;

	return ret;
}

void FowManager::SmoothEdges(FOW_Entity* fow_entity)
{

	std::list<iPoint> prev_frontier = fow_entity->frontier;

	for (std::list<iPoint>::iterator tile = fow_entity->frontier.begin(); tile != fow_entity->frontier.end(); tile++)
	{
		(*tile).x += fow_entity->motion.x;
		(*tile).y += fow_entity->motion.y;
	}

	SmoothEntitiesInnerEdges(fow_entity->frontier);
	
	std::list<iPoint> corners;
	for (std::list<iPoint>::const_iterator item = prev_frontier.cbegin(); item != prev_frontier.cend(); item++)
	{
		//Testing edge smoothing

		// THIS IS HARDCODED, SHOULD USE STATES MIRRORING POSITION IN THE SPRITESHEET
		int index = 0;
		if (GetVisibilityTileAt({ (*item).x, (*item).y - 1 }  ) == int8_t(FOW_TileState::UNVISITED)) //Check ABOVE
			index += 1;

		if (GetVisibilityTileAt({ (*item).x - 1 , (*item).y }  ) == int8_t(FOW_TileState::UNVISITED)) //Check LEFT
			index += 2;

		if (GetVisibilityTileAt({ (*item).x, (*item).y + 1 } ) == int8_t(FOW_TileState::UNVISITED)) //Check DOWN
			index += 4;

		if (GetVisibilityTileAt({ (*item).x + 1, (*item).y } ) == int8_t(FOW_TileState::UNVISITED)) //Check RIGHT
			index += 8;


		if (GetVisibilityTileAt((*item) ) == int8_t(FOW_TileState::SHROUDED))
		{
			switch (index)
			{
			case 0: 
				// A tile in the last frontier will never be a shrouded area, so we can assume it's an outer corner
				corners.push_back(*item);
				break;
			case 1:
				SetVisibilityTile((*item), FOW_TileState::BTOS_SMTH_TOP );
				break;
			case 3:
				SetVisibilityTile((*item), FOW_TileState::BTOS_SMTH_TLEFT_CORNER );
				break;
			case 2:
				SetVisibilityTile((*item), FOW_TileState::BTOS_SMTH_LEFT );
				break;
			case 4:
				SetVisibilityTile((*item), FOW_TileState::BTOS_SMTH_DOWN );
				break;
			case 6:
				SetVisibilityTile((*item), FOW_TileState::BTOS_SMTH_DLEFT_CORNER );
				break;
			case 7:
				SetVisibilityTile((*item), FOW_TileState::BTOS_SMTH_DEAD_END_LEFT );
				break;
			case 8:
				SetVisibilityTile((*item), FOW_TileState::BTOS_SMTH_RIGHT );
				break;
			case 9:
				SetVisibilityTile((*item), FOW_TileState::BTOS_SMTH_TRIGHT_CORNER );
				break;
			case 11:
				SetVisibilityTile((*item), FOW_TileState::BTOS_SMTH_DEAD_END_TOP );
				break;
			case 12:
				SetVisibilityTile((*item), FOW_TileState::BTOS_SMTH_DRIGHT_CORNER );
				break;
			case 13:
				SetVisibilityTile((*item), FOW_TileState::BTOS_SMTH_DEAD_END_RIGHT );
				break;
			case 14:
				SetVisibilityTile((*item), FOW_TileState::BTOS_SMTH_DEAD_END_BOTTOM );
				break;
			}
		}
		else
		{
			/*switch (index)
			{

			case 3:
				SetVisibilityTile((*item), FOW_TileState::BTOS_SMTH_TLEFT_CORNER );
				break;

			case 6:
				SetVisibilityTile((*item), FOW_TileState::BTOS_SMTH_DLEFT_CORNER );
				break;

			case 9:
				SetVisibilityTile((*item), FOW_TileState::BTOS_SMTH_TRIGHT_CORNER );
				break;

			case 12:
				SetVisibilityTile((*item), FOW_TileState::BTOS_SMTH_DRIGHT_CORNER );
				break;
			}*/
		}

		for (std::list<iPoint>::const_iterator item = corners.cbegin(); item != corners.cend(); item++)
		{
			//Since we know these elements where in a frontier, thus they can only be corners, we will check the diagonal tiles
			//since there can only be one tile that is still unvisited, when we find it we will: continue; the loop
			if (GetVisibilityTileAt({ (*item).x - 1, (*item).y - 1 } ) == int8_t(FOW_TileState::UNVISITED)) //Check ABOVE-LEFT
			{
				SetVisibilityTile((*item), FOW_TileState::BTOS_SMTH_TLEFT_OUT_CORNER );
			}
			else if (GetVisibilityTileAt({ (*item).x - 1 , (*item).y +1 } ) == int8_t(FOW_TileState::UNVISITED)) //Check DOWN-LEFT
			{
				SetVisibilityTile((*item), FOW_TileState::BTOS_SMTH_DLEFT_OUT_CORNER );
			}
			else if (GetVisibilityTileAt({ (*item).x + 1, (*item).y + 1 } ) == int8_t(FOW_TileState::UNVISITED)) //Check DOWN-RIGHT
			{
				SetVisibilityTile((*item), FOW_TileState::BTOS_SMTH_DRIGHT_OUT_CORNER );
			}
			else if (GetVisibilityTileAt({ (*item).x + 1, (*item).y -1} ) == int8_t(FOW_TileState::UNVISITED)) //Check ABOVE-RIGHT
			{
				SetVisibilityTile((*item), FOW_TileState::BTOS_SMTH_TRIGHT_OUT_CORNER );
			}
		}
	}
}

void FowManager::SmoothEntitiesInnerEdges(std::list<iPoint> inner_frontier)
{
	std::list<iPoint> inner_corners;
	for (std::list<iPoint>::iterator item = inner_frontier.begin(); item != inner_frontier.end(); item++)
	{
		/*(*item).x += player.motion.x;
		(*item).y += player.motion.y;*/
		//Testing edge smoothing

		// THE ENUM SWITCH IS USED AS A TOOL, SHOULD USE THE NUMBER OF STATES TO MIRROR POSITION IN THE SPRITESHEET
		// If we did this we would only need to typecast index to FOW_TileState 
		int index = 0;
		int8_t st = GetVisibilityTileAt({ (*item).x, (*item).y - 1 } ); //Check ABOVE
		if ( st == int8_t(FOW_TileState::UNVISITED) || st == int8_t(FOW_TileState::SHROUDED) || st >= int8_t(FOW_TileState::BTOS_SMTH_TOP)) 
			index += 1;

		st = GetVisibilityTileAt({ (*item).x - 1 , (*item).y } ); //Check LEFT
		if (st == int8_t(FOW_TileState::UNVISITED) || st == int8_t(FOW_TileState::SHROUDED) || st >= int8_t(FOW_TileState::BTOS_SMTH_TOP)) 
			index += 2;

		st = GetVisibilityTileAt({ (*item).x , (*item).y + 1 } ); //Check DOWN
		if (st == int8_t(FOW_TileState::UNVISITED) || st == int8_t(FOW_TileState::SHROUDED) || st >= int8_t(FOW_TileState::BTOS_SMTH_TOP)) 
			index += 4;

		st = GetVisibilityTileAt({ (*item).x + 1 , (*item).y } ); //Check RIGHT
		if (st == int8_t(FOW_TileState::UNVISITED) || st == int8_t(FOW_TileState::SHROUDED) || st >= int8_t(FOW_TileState::BTOS_SMTH_TOP)) 
			index += 8;

		switch (index)
		{
		case 0:
			// A tile in the last frontier will never be a shrouded area, so we can assume it's an outer corner
			inner_corners.push_back(*item);
			break;
		case 1:
			SetVisibilityTile((*item), FOW_TileState::SHROUD_SMTH_TOP );
			break;
		case 3:
			SetVisibilityTile((*item), FOW_TileState::SHROUD_SMTH_TLEFT_CORNER );
			break;
		case 2:
			SetVisibilityTile((*item), FOW_TileState::SHROUD_SMTH_LEFT );
			break;
		case 4:
			SetVisibilityTile((*item), FOW_TileState::SHROUD_SMTH_DOWN );
			break;
		case 6:
			SetVisibilityTile((*item), FOW_TileState::SHROUD_SMTH_DLEFT_CORNER );
			break;
		case 7:
			SetVisibilityTile((*item), FOW_TileState::SHROUD_SMTH_DEAD_END_LEFT );
			break;
		case 8:
			SetVisibilityTile((*item), FOW_TileState::SHROUD_SMTH_RIGHT );
			break;
		case 9:
			SetVisibilityTile((*item), FOW_TileState::SHROUD_SMTH_TRIGHT_CORNER );
			break;
		case 11:
			SetVisibilityTile((*item), FOW_TileState::SHROUD_SMTH_DEAD_END_TOP );
			break;
		case 12:
			SetVisibilityTile((*item), FOW_TileState::SHROUD_SMTH_DRIGHT_CORNER );
			break;
		case 13:
			SetVisibilityTile((*item), FOW_TileState::SHROUD_SMTH_DEAD_END_RIGHT );
			break;
		case 14:
			SetVisibilityTile((*item), FOW_TileState::SHROUD_SMTH_DEAD_END_BOTTOM );
			break;
		}
	}

	for (std::list<iPoint>::const_iterator item = inner_corners.cbegin(); item != inner_corners.cend(); item++)
	{
		//Since we know these elements where in a frontier, thus they can only be corners, we will check the diagonal tiles
		//since there can only be one tile that is still unvisited, when we find it we will: continue; the loop
		if (GetVisibilityTileAt({ (*item).x - 1, (*item).y - 1 } ) == int8_t(FOW_TileState::UNVISITED)) //Check ABOVE-LEFT
		{
			SetVisibilityTile((*item), FOW_TileState::SHROUD_SMTH_TLEFT_OUT_CORNER );
		}
		else if (GetVisibilityTileAt({ (*item).x - 1 , (*item).y + 1 } ) == int8_t(FOW_TileState::UNVISITED)) //Check DOWN-LEFT
		{
			SetVisibilityTile((*item), FOW_TileState::SHROUD_SMTH_DLEFT_OUT_CORNER );
		}
		else if (GetVisibilityTileAt({ (*item).x + 1, (*item).y + 1 } ) == int8_t(FOW_TileState::UNVISITED)) //Check DOWN-RIGHT
		{
			SetVisibilityTile((*item), FOW_TileState::SHROUD_SMTH_DRIGHT_OUT_CORNER );
		}
		else if (GetVisibilityTileAt({ (*item).x + 1, (*item).y - 1 } ) == int8_t(FOW_TileState::UNVISITED)) //Check ABOVE-RIGHT
		{
			SetVisibilityTile((*item), FOW_TileState::SHROUD_SMTH_TRIGHT_OUT_CORNER );
		}
	}
}

void FowManager::SetVisibilityTile(iPoint pos, FOW_TileState state)
{
	if (CheckBoundaries(pos))
			visibility_map[(pos.y * width) + pos.x] = (int8_t)state;
}

//We will manage the bool is_visible in the entities, so that the entity manager module manages everything else
void FowManager::ManageEntitiesVisibility()
{
	//NEW, correct way to do things
	for (std::list<FOW_Entity*>::iterator item = fow_entities.begin(); item != fow_entities.end(); ++item)
	{
		int8_t st = GetVisibilityTileAt((*item)->position);
		if (st == (int8_t)FOW_TileState::VISIBLE || (st < (int8_t)FOW_TileState::BTOS_SMTH_TOP && st >(int8_t)FOW_TileState::SHROUDED))
		{
			(*item)->is_visible = true;
		} else {
			(*item)->is_visible = false;
		}
	}
}

bool FowManager::TileInsideList(iPoint tile, const std::list<iPoint>& list_checked) const
{
	for (std::list<iPoint>::const_iterator item = list_checked.cbegin(); item != list_checked.end(); item++)
	{
		if ((*item).x == tile.x && (*item).y == tile.y)
		{
			return true;
		}
	}

	return false;
}

void FowManager::ResetVisibilityMap()
{
	// We simply set the map again this way other modules can call and it will be
	// easier to understand rather than setting the the map again manually
	SetVisibilityMap(width,height);

	if (debug)
	{
		debug = false;
	}
}


bool FowManager::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x < (int)width &&
		pos.y >= 0 && pos.y <= (int)height);
}

std::list<iPoint> FowManager::CreateFrontierRect(uint w, uint h, iPoint center)
{
	std::list<iPoint> frontier_to_fill;

	iPoint st_pos = { center.x - (int)(w/2), center.y - (int)(h / 2) };
	iPoint final_pos = { st_pos.x + (int)w, (int)st_pos.y + (int)h};

	// We iterate to find the iPoints that ARE part of the frontier
	for (int j = st_pos.y; j < final_pos.y; ++j)
	{
		int i = st_pos.x;
		for (; i < final_pos.x - 1; ++i) //check the explanations below to understand why i < w -1
		{
			// Since it's a rectangle some assumptions can be made: i== 0 will always be at the 
			// frontier as the left-most tile, i == w -1 will be for the right-most tile
			if (i == st_pos.x || j == st_pos.y || j == final_pos.y -1)
			{
				frontier_to_fill.push_back({ i, j });
			}
		}
		// We exit the for loop when i == w -1
		frontier_to_fill.push_back({ i, j });
	}
	
	return frontier_to_fill;
}

std::list<iPoint> FowManager::CreateFrontierSquare(uint radius, iPoint center)
{
	return CreateFrontierRect(radius, radius, center);
}

std::list<iPoint> FowManager::FillFrontier(const std::list<iPoint>& frontier)
{
	std::list<iPoint> shape_to_fill;

	//Iterate the frontier
	for (std::list<iPoint>::const_iterator curr = frontier.cbegin(); curr != frontier.cend(); ++curr)
	{
		if ((*std::next(curr)).y == (*curr).y)
		{
			int w = (*std::next(curr)).x - (*curr).x;
			for (int i = 0; i < w; ++i)
			{
				shape_to_fill.push_back({ (*curr).x + i ,(*curr).y });
			}
		}
		else
		{
			shape_to_fill.push_back(*curr);
		}
	}

	return shape_to_fill;
}

FOW_Entity::FOW_Entity(iPoint position, bool provides_visibility):
	position(App->map->WorldToMap(position.x, position.y)),
	provides_visibility(provides_visibility)
{}


void FOW_Entity::SetPos(iPoint new_pos)
{
	new_pos = App->map->WorldToMap(new_pos.x, new_pos.y);
	// Our position is always in map reference
	if (position != new_pos)
	{
		moved_in_map = true;
		motion = { (new_pos.x - position.x), (new_pos.y - position.y) };
		position = new_pos;

	}
}
