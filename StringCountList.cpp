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
// StringCountList.cpp: implementation of the CStringCountList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StringCountList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CStringCountList::CStringCount::CStringCount()
{
	count = 0;
	string = "";
}

CStringCountList::CStringCount::~CStringCount()
{
}


//
CStringCountList::CListedStringCount::CListedStringCount()
{
	next = NULL;
}

CStringCountList::CListedStringCount::~CListedStringCount()
{
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStringCountList::CStringCountList()
{
	m_pHead = NULL;
}


CStringCountList::~CStringCountList()
{
	ClearList();
}



/*
ClearList
*********
Empty the list (remove all its string count elements).

In:
  nothing

Out:
  nothing

Return:
  nothing

*/
void CStringCountList::ClearList()
{
	PCListedStringCount ptLStrCount;

	while(NULL != m_pHead)
	{
		ptLStrCount	= m_pHead;
		if (m_pHead == m_pHead->next)
		{// in case of a cyclic bug
			m_pHead = NULL;
		}
		else
		{
			m_pHead = m_pHead->next;
		}
		delete ptLStrCount;	
	}
}


/*
AddString
*********
Add a string to the list or increase his counter if it's already in the list.

In:
  CString str: string to add

Out:
  nothing

Return:
  nothing

*/
void CStringCountList::AddString(CString str)
{
	PCListedStringCount ptLStrCount;
	ptLStrCount = m_pHead;
	BOOL bFound = FALSE;

	while((NULL != ptLStrCount) && (FALSE == bFound))
	{
		if (str == ptLStrCount->stringCount.string)
		{// string found, increase its count
			ptLStrCount->stringCount.count++;
			// and say it has been found
			bFound = TRUE;
		}
		if (ptLStrCount == ptLStrCount->next)
		{// cyclic bug
			ptLStrCount->next = NULL;
		}
		ptLStrCount = ptLStrCount->next;
	}

	if (FALSE == bFound)
	{// not found, add it to the list
		ptLStrCount = new CListedStringCount;
		ptLStrCount->stringCount.string = str;
		ptLStrCount->stringCount.count = 1; // 1 occurence found
		ptLStrCount->next = m_pHead;
		m_pHead = ptLStrCount;
	}
}


/*
GetBestCount
************
Returns the most used string (with the highest count) or a null string if the list is empty.

In:
  nothing

Out:
  nothing

Return:
  CString: the string of the list with the highest count or a null string if the list is empty

*/
CString CStringCountList::GetBestCountStr() const
{
	PCListedStringCount ptLStrCount;
	PCListedStringCount ptLStrCountBest;
	CString strRet;

	ptLStrCountBest = m_pHead;
	ptLStrCount = m_pHead;
	while(NULL != ptLStrCount)
	{
		if (ptLStrCountBest->stringCount.count < ptLStrCount->stringCount.count)
		{// change the best pointer to point the best string count
			ptLStrCountBest = ptLStrCount;
		}
		if (ptLStrCount == ptLStrCount->next)
		{// cyclic bug
			ptLStrCount->next = NULL;
		}
		ptLStrCount = ptLStrCount->next;
	}

	if (NULL != ptLStrCountBest)
	{// the list is not empty
		strRet = ptLStrCountBest->stringCount.string;
	}
	else
	{
		strRet = "";
	}
	return strRet;
}


/*
GetBestCountRatio
*****************
Returns a number between 0 and 100 that is the % of the highest string in the string list.
For example the list contains 3 strings "foo", "faa" and "fee".
"foo" is there 13 times, "faa" 10 and "fee" only 4.
The function will return: (13/ (13 + 10 + 4))*100.
If the list is empty, it'll return 0.

In:
  Nothing

Out:
  Nothing

Return:
 double: the best string count ratio or 0 if the list is empty

*/
double CStringCountList::GetBestCountRatio()
{
	double dRet = 0;
	double dSum = 0;
	PCListedStringCount ptLStrCount;
	PCListedStringCount ptLStrCountBest;

	ptLStrCountBest = m_pHead;
	ptLStrCount = m_pHead;
	while(NULL != ptLStrCount)
	{
		dSum += ptLStrCount->stringCount.count;
		if (ptLStrCountBest->stringCount.count < ptLStrCount->stringCount.count)
		{
			ptLStrCountBest = ptLStrCount;
		}
		if (ptLStrCount == ptLStrCount->next)
		{// cyclic bug
			ptLStrCount->next = NULL;
		}
		ptLStrCount = ptLStrCount->next;
	}

	if (0 != dSum)
	{
		dRet = ((double)ptLStrCountBest->stringCount.count/dSum)*100;
	}
	else
	{
		dRet = 0;
	}
	return dRet;
}
