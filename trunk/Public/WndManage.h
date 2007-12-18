//********************************************
//  project: avgscript 
//  filename: WndManage.h
//  author:   sgch1982@gmail.com 
//  date:     200712
//  descript: CWndManage  class  
//  history: v1.0
//********************************************

#ifndef _WNDMANAGE_H_
#define _WNDMANAGE_H_
#include "CommonWnd.h"

class CWndManage
{
public:

	CWndManage();
	~CWndManage();

	bool AddWnd(CCommonWnd* pWnd);	
	bool DelWnd(CCommonWnd* pWnd);
	void ChangeCurWnd(CCommonWnd* pWnd);
	
	void MouseMove(int x, int y);
	void MouseDown(int x, int y);
	void MouseUp(int x, int y);

	void Draw(_RECT rect);						//draw all wnd object, rect is redraw area
private:

	CCommonWnd* GetLastWnd(CCommonWnd* pWnd);	//get the last wnd 
	CCommonWnd* m_pTopParnetWnd;				//all CCommonWnd object's parent 
	CCommonWnd* m_pCurParentWnd;				//current  Parent CCommonWnd 


};

#endif
