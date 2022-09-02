#include "Board.h"
#include "Piece.h"
#include <wx/wx.h>
Board::Board()
{
	startingFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	fenManager = new Fen();
	movingPlayer = Piece::White;
	squares = new int[64];
	for (int i = 0; i < 64; i++) {
		squares[i] = Piece::None;
	}
	LoadStartPosition();
}
Board::~Board()
{
	delete squares;
	delete fenManager;
}

void Board::MakeMove(int startIndex, int targetIndex)
{
	squares[targetIndex] = squares[startIndex];
	squares[startIndex] = Piece::None;
	movingPlayer = movingPlayer == Piece::White ? Piece::Black : Piece::White;
}

void Board::LoadStartPosition()
{
	LoadPositionFromFen(startingFen);
}

void Board::LoadPositionFromFen(std::string fen)
{
	int* loadedPosition = fenManager->PositionFromFen(fen);
	//already doing it this way for optimization later on (piece lists)
	for (int square = 0; square < 64; square++)
	{
		int piece = loadedPosition[square];
		squares[square] = piece;
	}
	
}