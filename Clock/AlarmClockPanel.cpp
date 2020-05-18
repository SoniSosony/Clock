#include "AlarmClockPanel.h"

wxBEGIN_EVENT_TABLE(AlarmClockPanel, wxFrame)
	EVT_BUTTON(10004, OnButtonClicked)
wxEND_EVENT_TABLE()

AlarmClockPanel::AlarmClockPanel() : wxFrame(nullptr, wxID_ANY, "Alarm", wxPoint(40, 50), wxSize(600, 200))
{
	wxPanel *panel = new wxPanel(this, -1);
	wxBoxSizer *BoxSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *VBoxSizer = new wxBoxSizer(wxVERTICAL);

	txc_AlarmName = new wxTextCtrl(panel, 10001, wxEmptyString, wxPoint(10,10), wxSize(88, 22), 0, 
		wxDefaultValidator, wxString("Alarm name"));

	AlarmTypes.Add(wxString("Single"), 1);
	AlarmTypes.Add(wxString("Everyday"), 1);
	AlarmTypes.Add(wxString("Weekdays"), 1);
	AlarmTypes.Add(wxString("Weekend"), 1);
	ch_AlarmType = new wxChoice(panel, 10002, wxPoint(0, 0), wxSize(88, 22), AlarmTypes, 0,
		wxDefaultValidator, wxString("AlarmTypes"));

	tpc_TimePicker = new wxTimePickerCtrl(panel, 10003, wxDefaultDateTime, wxPoint(0, 0), wxSize(88, 22),
		wxTP_DEFAULT, wxDefaultValidator, wxString("Alarm time"));

	btn_SetAlarm = new wxButton(panel, 10004, wxT("Set alarm"), wxPoint(0,0), wxSize(88, 26), 0, 
		wxDefaultValidator, wxString("Set alarm"));

	VBoxSizer->Add(txc_AlarmName);
	VBoxSizer->AddSpacer(5);
	VBoxSizer->Add(tpc_TimePicker);
	VBoxSizer->AddSpacer(5);
	VBoxSizer->Add(ch_AlarmType);
	VBoxSizer->AddSpacer(20);
	VBoxSizer->Add(btn_SetAlarm);

	cc_DatePicker = new wxCalendarCtrl(panel, 10005, wxDefaultDateTime, wxPoint(0, 0), wxSize(240, 180), 
		wxCAL_SHOW_HOLIDAYS, wxString("Alarm Date"));

	BoxSizer->AddSpacer(5);
	BoxSizer->Add(VBoxSizer);
	BoxSizer->AddSpacer(5);
	BoxSizer->Add(cc_DatePicker);

	panel->SetSizer(BoxSizer);
}

AlarmClockPanel::~AlarmClockPanel()
{

}

void AlarmClockPanel::OnButtonClicked(wxCommandEvent & evt)
{
}
