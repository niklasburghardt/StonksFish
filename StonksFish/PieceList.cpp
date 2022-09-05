#include "PieceList.h"

PieceList::PieceList(int maxPieceCount)
{
	occupiedSquares = new int[maxPieceCount];
	map = new int[64];
	numPieces = 0;
}

int PieceList::Count() {
	return numPieces;
}

void PieceList::AddPieceAtSquare(int square)
{
	occupiedSquares[numPieces] = square;
	map[square] = numPieces;
	numPieces++;
}

void PieceList::RemovePieceAtSquare(int square)
{
	int pieceIndex = map[square];
	occupiedSquares[pieceIndex] = occupiedSquares[numPieces - 1];
	map[occupiedSquares[pieceIndex]] = pieceIndex;
	numPieces--;
}

void PieceList::MovePiece(int startSquare, int targetSquare)
{
	int pieceIndex = map[startSquare];
	occupiedSquares[pieceIndex] = targetSquare;
	map[targetSquare] = pieceIndex;
	map[startSquare] = -1;
}

int PieceList::operator[](int index)
{
	return occupiedSquares[index];
}