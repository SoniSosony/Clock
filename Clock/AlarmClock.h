#pragma once
#include <wx/wx.h>

class AlarmClock
{

private:
	enum E_AlarmType
	{
		single,
		everyday,
		weekdays,
		weekend
	};

	E_AlarmType AlarmType;
	wxDateTime AlarmDateTime;
	bool RepeatAlarm(E_AlarmType AlarmType);


public:
	AlarmClock();
	AlarmClock(wxDateTime date, E_AlarmType AlarmType);
	~AlarmClock();

	bool alarmDelay(int min);
	bool IsAlarm();
	void chooseMelody();

};

