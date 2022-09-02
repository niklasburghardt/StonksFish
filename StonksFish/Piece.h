#pragma once
class Piece
{
public:
	static const int None = 0;
	static const int King = 1;
	static const int Pawn = 2;
	static const int Knight = 3;
	static const int Bishop = 5;
	static const int Rook = 6;
	static const int Queen = 7;


	static const int White = 8;
	static const int Black = 16;

	static const int typeMask = 0b00111;
	static const int blackMask = 0b10000;
	static const int whiteMask = 0b1000;
	static const int colorMask = whiteMask | blackMask;

public:
	static bool IsColor(int piece, int color);
	static int Color(int piece);
	static int PieceType(int piece);
	static bool IsRookOrQueen(int piece);
	static bool IsBishipOrQueen(int piece);
	static bool isSlidingPiece(int piece);
};

