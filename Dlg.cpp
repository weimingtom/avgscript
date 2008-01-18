#include "Dlg.h"


CDlg::CDlg():m_DragArea(0,0,0,0)
{
}
CDlg::~CDlg()
{
}

bool CDlg::CreateTexture(const char* pszFileName)
{

	if(!CCommonWnd::CreateTexture(pszFileName))
		return false;
 	SetWidth(GetTexture()->w );
	SetHeight(GetTexture()->h);
	SetIsContainer(true);
	return true;
}
void CDlg::Draw(_RECT rect)
{

	IVideoDriver* pVideoDriver;
	pVideoDriver = GetVideoDriver();
	int nSourceX, nSourceY,nSourceWidth,nSourceHeight,nDestX,nDestY;
	nSourceX = 0;
	nSourceY = 0;
	nSourceWidth = GetWidth();
	nSourceHeight = GetHeight();
	nDestX = GetDisplayLeft();
	nDestY = GetDisplayTop();
	pVideoDriver->RenderQuad(GetTexture(), nSourceX, nSourceY, nSourceWidth,
		nSourceHeight,nDestX,nDestY,XSCALE, YSCALE, ANGLE, GetAlpha());
	

	CCommonWnd::Draw();

}
/*
void CDlg::DragStart()
{
}
void CDlg::DragIng()
{
}*/
void CDlg::Drag(int x, int y,int nNextDragStatus)
{
	if(IsPtInDragArea(x,y))
	{
		CCommonWnd::Drag(x,y,nNextDragStatus);
	}

}

bool CDlg::IsPtInDragArea(int x, int y) const
{
	if( m_DragArea.width ==0|| m_DragArea.height==0)
	{
		return true;
		//all area can be drag
//		return IsPtInArea(x, y);
	}
	else
	{
		//is pt in m_DragArea?
		
		//change m_DragArea to display coordinate
		_RECT rect(0,0,0,0);
		rect.top = m_DragArea.top + GetDisplayTop();
		rect.left = m_DragArea.left + GetDisplayLeft();
		rect.width = m_DragArea.width;
		rect.height = m_DragArea.height;

		return ::IsPtInRect(x, y, rect);		//global function
	}
}

