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
#define MapEdit_Status_Selected 2   //selected a pic ,than set cell attribute


#include "libnge.h"
#include "../Public/include/Map.h"
class CMapEdit  
{
public:
	CMapEdit();
	~CMapEdit();
	bool Init(IVideoDriver* pVideoDriver);
	void Move(int nOffsetX, int  nOffsetY);	//this function use for keyboard
	void MoveTo(int x, int y);				//for mouse
	void Draw();
	void ChangeStatus(int nNextMapStatus);
	void ChangeLayer(int nNextLayer);		//change edit layer 
	void SetDrawFlag(int nFlag);			
	void SetCellAttribute();
	bool NewMap();

	void Scroll(int nOffsetX, int nOffsetY);


	void KeyDown();
	
#ifdef _TEST_
	void Test();

#endif



private:
	IVideoDriver* m_pVideoDriver;
	CTexture*     m_pTexture;
	CMap m_Map;
	
	int m_nRow;		//current row
	int m_nCol;    

	int m_nCurX;
	int m_nCurY;

	int m_nFreeStatusCurX;
	int m_nFreeStatusCurY;
	int m_nSelectStatusCurX;
	int m_nSelectStatusCurY;

	int m_nCurrentStatus;
	int m_nCurrentTexture;	//current Texture 
	int m_nDisplayTop;		//display top ,when texture size big than texture size
	int m_nDisplayLeft;    
	int m_nCurLayer;		//for select status , edit current layer value can be:GROUND_LAYER_1, OBJECT_LAYER_1, OBJECT_LAYER_2
	int m_nCurDrawMapFlag;	//combine by GROUND_LAYER_1, OBJECT_LAYER_1, OBJECT_LAYER_2
};

#endif  