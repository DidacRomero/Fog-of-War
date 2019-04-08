#include "Player.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "FowManager.h"

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

	// TODO 2.3: Exactly as we did in TODO 2.1, create the fow_entity for the player, keep in mind that
	// this entity PROVIDES VISIBILITY!!!
	fow_entity = App->fow_manager->CreateFOWEntity(position, true);

	// TODO 3.1: Create 5 tile radius Square frontier, (function named CreateFrontierSquare, inside FowManager) and 
	// assign it to our fow_entity->frontier. 
	// Once you have a frontier, fill our Line of Sight Area, fow_entity->LOS, by assigning to it the list
	// returned by FillFrontier, this function receives a frontier, and returns all tiles inside it including the frontier tiles
	fow_entity->frontier = App->fow_manager->CreateFrontierSquare(5,fow_entity->position);
	fow_entity->LOS = App->fow_manager->FillFrontier(fow_entity->frontier);

	return true;
}

bool Player::Update(float dt, bool do_logic)
{
	iPoint last_pos = position;

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		position.y -= 1;

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		position.y += 1;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		position.x -= 1;

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		position.x += 1;

	App->render->FollowPlayer(position.x - last_pos.x, position.y - last_pos.y);

	// TODO 2.4: Exactly as TODO 2.2 Update our variable visible, which determines if we will be drawn or not, must be the same as the 
	// variable is_visible within our fow_entity. The FowManager manages if the fow_entity is visible. Our variable
	// visible is managed by the EntityManager and determines wheter we will be drawn or not.
	visible = fow_entity->is_visible;

	// TODO 3.2: Update the fow_entity position, use fow_entity function to SetPos
	//Our player should update the position of the Fog of war entity that serves as refernce
	fow_entity->SetPos(position);
	

	return true;
}

bool Player::CleanUp()
{
	App->tex->UnLoad(entity_tex);
	return true;
}

void Player::Draw()
{
	App->render->Blit(entity_tex, position.x - 16, position.y -64, &entity_rect);
}
