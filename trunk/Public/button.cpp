#include "button.h"



Cbutton::Cbutton()
{
	m_nButtonStatus = BUTTON_STATUS_NORMAL;
}
Cbutton::~Cbutton()
{

}

bool Cbutton::CreateTexture(const char* pszFileName)
{
	if(!CCommonWnd::CreateTexture(pszFileName))
		return false;

	//button have five status ,so button weight = pic weight/5

 	SetWidth(GetTexture()->w / BUTTON_STATUS_COUNT);
	SetHeight(GetTexture()->h);
	return true;
	
}


void Cbutton::Draw(_RECT rect)
{
	IVideoDriver* pVideoDriver;
	pVideoDriver = GetVideoDriver();
	int nSourceX, nSourceY,nSourceWidth,nSourceHeight,nDestX,nDestY;
	nSourceX = m_nButtonStatus* GetWidth();
	nSourceY = 0;
	nSourceWidth = GetWidth();
	nSourceHeight = GetHeight();
	nDestX = GetDisplayLeft();
	nDestY = GetDisplayTop();
	pVideoDriver->RenderQuad(GetTexture(), nSourceX, nSourceY, nSourceWidth,
		nSourceHeight,nDestX,nDestY/*,XSCALE, YSCALE, ANGLE, m_nAlpha*/);
	
	CCommonWnd::Draw(rect);
}
void Cbutton::Move(int nOffsetX, int nOffsetY)
{
	
}
void Cbutton::MouseMove(int x , int y)
{
	if( IsPtInArea(x, y) )
	{
		
		ChangeStatue(BUTTON_STATUS_MOUSEON);
	}
	else
	{
		ChangeStatue(BUTTON_STATUS_NORMAL);
	}
	CCommonWnd::MouseMove(x,y);
}
void Cbutton::MouseDown(int x, int y)
{

	if(IsPtInArea(x, y))
	{
		ChangeStatue(BUTTON_STATUS_CLICK);

		//add 
	}
}
void Cbutton::MouseUp(int x, int y)
{
	if(IsPtInArea(x, y))
	{
		//add 
	}
}

void Cbutton::ChangeStatue(int nNextStatus)
{
	m_nButtonStatus = nNextStatus;
}

void Cbutton::SetAlpha(int nAlpha)
{
	m_nAlpha = nAlpha;
}


#ifdef _TEST_
void Cbutton::Test()
{
	CreateTexture("data\\pic\\button1.png");
	SetDisplayLeft(100);
	SetDisplayTop(50);
}
#endif
