#pragma once

#include "wx/wx.h"
#include <wx/timectrl.h>
#include <wx/calctrl.h>
#include <wx/choice.h>
#include <vector>
#include "AlarmClock.h"
#include "clockMain.h"

class clockMain;

class AlarmClockPanel : public wxFrame
{
public:
	AlarmClockPanel(clockMain &);
	AlarmClockPanel(clockMain &, AlarmClock &alram);
	~AlarmClockPanel();

	wxTextCtrl *txc_AlarmName = nullptr;
	wxTextCtrl *tct_SongName = nullptr;
	wxButton *btn_ChooseSong = nullptr;
	wxButton *btn_SetAlarm = nullptr;
	wxTimePickerCtrl *tpc_TimePicker = nullptr;
	wxCalendarCtrl *cc_DatePicker = nullptr;
	wxChoice *ch_AlarmType = nullptr;

	void OnButtonClicked(wxCommandEvent &evt);

	wxDECLARE_EVENT_TABLE();

private:
	wxArrayString AlarmTypes;
	clockMain *MainWindow;
	std::vector<AlarmClock*> AlarmClockList;
	wxString *path = nullptr;

	void OnOpen(wxCommandEvent& WXUNUSED(event));


};

