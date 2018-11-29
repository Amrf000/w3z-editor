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
// W3ZMapEdit.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "W3ZMapEdit.h"
#include "W3ZMapEditDoc.h"
#include "W3ZMapEditView.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "3DChildFrame.h"
#include "sfmpqapi.h"
#include "W3ZMapOpenGLView.h"
#include "MvDocTemplate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CW3ZMapEditApp

BEGIN_MESSAGE_MAP(CW3ZMapEditApp, CWinApp)
	//{{AFX_MSG_MAP(CW3ZMapEditApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_BTN_TILE_PEN, OnBtnTilePen)
	ON_UPDATE_COMMAND_UI(ID_BTN_TILE_PEN, OnUpdateBtnTilePen)
	ON_COMMAND(ID_BTN_TILE_SELECT, OnBtnTileSelect)
	ON_UPDATE_COMMAND_UI(ID_BTN_TILE_SELECT, OnUpdateBtnTileSelect)
	ON_COMMAND(ID_BTN_TILE_BENDDOWN, OnBtnTileBendDown)
	ON_UPDATE_COMMAND_UI(ID_BTN_TILE_BENDDOWN, OnUpdateBtnTileBendDown)
	ON_COMMAND(ID_BTN_TILE_BENDDOWN_WATER, OnBtnTileBendDownWater)
	ON_UPDATE_COMMAND_UI(ID_BTN_TILE_BENDDOWN_WATER, OnUpdateBtnTileBendDownWater)
	ON_COMMAND(ID_BTN_TILE_BENDUP, OnBtnTileBendUp)
	ON_UPDATE_COMMAND_UI(ID_BTN_TILE_BENDUP, OnUpdateBtnTileBendUp)
	ON_COMMAND(ID_BTN_TILE_BENDUP_WATER, OnBtnTileBendUpWater)
	ON_UPDATE_COMMAND_UI(ID_BTN_TILE_BENDUP_WATER, OnUpdateBtnTileBendUpWater)
	ON_COMMAND(ID_BTN_TILE_ELLIPSE, OnBtnTileEllipse)
	ON_UPDATE_COMMAND_UI(ID_BTN_TILE_ELLIPSE, OnUpdateBtnTileEllipse)
	ON_COMMAND(ID_BTN_TILE_FILL, OnBtnTileFill)
	ON_UPDATE_COMMAND_UI(ID_BTN_TILE_FILL, OnUpdateBtnTileFill)
	ON_COMMAND(ID_BTN_TILE_FLAT, OnBtnTileFlat)
	ON_UPDATE_COMMAND_UI(ID_BTN_TILE_FLAT, OnUpdateBtnTileFlat)
	ON_COMMAND(ID_BTN_TILE_FLAT_WATER, OnBtnTileFlatWater)
	ON_UPDATE_COMMAND_UI(ID_BTN_TILE_FLAT_WATER, OnUpdateBtnTileFlatWater)
	ON_COMMAND(ID_BTN_TILE_RAND, OnBtnTileRand)
	ON_UPDATE_COMMAND_UI(ID_BTN_TILE_RAND, OnUpdateBtnTileRand)
	ON_COMMAND(ID_BTN_TILE_RAND_WATER, OnBtnTileRandWater)
	ON_UPDATE_COMMAND_UI(ID_BTN_TILE_RAND_WATER, OnUpdateBtnTileRandWater)
	ON_COMMAND(ID_BTN_TILE_REPLACE, OnBtnTileReplace)
	ON_UPDATE_COMMAND_UI(ID_BTN_TILE_REPLACE, OnUpdateBtnTileReplace)
	ON_COMMAND(ID_BTN_TILE_ROTATE, OnBtnTileRotate)
	ON_UPDATE_COMMAND_UI(ID_BTN_TILE_ROTATE, OnUpdateBtnTileRotate)
	ON_COMMAND(ID_BTN_TILE_SLOPE, OnBtnTileSlope)
	ON_UPDATE_COMMAND_UI(ID_BTN_TILE_SLOPE, OnUpdateBtnTileSlope)
	ON_COMMAND(ID_BTN_TILE_SLOPE_WATER, OnBtnTileSlopeWater)
	ON_UPDATE_COMMAND_UI(ID_BTN_TILE_SLOPE_WATER, OnUpdateBtnTileSlopeWater)
	ON_COMMAND(ID_BTN_TILE_RECT, OnBtnTileRect)
	ON_UPDATE_COMMAND_UI(ID_BTN_TILE_RECT, OnUpdateBtnTileRect)
	ON_COMMAND(ID_BTN_TILE_H_MIRROR, OnBtnTileHMirror)
	ON_UPDATE_COMMAND_UI(ID_BTN_TILE_H_MIRROR, OnUpdateBtnTileHMirror)
	ON_COMMAND(ID_BTN_TILE_V_MIRROR, OnBtnTileVMirror)
	ON_UPDATE_COMMAND_UI(ID_BTN_TILE_V_MIRROR, OnUpdateBtnTileVMirror)
	ON_COMMAND(ID_BTN_DOOD_ADD, OnBtnDoodAdd)
	ON_UPDATE_COMMAND_UI(ID_BTN_DOOD_ADD, OnUpdateBtnDoodAdd)
	ON_COMMAND(ID_BTN_DOOD_DELETE, OnBtnDoodDelete)
	ON_UPDATE_COMMAND_UI(ID_BTN_DOOD_DELETE, OnUpdateBtnDoodDelete)
	ON_COMMAND(ID_BTN_DOOD_ELLIPSE, OnBtnDoodEllipse)
	ON_UPDATE_COMMAND_UI(ID_BTN_DOOD_ELLIPSE, OnUpdateBtnDoodEllipse)
	ON_COMMAND(ID_BTN_DOOD_ELLIPSE_FILL, OnBtnDoodEllipseFill)
	ON_UPDATE_COMMAND_UI(ID_BTN_DOOD_ELLIPSE_FILL, OnUpdateBtnDoodEllipseFill)
	ON_COMMAND(ID_BTN_DOOD_REPLACE, OnBtnDoodReplace)
	ON_UPDATE_COMMAND_UI(ID_BTN_DOOD_REPLACE, OnUpdateBtnDoodReplace)
	ON_COMMAND(ID_BTN_DOOD_SELECT, OnBtnDoodSelect)
	ON_UPDATE_COMMAND_UI(ID_BTN_DOOD_SELECT, OnUpdateBtnDoodSelect)
	ON_COMMAND(ID_BTN_DOOD_SPACING, OnBtnDoodSpacing)
	ON_UPDATE_COMMAND_UI(ID_BTN_DOOD_SPACING, OnUpdateBtnDoodSpacing)
	ON_COMMAND(ID_BTN_DOOD_RECT, OnBtnDoodRect)
	ON_UPDATE_COMMAND_UI(ID_BTN_DOOD_RECT, OnUpdateBtnDoodRect)
	ON_COMMAND(ID_BTN_DOOD_RECT_FILL, OnBtnDoodRectFill)
	ON_UPDATE_COMMAND_UI(ID_BTN_DOOD_RECT_FILL, OnUpdateBtnDoodRectFill)
	ON_COMMAND(ID_BTN_DOOD_UPDT, OnBtnDoodUpdt)
	ON_UPDATE_COMMAND_UI(ID_BTN_DOOD_UPDT, OnUpdateBtnDoodUpdt)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CW3ZMapEditApp construction

CW3ZMapEditApp::CW3ZMapEditApp()
{
	CFileFind ffDir;
	CFileException e;
	char Buf[6];

	Buf[4] = (char)0x6E; // zepbuff
	Buf[1] = (char)(0x78 + Buf[4]); // zepbuff
	//+FIXME: nettoyer les fichiers temporaires ou les recuperer..
	srand((unsigned)time( NULL )); // init randomizer
	m_iEditMode = MODE_TILEPOINTS;
	m_pCurrentTool = (CTool *) &m_ToolTilePointsEdit;
	m_pCurrentTool->SetToolMode(TOOL_MODE_TILE_PEN);
	m_MPQInit = TRUE;

	Buf[4] += 4; // zepbuff
	Buf[0] = (char)0x56; // zepbuff
	Buf[3] = (char)0x65; // zepbuff

	if (!MpqInitialize())
	{
		DWORD e = GetLastError();
		m_MPQInit = FALSE;
		CString str = STR_ERROR_APP00;
/*+zep: update ShadowFlare
		if (e == MPQ_ERROR_INIT_FAILED)
		{
			str += STR_ERROR_APP01;
		}
		else if (e == MPQ_ERROR_NO_STAREDIT)
		{
			str += STR_ERROR_APP02;
		}
		else if (e == MPQ_ERROR_BAD_STAREDIT)
		{
			str += STR_ERROR_APP03;
		}
		else if (e == MPQ_ERROR_STAREDIT_RUNNING)
		{
			str += STR_ERROR_APP04;
		}
		else
		{
			str += STR_ERROR_APP05;
		}
		W3Z_ERROR(str);
*/
	}

	Buf[0] += 4; // zepbuff
	Buf[5] = '\0'; // zepbuff
	Buf[1] += 3; // zepbuff
	Buf[2] = (char)0x56; // zepbuff

	m_strAppPath = ".";
	if (ffDir.FindFile(m_strAppPath))
	{
		ffDir.FindNextFile();
		m_strAppPath = ffDir.GetFilePath() + "\\";
	}

	Buf[3] += (char)(Buf[0] - Buf[2]); // zepbuff
	Buf[2] += 0x1A; // zepbuff
	strcpy(Zepir, Buf); // zepbuff

}

/////////////////////////////////////////////////////////////////////////////
// The one and only CW3ZMapEditApp object

CW3ZMapEditApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CW3ZMapEditApp initialization

BOOL CW3ZMapEditApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization

//+zep: obsolete:
//#ifdef _AFXDLL
//	Enable3dControls();			// Call this when using MFC in a shared DLL
//#else
//	Enable3dControlsStatic();	// Call this when linking to MFC statically
//#endif

	// Change the registry key under which our settings are stored.
	SetRegistryKey(STR_REGISTRY);

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	//LoadSettings
	m_AppSettings.InitParameters();

	// Register document templates


/*
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_W3ZMAPTYPE,
		RUNTIME_CLASS(CW3ZMapEditDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CW3ZMapEditView));
*/
	CMDIMVDocTemplate *pDocTemplate = new CMDIMVDocTemplate(IDR_W3ZMAPTYPE, RUNTIME_CLASS(CW3ZMapEditDoc));
    pDocTemplate->AddFrameTemplate(
        new CFrameTemplate( 
            IDR_W3ZMAPTYPE, 
            RUNTIME_CLASS( CChildFrame ), 
            RUNTIME_CLASS( CW3ZMapEditView ), 
            ID_VIEW_VIEW1, 
            SW_SHOWNORMAL, 
            TRUE));

    pDocTemplate->AddFrameTemplate(
        new CFrameTemplate( 
            IDR_W3ZMAPTYPE, 
            RUNTIME_CLASS( C3DChildFrame ), 
            RUNTIME_CLASS( CW3ZMapOpenGLView ), 
            ID_VIEW_VIEW2, 
            SW_SHOWNORMAL, 
            TRUE));

	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	//don't create a new file on startup
/*
	if (cmdInfo.m_strFileName.GetLength() < 1)
	{
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	}
*/
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
	{
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	}


	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	int infosindex;
	int stvalvy;
	int stvalvx;
	UINT m_TimerID;
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CStatic	m_statval;
	CStatic	m_statinfos;
	CButton	m_quad;
	CString	m_scrinfos;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_scrinfos = _T("");
	//}}AFX_DATA_INIT
 m_TimerID = 0;
 stvalvy = 2;
 stvalvx = 2;
 infosindex = 0;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_STATICZEP, m_statval);
	DDX_Control(pDX, IDC_SCRINFOS, m_statinfos);
	DDX_Control(pDX, IDC_STATICQUAD, m_quad);
	DDX_Text(pDX, IDC_SCRINFOS, m_scrinfos);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CW3ZMapEditApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString str;

	m_TimerID = SetTimer(1, 50, NULL);
	str = "-=";
	str += Zepir;
	str += " oo";
	m_statval.SetWindowText(str);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CAboutDlg::DestroyWindow() 
{
	if (m_TimerID != 0)
		KillTimer(m_TimerID);	
	return CDialog::DestroyWindow();
}

void CAboutDlg::OnTimer(UINT nIDEvent) 
{
 WINDOWPLACEMENT wndplval,wndplcadre;
 int delta;
 CRect rect(0,0,800,16);
 CString str;

 str = STR_ABOUT_INFOS1;
 str += Zepir;
 str += STR_ABOUT_INFOS2;
 m_scrinfos = str.Mid(infosindex/4) + str;
 infosindex++;
 UpdateData(FALSE);

 if (infosindex >= str.GetLength()*4)
 {infosindex = 0;}
 
 m_quad.GetWindowPlacement( &wndplcadre );
 m_statval.GetWindowPlacement( &wndplval );

 wndplval.rcNormalPosition.top += stvalvy;
 wndplval.rcNormalPosition.bottom += stvalvy;
 wndplval.rcNormalPosition.left += stvalvx;
 wndplval.rcNormalPosition.right += stvalvx;

 delta = wndplval.rcNormalPosition.bottom - wndplval.rcNormalPosition.top;

 if (wndplval.rcNormalPosition.bottom > wndplcadre.rcNormalPosition.bottom - 2)
 { // collision en bas du cadre
  wndplval.rcNormalPosition.bottom = wndplcadre.rcNormalPosition.bottom - 2;
  wndplval.rcNormalPosition.top = wndplval.rcNormalPosition.bottom - delta;
  if (stvalvy > 0)
  {stvalvy = -1*stvalvy;}
 }
 else if (wndplval.rcNormalPosition.top < wndplcadre.rcNormalPosition.top + 8)
 {// collision en haut du cadre
  wndplval.rcNormalPosition.top = wndplcadre.rcNormalPosition.top + 8;
  wndplval.rcNormalPosition.bottom = wndplval.rcNormalPosition.top + delta;
  if (stvalvy < 0)
  {stvalvy = -1*stvalvy;}
 }

 delta = wndplval.rcNormalPosition.right - wndplval.rcNormalPosition.left;

 if (wndplval.rcNormalPosition.right > wndplcadre.rcNormalPosition.right - 2)
 {//collision a droite
  wndplval.rcNormalPosition.right = wndplcadre.rcNormalPosition.right - 2;
  wndplval.rcNormalPosition.left = wndplval.rcNormalPosition.right - delta;
  if (stvalvx > 0)
  {stvalvx = -1*stvalvx;}
 }
 else if (wndplval.rcNormalPosition.left < wndplcadre.rcNormalPosition.left + 2)
 {// collision a gauche
  wndplval.rcNormalPosition.left = wndplcadre.rcNormalPosition.left + 2;
  wndplval.rcNormalPosition.right = wndplval.rcNormalPosition.left + delta;
  if (stvalvx < 0)
  {stvalvx = -1*stvalvx;}
 }


 m_statval.SetWindowPlacement( &wndplval);
	
	CDialog::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////////////
// CW3ZMapEditApp message handlers



/*
Update main edit mode using the specified mode

Input:
-iMode: use constants defined in W3ZMapEdit.h
   (MODE_DEFAULT, MODE_TILEPOINTS, MODE_DOODADS,...)

*/

void CW3ZMapEditApp::SetEditMode(int iEditMode)
{
	if (iEditMode == MODE_DEFAULT)
	{
		m_pCurrentTool = (CTool *) &(m_ToolTilePointsEdit);	
		m_iEditMode = MODE_DEFAULT;
	}
	else if (iEditMode == MODE_TILEPOINTS)
	{
		m_pCurrentTool = (CTool *) &(m_ToolTilePointsEdit);	
		m_iEditMode = MODE_TILEPOINTS;
	}
	else if (iEditMode == MODE_DOODADS)
	{
		m_pCurrentTool = (CTool *) &(m_ToolDoodadsEdit);
		m_iEditMode = MODE_DOODADS;
	}
	else if (iEditMode == MODE_SHADOWS)
	{
		m_pCurrentTool = (CTool *) &(m_ToolShadowsEdit);	
		m_iEditMode = MODE_SHADOWS;
	}
	else if (iEditMode == MODE_PATH)
	{
		m_pCurrentTool = (CTool *) &(m_ToolPathEdit);
		m_iEditMode = MODE_PATH;
	}
/*
fonctions a implementer
	else if (iEditMode == MODE_UNITS)
	{
		m_pCurrentTool = (CTool *) &(m_ToolUnitsEdit);
		m_iEditMode = MODE_UNITS;
	}
*/	else
	{
		m_pCurrentTool = (CTool *) &(m_ToolTilePointsEdit);
		m_iEditMode = MODE_DEFAULT;
	}
	((CMainFrame*)m_pMainWnd)->UpdateEditMode();
}

int CW3ZMapEditApp::GetEditMode()
{
	return m_iEditMode;
}

CMainFrame* CW3ZMapEditApp::GetMainFrame()
{
	return (CMainFrame*)m_pMainWnd;
}

void CW3ZMapEditApp::OnFileOpen()
{
//	CWinApp::DoPromptFileName followed by CWinApp::OpenDocumentFile 
	CFileDialog dFileOpenDialog( TRUE, STR_W3M_EXTENSION, NULL, OFN_ALLOWMULTISELECT | OFN_EXPLORER | OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, STR_FILES_FILTER, this->GetMainFrame());

//OFN_ENABLESIZING  OFN_FILEMUSTEXIST  OFN_HIDEREADONLY  OFN_LONGNAMES  OFN_PATHMUSTEXIST
	if (IDOK == dFileOpenDialog.DoModal())
	{
		POSITION p;
		CString filename;
		p = dFileOpenDialog.GetStartPosition();
		while (p)
		{
			filename = dFileOpenDialog.GetNextPathName(p);
			CDocument* pDoc = OpenDocumentFile(filename);
			if (filename.Right(3) != pDoc->GetPathName().Right(3))
			{ // compares extensions to check for convertions
				pDoc->SetModifiedFlag();
			}
		}
	}
}



BOOL CW3ZMapEditApp::IsMPQInit()
{
	return m_MPQInit;
}


/*
**********
*/
CString CW3ZMapEditApp::GetAppPath()
{
	return  m_strAppPath;
}




/*******************
* TILEPOINTS TOOLS *
*******************/
void CW3ZMapEditApp::OnBtnTilePen() 
{
	m_pCurrentTool = (CTool *) &(m_ToolTilePointsEdit);	
	m_pCurrentTool->SetToolMode(TOOL_MODE_TILE_PEN);
	m_iEditMode = MODE_TILEPOINTS;
}

void CW3ZMapEditApp::OnUpdateBtnTilePen(CCmdUI* pCmdUI) 
{
	if ((m_pCurrentTool == &m_ToolTilePointsEdit) && (TOOL_MODE_TILE_PEN == m_ToolTilePointsEdit.GetToolMode()))
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void CW3ZMapEditApp::OnBtnTileSelect() 
{
	m_pCurrentTool = (CTool *) &(m_ToolTilePointsEdit);	
	m_pCurrentTool->SetToolMode(TOOL_MODE_TILE_SELECT);
	m_iEditMode = MODE_TILEPOINTS;
}

void CW3ZMapEditApp::OnUpdateBtnTileSelect(CCmdUI* pCmdUI) 
{
	if ((m_pCurrentTool == &m_ToolTilePointsEdit) && (TOOL_MODE_TILE_SELECT == m_ToolTilePointsEdit.GetToolMode()))
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void CW3ZMapEditApp::OnBtnTileBendDown()
{
//	m_pCurrentTool = (CTool *) &(m_ToolTilePointsEdit);	
//	m_pCurrentTool->SetToolMode(TOOL_MODE_TILE_BEND_DOWN);
	m_ToolTilePointsEdit.GroundBendDown();
//	m_iEditMode = MODE_TILEPOINTS;
}

void CW3ZMapEditApp::OnUpdateBtnTileBendDown(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(FALSE);
	CMainFrame *pMainFrame = GetMainFrame();
	if (pMainFrame)
	{
		CChildFrame *pChildFrame = (CChildFrame*)pMainFrame->GetActiveFrame();
		if (pChildFrame)
		{
			CW3ZMapEditView *pView = pChildFrame->GetEditView();
			if (pView)
			{
				CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pChildFrame->GetActiveDocument();
				if (pDoc)
				{
					ASSERT_VALID(pDoc);
					if ((4 == pDoc->m_War3Map.mapTilePointSelState))
					{
						pCmdUI->Enable(TRUE);
						return;
					}
				}
			}
		}
	}
	pCmdUI->Enable(FALSE);
}

void CW3ZMapEditApp::OnBtnTileBendDownWater() 
{
//	m_pCurrentTool = (CTool *) &(m_ToolTilePointsEdit);	
//	m_pCurrentTool->SetToolMode(TOOL_MODE_TILE_BEND_DOWN_WATER);
	m_ToolTilePointsEdit.WaterBendDown();
//	m_iEditMode = MODE_TILEPOINTS;
}

void CW3ZMapEditApp::OnUpdateBtnTileBendDownWater(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(FALSE);
	CMainFrame *pMainFrame = GetMainFrame();
	if (pMainFrame)
	{
		CChildFrame *pChildFrame = (CChildFrame*)pMainFrame->GetActiveFrame();
		if (pChildFrame)
		{
			CW3ZMapEditView *pView = pChildFrame->GetEditView();
			if (pView)
			{
				CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pChildFrame->GetActiveDocument();
				if (pDoc)
				{
					ASSERT_VALID(pDoc);
					if ((4 == pDoc->m_War3Map.mapTilePointSelState))
					{
						pCmdUI->Enable(TRUE);
						return;
					}
				}
			}
		}
	}
	pCmdUI->Enable(FALSE);
}

void CW3ZMapEditApp::OnBtnTileBendUp() 
{
//	m_pCurrentTool = (CTool *) &(m_ToolTilePointsEdit);	
//	m_pCurrentTool->SetToolMode(TOOL_MODE_TILE_BEND_UP);
	m_ToolTilePointsEdit.GroundBendUp();
//	m_iEditMode = MODE_TILEPOINTS;
}

void CW3ZMapEditApp::OnUpdateBtnTileBendUp(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(FALSE);
	CMainFrame *pMainFrame = GetMainFrame();
	if (pMainFrame)
	{
		CChildFrame *pChildFrame = (CChildFrame*)pMainFrame->GetActiveFrame();
		if (pChildFrame)
		{
			CW3ZMapEditView *pView = pChildFrame->GetEditView();
			if (pView)
			{
				CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pChildFrame->GetActiveDocument();
				if (pDoc)
				{
					ASSERT_VALID(pDoc);
					if ((4 == pDoc->m_War3Map.mapTilePointSelState))
					{
						pCmdUI->Enable(TRUE);
						return;
					}
				}
			}
		}
	}
	pCmdUI->Enable(FALSE);
}

void CW3ZMapEditApp::OnBtnTileBendUpWater() 
{
//	m_pCurrentTool = (CTool *) &(m_ToolTilePointsEdit);	
//	m_pCurrentTool->SetToolMode(TOOL_MODE_TILE_BEND_UP_WATER);
	m_ToolTilePointsEdit.WaterBendUp();
//	m_iEditMode = MODE_TILEPOINTS;
}

void CW3ZMapEditApp::OnUpdateBtnTileBendUpWater(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(FALSE);
	CMainFrame *pMainFrame = GetMainFrame();
	if (pMainFrame)
	{
		CChildFrame *pChildFrame = (CChildFrame*)pMainFrame->GetActiveFrame();
		if (pChildFrame)
		{
			CW3ZMapEditView *pView = pChildFrame->GetEditView();
			if (pView)
			{
				CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pChildFrame->GetActiveDocument();
				if (pDoc)
				{
					ASSERT_VALID(pDoc);
					if ((4 == pDoc->m_War3Map.mapTilePointSelState))
					{
						pCmdUI->Enable(TRUE);
						return;
					}
				}
			}
		}
	}
	pCmdUI->Enable(FALSE);
}

void CW3ZMapEditApp::OnBtnTileEllipse() 
{
	m_pCurrentTool = (CTool *) &(m_ToolTilePointsEdit);	
	m_pCurrentTool->SetToolMode(TOOL_MODE_TILE_ELLIPSE);
	m_iEditMode = MODE_TILEPOINTS;
}

void CW3ZMapEditApp::OnUpdateBtnTileEllipse(CCmdUI* pCmdUI) 
{
	if ((m_pCurrentTool == &m_ToolTilePointsEdit) && (TOOL_MODE_TILE_ELLIPSE == m_ToolTilePointsEdit.GetToolMode()))
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void CW3ZMapEditApp::OnBtnTileFill() 
{
	m_pCurrentTool = (CTool *) &(m_ToolTilePointsEdit);	
	m_pCurrentTool->SetToolMode(TOOL_MODE_TILE_FILL);
	m_iEditMode = MODE_TILEPOINTS;
}

void CW3ZMapEditApp::OnUpdateBtnTileFill(CCmdUI* pCmdUI) 
{
	if ((m_pCurrentTool == &m_ToolTilePointsEdit) && (TOOL_MODE_TILE_FILL == m_ToolTilePointsEdit.GetToolMode()))
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void CW3ZMapEditApp::OnBtnTileFlat() 
{
//	m_pCurrentTool = (CTool *) &(m_ToolTilePointsEdit);	
//	m_pCurrentTool->SetToolMode(TOOL_MODE_TILE_FLAT);
	m_ToolTilePointsEdit.GroundFlat();
//	m_iEditMode = MODE_TILEPOINTS;
}

void CW3ZMapEditApp::OnUpdateBtnTileFlat(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(FALSE);
	CMainFrame *pMainFrame = GetMainFrame();
	if (pMainFrame)
	{
		CChildFrame *pChildFrame = (CChildFrame*)pMainFrame->GetActiveFrame();
		if (pChildFrame)
		{
			CW3ZMapEditView *pView = pChildFrame->GetEditView();
			if (pView)
			{
				CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pChildFrame->GetActiveDocument();
				if (pDoc)
				{
					ASSERT_VALID(pDoc);
					if ((4 == pDoc->m_War3Map.mapTilePointSelState))
					{
						pCmdUI->Enable(TRUE);
						return;
					}
				}
			}
		}
	}
	pCmdUI->Enable(FALSE);
}

void CW3ZMapEditApp::OnBtnTileFlatWater() 
{
//	m_pCurrentTool = (CTool *) &(m_ToolTilePointsEdit);	
//	m_pCurrentTool->SetToolMode(TOOL_MODE_TILE_FLAT_WATER);
	m_ToolTilePointsEdit.WaterFlat();
//	m_iEditMode = MODE_TILEPOINTS;
}

void CW3ZMapEditApp::OnUpdateBtnTileFlatWater(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(FALSE);
	CMainFrame *pMainFrame = GetMainFrame();
	if (pMainFrame)
	{
		CChildFrame *pChildFrame = (CChildFrame*)pMainFrame->GetActiveFrame();
		if (pChildFrame)
		{
			CW3ZMapEditView *pView = pChildFrame->GetEditView();
			if (pView)
			{
				CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pChildFrame->GetActiveDocument();
				if (pDoc)
				{
					ASSERT_VALID(pDoc);
					if ((4 == pDoc->m_War3Map.mapTilePointSelState))
					{
						pCmdUI->Enable(TRUE);
						return;
					}
				}
			}
		}
	}
	pCmdUI->Enable(FALSE);
}

void CW3ZMapEditApp::OnBtnTileRand() 
{
//	m_pCurrentTool = (CTool *) &(m_ToolTilePointsEdit);	
//	m_pCurrentTool->SetToolMode(TOOL_MODE_TILE_RANDOM);
	m_ToolTilePointsEdit.GroundRand();
//	m_iEditMode = MODE_TILEPOINTS;
}

void CW3ZMapEditApp::OnUpdateBtnTileRand(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(FALSE);
	CMainFrame *pMainFrame = GetMainFrame();
	if (pMainFrame)
	{
		CChildFrame *pChildFrame = (CChildFrame*)pMainFrame->GetActiveFrame();
		if (pChildFrame)
		{
			CW3ZMapEditView *pView = pChildFrame->GetEditView();
			if (pView)
			{
				CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pChildFrame->GetActiveDocument();
				if (pDoc)
				{
					ASSERT_VALID(pDoc);
					if ((4 == pDoc->m_War3Map.mapTilePointSelState))
					{
						pCmdUI->Enable(TRUE);
						return;
					}
				}
			}
		}
	}
	pCmdUI->Enable(FALSE);
}

void CW3ZMapEditApp::OnBtnTileRandWater() 
{
//	m_pCurrentTool = (CTool *) &(m_ToolTilePointsEdit);	
//	m_pCurrentTool->SetToolMode(TOOL_MODE_TILE_RANDOM_WATER);
	m_ToolTilePointsEdit.WaterRand();
//	m_iEditMode = MODE_TILEPOINTS;
}

void CW3ZMapEditApp::OnUpdateBtnTileRandWater(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(FALSE);
	CMainFrame *pMainFrame = GetMainFrame();
	if (pMainFrame)
	{
		CChildFrame *pChildFrame = (CChildFrame*)pMainFrame->GetActiveFrame();
		if (pChildFrame)
		{
			CW3ZMapEditView *pView = pChildFrame->GetEditView();
			if (pView)
			{
				CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pChildFrame->GetActiveDocument();
				if (pDoc)
				{
					ASSERT_VALID(pDoc);
					if ((4 == pDoc->m_War3Map.mapTilePointSelState))
					{
						pCmdUI->Enable(TRUE);
						return;
					}
				}
			}
		}
	}
	pCmdUI->Enable(FALSE);
}

void CW3ZMapEditApp::OnBtnTileReplace() 
{
	m_pCurrentTool = (CTool *) &(m_ToolTilePointsEdit);	
	m_pCurrentTool->SetToolMode(TOOL_MODE_TILE_REPLACE);
	m_iEditMode = MODE_TILEPOINTS;
}

void CW3ZMapEditApp::OnUpdateBtnTileReplace(CCmdUI* pCmdUI) 
{
	if ((m_pCurrentTool == &m_ToolTilePointsEdit) && (TOOL_MODE_TILE_REPLACE == m_ToolTilePointsEdit.GetToolMode()))
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void CW3ZMapEditApp::OnBtnTileRotate() 
{
//	m_pCurrentTool = (CTool *) &(m_ToolTilePointsEdit);	
//	m_pCurrentTool->SetToolMode(TOOL_MODE_TILE_ROTATE);
//	m_iEditMode = MODE_TILEPOINTS;
}

void CW3ZMapEditApp::OnUpdateBtnTileRotate(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(FALSE);
/*
	CMainFrame *pMainFrame = GetMainFrame();
	if (pMainFrame)
	{
		CChildFrame *pChildFrame = (CChildFrame*)pMainFrame->GetActiveFrame();
		if (pChildFrame)
		{
			CW3ZMapEditView *pView = pChildFrame->GetEditView();
			if (pView)
			{
				CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pChildFrame->GetActiveDocument();
				if (pDoc)
				{
					ASSERT_VALID(pDoc);
					if ((4 == pDoc->m_War3Map.mapTilePointSelState))
					{
						pCmdUI->Enable(TRUE);
						return;
					}
				}
			}
		}
	}
*/
	pCmdUI->Enable(FALSE);
}

void CW3ZMapEditApp::OnBtnTileSlope() 
{
//	m_pCurrentTool = (CTool *) &(m_ToolTilePointsEdit);	
//	m_pCurrentTool->SetToolMode(TOOL_MODE_TILE_SLOPE);
	m_ToolTilePointsEdit.GroundSlope();
////	m_iEditMode = MODE_TILEPOINTS;
}

void CW3ZMapEditApp::OnUpdateBtnTileSlope(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(FALSE);
	CMainFrame *pMainFrame = GetMainFrame();
	if (pMainFrame)
	{
		CChildFrame *pChildFrame = (CChildFrame*)pMainFrame->GetActiveFrame();
		if (pChildFrame)
		{
			CW3ZMapEditView *pView = pChildFrame->GetEditView();
			if (pView)
			{
				CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pChildFrame->GetActiveDocument();
				if (pDoc)
				{
					ASSERT_VALID(pDoc);
					if ((4 == pDoc->m_War3Map.mapTilePointSelState))
					{
						pCmdUI->Enable(TRUE);
						return;
					}
				}
			}
		}
	}
	pCmdUI->Enable(FALSE);
}

void CW3ZMapEditApp::OnBtnTileSlopeWater() 
{
//	m_pCurrentTool = (CTool *) &(m_ToolTilePointsEdit);	
//	m_pCurrentTool->SetToolMode(TOOL_MODE_TILE_SLOPE_WATER);
	m_ToolTilePointsEdit.WaterSlope();
//	m_iEditMode = MODE_TILEPOINTS;
}

void CW3ZMapEditApp::OnUpdateBtnTileSlopeWater(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(FALSE);
	CMainFrame *pMainFrame = GetMainFrame();
	if (pMainFrame)
	{
		CChildFrame *pChildFrame = (CChildFrame*)pMainFrame->GetActiveFrame();
		if (pChildFrame)
		{
			CW3ZMapEditView *pView = pChildFrame->GetEditView();
			if (pView)
			{
				CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pChildFrame->GetActiveDocument();
				if (pDoc)
				{
					ASSERT_VALID(pDoc);
					if ((4 == pDoc->m_War3Map.mapTilePointSelState))
					{
						pCmdUI->Enable(TRUE);
						return;
					}
				}
			}
		}
	}
	pCmdUI->Enable(FALSE);
}

void CW3ZMapEditApp::OnBtnTileRect() 
{
	m_pCurrentTool = (CTool *) &(m_ToolTilePointsEdit);	
	m_pCurrentTool->SetToolMode(TOOL_MODE_TILE_RECT);
	m_iEditMode = MODE_TILEPOINTS;
}

void CW3ZMapEditApp::OnUpdateBtnTileRect(CCmdUI* pCmdUI) 
{
	if ((m_pCurrentTool == &m_ToolTilePointsEdit) && (TOOL_MODE_TILE_RECT == m_ToolTilePointsEdit.GetToolMode()))
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void CW3ZMapEditApp::OnBtnTileHMirror() 
{
//	m_pCurrentTool = (CTool *) &(m_ToolTilePointsEdit);	
//	m_pCurrentTool->SetToolMode(TOOL_MODE_TILE_H_MIRROR);
//	m_iEditMode = MODE_TILEPOINTS;
}

void CW3ZMapEditApp::OnUpdateBtnTileHMirror(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(FALSE);
/*
	CMainFrame *pMainFrame = GetMainFrame();
	if (pMainFrame)
	{
		CChildFrame *pChildFrame = (CChildFrame*)pMainFrame->GetActiveFrame();
		if (pChildFrame)
		{
			CW3ZMapEditView *pView = pChildFrame->GetEditView();
			if (pView)
			{
				CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pChildFrame->GetActiveDocument();
				if (pDoc)
				{
					ASSERT_VALID(pDoc);
					if ((4 == pDoc->m_War3Map.mapTilePointSelState))
					{
						pCmdUI->Enable(TRUE);
						return;
					}
				}
			}
		}
	}
*/
	pCmdUI->Enable(FALSE);
}

void CW3ZMapEditApp::OnBtnTileVMirror() 
{
//	m_pCurrentTool = (CTool *) &(m_ToolTilePointsEdit);	
//	m_pCurrentTool->SetToolMode(TOOL_MODE_TILE_V_MIRROR);
//	m_iEditMode = MODE_TILEPOINTS;
}

void CW3ZMapEditApp::OnUpdateBtnTileVMirror(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(FALSE);
/*
	CMainFrame *pMainFrame = GetMainFrame();
	if (pMainFrame)
	{
		CChildFrame *pChildFrame = (CChildFrame*)pMainFrame->GetActiveFrame();
		if (pChildFrame)
		{
			CW3ZMapEditView *pView = pChildFrame->GetEditView();
			if (pView)
			{
				CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pChildFrame->GetActiveDocument();
				if (pDoc)
				{
					ASSERT_VALID(pDoc);
					if ((4 == pDoc->m_War3Map.mapTilePointSelState))
					{
						pCmdUI->Enable(TRUE);
						return;
					}
				}
			}
		}
	}
*/
	pCmdUI->Enable(FALSE);
}

/****************
* DOODADS TOOLS *
****************/

void CW3ZMapEditApp::OnBtnDoodAdd() 
{
	m_pCurrentTool = (CTool *) &(m_ToolDoodadsEdit);	
	m_pCurrentTool->SetToolMode(TOOL_MODE_DOOD_ADD);
	m_iEditMode = MODE_DOODADS;
}

void CW3ZMapEditApp::OnUpdateBtnDoodAdd(CCmdUI* pCmdUI) 
{
	if ((m_pCurrentTool == &m_ToolDoodadsEdit) && (TOOL_MODE_DOOD_ADD == m_ToolDoodadsEdit.GetToolMode()))
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void CW3ZMapEditApp::OnBtnDoodDelete() 
{
	m_pCurrentTool = (CTool *) &(m_ToolDoodadsEdit);	
	m_pCurrentTool->SetToolMode(TOOL_MODE_DOOD_DELETE);
	m_iEditMode = MODE_DOODADS;
}

void CW3ZMapEditApp::OnUpdateBtnDoodDelete(CCmdUI* pCmdUI) 
{
	if ((m_pCurrentTool == &m_ToolDoodadsEdit) && (TOOL_MODE_DOOD_DELETE == m_ToolDoodadsEdit.GetToolMode()))
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void CW3ZMapEditApp::OnBtnDoodEllipse() 
{
	m_pCurrentTool = (CTool *) &(m_ToolDoodadsEdit);	
	m_pCurrentTool->SetToolMode(TOOL_MODE_DOOD_ELLIPSE);
	m_iEditMode = MODE_DOODADS;
}

void CW3ZMapEditApp::OnUpdateBtnDoodEllipse(CCmdUI* pCmdUI) 
{
	if ((m_pCurrentTool == &m_ToolDoodadsEdit) && (TOOL_MODE_DOOD_ELLIPSE == m_ToolDoodadsEdit.GetToolMode()))
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void CW3ZMapEditApp::OnBtnDoodEllipseFill() 
{
	m_pCurrentTool = (CTool *) &(m_ToolDoodadsEdit);	
	m_pCurrentTool->SetToolMode(TOOL_MODE_DOOD_F_ELLIPSE);
	m_iEditMode = MODE_DOODADS;
}

void CW3ZMapEditApp::OnUpdateBtnDoodEllipseFill(CCmdUI* pCmdUI) 
{
	if ((m_pCurrentTool == &m_ToolDoodadsEdit) && (TOOL_MODE_DOOD_F_ELLIPSE == m_ToolDoodadsEdit.GetToolMode()))
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void CW3ZMapEditApp::OnBtnDoodReplace() 
{
	m_pCurrentTool = (CTool *) &(m_ToolDoodadsEdit);	
	m_pCurrentTool->SetToolMode(TOOL_MODE_DOOD_REPLACE);
	m_iEditMode = MODE_DOODADS;
}

void CW3ZMapEditApp::OnUpdateBtnDoodReplace(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(FALSE);//+disabled
	pCmdUI->Enable(FALSE);//+disabled
	if ((m_pCurrentTool == &m_ToolDoodadsEdit) && (TOOL_MODE_DOOD_REPLACE == m_ToolDoodadsEdit.GetToolMode()))
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void CW3ZMapEditApp::OnBtnDoodSelect() 
{
	m_pCurrentTool = (CTool *) &(m_ToolDoodadsEdit);	
	m_pCurrentTool->SetToolMode(TOOL_MODE_DOOD_SELECT);
	m_iEditMode = MODE_DOODADS;
}

void CW3ZMapEditApp::OnUpdateBtnDoodSelect(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(FALSE);//+disabled
	pCmdUI->Enable(FALSE);//+disabled
	if ((m_pCurrentTool == &m_ToolDoodadsEdit) && (TOOL_MODE_DOOD_SELECT == m_ToolDoodadsEdit.GetToolMode()))
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void CW3ZMapEditApp::OnBtnDoodSpacing() 
{
	m_pCurrentTool = (CTool *) &(m_ToolDoodadsEdit);	
	m_pCurrentTool->SetToolMode(TOOL_MODE_DOOD_SPACING);
	m_iEditMode = MODE_DOODADS;
}

void CW3ZMapEditApp::OnUpdateBtnDoodSpacing(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(FALSE);//+disabled
	pCmdUI->Enable(FALSE);//+disabled
	if ((m_pCurrentTool == &m_ToolDoodadsEdit) && (TOOL_MODE_DOOD_SPACING == m_ToolDoodadsEdit.GetToolMode()))
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void CW3ZMapEditApp::OnBtnDoodRect() 
{
	m_pCurrentTool = (CTool *) &(m_ToolDoodadsEdit);	
	m_pCurrentTool->SetToolMode(TOOL_MODE_DOOD_RECT);
	m_iEditMode = MODE_DOODADS;
}

void CW3ZMapEditApp::OnUpdateBtnDoodRect(CCmdUI* pCmdUI) 
{
	if ((m_pCurrentTool == &m_ToolDoodadsEdit) && (TOOL_MODE_DOOD_RECT == m_ToolDoodadsEdit.GetToolMode()))
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void CW3ZMapEditApp::OnBtnDoodRectFill() 
{
	m_pCurrentTool = (CTool *) &(m_ToolDoodadsEdit);	
	m_pCurrentTool->SetToolMode(TOOL_MODE_DOOD_F_RECT);
	m_iEditMode = MODE_DOODADS;
}

void CW3ZMapEditApp::OnUpdateBtnDoodRectFill(CCmdUI* pCmdUI) 
{
	if ((m_pCurrentTool == &m_ToolDoodadsEdit) && (TOOL_MODE_DOOD_F_RECT == m_ToolDoodadsEdit.GetToolMode()))
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void CW3ZMapEditApp::OnBtnDoodUpdt() 
{
	m_pCurrentTool = (CTool *) &(m_ToolDoodadsEdit);	
	m_pCurrentTool->SetToolMode(TOOL_MODE_DOOD_UPDATE);
	m_iEditMode = MODE_DOODADS;
}

void CW3ZMapEditApp::OnUpdateBtnDoodUpdt(CCmdUI* pCmdUI) 
{
	if ((m_pCurrentTool == &m_ToolDoodadsEdit) && (TOOL_MODE_DOOD_UPDATE == m_ToolDoodadsEdit.GetToolMode()))
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}




/*************
* PATH TOOLS *
*************/

void CW3ZMapEditApp::OnBtnPathBar()
{
	m_pCurrentTool = (CTool *) &(m_ToolPathEdit);	
	m_pCurrentTool->SetToolMode(TOOL_MODE_PATH_WRITE);
	m_iEditMode = MODE_PATH;
}
