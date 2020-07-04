#pragma once
#include <wx/wx.h>
#include <string>

class AlarmClock
{
public:
	AlarmClock();
	~AlarmClock();

	wxDateTime GetAlarmAbsoluteTime();
	wxDateTime GetAlarmAbsoluteDate();
	wxString GetAlarmTime();
	wxString GetAlarmName();
	int GetAlarmType() const;
	bool IsAlarmDeactivate();
	bool IsAlarm();
	bool IsUnuseful();

	void SetAlarmParams(wxString AlarmName, wxDateTime AlarmTime, wxDateTime AlarmDate, int AlarmType);
	void OnAlarm();
	void OffAlarm();
	void DeactivateAlarm(bool deactivate);
	bool IsWasAlarmedToday();
	void DelayAlarmTime(int Delay);
	void SetSongName(std::string str);
	std::string GetSongName() const;

private:
	wxString AlarmName;
	wxDateTime AlarmTime;
	wxDateTime AlarmDate;
	int AlarmType;
	bool AlarmDeactivated;
	bool WasAlarmedToday;
	const wchar_t* SongCallName;
	std::string SongName;
	std::wstring wstr;
};

