#pragma once
#include "Fen.h"
#include <string>
#include "PseudoLegalMoves.h"
#include "Move.h"
#include <stack>
#include "PieceList.h"

//Indicies that cause loosing of castle rights
#define WHITE_KC_INDEX 7
#define WHITE_QC_INDEX 0
#define BLACK_KC_INDEX 63
#define BLACK_QC_INDEX 56


class Board
{
public:
	Board();
	~Board();
public:
	std::string startingFen;
public:
	//Contains pieces like: Piece::Pawn | Piece::Black -> int
	int* squares;
	int movingPlayer;
	int friendlyColorIndex;
	int opponentColorIndex;
	Fen* fenManager;
//Position for single pieces for optimization
public:
	int kingSquares[2];
	PieceList pawns[2];
	PieceList knights[2];
	PieceList bishops[2];
	PieceList rooks[2];
	PieceList queens[2];
	PieceList** allPieces;
	void Initialize();
	PieceList GetPieceList(int pieceType, int colorIndex);
//Caste Rights
public:
	bool whiteQueensideCastle;
	bool whiteKingsideCastle;
	bool blackQueensideCastle;
	bool blackKingsideCastle;

	//0-3 casteling legality
	//4-7 file of ep square (starting at 1 | 0 is no ep)
	//8-13 captured piece
	//14- ..fifty move counter
	int gameState;
	std::stack<int> gameStack;

public:
	MoveGenerator* moveGenerator;
public:
	int epIndex;
private:
	PieceMove* lastMove;
	
public:
	void MakeMove(PieceMove move);
	void UnmakeMove();
	void LoadStartPosition();
	void LoadPositionFromFen(std::string fen);
};

