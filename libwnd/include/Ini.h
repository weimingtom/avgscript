//********************************************
//  project : libwnd
//	filename: Ini.h
//  author:   sgch1982@gmail.com 
//  date:     2007.11
//  descript: ini utility  
//********************************************

#ifndef _INI_H_
#define _INI_H_

#include "Def.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <io.h>

#define MAX_LINE_LEN 512
#define MAX_VALUE_BUF_LEN 256
#define MAX_INDEX_LEN 256
#define MAX_KEY_LEN 256
class CIni
{
private:
/*
	struct STValue
	{
		char szValue[MAX_VALUE_BUF_LEN];
		STValue()
		{
			memset(this,0,sizeof(STValue));
		}
	};
*/
	struct STKey
	{
		char szKey[MAX_KEY_LEN];
		char szValue[MAX_VALUE_BUF_LEN];
		STKey* pNextKey;
		
		//STValue* pValue;
		STKey()
		{
			memset(this, 0, sizeof(STKey));
		}
	};


	struct STIndex
	{
		char szIndex[MAX_INDEX_LEN];
		STIndex* pNextIndex;
		STKey* pFirstKey;
		STIndex()
		{
			memset(this,0,sizeof(STIndex));
		}
	};



public:
	CIni();
	~CIni();


	
	bool Open(const char* pszFileName);
	bool IsOpen()	{return m_bOpen;}

	void Save(const char* pszFileName);


	//Get int key example: 
	// [pszIndex]
	// pszKey = 5
	int GetInt(const char* pszIndex, const char* pszKey, int nDefault);

	void GetStr(const char* pszIndex, const char* pszKey,
				const char* pszDefault,
				char* szBuf, int nBufLen);

	void SetInt(const char* pszIndex, const char* pszKey, int nValue);
	void SetStr(const char* pszIndex, const char* pszKey,const char* pszValue);

	void AddKeyAndValue(const char* pszKey,const char* pszValue );
	void AddKeyAndValue(const char* pszKey, int nValue);
	void AddIndex(const char* pszIndex);

private:
	void Init();
	bool SetIndex(const char* psz);
	bool SetKeyAndValue(const char* psz);

	STIndex* GetLastIndex();
	
	STIndex* GetIndex(const char* pszIndex);
	STKey*   GetKey(const char* pszIndex,const char* pszKey);
	STKey*   GetLastKey(STIndex* pIndex);
//	STValue* GetValue(const char* pszIndex, const char* pszKey);

	int GetFileLength(const char *pszFileName);
	int  FindIndex(const char* pszIndex);
	int  FindKey(const char* pszKey, int nStartPos=0);
	bool GetValue(int nPos, char* pszBuf, int nBufLen,int* pRet=NULL);	
	void TrimSpace(char* pszBuf);

	bool		m_bOpen;
	char*	    m_pData;
	int         m_nDateLen;
	STIndex*    m_pFirstIndex;

};


#endif