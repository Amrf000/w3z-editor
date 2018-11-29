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
// War3Map.h: interface for the CWar3Map class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAR3MAP_H__C97722C8_D75A_4FF8_83ED_5143104C7521__INCLUDED_)
#define AFX_WAR3MAP_H__C97722C8_D75A_4FF8_83ED_5143104C7521__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "W3Info.h"
#include "TilePointsTable.h"
#include "DoodadsList.h"
#include "TrigStringTable.h"
#include "ShadowMap.h"
#include "Targa.h"
#include "PathMap.h"
#include "UnitsList.h"
#include "MiniMap.h"
#include "FloatPoint.h"
#include "W3ZMapEditSettings.h"

class CWar3Map  
{
public:
	void FixMap();
	BOOL IsBuildable(float fx, float fy);
	CFloatPoint TilePointCoord2FloatCoord(int x, int y);
	CFloatPoint TilePointCoord2FloatCoord(CPoint pt);
	CPoint TilePointCoord2SubMapCoord(int x, int y);
	CPoint TilePointCoord2SubMapCoord(CPoint pt);
	CPoint FloatCoord2SubMapCoord(float fx, float fy);
	CPoint FloatCoord2SubMapCoord(CFloatPoint fpt);
	CPoint FloatCoord2TilePointCoord(float fx, float fy);
	CPoint FloatCoord2TilePointCoord(CFloatPoint fpt);
	CFloatPoint SubMapCoord2FloatCoord(int x, int y);
	CFloatPoint SubMapCoord2FloatCoord(CPoint pt);
	CPoint SubMapCoord2TilePointCoord(int x, int y);
	CPoint SubMapCoord2TilePointCoord(CPoint pt);
	CFloatPoint FloatCoord2AbsFloatCoord(float fx, float fy);
	CFloatPoint FloatCoord2AbsFloatCoord(CFloatPoint fpt);
	CFloatPoint AbsFloatCoord2FloatCoord(float fx, float fy);
	CFloatPoint AbsFloatCoord2FloatCoord(CFloatPoint fpt);
	BOOL IsCliff(float fx, float fy);
	void ResetPathing();
	BOOL WPM2PathTGA();
	BOOL PathTGA2WPM();
	void SetShadowMapSize(int iWidth, int iHeight);
	byte* GetShadowMapPtr();
	int GetShadowMapHeight();
	int GetShadowMapWidth();
	int GetPathMapHeight();
	int GetPathMapWidth();
	int GetPathMapLength();
	byte* GetPathMapPtr();
	BOOL SetPathMapSize(int v_iWidth, int v_iHeight);
	BOOL SetW3IUnsupportedBufferSize(int v_iSize);
	int GetW3IUnsupportedBufferSize();
	BYTE* GetW3IUnsupportedBuffer();
	BOOL SetW3EUnsupportedBufferSize(int v_iSize);
	int GetW3EUnsupportedBufferSize();
	BYTE* GetW3EUnsupportedBuffer();
	BOOL SetDOOUnsupportedBufferSize(int v_iSize);
	int GetDOOUnsupportedBufferSize();
	BYTE* GetDOOUnsupportedBuffer();
	int mapW3EVersion;
	CTrigStringTable mapTrigStringsTable; // .WTS
	void UnselectDoodad(CDoodad doodad);
	int GetNbUnits();
	void AddUnit(CUnit unit);
	CUnit GetNextUnit();
	void GoFirstUnit();
	int mapTilePointSelState;
	CPoint mapTilePointSelFirst;
	CPoint mapTilePointSelSecond;
	CPoint mapTilePointSelThird;
	CPoint mapTilePointSelForth;
	void SetSelectedDoodad(CDoodad doodad);
	CDoodad GetSelectedDoodad();
	CDoodad GetNextMapDoodad();
	void GoMapDoodad(float fx, float fy);
	float GetMapCenterY();
	float GetMapCenterX();
	void SetMapCenter(float fCenterX, float fCenterY);
	int mapCustomTilesets;
	void DelDoodad(CDoodad doodad);
	float GetGroundLevel(float x, float y);
	void SelectTilePoints(CPoint pt1, CPoint pt2,RGBQUAD color = RGB_BLACK);
	void SelectTilePoints(CPoint pt1, CPoint pt2, CPoint pt3, CPoint pt4);
	BOOL SelectedTilePointsOnly;
	void UnselectAllTilePoints();
	CTarga mapTGAPath; // Path.TGA
	int mapUnknownDooInt;
	void UnselectAllDoodads();
	float GetDistance(float x1, float y1, float x2, float y2);
	void SetDoodad(CDoodad doodad);
	CDoodad GetDoodad(int ID);
	CDoodad GetClosestDoodad(float x, float y);
	CW3Info mapW3Info; // W3I
	CDoodad GetNextDoodad();
	void GoFirstDoodad();
	int GetNbDoodads();
	void AddDoodad(CDoodad &doodad);
	void ReplaceGroundType(int tileold, int tilenew, bool selectedonly= false);
	void SetTilePoint(int x, int y, CTilePoint tilepoint);
	CTilePoint GetTilePoint(int num);
	void SetTilePoint(int num, CTilePoint tilepoint);
	int GetMapHeight();
	int GetMapWidth();
	CTilePoint GetTilePoint(int x, int y);
	BOOL SetMapSize(int v_iWidth, int v_iHeight);
	CString mapGroundTiles;
	CString mapCliffTiles;
	char mapMainTileset;
	CWar3Map();
	virtual ~CWar3Map();

protected:
	int mapWidth;
	int mapHeight;
	float mapCenterY;
	float mapCenterX;
	CW3ZMapEditSettings * m_pAppSettings;
	CShadowMap mapShadowMap; // SHD
	CMiniMap mapMiniMap; // MMP
	CPathMap mapPathMap; // WPM
	CUnitsList mapUnitsList; // JASS
	CTilePointsTable mapTilePointsTable; //W3E
	CDoodadsList mapDoodadsList; // DOO
	CDoodad mapSelectedDoodad;
};

#endif // !defined(AFX_WAR3MAP_H__C97722C8_D75A_4FF8_83ED_5143104C7521__INCLUDED_)
