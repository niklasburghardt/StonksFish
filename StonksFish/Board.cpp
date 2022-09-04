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
	if (flag == FLAG_DOUBLE_PAWN_PUSH) {
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
	if (startIndex == WHITE_KC_INDEX || targetIndex == WHITE_KC_INDEX)
		whiteKingsideCastle = false;
	else if (startIndex == WHITE_QC_INDEX || targetIndex == WHITE_QC_INDEX)
		whiteQueensideCastle = false;
	else if (startIndex == BLACK_KC_INDEX || targetIndex == BLACK_KC_INDEX)
		blackKingsideCastle = false;
	else if (startIndex == BLACK_QC_INDEX || targetIndex == BLACK_QC_INDEX)
		blackQueensideCastle = false;

	//If promotion replace pawn with Promotion Piece
	squares[targetIndex] = flag == FLAG_PROMOTION ? (move.promotionType | movingPlayer) : squares[startIndex];
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
	Position loadedPosition = fenManager->PositionFromFen(fen);
	//already doing it this way for optimization later on (piece lists)
	for (int square = 0; square < 64; square++)
	{
		int piece = loadedPosition.squares[square];
		squares[square] = piece;
	}
	movingPlayer = loadedPosition.whiteToMove ? Piece::White : Piece::Black;
	whiteKingsideCastle = loadedPosition.whiteCastleKingside;
	whiteQueensideCastle = loadedPosition.whiteCastleQueenside;
	blackKingsideCastle = loadedPosition.blackCasteKingside;
	blackQueensideCastle = loadedPosition.blackCastleQueenside;
	if (loadedPosition.epFile >= 0) {
		int epPlayerOffset = loadedPosition.whiteToMove ? 32 : 24; //player not playing right now (!loadedPOsition.whiteTomove)
		epIndex = epPlayerOffset + loadedPosition.epFile;
	}
	moveGenerator->GenerateLegalMoves(this);

}