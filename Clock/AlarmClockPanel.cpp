#include "AlarmClockPanel.h"
#include <wx/wfstream.h>
#include <string>

wxBEGIN_EVENT_TABLE(AlarmClockPanel, wxFrame)
	EVT_BUTTON(10004, OnButtonClicked)
	EVT_BUTTON(10005, OnOpen)
wxEND_EVENT_TABLE()

AlarmClockPanel::AlarmClockPanel(clockMain &MainWindow) : wxFrame(nullptr, wxID_ANY, "Alarm", wxPoint(40, 50), wxSize(600, 200))
{
	wxPanel *panel = new wxPanel(this, -1);
	wxBoxSizer *BoxSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *VBoxSizer = new wxBoxSizer(wxVERTICAL);

	tct_SongName = new wxTextCtrl(panel, 10010, wxEmptyString, wxPoint(0, 0), wxSize(400, 20), 0,
		wxDefaultValidator, wxString("SongName"));

	tct_SongName->SetValue("HelloE:\Music\(Can't Get My) Head Around You.mp3");

	txc_AlarmName = new wxTextCtrl(panel, 10001, wxEmptyString, wxPoint(10,10), wxSize(88, 22), 0, 
		wxDefaultValidator, wxString("Alarm name"));
	txc_AlarmName->SetValue(wxString("Alarm1"));

	AlarmTypes.Add(wxString("Single"), 1);
	AlarmTypes.Add(wxString("Everyday"), 1);
	AlarmTypes.Add(wxString("Weekdays"), 1);
	AlarmTypes.Add(wxString("Weekend"), 1);
	ch_AlarmType = new wxChoice(panel, 10002, wxPoint(0, 0), wxSize(88, 22), AlarmTypes, 0,
		wxDefaultValidator, wxString("AlarmTypes"));
	ch_AlarmType->SetSelection(0);

	tpc_TimePicker = new wxTimePickerCtrl(panel, 10003, wxDefaultDateTime, wxPoint(0, 0), wxSize(88, 22),
		wxTP_DEFAULT, wxDefaultValidator, wxString("Alarm time"));

	btn_ChooseSong = new wxButton(panel, 10005, wxT("Choose song"), wxPoint(0, 0), wxSize(88, 26), 0,
		wxDefaultValidator, wxString("Choose song"));
	btn_SetAlarm = new wxButton(panel, 10004, wxT("Set alarm"), wxPoint(0,0), wxSize(88, 26), 0, 
		wxDefaultValidator, wxString("Set alarm"));

	VBoxSizer->Add(txc_AlarmName);
	VBoxSizer->AddSpacer(5);
	VBoxSizer->Add(tpc_TimePicker);
	VBoxSizer->AddSpacer(5);
	VBoxSizer->Add(ch_AlarmType);
	VBoxSizer->AddSpacer(5);
	VBoxSizer->Add(btn_ChooseSong);
	VBoxSizer->AddSpacer(15);
	VBoxSizer->Add(btn_SetAlarm);

	cc_DatePicker = new wxCalendarCtrl(panel, 10005, wxDefaultDateTime, wxPoint(0, 0), wxSize(240, 180), 
		wxCAL_SHOW_HOLIDAYS, wxString("Alarm Date"));

	BoxSizer->AddSpacer(5);
	BoxSizer->Add(VBoxSizer);
	BoxSizer->AddSpacer(5);
	BoxSizer->Add(cc_DatePicker);
	BoxSizer->Add(tct_SongName);

	panel->SetSizer(BoxSizer);

	this->MainWindow = &MainWindow;
}

AlarmClockPanel::AlarmClockPanel(clockMain &MainWindow, AlarmClock &alarm)
	: AlarmClockPanel(MainWindow)
{
	txc_AlarmName->SetValue(alarm.GetAlarmName());
	tct_SongName->SetValue(alarm.GetSongName());

	wxDateTime DateTime = wxDateTime::Now();
	wxDateTime DateDate = wxDateTime::Today();

	DateDate.SetYear(alarm.GetAlarmAbsoluteDate().GetYear());
	DateDate.SetMonth(alarm.GetAlarmAbsoluteDate().GetMonth());
	DateDate.SetDay(alarm.GetAlarmAbsoluteDate().GetDay());
	DateTime.SetHour(alarm.GetAlarmAbsoluteTime().GetHour());
	DateTime.SetMinute(alarm.GetAlarmAbsoluteTime().GetMinute());

	tpc_TimePicker->SetValue(DateTime);
	cc_DatePicker->SetDate(DateDate.GetDateOnly());
	ch_AlarmType->SetSelection(alarm.GetAlarmType());
}

AlarmClockPanel::~AlarmClockPanel()
{
	MainWindow->Enable();
	MainWindow->SetFocus();
}

void AlarmClockPanel::OnButtonClicked(wxCommandEvent & evt)
{

	bool TimeIsTaken = false;

	this->AlarmClockList = MainWindow->GetAlarmClockList();

	for (size_t i = 0; i < this->AlarmClockList.size(); i++)
	{
		wxDateTime AlarmTime = tpc_TimePicker->GetValue();
		wxDateTime AlarmTimeIsUse = AlarmClockList.at(i)->GetAlarmAbsoluteTime();
		if (AlarmTime == AlarmTimeIsUse)
			TimeIsTaken = true;
	}

	if (!TimeIsTaken) {
		wxString AlarmName = txc_AlarmName->GetLineText(0);
		wxDateTime AlarmTime = tpc_TimePicker->GetValue();
		wxDateTime AlarmDate = cc_DatePicker->GetDate();
		int AlarmType = ch_AlarmType->GetSelection();

		AlarmClock *alarm = new AlarmClock();
		alarm->SetAlarmParams(AlarmName, AlarmTime, AlarmDate, AlarmType);
		if (path != nullptr)
		{
			alarm->SetSongName(path->ToStdString());
		}
		MainWindow->AddToAlarmClockList(*alarm);

		MainWindow->UpdateListBox();
		this->Destroy();
	}

}

void AlarmClockPanel::OnOpen(wxCommandEvent& WXUNUSED(event))
{

	wxFileDialog
		openFileDialog(this, _("Open XYZ file"), "", "",
			"mp3 files (*.mp3)|*.mp3", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;     

	path = new wxString(openFileDialog.GetPath());
	tct_SongName->AppendText(*path);
	wxFileInputStream input_stream(openFileDialog.GetPath());
	if (!input_stream.IsOk())
	{
		wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
		return;
	}
}
