#include "CommonWnd.h"

CCommonWnd::CCommonWnd()
{

}
CCommonWnd::~CCommonWnd()
{
	if(m_pBackTexture)
		FreeTexture(m_pBackTexture);
}

IVideoDriver* CCommonWnd::GetVideoDriver() const
{
	return m_pVideoDriver;
}

int CCommonWnd::GetWidth() const
{
	return m_nWidth;
}
int CCommonWnd::GetHeight() const
{
	return m_nHeight;
}
int CCommonWnd::GetLeft() const
{
	return m_nLeft;
}
int CCommonWnd::GetTop() const
{
	return m_nTop;
}
void CCommonWnd::SetWidth(int nWidth)
{
	m_nWidth = nWidth;
}
void CCommonWnd::SetHeight(int nHeight)
{
	m_nHeight = nHeight;
}
void CCommonWnd::SetLeft(int nLeft)
{
	m_nLeft  =nLeft;
}
void CCommonWnd::SetTop(int nTop)
{
	m_nTop = nTop;
}
CCommonWnd* CCommonWnd::GetParent() const
{
	return m_pParent;
}
CCommonWnd* CCommonWnd::GetChild()  const
{
	return m_pChild;
}
CCommonWnd* CCommonWnd::GetNext() const
{
	return m_pNext;
}
CCommonWnd* CCommonWnd::GetPrev() const
{
	return m_pPrev;
}

void  CCommonWnd::SetParent(CCommonWnd* pParent)
{
	m_pParent = pParent;
}
void  CCommonWnd::SetChild(CCommonWnd* pChild)
{
	m_pChild = pChild;
}
void  CCommonWnd::SetNext(CCommonWnd* pNext)
{
	m_pNext = pNext;
}
void  CCommonWnd::SetPrev(CCommonWnd* pPrev)
{
	m_pPrev = pPrev;
}
bool CCommonWnd::CreateTexture(const char* pszFileName)
{
	if(pszFileName==NULL)
		return false;

	m_pBackTexture =  LoadTexture(pszFileName);
	if(m_pBackTexture==NULL)
		return false;
	return true;
}
CTexture* CCommonWnd::GetTexture() const
{
	return m_pBackTexture;
}


void CCommonWnd::Draw() 
{
	//draw background texture
//	m_pVideoDriver->RenderQuad(m_pBackTexture,  )

}


#ifdef _TEST_ 
void CCommonWnd::Test() 
{

}
#endif
