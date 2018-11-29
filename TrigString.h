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
// TrigString.h: interface for the CTrigString class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRIGSTRING_H__A0FF8549_C757_4712_8EFB_9D7E7E499FCF__INCLUDED_)
#define AFX_TRIGSTRING_H__A0FF8549_C757_4712_8EFB_9D7E7E499FCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTrigString
{
public:
	void UnsetTrigString();
	CTrigString(CString str, int ID);
	CString GetTrigStringName();
	CString GetTrigStringValue();
	void SetTrigStringValue(CString str);
	int GetTrigStringID();
	void SetTrigStringID(int ID);
	static BOOL IsTrigStringName(CString str);
	static int GetTrigStringID(CString trigstr);
	CTrigString();
	virtual ~CTrigString();

private:
	CString m_sStrValue;
	int m_iTrigID;
};


#endif // !defined(AFX_TRIGSTRING_H__A0FF8549_C757_4712_8EFB_9D7E7E499FCF__INCLUDED_)
