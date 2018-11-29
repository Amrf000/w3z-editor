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
// W3ZMapEditSettings.cpp: implementation of the CW3ZMapEditSettings class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "W3ZMapEdit.h"
#include "W3ZMapEditSettings.h"
#include "War3ConversionData.h"
#include "MainFrm.h"
#include "PluginRunningDialog.h"
#include ".\w3zmapeditsettings.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//********************************************************************************

CW3ZMapEditSettings::CListedPlugin::CListedPlugin()
{
	strPluginName = "";
	strCommandLine = "";
	strArguments = "";
	strFileMask = "";
	next = NULL;
}

CW3ZMapEditSettings::CListedPlugin::~CListedPlugin()
{

}

BOOL CW3ZMapEditSettings::CListedPlugin::Matches(CString strFilename)
{
	int i, j;
	BOOL bRet = TRUE;
	i = j = 0;
	while ((i < strFileMask.GetLength()) && (j < strFilename.GetLength()) && (TRUE == bRet))
	{
		if ('*' == strFileMask[i])
		{
			// gets ride of following wildcards and question marks
			while ((i < strFileMask.GetLength())
				   && (('*' == strFileMask[i]) || ('?' == strFileMask[i])))
			{
				if ('?' == strFileMask[i])
				{
					j++;
				}
				i++;
			}
			// check if the mask is not done
			if (i < strFileMask.GetLength())
			{
				// mask not done, skip any chars from the filename until resync
				while ((j < strFilename.GetLength())
					&& (strFilename[j] != strFileMask[i]))
				{
					j++;
				}
			}
		}
		else if ('?' == strFileMask[i])
		{
			// skip 1 char on each
			i++;
			j++;
		}
		else
		{
			if (strFilename[j] != strFileMask[i])
			{
				bRet = FALSE;
			}
			else
			{
				// matched, go next
				j++;
				i++;
			}
		}
	}
	// check if the mask was completed
	if (i < strFileMask.GetLength())
	{
		bRet = FALSE;
	}
	return bRet;
}

void CW3ZMapEditSettings::CListedPlugin::Run(CString strFilename)
{
	PROCESS_INFORMATION tProcessInformation;
	DWORD dwExitCode;
	STARTUPINFO tStartupInfo;
	CPluginRunningDialog tPluginRunningDialog;
	char szFilePath[2048];
	char* pcFileName;

	GetStartupInfo(&tStartupInfo);

	CString strFullArguments = strArguments + " " + strFilename;
	LPSTR szFullArguments = strFullArguments.GetBuffer();
	DWORD dwStart = GetTickCount();
	if (0 < SearchPath(NULL, strCommandLine, NULL, 2048, szFilePath, &pcFileName))
	{
		strCommandLine = szFilePath;
	}
	if (FALSE == CreateProcess(strCommandLine, szFullArguments, NULL, NULL, TRUE, 0, NULL, NULL, &tStartupInfo, &tProcessInformation))
	{
		TRACE("W3ZME: W3ZMapEditSettings: Process not started! ErrorCode=%i\n", GetLastError());
		AfxMessageBox(STR_WARNING_PLUG00 + strCommandLine, MB_OK | MB_APPLMODAL | MB_ICONWARNING);
	}
	else
	{
		// plugin executed
		if (IDCANCEL == tPluginRunningDialog.DoModal(tProcessInformation.hProcess, strPluginName, strFilename))
		{
			// plugin canceled, try to stop it if still runinng
			GetExitCodeProcess(tProcessInformation.hProcess, &dwExitCode);
			if (STILL_ACTIVE == dwExitCode)
			{
				TerminateProcess(tProcessInformation.hProcess, WAIT_TIMEOUT);
			}
		}
		CloseHandle(tProcessInformation.hProcess);
		CloseHandle(tProcessInformation.hThread);
	}
	strFullArguments.ReleaseBuffer();
}

//********************************************************************************

CW3ZMapEditSettings::SPathingProp::SPathingProp()
{
	clrBlue = clrGreen = clrRed = TS_COLOR_UNDEFINED & 0x000000FF;
	mode = 0;
}

CW3ZMapEditSettings::SPathingProp::~SPathingProp()
{

}

//********************************************************************************

CW3ZMapEditSettings::STileMaskProp::STileMaskProp()
{
	tMaskColor = TS_COLOR_UNDEFINED;
	fAlpha = 0.0f;
}

CW3ZMapEditSettings::STileMaskProp::~STileMaskProp()
{

}

//********************************************************************************

CW3ZMapEditSettings::CListedTileset::CListedTileset()
{
	next = NULL;
}

CW3ZMapEditSettings::CListedTileset::~CListedTileset()
{

}

//********************************************************************************

CW3ZMapEditSettings::CListedCliffTileset::CListedCliffTileset()
{
	next = NULL;
}

CW3ZMapEditSettings::CListedCliffTileset::~CListedCliffTileset()
{

}

//********************************************************************************

CW3ZMapEditSettings::CListedTilesetGroup::CListedTilesetGroup()
{
	next = NULL;
}

CW3ZMapEditSettings::CListedTilesetGroup::~CListedTilesetGroup()
{

}


//********************************************************************************

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CW3ZMapEditSettings::CW3ZMapEditSettings()
{
//Init members
	m_pTilesetsHead			= NULL;
	m_pTilesetsNext			= NULL;
	m_pCliffTilesetsHead	= NULL;
	m_pCliffTilesetsNext	= NULL;
	m_pTilesetGroupsHead	= NULL;
	m_pTilesetGroupsNext	= NULL;
	m_pLoadPluginList		= NULL;
	m_pSavePluginList		= NULL;
	m_pMenuPluginList		= NULL;

// Sets default parameters

	//B G R Res
	// Dark Purpule
	LayersColors[0]		= 0x00800040;

	// Purple
	LayersColors[1]		= 0x00FF0080;

	// Dark Blue
	LayersColors[2]		= 0x00800000;

	// Blue
	LayersColors[3]		= 0x00FF0000;

	// Dark Cyan
	LayersColors[4]		= 0x00808000;

	// Cyan
	LayersColors[5]		= 0x00FFFF00;

	// Dark Green
	LayersColors[6]		= 0x00008000;

	// Green
	LayersColors[7]		= 0x0000FF00;

	// Dark Yellow
	LayersColors[8]		= 0x00008080;

	// Yellow
	LayersColors[9]		= 0x0000FFFF;

	// Dark Orange
	LayersColors[10]	= 0x00004080;

	// Orange
	LayersColors[11]	= 0x000080FF;

	// Dark Red
	LayersColors[12]	= 0x00000080;

	// Red
	LayersColors[13]	= 0x000000FF;

	// Dark Magenta
	LayersColors[14]	= 0x00800080;

	// Magenta
	LayersColors[15]	= 0x00FF00FF;

	// Unknown (used when display mode is set to level
	AddTileDef(TS_STR_ID_UNDEFINED, TS_STR_NAME_UNDEFINED, TS_COLOR_UNDEFINED, 0);

	// Lordaeron Summer Tileset Group
	// these strings must not be used anywhere else
	// the tilesets are defined outside in the ini file! use the ini ones
	// These are just here in case the ini was not found
	AddTilesetGroup(W3E_MAINTILE_DEFAULT, W3E_TILENAME_DEFAULT, TS_WATER_COLOR, TS_WATER_ALPHA);
	AddTileDef("Ldrt", "Lordaeron Summer Dirt", 0x00356490, 0);
	AddTileDef("Ldro", "Lordaeron Summer Rough Dirt", 0x0031608C, 0);
	AddTileDef("Ldrg", "Lordaeron Summer Grassy Dirt", 0x00225F68, 0);
	AddTileDef("Lrok", "Lordaeron Summer Rock", 0x005D9688, 0x08);
	AddTileDef("Lgrs", "Lordaeron Summer Grass", 0x00197727, 0);
	AddTileDef("Lgrd", "Lordaeron Summer Dark Grass", 0x000B560A, 0);

	AddCliffTileDef("CLdi", "Lordaeron Summer Dirt Cliff", "Ldrt");
	AddCliffTileDef("CLgr", "Lordaeron Summer Grass Cliff", "Lgrs");

	//Path settings default color --> overwritten by the ini file
	pathNormal.clrBlue = 0;
	pathNormal.clrGreen = 0;
	pathNormal.clrRed = 0;
	pathNormal.mode = PATH_PLAIN;

	pathNoWalk.clrBlue = 0;
	pathNoWalk.clrGreen = 0;
	pathNoWalk.clrRed = 255;
	pathNoWalk.mode = PATH_STRIPED3;

	pathNoBuild.clrBlue = 255;
	pathNoBuild.clrGreen = 0;
	pathNoBuild.clrRed = 0;
	pathNoBuild.mode = PATH_PLAIN;

	pathNoFly.clrBlue = 0;
	pathNoFly.clrGreen = 255;
	pathNoFly.clrRed = 0;
	pathNoFly.mode = PATH_STRIPED4;

	pathNoWater.clrBlue = 0;
	pathNoWater.clrGreen = 128;
	pathNoWater.clrRed = 192;
	pathNoWater.mode = PATH_NOTHING;

	pathUnknown.clrBlue = 255;
	pathUnknown.clrGreen = 255;
	pathUnknown.clrRed = 255;
	pathUnknown.mode = PATH_DOTS1;

	pathUnkn01.clrBlue = 255;
	pathUnkn01.clrGreen = 255;
	pathUnkn01.clrRed = 255;
	pathUnkn01.mode = PATH_DOTS1;

	pathUnkn02.clrBlue = 255;
	pathUnkn02.clrGreen = 255;
	pathUnkn02.clrRed = 255;
	pathUnkn02.mode = PATH_DOTS1;

	pathBlight.clrBlue = 96;
	pathBlight.clrGreen = 128;
	pathBlight.clrRed = 128;
	pathBlight.mode = PATH_DOTS2;

	pathNoBuildNoWalk.clrBlue = 255;
	pathNoBuildNoWalk.clrGreen = 0;
	pathNoBuildNoWalk.clrRed = 128;
	pathNoBuildNoWalk.mode = PATH_PLAIN;

	pathNoBuildNoWalkNoWater.clrBlue = 255;
	pathNoBuildNoWalkNoWater.clrGreen = 0;
	pathNoBuildNoWalkNoWater.clrRed = 255;
	pathNoBuildNoWalkNoWater.mode = PATH_PLAIN;

	pathNoBuildNoWalkNoFly.clrBlue = 255;
	pathNoBuildNoWalkNoFly.clrGreen = 255;
	pathNoBuildNoWalkNoFly.clrRed = 255;
	pathNoBuildNoWalkNoFly.mode = PATH_PLAIN;

	pathNoBuildHeightDelta = PATH_MAX_BUILDHEIGHTDELTA;
	pathNoWalkHeightDelta = PATH_MAX_WALKHEIGHTDELTA;

	tRamp.tMaskColor = TS_RAMP_COLOR;
	tRamp.fAlpha = TS_RAMP_ALPHA;

	tBlight.tMaskColor = TS_BLIGHT_COLOR;
	tBlight.fAlpha = TS_BLIGHT_ALPHA;

	tBoundary.tMaskColor = TS_BOUNDARY_COLOR;
	tBoundary.fAlpha = TS_BOUNDARY_ALPHA;

	fLevelFactor = TS_LEVEL_FACTOR;

	// inits War2 Forest default
	m_acWar2MainTileset[INDEX_SET_W2FOREST]		= 'L';

	m_astrWar2Tilesets[INDEX_SET_W2FOREST]		= "LdrtLdroLgrsLgrdLrokFdrg";
	m_astrWar2Cliffsets[INDEX_SET_W2FOREST]		= "CLdiCLgr";

	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_LIGHTWATER_INDEX].groundtype = GetTileIndex("Ldrt", m_astrWar2Tilesets[INDEX_SET_W2FOREST]);
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_LIGHTWATER_INDEX].clifftype = GetTileIndex("Cldi", m_astrWar2Cliffsets[INDEX_SET_W2FOREST]);
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_LIGHTWATER_INDEX].layer = TILE_LAYER_LEVEL_ZERO - 1;
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_LIGHTWATER_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_LIGHTWATER_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_LIGHTWATER_INDEX].flags = TILE_FL_WATER;

	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_DARKWATER_INDEX].groundtype = GetTileIndex("Ldrt", m_astrWar2Tilesets[INDEX_SET_W2FOREST]);
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_DARKWATER_INDEX].clifftype = GetTileIndex("Cldi", m_astrWar2Cliffsets[INDEX_SET_W2FOREST]);
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_DARKWATER_INDEX].layer = TILE_LAYER_LEVEL_ZERO - 2;
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_DARKWATER_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_DARKWATER_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_DARKWATER_INDEX].flags = TILE_FL_WATER;

	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_LIGHTDIRT_INDEX].groundtype = GetTileIndex("Ldrt", m_astrWar2Tilesets[INDEX_SET_W2FOREST]);
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_LIGHTDIRT_INDEX].clifftype = GetTileIndex("Cldi", m_astrWar2Cliffsets[INDEX_SET_W2FOREST]);
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_LIGHTDIRT_INDEX].layer = TILE_LAYER_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_LIGHTDIRT_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_LIGHTDIRT_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_LIGHTDIRT_INDEX].flags = TILE_FL_WATER;

	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_DARKDIRT_INDEX].groundtype = GetTileIndex("Ldro", m_astrWar2Tilesets[INDEX_SET_W2FOREST]);
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_DARKDIRT_INDEX].clifftype = GetTileIndex("Cldi", m_astrWar2Cliffsets[INDEX_SET_W2FOREST]);
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_DARKDIRT_INDEX].layer = TILE_LAYER_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_DARKDIRT_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_DARKDIRT_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_DARKDIRT_INDEX].flags = 0;

	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_LIGHTGROUND_INDEX].groundtype = GetTileIndex("Lgrs", m_astrWar2Tilesets[INDEX_SET_W2FOREST]);
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_LIGHTGROUND_INDEX].clifftype = GetTileIndex("Cldi", m_astrWar2Cliffsets[INDEX_SET_W2FOREST]);
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_LIGHTGROUND_INDEX].layer = TILE_LAYER_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_LIGHTGROUND_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_LIGHTGROUND_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_LIGHTGROUND_INDEX].flags = 0;

	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_DARKGROUND_INDEX].groundtype = GetTileIndex("Lgrd", m_astrWar2Tilesets[INDEX_SET_W2FOREST]);
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_DARKGROUND_INDEX].clifftype = GetTileIndex("Cldi", m_astrWar2Cliffsets[INDEX_SET_W2FOREST]);
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_DARKGROUND_INDEX].layer = TILE_LAYER_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_DARKGROUND_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_DARKGROUND_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_DARKGROUND_INDEX].flags = 0;

	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_FOREST_INDEX].groundtype = GetTileIndex("Lgrd", m_astrWar2Tilesets[INDEX_SET_W2FOREST]);
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_FOREST_INDEX].clifftype = GetTileIndex("Cldi", m_astrWar2Cliffsets[INDEX_SET_W2FOREST]);
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_FOREST_INDEX].layer = TILE_LAYER_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_FOREST_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_FOREST_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_FOREST_INDEX].flags = 0;
	m_astrWar2Doodads[INDEX_SET_W2FOREST][PUD_FOREST_INDEX] = "LTlt";

	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_MOUNTAINS_INDEX].groundtype = GetTileIndex("Lrok", m_astrWar2Tilesets[INDEX_SET_W2FOREST]);
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_MOUNTAINS_INDEX].clifftype = GetTileIndex("Cldi", m_astrWar2Cliffsets[INDEX_SET_W2FOREST]);
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_MOUNTAINS_INDEX].layer = TILE_LAYER_LEVEL_ZERO + 1;
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_MOUNTAINS_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_MOUNTAINS_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_MOUNTAINS_INDEX].flags = 0;

	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_HUMANWALL_INDEX].groundtype = GetTileIndex("Fdrg", m_astrWar2Tilesets[INDEX_SET_W2FOREST]);
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_HUMANWALL_INDEX].clifftype = GetTileIndex("Cldi", m_astrWar2Cliffsets[INDEX_SET_W2FOREST]);
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_HUMANWALL_INDEX].layer = TILE_LAYER_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_HUMANWALL_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_HUMANWALL_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_HUMANWALL_INDEX].flags = 0;

	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_ORCWALL_INDEX].groundtype = GetTileIndex("Fdrg", m_astrWar2Tilesets[INDEX_SET_W2FOREST]);
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_ORCWALL_INDEX].clifftype = GetTileIndex("Cldi", m_astrWar2Cliffsets[INDEX_SET_W2FOREST]);
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_ORCWALL_INDEX].layer = TILE_LAYER_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_ORCWALL_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2FOREST][PUD_ORCWALL_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_ORCWALL_INDEX].flags = 0;

	// inits War2 Winter default
	m_acWar2MainTileset[INDEX_SET_W2WINTER]		= 'N';

	m_astrWar2Tilesets[INDEX_SET_W2WINTER]		= "NdrtNiceNrckNsnwNsnrNgrsWrokNdrd";
	m_astrWar2Cliffsets[INDEX_SET_W2WINTER]		= "CNsnCNdi";

	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_LIGHTWATER_INDEX].groundtype = GetTileIndex("Ndrt", m_astrWar2Tilesets[INDEX_SET_W2WINTER]);
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_LIGHTWATER_INDEX].clifftype = GetTileIndex("CNsn", m_astrWar2Cliffsets[INDEX_SET_W2WINTER]);
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_LIGHTWATER_INDEX].layer = TILE_LAYER_LEVEL_ZERO - 1;
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_LIGHTWATER_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_LIGHTWATER_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_LIGHTWATER_INDEX].flags = TILE_FL_WATER;

	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_DARKWATER_INDEX].groundtype = GetTileIndex("Ndrt", m_astrWar2Tilesets[INDEX_SET_W2WINTER]);
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_DARKWATER_INDEX].clifftype = GetTileIndex("CNsn", m_astrWar2Cliffsets[INDEX_SET_W2WINTER]);
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_DARKWATER_INDEX].layer = TILE_LAYER_LEVEL_ZERO - 2;
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_DARKWATER_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_DARKWATER_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_DARKWATER_INDEX].flags = TILE_FL_WATER;

	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_LIGHTDIRT_INDEX].groundtype = GetTileIndex("Nice", m_astrWar2Tilesets[INDEX_SET_W2WINTER]);
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_LIGHTDIRT_INDEX].clifftype = GetTileIndex("CNsn", m_astrWar2Cliffsets[INDEX_SET_W2WINTER]);
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_LIGHTDIRT_INDEX].layer = TILE_LAYER_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_LIGHTDIRT_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_LIGHTDIRT_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_LIGHTDIRT_INDEX].flags = TILE_FL_WATER;

	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_DARKDIRT_INDEX].groundtype = GetTileIndex("Nrck", m_astrWar2Tilesets[INDEX_SET_W2WINTER]);
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_DARKDIRT_INDEX].clifftype = GetTileIndex("CNsn", m_astrWar2Cliffsets[INDEX_SET_W2WINTER]);
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_DARKDIRT_INDEX].layer = TILE_LAYER_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_DARKDIRT_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_DARKDIRT_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_DARKDIRT_INDEX].flags = 0;

	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_LIGHTGROUND_INDEX].groundtype = GetTileIndex("Nsnw", m_astrWar2Tilesets[INDEX_SET_W2WINTER]);
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_LIGHTGROUND_INDEX].clifftype = GetTileIndex("CNsn", m_astrWar2Cliffsets[INDEX_SET_W2WINTER]);
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_LIGHTGROUND_INDEX].layer = TILE_LAYER_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_LIGHTGROUND_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_LIGHTGROUND_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_LIGHTGROUND_INDEX].flags = 0;

	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_DARKGROUND_INDEX].groundtype = GetTileIndex("Nsnr", m_astrWar2Tilesets[INDEX_SET_W2WINTER]);
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_DARKGROUND_INDEX].clifftype = GetTileIndex("CNsn", m_astrWar2Cliffsets[INDEX_SET_W2WINTER]);
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_DARKGROUND_INDEX].layer = TILE_LAYER_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_DARKGROUND_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_DARKGROUND_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_DARKGROUND_INDEX].flags = 0;

	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_FOREST_INDEX].groundtype = GetTileIndex("Ngrs", m_astrWar2Tilesets[INDEX_SET_W2WINTER]);
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_FOREST_INDEX].clifftype = GetTileIndex("Cndi", m_astrWar2Cliffsets[INDEX_SET_W2WINTER]);
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_FOREST_INDEX].layer = TILE_LAYER_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_FOREST_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_FOREST_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_FOREST_INDEX].flags = 0;
	m_astrWar2Doodads[INDEX_SET_W2WINTER][PUD_FOREST_INDEX] = "WTst";

	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_MOUNTAINS_INDEX].groundtype = GetTileIndex("Wrok", m_astrWar2Tilesets[INDEX_SET_W2WINTER]);
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_MOUNTAINS_INDEX].clifftype = GetTileIndex("Cndi", m_astrWar2Cliffsets[INDEX_SET_W2WINTER]);
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_MOUNTAINS_INDEX].layer = TILE_LAYER_LEVEL_ZERO + 1;
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_MOUNTAINS_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_MOUNTAINS_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_MOUNTAINS_INDEX].flags = 0;

	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_HUMANWALL_INDEX].groundtype = GetTileIndex("Ndrd", m_astrWar2Tilesets[INDEX_SET_W2WINTER]);
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_HUMANWALL_INDEX].clifftype = GetTileIndex("Cndi", m_astrWar2Cliffsets[INDEX_SET_W2WINTER]);
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_HUMANWALL_INDEX].layer = TILE_LAYER_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_HUMANWALL_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_HUMANWALL_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_HUMANWALL_INDEX].flags = 0;

	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_ORCWALL_INDEX].groundtype = GetTileIndex("Ndrd", m_astrWar2Tilesets[INDEX_SET_W2WINTER]);
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_ORCWALL_INDEX].clifftype = GetTileIndex("Cndi", m_astrWar2Cliffsets[INDEX_SET_W2WINTER]);
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_ORCWALL_INDEX].layer = TILE_LAYER_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_ORCWALL_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_ORCWALL_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WINTER][PUD_ORCWALL_INDEX].flags = 0;

	// inits War2 Wasteland default
	m_acWar2MainTileset[INDEX_SET_W2WASTELAND]	= 'B';

	m_astrWar2Tilesets[INDEX_SET_W2WASTELAND]		= "BdrhBgrrBdrtBdsrBdsdBdrgBflrBdrr";
	m_astrWar2Cliffsets[INDEX_SET_W2WASTELAND]		= "CBgrCBde";

	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_LIGHTWATER_INDEX].groundtype = GetTileIndex("Bdrh", m_astrWar2Tilesets[INDEX_SET_W2WASTELAND]);
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_LIGHTWATER_INDEX].clifftype = GetTileIndex("CBgr", m_astrWar2Cliffsets[INDEX_SET_W2WASTELAND]);
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_LIGHTWATER_INDEX].layer = TILE_LAYER_LEVEL_ZERO - 1;
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_LIGHTWATER_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_LIGHTWATER_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_LIGHTWATER_INDEX].flags = TILE_FL_WATER;

	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_DARKWATER_INDEX].groundtype = GetTileIndex("Bdrh", m_astrWar2Tilesets[INDEX_SET_W2WASTELAND]);
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_DARKWATER_INDEX].clifftype = GetTileIndex("CBgr", m_astrWar2Cliffsets[INDEX_SET_W2WASTELAND]);
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_DARKWATER_INDEX].layer = TILE_LAYER_LEVEL_ZERO - 2;
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_DARKWATER_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_DARKWATER_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_DARKWATER_INDEX].flags = TILE_FL_WATER;

	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_LIGHTDIRT_INDEX].groundtype = GetTileIndex("Bgrr", m_astrWar2Tilesets[INDEX_SET_W2WASTELAND]);
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_LIGHTDIRT_INDEX].clifftype = GetTileIndex("CBgr", m_astrWar2Cliffsets[INDEX_SET_W2WASTELAND]);
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_LIGHTDIRT_INDEX].layer = TILE_LAYER_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_LIGHTDIRT_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_LIGHTDIRT_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_LIGHTDIRT_INDEX].flags = TILE_FL_WATER;

	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_DARKDIRT_INDEX].groundtype = GetTileIndex("Bdrt", m_astrWar2Tilesets[INDEX_SET_W2WASTELAND]);
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_DARKDIRT_INDEX].clifftype = GetTileIndex("CBgr", m_astrWar2Cliffsets[INDEX_SET_W2WASTELAND]);
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_DARKDIRT_INDEX].layer = TILE_LAYER_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_DARKDIRT_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_DARKDIRT_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_DARKDIRT_INDEX].flags = 0;

	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_LIGHTGROUND_INDEX].groundtype = GetTileIndex("Bdsr", m_astrWar2Tilesets[INDEX_SET_W2WASTELAND]);
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_LIGHTGROUND_INDEX].clifftype = GetTileIndex("CBgr", m_astrWar2Cliffsets[INDEX_SET_W2WASTELAND]);
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_LIGHTGROUND_INDEX].layer = TILE_LAYER_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_LIGHTGROUND_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_LIGHTGROUND_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_LIGHTGROUND_INDEX].flags = 0;

	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_DARKGROUND_INDEX].groundtype = GetTileIndex("Bdsd", m_astrWar2Tilesets[INDEX_SET_W2WASTELAND]);
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_DARKGROUND_INDEX].clifftype = GetTileIndex("CBgr", m_astrWar2Cliffsets[INDEX_SET_W2WASTELAND]);
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_DARKGROUND_INDEX].layer = TILE_LAYER_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_DARKGROUND_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_DARKGROUND_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_DARKGROUND_INDEX].flags = 0;

	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_FOREST_INDEX].groundtype = GetTileIndex("Bdrg", m_astrWar2Tilesets[INDEX_SET_W2WASTELAND]);
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_FOREST_INDEX].clifftype = GetTileIndex("CBgr", m_astrWar2Cliffsets[INDEX_SET_W2WASTELAND]);
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_FOREST_INDEX].layer = TILE_LAYER_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_FOREST_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_FOREST_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_FOREST_INDEX].flags = 0;
	m_astrWar2Doodads[INDEX_SET_W2WASTELAND][PUD_FOREST_INDEX] = "FTtw";

	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_MOUNTAINS_INDEX].groundtype = GetTileIndex("Bflr", m_astrWar2Tilesets[INDEX_SET_W2WASTELAND]);
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_MOUNTAINS_INDEX].clifftype = GetTileIndex("CBgr", m_astrWar2Cliffsets[INDEX_SET_W2WASTELAND]);
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_MOUNTAINS_INDEX].layer = TILE_LAYER_LEVEL_ZERO + 1;
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_MOUNTAINS_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_MOUNTAINS_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_MOUNTAINS_INDEX].flags = 0;

	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_HUMANWALL_INDEX].groundtype = GetTileIndex("Bdrr", m_astrWar2Tilesets[INDEX_SET_W2WASTELAND]);
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_HUMANWALL_INDEX].clifftype = GetTileIndex("CBgr", m_astrWar2Cliffsets[INDEX_SET_W2WASTELAND]);
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_HUMANWALL_INDEX].layer = TILE_LAYER_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_HUMANWALL_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_HUMANWALL_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_HUMANWALL_INDEX].flags = 0;

	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_ORCWALL_INDEX].groundtype = GetTileIndex("Bdrr", m_astrWar2Tilesets[INDEX_SET_W2WASTELAND]);
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_ORCWALL_INDEX].clifftype = GetTileIndex("CBgr", m_astrWar2Cliffsets[INDEX_SET_W2WASTELAND]);
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_ORCWALL_INDEX].layer = TILE_LAYER_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_ORCWALL_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_ORCWALL_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2WASTELAND][PUD_ORCWALL_INDEX].flags = 0;


	// inits War2 Swamp default
	m_acWar2MainTileset[INDEX_SET_W2SWAMP]		= 'C';

	m_astrWar2Tilesets[INDEX_SET_W2SWAMP]		= "CdrtCgrsCvinCdrdClvgCrckCpos";
	m_astrWar2Cliffsets[INDEX_SET_W2SWAMP]		= "CCgrCCdi";

	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_LIGHTWATER_INDEX].groundtype = GetTileIndex("Cdrt", m_astrWar2Tilesets[INDEX_SET_W2SWAMP]);
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_LIGHTWATER_INDEX].clifftype = GetTileIndex("CCgr", m_astrWar2Cliffsets[INDEX_SET_W2SWAMP]);
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_LIGHTWATER_INDEX].layer = TILE_LAYER_LEVEL_ZERO - 1;
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_LIGHTWATER_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_LIGHTWATER_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_LIGHTWATER_INDEX].flags = TILE_FL_WATER;

	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_DARKWATER_INDEX].groundtype = GetTileIndex("Cdrt", m_astrWar2Tilesets[INDEX_SET_W2SWAMP]);
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_DARKWATER_INDEX].clifftype = GetTileIndex("CCgr", m_astrWar2Cliffsets[INDEX_SET_W2SWAMP]);
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_DARKWATER_INDEX].layer = TILE_LAYER_LEVEL_ZERO - 2;
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_DARKWATER_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_DARKWATER_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_DARKWATER_INDEX].flags = TILE_FL_WATER;

	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_LIGHTDIRT_INDEX].groundtype = GetTileIndex("Cgrs", m_astrWar2Tilesets[INDEX_SET_W2SWAMP]);
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_LIGHTDIRT_INDEX].clifftype = GetTileIndex("CCgr", m_astrWar2Cliffsets[INDEX_SET_W2SWAMP]);
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_LIGHTDIRT_INDEX].layer = TILE_LAYER_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_LIGHTDIRT_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_LIGHTDIRT_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_LIGHTDIRT_INDEX].flags = TILE_FL_WATER;

	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_DARKDIRT_INDEX].groundtype = GetTileIndex("Cvin", m_astrWar2Tilesets[INDEX_SET_W2SWAMP]);
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_DARKDIRT_INDEX].clifftype = GetTileIndex("CCgr", m_astrWar2Cliffsets[INDEX_SET_W2SWAMP]);
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_DARKDIRT_INDEX].layer = TILE_LAYER_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_DARKDIRT_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_DARKDIRT_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_DARKDIRT_INDEX].flags = 0;

	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_LIGHTGROUND_INDEX].groundtype = GetTileIndex("Cdrt", m_astrWar2Tilesets[INDEX_SET_W2SWAMP]);
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_LIGHTGROUND_INDEX].clifftype = GetTileIndex("Ccdi", m_astrWar2Cliffsets[INDEX_SET_W2SWAMP]);
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_LIGHTGROUND_INDEX].layer = TILE_LAYER_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_LIGHTGROUND_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_LIGHTGROUND_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_LIGHTGROUND_INDEX].flags = 0;

	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_DARKGROUND_INDEX].groundtype = GetTileIndex("Cdrd", m_astrWar2Tilesets[INDEX_SET_W2SWAMP]);
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_DARKGROUND_INDEX].clifftype = GetTileIndex("Ccdi", m_astrWar2Cliffsets[INDEX_SET_W2SWAMP]);
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_DARKGROUND_INDEX].layer = TILE_LAYER_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_DARKGROUND_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_DARKGROUND_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_DARKGROUND_INDEX].flags = 0;

	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_FOREST_INDEX].groundtype = GetTileIndex("Clvg", m_astrWar2Tilesets[INDEX_SET_W2SWAMP]);
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_FOREST_INDEX].clifftype = GetTileIndex("Ccdi", m_astrWar2Cliffsets[INDEX_SET_W2SWAMP]);
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_FOREST_INDEX].layer = TILE_LAYER_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_FOREST_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_FOREST_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_FOREST_INDEX].flags = 0;
	m_astrWar2Doodads[INDEX_SET_W2SWAMP][PUD_FOREST_INDEX] = "DTsh";

	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_MOUNTAINS_INDEX].groundtype = GetTileIndex("Crck", m_astrWar2Tilesets[INDEX_SET_W2SWAMP]);
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_MOUNTAINS_INDEX].clifftype = GetTileIndex("Ccdi", m_astrWar2Cliffsets[INDEX_SET_W2SWAMP]);
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_MOUNTAINS_INDEX].layer = TILE_LAYER_LEVEL_ZERO + 1;
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_MOUNTAINS_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_MOUNTAINS_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_MOUNTAINS_INDEX].flags = 0;

	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_HUMANWALL_INDEX].groundtype = GetTileIndex("Cpos", m_astrWar2Tilesets[INDEX_SET_W2SWAMP]);
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_HUMANWALL_INDEX].clifftype = GetTileIndex("Ccdi", m_astrWar2Cliffsets[INDEX_SET_W2SWAMP]);
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_HUMANWALL_INDEX].layer = TILE_LAYER_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_HUMANWALL_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_HUMANWALL_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_HUMANWALL_INDEX].flags = 0;

	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_ORCWALL_INDEX].groundtype = GetTileIndex("Cpos", m_astrWar2Tilesets[INDEX_SET_W2SWAMP]);
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_ORCWALL_INDEX].clifftype = GetTileIndex("Ccdi", m_astrWar2Cliffsets[INDEX_SET_W2SWAMP]);
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_ORCWALL_INDEX].layer = TILE_LAYER_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_ORCWALL_INDEX].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_ORCWALL_INDEX].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atWar2Tilepoints[INDEX_SET_W2SWAMP][PUD_ORCWALL_INDEX].flags = 0;

	// Badlands
	m_acSCMainTileset[SCM_TERRAIN_BADLANDS] = 'X';
	m_astrSCTilesets[SCM_TERRAIN_BADLANDS] = "XdrtXdtrFgrsXbtlXblmQrck";
	m_astrSCCliffsets[SCM_TERRAIN_BADLANDS] = "CXsqCXdi";
	
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLDIRT].groundtype = GetTileIndex("Xdrt", m_astrSCTilesets[SCM_TERRAIN_BADLANDS]);
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLDIRT].clifftype = GetTileIndex("CXdi", m_astrSCCliffsets[SCM_TERRAIN_BADLANDS]);
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLDIRT].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLDIRT].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLDIRT].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLDIRT].flags = TILE_FL_WATER;

	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLHIGHDIRT].groundtype = GetTileIndex("Xdrt", m_astrSCTilesets[SCM_TERRAIN_BADLANDS]);
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLHIGHDIRT].clifftype = GetTileIndex("CXdi", m_astrSCCliffsets[SCM_TERRAIN_BADLANDS]);
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLHIGHDIRT].layer = TILE_LAYER_LEVEL_ZERO + 1;
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLHIGHDIRT].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLHIGHDIRT].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLHIGHDIRT].flags = TILE_FL_WATER;

	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLWATER].groundtype = GetTileIndex("Xdrt", m_astrSCTilesets[SCM_TERRAIN_BADLANDS]);
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLWATER].clifftype = GetTileIndex("CXdi", m_astrSCCliffsets[SCM_TERRAIN_BADLANDS]);
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLWATER].layer = TILE_LAYER_LEVEL_ZERO - 1;
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLWATER].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLWATER].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLWATER].flags = TILE_FL_WATER;

	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLGRASS].groundtype = GetTileIndex("Fgrs", m_astrSCTilesets[SCM_TERRAIN_BADLANDS]);
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLGRASS].clifftype = GetTileIndex("CXdi", m_astrSCCliffsets[SCM_TERRAIN_BADLANDS]);
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLGRASS].layer = TILE_LAYER_LEVEL_ZERO + 1;
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLGRASS].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLGRASS].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLGRASS].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLASPHALT].groundtype = GetTileIndex("Xblm", m_astrSCTilesets[SCM_TERRAIN_BADLANDS]);
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLASPHALT].clifftype = GetTileIndex("CXsq", m_astrSCCliffsets[SCM_TERRAIN_BADLANDS]);
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLASPHALT].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLASPHALT].height = TILE_HEIGHT_LEVEL_ZERO + 64;
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLASPHALT].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLASPHALT].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLROCKYGROUND].groundtype = GetTileIndex("Qrck", m_astrSCTilesets[SCM_TERRAIN_BADLANDS]);
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLROCKYGROUND].clifftype = GetTileIndex("CXdi", m_astrSCCliffsets[SCM_TERRAIN_BADLANDS]);
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLROCKYGROUND].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLROCKYGROUND].height = TILE_HEIGHT_LEVEL_ZERO + 32;
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLROCKYGROUND].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLROCKYGROUND].flags = TILE_FL_WATER;

	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLHIGHGRASS].groundtype = GetTileIndex("Fgrs", m_astrSCTilesets[SCM_TERRAIN_BADLANDS]);
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLHIGHGRASS].clifftype = GetTileIndex("CXdi", m_astrSCCliffsets[SCM_TERRAIN_BADLANDS]);
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLHIGHGRASS].layer = TILE_LAYER_LEVEL_ZERO + 1;
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLHIGHGRASS].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLHIGHGRASS].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLHIGHGRASS].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLSTRUCTURE].groundtype = GetTileIndex("Xbtl", m_astrSCTilesets[SCM_TERRAIN_BADLANDS]);
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLSTRUCTURE].clifftype = GetTileIndex("CXsq", m_astrSCCliffsets[SCM_TERRAIN_BADLANDS]);
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLSTRUCTURE].layer = TILE_LAYER_LEVEL_ZERO + 1;
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLSTRUCTURE].height = TILE_HEIGHT_LEVEL_ZERO + 64;
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLSTRUCTURE].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLSTRUCTURE].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLMUD].groundtype = GetTileIndex("Xdtr", m_astrSCTilesets[SCM_TERRAIN_BADLANDS]);
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLMUD].clifftype = GetTileIndex("CXdi", m_astrSCCliffsets[SCM_TERRAIN_BADLANDS]);
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLMUD].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLMUD].height = TILE_HEIGHT_LEVEL_ZERO - 128;
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLMUD].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_BADLANDS][INDEX_SET_SCBLMUD].flags = 0;

	// Space Platform
	m_acSCMainTileset[SCM_TERRAIN_SPACE] = 'J';
	m_astrSCTilesets[SCM_TERRAIN_SPACE] = "JsqdJblmJdtrJbtlJwmbJrtlJhdg";
	m_astrSCCliffsets[SCM_TERRAIN_SPACE] = "CJsq";

	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPSPACE].groundtype = GetTileIndex("Jsqd", m_astrSCTilesets[SCM_TERRAIN_SPACE]);
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPSPACE].clifftype = GetTileIndex("CJsq", m_astrSCCliffsets[SCM_TERRAIN_SPACE]);
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPSPACE].layer = TILE_LAYER_LEVEL_ZERO - 2;
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPSPACE].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPSPACE].waterlevel = TILE_HEIGHT_LEVEL_ZERO - 512;
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPSPACE].flags = TILE_FL_WATER;

	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPPLATFORM].groundtype = GetTileIndex("Jbtl", m_astrSCTilesets[SCM_TERRAIN_SPACE]);
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPPLATFORM].clifftype = GetTileIndex("CJsq", m_astrSCCliffsets[SCM_TERRAIN_SPACE]);
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPPLATFORM].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPPLATFORM].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPPLATFORM].waterlevel = TILE_HEIGHT_LEVEL_ZERO - 512;
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPPLATFORM].flags = TILE_FL_WATER;

	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPHIGHPLATFORM].groundtype = GetTileIndex("Jbtl", m_astrSCTilesets[SCM_TERRAIN_SPACE]);
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPHIGHPLATFORM].clifftype = GetTileIndex("CJsq", m_astrSCCliffsets[SCM_TERRAIN_SPACE]);
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPHIGHPLATFORM].layer = TILE_LAYER_LEVEL_ZERO + 1;
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPHIGHPLATFORM].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPHIGHPLATFORM].waterlevel = TILE_HEIGHT_LEVEL_ZERO - 512;
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPHIGHPLATFORM].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPSOLARARRAY].groundtype = GetTileIndex("Jrtl", m_astrSCTilesets[SCM_TERRAIN_SPACE]);
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPSOLARARRAY].clifftype = GetTileIndex("CJsq", m_astrSCCliffsets[SCM_TERRAIN_SPACE]);
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPSOLARARRAY].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPSOLARARRAY].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPSOLARARRAY].waterlevel = TILE_HEIGHT_LEVEL_ZERO - 512;
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPSOLARARRAY].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPLOWPLATFORM].groundtype = GetTileIndex("Jblm", m_astrSCTilesets[SCM_TERRAIN_SPACE]);
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPLOWPLATFORM].clifftype = GetTileIndex("CJsq", m_astrSCCliffsets[SCM_TERRAIN_SPACE]);
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPLOWPLATFORM].layer = TILE_LAYER_LEVEL_ZERO - 1;
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPLOWPLATFORM].height = TILE_HEIGHT_LEVEL_ZERO - 512;
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPLOWPLATFORM].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPLOWPLATFORM].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPRUSTYPIT].groundtype = GetTileIndex("Jdtr", m_astrSCTilesets[SCM_TERRAIN_SPACE]);
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPRUSTYPIT].clifftype = GetTileIndex("CJsq", m_astrSCCliffsets[SCM_TERRAIN_SPACE]);
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPRUSTYPIT].layer = TILE_LAYER_LEVEL_ZERO - 1;
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPRUSTYPIT].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPRUSTYPIT].waterlevel = TILE_HEIGHT_LEVEL_ZERO - 512;
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPRUSTYPIT].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPPLATING].groundtype = GetTileIndex("Jwmb", m_astrSCTilesets[SCM_TERRAIN_SPACE]);
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPPLATING].clifftype = GetTileIndex("CJsq", m_astrSCCliffsets[SCM_TERRAIN_SPACE]);
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPPLATING].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPPLATING].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPPLATING].waterlevel = TILE_HEIGHT_LEVEL_ZERO - 512;
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPPLATING].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPHIGHPLATING].groundtype = GetTileIndex("Jwmb", m_astrSCTilesets[SCM_TERRAIN_SPACE]);
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPHIGHPLATING].clifftype = GetTileIndex("CJsq", m_astrSCCliffsets[SCM_TERRAIN_SPACE]);
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPHIGHPLATING].layer = TILE_LAYER_LEVEL_ZERO + 1;
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPHIGHPLATING].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPHIGHPLATING].waterlevel = TILE_HEIGHT_LEVEL_ZERO - 512;
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPHIGHPLATING].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPELEVATEDCATWALK].groundtype = GetTileIndex("Jhdg", m_astrSCTilesets[SCM_TERRAIN_SPACE]);
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPELEVATEDCATWALK].clifftype = GetTileIndex("CJsq", m_astrSCCliffsets[SCM_TERRAIN_SPACE]);
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPELEVATEDCATWALK].layer = TILE_LAYER_LEVEL_ZERO + 1;
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPELEVATEDCATWALK].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPELEVATEDCATWALK].waterlevel = TILE_HEIGHT_LEVEL_ZERO - 512;
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPELEVATEDCATWALK].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPDARKPLATFORM].groundtype = GetTileIndex("Jsqd", m_astrSCTilesets[SCM_TERRAIN_SPACE]);
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPDARKPLATFORM].clifftype = GetTileIndex("CJsq", m_astrSCCliffsets[SCM_TERRAIN_SPACE]);
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPDARKPLATFORM].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPDARKPLATFORM].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPDARKPLATFORM].waterlevel = TILE_HEIGHT_LEVEL_ZERO - 512;
	m_atSCTilepoints[SCM_TERRAIN_SPACE][INDEX_SET_SCSPDARKPLATFORM].flags = 0;

	// Installation
	m_acSCMainTileset[SCM_TERRAIN_INSTALLATION] = 'X';
	m_astrSCTilesets[SCM_TERRAIN_INSTALLATION] = "XrtlXsqdXblmXbtlXwmb";
	m_astrSCCliffsets[SCM_TERRAIN_INSTALLATION] = "CXsq";

	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCISUBSTRUCTURE].groundtype = GetTileIndex("Xrtl", m_astrSCTilesets[SCM_TERRAIN_INSTALLATION]);
	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCISUBSTRUCTURE].clifftype = GetTileIndex("CXsq", m_astrSCCliffsets[SCM_TERRAIN_INSTALLATION]);
	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCISUBSTRUCTURE].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCISUBSTRUCTURE].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCISUBSTRUCTURE].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCISUBSTRUCTURE].flags = TILE_FL_WATER;

	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCIFLOOR].groundtype = GetTileIndex("Xsqd", m_astrSCTilesets[SCM_TERRAIN_INSTALLATION]);
	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCIFLOOR].clifftype = GetTileIndex("CXsq", m_astrSCCliffsets[SCM_TERRAIN_INSTALLATION]);
	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCIFLOOR].layer = TILE_LAYER_LEVEL_ZERO + 1;
	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCIFLOOR].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCIFLOOR].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCIFLOOR].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCIROOF].groundtype = GetTileIndex("Xblm", m_astrSCTilesets[SCM_TERRAIN_INSTALLATION]);
	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCIROOF].clifftype = GetTileIndex("CXsq", m_astrSCCliffsets[SCM_TERRAIN_INSTALLATION]);
	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCIROOF].layer = TILE_LAYER_LEVEL_ZERO + 0;
	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCIROOF].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCIROOF].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCIROOF].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCISUBSTRUCTUREPLATING].groundtype = GetTileIndex("Xbtl", m_astrSCTilesets[SCM_TERRAIN_INSTALLATION]);
	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCISUBSTRUCTUREPLATING].clifftype = GetTileIndex("CXsq", m_astrSCCliffsets[SCM_TERRAIN_INSTALLATION]);
	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCISUBSTRUCTUREPLATING].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCISUBSTRUCTUREPLATING].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCISUBSTRUCTUREPLATING].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCISUBSTRUCTUREPLATING].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCIPLATING].groundtype = GetTileIndex("Xbtl", m_astrSCTilesets[SCM_TERRAIN_INSTALLATION]);
	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCIPLATING].clifftype = GetTileIndex("CXsq", m_astrSCCliffsets[SCM_TERRAIN_INSTALLATION]);
	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCIPLATING].layer = TILE_LAYER_LEVEL_ZERO + 1;
	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCIPLATING].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCIPLATING].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCIPLATING].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCISUBSTRUCTUREPANELS].groundtype = GetTileIndex("Xwmb", m_astrSCTilesets[SCM_TERRAIN_INSTALLATION]);
	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCISUBSTRUCTUREPANELS].clifftype = GetTileIndex("CXsq", m_astrSCCliffsets[SCM_TERRAIN_INSTALLATION]);
	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCISUBSTRUCTUREPANELS].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCISUBSTRUCTUREPANELS].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCISUBSTRUCTUREPANELS].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCISUBSTRUCTUREPANELS].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCIBOTTOMLESSPIT].groundtype = GetTileIndex("Xblm", m_astrSCTilesets[SCM_TERRAIN_INSTALLATION]);
	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCIBOTTOMLESSPIT].clifftype = GetTileIndex("CXsq", m_astrSCCliffsets[SCM_TERRAIN_INSTALLATION]);
	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCIBOTTOMLESSPIT].layer = TILE_LAYER_LEVEL_ZERO - 2;
	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCIBOTTOMLESSPIT].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCIBOTTOMLESSPIT].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_INSTALLATION][INDEX_SET_SCIBOTTOMLESSPIT].flags = TILE_FL_WATER;

	// Ashworld
	m_acSCMainTileset[SCM_TERRAIN_ASHWORLD] = 'D';
	m_astrSCTilesets[SCM_TERRAIN_ASHWORLD] = "DdrtDlvcDgrsDdkrDbrk";
	m_astrSCCliffsets[SCM_TERRAIN_ASHWORLD] = "CDdi";

	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWMAGMA].groundtype = GetTileIndex("Ddrt", m_astrSCTilesets[SCM_TERRAIN_ASHWORLD]);
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWMAGMA].clifftype = GetTileIndex("CDdi", m_astrSCCliffsets[SCM_TERRAIN_ASHWORLD]);
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWMAGMA].layer = TILE_LAYER_LEVEL_ZERO - 2;
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWMAGMA].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWMAGMA].waterlevel = TILE_HEIGHT_LEVEL_ZERO - 384;
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWMAGMA].flags = TILE_FL_WATER;

	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWDIRT].groundtype = GetTileIndex("Ddrt", m_astrSCTilesets[SCM_TERRAIN_ASHWORLD]);
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWDIRT].clifftype = GetTileIndex("CDdi", m_astrSCCliffsets[SCM_TERRAIN_ASHWORLD]);
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWDIRT].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWDIRT].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWDIRT].waterlevel = TILE_HEIGHT_LEVEL_ZERO - 384;
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWDIRT].flags = TILE_FL_WATER;

	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWLAVA].groundtype = GetTileIndex("Dlvc", m_astrSCTilesets[SCM_TERRAIN_ASHWORLD]);
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWLAVA].clifftype = GetTileIndex("CDdi", m_astrSCCliffsets[SCM_TERRAIN_ASHWORLD]);
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWLAVA].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWLAVA].height = TILE_HEIGHT_LEVEL_ZERO - 192;
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWLAVA].waterlevel = TILE_HEIGHT_LEVEL_ZERO - 384;
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWLAVA].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWSHALE].groundtype = GetTileIndex("Dgrs", m_astrSCTilesets[SCM_TERRAIN_ASHWORLD]);
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWSHALE].clifftype = GetTileIndex("CDdi", m_astrSCCliffsets[SCM_TERRAIN_ASHWORLD]);
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWSHALE].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWSHALE].height = TILE_HEIGHT_LEVEL_ZERO + 192;
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWSHALE].waterlevel = TILE_HEIGHT_LEVEL_ZERO - 384;
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWSHALE].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWHIGHDIRT].groundtype = GetTileIndex("Ddrt", m_astrSCTilesets[SCM_TERRAIN_ASHWORLD]);
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWHIGHDIRT].clifftype = GetTileIndex("CDdi", m_astrSCCliffsets[SCM_TERRAIN_ASHWORLD]);
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWHIGHDIRT].layer = TILE_LAYER_LEVEL_ZERO + 1;
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWHIGHDIRT].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWHIGHDIRT].waterlevel = TILE_HEIGHT_LEVEL_ZERO - 384;
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWHIGHDIRT].flags = TILE_FL_WATER;

	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWHIGHLAVA].groundtype = GetTileIndex("Dbrk", m_astrSCTilesets[SCM_TERRAIN_ASHWORLD]);
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWHIGHLAVA].clifftype = GetTileIndex("CDdi", m_astrSCCliffsets[SCM_TERRAIN_ASHWORLD]);
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWHIGHLAVA].layer = TILE_LAYER_LEVEL_ZERO + 1;
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWHIGHLAVA].height = TILE_HEIGHT_LEVEL_ZERO - 192;
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWHIGHLAVA].waterlevel = TILE_HEIGHT_LEVEL_ZERO - 384;
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWHIGHLAVA].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWHIGHSHALE].groundtype = GetTileIndex("Dgrs", m_astrSCTilesets[SCM_TERRAIN_ASHWORLD]);
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWHIGHSHALE].clifftype = GetTileIndex("CDdi", m_astrSCCliffsets[SCM_TERRAIN_ASHWORLD]);
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWHIGHSHALE].layer = TILE_LAYER_LEVEL_ZERO + 1;
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWHIGHSHALE].height = TILE_HEIGHT_LEVEL_ZERO + 192;
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWHIGHSHALE].waterlevel = TILE_HEIGHT_LEVEL_ZERO - 384;
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWHIGHSHALE].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWBROKENROCK].groundtype = GetTileIndex("Ddkr", m_astrSCTilesets[SCM_TERRAIN_ASHWORLD]);
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWBROKENROCK].clifftype = GetTileIndex("CDdi", m_astrSCCliffsets[SCM_TERRAIN_ASHWORLD]);
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWBROKENROCK].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWBROKENROCK].height = TILE_HEIGHT_LEVEL_ZERO + 64;
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWBROKENROCK].waterlevel = TILE_HEIGHT_LEVEL_ZERO - 384;
	m_atSCTilepoints[SCM_TERRAIN_ASHWORLD][INDEX_SET_SCAWBROKENROCK].flags = 0;

	// Jungle World
	m_acSCMainTileset[SCM_TERRAIN_JUNGLE] = 'L';
	m_astrSCTilesets[SCM_TERRAIN_JUNGLE] = "LdrtLdroLgrsBflrLrokLgrdLdrg";
	m_astrSCCliffsets[SCM_TERRAIN_JUNGLE] = "CLdiCLgr";

	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWDIRT].groundtype = GetTileIndex("Ldrt", m_astrSCTilesets[SCM_TERRAIN_JUNGLE]);
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWDIRT].clifftype = GetTileIndex("CLdi", m_astrSCCliffsets[SCM_TERRAIN_JUNGLE]);
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWDIRT].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWDIRT].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWDIRT].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWDIRT].flags = TILE_FL_WATER;

	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWHIGHDIRT].groundtype = GetTileIndex("Ldrt", m_astrSCTilesets[SCM_TERRAIN_JUNGLE]);
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWHIGHDIRT].clifftype = GetTileIndex("CLdi", m_astrSCCliffsets[SCM_TERRAIN_JUNGLE]);
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWHIGHDIRT].layer = TILE_LAYER_LEVEL_ZERO + 1;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWHIGHDIRT].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWHIGHDIRT].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWHIGHDIRT].flags = TILE_FL_WATER;

	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWWATER].groundtype = GetTileIndex("Ldrt", m_astrSCTilesets[SCM_TERRAIN_JUNGLE]);
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWWATER].clifftype = GetTileIndex("CLdi", m_astrSCCliffsets[SCM_TERRAIN_JUNGLE]);
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWWATER].layer = TILE_LAYER_LEVEL_ZERO - 1;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWWATER].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWWATER].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWWATER].flags = TILE_FL_WATER;

	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWJUNGLE].groundtype = GetTileIndex("Lgrs", m_astrSCTilesets[SCM_TERRAIN_JUNGLE]);
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWJUNGLE].clifftype = GetTileIndex("CLgr", m_astrSCCliffsets[SCM_TERRAIN_JUNGLE]);
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWJUNGLE].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWJUNGLE].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWJUNGLE].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWJUNGLE].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWRAISEDJUNGLE].groundtype = GetTileIndex("Lgrd", m_astrSCTilesets[SCM_TERRAIN_JUNGLE]);
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWRAISEDJUNGLE].clifftype = GetTileIndex("CLdi", m_astrSCCliffsets[SCM_TERRAIN_JUNGLE]);
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWRAISEDJUNGLE].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWRAISEDJUNGLE].height = TILE_HEIGHT_LEVEL_ZERO + 288;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWRAISEDJUNGLE].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWRAISEDJUNGLE].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWROCKYGROUND].groundtype = GetTileIndex("Bflr", m_astrSCTilesets[SCM_TERRAIN_JUNGLE]);
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWROCKYGROUND].clifftype = GetTileIndex("CLdi", m_astrSCCliffsets[SCM_TERRAIN_JUNGLE]);
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWROCKYGROUND].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWROCKYGROUND].height = TILE_HEIGHT_LEVEL_ZERO + 64;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWROCKYGROUND].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWROCKYGROUND].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWRUINS].groundtype = GetTileIndex("Lrok", m_astrSCTilesets[SCM_TERRAIN_JUNGLE]);
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWRUINS].clifftype = GetTileIndex("CLdi", m_astrSCCliffsets[SCM_TERRAIN_JUNGLE]);
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWRUINS].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWRUINS].height = TILE_HEIGHT_LEVEL_ZERO + 192;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWRUINS].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWRUINS].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWTEMPLE].groundtype = GetTileIndex("Ldrg", m_astrSCTilesets[SCM_TERRAIN_JUNGLE]);
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWTEMPLE].clifftype = GetTileIndex("CLgr", m_astrSCCliffsets[SCM_TERRAIN_JUNGLE]);
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWTEMPLE].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWTEMPLE].height = TILE_HEIGHT_LEVEL_ZERO + 1;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWTEMPLE].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWTEMPLE].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWHIGHJUNGLE].groundtype = GetTileIndex("Lgrs", m_astrSCTilesets[SCM_TERRAIN_JUNGLE]);
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWHIGHJUNGLE].clifftype = GetTileIndex("CLgr", m_astrSCCliffsets[SCM_TERRAIN_JUNGLE]);
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWHIGHJUNGLE].layer = TILE_LAYER_LEVEL_ZERO + 1;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWHIGHJUNGLE].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWHIGHJUNGLE].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWHIGHJUNGLE].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWHIGHRUINS].groundtype = GetTileIndex("Lrok", m_astrSCTilesets[SCM_TERRAIN_JUNGLE]);
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWHIGHRUINS].clifftype = GetTileIndex("CLdi", m_astrSCCliffsets[SCM_TERRAIN_JUNGLE]);
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWHIGHRUINS].layer = TILE_LAYER_LEVEL_ZERO + 1;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWHIGHRUINS].height = TILE_HEIGHT_LEVEL_ZERO + 192;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWHIGHRUINS].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWHIGHRUINS].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWHIGHRAISEDJUNGLE].groundtype = GetTileIndex("Lgrd", m_astrSCTilesets[SCM_TERRAIN_JUNGLE]);
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWHIGHRAISEDJUNGLE].clifftype = GetTileIndex("CLdi", m_astrSCCliffsets[SCM_TERRAIN_JUNGLE]);
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWHIGHRAISEDJUNGLE].layer = TILE_LAYER_LEVEL_ZERO + 1;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWHIGHRAISEDJUNGLE].height = TILE_HEIGHT_LEVEL_ZERO + 288;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWHIGHRAISEDJUNGLE].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWHIGHRAISEDJUNGLE].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWHIGHTEMPLE].groundtype = GetTileIndex("Ldrg", m_astrSCTilesets[SCM_TERRAIN_JUNGLE]);
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWHIGHTEMPLE].clifftype = GetTileIndex("CLgr", m_astrSCCliffsets[SCM_TERRAIN_JUNGLE]);
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWHIGHTEMPLE].layer = TILE_LAYER_LEVEL_ZERO + 2;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWHIGHTEMPLE].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWHIGHTEMPLE].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWHIGHTEMPLE].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWMUD].groundtype = GetTileIndex("Ldro", m_astrSCTilesets[SCM_TERRAIN_JUNGLE]);
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWMUD].clifftype = GetTileIndex("CLdi", m_astrSCCliffsets[SCM_TERRAIN_JUNGLE]);
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWMUD].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWMUD].height = TILE_HEIGHT_LEVEL_ZERO - 128;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWMUD].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_JUNGLE][INDEX_SET_SCJWMUD].flags = 0;

	// Desert World
	m_acSCMainTileset[SCM_TERRAIN_DESERT] = 'B';
	m_astrSCTilesets[SCM_TERRAIN_DESERT] = "BdrtBdsdBdrhBdsrBdrrBflrBgrr";
	m_astrSCCliffsets[SCM_TERRAIN_DESERT] = "CBdeCBgr";

	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDDIRT].groundtype = GetTileIndex("Bdsd", m_astrSCTilesets[SCM_TERRAIN_DESERT]);
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDDIRT].clifftype = GetTileIndex("CBde", m_astrSCCliffsets[SCM_TERRAIN_DESERT]);
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDDIRT].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDDIRT].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDDIRT].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDDIRT].flags = TILE_FL_WATER;

	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDHIGHDIRT].groundtype = GetTileIndex("Bdsd", m_astrSCTilesets[SCM_TERRAIN_DESERT]);
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDHIGHDIRT].clifftype = GetTileIndex("CBde", m_astrSCCliffsets[SCM_TERRAIN_DESERT]);
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDHIGHDIRT].layer = TILE_LAYER_LEVEL_ZERO + 1;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDHIGHDIRT].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDHIGHDIRT].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDHIGHDIRT].flags = TILE_FL_WATER;

	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDTAR].groundtype = GetTileIndex("Bdrt", m_astrSCTilesets[SCM_TERRAIN_DESERT]);
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDTAR].clifftype = GetTileIndex("CBde", m_astrSCCliffsets[SCM_TERRAIN_DESERT]);
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDTAR].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDTAR].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDTAR].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDTAR].flags = TILE_FL_WATER;

	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDSANDDUNES].groundtype = GetTileIndex("Bdsr", m_astrSCTilesets[SCM_TERRAIN_DESERT]);
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDSANDDUNES].clifftype = GetTileIndex("CBgr", m_astrSCCliffsets[SCM_TERRAIN_DESERT]);
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDSANDDUNES].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDSANDDUNES].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDSANDDUNES].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDSANDDUNES].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDSANDYSUNKENPIT].groundtype = GetTileIndex("Bdrt", m_astrSCTilesets[SCM_TERRAIN_DESERT]);
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDSANDYSUNKENPIT].clifftype = GetTileIndex("CBgr", m_astrSCCliffsets[SCM_TERRAIN_DESERT]);
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDSANDYSUNKENPIT].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDSANDYSUNKENPIT].height = TILE_HEIGHT_LEVEL_ZERO - 256;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDSANDYSUNKENPIT].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDSANDYSUNKENPIT].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDROCKYGROUND].groundtype = GetTileIndex("Bdrr", m_astrSCTilesets[SCM_TERRAIN_DESERT]);
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDROCKYGROUND].clifftype = GetTileIndex("CBde", m_astrSCCliffsets[SCM_TERRAIN_DESERT]);
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDROCKYGROUND].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDROCKYGROUND].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDROCKYGROUND].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDROCKYGROUND].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDCRAGS].groundtype = GetTileIndex("Bflr", m_astrSCTilesets[SCM_TERRAIN_DESERT]);
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDCRAGS].clifftype = GetTileIndex("CBgr", m_astrSCCliffsets[SCM_TERRAIN_DESERT]);
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDCRAGS].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDCRAGS].height = TILE_HEIGHT_LEVEL_ZERO + 192;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDCRAGS].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDCRAGS].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDCOMPOUND].groundtype = GetTileIndex("Bgrr", m_astrSCTilesets[SCM_TERRAIN_DESERT]);
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDCOMPOUND].clifftype = GetTileIndex("CBgr", m_astrSCCliffsets[SCM_TERRAIN_DESERT]);
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDCOMPOUND].layer = TILE_LAYER_LEVEL_ZERO + 1;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDCOMPOUND].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDCOMPOUND].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDCOMPOUND].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDHIGHSANDDUNES].groundtype = GetTileIndex("Bdsr", m_astrSCTilesets[SCM_TERRAIN_DESERT]);
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDHIGHSANDDUNES].clifftype = GetTileIndex("CBgr", m_astrSCCliffsets[SCM_TERRAIN_DESERT]);
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDHIGHSANDDUNES].layer = TILE_LAYER_LEVEL_ZERO + 1;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDHIGHSANDDUNES].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDHIGHSANDDUNES].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDHIGHSANDDUNES].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDHIGHCRAGS].groundtype = GetTileIndex("Bflr", m_astrSCTilesets[SCM_TERRAIN_DESERT]);
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDHIGHCRAGS].clifftype = GetTileIndex("CBgr", m_astrSCCliffsets[SCM_TERRAIN_DESERT]);
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDHIGHCRAGS].layer = TILE_LAYER_LEVEL_ZERO + 1;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDHIGHCRAGS].height = TILE_HEIGHT_LEVEL_ZERO + 192;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDHIGHCRAGS].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDHIGHCRAGS].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDHIGHSANDYSUNKENPIT].groundtype = GetTileIndex("Bdrt", m_astrSCTilesets[SCM_TERRAIN_DESERT]);
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDHIGHSANDYSUNKENPIT].clifftype = GetTileIndex("CBgr", m_astrSCCliffsets[SCM_TERRAIN_DESERT]);
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDHIGHSANDYSUNKENPIT].layer = TILE_LAYER_LEVEL_ZERO + 1;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDHIGHSANDYSUNKENPIT].height = TILE_HEIGHT_LEVEL_ZERO - 256;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDHIGHSANDYSUNKENPIT].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDHIGHSANDYSUNKENPIT].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDHIGHCOMPOUND].groundtype = GetTileIndex("Bgrr", m_astrSCTilesets[SCM_TERRAIN_DESERT]);
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDHIGHCOMPOUND].clifftype = GetTileIndex("CBgr", m_astrSCCliffsets[SCM_TERRAIN_DESERT]);
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDHIGHCOMPOUND].layer = TILE_LAYER_LEVEL_ZERO + 2;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDHIGHCOMPOUND].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDHIGHCOMPOUND].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDHIGHCOMPOUND].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDDRIEDMUD].groundtype = GetTileIndex("Bdrh", m_astrSCTilesets[SCM_TERRAIN_DESERT]);
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDDRIEDMUD].clifftype = GetTileIndex("CBde", m_astrSCCliffsets[SCM_TERRAIN_DESERT]);
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDDRIEDMUD].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDDRIEDMUD].height = TILE_HEIGHT_LEVEL_ZERO - 128;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDDRIEDMUD].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_DESERT][INDEX_SET_SCDDRIEDMUD].flags = 0;

	// Arctic World
	m_acSCMainTileset[SCM_TERRAIN_ARCTIC] = 'W';
	m_astrSCTilesets[SCM_TERRAIN_ARCTIC] = "NiceWsnwIiceWdrtNsnrWgrs";
	m_astrSCCliffsets[SCM_TERRAIN_ARCTIC] = "CWsnCWgr";

	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWSNOW].groundtype = GetTileIndex("Wsnw", m_astrSCTilesets[SCM_TERRAIN_ARCTIC]);
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWSNOW].clifftype = GetTileIndex("CWsn", m_astrSCCliffsets[SCM_TERRAIN_ARCTIC]);
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWSNOW].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWSNOW].height = TILE_HEIGHT_LEVEL_ZERO + 128;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWSNOW].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWSNOW].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWHIGHSNOW].groundtype = GetTileIndex("Wsnw", m_astrSCTilesets[SCM_TERRAIN_ARCTIC]);
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWHIGHSNOW].clifftype = GetTileIndex("CWsn", m_astrSCCliffsets[SCM_TERRAIN_ARCTIC]);
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWHIGHSNOW].layer = TILE_LAYER_LEVEL_ZERO + 1;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWHIGHSNOW].height = TILE_HEIGHT_LEVEL_ZERO + 192;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWHIGHSNOW].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWHIGHSNOW].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWICE].groundtype = GetTileIndex("Nice", m_astrSCTilesets[SCM_TERRAIN_ARCTIC]);
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWICE].clifftype = GetTileIndex("CWsn", m_astrSCCliffsets[SCM_TERRAIN_ARCTIC]);
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWICE].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWICE].height = TILE_HEIGHT_LEVEL_ZERO - 192;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWICE].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWICE].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWDIRT].groundtype = GetTileIndex("Wdrt", m_astrSCTilesets[SCM_TERRAIN_ARCTIC]);
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWDIRT].clifftype = GetTileIndex("CWgr", m_astrSCCliffsets[SCM_TERRAIN_ARCTIC]);
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWDIRT].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWDIRT].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWDIRT].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWDIRT].flags = TILE_FL_WATER;

	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWWATER].groundtype = GetTileIndex("Wdrt", m_astrSCTilesets[SCM_TERRAIN_ARCTIC]);
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWWATER].clifftype = GetTileIndex("CWgr", m_astrSCCliffsets[SCM_TERRAIN_ARCTIC]);
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWWATER].layer = TILE_LAYER_LEVEL_ZERO - 2;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWWATER].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWWATER].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWWATER].flags = TILE_FL_WATER;

	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWROCKYSNOW].groundtype = GetTileIndex("Nsnr", m_astrSCTilesets[SCM_TERRAIN_ARCTIC]);
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWROCKYSNOW].clifftype = GetTileIndex("CWsn", m_astrSCCliffsets[SCM_TERRAIN_ARCTIC]);
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWROCKYSNOW].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWROCKYSNOW].height = TILE_HEIGHT_LEVEL_ZERO + 128;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWROCKYSNOW].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWROCKYSNOW].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWGRASS].groundtype = GetTileIndex("Wgrs", m_astrSCTilesets[SCM_TERRAIN_ARCTIC]);
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWGRASS].clifftype = GetTileIndex("CWgr", m_astrSCCliffsets[SCM_TERRAIN_ARCTIC]);
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWGRASS].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWGRASS].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWGRASS].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWGRASS].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWOUTPOST].groundtype = GetTileIndex("Wgrs", m_astrSCTilesets[SCM_TERRAIN_ARCTIC]);
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWOUTPOST].clifftype = GetTileIndex("CWgr", m_astrSCCliffsets[SCM_TERRAIN_ARCTIC]);
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWOUTPOST].layer = TILE_LAYER_LEVEL_ZERO + 1;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWOUTPOST].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWOUTPOST].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWOUTPOST].flags = TILE_FL_WATER;

	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWHIGHDIRT].groundtype = GetTileIndex("Wdrt", m_astrSCTilesets[SCM_TERRAIN_ARCTIC]);
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWHIGHDIRT].clifftype = GetTileIndex("CWgr", m_astrSCCliffsets[SCM_TERRAIN_ARCTIC]);
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWHIGHDIRT].layer = TILE_LAYER_LEVEL_ZERO + 1;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWHIGHDIRT].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWHIGHDIRT].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWHIGHDIRT].flags = TILE_FL_WATER;

	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWHIGHGRASS].groundtype = GetTileIndex("Wgrs", m_astrSCTilesets[SCM_TERRAIN_ARCTIC]);
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWHIGHGRASS].clifftype = GetTileIndex("CWgr", m_astrSCCliffsets[SCM_TERRAIN_ARCTIC]);
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWHIGHGRASS].layer = TILE_LAYER_LEVEL_ZERO + 1;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWHIGHGRASS].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWHIGHGRASS].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWHIGHGRASS].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWHIGHWATER].groundtype = GetTileIndex("Wdrt", m_astrSCTilesets[SCM_TERRAIN_ARCTIC]);
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWHIGHWATER].clifftype = GetTileIndex("CWgr", m_astrSCCliffsets[SCM_TERRAIN_ARCTIC]);
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWHIGHWATER].layer = TILE_LAYER_LEVEL_ZERO - 1;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWHIGHWATER].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWHIGHWATER].waterlevel = TILE_HEIGHT_LEVEL_ZERO + 512;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWHIGHWATER].flags = TILE_FL_WATER;

	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWHIGHOUTPOST].groundtype = GetTileIndex("Wgrs", m_astrSCTilesets[SCM_TERRAIN_ARCTIC]);
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWHIGHOUTPOST].clifftype = GetTileIndex("CWgr", m_astrSCCliffsets[SCM_TERRAIN_ARCTIC]);
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWHIGHOUTPOST].layer = TILE_LAYER_LEVEL_ZERO + 2;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWHIGHOUTPOST].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWHIGHOUTPOST].waterlevel = TILE_HEIGHT_LEVEL_ZERO + 512;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWHIGHOUTPOST].flags = TILE_FL_WATER;

	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWMOGULS].groundtype = GetTileIndex("Iice", m_astrSCTilesets[SCM_TERRAIN_ARCTIC]);
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWMOGULS].clifftype = GetTileIndex("CWsn", m_astrSCCliffsets[SCM_TERRAIN_ARCTIC]);
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWMOGULS].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWMOGULS].height = TILE_HEIGHT_LEVEL_ZERO + 192;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWMOGULS].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_ARCTIC][INDEX_SET_SCIWMOGULS].flags = 0;

	// Twilight World
	m_acSCMainTileset[SCM_TERRAIN_TWILIGHT] = 'G';
	m_astrSCTilesets[SCM_TERRAIN_TWILIGHT] = "GdrtCdrdGrdsGbrkGgrsGdkrGsqd";
	m_astrSCCliffsets[SCM_TERRAIN_TWILIGHT] = "CGdiCGsq";

	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTDIRT].groundtype = GetTileIndex("Gdrt", m_astrSCTilesets[SCM_TERRAIN_TWILIGHT]);
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTDIRT].clifftype = GetTileIndex("CGdi", m_astrSCCliffsets[SCM_TERRAIN_TWILIGHT]);
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTDIRT].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTDIRT].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTDIRT].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTDIRT].flags = TILE_FL_WATER;

	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTHIGHDIRT].groundtype = GetTileIndex("Gdrt", m_astrSCTilesets[SCM_TERRAIN_TWILIGHT]);
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTHIGHDIRT].clifftype = GetTileIndex("CGdi", m_astrSCCliffsets[SCM_TERRAIN_TWILIGHT]);
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTHIGHDIRT].layer = TILE_LAYER_LEVEL_ZERO + 1;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTHIGHDIRT].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTHIGHDIRT].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTHIGHDIRT].flags = TILE_FL_WATER;

	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTWATER].groundtype = GetTileIndex("Gdrt", m_astrSCTilesets[SCM_TERRAIN_TWILIGHT]);
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTWATER].clifftype = GetTileIndex("CGdi", m_astrSCCliffsets[SCM_TERRAIN_TWILIGHT]);
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTWATER].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTWATER].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTWATER].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTWATER].flags = TILE_FL_WATER;

	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTCRUSHEDROCK].groundtype = GetTileIndex("Grds", m_astrSCTilesets[SCM_TERRAIN_TWILIGHT]);
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTCRUSHEDROCK].clifftype = GetTileIndex("CGsq", m_astrSCCliffsets[SCM_TERRAIN_TWILIGHT]);
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTCRUSHEDROCK].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTCRUSHEDROCK].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTCRUSHEDROCK].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTCRUSHEDROCK].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTSUNKENGROUND].groundtype = GetTileIndex("Gdkr", m_astrSCTilesets[SCM_TERRAIN_TWILIGHT]);
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTSUNKENGROUND].clifftype = GetTileIndex("CGdi", m_astrSCCliffsets[SCM_TERRAIN_TWILIGHT]);
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTSUNKENGROUND].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTSUNKENGROUND].height = TILE_HEIGHT_LEVEL_ZERO - 192;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTSUNKENGROUND].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTSUNKENGROUND].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTCREVICES].groundtype = GetTileIndex("Gbrk", m_astrSCTilesets[SCM_TERRAIN_TWILIGHT]);
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTCREVICES].clifftype = GetTileIndex("CGdi", m_astrSCCliffsets[SCM_TERRAIN_TWILIGHT]);
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTCREVICES].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTCREVICES].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTCREVICES].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTCREVICES].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTFLAGSTONES].groundtype = GetTileIndex("Ggrs", m_astrSCTilesets[SCM_TERRAIN_TWILIGHT]);
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTFLAGSTONES].clifftype = GetTileIndex("CGdi", m_astrSCCliffsets[SCM_TERRAIN_TWILIGHT]);
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTFLAGSTONES].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTFLAGSTONES].height = TILE_HEIGHT_LEVEL_ZERO + 128;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTFLAGSTONES].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTFLAGSTONES].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTBASILICA].groundtype = GetTileIndex("Gsqd", m_astrSCTilesets[SCM_TERRAIN_TWILIGHT]);
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTBASILICA].clifftype = GetTileIndex("CGsq", m_astrSCCliffsets[SCM_TERRAIN_TWILIGHT]);
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTBASILICA].layer = TILE_LAYER_LEVEL_ZERO + 1;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTBASILICA].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTBASILICA].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTBASILICA].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTHIGHCRUSHEDROCKS].groundtype = GetTileIndex("Grds", m_astrSCTilesets[SCM_TERRAIN_TWILIGHT]);
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTHIGHCRUSHEDROCKS].clifftype = GetTileIndex("CGsq", m_astrSCCliffsets[SCM_TERRAIN_TWILIGHT]);
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTHIGHCRUSHEDROCKS].layer = TILE_LAYER_LEVEL_ZERO + 1;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTHIGHCRUSHEDROCKS].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTHIGHCRUSHEDROCKS].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTHIGHCRUSHEDROCKS].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTHIGHFLAGSTONES].groundtype = GetTileIndex("Ggrs", m_astrSCTilesets[SCM_TERRAIN_TWILIGHT]);
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTHIGHFLAGSTONES].clifftype = GetTileIndex("CGdi", m_astrSCCliffsets[SCM_TERRAIN_TWILIGHT]);
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTHIGHFLAGSTONES].layer = TILE_LAYER_LEVEL_ZERO + 1;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTHIGHFLAGSTONES].height = TILE_HEIGHT_LEVEL_ZERO + 128;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTHIGHFLAGSTONES].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTHIGHFLAGSTONES].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTHIGHSUNKENGROUND].groundtype = GetTileIndex("Gdkr", m_astrSCTilesets[SCM_TERRAIN_TWILIGHT]);
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTHIGHSUNKENGROUND].clifftype = GetTileIndex("CGdi", m_astrSCCliffsets[SCM_TERRAIN_TWILIGHT]);
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTHIGHSUNKENGROUND].layer = TILE_LAYER_LEVEL_ZERO + 1;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTHIGHSUNKENGROUND].height = TILE_HEIGHT_LEVEL_ZERO - 192;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTHIGHSUNKENGROUND].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTHIGHSUNKENGROUND].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTHIGHBASILICA].groundtype = GetTileIndex("Gsqd", m_astrSCTilesets[SCM_TERRAIN_TWILIGHT]);
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTHIGHBASILICA].clifftype = GetTileIndex("CGsq", m_astrSCCliffsets[SCM_TERRAIN_TWILIGHT]);
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTHIGHBASILICA].layer = TILE_LAYER_LEVEL_ZERO + 2;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTHIGHBASILICA].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTHIGHBASILICA].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTHIGHBASILICA].flags = 0;

	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTMUD].groundtype = GetTileIndex("Cdrd", m_astrSCTilesets[SCM_TERRAIN_TWILIGHT]);
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTMUD].clifftype = GetTileIndex("CGdi", m_astrSCCliffsets[SCM_TERRAIN_TWILIGHT]);
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTMUD].layer = TILE_LAYER_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTMUD].height = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTMUD].waterlevel = TILE_HEIGHT_LEVEL_ZERO;
	m_atSCTilepoints[SCM_TERRAIN_TWILIGHT][INDEX_SET_SCTMUD].flags = 0;

	//m_astrSCDoodads[SCM_TERRAIN_BADLANDS][];

}

CW3ZMapEditSettings::~CW3ZMapEditSettings()
{
	ClearTilesetsList();
	ClearCliffTilesetList();
	ClearTilesetGroupList();
	ClearPluginList();
}

/*
InitParameters
**************
Reads the ini file and set up the app parameters.

Int:
 none

Out:
 none

Return:
 none


*/
void CW3ZMapEditSettings::InitParameters()
{
	CFile paramfile;
	char* pBuf = NULL;
	int i=0, j=0, size = 0, pos = 0;
	BOOL bEndBlock = FALSE;

	TRY
	{
		paramfile.Open(STR_INIFILE, CFile::modeNoTruncate | CFile::modeReadWrite);
		size = (int)paramfile.GetLength();
		if (size > 0)
		{
			pBuf = new char[size];
		}
		if (pBuf)
		{
			paramfile.Read(pBuf, size);
			CString strInit(pBuf, size);
			delete []pBuf;
			pBuf = NULL;

			CString line, block, field;

			i = 0; 
			line = strInit.Tokenize("\n", i); // gets the line
			while ((i < strInit.GetLength()) && (0 <= i))
			{
				j = line.Find(";");
				if (0 <= j)
				{
					line = line.Left(j);// remove the comments
				}
				line.TrimLeft();
				line.TrimRight();
				if ((line.GetLength() > 0) && (line[0] == '['))
				{ // a block has been defined
					//gets the block name in 'field'
					field = line.Mid(1); //remove the '['
					pos = field.Find(']');
					if (pos > 0) // make sure the field is correctly defined
					{
						field = field.Left(pos);
						//gets the block
						block = "";
						bEndBlock = FALSE;
						while ((i < strInit.GetLength()) && (0 <= i) && !bEndBlock)
						{
							if (0 <= i)
							{
								line = strInit.Tokenize("\n", i); // gets next line
							}

							j = line.Find(";");
							if (0 <= j)
							{
								line = line.Left(j);// remove the comments
							}
							line.TrimLeft();
							line.TrimRight();
							if (line[0] == '[')
							{ // new block detected
								bEndBlock = TRUE;
								//i--;
							}
							else if (line.GetLength() > 0)
							{// otherwise, add the non-empty line to the block
								block = block + line + '\n';
							}
							//i++;
						}
						// find what to do with the block
						if (field.CompareNoCase(STR_SET_MISC) == 0)
						{
							LoadMisc(block);
						}
						else if (field.CompareNoCase(STR_SET_EDITMODES) == 0)
						{
							LoadEditModes(block);
						}
						else if (field.CompareNoCase(STR_SET_TILESPROP) == 0)
						{
							LoadTilesProp(block);
						}
						else if (field.CompareNoCase(STR_SET_LAYERS) == 0)
						{
							LoadLayersProp(block);
						}
						else if (field.CompareNoCase(STR_SET_TILESDEF) == 0)
						{// loads tileset definition from the block
							LoadTilesDef(block);
						}
						else if (field.CompareNoCase(STR_SET_LOADPLUGINS) == 0)
						{
							LoadLPlugins(block);
						}
						else if (field.CompareNoCase(STR_SET_SAVEPLUGINS) == 0)
						{
							LoadSPlugins(block);
						}
						else if (field.CompareNoCase(STR_SET_MENUPLUGINS) == 0)
						{
							LoadMPlugins(block);
						}
						else if (field.CompareNoCase(STR_SET_PATHING) == 0)
						{
							LoadPathingProp(block);
						}
						else if (field.CompareNoCase(STR_SET_CONVERSION) == 0)
						{
							LoadConvertion(block);
						}
					}
				}
				if (TRUE == bEndBlock)
				{
					bEndBlock = FALSE;
				}
				else
				{
					if (0 <= i)
					{
						line = strInit.Tokenize("\n", i); // gets next line
					}
				}
			}
		}
	}
	CATCH(CFileException, e)
	{
	}
	END_CATCH
	paramfile.Close();

}


/*
AddTileDef
**********
Add a tileset to the tileset table. A tileset can be defined only once.

In:
 CWar3StrID tileID: tile ID (like "Ldrt")
 CString tileName: tile long name (description like "Lordaeron Summer Dirt")
 COLORREF tileColor: color used in the editor and the minimap to display the tile

Out:
 none

Return:
 none

Note:
 If a tileset has already been defined, it'll not be added.

*/
void CW3ZMapEditSettings::AddTileDef(CWar3StrID tileID, CString tileName, COLORREF tileColor, int tilePathingFlags)
{
	CPListedTileset pLTileset;

	if (!IsTilesetInTilesetList(tileID) && (tileName != ""))
	{
		pLTileset = new CListedTileset;
		pLTileset->tileset.tileID = tileID;
		pLTileset->tileset.tileName = tileName;
		pLTileset->tileset.tileColor = tileColor;
		pLTileset->tileset.tilePathingFlags = tilePathingFlags;
		pLTileset->next = m_pTilesetsHead;
		m_pTilesetsHead = pLTileset;  
	}
}


/*
GetTileColor
************
Returns the color of the specified tileset or the TS_STR_ID_UNDEFINED tileset color
or a null color if not found.

In:
 CWar3StrID tileID: the tileset ID (like "Ldrt").

Out:
 none

Return:
 COLORREF: the tileset color if the tileset was found.

*/
COLORREF CW3ZMapEditSettings::GetTileColor(CWar3StrID tileID)
{
	COLORREF color;
	color = 0;
	CPListedTileset pLTileset;

	pLTileset = m_pTilesetsHead;
	while (pLTileset && (pLTileset->tileset.tileID != tileID))
	{
		if (pLTileset == pLTileset->next)
		{ // cyclic list bug
			pLTileset->next = NULL;
			pLTileset = NULL;
		}
		else
		{
			pLTileset = pLTileset->next;
		}
	}
	if (pLTileset)
	{// tileset found
		color = pLTileset->tileset.tileColor;
	}
	else
	{// not found
		if (tileID != TS_STR_ID_UNDEFINED)
		{// try to get the TS_STR_ID_UNDEFINED tileset color
			color = GetTileColor(TS_STR_ID_UNDEFINED);
		}
	}
	
	return color;
}


/*
IsTileBuildable
****************
Tels if a tile is buildable or not. If the tile has not been found,
it is not buildable by default.

In:
 CWar3StrID tileID: the tileset ID (like "Ldrt").

Out:
 none

Return:
 BOOL: TRUE if buildable, FALSE otherwise or if the tile wasn't found

*/
BOOL CW3ZMapEditSettings::IsTileBuildable(CWar3StrID tileID)
{
	BOOL bBuildable = FALSE;
	CPListedTileset pLTileset;

	pLTileset = m_pTilesetsHead;
	while (pLTileset && (pLTileset->tileset.tileID != tileID))
	{
		if (pLTileset == pLTileset->next)
		{ // cyclic list bug
			pLTileset->next = NULL;
			pLTileset = NULL;
		}
		else
		{
			pLTileset = pLTileset->next;
		}
	}
	if ((pLTileset) && !(pLTileset->tileset.tilePathingFlags & PATH_NOBUILD))
	{// tileset found
		bBuildable = TRUE;
	}
	
	return bBuildable;
}


/*
IsTilesetInTilesetList
**********************
Says if yes or no a tileset is defined in the tileset list.

In:
  CWar3StrID tileID: the tileset ID (like "Ldrt")

Out:
 none

Return:
 BOOL: TRUE if the tileset was found, FALSE otherwise

*/
BOOL CW3ZMapEditSettings::IsTilesetInTilesetList(CWar3StrID tileID)
{
	CPListedTileset pLTileset;
	BOOL ret = FALSE;

	pLTileset = m_pTilesetsHead;
	while (pLTileset && (tileID != pLTileset->tileset.tileName))
	{
		if (pLTileset == pLTileset->next)
		{ // cyclic list bug
			pLTileset->next = NULL;
			pLTileset = NULL;
		}
		else
		{
			pLTileset = pLTileset->next;
		}
	}
	if (pLTileset)
	{// tileset found
		ret = TRUE;
	}
	
	return ret;
}

/*
ClearTilesetsList
*****************
Empty the tileset list.

Int:
 none

Out:
 none

Return:
 none

*/
void CW3ZMapEditSettings::ClearTilesetsList()
{
	CPListedTileset pLTileset;

	while (m_pTilesetsHead)
	{
		pLTileset = m_pTilesetsHead;
		if (m_pTilesetsHead == m_pTilesetsHead->next)
		{ // cyclic list bug
			m_pTilesetsHead = NULL;
		}
		else
		{
			m_pTilesetsHead = m_pTilesetsHead->next;
		}
		//free memory
		delete pLTileset;
	}
}

/*
SetTile
*******
Change tileset name and color for the specified tile ID.

In:
 CWar3StrID tileID: tile ID (like "Ldrt")
 CString tileName: tile long name (description like "Lordaeron Summer Dirt")
 COLORREF tileColor: color used in the editor and the minimap to display the tile

Out:
 none

Return:
 none

*/
void CW3ZMapEditSettings::SetTile(CWar3StrID tileID, CString tileName, COLORREF tileColor)
{
	CPListedTileset pLTileset;

	pLTileset = m_pTilesetsHead;
	while (pLTileset && (tileID != pLTileset->tileset.tileName))
	{
		if (pLTileset == pLTileset->next)
		{ // cyclic list bug
			pLTileset->next = NULL;
			pLTileset = NULL;
		}
		else
		{
			pLTileset = pLTileset->next;
		}
	}
	if (pLTileset)
	{// tileset found
		pLTileset->tileset.tileColor =tileColor ;
		pLTileset->tileset.tileName = tileName;
	}
}

/*
SetTileName
***********
Change tileset name for the specified tile ID.

In:
 CWar3StrID tileID: tile ID (like "Ldrt")
 CString tileName: tile long name (description like "Lordaeron Summer Dirt")

Out:
 none

Return:
 none

*/
void CW3ZMapEditSettings::SetTileName(CWar3StrID tileID, CString tileName)
{
	CPListedTileset pLTileset;

	pLTileset = m_pTilesetsHead;
	while (pLTileset && (tileID != pLTileset->tileset.tileName))
	{
		if (pLTileset == pLTileset->next)
		{ // cyclic list bug
			pLTileset->next = NULL;
			pLTileset = NULL;
		}
		else
		{
			pLTileset = pLTileset->next;
		}
	}
	if (pLTileset)
	{// tileset found
		pLTileset->tileset.tileName = tileName;
	}
}

/*
SetTileColor
************
Change tileset color for the specified tile ID.

In:
 CWar3StrID tileID: tile ID (like "Ldrt")
 COLORREF tileColor: color used in the editor and the minimap to display the tile

Out:
 none

Return:
 none

*/
void CW3ZMapEditSettings::SetTileColor(CWar3StrID tileID, COLORREF tileColor)
{
	CPListedTileset pLTileset;

	pLTileset = m_pTilesetsHead;
	while (pLTileset && (tileID != pLTileset->tileset.tileName))
	{
		if (pLTileset == pLTileset->next)
		{ // cyclic list bug
			pLTileset->next = NULL;
			pLTileset = NULL;
		}
		else
		{
			pLTileset = pLTileset->next;
		}
	}
	if (pLTileset)
	{// tileset found
		pLTileset->tileset.tileColor =tileColor ;
	}
}

/*
LoadTilesDef
************
Loads tiles definition from a init text block.
Sample lines of the inti text block:
 GroupID=L,Lordaeron Summer
 Tile=Ldrt,Lordaeron Summer Dirt,144,100,53
 Cliff=CLdi,Lordaeron Summer Dirt Cliff,Ldrt

In:
 CString block: the block string to analyse

Out:
 none

Return:
 none

*/
void CW3ZMapEditSettings::LoadTilesDef(CString block)
{
	char groupID = TG_CHAR_ID_UNDEFINED;
	CString groupName, tiles, cliffs;
	CWar3StrID tileID;
	CString tileName;
	int iR,iG,iB;
	COLORREF tileColors;
	COLORREF tWaterColor;
	float fWaterAlpha;
	CWar3StrID cliffID;
	CString cliffName;
	CString line, field, values;
	int tilePathingFlags;
	int i, j, k;

	tileID = TS_STR_ID_UNDEFINED;
	cliffID = TC_STR_ID_UNDEFINED;
	i = 0;
	while ((i < block.GetLength()) && (0 <= i))
	{
		line = block.Tokenize("\n", i);
		j = line.Find(";");
		if (0 <= j)
		{
			line = line.Left(j);// remove the comments
		}
		line.TrimLeft();
		line.TrimRight();
		//gets the field type
		j = 0;
		field = line.Tokenize("=", j);
		field.TrimLeft();
		field.TrimRight();

		if (field.CompareNoCase(STR_SET_TILESDEFGRPID) == 0)
		{// group definition detected
// GroupID=L,Lordaeron Summer
			groupID = TG_CHAR_ID_UNDEFINED;
			groupName = TG_STR_NAME_UNDEFINED;
			tiles = TS_STR_NAME_UNDEFINED;
			cliffs = TC_STR_NAME_UNDEFINED;
			tileID = TS_STR_ID_UNDEFINED; // resets the last tileID used
			tWaterColor = TS_WATER_COLOR;
			fWaterAlpha = TS_WATER_ALPHA;
			//gets the values...
			values = line.Tokenize(";", j);
			//gets the groupdID
			k = 0;
			field = values.Tokenize(",", k);
			field.TrimLeft();
			field.TrimRight();
			if (field.GetLength() == 1)
			{// if the group ID is defined...
				groupID = field[0];
				if (0 <= k)
				{
					// gets group name
					groupName = values.Tokenize(",", k);
					groupName.TrimLeft();
					groupName.TrimRight();
					if (0 <= k)
					{
						// gets water color
						field = values.Tokenize(",", k);
						field.TrimLeft();
						field.TrimRight();
						if ("" != field)
						{
							iR = atoi(field);
							if (iR < 0)
							{iR = 0;}
							else if (iR > 255)
							{iR = 255;}

							if (0 <= k)
							{
								field = values.Tokenize(",", k);
								field.TrimLeft();
								field.TrimRight();
							}
							iG = atoi(field);
							if (iG < 0)
							{iG = 0;}
							else if (iG > 255)
							{iG = 255;}

							if (0 <= k)
							{
								field = values.Tokenize(",", k);
								field.TrimLeft();
								field.TrimRight();
							}
							iB = atoi(field);
							if (iB < 0)
							{iB = 0;}
							else if (iB > 255)
							{iB = 255;}
							tWaterColor = iB*0x00010000 + iG*0x00000100 + iR;
							// gets water alpha value
							if (0 <= k)
							{
								field = values.Tokenize(",", k);
								field.TrimLeft();
								field.TrimRight();
								fWaterAlpha = (float)atof(field);
								if (fWaterAlpha > 1.0f)
								{
									fWaterAlpha = 1.0f;
								}
								else if (fWaterAlpha < 0.0f)
								{
									fWaterAlpha = 0.0f;
								}
							}
						}
					}
				}
				AddTilesetGroup(groupID, groupName, tWaterColor, fWaterAlpha);
			}
		}
		else if (field.CompareNoCase(STR_SET_TILESDEFTILE) == 0)
		{// tile definition detected
// Tile=Ldrt,Lordaeron Summer Dirt,144,100,53,WBF
			tileID = TS_STR_ID_UNDEFINED;
			tileName = TS_STR_NAME_UNDEFINED;
			tileColors = 0;
			tilePathingFlags = 0;
			values = line.Tokenize(";", j);
			//gets the tileID
			k = 0;
			field = values.Tokenize(",", k);
			field.TrimLeft();
			field.TrimRight();
			if (field.GetLength() == 4)
			{// the tileID is correctly defined
				tileID = field.GetBufferSetLength(4);
				field.ReleaseBuffer();
				//gets the name
				if (0 <= k)
				{
					tileName = values.Tokenize(",", k);
					tileName.TrimLeft();
					tileName.TrimRight();
					//gets the colors
//					if (values.GetFieldCount(',') < 5)
//					{// not enough colors, use default
//						tileColors = TS_COLOR_UNDEFINED;
//					}
//					else
//					{// gets the colors
					if (0 <= k)
					{
						field = values.Tokenize(",", k);
						field.TrimLeft();
						field.TrimRight();
						if ("" != field)
						{
							iR = atoi(field);
							if (iR < 0)
							{iR = 0;}
							else if (iR > 255)
							{iR = 255;}

							if (0 <= k)
							{
								field = values.Tokenize(",", k);
								field.TrimLeft();
								field.TrimRight();
							}
							iG = atoi(field);
							if (iG < 0)
							{iG = 0;}
							else if (iG > 255)
							{iG = 255;}

							if (0 <= k)
							{
								field = values.Tokenize(",", k);
								field.TrimLeft();
								field.TrimRight();
							}
							iB = atoi(field);
							if (iB < 0)
							{iB = 0;}
							else if (iB > 255)
							{iB = 255;}
							tileColors = iB*0x00010000 + iG*0x00000100 + iR;
							// gets the flags
							if (0 <= k)
							{
								field = values.Tokenize(",", k);
								field.TrimLeft();
								field.TrimRight();
								if ("" != field)
								{
									tilePathingFlags = PATH_NOWALK | PATH_NOFLY | PATH_NOBUILD;
									for (int l=0; l<field.GetLength(); l++)
									{
										if (STR_SET_TILEPATHWALKABLE == field[l])
										{
											tilePathingFlags &= ~PATH_NOWALK;
										}
										else if (STR_SET_TILEPATHBUILDABLE == field[l])
										{
											tilePathingFlags &= ~PATH_NOBUILD;
										}
										else if (STR_SET_TILEPATHFLYABLE == field[l])
										{
											tilePathingFlags &= ~PATH_NOFLY;
										}
									}
								}
							}
						}
						else
						{// not enough colors, use default
							tileColors = TS_COLOR_UNDEFINED;
						}
					}
				}
				AddTileDef(tileID, tileName, tileColors, tilePathingFlags);
				if (groupID != TG_CHAR_ID_UNDEFINED)
				{
					AddTileToTilesetGroup(groupID, tileID);
				}
			}
		}
		else if (field.CompareNoCase(STR_SET_TILESDEFCLIFF) == 0)
		{// cliff definition detected
// Cliff=CLdi,Lordaeron Summer Dirt Cliff,Ldrt
			cliffID = TC_STR_ID_UNDEFINED;
			cliffName = TC_STR_NAME_UNDEFINED;
			values = line.Tokenize(";", j);
			//gets the cliffID
			k = 0;
			field = values.Tokenize(",", k);
			field.TrimLeft();
			field.TrimRight();
			if (field.GetLength() == 4)
			{// the cliffID is correctly defined
				cliffID = field.GetBufferSetLength(4);
				field.ReleaseBuffer();
				//gets the name
				if (0 <= k)
				{
					cliffName = values.Tokenize(",", k);
					cliffName.TrimLeft();
					cliffName.TrimRight();
					//gets the associated tileset
					if (0 <= k)
					{
						field = values.Tokenize(",", k);
						field.TrimLeft();
						field.TrimRight();
						if (field.GetLength() == 4)
						{// correct associated tileset
							tileID = field.GetBufferSetLength(4);
							field.ReleaseBuffer();
						}
						else if (tileID != TS_STR_ID_UNDEFINED)
						{// can't use the last tileset used
							tileID = TS_STR_ID_UNDEFINED;
						}
					}
				}
				AddCliffTileDef(cliffID, cliffName, tileID);
				if (groupID != TG_CHAR_ID_UNDEFINED)
				{
					AddCliffTileToTilesetGroup(groupID, cliffID);
				}
			}
		}
	}
}

/*
AddTilesetGroup
***************
Add a tileset group to the tileset group list.

In:
 char groupID: group ID char (like 'L')
 CString groupName: group name (like "Lordaeron Summer")
 COLOREF tWaterColor: water color
 float fWaterAlpha: water transparency (0: invisible, 1: not transparent)

Out:
 none

Return:
 none

*/
void CW3ZMapEditSettings::AddTilesetGroup(char groupID, CString groupName, COLORREF tWaterColor, float fWaterAlpha)
{
	CPListedTilesetGroup pLTilesetGroup;

	if (!IsTilesetGroupInList(groupID))
	{
		pLTilesetGroup = new CListedTilesetGroup;
		pLTilesetGroup->tilesetGroup.groupID = groupID;
		pLTilesetGroup->tilesetGroup.groupName = groupName;
		pLTilesetGroup->tilesetGroup.tiles = "";
		pLTilesetGroup->tilesetGroup.cliffs = "";
		pLTilesetGroup->tilesetGroup.waterColor = tWaterColor;
		pLTilesetGroup->tilesetGroup.fWaterAlpha = fWaterAlpha;
		pLTilesetGroup->next = m_pTilesetGroupsHead;
		m_pTilesetGroupsHead = pLTilesetGroup;
	}
	else
	{
		CPListedTilesetGroup pLTilesetGroup;

		pLTilesetGroup = m_pTilesetGroupsHead;
		while ((pLTilesetGroup) && (pLTilesetGroup->tilesetGroup.groupID != groupID))
		{
			if (pLTilesetGroup->next == pLTilesetGroup)
			{// cyclic list bug
				pLTilesetGroup->next = NULL;
			}
			pLTilesetGroup = pLTilesetGroup->next;
		}
		pLTilesetGroup->tilesetGroup.groupName = groupName;
		pLTilesetGroup->tilesetGroup.tiles = "";
		pLTilesetGroup->tilesetGroup.cliffs = "";
		pLTilesetGroup->tilesetGroup.waterColor = tWaterColor;
		pLTilesetGroup->tilesetGroup.fWaterAlpha = fWaterAlpha;
	}
}


/*
IsTilesetGroupInList
********************
Says if yes or no a tileset group is defined in the tileset group list.

In:
  char groupID: the group ID char (like 'L')

Out:
 none

Return:
 BOOL: TRUE if the tileset group was found, FALSE otherwise

*/
BOOL CW3ZMapEditSettings::IsTilesetGroupInList(char groupID)
{
	CPListedTilesetGroup pLTilesetGroup;
	BOOL ret = FALSE;

	pLTilesetGroup = m_pTilesetGroupsHead;
	while (pLTilesetGroup && (pLTilesetGroup->tilesetGroup.groupID != groupID))
	{
		if (pLTilesetGroup == pLTilesetGroup->next)
		{ // cyclic list bug
			pLTilesetGroup->next = NULL;
			pLTilesetGroup = NULL;
		}
		else
		{
			pLTilesetGroup = pLTilesetGroup->next;
		}
	}
	if (pLTilesetGroup)
	{// tileset group found
		ret = TRUE;
	}
	
	return ret;
}


/*
GoFirstTilesetGroup
*******************
Initialize the tileset list listing with GetNextTilesetGroup.

In:
 none

Out:
 none

Return:
 none

*/
void CW3ZMapEditSettings::GoFirstTilesetGroup()
{
	m_pTilesetGroupsNext = m_pTilesetGroupsHead;
}


/*
GetNextTilesetGroup
*******************
Return the next tileset group of the list. This function is used to list the tileset groups.

In:
 none

Out:
 none

Return:
 CTilesetGroup: the next tileset group in the list

Note:
 The last tileset group has the group ID 'TG_CHAR_ID_UNDEFINED'.

*/
CTilesetGroup CW3ZMapEditSettings::GetNextTilesetGroup()
{
	CTilesetGroup tilesetGroup;

	if (m_pTilesetGroupsNext)
	{
		tilesetGroup = m_pTilesetGroupsNext->tilesetGroup;
		if (m_pTilesetGroupsNext->next == m_pTilesetGroupsNext)
		{
			m_pTilesetGroupsNext->next = NULL;
		}
		m_pTilesetGroupsNext = m_pTilesetGroupsNext->next;
	}
	return tilesetGroup;
}


/*
GetTilesetGroup
***************
Return the tileset group of ID gourpID. If none was found, the returned group has the ID 'TG_CHAR_ID_UNDEFINED'.

In:
 none

Out:
 none

Return:
 CTilesetGroup: the tileset group of ID groupID or a tileset group of ID 'TG_CHAR_ID_UNDEFINED'.

*/
CTilesetGroup CW3ZMapEditSettings::GetTilesetGroup(char groupID)
{
	CPListedTilesetGroup pLTilesetGroup;
	CTilesetGroup tilesetGroup;

	pLTilesetGroup = m_pTilesetGroupsHead;
	while ((pLTilesetGroup) && (pLTilesetGroup->tilesetGroup.groupID != groupID))
	{
		if (pLTilesetGroup->next == pLTilesetGroup)
		{// cyclic list bug
			pLTilesetGroup->next = NULL;
		}
		pLTilesetGroup = pLTilesetGroup->next;
	}
	if (pLTilesetGroup)
	{
		tilesetGroup = pLTilesetGroup->tilesetGroup;
	}
	return tilesetGroup;
}

void CW3ZMapEditSettings::AddCliffTileDef(CWar3StrID cliffTileID, CString cliffTileName, CWar3StrID associatedTileID)
{
	CPListedCliffTileset pLCliffTileset;

	if (!IsCliffTilesetInCliffTilesetList(cliffTileID) && (cliffTileName != ""))
	{
		pLCliffTileset = new CListedCliffTileset;
		pLCliffTileset->cliffTileset.CliffID = cliffTileID;
		pLCliffTileset->cliffTileset.cliffName = cliffTileName;
		pLCliffTileset->cliffTileset.associatedTileID = associatedTileID;
		pLCliffTileset->next = m_pCliffTilesetsHead;
		m_pCliffTilesetsHead = pLCliffTileset;
	}
}

void CW3ZMapEditSettings::ClearCliffTilesetList()
{
	CPListedCliffTileset pLCliffTileset;

	while (m_pCliffTilesetsHead)
	{
		pLCliffTileset = m_pCliffTilesetsHead;
		if (m_pCliffTilesetsHead == m_pCliffTilesetsHead->next)
		{ // cyclic list bug
			m_pCliffTilesetsHead = NULL;
		}
		else
		{
			m_pCliffTilesetsHead = m_pCliffTilesetsHead->next;
		}
		//free memory
		delete pLCliffTileset;
	}
}

void CW3ZMapEditSettings::ClearTilesetGroupList()
{
	CPListedTilesetGroup pLTilesetGroup;

	while (m_pTilesetGroupsHead)
	{
		pLTilesetGroup = m_pTilesetGroupsHead;
		if (m_pTilesetGroupsHead == m_pTilesetGroupsHead->next)
		{ // cyclic list bug
			m_pTilesetGroupsHead = NULL;
		}
		else
		{
			m_pTilesetGroupsHead = m_pTilesetGroupsHead->next;
		}
		//free memory
		delete pLTilesetGroup;
	}
}

void CW3ZMapEditSettings::ClearPluginList()
{
	CPListedPlugin pLPlugin;

	while (m_pLoadPluginList)
	{
		pLPlugin = m_pLoadPluginList;
		if (m_pLoadPluginList == m_pLoadPluginList->next)
		{ // cyclic list bug
			m_pLoadPluginList = NULL;
		}
		else
		{
			m_pLoadPluginList = m_pLoadPluginList->next;
		}
		//free memory
		delete pLPlugin;
	}

	while (m_pSavePluginList)
	{
		pLPlugin = m_pSavePluginList;
		if (m_pSavePluginList == m_pSavePluginList->next)
		{ // cyclic list bug
			m_pSavePluginList = NULL;
		}
		else
		{
			m_pSavePluginList = m_pSavePluginList->next;
		}
		//free memory
		delete pLPlugin;
	}

	while (m_pMenuPluginList)
	{
		pLPlugin = m_pMenuPluginList;
		if (m_pMenuPluginList == m_pMenuPluginList->next)
		{ // cyclic list bug
			m_pMenuPluginList = NULL;
		}
		else
		{
			m_pMenuPluginList = m_pMenuPluginList->next;
		}
		//free memory
		delete pLPlugin;
	}
}

BOOL CW3ZMapEditSettings::IsCliffTilesetInCliffTilesetList(CWar3StrID cliffTileID)
{
	CPListedCliffTileset pLCliffTileset;
	BOOL ret = FALSE;

	pLCliffTileset = m_pCliffTilesetsHead;
	while (pLCliffTileset && (pLCliffTileset->cliffTileset.CliffID != cliffTileID))
	{
		if (pLCliffTileset == pLCliffTileset->next)
		{ // cyclic list bug
			pLCliffTileset->next = NULL;
		}
		pLCliffTileset = pLCliffTileset->next;
	}
	if (pLCliffTileset)
	{// cliff tileset found
		ret = TRUE;
	}
	
	return ret;
}



void CW3ZMapEditSettings::AddTileToTilesetGroup(char groupID, CWar3StrID tileID)
{
	CPListedTilesetGroup pLTilesetGroup;

	pLTilesetGroup = m_pTilesetGroupsHead;
	while (pLTilesetGroup && (pLTilesetGroup->tilesetGroup.groupID != groupID))
	{
		if (pLTilesetGroup == pLTilesetGroup->next)
		{ // cyclic list bug
			pLTilesetGroup->next = NULL;
			pLTilesetGroup = NULL;
		}
		else
		{
			pLTilesetGroup = pLTilesetGroup->next;
		}
	}
	if (pLTilesetGroup)
	{// tileset group found
		pLTilesetGroup->tilesetGroup.AddTile(tileID);
	}
}

void CW3ZMapEditSettings::AddCliffTileToTilesetGroup(char groupID, CWar3StrID cliffID)
{
	CPListedTilesetGroup pLTilesetGroup;

	pLTilesetGroup = m_pTilesetGroupsHead;
	while (pLTilesetGroup && (pLTilesetGroup->tilesetGroup.groupID != groupID))
	{
		if (pLTilesetGroup == pLTilesetGroup->next)
		{ // cyclic list bug
			pLTilesetGroup->next = NULL;
			pLTilesetGroup = NULL;
		}
		else
		{
			pLTilesetGroup = pLTilesetGroup->next;
		}
	}
	if (pLTilesetGroup)
	{// tileset group found
		pLTilesetGroup->tilesetGroup.AddCliff(cliffID);
	}
}

void CW3ZMapEditSettings::LoadLayersProp(CString block)
{
	CString line, field, values;
	int i, j, k, layer;
	int iR,iG,iB;

	i = 0;
	while ((i < block.GetLength()) && (0 <= i))
	{
		line = block.Tokenize("\n", i);
		//clean the line
		j = line.Find(";");
		if (0 <= j)
		{
			line = line.Left(j);// remove the comments
		}
		line.TrimLeft();
		line.TrimRight();
		//gets the field type
		j = 0;
		field = line.Tokenize("=", j);
		field.TrimLeft();
		field.TrimRight();

		if (field.CompareNoCase(STR_SET_LAYER) == 0)
		{
			field = field.Mid(strlen(STR_SET_LAYER));
			layer = atoi(field.GetBuffer(field.GetLength()));
			field.ReleaseBuffer();
			if ((layer > 0) && (layer < 17))
			{
				if (0 <= j)
				{
					values = line.Tokenize("=", j);
					values.TrimLeft();
					values.TrimRight();
					k = 0;
					field = values.Tokenize(",", k);
					field.TrimLeft();
					field.TrimRight();
					if ("" != field)
					{
						iR = atoi(field);
						if (iR < 0)
						{iR = 0;}
						else if (iR > 255)
						{iR = 255;}

						if (0 <= k)
						{
							field = values.Tokenize(",", k);
							field.TrimLeft();
							field.TrimRight();
						}
						iG = atoi(field);
						if (iG < 0)
						{iG = 0;}
						else if (iG > 255)
						{iG = 255;}

						if (0 <= k)
						{
							field = values.Tokenize(",", j);
							field.TrimLeft();
							field.TrimRight();
						}
						iB = atoi(field);
						if (iB < 0)
						{iB = 0;}
						else if (iB > 255)
						{iB = 255;}
						LayersColors[layer] = iB*0x00010000 + iG*0x00000100 + iR;
					}
				}
			}
		}
	}
}

void CW3ZMapEditSettings::LoadPathingProp(CString block)
{
	CString line, field, values, subfields;
	int i, j, k, pathmode;
	int iR,iG,iB;

	i = 0;
	while ((i < block.GetLength()) && (0 <= i))
	{
		line = block.Tokenize("\n", i);
		//clean the line
		j = line.Find(";");
		if (0 <= j)
		{
			line = line.Left(j);// remove the comments
		}
		line.TrimLeft();
		line.TrimRight();
		//gets the field type
		j = 0;
		field = line.Tokenize("=", j);
		field.TrimLeft();
		field.TrimRight();
		if (0 <= j)
		{
			values = line.Tokenize(";", j);
			values.TrimLeft();
			values.TrimRight();
			k = 0;
			subfields = values.Tokenize(",", k);
			subfields.TrimLeft();
			subfields.TrimRight();
			if ("" != subfields)
			{
				iR = atoi(subfields);
				if (iR < 0)
				{iR = 0;}
				else if (iR > 255)
				{iR = 255;}

				if (0 <= k)
				{
					subfields = values.Tokenize(",", k);
					subfields.TrimLeft();
					subfields.TrimRight();
				}
				iG = atoi(subfields);
				if (iG < 0)
				{iG = 0;}
				else if (iG > 255)
				{iG = 255;}

				if (0 <= k)
				{
					subfields = values.Tokenize(",", k);
					subfields.TrimLeft();
					subfields.TrimRight();
				}
				iB = atoi(subfields);
				if (iB < 0)
				{iB = 0;}
				else if (iB > 255)
				{iB = 255;}

				if (0 <= k)
				{
					subfields = values.Tokenize(",", k);
					subfields.TrimLeft();
					subfields.TrimRight();
					pathmode = atoi(subfields);
				}
				else
				{
					pathmode = 0; // use 0 if no pathmode set
				}

				if (field.CompareNoCase(STR_SET_PATH_NORMAL) == 0)
				{
					pathNormal.clrRed = (BYTE)iR;
					pathNormal.clrGreen = (BYTE)iG;
					pathNormal.clrBlue = (BYTE)iB;
					pathNormal.mode = pathmode;
				}
				else if (field.CompareNoCase(STR_SET_PATH_NOWALK) == 0)
				{
					pathNoWalk.clrRed = (BYTE)iR;
					pathNoWalk.clrGreen = (BYTE)iG;
					pathNoWalk.clrBlue = (BYTE)iB;
					pathNoWalk.mode = pathmode;
				}
				else if (field.CompareNoCase(STR_SET_PATH_NOBUILD) == 0)
				{
					if (field.CompareNoCase(STR_SET_PATH_NOBWF) == 0)
					{
						pathNoBuildNoWalkNoFly.clrRed = (BYTE)iR;
						pathNoBuildNoWalkNoFly.clrGreen = (BYTE)iG;
						pathNoBuildNoWalkNoFly.clrBlue = (BYTE)iB;
						pathNoBuildNoWalkNoFly.mode = pathmode;
					}
					else if (field.CompareNoCase(STR_SET_PATH_NOBWW) == 0)
					{
						pathNoBuildNoWalkNoWater.clrRed = (BYTE)iR;
						pathNoBuildNoWalkNoWater.clrGreen = (BYTE)iG;
						pathNoBuildNoWalkNoWater.clrBlue = (BYTE)iB;
						pathNoBuildNoWalkNoWater.mode = pathmode;
					}
					else if (field.CompareNoCase(STR_SET_PATH_NOBW) == 0)
					{
						pathNoBuildNoWalk.clrRed = (BYTE)iR;
						pathNoBuildNoWalk.clrGreen = (BYTE)iG;
						pathNoBuildNoWalk.clrBlue = (BYTE)iB;
						pathNoBuildNoWalk.mode = pathmode;
					}
					else
					{
						pathNoBuild.clrRed = (BYTE)iR;
						pathNoBuild.clrGreen = (BYTE)iG;
						pathNoBuild.clrBlue = (BYTE)iB;
						pathNoBuild.mode = pathmode;
					}
				}
				else if (field.CompareNoCase(STR_SET_PATH_NOFLY) == 0)
				{
					pathNoFly.clrRed = (BYTE)iR;
					pathNoFly.clrGreen = (BYTE)iG;
					pathNoFly.clrBlue = (BYTE)iB;
					pathNoFly.mode = pathmode;
				}
				else if (field.CompareNoCase(STR_SET_PATH_BLIGHT) == 0)
				{
					pathBlight.clrRed = (BYTE)iR;
					pathBlight.clrGreen = (BYTE)iG;
					pathBlight.clrBlue = (BYTE)iB;
					pathBlight.mode = pathmode;
				}
				else if (field.CompareNoCase(STR_SET_PATH_NOWATER) == 0)
				{
					pathNoWater.clrRed = (BYTE)iR;
					pathNoWater.clrGreen = (BYTE)iG;
					pathNoWater.clrBlue = (BYTE)iB;
					pathNoWater.mode = pathmode;
				}
				else if (field.CompareNoCase(STR_SET_PATH_UNKNOWN) == 0)
				{
					pathUnknown.clrRed = (BYTE)iR;
					pathUnknown.clrGreen = (BYTE)iG;
					pathUnknown.clrBlue = (BYTE)iB;
					pathUnknown.mode = pathmode;
				}
				else if (field.CompareNoCase(STR_SET_PATH_UNKN01) == 0)
				{
					pathUnkn01.clrRed = (BYTE)iR;
					pathUnkn01.clrGreen = (BYTE)iG;
					pathUnkn01.clrBlue = (BYTE)iB;
					pathUnkn01.mode = pathmode;
				}
				else if (field.CompareNoCase(STR_SET_PATH_UNKN02) == 0)
				{
					pathUnkn02.clrRed = (BYTE)iR;
					pathUnkn02.clrGreen = (BYTE)iG;
					pathUnkn02.clrBlue = (BYTE)iB;
					pathUnkn02.mode = pathmode;
				}
				else if (field.CompareNoCase(STR_SET_PATH_NOBHDELTA) == 0)
				{
					pathNoBuildHeightDelta = (float)atof(values);
				}
				else if (field.CompareNoCase(STR_SET_PATH_NOWHDELTA) == 0)
				{
					pathNoWalkHeightDelta = (float)atof(values);
				}
			}
		}
	}

	// updates the path edit bar
    CW3ZMapEditApp(* pTheApp) = (CW3ZMapEditApp *)AfxGetApp();
	if (pTheApp)
	{
		CMainFrame *pMainFrame = pTheApp->GetMainFrame();
		if (pMainFrame)
		{
			pMainFrame->UpdatePathEditBarColors();
		}
	}
}

void CW3ZMapEditSettings::LoadTilesProp(CString block)
{
	CString line, field, values, subfields;
	int i, j, k;
	int iR,iG,iB;
	float fAlpha;

	i = 0;
	while ((i < block.GetLength()) && (0 <= i))
	{
		line = block.Tokenize("\n", i);
		//clean the line
		j = line.Find(";");
		if (0 <= j)
		{
			line = line.Left(j);// remove the comments
		}
		line.TrimLeft();
		line.TrimRight();
		//gets the field type
		j = 0;
		field = line.Tokenize("=", j);
		field.TrimLeft();
		field.TrimRight();
		if (0 <= j)
		{
			values = line.Tokenize(";", j);
			values.TrimLeft();
			values.TrimRight();
			k = 0;
			subfields = values.Tokenize(",", k);
			subfields.TrimLeft();
			subfields.TrimRight();
			if ("" != subfields)
			{
				iR = atoi(subfields);
				if (iR < 0)
				{iR = 0;}
				else if (iR > 255)
				{iR = 255;}

				if (0 <= k)
				{
					subfields = values.Tokenize(",", k);
					subfields.TrimLeft();
					subfields.TrimRight();
				}
				iG = atoi(subfields);
				if (iG < 0)
				{iG = 0;}
				else if (iG > 255)
				{iG = 255;}

				if (0 <= k)
				{
					subfields = values.Tokenize(",", k);
					subfields.TrimLeft();
					subfields.TrimRight();
				}
				iB = atoi(subfields);
				if (iB < 0)
				{iB = 0;}
				else if (iB > 255)
				{iB = 255;}

				if (0 <= k)
				{
					subfields = values.Tokenize(",", k);
					subfields.TrimLeft();
					subfields.TrimRight();
					fAlpha = (float)atof(subfields);
				}
				else
				{
					fAlpha = 1.0;
				}

				if (field.CompareNoCase(STR_SET_RAMP) == 0)
				{
					tRamp.tMaskColor = iR | (iG << 8) | (iB << 16);
					tRamp.fAlpha = fAlpha;
				}
				else if (field.CompareNoCase(STR_SET_BLIGHT) == 0)
				{
					tBlight.tMaskColor = iR | (iG << 8) | (iB << 16);
					tBlight.fAlpha = fAlpha;
				}
				else if (field.CompareNoCase(STR_SET_BOUNDARY) == 0)
				{
					tBoundary.tMaskColor = iR | (iG << 8) | (iB << 16);
					tBoundary.fAlpha = fAlpha;
				}
				else if (field.CompareNoCase(STR_SET_LEVELFACTOR) == 0)
				{
					fLevelFactor = (float)atof(values);
				}
			}
		}
	}
}

void CW3ZMapEditSettings::LoadLPlugins(CString block)
{
	CString line, strPluginName, strFileMask, strPluginPath, strParameters;
	CPListedPlugin pLastLPlugin;
	int i, j;

	// gets a pointer to the last plugin
	pLastLPlugin = m_pLoadPluginList;
	if (NULL != pLastLPlugin)
	{
		while(NULL != pLastLPlugin->next)
		{
			pLastLPlugin = pLastLPlugin->next;
		}
	}
	i = 0;
	while ((i < block.GetLength()) && (0 <= i))
	{
		line = block.Tokenize("\n", i);
		//clean the line
		line.TrimLeft();
		line.TrimRight();
		//gets plugin name
		j = 0;
		strPluginName = line.Tokenize("=", j);
		strPluginName.TrimLeft();
		strPluginName.TrimRight();
		// gets only what is after "="
		line = line.Mid(j);
		line.TrimLeft();
		j = 0;
		if ('"' == line[0])
		{
			j++;
			strFileMask = line.Tokenize("\"", j);
			strFileMask.TrimLeft();
			strFileMask.TrimRight();
			line.Tokenize(",", j);
		}
		else
		{
			strFileMask = line.Tokenize(",", j);
			strFileMask.TrimLeft();
			strFileMask.TrimRight();
		}


		line = line.Mid(j);
		line.TrimLeft();
		j = 0;
		if ('"' == line[0])
		{
			j++;
			strPluginPath = line.Tokenize("\"", j);
			strPluginPath.TrimLeft();
			strPluginPath.TrimRight();
			line.Tokenize(",", j);
		}
		else
		{
			strPluginPath = line.Tokenize(",", j);
			strPluginPath.TrimLeft();
			strPluginPath.TrimRight();
		}


		line = line.Mid(j);
		line.TrimLeft();
		j = 0;
		if ('"' == line[0])
		{
			j++;
			strParameters = line.Tokenize("\"", j);
			strParameters.TrimLeft();
			strParameters.TrimRight();
			line.Tokenize(",", j);
		}
		else
		{
			strParameters = line.Tokenize(",", j);
			strParameters.TrimLeft();
			strParameters.TrimRight();
		}
		if (("" != strPluginName) && ("" != strFileMask) && ("" != strPluginPath))
		{
			CPListedPlugin pLPlugin = new CListedPlugin();
			pLPlugin->strPluginName = strPluginName;
			pLPlugin->strFileMask = strFileMask;
			pLPlugin->strCommandLine = strPluginPath;
			pLPlugin->strArguments = strParameters;
			pLPlugin->next = NULL;
			if (NULL != pLastLPlugin)
			{
				// not the first one
				pLastLPlugin->next = pLPlugin;
			}
			else
			{
				// first one
				m_pLoadPluginList = pLPlugin;
			}
			pLastLPlugin = pLPlugin;
		}
	}
}

void CW3ZMapEditSettings::LoadSPlugins(CString block)
{
	CString line, strPluginName, strFileMask, strPluginPath, strParameters;
	CPListedPlugin pLastSPlugin;
	int i, j;

	// gets a pointer to the last plugin
	pLastSPlugin = m_pSavePluginList;
	if (NULL != pLastSPlugin)
	{
		while(NULL != pLastSPlugin->next)
		{
			pLastSPlugin = pLastSPlugin->next;
		}
	}

	i = 0;
	while ((i < block.GetLength()) && (0 <= i))
	{
		line = block.Tokenize("\n", i);
		//clean the line
		line.TrimLeft();
		line.TrimRight();
		//gets plugin name
		j = 0;
		strPluginName = line.Tokenize("=", j);
		strPluginName.TrimLeft();
		strPluginName.TrimRight();
		// gets only what is after "="
		line = line.Mid(j);
		line.TrimLeft();
		j = 0;
		if ('"' == line[0])
		{
			j++;
			strFileMask = line.Tokenize("\"", j);
			strFileMask.TrimLeft();
			strFileMask.TrimRight();
			j--;
			line.Tokenize(",", j);
		}
		else
		{
			strFileMask = line.Tokenize(",", j);
			strFileMask.TrimLeft();
			strFileMask.TrimRight();
		}


		line = line.Mid(j);
		line.TrimLeft();
		j = 0;
		if ('"' == line[0])
		{
			j++;
			strPluginPath = line.Tokenize("\"", j);
			strPluginPath.TrimLeft();
			strPluginPath.TrimRight();
			j--;
			line.Tokenize(",", j);
		}
		else
		{
			strPluginPath = line.Tokenize(",", j);
			strPluginPath.TrimLeft();
			strPluginPath.TrimRight();
		}


		line = line.Mid(j);
		line.TrimLeft();
		j = 0;
		if ('"' == line[0])
		{
			j++;
			strParameters = line.Tokenize("\"", j);
			strParameters.TrimLeft();
			strParameters.TrimRight();
			j--;
			line.Tokenize(",", j);
		}
		else
		{
			strParameters = line.Tokenize(",", j);
			strParameters.TrimLeft();
			strParameters.TrimRight();
		}
		if (("" != strPluginName) && ("" != strFileMask) && ("" != strPluginPath))
		{
			CPListedPlugin pLPlugin = new CListedPlugin();
			pLPlugin->strPluginName = strPluginName;
			pLPlugin->strFileMask = strFileMask;
			pLPlugin->strCommandLine = strPluginPath;
			pLPlugin->strArguments = strParameters;
			pLPlugin->next = NULL;
			if (NULL != pLastSPlugin)
			{
				// not the first one
				pLastSPlugin->next = pLPlugin;
			}
			else
			{
				// first one
				m_pSavePluginList = pLPlugin;
			}
			pLastSPlugin = pLPlugin;
		}
	}
}

void CW3ZMapEditSettings::LoadMPlugins(CString block)
{
	CString line, strPluginName, strFileMask, strPluginPath, strParameters;
	CPListedPlugin pLastMPlugin;
	int i, j;

	// gets a pointer to the last plugin
	pLastMPlugin = m_pMenuPluginList;
	if (NULL != pLastMPlugin)
	{
		while(NULL != pLastMPlugin->next)
		{
			pLastMPlugin = pLastMPlugin->next;
		}
	}

	i = 0;
	while ((i < block.GetLength()) && (0 <= i))
	{
		line = block.Tokenize("\n", i);
		//clean the line
		line.TrimLeft();
		line.TrimRight();
		//gets plugin name
		j = 0;
		strPluginName = line.Tokenize("=", j);
		strPluginName.TrimLeft();
		strPluginName.TrimRight();
		// gets only what is after "="
		line = line.Mid(j);
		line.TrimLeft();
		j = 0;
		if ('"' == line[0])
		{
			j++;
			strFileMask = line.Tokenize("\"", j);
			strFileMask.TrimLeft();
			strFileMask.TrimRight();
			line.Tokenize(",", j);
		}
		else
		{
			strFileMask = line.Tokenize(",", j);
			strFileMask.TrimLeft();
			strFileMask.TrimRight();
		}


		line = line.Mid(j);
		line.TrimLeft();
		j = 0;
		if ('"' == line[0])
		{
			j++;
			strPluginPath = line.Tokenize("\"", j);
			strPluginPath.TrimLeft();
			strPluginPath.TrimRight();
			line.Tokenize(",", j);
		}
		else
		{
			strPluginPath = line.Tokenize(",", j);
			strPluginPath.TrimLeft();
			strPluginPath.TrimRight();
		}


		line = line.Mid(j);
		line.TrimLeft();
		j = 0;
		if ('"' == line[0])
		{
			j++;
			strParameters = line.Tokenize("\"", j);
			strParameters.TrimLeft();
			strParameters.TrimRight();
			line.Tokenize(",", j);
		}
		else
		{
			strParameters = line.Tokenize(",", j);
			strParameters.TrimLeft();
			strParameters.TrimRight();
		}
		if (("" != strPluginName) && ("" != strFileMask) && ("" != strPluginPath))
		{
			CPListedPlugin pLPlugin = new CListedPlugin();
			pLPlugin->strPluginName = strPluginName;
			pLPlugin->strFileMask = strFileMask;
			pLPlugin->strCommandLine = strPluginPath;
			pLPlugin->strArguments = strParameters;
			pLPlugin->next = NULL;
			if (NULL != pLastMPlugin)
			{
				// not the first one
				pLastMPlugin->next = pLPlugin;
			}
			else
			{
				// first one
				m_pMenuPluginList = pLPlugin;
			}
			pLastMPlugin = pLPlugin;
		}
	}
}

void CW3ZMapEditSettings::LoadMisc(CString block)
{

}

void CW3ZMapEditSettings::LoadEditModes(CString block)
{

}

void CW3ZMapEditSettings::LoadConvertion(CString block)
{
	CString line, field, values, subfields;
	int i, j, k;
	int iWar2CurrentTileIndex = 0;
	CWar3StrID strTileset, strCliffset, strDoodad;
	int iLayer, iFlags;
	int iGroundHeight, iWaterHeight;
	int iGroundType, iCliffType;
	int iSCTerrainIndex, iSCGroundIndex;

	i = 0;
	while ((i < block.GetLength()) && (0 <= i))
	{
		line = block.Tokenize("\n", i);
		//clean the line
		j = line.Find(";");
		if (0 <= j)
		{
			line = line.Left(j);// remove the comments
		}
		line.TrimLeft();
		line.TrimRight();
		//gets the field type
		j = 0;
		field = line.Tokenize("=", j);
		field.TrimLeft();
		field.TrimRight();

		values = line.Tokenize(";", j);
		values.TrimLeft();
		values.TrimRight();
		k = 0;
		subfields = values.Tokenize(",", k);
		subfields.TrimLeft();
		subfields.TrimRight();
		// ground tileset
		strTileset = subfields.Mid(0, WAR3STRID_MAXCHAR);
		//gets convertion tileset
		if (0 <= k)
		{
			// old_ground_type=<ground_id>,<cliff_id>,<layer>,[ground_height],[water_height],[flags],[doodad]
			// W2FLightWater=Ldrt,Cldi,1,8192,8192,0x0040
			subfields = values.Tokenize(",", k);
			subfields.TrimLeft();
			subfields.TrimRight();
			// cliff tileset
			strCliffset = subfields.Mid(0, WAR3STRID_MAXCHAR);

			// layer
			if (0 <= k)
			{
				subfields = values.Tokenize(",", k);
				subfields.TrimLeft();
				subfields.TrimRight();
				iLayer = atoi(subfields);
			}
			else
			{
				iLayer = TILE_LAYER_LEVEL_ZERO;
			}
			// ground height
			if (0 <= k)
			{
				subfields = values.Tokenize(",", k);
				subfields.TrimLeft();
				subfields.TrimRight();
				iGroundHeight = atoi(subfields);
			}
			else
			{
				iGroundHeight = TILE_HEIGHT_LEVEL_ZERO;
			}
			// water height
			if (0 <= k)
			{
				subfields = values.Tokenize(",", k);
				subfields.TrimLeft();
				subfields.TrimRight();
				iWaterHeight = atoi(subfields);
			}
			else
			{
				iWaterHeight = TILE_HEIGHT_LEVEL_ZERO;
			}
			// flags
			iFlags = 0;
			if (0 <= k)
			{
				subfields = values.Tokenize(",", k);
				subfields.TrimLeft();
				subfields.TrimRight();
				subfields = subfields.MakeUpper();
				for (int h=0; h<subfields.GetLength(); h++)
				{
					if (('A' <= subfields[h]) && ('F' >= subfields[h]))
					{
						iFlags = iFlags*0x10 + 0x0A + (subfields[h] - 'A');
					}
					else if (('0' <= subfields[h]) && ('9' >= subfields[h]))
					{
						iFlags = iFlags*0x10 + (subfields[h] - '0');
					}
					else
					{
						iFlags = 0;
					}
				}
			}
			// doodad
			if (0 <= k)
			{
				subfields = values.Tokenize(",", k);
				subfields.TrimLeft();
				subfields.TrimRight();
				strDoodad = subfields.Mid(0, WAR3STRID_MAXCHAR);
			}
			else
			{
				strDoodad = "";
			}
		}

		iSCTerrainIndex = -1;
		iSCGroundIndex = -1;

		// War2...
		if (field.CompareNoCase(STR_SET_W2FOREST) == 0)
		{
			iWar2CurrentTileIndex = INDEX_SET_W2FOREST;
			// War2 forest, get main tileset if one
			if (values.GetLength() > 0)
			{
				m_acWar2MainTileset[INDEX_SET_W2FOREST] = values[0];
			}
			m_astrWar2Tilesets[iWar2CurrentTileIndex] = "";
			m_astrWar2Cliffsets[iWar2CurrentTileIndex] = "";
		}
		else if (field.CompareNoCase(STR_SET_W2WINTER) == 0)
		{
			iWar2CurrentTileIndex = INDEX_SET_W2WINTER;
			// War2 winter, get main tileset if one
			if (values.GetLength() > 0)
			{
				m_acWar2MainTileset[INDEX_SET_W2WINTER] = values[0];
			}
			m_astrWar2Tilesets[iWar2CurrentTileIndex] = "";
			m_astrWar2Cliffsets[iWar2CurrentTileIndex] = "";
		}
		else if (field.CompareNoCase(STR_SET_W2WASTELAND) == 0)
		{
			iWar2CurrentTileIndex = INDEX_SET_W2WASTELAND;
			// War2 wasteland, get main tileset if one
			if (values.GetLength() > 0)
			{
				m_acWar2MainTileset[INDEX_SET_W2WASTELAND] = values[0];
			}
			m_astrWar2Tilesets[iWar2CurrentTileIndex] = "";
			m_astrWar2Cliffsets[iWar2CurrentTileIndex] = "";
		}
		else if (field.CompareNoCase(STR_SET_W2SWAMP) == 0)
		{
			iWar2CurrentTileIndex = INDEX_SET_W2SWAMP;
			// War2 swamp, get main tileset if one
			if (values.GetLength() > 0)
			{
				m_acWar2MainTileset[INDEX_SET_W2SWAMP] = values[0];
			}
			m_astrWar2Tilesets[iWar2CurrentTileIndex] = "";
			m_astrWar2Cliffsets[iWar2CurrentTileIndex] = "";
		}
		else if ((field.CompareNoCase(STR_SET_W2FLIGHTWATER) == 0)
		         || (field.CompareNoCase(STR_SET_W2ILIGHTWATER) == 0)
				 || (field.CompareNoCase(STR_SET_W2WLIGHTWATER) == 0)
				 || (field.CompareNoCase(STR_SET_W2SLIGHTWATER) == 0))
		{
			iGroundType = GetTileIndex(strTileset, m_astrWar2Tilesets[iWar2CurrentTileIndex]);
			if (iGroundType < 0)
			{
				// new tileset to add
				m_astrWar2Tilesets[iWar2CurrentTileIndex] = m_astrWar2Tilesets[iWar2CurrentTileIndex] + strTileset.GetStrID();
				iGroundType = GetTileIndex(strTileset, m_astrWar2Tilesets[iWar2CurrentTileIndex]);
			}
			iCliffType = GetTileIndex(strCliffset, m_astrWar2Cliffsets[iWar2CurrentTileIndex]);
			if (iCliffType < 0)
			{
				// new cliffset to add
				m_astrWar2Cliffsets[iWar2CurrentTileIndex] = m_astrWar2Cliffsets[iWar2CurrentTileIndex] + strCliffset.GetStrID();
				iCliffType = GetTileIndex(strCliffset, m_astrWar2Cliffsets[iWar2CurrentTileIndex]);
			}

			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_LIGHTWATER_INDEX].groundtype = iGroundType;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_LIGHTWATER_INDEX].clifftype = iCliffType;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_LIGHTWATER_INDEX].layer = iLayer;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_LIGHTWATER_INDEX].height = iGroundHeight;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_LIGHTWATER_INDEX].waterlevel = iWaterHeight;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_LIGHTWATER_INDEX].flags = iFlags;
			m_astrWar2Doodads[iWar2CurrentTileIndex][PUD_LIGHTWATER_INDEX] = strDoodad;
		}
		else if ((field.CompareNoCase(STR_SET_W2FDARKWATER) == 0)
		         || (field.CompareNoCase(STR_SET_W2IDARKWATER) == 0)
				 || (field.CompareNoCase(STR_SET_W2WDARKWATER) == 0)
				 || (field.CompareNoCase(STR_SET_W2SDARKWATER) == 0))
		{
			iGroundType = GetTileIndex(strTileset, m_astrWar2Tilesets[iWar2CurrentTileIndex]);
			if (iGroundType < 0)
			{
				// new tileset to add
				m_astrWar2Tilesets[iWar2CurrentTileIndex] = m_astrWar2Tilesets[iWar2CurrentTileIndex] + strTileset.GetStrID();
				iGroundType = GetTileIndex(strTileset, m_astrWar2Tilesets[iWar2CurrentTileIndex]);
			}
			iCliffType = GetTileIndex(strCliffset, m_astrWar2Cliffsets[iWar2CurrentTileIndex]);
			if (iCliffType < 0)
			{
				// new cliffset to add
				m_astrWar2Cliffsets[iWar2CurrentTileIndex] = m_astrWar2Cliffsets[iWar2CurrentTileIndex] + strCliffset.GetStrID();
				iCliffType = GetTileIndex(strCliffset, m_astrWar2Cliffsets[iWar2CurrentTileIndex]);
			}

			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_DARKWATER_INDEX].groundtype = iGroundType;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_DARKWATER_INDEX].clifftype = iCliffType;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_DARKWATER_INDEX].layer = iLayer;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_DARKWATER_INDEX].height = iGroundHeight;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_DARKWATER_INDEX].waterlevel = iWaterHeight;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_DARKWATER_INDEX].flags = iFlags;
			m_astrWar2Doodads[iWar2CurrentTileIndex][PUD_DARKWATER_INDEX] = strDoodad;
		}
		else if ((field.CompareNoCase(STR_SET_W2FLIGHTDIRT) == 0)
		         || (field.CompareNoCase(STR_SET_W2ILIGHTDIRT) == 0)
				 || (field.CompareNoCase(STR_SET_W2WLIGHTDIRT) == 0)
				 || (field.CompareNoCase(STR_SET_W2SLIGHTDIRT) == 0))
		{
			iGroundType = GetTileIndex(strTileset, m_astrWar2Tilesets[iWar2CurrentTileIndex]);
			if (iGroundType < 0)
			{
				// new tileset to add
				m_astrWar2Tilesets[iWar2CurrentTileIndex] = m_astrWar2Tilesets[iWar2CurrentTileIndex] + strTileset.GetStrID();
				iGroundType = GetTileIndex(strTileset, m_astrWar2Tilesets[iWar2CurrentTileIndex]);
			}
			iCliffType = GetTileIndex(strCliffset, m_astrWar2Cliffsets[iWar2CurrentTileIndex]);
			if (iCliffType < 0)
			{
				// new cliffset to add
				m_astrWar2Cliffsets[iWar2CurrentTileIndex] = m_astrWar2Cliffsets[iWar2CurrentTileIndex] + strCliffset.GetStrID();
				iCliffType = GetTileIndex(strCliffset, m_astrWar2Cliffsets[iWar2CurrentTileIndex]);
			}

			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_LIGHTDIRT_INDEX].groundtype = iGroundType;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_LIGHTDIRT_INDEX].clifftype = iCliffType;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_LIGHTDIRT_INDEX].layer = iLayer;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_LIGHTDIRT_INDEX].height = iGroundHeight;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_LIGHTDIRT_INDEX].waterlevel = iWaterHeight;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_LIGHTDIRT_INDEX].flags = iFlags;
			m_astrWar2Doodads[iWar2CurrentTileIndex][PUD_LIGHTDIRT_INDEX] = strDoodad;
		}
		else if ((field.CompareNoCase(STR_SET_W2FDARKDIRT) == 0)
		         || (field.CompareNoCase(STR_SET_W2IDARKDIRT) == 0)
				 || (field.CompareNoCase(STR_SET_W2WDARKDIRT) == 0)
				 || (field.CompareNoCase(STR_SET_W2SDARKDIRT) == 0))
		{
			iGroundType = GetTileIndex(strTileset, m_astrWar2Tilesets[iWar2CurrentTileIndex]);
			if (iGroundType < 0)
			{
				// new tileset to add
				m_astrWar2Tilesets[iWar2CurrentTileIndex] = m_astrWar2Tilesets[iWar2CurrentTileIndex] + strTileset.GetStrID();
				iGroundType = GetTileIndex(strTileset, m_astrWar2Tilesets[iWar2CurrentTileIndex]);
			}
			iCliffType = GetTileIndex(strCliffset, m_astrWar2Cliffsets[iWar2CurrentTileIndex]);
			if (iCliffType < 0)
			{
				// new cliffset to add
				m_astrWar2Cliffsets[iWar2CurrentTileIndex] = m_astrWar2Cliffsets[iWar2CurrentTileIndex] + strCliffset.GetStrID();
				iCliffType = GetTileIndex(strCliffset, m_astrWar2Cliffsets[iWar2CurrentTileIndex]);
			}

			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_DARKDIRT_INDEX].groundtype = iGroundType;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_DARKDIRT_INDEX].clifftype = iCliffType;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_DARKDIRT_INDEX].layer = iLayer;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_DARKDIRT_INDEX].height = iGroundHeight;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_DARKDIRT_INDEX].waterlevel = iWaterHeight;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_DARKDIRT_INDEX].flags = iFlags;
			m_astrWar2Doodads[iWar2CurrentTileIndex][PUD_DARKDIRT_INDEX] = strDoodad;
		}
		else if ((field.CompareNoCase(STR_SET_W2FLIGHTGROUND) == 0)
		         || (field.CompareNoCase(STR_SET_W2ILIGHTGROUND) == 0)
				 || (field.CompareNoCase(STR_SET_W2WLIGHTGROUND) == 0)
				 || (field.CompareNoCase(STR_SET_W2SLIGHTGROUND) == 0))
		{
			iGroundType = GetTileIndex(strTileset, m_astrWar2Tilesets[iWar2CurrentTileIndex]);
			if (iGroundType < 0)
			{
				// new tileset to add
				m_astrWar2Tilesets[iWar2CurrentTileIndex] = m_astrWar2Tilesets[iWar2CurrentTileIndex] + strTileset.GetStrID();
				iGroundType = GetTileIndex(strTileset, m_astrWar2Tilesets[iWar2CurrentTileIndex]);
			}
			iCliffType = GetTileIndex(strCliffset, m_astrWar2Cliffsets[iWar2CurrentTileIndex]);
			if (iCliffType < 0)
			{
				// new cliffset to add
				m_astrWar2Cliffsets[iWar2CurrentTileIndex] = m_astrWar2Cliffsets[iWar2CurrentTileIndex] + strCliffset.GetStrID();
				iCliffType = GetTileIndex(strCliffset, m_astrWar2Cliffsets[iWar2CurrentTileIndex]);
			}

			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_LIGHTGROUND_INDEX].groundtype = iGroundType;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_LIGHTGROUND_INDEX].clifftype = iCliffType;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_LIGHTGROUND_INDEX].layer = iLayer;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_LIGHTGROUND_INDEX].height = iGroundHeight;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_LIGHTGROUND_INDEX].waterlevel = iWaterHeight;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_LIGHTGROUND_INDEX].flags = iFlags;
			m_astrWar2Doodads[iWar2CurrentTileIndex][PUD_LIGHTGROUND_INDEX] = strDoodad;
		}
		else if ((field.CompareNoCase(STR_SET_W2FDARKGROUND) == 0)
		         || (field.CompareNoCase(STR_SET_W2IDARKGROUND) == 0)
				 || (field.CompareNoCase(STR_SET_W2WDARKGROUND) == 0)
				 || (field.CompareNoCase(STR_SET_W2SDARKGROUND) == 0))
		{
			iGroundType = GetTileIndex(strTileset, m_astrWar2Tilesets[iWar2CurrentTileIndex]);
			if (iGroundType < 0)
			{
				// new tileset to add
				m_astrWar2Tilesets[iWar2CurrentTileIndex] = m_astrWar2Tilesets[iWar2CurrentTileIndex] + strTileset.GetStrID();
				iGroundType = GetTileIndex(strTileset, m_astrWar2Tilesets[iWar2CurrentTileIndex]);
			}
			iCliffType = GetTileIndex(strCliffset, m_astrWar2Cliffsets[iWar2CurrentTileIndex]);
			if (iCliffType < 0)
			{
				// new cliffset to add
				m_astrWar2Cliffsets[iWar2CurrentTileIndex] = m_astrWar2Cliffsets[iWar2CurrentTileIndex] + strCliffset.GetStrID();
				iCliffType = GetTileIndex(strCliffset, m_astrWar2Cliffsets[iWar2CurrentTileIndex]);
			}

			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_DARKGROUND_INDEX].groundtype = iGroundType;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_DARKGROUND_INDEX].clifftype = iCliffType;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_DARKGROUND_INDEX].layer = iLayer;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_DARKGROUND_INDEX].height = iGroundHeight;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_DARKGROUND_INDEX].waterlevel = iWaterHeight;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_DARKGROUND_INDEX].flags = iFlags;
			m_astrWar2Doodads[iWar2CurrentTileIndex][PUD_DARKGROUND_INDEX] = strDoodad;
		}
		else if ((field.CompareNoCase(STR_SET_W2FFOREST) == 0)
		         || (field.CompareNoCase(STR_SET_W2IFOREST) == 0)
				 || (field.CompareNoCase(STR_SET_W2WFOREST) == 0)
				 || (field.CompareNoCase(STR_SET_W2SFOREST) == 0))
		{
			iGroundType = GetTileIndex(strTileset, m_astrWar2Tilesets[iWar2CurrentTileIndex]);
			if (iGroundType < 0)
			{
				// new tileset to add
				m_astrWar2Tilesets[iWar2CurrentTileIndex] = m_astrWar2Tilesets[iWar2CurrentTileIndex] + strTileset.GetStrID();
				iGroundType = GetTileIndex(strTileset, m_astrWar2Tilesets[iWar2CurrentTileIndex]);
			}
			iCliffType = GetTileIndex(strCliffset, m_astrWar2Cliffsets[iWar2CurrentTileIndex]);
			if (iCliffType < 0)
			{
				// new cliffset to add
				m_astrWar2Cliffsets[iWar2CurrentTileIndex] = m_astrWar2Cliffsets[iWar2CurrentTileIndex] + strCliffset.GetStrID();
				iCliffType = GetTileIndex(strCliffset, m_astrWar2Cliffsets[iWar2CurrentTileIndex]);
			}

			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_FOREST_INDEX].groundtype = iGroundType;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_FOREST_INDEX].clifftype = iCliffType;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_FOREST_INDEX].layer = iLayer;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_FOREST_INDEX].height = iGroundHeight;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_FOREST_INDEX].waterlevel = iWaterHeight;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_FOREST_INDEX].flags = iFlags;
			m_astrWar2Doodads[iWar2CurrentTileIndex][PUD_FOREST_INDEX] = strDoodad;
		}
		else if ((field.CompareNoCase(STR_SET_W2FMOUNTAINS) == 0)
		         || (field.CompareNoCase(STR_SET_W2IMOUNTAINS) == 0)
				 || (field.CompareNoCase(STR_SET_W2WMOUNTAINS) == 0)
				 || (field.CompareNoCase(STR_SET_W2SMOUNTAINS) == 0))
		{
			iGroundType = GetTileIndex(strTileset, m_astrWar2Tilesets[iWar2CurrentTileIndex]);
			if (iGroundType < 0)
			{
				// new tileset to add
				m_astrWar2Tilesets[iWar2CurrentTileIndex] = m_astrWar2Tilesets[iWar2CurrentTileIndex] + strTileset.GetStrID();
				iGroundType = GetTileIndex(strTileset, m_astrWar2Tilesets[iWar2CurrentTileIndex]);
			}
			iCliffType = GetTileIndex(strCliffset, m_astrWar2Cliffsets[iWar2CurrentTileIndex]);
			if (iCliffType < 0)
			{
				// new cliffset to add
				m_astrWar2Cliffsets[iWar2CurrentTileIndex] = m_astrWar2Cliffsets[iWar2CurrentTileIndex] + strCliffset.GetStrID();
				iCliffType = GetTileIndex(strCliffset, m_astrWar2Cliffsets[iWar2CurrentTileIndex]);
			}

			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_MOUNTAINS_INDEX].groundtype = iGroundType;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_MOUNTAINS_INDEX].clifftype = iCliffType;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_MOUNTAINS_INDEX].layer = iLayer;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_MOUNTAINS_INDEX].height = iGroundHeight;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_MOUNTAINS_INDEX].waterlevel = iWaterHeight;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_MOUNTAINS_INDEX].flags = iFlags;
			m_astrWar2Doodads[iWar2CurrentTileIndex][PUD_MOUNTAINS_INDEX] = strDoodad;
		}
		else if ((field.CompareNoCase(STR_SET_W2FHUMANWALL) == 0)
		         || (field.CompareNoCase(STR_SET_W2IHUMANWALL) == 0)
				 || (field.CompareNoCase(STR_SET_W2WHUMANWALL) == 0)
				 || (field.CompareNoCase(STR_SET_W2SHUMANWALL) == 0))
		{
			iGroundType = GetTileIndex(strTileset, m_astrWar2Tilesets[iWar2CurrentTileIndex]);
			if (iGroundType < 0)
			{
				// new tileset to add
				m_astrWar2Tilesets[iWar2CurrentTileIndex] = m_astrWar2Tilesets[iWar2CurrentTileIndex] + strTileset.GetStrID();
				iGroundType = GetTileIndex(strTileset, m_astrWar2Tilesets[iWar2CurrentTileIndex]);
			}
			iCliffType = GetTileIndex(strCliffset, m_astrWar2Cliffsets[iWar2CurrentTileIndex]);
			if (iCliffType < 0)
			{
				// new cliffset to add
				m_astrWar2Cliffsets[iWar2CurrentTileIndex] = m_astrWar2Cliffsets[iWar2CurrentTileIndex] + strCliffset.GetStrID();
				iCliffType = GetTileIndex(strCliffset, m_astrWar2Cliffsets[iWar2CurrentTileIndex]);
			}

			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_HUMANWALL_INDEX].groundtype = iGroundType;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_HUMANWALL_INDEX].clifftype = iCliffType;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_HUMANWALL_INDEX].layer = iLayer;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_HUMANWALL_INDEX].height = iGroundHeight;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_HUMANWALL_INDEX].waterlevel = iWaterHeight;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_HUMANWALL_INDEX].flags = iFlags;
			m_astrWar2Doodads[iWar2CurrentTileIndex][PUD_HUMANWALL_INDEX] = strDoodad;
		}
		else if ((field.CompareNoCase(STR_SET_W2FORCWALL) == 0)
		         || (field.CompareNoCase(STR_SET_W2IORCWALL) == 0)
				 || (field.CompareNoCase(STR_SET_W2WORCWALL) == 0)
				 || (field.CompareNoCase(STR_SET_W2SORCWALL) == 0))
		{
			iGroundType = GetTileIndex(strTileset, m_astrWar2Tilesets[iWar2CurrentTileIndex]);
			if (iGroundType < 0)
			{
				// new tileset to add
				m_astrWar2Tilesets[iWar2CurrentTileIndex] = m_astrWar2Tilesets[iWar2CurrentTileIndex] + strTileset.GetStrID();
				iGroundType = GetTileIndex(strTileset, m_astrWar2Tilesets[iWar2CurrentTileIndex]);
			}
			iCliffType = GetTileIndex(strCliffset, m_astrWar2Cliffsets[iWar2CurrentTileIndex]);
			if (iCliffType < 0)
			{
				// new cliffset to add
				m_astrWar2Cliffsets[iWar2CurrentTileIndex] = m_astrWar2Cliffsets[iWar2CurrentTileIndex] + strCliffset.GetStrID();
				iCliffType = GetTileIndex(strCliffset, m_astrWar2Cliffsets[iWar2CurrentTileIndex]);
			}

			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_ORCWALL_INDEX].groundtype = iGroundType;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_ORCWALL_INDEX].clifftype = iCliffType;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_ORCWALL_INDEX].layer = iLayer;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_ORCWALL_INDEX].height = iGroundHeight;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_ORCWALL_INDEX].waterlevel = iWaterHeight;
			m_atWar2Tilepoints[iWar2CurrentTileIndex][PUD_ORCWALL_INDEX].flags = iFlags;
			m_astrWar2Doodads[iWar2CurrentTileIndex][PUD_ORCWALL_INDEX] = strDoodad;
		}
		// Starcraft...
		// SC terrains
		else if (field.CompareNoCase(STR_SET_SCBADLANDS) == 0)
		{
			// SC Badlands
			iSCTerrainIndex = SCM_TERRAIN_BADLANDS;
			// gets main tileset if one
			if (values.GetLength() > 0)
			{
				m_acSCMainTileset[SCM_TERRAIN_BADLANDS] = values[0];
			}
			m_astrSCTilesets[SCM_TERRAIN_BADLANDS] = "";
			m_astrSCCliffsets[SCM_TERRAIN_BADLANDS] = "";
		}
		else if (field.CompareNoCase(STR_SET_SCSPACEPLATFORM) == 0)
		{
			// SC Space Platform
			iSCTerrainIndex = SCM_TERRAIN_SPACE;
			// gets main tileset if one
			if (values.GetLength() > 0)
			{
				m_acSCMainTileset[SCM_TERRAIN_SPACE] = values[0];
			}
			m_astrSCTilesets[SCM_TERRAIN_SPACE] = "";
			m_astrSCCliffsets[SCM_TERRAIN_SPACE] = "";
		}
		else if (field.CompareNoCase(STR_SET_SCINSTALLATION) == 0)
		{
			// SC Installation
			iSCTerrainIndex = SCM_TERRAIN_INSTALLATION;
			// gets main tileset if one
			if (values.GetLength() > 0)
			{
				m_acSCMainTileset[SCM_TERRAIN_INSTALLATION] = values[0];
			}
			m_astrSCTilesets[SCM_TERRAIN_INSTALLATION] = "";
			m_astrSCCliffsets[SCM_TERRAIN_INSTALLATION] = "";
		}
		else if (field.CompareNoCase(STR_SET_SCASHWORLD) == 0)
		{
			// SC Ashworld
			iSCTerrainIndex = SCM_TERRAIN_ASHWORLD;
			// gets main tileset if one
			if (values.GetLength() > 0)
			{
				m_acSCMainTileset[SCM_TERRAIN_ASHWORLD] = values[0];
			}
			m_astrSCTilesets[SCM_TERRAIN_ASHWORLD] = "";
			m_astrSCCliffsets[SCM_TERRAIN_ASHWORLD] = "";
		}
		else if (field.CompareNoCase(STR_SET_SCJUNGLEWORLD) == 0)
		{
			// SC Jungle
			iSCTerrainIndex = SCM_TERRAIN_JUNGLE;
			// gets main tileset if one
			if (values.GetLength() > 0)
			{
				m_acSCMainTileset[SCM_TERRAIN_JUNGLE] = values[0];
			}
			m_astrSCTilesets[SCM_TERRAIN_JUNGLE] = "";
			m_astrSCCliffsets[SCM_TERRAIN_JUNGLE] = "";
		}
		else if (field.CompareNoCase(STR_SET_SCDESERT) == 0)
		{
			// SC Desert
			iSCTerrainIndex = SCM_TERRAIN_DESERT;
			// gets main tileset if one
			if (values.GetLength() > 0)
			{
				m_acSCMainTileset[SCM_TERRAIN_DESERT] = values[0];
			}
			m_astrSCTilesets[SCM_TERRAIN_DESERT] = "";
			m_astrSCCliffsets[SCM_TERRAIN_DESERT] = "";
		}
		else if (field.CompareNoCase(STR_SET_SCICEWORLD) == 0)
		{
			// SC Arctic
			iSCTerrainIndex = SCM_TERRAIN_ARCTIC;
			// gets main tileset if one
			if (values.GetLength() > 0)
			{
				m_acSCMainTileset[SCM_TERRAIN_ARCTIC] = values[0];
			}
			m_astrSCTilesets[SCM_TERRAIN_ARCTIC] = "";
			m_astrSCCliffsets[SCM_TERRAIN_ARCTIC] = "";
		}
		else if (field.CompareNoCase(STR_SET_SCTWILIGHT) == 0)
		{
			// SC Twilight
			iSCTerrainIndex = SCM_TERRAIN_TWILIGHT;
			// gets main tileset if one
			if (values.GetLength() > 0)
			{
				m_acSCMainTileset[SCM_TERRAIN_TWILIGHT] = values[0];
			}
			m_astrSCTilesets[SCM_TERRAIN_TWILIGHT] = "";
			m_astrSCCliffsets[SCM_TERRAIN_TWILIGHT] = "";
		}
		// SC grounds
		else if (field.CompareNoCase(STR_SET_SCBLDIRT) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_BADLANDS;
			iSCGroundIndex = INDEX_SET_SCBLDIRT;
		}
		else if (field.CompareNoCase(STR_SET_SCBLMUD) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_BADLANDS;
			iSCGroundIndex = INDEX_SET_SCBLMUD;
		}
		else if (field.CompareNoCase(STR_SET_SCBLHIGHDIRT) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_BADLANDS;
			iSCGroundIndex = INDEX_SET_SCBLHIGHDIRT;
		}
		else if (field.CompareNoCase(STR_SET_SCBLWATER) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_BADLANDS;
			iSCGroundIndex = INDEX_SET_SCBLWATER;
		}
		else if (field.CompareNoCase(STR_SET_SCBLGRASS) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_BADLANDS;
			iSCGroundIndex = INDEX_SET_SCBLGRASS;
		}
		else if (field.CompareNoCase(STR_SET_SCBLHIGHGRASS) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_BADLANDS;
			iSCGroundIndex = INDEX_SET_SCBLHIGHGRASS;
		}
		else if (field.CompareNoCase(STR_SET_SCBLSTRUCTURE) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_BADLANDS;
			iSCGroundIndex = INDEX_SET_SCBLSTRUCTURE;
		}
		else if (field.CompareNoCase(STR_SET_SCBLASPHALT) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_BADLANDS;
			iSCGroundIndex = INDEX_SET_SCBLASPHALT;
		}
		else if (field.CompareNoCase(STR_SET_SCBLROCKYGROUND) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_BADLANDS;
			iSCGroundIndex = INDEX_SET_SCBLROCKYGROUND;
		}
		else if (field.CompareNoCase(STR_SET_SCSPSPACE) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_SPACE;
			iSCGroundIndex = INDEX_SET_SCSPSPACE;
		}
		else if (field.CompareNoCase(STR_SET_SCSPLOWPLATFORM) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_SPACE;
			iSCGroundIndex = INDEX_SET_SCSPLOWPLATFORM;
		}
		else if (field.CompareNoCase(STR_SET_SCSPRUSTYPIT) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_SPACE;
			iSCGroundIndex = INDEX_SET_SCSPRUSTYPIT;
		}
		else if (field.CompareNoCase(STR_SET_SCSPPLATFORM) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_SPACE;
			iSCGroundIndex = INDEX_SET_SCSPPLATFORM;
		}
		else if (field.CompareNoCase(STR_SET_SCSPDARKPLATFORM) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_SPACE;
			iSCGroundIndex = INDEX_SET_SCSPDARKPLATFORM;
		}
		else if (field.CompareNoCase(STR_SET_SCSPPLATING) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_SPACE;
			iSCGroundIndex = INDEX_SET_SCSPPLATING;
		}
		else if (field.CompareNoCase(STR_SET_SCSPSOLARARRAY) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_SPACE;
			iSCGroundIndex = INDEX_SET_SCSPSOLARARRAY;
		}
		else if (field.CompareNoCase(STR_SET_SCSPHIGHPLATFORM) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_SPACE;
			iSCGroundIndex = INDEX_SET_SCSPHIGHPLATFORM;
		}
		else if (field.CompareNoCase(STR_SET_SCSPHIGHPLATING) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_SPACE;
			iSCGroundIndex = INDEX_SET_SCSPHIGHPLATING;
		}
		else if (field.CompareNoCase(STR_SET_SCSPELEVATEDCATWALK) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_SPACE;
			iSCGroundIndex = INDEX_SET_SCSPELEVATEDCATWALK;
		}
		else if (field.CompareNoCase(STR_SET_SCISUBSTRUCTURE) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_INSTALLATION;
			iSCGroundIndex = INDEX_SET_SCISUBSTRUCTURE;
		}
		else if (field.CompareNoCase(STR_SET_SCIFLOOR) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_INSTALLATION;
			iSCGroundIndex = INDEX_SET_SCIFLOOR;
		}
		else if (field.CompareNoCase(STR_SET_SCIROOF) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_INSTALLATION;
			iSCGroundIndex = INDEX_SET_SCIROOF;
		}
		else if (field.CompareNoCase(STR_SET_SCISUBSTRUCTUREPLATING) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_INSTALLATION;
			iSCGroundIndex = INDEX_SET_SCISUBSTRUCTUREPLATING;
		}
		else if (field.CompareNoCase(STR_SET_SCIPLATING) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_INSTALLATION;
			iSCGroundIndex = INDEX_SET_SCIPLATING;
		}
		else if (field.CompareNoCase(STR_SET_SCISUBSTRUCTUREPANELS) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_INSTALLATION;
			iSCGroundIndex = INDEX_SET_SCISUBSTRUCTUREPANELS;
		}
		else if (field.CompareNoCase(STR_SET_SCIBOTTOMLESSPIT) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_INSTALLATION;
			iSCGroundIndex = INDEX_SET_SCIBOTTOMLESSPIT;
		}
		else if (field.CompareNoCase(STR_SET_SCAWMAGMA) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_ASHWORLD;
			iSCGroundIndex = INDEX_SET_SCAWMAGMA;
		}
		else if (field.CompareNoCase(STR_SET_SCAWDIRT) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_ASHWORLD;
			iSCGroundIndex = INDEX_SET_SCAWDIRT;
		}
		else if (field.CompareNoCase(STR_SET_SCAWLAVA) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_ASHWORLD;
			iSCGroundIndex = INDEX_SET_SCAWLAVA;
		}
		else if (field.CompareNoCase(STR_SET_SCAWSHALE) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_ASHWORLD;
			iSCGroundIndex = INDEX_SET_SCAWSHALE;
		}
		else if (field.CompareNoCase(STR_SET_SCAWBROKENROCK) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_ASHWORLD;
			iSCGroundIndex = INDEX_SET_SCAWBROKENROCK;
		}
		else if (field.CompareNoCase(STR_SET_SCAWHIGHDIRT) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_ASHWORLD;
			iSCGroundIndex = INDEX_SET_SCAWHIGHDIRT;
		}
		else if (field.CompareNoCase(STR_SET_SCAWHIGHLAVA) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_ASHWORLD;
			iSCGroundIndex = INDEX_SET_SCAWHIGHLAVA;
		}
		else if (field.CompareNoCase(STR_SET_SCAWHIGHSHALE) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_ASHWORLD;
			iSCGroundIndex = INDEX_SET_SCAWHIGHSHALE;
		}
		else if (field.CompareNoCase(STR_SET_SCJWWATER) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_JUNGLE;
			iSCGroundIndex = INDEX_SET_SCJWWATER;
		}
		else if (field.CompareNoCase(STR_SET_SCJWDIRT) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_JUNGLE;
			iSCGroundIndex = INDEX_SET_SCJWDIRT;
		}
		else if (field.CompareNoCase(STR_SET_SCJWMUD) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_JUNGLE;
			iSCGroundIndex = INDEX_SET_SCJWMUD;
		}
		else if (field.CompareNoCase(STR_SET_SCJWJUNGLE) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_JUNGLE;
			iSCGroundIndex = INDEX_SET_SCJWJUNGLE;
		}
		else if (field.CompareNoCase(STR_SET_SCJWROCKYGROUND) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_JUNGLE;
			iSCGroundIndex = INDEX_SET_SCJWROCKYGROUND;
		}
		else if (field.CompareNoCase(STR_SET_SCJWRUINS) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_JUNGLE;
			iSCGroundIndex = INDEX_SET_SCJWRUINS;
		}
		else if (field.CompareNoCase(STR_SET_SCJWRAISEDJUNGLE) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_JUNGLE;
			iSCGroundIndex = INDEX_SET_SCJWRAISEDJUNGLE;
		}
		else if (field.CompareNoCase(STR_SET_SCJWTEMPLE) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_JUNGLE;
			iSCGroundIndex = INDEX_SET_SCJWTEMPLE;
		}
		else if (field.CompareNoCase(STR_SET_SCJWHIGHDIRT) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_JUNGLE;
			iSCGroundIndex = INDEX_SET_SCJWHIGHDIRT;
		}
		else if (field.CompareNoCase(STR_SET_SCJWHIGHJUNGLE) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_JUNGLE;
			iSCGroundIndex = INDEX_SET_SCJWHIGHJUNGLE;
		}
		else if (field.CompareNoCase(STR_SET_SCJWHIGHRUINS) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_JUNGLE;
			iSCGroundIndex = INDEX_SET_SCJWHIGHRUINS;
		}
		else if (field.CompareNoCase(STR_SET_SCJWHIGHRAISEDJUNGLE) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_JUNGLE;
			iSCGroundIndex = INDEX_SET_SCJWHIGHRAISEDJUNGLE;
		}
		else if (field.CompareNoCase(STR_SET_SCJWHIGHTEMPLE) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_JUNGLE;
			iSCGroundIndex = INDEX_SET_SCJWHIGHTEMPLE;
		}
		else if (field.CompareNoCase(STR_SET_SCDTAR) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_DESERT;
			iSCGroundIndex = INDEX_SET_SCDTAR;
		}
		else if (field.CompareNoCase(STR_SET_SCDDIRT) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_DESERT;
			iSCGroundIndex = INDEX_SET_SCDDIRT;
		}
		else if (field.CompareNoCase(STR_SET_SCDDRIEDMUD) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_DESERT;
			iSCGroundIndex = INDEX_SET_SCDDRIEDMUD;
		}
		else if (field.CompareNoCase(STR_SET_SCDSANDDUNES) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_DESERT;
			iSCGroundIndex = INDEX_SET_SCDSANDDUNES;
		}
		else if (field.CompareNoCase(STR_SET_SCDROCKYGROUND) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_DESERT;
			iSCGroundIndex = INDEX_SET_SCDROCKYGROUND;
		}
		else if (field.CompareNoCase(STR_SET_SCDCRAGS) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_DESERT;
			iSCGroundIndex = INDEX_SET_SCDCRAGS;
		}
		else if (field.CompareNoCase(STR_SET_SCDSANDYSUNKENPIT) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_DESERT;
			iSCGroundIndex = INDEX_SET_SCDSANDYSUNKENPIT;
		}
		else if (field.CompareNoCase(STR_SET_SCDCOMPOUND) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_DESERT;
			iSCGroundIndex = INDEX_SET_SCDCOMPOUND;
		}
		else if (field.CompareNoCase(STR_SET_SCDHIGHDIRT) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_DESERT;
			iSCGroundIndex = INDEX_SET_SCDHIGHDIRT;
		}
		else if (field.CompareNoCase(STR_SET_SCDHIGHSANDDUNES) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_DESERT;
			iSCGroundIndex = INDEX_SET_SCDHIGHSANDDUNES;
		}
		else if (field.CompareNoCase(STR_SET_SCDHIGHCRAGS) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_DESERT;
			iSCGroundIndex = INDEX_SET_SCDHIGHCRAGS;
		}
		else if (field.CompareNoCase(STR_SET_SCDHIGHSANDYSUNKENPIT) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_DESERT;
			iSCGroundIndex = INDEX_SET_SCDHIGHSANDYSUNKENPIT;
		}
		else if (field.CompareNoCase(STR_SET_SCDHIGHCOMPOUND) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_DESERT;
			iSCGroundIndex = INDEX_SET_SCDHIGHCOMPOUND;
		}
		else if (field.CompareNoCase(STR_SET_SCIWICE) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_ARCTIC;
			iSCGroundIndex = INDEX_SET_SCIWICE;
		}
		else if (field.CompareNoCase(STR_SET_SCIWSNOW) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_ARCTIC;
			iSCGroundIndex = INDEX_SET_SCIWSNOW;
		}
		else if (field.CompareNoCase(STR_SET_SCIWMOGULS) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_ARCTIC;
			iSCGroundIndex = INDEX_SET_SCIWMOGULS;
		}
		else if (field.CompareNoCase(STR_SET_SCIWDIRT) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_ARCTIC;
			iSCGroundIndex = INDEX_SET_SCIWDIRT;
		}
		else if (field.CompareNoCase(STR_SET_SCIWROCKYSNOW) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_ARCTIC;
			iSCGroundIndex = INDEX_SET_SCIWROCKYSNOW;
		}
		else if (field.CompareNoCase(STR_SET_SCIWGRASS) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_ARCTIC;
			iSCGroundIndex = INDEX_SET_SCIWGRASS;
		}
		else if (field.CompareNoCase(STR_SET_SCIWWATER) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_ARCTIC;
			iSCGroundIndex = INDEX_SET_SCIWWATER;
		}
		else if (field.CompareNoCase(STR_SET_SCIWOUTPOST) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_ARCTIC;
			iSCGroundIndex = INDEX_SET_SCIWOUTPOST;
		}
		else if (field.CompareNoCase(STR_SET_SCIWHIGHSNOW) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_ARCTIC;
			iSCGroundIndex = INDEX_SET_SCIWHIGHSNOW;
		}
		else if (field.CompareNoCase(STR_SET_SCIWHIGHDIRT) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_ARCTIC;
			iSCGroundIndex = INDEX_SET_SCIWHIGHDIRT;
		}
		else if (field.CompareNoCase(STR_SET_SCIWHIGHGRASS) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_ARCTIC;
			iSCGroundIndex = INDEX_SET_SCIWHIGHGRASS;
		}
		else if (field.CompareNoCase(STR_SET_SCIWHIGHWATER) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_ARCTIC;
			iSCGroundIndex = INDEX_SET_SCIWHIGHWATER;
		}
		else if (field.CompareNoCase(STR_SET_SCIWHIGHOUTPOST) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_ARCTIC;
			iSCGroundIndex = INDEX_SET_SCIWHIGHOUTPOST;
		}
		else if (field.CompareNoCase(STR_SET_SCTWATER) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_TWILIGHT;
			iSCGroundIndex = INDEX_SET_SCTWATER;
		}
		else if (field.CompareNoCase(STR_SET_SCTDIRT) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_TWILIGHT;
			iSCGroundIndex = INDEX_SET_SCTDIRT;
		}
		else if (field.CompareNoCase(STR_SET_SCTMUD) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_TWILIGHT;
			iSCGroundIndex = INDEX_SET_SCTMUD;
		}
		else if (field.CompareNoCase(STR_SET_SCTCRUSHEDROCK) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_TWILIGHT;
			iSCGroundIndex = INDEX_SET_SCTCRUSHEDROCK;
		}
		else if (field.CompareNoCase(STR_SET_SCTCREVICES) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_TWILIGHT;
			iSCGroundIndex = INDEX_SET_SCTCREVICES;
		}
		else if (field.CompareNoCase(STR_SET_SCTFLAGSTONES) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_TWILIGHT;
			iSCGroundIndex = INDEX_SET_SCTFLAGSTONES;
		}
		else if (field.CompareNoCase(STR_SET_SCTSUNKENGROUND) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_TWILIGHT;
			iSCGroundIndex = INDEX_SET_SCTSUNKENGROUND;
		}
		else if (field.CompareNoCase(STR_SET_SCTBASILICA) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_TWILIGHT;
			iSCGroundIndex = INDEX_SET_SCTBASILICA;
		}
		else if (field.CompareNoCase(STR_SET_SCTHIGHDIRT) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_TWILIGHT;
			iSCGroundIndex = INDEX_SET_SCTHIGHDIRT;
		}
		else if (field.CompareNoCase(STR_SET_SCTHIGHCRUSHEDROCKS) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_TWILIGHT;
			iSCGroundIndex = INDEX_SET_SCTHIGHCRUSHEDROCKS;
		}
		else if (field.CompareNoCase(STR_SET_SCTHIGHFLAGSTONES) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_TWILIGHT;
			iSCGroundIndex = INDEX_SET_SCTHIGHFLAGSTONES;
		}
		else if (field.CompareNoCase(STR_SET_SCTHIGHSUNKENGROUND) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_TWILIGHT;
			iSCGroundIndex = INDEX_SET_SCTHIGHSUNKENGROUND;
		}
		else if (field.CompareNoCase(STR_SET_SCTHIGHBASILICA) == 0)
		{
			iSCTerrainIndex = SCM_TERRAIN_TWILIGHT;
			iSCGroundIndex = INDEX_SET_SCTHIGHBASILICA;
		}

		// SC tilesets init
		if ((0 <= iSCGroundIndex) && (0 <= iSCTerrainIndex))
		{
			iGroundType = GetTileIndex(strTileset, m_astrSCTilesets[iSCTerrainIndex]);
			if (iGroundType < 0)
			{
				// new tileset to add
				m_astrSCTilesets[iSCTerrainIndex] = m_astrSCTilesets[iSCTerrainIndex] + strTileset.GetStrID();
				iGroundType = GetTileIndex(strTileset, m_astrSCTilesets[iSCTerrainIndex]);
			}
			iCliffType = GetTileIndex(strCliffset, m_astrSCCliffsets[iSCTerrainIndex]);
			if (iCliffType < 0)
			{
				// new cliffset to add
				m_astrSCCliffsets[iSCTerrainIndex] = m_astrSCCliffsets[iSCTerrainIndex] + strCliffset.GetStrID();
				iCliffType = GetTileIndex(strCliffset, m_astrSCCliffsets[iSCTerrainIndex]);
			}
			m_atSCTilepoints[iSCTerrainIndex][iSCGroundIndex].groundtype = iGroundType;
			m_atSCTilepoints[iSCTerrainIndex][iSCGroundIndex].clifftype = iCliffType;
			m_atSCTilepoints[iSCTerrainIndex][iSCGroundIndex].layer = iLayer;
			m_atSCTilepoints[iSCTerrainIndex][iSCGroundIndex].height = iGroundHeight;
			m_atSCTilepoints[iSCTerrainIndex][iSCGroundIndex].waterlevel = iWaterHeight;
			m_atSCTilepoints[iSCTerrainIndex][iSCGroundIndex].flags = iFlags;
			m_astrSCDoodads[iSCTerrainIndex][iSCGroundIndex] = strDoodad;
		}
	}
}

COLORREF CW3ZMapEditSettings::GetWaterColor(char groupID)
{
	return GetTilesetGroup(groupID).waterColor;
}

float CW3ZMapEditSettings::GetWaterAlpha(char groupID)
{
	return GetTilesetGroup(groupID).fWaterAlpha;
}

/*
returns the tile index if in the list, -1 otherwise
*/
int CW3ZMapEditSettings::GetTileIndex(CWar3StrID strTileID, CString strTilelist)
{
	int i=0;
	int iTileIndex = -1;
	while((i < strTilelist.GetLength()) && (iTileIndex < 0))
	{
		if (strTileID == strTilelist.Mid(i, WAR3STRID_MAXCHAR))
		{
			iTileIndex = i/4;
		}
		i += 4;
	}
	return iTileIndex;
}

void CW3ZMapEditSettings::CheckLoadPlugins(CString strTempFileName, CString strRealFileName)
{
	CPListedPlugin pLPlugin;

	pLPlugin = m_pLoadPluginList;
	while(NULL != pLPlugin)
	{
		if (pLPlugin->Matches(strRealFileName))
		{
			pLPlugin->Run(strTempFileName);
		}
		pLPlugin = pLPlugin->next;
	}
}

void CW3ZMapEditSettings::CheckSavePlugins(CString strTempFileName, CString strRealFileName)
{
	CPListedPlugin pLPlugin;

	pLPlugin = m_pSavePluginList;
	while(NULL != pLPlugin)
	{
		if (pLPlugin->Matches(strRealFileName))
		{
			pLPlugin->Run(strTempFileName);
		}
		pLPlugin = pLPlugin->next;
	}
}

void CW3ZMapEditSettings::CheckMenuPlugin(CString strTempFileName, CString strRealFileName, CString strPluginName)
{
	CPListedPlugin pLPlugin;

	pLPlugin = m_pMenuPluginList;
	while(NULL != pLPlugin)
	{
		if (strPluginName == pLPlugin->strPluginName)
		{
			if (pLPlugin->Matches(strRealFileName))
			{
				pLPlugin->Run(strTempFileName);
			}
		}
		pLPlugin = pLPlugin->next;
	}
}

CString CW3ZMapEditSettings::GetMenuPluginList()
{
	CString strPluginList;
	CPListedPlugin pLPlugin;

	strPluginList = "";

	pLPlugin = m_pMenuPluginList;
	while(NULL != pLPlugin)
	{
		strPluginList += pLPlugin->strPluginName + "\n";
		pLPlugin = pLPlugin->next;
	}

	return strPluginList;
}
