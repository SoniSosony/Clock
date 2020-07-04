#include "ClockDlg.h"

BEGIN_EVENT_TABLE(ClockDlg,wxControl)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(ClockDlg::OnClose)
	EVT_SIZE(ClockDlg::ClockDlgSize)
	EVT_PAINT(ClockDlg::ClockDlgPaint)
END_EVENT_TABLE()
////Event Table End

ClockDlg::ClockDlg(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style, const wxValidator& val, const wxString& name)
{
	//CreateGUIControls();
	wxSize *sizeT = new wxSize();
	sizeT->Set(400, 400);

	background = wxColour(255, 255, 255);
	IsDarkModeOn = false;
	Create(parent, id, position, *sizeT, style, val, name);

}

ClockDlg::~ClockDlg()
{
} 

void ClockDlg::CreateGUIControls()
{
	SetSize(8,8,509,412);
	Center();
	

	WxTimer1 = new wxTimer();
	WxTimer1->SetOwner(this, ID_WXTIMER1);
	WxTimer1->Start(200);
}

void ClockDlg::OnDarkMode()
{
	if (!IsDarkModeOn)
	{
		background = wxColor(70, 70, 70);
		IsDarkModeOn = true;
	}
	else
	{
		background = wxColor(255, 255, 255);
		IsDarkModeOn = false;
	}
}

void ClockDlg::OnClose(wxCloseEvent& /*event*/)
{
	Destroy();
}

/*
 * Convert a float value to int
 */
int toInt(float value)
{
    return static_cast<int>(value);
}

/*
 * ClockDlgPaint
 */
void ClockDlg::ClockDlgPaint(wxPaintEvent& event)
{
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
    //wxPaintDC dc(this);
    wxBufferedPaintDC dc(this);

    // Get window dimensions
    wxSize sz = GetClientSize();

    wxPoint center = wxPoint(sz.x / 2, sz.y / 2);
    wxCoord radius = wxMin(sz.x / 2 - 20, sz.y / 2 - 20);
    radius = wxMax(radius, 50);

    float factor = wxMax(radius/60.0, 1.0);

    // create a few colors
    wxColour lightblue = wxColour(220, 245, 255);
    wxColour darkblue = wxColour(0, 0, 120);

    // draw background
    dc.SetPen(background);
    dc.SetBrush(background);
    dc.DrawRectangle(0, 0, sz.x, sz.y);

    // draw clock border
    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxBLACK_BRUSH);
    dc.DrawCircle(center, radius);
    dc.SetPen(*wxBLACK_PEN);
	if (IsDarkModeOn)
		dc.SetBrush(*wxGREY_BRUSH);
	else
		dc.SetBrush(*wxWHITE_BRUSH);
    dc.DrawCircle(center, toInt(radius - 3.0 * factor));

    // paint lines for minutes
    float pi = 3.141592654;
    float r_outer = radius - 5 * factor;
    float r_inner_min = radius - 7 * factor;
    float r_inner_hour = radius - 10 * factor;
    float r_inner_text = radius - 15 * factor;
    int linewidth = 1;
    for (int r = 1; r <= 60; r ++)
    {
        
        float s = sin(r/60.0*2.0*pi);
        float c = -cos(r/60.0*2.0*pi);
        
        linewidth = wxMax(toInt(factor / 2), 1);
        dc.SetPen(wxPen(*wxBLACK, linewidth));
        wxPoint outer = wxPoint(toInt(r_outer * s), toInt(r_outer * c));
        wxPoint inner = wxPoint(toInt(r_inner_min * s), toInt(r_inner_min * c));
        dc.DrawLine(center + inner, center + outer);
        
        if (r % 5 == 0)
        {
            linewidth = wxMax(toInt(factor), 1);
            dc.SetPen(wxPen(*wxBLACK, linewidth));
    
            // paint larger lines for the hours
            outer = wxPoint(toInt(r_outer * s), toInt(r_outer * c));
            inner = wxPoint(toInt(r_inner_hour * s), toInt(r_inner_hour * c));
            dc.DrawLine(center + inner, center + outer);

            // paint value of the hour
            wxString hour;
            hour.Printf("%i", r / 5);
            
            dc.SetFont(wxFont(toInt(5 * factor), wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, "tahoma", wxFONTENCODING_DEFAULT));
            
            // calculate the size of the text and paint it in the window
            wxCoord w = 0, h = 0;
            dc.GetTextExtent(hour, &w, &h);
            wxPoint text_pos = wxPoint(toInt(r_inner_text * s - w / 2), toInt(r_inner_text * c - h / 2));
            dc.DrawText(hour, center + text_pos);
            //dc.DrawRotatedText(hour, center.x + text_pos.x, center.y + text_pos.y, (60-r)/60.0*360);  // cool :)
        }
    }
    
    // draw hour, minute and second hand of the clock
    wxDateTime time = wxDateTime::Now();    
    int h = time.GetHour();
    int m = time.GetMinute();
    int s = time.GetSecond();

    int r = 1;
/*    int h_deg = toInt((h % 12) / 12.0 * 360);
    int m_deg = toInt(m / 60.0 * 360.0);
    int s_deg = toInt(s / 60.0 * 360.0);*/
    float h_rad = (h % 12 + m / 60.0 + s / 3600.0) / 12.0 * 2.0 * pi;
    float m_rad = (m / 60.0 + s / 3600.0) * 2.0 * pi;    
    float s_rad = s / 60.0 * 2.0 * pi;

    // show digital time in caption
    wxString title;
    title.Printf("Clock %i:%.2i:%.2i", h, m, s);
    //SetTitle(title);

    r = toInt(factor * 20);
    linewidth = wxMax(toInt(factor * 5), 1);
    dc.SetPen(wxPen(*wxBLACK, linewidth));
    dc.DrawLine(center, center + wxPoint(toInt(r * sin(h_rad)), toInt(r * -cos(h_rad))));

    r = toInt(factor * 40);
    linewidth = wxMax(toInt(factor * 2), 1);
    dc.SetPen(wxPen(*wxBLACK, linewidth));
    dc.DrawLine(center, center + wxPoint(toInt(r * sin(m_rad)), toInt(r * -cos(m_rad))));

    r = toInt(factor * 50);
    linewidth = wxMax(toInt(factor), 1);
    dc.SetPen(wxPen(*wxRED, linewidth));
    dc.DrawLine(center, center + wxPoint(toInt(r * sin(s_rad)), toInt(r * -cos(s_rad))));

    // credits for wxWidgets
    //dc.SetTextForeground(darkblue);
    //dc.SetFont(wxFont(8, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, "arial", wxFONTENCODING_DEFAULT));
    //dc.DrawRotatedText("Created with wxWidgets", toInt(sz.x - 5 - 10), toInt(sz.y - 5), 90);
}

/*
 * ClockDlgSize
 */
void ClockDlg::ClockDlgSize(wxSizeEvent& event)
{
	Refresh();
}

/*
 * WxTimer1Timer
 */
void ClockDlg::RefreshClock()
{
    Refresh();
}