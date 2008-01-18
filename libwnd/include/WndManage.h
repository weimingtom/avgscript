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


#define MAX_WND_VALUE_LEN 256	//max value len in xxx.ini file
#define Wnd_Default_IntValue 0xffffff  


#define Wnd_Index		"Wnd"
#define Wnd_Key_Count   "WndCount"
#define Wnd_Key_Kind	"WndKind"
#define Wnd_Key_Name	"WndName"
#define Wnd_Key_Caption "WndCaption"
#define Wnd_Key_TextureFile "TextureFile"
#define Wnd_Key_ID		"ID"
#define Wnd_Key_X       "X"
#define Wnd_Key_Y       "Y"
#define Wnd_Key_Width   "Width"
#define Wnd_Key_Height	"Height"
#define Wnd_Key_CanDrag "CanDrag"
#define Wnd_Key_CanMove "CanMove"

#define WndKind_UnKnown 0xffffff
#define WndKind_Dlg		0
#define WndKind_Button  1
#define WndKind_Text	2
#define WndKind_Label	3
#define WndKind_Combo	4
#define WndKind_Scroll	5
#define WndKind_Menu	6
#define WndKind_List	7
#define WndKind_Radio	8
#define WndKind_CheckBox 9

#define szWndKind_UnKnown "WndKind_UnKnown"
#define szWndKind_Dlg     "WndKind_Dlg"
#define szWndKind_Button  "WndKind_Button"
#define szWndKind_Text	  "WndKind_Text"
#define szWndKind_Label   "WndKind_Label"
#define szWndKind_Combo	  "WndKind_Combo"
#define szWndKind_Scroll  "WndKind_Scroll"
#define szWndKind_Menu    "WndKind_Menu"
#define szWndKind_List    "WndKind_List"
#define szWndKind_Radio   "WndKind_Radio"
#define szWndKind_CheckBox "WndKind_CheckBox"



class CWndManage
{
public:

	CWndManage();
	~CWndManage();

	bool AddWnd(CCommonWnd* pWnd);	
	bool DelWnd(CCommonWnd* pWnd);
	void ChangeCurWnd(CCommonWnd* pWnd);
	void ActiveWnd(CCommonWnd* pWnd);
	
	void MouseMove(int x, int y);
	void MouseDown(int x, int y);
	void MouseUp(int x, int y);
	CCommonWnd* HitTest(int x, int y, CCommonWnd* pWndParent=NULL);

	void Drag(int x, int y, int nNextDragStatus);
	void Draw(_RECT rect);						//draw all wnd object, rect is redraw area
	/*
	 [CWnd]
	  CWndCount = 1     
	 [CWnd0]
	  Name =  windows1
	  .....




	*/
	bool LoadWnd(const char* pszFileName);		//load script from file
	
	bool SaveWnd(const char* pszFileName);		//save to file
private:


	int szKindToIntKind(const char* pszKind);	//change str kind to int kind

	CCommonWnd* GetLastWnd(CCommonWnd* pWnd);	//get the last wnd 
	CCommonWnd* GetFirstWnd(CCommonWnd* pWnd);
	CCommonWnd* m_pTopParnetWnd;				//all CCommonWnd object's parent 
	CCommonWnd* m_pCurParentWnd;				//current  Parent CCommonWnd 

	CCommonWnd* m_pActiveWnd;					
};

#endif
