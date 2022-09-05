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
bool Piece::isPiece(int piece) {
	return piece > 8 && piece < 24;
}
int Piece::Color(int piece)
{
	return piece & colorMask;
}
int Piece::PieceType(int piece)
{
	if (piece < 1)
		return 0;
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