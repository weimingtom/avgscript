#include "CommonWnd.h"

CCommonWnd::CCommonWnd()
{
	m_pParent = m_pChild = m_pNext = m_pPrev = NULL;
	m_pVideoDriver = NULL;
	m_pBackTexture = NULL;
	m_nDisplayLeft = m_nDisplayTop = 0;
	m_nWidth = m_nHeight = m_nLeft = m_nTop = 0;
	m_nDragStatus = Drag_Status_None;
	SetIsContainer(false);
	SetCanMove(true);
	SetCanDrag(true);
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


int CCommonWnd::GetZorder() const
{
	return m_nZorder;
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

void CCommonWnd::SetCanMove(bool bCanMove)
{
	m_bCanMove = bCanMove;
}
bool CCommonWnd::GetCanMove() const
{
	return m_bCanMove;
}

int CCommonWnd::GetAlpha() const
{
	return m_nAlpha;
}
void CCommonWnd::SetAlpha(int nAlpha)
{
	m_nAlpha = nAlpha;
}
void CCommonWnd::SetZOrder(int nZOrder)
{
	m_nZorder = nZOrder;
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

bool  CCommonWnd::IsKindOf(const char* pszKindName) const
{
	return true;
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
	if(!GetCanMove())
		return;
	m_nLeft+=nOffsetX;
	m_nTop +=nOffsetY;

	m_nDisplayLeft +=nOffsetX;
	m_nDisplayTop +=nOffsetY;

	//move all his child
	CCommonWnd* pChild;
	pChild = GetChild();
	while(pChild)
	{
		pChild->Move(nOffsetX, nOffsetY);
		pChild = pChild->GetNext();
	}
}
/*
void CCommonWnd::DragStart(int x, int y)
{
}
void CCommonWnd::DragIng(int x, int y)
{
}*/

bool CCommonWnd::GetCanDrag() const
{
	return m_bCanDrag;
}
void CCommonWnd::SetCanDrag(bool bCanDrag)
{
	m_bCanDrag = bCanDrag;
}
void CCommonWnd::Drag(int x, int y, int nNextDragStatus)
{
	if(!GetCanDrag())
		return;
	if(!CCommonWnd::IsPtInDragArea(x, y))
		return;
	switch(m_nDragStatus)
	{
	case Drag_Status_None:
		{
			if(nNextDragStatus == Drag_Status_Starting)
			{
				//start drag 
				m_nStartDragX = x;
				m_nStartDragY = y;
				ChangeDragStatus(Drag_Status_Starting);
			}
		}
		break;
	case Drag_Status_Starting:
		{
			if(nNextDragStatus==Drag_Status_Starting)
			{
				//Move wnd
				Move(x-m_nStartDragX, y - m_nStartDragY);
				m_nStartDragX = x;
				m_nStartDragY = y;
				
			}
			else if(nNextDragStatus = Drag_Status_None)
			{
				ChangeDragStatus(Drag_Status_None);
			}
		}
		break;
	//case Drag_Status_End:
	//	{
	//		if(nNextDragStatus = Drag_Status_Starting)
	//		{
	//			ChangeDragStatus(Drag_Status_None);
	//		}
	//	}
	//	break;
	}
/*
	//ok now we drag all his child
	CCommonWnd* pWnd;
	pWnd = GetChild();
	while(pWnd)
	{
		pWnd->Drag(x, y, nNextDragStatus);
		pWnd = pWnd->GetNext();
	}*/
	
}
bool  CCommonWnd::IsPtInDragArea(int x, int y) const
{
	return IsPtInArea(x, y);
}

bool CCommonWnd::GetIsContainer() const
{
	return m_bContainer;
}
void CCommonWnd::SetIsContainer(bool bContainer)
{
	m_bContainer = bContainer;
}

void CCommonWnd::ChangeDragStatus(int nNextStatus)
{
	m_nDragStatus = nNextStatus;
}

void CCommonWnd::MouseMove(int x , int y)
{

	//in here ,we do not need to do anything ,so we MouseMove all his child,
	//if his child inherit CCommonWnd , implement in child class MouseMove ,do that

	//all his child
	CCommonWnd* pWnd;
	pWnd = GetChild();
	while(pWnd)
	{
		pWnd->MouseMove(x, y);
		pWnd = pWnd->GetNext();
	}


}
void CCommonWnd::MouseDown(int x, int y)
{
	/*
	//all his child
	CCommonWnd* pWnd;
	pWnd = GetChild();
	while(pWnd)
	{
		pWnd->MouseDown(x, y);
		pWnd = pWnd->GetNext();
	}*/
}
void CCommonWnd::MouseUp(int x, int y)
{
	//all his child
	CCommonWnd* pWnd;
	pWnd = GetChild();
	while(pWnd)
	{
		pWnd->MouseUp(x, y);
		pWnd = pWnd->GetNext();
	}	
}

bool CCommonWnd::HitTest(int x ,int y)
{
	//all CCommonWnd have a z-order ,hit test return the top CCommonWnd object
	
	
	return IsPtInArea(x,y);
}


#ifdef _TEST_ 
void CCommonWnd::Test() 
{

}
#endif
