#include "Main.h"
IMPLEMENT_APP(ChessApp);



ChessApp::ChessApp()
{

}

ChessApp::~ChessApp()
{
	
}

bool ChessApp::OnInit()
{
	m_board = new Board();
	m_frame = new Frame(m_board);
	m_frame->Show();
	return true;
}
