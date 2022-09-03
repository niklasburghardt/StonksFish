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

void Board::MakeMove(PieceMove move)
{
	int startIndex = move.startSquare;
	int targetIndex = move.targetSquare;
	int flag = move.moveFlag;
	//Mark sqaure as EP after double Pawn Push
	if (flag==FLAG_DOUBLE_PAWN_PUSH) {
		epIndex = targetIndex;
	}
	//Reset the EP file if the last move was not a double Pawn Push
	else {
		epIndex = -1;
	}
	//Check if its EP and capture enemy Pawn
	if (flag == FLAG_EP) {
		int enemyPawnIndex = targetIndex + (movingPlayer == Piece::White ? -8 : 8);
		squares[enemyPawnIndex] = Piece::None;
	}
	//Handle Castle
	
	int blackOffset = (movingPlayer == Piece::White ? 0 : 56);
	if (flag == FLAG_CASTLE_KING) {
		squares[WHITE_KC_INDEX + blackOffset] = Piece::None;
		squares[WHITE_KC_INDEX + blackOffset - 2] = Piece::Rook | movingPlayer;
	}
	else if (flag == FLAG_CASTLE_QUEEN) {
		squares[WHITE_QC_INDEX + blackOffset] = Piece::None;
		squares[WHITE_QC_INDEX + blackOffset + 3] = Piece::Rook | movingPlayer;
	}
	//remove castle rights for player
	if (flag == FLAG_CASTLE_KING || flag == FLAG_CASTLE_QUEEN) {
		if (movingPlayer == Piece::White) {
			whiteKingsideCastle = false;
			whiteQueensideCastle = false;
		}
		else {
			blackKingsideCastle = false;
			blackQueensideCastle = false;
		}
	}
	//Check if castle rights are lost
	if (startIndex == WHITE_KC_INDEX + blackOffset || targetIndex == WHITE_KC_INDEX + blackOffset)
		(movingPlayer == Piece::White) ? whiteKingsideCastle = false : blackKingsideCastle = true;
	else if (startIndex == WHITE_QC_INDEX+blackOffset || targetIndex == WHITE_QC_INDEX+blackOffset)
		(movingPlayer == Piece::White) ? whiteQueensideCastle = false : blackQueensideCastle = true;
	
	//If promotion replace pawn with Promotion Piece
	squares[targetIndex] = flag == FLAG_PROMOTION ? (move.promotionType | movingPlayer): squares[startIndex];
	squares[startIndex] = Piece::None;
	movingPlayer = movingPlayer == Piece::White ? Piece::Black : Piece::White;
	moveGenerator->GenerateLegalMoves(this);

	/*if (movingPlayer == Piece::Black) {
		int min = 0;
		int max = moveGenerator->moves.size() -1;
		PieceMove randomMove = moveGenerator->moves[min + (std::rand() % (max - min + 1))];
		MakeMove(randomMove);
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
	whiteQueensideCastle = true;
	whiteKingsideCastle = true;
	blackQueensideCastle = true;
	blackKingsideCastle = true;
	moveGenerator->GenerateLegalMoves(this);
	
}