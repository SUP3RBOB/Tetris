#pragma once
#include "block.h"
#include <vector>

#define PIECE_BLOCK_COUNT 4

class BlockOffsets {
public:
    BlockOffsets(std::pair<float, float> offset1, std::pair<float, float> offset2, std::pair<float, float> offset3);
    std::pair<float, float> offsets[3];
};

class PieceRotations {
public:
    std::vector<BlockOffsets> rotations;
};

typedef enum {
    PieceO,
    PieceI,
    PieceS,
    PieceZ,
    PieceL,
    PieceJ,
    PieceT
} PieceType;

class Piece
{
private:
    Block* blocks[PIECE_BLOCK_COUNT];
    PieceType type;
    Block** pivotBlock;
    int rotationIndex;
    PieceRotations rotation;

    void SetBlockValues();
public:
    Piece(PieceType type);
    ~Piece();
    Block* GetBlock(int index) const;
    Block* GetPivotBlock() const;
    PieceRotations& GetCurrentRotation();
    BlockOffsets& GetCurrentRotationOffset();

    void AddRotationIndex(int amount);
    void ApplyGravity();

    void Draw();

    static PieceType GetRandomType();
};