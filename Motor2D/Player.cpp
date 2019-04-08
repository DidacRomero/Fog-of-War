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

	// Create the fow_entity for the player, we call the function CreateFOWEntity()
	fow_entity = App->fow_manager->CreateFOWEntity(position,true);
	//From now on we should only call fow_entity.setpos()
	fow_entity->frontier = App->fow_manager->CreateFrontierSquare(10, fow_entity->position);
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

	//Each time we move, our player should update the position of the Fog of war entity that serves as refernce
	fow_entity->SetPos(position);
	is_visible = fow_entity->is_visible;

	return true;
}

bool Player::CleanUp()
{
	App->tex->UnLoad(entity_tex);
	return true;
}

void Player::Draw()
{
	App->render->Blit(entity_tex, position.x, position.y -64, &entity_rect);
}
