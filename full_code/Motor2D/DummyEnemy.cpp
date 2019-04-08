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
	fow_entity = App->fow_manager->CreateFOWEntity(position,false);
	return true;
}

bool DummyEnemy::Update(float dt, bool do_logic)
{
	//If the entity in the fog of war module says we're not visible, we're not
	//If we're visible, congrats!
	is_visible = fow_entity->is_visible;

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
