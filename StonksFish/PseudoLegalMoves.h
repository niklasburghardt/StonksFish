#pragma once
#include <vector>

#include "BoardRepresentation.h"
#include "Move.h"
#include "Piece.h"

class Board;

class PseudoLegalMoves
{
public:
	PseudoLegalMoves();
public:
	std::vector<PieceMove> moves;
	bool isWhiteToMove;
	int friendlyColor;
	int opponentColor;

	Board* m_board;
public:
	std::vector<PieceMove> GenerateLegalMoves(Board* board);
	void GenerateSlidingMoves(int piece, int index);
	void GeneratePawnMoves(int index);
	void GenerateKnightMoves(int index);
	void GenerateKingMoves(int index);
private:
	void MakePawnMove(int startIndex, int targetIndex);//Checking for promotion
};

