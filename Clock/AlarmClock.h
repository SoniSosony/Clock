#pragma once
#include <wx/wx.h>

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

private:
	wxString AlarmName;
	wxDateTime AlarmTime;
	wxDateTime AlarmDate;
	int AlarmType;
	bool AlarmDeactivated;
	bool WasAlarmedToday;

	
};

