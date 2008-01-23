//********************************************
//  project: libwnd 
//  filename: CommonWnd.h
//  author:   sgch1982@gmail.com 
//  date:     200712
//  descript: CCommonWnd class  base class for CDlg, Cbutton ... 
//  history: v1.0
//********************************************

#ifndef _COMMONWND_H_
#define _COMMONWND_H_
#include "Global.h"

 #define MAX_WND_NAME_LEN 256

#define Drag_Status_None 1
#define Drag_Status_Starting 2


class CCommonWnd
{
public:
	CCommonWnd();

	virtual ~CCommonWnd();

	
	IVideoDriver* GetVideoDriver() const ;
	int GetWidth() const;
	int GetHeight() const;
	int GetLeft() const;
	int GetTop() const;
	int GetDisplayTop() const;
	int GetDisplayLeft() const;
	int GetAlpha() const;
	int GetZorder() const;
	int GetID() const;
	
	bool GetIsContainer() const;
	bool GetCanMove() const;
	bool GetCanDrag() const;
	const char* GetWndName() const;
	const char* GetWndCaption() const;
	const char* GetWndTextureFileName() const;
	const char* GetWndSZKind() const;
	void SetWndName(const char* pszWndName);
	void SetWndCaption(const char* pszWndCaption);

	void SetWndTextureFileName(const char* pszTextureFileName);
	void SetWndSZKind(const char* pszKind);
	void SetWidth(int nWidth);
	void SetHeight(int nHeight);
	void SetLeft(int nLeft);
	void SetTop(int nTop);
	void SetDisplayTop(int nTop);
	void SetDisplayLeft(int nLeft);
	void SetAlpha(int nAlpha);
	void SetZOrder(int nZOrder);
	void SetIsContainer(bool bContainer);
	void SetID(int nID);
	CCommonWnd* GetParent() const;
	CCommonWnd* GetChild()  const;
	CCommonWnd* GetNext() const;
	CCommonWnd* GetPrev() const;
	void  SetParent(CCommonWnd* pParent);
	void  SetChild(CCommonWnd* pChild);
	void  SetNext(CCommonWnd* pNext);
	void  SetPrev(CCommonWnd* pPrev);

	void SetCanMove(bool bCanMove);
	void SetCanDrag(bool bCanDrag);
	CTexture* GetTexture() const;
	bool  IsPtInArea(int x, int y) const;
	bool  IsKindOf(const char* pszKindName) const;

	void ChangeDragStatus(int nNextStatus);


	//virtual function
	virtual bool CreateTexture(const char* pszFileName);
	virtual void Release();
	virtual bool IsPtInDragArea(int x, int y) const;

	virtual void Draw(_RECT rect = _RECT(0,0,0,0)) ;
	virtual void Move(int nOffsetX, int nOffsetY) ;
	
//	virtual void DragStart(int x, int y);
//	virtual void DragIng(int x, int y);
	virtual void Drag(int x, int y, int nNextDragStatus);
	//mouse
	virtual void MouseMove(int x , int y);
	virtual void MouseDown(int x, int y);
	virtual void MouseUp(int x, int y);


	//hit test
	virtual bool HitTest(int x ,int y);

private:

	int           m_nWndID;		
	
	int			  m_nWidth;			
	int           m_nHeight;
	int           m_nLeft;		//this left and top is relate to his parent
	int           m_nTop;

	int           m_nDisplayLeft;
	int           m_nDisplayTop;	//this is relate to display
	int			  m_nAlpha;
	int           m_nDragStatus;	
	int           m_nStartDragX;
	int           m_nStartDragY;

	int           m_nZorder;		//z-order 
	bool          m_bContainer;		//
	bool          m_bCanMove;
	bool          m_bCanDrag;
	CCommonWnd*   m_pParent;
	CCommonWnd*   m_pChild;
	CCommonWnd*   m_pNext;
	CCommonWnd*   m_pPrev;
	IVideoDriver* m_pVideoDriver;
	CTexture*     m_pBackTexture;	//back ground texture 

	char          m_szWndName[MAX_WND_NAME_LEN];
	char          m_szWndCaption[MAX_WND_NAME_LEN];
	char          m_szWndTextureFileName[MAX_WND_NAME_LEN];
	char          m_szWndKind[MAX_WND_NAME_LEN];
};

#endif