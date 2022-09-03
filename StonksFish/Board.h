#pragma once
#include "Fen.h"
#include <string>
#include "PseudoLegalMoves.h"
#include "Move.h"

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
	Fen* fenManager;
//Caste Rights
public:
	bool whiteQueensideCastle;
	bool whiteKingsideCastle;
	bool blackQueensideCastle;
	bool blackKingsideCastle;
public:
	PseudoLegalMoves* moveGenerator;
public:
	int epIndex;
public:
	void MakeMove(PieceMove move);
	void LoadStartPosition();
	void LoadPositionFromFen(std::string fen);
};

