#ifndef __CLOCKDLG_h__
#define __CLOCKDLG_h__

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#include <wx/dialog.h>
#else
#include <wx/wxprec.h>
#endif

#include <wx/timer.h>
#include <wx/dcbuffer.h>
#include <cmath>

#undef ClockDlg_STYLE
#define ClockDlg_STYLE wxSUNKEN_BORDER | wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxDIALOG_NO_PARENT | wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxCLOSE_BOX


class ClockDlg : public wxControl
{
private:
	DECLARE_EVENT_TABLE();

public:
	ClockDlg(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("Clock"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = ClockDlg_STYLE, const wxValidator& val = wxDefaultValidator, const wxString& name = "MySpecialWidget");
	virtual ~ClockDlg();
	void ClockDlgPaint(wxPaintEvent& event);
	void ClockDlgSize(wxSizeEvent& event);
	void WxTimer1Timer(wxTimerEvent& event);
	void RefreshClock();

private:
	wxTimer *WxTimer1;

private:
	enum
	{
		ID_WXTIMER1 = 1001,
		ID_DUMMY_VALUE_ 
	};

private:
	void OnClose(wxCloseEvent& event);
	void CreateGUIControls();
};

#endif
