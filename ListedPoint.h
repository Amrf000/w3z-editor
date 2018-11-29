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
// ListedPoint.h: interface for the CListedPoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LISTEDPOINT_H__4A2193D3_A4FF_463A_91E4_BF1610CFD8DE__INCLUDED_)
#define AFX_LISTEDPOINT_H__4A2193D3_A4FF_463A_91E4_BF1610CFD8DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CListedPoint  
{
public:
	CPoint point;
	CListedPoint* next;
	CListedPoint();
	virtual ~CListedPoint();

};

#endif // !defined(AFX_LISTEDPOINT_H__4A2193D3_A4FF_463A_91E4_BF1610CFD8DE__INCLUDED_)
