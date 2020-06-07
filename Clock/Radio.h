#pragma once
#include "wx/wx.h"
#include <windows.h>
#include <process.h>
#include <stdio.h>

class Radio : public wxFrame
{
public:
	Radio();
	~Radio();

private:
	INT_PTR CALLBACK dialogproc(HWND h, UINT m, WPARAM w, LPARAM l);
};

