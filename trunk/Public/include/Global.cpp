#include "Global.h"
#include "WndManage.h"


CWndManage    g_WndManage;
IVideoDriver* g_pVideoDriver;

IVideoDriver* GetVideoDriver()
{
	return g_pVideoDriver;
}

void Update(_RECT rect )
{
	g_WndManage.Draw(rect);
}
