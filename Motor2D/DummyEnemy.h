#ifndef __DUMMYENEMY_H__
#define __DUMMYENEMY_H__

#include "j2Entity.h"

class DummyEnemy : public j2Entity
{
public:
	DummyEnemy();
	~DummyEnemy();

	bool Start();

	bool CleanUp();

	void Draw();


};

#endif
