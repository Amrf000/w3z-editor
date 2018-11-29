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
// StringCountList.h: interface for the CStringCountList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRINGCOUNTLIST_H__80587DED_340E_44BE_810D_67142BAACA68__INCLUDED_)
#define AFX_STRINGCOUNTLIST_H__80587DED_340E_44BE_810D_67142BAACA68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStringCountList
{

	class CStringCount
	{
	public:
		int count;
		CString string;
		CStringCount();
		virtual ~CStringCount();	
	};

	class CListedStringCount
	{
	public:
		CStringCount stringCount;
		CListedStringCount *next;
		CListedStringCount();
		virtual ~CListedStringCount();	
	};

	typedef CListedStringCount* PCListedStringCount;

public:
	void ClearList();
	PCListedStringCount m_pHead;
	double GetBestCountRatio();
	CString GetBestCountStr() const;
	void AddString(CString str);
	CStringCountList();
	virtual ~CStringCountList();

};

#endif // !defined(AFX_STRINGCOUNTLIST_H__80587DED_340E_44BE_810D_67142BAACA68__INCLUDED_)
