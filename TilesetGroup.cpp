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
// TilesetGroup.cpp: implementation of the CTilesetGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "W3ZMapEdit.h"
#include "TilesetGroup.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTilesetGroup::CTilesetGroup()
{
	groupID = TG_CHAR_ID_UNDEFINED;
	groupName = TG_STR_NAME_UNDEFINED;
	tiles = "";
	cliffs = "";
}

CTilesetGroup::~CTilesetGroup()
{

}

void CTilesetGroup::AddTile(CWar3StrID tileID)
{
	int i, nbTiles;
	CString str;
	BOOL bFound = FALSE;

	nbTiles = tiles.GetLength()/4;
	i = 0;
	while ((i < nbTiles) && !bFound)
	{
		str = tiles.Mid(i*4);
		if (tileID == str.GetBufferSetLength(4))
		{
			bFound = TRUE;
		}
		str.ReleaseBuffer();
		i++;
	}
	if (FALSE == bFound)
	{
		tiles = tiles + tileID[0] + tileID[1] + tileID[2] + tileID[3];
	}
}

void CTilesetGroup::AddCliff(CWar3StrID cliffID)
{
	int i, nbCliffs;
	CString str;
	BOOL bFound = FALSE;

	nbCliffs = cliffs.GetLength()/4;
	i = 0;
	while ((i < nbCliffs) && !bFound)
	{
		str = cliffs.Mid(i*4);
		if (cliffID == str.GetBufferSetLength(4))
		{
			bFound = TRUE;
		}
		str.ReleaseBuffer();
		i++;
	}
	if (FALSE == bFound)
	{
		cliffs = cliffs + cliffID[0] + cliffID[1] + cliffID[2] + cliffID[3];
	}
}
