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

void MouseMove(int x, int y)
{
	g_WndManage.MouseMove(x, y);
}

void MouseDown(int x, int y)
{
	g_WndManage.MouseDown(x,y);
}
void MouseUp(int x, int y)
{
	g_WndManage.MouseUp(x,y);
}

void Drag(int x, int y, int nNextDragStatus)
{

	
	g_WndManage.Drag(x, y, nNextDragStatus);
}


bool IsPtInRect(int x, int y,_RECT rect)
{
	if( rect.left < x && 
		x <rect.left + rect.width  &&
		rect.top < y &&
		y < rect.top + rect.height)
	{
		return true;
	}
	else
		return false;
}