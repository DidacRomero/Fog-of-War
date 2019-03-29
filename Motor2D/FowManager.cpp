#include "FowManager.h"
#include "j1App.h"
#include "j2EntityManager.h"
#include "j2Entity.h"
#include "j1Map.h"
#include "j1Input.h"

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
	//Testing GetRectFrontier
	std::list<iPoint> turn_visible = GetRectFrontier(20, 20, { 25,25 });

	int i = 0;
	for (std::list<iPoint>::const_iterator item = turn_visible.cbegin(); item != turn_visible.end(); item++)
	{
		SetVisibilityTile((*item), 1);
		i++;
	}

	// ------
	// Get the position of all entities, we call the entity manager to provide us all the entities (const)
	// When implementing into your game, you will have to do the same thing but adapted into your own entity and entity manager
	std::list<j2Entity*> entities_info = App->entity_manager->GetEntitiesInfo();

	std::list<j2Entity*>::iterator item = entities_info.begin();

	for (; item != entities_info.end(); ++item)
	{
		entities_pos.push_back(App->map->WorldToMap((*item)->position.x, (*item)->position.y));
	}
	// ------


	return true;
}

bool FowManager::PreUpdate()
{
	return true;
}

bool FowManager::Update(float dt)
{
	ManageEntitiesVisibility();

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

	return true;
}

bool FowManager::PostUpdate()
{
	return true;
}

bool FowManager::CleanUp()
{
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
		delete[] visibility_map;
		visibility_debug_holder = nullptr;
	}
	
	if (debug_map != nullptr)
		delete[] debug_map;

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
	// Utility: return the walkability value of a tile
	if ((pos.y * width) + pos.x < width*height)
		return visibility_map[(pos.y * width) + pos.x];
	else
		return 0;
}

void FowManager::SetVisibilityTile(iPoint pos, int8_t value)
{
	if ((pos.y * width) + pos.x < width*height)
	visibility_map[(pos.y * width) + pos.x] = value;
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
		}
		entity_position++;
	}
}
