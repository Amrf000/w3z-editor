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
// TilePointsTable.h: interface for the CTilePointsTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TILEPOINTSTABLE_H__88878ABD_439B_4202_96DA_DE693D223233__INCLUDED_)
#define AFX_TILEPOINTSTABLE_H__88878ABD_439B_4202_96DA_DE693D223233__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TilePoint.h"

class CTilePointsTable  
{
public: 
	int GetVersion();
	void SetVersion(int iVersion);
	CByteArray m_baUnsupportedBuffer;

	void ReplaceGroundType(int tileold, int tilenew, bool selectedonly= false);
	void UnselectAll();
	CTilePoint GetTilePoint(int num);
	void SetTilePoint(int num, CTilePoint tilepoint);
	void ClearTable();
	int GetWidth();
	int GetHeight();
	void SetSize(int width, int height = 1);
	CTilePointsTable(int width, int height =  1);
	CTilePointsTable();
	virtual ~CTilePointsTable();

protected:
	int m_iW3EVersion;
	int m_width;
	int m_height;
	CTilePoint* m_pTilePoints;

};

#endif // !defined(AFX_TILEPOINTSTABLE_H__88878ABD_439B_4202_96DA_DE693D223233__INCLUDED_)
