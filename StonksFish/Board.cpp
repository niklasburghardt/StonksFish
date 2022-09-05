#include "Board.h"
#include "Piece.h"
#include <wx/wx.h>
Board::Board()
{
	Initialize();
	moveGenerator = new MoveGenerator();
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

void Board::Initialize()
{
	allPieces = new PieceList * [16];
	for (int color = 0; color < 2; color++) {
		pawns[color] = PieceList(8);
		knights[color] = PieceList(10);
		bishops[color] = PieceList(10);
		rooks[color] = PieceList(10);
		queens[color] = PieceList(9);
		kingSquares[color] = 0;
	 }
	PieceList emptyList = PieceList(0);
	allPieces[0] = &emptyList;
	allPieces[1] = &emptyList;
	allPieces[2] = &pawns[0];
	allPieces[3] = &knights[0];
	allPieces[4] = &emptyList;
	allPieces[5] = &bishops[0];
	allPieces[6] = &rooks[0];
	allPieces[7] = &queens[0];
	allPieces[8] = &emptyList;
	allPieces[9] = &emptyList;
	allPieces[10] = &pawns[1];
	allPieces[11] = &knights[1];
	allPieces[12] = &emptyList;
	allPieces[13] = &bishops[1];
	allPieces[14] = &rooks[1];
	allPieces[15] = &queens[1];
	

	
}

PieceList Board::GetPieceList(int pieceType, int colorIndex)
{
	return *allPieces[colorIndex * 8 + pieceType];
}

void Board::MakeMove(PieceMove move)
{
	lastMove = &move;
	int startIndex = move.startSquare;
	int targetIndex = move.targetSquare;
	int flag = move.moveFlag;
	int movePiece = squares[startIndex];
	int movePieceType = Piece::PieceType(movePiece);

	int capturedPieceType = Piece::PieceType(squares[targetIndex]);
	//Remove captured Piece from piece list
	if (capturedPieceType != 0 && flag != FLAG_EP) {
		GetPieceList(capturedPieceType, opponentColorIndex).RemovePieceAtSquare(targetIndex);
	}
	//Move moving piece in piece list
	if (movePieceType == Piece::King)
	{
		kingSquares[friendlyColorIndex] = targetIndex;
	}
	else {
		GetPieceList(movePieceType, friendlyColorIndex).MovePiece(startIndex, targetIndex);
	}
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
		pawns[opponentColorIndex].RemovePieceAtSquare(enemyPawnIndex);
	}
	//Handle Castle

	int blackOffset = (movingPlayer == Piece::White ? 0 : 56);
	if (flag == FLAG_CASTLE_KING) {
		squares[WHITE_KC_INDEX + blackOffset] = Piece::None;
		squares[WHITE_KC_INDEX + blackOffset - 2] = Piece::Rook | movingPlayer;
		rooks[friendlyColorIndex].MovePiece(WHITE_KC_INDEX + blackOffset, WHITE_KC_INDEX + blackOffset - 2);
	}
	else if (flag == FLAG_CASTLE_QUEEN) {
		squares[WHITE_QC_INDEX + blackOffset] = Piece::None;
		squares[WHITE_QC_INDEX + blackOffset + 3] = Piece::Rook | movingPlayer;
		rooks[friendlyColorIndex].MovePiece(WHITE_QC_INDEX + blackOffset, WHITE_QC_INDEX + blackOffset + 3);

	}
	//remove castle rights for player
	if (flag == FLAG_CASTLE_KING || flag == FLAG_CASTLE_QUEEN || flag == FLAG_KING_MOVE) {
		if (movingPlayer == Piece::White) {
			whiteKingsideCastle = false;
			whiteQueensideCastle = false;
		}
		else {
			blackKingsideCastle = false;
			blackQueensideCastle = false;
		}
	}
	
	if (flag == FLAG_CASTLE_KING || flag == FLAG_CASTLE_QUEEN) {
		//Check if castle rights are lost
		if (startIndex == WHITE_KC_INDEX || targetIndex == WHITE_KC_INDEX)
			whiteKingsideCastle = false;
		else if (startIndex == WHITE_QC_INDEX || targetIndex == WHITE_QC_INDEX)
			whiteQueensideCastle = false;
		else if (startIndex == BLACK_KC_INDEX || targetIndex == BLACK_KC_INDEX)
			blackKingsideCastle = false;
		else if (startIndex == BLACK_QC_INDEX || targetIndex == BLACK_QC_INDEX)
			blackQueensideCastle = false;
	}
	//update Piece lists
	if (flag == FLAG_KING_MOVE) {
		kingSquares[(movingPlayer / 8) - 1] = targetIndex;
	}
	//default promote type to queen
	
	//Handle Promotions
	if (flag == FLAG_PROMOTION)
	{
		switch (move.promotionType)
		{
		case PROMOTION_QUEEN:
			queens[friendlyColorIndex].AddPieceAtSquare(targetIndex);
			
			break;
		default:
			break;
		}
	}
	squares[targetIndex] = flag == FLAG_PROMOTION ? (move.promotionType | movingPlayer) : squares[startIndex];
	squares[startIndex] = Piece::None;
	movingPlayer = movingPlayer == Piece::White ? Piece::Black : Piece::White;
	friendlyColorIndex = 1 - friendlyColorIndex;
	opponentColorIndex = 1 - opponentColorIndex;
	moveGenerator->GenerateLegalMoves(this);

	/*if (movingPlayer == Piece::Black) {
		int min = 0;
		int max = moveGenerator->moves.size() -1;
		PieceMove randomMove = moveGenerator->moves[min + (std::rand() % (max - min + 1))];
		MakeMove(randomMove);
	}*/
}

void Board::UnmakeMove() {
	
	
}

void Board::LoadStartPosition()
{
	LoadPositionFromFen(startingFen);
	kingSquares[0] = 4;
	kingSquares[1] = 60;
}	

void Board::LoadPositionFromFen(std::string fen)
{
	Position loadedPosition = fenManager->PositionFromFen(fen);
	//already doing it this way for optimization later on (piece lists)
	movingPlayer = loadedPosition.whiteToMove ? Piece::White : Piece::Black;
	friendlyColorIndex = movingPlayer == Piece::White ? 0 : 1;
	opponentColorIndex = movingPlayer == Piece::White ? 1 : 0;
	for (int square = 0; square < 64; square++)
	{
		int piece = loadedPosition.squares[square];
		squares[square] = piece;
		if (Piece::isPiece(piece)) {
			int pieceType = Piece::PieceType(piece);
			int pieceColorIndex = (Piece::IsColor(piece, Piece::White)) ? 0 : 1;
			switch (pieceType)
			{
			case Piece::Pawn:
				pawns[pieceColorIndex].AddPieceAtSquare(square);
				break;
			case Piece::Knight:
				knights[pieceColorIndex].AddPieceAtSquare(square);
				break;
			case Piece::Bishop:
				bishops[pieceColorIndex].AddPieceAtSquare(square);
				break;
			case Piece::Rook:
				rooks[pieceColorIndex].AddPieceAtSquare(square);
				break;
			case Piece::Queen:
				queens[pieceColorIndex].AddPieceAtSquare(square);
				break;
			case Piece::King:
				kingSquares[pieceColorIndex] = square;
			default:
				break;
			}
		}
	}
	PieceList testList = *allPieces[2];
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