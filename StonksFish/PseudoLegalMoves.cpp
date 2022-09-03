#include "PseudoLegalMoves.h"
#include "PrecompiledMoveData.h"
#include "Board.h"

PseudoLegalMoves::PseudoLegalMoves()
{
	moves = std::vector<PieceMove>();

}

std::vector<PieceMove> PseudoLegalMoves::GenerateLegalMoves(Board* board)
{
	moves.clear();
	m_board = board;
	
	isWhiteToMove = m_board->movingPlayer == Piece::White;
	friendlyColor = isWhiteToMove ? Piece::White : Piece::Black;
	opponentColor = isWhiteToMove ? Piece::Black : Piece::White;

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

void PseudoLegalMoves::GenerateSlidingMoves(int piece, int index)
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
			moves.push_back(PieceMove(index, targetSquare));
			if (Piece::IsColor(m_board->squares[targetSquare], opponentColor)) {
				break;
			}
		}
	}
}

void PseudoLegalMoves::GeneratePawnMoves(int index)
{
	int left = friendlyColor == Piece::White ? 9 : -7;
	int right = friendlyColor == Piece::White ? 7 : -9;
	int front = friendlyColor == Piece::White ? 8 : -8;
	//En Passent
	int epIndex = m_board->epIndex;
	if (epIndex != -1) {
		if (index - epIndex == -1){
			moves.push_back(PieceMove(index, index + left, true));
		}
		else if (index - epIndex == 1) {
			moves.push_back(PieceMove(index, index + right, true));
		}
	}

	if (Piece::IsColor(m_board->squares[index + left], opponentColor)) {
		moves.push_back(PieceMove(index, index + left));
	}
	if (Piece::IsColor(m_board->squares[index + right], opponentColor)) {
		moves.push_back(PieceMove(index, index + right));
	}
	if (Piece::IsColor(m_board->squares[index + front], friendlyColor) || Piece::IsColor(m_board->squares[index + front], opponentColor)) {
		return;
	}
	moves.push_back(PieceMove(index, index + front));
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
			moves.push_back(PieceMove(index, index + front * 2));
		}
	}
}
void PseudoLegalMoves::GenerateKnightMoves(int index)
{
	for (int i = 0; i < 8; i++) {
		int targetIndex = PrecompiledMoveData::knightMoves[index][i];
		if (!(targetIndex >= 0 && targetIndex < 64)) {
			continue;
		}
		if (Piece::IsColor(m_board->squares[targetIndex], friendlyColor))
			continue;
		moves.push_back(PieceMove(index, targetIndex));
	}
}
void PseudoLegalMoves::GenerateKingMoves(int index)
{
	for (int i = 0; i < 8; i++) {
		int direction = PrecompiledMoveData::directionalOffsets[i];
		int squaresToEdge = PrecompiledMoveData::squaresToEdge[index][direction];
		if (squaresToEdge != 0) {
			if (Piece::IsColor(m_board->squares[index + direction], friendlyColor)) {
				continue;
			}
			moves.push_back(PieceMove(index, index + direction));
		}
	}
}