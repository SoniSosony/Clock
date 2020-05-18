#include "clockMain.h"
#include <wx/grid.h>

wxBEGIN_EVENT_TABLE(clockMain, wxFrame)
	EVT_BUTTON(10001, OnButtonClicked)
	EVT_TIMER(10002, clockMain::WxTimer1Timer)
wxEND_EVENT_TABLE()


clockMain::clockMain() : wxFrame(nullptr, wxID_ANY, "Clock main", wxPoint(40, 50), wxSize(800, 600))
{
	wxPanel *panel = new wxPanel(this, -1);
	wxBoxSizer *boxSizer = new wxBoxSizer(wxHORIZONTAL);
	wxPanel *clockPanel = new wxPanel(panel);
	w_clock = new ClockDlg(clockPanel);
	boxSizer->Add(clockPanel);
	//boxSizer->Add(w_clock, 1, wxEXPAND | wxALL, 20);
	w_clock->Show(true);

	wxBoxSizer *AlarmBoxSizer = new wxBoxSizer(wxVERTICAL);
	btn_SetAlarm = new wxButton(panel, 10001, wxT("Set alarm"));
	l_AlarmList = new wxListBox(panel, 10003, wxPoint(0, 0), wxSize(300, 400), 0, NULL, 0, wxDefaultValidator, wxString("Alarms"));
	l_AlarmList->Append(wxString("Hello"));
	AlarmBoxSizer->Add(l_AlarmList);
	AlarmBoxSizer->Add(btn_SetAlarm);
	boxSizer->Add(AlarmBoxSizer);

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
	AlarmClockPanel *AlarmPanel = new AlarmClockPanel();
	AlarmPanel->Show();
	evt.Skip();
}
