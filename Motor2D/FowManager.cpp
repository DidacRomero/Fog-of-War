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
	meta_FOW = App->tex->Load("maps/meta_FOW.png");

	// Testing getting a frontier
	std::list<iPoint> test = CreateFrontierRect(5,5);

	// Testing Filling a frontier
	std::list<iPoint> LOS = FillFrontier(test);
	return true;
}

bool FowManager::PreUpdate()
{
	return true;
}

bool FowManager::Update(float dt)
{
	ManageEntitiesVisibility();
	UpdateEntitiesPositions();

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		debug = !debug;

		// If we enter debug mode, our visibility map should be clear.
		// We will point to a clear visibility map (debug_map) so all calls depending on visibility_map don't
		// need further management. But before we store 
		if (debug == true)
		{
			//To keep the pointer to the visibility map we use or debug_holder;
			visibility_debug_holder = visibility_map;
			visibility_map = debug_map;
		}
		else // Debug == false
		{
			visibility_map = visibility_debug_holder;
		}
	}


	//Testing player frontier
	player.position.x = (*entities_pos.begin()).x;
	player.position.y = (*entities_pos.begin()).y;

	player.frontier.clear();
	player.frontier = GetRectFrontier(10, 10, { player.position.x, player.position.y });

	for (std::list<iPoint>::const_iterator item = player.frontier.cbegin(); item != player.frontier.cend(); item++)
	{
		SetVisibilityTile((*item), FOW_TileState::VISIBLE);
	}


	for (std::list<iPoint>::const_iterator lf_item = player.last_frontier.cbegin(); lf_item != player.last_frontier.end(); lf_item++)
	{
		if (TileInsideFrontier((*lf_item), player.frontier) == 0)
		{
			SetVisibilityTile((*lf_item), FOW_TileState::SHROUDED);
		} 
	}

	player.last_frontier = player.frontier;

	return true;
}

bool FowManager::PostUpdate()
{
	return true;
}

bool FowManager::CleanUp()
{
	App->tex->UnLoad(meta_FOW);
	meta_FOW = nullptr;
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

	visibility_map = new int8_t [width*height];
	memset(visibility_map, 0,width*height);

	// Keep a totally clear map for debug purposes
	debug_map = new int8_t[width*height];
	memset(debug_map, 1, width*height);
}

int8_t FowManager::GetVisibilityTileAt(const iPoint& pos) const
{
	// Utility: return the visibility value of a tile
	if (CheckBoundaries(pos))
		return visibility_map[(pos.y * width) + pos.x];
	else
		return 0;
}

void FowManager::SetVisibilityTile(iPoint pos, FOW_TileState state)
{
	if (CheckBoundaries(pos))
	visibility_map[(pos.y * width) + pos.x] = (int8_t)state;
}

std::list<iPoint> FowManager::GetRectFrontier(uint w, uint h, iPoint pos)
{
	std::list<iPoint> square;

	iPoint st_point = { int(pos.x - w / 2), int(pos.y - h / 2)};
	iPoint curr = st_point;

	for (int i = 1; i <= w; ++i)
	{
		for (int j = 0; j < h; ++j)
		{
			curr.y = st_point.y + j;
			square.push_back(curr);
		}
		curr.x = st_point.x + i;
	}

	return square;
}

//We will manage the bool is_visible in the entities, so that the entity manager module manages everything else
void FowManager::ManageEntitiesVisibility()
{
	//Get the entities 
	std::list<j2Entity*> entities_info = App->entity_manager->GetEntitiesInfo();

	std::list<iPoint>::const_iterator entity_position = entities_pos.begin();

	for (std::list<j2Entity*>::iterator entity_item = entities_info.begin(); entity_item != entities_info.end(); entity_item++)
	{
		switch (GetVisibilityTileAt((*entity_position)))
		{
		case 0:
			(*entity_item)->is_visible = false;
			break;

		case 1:
			(*entity_item)->is_visible = true;
			break;

		case 2:
			(*entity_item)->is_visible = false;
			break;
		}
		entity_position++;
	}
}

void FowManager::UpdateEntitiesPositions()
{
	// ------
	// Get the position of all entities, we call the entity manager to provide us all the entities (const)
	// When implementing into your game, you will have to do the same thing but adapted into your own entity and entity manager
	std::list<j2Entity*> entities_info = App->entity_manager->GetEntitiesInfo();

	std::list<j2Entity*>::iterator item = entities_info.begin();

	if (entities_pos.size() == 0)
	{
		for (; item != entities_info.end(); ++item)
		{
			entities_pos.push_back(App->map->WorldToMap((*item)->position.x, (*item)->position.y));
		}
	}
	else
	{
		//We iterate the list of positions of entities to update the list of positions we have
		std::list<iPoint>::iterator pos_item = entities_pos.begin();

		for (; item != entities_info.end(); ++item)
		{
			(*pos_item) = App->map->WorldToMap((*item)->position.x, (*item)->position.y);
			pos_item++;
		}
	}
	// ------

}

int8_t FowManager::TileInsideFrontier(iPoint tile, const std::list<iPoint>& frontier_checked) const
{
	for (std::list<iPoint>::const_iterator item = frontier_checked.cbegin(); item != frontier_checked.end(); item++)
	{
		if ((*item).x == tile.x && (*item).y == tile.y)
		{
			return 1;
		}
	}

	return 0;
}

void FowManager::ResetFOWVisibility()
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
	return (pos.x >= 0 && pos.x <= (int)width &&
		pos.y >= 0 && pos.y <= (int)height);
}

std::list<iPoint> FowManager::CreateFrontierRect(uint w, uint h)
{
	std::list<iPoint> frontier_to_fill;

	// We iterate to find the iPoints that ARE part of the frontier
	int j = 0;
	for (; j < h; ++j)
	{
		int i = 0;
		for (; i < w - 1; ++i) //check the explanations below to understand why i < w -1
		{
			// Since it's a rectangle some assumptions can be made: i== 0 will always be at the 
			// frontier as the left-most tile, i == w -1 will be for the right-most tile
			if (i == 0 || j== 0 || j == h -1)
			{
				frontier_to_fill.push_back({ i, j });
			}
		}
		// We exit the for loop when i == w -1
		frontier_to_fill.push_back({ i, j });
	}
	
	return frontier_to_fill;
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
