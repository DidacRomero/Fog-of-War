#include "DummyEnemy.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "FowManager.h"

DummyEnemy::DummyEnemy() : j2Entity(ENTITY_TYPE::ENEMY)
{
}

DummyEnemy::~DummyEnemy()
{
}

bool DummyEnemy::Start()
{
	entity_rect = {62,4,32,57};
	entity_tex = App->tex->Load("textures/entities_sprites.png");
	
	// TODO 2.1: Create the fow_entity. Essentially we need to tell the FowManager to create a new FOW_Entity*.
	// our FOW_Entity* fow_entity should be filled with the pointer that the 
	// function CreateFOWEntity (in the FowManager ) returns.
	fow_entity = App->fow_manager->CreateFOWEntity(position,false);

	return true;
}

bool DummyEnemy::Update(float dt, bool do_logic)
{
	//If the entity in the fog of war module says we're not visible, we're not
	//If we're visible, congrats!

	// TODO 2.2: Update our variable visible, which determines if we will be drawn or not, must be the same as the 
	// variable is_visible within our fow_entity. The FowManager manages if the fow_entity is visible. Our variable
	// visible is managed by the EntityManager and determines wheter we will be drawn or not.
	visible = fow_entity->is_visible;

	return true;
}

bool DummyEnemy::CleanUp()
{
	App->tex->UnLoad(entity_tex);
	return true;
}

void DummyEnemy::Draw()
{
	App->render->Blit(entity_tex, position.x -16, position.y -64, &entity_rect);
}
