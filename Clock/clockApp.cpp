#include "clockApp.h"

wxIMPLEMENT_APP(clockApp);

bool clockApp::OnInit()
{
	m_frame1 = new clockMain();
	m_frame1->Show();

	return true;
}
