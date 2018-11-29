/*
    This file is part of W3ZMapEditor (c).

    W3ZMapEditor is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    W3ZMapEditor is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with W3ZMapEditor; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
// IDList.cpp: implementation of the CIDList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "War3TypesAndConstants.h"
#include "IDList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIDList::CIDList()
{
	m_pHead = NULL;
}

CIDList::~CIDList()
{
	ClearList();
}


/*
AddID
*****
Add a >= 0 ID to the IDs list. If the ID is < 0, it'll be ignored.

In:
  int ID: new ID

Out:
  none

Return:
  none

*/
void CIDList::AddID(int ID)
{
	PCListedInt pLint, pLintPrev;

	if (ID < 0)
	{// ignore negative IDs
		TRACE("AddID: negative ID not added: %i\n", ID);
		return;	
	}
	TRACE("AddID: trying to add ID %i\n", ID);
	pLint = m_pHead;
	pLintPrev = NULL;

	// sorted list, so find the right place to put the ID
	while (pLint && (pLint->i < ID))
	{
		pLintPrev = pLint;
		if (pLint == pLint->next)
		{// in case of a cyclic list bug
			TRACE("AddID: cyclic list bug detected!\n");
			pLint->next = pLint = NULL;
		}
		else
		{// next one!
			pLint = pLint->next;
		}
	}

	if (pLintPrev)
	{// not the head
		if (pLint)
		{// there is one after, check if the ID is already in the list
			if (pLint->i != ID)
			{ // ID not already here
				pLint = new CListedInt;
				pLint->i = ID;
				pLint->next = pLintPrev->next;
				pLintPrev->next = pLint;
			}
		}
		else
		{// last one
			pLint = new CListedInt;
			pLint->i = ID;
			pLint->next = NULL;
			pLintPrev->next = pLint;
		}
	}
	else
	{//first of the list
		pLint = new CListedInt;
		pLint->i = ID;
		pLint->next = m_pHead;
		m_pHead = pLint;
	}
}


/*
GetAndRemoveID
**************
Returns the next ID and remove that ID from the list.

In:
 none

Out:
 none

Return:
 int: a >= 0 ID number or -1 if the list is empty.

*/
int CIDList::GetAndRemoveID()
{
	int i;
	PCListedInt pLint;

	if (m_pHead)
	{
		i = m_pHead->i;
		pLint = m_pHead;
		m_pHead = m_pHead->next;
		delete pLint;
	}
	else
	{
		i = -1;
	}
	return i;
}


/*
ClearList
*********
Empty the ID list and free memory.

In:
 none

Out:
 none

Return:
 none

*/
void CIDList::ClearList()
{
	PCListedInt pLint;

	//remove each element of the list
	while(m_pHead)
	{
		pLint = m_pHead;
		if (m_pHead != m_pHead->next)
		{
			m_pHead = m_pHead->next;
		}
		else
		{
			TRACE("ClearList: cyclic list bug detected!\n");
			m_pHead = NULL;
		}
		// free memory
		delete pLint;
	}
}


/*
RemoveID
********
Remove an ID from the list.

In:
 int ID: ID to remove

Out:
 none

Remove:
 none

*/
void CIDList::RemoveID(int ID)
{
	PCListedInt pLint, pLintPrev;

	pLint = m_pHead;
	pLintPrev = NULL;

	// search the ID
	while (pLint && (pLint->i != ID))
	{
		pLintPrev = pLint;
		if (pLint == pLint->next)
		{// in case of a cyclic list bug
			TRACE("RemoveID: cyclic list bug detected!\n");
			pLint->next = pLint = NULL;
		}
		else
		{// next one!
			pLint = pLint->next;
		}
	}

	if (pLint)
	{// ID found
		//check if it's the head
		if (pLintPrev)
		{// not the head
			pLintPrev->next = pLint->next;
			delete pLint;
		}
		else
		{// is the head
			pLint = m_pHead;
			m_pHead = m_pHead->next;
			delete pLint;
		}
	}
}

/*
AddIDRange
**********
Add >= 0 IDs to the IDs list. If one of the IDs is < 0, it'll be ignored.
If last <= first, nothing will be added.

In:
  int first: first ID of the range
  int last: last ID of the range

Out:
  none

Return:
  none

*/
void CIDList::AddIDRange(int first, int last)
{
	PCListedInt pLint, pLintPrev;
	int ifrom, ito;

	ifrom = first;
	ito = last;
	if (last < 0)
	{// ignore negative IDs
		TRACE("AddIDRange: negative IDs not added!\n");
		return;	
	}
	if (ifrom < 0)
	{// add only >= 0 IDs
		ifrom = 0;
	}
	if (ifrom > ito)
	{// ignore
		return;
	}

	pLint = m_pHead;
	pLintPrev = NULL;

	if ((!m_pHead) || (ifrom < m_pHead->i))
	{// must replace the head
		pLint = new CListedInt;
		pLint->i = ifrom;
		pLint->next = m_pHead;
		pLintPrev = m_pHead = pLint;
		ifrom++;
	}

	// add the range
	while (ifrom <= ito)
	{
		if (pLint && (pLint->i < ifrom))
		{// ifrom is bigger, go to next
			pLintPrev = pLint;
			pLint = pLint->next;
		}
		else if (pLint && (pLint->i == ifrom))
		{// skip this ID which is already there
			pLintPrev = pLint;
			pLint = pLint->next;
			ifrom++;
		}
		else
		{// (pLint == NULL) or (pLint->i > ifrom) so add the ID
			pLint = new CListedInt;
			pLint->i = ifrom;
			pLint->next = pLintPrev->next;
			pLintPrev->next = pLint;
			pLintPrev = pLint;
			pLint = pLint->next;
			ifrom++;
		}

	}
}
