#ifndef _LIBWND_H
#define _LIBWND_H

#include "def.h"
#include "Global.h"
#include <ini.h>
#include "CommonWnd.h"
#include "Dlg.h"
#include "Button.h"

#ifdef WIN32
#pragma comment(lib, "libwnd.lib")
#endif

void Update(_RECT rect = _RECT(0,0,0,0));
void MouseMove(int x, int y);
void MouseDown(int x, int y);
void MouseUp(int x, int y);
void Drag(int x, int y, int nNextDragStatus);



#endif


