//********************************************
//  project: avgscript 
//  filename: cdlg.h
//  author:   sgch1982@gmail.com 
//  date:     200712
//  descript: common dlg class 
//  history: v1.0
//********************************************


#ifndef _DLG_H
#define _DLG_H

#include "def.h"
#include "libnge.h"
#include "Ini.h"
#include "CommonWnd.h"
class CDlg: public CCommonWnd
{
public:
	CDlg();
	~CDlg();

	
	virtual bool CreateTexture(const char* pszFileName);

	virtual void Draw(_RECT rect=_RECT(0,0,0,0));
//	virtual void DragStart();
//	virtual void DragIng();
	virtual void Drag(int x, int y,int nNextDragStatus);
	virtual bool IsPtInDragArea(int x, int y) const;

#ifdef _TEST_ 
	virtual void Test();	
#endif

private:
	
	_RECT m_DragArea;	//this coordinate is relative this dlg 
	

};


#endif