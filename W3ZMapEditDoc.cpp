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
// W3ZMapEditDoc.cpp : implementation of the CW3ZMapEditDoc class
//

#include "stdafx.h"
#include "W3ZMapEdit.h"
#include "W3ZMapEditDoc.h"
#include ".\w3zmapeditdoc.h"
#include "W3ZMapEditView.h"
#include "W3ZMapOpenGLView.h"
#include "MainFrm.h"
#include "TrigString.h"
#include "ListedUndoObj.h"
#include "UndoObj.h"
#include "MapStatisticsDialog.h"
#include "TilesetGroup.h"
#include "PasswordDialog.h"
#include "War3FileException.h"
#include "ProgressWnd.h"
#include "SCMConversionDialog.h"
#include <math.h>
#include "War3ConversionData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CW3ZMapEditDoc

IMPLEMENT_DYNCREATE(CW3ZMapEditDoc, CDocument)

BEGIN_MESSAGE_MAP(CW3ZMapEditDoc, CDocument)
	//{{AFX_MSG_MAP(CW3ZMapEditDoc)
	ON_COMMAND(ID_EDIT_MAP_PROPERTIES, OnEditMapProperties)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_MAP_RESIZEMAP, OnEditMapResizeMap)
	ON_COMMAND(ID_EDIT_MAP_MENUOPTIONS, OnEditMapMenuOptions)
	ON_COMMAND(ID_EDIT_MAP_PROTECTION, OnEditMapProtection)
	ON_COMMAND(ID_VIEW_STATISTICS, OnViewStatistics)
	ON_COMMAND(ID_FILE_QUICKBACKUP, OnFileQuickBackup)
	ON_COMMAND(ID_EDIT_MINIMAP, OnEditMinimap)
	ON_COMMAND(ID_EDIT_PLAYERSPROPERTIES, OnEditPlayersProperties)
	ON_BN_CLICKED(IDC_BTN_PATH_RESET, OnBtnPathReset)
	//}}AFX_MSG_MAP
//	ON_COMMAND(ID_PLUGINS, OnPlugins)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CW3ZMapEditDoc construction/destruction

CW3ZMapEditDoc::CW3ZMapEditDoc()
{
	CString tempdir, newdir;
	CFileFind ffDir;
	int i;

	m_pt3DChildFrame = NULL;
	m_strOriginalMPQFilename = "";
	m_strListfile = "";
	m_bEnableProtect = FALSE;
	m_bPPassCorrupted = FALSE;
	m_bAPassCorrupted = FALSE;
	m_bUnpRec = FALSE;
	m_bMakeBackup = FALSE;
	m_bAuthorMsg = FALSE;
	m_strPublicPassword = "";
	m_strAuthorPassword = "";
	m_iProtectionType = W3Z_MAP_PROT_NOTPROTECTED;
//	m_uHashPublicPassword = 0;
//	m_uHashAuthorPassword = 0;
	m_iFilesProtectStatus = 0;
	m_iFilesInitStatus = FL_W3M_NONE;
	m_iFilesSaveStatus = 0;
	m_bAuthorRegistered = FALSE;
	m_bPublicRegistered = FALSE;
	m_bMapProtected = FALSE;

	m_pEditMapPropSheet = new CEditMapPropSheet(STR_MAP_PROPERTIES);
	m_pEditMapPropSheet->m_MapPropProtectPage.m_pDoc = this; // link the protection page

	// cherche le repertoire temporaire
	tempdir = getenv("TEMP");
	if (0 <= tempdir.GetLength())
	{
		i = tempdir.Find(";");
		if (i > 0) // i == -1 --> pas de ; trouve
		{
			// potentiellement plusieurs rep specifies
			tempdir = tempdir.Left(i); // coupe avant le ;
		}
		if ("\\" != tempdir.Right(1)) // ajoute un \ a la fin s'il n'y en a pas
		{
			tempdir += "\\";
		}
	}
	if (!ffDir.FindFile(tempdir + ".")) // verifie si le repertoire existe
	{
		tempdir = ""; // sinon, prends le repertoire courant
	}
	newdir = tempdir + STR_W3Z_TEMP_DIR;
	if (CreateDirectory(newdir,NULL))
	{
		tempdir = tempdir + STR_W3Z_TEMP_DIR;
	}
	else if (ffDir.FindFile(tempdir + STR_W3Z_TEMP_DIR + ".")) // verifie si le repertoire existe deja
	{
		tempdir = tempdir + STR_W3Z_TEMP_DIR;
	}
	// ici, tempdir contient un nom de repertoire correct
	m_strTempDir = tempdir;

	EmptyTemporaryDirectory(m_strTempDir);

	m_bTmpFilesCreated =
		(OpenTemporaryFile(m_fiW3E) &&
		OpenTemporaryFile(m_fiWTS) &&
		OpenTemporaryFile(m_fiW3I) &&
		OpenTemporaryFile(m_fiWTG) &&
		OpenTemporaryFile(m_fiWCT) &&
		OpenTemporaryFile(m_fiJASS) &&
		OpenTemporaryFile(m_fiSHD) &&
		OpenTemporaryFile(m_fiPreviewTGA) &&
		OpenTemporaryFile(m_fiMapTGA) &&
		OpenTemporaryFile(m_fiMapBLP) &&
		OpenTemporaryFile(m_fiMapB00) &&
		OpenTemporaryFile(m_fiMMP) &&
		OpenTemporaryFile(m_fiPathTGA) &&
		OpenTemporaryFile(m_fiWPM) &&		
		OpenTemporaryFile(m_fiDOO) &&
		OpenTemporaryFile(m_fiUnitsDOO) &&
		OpenTemporaryFile(m_fiW3R) &&
		OpenTemporaryFile(m_fiW3C) &&
		OpenTemporaryFile(m_fiW3U) &&
		OpenTemporaryFile(m_fiW3S) &&
		OpenTemporaryFile(m_fiIMP) &&
		OpenTemporaryFile(m_fiUNP) &&
		OpenTemporaryFile(m_fiListfile) &&
		OpenTemporaryFile(m_fiEmptyFile));
	if (!m_bTmpFilesCreated)
	{
		W3Z_ERROR(STR_ERROR_TEMP00);
	}
	m_pUndoListHead= NULL;
	m_pUndoListTail = NULL;
	m_bOpenedUndoObj = false;
	m_iMaxUndos = UNDO_DEFAULT_MAX;
	m_iNbUndos = 0;
	m_pUndoTPMap = NULL;
	m_pUndoPathMap = NULL;
	m_pUndoShadowsMap = NULL;
	m_iMapWidth = 0;
	m_iMapHeight = 0;

}

CW3ZMapEditDoc::~CW3ZMapEditDoc()
{
	CListedUndoObj* pLUndoObj;

	if (m_pEditMapPropSheet)
	{
		delete m_pEditMapPropSheet;
	}

	CloseAndDeleteFile(m_fiW3E);
	CloseAndDeleteFile(m_fiW3I);
	CloseAndDeleteFile(m_fiWTG);
	CloseAndDeleteFile(m_fiWCT);
	CloseAndDeleteFile(m_fiWTS);
	CloseAndDeleteFile(m_fiJASS);
	CloseAndDeleteFile(m_fiSHD);
	CloseAndDeleteFile(m_fiPreviewTGA);
	CloseAndDeleteFile(m_fiMapTGA);
	CloseAndDeleteFile(m_fiMapBLP);
	CloseAndDeleteFile(m_fiMapB00);
	CloseAndDeleteFile(m_fiMMP);
	CloseAndDeleteFile(m_fiPathTGA);
	CloseAndDeleteFile(m_fiWPM);
	CloseAndDeleteFile(m_fiDOO);
	CloseAndDeleteFile(m_fiUnitsDOO);
	CloseAndDeleteFile(m_fiW3R);
	CloseAndDeleteFile(m_fiW3C);
	CloseAndDeleteFile(m_fiW3U);
	CloseAndDeleteFile(m_fiW3S);
	CloseAndDeleteFile(m_fiIMP);
	CloseAndDeleteFile(m_fiUNP);
	CloseAndDeleteFile(m_fiListfile);
	CloseAndDeleteFile(m_fiEmptyFile);
	while(m_pUndoListHead)
	{
		pLUndoObj = m_pUndoListHead;
		m_pUndoListHead = m_pUndoListHead->next;
		delete pLUndoObj;
	}
	m_pUndoListTail = NULL;
	if (m_pUndoTPMap)
	{
		delete []m_pUndoTPMap;
	}
	if (m_pUndoPathMap)
	{
		delete []m_pUndoPathMap;
	}
	if (m_pUndoShadowsMap)
	{
		delete []m_pUndoShadowsMap;
	}
}

BOOL CW3ZMapEditDoc::OnNewDocument()
{
	int i, iWidth, iHeight;
	CString str;
	CTilePoint tilepoint;

    CW3ZMapEditApp(* pTheApp) = (CW3ZMapEditApp *)AfxGetApp();
	CMainFrame *pMainFrame;
	if (pTheApp)
	{pMainFrame = pTheApp->GetMainFrame();}

	if (!CDocument::OnNewDocument())
		return FALSE;
	if (!m_bTmpFilesCreated)
	{
		W3Z_ERROR(STR_ERROR_TEMP00);
		return FALSE;
	}

	//++ tester les grounds et les cliffs ici
	// afficher un dialog en cas d'erreur
	// et retourner false
	//create tilesets
	m_pEditMapPropSheet->m_MapPropSizePage.m_iWidth = W3E_DEFAULT_WIDTH;
	m_pEditMapPropSheet->m_MapPropSizePage.m_iHeight = W3E_DEFAULT_HEIGHT;
	m_pEditMapPropSheet->m_MapPropMainPage.m_sName = W3I_STR_MAPNAME_DEFAULT;
	m_pEditMapPropSheet->m_MapPropMainPage.m_sAuthor = W3I_STR_MAPAUTHOR_DEFAULT;
	m_pEditMapPropSheet->m_MapPropMainPage.m_sPlayText = W3I_STR_MAPSUGPLYR_DEFAULT;
	m_pEditMapPropSheet->m_MapPropMainPage.m_sDesc = W3I_STR_MAPDESC_DEFAULT;
	m_pEditMapPropSheet->m_MapPropMainPage.m_sGround = W3E_TILESET_DEFAULT;
	m_pEditMapPropSheet->m_MapPropMainPage.m_sCliff = W3E_CLIFFSET_DEFAULT;
	m_pEditMapPropSheet->m_MapPropMainPage.m_cTilesetGroup = W3E_MAINTILE_DEFAULT;
	m_pEditMapPropSheet->m_MapPropMainPage.m_iNbPlayers = W3I_NBPLAYERS_DEFAULT;
	m_pEditMapPropSheet->m_MapPropSizePage.m_iPlayableWidth = W3E_DEFAULT_PLAYWIDTH;
	m_pEditMapPropSheet->m_MapPropSizePage.m_iPlayableHeight = W3E_DEFAULT_PLAYHEIGHT;
	m_pEditMapPropSheet->m_MapPropSizePage.m_fCBblx = W3I_BOUND_BOTTOMLEFT_X;
	m_pEditMapPropSheet->m_MapPropSizePage.m_fCBbly = W3I_BOUND_BOTTOMLEFT_Y;
	m_pEditMapPropSheet->m_MapPropSizePage.m_fCBtrx = W3I_BOUND_TOPRIGHT_X;
	m_pEditMapPropSheet->m_MapPropSizePage.m_fCBtry = W3I_BOUND_TOPRIGHT_Y;
	m_pEditMapPropSheet->m_MapPropSizePage.m_fCBtlx = W3I_BOUND_TOPLEFT_X;
	m_pEditMapPropSheet->m_MapPropSizePage.m_fCBtly = W3I_BOUND_TOPLEFT_Y;
	m_pEditMapPropSheet->m_MapPropSizePage.m_fCBbrx = W3I_BOUND_BOTTOMRIGHT_X;
	m_pEditMapPropSheet->m_MapPropSizePage.m_fCBbry = W3I_BOUND_BOTTOMRIGHT_Y;
	m_pEditMapPropSheet->m_MapPropMainPage.m_iEditorVer = W3I_EDITOR_ID_W3Z;
	m_pEditMapPropSheet->m_MapPropMainPage.m_iMapVer = 0;

	// Protection
	m_pEditMapPropSheet->m_MapPropProtectPage.m_iTypeSelected = 0;
	m_pEditMapPropSheet->m_MapPropProtectPage.m_iProtectLvl = 0;

	
	if (IDCANCEL == m_pEditMapPropSheet->DoModal())
	{
		return FALSE;
	}

	TRY
	{

		//W3E
		m_War3Map.SetMapSize(m_pEditMapPropSheet->m_MapPropSizePage.m_iWidth, m_pEditMapPropSheet->m_MapPropSizePage.m_iHeight);
		m_War3Map.mapGroundTiles = m_pEditMapPropSheet->m_MapPropMainPage.m_sGround;
		m_War3Map.mapCliffTiles = m_pEditMapPropSheet->m_MapPropMainPage.m_sCliff;

		m_War3Map.mapMainTileset = m_pEditMapPropSheet->m_MapPropMainPage.m_cTilesetGroup;
		m_War3Map.mapCustomTilesets = m_pEditMapPropSheet->m_MapPropMainPage.m_bCustTilesets;
		m_War3Map.SetMapCenter((float)(m_pEditMapPropSheet->m_MapPropSizePage.m_iWidth * 128. / -2.), (float)(m_pEditMapPropSheet->m_MapPropSizePage.m_iHeight * 128. / -2.));

		//sets map boundaries
		iWidth = m_War3Map.GetMapWidth();
		iHeight = m_War3Map.GetMapHeight();
		for (i = 0; i <= iWidth; i++)
		{
			tilepoint = m_War3Map.GetTilePoint(i, 0);
			tilepoint.flags |= TILE_FL_SHADOW;
			m_War3Map.SetTilePoint(i, 0, tilepoint);
			tilepoint = m_War3Map.GetTilePoint(i, iHeight-1);
			tilepoint.flags |= TILE_FL_SHADOW;
			m_War3Map.SetTilePoint(i, iHeight-1, tilepoint);
			tilepoint = m_War3Map.GetTilePoint(i, iHeight);
			tilepoint.flags |= TILE_FL_SHADOW;
			m_War3Map.SetTilePoint(i, iHeight, tilepoint);
		}
		for (i = 0; i <= iHeight; i++)
		{
			tilepoint = m_War3Map.GetTilePoint(0, i);
			tilepoint.flags |= TILE_FL_SHADOW;
			m_War3Map.SetTilePoint(0, i, tilepoint);
			tilepoint = m_War3Map.GetTilePoint(iWidth-1, i);
			tilepoint.flags |= TILE_FL_SHADOW;
			m_War3Map.SetTilePoint(iWidth-1, i, tilepoint);
			tilepoint = m_War3Map.GetTilePoint(iWidth, i);
			tilepoint.flags |= TILE_FL_SHADOW;
			m_War3Map.SetTilePoint(iWidth, i, tilepoint);
		}

		//DOO
		m_War3Map.mapUnknownDooInt = 07;

		//SHD

		//PathTGA

		//WPM

		//W3I
		m_War3Map.mapW3Info.mapEditVer = m_pEditMapPropSheet->m_MapPropMainPage.m_iEditorVer;
		m_War3Map.mapW3Info.mapNbSaves = m_pEditMapPropSheet->m_MapPropMainPage.m_iMapVer;
		m_War3Map.mapW3Info.mapNameTID = m_War3Map.mapTrigStringsTable.AddTrigString(m_pEditMapPropSheet->m_MapPropMainPage.m_sName);
		m_War3Map.mapW3Info.mapAuthorTID = m_War3Map.mapTrigStringsTable.AddTrigString(m_pEditMapPropSheet->m_MapPropMainPage.m_sAuthor);
		m_War3Map.mapW3Info.mapSugPlyrTID = m_War3Map.mapTrigStringsTable.AddTrigString(m_pEditMapPropSheet->m_MapPropMainPage.m_sPlayText);
		m_War3Map.mapW3Info.mapDescTID = m_War3Map.mapTrigStringsTable.AddTrigString(m_pEditMapPropSheet->m_MapPropMainPage.m_sDesc);
		m_War3Map.mapW3Info.mapCameraBounds.fX1 = m_pEditMapPropSheet->m_MapPropSizePage.m_fCBblx;
		m_War3Map.mapW3Info.mapCameraBounds.fY1 = m_pEditMapPropSheet->m_MapPropSizePage.m_fCBbly;
		m_War3Map.mapW3Info.mapCameraBounds.fX2 = m_pEditMapPropSheet->m_MapPropSizePage.m_fCBtrx;
		m_War3Map.mapW3Info.mapCameraBounds.fY2 = m_pEditMapPropSheet->m_MapPropSizePage.m_fCBtry;
		m_War3Map.mapW3Info.mapCameraBounds.fX3 = m_pEditMapPropSheet->m_MapPropSizePage.m_fCBtlx;
		m_War3Map.mapW3Info.mapCameraBounds.fY3 = m_pEditMapPropSheet->m_MapPropSizePage.m_fCBtly;
		m_War3Map.mapW3Info.mapCameraBounds.fX4 = m_pEditMapPropSheet->m_MapPropSizePage.m_fCBbrx;
		m_War3Map.mapW3Info.mapCameraBounds.fY4 = m_pEditMapPropSheet->m_MapPropSizePage.m_fCBbry;
		m_War3Map.mapW3Info.mapPlayableWidth = m_pEditMapPropSheet->m_MapPropSizePage.m_iPlayableWidth;
		m_War3Map.mapW3Info.mapPlayableHeight = m_pEditMapPropSheet->m_MapPropSizePage.m_iPlayableHeight;

		//map bounds
		m_War3Map.mapW3Info.mapLeftBoundWidth = (m_War3Map.GetMapWidth() - m_War3Map.mapW3Info.mapPlayableWidth)/2;
		m_War3Map.mapW3Info.mapRightBoundWidth = (m_War3Map.GetMapWidth() - m_War3Map.mapW3Info.mapPlayableWidth)/2;
		m_War3Map.mapW3Info.mapBottomBoundHeight = (m_War3Map.GetMapHeight() - m_War3Map.mapW3Info.mapPlayableHeight)/2;
		m_War3Map.mapW3Info.mapTopBoundHeight = (m_War3Map.GetMapHeight() - m_War3Map.mapW3Info.mapPlayableHeight)/2;

		//flags
		m_War3Map.mapW3Info.mapFlags = W3I_FLG_MASKEDAREA | W3I_FLG_MELEE | W3I_FLG_WAVESCLIFF | W3I_FLG_W3Z;

		// map Campaign background file path
		m_War3Map.mapW3Info.mapCampBackgrnd = -1;

		// map loading screen text
		m_War3Map.mapW3Info.mapLoadTextTID = -1;

		// map loading screen title
		m_War3Map.mapW3Info.mapLoadTitleTID = -1;

		// map loading screen subtitle
		m_War3Map.mapW3Info.mapLoadSubtitleTID = -1;

		// map loading screen glue file path
		m_War3Map.mapW3Info.mapLoadGlue = -1;

		// map prologue screen text
		m_War3Map.mapW3Info.mapProlTextTID = -1;

		// map prologue screen title
		m_War3Map.mapW3Info.mapProlTitleTID = -1;

		// map prologue screen subtitle
		m_War3Map.mapW3Info.mapProlSubtitleTID = -1;

		m_War3Map.mapW3Info.mapNbMaxPlayers = m_pEditMapPropSheet->m_MapPropMainPage.m_iNbPlayers;

		for (i = 0; i < 12; i++)
		{
			m_War3Map.mapW3Info.mapPlayers[i].iUnknown00 = i;
			m_War3Map.mapW3Info.mapPlayers[i].type = 1; // human control
			m_War3Map.mapW3Info.mapPlayers[i].race = 1 + i%4;
			m_War3Map.mapW3Info.mapPlayers[i].fixedstart = 1;
			if (i < m_War3Map.mapW3Info.mapNbMaxPlayers)
			{
				str.Format(W3I_PLAYER_NAME_DEFAULT, i);
				m_War3Map.mapW3Info.mapPlayers[i].trigID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}
			else
			{
				m_War3Map.mapW3Info.mapPlayers[i].trigID = -1;
			}
			m_War3Map.mapW3Info.mapPlayers[i].startX = (float)4000. - (float)300.*i;
			m_War3Map.mapW3Info.mapPlayers[i].startY = (float)4000. - (float)300.*i;
			m_War3Map.mapW3Info.mapPlayers[i].allyLowPrio = 0;
			m_War3Map.mapW3Info.mapPlayers[i].allyHighPrio = 0;
		}

		m_War3Map.mapW3Info.mapNbMaxForces = 1;
		for (i = 0; i < 12; i++)
		{
			str.Format(W3I_FORCE_NAME_DEFAULT, i);
			m_War3Map.mapW3Info.mapForces[i].trigID =  m_War3Map.mapTrigStringsTable.AddTrigString(str);
			m_War3Map.mapW3Info.mapForces[i].iUnknown00 = 0;
			m_War3Map.mapW3Info.mapForces[i].iUnknown01 = 0;
			m_War3Map.mapW3Info.mapForces[i].number = i;
		}

		m_War3Map.mapCustomTilesets = 0;

	}
	CATCH(CMemoryException, e)
	{
		W3Z_ERROR(STR_ERROR_DOC00);
		return FALSE;
	}
	END_CATCH
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CW3ZMapEditDoc serialization

void CW3ZMapEditDoc::Serialize(CArchive& ar)
{
	CWaitCursor wait;

	// password stuff in case of protected map
	BOOL bDone;
	int iTries;

	//to check file nature and add the imported files
	CString str, sFileName;
	CFile *pFile;
	CFile fiImportedFile;
	CFile fiTempFile;
	int i;
	// for the W3M header
	char *pBuf= NULL;

	CW3ZMapEditApp *pTheApp;
	pTheApp = (CW3ZMapEditApp *)AfxGetApp();

	pFile = ar.GetFile();
	if (NULL != pFile)
	{
		sFileName = pFile->GetFilePath();
		str = sFileName.Right(4);
// W3E format
		if (!str.CompareNoCase(STR_W3E_EXTENSION))
		{
			if (ar.IsStoring())
			{
				SerializeW3E(TRUE);
				// peut-etre un try-except a faire ici
				int w = (int)m_fiW3E.GetLength();
				pBuf = new char[w];
				if (pBuf)
				{
					w = m_fiW3E.Read( pBuf, w);
					ar.Write(pBuf, w);
					delete []pBuf;
				}
			}
			else
			{
				// peut-etre un try-except a faire ici
				int w = (int)pFile->GetLength();
				char *pBuf;
				pBuf = new char[w];
				if (pBuf)
				{
					w = pFile->Read( pBuf, w);
					m_fiW3E.Write(pBuf, w);
					SerializeW3E(FALSE); // load the W3E
					delete []pBuf;
				}
			}
			// cree les autres fichiers en temporaire (?)
		}
// PUD format
		else if (!str.CompareNoCase(STR_PUD_EXTENSION))
		{
			if (ar.IsStoring())
			{
				W3Z_ERROR(STR_ERROR_SAVING02);
				m_bSaved = FALSE;
			}
			else
			{// loading
				CProgressWnd wndProgress(NULL, "Converting PUD, please wait...", TRUE);
				//wndProgress.GoModal(); // Call this if you want a modal window
				wndProgress.SetRange(0, 100);
				wndProgress.SetPos(0);
				wndProgress.SetText("Reading PUD...");

				W2PUDSECTIONHEADER tSecHeader;
				CWar3StrID tStrID;
				UINT uBytesRead;
				CByteArray abBuffer;
				byte* pbyBuffer = NULL;
				int i, iWidth, iHeight, ix, iy;
				float fMinX,fMaxX,fMinY,fMaxY;
				short si;
				BOOL bContinueRead = TRUE;
				BOOL bMapInited = FALSE;
				CWar3StrID tBlankStrID;
				int iTerrainType = 0;
				int iTileType = 0;
				CTilePoint tilepoint;
				CDoodad tDoodad;
				// init randomizer
				srand((unsigned)time(NULL));

				while (TRUE == bContinueRead)
				{
					uBytesRead = ar.Read(&tSecHeader, W2PUDSECTIONHEADER_LENGTH);
					if ((W2PUDSECTIONHEADER_LENGTH == uBytesRead) && (tSecHeader.iSectionSize < pFile->GetLength()))
					{
						try
						{
							abBuffer.SetSize(tSecHeader.iSectionSize);
							pbyBuffer = abBuffer.GetData();
							uBytesRead = ar.Read(pbyBuffer, tSecHeader.iSectionSize);
							// if all the data has been read, stop reading
							if (uBytesRead < tSecHeader.iSectionSize)
							{
								bContinueRead = FALSE;
							}
							// gets the ID
							tStrID = tSecHeader.acSectionID;
							// see what to do...
							if (tStrID == PUD_STRID_TYPE)
							{// pud type
								wndProgress.OffsetPos(10);
								wndProgress.PeekAndPump();
								wndProgress.SetText("Converting map type...");
								if (uBytesRead > strlen(PUD_MAPTYPE))
								{// buffer size ok
									if (0 != strncmp(PUD_MAPTYPE, (char*)pbyBuffer, strlen(PUD_MAPTYPE)))
									{// unrecognized type
										bContinueRead = FALSE;
									}
								}
								else
								{// unrecognized type
									bContinueRead = FALSE;
								}
							}
							else if (tStrID == PUD_STRID_VERSION)
							{// pud version
								wndProgress.OffsetPos(10);
								wndProgress.PeekAndPump();
								wndProgress.SetText("Checking version...");
								if (uBytesRead >= sizeof(short))
								{// buffer size ok, gets the version
									si = (*((short*)pbyBuffer));
									if (PUD_VERSION_DEFAULT != si)
									{// unrecognized version
										bContinueRead = FALSE;
									}
								}
								else
								{// invalid format
									bContinueRead = FALSE;
								}
							}
							else if (tStrID == PUD_STRID_DESCRIPTION)
							{
								wndProgress.OffsetPos(10);
								wndProgress.PeekAndPump();
								wndProgress.SetText("Converting map description...");
								if (uBytesRead >= 1)
								{// at least 1 char to read
									str = "";
									i = 0;
									while ((i < (signed)uBytesRead) && (0 != pbyBuffer[i]))
									{// copy the string
										str += (char) pbyBuffer[i];
										i++;
									}
									// adds the map description
									m_War3Map.mapW3Info.mapDescTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
								}
								else
								{// invalid format
									bContinueRead = FALSE;
								}
							}
							else if ((tStrID == PUD_STRID_TERRAIN) || (tStrID == PUD_STRID_TERRAINX))
							{// gets the map type
								wndProgress.OffsetPos(10);
								wndProgress.PeekAndPump();
								wndProgress.SetText("Converting terrain...");
								if (uBytesRead >= sizeof(short))
								{// buffer size ok
									si = (*((short*)pbyBuffer));
									if (PUD_TERRAIN_FOREST == si)
									{
										iTerrainType = PUD_TERRAIN_FOREST;
									}
									else if (PUD_TERRAIN_WINTER == si)
									{
										iTerrainType = PUD_TERRAIN_WINTER;
									}
									else if (PUD_TERRAIN_WASTELAND == si)
									{
										iTerrainType = PUD_TERRAIN_WASTELAND;
									}
									else if (PUD_TERRAIN_SWAMP == si)
									{
										iTerrainType = PUD_TERRAIN_SWAMP;
									}
									else
									{
										iTerrainType = PUD_TERRAIN_FOREST;
									}
									m_War3Map.mapMainTileset = pTheApp->m_AppSettings.m_acWar2MainTileset[iTerrainType];
									m_War3Map.mapW3Info.mapMainTileset = pTheApp->m_AppSettings.m_acWar2MainTileset[iTerrainType];
									m_War3Map.mapGroundTiles = pTheApp->m_AppSettings.m_astrWar2Tilesets[iTerrainType];
									m_War3Map.mapCliffTiles = pTheApp->m_AppSettings.m_astrWar2Cliffsets[iTerrainType];
									m_War3Map.mapCustomTilesets = 1;
								}
								else
								{// invalid format
									bContinueRead = FALSE;
								}
							}
							else if (tStrID == PUD_STRID_DIMENSIONS)
							{// gets map dimensions
								wndProgress.OffsetPos(10);
								wndProgress.PeekAndPump();
								wndProgress.SetText("Initializing map data...");

								if (uBytesRead >= 2*sizeof(short))
								{// buffer size ok
									iWidth = (*((short*)pbyBuffer));
									iHeight = (*((short*)(pbyBuffer+2)));
									m_War3Map.SetMapSize(iWidth, iHeight);
									m_War3Map.SetMapCenter((float)iWidth*-64, (float)iHeight*-64);
									//W3I
									m_War3Map.mapW3Info.mapEditVer = W3I_EDITOR_ID_W3Z;
									m_War3Map.mapW3Info.mapNbSaves = 0;
									m_War3Map.mapW3Info.mapAuthorTID = m_War3Map.mapTrigStringsTable.AddTrigString(W3I_STR_MAPAUTHOR_DEFAULT);
									m_War3Map.mapW3Info.mapNameTID = m_War3Map.mapTrigStringsTable.AddTrigString(pFile->GetFileTitle());
									m_War3Map.mapW3Info.mapSugPlyrTID = m_War3Map.mapTrigStringsTable.AddTrigString(W3I_STR_MAPSUGPLYR_DEFAULT);
									m_War3Map.mapW3Info.mapCameraBounds.fX1 = (float)iWidth*-64;
									m_War3Map.mapW3Info.mapCameraBounds.fY1 = (float)iHeight*-64;
									m_War3Map.mapW3Info.mapCameraBounds.fX2 = (float)iWidth*64;
									m_War3Map.mapW3Info.mapCameraBounds.fY2 = (float)iHeight*64;
									m_War3Map.mapW3Info.mapCameraBounds.fX3 = (float)iWidth*-64;
									m_War3Map.mapW3Info.mapCameraBounds.fY3 = (float)iHeight*64;
									m_War3Map.mapW3Info.mapCameraBounds.fX4 = (float)iWidth*64;
									m_War3Map.mapW3Info.mapCameraBounds.fY4 = (float)iHeight*-64;
									m_War3Map.mapW3Info.mapPlayableWidth = iWidth - 2;
									m_War3Map.mapW3Info.mapPlayableHeight = iHeight - 2;
									//map bounds
									m_War3Map.mapW3Info.mapLeftBoundWidth = 1;
									m_War3Map.mapW3Info.mapRightBoundWidth = 1;
									m_War3Map.mapW3Info.mapBottomBoundHeight = 1;
									m_War3Map.mapW3Info.mapTopBoundHeight = 1;
									//flags
									m_War3Map.mapW3Info.mapFlags = W3I_FLG_MASKEDAREA | W3I_FLG_MELEE | W3I_FLG_WAVESCLIFF | W3I_FLG_W3Z | W3I_FLG_WAR2CONV;
									// map Campaign background file path
									m_War3Map.mapW3Info.mapCampBackgrnd = -1;
									// map loading screen text
									m_War3Map.mapW3Info.mapLoadTextTID = -1;
									// map loading screen title
									m_War3Map.mapW3Info.mapLoadTitleTID = -1;
									// map loading screen subtitle
									m_War3Map.mapW3Info.mapLoadSubtitleTID = -1;
									// map loading screen glue file path
									m_War3Map.mapW3Info.mapLoadGlue = -1;
									// map prologue screen text
									m_War3Map.mapW3Info.mapProlTextTID = -1;
									// map prologue screen title
									m_War3Map.mapW3Info.mapProlTitleTID = -1;
									// map prologue screen subtitle
									m_War3Map.mapW3Info.mapProlSubtitleTID = -1;
									//max players
									m_War3Map.mapW3Info.mapNbMaxPlayers = W3I_NBPLAYERS_DEFAULT;

									for (i = 0; i < 12; i++)
									{
										m_War3Map.mapW3Info.mapPlayers[i].iUnknown00 = i;
										m_War3Map.mapW3Info.mapPlayers[i].type = 1; // human control
										m_War3Map.mapW3Info.mapPlayers[i].race = 1 + i%4;
										m_War3Map.mapW3Info.mapPlayers[i].fixedstart = 1;
										if (i < m_War3Map.mapW3Info.mapNbMaxPlayers)
										{
											str.Format(W3I_PLAYER_NAME_DEFAULT, i);
											m_War3Map.mapW3Info.mapPlayers[i].trigID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
										}
										else
										{
											m_War3Map.mapW3Info.mapPlayers[i].trigID = -1;
										}
										m_War3Map.mapW3Info.mapPlayers[i].startX = (float)4000. - (float)300.*i;
										m_War3Map.mapW3Info.mapPlayers[i].startY = (float)4000. - (float)300.*i;
										m_War3Map.mapW3Info.mapPlayers[i].allyLowPrio = 0;
										m_War3Map.mapW3Info.mapPlayers[i].allyHighPrio = 0;
									}

									m_War3Map.mapW3Info.mapNbMaxForces = 1;
									for (i = 0; i < 12; i++)
									{
										str.Format(W3I_FORCE_NAME_DEFAULT, i);
										m_War3Map.mapW3Info.mapForces[i].trigID =  m_War3Map.mapTrigStringsTable.AddTrigString(str);
										m_War3Map.mapW3Info.mapForces[i].iUnknown00 = 0;
										m_War3Map.mapW3Info.mapForces[i].iUnknown01 = 0;
										m_War3Map.mapW3Info.mapForces[i].number = i;
									}

									//DOO
									m_War3Map.mapUnknownDooInt = 07;

									// min and max coordinates for doodads
									fMinX = (float)(-64*iWidth + 64);
									fMaxX = (float)(64*iWidth - 64);
									fMinY = (float)(-64*iHeight + 64);
									fMaxY = (float)(64*iHeight - 64);
									m_War3Map.SetDOOUnsupportedBufferSize(8);
									byte* pbyBuffer2 = m_War3Map.GetDOOUnsupportedBuffer();
									for (i=0; i<8; i++)
									{
										pbyBuffer2[i] = 0;
									}

									bMapInited = TRUE;
								}
								else
								{// invalid format
									bContinueRead = FALSE;
								}
							}
							else if (tStrID == PUD_STRID_GROUNDMAP)
							{// gets ground data
								wndProgress.OffsetPos(10);
								wndProgress.PeekAndPump();
								wndProgress.SetText("Converting ground...");
								if (TRUE == bMapInited)
								{// the map has been inited and is not empty
									for (i = 0; i < (abBuffer.GetSize()/2); i++)
									{
										// 40% left
										if (0 == (i%(abBuffer.GetSize()/8)))
										{
											wndProgress.OffsetPos(5);
											wndProgress.PeekAndPump();
										}
										si = (*((short*)(pbyBuffer+2*i)));
										//pbyBuffer[si];
										ix = i%iWidth;
										iy = (iHeight-1) - (i/iWidth); // inverts Y
										if (0 == (si & PUD_MSK_BOUNDTYPE))
										{// plain tile
											if (PUD_LIGHTWATER == (si & PUD_MSK_TILETYPE))
											{
												iTileType = PUD_LIGHTWATER_INDEX;
											}
											else if (PUD_DARKWATER == (si & PUD_MSK_TILETYPE))
											{
												iTileType = PUD_DARKWATER_INDEX;
											}
											else if (PUD_LIGHTDIRT == (si & PUD_MSK_TILETYPE))
											{
												iTileType = PUD_LIGHTDIRT_INDEX;
											}
											else if (PUD_DARKDIRT == (si & PUD_MSK_TILETYPE))
											{
												iTileType = PUD_DARKDIRT_INDEX;
											}
											else if (PUD_LIGHTGROUND == (si & PUD_MSK_TILETYPE))
											{
												iTileType = PUD_LIGHTGROUND_INDEX;
											}
											else if (PUD_DARKGROUND == (si & PUD_MSK_TILETYPE))
											{
												iTileType = PUD_DARKGROUND_INDEX;
											}
											else if (PUD_FOREST == (si & PUD_MSK_TILETYPE))
											{
												iTileType = PUD_FOREST_INDEX;
											}
											else if (PUD_MOUNTAINS == (si & PUD_MSK_TILETYPE))
											{
												iTileType = PUD_MOUNTAINS_INDEX;
											}
											else if (PUD_HUMANWALL == (si & PUD_MSK_TILETYPE))
											{
												iTileType = PUD_HUMANWALL_INDEX;
											}
											else if (PUD_ORCWALL == (si & PUD_MSK_TILETYPE))
											{
												iTileType = PUD_ORCWALL_INDEX;
											}
											else if (PUD_HUMANWALL2 == (si & PUD_MSK_TILETYPE))
											{
												iTileType = PUD_HUMANWALL_INDEX;
											}
											else if (PUD_ORCWALL2 == (si & PUD_MSK_TILETYPE))
											{
												iTileType = PUD_ORCWALL_INDEX;
											}

											// sets the upper left tilepoint of the corresponding war3 tile
											pTheApp->m_AppSettings.m_atWar2Tilepoints[iTerrainType][iTileType].details = (byte)(((si & PUD_MSK_TILEDETAIL)%5) * 4);
											m_War3Map.SetTilePoint(ix, iy, pTheApp->m_AppSettings.m_atWar2Tilepoints[iTerrainType][iTileType]);
											// if we're on the last tile in War2, then also sets the upper right tilepoint
											if (ix == (iWidth - 1))
											{
												m_War3Map.SetTilePoint(ix+1, iy, pTheApp->m_AppSettings.m_atWar2Tilepoints[iTerrainType][iTileType]);
											}
											// if we're on the bottom part of the map, then also sets the bottom right tilepoint
											if (iy == (iHeight - 1))
											{
												m_War3Map.SetTilePoint(ix, iy+1, pTheApp->m_AppSettings.m_atWar2Tilepoints[iTerrainType][iTileType]);
												// and if we're on the last tile in War2, then also sets the bottom right tilepoint
												if (ix == (iWidth - 1))
												{
													m_War3Map.SetTilePoint(ix+1, iy+1, pTheApp->m_AppSettings.m_atWar2Tilepoints[iTerrainType][iTileType]);
												}
											}
											// check if there is a doodad to add
											if (tBlankStrID != pTheApp->m_AppSettings.m_astrWar2Doodads[iTerrainType][iTileType])
											{
												tDoodad.type = pTheApp->m_AppSettings.m_astrWar2Doodads[iTerrainType][iTileType];
												if ((PUD_HUMANWALL_INDEX != iTileType) && (PUD_ORCWALL_INDEX != iTileType))
												{
													tDoodad.x = (ix - (float)iWidth/(float)2) * 128 + rand()%64 - 32;
													tDoodad.y = (iy - (float)iHeight/(float)2) * 128 + rand()%64 - 32;
													tDoodad.z = 0;
													tDoodad.angle = (float)((((double)rand())/RAND_MAX)*6.2831853071);
													tDoodad.variation = rand()%8;
													tDoodad.scaleX = tDoodad.scaleY = (float)(1 + 0.4 * (((double)rand())/RAND_MAX) - 0.2);
													tDoodad.scaleZ = (float)(1 + 0.4 * (((double)rand())/RAND_MAX) - 0.2);
												}
												else
												{ // no random for walls
													tDoodad.x = (ix - (float)iWidth/2.0f) * 128.0f;
													tDoodad.y = (iy - (float)iHeight/2.0f) * 128.0f;
													tDoodad.z = 0;
													tDoodad.angle = 0;
													tDoodad.variation = 0;
													tDoodad.scaleX = tDoodad.scaleY = tDoodad.scaleZ = 1.0f;
												}
												// adds it if the doodad is inside the map dimensions
												if ((tDoodad.x >= fMinX) && (tDoodad.x <= fMaxX) && (tDoodad.y >= fMinY) && (tDoodad.y <= fMaxY))
												{
													m_War3Map.AddDoodad(tDoodad);
												}
											}
										}
										else
										{// boundary
											// add the position constant
											si += PUD_BOUND_POS_CONST;
											// use only the bottom left corner of war2 tiles to get the upper left tilepoint in War3 because of Y inversion
											if (0 != (si & PUD_BOUND_FILLED_BOTTOMLEFT))
											{// top left corner set
												if (PUD_BOUND_DARKLIGHTWATER == (si & PUD_MSK_BOUNDTYPE))
												{// dark water
													iTileType = PUD_DARKWATER_INDEX;
												}
												else if (PUD_BOUND_WATERDIRT == (si & PUD_MSK_BOUNDTYPE))
												{
													iTileType = PUD_LIGHTWATER_INDEX;
												}
												else if (PUD_BOUND_DARKLIGHTDIRT == (si & PUD_MSK_BOUNDTYPE))
												{
													iTileType = PUD_DARKDIRT_INDEX;
												}
												else if (PUD_BOUND_MOUNTDIRT == (si & PUD_MSK_BOUNDTYPE))
												{
													iTileType = PUD_MOUNTAINS_INDEX;
												}
												else if (PUD_BOUND_DIRTGRASS == (si & PUD_MSK_BOUNDTYPE))
												{
													iTileType = PUD_LIGHTDIRT_INDEX;
												}
												else if (PUD_BOUND_DARKLIGHTGRASS == (si & PUD_MSK_BOUNDTYPE))
												{
													iTileType = PUD_DARKGROUND_INDEX;
												}
												else if (PUD_BOUND_FORESTGRASS == (si & PUD_MSK_BOUNDTYPE))
												{
													iTileType = PUD_FOREST_INDEX;
												}
												else if (PUD_BOUND_HUMANWALL == (si & PUD_MSK_BOUNDTYPE))
												{
													iTileType = PUD_HUMANWALL_INDEX;
												}
												else if (PUD_BOUND_ORCWALL == (si & PUD_MSK_BOUNDTYPE))
												{
													iTileType = PUD_ORCWALL_INDEX;
												}
											}//if (0 != (si & PUD_BOUND_FILLED_BOTTOMLEFT))
											else
											{// bottom left corner using the tile complement
												if (PUD_BOUND_DARKLIGHTWATER == (si & PUD_MSK_BOUNDTYPE))
												{// dark water
													iTileType = PUD_LIGHTWATER_INDEX;
												}
												else if (PUD_BOUND_WATERDIRT == (si & PUD_MSK_BOUNDTYPE))
												{
													iTileType = PUD_LIGHTDIRT_INDEX;
												}
												else if (PUD_BOUND_DARKLIGHTDIRT == (si & PUD_MSK_BOUNDTYPE))
												{
													iTileType = PUD_LIGHTDIRT_INDEX;
												}
												else if (PUD_BOUND_MOUNTDIRT == (si & PUD_MSK_BOUNDTYPE))
												{
													iTileType = PUD_LIGHTDIRT_INDEX;
												}
												else if (PUD_BOUND_DIRTGRASS == (si & PUD_MSK_BOUNDTYPE))
												{
													iTileType = PUD_LIGHTGROUND_INDEX;
												}
												else if (PUD_BOUND_DARKLIGHTGRASS == (si & PUD_MSK_BOUNDTYPE))
												{
													iTileType = PUD_LIGHTGROUND_INDEX;
												}
												else if (PUD_BOUND_FORESTGRASS == (si & PUD_MSK_BOUNDTYPE))
												{
													iTileType = PUD_LIGHTGROUND_INDEX;
												}
												else if (PUD_BOUND_HUMANWALL == (si & PUD_MSK_BOUNDTYPE))
												{
													iTileType = PUD_HUMANWALL_INDEX;
												}
												else if (PUD_BOUND_ORCWALL == (si & PUD_MSK_BOUNDTYPE))
												{
													iTileType = PUD_ORCWALL_INDEX;
												}
											}//else of if (0 != (si & PUD_BOUND_FILLED_BOTTOMLEFT))
											// sets the top-left tilepoint
											pTheApp->m_AppSettings.m_atWar2Tilepoints[iTerrainType][iTileType].details = (byte)(((si & PUD_MSK_TILEDETAIL)%5) * 4);
											m_War3Map.SetTilePoint(ix, iy, pTheApp->m_AppSettings.m_atWar2Tilepoints[iTerrainType][iTileType]);
											// check if there is a doodad to add
											if (tBlankStrID != pTheApp->m_AppSettings.m_astrWar2Doodads[iTerrainType][iTileType])
											{
												tDoodad.type = pTheApp->m_AppSettings.m_astrWar2Doodads[iTerrainType][iTileType];
												if ((PUD_HUMANWALL_INDEX != iTileType) && (PUD_ORCWALL_INDEX != iTileType))
												{
													tDoodad.x = (ix - (float)iWidth/(float)2) * 128 + rand()%64 - 32;
													tDoodad.y = (iy - (float)iHeight/(float)2) * 128 + rand()%64 - 32;
													tDoodad.z = 0;
													tDoodad.angle = (float)((((double)rand())/RAND_MAX)*6.2831853071);
													tDoodad.variation = rand()%8;
													tDoodad.scaleX = tDoodad.scaleY = (float)(1 + 0.4 * (((double)rand())/RAND_MAX) - 0.2);
													tDoodad.scaleZ = (float)(1 + 0.4 * (((double)rand())/RAND_MAX) - 0.2);
												}
												else
												{ // no random for walls
													tDoodad.x = (ix - (float)iWidth/2.0f) * 128.0f;
													tDoodad.y = (iy - (float)iHeight/2.0f) * 128.0f;
													tDoodad.z = 0;
													tDoodad.angle = 0;
													tDoodad.variation = rand()%8;
													tDoodad.scaleX = tDoodad.scaleY = tDoodad.scaleZ = 1.0f;
												}
												if ((tDoodad.x >= fMinX) && (tDoodad.x <= fMaxX) && (tDoodad.y >= fMinY) && (tDoodad.y <= fMaxY))
												{
													m_War3Map.AddDoodad(tDoodad);
												}
											}
											// check now if we are on the max edges
											if (ix == (iWidth-1))
											{// only bottom right
												if (0 != (si & PUD_BOUND_FILLED_BOTTOMRIGHT))
												{
													if (PUD_BOUND_DARKLIGHTWATER == (si & PUD_MSK_BOUNDTYPE))
													{// dark water
														iTileType = PUD_DARKWATER_INDEX;
													}
													else if (PUD_BOUND_WATERDIRT == (si & PUD_MSK_BOUNDTYPE))
													{
														iTileType = PUD_LIGHTWATER_INDEX;
													}
													else if (PUD_BOUND_DARKLIGHTDIRT == (si & PUD_MSK_BOUNDTYPE))
													{
														iTileType = PUD_DARKDIRT_INDEX;
													}
													else if (PUD_BOUND_MOUNTDIRT == (si & PUD_MSK_BOUNDTYPE))
													{
														iTileType = PUD_MOUNTAINS_INDEX;
													}
													else if (PUD_BOUND_DIRTGRASS == (si & PUD_MSK_BOUNDTYPE))
													{
														iTileType = PUD_LIGHTDIRT_INDEX;
													}
													else if (PUD_BOUND_DARKLIGHTGRASS == (si & PUD_MSK_BOUNDTYPE))
													{
														iTileType = PUD_DARKGROUND_INDEX;
													}
													else if (PUD_BOUND_FORESTGRASS == (si & PUD_MSK_BOUNDTYPE))
													{
														iTileType = PUD_FOREST_INDEX;
													}
													else if (PUD_BOUND_HUMANWALL == (si & PUD_MSK_BOUNDTYPE))
													{
														iTileType = PUD_HUMANWALL_INDEX;
													}
													else if (PUD_BOUND_ORCWALL == (si & PUD_MSK_BOUNDTYPE))
													{
														iTileType = PUD_ORCWALL_INDEX;
													}
												}
												else
												{//complement
													if (PUD_BOUND_DARKLIGHTWATER == (si & PUD_MSK_BOUNDTYPE))
													{// dark water
														iTileType = PUD_LIGHTWATER_INDEX;
													}
													else if (PUD_BOUND_WATERDIRT == (si & PUD_MSK_BOUNDTYPE))
													{
														iTileType = PUD_LIGHTDIRT_INDEX;
													}
													else if (PUD_BOUND_DARKLIGHTDIRT == (si & PUD_MSK_BOUNDTYPE))
													{
														iTileType = PUD_LIGHTDIRT_INDEX;
													}
													else if (PUD_BOUND_MOUNTDIRT == (si & PUD_MSK_BOUNDTYPE))
													{
														iTileType = PUD_LIGHTDIRT_INDEX;
													}
													else if (PUD_BOUND_DIRTGRASS == (si & PUD_MSK_BOUNDTYPE))
													{
														iTileType = PUD_LIGHTGROUND_INDEX;
													}
													else if (PUD_BOUND_DARKLIGHTGRASS == (si & PUD_MSK_BOUNDTYPE))
													{
														iTileType = PUD_LIGHTGROUND_INDEX;
													}
													else if (PUD_BOUND_FORESTGRASS == (si & PUD_MSK_BOUNDTYPE))
													{
														iTileType = PUD_LIGHTGROUND_INDEX;
													}
													else if (PUD_BOUND_HUMANWALL == (si & PUD_MSK_BOUNDTYPE))
													{
														iTileType = PUD_HUMANWALL_INDEX;
													}
													else if (PUD_BOUND_ORCWALL == (si & PUD_MSK_BOUNDTYPE))
													{
														iTileType = PUD_ORCWALL_INDEX;
													}
												}
												// sets the top-right tilepoint
												pTheApp->m_AppSettings.m_atWar2Tilepoints[iTerrainType][iTileType].details = (byte)(((si & PUD_MSK_TILEDETAIL)%5) * 4);
												m_War3Map.SetTilePoint(ix+1, iy, pTheApp->m_AppSettings.m_atWar2Tilepoints[iTerrainType][iTileType]);
											}// if (ix == (iWidth-1))
											if (iy == (iHeight-1))
											{//only top left
												if (0 != (si & PUD_BOUND_FILLED_TOPLEFT))
												{
													if (PUD_BOUND_DARKLIGHTWATER == (si & PUD_MSK_BOUNDTYPE))
													{// dark water
														iTileType = PUD_DARKWATER_INDEX;
													}
													else if (PUD_BOUND_WATERDIRT == (si & PUD_MSK_BOUNDTYPE))
													{
														iTileType = PUD_LIGHTWATER_INDEX;
													}
													else if (PUD_BOUND_DARKLIGHTDIRT == (si & PUD_MSK_BOUNDTYPE))
													{
														iTileType = PUD_DARKDIRT_INDEX;
													}
													else if (PUD_BOUND_MOUNTDIRT == (si & PUD_MSK_BOUNDTYPE))
													{
														iTileType = PUD_MOUNTAINS_INDEX;
													}
													else if (PUD_BOUND_DIRTGRASS == (si & PUD_MSK_BOUNDTYPE))
													{
														iTileType = PUD_LIGHTDIRT_INDEX;
													}
													else if (PUD_BOUND_DARKLIGHTGRASS == (si & PUD_MSK_BOUNDTYPE))
													{
														iTileType = PUD_DARKGROUND_INDEX;
													}
													else if (PUD_BOUND_FORESTGRASS == (si & PUD_MSK_BOUNDTYPE))
													{
														iTileType = PUD_FOREST_INDEX;
													}
													else if (PUD_BOUND_HUMANWALL == (si & PUD_MSK_BOUNDTYPE))
													{
														iTileType = PUD_HUMANWALL_INDEX;
													}
													else if (PUD_BOUND_ORCWALL == (si & PUD_MSK_BOUNDTYPE))
													{
														iTileType = PUD_ORCWALL_INDEX;
													}
												}// if (0 != (si & PUD_BOUND_FILLED_TOPLEFT))
												else
												{//complement
													if (PUD_BOUND_DARKLIGHTWATER == (si & PUD_MSK_BOUNDTYPE))
													{// dark water
														iTileType = PUD_LIGHTWATER_INDEX;
													}
													else if (PUD_BOUND_WATERDIRT == (si & PUD_MSK_BOUNDTYPE))
													{
														iTileType = PUD_LIGHTDIRT_INDEX;
													}
													else if (PUD_BOUND_DARKLIGHTDIRT == (si & PUD_MSK_BOUNDTYPE))
													{
														iTileType = PUD_LIGHTDIRT_INDEX;
													}
													else if (PUD_BOUND_MOUNTDIRT == (si & PUD_MSK_BOUNDTYPE))
													{
														iTileType = PUD_LIGHTDIRT_INDEX;
													}
													else if (PUD_BOUND_DIRTGRASS == (si & PUD_MSK_BOUNDTYPE))
													{
														iTileType = PUD_LIGHTGROUND_INDEX;
													}
													else if (PUD_BOUND_DARKLIGHTGRASS == (si & PUD_MSK_BOUNDTYPE))
													{
														iTileType = PUD_LIGHTGROUND_INDEX;
													}
													else if (PUD_BOUND_FORESTGRASS == (si & PUD_MSK_BOUNDTYPE))
													{
														iTileType = PUD_LIGHTGROUND_INDEX;
													}
													else if (PUD_BOUND_HUMANWALL == (si & PUD_MSK_BOUNDTYPE))
													{
														iTileType = PUD_HUMANWALL_INDEX;
													}
													else if (PUD_BOUND_ORCWALL == (si & PUD_MSK_BOUNDTYPE))
													{
														iTileType = PUD_ORCWALL_INDEX;
													}
												}
												// sets bottom-left tilepoint
												pTheApp->m_AppSettings.m_atWar2Tilepoints[iTerrainType][iTileType].details = (byte)(((si & PUD_MSK_TILEDETAIL)%5) * 4);
												m_War3Map.SetTilePoint(ix, iy+1, pTheApp->m_AppSettings.m_atWar2Tilepoints[iTerrainType][iTileType]);
												if (ix == (iWidth-1))
												{// only top right
													if (0 != (si & PUD_BOUND_FILLED_TOPRIGHT))
													{
														if (PUD_BOUND_DARKLIGHTWATER == (si & PUD_MSK_BOUNDTYPE))
														{// dark water
															iTileType = PUD_DARKWATER_INDEX;
														}
														else if (PUD_BOUND_WATERDIRT == (si & PUD_MSK_BOUNDTYPE))
														{
															iTileType = PUD_LIGHTWATER_INDEX;
														}
														else if (PUD_BOUND_DARKLIGHTDIRT == (si & PUD_MSK_BOUNDTYPE))
														{
															iTileType = PUD_DARKDIRT_INDEX;
														}
														else if (PUD_BOUND_MOUNTDIRT == (si & PUD_MSK_BOUNDTYPE))
														{
															iTileType = PUD_MOUNTAINS_INDEX;
														}
														else if (PUD_BOUND_DIRTGRASS == (si & PUD_MSK_BOUNDTYPE))
														{
															iTileType = PUD_LIGHTDIRT_INDEX;
														}
														else if (PUD_BOUND_DARKLIGHTGRASS == (si & PUD_MSK_BOUNDTYPE))
														{
															iTileType = PUD_DARKGROUND_INDEX;
														}
														else if (PUD_BOUND_FORESTGRASS == (si & PUD_MSK_BOUNDTYPE))
														{
															iTileType = PUD_FOREST_INDEX;
														}
														else if (PUD_BOUND_HUMANWALL == (si & PUD_MSK_BOUNDTYPE))
														{
															iTileType = PUD_HUMANWALL_INDEX;
														}
														else if (PUD_BOUND_ORCWALL == (si & PUD_MSK_BOUNDTYPE))
														{
															iTileType = PUD_ORCWALL_INDEX;
														}
													}
													else
													{//complement
														if (PUD_BOUND_DARKLIGHTWATER == (si & PUD_MSK_BOUNDTYPE))
														{// dark water
															iTileType = PUD_LIGHTWATER_INDEX;
														}
														else if (PUD_BOUND_WATERDIRT == (si & PUD_MSK_BOUNDTYPE))
														{
															iTileType = PUD_LIGHTDIRT_INDEX;
														}
														else if (PUD_BOUND_DARKLIGHTDIRT == (si & PUD_MSK_BOUNDTYPE))
														{
															iTileType = PUD_LIGHTDIRT_INDEX;
														}
														else if (PUD_BOUND_MOUNTDIRT == (si & PUD_MSK_BOUNDTYPE))
														{
															iTileType = PUD_LIGHTDIRT_INDEX;
														}
														else if (PUD_BOUND_DIRTGRASS == (si & PUD_MSK_BOUNDTYPE))
														{
															iTileType = PUD_LIGHTGROUND_INDEX;
														}
														else if (PUD_BOUND_DARKLIGHTGRASS == (si & PUD_MSK_BOUNDTYPE))
														{
															iTileType = PUD_LIGHTGROUND_INDEX;
														}
														else if (PUD_BOUND_FORESTGRASS == (si & PUD_MSK_BOUNDTYPE))
														{
															iTileType = PUD_LIGHTGROUND_INDEX;
														}
														else if (PUD_BOUND_HUMANWALL == (si & PUD_MSK_BOUNDTYPE))
														{
															iTileType = PUD_HUMANWALL_INDEX;
														}
														else if (PUD_BOUND_ORCWALL == (si & PUD_MSK_BOUNDTYPE))
														{
															iTileType = PUD_ORCWALL_INDEX;
														}
													}
													// sets bottom-right tilepoint
													pTheApp->m_AppSettings.m_atWar2Tilepoints[iTerrainType][iTileType].details = (byte)(((si & PUD_MSK_TILEDETAIL)%5) * 4);
													m_War3Map.SetTilePoint(ix+1, iy+1, pTheApp->m_AppSettings.m_atWar2Tilepoints[iTerrainType][iTileType]);
												}// if (ix == (iWidth-1))
											}//if (iy == (iHeight-1))
										}//else of if (0 == (si & PUD_MSK_BOUNDTYPE))
									}//for (i = 0; i < (abBuffer.GetSize()/2); i++)
									//sets map boundaries
									for (i = 0; i <= iWidth; i++)
									{
										tilepoint = m_War3Map.GetTilePoint(i, 0);
										tilepoint.flags |= TILE_FL_SHADOW;
										m_War3Map.SetTilePoint(i, 0, tilepoint);
										tilepoint = m_War3Map.GetTilePoint(i, iHeight-1);
										tilepoint.flags |= TILE_FL_SHADOW;
										m_War3Map.SetTilePoint(i, iHeight-1, tilepoint);
										tilepoint = m_War3Map.GetTilePoint(i, iHeight);
										tilepoint.flags |= TILE_FL_SHADOW;
										m_War3Map.SetTilePoint(i, iHeight, tilepoint);
									}
									for (i = 0; i <= iHeight; i++)
									{
										tilepoint = m_War3Map.GetTilePoint(0, i);
										tilepoint.flags |= TILE_FL_SHADOW;
										m_War3Map.SetTilePoint(0, i, tilepoint);
										tilepoint = m_War3Map.GetTilePoint(iWidth-1, i);
										tilepoint.flags |= TILE_FL_SHADOW;
										m_War3Map.SetTilePoint(iWidth-1, i, tilepoint);
										tilepoint = m_War3Map.GetTilePoint(iWidth, i);
										tilepoint.flags |= TILE_FL_SHADOW;
										m_War3Map.SetTilePoint(iWidth, i, tilepoint);
									}
								}//if (TRUE == bMapInited)
								else
								{// map has not been inited before ground data, format not supported
									bContinueRead = FALSE;
								}
								m_War3Map.ResetPathing();
							}//else if (tStrID == PUD_STRID_GROUNDMAP)
						}
						catch (CMemoryException *e)
						{
							delete e;
						}
					}
					else
					{ // unable to read section --> no more bytes
						bContinueRead = FALSE;
					}

				}// while (TRUE == bContinueRead)			
				wndProgress.SetText("Done!");
				wndProgress.SetPos(100);
				wndProgress.PeekAndPump();
			}// else loading
		}
// SCM/SCX format
		else if (!str.CompareNoCase(STR_SCM_EXTENSION) || !str.CompareNoCase(STR_SCX_EXTENSION))
		{
			MPQHANDLE hMPQ;
			if (ar.IsStoring())
			{
				W3Z_ERROR(STR_ERROR_SAVING02);
				m_bSaved = FALSE;
			}
			else
			{
				const int SCM_RATIO_SAMEDIM	= 0;
				const int SCM_RATIO_ANGLES	= 1;
				const int SCM_RATIO_EVEN	= 2;
				const int SCM_RATIO_ODD		= 3;

				CSCMConversionDialog tSCMConversion;
				tSCMConversion.m_bSmooth = TRUE;
				tSCMConversion.m_iRatio = 0;
				tSCMConversion.DoModal();

				CProgressWnd wndProgress(NULL, "Loading map, please wait...", TRUE);
				wndProgress.GoModal(); // Call this if you want a modal window
				wndProgress.SetRange(0, 100);
				wndProgress.SetPos(0);
				wndProgress.SetText("Reading SCM...");

				if ((TRUE ==pTheApp->IsMPQInit()) && SFileOpenArchive(sFileName, 0, 0, &hMPQ))
				{// initialisation ok
					// save the original MPQ filename
					m_strOriginalMPQFilename = sFileName;

					// creates a temporary file
					OpenTemporaryFile(m_fiCHK);

					wndProgress.OffsetPos(5);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Processing data...");


					// CHK
					//*****
					if (ReadEmbededFile(hMPQ, STR_SC_SCENARIO, m_fiCHK))
					{
						// CHK conversion code goes here
						// size of the ISOM:
						// 4bytes ID + 4bytes section length + section_length bytes
						// section length= (width/2+1)*(height+1)*8
						SCCHKSECTIONHEADER tSecHeader;
						CWar3StrID tStrID;
						UINT uBytesRead;
						CByteArray abBuffer;
						byte* pbyBuffer = NULL;
						int i, iWidth, iHeight, ix, iy, iRotateWidth, iRotateHeight;
						float fMinX,fMaxX,fMinY,fMaxY;
						short si;
						BOOL bContinueRead = TRUE;
						BOOL bMapInited = FALSE;
						CTilePoint tTilepoint;
						// transition tiles conversion table
						int iTerrainType = 0;
						const int *paiTilesConversionTable;
						CWar3StrID tBlankStrID;
						CDoodad tDoodad;
						// init randomizer
						srand((unsigned)time(NULL));

						while (TRUE == bContinueRead)
						{
							uBytesRead = m_fiCHK.Read(&tSecHeader, SCCHKSECTIONHEADER_LENGTH);
							if ((SCCHKSECTIONHEADER_LENGTH == uBytesRead) && (tSecHeader.iSectionSize < m_fiCHK.GetLength()))
							{
								try
								{
									abBuffer.SetSize(tSecHeader.iSectionSize);
									pbyBuffer = abBuffer.GetData();
									uBytesRead = m_fiCHK.Read(pbyBuffer, tSecHeader.iSectionSize);
									// if all the data has been read, stop reading
									if (uBytesRead < tSecHeader.iSectionSize)
									{
										bContinueRead = FALSE;
									}
									// gets the ID
									tStrID = tSecHeader.acSectionID;
									// see what to do...
									if (tStrID == SCM_STRID_TERRAIN)
									{// scm ground type
										wndProgress.OffsetPos(5);
										wndProgress.PeekAndPump();
										wndProgress.SetText("Converting terrain type...");
										if (uBytesRead >= sizeof(short))
										{// buffer size ok
											si = (*((short*)pbyBuffer));
											if (SCM_TERRAIN_BADLANDS == si)
											{
												iTerrainType = SCM_TERRAIN_BADLANDS;
											}
											else if (SCM_TERRAIN_SPACE == si)
											{
												iTerrainType = SCM_TERRAIN_SPACE;
											}
											else if (SCM_TERRAIN_INSTALLATION == si)
											{
												iTerrainType = SCM_TERRAIN_INSTALLATION;
											}
											else if (SCM_TERRAIN_ASHWORLD == si)
											{
												iTerrainType = SCM_TERRAIN_ASHWORLD;
											}
											else if (SCM_TERRAIN_JUNGLE == si)
											{
												iTerrainType = SCM_TERRAIN_JUNGLE;
											}
											else if (SCM_TERRAIN_DESERT == si)
											{
												iTerrainType = SCM_TERRAIN_DESERT;
											}
											else if (SCM_TERRAIN_ARCTIC == si)
											{
												iTerrainType = SCM_TERRAIN_ARCTIC;
											}
											else if (SCM_TERRAIN_TWILIGHT == si)
											{
												iTerrainType = SCM_TERRAIN_TWILIGHT;
											}
											else
											{
												iTerrainType = SCM_TERRAIN_JUNGLE;
											}

											// init lookup table
											if (TRUE == tSCMConversion.m_bSmooth)
											{
												paiTilesConversionTable = (const int *)(SC_TILESTABLES_SMOOTHED[iTerrainType]);
											}
											else
											{
												paiTilesConversionTable = (const int *)(SC_TILESTABLES[iTerrainType]);
											}
											m_War3Map.mapMainTileset = pTheApp->m_AppSettings.m_acSCMainTileset[iTerrainType];
											m_War3Map.mapW3Info.mapMainTileset = pTheApp->m_AppSettings.m_acSCMainTileset[iTerrainType];
											m_War3Map.mapGroundTiles = pTheApp->m_AppSettings.m_astrSCTilesets[iTerrainType];
											m_War3Map.mapCliffTiles = pTheApp->m_AppSettings.m_astrSCCliffsets[iTerrainType];
											m_War3Map.mapCustomTilesets = 1;

										}
										else
										{// invalid format
											bContinueRead = FALSE;
										}
										wndProgress.OffsetPos(10);
										wndProgress.PeekAndPump();
									}
									else if (tStrID == SCM_STRID_VERSION)
									{// pud version
										wndProgress.SetText("Checking version...");
										if (uBytesRead >= sizeof(short))
										{// buffer size ok, gets the version
											si = (*((short*)pbyBuffer));
											//+FIXME: compatible version check
										}
										else
										{// invalid format
											bContinueRead = FALSE;
										}
										wndProgress.OffsetPos(10);
										wndProgress.PeekAndPump();
									}
									else if (tStrID == SCM_STRID_DIMENSIONS)
									{
										wndProgress.SetText("Initializing map data and size...");
										if (uBytesRead >= 2*sizeof(short))
										{// buffer size ok
											if (SCM_RATIO_ANGLES == tSCMConversion.m_iRatio)
											{
												iWidth = (*((short*)pbyBuffer))/2;
												iHeight = (*((short*)(pbyBuffer+2)));
												if (TRUE == tSCMConversion.m_bRotate45Deg)
												{ // rotation
													// w'=w+h
													// h'=h+w
													iRotateWidth = iWidth;
													iRotateHeight = iHeight;
													iWidth = iRotateWidth + iRotateHeight; // rounded
													iHeight = iRotateHeight + iRotateWidth; // rounded
												}
											}
											else if (SCM_RATIO_SAMEDIM == tSCMConversion.m_iRatio)
											{
												iWidth = (*((short*)pbyBuffer));
												iHeight = (*((short*)(pbyBuffer+2)));
											}
											else
											{ // reduce by half
												iWidth = (*((short*)pbyBuffer))/2;
												iHeight = (*((short*)(pbyBuffer+2)))/2;
											}
											m_War3Map.SetMapSize(iWidth, iHeight);
											m_War3Map.SetMapCenter((float)iWidth*-64, (float)iHeight*-64);

											wndProgress.OffsetPos(5);
											wndProgress.PeekAndPump();
											wndProgress.SetText("Initializing map data and size... (W3I)");
											//W3I
											m_War3Map.mapW3Info.mapEditVer = W3I_EDITOR_ID_W3Z;
											m_War3Map.mapW3Info.mapNbSaves = 0;
											m_War3Map.mapW3Info.mapAuthorTID = m_War3Map.mapTrigStringsTable.AddTrigString(W3I_STR_MAPAUTHOR_DEFAULT);
											m_War3Map.mapW3Info.mapNameTID = m_War3Map.mapTrigStringsTable.AddTrigString(pFile->GetFileTitle());
											m_War3Map.mapW3Info.mapSugPlyrTID = m_War3Map.mapTrigStringsTable.AddTrigString(W3I_STR_MAPSUGPLYR_DEFAULT);
											m_War3Map.mapW3Info.mapCameraBounds.fX1 = (float)iWidth*-64;
											m_War3Map.mapW3Info.mapCameraBounds.fY1 = (float)iHeight*-64;
											m_War3Map.mapW3Info.mapCameraBounds.fX2 = (float)iWidth*64;
											m_War3Map.mapW3Info.mapCameraBounds.fY2 = (float)iHeight*64;
											m_War3Map.mapW3Info.mapCameraBounds.fX3 = (float)iWidth*-64;
											m_War3Map.mapW3Info.mapCameraBounds.fY3 = (float)iHeight*64;
											m_War3Map.mapW3Info.mapCameraBounds.fX4 = (float)iWidth*64;
											m_War3Map.mapW3Info.mapCameraBounds.fY4 = (float)iHeight*-64;
											m_War3Map.mapW3Info.mapPlayableWidth = iWidth - 2;
											m_War3Map.mapW3Info.mapPlayableHeight = iHeight - 2;
											//map bounds
											m_War3Map.mapW3Info.mapLeftBoundWidth = 1;
											m_War3Map.mapW3Info.mapRightBoundWidth = 1;
											m_War3Map.mapW3Info.mapBottomBoundHeight = 1;
											m_War3Map.mapW3Info.mapTopBoundHeight = 1;
											//flags
											m_War3Map.mapW3Info.mapFlags = W3I_FLG_MASKEDAREA | W3I_FLG_MELEE | W3I_FLG_WAVESCLIFF | W3I_FLG_W3Z | W3I_FLG_WAR2CONV;
											// map Campaign background file path
											m_War3Map.mapW3Info.mapCampBackgrnd = -1;
											// map loading screen text
											m_War3Map.mapW3Info.mapLoadTextTID = -1;
											// map loading screen title
											m_War3Map.mapW3Info.mapLoadTitleTID = -1;
											// map loading screen subtitle
											m_War3Map.mapW3Info.mapLoadSubtitleTID = -1;
											// map loading screen glue file path
											m_War3Map.mapW3Info.mapLoadGlue = -1;
											// map prologue screen text
											m_War3Map.mapW3Info.mapProlTextTID = -1;
											// map prologue screen title
											m_War3Map.mapW3Info.mapProlTitleTID = -1;
											// map prologue screen subtitle
											m_War3Map.mapW3Info.mapProlSubtitleTID = -1;
											//max players
											m_War3Map.mapW3Info.mapNbMaxPlayers = W3I_NBPLAYERS_DEFAULT;

											for (i = 0; i < 12; i++)
											{
												m_War3Map.mapW3Info.mapPlayers[i].iUnknown00 = i;
												m_War3Map.mapW3Info.mapPlayers[i].type = 1; // human control
												m_War3Map.mapW3Info.mapPlayers[i].race = 1 + i%4;
												m_War3Map.mapW3Info.mapPlayers[i].fixedstart = 1;
												if (i < m_War3Map.mapW3Info.mapNbMaxPlayers)
												{
													str.Format(W3I_PLAYER_NAME_DEFAULT, i);
													m_War3Map.mapW3Info.mapPlayers[i].trigID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
												}
												else
												{
													m_War3Map.mapW3Info.mapPlayers[i].trigID = -1;
												}
												m_War3Map.mapW3Info.mapPlayers[i].startX = (float)4000. - (float)300.*i;
												m_War3Map.mapW3Info.mapPlayers[i].startY = (float)4000. - (float)300.*i;
												m_War3Map.mapW3Info.mapPlayers[i].allyLowPrio = 0;
												m_War3Map.mapW3Info.mapPlayers[i].allyHighPrio = 0;
											}

											m_War3Map.mapW3Info.mapNbMaxForces = 1;
											for (i = 0; i < 12; i++)
											{
												str.Format(W3I_FORCE_NAME_DEFAULT, i);
												m_War3Map.mapW3Info.mapForces[i].trigID =  m_War3Map.mapTrigStringsTable.AddTrigString(str);
												m_War3Map.mapW3Info.mapForces[i].iUnknown00 = 0;
												m_War3Map.mapW3Info.mapForces[i].iUnknown01 = 0;
												m_War3Map.mapW3Info.mapForces[i].number = i;
											}

											wndProgress.OffsetPos(5);
											wndProgress.PeekAndPump();
											wndProgress.SetText("Initializing map data and size... (DOO)");
											//DOO
											m_War3Map.mapUnknownDooInt = 07;

											// min and max coordinates for doodads
											fMinX = (float)(-64*iWidth + 64);
											fMaxX = (float)(64*iWidth - 64);
											fMinY = (float)(-64*iHeight + 64);
											fMaxY = (float)(64*iHeight - 64);
											m_War3Map.SetDOOUnsupportedBufferSize(8);
											byte* pbyBuffer2 = m_War3Map.GetDOOUnsupportedBuffer();
											for (i=0; i<8; i++)
											{
												pbyBuffer2[i] = 0;
											}

											if ((SCM_RATIO_ANGLES == tSCMConversion.m_iRatio)
												&& (TRUE == tSCMConversion.m_bRotate45Deg))
											{
												CTilePoint tBoundary = pTheApp->m_AppSettings.m_atSCTilepoints[iTerrainType][1];
												tBoundary.flags |= TILE_FL_SHADOW | TILE_FL_MINISHAD;

												for (i=0; i<=iWidth; i++)
												{
													for (int j=0; j<=iHeight; j++)
													{
														m_War3Map.SetTilePoint(i, j, tBoundary);
													}
												}
												iWidth = iRotateWidth;
												iHeight = iRotateHeight;
											}

											bMapInited = TRUE;
										}
										else
										{// invalid format
											bContinueRead = FALSE;
										}
										wndProgress.OffsetPos(10);
										wndProgress.PeekAndPump();
									}
									else if (tStrID == SCM_STRID_ISOM)
									{// gets ground data
										wndProgress.SetText("Converting ground...");
										int iIsomDataLow;//, iIsomDataHigh;
										int iTileType = 0;
										short **asTilesMap;

										if (TRUE == bMapInited)
										{// the map has been inited and is not empty
											// creates a temporary tile map of shorts
											asTilesMap = new short*[iHeight+1];
											for (i = 0; i <= iHeight; i++)
											{
												asTilesMap[i] = new short[iWidth+1];
											}
											// initialize the conversion tile map
											for (iy = 0; iy <= iHeight; iy++)
											{
												for (ix = 0; ix <= iWidth; ix++)
												{
													// get the tile in the SC map using the specified method
													if (SCM_RATIO_ANGLES == tSCMConversion.m_iRatio)
													{
														iIsomDataLow = *((int*)( pbyBuffer + 8*(ix + (iHeight-iy)*(iWidth+1)) ));
														//iIsomDataHigh = *((int*)( pbyBuffer + 4 + 8*(ix + (iHeight+1-iy)*(iWidth+1)) ));
														iTileType = (iIsomDataLow & SCM_MSK_TILETYPE1) >> SCM_MSK_TILETYPE1_SHIFT;
													}
													else if (SCM_RATIO_SAMEDIM == tSCMConversion.m_iRatio)
													{
														iIsomDataLow = *((int*)( pbyBuffer + 8*(ix/2 + (iHeight-iy)*(iWidth/2+1)) ));
														if (ix%2)
														{ // diamond bottom point
															iTileType = (iIsomDataLow & SCM_MSK_TILETYPE2) >> SCM_MSK_TILETYPE2_SHIFT;
														}
														else
														{ // diamonds left point
															iTileType = (iIsomDataLow & SCM_MSK_TILETYPE1);
															iTileType >>= SCM_MSK_TILETYPE1_SHIFT;
														}
													}
													else if (SCM_RATIO_ODD == tSCMConversion.m_iRatio)
													{ // odd
														iIsomDataLow = *((int*)( pbyBuffer + 8*(ix + 1+2*(iHeight-iy)*(iWidth+1)) ));
														iTileType = (iIsomDataLow & SCM_MSK_TILETYPE1) >> SCM_MSK_TILETYPE1_SHIFT;
													}
													else if (SCM_RATIO_EVEN == tSCMConversion.m_iRatio)
													{ // even
														iIsomDataLow = *((int*)( pbyBuffer + 8*(ix + 2*(iHeight-iy)*(iWidth+1)) ));
														iTileType = (iIsomDataLow & SCM_MSK_TILETYPE1) >> SCM_MSK_TILETYPE1_SHIFT;
													}
													// convert the tile
													asTilesMap[iy][ix] = paiTilesConversionTable[iTileType];
												}
											}

											wndProgress.OffsetPos(25);
											wndProgress.PeekAndPump();
											// initialize the War3 tile map
											for (iy = 0; iy <= iHeight; iy++)
											{
												for (ix = 0; ix <= iWidth; ix++)
												{
													if ((SCM_RATIO_ANGLES == tSCMConversion.m_iRatio)
														&& (TRUE == tSCMConversion.m_bRotate45Deg))
													{ // rotation
														// x'=x+y
														// y'=y-x offset to have >0 values
														m_War3Map.SetTilePoint(ix+iy, iy-ix+iWidth, pTheApp->m_AppSettings.m_atSCTilepoints[iTerrainType][asTilesMap[iy][ix]]);
														if (tBlankStrID != pTheApp->m_AppSettings.m_astrSCDoodads[iTerrainType][asTilesMap[iy][ix]])
														{
															tDoodad.type = pTheApp->m_AppSettings.m_astrSCDoodads[iTerrainType][asTilesMap[iy][ix]];
															tDoodad.x = (ix+iy - (iWidth+iHeight)/2.0f) * 128.0f + rand()%64 - 32;
															tDoodad.y = (iy-ix+iWidth - (iWidth+iHeight)/2.0f) * 128.0f + rand()%64 - 32;
															tDoodad.z = 0;
															tDoodad.angle = (float)((((double)rand())/RAND_MAX)*6.2831853071);
															tDoodad.variation = rand()%8;
															tDoodad.scaleX = tDoodad.scaleY = (float)(1 + 0.4 * (((double)rand())/RAND_MAX) - 0.2);
															tDoodad.scaleZ = (float)(1 + 0.4 * (((double)rand())/RAND_MAX) - 0.2);
															if ((tDoodad.x >= fMinX) && (tDoodad.x <= fMaxX) && (tDoodad.y >= fMinY) && (tDoodad.y <= fMaxY))
															{
																m_War3Map.AddDoodad(tDoodad);
															}
														}
														if ((ix < iWidth) && (iy < iHeight))
														{
															if (asTilesMap[iy][ix] == asTilesMap[iy+1][ix+1])
															{ // detected "\" diagonal
																m_War3Map.SetTilePoint(ix+iy+1, iy-ix+iWidth, pTheApp->m_AppSettings.m_atSCTilepoints[iTerrainType][asTilesMap[iy][ix]]);
															}
															else
															{ // otherwise treats it as a "/" diagonal
																m_War3Map.SetTilePoint(ix+iy+1, iy-ix+iWidth, pTheApp->m_AppSettings.m_atSCTilepoints[iTerrainType][asTilesMap[iy][ix+1]]);
															}
														}
													}
													else
													{
														m_War3Map.SetTilePoint(ix, iy, pTheApp->m_AppSettings.m_atSCTilepoints[iTerrainType][asTilesMap[iy][ix]]);
														if (tBlankStrID != pTheApp->m_AppSettings.m_astrSCDoodads[iTerrainType][asTilesMap[iy][ix]])
														{
															tDoodad.type = pTheApp->m_AppSettings.m_astrSCDoodads[iTerrainType][asTilesMap[iy][ix]];
															tDoodad.x = (ix - (float)iWidth/(float)2) * 128 + rand()%64 - 32;
															tDoodad.y = (iy - (float)iHeight/(float)2) * 128 + rand()%64 - 32;
															tDoodad.z = 0;
															tDoodad.angle = (float)((((double)rand())/RAND_MAX)*6.2831853071);
															tDoodad.variation = rand()%8;
															tDoodad.scaleX = tDoodad.scaleY = (float)(1 + 0.4 * (((double)rand())/RAND_MAX) - 0.2);
															tDoodad.scaleZ = (float)(1 + 0.4 * (((double)rand())/RAND_MAX) - 0.2);
															if ((tDoodad.x >= fMinX) && (tDoodad.x <= fMaxX) && (tDoodad.y >= fMinY) && (tDoodad.y <= fMaxY))
															{
																m_War3Map.AddDoodad(tDoodad);
															}
														}
													}
												}
											}
											wndProgress.OffsetPos(15);
											wndProgress.PeekAndPump();
											if (!((SCM_RATIO_ANGLES == tSCMConversion.m_iRatio)
												&& (TRUE == tSCMConversion.m_bRotate45Deg)))
											{
												// boundaries
												for (iy = 0; iy <= iHeight; iy++)
												{
													tTilepoint = m_War3Map.GetTilePoint(iWidth, iy);
													tTilepoint.flags |= TILE_FL_SHADOW | TILE_FL_MINISHAD;
													m_War3Map.SetTilePoint(iWidth, iy, tTilepoint);
												}
												// boundaries
												for (ix = 0; ix <= iWidth; ix++)
												{
													tTilepoint = m_War3Map.GetTilePoint(ix, 0);
													tTilepoint.flags |= TILE_FL_SHADOW | TILE_FL_MINISHAD;
													m_War3Map.SetTilePoint(ix, 0, tTilepoint);
												}
											}

											for (i = iHeight; i >= 0; i--)
											{
												delete[] (asTilesMap[i]);
											}
											delete[] asTilesMap;
											m_War3Map.ResetPathing();
										}//if (TRUE == bMapInited)
										else
										{// map has not been inited before ground data, format not supported
											bContinueRead = FALSE;
										}
										wndProgress.OffsetPos(10);
										wndProgress.PeekAndPump();
									}
								}
								catch (CMemoryException *e)
								{
									delete e;
								}
							}
							else
							{ // unable to read section --> no more bytes
								bContinueRead = FALSE;
							}
						}// while (TRUE == bContinueRead)			
						wndProgress.SetText("Done!");
						wndProgress.SetPos(100);
						wndProgress.PeekAndPump();
					}
					else
					{
						//Failed
					}
				}
				CloseAndDeleteFile(m_fiCHK); // free temporary extracted CHK file
			}
		}
		else /*if (!str.CompareNoCase(STR_W3M_EXTENSION))*/
		{
			//w3m
			MPQHANDLE hMPQ, hMPQOriginal;
			CW3ZMapEditApp *pTheApp;
			pTheApp = (CW3ZMapEditApp *)AfxGetApp();
			CString srtSaveError = "";
			//check store status and if the map is readonly
			if ((TRUE == ar.IsStoring()) && (FALSE == IsReadOnly()))
			{
				CProgressWnd wndProgress(NULL, "Saving map, please wait...", TRUE);
				// wndProgress.GoModal(); // Call this if you want a modal window
				wndProgress.SetRange(0, 100);
				wndProgress.SetPos(0);
				wndProgress.PeekAndPump();
				wndProgress.SetText("Creating a new W3M file...");

				if (pTheApp->IsMPQInit())
				{
					// initialisation ok
					CFile newW3M; // temporary MPQ file
					CString W3Mfilename, EmbFileName;

					OpenTemporaryFile(newW3M); // create a temporary MPQ file
					W3Mfilename = newW3M.GetFilePath(); // get the filename
					newW3M.Close(); // close it to allow SFMPQAPI to open it
					hMPQ = MpqOpenArchiveForUpdate(W3Mfilename, MOAU_CREATE_ALWAYS | MOAU_MAINTAIN_LISTFILE, MPQ_MAX_FILES);

					// WTS file must be read before W3I and JASS
					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Saving trigger strings...");
					// WTS
					//*****
					// update, get name, close, add to MPQ, and re-open WTS file
					SerializeWTS(TRUE);
					if ((CFile::hFileNull != m_fiWTS.m_hFile) && (m_fiWTS.GetLength() > 0))
					{
						EmbFileName = m_fiWTS.GetFilePath();
						m_fiWTS.Close();
						pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_WTS); // check save plugin
						if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_WTS))
						{
							srtSaveError = srtSaveError + STR_W3M_WTS + "\n";
						}
						m_fiWTS.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
					}

					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Saving ground...");
					// W3E
					//*****
					// update, get name, close, add to MPQ, and re-open W3E file
					SerializeW3E(TRUE);
					if ((CFile::hFileNull != m_fiW3E.m_hFile) && (m_fiW3E.GetLength() > 0))
					{
						EmbFileName = m_fiW3E.GetFilePath();
						m_fiW3E.Close();
						pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_W3E); // check save plugin
						if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_W3E))
						{
							srtSaveError = srtSaveError + STR_W3M_W3E + "\n";
						}
						m_fiW3E.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
					}

					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Saving map info...");
					// W3I
					//*****
					// update, get name, close, add to MPQ, and re-open W3I file
					SerializeW3I(TRUE);
					if ((CFile::hFileNull != m_fiW3I.m_hFile) && (m_fiW3I.GetLength() > 0))
					{
						EmbFileName = m_fiW3I.GetFilePath();
						m_fiW3I.Close();
						pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_W3I); // check save plugin
						if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_W3I))
						{
							srtSaveError = srtSaveError + STR_W3M_W3I + "\n";
						}
						m_fiW3I.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
					}

					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Saving doodads...");
					// DOO
					//*****
					// update, get name, close, add to MPQ, and re-open DOO file
					SerializeDOO(TRUE);
					if ((CFile::hFileNull != m_fiDOO.m_hFile) && (m_fiDOO.GetLength() > 0))
					{
						EmbFileName = m_fiDOO.GetFilePath();
						m_fiDOO.Close();
						pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_DOO); // check save plugin
						if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_DOO))
						{
							srtSaveError = srtSaveError + STR_W3M_DOO + "\n";
						}
						m_fiDOO.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
					}

					wndProgress.OffsetPos(10);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Saving JASS script...");
					// JASS
					//******
					// update, get name, close, add to MPQ, and re-open JASS file
					SerializeJASS(TRUE);
					if ((CFile::hFileNull != m_fiJASS.m_hFile) && (m_fiJASS.GetLength() > 0))
					{
						EmbFileName = m_fiJASS.GetFilePath();
						m_fiJASS.Close();
						// check if a protection is set above level 1
						if ((TRUE == m_bEnableProtect)
							&& ((W3Z_MAP_PROT_W3ZV1L3 == GetProtection()) || (W3Z_MAP_PROT_W3ZV1L2 == GetProtection())))
						{// if so, "hide" the script
							CString strJassPath;
							strJassPath = STR_W3M_SCRIPTS;
							strJassPath += STR_W3M_JASS;
							pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_JASS); // check save plugin
							if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, strJassPath))
							{
								srtSaveError = srtSaveError + STR_W3M_JASS + "\n";
							}
						}
						else
						{// otherwise just leave it alone
							pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_JASS); // check save plugin
							if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_JASS))
							{
								srtSaveError = srtSaveError + STR_W3M_JASS + "\n";
							}
						}
						m_fiJASS.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
					}
					else
					{
						EmbFileName = pTheApp->GetAppPath() + STR_W3Z_MAP_FILESDIR + STR_W3M_JASS;
						// check if we need a clean new file
						// Note: because of previous test, if the file exists, its size is <= 0
						if (CFile::hFileNull == m_fiJASS.m_hFile)
						{
							// creates a new file
							OpenTemporaryFile(m_fiJASS);
						}
						// opens original STR_W3Z_MAP_FILESDIR file
						fiTempFile.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
						if (CFile::hFileNull != fiTempFile)
						{
							int w = (int)fiTempFile.GetLength();
							pBuf = new char[w];
							if (pBuf)
							{
								w = fiTempFile.Read(pBuf, w);
								m_fiJASS.Write(pBuf, w);
								delete []pBuf;
							}
							fiTempFile.Close();
							EmbFileName = m_fiJASS.GetFilePath();
							m_fiJASS.Close();
							pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_JASS); // check save plugin
							if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_JASS))
							{
								srtSaveError = srtSaveError + STR_W3M_JASS + "\n";
							}
							m_fiJASS.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
						}
					}

					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Saving minimap info...");
					// MMP
					//*****
					// update, get name, close, add to MPQ, and re-open MMP file
					SerializeMMP(TRUE);
					if ((CFile::hFileNull != m_fiMMP.m_hFile) && (m_fiMMP.GetLength() > 0))
					{
						EmbFileName = m_fiMMP.GetFilePath();
						m_fiMMP.Close();
						pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_MMP); // check save plugin
						if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_MMP))
						{
							srtSaveError = srtSaveError + STR_W3M_MMP + "\n";
						}
						m_fiMMP.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
					}
					else
					{
						EmbFileName = pTheApp->GetAppPath() + STR_W3Z_MAP_FILESDIR + STR_W3M_MMP;
						// check if we need a clean new file
						// Note: because of previous test, if the file exists, its size is <= 0
						if (CFile::hFileNull == m_fiMMP.m_hFile)
						{
							// creates a new file
							OpenTemporaryFile(m_fiMMP);
						}
						// opens original STR_W3Z_MAP_FILESDIR file
						fiTempFile.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
						if (CFile::hFileNull != fiTempFile)
						{
							int w = (int)fiTempFile.GetLength();
							pBuf = new char[w];
							if (pBuf)
							{
								w = fiTempFile.Read(pBuf, w);
								m_fiMMP.Write(pBuf, w);
								delete []pBuf;
							}
							fiTempFile.Close();
							EmbFileName = m_fiMMP.GetFilePath();
							m_fiMMP.Close();
							pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_MMP); // check save plugin
							if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_MMP))
							{
								srtSaveError = srtSaveError + STR_W3M_MMP + "\n";
							}
							m_fiMMP.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
						}
					}

					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Saving shadows...");
					// SHD
					//*****
					// update, get name, close, add to MPQ, and re-open SHD file
					SerializeSHD(TRUE);
					if ((CFile::hFileNull != m_fiSHD.m_hFile) && (m_fiSHD.GetLength() > 0))
					{
						EmbFileName = m_fiSHD.GetFilePath();
						m_fiSHD.Close();
						pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_SHD); // check save plugin
						if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_SHD))
						{
							srtSaveError = srtSaveError + STR_W3M_SHD + "\n";
						}
						m_fiSHD.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
					}

					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Saving cameras...");
					// W3C
					//*****
					if ((TRUE == m_bEnableProtect)
						&& (W3Z_MAP_PROT_W3ZV1L3 == GetProtection()))
					{
						// map protected level 3 --> a file with the null size instead
						EmbFileName = m_fiEmptyFile.GetFilePath();
						m_fiEmptyFile.Close();
						pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_W3C); // check save plugin
						if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_W3C))
						{
							srtSaveError = srtSaveError + STR_W3M_W3C + "\n";
						}
						m_fiEmptyFile.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);

					}
					else
					{
						// update, get name, close, add to MPQ, and re-open W3C file
						SerializeW3C(TRUE);
						if ((CFile::hFileNull != m_fiW3C.m_hFile) && (m_fiW3C.GetLength() > 0))
						{
							EmbFileName = m_fiW3C.GetFilePath();
							m_fiW3C.Close();
							pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_W3C); // check save plugin
							if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_W3C))
							{
								srtSaveError = srtSaveError + STR_W3M_W3C + "\n";
							}
							m_fiW3C.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
						}
						else
						{
							EmbFileName = pTheApp->GetAppPath() + STR_W3Z_MAP_FILESDIR + STR_W3M_W3C;
							// check if we need a clean new file
							// Note: because of previous test, if the file exists, its size is <= 0
							if (CFile::hFileNull == m_fiW3C.m_hFile)
							{
								// creates a new file
								OpenTemporaryFile(m_fiW3C);
							}
							// opens original STR_W3Z_MAP_FILESDIR file
							fiTempFile.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
							if (CFile::hFileNull != fiTempFile)
							{
								int w = (int)fiTempFile.GetLength();
								pBuf = new char[w];
								if (pBuf)
								{
									w = fiTempFile.Read(pBuf, w);
									m_fiW3C.Write(pBuf, w);
									delete []pBuf;
								}
								fiTempFile.Close();
								EmbFileName = m_fiW3C.GetFilePath();
								m_fiW3C.Close();
								pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_W3C); // check save plugin
								if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_W3C))
								{
									srtSaveError = srtSaveError + STR_W3M_W3C + "\n";
								}
								m_fiW3C.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
							}
						}
					}

					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Saving regions...");
					// W3R
					//*****
					if ((TRUE == m_bEnableProtect)
						&& (W3Z_MAP_PROT_W3ZV1L3 == GetProtection()))
					{
						// map protected level 3 --> a file with the null size instead
						EmbFileName = m_fiEmptyFile.GetFilePath();
						m_fiEmptyFile.Close();
						pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_W3R); // check save plugin
						if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_W3R))
						{
							srtSaveError = srtSaveError + STR_W3M_W3R + "\n";
						}
						m_fiEmptyFile.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
					}
					else
					{
						// update, get name, close, add to MPQ, and re-open W3R file
						SerializeW3R(TRUE);
						if ((CFile::hFileNull != m_fiW3R.m_hFile) && (m_fiW3R.GetLength() > 0))
						{
							EmbFileName = m_fiW3R.GetFilePath();
							m_fiW3R.Close();
							pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_W3R); // check save plugin
							if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_W3R))
							{
								srtSaveError = srtSaveError + STR_W3M_W3R + "\n";
							}
							m_fiW3R.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
						}
						else
						{
							EmbFileName = pTheApp->GetAppPath() + STR_W3Z_MAP_FILESDIR + STR_W3M_W3R;
							// check if we need a clean new file
							// Note: because of previous test, if the file exists, its size is <= 0
							if (CFile::hFileNull == m_fiW3R.m_hFile)
							{
								// creates a new file
								OpenTemporaryFile(m_fiW3R);
							}
							// opens original STR_W3Z_MAP_FILESDIR file
							fiTempFile.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
							if (CFile::hFileNull != fiTempFile)
							{
								int w = (int)fiTempFile.GetLength();
								pBuf = new char[w];
								if (pBuf)
								{
									w = fiTempFile.Read(pBuf, w);
									m_fiW3R.Write(pBuf, w);
									delete []pBuf;
								}
								fiTempFile.Close();
								EmbFileName = m_fiW3R.GetFilePath();
								m_fiW3R.Close();
								pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_W3R); // check save plugin
								if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_W3R))
								{
									srtSaveError = srtSaveError + STR_W3M_W3R + "\n";
								}
								m_fiW3R.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
							}
						}
					}

					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Saving custom units...");
					// W3U
					//*****
					// update, get name, close, add to MPQ, and re-open W3U file
					SerializeW3U(TRUE);
					if ((CFile::hFileNull != m_fiW3U.m_hFile) && (m_fiW3U.GetLength() > 0))
					{
						EmbFileName = m_fiW3U.GetFilePath();
						m_fiW3U.Close();
						pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_W3U); // check save plugin
						if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_W3U))
						{
							srtSaveError = srtSaveError + STR_W3M_W3U + "\n";
						}
						m_fiW3U.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
					}
					else
					{// if no W3U found, no additionnal W3U is needed, this file is optionnal
					}

					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Saving sounds info...");
					// W3S
					//*****
					// update, get name, close, add to MPQ, and re-open W3S file
					SerializeW3S(TRUE);
					if ((CFile::hFileNull != m_fiW3S.m_hFile) && (m_fiW3S.GetLength() > 0))
					{
						EmbFileName = m_fiW3S.GetFilePath();
						m_fiW3S.Close();
						pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_W3S); // check save plugin
						if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_W3S))
						{
							srtSaveError = srtSaveError + STR_W3M_W3S + "\n";
						}
						m_fiW3S.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
					}
					else
					{
						EmbFileName = pTheApp->GetAppPath() + STR_W3Z_MAP_FILESDIR + STR_W3M_W3S;
						// check if we need a clean new file
						// Note: because of previous test, if the file exists, its size is <= 0
						if (CFile::hFileNull == m_fiW3S.m_hFile)
						{
							// creates a new file
							OpenTemporaryFile(m_fiW3S);
						}
						// opens original STR_W3Z_MAP_FILESDIR file
						fiTempFile.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
						if (CFile::hFileNull != fiTempFile)
						{
							int w = (int)fiTempFile.GetLength();
							pBuf = new char[w];
							if (pBuf)
							{
								w = fiTempFile.Read(pBuf, w);
								m_fiW3S.Write(pBuf, w);
								delete []pBuf;
							}
							fiTempFile.Close();
							EmbFileName = m_fiW3S.GetFilePath();
							m_fiW3S.Close();
							pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_W3S); // check save plugin
							if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_W3S))
							{
								srtSaveError = srtSaveError + STR_W3M_W3S + "\n";
							}
							m_fiW3S.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
						}
					}

					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Saving custom text triggers...");
					// WCT
					//*****
					if ((TRUE == m_bEnableProtect)
						&& (W3Z_MAP_PROT_W3ZV1L3 == GetProtection()))
					{
						// map protected level 3 --> a file with the null size instead
						EmbFileName = m_fiEmptyFile.GetFilePath();
						m_fiEmptyFile.Close();
						pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_WCT); // check save plugin
						if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_WCT))
						{
							srtSaveError = srtSaveError + STR_W3M_WCT + "\n";
						}
						m_fiEmptyFile.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
					}
					else
					{
						// update, get name, close, add to MPQ, and re-open WCT file
						SerializeWCT(TRUE);
						if ((CFile::hFileNull != m_fiWCT.m_hFile) && (m_fiWCT.GetLength() > 0))
						{
							EmbFileName = m_fiWCT.GetFilePath();
							m_fiWCT.Close();
							pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_WCT); // check save plugin
							if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_WCT))
							{
								srtSaveError = srtSaveError + STR_W3M_WCT + "\n";
							}
							m_fiWCT.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
						}
						else
						{
							EmbFileName = pTheApp->GetAppPath() + STR_W3Z_MAP_FILESDIR + STR_W3M_WCT;
							// check if we need a clean new file
							// Note: because of previous test, if the file exists, its size is <= 0
							if (CFile::hFileNull == m_fiWCT.m_hFile)
							{
								// creates a new file
								OpenTemporaryFile(m_fiWCT);
							}
							// opens original STR_W3Z_MAP_FILESDIR file
							fiTempFile.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
							if (CFile::hFileNull != fiTempFile)
							{
								int w = (int)fiTempFile.GetLength();
								pBuf = new char[w];
								if (pBuf)
								{
									w = fiTempFile.Read(pBuf, w);
									m_fiWCT.Write(pBuf, w);
									delete []pBuf;
								}
								fiTempFile.Close();
								EmbFileName = m_fiWCT.GetFilePath();
								m_fiWCT.Close();
								pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_WCT); // check save plugin
								if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_WCT))
								{
									srtSaveError = srtSaveError + STR_W3M_WCT + "\n";
								}
								m_fiWCT.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
							}
						}
					}

					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Saving triggers...");
					// WTG
					//*****
					// if the Unlucky-Weaaddar protection is not set put the file
					if ((TRUE == m_bEnableProtect)
						&& ((W3Z_MAP_PROT_UNLUCKY == GetProtection())
							|| (W3Z_MAP_PROT_W3ZV1L3 == GetProtection())))
					{
						//protection set, so the empty file is added instead
						EmbFileName = m_fiEmptyFile.GetFilePath();
						m_fiEmptyFile.Close();
						pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_WTG); // check save plugin
						if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_WTG))
						{
							srtSaveError = srtSaveError + STR_W3M_WTG + "\n";
						}
						m_fiEmptyFile.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
					}
					else
					{
						// update, get name, close, add to MPQ, and re-open WTG file
						SerializeWTG(TRUE);
						if ((CFile::hFileNull != m_fiWTG.m_hFile) && (m_fiWTG.GetLength() > 0))
						{
							EmbFileName = m_fiWTG.GetFilePath();
							m_fiWTG.Close();
							pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_WTG); // check save plugin
							if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_WTG))
							{
								srtSaveError = srtSaveError + STR_W3M_WTG + "\n";
							}
							m_fiWTG.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
						}
						else
						{
							EmbFileName = pTheApp->GetAppPath() + STR_W3Z_MAP_FILESDIR + STR_W3M_WTG;
							// check if we need a clean new file
							// Note: because of previous test, if the file exists, its size is <= 0
							if (CFile::hFileNull == m_fiWTG.m_hFile)
							{
								// creates a new file
								OpenTemporaryFile(m_fiWTG);
							}
							// opens original STR_W3Z_MAP_FILESDIR file
							fiTempFile.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
							if (CFile::hFileNull != fiTempFile)
							{
								int w = (int)fiTempFile.GetLength();
								pBuf = new char[w];
								if (pBuf)
								{
									w = fiTempFile.Read(pBuf, w);
									m_fiWTG.Write(pBuf, w);
									delete []pBuf;
								}
								fiTempFile.Close();
								EmbFileName = m_fiWTG.GetFilePath();
								m_fiWTG.Close();
								pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_WTG); // check save plugin
								if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_WTG))
								{
									srtSaveError = srtSaveError + STR_W3M_WTG + "\n";
								}
								m_fiWTG.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
							}
						}
					}

					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Saving map preview...");
					// PreviewTGA
					//************
					// update, get name, close, add to MPQ, and re-open PreviewTGA file
					SerializePreviewTGA(TRUE);
					if ((CFile::hFileNull != m_fiPreviewTGA.m_hFile) && (m_fiPreviewTGA.GetLength() > 0))
					{
						EmbFileName = m_fiPreviewTGA.GetFilePath();
						m_fiPreviewTGA.Close();
						pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_PREVIEWTGA); // check save plugin
						if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_PREVIEWTGA))
						{
							srtSaveError = srtSaveError + STR_W3M_PREVIEWTGA + "\n";
						}
						m_fiPreviewTGA.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
					}
					else
					{// if no preview file was found, add a new one
						EmbFileName = pTheApp->GetAppPath() + STR_W3Z_MAP_FILESDIR + STR_W3M_PREVIEWTGA;
						// check if we need a clean new file
						// Note: because of previous test, if the file exists, its size is <= 0
						if (CFile::hFileNull == m_fiPreviewTGA.m_hFile)
						{
							// creates a new file
							OpenTemporaryFile(m_fiPreviewTGA);
						}
						// opens original STR_W3Z_MAP_FILESDIR file
						fiTempFile.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
						if (CFile::hFileNull != fiTempFile)
						{
							int w = (int)fiTempFile.GetLength();
							pBuf = new char[w];
							if (pBuf)
							{
								w = fiTempFile.Read(pBuf, w);
								m_fiPreviewTGA.Write(pBuf, w);
								delete []pBuf;
							}
							fiTempFile.Close();
							EmbFileName = m_fiPreviewTGA.GetFilePath();
							m_fiPreviewTGA.Close();
							pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_PREVIEWTGA); // check save plugin
							if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_PREVIEWTGA))
							{
								srtSaveError = srtSaveError + STR_W3M_PREVIEWTGA + "\n";
							}
							m_fiPreviewTGA.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
						}
					}

					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Saving minimap image...");
					// MapBLP
					//********
					// update, get name, close, add to MPQ, and re-open MapBLP file
					SerializeMapBLP(TRUE);
					if ((CFile::hFileNull != m_fiMapBLP.m_hFile) && (m_fiMapBLP.GetLength() > 0))
					{
						EmbFileName = m_fiMapBLP.GetFilePath();
						m_fiMapBLP.Close();
						pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_MAPBLP); // check save plugin
						if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_MAPBLP))
						{
							srtSaveError = srtSaveError + STR_W3M_MAPBLP + "\n";
						}
						m_fiMapBLP.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
					}
					else
					{// if no map file was found, check for a new one
						EmbFileName = pTheApp->GetAppPath() + STR_W3Z_MAP_FILESDIR + STR_W3M_MAPBLP;
						pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_MAPBLP); // check save plugin
						if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_MAPBLP))
						{
							srtSaveError = srtSaveError + STR_W3M_MAPBLP + "\n";
						}
						EmbFileName = pTheApp->GetAppPath() + STR_W3Z_MAP_FILESDIR + STR_W3M_MAPBLP;
						// check if we need a clean new file
						// Note: because of previous test, if the file exists, its size is <= 0
						if (CFile::hFileNull == m_fiMapBLP.m_hFile)
						{
							// creates a new file
							OpenTemporaryFile(m_fiMapBLP);
						}
						// opens original STR_W3Z_MAP_FILESDIR file
						fiTempFile.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
						if (CFile::hFileNull != fiTempFile)
						{
							int w = (int)fiTempFile.GetLength();
							pBuf = new char[w];
							if (pBuf)
							{
								w = fiTempFile.Read(pBuf, w);
								m_fiMapBLP.Write(pBuf, w);
								delete []pBuf;
							}
							fiTempFile.Close();
							EmbFileName = m_fiMapBLP.GetFilePath();
							m_fiMapBLP.Close();
							pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_MAPBLP); // check save plugin
							if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_MAPBLP))
							{
								srtSaveError = srtSaveError + STR_W3M_MAPBLP + "\n";
							}
							m_fiMapBLP.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
						}
					}

					// MapB00
					//********
					// update, get name, close, add to MPQ, and re-open MapB00 file
					SerializeMapB00(TRUE);
					if ((CFile::hFileNull != m_fiMapB00.m_hFile) && (m_fiMapB00.GetLength() > 0))
					{
						EmbFileName = m_fiMapB00.GetFilePath();
						m_fiMapB00.Close();
						pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_MAPB00); // check save plugin
						if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_MAPB00))
						{
							srtSaveError = srtSaveError + STR_W3M_MAPB00 + "\n";
						}
						m_fiMapB00.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
					}
					else
					{
					}

					// MapTGA
					//********
					// update, get name, close, add to MPQ, and re-open MapTGA file
					SerializeMapTGA(TRUE);
					if ((CFile::hFileNull != m_fiMapTGA.m_hFile) && (m_fiMapTGA.GetLength() > 0))
					{
						EmbFileName = m_fiMapTGA.GetFilePath();
						m_fiMapTGA.Close();
						pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_MAPTGA); // check save plugin
						if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_MAPTGA))
						{
							srtSaveError = srtSaveError + STR_W3M_MAPTGA + "\n";
						}
						m_fiMapTGA.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
					}
					else if ((CFile::hFileNull == m_fiMapBLP.m_hFile) || (m_fiMapBLP.GetLength() <= 0))
					{// if no map file was found, check for a new one
						EmbFileName = pTheApp->GetAppPath() + STR_W3Z_MAP_FILESDIR + STR_W3M_MAPTGA;
						// check if we need a clean new file
						// Note: because of previous test, if the file exists, its size is <= 0
						if (CFile::hFileNull == m_fiMapTGA.m_hFile)
						{
							// creates a new file
							OpenTemporaryFile(m_fiMapTGA);
						}
						// opens original STR_W3Z_MAP_FILESDIR file
						fiTempFile.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
						if (CFile::hFileNull != fiTempFile)
						{
							int w = (int)fiTempFile.GetLength();
							pBuf = new char[w];
							if (pBuf)
							{
								w = fiTempFile.Read(pBuf, w);
								m_fiMapTGA.Write(pBuf, w);
								delete []pBuf;
							}
							fiTempFile.Close();
							EmbFileName = m_fiMapTGA.GetFilePath();
							m_fiMapTGA.Close();
							pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_MAPTGA); // check save plugin
							if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_MAPTGA))
							{
								srtSaveError = srtSaveError + STR_W3M_MAPTGA + "\n";
							}
							m_fiMapTGA.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
						}
					}

					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Saving path map...");
					// WPM
					//*****
					// update, get name, close, add to MPQ, and re-open WPM file
					// WPM must be load prior to PathTGA
					SerializeWPM(TRUE);
					if ((CFile::hFileNull != m_fiWPM.m_hFile) && (m_fiWPM.GetLength() > 0))
					{
						EmbFileName = m_fiWPM.GetFilePath();
						m_fiWPM.Close();
						pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_WPM); // check save plugin
						if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_WPM))
						{
							srtSaveError = srtSaveError + STR_W3M_WPM + "\n";
						}
						m_fiWPM.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
					}

					// PathTGA
					//*********
					// update, get name, close, add to MPQ, and re-open PathTGA file
					SerializePathTGA(TRUE);
					if ((CFile::hFileNull != m_fiPathTGA.m_hFile) && (m_fiPathTGA.GetLength() > 0)
						&& ((CFile::hFileNull != m_fiWPM.m_hFile) || (m_fiWPM.GetLength() <= 0)))
					{
						EmbFileName = m_fiPathTGA.GetFilePath();
						m_fiPathTGA.Close();
						pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_PATHTGA); // check save plugin
						if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_PATHTGA))
						{
							srtSaveError = srtSaveError + STR_W3M_PATHTGA + "\n";
						}
						m_fiPathTGA.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
					}

					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Saving units...");
					// UnitsDOO
					//**********
					// update, get name, close, add to MPQ, and re-open UnitsDOO file
					SerializeUnitsDOO(TRUE);
					if ((CFile::hFileNull != m_fiUnitsDOO.m_hFile) && (m_fiUnitsDOO.GetLength() > 0))
					{
						EmbFileName = m_fiUnitsDOO.GetFilePath();
						m_fiUnitsDOO.Close();
						pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_UNITSDOO); // check save plugin
						if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_UNITSDOO))
						{
							srtSaveError = srtSaveError + STR_W3M_UNITSDOO + "\n";
						}
						m_fiUnitsDOO.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
					}
					else
					{
						EmbFileName = pTheApp->GetAppPath() + STR_W3Z_MAP_FILESDIR + STR_W3M_UNITSDOO;
						// check if we need a clean new file
						// Note: because of previous test, if the file exists, its size is <= 0
						if (CFile::hFileNull == m_fiUnitsDOO.m_hFile)
						{
							// creates a new file
							OpenTemporaryFile(m_fiUnitsDOO);
						}
						// opens original STR_W3Z_MAP_FILESDIR file
						fiTempFile.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
						if (CFile::hFileNull != fiTempFile)
						{
							int w = (int)fiTempFile.GetLength();
							pBuf = new char[w];
							if (pBuf)
							{
								w = fiTempFile.Read(pBuf, w);
								m_fiUnitsDOO.Write(pBuf, w);
								delete []pBuf;
							}
							fiTempFile.Close();
							EmbFileName = m_fiUnitsDOO.GetFilePath();
							m_fiUnitsDOO.Close();
							pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_UNITSDOO); // check save plugin
							if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_UNITSDOO))
							{
								srtSaveError = srtSaveError + STR_W3M_UNITSDOO + "\n";
							}
							m_fiUnitsDOO.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
						}
					}

					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Saving imports...");
					// IMP
					//*****
					// update, get name, close, add to MPQ, and re-open IMP file
					SerializeIMP(TRUE);
					if ((CFile::hFileNull != m_fiIMP.m_hFile) && (m_fiIMP.GetLength() > 0))
					{
						EmbFileName = m_fiIMP.GetFilePath();
						m_fiIMP.Close();
						pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_IMP); // check save plugin
						if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_IMP))
						{
							srtSaveError = srtSaveError + STR_W3M_IMP + "\n";
						}
						m_fiIMP.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
					}
					else
					{
						EmbFileName = pTheApp->GetAppPath() + STR_W3Z_MAP_FILESDIR + STR_W3M_IMP;
						// check if we need a clean new file
						// Note: because of previous test, if the file exists, its size is <= 0
						if (CFile::hFileNull == m_fiIMP.m_hFile)
						{
							// creates a new file
							OpenTemporaryFile(m_fiIMP);
						}
						// opens original STR_W3Z_MAP_FILESDIR file
						fiTempFile.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
						if (CFile::hFileNull != fiTempFile)
						{
							int w = (int)fiTempFile.GetLength();
							pBuf = new char[w];
							if (pBuf)
							{
								w = fiTempFile.Read(pBuf, w);
								m_fiIMP.Write(pBuf, w);
								delete []pBuf;
							}
							fiTempFile.Close();
							EmbFileName = m_fiIMP.GetFilePath();
							m_fiIMP.Close();
							pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_IMP); // check save plugin
							if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_IMP))
							{
								srtSaveError = srtSaveError + STR_W3M_IMP + "\n";
							}
							m_fiIMP.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
						}
					}

					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Saving unprotect record...");
					// if protection set and unprotect record is needed,
					// packs the WTG, WCT, W3R and W3C files
					if ((TRUE == m_bEnableProtect)
						&& (W3Z_MAP_PROT_W3ZV1L3 == GetProtection())
						&& (TRUE == m_bUnpRec))
					{
						CByteArray baBuffer;
						int iSize;
						byte byRand;
						srand((unsigned)time(NULL));

						// write ID
						m_fiUNP.WriteStrID(W3Z_UNP_ID);
						//write version
						m_fiUNP.WriteInt(W3Z_UNP_VER);
						//write number of files stored
						iSize = 4; //4 files (WTG, WTC, W3R, W3C)
						m_fiUNP.WriteInt(iSize);

						// write the first file info block...
						// WTG
						// write filename
						m_fiUNP.WriteString(STR_W3M_WTG);
						// write file size + 1 byte for the xor pre-encryption					
						iSize = (int)m_fiWTG.GetLength();
						m_fiUNP.WriteInt(iSize+1);
						// sets buffer size
						baBuffer.SetSize(iSize+1);
						// fill buffer with the original file data
						m_fiWTG.Read(baBuffer.GetData(), iSize);
						//pre-encrypt the file
						byRand = (byte)(rand()%256);
						for (i = 0; i < iSize; i++)
						{
							baBuffer.SetAt(i, (byte)(baBuffer.GetAt(i)^byRand));
						}
						baBuffer.SetAt(iSize, byRand);
						CryptData(m_strAuthorPassword, baBuffer.GetData(), baBuffer.GetSize());
						// and write the buffer
						m_fiUNP.Write(baBuffer.GetData(), iSize+1);

						// WCT
						// write filename
						m_fiUNP.WriteString(STR_W3M_WCT);
						// write file size							
						iSize = (int)m_fiWCT.GetLength();
						m_fiUNP.WriteInt(iSize+1);
						// fill buffer with the original file data
						baBuffer.SetSize(iSize+1);
						m_fiWCT.Read(baBuffer.GetData(), iSize);
						//pre-encrypt the file
						byRand = (byte)(rand()%256);
						for (i = 0; i < iSize; i++)
						{
							baBuffer.SetAt(i, (byte)(baBuffer.GetAt(i)^byRand));
						}
						baBuffer.SetAt(iSize, byRand);
						CryptData(m_strAuthorPassword, baBuffer.GetData(), baBuffer.GetSize());
						// and write the buffer
						m_fiUNP.Write(baBuffer.GetData(), iSize+1);

						// W3R
						// write filename
						m_fiUNP.WriteString(STR_W3M_W3R);
						// write file size							
						iSize = (int)m_fiW3R.GetLength();
						m_fiUNP.WriteInt(iSize+1);
						// fill buffer with the original file data
						baBuffer.SetSize(iSize+1);
						m_fiW3R.Read(baBuffer.GetData(), iSize);
						//pre-encrypt the file
						byRand = (byte)(rand()%256);
						for (i = 0; i < iSize; i++)
						{
							baBuffer.SetAt(i, (byte)(baBuffer.GetAt(i)^byRand));
						}
						baBuffer.SetAt(iSize, byRand);
						CryptData(m_strAuthorPassword, baBuffer.GetData(), baBuffer.GetSize());
						// and write the buffer
						m_fiUNP.Write(baBuffer.GetData(), iSize+1);

						// W3C
						// write filename
						m_fiUNP.WriteString(STR_W3M_W3C);
						// write file size							
						iSize = (int)m_fiW3C.GetLength();
						m_fiUNP.WriteInt(iSize+1);
						// fill buffer with the original file data
						baBuffer.SetSize(iSize+1);
						m_fiW3C.Read(baBuffer.GetData(), iSize);
						//pre-encrypt the file
						byRand = (byte)(rand()%256);
						for (i = 0; i < iSize; i++)
						{
							baBuffer.SetAt(i, (byte)(baBuffer.GetAt(i)^byRand));
						}
						baBuffer.SetAt(iSize, byRand);
						CryptData(m_strAuthorPassword, baBuffer.GetData(), baBuffer.GetSize());
						// and write the buffer
						m_fiUNP.Write(baBuffer.GetData(), iSize+1);

						EmbFileName = m_fiUNP.GetFilePath();
						m_fiUNP.Close();
						str.Format("%u", GetAshedAuthorPassword());
						if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, str))
						{
							srtSaveError = srtSaveError + "Unprotect record\n";
						}
						m_fiUNP.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
					}


					wndProgress.OffsetPos(10);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Saving listfile...");
					// (listfile)
					//************
					// the (listfile) is auto-generated when a file is added to the MPQ
					// so we don't need to add it
					// but if protection level 2 or 3, we remove the "(listfile)"
					if ((TRUE == m_bEnableProtect)
						&& ((W3Z_MAP_PROT_W3ZV1L3 == GetProtection()) || (W3Z_MAP_PROT_W3ZV1L2 == GetProtection())))
					{
						// protection above level 1, remove the listfile
						if (!MpqDeleteFile(hMPQ, STR_W3M_LISTFILE))
						{
							//failed
						}
					}
					else
					{
						// listfile not deleted
						EmbFileName = m_fiListfile.GetFilePath();
						// updates listfile
						ReadEmbededFile(hMPQ, STR_W3M_LISTFILE, m_fiListfile);
						m_fiListfile.Close();
						pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, STR_W3M_LISTFILE); // check load plugin
						if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, STR_W3M_LISTFILE))
						{
							srtSaveError = srtSaveError + STR_W3M_LISTFILE + "\n";
						}
						m_fiListfile.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
					}

					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Saving imported files...");
					// Imported files
					//****************
					// Add imported files.
					if (("" != m_strOriginalMPQFilename) && ("" != m_strListfile)
						&& (SFileOpenArchive(m_strOriginalMPQFilename, 0, 0, &hMPQOriginal))) // opens the original MPQ
					{// if there is an original W3M file with a non empty (listfile)
						if (TRUE == OpenTemporaryFile(fiImportedFile))// open a temporary file
						{
							// counts the fields
							i = 0;
							str = m_strListfile.Tokenize("\n", i);
							while (i < m_strListfile.GetLength() -1)
							{
								try
								{
									// remove the spaces and CR/LF
									str.TrimLeft();
									str.TrimRight();
									// check if the file is not in the processed files list
									if (("" != str) && (TRUE == IsImportedFile(str)))
									{// string not null and file is an imported file
										// read the original file
										if (TRUE == ReadEmbededFile(hMPQOriginal, str, fiImportedFile))
										{
											EmbFileName = fiImportedFile.GetFilePath();
											fiImportedFile.Close(); // temporary close the file
											pTheApp->m_AppSettings.CheckSavePlugins(EmbFileName, str); // check save plugin
											if (FALSE == WriteEmbededFile(hMPQ, EmbFileName, str))
											{
												srtSaveError = srtSaveError + str + "\n";
											}
											// re-open the file
											fiImportedFile.Open(EmbFileName, CFile::modeReadWrite | CFile::shareDenyNone);
										}
									}
								}
								catch (CFileException *e)
								{
									delete e;
								}
								// gets next field
								str = m_strListfile.Tokenize("\n", i);
							}
							// close and remove the temporary file
							CloseAndDeleteFile(fiImportedFile);
						}
						// closes original MPQ
						SFileCloseArchive(hMPQOriginal);
						// updates the original filename
						m_strOriginalMPQFilename = sFileName;
					}

					wndProgress.OffsetPos(10);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Generating W3M header...");

					MpqCloseUpdatedArchive(hMPQ, NULL); // SFMPQAPI releases temporary MPQ
					//re-open the temporary MPQ file and save it into the archive
					newW3M.Open(W3Mfilename, CFile::modeRead | CFile::shareDenyNone);
					// writes the W3M header
					pBuf = new char[512];
					if (pBuf)
					{
						memset(pBuf,'\0',512);
						strcpy(pBuf, STR_W3M_HEADER);
						// map name trigger string
						CString str;
						str = m_War3Map.mapTrigStringsTable.GetTrigStringValue(m_War3Map.mapW3Info.mapNameTID);
						if (str != "")
						{	
							if (str.GetLength() >= 480)
							{// truncate the name if too long
								str = str.Left(480);
							}
							if (str.GetLength() > 0)
							{
								strcpy(pBuf+8, str.GetBuffer(str.GetLength()));
							}
							str.ReleaseBuffer();
						}
						pBuf[9 + str.GetLength()] = (char) m_War3Map.mapW3Info.mapFlags;
						pBuf[13 + str.GetLength()] = (char) m_War3Map.mapW3Info.mapNbMaxPlayers;
						ar.Write(pBuf, 512);
						delete []pBuf;
					}
					int w = (int)newW3M.GetLength();
					pBuf = new char[w];
					if (pBuf)
					{
						w = newW3M.Read( pBuf, w);
						ar.Write(pBuf, w);
						delete []pBuf;
					}
					// remove the temporary MPQ file
					CloseAndDeleteFile(newW3M);
					pTheApp->m_AppSettings.CheckSavePlugins(m_strOriginalMPQFilename, ar.GetFile()->GetFileName()); // check save plugin
				}//if (pTheApp->IsMPQInit())
				else
				{// error saving
					AfxMessageBox(STR_ERROR_SAVING00, MB_OK | MB_APPLMODAL | MB_ICONSTOP);
				}
				wndProgress.SetText("Done!");
				wndProgress.SetPos(100);
				wndProgress.PeekAndPump();
				if ("" != srtSaveError)
				{
					AfxMessageBox(STR_ERROR_SAVING05 + srtSaveError, MB_OK | MB_APPLMODAL | MB_ICONSTOP);
				}
			}
			else if (TRUE == ar.IsStoring())
			{ // map protected: no save
				AfxMessageBox(STR_ERROR_SAVING01, MB_OK | MB_APPLMODAL | MB_ICONSTOP);
				m_bSaved = FALSE;
			}
			else if (FALSE == ar.IsStoring())
			{// map loading
				CProgressWnd wndProgress(NULL, "Loading map, please wait...", TRUE);
				wndProgress.GoModal(); // Call this if you want a modal window
				wndProgress.SetRange(0, 100);
				wndProgress.SetPos(0);
				wndProgress.SetText("Reading W3M...");

				pTheApp->m_AppSettings.CheckLoadPlugins(sFileName, ar.GetFile()->GetFileName()); // check save plugin
				if ((TRUE ==pTheApp->IsMPQInit()) && SFileOpenArchive(sFileName, 0, 0, &hMPQ))
				{// initialisation ok
					// save the original MPQ filename
					m_strOriginalMPQFilename = sFileName;

					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Loading trigger strings...");
					// WTS
					//*****
					if (ReadEmbededFile(hMPQ, STR_W3M_WTS, m_fiWTS))
					{
						pTheApp->m_AppSettings.CheckLoadPlugins(m_fiWTS.GetFilePath(), STR_W3M_WTS); // check load plugin
						SerializeWTS(FALSE); // load the WTS
					}
					else
					{
						//Failed
					}

					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Loading ground...");
					// W3E
					//*****
					if (ReadEmbededFile(hMPQ, STR_W3M_W3E, m_fiW3E))
					{
						pTheApp->m_AppSettings.CheckLoadPlugins(m_fiW3E.GetFilePath(), STR_W3M_W3E); // check load plugin
						SerializeW3E(FALSE); // load the W3E
					}
					else
					{
						//Failed
					}

					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Loading map info...");
					// W3I
					//*****
					if (ReadEmbededFile(hMPQ, STR_W3M_W3I, m_fiW3I))
					{
						pTheApp->m_AppSettings.CheckLoadPlugins(m_fiW3I.GetFilePath(), STR_W3M_W3I); // check load plugin
						SerializeW3I(FALSE); // load the W3I
					}
					else
					{
						//Failed
					}


					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Loading doodads...");
					// DOO
					//*****
					if (ReadEmbededFile(hMPQ, STR_W3M_DOO, m_fiDOO))
					{
						pTheApp->m_AppSettings.CheckLoadPlugins(m_fiDOO.GetFilePath(), STR_W3M_DOO); // check load plugin
						SerializeDOO(FALSE); // load the DOO
					}
					else
					{
						//Failed
					}

					wndProgress.OffsetPos(10);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Loading JASS Script...");
					// JASS
					//******
					if (ReadEmbededFile(hMPQ, STR_W3M_JASS, m_fiJASS))
					{
						pTheApp->m_AppSettings.CheckLoadPlugins(m_fiJASS.GetFilePath(), STR_W3M_JASS); // check load plugin
						SerializeJASS(FALSE); // load the JASS
					}
					else
					{ //Failed, tries to see if it was "hiden"
						CString strJassPath;
						strJassPath = STR_W3M_SCRIPTS;
						strJassPath += STR_W3M_JASS;
						if (ReadEmbededFile(hMPQ, strJassPath, m_fiJASS))
						{
							pTheApp->m_AppSettings.CheckLoadPlugins(m_fiJASS.GetFilePath(), strJassPath); // check load plugin
							SerializeJASS(FALSE); // load the JASS
						}
					}

					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Loading minimap info...");
					// MMP
					//*****
					if (ReadEmbededFile(hMPQ, STR_W3M_MMP, m_fiMMP))
					{
						pTheApp->m_AppSettings.CheckLoadPlugins(m_fiMMP.GetFilePath(), STR_W3M_MMP); // check load plugin
						SerializeMMP(FALSE); // load the MMP
					}
					else
					{
						//Failed
					}

					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Loading shadows...");
					// SHD
					//*****
					if (ReadEmbededFile(hMPQ, STR_W3M_SHD, m_fiSHD))
					{
						pTheApp->m_AppSettings.CheckLoadPlugins(m_fiSHD.GetFilePath(), STR_W3M_SHD); // check load plugin
						SerializeSHD(FALSE); // load the SHD
					}
					else
					{
						//Failed
					}


					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Loading cameras...");
					// W3C
					//*****
					if (ReadEmbededFile(hMPQ, STR_W3M_W3C, m_fiW3C))
					{
						pTheApp->m_AppSettings.CheckLoadPlugins(m_fiW3C.GetFilePath(), STR_W3M_W3C); // check load plugin
						SerializeW3C(FALSE); // load the W3C
					}
					else
					{
						//Failed: the W3C was removed
						if (FALSE == IsProtected())
						{// if no protection were set --> W3Z V1.0 Level 3
							SetProtected();
							SetProtection(W3Z_MAP_PROT_W3ZV1L3);
						}
					}

					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Loading regions...");
					// W3R
					//*****
					if (ReadEmbededFile(hMPQ, STR_W3M_W3R, m_fiW3R))
					{
						pTheApp->m_AppSettings.CheckLoadPlugins(m_fiW3R.GetFilePath(), STR_W3M_W3R); // check load plugin
						SerializeW3R(FALSE); // load the W3R
					}
					else
					{
						//Failed: the W3R was removed
						if (FALSE == IsProtected())
						{// if no protection were set --> W3Z V1.0 Level 3
							SetProtected();
							SetProtection(W3Z_MAP_PROT_W3ZV1L3);
						}
					}

					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Loading custom unit...");
					// W3U
					//*****
					if (ReadEmbededFile(hMPQ, STR_W3M_W3U, m_fiW3U))
					{
						pTheApp->m_AppSettings.CheckLoadPlugins(m_fiW3U.GetFilePath(), STR_W3M_W3U); // check load plugin
						SerializeW3U(FALSE); // load the W3U
					}
					else
					{
						//Failed
					}

					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Loading sounds info...");
					// W3S
					//*****
					if (ReadEmbededFile(hMPQ, STR_W3M_W3S, m_fiW3S))
					{
						pTheApp->m_AppSettings.CheckLoadPlugins(m_fiW3S.GetFilePath(), STR_W3M_W3S); // check load plugin
						SerializeW3S(FALSE); // load the W3S
					}
					else
					{
						//Failed
					}

					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Loading custom text triggers...");
					// WCT
					//*****
					if (ReadEmbededFile(hMPQ, STR_W3M_WCT, m_fiWCT))
					{
						if (0 == m_fiWCT.GetLength())
						{
							//Failed: the WCT was cleared
							if (FALSE == IsProtected())
							{// if no protection were set --> W3Z V1.0 Level 3
								SetProtected();
								SetProtection(W3Z_MAP_PROT_W3ZV1L3);
							}
						}
						else
						{
							pTheApp->m_AppSettings.CheckLoadPlugins(m_fiWCT.GetFilePath(), STR_W3M_WCT); // check load plugin
							SerializeWCT(FALSE); // load the WCT
						}
					}
					else
					{
						//Failed: the WCT was removed
						if (FALSE == IsProtected())
						{// if no protection were set --> W3Z V1.0 Level 3
							SetProtected();
							SetProtection(W3Z_MAP_PROT_W3ZV1L3);
						}
					}

					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Loading triggers...");
					// WTG
					//*****
					if (ReadEmbededFile(hMPQ, STR_W3M_WTG, m_fiWTG))
					{
						if (0 == m_fiWTG.GetLength())
						{
							//Failed: the WTG was cleared
							if (FALSE == IsProtected())
							{// if no protection were set --> Unlucky_Footman/Weaaddar protection
								SetProtected();
								SetProtection(W3Z_MAP_PROT_UNLUCKY);
							}
						}
						else
						{
							pTheApp->m_AppSettings.CheckLoadPlugins(m_fiWTG.GetFilePath(), STR_W3M_WTG); // check load plugin
							SerializeWTG(FALSE); // load the WTG
						}
					}
					else
					{
						//Failed: the WTG was removed
						if (FALSE == IsProtected())
						{// if no protection were set --> Unlucky_Footman/Weaaddar protection
							SetProtected();
							SetProtection(W3Z_MAP_PROT_UNLUCKY);
						}
					}


					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Loading map preview...");
					// PreviewTGA
					//************
					if (ReadEmbededFile(hMPQ, STR_W3M_PREVIEWTGA, m_fiPreviewTGA))
					{
						pTheApp->m_AppSettings.CheckLoadPlugins(m_fiPreviewTGA.GetFilePath(), STR_W3M_PREVIEWTGA); // check load plugin
						SerializePreviewTGA(FALSE); // load the PreviewTGA
					}
					else
					{
						//Failed
					}

					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Loading minimap picture...");
					// MapBLP
					//********
					if (ReadEmbededFile(hMPQ, STR_W3M_MAPBLP, m_fiMapBLP))
					{
						pTheApp->m_AppSettings.CheckLoadPlugins(m_fiMapBLP.GetFilePath(), STR_W3M_MAPBLP); // check load plugin
						SerializeMapBLP(FALSE); // load the MapBLP
					}
					else
					{
						//Failed
					}

					// MapB00
					//********
					if (ReadEmbededFile(hMPQ, STR_W3M_MAPB00, m_fiMapB00))
					{
						pTheApp->m_AppSettings.CheckLoadPlugins(m_fiMapB00.GetFilePath(), STR_W3M_MAPB00); // check load plugin
						SerializeMapB00(FALSE); // load the MapTGA
					}
					else
					{
						//Failed
					}

					// MapTGA
					//********
					if (ReadEmbededFile(hMPQ, STR_W3M_MAPTGA, m_fiMapTGA))
					{
						pTheApp->m_AppSettings.CheckLoadPlugins(m_fiMapTGA.GetFilePath(), STR_W3M_MAPTGA); // check load plugin
						SerializeMapTGA(FALSE); // load the MapTGA
					}
					else
					{
						//Failed
					}

					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Loading path map...");
					// WPM
					//*****
					if (ReadEmbededFile(hMPQ, STR_W3M_WPM, m_fiWPM))
					{
						pTheApp->m_AppSettings.CheckLoadPlugins(m_fiWPM.GetFilePath(), STR_W3M_WPM); // check load plugin
						SerializeWPM(FALSE); // load the Path Map
					}
					else
					{
						//Failed
					}

					// PathTGA
					//*********
					if (ReadEmbededFile(hMPQ, STR_W3M_PATHTGA, m_fiPathTGA))
					{
						pTheApp->m_AppSettings.CheckLoadPlugins(m_fiPathTGA.GetFilePath(), STR_W3M_PATHTGA); // check load plugin
						SerializePathTGA(FALSE); // load the PathTGA
					}
					else
					{
						//Failed
					}

					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Loading units...");
					// UnitsDOO
					//**********
					if (ReadEmbededFile(hMPQ, STR_W3M_UNITSDOO, m_fiUnitsDOO))
					{
						pTheApp->m_AppSettings.CheckLoadPlugins(m_fiUnitsDOO.GetFilePath(), STR_W3M_UNITSDOO); // check load plugin
						SerializeUnitsDOO(FALSE); // load the UnitsDOO
					}
					else
					{
						//Failed
					}

					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Loading imports...");
					// IMP
					//*****
					if (ReadEmbededFile(hMPQ, STR_W3M_IMP, m_fiIMP))
					{
						pTheApp->m_AppSettings.CheckLoadPlugins(m_fiIMP.GetFilePath(), STR_W3M_IMP); // check load plugin
						SerializeIMP(FALSE); // load the IMP
					}
					else
					{
						//Failed
					}

					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Loading unprotection records...");
					// UNP
					//*****
					str.Format("%u", m_tAuthorHashCountList.GetBestCountHash());
					if (ReadEmbededFile(hMPQ, str, m_fiUNP))
					{
					}
					else
					{
						//Failed
					}


					wndProgress.OffsetPos(3);
					wndProgress.PeekAndPump();
					wndProgress.SetText("Loading listfile...");
					// (listfile)
					//************
					if (ReadEmbededFile(hMPQ, STR_W3M_LISTFILE, m_fiListfile))
					{
						pTheApp->m_AppSettings.CheckLoadPlugins(m_fiListfile.GetFilePath(), STR_W3M_LISTFILE); // check load plugin
						SerializeListfile(FALSE); // load the (listfile)
					}
					else
					{
						//Failed
					}


					// closes MPQ
					SFileCloseArchive(hMPQ);

					// check if the path map needs to be inited
					if (!(FL_W3M_WPM & m_iFilesInitStatus))
					{
						wndProgress.OffsetPos(1);
						wndProgress.PeekAndPump();
						wndProgress.SetText("Convert Path.TGA to WPM format...");
						if (FL_W3M_PATHTGA & m_iFilesInitStatus)
						{// use PathTGA if it has been inited
							m_War3Map.PathTGA2WPM();
						}
						else
						{// generate a new pathing data otherwise
							m_War3Map.ResetPathing();
						}
					}
					wndProgress.SetText("Done!");
					wndProgress.SetPos(100);
					wndProgress.PeekAndPump();


					// if the map is protected with W3Z protection, ask a password (if above level 1)
					iTries = 0;
					if ((TRUE == IsProtected())
						&& (GetProtection() == W3Z_MAP_PROT_NOTPROTECTED))
					{
						SetProtection(W3Z_MAP_PROT_UNKNOWN);
					}

					if ((TRUE == IsProtected())
						&& ((GetProtection() == W3Z_MAP_PROT_W3ZV1L2)
							|| (GetProtection() == W3Z_MAP_PROT_W3ZV1L3))
						&& (m_tAuthorHashCountList.m_pHead == NULL)
						&& (m_tPublicHashCountList.m_pHead == NULL))
					{ // no password found
						SetProtection(W3Z_MAP_PROT_UNKNOWN);
					}

					if ((TRUE == IsProtected())
						&& ((GetProtection() == W3Z_MAP_PROT_W3ZV1L2)
							|| (GetProtection() == W3Z_MAP_PROT_W3ZV1L3)))
					{// the document exists and the map is protected
						bDone = FALSE;
						while(FALSE == bDone) // asks the password
						{
							CPasswordDialog dlgPassDialog;
							if (IDCANCEL == dlgPassDialog.DoModal())
							{
								W3Z_WARNING(STR_WARNING_MAPLOAD00);
								bDone = TRUE;
							}
							else
							{
								// try to register the author password
								if (TRUE == RegisterAuthorPassword(dlgPassDialog.m_sPassword))
								{// the author password has been verified
									bDone = TRUE;
								}//if (TRUE == RegisterAuthorPassword(dlgPassDialog.m_sPassword))
								else if (TRUE == RegisterPublicPassword(dlgPassDialog.m_sPassword))
								// try to register the public password
								{// the public password has been verified
									W3Z_WARNING(STR_WARNING_MAPLOAD01);
									bDone = TRUE;
								}//else if (TRUE == RegisterPublicPassword(dlgPassDialog.m_sPassword))
								else
								{// none of the password matched
									iTries++;
									if (iTries >= W3Z_MAX_TRIES)
									{//too many tries
										W3Z_WARNING(STR_WARNING_MAPLOAD00);
										bDone = TRUE;
									}
									else
									{
										bDone = FALSE;
									}
								}
							}
						}// while(FALSE == bDone) // asks the password
					}// if (TRUE == IsProtected())
				} // if ((TRUE ==pTheApp->IsMPQInit()) && SFileOpenArchive(sFileName, 0, 0, &hMPQ))
			}//else if (FALSE == ar.IsStoring())
		}//else /*if (!str.CompareNoCase(STR_W3M_EXTENSION))*/
	}//if (NULL != pFile)
}

/////////////////////////////////////////////////////////////////////////////
// CW3ZMapEditDoc diagnostics

#ifdef _DEBUG
void CW3ZMapEditDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CW3ZMapEditDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CW3ZMapEditDoc commands

/*
SerializeW3E
************


In:
 BOOL IsStoring: TRUE if the file has to be written, FALSE if the W3E structure has to
  be initialised.

Out:
 nothing

Return:
 nothing

Notes:
 Protection notes:
  The W3E protection is made in 2 parts:
  1) The first consists of the original author name being stored in the cliffsets list.
	After the last map cliffset, a protection identifier cliffset is added. That
	cliffset starts with a '\0' so if the cliffset list has to be displayed by a program
	the protection part would be hidden. The 3 following chars of that cliffset identify
	the protection type. Then several cliffset rooms are used to store crypted author
	name. The author name is first crypted using the standard cryption function and then
	a different value is added to each char of a group of 4 chars (which corresponds
	to a cliffset). The purpose of these additions is to make sure we don't	have two
	cliffsets with the same name which could make the duplicates removed when saving
	in the WE and it adds also an additional encryption in case someone would like
	to try to decrypt the author name using the decrypt function. Each value added is
	stored in the cliffset following the crypted name. As a cliffset can contain at most
	4 values, the author name can't be more than 4*4=16 chars.
  2) The second part uses the tilepoints data to hide protection data.
    There are 4 area used to hide data:
	2.1) the top and the right tilepoints are not shown in the WE so I used the two 
	  boundaries flags as a tristate value (so a boundary flag is always set):

	  Normal bound:	WE bound:	Value:
	  0				1			0
	  1				0			1
	  1				1			2

	  These flags are used to store in a different order for the top and the right the
	  same protection structure (structure 2 which contains a header, the 2 passwords
	  hashes and a checksum).
	  As the tilepoints are not displayed I also used the details as a byte to store
	  again a protection structure (structure 1 which is the same as 2 with the author
	  name).
	2.2) the left and bottom tilepoints are usually used as boundaries for the map so, if
	  at least one of their boundary flags is set, I change them to save extra data the
	  same way as described before in 2.1). In this case, detail can't be changed.
	2.3) the tilepoints with their water flag NOT set: I used the low byte of the
	  water height to store the protection structure. Each byte of the protection
	  strucutre is divided in two half-bytes. Each half-byte is stored in the free water
	  byte and followed by its complement in the next water byte available. That way,
	  even if the water flag is set later, the height difference is hard to see and W3Z
	  can detects eroneous data and eventually try to fix it.
	2.4) the ground height lower 2bit can be used to store data as the heigh modification
	  is really hard to see.

*/
void CW3ZMapEditDoc::SerializeW3E(BOOL IsStoring)
{
	int i, j, height, width;
	int nbTilePoint; // number of tilepoints to save
	CWar3StrID strID; // various uses
	CTilePoint tilepoint; // tilepoint use to store or load tilepoints
	byte bBuf[7]; // size of a tileset storage data
	byte *pBuf; // pointer used for the unsupported buffer
	float fBuf[2]; // buffer to read the map center coordinates
	char *pcGrndTiles; // char pointer for ground tilesets
	char *pcClfTiles; // char pointer for cliff tilesets
	BOOL bCliffProtected = FALSE; // used to detect protected cliffs
	AUTHORNAME szAuthorName; // used to extract stored author name
	CString strAuthorName; // used to get and crypt or decrypt the author name
	unsigned char caAdd[4]; // used to add another simple encryption to the author name in the cliffsets
	int iCheckSum; // used to check the checksum when loading
	byte* pbyProtectData01; //pointer used to store the protection data
	byte* pbyProtectData02; //pointer used to store the protection data

	// protected data index for the data stored in the water height (tiles without water flag set)
	int iWater; // protect data index for "water storage"
	W3ZPROTECTDATA01 tProtectWater; // structure used to read protect data from "water storage"
	byte* pbyReadDataWater = (byte*)(&tProtectWater);
	BOOL bWaterProtectDataLoaded; // set to TRUE when a valid data block has been loaded

	// protected data index for the data stored in the gournd height
	int iGround; // protect data index for "ground storage"
	W3ZPROTECTDATA01 tProtectGround; // structure used to read protect data from "ground storage"
	byte* pbyReadDataGround = (byte*)(&tProtectGround);
	BOOL bGroundProtectDataLoaded; // set to TRUE when a valid data block has been loaded

	// each block of variable is used to keep track of protection data in a specific part of the W3E
	// right boundaries...
	int iDetails1 = 0; // protect data index for "details storage"
	int iCount1 = 0; // tilepoint storage cycle for the "boundaries storage"
	unsigned int iVal1; // the current int to store
	unsigned int iStoreVal1; // used to know the tri-state value on each cycle
	unsigned int iFact1; // used to retrieve the value
	W3ZPROTECTDATA02 tProtect1; // structure used to read protect data from boundaries
	byte* pbyReadData1 = (byte*)(&tProtect1);
	W3ZPROTECTDATA01 tProtectDetails1; // structure used to read protect data from details
	byte* pbyReadDataDetails1 = (byte*)(&tProtectDetails1);
	BOOL bBounds1ProtectDataLoaded; // set to TRUE when a valid data block has been loaded
	BOOL bDetails1ProtectDataLoaded; // set to TRUE when a valid data block has been loaded
	// top boundaries
	int iDetails2 = 0;
	int iCount2 = 0;
	unsigned int iVal2;
	unsigned int iStoreVal2;
	unsigned int iFact2;
	W3ZPROTECTDATA02 tProtect2;
	byte* pbyReadData2 = (byte*)(&tProtect2);
	W3ZPROTECTDATA01 tProtectDetails2; // structure used to read protect data from details
	byte* pbyReadDataDetails2 = (byte*)(&tProtectDetails2);
	BOOL bBounds2ProtectDataLoaded; // set to TRUE when a valid data block has been loaded
	BOOL bDetails2ProtectDataLoaded; // set to TRUE when a valid data block has been loaded
	// left tilepoints
	int iCount3 = 0;
	unsigned int iVal3;
	unsigned int iStoreVal3;
	unsigned int iFact3;
	W3ZPROTECTDATA02 tProtect3;
	byte* pbyReadData3 = (byte*)(&tProtect3);
	BOOL bBounds3ProtectDataLoaded; // set to TRUE when a valid data block has been loaded
	// bottom tilepoints
	int iCount4 = 0;
	unsigned int iVal4;
	unsigned int iStoreVal4;
	unsigned int iFact4;
	W3ZPROTECTDATA02 tProtect4;
	byte* pbyReadData4 = (byte*)(&tProtect4);
	BOOL bBounds4ProtectDataLoaded; // set to TRUE when a valid data block has been loaded

	if (TRUE == IsStoring)
	{//save
		if (TRUE == IsReadOnly())
		{// can't be saved because is read-only
			//+FIXME: set the flags that say the file has not been saved
			m_fiW3E.SeekToBegin();
		}//if (TRUE == IsReadOnly())
		else
		{//not protected
			m_fiW3E.SetLength(0); // empty file
			m_fiW3E.SeekToBegin( ); // go to begining

			m_fiW3E.WriteStrID(STR_W3E_HEADER); // write file ID

			m_fiW3E.WriteInt(W3E_VERSION_DEFAULT); // write version

			m_fiW3E.WriteChar(m_War3Map.mapMainTileset); // write map type

			m_fiW3E.WriteInt(m_War3Map.mapCustomTilesets); // write the "Use Custom Tilesets" flag

			// stores ground tilesets
			j = m_War3Map.mapGroundTiles.GetLength();
			j -= j %4; // make sure we got a correct length
			i = j/4;
			m_fiW3E.WriteInt(i); // write number of ground tilesets
			pcGrndTiles = m_War3Map.mapGroundTiles.GetBufferSetLength(j);
			m_fiW3E.Write(pcGrndTiles, j); //write ground tilesets
			pcGrndTiles = NULL;
			m_War3Map.mapGroundTiles.ReleaseBuffer();

			// gets author name
			strAuthorName = GetAuthorName();
			// set author name length and crypt it 
			CryptString(strAuthorName, W3Z_MAX_NAME_LENGTH);

			// store cliff tilesets
			// check if map has to be protected
			if ((TRUE == m_bEnableProtect)
				&& ((W3Z_MAP_PROT_W3ZV1L1 == GetProtection())
					||(W3Z_MAP_PROT_W3ZV1L2 == GetProtection())
					||(W3Z_MAP_PROT_W3ZV1L3 == GetProtection())))
			{// to be protected				
				// adds the special cliffsets
				j = m_War3Map.mapCliffTiles.GetLength();
				j -= j%4; // make sure we got a correct length multiple of 4
				i = (j + 8 + W3Z_MAX_NAME_LENGTH) / 4; // 8 = 4 for W3Z_W3E_PROTILE01 and 4 for the add buffer
				m_fiW3E.WriteInt(i); // write number of cliff tilesets
				pcClfTiles = m_War3Map.mapCliffTiles.GetBufferSetLength(j);
				m_fiW3E.Write(pcClfTiles, j); //write cliff tilesets
				pcClfTiles = NULL;
				m_War3Map.mapCliffTiles.ReleaseBuffer();
				//write protection indicator cliff tileset
				m_fiW3E.Write(W3Z_W3E_PROT_TILE01, 4);
				srand ((unsigned)time(NULL));
				//add another encryption
				pcClfTiles = strAuthorName.GetBuffer(W3Z_MAX_NAME_LENGTH);
				for (j=0; j < 4; j++)
				{
					caAdd[j] = (unsigned char)(rand() % 256);
					pcClfTiles[j*4] = (char)(pcClfTiles[j*4] + caAdd[j]);
					pcClfTiles[j*4+1] = (char)(pcClfTiles[j*4+1] - caAdd[j]);
					pcClfTiles[j*4+2] = (char)(pcClfTiles[j*4+2] + caAdd[j]);
					pcClfTiles[j*4+3] = (char)(pcClfTiles[j*4+3] - caAdd[j]);
				}
				m_fiW3E.Write(pcClfTiles, W3Z_MAX_NAME_LENGTH);
				strAuthorName.ReleaseBuffer();
				//write add buffer
				m_fiW3E.Write(caAdd, 4);
				//+FIXME: each group of 4 chars in the cliffset list should be different: use caAdd to make sure
			}
			else
			{// map not to protect
				j = m_War3Map.mapCliffTiles.GetLength();
				j -= j%4;
				i = j/4;
				m_fiW3E.WriteInt(i); // write number of cliff tilesets
				pcClfTiles = m_War3Map.mapCliffTiles.GetBufferSetLength(j);
				m_fiW3E.Write(pcClfTiles, j); //write cliff tilesets
				pcClfTiles = NULL;
				m_War3Map.mapCliffTiles.ReleaseBuffer();
			} //if ((W3Z_MAP_PROT_W3ZV1L1 == GetProtection())


			width = m_War3Map.GetMapWidth() + 1;
			m_fiW3E.WriteInt(width); // write map width

			height = m_War3Map.GetMapHeight() + 1;
			m_fiW3E.WriteInt(height); // write map height

			// write center coordinates
			m_fiW3E.WriteFloat(m_War3Map.GetMapCenterX());
			m_fiW3E.WriteFloat(m_War3Map.GetMapCenterY());

			//write tilepoints data
			nbTilePoint = width * height;
			// if the protection is enabled,
			// and the W3E file was not protected or the author is registered,
			// and a W3Z protection is set,
			// set a new watermark,
			// otherwise leave the watermark as is
			if ((TRUE == m_bEnableProtect)
				&& ((0 == (m_iFilesProtectStatus & FL_W3M_W3E)) || (TRUE == IsAuthorRegistered()))
				&& ((W3Z_MAP_PROT_W3ZV1L1 == GetProtection())
					 ||(W3Z_MAP_PROT_W3ZV1L2 == GetProtection())
					 ||(W3Z_MAP_PROT_W3ZV1L3 == GetProtection())))
			{// protected by W3Z v1.0
				// initialises protect data structures
				InitProtectDataStructs();
				//sets the protect data structure 01 pointer
				pbyProtectData01 = (byte *)(&m_tW3ZProtectData01);
				pbyProtectData02 = (byte *)(&m_tW3ZProtectData02);

				iCount1 = iCount2 = iCount3 = iCount4 = 0; // initialises the 4 edges counters
				iDetails1 = 0; // initialises bounds counters
				iDetails2 = 8; // initialises bounds counters
				iWater = 0; // initialises water counter
				iGround = 0; // initialises ground counter
				
				//gets author name
				strAuthorName = GetAuthorName();
				//set author name length and crypt it 
				CryptString(strAuthorName, W3Z_MAX_NAME_LENGTH);
				
				for (i = 0; i < nbTilePoint;i++)
				{
					tilepoint = m_War3Map.GetTilePoint(i);
					bBuf[0] = (byte)(tilepoint.height & 0x00FF);
					bBuf[1] = (byte)((tilepoint.height & 0xFF00) / 0x0100);
					bBuf[2]	= (byte)(tilepoint.waterlevel & 0x00FF);
					bBuf[3] = (byte)(((tilepoint.waterlevel & 0xFF00) | (tilepoint.flags & TILE_MSK_FLAGS_EXT)) / 0x0100);
					bBuf[4] = (byte)((tilepoint.flags & 0x00F0) | (tilepoint.groundtype & 0x000F));
					bBuf[5] = (byte)((tilepoint.details & 0x00FF));
					bBuf[6] = (byte)(((tilepoint.clifftype & 0x000F) * 0x0010) | (tilepoint.layer & 0x000F));
					if ((width-1) == (i%width))
					{// right: boundaries -> author password + public password + author name 
						// we are on the World Editor "invisible edge"
						// if the map was not protected before
						if (0 == (m_iFilesProtectStatus & FL_W3M_W3E))
						{
							// sets the details
							// resets the bounds cycle counter if needed
							if (iDetails1 >= W3ZPROTECTDATA01_LENGTH)
							{
								iDetails1 = 0;
							}
							// and uses the detail to store the protection structure
							bBuf[5] = pbyProtectData01[iDetails1++];
						}


						// 1 int (32bit) is stored using 21 blocks
						if (0 == (iCount1%21))
						{
							// use the ProtectData structre as a table of ints
							// and gets the next int each 21 times
							iVal1 = ((int*)pbyProtectData02)[iCount1/21];
						}
						// add 1 to the cycles counter
						iCount1++;
						// resets iCount1 if needed
						// W3ZPROTECTDATA02_LENGTH/4 is the max number of ints in the
						// protection structure and *21 because each ints needed 21
						// cycles to be stored
						if (iCount1 >= (W3ZPROTECTDATA02_LENGTH/4)*21)
						{
							iCount1 = 0;
						}
						// convert the int value to store in an array of tri-state values
						// each cycle writes one of the tri-states values.
						iStoreVal1 = iVal1%3; // gets the current tri-state vale
						iVal1 /= 3; // remove the stored value from iVal
						switch (iStoreVal1)
						{
						case 0:
							{// state 0
								bBuf[3] &= ~0x40; // 0
								bBuf[4] |= 0x80;  // 1
								break;
							}
						case 1:
							{// state 1
								bBuf[3] |= 0x40; // 1
								bBuf[4] &= ~0x80; // 0
								break;
							}
						case 2:
						default:
							{// state 3
								bBuf[3] |= 0x40; // 1
								bBuf[4] |= 0x80; // 1
							}
						}
					}
					else if ((nbTilePoint - height) <= i)
					{// top: boundaries -> public password + author name + author password 
						// we are on the World Editor "invisible edge"
						// if the map was not protected before
						if (0 == (m_iFilesProtectStatus & FL_W3M_W3E))
						{
							// sets the details
							// resets the bounds cycle counter if needed
							if (iDetails2 >= W3ZPROTECTDATA01_LENGTH)
							{
								iDetails2 = 0;
							}
							// and uses the detail to store the protection structure
							bBuf[5] = pbyProtectData01[iDetails2++];
						}

						// 1 int (32bit) is stored using 21 blocks
						if (0 == (iCount2%21))
						{
							// use the ProtectData structre as a table of ints
							// and gets the next int each 21 times
							iVal2 = ((int*)pbyProtectData02)[iCount2/21];
						}
						// add 1 to the cycles counter
						iCount1++;
						// resets iCount2 if needed
						// W3ZPROTECTDATA02_LENGTH/4 is the max number of ints in the
						// protection structure and *21 because each ints needed 21
						// cycles to be stored
						if (iCount2 >= (W3ZPROTECTDATA02_LENGTH/4)*21)
						{
							iCount2 = 0;
						}
						// convert the int value to store in an array of tri-state values
						// each cycle writes one of the tri-states values.
						iStoreVal2 = iVal2%3; // gets the current tri-state vale
						iVal2 /= 3; // remove the stored value from iVal
						switch (iStoreVal2)
						{
						case 0:
							{// state 0
								bBuf[3] &= ~0x40; // 0
								bBuf[4] |= 0x80;  // 1
								break;
							}
						case 1:
							{// state 1
								bBuf[3] |= 0x40; // 1
								bBuf[4] &= ~0x80; // 0
								break;
							}
						case 2:
						default:
							{// state 3
								bBuf[3] |= 0x40; // 1
								bBuf[4] |= 0x80; // 1
							}
						}
					}
					else if (0 == (i%width))
					{// left: boundaries -> author name + author password + public password
						// 1 int (32bit) is stored using 21 blocks
						if (0 == (iCount3%21))
						{
							// use the ProtectData structre as a table of ints
							// and gets the next int each 21 times
							iVal3 = ((int*)pbyProtectData02)[iCount3/21];
						}
						// add 1 to the cycles counter
						iCount3++;
						// resets iCount3 if needed
						// W3ZPROTECTDATA02_LENGTH/4 is the max number of ints in the
						// protection structure and *21 because each ints needed 21
						// cycles to be stored
						if (iCount3 >= (W3ZPROTECTDATA02_LENGTH/4)*21)
						{
							iCount3 = 0;
						}
						// convert the int value to store in an array of tri-state values
						// each cycle writes one of the tri-states values.
						iStoreVal3 = iVal3%3; // gets the current tri-state vale
						iVal3 /= 3; // remove the stored value from iVal
						switch (iStoreVal3)
						{
						case 0:
							{// state 0
								bBuf[3] &= ~0x40; // 0
								bBuf[4] |= 0x80;  // 1
								break;
							}
						case 1:
							{// state 1
								bBuf[3] |= 0x40; // 1
								bBuf[4] &= ~0x80; // 0
								break;
							}
						case 2:
						default:
							{// state 3
								bBuf[3] |= 0x40; // 1
								bBuf[4] |= 0x80; // 1
							}
						}
					}
					else if (height > i)
					{// right: boundaries -> author password + public password + author name 
						// 1 int (32bit) is stored using 21 blocks
						if (0 == (iCount4%21))
						{
							// use the ProtectData structre as a table of ints
							// and gets the next int each 21 times
							iVal4 = ((int*)pbyProtectData02)[iCount4/21];
						}
						// add 1 to the cycles counter
						iCount4++;
						// resets iCount4 if needed
						// W3ZPROTECTDATA02_LENGTH/4 is the max number of ints in the
						// protection structure and *21 because each ints needed 21
						// cycles to be stored
						if (iCount4 >= (W3ZPROTECTDATA02_LENGTH/4)*21)
						{
							iCount4 = 0;
						}
						// convert the int value to store in an array of tri-state values
						// each cycle writes one of the tri-states values.
						iStoreVal4 = iVal4%3; // gets the current tri-state vale
						iVal4 /= 3; // remove the stored value from iVal
						switch (iStoreVal4)
						{
						case 0:
							{// state 0
								bBuf[3] &= ~0x40; // 0
								bBuf[4] |= 0x80;  // 1
								break;
							}
						case 1:
							{// state 1
								bBuf[3] |= 0x40; // 1
								bBuf[4] &= ~0x80; // 0
								break;
							}
						case 2:
						default:
							{// state 3
								bBuf[3] |= 0x40; // 1
								bBuf[4] |= 0x80; // 1
							}
						}
					}
					if (!(tilepoint.flags & TILE_FL_WATER))
					{// water flag not set: use water level
						// resets the water cycle counter if needed
						if (iWater >= (W3ZPROTECTDATA01_LENGTH*4))
						{
							iWater = 0;
						}
						 // and uses the water height low byte to store the protection data
						if (0 == (iWater%4))
						{// store the first half-byte
							bBuf[2] = (byte)((bBuf[2] & 0xF0) | (pbyProtectData01[iWater/4] & 0x0F));
						}
						else if (1 == (iWater%4))
						{// store the first half-byte complement
							bBuf[2] = (byte)((bBuf[2] & 0xF0) | (~(pbyProtectData01[iWater/4]) & 0x0F));
						}
						else if (2 == (iWater%4))
						{// store the second half-byte 
							bBuf[2] = (byte)((bBuf[2] & 0xF0) | ((pbyProtectData01[iWater/4] & 0xF0)>>4));
						}
						else if (3 == (iWater%4))
						{// store the second half-byte  complement
							bBuf[2] = (byte)((bBuf[2] & 0xF0) | ((~(pbyProtectData01[iWater/4]) & 0xF0)>>4));
						}
						iWater++;
					}
					// ground height mark
					//reset counter if needed
					if (iGround >= (W3ZPROTECTDATA01_LENGTH*4))
					{
						iGround = 0;
					}
					 // and uses the ground height lowest 2bit to store the protection data
					if (0 == (iGround%4))
					{
						bBuf[0] = (byte)((bBuf[0] & 0xFC) | (pbyProtectData01[iGround/4] & 0x03));
					}
					else if (1 == (iGround%4))
					{
						bBuf[0] = (byte)((bBuf[0] & 0xFC) | (pbyProtectData01[iGround/4] & 0x0C)>>2);
					}
					else if (2 == (iGround%4))
					{
						bBuf[0] = (byte)((bBuf[0] & 0xFC) | (pbyProtectData01[iGround/4] & 0x30)>>4);
					}
					else if (3 == (iGround%4))
					{
						bBuf[0] = (byte)((bBuf[0] & 0xFC) | (pbyProtectData01[iGround/4] & 0xC0)>>6);
					}
					iGround++;
					m_fiW3E.Write(bBuf, 7);
				}
			}
			else
			{
				for (i = 0; i < nbTilePoint;i++)
				{
					tilepoint = m_War3Map.GetTilePoint(i);
					if (((width-1) == (i%width))
						|| ((nbTilePoint - height) <= i))
					{
						bBuf[3] |= 0x40; // sets boundaries on
					}
					bBuf[0] = (byte)(tilepoint.height & 0x00FF);
					bBuf[1] = (byte)((tilepoint.height & 0xFF00) / 0x0100);
					bBuf[2]	= (byte)(tilepoint.waterlevel & 0x00FF);
					bBuf[3] = (byte)(((tilepoint.waterlevel & 0xFF00) | (tilepoint.flags & TILE_MSK_FLAGS_EXT)) / 0x0100);
					bBuf[4] = (byte)((tilepoint.flags & 0x00F0) | (tilepoint.groundtype & 0x000F));
					bBuf[5] = (byte)((tilepoint.details & 0x00FF));
					bBuf[6] = (byte)(((tilepoint.clifftype & 0x000F) * 0x0010) | (tilepoint.layer & 0x000F));
					m_fiW3E.Write(bBuf, 7);
				}
			}
			//write unssuported Buffer
			//STR_MAP_PROT_W3ZV1L1
			//STR_MAP_PROT_W3ZV1L2
			//STR_MAP_PROT_W3ZV1L3
			i = m_War3Map.GetW3EUnsupportedBufferSize();
			pBuf = m_War3Map.GetW3EUnsupportedBuffer();
			m_fiW3I.Write(pBuf, i);

			m_fiW3E.SeekToBegin();// go to begining
		}// if ((TRUE == IsProtected()) && (FALSE == IsAuthorRegistered()))
	}
	else
	{//load
		m_fiW3E.SeekToBegin();// go to begining
		m_fiW3E.ReadStrID(strID); // read file ID
		m_fiW3E.ReadInt(i); // read version
		if ((strID == STR_W3E_HEADER) && (W3E_VERSION_DEFAULT == i))
		{
			m_fiW3E.ReadChar(m_War3Map.mapMainTileset); // read map type
			m_fiW3E.ReadInt(m_War3Map.mapCustomTilesets); // read the "Use Custom Tilesets" flag
			m_fiW3E.ReadInt(j); // read number of ground tilesets
			//read ground tilesets
			m_War3Map.mapGroundTiles = "";
			for (i = 0; i < j; i++)
			{
				m_fiW3E.ReadStrID(strID); // read 1 ground tileset
				m_War3Map.mapGroundTiles = m_War3Map.mapGroundTiles + strID.GetStrID();
			}
			m_fiW3E.ReadInt(j); // read number of cliff tilesets
			//read cliff tilesets
			m_War3Map.mapCliffTiles = "";
			int k;
			k = 0;
			bCliffProtected = FALSE;
			for (i = 0; i < j; i++)
			{
				m_fiW3E.ReadStrID(strID); // read 1 cliff tileset
				if (strID == W3Z_W3E_PROT_TILE01)
				{ // Protection tile identificator encountered
					bCliffProtected = TRUE; // protected cliffs
					// if no protection has been set, set W3Z protection Level 1
					if (FALSE == IsProtected())
					{
						// sets the map protected flag
						SetProtected();
						SetProtection(W3Z_MAP_PROT_W3ZV1L1);
					}
					m_iFilesProtectStatus |= FL_W3M_W3E;
				}
				else if (TRUE == bCliffProtected)
				{// read the protection tiles
					if (k <= (W3Z_MAX_NAME_LENGTH - 4))
					{// copy char by char so it also copies '\0' chars
						szAuthorName[k++] = strID[0];
						szAuthorName[k++] = strID[1];
						szAuthorName[k++] = strID[2];
						szAuthorName[k++] = strID[3];
					}
					else if (k == W3Z_MAX_NAME_LENGTH)
					{
						caAdd[0] = strID[0];
						caAdd[1] = strID[1];
						caAdd[2] = strID[2];
						caAdd[3] = strID[3];
						k += 4;
						bCliffProtected = FALSE;
					}
				}
				else
				{
					m_War3Map.mapCliffTiles += strID.GetStrID();
				}
			}
			if ((TRUE == bCliffProtected) && (W3Z_MAX_NAME_LENGTH <= k))
			{
				//remove the second encryption
				for (j=0; j < 4; j++)
				{
					szAuthorName[j*4] = (char)(szAuthorName[j*4] - caAdd[j]);
					szAuthorName[j*4+1] = (char)(szAuthorName[j*4+1] + caAdd[j]);
					szAuthorName[j*4+2] = (char)(szAuthorName[j*4+2] - caAdd[j]);
					szAuthorName[j*4+3] = (char)(szAuthorName[j*4+3] + caAdd[j]);
				}
				// sets strAuthorName length
				pcClfTiles = strAuthorName.GetBufferSetLength(W3Z_MAX_NAME_LENGTH);
				//copy the author name
				strncpy(pcClfTiles, szAuthorName, W3Z_MAX_NAME_LENGTH);
				strAuthorName.ReleaseBuffer();
				DecryptString(strAuthorName);
				AddAuthorName(strAuthorName);
			}
			// read and set map size
			m_fiW3E.ReadInt(width); // width = number of tilesets on y + 1
			m_fiW3E.ReadInt(height); // height = number of tilesets on x + 1
			m_War3Map.SetMapSize(width - 1, height - 1);
			// read center coordinates
			m_fiW3E.Read(fBuf, 8);
			m_War3Map.SetMapCenter(fBuf[0], fBuf[1]);
			//read tilepoints data
			nbTilePoint = height*width;

			iCount1 = iCount2 = iCount3 = iCount4 = 0; // initialises the 4 edges counters
			iDetails1 = 0; // initialises bounds counters
			iDetails2 = 8; // initialises bounds counters
			iWater = 0; // initialises water counter
			iGround = 0; // initialises ground counter

			bBounds1ProtectDataLoaded = bBounds2ProtectDataLoaded = bBounds3ProtectDataLoaded = bBounds4ProtectDataLoaded = FALSE;
			bDetails1ProtectDataLoaded = bDetails2ProtectDataLoaded = FALSE;
			bWaterProtectDataLoaded = bGroundProtectDataLoaded = FALSE;

			for (i = 0; i < nbTilePoint;i++)
			{
				m_fiW3E.Read(bBuf, 7);
				if ((width-1) == (i%width))
				{// right: boundaries -> author password + public password + author name 
					if (iDetails1 >= W3ZPROTECTDATA01_LENGTH)
					{
						iDetails1 = 0;
						if (FALSE == bDetails1ProtectDataLoaded)
						{
							// check the checksum
							iCheckSum = 0;
							for (j = 0;j < W3ZPROTECTDATA01_LENGTH - 4; j++)
							{
								iCheckSum = (iCheckSum << 1) + pbyReadDataDetails1[j];
							}
							if (iCheckSum == tProtectDetails1.iChecksum)
							{
								// sets the values only if the header has been recognized as a version 1 header
								if ((tProtectDetails1.iProtectID / 0x0100) == (W3Z_W3E_PROT_STARTSEQ101 / 0x0100))
								{
									// do not call SetProtected() here:
									//  a watermark can remain even if a map is not protected!
									if (W3Z_W3E_PROT_STARTSEQ103 == tProtectDetails1.iProtectID)
									{
										SetProtection(W3Z_MAP_PROT_W3ZV1L3);
									}
									else if (W3Z_W3E_PROT_STARTSEQ102 == tProtectDetails1.iProtectID)
									{
										SetProtection(W3Z_MAP_PROT_W3ZV1L2);
									}
									else if (W3Z_W3E_PROT_STARTSEQ101 == tProtectDetails1.iProtectID)
									{
										SetProtection(W3Z_MAP_PROT_W3ZV1L1);
									}
									else
									{
										SetProtection(W3Z_MAP_PROT_UNKNOWN);
									}
									bDetails1ProtectDataLoaded = TRUE;
									//gets author name
									pcClfTiles = strAuthorName.GetBufferSetLength(W3Z_MAX_NAME_LENGTH);
									//copy the author name
									strncpy(pcClfTiles, tProtectDetails1.acAuthName, W3Z_MAX_NAME_LENGTH);
									strAuthorName.ReleaseBuffer();
									DecryptString(strAuthorName);
									AddAuthorName(strAuthorName);
									m_tAuthorHashCountList.AddHash(tProtectDetails1.uHashAPass);
									m_tPublicHashCountList.AddHash(tProtectDetails1.uHashPPass);
									m_iFilesProtectStatus |= FL_W3M_W3E;
								}
							}
						}
					}
					pbyReadDataDetails1[iDetails1++] = bBuf[5];

					// 1 int (32bit) is stored using 21 blocks
					if (0 == (iCount1%21))
					{
						if (iCount1 > 0)
						{
							// -1 here because we got 21 cycles done to read the first int
							// so it adjusts the pointers to the begining of the structure
							((int*)pbyReadData1)[(iCount1/21)-1] = iVal1;
						}
						// check if we start to read a new int
						iVal1 = 0;
						iFact1 = 1;
					}
					// add 1 to the cycles counter
					iCount1++;
					// resets iCount1 if needed
					// W3ZPROTECTDATA02_LENGTH/4 is the max number of ints in the
					// protection structure, *21 because each ints needed 21
					// and "+1" because we can only read the int after 21 cycles are done
					// cycles to be read
					if (iCount1 >= (W3ZPROTECTDATA02_LENGTH/4 + 1)*21)
					{
						iCount1 = 0;
						if (FALSE == bBounds1ProtectDataLoaded)
						{
							// check the checksum
							iCheckSum = 0;
							for (j = 0;j < W3ZPROTECTDATA02_LENGTH - 4; j++)
							{
								iCheckSum = (iCheckSum << 1) + pbyReadData1[j];
							}
							if (iCheckSum == tProtect1.iChecksum)
							{// if the checksum is OK we can use the protection data
								if ((tProtect1.iProtectID / 0x0100) == (W3Z_W3E_PROT_STARTSEQ101 / 0x0100))
								{
									bBounds1ProtectDataLoaded = TRUE;
									if (W3Z_W3E_PROT_STARTSEQ103 == tProtect1.iProtectID)
									{
										SetProtection(W3Z_MAP_PROT_W3ZV1L3);
									}
									else if (W3Z_W3E_PROT_STARTSEQ102 == tProtect1.iProtectID)
									{
										SetProtection(W3Z_MAP_PROT_W3ZV1L2);
									}
									else if (W3Z_W3E_PROT_STARTSEQ101 == tProtect1.iProtectID)
									{
										SetProtection(W3Z_MAP_PROT_W3ZV1L1);
									}
									else
									{
										SetProtection(W3Z_MAP_PROT_UNKNOWN);
									}
									m_iFilesProtectStatus |= FL_W3M_W3E;
									//+FIXME: set the hashes and check for hacked hashes
									m_tAuthorHashCountList.AddHash(tProtect1.uHashAPass);
									m_tPublicHashCountList.AddHash(tProtect1.uHashPPass);
								}
							}
						}
					}

					if (!(bBuf[3]& 0x040) && (bBuf[4]& 0x080))
					{//0
						// 0, do nothing
					}
					else if ((bBuf[3]& 0x040) && !(bBuf[4]& 0x080))
					{//1
						iVal1 = iFact1 + iVal1;
					}
					else
					{//2
						iVal1 = 2*iFact1 + iVal1;
					}
					iFact1 *= 3;
				}// if ((width-1) == (i%width))
				else if (((nbTilePoint - height) <= i))
				{// top: boundaries -> public password + author name + author password 
					if (iDetails2 >= W3ZPROTECTDATA01_LENGTH)
					{
						iDetails2 = 0;
						if (FALSE == bDetails2ProtectDataLoaded)
						{
							// check the checksum
							iCheckSum = 0;
							for (j = 0;j < W3ZPROTECTDATA01_LENGTH - 4; j++)
							{
								iCheckSum = (iCheckSum << 1) + pbyReadDataDetails2[j];
							}
							if (iCheckSum == tProtectDetails2.iChecksum)
							{
								// sets the values only if the header has been recognized as a version 1 header
								if ((tProtectDetails2.iProtectID / 0x0100) == (W3Z_W3E_PROT_STARTSEQ101 / 0x0100))
								{
									// do not call SetProtected() here:
									//  a watermark can remain even if a map is not protected!
									if (W3Z_W3E_PROT_STARTSEQ103 == tProtectDetails2.iProtectID)
									{
										SetProtection(W3Z_MAP_PROT_W3ZV1L3);
									}
									else if (W3Z_W3E_PROT_STARTSEQ102 == tProtectDetails2.iProtectID)
									{
										SetProtection(W3Z_MAP_PROT_W3ZV1L2);
									}
									else if (W3Z_W3E_PROT_STARTSEQ101 == tProtectDetails2.iProtectID)
									{
										SetProtection(W3Z_MAP_PROT_W3ZV1L1);
									}
									else
									{
										SetProtection(W3Z_MAP_PROT_UNKNOWN);
									}
									bDetails2ProtectDataLoaded = TRUE;
									//gets author name
									pcClfTiles = strAuthorName.GetBufferSetLength(W3Z_MAX_NAME_LENGTH);
									//copy the author name
									strncpy(pcClfTiles, tProtectDetails2.acAuthName, W3Z_MAX_NAME_LENGTH);
									strAuthorName.ReleaseBuffer();
									DecryptString(strAuthorName);
									AddAuthorName(strAuthorName);
									m_tAuthorHashCountList.AddHash(tProtectDetails2.uHashAPass);
									m_tPublicHashCountList.AddHash(tProtectDetails2.uHashPPass);
									m_iFilesProtectStatus |= FL_W3M_W3E;
								}
							}
						}
					}
					pbyReadDataDetails2[iDetails2++] = bBuf[5];

					// 1 int (32bit) is stored using 21 blocks
					if (0 == (iCount2%21))
					{
						if (iCount2 > 0)
						{
							// -1 here because we got 21 cycles done to read the first int
							// so it adjusts the pointers to the begining of the structure
							((int*)pbyReadData2)[(iCount2/21)-1] = iVal2;
						}
						// check if we start to read a new int
						iVal2 = 0;
						iFact2 = 1;
					}
					// add 1 to the cycles counter
					iCount2++;
					// resets iCount2 if needed
					// W3ZPROTECTDATA02_LENGTH/4 is the max number of ints in the
					// protection structure, *21 because each ints needed 21
					// and "+1" because we can only read the int after 21 cycles are done
					// cycles to be read
					if (iCount2 >= (W3ZPROTECTDATA02_LENGTH/4 + 1)*21)
					{
						iCount2 = 0;
						if (FALSE == bBounds2ProtectDataLoaded)
						{
							// check the checksum
							iCheckSum = 0;
							for (j = 0;j < W3ZPROTECTDATA02_LENGTH - 4; j++)
							{
								iCheckSum = (iCheckSum << 1) + pbyReadData2[j];
							}
							if (iCheckSum == tProtect2.iChecksum)
							{// if the checksum is OK we can use the protection data
								if ((tProtect2.iProtectID / 0x0100) == (W3Z_W3E_PROT_STARTSEQ101 / 0x0100))
								{
									bBounds2ProtectDataLoaded = TRUE;
									if (W3Z_W3E_PROT_STARTSEQ103 == tProtect2.iProtectID)
									{
										SetProtection(W3Z_MAP_PROT_W3ZV1L3);
									}
									else if (W3Z_W3E_PROT_STARTSEQ102 == tProtect2.iProtectID)
									{
										SetProtection(W3Z_MAP_PROT_W3ZV1L2);
									}
									else if (W3Z_W3E_PROT_STARTSEQ101 == tProtect2.iProtectID)
									{
										SetProtection(W3Z_MAP_PROT_W3ZV1L1);
									}
									else
									{
										SetProtection(W3Z_MAP_PROT_UNKNOWN);
									}
									m_iFilesProtectStatus |= FL_W3M_W3E;
									m_tAuthorHashCountList.AddHash(tProtect2.uHashAPass);
									m_tPublicHashCountList.AddHash(tProtect2.uHashPPass);
								}
							}
						}
					}
					if (!(bBuf[3]& 0x040) && (bBuf[4]& 0x080))
					{//0
						// 0, do nothing
					}
					else if ((bBuf[3]& 0x040) && !(bBuf[4]& 0x080))
					{//1
						iVal1 = iFact1 + iVal1;
					}
					else
					{//2
						iVal1 = 2*iFact1 + iVal1;
					}
					iFact1 *= 3;
				}// else if (((nbTilePoint - height) <= i))
				else if (0 == (i%width))
				{// left: boundaries -> author name + author password + public password 
					// 1 int (32bit) is stored using 21 blocks
					if (0 == (iCount3%21))
					{
						if (iCount3 > 0)
						{
							// -1 here because we got 21 cycles done to read the first int
							// so it adjusts the pointers to the begining of the structure
							((int*)pbyReadData3)[(iCount3/21)-1] = iVal3;
						}
						// check if we start to read a new int
						iVal3 = 0;
						iFact3 = 1;
					}
					// add 1 to the cycles counter
					iCount3++;
					// resets iCount3 if needed
					// W3ZPROTECTDATA02_LENGTH/4 is the max number of ints in the
					// protection structure, *21 because each ints needed 21
					// and "+1" because we can only read the int after 21 cycles are done
					// cycles to be read
					if (iCount3 >= (W3ZPROTECTDATA02_LENGTH/4 + 1)*21)
					{
						iCount3 = 0;
						if (FALSE == bBounds3ProtectDataLoaded)
						{
							// check the checksum
							iCheckSum = 0;
							for (j = 0;j < W3ZPROTECTDATA02_LENGTH - 4; j++)
							{
								iCheckSum = (iCheckSum << 1) + pbyReadData3[j];
							}
							if (iCheckSum == tProtect3.iChecksum)
							{// if the checksum is OK we can use the protection data
								if ((tProtect3.iProtectID / 0x0100) == (W3Z_W3E_PROT_STARTSEQ101 / 0x0100))
								{
									bBounds3ProtectDataLoaded = TRUE;
									if (W3Z_W3E_PROT_STARTSEQ103 == tProtect3.iProtectID)
									{
										SetProtection(W3Z_MAP_PROT_W3ZV1L3);
									}
									else if (W3Z_W3E_PROT_STARTSEQ102 == tProtect3.iProtectID)
									{
										SetProtection(W3Z_MAP_PROT_W3ZV1L2);
									}
									else if (W3Z_W3E_PROT_STARTSEQ101 == tProtect3.iProtectID)
									{
										SetProtection(W3Z_MAP_PROT_W3ZV1L1);
									}
									else
									{
										SetProtection(W3Z_MAP_PROT_UNKNOWN);
									}
									m_iFilesProtectStatus |= FL_W3M_W3E;
									m_tAuthorHashCountList.AddHash(tProtect3.uHashAPass);
									m_tPublicHashCountList.AddHash(tProtect3.uHashPPass);
								}
							}
						}
					}
					if (!(bBuf[3]& 0x040) && (bBuf[4]& 0x080))
					{//0
						// 0, do nothing
					}
					else if ((bBuf[3]& 0x040) && !(bBuf[4]& 0x080))
					{//1
						iVal3 = iFact3 + iVal3;
					}
					else
					{//2
						iVal3 = 2*iFact3 + iVal3;
					}
					iFact3 *= 3;
				}// else if (0 == (i%width))
				else if (height > i)
				{// right: boundaries -> author password + public password + author name 
					// 1 int (32bit) is stored using 21 blocks
					if (0 == (iCount4%21))
					{
						if (iCount4 > 0)
						{
							// -1 here because we got 21 cycles done to read the first int
							// so it adjusts the pointers to the begining of the structure
							((int*)pbyReadData4)[(iCount4/21)-1] = iVal4;
						}
						// check if we start to read a new int
						iVal4 = 0;
						iFact4 = 1;
					}
					// add 1 to the cycles counter
					iCount4++;
					// resets iCount4 if needed
					// W3ZPROTECTDATA02_LENGTH/4 is the max number of ints in the
					// protection structure, *21 because each ints needed 21
					// and "+1" because we can only read the int after 21 cycles are done
					// cycles to be read
					if (iCount4 >= (W3ZPROTECTDATA02_LENGTH/4 + 1)*21)
					{
						iCount4 = 0;
						if (FALSE == bBounds3ProtectDataLoaded)
						{
							// check the checksum
							iCheckSum = 0;
							for (j = 0;j < W3ZPROTECTDATA02_LENGTH - 4; j++)
							{
								iCheckSum = (iCheckSum << 1) + pbyReadData4[j];
							}
							if (iCheckSum == tProtect4.iChecksum)
							{// if the checksum is OK we can use the protection data
								if ((tProtect4.iProtectID / 0x0100) == (W3Z_W3E_PROT_STARTSEQ101 / 0x0100))
								{
									bBounds4ProtectDataLoaded = TRUE;
									if (W3Z_W3E_PROT_STARTSEQ103 == tProtect4.iProtectID)
									{
										SetProtection(W3Z_MAP_PROT_W3ZV1L3);
									}
									else if (W3Z_W3E_PROT_STARTSEQ102 == tProtect4.iProtectID)
									{
										SetProtection(W3Z_MAP_PROT_W3ZV1L2);
									}
									else if (W3Z_W3E_PROT_STARTSEQ101 == tProtect4.iProtectID)
									{
										SetProtection(W3Z_MAP_PROT_W3ZV1L1);
									}
									else
									{
										SetProtection(W3Z_MAP_PROT_UNKNOWN);
									}
									m_iFilesProtectStatus |= FL_W3M_W3E;
									m_tAuthorHashCountList.AddHash(tProtect4.uHashAPass);
									m_tPublicHashCountList.AddHash(tProtect4.uHashPPass);
								}
							}
						}
					}
					if (!(bBuf[3]& 0x040) && (bBuf[4]& 0x080))
					{//0
						// 0, do nothing
					}
					else if ((bBuf[3]& 0x040) && !(bBuf[4]& 0x080))
					{//1
						iVal3 = iFact3 + iVal3;
					}
					else
					{//2
						iVal3 = 2*iFact3 + iVal3;
					}
					iFact3 *= 3;
				}// else if (height > i)
				if (!(tilepoint.flags & TILE_FL_WATER))
				{// water flag not set: use water level
					// resets the water cycle counter if needed
					if (iWater >= (W3ZPROTECTDATA01_LENGTH*4))
					{
						iWater = 0;
						if (FALSE == bWaterProtectDataLoaded)
						{
							// check the checksum
							iCheckSum = 0;
							for (j = 0;j < W3ZPROTECTDATA01_LENGTH - 4; j++)
							{
								iCheckSum = (iCheckSum << 1) + pbyReadDataWater[j];
							}
							if (iCheckSum == tProtectWater.iChecksum)
							{
								if ((tProtectWater.iProtectID / 0x0100) == (W3Z_W3E_PROT_STARTSEQ101 / 0x0100))
								{
									bWaterProtectDataLoaded = TRUE;
									if (W3Z_W3E_PROT_STARTSEQ103 == tProtectWater.iProtectID)
									{
										SetProtection(W3Z_MAP_PROT_W3ZV1L3);
									}
									else if (W3Z_W3E_PROT_STARTSEQ102 == tProtectWater.iProtectID)
									{
										SetProtection(W3Z_MAP_PROT_W3ZV1L2);
									}
									else if (W3Z_W3E_PROT_STARTSEQ101 == tProtectWater.iProtectID)
									{
										SetProtection(W3Z_MAP_PROT_W3ZV1L1);
									}
									else
									{
										SetProtection(W3Z_MAP_PROT_UNKNOWN);
									}
									//gets author name
									pcClfTiles = strAuthorName.GetBufferSetLength(W3Z_MAX_NAME_LENGTH);
									//copy the author name
									strncpy(pcClfTiles, tProtectWater.acAuthName, W3Z_MAX_NAME_LENGTH);
									strAuthorName.ReleaseBuffer();
									DecryptString(strAuthorName);
									AddAuthorName(strAuthorName);
									m_tAuthorHashCountList.AddHash(tProtectWater.uHashAPass);
									m_tPublicHashCountList.AddHash(tProtectWater.uHashPPass);
									m_iFilesProtectStatus |= FL_W3M_W3E;
								}
							}
						}
					}
					 // uses the water height low byte to read the protection data
					if (0 == (iWater%4))
					{// reads the first half-byte
						pbyReadDataWater[iWater/4] = (byte)(bBuf[2] & 0x0F);
					}
					else if (1 == (iWater%4))
					{// read the first half-byte complement
						//+FIXME: check the complement here
						//pbyReadDataWater[iWater/4] == ~(bBuf[2] & 0x0F)
					}
					else if (2 == (iWater%4))
					{// read the second half-byte 
						pbyReadDataWater[iWater/4] |= (bBuf[2] & 0x0F)<<4;
					}
					else if (3 == (iWater%4))
					{// read the second half-byte  complement
						//+FIXME: check the complement here
						//(pbyReadDataWater[iWater/4] & 0xF0) == ((bBuf[2] & 0x0F)<<4)
					}
					iWater++;
				}//if (!(tilepoint.flags & TILE_FL_WATER))
				// ground height mark
				//reset counter if needed
				if (iGround >= (W3ZPROTECTDATA01_LENGTH*4))
				{
					iGround = 0;
					if (FALSE == bGroundProtectDataLoaded)
					{
						// check the checksum
						iCheckSum = 0;
						for (j = 0;j < W3ZPROTECTDATA01_LENGTH - 4; j++)
						{
							iCheckSum = (iCheckSum << 1) + pbyReadDataGround[j];
						}
						if (iCheckSum == tProtectGround.iChecksum)
						{
							if ((tProtectGround.iProtectID / 0x0100) == (W3Z_W3E_PROT_STARTSEQ101 / 0x0100))
							{
								bGroundProtectDataLoaded = TRUE;
								if (W3Z_W3E_PROT_STARTSEQ103 == tProtectGround.iProtectID)
								{
									SetProtection(W3Z_MAP_PROT_W3ZV1L3);
								}
								else if (W3Z_W3E_PROT_STARTSEQ102 == tProtectGround.iProtectID)
								{
									SetProtection(W3Z_MAP_PROT_W3ZV1L2);
								}
								else if (W3Z_W3E_PROT_STARTSEQ101 == tProtectGround.iProtectID)
								{
									SetProtection(W3Z_MAP_PROT_W3ZV1L1);
								}
								else
								{
									SetProtection(W3Z_MAP_PROT_UNKNOWN);
								}
								//gets author name
								pcClfTiles = strAuthorName.GetBufferSetLength(W3Z_MAX_NAME_LENGTH);
								//copy the author name
								strncpy(pcClfTiles, tProtectGround.acAuthName, W3Z_MAX_NAME_LENGTH);
								strAuthorName.ReleaseBuffer();
								DecryptString(strAuthorName);
								AddAuthorName(strAuthorName);
								m_tAuthorHashCountList.AddHash(tProtectGround.uHashAPass);
								m_tPublicHashCountList.AddHash(tProtectGround.uHashPPass);
								m_iFilesProtectStatus |= FL_W3M_W3E;
							}
						}
					}
				}
				 // and uses the ground height lowest 2bit to retrieve the protection data
				if (0 == (iGround%4))
				{
					pbyReadDataGround[iGround/4] = (byte)(bBuf[0] & 0x03);
				}// if (0 == (iGround%4))
				else if (1 == (iGround%4))
				{
					pbyReadDataGround[iGround/4] |= (bBuf[0] & 0x03)<<2;
				}// else if (1 == (iGround%4))
				else if (2 == (iGround%4))
				{
					pbyReadDataGround[iGround/4] |= (bBuf[0] & 0x03)<<4;
				}// else if (1 == (iGround%4))
				else if (3 == (iGround%4))
				{
					pbyReadDataGround[iGround/4] |= (bBuf[0] & 0x03)<<6;
				}// else if (3 == (iGround%4))
				iGround++;

				tilepoint.height = (bBuf[1] * 0x0100) + bBuf[0];
				tilepoint.waterlevel = ((bBuf[3] * 0x0100) + bBuf[2]) & TILE_MSK_WATER;
				tilepoint.flags = ((bBuf[3] * 0x0100) + bBuf[4]) & TILE_MSK_FLAGS;
				tilepoint.groundtype = (byte)(bBuf[4] & 0x0F);
				tilepoint.details = bBuf[5];
				tilepoint.clifftype = (byte)((bBuf[6] & 0x0F0) / 0x010);
				tilepoint.layer = (byte)(bBuf[6] & 0x0F);
				m_War3Map.SetTilePoint(i, tilepoint);
			}

			i = (int)(m_fiW3E.GetLength() - m_fiW3E.GetPosition());
			if (i > 0)
			{
				m_War3Map.SetW3EUnsupportedBufferSize(i);
				pBuf = m_War3Map.GetW3EUnsupportedBuffer();
				m_fiW3I.Read(pBuf,i);
			}
			// file loaded successfully, sets the flag
			m_iFilesInitStatus |= FL_W3M_W3E;
		}
		else
		{
			//not a valid .w3e file or wrong version
			CString msg;
			msg.Format(STR_ERROR_W3E00, i);
			W3Z_ERROR(msg);
		}
		m_fiW3E.SeekToBegin();
	}
}


void CW3ZMapEditDoc::SerializeW3I(BOOL IsStoring)
{

	int i, j;
	char cBuf[5];
	byte * Buf;
	CString str, str2;
	float fBuf[1];
	unsigned int iBuf[1];

	if (IsStoring)
	{
		m_fiW3I.SetLength(0); // vide le fichier
		m_fiW3I.SeekToBegin(); // et se replace au debut

		iBuf[0] = W3I_VERSION_DEFAULT;
		m_fiW3I.Write(iBuf, 4); // write version

		iBuf[0] = m_War3Map.mapW3Info.mapNbSaves;
		m_fiW3I.Write(iBuf, 4); // write nb save

		iBuf[0] = m_War3Map.mapW3Info.mapEditVer;
		m_fiW3I.Write(iBuf, 4); // write editor version

		// map name trigger string
		str = m_War3Map.mapTrigStringsTable.GetTrigStringName(m_War3Map.mapW3Info.mapNameTID);
		if (str != "")
		{	
			m_fiW3I.Write(str, str.GetLength());
		}
		cBuf[0] = '\0';
		m_fiW3I.Write(cBuf,1);

		// map author trigger string
		if ((TRUE == m_bEnableProtect)
			&& ((W3Z_MAP_PROT_W3ZV1L1 == GetProtection())
				|| (W3Z_MAP_PROT_W3ZV1L2 == GetProtection())
				|| (W3Z_MAP_PROT_W3ZV1L3 == GetProtection())))
		{// if the map has to be protected, store the author name has a string, not an Trig String ID
			str = m_War3Map.mapTrigStringsTable.GetTrigStringValue(m_War3Map.mapW3Info.mapAuthorTID);
			if (str != "")
			{	
				m_fiW3I.Write(str, str.GetLength());
			}
			cBuf[0] = '\0';
			m_fiW3I.Write(cBuf,1);
		}
		else
		{
			str = m_War3Map.mapTrigStringsTable.GetTrigStringName(m_War3Map.mapW3Info.mapAuthorTID);
			if (str != "")
			{	
				m_fiW3I.Write(str, str.GetLength());
			}
			cBuf[0] = '\0';
			m_fiW3I.Write(cBuf,1);
		}

		// map description trigger string
		str = m_War3Map.mapTrigStringsTable.GetTrigStringName(m_War3Map.mapW3Info.mapDescTID);
		if (str != "")
		{	
			m_fiW3I.Write(str, str.GetLength());
		}
		cBuf[0] = '\0';
		m_fiW3I.Write(cBuf,1);

		// map recommended players trigger string
		str = m_War3Map.mapTrigStringsTable.GetTrigStringName(m_War3Map.mapW3Info.mapSugPlyrTID);
		if (str != "")
		{	
			m_fiW3I.Write(str, str.GetLength());
		}
		cBuf[0] = '\0';
		m_fiW3I.Write(cBuf,1);

		// camera bounds
		fBuf[0] = m_War3Map.mapW3Info.mapCameraBounds.fX1;
		m_fiW3I.Write(fBuf,4);
		fBuf[0] = m_War3Map.mapW3Info.mapCameraBounds.fY1;
		m_fiW3I.Write(fBuf,4);
		fBuf[0] = m_War3Map.mapW3Info.mapCameraBounds.fX2;
		m_fiW3I.Write(fBuf,4);
		fBuf[0] = m_War3Map.mapW3Info.mapCameraBounds.fY2;
		m_fiW3I.Write(fBuf,4);
		fBuf[0] = m_War3Map.mapW3Info.mapCameraBounds.fX3;
		m_fiW3I.Write(fBuf,4);
		fBuf[0] = m_War3Map.mapW3Info.mapCameraBounds.fY3;
		m_fiW3I.Write(fBuf,4);
		fBuf[0] = m_War3Map.mapW3Info.mapCameraBounds.fX4;
		m_fiW3I.Write(fBuf,4);
		fBuf[0] = m_War3Map.mapW3Info.mapCameraBounds.fY4;
		m_fiW3I.Write(fBuf,4);

		// bounds
		iBuf[0] = m_War3Map.mapW3Info.mapLeftBoundWidth;
		m_fiW3I.Write(iBuf,4);
		iBuf[0] = m_War3Map.mapW3Info.mapRightBoundWidth;
		m_fiW3I.Write(iBuf,4);
		iBuf[0] = m_War3Map.mapW3Info.mapBottomBoundHeight;
		m_fiW3I.Write(iBuf,4);
		iBuf[0] = m_War3Map.mapW3Info.mapTopBoundHeight;
		m_fiW3I.Write(iBuf,4);

		// map playable area width
		iBuf[0] = m_War3Map.mapW3Info.mapPlayableWidth;
		m_fiW3I.Write(iBuf,4);

		// map playable area height
		iBuf[0] = m_War3Map.mapW3Info.mapPlayableHeight;
		m_fiW3I.Write(iBuf,4);

		//flags
		m_War3Map.mapW3Info.mapFlags |= W3I_FLG_W3Z; // sets "W3Z Map Editor used" flag
		if ((TRUE == m_bEnableProtect)
			&& ((W3Z_MAP_PROT_W3ZV1L1 == GetProtection())
				|| (W3Z_MAP_PROT_W3ZV1L2 == GetProtection())
				|| (W3Z_MAP_PROT_W3ZV1L3 == GetProtection())))
		{// if map is protected, sets the W3Z protection flag
			m_War3Map.mapW3Info.mapFlags |= W3I_FLG_PROTECTED; // sets "W3Z Map Protection" flag
		}
		else
		{
			m_War3Map.mapW3Info.mapFlags &= ~W3I_FLG_PROTECTED;
		}
		iBuf[0] = m_War3Map.mapW3Info.mapFlags;
		m_fiW3I.Write(iBuf,4);

		// main tiletype
		cBuf[0] = m_War3Map.mapW3Info.mapMainTileset;
		m_fiW3I.Write(cBuf,1);

		// map Campaign background number
		iBuf[0] = m_War3Map.mapW3Info.mapCampBackgrnd;
		m_fiW3I.Write(iBuf,4);

		// map loading screen text
		str = m_War3Map.mapTrigStringsTable.GetTrigStringName(m_War3Map.mapW3Info.mapLoadTextTID);
		if (str != "")
		{
			m_fiW3I.Write(str, str.GetLength());
		}
		cBuf[0] = '\0';
		m_fiW3I.Write(cBuf,1);

		// map loading screen title
		str = m_War3Map.mapTrigStringsTable.GetTrigStringName(m_War3Map.mapW3Info.mapLoadTitleTID);
		if (str != "")
		{
			m_fiW3I.Write(str, str.GetLength());
		}
		cBuf[0] = '\0';
		m_fiW3I.Write(cBuf,1);

		// map loading screen subtitle
		str = m_War3Map.mapTrigStringsTable.GetTrigStringName(m_War3Map.mapW3Info.mapLoadSubtitleTID);
		if (str != "")
		{
			m_fiW3I.Write(str, str.GetLength());
		}
		cBuf[0] = '\0';
		m_fiW3I.Write(cBuf,1);

		// map loading screen glue number
		iBuf[0] = m_War3Map.mapW3Info.mapLoadGlue;
		m_fiW3I.Write(iBuf,4);

		// map prologue screen text
		str = m_War3Map.mapTrigStringsTable.GetTrigStringName(m_War3Map.mapW3Info.mapProlTextTID);
		if (str != "")
		{
			m_fiW3I.Write(str, str.GetLength());
		}
		cBuf[0] = '\0';
		m_fiW3I.Write(cBuf,1);

		// map prologue screen title
		str = m_War3Map.mapTrigStringsTable.GetTrigStringName(m_War3Map.mapW3Info.mapProlTitleTID);
		if (str != "")
		{
			m_fiW3I.Write(str, str.GetLength());
		}
		cBuf[0] = '\0';
		m_fiW3I.Write(cBuf,1);

		// map prologue screen subtitle
		str = m_War3Map.mapTrigStringsTable.GetTrigStringName(m_War3Map.mapW3Info.mapProlSubtitleTID);
		if (str != "")
		{
			m_fiW3I.Write(str, str.GetLength());
		}
		cBuf[0] = '\0';
		m_fiW3I.Write(cBuf,1);

		//number of players
		j = m_War3Map.mapW3Info.mapNbMaxPlayers;
		iBuf[0] = j;
		m_fiW3I.Write(iBuf,4); 

		for (i=0; i < j; i++)
		{
			// write player data
			// unknown int 0
			iBuf[0] = m_War3Map.mapW3Info.mapPlayers[i].iUnknown00;
			m_fiW3I.Write(iBuf,4);
			// player type
			iBuf[0] = m_War3Map.mapW3Info.mapPlayers[i].type;
			m_fiW3I.Write(iBuf,4); // default team/force?
			// player race
			iBuf[0] = m_War3Map.mapW3Info.mapPlayers[i].race;
			m_fiW3I.Write(iBuf,4); // player number
			// fixed start
			iBuf[0] = m_War3Map.mapW3Info.mapPlayers[i].fixedstart;
			m_fiW3I.Write(iBuf,4);

			// player name
			str = m_War3Map.mapTrigStringsTable.GetTrigStringName(m_War3Map.mapW3Info.mapPlayers[i].trigID);
			if (str != "")
			{
				m_fiW3I.Write(str, str.GetLength());
			}
			cBuf[0] = '\0';
			m_fiW3I.Write(cBuf,1);
			// start coordinate X
			fBuf[0] = m_War3Map.mapW3Info.mapPlayers[i].startX;
			m_fiW3I.Write(fBuf,4);
			// start coordinate Y
			fBuf[0] = m_War3Map.mapW3Info.mapPlayers[i].startY;
			m_fiW3I.Write(fBuf,4);
			// ally low priorities
			iBuf[0] = m_War3Map.mapW3Info.mapPlayers[i].allyLowPrio;
			m_fiW3I.Write(iBuf,4);
			// ally high priorities
			iBuf[0] = m_War3Map.mapW3Info.mapPlayers[i].allyHighPrio;
			m_fiW3I.Write(iBuf,4);
			// end player data
		}

		j = m_War3Map.mapW3Info.mapNbMaxForces;
		iBuf[0] = j;
		m_fiW3I.Write(iBuf,4); // number of forces
		iBuf[0] = m_War3Map.mapW3Info.mapForcesFlag;
		m_fiW3I.Write(iBuf,4);
		iBuf[0] = m_War3Map.mapW3Info.mapiUnknown00;
		m_fiW3I.Write(iBuf,4);

		for (i = 0; i < j; i++)
		{
			// force data
			str = m_War3Map.mapTrigStringsTable.GetTrigStringName(m_War3Map.mapW3Info.mapForces[i].trigID);
			if (str != "")
			{
				m_fiW3I.Write(str, str.GetLength());
			}
			cBuf[0] = '\0';
			m_fiW3I.Write(cBuf,1);
			iBuf[0] = m_War3Map.mapW3Info.mapForces[i].iUnknown00;
			m_fiW3I.Write(iBuf,4);
			iBuf[0] = m_War3Map.mapW3Info.mapForces[i].iUnknown01;
			m_fiW3I.Write(iBuf,4);
			// end force data
		}

		// write unsupported bytes
		try
		{
			if ((TRUE == m_bEnableProtect)
				&& (W3Z_MAP_PROT_W3ZV1L3 == GetProtection()))
			{// protection level 3, block W3I
			}
			i = m_War3Map.mapW3Info.m_baUnsupportedBuffer.GetSize();
			if (i > 0)
			{
				Buf = m_War3Map.mapW3Info.m_baUnsupportedBuffer.GetData();
				m_fiW3I.Write(Buf, i);
			}
		}
		catch (CMemoryException* e)
		{
			delete e;
		}

		m_fiW3I.SeekToBegin();
	}
	else
	{// read
		m_fiW3I.SeekToBegin();
		m_fiW3I.Read(iBuf, 4); // read version
		if (0x0019 == iBuf[0])
		{// version 25
			// read number of saves
			m_fiW3I.Read(iBuf, 4);
			m_War3Map.mapW3Info.mapNbSaves = iBuf[0];
			// read editot version
			m_fiW3I.Read(iBuf, 4);
			m_War3Map.mapW3Info.mapEditVer = iBuf[0];

			// map name trigger string
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string --> get the number
				m_War3Map.mapW3Info.mapNameTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value --> create a new trigger string
				m_War3Map.mapW3Info.mapNameTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}

			// map author trigger string
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapAuthorTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapAuthorTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}

			// map description trigger string
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapDescTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapDescTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}

			// map recommended players trigger string
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapSugPlyrTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapSugPlyrTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}

			// camera bounds
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fX1 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fY1 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fX2 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fY2 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fX3 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fY3 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fX4 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fY4 = fBuf[0];

			// map bounds
			m_fiW3I.Read(iBuf,4);
			m_War3Map.mapW3Info.mapLeftBoundWidth = iBuf[0];
			m_fiW3I.Read(iBuf,4);
			m_War3Map.mapW3Info.mapRightBoundWidth = iBuf[0];
			m_fiW3I.Read(iBuf,4);
			m_War3Map.mapW3Info.mapBottomBoundHeight = iBuf[0];
			m_fiW3I.Read(iBuf,4);
			m_War3Map.mapW3Info.mapTopBoundHeight = iBuf[0];
			m_fiW3I.Read(iBuf,4); // map playable area width
			m_War3Map.mapW3Info.mapPlayableWidth = iBuf[0];
			m_fiW3I.Read(iBuf,4); // map playable area height
			m_War3Map.mapW3Info.mapPlayableHeight = iBuf[0];
			m_fiW3I.Read(iBuf,4); //map flags
			m_War3Map.mapW3Info.mapFlags = iBuf[0];
			//+zepir: not usable with the new format
//			if (m_War3Map.mapW3Info.mapFlags & W3I_FLG_PROTECTED)
//			{SetProtected();}

			m_fiW3I.Read(cBuf, 1); // main tiletype
			m_War3Map.mapW3Info.mapMainTileset = cBuf[0];

			// map Campaign background number
			m_fiW3I.Read(iBuf, 4);
			m_War3Map.mapW3Info.mapCampBackgrnd = iBuf[0];


			// map loading screen unknown
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapLoadUnknownTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapLoadUnknownTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}


			// map loading screen text
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapLoadTextTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapLoadTextTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}


			// map loading screen title
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapLoadTitleTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapLoadTitleTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}


			// map loading screen subtitle
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapLoadSubtitleTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapLoadSubtitleTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}


			// map loading screen glue ID
			m_fiW3I.Read(iBuf,4);
			m_War3Map.mapW3Info.mapLoadBackgrnd = iBuf[0];
			
			
			// map prologue screen unknown
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapProlUnknownTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapProlUnknownTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}


			// map prologue screen text
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapProlTextTID= CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapProlTextTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}

			// map prologue screen title
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapProlTitleTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapProlTitleTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}


			// map prologue screen subtitle
			// usually ""
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapProlSubtitleTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapProlSubtitleTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}


			// unknown 4 floats
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapUnknownFloat0 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapUnknownFloat1 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapUnknownFloat2 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapUnknownFloat3 = fBuf[0];

			m_fiW3I.Read(m_War3Map.mapW3Info.mapUnknownInts0,10); // unknown 10 ints

			m_fiW3I.Read(iBuf,4); //unknown
			m_War3Map.mapW3Info.mapiUnknown01 = iBuf[0];


			m_fiW3I.Read(iBuf,4); //number of players
			m_War3Map.mapW3Info.mapNbMaxPlayers = j = iBuf[0];
			if (j > 12)
			{
				j = 12;
			}
			else if (j < 0)
			{
				j = 0;
			}

			for (i=0; i < j; i++)
			{
				// read player data
				m_fiW3I.Read(iBuf,4); // unknown int 0
				m_War3Map.mapW3Info.mapPlayers[i].iUnknown00 = iBuf[0];
				m_fiW3I.Read(iBuf,4); // default player type
				m_War3Map.mapW3Info.mapPlayers[i].type = iBuf[0];
				m_fiW3I.Read(iBuf,4); // player race
				m_War3Map.mapW3Info.mapPlayers[i].race = iBuf[0];
				m_fiW3I.Read(iBuf,4); // fixed starts
				m_War3Map.mapW3Info.mapPlayers[i].fixedstart = iBuf[0];

				// player name
				str = "";
				m_fiW3I.Read(cBuf,1);
				while (cBuf[0])
				{
					str += cBuf[0];
					m_fiW3I.Read(cBuf,1);
				}
				if (CTrigString::IsTrigStringName(str))
				{//trigger string
					m_War3Map.mapW3Info.mapPlayers[i].trigID = CTrigString::GetTrigStringID(str);
				}
				else
				{//value
					m_War3Map.mapW3Info.mapPlayers[i].trigID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
				}

				m_fiW3I.Read(fBuf,4); // start coordinate X
				m_War3Map.mapW3Info.mapPlayers[i].startX = fBuf[0];
				m_fiW3I.Read(fBuf,4); // start coordinate Y
				m_War3Map.mapW3Info.mapPlayers[i].startY = fBuf[0];
				m_fiW3I.Read(iBuf,4);
				m_War3Map.mapW3Info.mapPlayers[i].allyLowPrio = iBuf[0];
				m_fiW3I.Read(iBuf,4);
				m_War3Map.mapW3Info.mapPlayers[i].allyHighPrio = iBuf[0];
				// end player data
			}

			m_fiW3I.Read(iBuf,4); // number of forces
			j = iBuf[0];
			m_War3Map.mapW3Info.mapNbMaxForces = j;
			if (j > 12)
			{
				j = 12;
			}
			else if (j < 0)
			{
				j = 0;
			}

			m_fiW3I.Read(iBuf,4);
			m_War3Map.mapW3Info.mapForcesFlag = iBuf[0];
			m_fiW3I.Read(iBuf,4);
			m_War3Map.mapW3Info.mapiUnknown00 = iBuf[0];

			for (i = 0; i < j; i++)
			{
				// force data
				str = "";
				m_fiW3I.Read(cBuf,1);
				while (cBuf[0])
				{
					str += cBuf[0];
					m_fiW3I.Read(cBuf,1);
				}
				if (CTrigString::IsTrigStringName(str))
				{//trigger string
					m_War3Map.mapW3Info.mapForces[i].trigID = CTrigString::GetTrigStringID(str);
				}
				else
				{//value
					m_War3Map.mapW3Info.mapForces[i].trigID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
				}
				m_fiW3I.Read(iBuf,4);
				m_War3Map.mapW3Info.mapForces[i].iUnknown00 = iBuf[0];
				m_fiW3I.Read(iBuf,4);
				m_War3Map.mapW3Info.mapForces[i].iUnknown01 = iBuf[0];
				// end force data
			}

			// read unsupported bytes
			i = (int)(m_fiW3I.GetLength() - m_fiW3I.GetPosition());
			if (i > 0)
			{
				TRY
				{
					m_War3Map.mapW3Info.m_baUnsupportedBuffer.SetSize(i);
					Buf = m_War3Map.mapW3Info.m_baUnsupportedBuffer.GetData();
					m_fiW3I.Read(Buf,i);
				}
				CATCH (CMemoryException, e)
				{

				}
				END_CATCH
			}
			// file loaded successfully, sets the flag
			m_iFilesInitStatus |= FL_W3M_W3I;
		}
		else if (0x0012 == iBuf[0])
		{// version 18
			// read number of saves
			m_fiW3I.Read(iBuf, 4);
			m_War3Map.mapW3Info.mapNbSaves = iBuf[0];
			// read editot version
			m_fiW3I.Read(iBuf, 4);
			m_War3Map.mapW3Info.mapEditVer = iBuf[0];

			// map name trigger string
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string --> get the number
				m_War3Map.mapW3Info.mapNameTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value --> create a new trigger string
				m_War3Map.mapW3Info.mapNameTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}

			// map author trigger string
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapAuthorTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapAuthorTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}

			// map description trigger string
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapDescTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapDescTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}

			// map recommended players trigger string
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapSugPlyrTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapSugPlyrTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}

			// camera bounds
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fX1 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fY1 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fX2 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fY2 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fX3 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fY3 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fX4 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fY4 = fBuf[0];

			// map bounds
			m_fiW3I.Read(iBuf,4);
			m_War3Map.mapW3Info.mapLeftBoundWidth = iBuf[0];
			m_fiW3I.Read(iBuf,4);
			m_War3Map.mapW3Info.mapRightBoundWidth = iBuf[0];
			m_fiW3I.Read(iBuf,4);
			m_War3Map.mapW3Info.mapBottomBoundHeight = iBuf[0];
			m_fiW3I.Read(iBuf,4);
			m_War3Map.mapW3Info.mapTopBoundHeight = iBuf[0];
			m_fiW3I.Read(iBuf,4); // map playable area width
			m_War3Map.mapW3Info.mapPlayableWidth = iBuf[0];
			m_fiW3I.Read(iBuf,4); // map playable area height
			m_War3Map.mapW3Info.mapPlayableHeight = iBuf[0];
			m_fiW3I.Read(iBuf,4); //map flags
			m_War3Map.mapW3Info.mapFlags = iBuf[0];
			if (m_War3Map.mapW3Info.mapFlags & W3I_FLG_PROTECTED)
			{SetProtected();}

			m_fiW3I.Read(cBuf,1); // main tiletype
			m_War3Map.mapW3Info.mapMainTileset = cBuf[0];

			// map Campaign background number
			m_fiW3I.Read(iBuf,4);
			m_War3Map.mapW3Info.mapCampBackgrnd = iBuf[0];

			// map loading screen text
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapLoadTextTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapLoadTextTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}


			// map loading screen title
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapLoadTitleTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapLoadTitleTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}


			// map loading screen subtitle
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapLoadSubtitleTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapLoadSubtitleTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}


			// map loading screen glue file path
			m_fiW3I.Read(iBuf,4); //+FIXME
			
			
			// map prologue screen text
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapProlTextTID= CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapProlTextTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}

			// map prologue screen title
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapProlTitleTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapProlTitleTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}


			// map prologue screen subtitle
			// usually ""
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapProlSubtitleTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapProlSubtitleTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}

			m_fiW3I.Read(iBuf,4); //number of players
			j = iBuf[0];
			m_War3Map.mapW3Info.mapNbMaxPlayers = j;
			if (j > 12)
			{
				j = 12;
			}
			else if (j < 0)
			{
				j = 0;
			}

			for (i=0; i < j; i++)
			{
				// read player data
				m_fiW3I.Read(iBuf,4); // unknown int 0
				m_War3Map.mapW3Info.mapPlayers[i].iUnknown00 = iBuf[0];
				m_fiW3I.Read(iBuf,4); // default player type
				m_War3Map.mapW3Info.mapPlayers[i].type = iBuf[0];
				m_fiW3I.Read(iBuf,4); // player race
				m_War3Map.mapW3Info.mapPlayers[i].race = iBuf[0];
				m_fiW3I.Read(iBuf,4); // fixed starts
				m_War3Map.mapW3Info.mapPlayers[i].fixedstart = iBuf[0];

				// player name
				str = "";
				m_fiW3I.Read(cBuf,1);
				while (cBuf[0])
				{
					str += cBuf[0];
					m_fiW3I.Read(cBuf,1);
				}
				if (CTrigString::IsTrigStringName(str))
				{//trigger string
					m_War3Map.mapW3Info.mapPlayers[i].trigID = CTrigString::GetTrigStringID(str);
				}
				else
				{//value
					m_War3Map.mapW3Info.mapPlayers[i].trigID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
				}

				m_fiW3I.Read(fBuf,4); // start coordinate X
				m_War3Map.mapW3Info.mapPlayers[i].startX = fBuf[0];
				m_fiW3I.Read(fBuf,4); // start coordinate Y
				m_War3Map.mapW3Info.mapPlayers[i].startY = fBuf[0];
				m_fiW3I.Read(iBuf,4);
				m_War3Map.mapW3Info.mapPlayers[i].allyLowPrio = iBuf[0];
				m_fiW3I.Read(iBuf,4);
				m_War3Map.mapW3Info.mapPlayers[i].allyHighPrio = iBuf[0];
				// end player data
			}

			m_fiW3I.Read(iBuf,4); // number of forces
			j = iBuf[0];
			m_War3Map.mapW3Info.mapNbMaxForces = j;
			if (j > 12)
			{
				j = 12;
			}
			else if (j < 0)
			{
				j = 0;
			}

			m_fiW3I.Read(iBuf,4);
			m_War3Map.mapW3Info.mapForcesFlag = iBuf[0];
			m_fiW3I.Read(iBuf,4);
			m_War3Map.mapW3Info.mapiUnknown00 = iBuf[0];

			for (i = 0; i < j; i++)
			{
				// force data
				str = "";
				m_fiW3I.Read(cBuf,1);
				while (cBuf[0])
				{
					str += cBuf[0];
					m_fiW3I.Read(cBuf,1);
				}
				if (CTrigString::IsTrigStringName(str))
				{//trigger string
					m_War3Map.mapW3Info.mapForces[i].trigID = CTrigString::GetTrigStringID(str);
				}
				else
				{//value
					m_War3Map.mapW3Info.mapForces[i].trigID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
				}
				m_fiW3I.Read(iBuf,4);
				m_War3Map.mapW3Info.mapForces[i].iUnknown00 = iBuf[0];
				m_fiW3I.Read(iBuf,4);
				m_War3Map.mapW3Info.mapForces[i].iUnknown01 = iBuf[0];
				// end force data
			}

			// read unsupported bytes
			i = (int)(m_fiW3I.GetLength() - m_fiW3I.GetPosition());
			if (i > 0)
			{
				TRY
				{
					m_War3Map.mapW3Info.m_baUnsupportedBuffer.SetSize(i);
					Buf = m_War3Map.mapW3Info.m_baUnsupportedBuffer.GetData();
					m_fiW3I.Read(Buf,i);
				}
				CATCH (CMemoryException, e)
				{

				}
				END_CATCH
			}
			// file loaded successfully, sets the flag
			m_iFilesInitStatus |= FL_W3M_W3I;
		}
		else if (0x0010 == iBuf[0])
		{// version 16

			// read number of saves
			m_fiW3I.Read(iBuf, 4);
			m_War3Map.mapW3Info.mapNbSaves = iBuf[0];
			// read editot version
			m_fiW3I.Read(iBuf, 4);
			m_War3Map.mapW3Info.mapEditVer = iBuf[0];

			// map name trigger string
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapNameTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapNameTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}

			// map author trigger string
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapAuthorTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapAuthorTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}

			// map description trigger string
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapDescTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapDescTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}

			// map recommended players trigger string
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapSugPlyrTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapSugPlyrTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}

			// camera bounds
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fX1 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fY1 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fX2 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fY2 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fX3 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fY3 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fX4 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fY4 = fBuf[0];

			// map bounds
			m_fiW3I.Read(iBuf,4);
			m_War3Map.mapW3Info.mapLeftBoundWidth = iBuf[0];
			m_fiW3I.Read(iBuf,4);
			m_War3Map.mapW3Info.mapRightBoundWidth = iBuf[0];
			m_fiW3I.Read(iBuf,4);
			m_War3Map.mapW3Info.mapBottomBoundHeight = iBuf[0];
			m_fiW3I.Read(iBuf,4);
			m_War3Map.mapW3Info.mapTopBoundHeight = iBuf[0];
			m_fiW3I.Read(iBuf,4); // map playable area width
			m_War3Map.mapW3Info.mapPlayableWidth = iBuf[0];
			m_fiW3I.Read(iBuf,4); // map playable area height
			m_War3Map.mapW3Info.mapPlayableHeight = iBuf[0];
			m_fiW3I.Read(iBuf,4); //map flags
			m_War3Map.mapW3Info.mapFlags = iBuf[0];
			if (m_War3Map.mapW3Info.mapFlags & W3I_FLG_PROTECTED)
			{SetProtected();}

			m_fiW3I.Read(cBuf,1); // main tiletype
			m_War3Map.mapW3Info.mapMainTileset = cBuf[0];


			// map Campaign background number
			m_fiW3I.Read(iBuf,4);
			m_War3Map.mapW3Info.mapCampBackgrnd = iBuf[0];


			// map loading screen text
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapLoadTextTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapLoadTextTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}


			// map loading screen title
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapLoadTitleTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapLoadTitleTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}


			// map loading screen subtitle
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapLoadSubtitleTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapLoadSubtitleTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}


			// map loading screen glue number
			m_fiW3I.Read(iBuf,4);
			m_War3Map.mapW3Info.mapLoadGlue = iBuf[0];


			// map prologue screen text
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapProlTextTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapProlTextTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}


			// map prologue screen title
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapProlTitleTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapProlTitleTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}


			// map prologue screen subtitle
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapProlSubtitleTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapProlSubtitleTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}


			m_fiW3I.Read(iBuf,4); //number of players
			j = iBuf[0];
			m_War3Map.mapW3Info.mapNbMaxPlayers = j;
			if (j > 12)
			{
				j = 12;
			}
			else if (j < 0)
			{
				j = 0;
			}

			for (i=0; i < j; i++)
			{
				// read player data
				m_fiW3I.Read(iBuf,4); // unknown int 0
				m_War3Map.mapW3Info.mapPlayers[i].iUnknown00 = iBuf[0];
				m_fiW3I.Read(iBuf,4); // default player type
				m_War3Map.mapW3Info.mapPlayers[i].type = iBuf[0];
				m_fiW3I.Read(iBuf,4); // player race
				m_War3Map.mapW3Info.mapPlayers[i].race = iBuf[0];
				m_fiW3I.Read(iBuf,4); // fixed starts
				m_War3Map.mapW3Info.mapPlayers[i].fixedstart = iBuf[0];

				// player name
				str = "";
				m_fiW3I.Read(cBuf,1);
				while (cBuf[0])
				{
					str += cBuf[0];
					m_fiW3I.Read(cBuf,1);
				}
				if (CTrigString::IsTrigStringName(str))
				{//trigger string
					m_War3Map.mapW3Info.mapPlayers[i].trigID = CTrigString::GetTrigStringID(str);
				}
				else
				{//value
					m_War3Map.mapW3Info.mapPlayers[i].trigID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
				}

				m_fiW3I.Read(fBuf,4); // start coordinate X
				m_War3Map.mapW3Info.mapPlayers[i].startX = fBuf[0];
				m_fiW3I.Read(fBuf,4); // start coordinate Y
				m_War3Map.mapW3Info.mapPlayers[i].startY = fBuf[0];
				m_fiW3I.Read(iBuf,4);
				m_War3Map.mapW3Info.mapPlayers[i].allyLowPrio = iBuf[0];
				m_fiW3I.Read(iBuf,4);
				m_War3Map.mapW3Info.mapPlayers[i].allyHighPrio = iBuf[0];
				// end player data
			}

			m_fiW3I.Read(iBuf,4); // number of forces
			j = iBuf[0];
			m_War3Map.mapW3Info.mapNbMaxForces = j;
			if (j > 12)
			{
				j = 12;
			}
			else if (j < 0)
			{
				j = 0;
			}

			m_fiW3I.Read(iBuf,4);
			m_War3Map.mapW3Info.mapForcesFlag = iBuf[0];
			m_fiW3I.Read(iBuf,4);
			m_War3Map.mapW3Info.mapiUnknown00 = iBuf[0];

			for (i = 0; i < j; i++)
			{
				// force data
				str = "";
				m_fiW3I.Read(cBuf,1);
				while (cBuf[0])
				{
					str += cBuf[0];
					m_fiW3I.Read(cBuf,1);
				}
				if (CTrigString::IsTrigStringName(str))
				{//trigger string
					m_War3Map.mapW3Info.mapForces[i].trigID = CTrigString::GetTrigStringID(str);
				}
				else
				{//value
					m_War3Map.mapW3Info.mapForces[i].trigID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
				}
				m_fiW3I.Read(iBuf,4);
				m_War3Map.mapW3Info.mapForces[i].iUnknown00 = iBuf[0];
				m_fiW3I.Read(iBuf,4);
				m_War3Map.mapW3Info.mapForces[i].iUnknown01 = iBuf[0];
				// end force data
			}

//			m_fiW3I.Read(iBuf,4);
//			m_War3Map.mapW3Info.mapiUnknown01 = iBuf[0];
			// read unsupported bytes
			i = (int)(m_fiW3I.GetLength() - m_fiW3I.GetPosition());
			if (i > 0)
			{
				TRY
				{
					m_War3Map.mapW3Info.m_baUnsupportedBuffer.SetSize(i);
					Buf = m_War3Map.mapW3Info.m_baUnsupportedBuffer.GetData();
					m_fiW3I.Read(Buf,i);
				}
				CATCH (CMemoryException, e)
				{

				}
				END_CATCH
			}
			// file loaded successfully, sets the flag
			m_iFilesInitStatus |= FL_W3M_W3I;
		}
		else if (0x000F == iBuf[0])
		{// version 15
			// map name trigger string
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapNameTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapNameTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}

			// map author trigger string
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapAuthorTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapAuthorTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}

			// map description trigger string
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapDescTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapDescTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}

			// map recommended players trigger string
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapSugPlyrTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapSugPlyrTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}

			// camera bounds
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fX1 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fY1 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fX2 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fY2 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fX3 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fY3 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fX4 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fY4 = fBuf[0];

			// map bounds
			m_fiW3I.Read(iBuf,4);
			m_War3Map.mapW3Info.mapLeftBoundWidth = iBuf[0];
			m_fiW3I.Read(iBuf,4);
			m_War3Map.mapW3Info.mapRightBoundWidth = iBuf[0];
			m_fiW3I.Read(iBuf,4);
			m_War3Map.mapW3Info.mapBottomBoundHeight = iBuf[0];
			m_fiW3I.Read(iBuf,4);
			m_War3Map.mapW3Info.mapTopBoundHeight = iBuf[0];
			m_fiW3I.Read(iBuf,4); // map playable area width
			m_War3Map.mapW3Info.mapPlayableWidth = iBuf[0];
			m_fiW3I.Read(iBuf,4); // map playable area height
			m_War3Map.mapW3Info.mapPlayableHeight = iBuf[0];
			m_fiW3I.Read(iBuf,4); //map flags
			m_War3Map.mapW3Info.mapFlags = iBuf[0];
			if (m_War3Map.mapW3Info.mapFlags & W3I_FLG_PROTECTED)
			{SetProtected();}

			m_fiW3I.Read(cBuf,1); // main tiletype
			m_War3Map.mapW3Info.mapMainTileset = cBuf[0];

			// map Campaign background number
			m_fiW3I.Read(iBuf,4);
			m_War3Map.mapW3Info.mapCampBackgrnd = iBuf[0];


			// map loading screen text
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapLoadTextTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapLoadTextTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}


			// map loading screen title
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapLoadTitleTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapLoadTitleTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}


			// map loading screen subtitle
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapLoadSubtitleTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapLoadSubtitleTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}


			// map loading screen glue number
			m_fiW3I.Read(iBuf,4);
			m_War3Map.mapW3Info.mapLoadGlue = iBuf[0];


			// map prologue screen text
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapProlTextTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapProlTextTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}


			// map prologue screen title
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapProlTitleTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapProlTitleTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}


			// map prologue screen subtitle
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapProlSubtitleTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapProlSubtitleTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}


			m_fiW3I.Read(iBuf,4); //number of players
			j = iBuf[0];
			m_War3Map.mapW3Info.mapNbMaxPlayers = j;
			if (j > 12)
			{
				j = 12;
			}
			else if (j < 0)
			{
				j = 0;
			}

			for (i=0; i < j; i++)
			{
				// read player data
				m_fiW3I.Read(iBuf,4); // unknown int 0
				m_War3Map.mapW3Info.mapPlayers[i].iUnknown00 = iBuf[0];
				m_fiW3I.Read(iBuf,4); // default player type
				m_War3Map.mapW3Info.mapPlayers[i].type = iBuf[0];
				m_fiW3I.Read(iBuf,4); // player race
				m_War3Map.mapW3Info.mapPlayers[i].race = iBuf[0];
				m_fiW3I.Read(iBuf,4); // fixed starts
				m_War3Map.mapW3Info.mapPlayers[i].fixedstart = iBuf[0];

				// player name
				str = "";
				m_fiW3I.Read(cBuf,1);
				while (cBuf[0])
				{
					str += cBuf[0];
					m_fiW3I.Read(cBuf,1);
				}
				if (CTrigString::IsTrigStringName(str))
				{//trigger string
					m_War3Map.mapW3Info.mapPlayers[i].trigID = CTrigString::GetTrigStringID(str);
				}
				else
				{//value
					m_War3Map.mapW3Info.mapPlayers[i].trigID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
				}

				m_fiW3I.Read(fBuf,4); // start coordinate X
				m_War3Map.mapW3Info.mapPlayers[i].startX = fBuf[0];
				m_fiW3I.Read(fBuf,4); // start coordinate Y
				m_War3Map.mapW3Info.mapPlayers[i].startY = fBuf[0];
				m_fiW3I.Read(iBuf,4);
				m_War3Map.mapW3Info.mapPlayers[i].allyLowPrio = iBuf[0];
				m_fiW3I.Read(iBuf,4);
				m_War3Map.mapW3Info.mapPlayers[i].allyHighPrio = iBuf[0];
				// end player data
			}

			m_fiW3I.Read(iBuf,4); // number of forces
			j = iBuf[0];
			m_War3Map.mapW3Info.mapNbMaxForces = j;
			if (j > 12)
			{
				j = 12;
			}
			else if (j < 0)
			{
				j = 0;
			}

			m_fiW3I.Read(iBuf,4);
			m_War3Map.mapW3Info.mapForcesFlag = iBuf[0];
			m_fiW3I.Read(iBuf,4);
			m_War3Map.mapW3Info.mapiUnknown00 = iBuf[0];

			for (i = 0; i < j; i++)
			{
				// force data
				str = "";
				m_fiW3I.Read(cBuf,1);
				while (cBuf[0])
				{
					str += cBuf[0];
					m_fiW3I.Read(cBuf,1);
				}
				if (CTrigString::IsTrigStringName(str))
				{//trigger string
					m_War3Map.mapW3Info.mapForces[i].trigID = CTrigString::GetTrigStringID(str);
				}
				else
				{//value
					m_War3Map.mapW3Info.mapForces[i].trigID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
				}
				m_fiW3I.Read(iBuf,4);
				m_War3Map.mapW3Info.mapForces[i].iUnknown00 = iBuf[0];
				m_fiW3I.Read(iBuf,4);
				m_War3Map.mapW3Info.mapForces[i].iUnknown01 = iBuf[0];
				// end force data
			}

//			m_fiW3I.Read(iBuf,4);
//			m_War3Map.mapW3Info.mapiUnknown01 = iBuf[0];
			// read unsupported bytes
			i = (int)(m_fiW3I.GetLength() - m_fiW3I.GetPosition());
			if (i > 0)
			{
				TRY
				{
					m_War3Map.mapW3Info.m_baUnsupportedBuffer.SetSize(i);
					Buf = m_War3Map.mapW3Info.m_baUnsupportedBuffer.GetData();
					m_fiW3I.Read(Buf,i);
				}
				CATCH (CMemoryException, e)
				{

				}
				END_CATCH
			}
			// file loaded successfully, sets the flag
			m_iFilesInitStatus |= FL_W3M_W3I;
		}
		else if (0x000B == iBuf[0])
		{//version 11
			// map name trigger string
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapNameTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapNameTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}


			// map author trigger string
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapAuthorTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapAuthorTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}

			// map description trigger string
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapDescTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapDescTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}

			// map recommended players trigger string
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapSugPlyrTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapSugPlyrTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}

			// camera bounds
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fX1 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fY1 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fX2 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fY2 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fX3 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fY3 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fX4 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fY4 = fBuf[0];

			m_fiW3I.Read(iBuf,4); // map playable area width
			m_War3Map.mapW3Info.mapPlayableWidth = iBuf[0];
			m_fiW3I.Read(iBuf,4); // map playable area height
			m_War3Map.mapW3Info.mapPlayableHeight = iBuf[0];
			m_fiW3I.Read(iBuf,4); //map flags
			m_War3Map.mapW3Info.mapFlags = iBuf[0];
			if (m_War3Map.mapW3Info.mapFlags & W3I_FLG_PROTECTED)
			{SetProtected();}

			m_fiW3I.Read(cBuf,1); // main tiletype
			m_War3Map.mapW3Info.mapMainTileset = cBuf[0];

			// map Campaign background number
			m_fiW3I.Read(iBuf,4);
			m_War3Map.mapW3Info.mapCampBackgrnd = iBuf[0];


			// map loading screen text
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapLoadTextTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapLoadTextTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}


			// map loading screen title
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapLoadTitleTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapLoadTitleTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}


			// map loading screen subtitle
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapLoadSubtitleTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapLoadSubtitleTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}


			m_fiW3I.Read(iBuf,4); //number of players
			j = iBuf[0];
			m_War3Map.mapW3Info.mapNbMaxPlayers = j;
			if (j > 12)
			{
				j = 12;
			}
			else if (j < 0)
			{
				j = 0;
			}

			for (i=0; i < j; i++)
			{
				// read player data
				m_fiW3I.Read(iBuf,4); // unknown int 0
				m_War3Map.mapW3Info.mapPlayers[i].iUnknown00 = iBuf[0];
				m_fiW3I.Read(iBuf,4); // default player type
				m_War3Map.mapW3Info.mapPlayers[i].type = iBuf[0];
				m_fiW3I.Read(iBuf,4); // player race
				m_War3Map.mapW3Info.mapPlayers[i].race = iBuf[0];
				m_fiW3I.Read(iBuf,4); // fixed starts
				m_War3Map.mapW3Info.mapPlayers[i].fixedstart = iBuf[0];

				// player name
				str = "";
				m_fiW3I.Read(cBuf,1);
				while (cBuf[0])
				{
					str += cBuf[0];
					m_fiW3I.Read(cBuf,1);
				}
				if (CTrigString::IsTrigStringName(str))
				{//trigger string
					m_War3Map.mapW3Info.mapPlayers[i].trigID = CTrigString::GetTrigStringID(str);
				}
				else
				{//value
					m_War3Map.mapW3Info.mapPlayers[i].trigID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
				}

				m_fiW3I.Read(fBuf,4); // start coordinate X
				m_War3Map.mapW3Info.mapPlayers[i].startX = fBuf[0];
				m_fiW3I.Read(fBuf,4); // start coordinate Y
				m_War3Map.mapW3Info.mapPlayers[i].startY = fBuf[0];
				m_fiW3I.Read(iBuf,4);
				m_War3Map.mapW3Info.mapPlayers[i].allyLowPrio = iBuf[0];
				m_fiW3I.Read(iBuf,4);
				m_War3Map.mapW3Info.mapPlayers[i].allyHighPrio = iBuf[0];
				// end player data
			}

			m_fiW3I.Read(iBuf,4); // number of forces
			j = iBuf[0];
			m_War3Map.mapW3Info.mapNbMaxForces = j;
			if (j > 12)
			{
				j = 12;
			}
			else if (j < 0)
			{
				j = 0;
			}

			m_fiW3I.Read(iBuf,4);
			m_War3Map.mapW3Info.mapForcesFlag = iBuf[0];
			m_fiW3I.Read(iBuf,4);
			m_War3Map.mapW3Info.mapiUnknown00 = iBuf[0];

			for (i = 0; i < j; i++)
			{
				// force data
				str = "";
				m_fiW3I.Read(cBuf,1);
				while (cBuf[0])
				{
					str += cBuf[0];
					m_fiW3I.Read(cBuf,1);
				}
				if (CTrigString::IsTrigStringName(str))
				{//trigger string
					m_War3Map.mapW3Info.mapForces[i].trigID = CTrigString::GetTrigStringID(str);
				}
				else
				{//value
					m_War3Map.mapW3Info.mapForces[i].trigID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
				}
				m_fiW3I.Read(iBuf,4);
				m_War3Map.mapW3Info.mapForces[i].iUnknown00 = iBuf[0];
				m_fiW3I.Read(iBuf,4);
				m_War3Map.mapW3Info.mapForces[i].iUnknown01 = iBuf[0];
				// end force data
			}
			// read unsupported bytes
			i = (int)(m_fiW3I.GetLength() - m_fiW3I.GetPosition());
			if (i > 0)
			{
				TRY
				{
					m_War3Map.mapW3Info.m_baUnsupportedBuffer.SetSize(i);
					Buf = m_War3Map.mapW3Info.m_baUnsupportedBuffer.GetData();
					m_fiW3I.Read(Buf,i);
				}
				CATCH (CMemoryException, e)
				{

				}
				END_CATCH
			}
			// file loaded successfully, sets the flag
			m_iFilesInitStatus |= FL_W3M_W3I;
		}
		else if (0x0008 == iBuf[0])
		{// version 8
			// map name trigger string
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapNameTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapNameTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}


			// map author trigger string
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapAuthorTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapAuthorTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}

			// map description trigger string
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapDescTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapDescTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}

			// map recommended players trigger string
			str = "";
			m_fiW3I.Read(cBuf,1);
			while (cBuf[0])
			{
				str += cBuf[0];
				m_fiW3I.Read(cBuf,1);
			}
			if (CTrigString::IsTrigStringName(str))
			{//trigger string
				m_War3Map.mapW3Info.mapSugPlyrTID = CTrigString::GetTrigStringID(str);
			}
			else
			{//value
				m_War3Map.mapW3Info.mapSugPlyrTID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
			}
			
			m_fiW3I.Read(fBuf,4); // unknown
			m_fiW3I.Read(iBuf,4); // unknown
			m_fiW3I.Read(fBuf,4); // unknown
			m_fiW3I.Read(fBuf,4); // unknown
			m_fiW3I.Read(fBuf,4); // unknown
			m_fiW3I.Read(iBuf,4); // unknown

			// camera bounds
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fX1 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fY1 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fX2 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fY2 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fX3 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fY3 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fX4 = fBuf[0];
			m_fiW3I.Read(fBuf,4);
			m_War3Map.mapW3Info.mapCameraBounds.fY4 = fBuf[0];

			m_fiW3I.Read(iBuf,4); // map playable area width
			m_War3Map.mapW3Info.mapPlayableWidth = iBuf[0];
			m_fiW3I.Read(iBuf,4); // map playable area height
			m_War3Map.mapW3Info.mapPlayableHeight = iBuf[0];

			m_fiW3I.Read(iBuf,4); // unknown
			m_fiW3I.Read(iBuf,4); // unknown

			m_fiW3I.Read(cBuf,1); // main tiletype
			m_War3Map.mapW3Info.mapMainTileset = cBuf[0];

			m_fiW3I.Read(iBuf,4); //number of players
			j = iBuf[0];
			m_War3Map.mapW3Info.mapNbMaxPlayers = j;
			if (j > 12)
			{
				j = 12;
			}
			else if (j < 0)
			{
				j = 0;
			}

			for (i=0; i < j; i++)
			{
				// read player data
				m_fiW3I.Read(iBuf,4); // unknown int 0
				m_War3Map.mapW3Info.mapPlayers[i].iUnknown00 = iBuf[0];
				m_fiW3I.Read(iBuf,4); // default player type
				m_War3Map.mapW3Info.mapPlayers[i].type = iBuf[0];
				m_fiW3I.Read(iBuf,4); // player race
				m_War3Map.mapW3Info.mapPlayers[i].race = iBuf[0];
				m_fiW3I.Read(iBuf,4); // fixed starts
				m_War3Map.mapW3Info.mapPlayers[i].fixedstart = iBuf[0];

				// player name
				str = "";
				m_fiW3I.Read(cBuf,1);
				while (cBuf[0])
				{
					str += cBuf[0];
					m_fiW3I.Read(cBuf,1);
				}
				if (CTrigString::IsTrigStringName(str))
				{//trigger string
					m_War3Map.mapW3Info.mapPlayers[i].trigID = CTrigString::GetTrigStringID(str);
				}
				else
				{//value
					m_War3Map.mapW3Info.mapPlayers[i].trigID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
				}

				m_fiW3I.Read(fBuf,4); // start coordinate X
				m_War3Map.mapW3Info.mapPlayers[i].startX = fBuf[0];
				m_fiW3I.Read(fBuf,4); // start coordinate Y
				m_War3Map.mapW3Info.mapPlayers[i].startY = fBuf[0];
				m_fiW3I.Read(iBuf,4);
				m_War3Map.mapW3Info.mapPlayers[i].allyLowPrio = iBuf[0];
				m_fiW3I.Read(iBuf,4);
				m_War3Map.mapW3Info.mapPlayers[i].allyHighPrio = iBuf[0];
				// end player data
			}

			m_fiW3I.Read(iBuf,4); // number of forces
			j = iBuf[0];
			m_War3Map.mapW3Info.mapNbMaxForces = j;
			if (j > 12)
			{
				j = 12;
			}
			else if (j < 0)
			{
				j = 0;
			}

			m_fiW3I.Read(iBuf,4);
			m_War3Map.mapW3Info.mapForcesFlag = iBuf[0];
			m_fiW3I.Read(iBuf,4);
			m_War3Map.mapW3Info.mapiUnknown00 = iBuf[0];

			for (i = 0; i < j; i++)
			{
				// force data
				str = "";
				m_fiW3I.Read(cBuf,1);
				while (cBuf[0])
				{
					str += cBuf[0];
					m_fiW3I.Read(cBuf,1);
				}
				if (CTrigString::IsTrigStringName(str))
				{//trigger string
					m_War3Map.mapW3Info.mapForces[i].trigID = CTrigString::GetTrigStringID(str);
				}
				else
				{//value
					m_War3Map.mapW3Info.mapForces[i].trigID = m_War3Map.mapTrigStringsTable.AddTrigString(str);
				}
				m_fiW3I.Read(iBuf,4);
				m_War3Map.mapW3Info.mapForces[i].iUnknown00 = iBuf[0];
				m_fiW3I.Read(iBuf,4);
				m_War3Map.mapW3Info.mapForces[i].iUnknown01 = iBuf[0];
				// end force data
			}
			if (i > 0)
			{
				TRY
				{
					m_War3Map.mapW3Info.m_baUnsupportedBuffer.SetSize(i);
					Buf = m_War3Map.mapW3Info.m_baUnsupportedBuffer.GetData();
					m_fiW3I.Read(Buf,i);
				}
				CATCH (CMemoryException, e)
				{

				}
				END_CATCH
			}
			// file loaded successfully, sets the flag
			m_iFilesInitStatus |= FL_W3M_W3I;
		}
		else
		{ // unknown version
				CString msg;
				msg.Format(STR_ERROR_W3I00, iBuf[0]);
				W3Z_ERROR(msg);
		}

		m_fiW3I.SeekToBegin();	
	}
	

}

void CW3ZMapEditDoc::SerializeWTG(BOOL IsStoring)
{
	if (IsStoring)
	{
//		m_fiWTG.SetLength(0); // vide le fichier
		m_fiWTG.SeekToBegin( ); // et se replace au debut
//code goes here
		m_fiWTG.SeekToBegin();
	}
	else
	{
		m_fiWTG.SeekToBegin();
// code goes here
		m_fiWTG.SeekToBegin();	
	}
}

void CW3ZMapEditDoc::SerializeWCT(BOOL IsStoring)
{
	if (IsStoring)
	{
//		m_fiWCT.SetLength(0); // vide le fichier
		m_fiWCT.SeekToBegin( ); // et se replace au debut
// code goes here
		m_fiWCT.SeekToBegin();
	}
	else
	{
		m_fiWCT.SeekToBegin();
// code goes here
		m_fiWCT.SeekToBegin();	
	}
}

void CW3ZMapEditDoc::SerializeWTS(BOOL IsStoring)
{

	int i, j, trigID, size;
	CString str1, str2;
	CTrigString ts;
	char* cBuf = NULL;

	if (IsStoring)
	{
		m_fiWTS.SetLength(0); // vide le fichier
		m_fiWTS.SeekToBegin( ); // et se replace au debut

		//ecrit les 3 chars bizards au debut du fichier
		str1 = STR_WTS_HEADER;
		m_fiWTS.Write(str1, str1.GetLength());

		m_War3Map.mapTrigStringsTable.GoFirstTrigString();
		ts = m_War3Map.mapTrigStringsTable.GetNextTrigString();
		while (ts.GetTrigStringID() >= 0)
		{
			str1.Format(TSTR_TRIGSTR_DEF, ts.GetTrigStringID(), ts.GetTrigStringValue());
			m_fiWTS.Write(str1.GetBuffer(str1.GetLength()), str1.GetLength());
			str1.ReleaseBuffer();
			ts = m_War3Map.mapTrigStringsTable.GetNextTrigString();		
		}
		m_fiWTS.SeekToBegin();
	}
	else
	{
		m_fiWTS.SeekToBegin();
		size = (int)m_fiWTS.GetLength();
		cBuf = new char[size + 1];
		if (cBuf)
		{
			m_fiWTS.Read(cBuf, size);
			cBuf[size] = '\0';
			str1 = cBuf;
			str1.MakeUpper(); // facilite la recherche
			i = 0;
			while(i < size)
			{
				// cherche la prochaine chaine "string"
				j = str1.Find(TSTR_TRIGSTR_KEY, i);
				if (j >= 0)
				{// "STRING" trouve (j >=0 )
					j += 6;
					// vire les espaces et autres sauts de lignes
					while((j < size) && ((' ' == str1[j]) || (0x000D == str1[j]) || (0x000A == str1[j]) || (0x0009 == str1[j])))
					{j++;}
					str2 = "";
					while ((j < size) && (' ' != str1[j]) && (0x000D != str1[j]) && (0x000A != str1[j]) && (0x0009 != str1[j]))
					{
						str2 += str1[j];
						j++;
					}
					// ici str2 contient le numero du trigger
					trigID = atoi(str2);
					// elimine les espaces et sauts de lignes suivant le numero de trigger
					while((j < size) && ((' ' == str1[j]) || (0x000D == str1[j]) || (0x000A == str1[j]) || (0x0009 == str1[j])))
					{j++;}
					// lit ce qui doit etre '{'					
					str2 = str1[j++];
					//while ((j < size) && (' ' != str1[j]) && (0x000D != str1[j]) && (0x000A != str1[j]) && (0x0009 != str1[j]))
					//{
					//	str2 += str1[j];
					//	j++;
					//}
					// ici str2 doit etre egal a '{'
					if ("{" == str2)
					{
						// chercher le prochain '}' en sautant les espaces inutils
						while((j < size) && ((' ' == str1[j]) || (0x000D == str1[j]) || (0x000A == str1[j]) || (0x0009 == str1[j])) && ('}' != str1[j]))
						{j++;}
						str2 = "";
						// lit le string jusqu'au '}'
						while ((j < size) && ('}' != str1[j]))
						{
							str2 += cBuf[j]; // utilise cBuf pour avoir la casse d'origine
							j++;
						}
						j++;
						// ici j pointe apres le '}' et str2 contient la valeur du trigger
						//supprime les espaces, sauts de lignes et autres tabulations
						str2.TrimRight();
						m_War3Map.mapTrigStringsTable.AddTrigString(str2, trigID);
					}
				}

				if (j > 0)
				{
					if (i == j)
					{
						i++;
					}
					else
					{
						i = j;
					}
				} // on passe au suivant
				else
				{i = size;} // fin de recherche
			}
			delete []cBuf;
		}
		// file loaded successfully, sets the flag
		m_iFilesInitStatus |= FL_W3M_WTS;
		m_fiWTS.SeekToBegin();	
	}
	

}

void CW3ZMapEditDoc::SerializeJASS(BOOL IsStoring)
{

	int size, i, j;
	char* pBuff;
	CString str, str2;
	CUnit unit;

	if (IsStoring)
	{
//		m_fiJASS.SetLength(0); // vide le fichier
//		m_fiJASS.SeekToBegin( ); // et se replace au debut

//		m_fiJASS.SeekToBegin();
	}
	else
	{
		bool badcoords;
		m_fiJASS.SeekToBegin();
		size = (int)m_fiJASS.GetLength();
		pBuff = new char[size];
		m_fiJASS.Read(pBuff, size);
		i = 0;
		while(i < size)
		{
			str = "";
			while ((i < size) && (pBuff[i] != '\n'))
			{
				str += pBuff[i];
				i++;
			}
			
			badcoords = false;
			//ex.: CreateUnit( p, 'nadr', -3000, -3150, 2.000 )
			j = str.Find(JASS_STR_CREATEUNIT);
			if (0 < j)
			{// CreateUnit command found
				unit.x = 0;
				unit.y = 0;
				// loop for the type
				while ((j < str.GetLength()) && (str[j] != '\''))
				{
					j++;
				}
				j++;
				//copy the type
				unit.type = "";
				while ((j < str.GetLength()) && (str[j] != '\''))
				{
					unit.type += str[j];
					j++;
				}
				j++;
				//look for the next parameter
				while ((j < str.GetLength()) && ((str[j] == ' ') || (str[j] == '\t')))
				{
					j++;
				}
				if ((j < str.GetLength()) && (str[j] == ','))
				{ // next parameter = x
					j++;
					str2 = "";
					//remove spaces
					while ((j < str.GetLength()) && ((str[j] == ' ') || (str[j] == '\t')))
					{
						j++;
					}
					// copy the parameter into str2
					while ((j < str.GetLength()) && (str[j] != ' ') && (str[j] != '\t') && (str[j] != ','))
					{
						str2 += str[j];
						if ((str[j] != '.')
							&& (str[j] != '-')
							&& !(('0' <= str[j]) && (str[j] <= '9')))
						{badcoords = true;}
						j++;
					}
					unit.x = (float)atof(str2);
					// get next parameter = y
					while ((j < str.GetLength()) && (str[j] != ','))
					{
						j++;
					}
					if ((j < str.GetLength()) && (str[j] == ','))
					{
						j++;
						str2 = "";
						//remove spaces
						while ((j < str.GetLength()) && ((str[j] == ' ') || (str[j] == '\t')))
						{
							j++;
						}
						// copy the parameter into str2
						while ((j < str.GetLength()) && (str[j] != ' ') && (str[j] != '\t') && (str[j] != ','))
						{
							str2 += str[j];
							if ((str[j] != '.')
								&& (str[j] != '-')
								&& !(('0' <= str[j]) && (str[j] <= '9')))
							{badcoords = true;}
							j++;
						}
						unit.y = (float)atof(str2);
					}
				}
			}
			else
			{
				//ex.: DefineStartLocation( cur_start_loc, 2000, -6500 )
				j = str.Find(JASS_STR_DEFSTARTLOC);
				if (0 < j)
				{
					unit.type = UNIT_STR_ID_STARTPOS;
					while ((j < str.GetLength()) && (str[j] != ','))
					{
						j++;
					}
					j++;
					//remove spaces
					while ((j < str.GetLength()) && ((str[j] == ' ') || (str[j] == '\t')))
					{
						j++;
					}
					// copy the parameter into str2
					str2 = "";
					while ((j < str.GetLength()) && (str[j] != ' ') && (str[j] != '\t') && (str[j] != ','))
					{
						str2 += str[j];
						if ((str[j] != '.')
							&& (str[j] != '-')
							&& !(('0' <= str[j]) && (str[j] <= '9')))
						{badcoords = true;}
						j++;
					}
					unit.x = (float)atof(str2);
					while ((j < str.GetLength()) && (str[j] != ','))
					{
						j++;
					}
					j++;
					//remove spaces
					while ((j < str.GetLength()) && ((str[j] == ' ') || (str[j] == '\t')))
					{
						j++;
					}
					// copy the parameter into str2
					str2 = "";
					while ((j < str.GetLength()) && (str[j] != ' ') && (str[j] != '\t') && (str[j] != ','))
					{
						str2 += str[j];
						if ((str[j] != '.')
							&& (str[j] != '-')
							&& !(('0' <= str[j]) && (str[j] <= '9')))
						{badcoords = true;}
						j++;
					}
					unit.y = (float)atof(str2);
				}
			}

			if ((unit.type != "") && !badcoords)
			{
				m_War3Map.AddUnit(unit);				
			}
			i++;
		}

		delete []pBuff;
		m_fiJASS.SeekToBegin();	
	}
	

}

/*
SerializeSHD
************
Serialize shadow data into the shadow file (m_fiSHD)

In:
  BOOL IsStoring: TRUE=storing, FALSE=loading

Out:
  nothing

Return:
  nothing

Protection Notes:
  A protection data structure is stored using the parts of the map with shadow set.
  Since a non 0 shadow byte is used to set shadow, whe can use any of the non-zero
  bytes to store protection data but we must insure we don't set one of them to 0!
  To do so, I just randomize a half byte and make sure the random value is not 0,
  then I mix it with a half byte of the protection structure xor-ed with that half.

*/
void CW3ZMapEditDoc::SerializeSHD(BOOL IsStoring)
{

	int i, j, size;
	byte * pBytemap;
	// protected data index for the data hidden in the shadows
	int iProtect = 0; // protect data index
	int iCheckSum = 0;
	W3ZPROTECTDATA01 tProtect; // structure used to read protect data
	byte* pbyProtectData = (byte*)(&tProtect);
	BOOL bProtectDataLoaded = FALSE; // set to TRUE when a valid data block has been loaded
	byte byNewShad;
	CString strAuthorName;

	if (IsStoring)
	{
		if (TRUE == IsReadOnly())
		{// can't be saved because is read-only
			//+FIXME: set the flags that say the file has not been saved
			m_fiSHD.SeekToBegin();
		}//if (TRUE == IsReadOnly())
		else
		{//not protected
			pBytemap = m_War3Map.GetShadowMapPtr();
			i = m_War3Map.GetShadowMapWidth();
			j = m_War3Map.GetShadowMapHeight();
			size = i*j;

			if (((W3Z_MAP_PROT_W3ZV1L3 == GetProtection())
					||(W3Z_MAP_PROT_W3ZV1L1 == GetProtection())
					||(W3Z_MAP_PROT_W3ZV1L2 == GetProtection()))
				&& (TRUE == m_bEnableProtect))
			{// to be protected
				if (NULL != pBytemap)
				{// shadow map exists
					//setup protection data
					// init random values
					srand((unsigned)time(NULL));
					// initialises protect data structures
					InitProtectDataStructs();
					//sets the protect data structure pointer
					pbyProtectData = (byte *)(&m_tW3ZProtectData01);

					iProtect = 0;
					while(i < size)
					{
						if (0xFF == pBytemap[i])
						{
							// gets the half byte needed
							if (0 == (iProtect%2))
							{
								byNewShad = (byte)(pbyProtectData[iProtect/2] & 0x0F);
							}
							else
							{
								byNewShad = (byte)((pbyProtectData[iProtect/2] & 0xF0) >> 4);
							}
							// adds the random part > 0
							byNewShad |= (rand()%15 + 1) * 0x10;
							// xor the random part on the protected data
							byNewShad ^= (byNewShad / 0x10);
							// put the new value in the shadow map
							pBytemap[i] = byNewShad;
							// update the index
							iProtect++;
							if ((iProtect/2) >= W3ZPROTECTDATA01_LENGTH)
							{
								iProtect = 0;
							}
						}
						i++;
					}
				}
			}
			if (NULL != pBytemap)
			{// shadow map exists
				m_fiSHD.SetLength(0); // vide le fichier
				m_fiSHD.SeekToBegin( ); // et se replace au debut
				m_fiSHD.Write(pBytemap, size);
			}
			m_fiSHD.SeekToBegin();
		}
	}
	else
	{
		m_fiSHD.SeekToBegin();
		i = m_War3Map.GetShadowMapWidth();
		j = m_War3Map.GetShadowMapHeight();
		size = i*j;
		m_War3Map.SetShadowMapSize(i,j);
		pBytemap = m_War3Map.GetShadowMapPtr();
		if ((m_fiSHD.GetLength() <= (unsigned) size) && (pBytemap))
		{ // ne lit que si le fichier peut rentrer dans le buffer
			m_fiSHD.Read(pBytemap, size);

			pbyProtectData = (byte *)(&m_tW3ZProtectData01);
			iProtect = 0;
			while(i < size)
			{
				if (0x00 != pBytemap[i])
				{
					// gets the byte
					byNewShad = pBytemap[i];

					// xor the random part on the protected data
					byNewShad ^= (byNewShad / 0x10);

					// gets the half byte needed
					if (0 == (iProtect%2))
					{
						pbyProtectData[iProtect/2] = (byte)(byNewShad & 0x0F);
					}
					else
					{
						pbyProtectData[iProtect/2] |= (byte)((byNewShad & 0x0F) << 4);
					}

					// update the index
					iProtect++;
					if ((iProtect/2) >= W3ZPROTECTDATA01_LENGTH)
					{
						iProtect = 0;
						// check the checksum
						iCheckSum = 0;
						for (j = 0;j < W3ZPROTECTDATA01_LENGTH - 4; j++)
						{
							iCheckSum = (iCheckSum << 1) + pbyProtectData[j];
						}
						if ((iCheckSum == m_tW3ZProtectData01.iChecksum) && (FALSE == bProtectDataLoaded))
						{
							bProtectDataLoaded = TRUE;
							if ((m_tW3ZProtectData01.iProtectID / 0x0100) == (W3Z_W3E_PROT_STARTSEQ101 / 0x0100))
							{
								if (W3Z_W3E_PROT_STARTSEQ103 == m_tW3ZProtectData01.iProtectID)
								{
									SetProtection(W3Z_MAP_PROT_W3ZV1L3);
								}
								else if (W3Z_W3E_PROT_STARTSEQ102 == m_tW3ZProtectData01.iProtectID)
								{
									SetProtection(W3Z_MAP_PROT_W3ZV1L2);
								}
								else if (W3Z_W3E_PROT_STARTSEQ101 == m_tW3ZProtectData01.iProtectID)
								{
									SetProtection(W3Z_MAP_PROT_W3ZV1L1);
								}
								else
								{
									SetProtection(W3Z_MAP_PROT_UNKNOWN);
								}
								//gets author name
								strncpy(strAuthorName.GetBufferSetLength(W3Z_MAX_NAME_LENGTH), m_tW3ZProtectData01.acAuthName, W3Z_MAX_NAME_LENGTH);
								strAuthorName.ReleaseBuffer();
								DecryptString(strAuthorName);
								AddAuthorName(strAuthorName);
								m_tAuthorHashCountList.AddHash(m_tW3ZProtectData01.uHashAPass);
								m_tPublicHashCountList.AddHash(m_tW3ZProtectData01.uHashPPass);
								m_iFilesProtectStatus |= FL_W3M_SHD;
							}
						}
					}
				}
				i++;
			}
		}

		// file loaded successfully, sets the flag
		m_iFilesInitStatus |= FL_W3M_SHD;

		m_fiSHD.SeekToBegin();
	}

}

void CW3ZMapEditDoc::SerializePreviewTGA(BOOL IsStoring)
{
	if (IsStoring)
	{
//		m_fiPreviewTGA.SetLength(0); // vide le fichier
		m_fiPreviewTGA.SeekToBegin( ); // et se replace au debut
// code goes here
		m_fiPreviewTGA.SeekToBegin();
	}
	else
	{
		m_fiPreviewTGA.SeekToBegin();
// code goes here
		m_fiPreviewTGA.SeekToBegin();	
	}
}

void CW3ZMapEditDoc::SerializeMapBLP(BOOL IsStoring)
{
	if (IsStoring)
	{
//		m_fiMapBLP.SetLength(0); // vide le fichier
		m_fiMapBLP.SeekToBegin( ); // et se replace au debut
// code goes here
		m_fiMapBLP.SeekToBegin();
	}
	else
	{
		m_fiMapBLP.SeekToBegin();
// code goes here
		m_fiMapBLP.SeekToBegin();	
	}
}

void CW3ZMapEditDoc::SerializeMapB00(BOOL IsStoring)
{
	if (IsStoring)
	{
//		m_fiMapB00.SetLength(0); // vide le fichier
		m_fiMapB00.SeekToBegin( ); // et se replace au debut
// code goes here
		m_fiMapB00.SeekToBegin();
	}
	else
	{
		m_fiMapB00.SeekToBegin();
// code goes here
		m_fiMapB00.SeekToBegin();	
	}
}

void CW3ZMapEditDoc::SerializeMapTGA(BOOL IsStoring)
{
	if (IsStoring)
	{
//		m_fiMapTGA.SetLength(0); // vide le fichier
		m_fiMapTGA.SeekToBegin( ); // et se replace au debut
// code goes here
		m_fiMapTGA.SeekToBegin();
	}
	else
	{
		m_fiMapTGA.SeekToBegin();
// code goes here
		m_fiMapTGA.SeekToBegin();	
	}
}

void CW3ZMapEditDoc::SerializeMMP(BOOL IsStoring)
{
	if (IsStoring)
	{
//		m_fiMMP.SetLength(0); // vide le fichier
		m_fiMMP.SeekToBegin( ); // et se replace au debut
// code goes here
		m_fiMMP.SeekToBegin();
	}
	else
	{
		m_fiMMP.SeekToBegin();
// code goes here
		m_fiMMP.SeekToBegin();	
	}
}

void CW3ZMapEditDoc::SerializePathTGA(BOOL IsStoring)
{

	int i, iWidth, iHeight;
	byte abBuf[30];
	byte* pBitmap;

	if (IsStoring)
	{
		m_fiPathTGA.SetLength(0); // vide le fichier
		m_fiPathTGA.SeekToBegin( ); // et se replace au debut

		// gets path data from the WPM
		m_War3Map.WPM2PathTGA();

		// HEADER
		abBuf[0] = 0; // m_IDLength
		abBuf[1] = 0; // m_ColorMapType
		abBuf[2] = TGA_TYPE_UNCOMP_RGB; // m_ImageType 
		//	Color Map Spec 5 bytes
		abBuf[3] = 0; // m_fiirstEntryIndex
		abBuf[4] = 0; // m_fiirstEntryIndex
		abBuf[5] = 0; // m_ColorMapLength
		abBuf[6] = 0; // m_ColorMapLength
		abBuf[7] = 0; // m_ColorMapEntrySize
		// Image Spec 10 bytes
		abBuf[8] = 0; // m_XOrg
		abBuf[9] = 0; // m_XOrg
		abBuf[10] = 0; // m_YOrg
		abBuf[11] = 0; // m_YOrg
		iWidth = m_War3Map.mapTGAPath.GetWidth();
		abBuf[12] = (byte)(iWidth & 0x00FF); // Width
		abBuf[13] = (byte)((iWidth & 0xFF00) / 0x0100); // Width
		iHeight = m_War3Map.mapTGAPath.GetHeight();
		abBuf[14] = (byte)(iHeight & 0x00FF); // Height
		abBuf[15] = (byte)((iHeight & 0xFF00) / 0x0100); // Height
		abBuf[16] = 32; // m_PixelDepth
		abBuf[17] = TGA_IMG_WAR3_FORMAT; //m_ImgDesc
		m_fiPathTGA.Write(abBuf,18);
		//TARGA DATA
		//data size
		i = m_War3Map.mapTGAPath.GetWidth() * m_War3Map.mapTGAPath.GetHeight() * 4;
		pBitmap = m_War3Map.mapTGAPath.GetBitmapPtr();
		if (pBitmap)
		{m_fiPathTGA.Write(pBitmap, i);}
		for (i=0; i < STR_TRUEVISION_LEN; i++)
		{
			abBuf[i] = STR_TRUEVISION[i];
		}
		m_fiPathTGA.Write(abBuf, STR_TRUEVISION_LEN);
		m_fiPathTGA.SeekToBegin();
	}
	else
	{
		try
		{
			m_fiPathTGA.SeekToBegin();
			if (m_fiPathTGA.GetLength() <= 18)
			{// file too short
				throw CWar3FileException(CWar3FileException::missingData);
			}
			//read header
			m_fiPathTGA.Read(abBuf,18);
			iWidth = (abBuf[12] + abBuf[13] * 0x0100);
			iHeight = (abBuf[14] + abBuf[15] * 0x0100);

			// format tests
			if ((abBuf[0]) // IDlength
				|| (abBuf[1]) // ColorMapType
				|| (abBuf[2] != TGA_TYPE_UNCOMP_RGB) // image type
				//	Color Map Spec 5 bytes
				|| (abBuf[3] + abBuf[4] * 0x0100) // FirstEntryIndex
				|| (abBuf[5] + abBuf[6] * 0x0100) // ColorMapLength
				|| (abBuf[7]) // ColorMapEntrySize
				// Image Spec 10 bytes
				|| (abBuf[8] + abBuf[9] * 0x0100) // XOrg
				|| (abBuf[10] + abBuf[11] * 0x0100) // YOrg
				|| (0 >= iWidth) // Width
				|| (0 >= iHeight) // Height
				|| (abBuf[16] != 32) // PixelDepth
				|| (abBuf[17] != TGA_IMG_WAR3_FORMAT) // ImgDesc
				)
			{// wrong format
				throw CWar3FileException(CWar3FileException::unhandledData);
			}

			//check PathTGA image size
			if ((iWidth != m_War3Map.GetMapWidth()*PATH_MAP_SUBDIV) || (iHeight != m_War3Map.GetMapHeight()*PATH_MAP_SUBDIV))
			{// invalid size
				throw CWar3FileException(CWar3FileException::erroneousData);
			}

			//check data size
			i = iWidth * iHeight * 4; //4 bytes for each pixel
			if (m_fiPathTGA.GetLength() < (unsigned)(i + 18))
			{ // file too short
				throw CWar3FileException(CWar3FileException::missingData);
			}
			// size ok
			m_War3Map.mapTGAPath.SetSize(iWidth, iHeight);
			pBitmap = m_War3Map.mapTGAPath.GetBitmapPtr();
			m_fiPathTGA.Read(pBitmap, i);
			// file loaded successfully, sets the flag
			m_iFilesInitStatus |= FL_W3M_PATHTGA;
		}
		catch (CWar3FileException *ex)
		{
			// corrupted PathTGA
			// displays a warning
			W3Z_WARNING(STR_WARNING_PATHTGA00);
			// create a new PathTGA
			m_War3Map.mapTGAPath.SetSize(m_War3Map.GetMapWidth()*PATH_MAP_SUBDIV, m_War3Map.GetMapHeight()*PATH_MAP_SUBDIV);
			delete ex;
		}
		m_fiPathTGA.SeekToBegin();
	}
}


/*
SerializeWPM
************
Serialize the pathing data of the path map in the WPM file.

In:
 BOOL IsStoring: TRUE if storing, FALSE if loading

Out:
 nothing

Return:
 nothing

*/
void CW3ZMapEditDoc::SerializeWPM(BOOL IsStoring)
{
	int i, iWidth, iHeight, size;
	byte *pBytemap;
	CWar3StrID strID;

	if (IsStoring)
	{
		iWidth = m_War3Map.GetPathMapWidth();
		iHeight = m_War3Map.GetPathMapHeight();
		size = iWidth*iHeight;
		pBytemap = m_War3Map.GetPathMapPtr();
		if (pBytemap)
		{
			m_fiWPM.SetLength(0); // clear file
			m_fiWPM.SeekToBegin( ); // seek to the begining
			m_fiWPM.WriteStrID(STR_WPM_HEADER);
			m_fiWPM.WriteInt(WPM_VERSION_DEFAULT);
			m_fiWPM.WriteInt(iWidth);
			m_fiWPM.WriteInt(iHeight);
			m_fiWPM.Write(pBytemap, size);
		}
		m_fiWPM.SeekToBegin();
	}
	else
	{
		m_fiWPM.SeekToBegin();
		// gets what is the expected path map size
		iWidth = m_War3Map.GetMapWidth()*PATH_MAP_SUBDIV;
		iHeight = m_War3Map.GetMapHeight()*PATH_MAP_SUBDIV;
		size = iWidth*iHeight;
		try
		{
			if (m_fiWPM.GetLength() < (unsigned)(size + WPM_HEADER_SIZE))
			{throw CWar3FileException(CWar3FileException::missingData, STR_WARNING_WPM05);}
			// reads file header...
			// checks WPM file ID
			m_fiWPM.ReadStrID(strID);
			if (strID != STR_WPM_HEADER)
			{throw CWar3FileException(CWar3FileException::invalidFileID, STR_WARNING_WPM01);}

			//checks WPM version
			m_fiWPM.ReadInt(i);
			if (i != WPM_VERSION_DEFAULT)
			{throw CWar3FileException(CWar3FileException::unsupportedVersion, STR_WARNING_WPM02);}

			// reads and check the width
			m_fiWPM.ReadInt(i);
			if (i != iWidth)
			{throw CWar3FileException(CWar3FileException::erroneousData, STR_WARNING_WPM03);}

			// reads and check the height
			m_fiWPM.ReadInt(i);
			if (i != iHeight)
			{throw CWar3FileException(CWar3FileException::erroneousData, STR_WARNING_WPM04);}

			// sets the path map...
			m_War3Map.SetPathMapSize(iWidth, iHeight);
			pBytemap = m_War3Map.GetPathMapPtr();
			// ... and fill it with the data
			m_fiWPM.Read(pBytemap, size);

			// sets the WPM initialisation flag
			m_iFilesInitStatus |= FL_W3M_WPM;
		}
		catch (CWar3FileException *ex)
		{ // displays the warning message
			CString strFullErrorMsg;
			strFullErrorMsg = STR_WARNING_WPM00;
			strFullErrorMsg += ex->GetMessage();
			strFullErrorMsg += STR_WARNING_WPM00b;
			W3Z_WARNING(strFullErrorMsg);
			delete ex;
		}
		m_fiWPM.SeekToBegin();	
	}
}

void CW3ZMapEditDoc::SerializeDOO(BOOL IsStoring)
{
	int i, nbDoodads;
	char cBuf[5];
	byte bBuf[60]; // taille d'un block definissant un doodad
	float fBuf[1];
	BYTE *pBuf= NULL; // unsupported buffer
	unsigned int iBuf[1];
	CDoodad doodad;

	if (IsStoring)
	{
		m_fiDOO.SetLength(0); // vide le fichier
		m_fiDOO.SeekToBegin( ); // et se replace au debut
		strcpy(cBuf, STR_DOO_HEADER);
		m_fiDOO.Write(cBuf, 4); // write file ID
		iBuf[0] = DOO_VERSION_DEFAULT;
		m_fiDOO.Write(iBuf, 4); // write version
		iBuf[0] = m_War3Map.mapUnknownDooInt;
		m_fiDOO.Write(iBuf, 4); // write unknown UINT
		nbDoodads = m_War3Map.GetNbDoodads();
		m_fiDOO.Write(&nbDoodads, 4); // write number of doodads
		m_War3Map.GoFirstDoodad();
		for (i=0; i < nbDoodads; i++)
		{
			doodad = m_War3Map.GetNextDoodad();
			m_fiDOO.WriteStrID(doodad.type); // write doodad type
			iBuf[0] = doodad.variation;
			m_fiDOO.Write(iBuf,4); // write variation
			fBuf[0] = doodad.x;
			m_fiDOO.Write(fBuf,4); // write X
			fBuf[0] = doodad.y;
			m_fiDOO.Write(fBuf,4); // write Y
			fBuf[0] = doodad.z;
			m_fiDOO.Write(fBuf,4); // write Z
			fBuf[0] = doodad.angle;
			m_fiDOO.Write(fBuf,4); // write angle
			fBuf[0] = doodad.scaleX;
			m_fiDOO.Write(fBuf,4); // write scale X
			fBuf[0] = doodad.scaleY;
			m_fiDOO.Write(fBuf,4); // write scale Y
			fBuf[0] = doodad.scaleZ;
			m_fiDOO.Write(fBuf,4); // write scale Z
			bBuf[0] = (byte) doodad.flags;
			bBuf[1] = (byte) doodad.life;
			m_fiDOO.Write(bBuf, 2); // write unknown byte and life
			iBuf[0] = doodad.ID;
			m_fiDOO.Write(iBuf,4); // write variation
		}
//		iBuf[0] = 0;
//		m_fiDOO.Write(iBuf, 4); // write last 8 '0' bytes
//		iBuf[0] = 0;
//		m_fiDOO.Write(iBuf, 4);
		// write unsupported bytes
		try
		{
			i = m_War3Map.GetDOOUnsupportedBufferSize();
			if (i > 0)
			{
				pBuf = m_War3Map.GetDOOUnsupportedBuffer();
				m_fiDOO.Write(pBuf, i);
			}
		}
		catch (CMemoryException* e)
		{
			delete e;
		}
		m_fiDOO.SeekToBegin();
	}
	else
	{
		m_fiDOO.SeekToBegin();

		m_fiDOO.Read(cBuf, 4); // read file ID
		cBuf[4] = '\0';
		m_fiDOO.Read(iBuf, 4); // read version
		if (!strcmp(cBuf, STR_DOO_HEADER))
		{
			if (8 == iBuf[0])
			{// version 8
				m_fiDOO.Read(iBuf, 4); // read unknown UINT
				m_War3Map.mapUnknownDooInt = iBuf[0];
				m_fiDOO.Read(&nbDoodads, 4); // read number of trees
				for (i=0; i < nbDoodads; i++)
				{
					m_fiDOO.Read(bBuf, 50); // read 1 doodad block
					strncpy(cBuf, (char*)bBuf, 4);
					cBuf[4] = '\0';
					doodad.type = cBuf;
					doodad.variation = *(int*)(bBuf + 4);
					doodad.x = *(float*)(bBuf + 8);
					doodad.y = *(float*)(bBuf + 12);
					doodad.z = *(float*)(bBuf + 16);
					doodad.angle = *(float*)(bBuf + 20);
					doodad.scaleX = *(float*)(bBuf + 24);
					doodad.scaleY = *(float*)(bBuf + 28);
					doodad.scaleZ = *(float*)(bBuf + 32);
					doodad.flags = bBuf[36];
					doodad.life = bBuf[37];
					doodad.ID = *(int*)(bBuf + 46);
					m_War3Map.AddDoodad(doodad);
				}
				// gets unsupported buffer
				i = (int)(m_fiDOO.GetLength() - m_fiDOO.GetPosition());
				if (i > 0)
				{
					m_War3Map.SetDOOUnsupportedBufferSize(i);
					pBuf = m_War3Map.GetDOOUnsupportedBuffer();
					m_fiDOO.Read(pBuf,i);
				}

				// file loaded successfully, sets the flag
				m_iFilesInitStatus |= FL_W3M_DOO;
			}
			else if (7 == iBuf[0])
			{// version 7
				m_fiDOO.Read(iBuf, 4); // read unknown UINT
				m_War3Map.mapUnknownDooInt = iBuf[0];
				m_fiDOO.Read(&nbDoodads, 4); // read number of trees
				for (i=0; i < nbDoodads; i++)
				{
					m_fiDOO.Read(bBuf, 42); // read 1 doodad block
					strncpy(cBuf, (char*)bBuf, 4);
					cBuf[4] = '\0';
					doodad.type = cBuf;
					doodad.variation = *(int*)(bBuf + 4);
					doodad.x = *(float*)(bBuf + 8);
					doodad.y = *(float*)(bBuf + 12);
					doodad.z = *(float*)(bBuf + 16);
					doodad.angle = *(float*)(bBuf + 20);
					doodad.scaleX = *(float*)(bBuf + 24);
					doodad.scaleY = *(float*)(bBuf + 28);
					doodad.scaleZ = *(float*)(bBuf + 32);
					doodad.flags = bBuf[36];
					doodad.life = bBuf[37];
					doodad.ID = *(int*)(bBuf + 38);
					m_War3Map.AddDoodad(doodad);
				}
				// gets unsupported buffer
				i = (int)(m_fiDOO.GetLength() - m_fiDOO.GetPosition());
				if (i > 0)
				{
					m_War3Map.SetDOOUnsupportedBufferSize(i);
					pBuf = m_War3Map.GetDOOUnsupportedBuffer();
					m_fiDOO.Read(pBuf,i);
				}

				// file loaded successfully, sets the flag
				m_iFilesInitStatus |= FL_W3M_DOO;
			}
			else if (6 == iBuf[0])
			{// version 6
				m_fiDOO.Read(iBuf, 4); // read unknown UINT
				m_War3Map.mapUnknownDooInt = iBuf[0];
				m_fiDOO.Read(&nbDoodads, 4); // read number of trees
				for (i=0; i < nbDoodads; i++)
				{
					m_fiDOO.Read(bBuf, 41); // read 1 doodad block
					strncpy(cBuf, (char*)bBuf, 4);
					cBuf[4] = '\0';
					doodad.type = cBuf;
					doodad.variation = *(int*)(bBuf + 4);
					doodad.x = *(float*)(bBuf + 8);
					doodad.y = *(float*)(bBuf + 12);
					doodad.z = *(float*)(bBuf + 16);
					doodad.angle = *(float*)(bBuf + 20);
					doodad.scaleX = *(float*)(bBuf + 24);
					doodad.scaleY = *(float*)(bBuf + 28);
					doodad.scaleZ = *(float*)(bBuf + 32);
					doodad.flags = bBuf[36];
					doodad.ID = *(int*)(bBuf + 37);
					m_War3Map.AddDoodad(doodad);
				}

				// gets unsupported buffer
				i = (int)(m_fiDOO.GetLength() - m_fiDOO.GetPosition());
				if (i > 0)
				{
					m_War3Map.SetDOOUnsupportedBufferSize(i);
					pBuf = m_War3Map.GetDOOUnsupportedBuffer();
					m_fiDOO.Read(pBuf,i);
				}

				// file loaded successfully, sets the flag
				m_iFilesInitStatus |= FL_W3M_DOO;
			}
			else
			{// unsupported version
				CString msg;
				msg.Format(STR_ERROR_DOO00, iBuf[0]);
				W3Z_ERROR(msg);
			}
		}
		else
		{// header incorrecte
		}
		m_fiDOO.SeekToBegin();
	}
}

void CW3ZMapEditDoc::SerializeUnitsDOO(BOOL IsStoring)
{
	if (IsStoring)
	{
//		m_fiUnitsDOO.SetLength(0); // vide le fichier
		m_fiUnitsDOO.SeekToBegin( ); // et se replace au debut
// code goes here
		m_fiUnitsDOO.SeekToBegin();
	}
	else
	{
		m_fiUnitsDOO.SeekToBegin();
// code goes here
		m_fiUnitsDOO.SeekToBegin();	
	}
}

void CW3ZMapEditDoc::SerializeW3R(BOOL IsStoring)
{
	if (IsStoring)
	{
//		m_fiW3R.SetLength(0); // vide le fichier
		m_fiW3R.SeekToBegin( ); // et se replace au debut
// code goes here
		m_fiW3R.SeekToBegin();
	}
	else
	{
		m_fiW3R.SeekToBegin();
// code goes here
		m_fiW3R.SeekToBegin();	
	}
}

void CW3ZMapEditDoc::SerializeW3C(BOOL IsStoring)
{
	if (IsStoring)
	{
//		m_fiW3C.SetLength(0); // vide le fichier
		m_fiW3C.SeekToBegin( ); // et se replace au debut
// code goes here
		m_fiW3C.SeekToBegin();
	}
	else
	{
		m_fiW3C.SeekToBegin();
// code goes here
		m_fiW3C.SeekToBegin();	
	}
}

void CW3ZMapEditDoc::SerializeW3U(BOOL IsStoring)
{
	if (IsStoring)
	{
//		m_fiW3U.SetLength(0); // vide le fichier
		m_fiW3U.SeekToBegin( ); // et se replace au debut
// code goes here
		m_fiW3U.SeekToBegin();
	}
	else
	{
		m_fiW3U.SeekToBegin();
// code goes here
		m_fiW3U.SeekToBegin();	
	}
}

void CW3ZMapEditDoc::SerializeW3S(BOOL IsStoring)
{
	if (IsStoring)
	{
//		m_fiW3S.SetLength(0); // vide le fichier
		m_fiW3S.SeekToBegin( ); // et se replace au debut
// code goes here
		m_fiW3S.SeekToBegin();
	}
	else
	{
		m_fiW3S.SeekToBegin();
// code goes here
		m_fiW3S.SeekToBegin();	
	}
}

void CW3ZMapEditDoc::SerializeIMP(BOOL IsStoring)
{
	if (IsStoring)
	{
//		m_fiIMP.SetLength(0); // vide le fichier
		m_fiIMP.SeekToBegin( ); // et se replace au debut
// code goes here
		m_fiIMP.SeekToBegin();
	}
	else
	{
		m_fiIMP.SeekToBegin();
// code goes here
		m_fiIMP.SeekToBegin();	
	}
}

void CW3ZMapEditDoc::SerializeListfile(BOOL IsStoring)
{
	int i;

	if (IsStoring)
	{
		// do nothing
	}
	else
	{
		m_fiListfile.SeekToBegin();
		i = (int)m_fiListfile.GetLength();
		m_fiListfile.Read(m_strListfile.GetBufferSetLength(i), i);
		m_strListfile.ReleaseBuffer();
		m_fiListfile.SeekToBegin();
		// file loaded successfully, sets the flag
		m_iFilesInitStatus |= FL_W3M_LISTEFILE;
	}
}


BOOL CW3ZMapEditDoc::OpenTemporaryFile(CFile &file)
{
	CFileFind ffDir;
	CString tempfile;
	int i;

	// cherche un nom de fichier libre
	tempfile = m_strTempDir + STR_W3Z_TEMP_FILENAME;
	i =0;
	while(ffDir.FindFile(tempfile) && (i < 9999))
	{ // tant que le fichier existe deja regarde pour le suivant
		i++;
		tempfile.Format("%04d", i);
		tempfile = m_strTempDir + STR_W3Z_TEMP_HEADFILENAME + tempfile + STR_W3Z_TEMP_FILEEXT;
	}
	if (ffDir.FindFile(tempfile))
	{
		//cas ou les noms standards ne suffisent plus
		//randomize un nom		
		srand( (unsigned)time( NULL ));
		tempfile = m_strTempDir;
		for(i=0; i < 8; i++ )
		{
			tempfile += (char) ('A' + (char) (rand()%26));
		}
		tempfile += STR_W3Z_TEMP_FILEEXT;
		if (ffDir.FindFile(tempfile))
		{
			//+FIXME
			//erreur: impossible de trouver un nom de fichier libre!
		}
	}

	CFileException e;

	if (!file.Open(tempfile, CFile::modeCreate | CFile::modeReadWrite | CFile::shareDenyNone, &e))
	{
		// le fichier n'a pu etre ouvert
		return FALSE;
	}

	return TRUE;
}

void CW3ZMapEditDoc::CloseAndDeleteFile(CFile &file)
{
	CString fname;
	if (CFile::hFileNull != file.m_hFile)
	{
		try
		{
			fname = file.GetFilePath();
			file.Close();
			CFile::Remove(fname);
		}
		catch (CFileException* pEx)
		{
			pEx->Delete();
		}
	}
}


BOOL CW3ZMapEditDoc::ReadEmbededFile(MPQHANDLE hMPQ, CString filename, CFile &file)
{
	char *pBuf;
	MPQHANDLE hFile;

	file.SetLength(0); // clear the file
	file.SeekToBegin();

	if (SFileOpenFileEx(hMPQ,filename , 0, &hFile))
	{//teste si le fichier existe
		DWORD sz;
		sz = SFileGetFileSize(hFile,0); // taille du fichier
		if (sz)
		{
			// peut-etre un try-except ici
			pBuf = new char[sz];
			if (pBuf)
			{
				SFileReadFile(hFile, pBuf, sz, 0, 0);
				SFileCloseFile(hFile);
				file.Write(pBuf, sz);
				file.SeekToBegin();
				delete []pBuf;
			}
		}
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CW3ZMapEditDoc::WriteEmbededFile(MPQHANDLE hMPQ, CString srcFilename, CString destFilename)
{

/*
// AddFileToArchive flags
#define MAFA_EXISTS           0x80000000 //Will be added if not present
#define MAFA_UNKNOWN40000000  0x40000000
#define MAFA_MODCRYPTKEY      0x00020000
#define MAFA_ENCRYPT          0x00010000
#define MAFA_COMPRESS         0x00000200
#define MAFA_COMPRESS2        0x00000100
#define MAFA_REPLACE_EXISTING 0x00000001

// AddFileToArchiveEx compression flags
#define MAFA_COMPRESS_STANDARD 0x08 //Standard PKWare DCL compression
#define MAFA_COMPRESS_DEFLATE  0x02 //ZLib's deflate compression
#define MAFA_COMPRESS_WAVE     0x81 //Standard wave compression
#define MAFA_COMPRESS_WAVE2    0x41 //Unused wave compression

// Flags for individual compression types used for wave compression
#define MAFA_COMPRESS_WAVECOMP1 0x80 //Main compressor for standard wave compression
#define	MAFA_COMPRESS_WAVECOMP2 0x40 //Main compressor for unused wave compression
#define MAFA_COMPRESS_WAVECOMP3 0x01 //Secondary compressor for wave compression

// AddWAVToArchive flags
#define MAWA_QUALITY_HIGH    1
#define MAWA_QUALITY_MEDIUM  0
#define MAWA_QUALITY_LOW     2

MpqAddFileToArchiveEx(MPQHANDLE hMPQ, LPCSTR lpSourceFileName, LPCSTR lpDestFileName, DWORD dwFlags, DWORD dwCompressionType, DWORD dwWAVQuality);

*/
	if (!MpqAddFileToArchiveEx(hMPQ, srcFilename, destFilename, MAFA_REPLACE_EXISTING | MAFA_COMPRESS, MAFA_COMPRESS_DEFLATE, MAWA_QUALITY_HIGH))
	{
		//failed
		return FALSE;
	}
	return TRUE;
}


// This is a recursive function which empty a directory path 
// of its files and call itself if necessary to delete
// subdirectories.
// Used to clean temporary files in the temporary directory.
void CW3ZMapEditDoc::EmptyTemporaryDirectory(CString &sPath)
{

 CFileFind finder;

 CString  sWildCard = sPath + "\\*.tmp";

 BOOL bWorking = finder.FindFile(sWildCard);

 while (bWorking) 
 {
  bWorking = finder.FindNextFile();

  if (finder.IsDots()) continue;

  if (finder.IsDirectory()) 
  {
   CString s = finder.GetFilePath();
   EmptyTemporaryDirectory(s);
   RemoveDirectory(finder.GetFilePath());
   continue; 
  }
  _unlink( finder.GetFilePath() );
 }

}


/*
 UNDO LIST
 ---------
 Empile les nouveaux elements en tete de liste,
 retire les vieux elements en trop depuis la queue de liste.

*/
void CW3ZMapEditDoc::OpenUndoObj()
{
	CListedUndoObj* pLUndoObj;

	m_bOpenedUndoObj = true;
	pLUndoObj = new CListedUndoObj();
	pLUndoObj->prev = NULL; 
	if (m_pUndoListHead)
	{ // liste non vide
		pLUndoObj->next = m_pUndoListHead;
		m_pUndoListHead->prev = pLUndoObj;
		m_pUndoListHead = pLUndoObj;
	}
	else
	{ // liste vide
		pLUndoObj->next = NULL;
		m_pUndoListHead = m_pUndoListTail = pLUndoObj;
	}
	m_iNbUndos++;
	// s'il y a trop d'elements, retirer l'excedent en queue de liste
	while ((m_iNbUndos > m_iMaxUndos) && m_pUndoListTail)
	{
		pLUndoObj = m_pUndoListTail;
		m_pUndoListTail = m_pUndoListTail->prev;
		if (m_pUndoListTail)
		{m_pUndoListTail->next = NULL;}
		else
		{m_pUndoListHead = NULL;}
		delete pLUndoObj;
		m_iNbUndos--;
	}
	if ((m_iMapWidth != m_War3Map.GetMapWidth()) || (m_iMapHeight != m_War3Map.GetMapHeight()))
	{
		m_iMapWidth = m_War3Map.GetMapWidth();
		m_iMapHeight = m_War3Map.GetMapHeight();
		if (m_pUndoTPMap)
		{
			delete []m_pUndoTPMap;
		}
		m_pUndoTPMap = new char[(m_iMapWidth+1)*(m_iMapHeight+1)];
		if (m_pUndoPathMap)
		{
			delete []m_pUndoPathMap;
		}
		m_pUndoPathMap = new char[(m_iMapWidth*4)*(m_iMapHeight*4)];
		if (m_pUndoShadowsMap)
		{
			delete []m_pUndoShadowsMap;
		}
		m_pUndoShadowsMap = new char[(m_iMapWidth*4)*(m_iMapHeight*4)];
	}
	// clear tilepoints map
	if (m_pUndoTPMap)
	{
		memset(m_pUndoTPMap, 0, (m_iMapWidth+1)*(m_iMapHeight+1));
	}
	if (m_pUndoPathMap)
	{
		memset(m_pUndoPathMap, 0, (m_iMapWidth*4)*(m_iMapHeight*4));
	}
	if (m_pUndoShadowsMap)
	{
		memset(m_pUndoShadowsMap, 0, (m_iMapWidth*4)*(m_iMapHeight*4));
	}
}

void CW3ZMapEditDoc::CloseUndoObj()
{
	CListedUndoObj* pLUndoObj;

	// remove the undo if it is empty
	if ((NULL != m_pUndoListHead) && (TRUE == m_pUndoListHead->m_UndoObj.IsEmpty()))
	{
		pLUndoObj = m_pUndoListHead;
		m_pUndoListHead = m_pUndoListHead->next;
		if (m_pUndoListHead)
		{m_pUndoListHead->prev = NULL;}
		else
		{m_pUndoListTail = NULL;}
		delete pLUndoObj;
		m_iNbUndos--;
	}
	m_bOpenedUndoObj = false;
}

void CW3ZMapEditDoc::DoUndo(int n)
{
	CWaitCursor wait;
	CListedUndoObj* pLUndoObj;
	int i;
	i = n;
	if (m_bOpenedUndoObj)
	{
		TRACE("Doc-Undo-Warning: DoUndo called while an undo object was opened!\n");
		CloseUndoObj();
	}
	while ((i > 0) && (m_pUndoListHead))
	{
		pLUndoObj = m_pUndoListHead;
		m_pUndoListHead = m_pUndoListHead->next;
		if (m_pUndoListHead)
		{m_pUndoListHead->prev = NULL;}
		else
		{m_pUndoListTail = NULL;}
		pLUndoObj->m_UndoObj.DoUndo(this);
		delete pLUndoObj;
		m_iNbUndos--;
		i--;
	}
	POSITION pos = GetFirstViewPosition();
	CW3ZMapEditView* pView = (CW3ZMapEditView *)GetNextView(pos);
	pView->InvalidateBitmap();
	pView->Invalidate();
	CW3ZMapOpenGLView* pGLView = NULL;
	if (pos != NULL)
	{
		pGLView = (CW3ZMapOpenGLView*)GetNextView(pos);
		if (NULL != pGLView)
		{
			pGLView->Update3DView();
		}
	}
}

void CW3ZMapEditDoc::AddToUndoObj(CTilePoint tilepoint, int x, int y)
{
	int i;
	if (m_bOpenedUndoObj && m_pUndoListHead)
	{
		if ((m_pUndoTPMap) && (x >= 0) && (x <= m_iMapWidth) && (y >= 0) && (y <= m_iMapHeight))
		{
			i = y * (m_iMapWidth+1) + x;
			if (!m_pUndoTPMap[i])
			{// add the tilepoint only if it has not already been added before
				m_pUndoTPMap[i] = '\x0FF';
				m_pUndoListHead->m_UndoObj.AddTilePoint(tilepoint,x,y);
				m_pUndoListHead->m_UndoObj.m_sUndoMsg = UNDO_STR_TILEPOINT;
			}
		}
		else
		{// no undo tilepoints map set
			m_pUndoListHead->m_UndoObj.AddTilePoint(tilepoint,x,y);
			m_pUndoListHead->m_UndoObj.m_sUndoMsg = UNDO_STR_TILEPOINT;
		}
	}
}

void CW3ZMapEditDoc::AddToUndoObj(CDoodad doodad, int flag)
{
	if (m_bOpenedUndoObj && m_pUndoListHead)
	{
		if (UNDO_DOOD_ADD == flag)
		{
			m_pUndoListHead->m_UndoObj.AddAddDoodad(doodad);
			m_pUndoListHead->m_UndoObj.m_sUndoMsg = UNDO_STR_DOOD_ADD;
		}
		else if (UNDO_DOOD_DEL == flag)
		{
			m_pUndoListHead->m_UndoObj.AddDelDoodad(doodad);
			m_pUndoListHead->m_UndoObj.m_sUndoMsg = UNDO_STR_DOOD_REMOVE;
		}
		else if (UNDO_DOOD_UPD == flag)
		{
			m_pUndoListHead->m_UndoObj.AddUpdDoodad(doodad);
			m_pUndoListHead->m_UndoObj.m_sUndoMsg = UNDO_STR_DOOD_EDIT;
		}
	}

}

void CW3ZMapEditDoc::OnEditUndo() 
{
	DoUndo(1);
}

void CW3ZMapEditDoc::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	CString msg;
	if (m_pUndoListHead)
	{
		msg = UNDO_STR_MENU00 + m_pUndoListHead->m_UndoObj.m_sUndoMsg + UNDO_STR_MENU01;
		pCmdUI->SetText(msg);
		pCmdUI->Enable(TRUE);
	}
	else
	{
		msg = UNDO_STR_MENU02;
		pCmdUI->SetText(msg);
		pCmdUI->Enable(FALSE);
	}
	
}

void CW3ZMapEditDoc::AddPathToUndoObj(int oldpath, int x, int y)
{
	int i;
	if (m_bOpenedUndoObj && m_pUndoListHead)
	{
		if ((m_pUndoPathMap) && (x >= 0) && (x < m_iMapWidth*4) && (y >= 0) && (y < m_iMapHeight*4))
		{
			i = y*m_iMapWidth*4 + x;
			if (!m_pUndoPathMap[i])
			{// add the path point only if it has not already been added before
				m_pUndoPathMap[i] = '\x0FF';
				m_pUndoListHead->m_UndoObj.AddPath(oldpath, x, y);
				m_pUndoListHead->m_UndoObj.m_sUndoMsg = UNDO_STR_PATHING;
			}
		}
		else
		{// no undo path map set
			m_pUndoListHead->m_UndoObj.AddPath(oldpath,x,y);
			m_pUndoListHead->m_UndoObj.m_sUndoMsg = UNDO_STR_PATHING;
		}
	}
}


/*
SetTitle
********
Overriden to add an extension to a newly created file.

*/
void CW3ZMapEditDoc::SetTitle(LPCTSTR lpszTitle) 
{
	CString str, sTitle;
	sTitle = lpszTitle;
	str = sTitle.Right(4);

	// check if it's a W3E or a W3M
	if ((0 != str.CompareNoCase(STR_W3E_EXTENSION)) && ( 0!= str.CompareNoCase(STR_W3M_EXTENSION)) && ( 0!= str.CompareNoCase(STR_W3X_EXTENSION)))
	{// if not
		if (0 == str.ReverseFind('.'))
		{// change the extension
			sTitle = sTitle.Left(sTitle.GetLength() - 4) + STR_W3M_EXTENSION;
		}
		else
		{// or add the right one
			sTitle += STR_W3M_EXTENSION;
		}
	}
	CDocument::SetTitle(sTitle);
}


/*
SetPathName
***********
Overriden to make sure the user is using a supported extension.

*/
void CW3ZMapEditDoc::SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU) 
{
	CString str, strPathName;
	BOOL l_bAddToMRU;

	l_bAddToMRU = bAddToMRU;
	strPathName = lpszPathName;
	str = strPathName.Right(4);
	// check if it's a W3E or a W3M
	if ((str.CompareNoCase(STR_W3E_EXTENSION)) && (str.CompareNoCase(STR_W3M_EXTENSION)) && ( 0!= str.CompareNoCase(STR_W3X_EXTENSION)))
	{// if not
		if (0 == str.ReverseFind('.'))
		{// replace the extension
			strPathName = strPathName.Left(strPathName.GetLength() - 4) + STR_W3M_EXTENSION;
		}
		else
		{// or add the correct one
			strPathName += STR_W3M_EXTENSION;
		}
		l_bAddToMRU = FALSE;
	}
	CDocument::SetPathName(strPathName, l_bAddToMRU);
}

void CW3ZMapEditDoc::UpdateMapPropertiesDialog()
{
	//  Main Page
	m_pEditMapPropSheet->m_MapPropMainPage.m_sName			= m_War3Map.mapTrigStringsTable.GetTrigStringValue(m_War3Map.mapW3Info.mapNameTID);
	m_pEditMapPropSheet->m_MapPropMainPage.m_sAuthor		= m_War3Map.mapTrigStringsTable.GetTrigStringValue(m_War3Map.mapW3Info.mapAuthorTID);
	m_pEditMapPropSheet->m_MapPropMainPage.m_sPlayText		= m_War3Map.mapTrigStringsTable.GetTrigStringValue(m_War3Map.mapW3Info.mapSugPlyrTID);
	m_pEditMapPropSheet->m_MapPropMainPage.m_sDesc			= m_War3Map.mapTrigStringsTable.GetTrigStringValue(m_War3Map.mapW3Info.mapDescTID);
	m_pEditMapPropSheet->m_MapPropMainPage.m_bCustTilesets	= m_War3Map.mapCustomTilesets;
	m_pEditMapPropSheet->m_MapPropMainPage.m_sGround		= m_War3Map.mapGroundTiles;
	m_pEditMapPropSheet->m_MapPropMainPage.m_sCliff			= m_War3Map.mapCliffTiles;
	m_pEditMapPropSheet->m_MapPropMainPage.m_cTilesetGroup	= m_War3Map.mapMainTileset;
	m_pEditMapPropSheet->m_MapPropMainPage.m_iNbPlayers		= m_War3Map.mapW3Info.mapNbMaxPlayers;
	m_pEditMapPropSheet->m_MapPropMainPage.m_iEditorVer		= m_War3Map.mapW3Info.mapEditVer;
	m_pEditMapPropSheet->m_MapPropMainPage.m_iMapVer		= m_War3Map.mapW3Info.mapNbSaves;
	  // show water waves on cliff shores
	if (m_War3Map.mapW3Info.mapFlags & W3I_FLG_WAVESCLIFF)
	{
		m_pEditMapPropSheet->m_MapPropMainPage.m_bWavesCliff	= TRUE;
	}
	else
	{
		m_pEditMapPropSheet->m_MapPropMainPage.m_bWavesCliff	= FALSE;
	}
	  // show water waves on rolling shores
	if (m_War3Map.mapW3Info.mapFlags & W3I_FLG_WAVEROLLING)
	{
		m_pEditMapPropSheet->m_MapPropMainPage.m_bWavesRolling	= TRUE;
	}
	else
	{
		m_pEditMapPropSheet->m_MapPropMainPage.m_bWavesRolling	= FALSE;
	}
	  // melee map
	if (m_War3Map.mapW3Info.mapFlags & W3I_FLG_MELEE)
	{
		m_pEditMapPropSheet->m_MapPropMainPage.m_bMelee			= TRUE;
	}
	else
	{
		m_pEditMapPropSheet->m_MapPropMainPage.m_bMelee			= FALSE;
	}
	  // unknown flag 1
	if (m_War3Map.mapW3Info.mapFlags & W3I_FLG_NOTREDUCED)
	{
		m_pEditMapPropSheet->m_MapPropMainPage.m_bUnknown0	= TRUE;
	}
	else
	{
		m_pEditMapPropSheet->m_MapPropMainPage.m_bUnknown0	= FALSE;
	}
	  // unknown flag 2
	if (m_War3Map.mapW3Info.mapFlags & W3I_FLG_MENUPROP)
	{
		m_pEditMapPropSheet->m_MapPropMainPage.m_bUnknown1	= TRUE;
	}
	else
	{
		m_pEditMapPropSheet->m_MapPropMainPage.m_bUnknown1	= FALSE;
	}

	// Size Page
	m_pEditMapPropSheet->m_MapPropSizePage.m_iWidth			= m_War3Map.GetMapWidth();
	m_pEditMapPropSheet->m_MapPropSizePage.m_iHeight		= m_War3Map.GetMapHeight();
	m_pEditMapPropSheet->m_MapPropSizePage.m_iPlayableWidth	= m_War3Map.mapW3Info.mapPlayableWidth;
	m_pEditMapPropSheet->m_MapPropSizePage.m_iPlayableHeight	= m_War3Map.mapW3Info.mapPlayableHeight;
	m_pEditMapPropSheet->m_MapPropSizePage.m_fCBblx			= m_War3Map.mapW3Info.mapCameraBounds.fX1;
	m_pEditMapPropSheet->m_MapPropSizePage.m_fCBbly			= m_War3Map.mapW3Info.mapCameraBounds.fY1;
	m_pEditMapPropSheet->m_MapPropSizePage.m_fCBtrx			= m_War3Map.mapW3Info.mapCameraBounds.fX2;
	m_pEditMapPropSheet->m_MapPropSizePage.m_fCBtry			= m_War3Map.mapW3Info.mapCameraBounds.fY2;
	m_pEditMapPropSheet->m_MapPropSizePage.m_fCBtlx			= m_War3Map.mapW3Info.mapCameraBounds.fX3;
	m_pEditMapPropSheet->m_MapPropSizePage.m_fCBtly			= m_War3Map.mapW3Info.mapCameraBounds.fY3;
	m_pEditMapPropSheet->m_MapPropSizePage.m_fCBbrx			= m_War3Map.mapW3Info.mapCameraBounds.fX4;
	m_pEditMapPropSheet->m_MapPropSizePage.m_fCBbry			= m_War3Map.mapW3Info.mapCameraBounds.fY4;

	// Protection Page
	if (W3Z_MAP_PROT_NOTPROTECTED == GetProtection())
	{// if no protection has been set, then disable protection
		m_pEditMapPropSheet->m_MapPropProtectPage.m_bEnableProtect		= FALSE;
	}
	else
	{// enable the protection if it was enabled
		m_pEditMapPropSheet->m_MapPropProtectPage.m_bEnableProtect		= m_bEnableProtect;
	}
	m_pEditMapPropSheet->m_MapPropProtectPage.m_bAuthorMsg			= m_bAuthorMsg;
	m_pEditMapPropSheet->m_MapPropProtectPage.m_bMakeBackup			= m_bMakeBackup;
	m_pEditMapPropSheet->m_MapPropProtectPage.m_bUnpRec				= m_bUnpRec;
	m_pEditMapPropSheet->m_MapPropProtectPage.m_sAuthorPassword		= m_strAuthorPassword;
	m_pEditMapPropSheet->m_MapPropProtectPage.m_sPublicPassword		= m_strPublicPassword;
	m_pEditMapPropSheet->m_MapPropProtectPage.m_sProtectVer			= GetProtectionName();
	m_pEditMapPropSheet->m_MapPropProtectPage.m_sOrgAuthorName		= GetAuthorName();
	if (TRUE == IsAuthorRegistered())
	{
		m_pEditMapPropSheet->m_MapPropProtectPage.m_strRegisteredStatus	= STR_MAP_PROT_AUTHORREGGED;
	}
	else if (TRUE == IsPublicRegistered())
	{
		m_pEditMapPropSheet->m_MapPropProtectPage.m_strRegisteredStatus	= STR_MAP_PROT_PUBLICREGGED;
	}
	else
	{
		m_pEditMapPropSheet->m_MapPropProtectPage.m_strRegisteredStatus	= STR_MAP_PROT_NOTREGGED;
	}
	/*
	ComboBox Items Order (m_iTypeSelected):
		0: No protection type set
		1: Unlucky_Footman-Weaaddar Protection
		2: W3Z Map Protection
		3: Unknown protection type
	*/
	if (W3Z_MAP_PROT_NOTPROTECTED == GetProtection())
	{
		m_pEditMapPropSheet->m_MapPropProtectPage.m_iTypeSelected = 0;
		m_pEditMapPropSheet->m_MapPropProtectPage.m_iProtectLvl = 0;
	}
	else if (W3Z_MAP_PROT_UNLUCKY == GetProtection())
	{
		m_pEditMapPropSheet->m_MapPropProtectPage.m_iTypeSelected = 1;
		m_pEditMapPropSheet->m_MapPropProtectPage.m_iProtectLvl = 0;
	}
	else if ((W3Z_MAP_PROT_W3ZV1L1 == GetProtection()) || (W3Z_MAP_PROT_W3ZV1L2 == GetProtection()) || (W3Z_MAP_PROT_W3ZV1L3 == GetProtection()))
	{
		m_pEditMapPropSheet->m_MapPropProtectPage.m_iTypeSelected = 2;
		if (W3Z_MAP_PROT_W3ZV1L1 == GetProtection())
		{
			m_pEditMapPropSheet->m_MapPropProtectPage.m_iProtectLvl = 0;
		}
		else if (W3Z_MAP_PROT_W3ZV1L2 == GetProtection())
		{
			m_pEditMapPropSheet->m_MapPropProtectPage.m_iProtectLvl = 1;
		}
		else if (W3Z_MAP_PROT_W3ZV1L3 == GetProtection())
		{
			m_pEditMapPropSheet->m_MapPropProtectPage.m_iProtectLvl = 2;
		}
	}
	else
	{
		m_pEditMapPropSheet->m_MapPropProtectPage.m_iTypeSelected = 3;
		m_pEditMapPropSheet->m_MapPropProtectPage.m_iProtectLvl = 0;
	}
	m_pEditMapPropSheet->m_MapPropProtectPage.m_sProtectStat		= "";

	// Sheet pages status
	if (TRUE == IsProtected())
	{// map is protected
		if (TRUE == IsAuthorRegistered())
		{// author is already registered, we don't need him to use the register button anymore
			m_pEditMapPropSheet->EnableFeatures(W3Z_PROP_ALLNOREGISTER);
		}
		else if (TRUE == IsPublicRegistered())
		{// public user registered, author may need to regiter to get higher permissions
			m_pEditMapPropSheet->EnableFeatures(W3Z_PROP_ALLNOPROTECT);
		}
		else
		{// nobody registered
			if ((W3Z_MAP_PROT_W3ZV1L1 == GetProtection()) || (W3Z_MAP_PROT_W3ZV1L2 == GetProtection()) || (W3Z_MAP_PROT_W3ZV1L3 == GetProtection()))
			{// there may be a password to register
				if (TRUE == IsReadOnly())
				{// Map is read only, no editing allowed
					m_pEditMapPropSheet->EnableFeatures(W3Z_PROP_REGISTER);
				}
				else
				{// map not read only (no author registered), only the protect page is forbiden
					m_pEditMapPropSheet->EnableFeatures(W3Z_PROP_ALLNOPROTECT);
				}
			}
			else
			{// no password/unknown protection --> all disabled
				m_pEditMapPropSheet->EnableFeatures(W3Z_PROP_ALLDISABLED);
			}
		}
	}
	else
	{// map not protected, no need to register
		m_pEditMapPropSheet->EnableFeatures(W3Z_PROP_ALLNOREGISTER);
	}
}

void CW3ZMapEditDoc::UpdateFromMapPropertiesDialog()
{
	char groupID;
	CString msg;
    CW3ZMapEditApp(* pTheApp) = (CW3ZMapEditApp *)AfxGetApp();
	CMainFrame *pMainFrame;
	int iPrevProtect;

	if (pTheApp)
	{pMainFrame = pTheApp->GetMainFrame();}
	POSITION pos = GetFirstViewPosition(); // gets the document view
	CW3ZMapEditView* pView = (CW3ZMapEditView *)GetNextView(pos);
	CW3ZMapOpenGLView* pGLView = NULL;
	if (pos != NULL)
	{
		pGLView = (CW3ZMapOpenGLView*)GetNextView(pos);
		if (NULL != pGLView)
		{
			pGLView->Update3DView();
		}
	}

	// if map is read only, change nothing
	if (TRUE == IsReadOnly())
	{
		return;
	}
	
	// changes may occure set modified flag
	SetModifiedFlag();

	//  Main Page
	msg = "";
	m_War3Map.mapTrigStringsTable.SetTrigString(m_pEditMapPropSheet->m_MapPropMainPage.m_sName, m_War3Map.mapW3Info.mapNameTID);
	m_War3Map.mapTrigStringsTable.SetTrigString(m_pEditMapPropSheet->m_MapPropMainPage.m_sAuthor, m_War3Map.mapW3Info.mapAuthorTID);
	m_War3Map.mapTrigStringsTable.SetTrigString(m_pEditMapPropSheet->m_MapPropMainPage.m_sPlayText, m_War3Map.mapW3Info.mapSugPlyrTID);
	m_War3Map.mapTrigStringsTable.SetTrigString(m_pEditMapPropSheet->m_MapPropMainPage.m_sDesc, m_War3Map.mapW3Info.mapDescTID);
	// change main tileset
	groupID = m_pEditMapPropSheet->m_MapPropMainPage.m_cTilesetGroup;
	if (groupID != TG_CHAR_ID_UNDEFINED)
	{
		m_War3Map.mapMainTileset = groupID;
	}
	//
	m_War3Map.mapCustomTilesets = m_pEditMapPropSheet->m_MapPropMainPage.m_bCustTilesets;
	// checks ground tilesets
	if (m_pEditMapPropSheet->m_MapPropMainPage.m_sGround.GetLength()%4)
	{
		msg += STR_WARNING_MAPPROP01;
	}
	else if (m_pEditMapPropSheet->m_MapPropMainPage.m_sGround.GetLength() <= 0)
	{
		msg += STR_WARNING_MAPPROP02;
	}
	else if (m_pEditMapPropSheet->m_MapPropMainPage.m_sGround.GetLength() > W3E_MAX_GROUNDTILES*4)
	{
		msg.Format(STR_WARNING_MAPPROP03, W3E_MAX_GROUNDTILES);
	}
	else
	{
		m_War3Map.mapGroundTiles = m_pEditMapPropSheet->m_MapPropMainPage.m_sGround;
		if (pMainFrame)
		{pMainFrame->UpdateTilesEditBar(this);}
		if (NULL != pView)
		{
			pView->UpdateTilesColors();
		}
		if (NULL != pGLView)
		{
			pGLView->Update3DView();
		}
	}
	// checks cliff tilesets
	if (m_pEditMapPropSheet->m_MapPropMainPage.m_sCliff.GetLength()%4)
	{
		msg += STR_WARNING_MAPPROP04;
	}
	else if (m_pEditMapPropSheet->m_MapPropMainPage.m_sCliff.GetLength() <= 0)
	{
		msg += STR_WARNING_MAPPROP05;
	}
	else if (m_pEditMapPropSheet->m_MapPropMainPage.m_sCliff.GetLength() > W3E_MAX_GROUNDTILES*4)
	{
		msg.Format(STR_WARNING_MAPPROP06, W3E_MAX_GROUNDTILES);
	}
	else
	{
		m_War3Map.mapCliffTiles = m_pEditMapPropSheet->m_MapPropMainPage.m_sCliff;
		if (pMainFrame)
		{pMainFrame->UpdateTilesEditBar(this);}
		/* not needed for cliffs
		if (pView)
		{pView->UpdateTileColors();}
		*/
	}
	m_War3Map.mapW3Info.mapNbMaxPlayers = m_pEditMapPropSheet->m_MapPropMainPage.m_iNbPlayers;
	m_War3Map.mapW3Info.mapEditVer = m_pEditMapPropSheet->m_MapPropMainPage.m_iEditorVer;
	m_War3Map.mapW3Info.mapNbSaves = m_pEditMapPropSheet->m_MapPropMainPage.m_iMapVer;

	if (msg.GetLength() > 0)
	{ // displays a Warning if needed
		W3Z_WARNING(msg);
	}
	  // show water waves on cliff shores
	if (TRUE == m_pEditMapPropSheet->m_MapPropMainPage.m_bWavesCliff)
	{
		m_War3Map.mapW3Info.mapFlags |= W3I_FLG_WAVESCLIFF;
	}
	else
	{
		m_War3Map.mapW3Info.mapFlags &= ~W3I_FLG_WAVESCLIFF;
	}
	  // show water waves on rolling shores
	if (TRUE == m_pEditMapPropSheet->m_MapPropMainPage.m_bWavesRolling)
	{
			m_War3Map.mapW3Info.mapFlags |= W3I_FLG_WAVEROLLING;
	}
	else
	{
			m_War3Map.mapW3Info.mapFlags &= ~W3I_FLG_WAVEROLLING;
	}
	  // melee map
	if (TRUE == m_pEditMapPropSheet->m_MapPropMainPage.m_bMelee)
	{
		m_War3Map.mapW3Info.mapFlags |= W3I_FLG_MELEE;
	}
	else
	{
		m_War3Map.mapW3Info.mapFlags &= ~W3I_FLG_MELEE;
	}
	  // unknown flag 1
	if (m_pEditMapPropSheet->m_MapPropMainPage.m_bUnknown0)
	{
		m_War3Map.mapW3Info.mapFlags |= W3I_FLG_NOTREDUCED;
	}
	else
	{
		m_War3Map.mapW3Info.mapFlags &= ~W3I_FLG_NOTREDUCED;
	}
	  // unknown flag 2
	if (m_pEditMapPropSheet->m_MapPropMainPage.m_bUnknown1)
	{
		m_War3Map.mapW3Info.mapFlags |= W3I_FLG_MENUPROP;
	}
	else
	{
		m_War3Map.mapW3Info.mapFlags &= ~W3I_FLG_MENUPROP;
	}


	// Size Page
	msg = "";
	// if size has changed...
	if ((m_pEditMapPropSheet->m_MapPropSizePage.m_iWidth != m_War3Map.GetMapWidth())
		|| (m_pEditMapPropSheet->m_MapPropSizePage.m_iHeight != m_War3Map.GetMapHeight()))
	{
		// prompts the user the changes can't be undone
		if (IDYES == AfxMessageBox(STR_WARNING_MAPPROP00, MB_YESNO | MB_APPLMODAL | MB_ICONEXCLAMATION))
		{
			m_War3Map.SetMapSize(m_pEditMapPropSheet->m_MapPropSizePage.m_iWidth, m_pEditMapPropSheet->m_MapPropSizePage.m_iHeight);
			if (NULL != pView)
			{
				pView->ChangeBitmapsSize();
			}//if (pView)
			if (NULL != pGLView)
			{
				pGLView->Update3DView();
			}
		}//if (IDYES == AfxMessageBox(STR_WARNING_MAPPROP00, MB_YESNO | MB_APPLMODAL | MB_ICONEXCLAMATION))
	}//if ((m_pEditMapPropSheet->m_MapPropSizePage.m_iWidth != m_War3Map.GetMapWidth()) || (m_pEditMapPropSheet->m_MapPropSizePage.m_iHeight != m_War3Map.GetMapHeight()))
	m_War3Map.mapW3Info.mapPlayableWidth = m_pEditMapPropSheet->m_MapPropSizePage.m_iPlayableWidth;
	m_War3Map.mapW3Info.mapPlayableHeight = m_pEditMapPropSheet->m_MapPropSizePage.m_iPlayableHeight;
	m_War3Map.mapW3Info.mapCameraBounds.fX1 = m_pEditMapPropSheet->m_MapPropSizePage.m_fCBblx;
	m_War3Map.mapW3Info.mapCameraBounds.fY1 = m_pEditMapPropSheet->m_MapPropSizePage.m_fCBbly;
	m_War3Map.mapW3Info.mapCameraBounds.fX2 = m_pEditMapPropSheet->m_MapPropSizePage.m_fCBtrx;
	m_War3Map.mapW3Info.mapCameraBounds.fY2 = m_pEditMapPropSheet->m_MapPropSizePage.m_fCBtry;
	m_War3Map.mapW3Info.mapCameraBounds.fX3 = m_pEditMapPropSheet->m_MapPropSizePage.m_fCBtlx;
	m_War3Map.mapW3Info.mapCameraBounds.fY3 = m_pEditMapPropSheet->m_MapPropSizePage.m_fCBtly;
	m_War3Map.mapW3Info.mapCameraBounds.fX4 = m_pEditMapPropSheet->m_MapPropSizePage.m_fCBbrx;
	m_War3Map.mapW3Info.mapCameraBounds.fY4 = m_pEditMapPropSheet->m_MapPropSizePage.m_fCBbry;

	// Protection Page
	msg = "";
	iPrevProtect = GetProtection();
	if (TRUE == m_pEditMapPropSheet->m_MapPropProtectPage.m_bEnableProtect)
	{
		// enable the choosen protection...
		m_bEnableProtect = TRUE;
		/*
		ComboBox Items Order (m_iTypeSelected):
			0: No protection type set
			1: Unlucky_Footman-Weaaddar Protection
			2: W3Z Map Protection
			3: Unknown protection type
		*/
		if (0 == m_pEditMapPropSheet->m_MapPropProtectPage.m_iTypeSelected)
		{
			SetProtection(W3Z_MAP_PROT_NOTPROTECTED);
			m_bEnableProtect = FALSE; // no protection set so disable protection
		}
		else if (1 == m_pEditMapPropSheet->m_MapPropProtectPage.m_iTypeSelected)
		{
			SetProtection(W3Z_MAP_PROT_UNLUCKY);
		}
		else if (2 == m_pEditMapPropSheet->m_MapPropProtectPage.m_iTypeSelected)
		{
			if (0 == m_pEditMapPropSheet->m_MapPropProtectPage.m_iProtectLvl)
			{
				SetProtection(W3Z_MAP_PROT_W3ZV1L1);
			}
			else if (1 == m_pEditMapPropSheet->m_MapPropProtectPage.m_iProtectLvl)
			{
				SetProtection(W3Z_MAP_PROT_W3ZV1L2);
			}
			else if (2 == m_pEditMapPropSheet->m_MapPropProtectPage.m_iProtectLvl)
			{
				SetProtection(W3Z_MAP_PROT_W3ZV1L3);
			}
		}
		else
		{// protection unknown can't be used, warn the user
			msg += STR_WARNING_MAPPROP07;
		}
		// remove all the author names and replace with the new one
		//+FIXME: check author name
		m_tStrCountListAuthorName.ClearList();
		m_tStrCountListAuthorName.AddString(m_pEditMapPropSheet->m_MapPropProtectPage.m_sOrgAuthorName);
		//+FIXME: check the password
		SetAuthorPassword(m_pEditMapPropSheet->m_MapPropProtectPage.m_sAuthorPassword);
		//+FIXME: check the password
		SetPublicPassword(m_pEditMapPropSheet->m_MapPropProtectPage.m_sPublicPassword);
		m_bAuthorMsg = m_pEditMapPropSheet->m_MapPropProtectPage.m_bAuthorMsg;
		m_bMakeBackup = m_pEditMapPropSheet->m_MapPropProtectPage.m_bMakeBackup;
		m_bUnpRec = m_pEditMapPropSheet->m_MapPropProtectPage.m_bUnpRec;

	}
	else if ((TRUE == IsProtected()) && (TRUE == IsAuthorRegistered()) && ("" != m_strAuthorPassword))
	{// disable protection if a protection was set
		m_bEnableProtect = FALSE;
		SetProtection(W3Z_MAP_PROT_NOTPROTECTED);
		m_bMapProtected = FALSE;
	}
	if (msg.GetLength() > 0)
	{ // displays a Warning if needed
		W3Z_WARNING(msg);
	}
}

void CW3ZMapEditDoc::OnEditMapProperties() 
{
	UpdateMapPropertiesDialog();
	m_pEditMapPropSheet->SetActivePage(m_pEditMapPropSheet->GetPageIndex(&m_pEditMapPropSheet->m_MapPropMainPage));
	if (IDOK == m_pEditMapPropSheet->DoModal())
	{
		UpdateFromMapPropertiesDialog();
	}
}

void CW3ZMapEditDoc::OnEditMapResizeMap() 
{
	UpdateMapPropertiesDialog();
	m_pEditMapPropSheet->SetActivePage(m_pEditMapPropSheet->GetPageIndex(&m_pEditMapPropSheet->m_MapPropSizePage));
	if (IDOK == m_pEditMapPropSheet->DoModal())
	{
		UpdateFromMapPropertiesDialog();
	}
}

void CW3ZMapEditDoc::OnEditMinimap() 
{
	UpdateMapPropertiesDialog();
	m_pEditMapPropSheet->SetActivePage(m_pEditMapPropSheet->GetPageIndex(&m_pEditMapPropSheet->m_MapPropMinimapPage));
	if (IDOK == m_pEditMapPropSheet->DoModal())
	{
		UpdateFromMapPropertiesDialog();
	}
}

void CW3ZMapEditDoc::OnEditMapMenuOptions() 
{
	UpdateMapPropertiesDialog();
	m_pEditMapPropSheet->SetActivePage(m_pEditMapPropSheet->GetPageIndex(&m_pEditMapPropSheet->m_MapPropMenuOptPage));
	if (IDOK == m_pEditMapPropSheet->DoModal())
	{
		UpdateFromMapPropertiesDialog();
	}
}


void CW3ZMapEditDoc::OnEditPlayersProperties() 
{
	UpdateMapPropertiesDialog();
	m_pEditMapPropSheet->SetActivePage(m_pEditMapPropSheet->GetPageIndex(&m_pEditMapPropSheet->m_MapPropPlayersPage));
	if (IDOK == m_pEditMapPropSheet->DoModal())
	{
		UpdateFromMapPropertiesDialog();
	}
}

void CW3ZMapEditDoc::OnEditMapProtection() 
{
	UpdateMapPropertiesDialog();
	m_pEditMapPropSheet->SetActivePage(m_pEditMapPropSheet->GetPageIndex(&m_pEditMapPropSheet->m_MapPropProtectPage));
	if (IDOK == m_pEditMapPropSheet->DoModal())
	{
		UpdateFromMapPropertiesDialog();
	}
}


void CW3ZMapEditDoc::OnViewStatistics() 
{
	CString szValue;
	CTilesetGroup tilesetGroup;
	CW3ZMapEditApp *pTheApp;
	pTheApp = (CW3ZMapEditApp *)AfxGetApp();

	CMapStatisticsDialog statDialog;
	// stats title
	statDialog.m_sStats = STR_STAT_TITLE;
	// Part I
	//--------
	statDialog.m_sStats += STR_STAT_STATS1;
	// starts with the filename
	statDialog.m_sStats += STR_STAT_FILENAME;
	statDialog.m_sStats += GetTitle() + WIN_CR;
	// map name
	statDialog.m_sStats += STR_STAT_MAPNAME;
	statDialog.m_sStats += m_War3Map.mapTrigStringsTable.GetTrigStringValue(m_War3Map.mapW3Info.mapNameTID) + WIN_CR;
	// map version
	statDialog.m_sStats += STR_STAT_MAPVER;
	szValue.Format("%i", m_War3Map.mapW3Info.mapNbSaves);
	statDialog.m_sStats += szValue + WIN_CR;
	// map date
	statDialog.m_sStats += STR_STAT_MAPDATE;
	statDialog.m_sStats += WIN_CR;
	// map author
	statDialog.m_sStats += STR_STAT_MAPAUTHOR;
	//use the watermark if there is one
	if (TRUE == IsProtected())
	{// get the watermark data
		statDialog.m_sStats += GetAuthorName();
		statDialog.m_sStats += STR_STAT_MAPAUTHORREL0;
		szValue.Format("%.1f", GetAuthorNameRatio());		 
		statDialog.m_sStats += szValue + STR_STAT_MAPAUTHORREL1;
		statDialog.m_sStats += WIN_CR;
	}
	else
	{// not protected, use the standard author name
		statDialog.m_sStats += m_War3Map.mapTrigStringsTable.GetTrigStringValue(m_War3Map.mapW3Info.mapAuthorTID) + WIN_CR;
	}
	// gets the protection
	statDialog.m_sStats += STR_STAT_PROTECTION;
	if (W3I_FLG_PROTECTED & m_War3Map.mapW3Info.mapFlags)
	{statDialog.m_sStats += STR_MAP_PROT_PROTECTED;}
	else
	{statDialog.m_sStats += STR_MAP_PROT_NOTPROTECTED;}
	statDialog.m_sStats += WIN_CR;
	// main tileset
	statDialog.m_sStats += STR_STAT_MAINTILESET;
	tilesetGroup = pTheApp->m_AppSettings.GetTilesetGroup(m_War3Map.mapMainTileset);
	szValue.Format("%s (%c)", tilesetGroup.groupName, m_War3Map.mapMainTileset);
	statDialog.m_sStats += szValue + WIN_CR;
	// map description
	statDialog.m_sStats += STR_STAT_MAPDESC;
	statDialog.m_sStats += m_War3Map.mapTrigStringsTable.GetTrigStringValue(m_War3Map.mapW3Info.mapDescTID) + WIN_CR;
	// additionnal info
	statDialog.m_sStats += STR_STAT_MAPDESCEX;
	statDialog.m_sStats +=  WIN_CR;
	// playable with
	statDialog.m_sStats += STR_STAT_MAPPAREA;
	szValue.Format("%ix%i", m_War3Map.mapW3Info.mapPlayableWidth, m_War3Map.mapW3Info.mapPlayableHeight);
	statDialog.m_sStats += szValue + WIN_CR;
	// suggested players
	statDialog.m_sStats += STR_STAT_SUGPLAYERS;
	statDialog.m_sStats += m_War3Map.mapTrigStringsTable.GetTrigStringValue(m_War3Map.mapW3Info.mapSugPlyrTID) + WIN_CR;
	// max players
	statDialog.m_sStats += STR_STAT_MAXPLAYERS;
	szValue.Format("%i", m_War3Map.mapW3Info.mapNbMaxPlayers);
	statDialog.m_sStats += szValue + WIN_CR;
	// max forces
	statDialog.m_sStats += STR_STAT_MAXFORCES;
	szValue.Format("%i", m_War3Map.mapW3Info.mapNbMaxForces);
	statDialog.m_sStats += szValue + WIN_CR;
	// map type
	statDialog.m_sStats += STR_STAT_MAPTYPE;
	if (W3I_FLG_MELEE & m_War3Map.mapW3Info.mapFlags)
	{statDialog.m_sStats += STR_STAT_MELEEMAP;}
	else
	{statDialog.m_sStats += STR_STAT_NONMELEEMAP;}

	// Part II
	//---------
	statDialog.m_sStats += STR_STAT_STATS2;
	statDialog.m_sStats += STR_STAT_CINEMATICS;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_QUESTS;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_NEXTMAPS;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_CUSTUNITS;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_CUSTABIL;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_CUSTSPELLS;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_CUSTUPGRADES;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_CUSTTECHTREE;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_CUSTITEMS;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_CUSTFORCES;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_NEWSOUNDS;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_NEWMUSIC;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_NEWMODELS;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_NEWTILES;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_PREVIEWMAP;
	statDialog.m_sStats += WIN_CR;

	// Part III
	//----------
	statDialog.m_sStats += STR_STAT_STATS3;
	statDialog.m_sStats += STR_STAT_NBITEMS;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_NBUNITS;
//	szValue.Format("%i", m_War3Map.GetNbUnits());
	statDialog.m_sStats += /*szValue + */WIN_CR;
	statDialog.m_sStats += STR_STAT_NBPSUNITS;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_NBAGUNITS;
	statDialog.m_sStats += WIN_CR;
//	statDialog.m_sStats += STR_STAT_NBPLUNITS;
//	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_PLUNITS;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_STARTMINDIST;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_STARTMAXDIST;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_NBDOODADS;
	szValue.Format("%i", m_War3Map.GetNbDoodads());
	statDialog.m_sStats += szValue + WIN_CR;
	statDialog.m_sStats += STR_STAT_NBUNITDOO;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_NBTREE;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_LUMBERAMOUNT;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_AVGLUMBER;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_NBGOLDMINES;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_GOLDAMOUNT;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_AVGGOLD;
	statDialog.m_sStats += WIN_CR;

	// Part IV
	//---------
	statDialog.m_sStats += STR_STAT_STATS4;
	// map size
	statDialog.m_sStats += STR_STAT_MAPSIZE;
	szValue.Format("%ix%i", m_War3Map.GetMapWidth(), m_War3Map.GetMapHeight());
	statDialog.m_sStats += szValue + WIN_CR;
	statDialog.m_sStats += STR_STAT_POLYCOUNT;
	statDialog.m_sStats += WIN_CR;
	// tiles
	statDialog.m_sStats += STR_STAT_MAPTILES;
	statDialog.m_sStats += m_War3Map.mapGroundTiles + WIN_CR;
	statDialog.m_sStats += STR_STAT_MAPCLIFFS;
	statDialog.m_sStats += m_War3Map.mapCliffTiles + WIN_CR;
	statDialog.m_sStats += STR_STAT_AVGHEIGHT;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_MAXHEIGHT;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_MINHEIGHT;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_BUILDCELLS;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_PLAYABLECELLS;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_WATERCELLS;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_BLIGHTCELLS;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_BOUNDARYCELLS;
	statDialog.m_sStats += WIN_CR;

	statDialog.m_sStats += STR_STAT_STATS5;
	statDialog.m_sStats += STR_STAT_NBVARS;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_NBTRIGGERS;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_NBFUNCTIONS;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_NBEVENTS;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_NBREGIONS;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_JASSLIGNES;
	statDialog.m_sStats += WIN_CR;

	statDialog.m_sStats += STR_STAT_STATS6;
	statDialog.m_sStats += STR_STAT_VEREDITOR;
	szValue.Format("%i", m_War3Map.mapW3Info.mapEditVer);
	statDialog.m_sStats += szValue + WIN_CR;
	statDialog.m_sStats += STR_STAT_VERW3I;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_VERW3E;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_VERDOO;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_VERUNITSDOO;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_VERW3U;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_VERWPM;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_VERMAPBLP;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_VERIMP;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_VERW3S;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_VERW3R;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_VERW3C;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_VERWTG;
	statDialog.m_sStats += WIN_CR;
	statDialog.m_sStats += STR_STAT_VERMMP;
	statDialog.m_sStats += WIN_CR;
	
	statDialog.DoModal();
}


/*
IsProtected
***********
Says if yes or no a map is protected.

In:
  nothing

Out:
  nothing

Return:
  BOOL: TRUE if the map is protected, FALSE otherwise.
*/
BOOL CW3ZMapEditDoc::IsProtected()
{
	return m_bMapProtected;
}


/*
RegisterAuthorPassword
**********************
Check if a hash matches the author password hash.

In:
 CString strPassword: password to check

Out:
 nothing

Return:
 BOOL: TRUE if the password matches the author password hash, FALSE otherwise.

*/
BOOL CW3ZMapEditDoc::RegisterAuthorPassword(const CString &strPassword)
{
	BOOL bRet = FALSE;
	unsigned int seed1 = 0x7FED7FED, seed2 = 0xEEEEEEEE;
	int ch, i;

	//gets the hash
	for (i = 0; i < strPassword.GetLength();i++)
	{ 
		ch = strPassword[i];
		seed1 = W3Z_CRYPTTABLE[ch] ^ (seed1 + seed2);
		seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3; 
	}
	
	// checks it
	if (m_tAuthorHashCountList.GetBestCountHash() == seed1)
	{
		m_bAuthorRegistered = TRUE;
		m_strAuthorPassword = strPassword;
		bRet = TRUE;
	}
	else
	{
		bRet = FALSE;
	}

	if (TRUE == bRet)
	{ // initialise map properties fields access
		m_pEditMapPropSheet->EnableFeatures(W3Z_PROP_ALLNOREGISTER);
		// and remove the level 3 protection if it was set
		if (W3Z_MAP_PROT_W3ZV1L3 == GetProtection())
		{
			UnprotectW3Zv1Level3();
		}
	}
	return bRet;
}


/*
RegiterPublicPassword
*********************
Check if a hash matches the public password hash.

In:
 unsigned int v_uHash: password to check

Out:
 nothing

Return:
 BOOL: TRUE if the password matches the public password hash, FALSE otherwise.

*/
BOOL CW3ZMapEditDoc::RegisterPublicPassword(const CString &strPassword)
{
	BOOL bRet = FALSE;
	unsigned int seed1 = 0x7FED7FED, seed2 = 0xEEEEEEEE;
	int ch, i;

	//gets the hash
	for (i = 0; i < strPassword.GetLength();i++)
	{ 
		ch = strPassword[i];
		seed1 = W3Z_CRYPTTABLE[ch] ^ (seed1 + seed2);
		seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3; 
	}
	
	// checks it
	if (m_tPublicHashCountList.GetBestCountHash() == seed1)
	{
		m_bPublicRegistered = TRUE;
		m_strPublicPassword = strPassword;
		bRet = TRUE;
	}
	else
	{
		bRet = FALSE;
	}

	if (TRUE == bRet)
	{ // initialise map properties fields access
		m_pEditMapPropSheet->EnableFeatures(W3Z_PROP_ALLNOPROTECT);
	}
	return bRet;
}


/*
CryptString
***********
Crypts a string. If iLength is > 0, the source string is truncated to iLength and
the returned string as an exact length of iLength. If iLength is < 0, the returned
string has the same length as the input one.

In:
 CString &strTheString: the source string to be crypted
 int iLength: the exact length of the result

Out:
 CString &strTheString: the crypted string

Return:
 nothing

Note: when iLength is set and the given string is shorter, the string is enlarged
 with and ending '\0' null char followed by random chars to feet iLength chars.

*/
void CW3ZMapEditDoc::CryptString(CString &strTheString, int iLength)
{
	int i, iMax;
	CString strCryptedString = "";
	srand ((unsigned)time(NULL));

	//if the length has been set, truncate the string
	if (iLength > 0)
	{
		strTheString = strTheString.Left(iLength);
		iMax = iLength;
	}
	else
	{
		iMax = strTheString.GetLength();
	}
	
	// crypts
	for (i = 0; i < iMax; i++)
	{
		if (i < strTheString.GetLength())
		{
			strCryptedString += (char)(strTheString[i] ^ STR_CRYPT_KEY[i]);
		}
		else if (i == strTheString.GetLength())
		{// crypts the null terminating char
			strCryptedString +=  (char)('\0' ^ STR_CRYPT_KEY[i]);
		}
		else
		{ // fill the rest with random chars
			strCryptedString += (char)(rand() % 256);
		}
	}
	strTheString = strCryptedString;
}


/*
DecryptAuthorName
*****************
Decrypts a string. If a '\0' null char is found in the decrypted string, the string is
 truncated at the '\0'.

In:
 CString &strCryptedString: the crypted string

Out:
 CString &strCryptedString: the decrypted string

Return;
 nothing

*/
void CW3ZMapEditDoc::DecryptString(CString &strCryptedString)
{
	int i;
	CString strDecryptedString = "";

	for (i = 0; i < strCryptedString.GetLength(); i++)
	{
		strDecryptedString += (char)(strCryptedString[i] ^ STR_CRYPT_KEY[i]);
	}

	strCryptedString = strDecryptedString.Left(strDecryptedString.Find('\0'));
}


/*
CryptData
*********
Crypts a buffer using a password.

In:
 const CString &strPassword: password to use for encryption
 byte *pbyData: pointer to the data to crypt
 int iDataSize: size of the data to crypt

Out:
 byte *pbyData: points the crypted data

Return:
 nothing

*/
void CW3ZMapEditDoc::CryptData(const CString &strPassword, byte *pbyData, int iDataSize)
{
	unsigned int iData = 0, iPassword = 0;
	unsigned __int64 iKey = 0x1234567890ABCDEF, iKey2 = 0xF1D3B59786A4C2E0;
	byte byData = 0;

	for (iPassword = 0; iPassword < (unsigned)strPassword.GetLength(); iPassword++)
	{
		byData = strPassword.GetAt(iPassword);
		iKey = W3Z_CRYPTTABLE[byData] ^ (iKey + iKey2);
		iKey2 = byData + iKey + iKey2 + (iKey2 << 5) + 3; 
	}

	iPassword = 0;
	for (iData = 0; iData < (unsigned)iDataSize; iData++)
	{
		iKey = (iKey << 1) - strPassword.GetAt(iPassword) + W3Z_CRYPTTABLE[iData%64 + iKey%128 + iPassword%64];
		byData = (byte) ((iKey%256) ^ ((iKey/0x100)%256) ^ ((iKey/0x10000)%((iKey/0x1000000)%0x10000)));

		pbyData[iData] = (byte)(pbyData[iData] + byData);
		iPassword++;
		if (iPassword >= (unsigned)strPassword.GetLength())
		{
			iPassword = 0;
		}
	}
}


/*
DecryptData
***********
Decrypts a buffer using a password.

In:
 const CString &strPassword: password to use for decryption
 byte *pbyData: pointer to the data to decrypt
 int iDataSize: size of the data to decrypt

Out:
 byte *pbyData: points the decrypted data

Return:
 nothing

*/
void CW3ZMapEditDoc::DecryptData(const CString &strPassword, byte *pbyData, int iDataSize)
{
	unsigned int iData = 0, iPassword = 0;
	unsigned __int64 iKey = 0x1234567890ABCDEF, iKey2 = 0xF1D3B59786A4C2E0;
	byte byData = 0;

	for (iPassword = 0; iPassword < (unsigned)strPassword.GetLength(); iPassword++)
	{
		byData = strPassword.GetAt(iPassword);
		iKey = W3Z_CRYPTTABLE[byData] ^ (iKey + iKey2);
		iKey2 = byData + iKey + iKey2 + (iKey2 << 5) + 3; 
	}

	iPassword = 0;
	for (iData = 0; iData < (unsigned)iDataSize; iData++)
	{
		iKey = (iKey << 1) - strPassword.GetAt(iPassword) + W3Z_CRYPTTABLE[iData%64 + iKey%128 + iPassword%64];
		byData = (byte) ((iKey%256) ^ ((iKey/0x100)%256) ^ ((iKey/0x10000)%((iKey/0x1000000)%0x10000)));

		pbyData[iData] = (byte)(pbyData[iData] - byData);
		iPassword++;
		if (iPassword >= (unsigned)strPassword.GetLength())
		{
			iPassword = 0;
		}
	}
}



/*
AddAuthorName
*************
Adds an author name to the author name list.

In:
 CString strAuthorName: author name to add

Out:
 nothing

Return:
 nothing

*/
void CW3ZMapEditDoc::AddAuthorName(const CString &strAuthorName)
{
	m_tStrCountListAuthorName.AddString(strAuthorName);
}



/*
GetAuthorName
*************
Returns the most common author name of the author name list.

In:
 nothing

Out:
 nothing

Return:
 CString: the most common author name of the list

*/
CString CW3ZMapEditDoc::GetAuthorName() const
{
	return m_tStrCountListAuthorName.GetBestCountStr();
}



/*
GetAuthorNameReliability
************************
Returns the % reliability of the author name found.
The returned number is between 0 and 1:
1: the author name is 100% verified
0.5: the author name has 1 chance on 2 to be the real one
0: the author name is unknown

In:
 nothing

Out:
 nothing

Return:
 double: a number between 0 and 1 corresponding to the reliability of the author name.

*/
double CW3ZMapEditDoc::GetAuthorNameRatio()
{
	return m_tStrCountListAuthorName.GetBestCountRatio();
}


/*
GetAshedAuthorPassword
**********************
Returns the hashed author password.

In:
 nothing

Out:
 nothing

Return:
 unsigned int: the hash of the author password

*/
unsigned int CW3ZMapEditDoc::GetAshedAuthorPassword()
{
	unsigned int seed1 = 0x7FED7FED, seed2 = 0xEEEEEEEE;
	int ch, i;

	for (i = 0; i < m_strAuthorPassword.GetLength();i++)
	{ 
		ch = m_strAuthorPassword[i];
		seed1 = W3Z_CRYPTTABLE[ch] ^ (seed1 + seed2);
		seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3; 
	}
	return seed1; 
}


/*
GetAshedPublicPassword
**********************
Returns the hashed public password.

In:
 nothing

Out:
 nothing

Return:
 unsigned int: the hash of the author password

*/
unsigned int CW3ZMapEditDoc::GetAshedPublicPassword()
{
	unsigned int seed1 = 0x7FED7FED, seed2 = 0xEEEEEEEE;
	int ch, i;

	for (i = 0; i < m_strPublicPassword.GetLength();i++)
	{ 
		ch = m_strPublicPassword[i];
		seed1 = W3Z_CRYPTTABLE[ch] ^ (seed1 + seed2);
		seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3; 
	}
	return seed1; 
}


/*
GetProtection
*************
Returns the protection type.

In:
  nothing

Out:
  nothing

Return:
  int: protection type used.

*/
int CW3ZMapEditDoc::GetProtection()
{
	return m_iProtectionType;
}


/*
SetProtection
*************
Set the type of protection used to save the map.

In:
  int iProtType: protection type.

Out:
  nothing

Return:
  nothing

*/
void CW3ZMapEditDoc::SetProtection(int iProtectType)
{
	m_iProtectionType = iProtectType;
}


/*
IsAuthorRegistered
******************
Says if the author has been registered or not.

In:
 nothing

Out:
 nothing

Return:
 BOOL: TRUE if the author password has been checked and OK

*/
BOOL CW3ZMapEditDoc::IsAuthorRegistered()
{
	return m_bAuthorRegistered;
}

/*
IsPublicRegistered
******************
Says if the public user has been registered or not.

In:
 nothing

Out:
 nothing

Return:
 BOOL: TRUE if the public password has been checked and OK

*/
BOOL CW3ZMapEditDoc::IsPublicRegistered()
{
	return m_bPublicRegistered;
}


BOOL CW3ZMapEditDoc::SetAuthorPassword(const CString &strPassword)
{
	BOOL bRet = FALSE;
	if (TRUE == IsProtected())
	{
		if (TRUE == IsAuthorRegistered())
		{
			m_strAuthorPassword = strPassword.Left(W3Z_MAX_PASS_LENGTH);
			bRet = TRUE;
		}
	}
	else
	{
		m_strAuthorPassword = strPassword.Left(W3Z_MAX_PASS_LENGTH);
		bRet = TRUE;
	}
	//+FIXME: invalid password
	return bRet;
}

BOOL CW3ZMapEditDoc::SetPublicPassword(const CString &strPassword)
{
	BOOL bRet = FALSE;
	if (TRUE == IsProtected())
	{
		if (TRUE == IsAuthorRegistered())
		{
			m_strPublicPassword = strPassword.Left(W3Z_MAX_PASS_LENGTH);
			bRet = TRUE;
		}
	}
	else
	{
		m_strPublicPassword = strPassword.Left(W3Z_MAX_PASS_LENGTH);
		bRet = TRUE;
	}
	//+FIXME: invalid password
	return bRet;
}


/*
SetProtected
************
Set the protection flag. The protection flags says that the map read had at least one
file protected and the protection may require a password to save modifications.
The protection flags is ONLY set when a map is loaded. Never while a map is open and was
not protected before being opened.
The flag is only cleared when the author registered (by the UpdateFromMapPropertiesDialog).

In:
 nothing

Out:
 nothing

Return:
 nothing

*/
void CW3ZMapEditDoc::SetProtected()
{
	m_bMapProtected = TRUE; // sets the protection flag
	m_bEnableProtect = TRUE; // says that the map is protected in the map property dialog
}



CString CW3ZMapEditDoc::GetProtectionName() const
{
	CString strRet;
	switch (m_iProtectionType)
	{
	case W3Z_MAP_PROT_NOTPROTECTED:
		{
			strRet = STR_MAP_PROT_NOTPROTECTED;
			break;
		}
	case W3Z_MAP_PROT_UNLUCKY:
		{
			strRet = STR_MAP_PROT_UNLUCKY;
			break;
		}
	case W3Z_MAP_PROT_W3ZV1L1:
		{
			strRet = STR_MAP_PROT_W3ZV1L1;
			break;
		}
	case W3Z_MAP_PROT_W3ZV1L2:
		{
			strRet = STR_MAP_PROT_W3ZV1L2;
			break;
		}
	case W3Z_MAP_PROT_W3ZV1L3:
		{
			strRet = STR_MAP_PROT_W3ZV1L3;
			break;
		}
	default:
		{
			strRet = STR_MAP_PROT_UNKNOWN;
		}
	}
	return strRet;
}

void CW3ZMapEditDoc::InitProtectDataStructs()
{
	CString strAuthorName;
	byte *pbyProtectData;
	int i;

	// gets author name
	strAuthorName = GetAuthorName();
	// set author name length and crypt it 
	CryptString(strAuthorName, W3Z_MAX_NAME_LENGTH);
	// initialize the structure
	if (GetProtection() == W3Z_MAP_PROT_W3ZV1L3)
	{
		m_tW3ZProtectData01.iProtectID = m_tW3ZProtectData02.iProtectID = W3Z_W3E_PROT_STARTSEQ103; // sets the ID
	}
	else if (GetProtection() == W3Z_MAP_PROT_W3ZV1L2)
	{
		m_tW3ZProtectData01.iProtectID =  m_tW3ZProtectData02.iProtectID = W3Z_W3E_PROT_STARTSEQ102; // sets the ID
	}
	else/* if (GetProtection() == W3Z_MAP_PROT_W3ZV1L1)*/
	{
		m_tW3ZProtectData01.iProtectID =  m_tW3ZProtectData02.iProtectID = W3Z_W3E_PROT_STARTSEQ101; // sets the ID
	}
	strncpy(m_tW3ZProtectData01.acAuthName, strAuthorName, W3Z_MAX_NAME_LENGTH); // copy the crypted author name
	m_tW3ZProtectData01.uHashAPass = m_tW3ZProtectData02.uHashAPass = GetAshedAuthorPassword(); // gets author password hashe
	m_tW3ZProtectData01.uHashPPass = m_tW3ZProtectData02.uHashPPass = GetAshedPublicPassword(); // gets public password hashe
	
	// computes and sets the new checksums...
	//...for structure 1
	m_tW3ZProtectData01.iChecksum = 0;
	pbyProtectData = (byte *)(&m_tW3ZProtectData01); // inits the protect data pointer
	// -4 to not include the checksum itself!
	for (i = 0;i < W3ZPROTECTDATA01_LENGTH - 4; i++)
	{
		m_tW3ZProtectData01.iChecksum = (m_tW3ZProtectData01.iChecksum << 1) + pbyProtectData[i];
	}
	//...for structure 2
	m_tW3ZProtectData02.iChecksum = 0;
	pbyProtectData = (byte *)(&m_tW3ZProtectData02); // inits the protect data pointer
	// -4 to not include the checksum itself!
	for (i = 0;i < W3ZPROTECTDATA02_LENGTH - 4; i++)
	{
		m_tW3ZProtectData02.iChecksum = (m_tW3ZProtectData02.iChecksum << 1) + pbyProtectData[i];
	}
}


BOOL CW3ZMapEditDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	BOOL bRet;
	// makes a quick unprotected backup if needed
	if (TRUE == m_bMakeBackup)
	{
		QuickBackup();
	}

	m_bSaved = TRUE;
	bRet = CDocument::OnSaveDocument(lpszPathName);
	if (FALSE == m_bSaved)
	{
		bRet = FALSE;
	}
	return bRet;
}


/*
QuickBackup
***********
This function performs an unprotected backup of the current map.
It temporary unprotect the map and put the protection back when finished.

*/
BOOL CW3ZMapEditDoc::QuickBackup()
{
	BOOL bRet = FALSE;
	// check if map is protected
	if (TRUE == IsReadOnly())
	{
		bRet = FALSE;
	}
	else
	{// map is allowed to be backuped
		int i, j, iProtectType;
		BOOL bProtectFlag;
		CFile fiBackup;
		CString strFilename = GetPathName();

		// changes the file extension
		j = strFilename.ReverseFind('\\');
		i = strFilename.ReverseFind('.');
		if ((i > 0) && (i > j))
		{strFilename = strFilename.Left(i);}
		strFilename += ".bak";

		// saves the protection status
		iProtectType = m_iProtectionType;
		bProtectFlag = m_bMapProtected;

		try
		{
			if (fiBackup.Open(strFilename, CFile::modeCreate | CFile::modeReadWrite))
			{// file opened successfully
				CArchive ar(&fiBackup, CArchive::store, 65536);
				//temporary remove any protection
				m_iProtectionType = W3Z_MAP_PROT_NOTPROTECTED;
				m_bMapProtected = FALSE;
				Serialize(ar);
				ar.Close();
				fiBackup.Close();
			}
			bRet = TRUE;
		}
		catch (CFileException* e)
		{
			bRet = FALSE;
			delete e;
		}
		catch (CMemoryException* e)
		{
			bRet = FALSE;
			delete e;
		}
		catch (CArchiveException* e)
		{
			bRet = FALSE;
			delete e;
		}

		//restores the protection status
		m_bMapProtected = bProtectFlag;
		m_iProtectionType = iProtectType;
	}
	return bRet;
}

/*
OnFileQuickBackup
*****************
Does a quick backup of the current document calling the QuickBackup() function.

*/
void CW3ZMapEditDoc::OnFileQuickBackup() 
{
	QuickBackup();
}

/*
IsReadOnly
**********
Returns TRUE if a map is protected and the author is not registered.
If the map is protected with W3Z 1.0 level 1, it retruns FALSE.

In:
 nothing

Out:
 nothing

Return:
 BOOL: TRUE if the map is read only

*/
BOOL CW3ZMapEditDoc::IsReadOnly()
{
	BOOL bRet;

	bRet = FALSE;
	if ((TRUE == IsProtected())
		&& ((FALSE == IsAuthorRegistered()) && (W3Z_MAP_PROT_W3ZV1L1 != GetProtection())))
	{
		bRet = TRUE;
	}

	return bRet;
}


/*
IsImportedFile
**************
Says if a file is or not in the standard W3M file list.
If the file is not in the list, it is considered as an imorted file.

In:
 CString strFilename: name of the file to check

Out:
 nothing

Return:
 BOOL: TRUE if the file is not in the standard

*/
BOOL CW3ZMapEditDoc::IsImportedFile(const CString &strFilename)
{
	BOOL bRet;
	bRet = TRUE;

	if ((STR_W3M_LISTFILE == strFilename)
		||(STR_W3M_SIGNATURE == strFilename)
		||(STR_W3M_ATTRIBUTES == strFilename)
		||(STR_W3M_WTS == strFilename)
		||(STR_W3M_W3E == strFilename)
		||(STR_W3M_W3I == strFilename)
		||(STR_W3M_WPM == strFilename)
		||(STR_W3M_PATHTGA == strFilename)
		||(STR_W3M_DOO == strFilename)
		||(STR_W3M_UNITSDOO == strFilename)
		||(STR_W3M_JASS == strFilename)
		||(STR_W3M_WTG == strFilename)
		||(STR_W3M_WCT == strFilename)
		||(STR_W3M_SHD == strFilename)
		||(STR_W3M_MAPBLP == strFilename)
		||(STR_W3M_MAPB00 == strFilename)
		||(STR_W3M_MAPTGA == strFilename)
		||(STR_W3M_PREVIEWTGA == strFilename)
		||(STR_W3M_MMP == strFilename)
		||(STR_W3M_W3R == strFilename)
		||(STR_W3M_W3C == strFilename)
		||(STR_W3M_W3U == strFilename)
		||(STR_W3M_W3S == strFilename)
		||(STR_W3M_IMP == strFilename))
	{
		bRet = FALSE;
	}

	return bRet;
}

/*
SaveModified
************
Called by the framework before a modified document is to be closed.
It returns TRUE if the document has been saved or was opened as "ReadOnly".

In:
 nothing

Out:
 nothing

Return:
 BOOL: TRUE is the document can be close safely.

*/
BOOL CW3ZMapEditDoc::SaveModified()
{
	if (TRUE == IsReadOnly())
	{
		return TRUE;
	}
	else
	{
		return CDocument::SaveModified();
	}
}


/*
UnprotectW3Zv1Level3
********************
Unprotect the 4 crypted files when a W3Z protection has been set with unprotect record
and the author is registered with the correct password.

In:
 nothing

Out:
 nothing

Return:
 nothing

*/
void CW3ZMapEditDoc::UnprotectW3Zv1Level3()
{
	CByteArray baBuffer;
	CString str, EmbFileName;
	int iSize, i, j, iNbFiles;
	CWar3StrID strID;
	byte byRand;

	try
	{
		// check file
		if (0 == m_fiUNP.GetLength())
		{ // empty file or no unprotect record found in the map
			return; //+zep 
			throw CWar3FileException(CWar3FileException::missingData);
		}

		//check file ID
		m_fiUNP.ReadStrID(strID);
		if (strID != W3Z_UNP_ID)
		{// bad ID
			return; //+zep 
			throw CWar3FileException(CWar3FileException::invalidFileID);
		}

		// check version
		m_fiUNP.ReadInt(i);
		if (W3Z_UNP_VER != i)
		{ // bad version
			return; //+zep 
			throw CWar3FileException(CWar3FileException::unknownVersion);
		}

		//read number of files
		m_fiUNP.ReadInt(iNbFiles);
		for (i=0; i < iNbFiles; i++)
		{
			//read file name
			m_fiUNP.ReadString(str);
			//read data size
			m_fiUNP.ReadInt(iSize);
			if ((m_fiUNP.GetLength() - m_fiUNP.GetPosition()) < (unsigned)iSize)
			{// invalid file record size
				return; //+zep 
				throw CWar3FileException(CWar3FileException::erroneousData);
			}
			//get the data
			baBuffer.SetSize(iSize);
			m_fiUNP.Read(baBuffer.GetData(), iSize);
			// decrypt data
			DecryptData(m_strAuthorPassword, baBuffer.GetData(), baBuffer.GetSize());
			//remove the pre-encryption
			byRand = baBuffer.GetAt(iSize-1);
			iSize--;
			for (j = 0; j < iSize; j++)
			{
				baBuffer.SetAt(j, (byte)(baBuffer.GetAt(j)^byRand));
			}
			if (STR_W3M_WTG == str)
			{
				m_fiWTG.SetLength(0);
				m_fiWTG.SeekToBegin();
				m_fiWTG.Write(baBuffer.GetData(), iSize);
				m_fiWTG.SeekToBegin();
				SerializeWTG(FALSE); //loads WTG data
			}
			else if (STR_W3M_WCT == str)
			{
				m_fiWCT.SetLength(0);
				m_fiWCT.SeekToBegin();
				m_fiWCT.Write(baBuffer.GetData(), iSize);
				m_fiWCT.SeekToBegin();
				SerializeWCT(FALSE); //loads WCT data
			}
			else if (STR_W3M_W3R == str)
			{
				m_fiW3R.SetLength(0);
				m_fiW3R.SeekToBegin();
				m_fiW3R.Write(baBuffer.GetData(), iSize);
				m_fiW3R.SeekToBegin();
				SerializeW3R(FALSE); //loads W3R data
			}
			else if (STR_W3M_W3C == str)
			{
				m_fiW3C.SetLength(0);
				m_fiW3C.SeekToBegin();
				m_fiW3C.Write(baBuffer.GetData(), iSize);
				m_fiW3C.SeekToBegin();
				SerializeW3C(FALSE); //loads W3C data
			}
			else
			{
				// unknown file
			}
		}
	}
	catch (CWar3FileException *ex)
	{
		delete ex;
	}
	catch (CFileException *ex)
	{
		ex->Delete();
	}
	catch (CMemoryException *ex)
	{
		ex->Delete();
	}
}

/*
OnBtnPathReset
**************
Resets path data of current document and use map data to set up new path.

*/
void CW3ZMapEditDoc::OnBtnPathReset() 
{
	byte* pBytemap;
	byte oldflag;
	int i, x, y;

	pBytemap = m_War3Map.GetPathMapPtr();
	if (NULL == pBytemap)
	{
		return;
	}

	//undo...
	OpenUndoObj();
	for (y = 0; y < m_War3Map.GetPathMapHeight(); y++)
	for (x = 0; x < m_War3Map.GetPathMapWidth(); x++)
	{
		i = (((m_War3Map.GetPathMapHeight() - y - 1)*m_War3Map.GetPathMapWidth()) + x);
		oldflag = pBytemap[i];
		AddPathToUndoObj(oldflag, x, y); 
	}
	CloseUndoObj();
	//...undo

	m_War3Map.ResetPathing();
	POSITION pos = GetFirstViewPosition();
	CW3ZMapEditView* pView = (CW3ZMapEditView *)GetNextView(pos);
	pView->InvalidateBitmap();
	pView->Invalidate();
	CW3ZMapOpenGLView* pGLView = NULL;
	if (pos != NULL)
	{
		pGLView = (CW3ZMapOpenGLView*)GetNextView(pos);
		if (NULL != pGLView)
		{
			pGLView->Update3DView();
		}
	}
}

C3DChildFrame* CW3ZMapEditDoc::Get3DChildFrame(void)
{
	C3DChildFrame* p3DChildFrame = NULL;
	POSITION pos = GetFirstViewPosition();
	CW3ZMapEditView* pView = (CW3ZMapEditView *)GetNextView(pos);
	CW3ZMapOpenGLView* pGLView = NULL;
	if (pos != NULL)
	{
		pGLView = (CW3ZMapOpenGLView*)GetNextView(pos);
		if (NULL != pGLView)
		{
			p3DChildFrame = (C3DChildFrame*) pGLView->GetParentFrame();
		}
	}
	return p3DChildFrame;
}


void CW3ZMapEditDoc::RunPlugin(CString strPluginName)
{
	CW3ZMapEditApp *pTheApp;
	pTheApp = (CW3ZMapEditApp *)AfxGetApp();

	pTheApp->m_AppSettings.CheckMenuPlugin(m_fiW3E.GetFilePath(), STR_W3M_W3E, strPluginName);
	pTheApp->m_AppSettings.CheckMenuPlugin(m_fiWTS.GetFilePath(), STR_W3M_WTS, strPluginName);
	pTheApp->m_AppSettings.CheckMenuPlugin(m_fiW3I.GetFilePath(), STR_W3M_W3I, strPluginName);
	pTheApp->m_AppSettings.CheckMenuPlugin(m_fiWTG.GetFilePath(), STR_W3M_WTG, strPluginName);
	pTheApp->m_AppSettings.CheckMenuPlugin(m_fiWCT.GetFilePath(), STR_W3M_WCT, strPluginName);
	pTheApp->m_AppSettings.CheckMenuPlugin(m_fiJASS.GetFilePath(), STR_W3M_JASS, strPluginName);
	pTheApp->m_AppSettings.CheckMenuPlugin(m_fiSHD.GetFilePath(), STR_W3M_SHD, strPluginName);
	pTheApp->m_AppSettings.CheckMenuPlugin(m_fiPreviewTGA.GetFilePath(), STR_W3M_PREVIEWTGA, strPluginName);
	pTheApp->m_AppSettings.CheckMenuPlugin(m_fiMapTGA.GetFilePath(), STR_W3M_MAPTGA, strPluginName);
	pTheApp->m_AppSettings.CheckMenuPlugin(m_fiMapBLP.GetFilePath(), STR_W3M_MAPBLP, strPluginName);
	pTheApp->m_AppSettings.CheckMenuPlugin(m_fiMapB00.GetFilePath(), STR_W3M_MAPB00, strPluginName);
	pTheApp->m_AppSettings.CheckMenuPlugin(m_fiMMP.GetFilePath(), STR_W3M_MMP, strPluginName);
	pTheApp->m_AppSettings.CheckMenuPlugin(m_fiPathTGA.GetFilePath(), STR_W3M_PATHTGA, strPluginName);
	pTheApp->m_AppSettings.CheckMenuPlugin(m_fiWPM.GetFilePath(), STR_W3M_WPM, strPluginName);
	pTheApp->m_AppSettings.CheckMenuPlugin(m_fiDOO.GetFilePath(), STR_W3M_DOO, strPluginName);
	pTheApp->m_AppSettings.CheckMenuPlugin(m_fiUnitsDOO.GetFilePath(), STR_W3M_UNITSDOO, strPluginName);
	pTheApp->m_AppSettings.CheckMenuPlugin(m_fiW3R.GetFilePath(), STR_W3M_W3R, strPluginName);
	pTheApp->m_AppSettings.CheckMenuPlugin(m_fiW3C.GetFilePath(), STR_W3M_W3C, strPluginName);
	pTheApp->m_AppSettings.CheckMenuPlugin(m_fiW3U.GetFilePath(), STR_W3M_W3U, strPluginName);
	pTheApp->m_AppSettings.CheckMenuPlugin(m_fiW3S.GetFilePath(), STR_W3M_W3S, strPluginName);
	pTheApp->m_AppSettings.CheckMenuPlugin(m_fiIMP.GetFilePath(), STR_W3M_IMP, strPluginName);
}
