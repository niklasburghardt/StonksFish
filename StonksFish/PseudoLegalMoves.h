#pragma once
#include <vector>

#include "BoardRepresentation.h"
#include "Move.h"
#include "Piece.h"

class Board;

class MoveGenerator
{
public:
	MoveGenerator();
private:
	bool inCheck;
	bool inDoubleCheck;
	bool pinsExistInPosition;
	long checkRayBitmask;
	long pinRayBitmask;
public:
	std::vector<PieceMove> moves;
	bool isWhiteToMove;
	int friendlyColor;
	int opponentColor;
	bool attackedSquares[64];
	Board* m_board;
private:
	void Init();
private:
	void CalculateAttackedSquares();
	void GenerateSlidingAttacks(int piece, int index);
	void GeneratePawnAttacks(int index);
	void GenerateKnightAttacks(int index);
	void GenerateKingAttacks(int index);
public:
	std::vector<PieceMove> GenerateLegalMoves(Board* board);
private:
	void GenerateSlidingMoves(int piece, int index);
	void GeneratePawnMoves(int index);
	void GenerateKnightMoves(int index);
	void GenerateKingMoves(int index);

private:
	bool InCheck();
	void MakePawnMove(int startIndex, int targetIndex);//Checking for promotion
	void AddMoveIfLegal(PieceMove move);
};

