#pragma once

#include "wx/wx.h"
#include <wx/timectrl.h>
#include <wx/calctrl.h>
#include <wx/choice.h>

class AlarmClockPanel : public wxFrame
{
public:
	AlarmClockPanel();
	~AlarmClockPanel();

	wxTextCtrl *txc_AlarmName = nullptr;
	wxButton *btn_SetAlarm = nullptr;
	wxTimePickerCtrl *tpc_TimePicker = nullptr;
	wxCalendarCtrl *cc_DatePicker = nullptr;
	wxChoice *ch_AlarmType = nullptr;

	wxArrayString AlarmTypes;

	void OnButtonClicked(wxCommandEvent &evt);

	wxDECLARE_EVENT_TABLE();

};

