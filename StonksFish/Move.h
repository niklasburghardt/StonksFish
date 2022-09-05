#pragma once
//Move Flags
#define FLAG_NONE 0
#define FLAG_EP 1
#define FLAG_CASTLE_KING 2
#define FLAG_CASTLE_QUEEN 3
#define FLAG_PROMOTION 4
#define FLAG_DOUBLE_PAWN_PUSH 5
#define FLAG_KING_MOVE 6
//Promotion Types
#define PROMOTION_NONE 0
#define PROMOTION_KNIGHT 3
#define PROMOTION_BISHOP 5
#define PROMOTION_ROOK 6
#define PROMOTION_QUEEN 7
class PieceMove
{
public:
	PieceMove(int start, int target, int flag=FLAG_NONE, int promotion=PROMOTION_QUEEN);
public:
	int startSquare;
	int targetSquare;
	int moveFlag;
	int promotionType;
};

