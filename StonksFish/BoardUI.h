#pragma once
#include <wx/wx.h>
#include <map>
#include "Board.h"
#include "Move.h"
class BoardUI : public wxPanel {
public:
	BoardUI(wxWindow* parent, Board* board);
	wxDECLARE_EVENT_TABLE();
public:
	void OnPaint(wxPaintEvent& evt);
	void OnMouseDown(wxMouseEvent& evt);
	void OnMouseUp(wxMouseEvent& evt);
	void OnIdle(wxIdleEvent& evt);
private:
	int m_draggingSquare;
	int m_selectedSquare;
	int* m_shownMoves;
private:
	wxButton* m_loadFenButton;
	wxTextCtrl* m_fenInput;
	wxPNGHandler* handler;
private:
	Board* m_board;
	void ActivateRenderLoop(bool on);
	void RenderBoard(int offset);
	void DrawPieces(int offset);
	
	void ShowLegalMoves(int index);
	bool MovePossible(int startIndex, int targetIndex);
	PieceMove GiveMoveFromCoords(int startIndex, int targetIndex);
	void DrawPieceLoop();
	int IndexFromMousePosition(wxMouseEvent &evt);
	std::map<int, wxString> symbolPath;
	int CalculateBoardOffset();
	
private:
	void OnLoadFen(wxCommandEvent& evt);

};