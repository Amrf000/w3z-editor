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
// W3ZMapEditSettings.h: interface for the CW3ZMapEditSettings class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_W3ZMAPEDITSETTINGS_H__C24960BA_9519_4BFC_B8D7_4C04CEBD71EA__INCLUDED_)
#define AFX_W3ZMAPEDITSETTINGS_H__C24960BA_9519_4BFC_B8D7_4C04CEBD71EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Tileset.h"
#include "CliffTileset.h"
#include "TilesetGroup.h"
#include "TilePoint.h"
#include "War3StrID.h"
#include "War3ConversionData.h"


class CW3ZMapEditSettings  
{
	class CListedPlugin
	{
	public:
		CListedPlugin();
		virtual ~CListedPlugin();
		BOOL Matches(CString strFilename);
		void Run(CString strFilename);
		CString strPluginName;
		CString strCommandLine;
		CString strArguments;
		CString strFileMask;
		CListedPlugin *next;
	};

	typedef CListedPlugin (*CPListedPlugin);

	class CListedTileset
	{
	public:
		CListedTileset();
		virtual ~CListedTileset();
		CTileset tileset;
		CListedTileset *next;
	};

	typedef CListedTileset (*CPListedTileset);

	class CListedCliffTileset
	{
	public:
		CListedCliffTileset();
		virtual ~CListedCliffTileset();
		CCliffTileset cliffTileset;
		CListedCliffTileset *next;
	};

	typedef CListedCliffTileset (*CPListedCliffTileset);


	class CListedTilesetGroup
	{
	public:
		CListedTilesetGroup();
		virtual ~CListedTilesetGroup();
		CTilesetGroup tilesetGroup;
		CListedTilesetGroup *next;
	};

	typedef CListedTilesetGroup (*CPListedTilesetGroup);

	class SPathingProp
	{
	public:
		SPathingProp();
		virtual ~SPathingProp();
		BYTE clrRed;
		BYTE clrGreen;
		BYTE clrBlue;
		int mode;	
	};
	
	class STileMaskProp
	{
	public:
		STileMaskProp();
		virtual ~STileMaskProp();
		COLORREF tMaskColor;
		float fAlpha;	
	};

public:
	char m_acWar2MainTileset[PUD_TERRAIN_COUNT];
	CString m_astrWar2Tilesets[PUD_TERRAIN_COUNT];
	CString m_astrWar2Cliffsets[PUD_TERRAIN_COUNT];
	CTilePoint m_atWar2Tilepoints[PUD_TERRAIN_COUNT][SET_WAR2_MAXTILESPERSET];
	CWar3StrID m_astrWar2Doodads[PUD_TERRAIN_COUNT][SET_WAR2_MAXTILESPERSET];
	char m_acSCMainTileset[SCM_TERRAIN_COUNT];
	CString m_astrSCTilesets[SCM_TERRAIN_COUNT];
	CString m_astrSCCliffsets[SCM_TERRAIN_COUNT];
	CTilePoint m_atSCTilepoints[SCM_TERRAIN_COUNT][SET_SC_MAXTILESPERSET];
	CWar3StrID m_astrSCDoodads[SCM_TERRAIN_COUNT][SET_SC_MAXTILESPERSET];
	float pathNoBuildHeightDelta;
	float pathNoWalkHeightDelta;
	float fLevelFactor;
	STileMaskProp tRamp;
	STileMaskProp tBlight;
	STileMaskProp tBoundary;
	SPathingProp pathNormal;
	SPathingProp pathNoWalk;
	SPathingProp pathNoBuild;
	SPathingProp pathNoFly;
	SPathingProp pathNoWater;
	SPathingProp pathUnknown;
	SPathingProp pathUnkn01;
	SPathingProp pathUnkn02;
	SPathingProp pathBlight;
	SPathingProp pathNoBuildNoWalk;
	SPathingProp pathNoBuildNoWalkNoWater;
	SPathingProp pathNoBuildNoWalkNoFly;
	void LoadMisc(CString block);
	void LoadEditModes(CString block);
	void LoadLPlugins(CString block);
	void LoadSPlugins(CString block);
	void LoadMPlugins(CString block);
	void LoadTilesProp(CString block);
	void LoadPathingProp(CString block);
	void LoadLayersProp(CString block);
	void AddCliffTileToTilesetGroup(char groupID, CWar3StrID cliffID);
	void AddTileToTilesetGroup(char groupID, CWar3StrID tileID);
	BOOL IsCliffTilesetInCliffTilesetList(CWar3StrID cliffTileID);
	void ClearPluginList();
	void ClearTilesetGroupList();
	void ClearCliffTilesetList();
	void AddCliffTileDef(CWar3StrID cliffTileID, CString cliffTileName, CWar3StrID associatedTileID);
	CTilesetGroup GetTilesetGroup(char groupID);
	CTilesetGroup GetNextTilesetGroup();
	void GoFirstTilesetGroup();
	BOOL IsTilesetGroupInList(char groupID);
	void AddTilesetGroup(char groupID, CString groupName, COLORREF tWaterColor, float fWaterAlpha);
	void LoadTilesDef(CString block);
	// Layers Colors
	COLORREF LayersColors[16];
	void SetTile(CWar3StrID tileID, CString tileName, COLORREF tileColor);
	void SetTileName(CWar3StrID tileID, CString tileName);
	void SetTileColor(CWar3StrID tileID, COLORREF tileColor);
	void ClearTilesetsList();
	BOOL IsTileBuildable(CWar3StrID tileID);
	BOOL IsTilesetInTilesetList(CWar3StrID tileID);
	void InitParameters();
	COLORREF GetTileColor(CWar3StrID tileID);
	COLORREF GetWaterColor(char groupID);
	float GetWaterAlpha(char groupID);
	void AddTileDef(CWar3StrID tileID, CString tileName, COLORREF tileColor, int tilePathingFlags);
	CW3ZMapEditSettings();
	virtual ~CW3ZMapEditSettings();

protected:
	CPListedPlugin m_pLoadPluginList;
	CPListedPlugin m_pSavePluginList;
	CPListedPlugin m_pMenuPluginList;
	CPListedTileset m_pTilesetsNext;
	CPListedTileset m_pTilesetsHead;
	CPListedCliffTileset m_pCliffTilesetsNext;
	CPListedCliffTileset m_pCliffTilesetsHead;
	CPListedTilesetGroup m_pTilesetGroupsNext;
	CPListedTilesetGroup m_pTilesetGroupsHead;

public:
	void LoadConvertion(CString block);
	int GetTileIndex(CWar3StrID strTileID, CString strTilelist);
	void CheckLoadPlugins(CString strTempFileName, CString strRealFileName);
	void CheckSavePlugins(CString strTempFileName, CString strRealFileName);
	void CheckMenuPlugin(CString strTempFileName, CString strRealFileName, CString strPluginName);
	CString GetMenuPluginList(void);
};

#endif // !defined(AFX_W3ZMAPEDITSETTINGS_H__C24960BA_9519_4BFC_B8D7_4C04CEBD71EA__INCLUDED_)
