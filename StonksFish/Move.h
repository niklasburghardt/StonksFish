#pragma once
class PieceMove
{
public:
	PieceMove(int start, int target, bool ep = false);
public:
	int startSquare;
	int targetSquare;
	bool isEP;
};

