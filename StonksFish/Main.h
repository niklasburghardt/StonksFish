#pragma once
#include <wx/wx.h>
#include "Frame.h"
#include "Board.h"

class ChessApp : public wxApp
{
public:
	ChessApp();
	~ChessApp();
private:
	Frame* m_frame = nullptr;
	Board* m_board = nullptr;
public:
	virtual bool OnInit();
};

