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
// ListedTilePoint.h: interface for the CListedTilePoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LISTEDTILEPOINT_H__1A79EB81_C7BE_4F9D_BC74_95580A8157F4__INCLUDED_)
#define AFX_LISTEDTILEPOINT_H__1A79EB81_C7BE_4F9D_BC74_95580A8157F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TilePoint.h"

class CListedTilePoint  
{
public:
	int y;
	int x;
	CListedTilePoint* next;
	CTilePoint tilepoint;
	CListedTilePoint();
	virtual ~CListedTilePoint();

};

#endif // !defined(AFX_LISTEDTILEPOINT_H__1A79EB81_C7BE_4F9D_BC74_95580A8157F4__INCLUDED_)
