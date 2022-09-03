#include <string>
#include <map>
#include "Piece.h"
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

class Fen {
public:
	
	std::map<char, int> pieceFromSymbol;
public:
	Fen();
public:
	Position PositionFromFen(std::string fen);
};



