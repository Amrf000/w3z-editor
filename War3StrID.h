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
// War3StrID.h: interface for the CWar3StrID class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAR3STRID_H__01939F38_3641_4940_80BD_DAA8A53E0434__INCLUDED_)
#define AFX_WAR3STRID_H__01939F38_3641_4940_80BD_DAA8A53E0434__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


const int WAR3STRID_MAXCHAR = 4;

class CWar3StrID  
{
public:
	char operator [](int nIndex) const;
	BOOL operator !=(const char acStrID[]) const;
	BOOL operator !=(const CWar3StrID &otherStrID) const;
	BOOL operator ==(const char acStrID[]) const;
	BOOL operator ==(const CWar3StrID &otherStrID) const;
	void operator =(const char acStrID[]);
	void SetStrID(const char acNewID[WAR3STRID_MAXCHAR]);
	char* GetStrID();
	CWar3StrID(const char acStrID[]);
	CWar3StrID();
	virtual ~CWar3StrID();

protected:
	char strID[WAR3STRID_MAXCHAR+1];
};

#endif // !defined(AFX_WAR3STRID_H__01939F38_3641_4940_80BD_DAA8A53E0434__INCLUDED_)
