#include "Ini.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <io.h>
CIni::CIni()
{
	m_bOpen = false;
	m_pData = NULL;
	m_nDateLen = 0;
}
CIni::~CIni()
{
	if(m_bOpen && m_pData!=NULL)
		delete [] m_pData;

}


bool CIni::Open(const char* pszFileName)
{
	FILE *fp;
	fp=fopen(pszFileName, "rb");
	if( fp==NULL )
		return false;
	
	m_nDateLen = GetFileLength(pszFileName);
	if(m_nDateLen <=0)
		return false;
	m_pData = new char[m_nDateLen];

	fread(m_pData, m_nDateLen, 1, fp);
	fclose(fp);

	m_bOpen = true;
	return true;
}

int CIni::GetFileLength(const char *pszFileName)
{
	int fh, nbytes;

	fh = _open( pszFileName, 0x0000 );	//Ö»¶ÁÄ£Ê½
	if( fh== -1 )
	{
		return -1;
	}
	
	nbytes=_filelength(fh);
	_close(fh);
	
	return nbytes;
}


int CIni::GetInt(const char* pszIndex, const char* pszKey, int nDefault)
{
	if(!m_bOpen|| pszIndex==NULL||pszKey==NULL) return nDefault;
	
	int nIndexPos = 0;
	int nKeyPos = 0;
	int nRet= 0 ;
	char szValueBuf[MAX_VALUE_BUF_LEN]={0};
	 if( (nIndexPos =FindIndex(pszIndex))!=-1)
	 {
		 if( (nKeyPos = FindKey(pszKey, nIndexPos))!=-1)
		 {
			if(GetValue(nKeyPos,szValueBuf,MAX_VALUE_BUF_LEN,&nRet))
			{
				return nRet;
			}
			
		 }
	 }

	 return nDefault;


}
void CIni::GetStr(const char* pszIndex, const char* pszKey,
			const char* pszDefault,
			char* szBuf, int nBufLen)
{
	if( pszIndex==NULL||pszKey==NULL|| pszDefault==NULL
		||szBuf==NULL||nBufLen<=0) return ;


	int nIndexPos = 0;
	int nKeyPos = 0;
	int nRet= 0 ;
	char szValueBuf[MAX_VALUE_BUF_LEN]={0};
	 if( (nIndexPos =FindIndex(pszIndex))!=-1)
	 {
		 if( (nKeyPos = FindKey(pszKey, nIndexPos))!=-1)
		 {
			if(GetValue(nKeyPos,szValueBuf,MAX_VALUE_BUF_LEN))
			{
				if(strlen(szValueBuf)<=nBufLen)
					strcpy(szBuf, szValueBuf);
				return;
			}
			
		 }
	 }
	if(strlen(pszDefault)<=nBufLen)
		strcpy(szBuf, pszDefault);
	else
		strcpy(szBuf,"");
}

int  CIni::FindIndex(const char* pszIndex)
{
	if( pszIndex==NULL) return -1;
	char szIndex[MAX_INDEX_LEN]={0};
	int i = 0;
	while( i < m_nDateLen)
	{
		if(m_pData[i] == '[')
		{
			int j = i; 

			while ( j < m_nDateLen && m_pData[j]!='\n'&&m_pData[j]!=']'  )
			{
				j++;
			}
			//fail  must return : 1. to the end of file 2. the index len big than MAX_INDEX_LEN
			if( j>= m_nDateLen || j-i-1 >=MAX_INDEX_LEN)
				return -1;
			else if( m_pData[j]=='\n') //fail but continue to find 
			{
				//i=j can contine 
			}
			else if( m_pData[j]==']')	//succeed, then compare with pszIndex
			{
				memcpy(szIndex, &m_pData[i+1], j-i-1);
				szIndex[j-i-1]='\0';
				if(strcmp(pszIndex, szIndex) == 0)
				{
					return j; 
				}
				 
				//else i=j to contine 
			}

			//find next 
			i = j;
		}
		else
			i++;
	}	
	return -1;
}

int  CIni::FindKey(const char* pszKey, int nStartPos)
{
	if( pszKey==NULL) return -1;
	char szKey[MAX_INDEX_LEN]={0};
	
	int i = nStartPos;
	while ( i< m_nDateLen && m_pData[i]!='[' )
	{
		int j = i;
		while( j < m_nDateLen && m_pData[j]!='\n'&& m_pData[j]!= '['&& m_pData[j]!='=')
		{
			j++;
		}
		//fail must return
		if( j>=m_nDateLen || j-i >=MAX_KEY_LEN || m_pData[j]=='[')
			return -1;
		else if(m_pData[j]=='\n')	//new line?
		{
			i = j+1;
		}
		else if ( m_pData[j]=='=')	//find '=' , 
		{
			memcpy(szKey, &m_pData[i], j-i);
			if( strcmp(szKey, pszKey)==0)
				return j;
			else
				i= j+1;
		}
		
	}

	return -1;

}



bool CIni::GetValue(int nPos,char* pszBuf, int nBufLen,int* pRet)
{
	if(pszBuf==NULL|| nBufLen<=0||nPos<0 ||nPos>=m_nDateLen)
	{
		return false;
	}
	
	int nStart = nPos+1;
	int nEnd;
	while (nPos!=m_nDateLen &&m_pData[nPos]!='\n' && m_pData[nPos]!=' ' )
	{
		nPos++;
	}

	if( nPos - nStart > nBufLen) //fail 
		return false;
	else if(m_pData[nPos] =='\n' )
	{
		
	}
	else if(m_pData[nPos] ==' ')
	{
		
	}

	nEnd = nPos-1;
	memcpy(pszBuf, &m_pData[nStart], nEnd-nStart );
	if(pRet)
	{
		*pRet= atoi(pszBuf);
	}
	return true;
}

