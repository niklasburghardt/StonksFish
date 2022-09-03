#pragma once
#include <vector>
class PrecompiledMoveData
{
public:
	
	static int* directionalOffsets;
	static int** squaresToEdge;
	static int** knightMoves;

public:
	static void Init();
private:
	static void CalculateSquaresToEdge();
	static void CalculateKnightMoves();
	static bool isAtEdge(int index, int direction);

};

