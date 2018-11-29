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
// W3Info.h: interface for the CW3Info class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_W3INFO_H__206EE084_835B_469C_8C4F_C9C9ADF6353E__INCLUDED_)
#define AFX_W3INFO_H__206EE084_835B_469C_8C4F_C9C9ADF6353E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CW3Info  
{
	class C4Points  
	{
	public:
		float fX1;
		float fY1;
		float fX2;
		float fY2;
		float fX3;
		float fY3;
		float fX4;
		float fY4;
		C4Points();
		virtual ~C4Points();

	};

	class CPlayer  
	{
	public:
		int allyHighPrio;
		int allyLowPrio;
		float startY;
		float startX;
		int trigID;
		int fixedstart;
		int race;
		int type;
		int iUnknown00;
		CPlayer();
		virtual ~CPlayer();
	};

	class CForce  
	{
	public:
		int iUnknown01;
		int iUnknown00;
		int trigID;
		int number;
		CForce();
		virtual ~CForce();
	};


public:
	CByteArray m_baUnsupportedBuffer;

	int mapUnknownInts0[10];

	float mapUnknownFloat0;
	float mapUnknownFloat1;
	float mapUnknownFloat2;
	float mapUnknownFloat3;

	int mapiUnknown00;
	int mapiUnknown01;
	CForce mapForces[12];
	int mapForcesFlag;
	int mapNbMaxForces;
	CPlayer mapPlayers[12];
	int mapNbMaxPlayers;

	int mapProlSubtitleTID;
	int mapProlTitleTID;
	int mapProlTextTID;
	int mapProlUnknownTID;

	int mapLoadGlue;
	int mapLoadSubtitleTID;
	int mapLoadTitleTID;
	int mapLoadTextTID;
	int mapLoadUnknownTID;
	int mapLoadBackgrnd;

	int mapCampBackgrnd;
	char mapMainTileset;
	int mapFlags;
	int mapPlayableHeight;
	int mapPlayableWidth;
	int mapTopBoundHeight;
	int mapBottomBoundHeight;
	int mapRightBoundWidth;
	int mapLeftBoundWidth;
	C4Points mapCameraBounds;
	int mapSugPlyrTID;
	int mapDescTID;
	int mapAuthorTID;
	int mapNameTID;
	int mapEditVer;
	int mapNbSaves;
	CW3Info();
	virtual ~CW3Info();

protected:
};

#endif // !defined(AFX_W3INFO_H__206EE084_835B_469C_8C4F_C9C9ADF6353E__INCLUDED_)
