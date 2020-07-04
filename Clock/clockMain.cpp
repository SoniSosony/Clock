#include "clockMain.h"
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <wx/grid.h>
#include "bass.h"

wxBEGIN_EVENT_TABLE(clockMain, wxFrame)
	EVT_BUTTON(10001, AddAlarmButtonClicked)
	EVT_BUTTON(10004, DeleteAlarmButtonClicked)
	EVT_BUTTON(10005, DisableAlarmButtonClicked)
	EVT_BUTTON(10006, EditAlarmButtonClicked)
	EVT_BUTTON(10008, RadioButtonClicked)
	EVT_BUTTON(10011, NightMode)
	EVT_TIMER(10002, clockMain::WxTimer1Timer)
	EVT_RADIOBUTTON(10009, LockButtons)
	EVT_RADIOBUTTON(10010, UnlockButtons)
wxEND_EVENT_TABLE()


clockMain::clockMain() : wxFrame(nullptr, wxID_ANY, "Clock main", wxPoint(40, 50), wxSize(800, 600))
{

	panel = new wxPanel(this, -1);
	//panel->SetBackgroundColour(wxColor(*wxBLACK));
	wxBoxSizer *boxSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *ClockBoxSizer = new wxBoxSizer(wxVERTICAL);
	wxGridSizer *LockGridSizer = new wxGridSizer(2, 4, 3, 3);
	wxPanel *clockPanel = new wxPanel(panel);
	w_clock = new ClockDlg(clockPanel);
	boxSizer->Add(ClockBoxSizer);
	ClockBoxSizer->Add(clockPanel);
	ClockBoxSizer->Add(LockGridSizer);
	w_clock->Show(true);

	wxBoxSizer *AlarmBoxSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *ButtonBoxSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *VolumeBoxSizer = new wxBoxSizer(wxHORIZONTAL);

	l_AlarmList = new wxListBox(panel, 10003, wxPoint(0, 0), wxSize(335, 400), 0, NULL, 0, 
		wxDefaultValidator, wxString("Alarms"));

	btn_AddAlarm = new wxButton(panel, 10001, wxT("Add alarm"));
	btn_DeleteAlarm = new wxButton(panel, 10004, wxT("Delete alarm"));
	btn_DisableAlarm = new wxButton(panel, 10005, wxT("Enable/Disable"));
	btn_EditAlarm = new wxButton(panel, 10006, wxT("Edit alarm"));
	btn_Lock = new wxRadioButton(panel, 10009, wxString("Lock"), wxPoint(0, 0), wxDefaultSize, 0,
		wxDefaultValidator, wxString("Lock"));
	btn_Unlock = new wxRadioButton(panel, 10010, wxString("Unlock"), wxPoint(0, 0), wxDefaultSize, 0,
		wxDefaultValidator, wxString("Lock"));
	btn_NightMode = new wxButton(panel, 10011, wxT("On Night mode"));

	wxStaticText *VolumeText = new wxStaticText(panel, wxID_ANY, wxString("Volume"), wxPoint(0,0), 
		wxSize(40, 30), 0, wxString("Volume level"));
	sl_VolumeLevel = new wxSlider(panel, 10007, 4, 0, 100, wxPoint(0, 0), wxSize(295, 30), wxSL_HORIZONTAL, 
		wxDefaultValidator, wxString("Volume"));

	btn_Radio = new wxButton(panel, 10008, wxT("Radio"));

	LockGridSizer->Add(btn_NightMode);
	LockGridSizer->Add(btn_Lock);
	LockGridSizer->Add(btn_Unlock);

	AlarmBoxSizer->Add(l_AlarmList);

	ButtonBoxSizer->Add(btn_AddAlarm);
	ButtonBoxSizer->Add(btn_DeleteAlarm);
	ButtonBoxSizer->Add(btn_DisableAlarm);
	ButtonBoxSizer->Add(btn_EditAlarm);

	VolumeBoxSizer->Add(VolumeText);
	VolumeBoxSizer->Add(sl_VolumeLevel);

	AlarmBoxSizer->Add(ButtonBoxSizer);
	AlarmBoxSizer->Add(VolumeBoxSizer);
	AlarmBoxSizer->Add(btn_Radio);
	boxSizer->Add(AlarmBoxSizer);

	WxTimer1 = new wxTimer();
	WxTimer1->SetOwner(this, 10002);
	WxTimer1->Start(200);

	panel->SetSizer(boxSizer);

	IsNightModeOn = false;
}

clockMain::~clockMain()
{
}

void clockMain::AddToAlarmClockList(AlarmClock &alarm)
{
	AlarmClockList.push_back(&alarm);
}

void clockMain::WxTimer1Timer(wxTimerEvent & event)
{
	w_clock->RefreshClock();

	for (int i = 0; i < AlarmClockList.size(); i++)
	{
		AlarmClock* alarm = AlarmClockList.at(i);
		if (alarm->IsAlarm())
		{
			this->Disable();

			float volume = (float)sl_VolumeLevel->GetValue() / 100;
			float CurrentVolume = ChangeVolume(volume, true);

			wxArrayString *AlarmDelayChoises = new wxArrayString();
			AlarmDelayChoises->Add(wxString("5"));
			AlarmDelayChoises->Add(wxString("10"));
			AlarmDelayChoises->Add(wxString("15"));

			wxSingleChoiceDialog *ChoiceDialog = new wxSingleChoiceDialog(this, AlarmClockList.at(i)->GetAlarmName(), 
				wxString("Caption"), *AlarmDelayChoises, NULL, wxCHOICEDLG_STYLE, wxDefaultPosition);
			if (ChoiceDialog->ShowModal() == wxID_OK)
			{
				alarm->OffAlarm();

				int SelectedDelay = ChoiceDialog->GetSelection();

				switch (SelectedDelay)
				{
				case 0:
					alarm->DelayAlarmTime(5);
					break;
				case 1:
					alarm->DelayAlarmTime(10);
					break;
				case 2:
					alarm->DelayAlarmTime(15);
					break;
				default:
					break;
				}

				UpdateListBox();
				this->Enable();
				this->SetFocus();
			}
			else
			{
				alarm->OffAlarm();
				if (alarm->IsUnuseful())
				{
					DeleteAlarmFromList(*alarm);
				}

				ChangeVolume(CurrentVolume, true);
				UpdateListBox();
				this->Enable();
				this->SetFocus();
			}
		}
	}
}

void clockMain::AddAlarmButtonClicked(wxCommandEvent & evt)
{
	AlarmClockPanel *AlarmPanel = new AlarmClockPanel(*this);
	AlarmPanel->Show();
	this->Disable();
	evt.Skip();
}

void clockMain::DeleteAlarmButtonClicked(wxCommandEvent & evt)
{
	AlarmClock *alarm = AlarmClockList.at(l_AlarmList->GetSelection());
	DeleteAlarmFromList(*alarm);
	UpdateListBox();
	evt.Skip();
}

void clockMain::DisableAlarmButtonClicked(wxCommandEvent & evt)
{
	AlarmClock *alarm = AlarmClockList.at(l_AlarmList->GetSelection());
	if (alarm->IsAlarmDeactivate())
	{
		alarm->DeactivateAlarm(false);
	}
	else
	{
		alarm->DeactivateAlarm(true);
	}

	UpdateListBox();
	evt.Skip();
}

void clockMain::EditAlarmButtonClicked(wxCommandEvent & evt)
{
	AlarmClock *alarm = AlarmClockList.at(l_AlarmList->GetSelection());
	AlarmClockPanel *AlarmPanel = new AlarmClockPanel(*this, *alarm);
	AlarmPanel->Show();
	this->Disable();
	evt.Skip();
}

void clockMain::RadioButtonClicked(wxCommandEvent & evt)
{
	Radio *radio = new Radio();
	radio->Show();
	evt.Skip();
}

void clockMain::LockButtons(wxCommandEvent & evt)
{

	btn_AddAlarm->Disable();
	btn_DeleteAlarm->Disable();
	btn_DisableAlarm->Disable();
	btn_EditAlarm->Disable();
	btn_Radio->Disable();
	sl_VolumeLevel->Disable();
}

void clockMain::UnlockButtons(wxCommandEvent & evt)
{
	btn_AddAlarm->Enable();
	btn_DeleteAlarm->Enable();
	btn_DisableAlarm->Enable();
	btn_EditAlarm->Enable();
	btn_Radio->Enable();
	sl_VolumeLevel->Enable();
}

void clockMain::NightMode(wxCommandEvent & evt)
{
	if (!IsNightModeOn) {
		SetMinSize(wxSize(416, 462));
		SetMaxSize(wxSize(416, 462));
		SetSize(wxSize(416, 462));

		w_clock->OnDarkMode();
		//this->SetBackgroundColour(wxColor(80, 80, 80));

		btn_NightMode->SetLabel("Off Dark Mode");
		btn_NightMode->SetBackgroundColour(wxColor(65, 65, 65));
		btn_NightMode->SetForegroundColour(wxColor(255, 255, 255));
		btn_Lock->Show(false);
		btn_Unlock->Show(false);

		IsNightModeOn = true;
	}
	else
	{
		SetMinSize(wxSize(40, 40));
		SetMaxSize(wxSize(2000, 1200));
		SetSize(wxSize(800, 600));

		w_clock->OnDarkMode();

		btn_NightMode->SetLabel("On Night Mode");
		btn_NightMode->SetBackgroundColour(wxColor(255, 255, 255));
		btn_NightMode->SetForegroundColour(wxColor(0, 0, 0));
		btn_Lock->Show(true);
		btn_Unlock->Show(true);

		IsNightModeOn = false;
	}
}

float clockMain::ChangeVolume(double nVolume, bool bScalar)
{
	HRESULT hr = NULL;
	bool decibels = false;
	bool scalar = false;
	double newVolume = nVolume;

	CoInitialize(NULL);
	IMMDeviceEnumerator *deviceEnumerator = NULL;
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
	IMMDevice *defaultDevice = NULL;

	hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
	deviceEnumerator->Release();
	deviceEnumerator = NULL;

	IAudioEndpointVolume *endpointVolume = NULL;
	hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume),
		CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
	defaultDevice->Release();
	defaultDevice = NULL;

	float currentVolume = 0;
	endpointVolume->GetMasterVolumeLevel(&currentVolume);

	hr = endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);

	if (bScalar == false)
	{
		hr = endpointVolume->SetMasterVolumeLevel((float)newVolume, NULL);
	}
	else if (bScalar == true)
	{
		hr = endpointVolume->SetMasterVolumeLevelScalar((float)newVolume, NULL);
	}
	endpointVolume->Release();

	CoUninitialize();

	return currentVolume;
}

void clockMain::UpdateListBox()
{
	l_AlarmList->Clear();
	for (size_t i = 0; i < AlarmClockList.size(); i++)
	{
		AlarmClock * alarm = AlarmClockList.at(i);
		wxString CellName = alarm->GetAlarmName();
		CellName.Append(' ');
		CellName.Append(alarm->GetAlarmTime());
		if (alarm->IsAlarmDeactivate())
		{
			CellName.Append(" Disable");
		}
		else
		{
			CellName.Append(" Enable");
		}
		l_AlarmList->Append(CellName);
	}
}

void clockMain::DeleteAlarmFromList(AlarmClock & alarm)
{
	for (size_t i = 0; i < AlarmClockList.size(); i++)
	{
		if (AlarmClockList.at(i) == &alarm)
			AlarmClockList.erase(AlarmClockList.begin() + i);
	}

	delete &alarm;
}

std::vector<AlarmClock*> clockMain::GetAlarmClockList() const
{
	return AlarmClockList;
}
