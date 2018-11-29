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
// HashCountList.cpp: implementation of the CHashCountList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "w3zmapedit.h"
#include "HashCountList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



CHashCountList::CHashCount::CHashCount()
{
	count = 0;
	uHash = 0;
}

CHashCountList::CHashCount::~CHashCount()
{
}


//
CHashCountList::CListedHashCount::CListedHashCount()
{
	next = NULL;
}

CHashCountList::CListedHashCount::~CListedHashCount()
{
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHashCountList::CHashCountList()
{
	m_pHead = NULL;
}

CHashCountList::~CHashCountList()
{
	ClearList();
}


/*
ClearList
*********
Empty the list (remove all its hash count elements).

In:
  nothing

Out:
  nothing

Return:
  nothing

*/
void CHashCountList::ClearList()
{
	PCListedHashCount ptLHashCount;

	while(NULL != m_pHead)
	{
		ptLHashCount	= m_pHead;
		if (m_pHead == m_pHead->next)
		{// in case of a cyclic bug
			m_pHead = NULL;
		}
		else
		{
			m_pHead = m_pHead->next;
		}
		delete ptLHashCount;	
	}
}


/*
AddHash
*******
Add a hash to the list or increase his counter if it's already in the list.

In:
  unsigned int uHash: hash to add

Out:
  nothing

Return:
  nothing

*/
void CHashCountList::AddHash(unsigned int uHash)
{
	PCListedHashCount ptLHashCount;
	ptLHashCount = m_pHead;
	BOOL bFound = FALSE;

	while((NULL != ptLHashCount) && (FALSE == bFound))
	{
		if (uHash == ptLHashCount->tHashCount.uHash)
		{// hash found, increase its count
			ptLHashCount->tHashCount.count++;
			// and say it has been found
			bFound = TRUE;
		}
		if (ptLHashCount == ptLHashCount->next)
		{// cyclic bug
			ptLHashCount->next = NULL;
		}
		ptLHashCount = ptLHashCount->next;
	}

	if (FALSE == bFound)
	{// not found, add it to the list
		ptLHashCount = new CListedHashCount;
		ptLHashCount->tHashCount.uHash = uHash;
		ptLHashCount->tHashCount.count = 1; // 1 occurence found
		ptLHashCount->next = m_pHead;
		m_pHead = ptLHashCount;
	}
}


/*
GetBestCount
************
Returns the most used hash (with the highest count) or 0 if the list is empty.

In:
  nothing

Out:
  nothing

Return:
  int: the hash of the list with the highest count or 0 if the list is empty

*/
unsigned int CHashCountList::GetBestCountHash()
{
	PCListedHashCount ptLHashCount;
	PCListedHashCount ptLHashCountBest;
	unsigned int uHashRet;

	ptLHashCountBest = m_pHead;
	ptLHashCount = m_pHead;
	while(NULL != ptLHashCount)
	{
		if (ptLHashCountBest->tHashCount.count < ptLHashCount->tHashCount.count)
		{// change the best pointer to point the best hash count
			ptLHashCountBest = ptLHashCount;
		}
		if (ptLHashCount == ptLHashCount->next)
		{// cyclic bug
			ptLHashCount->next = NULL;
		}
		ptLHashCount = ptLHashCount->next;
	}

	if (NULL != ptLHashCountBest)
	{// the list is not empty
		uHashRet = ptLHashCountBest->tHashCount.uHash;
	}
	else
	{
		uHashRet = 0;
	}
	return uHashRet;
}


/*
GetBestCountRatio
*****************
Returns a number between 0 and 100 that is the % of the highest hash in the hash list.
For example the list contains 3 hashs 0x12345678, 0x09876543 and 0x12121212.
0x12345678 is there 13 times, 0x09876543 10 and 0x12121212 only 4.
The function will return: (13/ (13 + 10 + 4))*100.
If the list is empty, it'll return 0.

In:
  Nothing

Out:
  Nothing

Return:
 double: the best hash count ratio or 0 if the list is empty

*/
double CHashCountList::GetBestCountRatio()
{
	double dRet = 0;
	double dSum = 0;
	PCListedHashCount ptLHashCount;
	PCListedHashCount ptLHashCountBest;

	ptLHashCountBest = m_pHead;
	ptLHashCount = m_pHead;
	while(NULL != ptLHashCount)
	{
		dSum += ptLHashCount->tHashCount.count;
		if (ptLHashCountBest->tHashCount.count < ptLHashCount->tHashCount.count)
		{
			ptLHashCountBest = ptLHashCount;
		}
		if (ptLHashCount == ptLHashCount->next)
		{// cyclic bug
			ptLHashCount->next = NULL;
		}
		ptLHashCount = ptLHashCount->next;
	}

	if (0 != dSum)
	{
		dRet = ((double)ptLHashCountBest->tHashCount.count/dSum)*100;
	}
	else
	{
		dRet = 0;
	}
	return dRet;
}
