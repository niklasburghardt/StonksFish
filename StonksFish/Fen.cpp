#include "Fen.h"
#include <vector>

Fen::Fen() {
	
	pieceFromSymbol = std::map<char, int>(); 
	//white Pieces
	pieceFromSymbol['P'] = Piece::Pawn | Piece::White;
	pieceFromSymbol['N'] = Piece::Knight | Piece::White;
	pieceFromSymbol['B'] = Piece::Bishop | Piece::White;
	pieceFromSymbol['R'] = Piece::Rook | Piece::White;
	pieceFromSymbol['Q'] = Piece::Queen | Piece::White;
	pieceFromSymbol['K'] = Piece::King | Piece::White;
	//black pieces
	pieceFromSymbol['p'] = Piece::Pawn | Piece::Black;
	pieceFromSymbol['n'] = Piece::Knight | Piece::Black;
	pieceFromSymbol['b'] = Piece::Bishop | Piece::Black;
	pieceFromSymbol['r'] = Piece::Rook | Piece::Black;
	pieceFromSymbol['q'] = Piece::Queen | Piece::Black;
	pieceFromSymbol['k'] = Piece::King | Piece::Black;
	
}

Position Fen::PositionFromFen(std::string fen)
{
	Position positionInfo = Position();
	positionInfo.squares = new int[64];

	std::vector<std::string> elements = std::vector<std::string>();
	int lastIndex = 0;
	int len = 0;
	for (int i = 0; i < fen.size(); i++) {
		if (fen[i] == ' ') {
			elements.push_back(fen.substr(lastIndex, len));
			lastIndex = i + 1;
			len = 0;
		}else
			len++;
	}

	int file = 0;
	int rank = 7;
	for (int i = 0; i < elements[0].size(); i++){
		char symbol = elements[0][i];
		if (symbol == '/') {
			file = 0;
			rank--;
		}
		else {
			if (isdigit((int)symbol)) {
				file += symbol - '0';
			}
			else {
				int piece = pieceFromSymbol[symbol];
				positionInfo.squares[rank * 8 + file] = piece;
				file++;
			}
		}
	}
	positionInfo.whiteToMove = elements[1] == "w";
	std::string castleRights = elements[2];
	positionInfo.whiteCastleKingside = castleRights.find("K") != std::string::npos;
	positionInfo.whiteCastleQueenside = castleRights.find("Q") != std::string::npos;
	positionInfo.blackCasteKingside = castleRights.find("k") != std::string::npos;
	positionInfo.blackCastleQueenside = castleRights.find("q") != std::string::npos;
	char ep = elements[3][0];
	if (ep != '-')
		positionInfo.epFile = (int)elements[3][1] - '0';
	
	return positionInfo;
}