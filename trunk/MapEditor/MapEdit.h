//********************************************
//  project : avgscript
//	filename: MapEdit.h
//  author:   sgch1982@gmail.com 
//  date:     2007.11
//  descript: MapEdit 
//********************************************

#ifndef _MAPEDIT_H_
#define _MAPEDIT_H_

#define MapEdit_Status_Free 0
#define MapEdit_Status_Select 1



#include "libnge.h"
#include "../Public/include/Map.h"
class CMapEdit  
{
public:
	CMapEdit();
	~CMapEdit();
	bool Init(IVideoDriver* pVideoDriver);
	void Move(int nOffsetX, int  nOffsetY);
	void Draw();
	void ChangeStatus(int nMapStatus);
	bool NewMap();

#ifdef _TEST_
	void Test();

#endif



private:
	IVideoDriver* m_pVideoDriver;
	CTexture*     m_pTexture;
	CMap m_Map;
	
	int m_nRow;		//current row
	int m_nCol;    

	int m_nCurrentStatus;

	
};

#endif  