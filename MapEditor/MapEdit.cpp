

#include "MapEdit.h"



CMapEdit::CMapEdit()
{
	m_nCurX = m_nCurY = m_nCurrentTexture = 0;
	m_nFreeStatusCurX = m_nFreeStatusCurY = m_nSelectStatusCurX= m_nSelectStatusCurY = 0;
	m_nCurrentStatus = MapEdit_Status_Free;
	m_pTexture =  NULL;
	m_pVideoDriver = NULL;
	m_nCurLayer = GROUND_LAYER_1;
	m_nCurDrawMapFlag = ALL_LAYER;
}

CMapEdit::~CMapEdit()
{
//	if(m_pTexture)
//		FreeTexture(m_pTexture);
}

bool CMapEdit::Init(IVideoDriver* pVideoDriver)
{
	m_pVideoDriver = pVideoDriver;


//	m_pTexture = LoadTexture("data\\pic\\tilemap1.png");
	//m_nMapRows 
	return m_Map.Init(pVideoDriver);
}

void CMapEdit::MoveTo(int x, int y)		
{
	m_nCurX = ( x / CELL_SIZE)* CELL_SIZE;
	m_nCurY = ( y / CELL_SIZE)* CELL_SIZE;

	//mouse move can't scroll map ,use Move to scroll

	
}
void CMapEdit::Move(int nOffsetX, int  nOffsetY)
{

	
	m_nCurY+=nOffsetY;
	m_nCurX+=nOffsetX;

	int nWidth, nHeight;
	if( m_nCurrentStatus == MapEdit_Status_Free)
	{
		nWidth = m_Map.GetMapCols()*CELL_SIZE;
		nHeight =  m_Map.GetMapRows()*CELL_SIZE;
	}
	else if(m_nCurrentStatus == MapEdit_Status_Select)
	{
		CTexture* pTexture = m_Map.GetMapTexture(m_nCurrentTexture);

		if(pTexture==NULL)return;
		nWidth = pTexture->w;
		nHeight = pTexture->h;
		
	}

	if(nWidth > DISPLAY_WIDTH)
		nWidth = DISPLAY_WIDTH;
	if(nHeight > DISPLAY_HEIGHT)
		nHeight = DISPLAY_HEIGHT;

	bool bScroll = false;
	if(m_nCurY<0) 
	{
		bScroll = true;
		m_nCurY = 0;
	}
	if(m_nCurY+ CELL_SIZE>nHeight) 
	{
		bScroll = true;
		nOffsetY = m_nCurY+ CELL_SIZE - nHeight;

		m_nCurY = nHeight-CELL_SIZE;//
		
	}
	if(m_nCurX<0)
	{
		m_nCurX=0;
		bScroll = true;
	}
	if(m_nCurX+CELL_SIZE>nWidth) 
	{
		bScroll = true;
		nOffsetX = m_nCurX+ CELL_SIZE - nWidth;

		m_nCurX= nWidth-CELL_SIZE;
		
	}
	if(bScroll)
	{
		 if( m_nCurrentStatus == MapEdit_Status_Free)
			m_Map.ScrollMap(nOffsetX, nOffsetY);
		 else if( m_nCurrentStatus == MapEdit_Status_Select)
			Scroll(nOffsetX, nOffsetY);
	
	}
}


bool CMapEdit::NewMap()
{
	return true;	
}

void CMapEdit::Draw()
{
	if(m_nCurrentStatus == MapEdit_Status_Free)
		m_Map.Draw(m_nCurDrawMapFlag);
	else if(m_nCurrentStatus == MapEdit_Status_Select)
	{
		if(m_pTexture!=NULL)
		{
			
			m_pVideoDriver->RenderQuad(m_Map.GetBackGroundTexture(),0,0,DISPLAY_WIDTH,DISPLAY_HEIGHT,0,0,1,1,0,0xffffffff);

			m_pVideoDriver->RenderQuad(m_pTexture, m_nDisplayLeft,m_nDisplayTop,
				DISPLAY_WIDTH, DISPLAY_HEIGHT, 
				0, 0 ,1,1,0,RGBA(255,255,255,100));

		}
		
	}

	//Draw cursor
	m_pVideoDriver->RenderQuad(m_pTexture, 0, 0, CELL_SIZE, CELL_SIZE, 
		m_nCurX, m_nCurY,1,1,0,RGBA(255,255,255,100));

}

void CMapEdit::ChangeLayer(int nNextLayer)
{
	if(m_nCurrentStatus==MapEdit_Status_Free)
		return;
	m_nCurLayer = nNextLayer;
}
void CMapEdit::SetDrawFlag(int nFlag)			
{
	m_nCurDrawMapFlag = nFlag;
}
void CMapEdit::ChangeStatus(int nNextMapStatus)
{
	if(m_nCurrentStatus==nNextMapStatus)return;


	
	switch(nNextMapStatus)
	{
	case MapEdit_Status_Free:
		{

			//selected -> free
			m_nSelectStatusCurX = m_nCurX;
			m_nSelectStatusCurY = m_nCurY;
			m_nCurX = m_nFreeStatusCurX;
			m_nCurY = m_nFreeStatusCurY;
			m_nCurrentStatus = MapEdit_Status_Free;

			
		}
 
		break;
	case MapEdit_Status_Select:
		{
			
			//free -> select
			m_nFreeStatusCurX = m_nCurX;
			m_nFreeStatusCurY = m_nCurY;
			
			m_nCurX = m_nSelectStatusCurX;
			m_nCurY = m_nSelectStatusCurY;
			m_nCurrentStatus = MapEdit_Status_Select;
			

			m_pTexture = m_Map.GetMapTexture(m_nCurrentTexture);

		}
		break;
	case MapEdit_Status_Selected:
		{
			//select ->selected
		}
		break;


	default:
		;
	}
}

//scroll the editing picture 
void CMapEdit::Scroll(int nOffsetX, int nOffsetY)
{
	int nCurTextureWidth;
	int nCurTextureHeight;

	CTexture* pTexture = m_Map.GetMapTexture(m_nCurrentTexture);

	if(pTexture==NULL)return;

	nCurTextureWidth = pTexture->w;
	nCurTextureHeight = pTexture->h;
//	nCurTextureWidth = m_Map.GetMapTexture(m_nCurrentTexture);

	
	m_nDisplayLeft+=nOffsetX;
	m_nDisplayTop+=nOffsetY;

	if(m_nDisplayTop<0)
		m_nDisplayTop = 0;

	if(m_nDisplayLeft<0)
		m_nDisplayLeft=0;

	if(m_nDisplayTop+ DISPLAY_HEIGHT>nCurTextureHeight)
		m_nDisplayTop = nCurTextureHeight - DISPLAY_HEIGHT;
	if(m_nDisplayLeft+ DISPLAY_WIDTH>nCurTextureWidth)
		m_nDisplayLeft = nCurTextureWidth - DISPLAY_WIDTH;

	
}
void CMapEdit::SetCellAttribute()
{
	
}

void CMapEdit::KeyDown()
{
	switch(m_nCurrentStatus)
	{
	case MapEdit_Status_Free:
		{
			//to select status 
			ChangeStatus(MapEdit_Status_Select);
			
		}
		break;
	case MapEdit_Status_Select:
		{
			//
			ChangeStatus(MapEdit_Status_Selected);
		}
		break;
	case MapEdit_Status_Selected:
		{
			ChangeStatus(MapEdit_Status_Free);
		}
		break;
	}
}



#ifdef _TEST_
void CMapEdit::Test()
{
	m_Map.Test();
}

#endif
