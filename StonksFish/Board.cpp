#include "Board.h"
#include "Piece.h"
#include <wx/wx.h>
Board::Board()
{
	moveGenerator = new PseudoLegalMoves();
	startingFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	fenManager = new Fen();
	movingPlayer = Piece::White;
	squares = new int[64];
	epIndex = -1;
	for (int i = 0; i < 64; i++) {
		squares[i] = Piece::None;
	}
	LoadStartPosition();
}
Board::~Board()
{
	delete squares;
	delete fenManager;
	delete moveGenerator;
}

void Board::MakeMove(int startIndex, int targetIndex)
{
	if (squares[startIndex] == (Piece::Pawn | movingPlayer) && targetIndex - startIndex == (movingPlayer == Piece::White ? 16 : -16)) {
		epIndex = targetIndex;
	}
	squares[targetIndex] = squares[startIndex];
	squares[startIndex] = Piece::None;
	movingPlayer = movingPlayer == Piece::White ? Piece::Black : Piece::White;
	moveGenerator->GenerateLegalMoves(this);

	/*if (movingPlayer == Piece::Black) {
		int min = 0;
		int max = moveGenerator->moves.size() -1;
		PieceMove randomMove = moveGenerator->moves[min + (std::rand() % (max - min + 1))];
		MakeMove(randomMove.startSquare, randomMove.targetSquare);
	}*/
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
	moveGenerator->GenerateLegalMoves(this);
	
}