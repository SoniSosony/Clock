#include "Radio.h"
#include "bass.h"
#include <windows.h>
#include <process.h>
#include <stdio.h>

#define BASS_SYNC_HLS_SEGMENT	0x10300
#define BASS_TAG_HLS_EXTINF		0x14000

wxBEGIN_EVENT_TABLE(Radio, wxFrame)
	EVT_BUTTON(10006, ChooseRadioStation)
	EVT_BUTTON(10007, PauseStation)
	EVT_BUTTON(10009, AddRadioStation)
	EVT_BUTTON(10010, RemoveRadioStation)
	EVT_SLIDER(10008, ChangeValime)
wxEND_EVENT_TABLE()

CRITICAL_SECTION lock;

Radio::Radio() : wxFrame(nullptr, wxID_ANY, "Clock main", wxPoint(40, 50), wxSize(725, 245))
{
	std::vector<std::string> Urls = {
		"http://stream-dc1.radioparadise.com/rp_192m.ogg",
		"http://network.absoluteradio.co.uk/core/audio/mp3/live.pls?service=a8bb", "http://network.absoluteradio.co.uk/core/audio/aacplus/live.pls?service=a8",
		"http://somafm.com/secretagent.pls", "http://somafm.com/secretagent32.pls",
		"http://somafm.com/suburbsofgoa.pls", "http://somafm.com/suburbsofgoa32.pls",
		"http://ai-radio.org/256.ogg", "http://ai-radio.org/48.aacp"
	};
	

	panel = new wxPanel(this, -1);
	wxBoxSizer *boxSizer = new wxBoxSizer(wxHORIZONTAL);
	VBoxSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *BtnVBoxSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *HBoxSizer = new wxBoxSizer(wxHORIZONTAL);
	tct_SongName = new wxTextCtrl(panel, 10002, wxEmptyString, wxPoint(0, 0), wxSize(400, 20), 0,
		wxDefaultValidator, wxString("SongName"));
	tct_IcyName = new wxTextCtrl(panel, 10003, wxEmptyString, wxPoint(0, 0), wxSize(400, 20), 0,
		wxDefaultValidator, wxString("IcyName"));
	tct_IcyUrl = new wxTextCtrl(panel, 10004, wxEmptyString, wxPoint(0, 0), wxSize(400, 20), 0,
		wxDefaultValidator, wxString("IcyUrl"));
	l_StationList = new wxListBox(panel, 10005, wxPoint(0, 0), wxSize(300, 200), 0, NULL, 0,
		wxDefaultValidator, wxString("StationList"));
	
	btn_ChooseStation = new wxButton(panel, 10006, wxT("Choose station"));
	btn_Pause = new wxButton(panel, 10007, wxT("Pause"));
	btn_AddStation = new wxButton(panel, 10009, wxT("Add station"));
	btn_RemoveStation = new wxButton(panel, 10010, wxT("Remove station"));
	sl_VolumeLevel = new wxSlider(panel, 10008, 4, 0, 100, wxPoint(0, 0), wxSize(250, 30), wxSL_HORIZONTAL,
		wxDefaultValidator, wxString("Volume"));

	BtnVBoxSizer->Add(btn_ChooseStation);
	BtnVBoxSizer->Add(btn_Pause);
	BtnVBoxSizer->Add(btn_AddStation);
	BtnVBoxSizer->Add(btn_RemoveStation);

	VBoxSizer->AddSpacer(5);
	VBoxSizer->Add(tct_SongName);
	VBoxSizer->Add(tct_IcyName);
	VBoxSizer->Add(tct_IcyUrl);
	VBoxSizer->AddSpacer(10);
	VBoxSizer->Add(BtnVBoxSizer);
	VBoxSizer->Add(sl_VolumeLevel);
	boxSizer->AddSpacer(5);
	boxSizer->Add(VBoxSizer);
	boxSizer->Add(l_StationList);

	panel->SetSizer(boxSizer);

	for (size_t i = 0; i < Urls.size(); i++)
	{
		AddUrlToStationList(Urls.at(i));
	}

	if (l_StationList->GetCount() > 0) {
		l_StationList->SetSelection(0);
		IsRadioBoxEmpty = false;
	}
	else
	{
		IsRadioBoxEmpty = true;
	}
	InitializeCriticalSection(&lock);
	IsPaused = false;
}

Radio::~Radio()
{
	BASS_Stop();
}

void Radio::DoMeta()
{
	tct_IcyUrl->Clear();
	tct_IcyName->Clear();
	tct_SongName->Clear();

	// get the broadcast name and URL
	const char *icy = BASS_ChannelGetTags(chan, BASS_TAG_ICY);
	if (!icy) icy = BASS_ChannelGetTags(chan, BASS_TAG_HTTP); // no ICY tags, try HTTP
	if (icy) {
		for (; *icy; icy += strlen(icy) + 1) {
			if (!_strnicmp(icy, "icy-name:", 9))
				tct_IcyName->WriteText(icy + 9);
			if (!_strnicmp(icy, "icy-url:", 8))
				tct_IcyUrl->WriteText(icy + 8);
		}
	}

	const char *meta = BASS_ChannelGetTags(chan, BASS_TAG_META);
	if (meta) { // got Shoutcast metadata
		const char *p = strstr(meta, "StreamTitle='"); // locate the title
		if (p) {
			const char *p2 = strstr(p, "';"); // locate the end of it
			if (p2) {
				char *t = _strdup(p + 13);
				t[p2 - (p + 13)] = 0;
				tct_SongName->WriteText(t);
				free(t);
			}
		}
	}
	else {
		meta = BASS_ChannelGetTags(chan, BASS_TAG_OGG);
		if (meta) { // got Icecast/OGG tags
			const char *artist = NULL, *title = NULL, *p = meta;
			for (; *p; p += strlen(p) + 1) {
				if (!_strnicmp(p, "artist=", 7)) // found the artist
					artist = p + 7;
				if (!_strnicmp(p, "title=", 6)) // found the title
					title = p + 6;
			}
			if (title) {
				if (artist) {
					char text[100];
					_snprintf(text, sizeof(text), "%s - %s", artist, title);
					tct_SongName->WriteText(text);
				}
				else
					tct_SongName->WriteText(title);
			}
		}
		else {
			meta = BASS_ChannelGetTags(chan, BASS_TAG_HLS_EXTINF);
			if (meta) { // got HLS segment info
				const char *p = strchr(meta, ',');
				if (p) tct_SongName->WriteText(p + 1);
			}
		}
	}
}

void Radio::ChooseRadioStation(wxCommandEvent &evt)
{
	if (!IsRadioBoxEmpty)
	{
		EnterCriticalSection(&lock); // make sure only 1 thread at a time can do the following
		LeaveCriticalSection(&lock);

		int StationIndex = l_StationList->GetSelection();
		std::string url = RadioUrls.at(StationIndex);

		BASS_Start();
		BASS_StreamFree(chan);
		BASS_Init(-1, 44100, BASS_DEVICE_3D, 0, NULL);
		chan = BASS_StreamCreateURL((char*)url.c_str(), 0, 0, NULL, 0);
		EnterCriticalSection(&lock);
		BASS_ChannelPlay(chan, FALSE);
		DoMeta();

		evt.Skip();
	}

}

void Radio::AddUrlToStationList(std::string str)
{
	bool CanAddUrl = true;
	if (RadioUrls.size() == 0)
	{
		RadioUrls.push_back(str);
		CanAddUrl = false;
	}
	else
	{
		for (size_t i = 0; i < RadioUrls.size(); i++)
		{
			if (RadioUrls.at(i).compare(str) == 0)
			{
				CanAddUrl = false;
			}
		}
	}
	
	/*std::string UrlName;
	int count = 0;
	for (size_t i = 0; i < str.length(); i++)
	{
		if (str.at(i) == '/')
		{
			count++;
		}
		if (count == 3)
		{
			UrlName = str.substr(0, i);
			break;
		}
	}*/

	if (CanAddUrl) {
		/*if(count < 3)*/
		l_StationList->Append(str);
		/*else
			l_StationList->Append(UrlName);*/
		RadioUrls.push_back(str);
	}
}

void Radio::UpdateStationList()
{
	l_StationList->Clear();
	for (size_t i = 0; i < RadioUrls.size(); i++)
	{
		l_StationList->Append(RadioUrls.at(i));
	}
}

void Radio::PauseStation(wxCommandEvent &evt)
{
	if (!IsPaused) {
		BASS_Pause();
		btn_Pause->SetLabel("Play");
		IsPaused = true;
	}
	else
	{
		BASS_Start();
		btn_Pause->SetLabel("Pause");
		IsPaused = false;
	}
	evt.Skip();
}

void Radio::ChangeValime(wxCommandEvent & evt)
{
	BASS_SetVolume((float)sl_VolumeLevel->GetValue()/100);
}

void Radio::AddRadioStation(wxCommandEvent & evt)
{
	AddUrlToStationList(tct_IcyUrl->GetLineText(0).ToStdString());
}

void Radio::RemoveRadioStation(wxCommandEvent & evt)
{
	if (!IsRadioBoxEmpty)
	{
		int StationIndex = l_StationList->GetSelection();
		RadioUrls.erase(RadioUrls.begin() + StationIndex);
		UpdateStationList();
	}

}
