#include "Move.h"
PieceMove::PieceMove(int start, int target, bool ep)
{
	startSquare = start;
	targetSquare = target;
	isEP = ep;
}