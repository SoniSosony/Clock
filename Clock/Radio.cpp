#include "Radio.h"
#include "bass.h"


#define BASS_SYNC_HLS_SEGMENT	0x10300
#define BASS_TAG_HLS_EXTINF		0x14000

HWND win = NULL;
CRITICAL_SECTION lock;
DWORD req = 0;
HSTREAM chan;

Radio::Radio() : wxFrame(nullptr, wxID_ANY, "Clock main", wxPoint(40, 50), wxSize(800, 600))
{

}

Radio::~Radio()
{
}

#define MESS(id,m,w,l) SendDlgItemMessage(win,id,m,(WPARAM)(w),(LPARAM)(l))

void Error(const char *es)
{
	char mes[200];
	sprintf(mes, "%s\n(error code: %d)", es, BASS_ErrorGetCode());
	MessageBox(win, (LPCWSTR)mes, 0, 0);
}

const char *urls[10] = {
	"http://stream-dc1.radioparadise.com/rp_192m.ogg", "http://www.radioparadise.com/m3u/mp3-32.m3u",
	"http://network.absoluteradio.co.uk/core/audio/mp3/live.pls?service=a8bb", "http://network.absoluteradio.co.uk/core/audio/aacplus/live.pls?service=a8",
	"http://somafm.com/secretagent.pls", "http://somafm.com/secretagent32.pls",
	"http://somafm.com/suburbsofgoa.pls", "http://somafm.com/suburbsofgoa32.pls",
	"http://ai-radio.org/256.ogg", "http://ai-radio.org/48.aacp"
};

void CALLBACK MetaSync(HSYNC handle, DWORD channel, DWORD data, void *user)
{
	//DoMeta();
}

void CALLBACK StallSync(HSYNC handle, DWORD channel, DWORD data, void *user)
{
	if (!data) // stalled
		SetTimer(win, 0, 50, 0); // start buffer monitoring
}

void CALLBACK EndSync(HSYNC handle, DWORD channel, DWORD data, void *user)
{
	KillTimer(win, 0); // stop buffer monitoring
	MESS(31, WM_SETTEXT, 0, "not playing");
	MESS(30, WM_SETTEXT, 0, "");
	MESS(32, WM_SETTEXT, 0, "");
}

void CALLBACK StatusProc(const void *buffer, DWORD length, void *user)
{
	if (buffer && !length && (DWORD)user == req) // got HTTP/ICY tags, and this is still the current request
		MESS(32, WM_SETTEXT, 0, buffer); // display status
}

void __cdecl OpenURL(void *url)
{
	DWORD c, r;
	EnterCriticalSection(&lock); // make sure only 1 thread at a time can do the following
	r = ++req; // increment the request counter for this request
	LeaveCriticalSection(&lock);
	KillTimer(win, 0); // stop buffer monitoring
	BASS_StreamFree(chan); // close old stream
	c = BASS_StreamCreateURL((char *)url, 0, BASS_STREAM_BLOCK | BASS_STREAM_STATUS | BASS_STREAM_AUTOFREE, StatusProc, (void*)r); // open URL
	free(url); // free temp URL buffer
	EnterCriticalSection(&lock);
	if (r != req) { // there is a newer request, discard this stream
		LeaveCriticalSection(&lock);
		if (c) BASS_StreamFree(c);
		return;
	}
	chan = c; // this is now the current stream
	LeaveCriticalSection(&lock);
	if (!chan) { // failed to open
		MESS(31, WM_SETTEXT, 0, "not playing");
		Error("Can't play the stream");
	}
	else {
		// start buffer monitoring
		SetTimer(win, 0, 50, 0);
		// set syncs for stream title updates
		BASS_ChannelSetSync(chan, BASS_SYNC_META, 0, MetaSync, 0); // Shoutcast
		BASS_ChannelSetSync(chan, BASS_SYNC_OGG_CHANGE, 0, MetaSync, 0); // Icecast/OGG
		BASS_ChannelSetSync(chan, BASS_SYNC_HLS_SEGMENT, 0, MetaSync, 0); // HLS
		// set sync for stalling/buffering
		BASS_ChannelSetSync(chan, BASS_SYNC_STALL, 0, StallSync, 0);
		// set sync for end of stream
		BASS_ChannelSetSync(chan, BASS_SYNC_END, 0, EndSync, 0);
		// play it!
		BASS_ChannelPlay(chan, FALSE);
	}
}

INT_PTR Radio::dialogproc(HWND h, UINT m, WPARAM w, LPARAM l)
{
	return INT_PTR();
}
