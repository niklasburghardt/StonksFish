#include "PrecompiledMoveData.h"
#include "BoardRepresentation.h"

int* PrecompiledMoveData::directionalOffsets;
int** PrecompiledMoveData::squaresToEdge;
int** PrecompiledMoveData::knightMoves;


void PrecompiledMoveData::Init()
{
	//Directions -------- N---S--W---O 
	directionalOffsets = new int[8]{8, -8, -1, 1, 7, 9, -7, -9 };
	squaresToEdge = new int*[64];
	knightMoves = new int*[64];
	for (int i = 0; i < 64; i++)
	{
		squaresToEdge[i] = new int[8];
		knightMoves[i] = new int[8];
	}
	
	CalculateSquaresToEdge();
	CalculateKnightMoves();
}

void PrecompiledMoveData::CalculateSquaresToEdge()
{
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			int direction = directionalOffsets[j];
			int count = 0;
			int moved = i;
			while (!isAtEdge(moved, direction))
			{
				count++;
				moved += direction;
			}
			squaresToEdge[i][direction] = count;

		}
	}
}

void PrecompiledMoveData::CalculateKnightMoves()
{
	//Like a watch: ->->    1    2   4   5    7    8   11 12
	int knightOffsets[8] = {17, 10, -6, -15, -17, -10, 6, 15};
	for (int i = 0; i < 64; i++) {
		for (int off = 0; off < 8; off++) {
			int targetIndex = i + knightOffsets[off];
			if (targetIndex >= 0 && targetIndex < 64) {
				int xMove = std::abs(BoardRepresentation::xFromIndex(i) - BoardRepresentation::xFromIndex(targetIndex));
				int yMove = std::abs(BoardRepresentation::yFromIndex(i) - BoardRepresentation::yFromIndex(targetIndex));
				if (std::max(xMove, yMove) == 2) {
					knightMoves[i][off] = targetIndex;
				}
			}
		}
	}
	
}

bool PrecompiledMoveData::isAtEdge(int index, int direction)
{
	switch (direction)
	{
	case 8: //N
		return index + 8 > 63;
		break;
	case -8://S
		return index - 8 < 0;
		break;
	case -1://W
		return index % 8 == 0;
		break;
	case 1://E
		return index % 8 == 7;
		break;
	case 7://NW
		return isAtEdge(index, -1) || isAtEdge(index, 8);
		break;
	case 9: //NE
		return isAtEdge(index, 1) || isAtEdge(index, 8);
		break;
	case -7://SE
		return isAtEdge(index, 1) || isAtEdge(index, -8);
		break;
	case -9://SW
		return isAtEdge(index, -1) || isAtEdge(index, -8);
		break;
	}
}