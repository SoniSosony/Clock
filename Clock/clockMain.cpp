#include "clockMain.h"
#include <wx/grid.h>

wxBEGIN_EVENT_TABLE(clockMain, wxFrame)
	EVT_BUTTON(10001, OnButtonClicked)
	EVT_TIMER(10002, clockMain::WxTimer1Timer)
wxEND_EVENT_TABLE()


clockMain::clockMain() : wxFrame(nullptr, wxID_ANY, "Clock main", wxPoint(40, 50), wxSize(800, 600))
{
	wxPanel *panel = new wxPanel(this, -1);
	wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
	wxPanel *clockPanel = new wxPanel(panel);
	w_clock = new ClockDlg(clockPanel);
	wxButton *ok = new wxButton(panel, -1, wxT("Ok"));
	boxSizer->Add(clockPanel);
	boxSizer->Add(ok);
	//boxSizer->Add(w_clock, 1, wxEXPAND | wxALL, 20);
	w_clock->Show(true);

	WxTimer1 = new wxTimer();
	WxTimer1->SetOwner(this, 10002);
	WxTimer1->Start(200);

	panel->SetSizer(boxSizer);
}

clockMain::~clockMain()
{
}

void clockMain::WxTimer1Timer(wxTimerEvent & event)
{
	w_clock->RefreshClock();
}

void clockMain::OnButtonClicked(wxCommandEvent & evt)
{
	m_list1->AppendString(m_txt1->GetValue());
	evt.Skip();
}
