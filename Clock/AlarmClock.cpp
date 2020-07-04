#include "AlarmClock.h"
#include <wx/calctrl.h>
#include <wx/buffer.h>
#include <Windows.h>
#include <sstream>

AlarmClock::AlarmClock()
{
	AlarmDeactivated = false;
	WasAlarmedToday = false;

	SongCallName = L"open \"E:\\Music\\(Can't Get My) Head Around You.mp3\" type mpegvideo alias mp3";
}

AlarmClock::~AlarmClock()
{
}

wxDateTime AlarmClock::GetAlarmAbsoluteTime()
{

	return AlarmTime;
}

wxDateTime AlarmClock::GetAlarmAbsoluteDate()
{
	return AlarmDate;
}

wxString AlarmClock::GetAlarmTime()
{
	unsigned short hour = AlarmTime.GetHour();
	unsigned short minute = AlarmTime.GetMinute();

	wxString StringHour = wxString::Format(wxT("%d"), (int)hour);
	wxString StringMinute = wxString::Format(wxT("%d"), (int)minute);

	wxString time;
	time.Append(StringHour);
	time.Append(':');
	time.Append(StringMinute);

	return time;
}

wxString AlarmClock::GetAlarmName()
{
	return AlarmName;
}

int AlarmClock::GetAlarmType() const
{
	return AlarmType;
}

bool AlarmClock::IsAlarmDeactivate()
{
	return AlarmDeactivated;
}

bool AlarmClock::IsAlarm()
{
	if (!AlarmDeactivated) {

		if (!IsWasAlarmedToday()) {

			wxDateTime today = wxDateTime::Today();
			wxDateTime now = wxDateTime::Now();

			switch (AlarmType) {
			case 0:				//single
				if (AlarmDate.GetDateOnly() == today)
				{
					if (AlarmTime.GetHour() == now.GetHour() && AlarmTime.GetMinute() == now.GetMinute())
					{
						OnAlarm();
						WasAlarmedToday = true;
						return true;
					}
				}
				break;

			case 1:				//everyday
				if (AlarmTime.GetHour() == now.GetHour() && AlarmTime.GetMinute() == now.GetMinute())
				{
					OnAlarm();
					WasAlarmedToday = true;
					return true;
				}
				break;

			case 2:				//Weekdays
				if (today.GetWeekDay() != (wxDateTime::Sat) && today.GetWeekDay() != (wxDateTime::Sun))
				{
					if (AlarmTime.GetHour() == now.GetHour() && AlarmTime.GetMinute() == now.GetMinute())
					{
						OnAlarm();
						WasAlarmedToday = true;
						return true;
					}
				}
				break;

			case 3:				//Weekend
				if (today.GetWeekDay() == (wxDateTime::Sat) && today.GetWeekDay() == (wxDateTime::Sun))
				{
					if (AlarmTime.GetHour() == now.GetHour() && AlarmTime.GetMinute() == now.GetMinute())
					{
						OnAlarm();
						WasAlarmedToday = true;
						return true;
					}
				}
				break;
			}
		}
	}

	return false;
}

bool AlarmClock::IsUnuseful()
{
	if (AlarmType == 0 && IsWasAlarmedToday())
		return true;

	return false;
}

void AlarmClock::SetAlarmParams(wxString AlarmName, wxDateTime AlarmTime, wxDateTime AlarmDate, int AlarmType)
{
	this->AlarmName = AlarmName;
	this->AlarmTime = AlarmTime;
	this->AlarmDate = AlarmDate;
	this->AlarmType = AlarmType;
	
	AlarmDeactivated = false;
}

void AlarmClock::OnAlarm()
{
	mciSendString(SongCallName, NULL, 0, NULL);
	mciSendString(L"play mp3", NULL, 0, NULL);
}

void AlarmClock::OffAlarm()
{
	mciSendString(L"stop mp3", NULL, 0, NULL);
	mciSendString(L"close mp3", NULL, 0, NULL);
}

void AlarmClock::DeactivateAlarm(bool deactivate = true)
{
	AlarmDeactivated = deactivate;
}

bool AlarmClock::IsWasAlarmedToday()
{
	wxDateTime now = wxDateTime::Now();

	if (WasAlarmedToday)
	{
		if (now.GetDay() != AlarmDate.GetDay())
		{
			AlarmDate.SetMonth(now.GetMonth());
			AlarmDate.SetDay(now.GetDay());
			WasAlarmedToday = false;
			return false;
		}
		return true;
	}


	return false;
}

void AlarmClock::DelayAlarmTime(int Delay)
{
	WasAlarmedToday = false;

	unsigned short minutes = AlarmTime.GetMinute();
	unsigned short hour = AlarmTime.GetHour();
	if (minutes > 59 - Delay)
	{
		if (hour != 23)
			AlarmTime.SetHour(AlarmTime.GetHour() + 1);
		else
			AlarmTime.SetHour(0);
	}
	else
	{
		AlarmTime.SetMinute(minutes + Delay);
	}
}

void AlarmClock::SetSongName(std::string str)
{
	std::wstringstream wss;
	wss << "open \"" << str << "\" type mpegvideo alias mp3";
	wstr.append(wss.str());
	SongName = str;
	SongCallName = wstr.c_str();
}

std::string AlarmClock::GetSongName() const
{
	return SongName;
}
