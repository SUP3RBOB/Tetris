///////////////////////////////////////////////////////////////////////////////
// Filename: GameTest.cpp
// Provides a demo of how to use the API
///////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>
#include <time.h>
//------------------------------------------------------------------------
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// Example data....
//------------------------------------------------------------------------
//------------------------------------------------------------------------
float dropTimer = 0.f;
float gameSpeed = 100.f;
GameBoard* gameBoard;
Piece* currentPiece;
PieceType nextPiece;
Piece* nextPieceDisplay;

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	srand(time(NULL));

	gameBoard = new GameBoard();
	currentPiece = new Piece(Piece::GetRandomType());
	gameBoard->AdjustPieceStartPosition(currentPiece);

	nextPiece = Piece::GetRandomType();
	nextPieceDisplay = new Piece(nextPiece);
	gameBoard->AdjustNextPiecePosition(nextPieceDisplay);
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(const float deltaTime)
{
	//------------------------------------------------------------------------
	// Example Sprite Code....
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, true))
	{
		gameBoard->MovePiece(currentPiece, -1.f);
	}

	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, true))
	{
		gameBoard->MovePiece(currentPiece, 1.f);
	}

	if (App::GetController().CheckButton(XINPUT_GAMEPAD_A, true))
	{
		gameBoard->RotatePiece(currentPiece, 1);
	}

	if (App::GetController().CheckButton(XINPUT_GAMEPAD_B, true))
	{
		gameBoard->RotatePiece(currentPiece, -1);
	}
	
	dropTimer += deltaTime;
	if (dropTimer >= gameSpeed)
	{
		if (gameBoard->CanPlacePiece(currentPiece))
		{
			gameBoard->AddPieceToGameBoard(currentPiece);
			gameBoard->ClearRows();

			currentPiece = new Piece(nextPiece);
			gameBoard->AdjustPieceStartPosition(currentPiece);

			nextPiece = Piece::GetRandomType();
			delete nextPieceDisplay;
			nextPieceDisplay = new Piece(nextPiece);
			gameBoard->AdjustNextPiecePosition(nextPieceDisplay);
		}
		else
		{
			currentPiece->ApplyGravity();
		}

		dropTimer = 0.f;
	}
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{	
	currentPiece->Draw();
	nextPieceDisplay->Draw();
	gameBoard->Draw();
}
//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{	
	//------------------------------------------------------------------------
	// Example Sprite Code....
	
	//------------------------------------------------------------------------
	for (int i = 0; i < PIECE_BLOCK_COUNT; i++) {
		delete currentPiece->GetBlock(i);
		delete nextPieceDisplay->GetBlock(i);
	}

	delete currentPiece;
	delete gameBoard;
}