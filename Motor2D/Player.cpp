#include "Player.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"

Player::Player() : j2Entity(ENTITY_TYPE::PLAYER)
{
}

Player::~Player()
{
}

bool Player::Start()
{
	entity_rect = { 13,4,32,57 };
	entity_tex = App->tex->Load("textures/entities_sprites.png");
	return true;
}

bool Player::CleanUp()
{
	App->tex->UnLoad(entity_tex);
	return true;
}

void Player::Draw()
{
	App->render->Blit(entity_tex, position.x, position.y, &entity_rect);
}
