#pragma once
#include <wx/wx.h>
#include "BoardUI.h"
#include "Board.h"
class Frame : public wxFrame
{
public:
	Frame(Board* board);
	~Frame();
public:
	BoardUI* boardUI;
public:
	

	
private:
	Board* m_board;
	
};

