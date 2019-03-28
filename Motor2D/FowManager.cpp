#include "FowManager.h"

FowManager::FowManager()
{
}

FowManager::~FowManager()
{
}

bool FowManager::Awake()
{
	return true;
}

bool FowManager::Start()
{
	return true;
}

bool FowManager::PreUpdate()
{
	return true;
}

bool FowManager::Update(float dt)
{
	for (int i = 20; i < 40; ++i)
	{
		for (int j = 20; j < 30; ++j)
		{
			SetVisibilityTile({i,j}, 1);
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
	}
	width = w;
	height = h;

	visibility_map = new int8_t [width*height];
	memset(visibility_map, 0,width*height);
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
