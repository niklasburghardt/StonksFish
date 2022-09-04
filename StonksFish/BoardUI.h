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
	void OnResize(wxSizeEvent& evt);
	void OnMouseDown(wxMouseEvent& evt);
	void OnMouseUp(wxMouseEvent& evt);
	void OnMarkSquare(wxMouseEvent& evt);
	void OnClearMarkedSquares(wxMouseEvent& evt);
	void OnIdle(wxIdleEvent& evt);
	void OnDragMouse(wxMouseEvent& evt);
private:
	void LoadUI();
private:
	int m_draggingSquare;
	bool m_drawDragging;
	int m_selectedSquare;
	int* m_shownMoves;
	int* m_markedSquares;
//store images so we don't have to download them each time
private:

	std::map<int, wxString> symbolPath;
	std::map<int, wxImage> imagePath;
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
	int CalculateBoardOffset();
	
private:
	void OnLoadFen(wxCommandEvent& evt);

};