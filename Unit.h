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
// Unit.h: interface for the CUnit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UNIT_H__4B38F20E_948B_4CE8_8F03_9E2B5E5A994B__INCLUDED_)
#define AFX_UNIT_H__4B38F20E_948B_4CE8_8F03_9E2B5E5A994B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CUnit  
{
public:
	int owner;
	CString type;
	float z;
	float y;
	float x;
	CUnit();
	virtual ~CUnit();
};


#endif // !defined(AFX_UNIT_H__4B38F20E_948B_4CE8_8F03_9E2B5E5A994B__INCLUDED_)
