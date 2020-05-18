#pragma once

#include <list>
#include "wx/wx.h"
#include <wx/listctrl.h>
#include "ClockDlg.h"
#include "AlarmClock.h"
#include "AlarmClockPanel.h"

class clockMain : public wxFrame
{
public:
	clockMain();
	~clockMain();

	wxButton *btn_SetAlarm = nullptr;
	ClockDlg *w_clock = nullptr;
	wxTimer *WxTimer1 = nullptr;
	wxListBox *l_AlarmList = nullptr;
	std::list<AlarmClock*> AlarmClockList;

	void WxTimer1Timer(wxTimerEvent& event);
	void OnButtonClicked(wxCommandEvent &evt);

	wxDECLARE_EVENT_TABLE();

private:

	void AddAlarm();
};

