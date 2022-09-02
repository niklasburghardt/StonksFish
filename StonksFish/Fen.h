#include <string>
#include <map>
#include "Piece.h"
class Fen {
public:
	
	std::map<char, int> pieceFromSymbol;
public:
	Fen();
public:
	int* PositionFromFen(std::string fen);
};



class Position {
public:
	int* squares;
	bool whiteCastleKingside;
	bool whiteCastleQueenside;
	bool blackCasteKingside;
	bool blackCastleQueenside;
	int epFile;
	bool whiteToMove;
	int plyCount;
};