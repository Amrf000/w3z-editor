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
// TilesetGroup.h: interface for the CTilesetGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TILESETGROUP_H__A9B2B76F_E92F_4249_A742_35E960D9D0CE__INCLUDED_)
#define AFX_TILESETGROUP_H__A9B2B76F_E92F_4249_A742_35E960D9D0CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTilesetGroup  
{
public:
	void AddTile(CWar3StrID tileID);
	void AddCliff(CWar3StrID cliffID);
	CTilesetGroup();
	virtual ~CTilesetGroup();
	CString groupName;
	CString tiles;
	CString cliffs;
	COLORREF waterColor;
	float fWaterAlpha;
	char groupID;
};

#endif // !defined(AFX_TILESETGROUP_H__A9B2B76F_E92F_4249_A742_35E960D9D0CE__INCLUDED_)
