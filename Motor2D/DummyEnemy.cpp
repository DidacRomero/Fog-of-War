#include "DummyEnemy.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"

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
	return true;
}

bool DummyEnemy::CleanUp()
{
	App->tex->UnLoad(entity_tex);
	return true;
}

void DummyEnemy::Draw()
{
	App->render->Blit(entity_tex, position.x, position.y -64, &entity_rect);
}
