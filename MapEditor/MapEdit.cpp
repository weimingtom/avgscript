

#include "MapEdit.h"



CMapEdit::CMapEdit()
{
	m_nRow = m_nCol = 0;
	m_nCurrentStatus = MapEdit_Status_Free;
	m_pTexture =  NULL;
	m_pVideoDriver = NULL;
}

CMapEdit::~CMapEdit()
{
	if(m_pTexture)
		FreeTexture(m_pTexture);
}

bool CMapEdit::Init(IVideoDriver* pVideoDriver)
{
	m_pVideoDriver = pVideoDriver;


	m_pTexture = LoadTexture("data\\pic\\tilemap1.png");
	//m_nMapRows 
	return m_Map.Init(pVideoDriver);
}


void CMapEdit::Move(int nOffsetX, int  nOffsetY)
{

	
	m_nRow+=nOffsetY;
	m_nCol+=nOffsetX;

	bool bScroll = false;
	if(m_nRow<0) 
	{
		bScroll = true;
		m_nRow = 0;
	}
	if(m_nRow>MAX_DISPLAY_CELLS_ROW) 
	{
		bScroll = true;
		m_nRow = MAX_DISPLAY_CELLS_ROW;//
	}
	if(m_nCol<0)
	{
		m_nCol=0;
		bScroll = true;
	}
	if(m_nCol>MAX_DISPLAY_CELLS_COL-1) 
	{
		m_nCol= MAX_DISPLAY_CELLS_COL-1;
		bScroll = true;
	}
	if(bScroll)
		m_Map.ScrollMap(nOffsetX*CELL_SIZE, nOffsetY*CELL_SIZE);	//scrollMap use pixel ,so *32
	

}


bool CMapEdit::NewMap()
{
	return true;	
}

void CMapEdit::Draw()
{
	m_Map.Draw();

	//Draw 
	m_pVideoDriver->RenderQuad(m_pTexture, 0, 0, CELL_SIZE, CELL_SIZE, 
		m_nCol*CELL_SIZE, m_nRow*CELL_SIZE,1,1,0,RGBA(255,255,255,100));

}


void CMapEdit::ChangeStatus(int nMapStatus)
{
	if(m_nCurrentStatus==nMapStatus)return;

	/*
	switch(nMapStatus)
	{
	case MapEdit_Status_Free:

		break;
	case MapEdit_Status_Select:
		{
			

		}
		break;

	default:
		;
	}*/
}

#ifdef _TEST_
void CMapEdit::Test()
{
	m_Map.Test();
}

#endif
