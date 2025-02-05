#include "stdafx.h"
#include "game_board.h"
#include <vector>

#define XOFFSET 128
#define YOFFSET 32
#define VERTICAL_LENGTH 640
#define HORIZONTAL_LENGTH 320
#define CELL_SIZE 32

void GameBoard::AdjustPieceStartPosition(Piece* piece)
{
    for (int i = 0; i < PIECE_BLOCK_COUNT; i++)
    {
        Block* block = piece->GetBlock(i);
        
        float x, y;
        block->GetPosition(x, y);

        float newX = XOFFSET + (x * CELL_SIZE) + (BLOCK_SIZE / 2.f);
        float newY = YOFFSET + (y * CELL_SIZE) + (BLOCK_SIZE / 2.f);
        block->SetPosition(newX, newY);
    }
}

void GameBoard::AdjustNextPiecePosition(Piece* piece)
{
    for (int i = 0; i < PIECE_BLOCK_COUNT; i++)
    {
        Block* block = piece->GetBlock(i);

        float x, y;
        block->GetPosition(x, y);

        float newX = XOFFSET + (x * CELL_SIZE) + (BLOCK_SIZE / 2.f) + HORIZONTAL_LENGTH - (CELL_SIZE * 2.f);
        float newY = YOFFSET + (y * CELL_SIZE) + (BLOCK_SIZE / 2.f);
        block->SetPosition(newX, newY);
    }
}

void GameBoard::AddPieceToGameBoard(Piece* piece)
{
    for (int i = 0; i < PIECE_BLOCK_COUNT; i++)
    {
        Block* block = piece->GetBlock(i);
        int x, y;
        GetBlockGridCoordinates(block, x, y);
        grid[x][y] = block;
    }

    delete piece;
}

bool GameBoard::CanPlacePiece(Piece* piece)
{
    for (int i = 0; i < PIECE_BLOCK_COUNT; i++)
    {
        Block* block = piece->GetBlock(i);
        int x, y;
        GetBlockGridCoordinates(block, x, y);

        if (y == 0)
        {
            return true;
        }

        if (grid[x][y - 1] != nullptr)
        {
            return true;
        }
    }

    return false;
}

void GameBoard::MovePiece(Piece* piece, float direction)
{
    for (int i = 0; i < PIECE_BLOCK_COUNT; i++)
    {
        Block* block = piece->GetBlock(i);

        int x, y;
        GetBlockGridCoordinates(block, x, y);

        float checkOffset = copysignf(1.f, direction);

        if (x == 0 && checkOffset == -1.f)
        {
            return;
        }

        if (x == BOARD_WIDTH - 1 && checkOffset == 1.f)
        {
            return;
        }

        if (!IsCellFree(x + checkOffset, y))
        {
            return;
        }
    }

    for (int i = 0; i < PIECE_BLOCK_COUNT; i++)
    {
        Block* block = piece->GetBlock(i);

        float x, y;
        block->GetPosition(x, y);
        x += (direction * CELL_SIZE);
        block->SetPosition(x, y);
    }
}

void GameBoard::RotatePiece(Piece* piece, int direction)
{
    if (piece->GetCurrentRotation().rotations.size() <= 0) {
        return;
    }
    
    piece->AddRotationIndex(direction);

    int offsetIndex = 0;

    for (int i = 0; i < PIECE_BLOCK_COUNT; i++) {
        Block* block = piece->GetBlock(i);

        if (block == piece->GetPivotBlock()) {
            continue;
        }

        BlockOffsets& blockOffsets = piece->GetCurrentRotationOffset();
        std::pair<float, float>& offset = blockOffsets.offsets[offsetIndex];

        float px, py;
        piece->GetPivotBlock()->GetPosition(px, py);
        float newX = px + offset.first;
        float newY = py + offset.second;

        int x, y;
        PositionToGridCoordinates(newX, newY, x, y);

        if (!IsCellFree(x, y)) {
            return;
        }

        offsetIndex++;
    }

    offsetIndex = 0;

    for (int i = 0; i < PIECE_BLOCK_COUNT; i++) {
        Block* block = piece->GetBlock(i);
        
        if (block == piece->GetPivotBlock()) {
            continue;
        }

        BlockOffsets& blockOffsets = piece->GetCurrentRotationOffset();
        std::pair<float, float>& offset = blockOffsets.offsets[offsetIndex];
        
        float px, py;
        piece->GetPivotBlock()->GetPosition(px, py);
        float newX = px + offset.first;
        float newY = py + offset.second;

        block->SetPosition(newX, newY);
        offsetIndex++;
    }
}

void GameBoard::ClearRows()
{
    std::vector<int> clearedRows = std::vector<int>();
    std::vector<int> dropSizes = std::vector<int>();

    int prevClearedRow = BOARD_HEIGHT - 1;
    for (int y = BOARD_HEIGHT - 1; y >= 0; y--)
    {
        if (IsRowFull(y))
        {
            ClearRow(y);
            
            if (prevClearedRow - y == 1)
            {
                if (!dropSizes.empty() && !clearedRows.empty()) {
                    clearedRows[clearedRows.size() - 1] = y;
                    dropSizes[dropSizes.size() - 1]++;
                }
            }
            else
            {
                clearedRows.push_back(y);
                dropSizes.push_back(1);
            }

            prevClearedRow = y;
        }
    }

    for (int i = 0; i < clearedRows.size(); i++) {
        DropRows(clearedRows[i], dropSizes[i]);
    }
}

void GameBoard::GetBlockGridCoordinates(Block* block, int& x, int& y)
{
    float bx, by;
    block->GetPosition(bx, by);

    PositionToGridCoordinates(bx, by, x, y);
}

void GameBoard::PositionToGridCoordinates(float px, float py, int& x, int& y)
{
    x = (px - XOFFSET - (BLOCK_SIZE / 2.f)) / CELL_SIZE;
    y = (py - YOFFSET - (BLOCK_SIZE / 2.f)) / CELL_SIZE;
}

bool GameBoard::IsCellFree(int x, int y) const
{
    if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT)
    {
        return false;
    }

    return grid[x][y] == nullptr;
}

bool GameBoard::IsRowFull(int row) const
{
    if (row < 0 || row >= BOARD_HEIGHT)
    {
        return false;
    }

    for (int x = 0; x < BOARD_WIDTH; x++)
    {
        if (grid[x][row] == nullptr)
        {
            return false;
        }
    }

    return true;
}

void GameBoard::ClearRow(int row)
{
    for (int x = 0; x < BOARD_WIDTH; x++)
    {
        delete grid[x][row];
        grid[x][row] = nullptr;
    }
}

void GameBoard::DropRows(int clearedRow, int rowCount)
{
    for (int y = clearedRow; y < BOARD_HEIGHT - 1; y++)
    {
        for (int x = 0; x < BOARD_WIDTH; x++)
        {
            if (y + rowCount >= BOARD_HEIGHT) {
                return;
            }

            grid[x][y] = grid[x][y + rowCount];
            
            Block* block = grid[x][y];
            if (block != nullptr)
            {
                float bx, by;
                grid[x][y]->GetPosition(bx, by);
                by -= CELL_SIZE * rowCount;
                grid[x][y]->SetPosition(bx, by);
            }

            grid[x][y + rowCount] = nullptr;
        }
    }
}

GameBoard::GameBoard()
{
    for (int x = 0; x < BOARD_WIDTH; x++)
    {
        for (int y = 0; y < BOARD_HEIGHT; y++)
        {
            grid[x][y] = nullptr;
        }
    }
}

GameBoard::~GameBoard()
{
    for (int x = 0; x < BOARD_WIDTH; x++)
    {
        for (int y = 0; y < BOARD_HEIGHT; y++)
        {
            if (grid[x][y] != nullptr) {
                delete grid[x][y];
            }
        }
    }
}

void GameBoard::Draw()
{
    for (int x = 0; x < BOARD_WIDTH; x++)
    {
        App::DrawLine(XOFFSET + (x * CELL_SIZE), YOFFSET, XOFFSET + (x * CELL_SIZE), YOFFSET + VERTICAL_LENGTH);

        for (int y = 0; y < BOARD_HEIGHT; y++)
        {
            App::DrawLine(XOFFSET, YOFFSET + (y * CELL_SIZE), XOFFSET + HORIZONTAL_LENGTH, YOFFSET + (y * CELL_SIZE));
            
            if (grid[x][y] != nullptr) {
                grid[x][y]->Draw();
            }
        }
    }

    App::DrawLine(XOFFSET, YOFFSET + VERTICAL_LENGTH, XOFFSET + HORIZONTAL_LENGTH, YOFFSET + VERTICAL_LENGTH);
    App::DrawLine(XOFFSET + HORIZONTAL_LENGTH, YOFFSET, XOFFSET + HORIZONTAL_LENGTH, YOFFSET + VERTICAL_LENGTH);
}
