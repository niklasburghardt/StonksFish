#include "Piece.h"

bool Piece::IsColor(int piece, int color) {
	return (piece & colorMask) == color;
}
int Piece::Color(int piece)
{
	return piece & colorMask;
}
int Piece::PieceType(int piece)
{
	return piece & typeMask;
}
bool Piece::IsRookOrQueen(int piece)
{
	return (piece & 0b110) == 0b110;
}
bool Piece::IsBishipOrQueen(int piece)
{
	return (piece & 0b101) == 0b101;
}
bool Piece::isSlidingPiece(int piece)
{
	return (piece & 0b100) != 0;
}