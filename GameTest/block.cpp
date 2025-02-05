#include "stdafx.h"
#include "Block.h"

Block::Block(float x, float y)
{
	sprite = App::CreateSprite(".\\Sprites\\block.bmp", 1, 1);
	sprite->SetPosition(x, y);
	sprite->SetScale(1.f);
}

Block::~Block()
{
	delete sprite;
}

void Block::GetPosition(float& x, float& y)
{
	sprite->GetPosition(x, y);
}

void Block::SetPosition(float x, float y)
{
	sprite->SetPosition(x, y);
}

void Block::Draw()
{
	sprite->Draw();
}
