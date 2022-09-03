#include "Move.h"
PieceMove::PieceMove(int start, int target, int flag, int promotion)
{
	startSquare = start;
	targetSquare = target;
	moveFlag = flag;
	promotionType = promotion;
	
}