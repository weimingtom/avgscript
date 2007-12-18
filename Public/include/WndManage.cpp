#include "WndManage.h"


CWndManage::CWndManage()
{
	//create top wnd ,all wnd 's parent

	m_pTopParnetWnd = new CCommonWnd();
	m_pCurParentWnd = m_pTopParnetWnd;

}
CWndManage::~CWndManage()
{
	if(m_pTopParnetWnd)
	{

		//delete all his child

		
		delete m_pTopParnetWnd;

		
	}




}

bool CWndManage::AddWnd(CCommonWnd* pWnd)
{


	//add as m_pCurParentWnd 's child

	if(m_pCurParentWnd==NULL)
		return false;

	if(m_pCurParentWnd->GetChild()==NULL)
	{
		// current wnd do not have child 
		m_pCurParentWnd->SetChild(pWnd);
		pWnd->SetParent(m_pCurParentWnd);
	}
	else
	{

		//add to last wnd
		CCommonWnd* pFirstWnd = m_pCurParentWnd->GetChild();
		CCommonWnd* pLastWnd;
		pLastWnd = GetLastWnd(pFirstWnd);
		pLastWnd->SetNext(pWnd);
		pWnd->SetPrev(pLastWnd);
		pWnd->SetParent(m_pCurParentWnd);
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


void CWndManage::Draw(_RECT rect)
{
	//draw object in the rect

	m_pTopParnetWnd->Draw();
	


	
}