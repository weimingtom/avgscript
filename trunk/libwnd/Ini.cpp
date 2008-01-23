#include "Ini.h"
#include <iostream>

using namespace std;
CIni::CIni()
{
	m_bOpen = false;
	m_pData = NULL;
	m_nDateLen = 0;

	m_pFirstIndex = NULL;
}
CIni::~CIni()
{
	if(m_bOpen && m_pData!=NULL)
		delete [] m_pData;


	//delete 

	if(m_pFirstIndex)
	{

		STIndex* pIndex;
		STIndex* pNextIndex;
		pIndex = m_pFirstIndex;
		while(pIndex)
		{
			pNextIndex = pIndex->pNextIndex;

			//delete all his key 
			STKey* pKey;
			pKey = pIndex->pFirstKey;
			while(pKey)
			{
				STKey* pNextKey;
				pNextKey = pKey->pNextKey;
				//delete current key
				delete pKey;
				//next key
				pKey = pNextKey;
			}
			//delete current index
			delete pIndex;
			//move to next index
			pIndex = pNextIndex;
		}

	}

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

	Init();
	m_bOpen = true;
	return true;
}


void CIni::Save(const char* pszFileName)
{
	if(pszFileName==NULL)
		return;


	FILE* fp;
	fp = fopen(pszFileName, "wb");
	if(fp==NULL)
		return;



	STIndex* pIndex;
	pIndex = m_pFirstIndex;

	
	while(pIndex)
	{
		//write index first
		
		fwrite("[", 1,1,fp);
		fwrite(pIndex->szIndex, strlen(pIndex->szIndex), 1, fp);
		fwrite("]", 1,1 ,fp);
		fwrite("\r\n", 2, 1, fp);

		STKey*  pKey;
		pKey = pIndex->pFirstKey;

		while(pKey)
		{
			//key 
			fwrite(pKey->szKey , strlen(pKey->szKey), 1, fp);
			fwrite("=", 1, 1, fp);
			int nLen ;
			nLen = strlen(pKey->szValue);
			fwrite(pKey->szValue, strlen(pKey->szValue), 1,fp);
			fwrite("\r\n",2, 1, fp );
			pKey = pKey->pNextKey;
		}

		pIndex = pIndex->pNextIndex;
	}

	fclose(fp);
	
}

void CIni::Init()
{
	if(m_nDateLen<=0)
		return;

	int i = 0, nLineStartPos, nLineEndPos;
	while(i < m_nDateLen)
	{
		nLineStartPos =nLineEndPos= i;

		//find line end pos
		while( nLineEndPos < m_nDateLen-1&& m_pData[nLineEndPos]!='\n')
		{
			nLineEndPos++;
		}
		if(nLineEndPos>nLineStartPos)
		{
			//get a line ,set index or key 
			char szTemp[MAX_LINE_LEN]={0};
			memcpy(szTemp, &m_pData[nLineStartPos], nLineEndPos - nLineStartPos+1);
			szTemp[nLineEndPos - nLineStartPos+1]='\0';
			if(!SetIndex(szTemp))
			{
				//the line is not a  index  ,try set key 
				SetKeyAndValue(szTemp);
			}
		}
		

		i = nLineEndPos+1;	//next line
	}
}
bool CIni::SetIndex(const char* psz)
{
	int nLen = strlen(psz);
	int i,  indexStartPos,indexEndPos;
	i =  indexStartPos = indexEndPos = 0;
	while(i< nLen)
	{
		
		if(psz[i]=='[')
		{
			
			indexStartPos = indexEndPos=i;
			//find ']'
			while(indexEndPos <  nLen && psz[indexEndPos]!=']')
			{
				indexEndPos++;
			}
			if(indexEndPos <  nLen)
			{
				indexStartPos++;
				indexEndPos--;
				if(indexEndPos>=indexStartPos)
				{
					//find index succeed!!
					char szIndex[MAX_INDEX_LEN];
					memcpy(szIndex, &psz[indexStartPos], indexEndPos-indexStartPos+1);
					szIndex[indexEndPos-indexStartPos+1]='\0';
					TrimSpace(szIndex);
					AddIndex(szIndex);
					return true;

				}
			}
		}
		
		i++;

		
	}
	return false;
}

void CIni::AddIndex(const char* pszIndex)
{
	STIndex* pNewIndex = new STIndex();

	strcpy(pNewIndex->szIndex, pszIndex);

	if(m_pFirstIndex==NULL)
	{
		//first index
		m_pFirstIndex = pNewIndex;
	}
	else
	{
		STIndex* pLastIndex = m_pFirstIndex;
		while(pLastIndex->pNextIndex)
		{
			pLastIndex = pLastIndex->pNextIndex;
		}
		pLastIndex->pNextIndex = pNewIndex;

	}
}
bool CIni::SetKeyAndValue(const char* psz)
{
	int i, nKeyEndPos,nValueStartPos;
	int nLen = strlen(psz);
	i = nKeyEndPos =nValueStartPos= 0;
	while(i<nLen)
	{
		if(psz[i]=='=')
		{
			
			nKeyEndPos = i-1;
			nValueStartPos = i+1;

			if(nKeyEndPos>=0 && nValueStartPos <nLen)
			{
				////find key!
				char szKey[MAX_KEY_LEN];
				char szValue[MAX_VALUE_BUF_LEN];
				memcpy(szKey, psz, nKeyEndPos-0+1);
				szKey[nKeyEndPos+1]='\0';
				memcpy(szValue,&psz[nValueStartPos], nLen-nValueStartPos);

				szValue[nLen-nValueStartPos]='\0';

			 
				TrimSpace(szKey);
				TrimSpace(szValue);
				AddKeyAndValue(szKey,szValue);
				return true;
			}

		}
		i++;
	}

	return false;
}

void CIni::AddKeyAndValue(const char* pszKey, int nValue)
{
	char szValue[MAX_VALUE_BUF_LEN];
	sprintf(szValue, "%d", nValue);
	AddKeyAndValue(pszKey, szValue);
}

void CIni::AddKeyAndValue(const char* pszKey,const char* pszValue )
{
	STKey* pKey = new STKey();
//	STValue* pValue = new STValue();
//	pKey->pValue = pValue;
	strcpy( pKey->szKey, pszKey);
	strcpy( pKey->szValue, pszValue);
	//the last pIndex in the list is pKey's parent
	cout<<"szKey ="<<pszKey<<" , szValue="<<pszValue<<endl;

	STIndex* pLastIndex = GetLastIndex();
	if(pLastIndex)
	{
		if(pLastIndex->pFirstKey==NULL)
		{
			//first key
			pLastIndex->pFirstKey = pKey;
		}
		else
		{
			//add to last key 
			STKey* pLastKey = pLastIndex->pFirstKey;
			while(pLastKey->pNextKey)
			{
				pLastKey = pLastKey->pNextKey;
			}
			pLastKey->pNextKey = pKey;
		}
	}

	
}

CIni::STIndex* CIni::GetLastIndex()
{
	STIndex* pIndex = m_pFirstIndex;

	while(pIndex->pNextIndex)
	{
		pIndex = pIndex->pNextIndex;
	}
	return pIndex;
}

int CIni::GetFileLength(const char *pszFileName)
{
	int fh, nbytes;

	fh = _open( pszFileName, 0x0000 );	
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
	
//	STValue*   pValue;
	STKey*  pKey;
	pKey = GetKey(pszIndex, pszKey);
	if(pKey==NULL)
		return nDefault;

	int nRet;
	nRet = atoi(pKey->szValue);
	return nRet;


}
void CIni::GetStr(const char* pszIndex, const char* pszKey,
			const char* pszDefault,
			char* szBuf, int nBufLen)
{
	if( pszIndex==NULL||pszKey==NULL|| pszDefault==NULL
		||szBuf==NULL||nBufLen<=0) return ;

	STKey*   pKey;
	pKey = GetKey(pszIndex, pszKey);
	if(pKey==NULL)
	{
		strcpy(szBuf, pszDefault);
	}
	else
	{
		strcpy(szBuf, pKey->szValue);
	}
	
/*

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
		strcpy(szBuf,"");*/

}


CIni::STIndex* CIni::GetIndex(const char* pszIndex)
{
	if(pszIndex==NULL)
		return NULL;

	STIndex* pIndex;
	pIndex = m_pFirstIndex;
	while(pIndex)
	{
		if( strcmp(pIndex->szIndex, pszIndex)==0)
		{
			return pIndex;
		}
		pIndex = pIndex->pNextIndex; 
	}
	return NULL;
}
CIni::STKey*   CIni::GetKey(const char* pszIndex,const char* pszKey)
{
	if(pszIndex==NULL||pszKey==NULL)
		return NULL;
	STIndex* pIndex;
	pIndex = GetIndex(pszIndex);
	if(pIndex==NULL)
		return NULL;
	STKey*  pKey;
	pKey = pIndex->pFirstKey;
	while(pKey)
	{
		if(strcmp(pKey->szKey, pszKey)==0)
			return pKey;
		pKey = pKey->pNextKey;
	}

	return NULL;
}
/*
CIni::STValue* CIni::GetValue(const char* pszIndex, const char* pszKey)
{
	if(pszIndex==NULL||pszKey==NULL)
		return NULL;

	STKey* pKey = GetKey(pszIndex, pszKey);
	if(pKey)
	{
		return pKey->pValue;
	}
	else
		return NULL;


}*/
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
				memset(szIndex,0,sizeof(szIndex));
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
			memset(szKey,0,sizeof(szKey));
			memcpy(szKey, &m_pData[i], j-i);

			TrimSpace(szKey);
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
	while (nPos!=m_nDateLen &&m_pData[nPos]!='\n' )
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
	TrimSpace(pszBuf);

	if(pRet)
	{
		*pRet= atoi(pszBuf);
	}
	return true;
}

void CIni::TrimSpace(char* pszBuf)
{
	if(pszBuf==NULL)
		return;

	int nLen= strlen(pszBuf);
	char szBufTemp[MAX_VALUE_BUF_LEN]={0};

	int i=0;
	int j =0;
	while(i< nLen )
	{
		if(pszBuf[i]!=' ' && pszBuf[i]!='\n'&&pszBuf[i]!='\r')
		{
			szBufTemp[j] = pszBuf[i];
			j++;
		}
		i++;
	}
	strcpy(pszBuf, szBufTemp);
	
}

CIni::STKey*   CIni::GetLastKey(STIndex* pIndex)
{
	if(pIndex==NULL)
		return NULL;
	STKey* pKey;
	pKey = pIndex->pFirstKey;
	while(pKey&&pKey->pNextKey)
	{
		pKey = pKey->pNextKey;
	}

	return pKey;
}


void CIni::SetInt(const char* pszIndex, const char* pszKey, int nValue)
{

	if(pszIndex==NULL||pszKey==NULL)
		return ;

	STIndex* pIndex;
	STKey*   pKey;
	//see index is exist?
	char szValue[MAX_VALUE_BUF_LEN];
	sprintf(szValue, "%d", nValue);

	pIndex = GetIndex(pszIndex);
	if(pIndex)
	{

		//exist 
		
		//see key is exist?
		pKey = GetKey(pszIndex, pszKey);
		if(pKey)
		{
			// key exist, reset value
		//	STValue* pValue = pKey->pValue;
			
			strcpy(pKey->szValue, szValue);

		}
		else
		{
			//key not exist ,add to tail
			STKey* pLastKey;
			pLastKey = GetLastKey(pIndex);
			STKey* pNewKey;
			pNewKey = new STKey();
			//STValue* pValue;
		//	pValue = new STValue();
		//	pNewKey->pValue = pValue;
			if(pNewKey==NULL)
				return;
			strcpy(pNewKey->szValue,szValue );
			
		}
	}
	else
	{
		//index not exist
	}

	/*
	//find index 
	int nIndexPos = 0;
	int nKeyPos = 0;
	int nRet= 0 ;
	char szValueBuf[MAX_VALUE_BUF_LEN]={0};
	 if( (nIndexPos =FindIndex(pszIndex))!=-1)
	 {
		 if( (nKeyPos = FindKey(pszKey, nIndexPos))!=-1)
		 {
			  //key matched ,so modify value

		 }
	 }*/
}
void CIni::SetStr(const char* pszIndex, const char* pszKey,const char* pszValue)
{
}