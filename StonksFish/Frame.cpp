#include "Frame.h"
#include <string>



Frame::Frame(Board* board) : wxFrame(nullptr, wxID_ANY, "StonksFish", wxPoint(30, 30), wxSize(1200, 800))
{
	m_board = board;
	wxBoxSizer* bSizer = new wxBoxSizer(wxVERTICAL);
	boardUI = new BoardUI(this, board);
	bSizer->Add(boardUI, 1, wxEXPAND);
	SetSizer(bSizer);

	
}
Frame::~Frame()
{
}


