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

	

	virtual void Draw();

#ifdef _TEST_ 
	virtual void Test();	
#endif

private:
	

	

};


#endif