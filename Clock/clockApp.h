#pragma once

#include "wx/wx.h"
#include "clockMain.h"

class clockApp : public wxApp
{

public:
	virtual bool OnInit();

private:
	clockMain* m_frame1 = nullptr;

};