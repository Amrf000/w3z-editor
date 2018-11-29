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
// IDList.h: interface for the CIDList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IDLIST_H__C1926479_103F_499A_8EE1_B5582F9B6B31__INCLUDED_)
#define AFX_IDLIST_H__C1926479_103F_499A_8EE1_B5582F9B6B31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CIDList  
{
	// listed int to keep track of IDs
	class CListedInt
	{
	public:
		int i;
		CListedInt* next;
	};

	typedef CListedInt (*PCListedInt);

public:
	void AddIDRange(int first, int last);
	void RemoveID(int ID);
	void ClearList();
	int GetAndRemoveID();
	void AddID(int ID);
	CIDList();
	virtual ~CIDList();

protected:
	PCListedInt m_pHead;
};

#endif // !defined(AFX_IDLIST_H__C1926479_103F_499A_8EE1_B5582F9B6B31__INCLUDED_)
