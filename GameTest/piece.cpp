#include "stdafx.h"
#include "piece.h"

#define TYPE_COUNT 7

Piece::Piece(PieceType type)
{
    for (int i = 0; i < PIECE_BLOCK_COUNT; i++) {
		blocks[i] = new Block();
	}

	this->type = type;
	rotationIndex = 0;

    rotation.rotations = std::vector<BlockOffsets>();
    SetBlockValues();
}

Piece::~Piece()
{
    /*for (Block* block : blocks) {
        delete block;
    }*/
}

Block* Piece::GetBlock(int index) const
{
    if (index < 0 || index >= PIECE_BLOCK_COUNT) {
        return nullptr;
    }

    return blocks[index];
}

Block* Piece::GetPivotBlock() const
{
    return (*pivotBlock);
}

PieceRotations& Piece::GetCurrentRotation()
{
    return rotation;
}

BlockOffsets& Piece::GetCurrentRotationOffset()
{
    return rotation.rotations[rotationIndex];
}

void Piece::AddRotationIndex(int amount)
{
    rotationIndex += amount;
    
    if (rotationIndex == -1)
    {
        rotationIndex = rotation.rotations.size() - 1;
    }
    else if (rotationIndex >= rotation.rotations.size())
    {
        rotationIndex = 0;
    }
}

void Piece::ApplyGravity()
{
    for (Block* block : blocks) {
        float x, y;
        block->GetPosition(x, y);

        y -= BLOCK_SIZE;
        block->SetPosition(x, y);
    }
}

void Piece::Draw()
{
	for (Block* block : blocks) {
		block->Draw();
	}
}

PieceType Piece::GetRandomType()
{
    return (PieceType)(rand() % TYPE_COUNT);
}

void Piece::SetBlockValues()
{
    PieceRotations rotation;

    switch (type) {
        case PieceO: {
            blocks[0]->SetPosition(4.f, 19.f);
            blocks[1]->SetPosition(5.f, 19.f);
            blocks[2]->SetPosition(4.f, 18.f);
            blocks[3]->SetPosition(5.f, 18.f);
            pivotBlock = &blocks[0];
        }
        break;

        case PieceI: {
            blocks[0]->SetPosition(3.f, 19.f);
            blocks[1]->SetPosition(4.f, 19.f);
            blocks[2]->SetPosition(5.f, 19.f);
            blocks[3]->SetPosition(6.f, 19.f);
            pivotBlock = &blocks[2];

            rotation.rotations.push_back(BlockOffsets({ -BLOCK_SIZE, 0.f }, { -BLOCK_SIZE * 2.f, 0.f }, { BLOCK_SIZE, 0.f }));
            rotation.rotations.push_back(BlockOffsets({ 0.f, BLOCK_SIZE }, { 0.f, -BLOCK_SIZE }, { 0.f, -BLOCK_SIZE * 2.f }));
        }
        break;

        case PieceS: {
            blocks[0]->SetPosition(5.f, 19.f);
            blocks[1]->SetPosition(6.f, 19.f);
            blocks[2]->SetPosition(4.f, 18.f);
            blocks[3]->SetPosition(5.f, 18.f);
            pivotBlock = &blocks[0];

            rotation.rotations.push_back(BlockOffsets({ BLOCK_SIZE, 0.f }, { -BLOCK_SIZE, -BLOCK_SIZE }, { 0.f, -BLOCK_SIZE }));
            rotation.rotations.push_back(BlockOffsets({ BLOCK_SIZE, 0.f }, { 0.f, BLOCK_SIZE }, { BLOCK_SIZE, -BLOCK_SIZE }));
        }
        break;

        case PieceZ: {
            blocks[0]->SetPosition(4.f, 19.f);
            blocks[1]->SetPosition(5.f, 19.f);
            blocks[2]->SetPosition(5.f, 18.f);
            blocks[3]->SetPosition(6.f, 18.f);
            pivotBlock = &blocks[1];

            rotation.rotations.push_back(BlockOffsets({ -BLOCK_SIZE, 0.f }, { 0.f, -BLOCK_SIZE }, { BLOCK_SIZE, -BLOCK_SIZE }));
            rotation.rotations.push_back(BlockOffsets({ 0.f, -BLOCK_SIZE }, { BLOCK_SIZE, 0.f }, { BLOCK_SIZE, BLOCK_SIZE }));
        }
        break;

        case PieceL: {
            blocks[0]->SetPosition(4.f, 19.f);
            blocks[1]->SetPosition(5.f, 19.f);
            blocks[2]->SetPosition(6.f, 19.f);
            blocks[3]->SetPosition(4.f, 18.f);
            pivotBlock = &blocks[1];

            rotation.rotations.push_back(BlockOffsets({ -BLOCK_SIZE, 0.f }, { BLOCK_SIZE, 0.f }, { -BLOCK_SIZE, -BLOCK_SIZE }));
            rotation.rotations.push_back(BlockOffsets({ 0.f, BLOCK_SIZE }, { 0.f, -BLOCK_SIZE }, { -BLOCK_SIZE, BLOCK_SIZE }));
            rotation.rotations.push_back(BlockOffsets({ BLOCK_SIZE, 0.f }, { -BLOCK_SIZE, 0.f }, { BLOCK_SIZE, BLOCK_SIZE }));
            rotation.rotations.push_back(BlockOffsets({ 0.f, BLOCK_SIZE }, { 0.f, -BLOCK_SIZE }, { BLOCK_SIZE, -BLOCK_SIZE }));
        }
        break;

        case PieceJ: {
            blocks[0]->SetPosition(4.f, 19.f);
            blocks[1]->SetPosition(5.f, 19.f);
            blocks[2]->SetPosition(6.f, 19.f);
            blocks[3]->SetPosition(6.f, 18.f);
            pivotBlock = &blocks[1];

            rotation.rotations.push_back(BlockOffsets({ -BLOCK_SIZE, 0.f }, { BLOCK_SIZE, 0.f }, { BLOCK_SIZE, -BLOCK_SIZE }));
            rotation.rotations.push_back(BlockOffsets({ 0.f, -BLOCK_SIZE }, { 0.f, BLOCK_SIZE }, { -BLOCK_SIZE, -BLOCK_SIZE }));
            rotation.rotations.push_back(BlockOffsets({ -BLOCK_SIZE, 0.f }, { BLOCK_SIZE, 0.f }, { -BLOCK_SIZE, BLOCK_SIZE }));
            rotation.rotations.push_back(BlockOffsets({ 0.f, -BLOCK_SIZE }, { 0.f, BLOCK_SIZE }, { BLOCK_SIZE, BLOCK_SIZE }));
        }
        break;

        case PieceT: {
            blocks[0]->SetPosition(4.f, 19.f);
            blocks[1]->SetPosition(5.f, 19.f);
            blocks[2]->SetPosition(6.f, 19.f);
            blocks[3]->SetPosition(5.f, 18.f);
            pivotBlock = &blocks[1];

            rotation.rotations.push_back(BlockOffsets({ -BLOCK_SIZE, 0.f }, { 0.f, -BLOCK_SIZE }, { BLOCK_SIZE, 0.f }));
            rotation.rotations.push_back(BlockOffsets({ 0.f, -BLOCK_SIZE }, { 0.f, BLOCK_SIZE }, { -BLOCK_SIZE, 0.f }));
            rotation.rotations.push_back(BlockOffsets({ -BLOCK_SIZE, 0.f }, { 0.f, BLOCK_SIZE }, { BLOCK_SIZE, 0.f }));
            rotation.rotations.push_back(BlockOffsets({ 0.f, -BLOCK_SIZE }, { 0.f, BLOCK_SIZE }, { BLOCK_SIZE, 0.f }));
        }
        break;
    }

    this->rotation = rotation;
}

BlockOffsets::BlockOffsets(std::pair<float, float> offset1, std::pair<float, float> offset2, std::pair<float, float> offset3)
{
    offsets[0] = offset1;
    offsets[1] = offset2;
    offsets[2] = offset3;
}
