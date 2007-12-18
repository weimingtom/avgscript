#include "CommonWnd.h"

CCommonWnd::CCommonWnd()
{
	m_pParent = m_pChild = m_pNext = m_pPrev = NULL;
	m_pVideoDriver = NULL;
	m_pBackTexture = NULL;
	m_nDisplayLeft = m_nDisplayTop = 0;
	m_nWidth = m_nHeight = m_nLeft = m_nTop = 0;

}
CCommonWnd::~CCommonWnd()
{
	if(m_pBackTexture)
		FreeTexture(m_pBackTexture);
}

IVideoDriver* CCommonWnd::GetVideoDriver() const
{
	return ::GetVideoDriver();
//	return m_pVideoDriver;
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
int CCommonWnd::GetDisplayTop() const
{
	return m_nDisplayTop;
}
int CCommonWnd::GetDisplayLeft() const
{
	return m_nDisplayLeft;
}
void CCommonWnd::SetDisplayTop(int nTop)
{
	m_nDisplayTop = nTop;
}
void CCommonWnd::SetDisplayLeft(int nLeft)
{
	m_nDisplayLeft = nLeft;
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

bool  CCommonWnd::IsPtInArea(int x, int y) const
{
	//x  ,y is display pos
	if( m_nDisplayLeft < x &&
		x < m_nDisplayLeft+m_nWidth&&
		m_nDisplayTop < y &&
		y < m_nDisplayTop+m_nHeight)
	{
		return true;
	}
	return false;
}
void CCommonWnd::Release()
{
	//delete his all child

	CCommonWnd* pChild;
	pChild = GetChild();
	while(pChild)
	{
		pChild->Release();
		pChild = pChild->GetNext();
	}

	//delete his self
	delete this;
	
}

void CCommonWnd::Draw(_RECT rect) 
{
	
	//draw his child

	CCommonWnd* pChild;
	pChild = GetChild();
//	if(pChild==NULL)
//	return;
//
	while(pChild)
	{
		pChild->Draw();
		pChild = pChild->GetNext();
	}




}
void CCommonWnd::Move(int nOffsetX, int nOffsetY) 
{
	m_nLeft+=nOffsetX;
	m_nTop +=nOffsetY;

	m_nDisplayLeft +=nOffsetX;
	m_nDisplayTop +=nOffsetY;

	
}
void CCommonWnd::MouseMove(int x , int y)
{
}
void CCommonWnd::MouseDown(int x, int y)
{
}
void CCommonWnd::MouseUp(int x, int y)
{
}



#ifdef _TEST_ 
void CCommonWnd::Test() 
{

}
#endif
