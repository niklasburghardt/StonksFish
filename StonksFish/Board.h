#pragma once
#include "Fen.h"
#include <string>
#include "PseudoLegalMoves.h"
class Board
{
public:
	Board();
	~Board();
public:
	std::string startingFen;
public:
	//Contains pieces like: Piece::Pawn | Piece::Black -> int
	int* squares;
	int movingPlayer;
	Fen* fenManager;
public:
	PseudoLegalMoves* moveGenerator;
public:
	int epIndex;
public:
	void MakeMove(int startIndex, int targetIndex);
	void LoadStartPosition();
	void LoadPositionFromFen(std::string fen);
};

