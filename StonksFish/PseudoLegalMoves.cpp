#include "PseudoLegalMoves.h"
#include "PrecompiledMoveData.h"
#include "Board.h"

MoveGenerator::MoveGenerator()
{
	moves = std::vector<PieceMove>();

}

std::vector<PieceMove> MoveGenerator::GenerateLegalMoves(Board* board)
{	
	m_board = board;
	//clear data
	Init();

	CalculateAttackedSquares();
	for (int i = 0; i < 64; i++)
	{
		int piece = board->squares[i];
		if (Piece::IsColor(piece, friendlyColor)) {
			if (Piece::isSlidingPiece(piece)) {
				GenerateSlidingMoves(piece, i);
			}
			else if (piece == (Piece::Pawn | friendlyColor)) {
				GeneratePawnMoves(i);
			}
			else if (piece == (Piece::King | friendlyColor)) {
				GenerateKingMoves(i);
			}
			else if (piece == (Piece::Knight | friendlyColor)) {
				GenerateKnightMoves(i);
			}
		}
	}

	return moves;
}

void MoveGenerator::Init()
{
	moves.clear();
	inCheck = false;
	inDoubleCheck = false;
	pinsExistInPosition = false;
	checkRayBitmask = 0;
	pinRayBitmask = 0;
	
	for (int i = 0; i < 64; i++) {
		attackedSquares[i] = 0;
	}
	isWhiteToMove = m_board->movingPlayer == Piece::White;
	friendlyColor = isWhiteToMove ? Piece::White : Piece::Black;
	opponentColor = isWhiteToMove ? Piece::Black : Piece::White;
}

void MoveGenerator::CalculateAttackedSquares()
{
	for (int i = 0; i < 64; i++)
	{
		int piece = m_board->squares[i];
		if (Piece::IsColor(piece, opponentColor)) {
			if (Piece::isSlidingPiece(piece)) {
				GenerateSlidingAttacks(piece, i);
			}
			else if (piece == (Piece::Pawn | opponentColor)) {
				GeneratePawnAttacks(i);
			}
			else if (piece == (Piece::King | opponentColor)) {
				GenerateKingAttacks(i);
			}
			else if (piece == (Piece::Knight | opponentColor)) {
				GenerateKnightAttacks(i);
			}
		}
	}
}

void MoveGenerator::GenerateSlidingAttacks(int piece, int index)
{
	int startIndex = piece == (Piece::Bishop | opponentColor) ? 4 : 0;
	int targetIndex = piece == (Piece::Rook | opponentColor) ? 4 : 8;
	for (int i = startIndex; i < targetIndex; i++)
	{
		int direction = PrecompiledMoveData::directionalOffsets[i];
		int squaresToEdge = PrecompiledMoveData::squaresToEdge[index][direction];
		for (int move = 1; move <= squaresToEdge; move++) {
			int targetSquare = index + move * direction;
			attackedSquares[targetSquare] = 1;
			//Own Piece is also attacked because the opponents king can't take it
			if (Piece::IsColor(m_board->squares[targetSquare], friendlyColor)) {
				int piece = m_board->squares[targetSquare];
				if (!(piece == (Piece::King | friendlyColor)))
					break;
			}
			if (Piece::IsColor(m_board->squares[targetSquare], opponentColor)) {
				break;
			}
		}
	}
}

void MoveGenerator::GeneratePawnAttacks(int index)
{
	int left = opponentColor == Piece::White ? 9 : -7;
	int right = opponentColor == Piece::White ? 7 : -9;
	int leftMove = index + left;
	int rightMove = index + right;
	if (leftMove >= 0 && leftMove < 64)
		attackedSquares[leftMove] = 1;
	if (rightMove >= 0 && rightMove < 64)
		attackedSquares[rightMove] = 1;
	//Check promotions
	//Check EP
}

void MoveGenerator::GenerateKnightAttacks(int index)
{
	for (int i = 0; i < 8; i++)
	{
		int targetIndex = PrecompiledMoveData::knightMoves[index][i];
		if (!(targetIndex >= 0 && targetIndex < 64))
			continue;
		attackedSquares[targetIndex] = 1;
	}
}

void MoveGenerator::GenerateKingAttacks(int index)
{
	for (int i = 0; i < 8; i++) {
		int direction = PrecompiledMoveData::directionalOffsets[i];
		int squaresToEdge = PrecompiledMoveData::squaresToEdge[index][direction];
		if (squaresToEdge != 0) {
			int targetIndex = index + direction;
			int xMove = (std::abs(BoardRepresentation::xFromIndex(index) - BoardRepresentation::xFromIndex(targetIndex)));
			int yMove = (std::abs(BoardRepresentation::yFromIndex(index) - BoardRepresentation::yFromIndex(targetIndex)));
			if (std::max(xMove, yMove) == 1) {

				attackedSquares[targetIndex] = 1;
			}
		}
	}
	//Attacked squares after castle???? Some Edge cases????? probably not
	/*bool kingsideCastle = (opponentColor == Piece::White ? m_board->whiteKingsideCastle : m_board->blackKingsideCastle);
	bool queensideCastle = (opponentColor == Piece::White ? m_board->whiteQueensideCastle : m_board->blackQueensideCastle);

	if (kingsideCastle) {
		if (!(Piece::isPiece(m_board->squares[index + 1]) || Piece::isPiece(m_board->squares[index + 2])))
			moves.push_back(PieceMove(index, index + 2, FLAG_CASTLE_KING));
	}
	if (queensideCastle) {
		if (!(Piece::isPiece(m_board->squares[index - 1]) || Piece::isPiece(m_board->squares[index - 2]) || Piece::isPiece(m_board->squares[index - 3])))
			moves.push_back(PieceMove(index, index - 2, FLAG_CASTLE_QUEEN));
	}*/
}

void MoveGenerator::GenerateSlidingMoves(int piece, int index)
{
	int startIndex = piece == (Piece::Bishop | friendlyColor) ? 4 : 0;
	int targetIndex = piece == (Piece::Rook | friendlyColor) ? 4 : 8;
	for (int i = startIndex; i < targetIndex; i++)
	{
		int direction = PrecompiledMoveData::directionalOffsets[i];
		int squaresToEdge = PrecompiledMoveData::squaresToEdge[index][direction];
		for (int move = 1; move <= squaresToEdge ; move++) {
			int targetSquare = index+move*direction;
			if (Piece::IsColor(m_board->squares[targetSquare], friendlyColor)) {
				break;
			}
			AddMoveIfLegal(PieceMove(index, targetSquare));
			if (Piece::IsColor(m_board->squares[targetSquare], opponentColor)) {
				break;
			}
		}
	}
}

void MoveGenerator::GeneratePawnMoves(int index)
{
	int left = friendlyColor == Piece::White ? 9 : -7;
	int right = friendlyColor == Piece::White ? 7 : -9;
	int front = friendlyColor == Piece::White ? 8 : -8;
	//En Passent
	int epIndex = m_board->epIndex;
	if (epIndex != -1) {
		if (index - epIndex == -1){
			AddMoveIfLegal(PieceMove(index, index + left, FLAG_EP));
		}
		else if (index - epIndex == 1) {
			AddMoveIfLegal(PieceMove(index, index + right, FLAG_EP));
		}
	}
	//Normal Moves and checking promotions
	if (Piece::IsColor(m_board->squares[index + left], opponentColor)) {
		MakePawnMove(index, index + left);
	}
	if (Piece::IsColor(m_board->squares[index + right], opponentColor)) {
		MakePawnMove(index, index + right);
	}
	if (Piece::IsColor(m_board->squares[index + front], friendlyColor) || Piece::IsColor(m_board->squares[index + front], opponentColor)) {
		return;
	}
	MakePawnMove(index, index + front);
	//double pawn pushes
	bool startingPosition = false;
	if (friendlyColor == Piece::White && BoardRepresentation::yFromIndex(index) == 6) //coordinates mirrowed
		startingPosition = true;
	else if (friendlyColor == Piece::Black && BoardRepresentation::yFromIndex(index) == 1)
		startingPosition = true;
	if (startingPosition) {
		if (Piece::IsColor(m_board->squares[index + 2*front], friendlyColor) || Piece::IsColor(m_board->squares[index + 2*front], opponentColor)) {
			return;
		}
		else {
			AddMoveIfLegal(PieceMove(index, index + front * 2, FLAG_DOUBLE_PAWN_PUSH));
		}
	}
}

void MoveGenerator::GenerateKnightMoves(int index)
{
	for (int i = 0; i < 8; i++) {
		int targetIndex = PrecompiledMoveData::knightMoves[index][i];
		if (!(targetIndex >= 0 && targetIndex < 64)) {
			continue;
		}
		if (Piece::IsColor(m_board->squares[targetIndex], friendlyColor))
			continue;
		AddMoveIfLegal(PieceMove(index, targetIndex));
	}
}

void MoveGenerator::GenerateKingMoves(int index)
{
	for (int i = 0; i < 8; i++) {
		int direction = PrecompiledMoveData::directionalOffsets[i];
		int squaresToEdge = PrecompiledMoveData::squaresToEdge[index][direction];
		if (squaresToEdge != 0) {
			if (Piece::IsColor(m_board->squares[index + direction], friendlyColor)) {
				continue;
			}
			int targetIndex = index + direction;
			int xMove = (std::abs(BoardRepresentation::xFromIndex(index) - BoardRepresentation::xFromIndex(targetIndex)));
			int yMove = (std::abs(BoardRepresentation::yFromIndex(index) -BoardRepresentation::yFromIndex(targetIndex)));
			if (std::max(xMove, yMove) == 1) {

				AddMoveIfLegal(PieceMove(index, index + direction, FLAG_KING_MOVE));
			}
		}
	}
	bool kingsideCastle = (friendlyColor == Piece::White ? m_board->whiteKingsideCastle : m_board->blackKingsideCastle);
	bool queensideCastle = (friendlyColor == Piece::White ? m_board->whiteQueensideCastle : m_board->blackQueensideCastle);

	if (kingsideCastle) {
		if(!(Piece::isPiece(m_board->squares[index+1]) || Piece::isPiece(m_board->squares[index+2])))
			AddMoveIfLegal(PieceMove(index, index + 2, FLAG_CASTLE_KING));
	}
	if (queensideCastle) {
		if (!(Piece::isPiece(m_board->squares[index - 1]) || Piece::isPiece(m_board->squares[index - 2]) || Piece::isPiece(m_board->squares[index - 3])))
			AddMoveIfLegal(PieceMove(index, index - 2, FLAG_CASTLE_QUEEN));
	}
}

bool MoveGenerator::InCheck()
{
	int kingIndex = m_board->kingSquares[(friendlyColor / 8) - 1];
	return attackedSquares[kingIndex] == 1;
}

void MoveGenerator::MakePawnMove(int startIndex, int targetIndex)
{
	//Check if its a promotion
	//Check indepenent of moving player because pawns can never reach a square behind them
	int y = BoardRepresentation::yFromIndex(targetIndex);
	if (y == 7 || y == 0) {
		AddMoveIfLegal(PieceMove(startIndex, targetIndex, FLAG_PROMOTION, PROMOTION_QUEEN));
		return;
	}
	AddMoveIfLegal(PieceMove(startIndex, targetIndex));
}

void MoveGenerator::AddMoveIfLegal(PieceMove move)
{
	if (move.moveFlag == FLAG_KING_MOVE) {
		if (attackedSquares[move.targetSquare] == 1) {
			return;
		}
	}
	else if (move.moveFlag == FLAG_CASTLE_KING) {
			for (int i = 1; i <= 2; i++) {
				if (attackedSquares[move.startSquare + i] == 1)
					return;
			}
		}
	else if (move.moveFlag == FLAG_CASTLE_QUEEN) {
			for (int i = 1; i <= 3; i++) {
				if (attackedSquares[move.startSquare - i] == 1)
					return;
			}
		}
	
	else {
		if (InCheck()) {
			return;
		}
	}
	
	moves.push_back(move);
}