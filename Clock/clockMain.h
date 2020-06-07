#pragma once

#include "wx/wx.h"
#include <vector>
#include <wx/listctrl.h>
#include "ClockDlg.h"
#include "AlarmClock.h"
#include "AlarmClockPanel.h"
#include "Radio.h"

class clockMain : public wxFrame
{

public:
	clockMain();
	~clockMain();

	wxButton *btn_AddAlarm = nullptr;
	wxButton *btn_DeleteAlarm = nullptr;
	wxButton *btn_DisableAlarm = nullptr;
	wxButton *btn_EditAlarm = nullptr;
	wxButton *btn_Radio = nullptr;
	ClockDlg *w_clock = nullptr;
	wxTimer *WxTimer1 = nullptr;
	wxListBox *l_AlarmList = nullptr;
	wxPanel *panel = nullptr;
	wxSlider *sl_VolumeLevel = nullptr;

	void AddToAlarmClockList(AlarmClock &);	
	void UpdateListBox();
	void DeleteAlarmFromList(AlarmClock &alarm);
	std::vector<AlarmClock*> GetAlarmClockList() const;

	wxDECLARE_EVENT_TABLE();

private:
	std::vector<AlarmClock*> AlarmClockList;

	void WxTimer1Timer(wxTimerEvent& event);
	void AddAlarmButtonClicked(wxCommandEvent &evt);
	void DeleteAlarmButtonClicked(wxCommandEvent &evt);
	void DisableAlarmButtonClicked(wxCommandEvent &evt);
	void EditAlarmButtonClicked(wxCommandEvent & evt);
	void RadioButtonClicked(wxCommandEvent & evt);
	float ChangeVolume(double nVolume, bool bScalar);

};

