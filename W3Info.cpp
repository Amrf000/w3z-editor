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
// W3Info.cpp: implementation of the CW3Info class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "War3TypesAndConstants.h"
#include "W3Info.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//**********************************************************************
CW3Info::C4Points::C4Points()
{
	fX1 = 0;
	fY1 = 0;
	fX2 = 0;
	fY2 = 0;
	fX3 = 0;
	fY3 = 0;
	fX4 = 0;
	fY4 = 0;
}

CW3Info::C4Points::~C4Points()
{

}

//**********************************************************************

CW3Info::CPlayer::CPlayer()
{
	allyHighPrio = 0;
	allyLowPrio = 0;
	startY = 0.;
	startX = 0.;
	trigID = -1;
	fixedstart = 0;
	race = 0;
	type = 0;
	iUnknown00 = 0;
}

CW3Info::CPlayer::~CPlayer()
{

}

//**********************************************************************

CW3Info::CForce::CForce()
{
	number = 0;
	trigID = -1;
}

CW3Info::CForce::~CForce()
{

}

//**********************************************************************

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CW3Info::CW3Info()
{
//	CForce mapForces[12];
	mapiUnknown00 = 0xFFFFFFFF;
	mapForcesFlag = 0;
	mapNbMaxForces = 0;
//	mapPlayers[12];
	mapNbMaxPlayers = 0;
	mapProlSubtitleTID = -1;
	mapProlTitleTID = -1;
	mapProlTextTID = -1;
	mapLoadGlue = -1;
	mapLoadSubtitleTID = -1;
	mapLoadTitleTID = -1;
	mapLoadTextTID = -1;
	mapCampBackgrnd = -1;
	mapMainTileset = 'L';
	mapFlags = 0;
	mapPlayableHeight = 0;
	mapPlayableWidth = 0;
	mapTopBoundHeight = 0;
	mapBottomBoundHeight = 0;
	mapRightBoundWidth = 0;
	mapLeftBoundWidth = 0;
	mapCameraBounds.fX1 = 0;
	mapCameraBounds.fY1 = 0;
	mapCameraBounds.fX2 = 0;
	mapCameraBounds.fY2 = 0;
	mapCameraBounds.fX3 = 0;
	mapCameraBounds.fY3 = 0;
	mapCameraBounds.fX4 = 0;
	mapCameraBounds.fY4 = 0;
	mapSugPlyrTID = -1;
	mapDescTID = -1;
	mapAuthorTID = -1;
	mapNameTID = -1;
	mapEditVer = 0;
	mapNbSaves = 0;
}

CW3Info::~CW3Info()
{
}
