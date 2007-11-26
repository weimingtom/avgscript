//********************************************
//	filename: Ini.h
//  author:   sgch1982@gmail.com 
//  date:     2007.11
//  
//********************************************

#ifndef _INI_H_
#define _INI_H_


#include "def.h"



class CIni
{
	
public:
	CIni();
	~CIni();


	
	bool Open(const char* pszFileName);
	bool IsOpen()	{return m_bOpen;}
	int GetFileLength(const char *pszFileName);


	//Get int key example: 
	// [pszIndex]
	// pszKey = 5
	int GetInt(const char* pszIndex, const char* pszKey, int nDefault);

	void GetStr(const char* pszIndex, const char* pszKey,
				const char* pszDefault,
				char* szBuf, int nBufLen);

	int  FindIndex(const char* pszIndex);
	int  FindKey(const char* pszKey, int nStartPos=0);
	bool GetValue(int nPos, char* pszBuf, int nBufLen,int* pRet=NULL);	
	void TrimSpace(char* pszBuf);

private:
	bool		m_bOpen;
	char*	    m_pData;
	int         m_nDateLen;

};


#endif