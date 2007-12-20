#include "WndManage.h"


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

	m_pTopParnetWnd->Draw();
	


	
}

void CWndManage::MouseMove(int x, int y)
{
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
