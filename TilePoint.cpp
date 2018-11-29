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
// TilePoint.cpp: implementation of the CTilePoint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "War3TypesAndConstants.h"
#include "TilePoint.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTilePoint::CTilePoint()
{
	height = TILE_HEIGHT_LEVEL_ZERO;
	waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	flags = 0;
	groundtype = 0;
	details = 0;
	clifftype = 0;
	layer = TILE_LAYER_LEVEL_ZERO;
	selected = false;
	viewcolor.rgbReserved = 0;
	viewcolor.rgbRed = 0;
	viewcolor.rgbGreen = 0;
	viewcolor.rgbBlue = 0;
}

CTilePoint::~CTilePoint()
{

}

BOOL CTilePoint::IsUnderWater()
{
	BOOL bval = false;
	if ((GetGroundLevel() < GetWaterLevel()) && (flags & TILE_FL_WATER))
	{
		bval = true;
	}
	return bval;
}

int CTilePoint::GetGroundLevel()
{
	int ival;
	ival = ((layer - TILE_LAYER_LEVEL_ZERO)*TILE_LAYER_HEIGHT + (height - TILE_HEIGHT_LEVEL_ZERO));
	return ival;
}

int CTilePoint::GetWaterLevel()
{
	return waterlevel - TILE_WATER_OFFSET;
}

BOOL CTilePoint::IsBlight()
{
	BOOL bval = false;
	if (flags & TILE_FL_BLIGHT)
	{
		bval = true;
	}
	return bval;
}
