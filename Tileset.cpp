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
// Tileset.cpp: implementation of the CTileset class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "War3TypesAndConstants.h"
#include "War3StrID.h"
#include "Tileset.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTileset::CTileset()
{
	//COLORREF: 0x00bbggrr
	tileColor = TS_COLOR_UNDEFINED;
	tileID = TS_STR_ID_UNDEFINED;
	tileName = TS_STR_NAME_UNDEFINED;
	tilePathingFlags = 0;
}

CTileset::~CTileset()
{

}
