#include "BoardUI.h"
#include "Piece.h"
#include "BoardRepresentation.h"
#include <math.h>
#include <string>
#include "Move.h"

wxBEGIN_EVENT_TABLE(BoardUI, wxPanel)
	EVT_BUTTON(10001, OnLoadFen)

wxEND_EVENT_TABLE();

BoardUI::BoardUI(wxWindow* parent, Board* board) : wxPanel(parent, wxID_ANY, wxPoint(0, 0), wxDefaultSize, wxFULL_REPAINT_ON_RESIZE) {

	m_board = board;
	m_draggingSquare = -1;
	m_selectedSquare = -1;
	m_shownMoves = new int[64];
	m_fenInput = new wxTextCtrl(this, wxID_ANY, "", wxPoint(900, 10), wxSize(100, 30));
	m_loadFenButton = new wxButton(this, 10001, "Load Fen", wxPoint(900, 80), wxSize(50, 30));
	handler = new wxPNGHandler();
	wxImage::AddHandler(handler);
	
	Bind(wxEVT_PAINT, &BoardUI::OnPaint, this);
	Bind(wxEVT_LEFT_DOWN, &BoardUI::OnMouseDown, this);
	Bind(wxEVT_LEFT_UP, &BoardUI::OnMouseUp, this);
	
	
	
	//white Pieces
	symbolPath[Piece::Pawn | Piece::White]		= "C:\\DevPictures\\white_pawn.png";
	symbolPath[Piece::Knight | Piece::White]	= "C:\\DevPictures\\white_knight.png";
	symbolPath[Piece::Bishop | Piece::White]	= "C:\\DevPictures\\white_bishop.png";
	symbolPath[Piece::Rook | Piece::White]		= "C:\\DevPictures\\white_rook.png";
	symbolPath[Piece::Queen | Piece::White]		= "C:\\DevPictures\\white_queen.png";
	symbolPath[Piece::King | Piece::White]		= "C:\\DevPictures\\white_king.png";
	//black pieces								   C
	symbolPath[Piece::Pawn | Piece::Black]		= "C:\\DevPictures\\black_pawn.png";
	symbolPath[Piece::Knight | Piece::Black]	= "C:\\DevPictures\\black_knight.png";
	symbolPath[Piece::Bishop | Piece::Black]	= "C:\\DevPictures\\black_bishop.png";
	symbolPath[Piece::Rook | Piece::Black]		= "C:\\DevPictures\\black_rook.png";
	symbolPath[Piece::Queen | Piece::Black]		= "C:\\DevPictures\\black_queen.png";
	symbolPath[Piece::King | Piece::Black]		= "C:\\DevPictures\\black_king.png";

	//Add images
	imagePath[Piece::Pawn | Piece::White] = wxImage(symbolPath[Piece::Pawn | Piece::White], wxBITMAP_TYPE_ANY);
	imagePath[Piece::Knight | Piece::White] = wxImage(symbolPath[Piece::Knight | Piece::White], wxBITMAP_TYPE_ANY);
	imagePath[Piece::Bishop | Piece::White] = wxImage(symbolPath[Piece::Bishop | Piece::White], wxBITMAP_TYPE_ANY);
	imagePath[Piece::Rook | Piece::White] = wxImage(symbolPath[Piece::Rook | Piece::White], wxBITMAP_TYPE_ANY);
	imagePath[Piece::Queen | Piece::White] = wxImage(symbolPath[Piece::Queen | Piece::White], wxBITMAP_TYPE_ANY);
	imagePath[Piece::King | Piece::White] = wxImage(symbolPath[Piece::King | Piece::White], wxBITMAP_TYPE_ANY);
	imagePath[Piece::Pawn | Piece::Black] = wxImage(symbolPath[Piece::Pawn | Piece::Black], wxBITMAP_TYPE_ANY);
	imagePath[Piece::Knight | Piece::Black] = wxImage(symbolPath[Piece::Knight | Piece::Black], wxBITMAP_TYPE_ANY);
	imagePath[Piece::Bishop | Piece::Black] = wxImage(symbolPath[Piece::Bishop | Piece::Black], wxBITMAP_TYPE_ANY);
	imagePath[Piece::Rook | Piece::Black] = wxImage(symbolPath[Piece::Rook | Piece::Black], wxBITMAP_TYPE_ANY);
	imagePath[Piece::Queen | Piece::Black] = wxImage(symbolPath[Piece::Queen | Piece::Black], wxBITMAP_TYPE_ANY);
	imagePath[Piece::King | Piece::Black] = wxImage(symbolPath[Piece::King | Piece::Black], wxBITMAP_TYPE_ANY);


	
}

void BoardUI::OnLoadFen(wxCommandEvent& evt)
{
	

	std::string fen = (std::string)m_fenInput->GetValue();
	m_board->LoadPositionFromFen(fen);
	wxWindow::Refresh();

}

void BoardUI::OnIdle(wxIdleEvent& evt)
{
	
}

void BoardUI::OnPaint(wxPaintEvent& evt)
{
	int offset = CalculateBoardOffset();
	
	RenderBoard(offset);
	DrawPieces(offset);
	evt.Skip();
}

void BoardUI::RenderBoard(int offset) 
{
	wxPaintDC dc(this);
	dc.SetPen(*wxTRANSPARENT_PEN);

	wxSize size = GetClientSize();

	int w = std::min(size.x, size.y) / 8;
	int h = w;
	wxColor dark = wxColor();
	wxColor light = wxColor();
	wxColor selectedFieldLight = wxColor();
	wxColor selectedFieldDark = wxColor();
	wxColor possibleMoveLight = wxColor();
	wxColor possibleMoveDark = wxColor();
	dark.Set(170, 120, 74, 255);
	light.Set(238, 210, 185, 255);
	selectedFieldLight.Set(195, 214, 141, 255);
	selectedFieldDark.Set(165, 191, 90, 255);
	possibleMoveLight.Set(214, 141, 141, 255);
	possibleMoveDark.Set(191, 90, 90, 255);

	int optionOffset = 0;
	/*if (size.x - size.y > 100)
		optionOffset = 100;*/
	for (int rank = 7; rank >= 0; rank--)
	{
		for (int file = 0; file < 8; file++) {
			int x = w * file + optionOffset + offset;
			int y = h * rank;
			wxBrush color = wxBrush();

			bool white = (rank + file) % 2 == 0;
			int currentIndex = BoardRepresentation::indexFromCoords(file, rank);

			color.SetColour(white ? light : dark);

			if (currentIndex == m_selectedSquare || currentIndex == m_draggingSquare) {
				color.SetColour(white ? selectedFieldLight : selectedFieldDark);
			}
			else if (m_shownMoves[BoardRepresentation::indexFromCoords(file, rank)] == 1) {
				color.SetColour(white ? possibleMoveLight : possibleMoveDark);
			}
			dc.SetBrush(color);
			wxRect rect(x, y, w, h);

			dc.DrawRectangle(rect);

		}
	}
}

void BoardUI::DrawPieces(int offset)
{
	wxPaintDC dc(this);
	wxSize size = GetClientSize();
	int w = std::min(size.x, size.y) / 8;
	for(int i = 0; i<64; i++){
		int piece = m_board->squares[i];
		if (piece > 0) {
			
			
			int x = BoardRepresentation::xFromIndex(i) * w + offset;
			int y = BoardRepresentation::yFromIndex(i) * w;
			if (i == m_draggingSquare) {
				const wxPoint mouse = wxGetMousePosition();
				int x = mouse.x;
				int y = mouse.y;

			}
			
			wxImage img = imagePath[piece];
			dc.DrawBitmap(wxBitmap(img.Scale(w, w)), wxPoint(x, y), false);
			
		}
	}

}

bool BoardUI::MovePossible(int startIndex, int targetIndex) {
	for (int i = 0; i < m_board->moveGenerator->moves.size(); i++) {
		PieceMove move = m_board->moveGenerator->moves[i];
		if (move.startSquare == startIndex && move.targetSquare == targetIndex) {
			return true;
		}
	}
	return false;
}

PieceMove BoardUI::GiveMoveFromCoords(int startIndex, int targetIndex) {
	for (int i = 0; i < m_board->moveGenerator->moves.size(); i++) {
		PieceMove move = m_board->moveGenerator->moves[i];
		if (move.startSquare == startIndex && move.targetSquare == targetIndex) {
			return move;
		}
	}
	//this can never happen its just a placeholder for the constructor
	m_board->moveGenerator->moves[0];
}

void BoardUI::ShowLegalMoves(int index)
{
	//Clear bitboard
	for (int i = 0; i < 64; i++) {
		m_shownMoves[i] = 0;
	}
	if (index == -1) {
		return;
	}
	for (int i = 0; i < m_board->moveGenerator->moves.size(); i++)
	{
		PieceMove move = m_board->moveGenerator->moves[i];
		if (move.startSquare == index)
		{
			m_shownMoves[move.targetSquare] = 1;
		}
	}
}

void BoardUI::DrawPieceLoop()
{
	int offset = CalculateBoardOffset();
	DrawPieces(offset);
}

void BoardUI::OnMouseDown(wxMouseEvent& evt)
{
	int index = IndexFromMousePosition(evt);
	if(index != -1){
		if (Piece::IsColor(m_board->squares[index], m_board->movingPlayer) && m_selectedSquare == -1)
		{
			m_draggingSquare = index;
			ShowLegalMoves(index);
			wxWindow::Refresh();
		}
	}
	evt.Skip();
}

void BoardUI::OnMouseUp(wxMouseEvent& evt)
{
	int index = IndexFromMousePosition(evt);
	if (m_selectedSquare != -1)
	{
		if (index != m_selectedSquare  )
		{
			ShowLegalMoves(-1);
			if (MovePossible(m_selectedSquare, index)) {
				m_board->MakeMove(GiveMoveFromCoords(m_selectedSquare, index));
				m_selectedSquare = -1;
			}
			else if (Piece::IsColor(m_board->squares[index], m_board->movingPlayer)) {
				m_selectedSquare = index;
				ShowLegalMoves(index);
			}
			
		}
		
		wxWindow::Refresh();

	}
	else if (m_draggingSquare != -1)
	{
		if (index != m_draggingSquare && MovePossible(m_draggingSquare, index)) {
			//check if possible
			m_board->MakeMove(GiveMoveFromCoords(m_draggingSquare, index));
			ShowLegalMoves(-1);
		}
		else {
			m_selectedSquare = m_draggingSquare;
		}
		m_draggingSquare = -1;
		wxWindow::Refresh();
	}
	evt.Skip();

}


int BoardUI::CalculateBoardOffset()
{
	wxSize screen = GetClientSize();
	int w = std::min(screen.x, screen.y);
	int offset = screen.x - w;
	return offset;
}

int BoardUI::IndexFromMousePosition(wxMouseEvent& evt)
{
	double x = evt.GetX() - CalculateBoardOffset();
	double y = evt.GetY();
	wxSize size = GetClientSize();
	double w = std::min(size.x, size.y);
	double xIndex = std::floor((x / w) * 8);
	double yIndex = std::floor((y / w) * 8);
	if (xIndex >= 0 && xIndex < 8 && yIndex >= 0 && yIndex < 8) {
		return BoardRepresentation::indexFromCoords((int)xIndex, (int)yIndex);
	}
	return -1;
}

