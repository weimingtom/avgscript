#include "WndManage.h"
#include "Ini.h"
#include "Dlg.h"
#include "Button.h"
CWndManage::CWndManage()
{
	//create top wnd ,all wnd 's parent

//	m_pTopParnetWnd = new CCommonWnd();
	m_pCurParentWnd = m_pTopParnetWnd= NULL;

}
CWndManage::~CWndManage()
{
//	if(m_pTopParnetWnd)
//	{

		//delete all his child

		
//		delete m_pTopParnetWnd;

		
//	}




}

bool CWndManage::AddWnd(CCommonWnd* pWnd)
{


	//add as m_pCurParentWnd 's child

//	if(m_pCurParentWnd==NULL)
//		return false;

	if(m_pCurParentWnd==NULL)	//most parent window
	{

		m_pCurParentWnd = m_pTopParnetWnd = pWnd;
		pWnd->SetParent(NULL);
		pWnd->SetChild(NULL);
		pWnd->SetZOrder(MAX_Z_ORDER-1);

	}
	else
	{
		//
		//add to last wnd
		CCommonWnd* pFirstWnd = m_pCurParentWnd->GetChild();

		if(pFirstWnd==NULL)
		{
			//he is first child
			m_pCurParentWnd->SetChild(pWnd);
			pWnd->SetParent(m_pCurParentWnd);
		//	pWnd->SetZOrder()
		}
		else
		{
			//add to end of child list
			CCommonWnd* pLastWnd;
			pLastWnd = GetLastWnd(pFirstWnd);
			pLastWnd->SetNext(pWnd);
			pWnd->SetPrev(pLastWnd);
			pWnd->SetParent(m_pCurParentWnd);

		}

	}

	return true;
}
bool CWndManage::DelWnd(CCommonWnd* pWnd)
{
	if(pWnd==NULL)
		return false;
	
	//first delete his child
	if(pWnd->GetChild())
	{
		DelWnd(pWnd->GetChild());
	}

	//he is first wnd 
	if(pWnd->GetPrev()==NULL)
	{
		//his parent 's child modify
		CCommonWnd* pParent = pWnd->GetParent();
		CCommonWnd* pNext = pWnd->GetNext();
		pParent->SetChild(pNext);
		if(pNext)
			pNext->SetPrev(NULL);
		
	}
	else
	{
		CCommonWnd* pNext = pWnd->GetNext();
		CCommonWnd* pPrev = pWnd->GetPrev();
		pPrev->SetNext(pNext);
		pNext->SetPrev(pPrev);

	}
	
	delete pWnd;
	
	return true;
}

bool CWndManage::LoadWnd(const char* pszFileName)
{

	if(pszFileName==NULL)
		return false;

	//load wnd count first
	CIni ini;
	if(!ini.Open(pszFileName))
		return false;

	int nCount = 0;
	nCount = ini.GetInt(Wnd_Index, Wnd_Key_Count, 0);
	for (int i = 0; i < nCount;i++)
	{
		//load all wnd

		//format index
		char szIndex[MAX_INDEX_LEN]={0};

		sprintf(szIndex, "%s%d", Wnd_Index, i);
		//load common attribute
		char szWndName[MAX_WND_VALUE_LEN]={0};
		char szWndCaption[MAX_WND_VALUE_LEN]={0};
		char szTextureFile[MAX_WND_VALUE_LEN]={0};
		int ID,X,Y,Width,Height,CanDrag,CanMove;

		ini.GetStr(szIndex, Wnd_Key_Name, "",szWndName, MAX_WND_VALUE_LEN);
		ini.GetStr(szIndex, Wnd_Key_Caption, "", szWndCaption, MAX_WND_VALUE_LEN);
		ini.GetStr(szIndex, Wnd_Key_TextureFile, "", szTextureFile, MAX_WND_VALUE_LEN);
		
		ID = ini.GetInt(szIndex,Wnd_Key_ID, Wnd_Default_IntValue);
		X = ini.GetInt(szIndex, Wnd_Key_X, Wnd_Default_IntValue);
		Y = ini.GetInt(szIndex, Wnd_Key_Y, Wnd_Default_IntValue);
		Width = ini.GetInt(szIndex, Wnd_Key_Width, Wnd_Default_IntValue);
		Height = ini.GetInt(szIndex, Wnd_Key_Height, Wnd_Default_IntValue);
		CanDrag = ini.GetInt(szIndex, Wnd_Key_CanDrag, 0);
		CanMove = ini.GetInt(szIndex, Wnd_Key_CanMove, 0);


		//get wnd kind 
		char szKind[MAX_WND_VALUE_LEN]={0};
		ini.GetStr(szIndex, Wnd_Key_Kind,szWndKind_UnKnown, szKind, MAX_WND_VALUE_LEN);
		int nKind;
		nKind = szKindToIntKind(szKind);

		
		CCommonWnd* pWnd = NULL;
		switch(nKind)
		{
		case WndKind_UnKnown:
			break;

		case WndKind_Dlg:
			{
				pWnd = new CDlg();

			}

			break;
		case WndKind_Button:
			{
				pWnd = new Cbutton();
			}
			break;
		}

		//set property
		pWnd->SetID(ID);
		pWnd->SetDisplayLeft(X);
		pWnd->SetDisplayTop(Y);
		pWnd->SetWidth(Width);
		pWnd->SetHeight(Height);
		pWnd->SetCanDrag(CanDrag==1?true:false);
		pWnd->SetCanMove(CanMove==1?true:false);
		pWnd->SetWndName(szWndName);
		pWnd->SetWndCaption(szWndCaption);
		pWnd->CreateTexture(szTextureFile);

		AddWnd(pWnd);
		
	}

	
	return true;
}

bool CWndManage::SaveWnd(const char* pszFileName)
{
	if(pszFileName==NULL)
		return false;

	return true;
}

int CWndManage::szKindToIntKind(const char* pszKind)
{
	if(pszKind==NULL)
		return WndKind_UnKnown;
	
	int nRet = WndKind_UnKnown;
	if(strcmp(pszKind, szWndKind_UnKnown))
		nRet = WndKind_UnKnown;
	else if(strcmp(pszKind, szWndKind_Dlg))
		nRet = WndKind_Dlg;
	else if(strcmp(pszKind, szWndKind_Button))
		nRet = WndKind_Button;
	else if(strcmp(pszKind, szWndKind_Text))
		nRet = WndKind_Text;
	else if(strcmp(pszKind, szWndKind_Combo))
		nRet = WndKind_Combo;
	else if(strcmp(pszKind, szWndKind_Scroll))
		nRet = WndKind_Scroll;
	else if(strcmp(pszKind, szWndKind_Menu))
		nRet = WndKind_Menu;
	else if(strcmp(pszKind, szWndKind_List))
		nRet = WndKind_List;
	else if(strcmp(pszKind, szWndKind_Radio))
		nRet = WndKind_Radio;
	else if(strcmp(pszKind, szWndKind_CheckBox))
		nRet = WndKind_CheckBox;
	return nRet;
}
void CWndManage::ChangeCurWnd(CCommonWnd* pWnd)
{
	m_pCurParentWnd = pWnd;
}

CCommonWnd* CWndManage::GetLastWnd(CCommonWnd* pWnd)
{
	if(pWnd==NULL)
		return NULL;

	while(pWnd->GetNext())
	{
		pWnd = pWnd->GetNext();
	}
	return pWnd;
}
CCommonWnd* CWndManage::GetFirstWnd(CCommonWnd* pWnd)
{
	if(pWnd==NULL)
		return NULL;

	while(pWnd->GetPrev())
	{
		pWnd = pWnd->GetPrev();
	}
	return pWnd;

}

void CWndManage::Draw(_RECT rect)
{
	//draw object in the rect

	if(m_pTopParnetWnd)
		m_pTopParnetWnd->Draw();
	


	
}

void CWndManage::MouseMove(int x, int y)
{
	if(m_pTopParnetWnd)
		m_pTopParnetWnd->MouseMove(x, y);
}

void CWndManage::MouseDown(int x, int y)
{
	CCommonWnd* pWnd=NULL;
	pWnd= HitTest(x, y);
	if(pWnd)
	{
		pWnd->MouseDown(x, y);
		ActiveWnd(pWnd);

	}
//	m_pTopParnetWnd->MouseDown(x, y);
}
void CWndManage::MouseUp(int x, int y)
{
	if(m_pTopParnetWnd)
		m_pTopParnetWnd->MouseUp(x, y);
}

void CWndManage::Drag(int x, int y,int nNextDragStatus)
{

	static CCommonWnd* pWnd=NULL;

	if(pWnd==NULL)
	{
		pWnd= HitTest(x, y);
		if(pWnd)
		{
			pWnd->Drag(x, y, nNextDragStatus);
			ActiveWnd(pWnd);

		}
	}
	else
	{
		pWnd->Drag(x, y, nNextDragStatus);

	}

	if(nNextDragStatus==Drag_Status_None)
		pWnd = NULL;
	
	//m_pTopParnetWnd->Drag(x, y,nNextDragStatus);
}

CCommonWnd* CWndManage::HitTest(int x, int y,CCommonWnd* pWndParent)
{

	//find Cwnd z-order most top

	CCommonWnd* pWnd ;
	CCommonWnd* pWndRet=NULL;


	if(pWndParent==NULL)
		pWnd = m_pTopParnetWnd;
	else
		pWnd = pWndParent;



	while(pWnd)
	{
		//his self
		if(pWnd->HitTest(x, y))
		{
			
			pWndRet = pWnd;
			if( pWnd->GetChild())
			{
				//recursion
				CCommonWnd* pWndChild;
				pWndChild = HitTest(x ,y , pWnd->GetChild());
				if(pWndChild)
				{
					//if his child hit 
					pWndRet = pWndChild;
				}
			}
		}



		//ok ,his brother ,because his next brother is draw later ,so is more
		//top than him 
		pWnd = pWnd->GetNext();
	}

	return pWndRet;
}


void CWndManage::ActiveWnd(CCommonWnd* pWnd)
{
	if(pWnd==NULL)
		return;

	//find parent until a container
	while(pWnd && !pWnd->GetIsContainer())
	{
		pWnd = pWnd->GetParent();
	}

	if(pWnd&& pWnd->GetIsContainer())
	{
		//he is a Container, so active him 
		//move him to end of list
		CCommonWnd* pParent = pWnd->GetParent();
		CCommonWnd* pFirst = GetFirstWnd(pWnd);
		CCommonWnd* pPrev = pWnd->GetPrev();
		CCommonWnd* pNext = pWnd->GetNext();
		CCommonWnd* pLast = GetLastWnd(pWnd);

		if(pLast == pWnd)
		{
			//he is already last 
			//do noting
			return;
		}


		if(pFirst ==  pWnd)
		{
			//he is first child , set his next brother as parent 's first child
			if(pParent)
				pParent->SetChild(pNext);

			pLast->SetNext(pWnd);
			pWnd->SetPrev(pLast);
			pNext->SetPrev(NULL);
			pWnd->SetNext(NULL);
		}
		else 
		{
			//he is not first child 
			pLast->SetNext(pWnd);
			
			pWnd->SetPrev(pLast);
			
			pNext->SetPrev(pPrev);
			pPrev->SetNext(pNext);
			pWnd->SetNext(NULL);
		}


	

		ChangeCurWnd(pWnd);

	}
}
