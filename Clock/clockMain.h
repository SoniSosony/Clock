#pragma once

#include "wx/wx.h"
#include "ClockDlg.h"

class clockMain : public wxFrame
{
public:
	clockMain();
	~clockMain();

public:
	wxButton *m_btn1 = nullptr;
	wxTextCtrl *m_txt1 = nullptr;
	wxListBox *m_list1 = nullptr;
	ClockDlg *w_clock = nullptr;

	void WxTimer1Timer(wxTimerEvent& event);
	void OnButtonClicked(wxCommandEvent &evt);

	wxDECLARE_EVENT_TABLE();

private:
	wxTimer *WxTimer1;
};

