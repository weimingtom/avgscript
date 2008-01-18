//********************************************
//  project: libwnd 
//  filename: button.h
//  author:   sgch1982@gmail.com 
//  date:     200712
//  descript: common button class 
//  history: v1.0
//********************************************
#ifndef _BUTTON_H_
#define _BUTTON_H_
#include "CommonWnd.h"

#define BUTTON_STATUS_COUNT 5   //one pic contain 5 button status
#define BUTTON_STATUS_NORMAL 0	
#define BUTTON_STATUS_CLICK 1
#define	BUTTON_STATUS_FOCUS 2
#define BUTTON_STATUS_MOUSEON 3
#define BUTTON_STATUS_DISABLE 4


class Cbutton :public CCommonWnd
{
public:
	Cbutton();
	~Cbutton();

	void ChangeStatue(int nNextStatus);
	
	virtual bool CreateTexture(const char* pszFileName);
	virtual void Draw(_RECT rect = _RECT(0,0,0,0)) ;
	virtual void Move(int nOffsetX, int nOffsetY);
	virtual void MouseMove(int x , int y);
	virtual void MouseDown(int x, int y);
	virtual void MouseUp(int x, int y);

private:



	int m_nButtonStatus;
};


#endif