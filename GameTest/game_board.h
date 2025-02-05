#pragma once
#include "piece.h"

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

class GameBoard
{
private:
	Block* grid[BOARD_WIDTH][BOARD_HEIGHT];
	void GetBlockGridCoordinates(Block* block, int& x, int& y);
	void PositionToGridCoordinates(float px, float py, int& x, int& y);
	bool IsCellFree(int x, int y) const;
	bool IsRowFull(int row) const;
	void ClearRow(int row);
	void DropRows(int clearedRow, int rowCount);
public:
	GameBoard();
	~GameBoard();
	void AdjustPieceStartPosition(Piece* piece);
	void AdjustNextPiecePosition(Piece* piece);
	void AddPieceToGameBoard(Piece* piece);
	bool CanPlacePiece(Piece* piece);
	void MovePiece(Piece* piece, float direction);
	void RotatePiece(Piece* piece, int direction);
	void ClearRows();
	void Draw();
};

