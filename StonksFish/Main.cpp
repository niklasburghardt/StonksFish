#include "Main.h"
IMPLEMENT_APP(ChessApp);
#include "PrecompiledMoveData.h"



ChessApp::ChessApp()
{

}

ChessApp::~ChessApp()
{
	
}

bool ChessApp::OnInit()
{
	PrecompiledMoveData::Init();
	m_board = new Board();
	m_frame = new Frame(m_board);
	m_frame->Show();
	return true;
}
