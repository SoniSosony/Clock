#include "AlarmClock.h"
#include <wx/calctrl.h>

bool AlarmClock::RepeatAlarm(E_AlarmType AlarmType)
{
	switch (AlarmType)
	{
	case single:
		break;

	case everyday:
		break;

	case weekdays:
		break;

	case weekend:
		break;

	default:
		break;
	}
	return false;
}

AlarmClock::AlarmClock()
{

}

AlarmClock::AlarmClock(wxDateTime date, E_AlarmType AlarmType)
{
}

bool AlarmClock::IsAlarm()
{

	switch (AlarmType)
	{
	case single:
		break;

	case everyday:
		break;

	case weekdays:
		break;

	case weekend:
		break;

	default:
		break;
	}
	return false;

	return false;
}

