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
// UnitsList.cpp: implementation of the CUnitsList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "War3TypesAndConstants.h"
#include "Unit.h"
#include "UnitsList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//********************************************************************************
CUnitsList::CListedUnit::CListedUnit()
{
	next = NULL;
}

CUnitsList::CListedUnit::~CListedUnit()
{

}
//********************************************************************************

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUnitsList::CUnitsList()
{
	m_pHead = NULL;
	m_pCurrent = NULL;
	m_iNbUnits = 0;
}

CUnitsList::~CUnitsList()
{
	ClearList();
}

void CUnitsList::ClearList()
{
	CListedUnit *pLUnit;
	m_pCurrent = NULL;
	// clear unit list
	while(m_pHead)
	{
		pLUnit = m_pHead;
		m_pHead = m_pHead->next;
		delete pLUnit;
	}
	m_iNbUnits = 0;
}

void CUnitsList::AddUnit(CUnit unit)
{
	CListedUnit *pLUnit;
	pLUnit = new CListedUnit;
	pLUnit->next = m_pHead;
	pLUnit->unit = unit;
	m_pHead = pLUnit;
	m_iNbUnits++;
}

void CUnitsList::GoFirstUnit()
{
	m_pCurrent = m_pHead;
}

CUnit CUnitsList::GetNext()
{
	CUnit unit;
	if	(m_pCurrent)
	{
		unit = m_pCurrent->unit;
		m_pCurrent = m_pCurrent->next;
	}
	else
	{
		m_pCurrent = m_pHead;
		unit.type = "";
	}
	return unit;
}

int CUnitsList::GetNbUnits()
{
	return m_iNbUnits;
}
