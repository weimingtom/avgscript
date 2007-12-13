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

	void SetWidth(int nWidth);
	void SetHeight(int nHeight);
	void SetLeft(int nLeft);
	void SetTop(int nTop);
	
	CCommonWnd* GetParent() const;
	CCommonWnd* GetChild()  const;
	CCommonWnd* GetNext() const;
	CCommonWnd* GetPrev() const;
	void  SetParent(CCommonWnd* pParent);
	void  SetChild(CCommonWnd* pChild);
	void  SetNext(CCommonWnd* pNext);
	void  SetPrev(CCommonWnd* pPrev);
	bool CreateTexture(const char* pszFileName);
	CTexture* GetTexture() const;



	//virtual function
	virtual void Draw() ;


#ifdef _TEST_ 
	virtual void Test() ;	
#endif

private:

	int			  m_nWidth;			
	int           m_nHeight;
	int           m_nLeft;
	int           m_nTop;


	CCommonWnd*   m_pParent;
	CCommonWnd*   m_pChild;
	CCommonWnd*   m_pNext;
	CCommonWnd*   m_pPrev;
	IVideoDriver* m_pVideoDriver;
	CTexture*     m_pBackTexture;	//back ground texture 
};

#endif