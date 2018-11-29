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
// UnitsList.h: interface for the CUnitsList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UNITSLIST_H__1CE7F16C_7F1D_4A8C_B9CB_5F89F13D746B__INCLUDED_)
#define AFX_UNITSLIST_H__1CE7F16C_7F1D_4A8C_B9CB_5F89F13D746B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Unit.h"

class CUnitsList  
{
	class CListedUnit  
	{
	public:
		CListedUnit* next;
		CUnit unit;
		CListedUnit();
		virtual ~CListedUnit();
	};

public:
	int GetNbUnits();
	void ClearList();
	void GoFirstUnit();
	CUnit GetNext();
	void AddUnit(CUnit unit);
	CUnitsList();
	virtual ~CUnitsList();

protected:
	int m_iNbUnits;
	CListedUnit* m_pCurrent;
	CListedUnit* m_pHead;
};

#endif // !defined(AFX_UNITSLIST_H__1CE7F16C_7F1D_4A8C_B9CB_5F89F13D746B__INCLUDED_)
