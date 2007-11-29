//********************************************
//  project : avgscript
//	filename: Map.h
//  author:   sgch1982@gmail.com 
//  date:     2007.11
//  descript: common CMap class for avgscript
//********************************************
#ifndef _MAP_H_
#define _MAP_H_
#include "libnge.h"
#include "Ini.h"




//格子结构
struct stCell
{
	unsigned char GroundPic;	//地面页面编号（普通0-59  动态60-63）
	unsigned char Ground;		//地面编号
	unsigned char ObjPic;		//物体页面编号（普通0-59  动态60-63）
	unsigned char Obj;		//物体编号
	unsigned char Obj2Pic;		//物体2页面编号（普通0-59  动态60-63）
	unsigned char Obj2;		//物体2编号
	unsigned char Block;		//是否阻挡
	unsigned char Level;		//物体所在层次(0-15)
	unsigned char CP;			//陷阱
	unsigned char Level2;		//物体2所在层次(0-15)
	unsigned char MouseType;	//鼠标类型
	unsigned char CPType;		//陷阱类型
	unsigned char res[4];		//保留


	stCell()
	{
		memset(this, 0, sizeof(stCell));
	}
};	



class CMap
{
public:
		
	CMap();

	~CMap();

	bool Init(IVideoDriver* pVideoDriver);
	void Draw();

	bool Save(const char* pszFileName);
	bool Load(const char* pszFileName);
	void ScrollMap(int nOffsetX,int nOffsetY);

#ifdef _TEST_
	void Test();

#endif

private:
	CTexture* m_pMapTextureArr[MAX_MAP_TEXTURE_NUM] ;
	CTexture* m_pBackGroundTexture;
	IVideoDriver* m_pVideoDriver;
	CIni      m_ini;
	stCell*   m_pCell;
	int       m_nTextureCount;
	int		  m_nMapRows;
	int		  m_nMapCols;
	int       m_nDisplayTop;		//display top 0 -> map y
	int       m_nDisplayLeft;		//display Left 0-> map x
};


#endif