#pragma once
#include "wx/wx.h"
#include <windows.h>
#include <process.h>
#include <stdio.h>
#include "bass.h"
#include <vector>
#include <string>

class Radio : public wxFrame
{
public:
	Radio();
	~Radio();

	wxPanel *panel = nullptr;
	HSTREAM chan;

	bool IsPaused;
	bool IsRadioBoxEmpty;

	wxDECLARE_EVENT_TABLE();

private:
	wxBoxSizer *VBoxSizer = nullptr;
	wxTextCtrl *tct_SongName = nullptr;
	wxTextCtrl *tct_IcyName = nullptr;
	wxTextCtrl *tct_IcyUrl = nullptr;
	wxListBox *l_StationList = nullptr;
	wxButton *btn_ChooseStation = nullptr;
	wxButton *btn_Pause = nullptr;
	wxButton *btn_AddStation = nullptr;
	wxButton *btn_RemoveStation = nullptr;
	wxSlider *sl_VolumeLevel = nullptr;
	std::vector<std::string> RadioUrls;

	void DoMeta();
	void AddUrlToStationList(std::string str);
	void UpdateStationList();
	void ChooseRadioStation(wxCommandEvent &evt);
	void PauseStation(wxCommandEvent &evt);
	void ChangeValime(wxCommandEvent &evt);
	void AddRadioStation(wxCommandEvent &evt);
	void RemoveRadioStation(wxCommandEvent &evt);

};

