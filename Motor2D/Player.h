#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "j2Entity.h"

class Player : public j2Entity
{
public:
	Player();
	~Player();

	bool Start();

	bool CleanUp();

	void Draw();


};

#endif
