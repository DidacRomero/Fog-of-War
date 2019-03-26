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
