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
// TrigStringTable.cpp: implementation of the CTrigStringTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "War3TypesAndConstants.h"
#include "IDList.h"
#include "TrigString.h"
#include "TrigStringTable.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/********************************************************************************/
// CListedTrigString Class
/********************************************************************************/
/*
CListedTrigString
*****************
CListedTrigString constructors w/ and w/o trigger string member initialisation.
"next" member is always set to NULL.
*/
CTrigStringTable::CListedTrigString::CListedTrigString()
{
	next = NULL;
}

CTrigStringTable::CListedTrigString::CListedTrigString(CString str, int ID)
{
	next = NULL;
	trigstring.SetTrigStringID(ID);
	trigstring.SetTrigStringValue(str);
}

CTrigStringTable::CListedTrigString::~CListedTrigString()
{
}

/********************************************************************************/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTrigStringTable::CTrigStringTable()
{
	m_pHead = NULL;
	m_iHighestID = -1;
}

CTrigStringTable::~CTrigStringTable()
{
	ClearList();
}

/*
ClearList
*********
Empty list content and free memory.

In:
 nothing

Out:
 nothing

Return:
 nothing
*/
void CTrigStringTable::ClearList()
{
	PCListedTrigString pLTrigStr;

	// remove all the elements.
	while (m_pHead)
	{
		pLTrigStr = m_pHead;
		if (m_pHead == m_pHead->next)
		{// in case of a cyclic list bug
			m_pHead = NULL;
		}
		else
		{
			m_pHead = m_pHead->next;
		}
		delete pLTrigStr;
	}
	// free ID List
	m_iHighestID = -1;
	m_FreeIDList.ClearList();
}


/*
AddTrigString
*************
Add a trigger string to the table using the specified ID (must be >= 0).

In:
 CString str: the string value
 int ID: the string ID (must be >= 0 to be added)

Out:
 nothing

Return:
 BOOL: TRUE if succesfull, FALSE if the trigger string already exists or can't be added.

*/
BOOL CTrigStringTable::AddTrigString(CString str, int ID)
{
	BOOL bAdded = FALSE;
	PCListedTrigString pLTrigStr;

	if(ID < 0)	
	{return FALSE;}

	if (!m_pHead)
	{
		m_pHead = new CListedTrigString(str, ID);
		bAdded = TRUE;
	}
	else
	{// start from the head and add in the ordered list
		pLTrigStr = m_pHead;
		while (pLTrigStr && (pLTrigStr->trigstring.GetTrigStringID() != ID))
		{// check if the ID is already in use
			if (pLTrigStr->next)
			{
				if (pLTrigStr->next == pLTrigStr)
				{// in case of a cyclic list bug
					pLTrigStr->next = NULL; // fixes the problem
					pLTrigStr = NULL; // ends search
				}
				else
				{ // go to next
					pLTrigStr = pLTrigStr->next;
				}
			}
			else
			{// if there is no next one, it means the ID is free so we can add it
				pLTrigStr->next = new CListedTrigString(str, ID);
				pLTrigStr = NULL; // ends search
				bAdded = TRUE;
			}
		}
	}
	if (bAdded)
	{//if added, check if we have new free IDs
		if (ID > m_iHighestID)
		{
			if (ID > (m_iHighestID + 1))
			{// free space between the highest and the new ID, add it
				m_FreeIDList.AddIDRange(m_iHighestID, ID);
			}
			m_iHighestID = ID;
		}
		//remove the ID from the ID list
		m_FreeIDList.RemoveID(ID);
	}
	return bAdded;
}


/*
DelTrigString
*************
Remove a specific trigger string.

In:
 int ID: ID of the trigger string to remove

Out:
 nothing

Return:
 BOOL: TRUE if succesfully removed, FALSE if the trigger was not found.

*/
BOOL CTrigStringTable::DelTrigString(int ID)
{
	BOOL bDeled = FALSE;
	PCListedTrigString pLTrigStr, pLTrigStrPrev;

	// start from the head
	pLTrigStr = m_pHead;
	pLTrigStrPrev = NULL; // no previous here
	while (pLTrigStr)
	{
		if (pLTrigStr->trigstring.GetTrigStringID() == ID)
		{// ID found
			if (pLTrigStrPrev)
			{
				pLTrigStrPrev->next = pLTrigStr->next;
			}
			else
			{// head
				m_pHead = pLTrigStr->next;
			}
			delete pLTrigStr;
			pLTrigStr = NULL;
			bDeled = TRUE;
		}
		else
		{// ID not found
			if (pLTrigStr->next == pLTrigStr)
			{// in case of a cyclic list bug
				TRACE("DelTrigString: cyclic list bug detected!\n");
				pLTrigStr->next = NULL; // fixes the problem
				pLTrigStr = NULL; // ends search
			}
			else
			{// go to next
				pLTrigStrPrev = pLTrigStr;
				pLTrigStr = pLTrigStr->next;
			}
		}
	}

	if (bDeled)
	{// add the free ID to the ID list
		m_FreeIDList.AddID(ID);
	}
	return bDeled;
}


/*
SetTrigString
*************
set the trigger string content of the specified trigger string.

In:
 CString str: the new string value
 int ID: the string ID (must be >= 0)

Out:
 nothing

Return:
 BOOL: TRUE if succesfull, FALSE if the trigger string can't be found.

*/
BOOL CTrigStringTable::SetTrigString(CString str, int ID)
{
	BOOL bSet = FALSE;
	PCListedTrigString pLTrigStr;

	// start from the head
	pLTrigStr = m_pHead;
	while (pLTrigStr)
	{
		if (pLTrigStr->trigstring.GetTrigStringID() == ID)
		{// ID found
			pLTrigStr->trigstring.SetTrigStringValue(str);
			pLTrigStr = NULL; // ends search
			bSet = TRUE;
		}
		else
		{
			if (pLTrigStr->next == pLTrigStr)
			{// in case of a cyclic list bug
				pLTrigStr->next = NULL; // fixes the problem
				pLTrigStr = NULL; // ends search
			}
			else
			{ // go to next
				pLTrigStr = pLTrigStr->next;
			}
		}
	}
	return bSet;
}

/*
GetTrigString
*************
Returns the trigger string content of the specified trigger string.

In:
 int ID: trigger string ID (must be >= 0)

Out:
 nothing

Return:
 CString: the trigger string content or "" if the trigger string was not found

*/
CString CTrigStringTable::GetTrigStringValue(int ID)
{
	CString str = "";
	PCListedTrigString pLTrigStr;

	// start from the head
	pLTrigStr = m_pHead;
	while (pLTrigStr)
	{
		if (pLTrigStr->trigstring.GetTrigStringID() == ID)
		{// ID found
			str = pLTrigStr->trigstring.GetTrigStringValue();
			pLTrigStr = NULL; // ends search
		}
		else
		{
			if (pLTrigStr->next == pLTrigStr)
			{// in case of a cyclic list bug
				pLTrigStr->next = NULL; // fixes the problem
				pLTrigStr = NULL; // ends search
			}
			else
			{ // go to next
				pLTrigStr = pLTrigStr->next;
			}
		}
	}
	return str;
}

/*
AddTrigString
*************
Add a trigger string to the table and return the ID used.

In:
  CString str: the string value to add
Out:

Return:
 int: a >= 0 number wich is the trigger string ID if successfully added, -1 otherwise

*/
int CTrigStringTable::AddTrigString(CString str)
{
	int i = -1;
	i = m_FreeIDList.GetAndRemoveID();
	if (i < 0)
	{// empty ID list
		m_iHighestID++;
		i = m_iHighestID;
	}
	if (!AddTrigString(str, i))
	{i = -1;}


	return i;
}


/*
GetTrigStringName
*****************
Returns the trigger string name of the specified trigger string. (like "TRIGSTR_000")

In:
 int ID: trigger string ID (must be >= 0)

Out:
 nothing

Return:
 CString: the trigger string name or "" if the trigger string was not found

*/
CString CTrigStringTable::GetTrigStringName(int ID)
{
	CString str = "";
	PCListedTrigString pLTrigStr;

	// start from the head
	pLTrigStr = m_pHead;
	while (pLTrigStr)
	{
		if (pLTrigStr->trigstring.GetTrigStringID() == ID)
		{// ID found
			str = pLTrigStr->trigstring.GetTrigStringName();
			pLTrigStr = NULL; // ends search
		}
		else
		{
			if (pLTrigStr->next == pLTrigStr)
			{// in case of a cyclic list bug
				pLTrigStr->next = NULL; // fixes the problem
				pLTrigStr = NULL; // ends search
			}
			else
			{ // go to next
				pLTrigStr = pLTrigStr->next;
			}
		}
	}
	return str;
}


/*
GoFirstTrigString
*****************
Initialise the m_pNext pointer to m_pHead.
Should be called once before using GetNextTrigString to read the whole list.

In:
 none

Out:
 none

Return:
 none

*/
void CTrigStringTable::GoFirstTrigString()
{
	m_pNext = m_pHead;
}

/*
GetNextTrigString
*****************
Returns the trigger string pointed by m_pNext and make it point to the next one.

In:
 none

Out:
 none

Return:
 CTrigString: the next trigger string of the list if there is one.
  If the end of list has been reached, the trigger string ID returned is negative (== TSTR_UNKNOWN_ID).

*/
CTrigString CTrigStringTable::GetNextTrigString()
{
	CTrigString trigstr;

	if (m_pNext)
	{
		trigstr = m_pNext->trigstring;
		m_pNext = m_pNext->next;
	}

	return trigstr;
}
