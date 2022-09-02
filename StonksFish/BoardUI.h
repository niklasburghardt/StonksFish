#pragma once
#include <wx/wx.h>
#include <map>
#include "Board.h"
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
private:
	wxButton* m_loadFenButton;
	wxTextCtrl* m_fenInput;
private:
	Board* m_board;
	void ActivateRenderLoop(bool on);
	void RenderBoard();
	void DrawPieces();
	void DrawPieceLoop();
	int IndexFromMousePosition(wxMouseEvent &evt);
	std::map<int, wxString> symbolPath;
private:
	void OnLoadFen(wxCommandEvent& evt);

};