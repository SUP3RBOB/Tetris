#pragma once
#include "app\app.h"

#define BLOCK_SIZE 32

class Block
{
private:
	CSimpleSprite* sprite;
public:
	Block(float x = 0.f, float y = 0.f);
	~Block();

	void GetPosition(float& x, float& y);

	void SetPosition(float x, float y);
	void Draw();
};

