#pragma once
#include "Piece.h"
class PieceList
{
public:
	PieceList(int maxPieceCount = 16);
public:
	int* occupiedSquares;
	int* map;
	int numPieces;
public:
	int Count();
	void AddPieceAtSquare(int square);
	void RemovePieceAtSquare(int square);
	void MovePiece(int startSquare, int targetSquare);
	int operator[](int index);
};

