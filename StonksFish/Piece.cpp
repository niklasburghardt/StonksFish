#include "Piece.h"

bool Piece::IsColor(int piece, int color) {
	if (piece == 0)
		return false;
	if (color == Piece::White) {
		return piece < 16 && piece > 8;
	}
	else {
		return piece > 16 && piece <24;
	}
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