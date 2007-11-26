#include "Map.h"
#include "libnge.h"



CMap::CMap()
{
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
	
	//init map 
	m_nMapRows = m_ini.GetInt("MapSize","Row",0);
	m_nMapCols = m_ini.GetInt("MapSize","Col",0);
	if(m_nMapRows==0||m_nMapCols==0)
		return false;

	m_pCell = new stCell[m_nMapRows*m_nMapCols];

	

	return true;
}



void CMap::Draw()
{
	m_pVideoDriver->RenderQuad(m_pMapTextureArr[0],0,0,480,272,0,0,1.0,1.0,1.0,0xffffffff);

}


