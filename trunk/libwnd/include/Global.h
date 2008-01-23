//********************************************
//  project: libwnd 
//  filename: Global.h
//  author:   sgch1982@gmail.com 
//  date:     200712
//  descript: global function and variable 
//  history: v1.0
//********************************************

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "libnge.h"
#include "def.h"
//variable
class CWndManage;

extern IVideoDriver* g_pVideoDriver;
extern CWndManage    g_WndManage;

IVideoDriver* GetVideoDriver();

bool IsPtInRect(int x, int y, _RECT rect);



#endif