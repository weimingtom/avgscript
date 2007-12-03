#include "objectlist.h"

void CLineObjectList::Push(CLineObject* object)
{
	if(object)
		m_list.push_back(object);
}
void CLineObjectList::Clear()
{
	m_list.clear();
}
int  CLineObjectList::Size()
{
	return m_list.size();
}
void CLineObjectList::SetShowFlag(int flag)
{
}


