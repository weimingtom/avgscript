//********************************************
//  project: avgscript 
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

void Update(_RECT rect = _RECT(0,0,0,0));
void MouseMove(int x, int y);
void MouseDown(int x, int y);
void MouseUp(int x, int y);

void Drag(int x, int y, int nNextDragStatus);
bool IsPtInRect(int x, int y, _RECT rect);

#endif