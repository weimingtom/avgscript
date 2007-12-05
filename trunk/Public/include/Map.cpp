#include "Map.h"
#include "libnge.h"



CMap::CMap()
{
	m_bModify = false;
}

CMap::~CMap()
{
	for(int i = 0;i< m_nTextureCount;i++)
	{
		if(m_pMapTextureArr[i])
			FreeTexture(m_pMapTextureArr[i]);
	}

	if(m_pCell)
		delete [] m_pCell;
}

bool CMap::Init(IVideoDriver* pVideoDriver)
{
	if(pVideoDriver==NULL)
		return false;
	m_pVideoDriver = pVideoDriver;

	if(m_ini.IsOpen())
		return false;

	if(!m_ini.Open("data\\ini\\MapPicLoad.ini"))
		return false;


	//load pic 
	m_nTextureCount = m_ini.GetInt("Pic", "PicCount",0);

	if(m_nTextureCount==0|| m_nTextureCount>MAX_MAP_TEXTURE_NUM)
		return false;

	char szPicFileName[MAX_PATH]={0};
	char szKey[MAX_KEY_LEN]={0};
	for (int i=0;i<m_nTextureCount;i++)
	{
		sprintf(szKey,"Pic%d",i);
		m_ini.GetStr("Pic",szKey, "",szPicFileName, MAX_PATH);
		if(strcmp(szPicFileName,"")==0)
			return false;

		m_pMapTextureArr[i]=LoadTexture(szPicFileName);

	}


	m_ini.GetStr("Pic","PicBackGround", "",szPicFileName, MAX_PATH);
	
	m_pBackGroundTexture = LoadTexture(szPicFileName);
/*	
	//init map 
	m_nMapRows = m_ini.GetInt("MapSize","Row",0);
	m_nMapCols = m_ini.GetInt("MapSize","Col",0);
	if(m_nMapRows==0||m_nMapCols==0)
		return false;

	m_pCell = new stCell[m_nMapRows*m_nMapCols];
*/
	

	return true;
}



bool CMap::Save(const char* pszFileName)
{
	FILE *fp;
	fp=fopen(pszFileName, "wb");
	if( fp==NULL )
		return false;

	//first 8 bytes store map size : row, col
	fwrite(&m_nMapRows, sizeof(int),1, fp);
	fwrite(&m_nMapCols, sizeof(int),1, fp);
	
	//then store map cells : row*col*sizeof(stCell)

	fwrite(m_pCell, sizeof(stCell)*m_nMapRows*m_nMapCols, 1, fp);

	fclose(fp);

	m_bModify = false;
	return true;

}
bool CMap::Load(const char* pszFileName)
{
	FILE *fp;
	fp=fopen(pszFileName, "rb");
	if( fp==NULL )
		return false;

	//first 8 bytes  map size : row, col
	fread(&m_nMapRows, sizeof(int),1, fp);
	fread(&m_nMapCols, sizeof(int),1, fp);


	m_pCell = new stCell[m_nMapRows*m_nMapCols];
	//then  map cells : row*col*sizeof(stCell)

	fread(m_pCell, sizeof(stCell)*m_nMapRows*m_nMapCols, 1, fp);

	
	fclose(fp);

	return true;
}



void CMap::Draw(int nLayer)
{

	//draw background 

	
	m_pVideoDriver->RenderQuad(m_pBackGroundTexture,0,0,DISPLAY_WIDTH,DISPLAY_HEIGHT,0,0,1,1,0,0xffffffff);

	//draw ground

	//default map is big than display area
	//first get the row and col need to draw

	int nRow=0,nCol=0;
	nRow = (m_nDisplayTop + DISPLAY_HEIGHT)/CELL_SIZE;
	nCol = (m_nDisplayLeft+ DISPLAY_WIDTH)/CELL_SIZE;

	if((m_nDisplayTop + DISPLAY_HEIGHT)%CELL_SIZE!=0)
		nRow++;
	if((m_nDisplayLeft+ DISPLAY_WIDTH)%CELL_SIZE!=0)
		nCol++;
	//map cell pos
	int nMapX,nMapY;

	//Display pos  (left,top)->(right,bottom)
	int nLeft,nTop,nRight,nBottom;
	//
	int nSourceX,nSourceY,nSourceWidth,nSourceHeight,nDestX,nDestY;
	
	//Dispaly pos (0,0) --> Map cell pos
	int nOrigLeft = m_nDisplayLeft / CELL_SIZE;
	int nOrigTop = m_nDisplayTop / CELL_SIZE;

	//source texture offset
	int nLeftOffset,nTopOffset;

	for (int i = 0; i< nRow;i++)
	{
		for(int j = 0;j< nCol;j++)
		{
			//calc map pos;
			nMapX = nOrigLeft+j;
			nMapY = nOrigTop+i;
			//calc display pos
			nLeft = (nOrigLeft+j)*CELL_SIZE-m_nDisplayLeft;
			nTop = (nOrigTop+i)*CELL_SIZE-m_nDisplayTop;
			nRight = nLeft+CELL_SIZE;
			nBottom= nTop+CELL_SIZE;
			nLeftOffset = nTopOffset = 0;
			if(nLeft<0)
			{
				nLeftOffset = -nLeft;// nLeftOffset for calc source texture nSourceX
				nLeft=0;
			}
			if(nTop<0)
			{
				nTopOffset = -nTop;
				nTop=0;
			}
			if(nRight>DISPLAY_WIDTH)
			{
			//	nRightOffset = nRight - DISPLAY_WIDTH;
				nRight=DISPLAY_WIDTH;
			}
			if(nBottom>DISPLAY_HEIGHT)
			{
			//	nBottomOffset = nBottom - DISPLAY_HEIGHT;
				nBottom=DISPLAY_HEIGHT;
			}
			//cals nDestX,nDestY,nSourceWidth,nSourceHeight
			nDestX = nLeft;
			nDestY = nTop;
			nSourceWidth = nRight - nLeft;
			nSourceHeight = nBottom - nTop;


			//now calc ground layer's nSourceX and nSourceY
			//find texture 
			CTexture* pTexture;
			int nTextureIndex;
			
			nTextureIndex = m_pCell[nMapX+nMapY*m_nMapCols].GroundPic;
			pTexture = m_pMapTextureArr[nTextureIndex];
			if(pTexture==NULL)continue;	//contine to draw next col
			//get pTexture width and height
			int nTextureCells;
			nTextureCells = pTexture->w / CELL_SIZE;
			
			//find cell in texture pos
			int nCellRow,nCellCol;
			nCellRow = m_pCell[nMapX+nMapY*m_nMapCols].Ground / nTextureCells;
			nCellCol = m_pCell[nMapX+nMapY*m_nMapCols].Ground % nTextureCells;
			//calc nSourceX,nSourceY
			nSourceX = nCellCol * CELL_SIZE;
			nSourceY = nCellRow * CELL_SIZE;

			//add offset 
			nSourceX = nSourceX + nLeftOffset;
			nSourceY = nSourceY + nTopOffset;

			//ok we can draw now
			//ground layer 1
			if(nLayer&GROUND_LAYER_1)
				m_pVideoDriver->RenderQuad(pTexture,nSourceX,nSourceY,nSourceWidth,nSourceHeight,nDestX,nDestY,1,1,0, RGBA(255,255,255,255));
			

			//calc objcet layer1 nSourceX, nSourceY
			nTextureIndex = m_pCell[nMapX+nMapY*m_nMapCols].ObjPic;
			pTexture = m_pMapTextureArr[nTextureIndex];
			if(pTexture==NULL)continue;	//contine to draw next col
			//get pTexture width 
			nTextureCells = pTexture->w / CELL_SIZE;
			//find cell in texture pos
			nCellRow = m_pCell[nMapX+nMapY*m_nMapCols].Obj / nTextureCells;
			nCellCol = m_pCell[nMapX+nMapY*m_nMapCols].Obj % nTextureCells;
			//calc nSourceX,nSourceY
			nSourceX = nCellCol * CELL_SIZE;
			nSourceY = nCellRow * CELL_SIZE;
			//add offset 
			nSourceX = nSourceX + nLeftOffset;
			nSourceY = nSourceY + nTopOffset;
			//get object layer 's alpha value
			int nAlpha = 0;
			nAlpha = m_pCell[nMapX+nMapY*m_nMapCols].Level;

			//draw objcet layer
			if(nLayer&OBJECT_LAYER_1)
				m_pVideoDriver->RenderQuad(pTexture,nSourceX,nSourceY,nSourceWidth,nSourceHeight,nDestX,nDestY,1,1,0, RGBA(255,255,255,nAlpha));
			
			

			//calc objcet layer2 nSourceX, nSourceY
			nTextureIndex = m_pCell[nMapX+nMapY*m_nMapCols].Obj2Pic;
			pTexture = m_pMapTextureArr[nTextureIndex];
			if(pTexture==NULL)continue;	//contine to draw next col
			//get pTexture width 
			nTextureCells = pTexture->w / CELL_SIZE;
			//find cell in texture pos
			nCellRow = m_pCell[nMapX+nMapY*m_nMapCols].Obj2 / nTextureCells;
			nCellCol = m_pCell[nMapX+nMapY*m_nMapCols].Obj2 % nTextureCells;
			//calc nSourceX,nSourceY
			nSourceX = nCellCol * CELL_SIZE;
			nSourceY = nCellRow * CELL_SIZE;
			//add offset 
			nSourceX = nSourceX + nLeftOffset;
			nSourceY = nSourceY + nTopOffset;
			//get object layer 's alpha value
			
			nAlpha = m_pCell[nMapX+nMapY*m_nMapCols].Level2;
		
			//draw objcet layer 2
			if(nLayer&OBJECT_LAYER_2)
				m_pVideoDriver->RenderQuad(pTexture,nSourceX,nSourceY,nSourceWidth,nSourceHeight,nDestX,nDestY,1,1,0, RGBA(255,255,255,nAlpha));
				
		}
	}
	

}

void CMap::ScrollMap(int nOffsetX,int nOffsetY)
{

	m_nDisplayLeft+=nOffsetX;
	m_nDisplayTop+=nOffsetY;

	if(m_nDisplayLeft<0)
	{
		m_nDisplayLeft=0;
	}
	if(m_nDisplayLeft +DISPLAY_WIDTH>m_nMapCols*CELL_SIZE)
	{
		m_nDisplayLeft = m_nMapCols*CELL_SIZE - DISPLAY_WIDTH;

	}
	if(m_nDisplayTop<0)
	{
		m_nDisplayTop=0;
	}		

	if(m_nDisplayTop +DISPLAY_HEIGHT>m_nMapRows*CELL_SIZE)
	{
		m_nDisplayTop = m_nMapRows*CELL_SIZE - DISPLAY_HEIGHT;
	}

}

bool CMap::NewMap(int nRows,int nCols)
{
	if(nRows<=0 ||nRows>MAX_MAP_ROWS
		|| nCols<=0 || nCols>MAX_MAP_COLS)
		return false;
	if(m_bModify)
		return false; //must first save current modified map

	//delete 
	if(m_pCell)
		delete[] m_pCell;

	m_pCell = new stCell[nRows*nCols];
	return true;
}

bool CMap::SetMapCell(int nRow, int nCol, stCell cell)
{
	if(nRow<=0 ||nRow>MAX_MAP_ROWS
		|| nCol<=0 || nCol>MAX_MAP_COLS)
		return false;
	m_pCell[nRow*m_nMapCols+nCol]=cell;
	m_bModify = true;
	return true;
}


CTexture* CMap::GetMapTexture(int nIndex)
{
	if(nIndex<0||nIndex>=m_nTextureCount)
		return NULL;

	return m_pMapTextureArr[nIndex];
}


void CMap::CalcSourceXY(	CTexture* pTexture,int nMapX, int nMapY, int& nSourceX, int& nSourceY)
{
	
}
#ifdef _TEST_
void CMap::Test()
{
	m_nMapRows = 25;
	m_nMapCols = 25;
	m_pCell = new stCell[m_nMapRows*m_nMapCols];
	for ( int i = 0; i< m_nMapRows*m_nMapCols;i++)
	{
		m_pCell[i].GroundPic = 0;
		m_pCell[i].Ground = i %30 + 1;

	}
	

	Save("data\\save\\test.mapfile");
	Load("data\\save\\test.mapfile");

//	m_nDisplayTop = 16;
//	m_nDisplayLeft = 16;
}

#endif
