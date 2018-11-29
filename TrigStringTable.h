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
// TrigStringTable.h: interface for the CTrigStringTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRIGSTRINGTABLE_H__AB4E6705_EC31_491E_AA77_6BD4454E35E6__INCLUDED_)
#define AFX_TRIGSTRINGTABLE_H__AB4E6705_EC31_491E_AA77_6BD4454E35E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IDList.h"
#include "TrigString.h"

class CTrigStringTable  
{
	class CListedTrigString
	{
	public:
		CListedTrigString(CString str, int ID);
		CListedTrigString* next;
		CTrigString trigstring;
		CListedTrigString();
		virtual ~CListedTrigString();
	};

	typedef CListedTrigString (*PCListedTrigString);

public:
	CTrigString GetNextTrigString();
	void GoFirstTrigString();
	CString GetTrigStringName(int ID);
	int AddTrigString(CString str);
	CString GetTrigStringValue(int ID);
	BOOL SetTrigString(CString str, int ID);
	BOOL DelTrigString(int ID);
	BOOL AddTrigString(CString str, int ID);
	void ClearList();
	CTrigStringTable();
	virtual ~CTrigStringTable();

protected:
	int m_iHighestID;
	CIDList m_FreeIDList; // list of free IDs
	PCListedTrigString m_pNext;
	PCListedTrigString m_pHead;
};

#endif // !defined(AFX_TRIGSTRINGTABLE_H__AB4E6705_EC31_491E_AA77_6BD4454E35E6__INCLUDED_)
