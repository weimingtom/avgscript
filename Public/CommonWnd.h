//********************************************
//  project: avgscript 
//  filename: CommonWnd.h
//  author:   sgch1982@gmail.com 
//  date:     200712
//  descript: CCommonWnd class  base class for CDlg, Cbutton ... 
//  history: v1.0
//********************************************

#ifndef _COMMONWND_H_
#define _COMMONWND_H_
#include "Global.h"

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

	void SetWidth(int nWidth);
	void SetHeight(int nHeight);
	void SetLeft(int nLeft);
	void SetTop(int nTop);
	void SetDisplayTop(int nTop);
	void SetDisplayLeft(int nLeft);
	
	CCommonWnd* GetParent() const;
	CCommonWnd* GetChild()  const;
	CCommonWnd* GetNext() const;
	CCommonWnd* GetPrev() const;
	void  SetParent(CCommonWnd* pParent);
	void  SetChild(CCommonWnd* pChild);
	void  SetNext(CCommonWnd* pNext);
	void  SetPrev(CCommonWnd* pPrev);
	CTexture* GetTexture() const;
	bool  IsPtInArea(int x, int y) const;



	//virtual function
	virtual bool CreateTexture(const char* pszFileName);
	virtual void Release();

	virtual void Draw(_RECT rect = _RECT(0,0,0,0)) ;
	virtual void Move(int nOffsetX, int nOffsetY) ;
	//mouse
	virtual void MouseMove(int x , int y);
	virtual void MouseDown(int x, int y);
	virtual void MouseUp(int x, int y);

#ifdef _TEST_ 
	virtual void Test() ;	
#endif

private:

	int			  m_nWidth;			
	int           m_nHeight;
	int           m_nLeft;		//this left and top is relate to his parent
	int           m_nTop;

	int           m_nDisplayLeft;
	int           m_nDisplayTop;	//this is relate to display

	CCommonWnd*   m_pParent;
	CCommonWnd*   m_pChild;
	CCommonWnd*   m_pNext;
	CCommonWnd*   m_pPrev;
	IVideoDriver* m_pVideoDriver;
	CTexture*     m_pBackTexture;	//back ground texture 
};

#endif