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
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "W3ZMapEdit.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "W3ZMapEditDoc.h"
#include "W3ZMapEditView.h"
#include "mainfrm.h"
#include ".\mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_SHADOWSEDIT, OnShadowsEdit)
	ON_UPDATE_COMMAND_UI(ID_SHADOWSEDIT, OnUpdateShadowsEdit)
	ON_COMMAND(ID_TILESEDIT, OnTilesEdit)
	ON_UPDATE_COMMAND_UI(ID_TILESEDIT, OnUpdateTilesEdit)
	ON_COMMAND(ID_UNITSEDIT, OnUnitsEdit)
	ON_UPDATE_COMMAND_UI(ID_UNITSEDIT, OnUpdateUnitsEdit)
	ON_COMMAND(ID_PATHEDIT, OnPathEdit)
	ON_UPDATE_COMMAND_UI(ID_PATHEDIT, OnUpdatePathEdit)
	ON_COMMAND(ID_DOODADSEDIT, OnDoodadsEdit)
	ON_UPDATE_COMMAND_UI(ID_DOODADSEDIT, OnUpdateDoodadsEdit)
	ON_COMMAND(ID_VIEW_EDITMODEBAR, OnViewEditModeBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EDITMODEBAR, OnUpdateViewEditModeBar)
	ON_COMMAND(ID_VIEW_EDITTOOLBARSBAR, OnViewEditToolbarsBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EDITTOOLBARSBAR, OnUpdateViewEditToolbarsBar)
	ON_COMMAND(ID_VIEW_VIEWMODEBAR, OnViewViewModeBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_VIEWMODEBAR, OnUpdateViewViewModeBar)
	ON_COMMAND(ID_VIEW_ZOOMBAR, OnViewZoomBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMBAR, OnUpdateViewZoomBar)
	ON_COMMAND(ID_POPTPB_PROPERTIES, OnPopupTPBProperties)
	ON_COMMAND(ID_POPDB_PROPERTIES, OnPopupDBProperties)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_VIEW_3DVIEW, OnView3dview)
	ON_UPDATE_COMMAND_UI(ID_VIEW_3DVIEW, OnUpdateView3dview)
//	ON_COMMAND(ID_PLUGINS, OnPlugins)
ON_COMMAND(ID_PLUGINS, OnPlugins)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,       // status line indicator
	IDS_VIEW_POSITION,  // position
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// main toolbar
	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create main toolbar!\n");
		return -1;      // fail to create main toolbar
	}
	m_wndToolBar.SetWindowText(STR_TOOLBAR_MAIN);

	// edit mode bar
	if (!m_wndEditModeBar.Create(this, IDD_BAR_EDITMODE, 
		CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create edit mode bar!\n");
		return -1;		// fail to create edit mode bar
	}
	m_wndEditModeBar.SetWindowText(STR_TOOLBAR_EDITMODE);

	// view mode bar
	if (!m_wndViewModeBar.Create(this, IDD_BAR_VIEWMODE, 
		CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create view mode bar!\n");
		return -1;		// fail to create view mode bar
	}
	m_wndViewModeBar.SetWindowText(STR_TOOLBAR_VIEWMODE);

	// zoom bar
	if (!m_wndZoomBar.Create(this) ||
		!m_wndZoomBar.LoadToolBar(IDR_ZOOMBAR))
	{
		TRACE0("Failed to create zoom bar!\n");
		return -1;      // fail to create zoom bar
	}
	m_wndZoomBar.SetWindowText(STR_TOOLBAR_ZOOM);

	// toolbars bar
	if (!m_wndToolBarsBar.Create(this) ||
		!m_wndToolBarsBar.LoadToolBar(IDR_TOOLBARS))
	{
		TRACE0("Failed to create toolbars bar!\n");
		return -1;      // fail to create toolbars bar
	}
	m_wndToolBarsBar.SetWindowText(STR_TOOLBAR_TOOLBARS);

	// status bar
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar!\n");
		return -1;      // fail to create status bar
	}
	m_wndStatusBar.SetPaneStyle(0, SBPS_NORMAL);

	// tilepoints edit bar
	if (!m_wndTilesEditBar.Create(this, IDD_BAR_TILE, CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create tilepoints edit bar!\n");
		return -1;      // fail to create
	}
	m_wndTilesEditBar.SetWindowText(STR_TOOLBAR_TILEPOINT);

	// tilepoints toolbar
	if (!m_wndTilesEditToolBar.Create(this) ||
		!m_wndTilesEditToolBar.LoadToolBar(IDR_TILESBAR))
	{
		TRACE0("Failed to create tilepoints toolbar!\n");
		return -1;      // fail to create
	}
	m_wndTilesEditToolBar.SetWindowText(STR_TOOLBAR_TILETOOLS);

	// doodads edit bar
	if (!m_wndDoodsEditBar.Create(this, IDD_BAR_DOODADS, 
		CBRS_ALIGN_RIGHT, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create doodads edit bar!\n");
		return -1;      // fail to create
	}
	m_wndDoodsEditBar.SetWindowText(STR_TOOLBAR_DOODAD);

	// doodads toolbar
	if (!m_wndDoodadsEditToolBar.Create(this) ||
		!m_wndDoodadsEditToolBar.LoadToolBar(IDR_DOODADSBAR))
	{
		TRACE0("Failed to create doodads toolbar!\n");
		return -1;      // fail to create
	}
	m_wndDoodadsEditToolBar.SetWindowText(STR_TOOLBAR_DOODTOOLS);

	// units edit bar
	if (!m_wndUnitsEditBar.Create(this, IDD_BAR_UNITS,
		CBRS_ALIGN_RIGHT, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create units edit bar!\n");
		return -1;      // fail to create
	}
	m_wndUnitsEditBar.SetWindowText(STR_TOOLBAR_UNIT);

	// path toolbar
	if (!m_wndPathEditBar.Create(this, IDD_BAR_PATHING, 
		CBRS_ALIGN_RIGHT, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create path bar!\n");
		return -1;      // fail to create
	}
	m_wndPathEditBar.SetWindowText(STR_TOOLBAR_PATHTOOLS);

	// shadows toolbar
	if (!m_wndShadowsEditBar.Create(this) ||
		!m_wndShadowsEditBar.LoadToolBar(IDR_SHADOWSBAR))
	{
		TRACE0("Failed to create shadows toolbar!\n");
		return -1;      // fail to create
	}
	m_wndShadowsEditBar.SetWindowText(STR_TOOLBAR_SHADOWTOOLS);

	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndEditModeBar.SetBarStyle(m_wndEditModeBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndViewModeBar.SetBarStyle(m_wndViewModeBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndZoomBar.SetBarStyle(m_wndZoomBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndToolBarsBar.SetBarStyle(m_wndToolBarsBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndTilesEditBar.SetBarStyle(m_wndTilesEditBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndDoodsEditBar.SetBarStyle(m_wndDoodsEditBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndUnitsEditBar.SetBarStyle(m_wndUnitsEditBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndPathEditBar.SetBarStyle(m_wndPathEditBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndTilesEditToolBar.SetBarStyle(m_wndPathEditBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndDoodadsEditToolBar.SetBarStyle(m_wndPathEditBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndShadowsEditBar.SetBarStyle(m_wndShadowsEditBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);


	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndEditModeBar.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
	m_wndViewModeBar.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
	m_wndZoomBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarsBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndTilesEditBar.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
	m_wndDoodsEditBar.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
	m_wndUnitsEditBar.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
	m_wndPathEditBar.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
	m_wndTilesEditToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndDoodadsEditToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndShadowsEditBar.EnableDocking(CBRS_ALIGN_ANY);

	EnableDocking(CBRS_ALIGN_ANY);

	DockControlBar(&m_wndToolBar, AFX_IDW_DOCKBAR_TOP);
	DockControlBarLeftOf((CToolBar*)&m_wndEditModeBar, &m_wndToolBar);
	DockControlBarLeftOf(&m_wndToolBarsBar, (CToolBar*)&m_wndEditModeBar);
	DockControlBarLeftOf(&m_wndZoomBar, &m_wndToolBarsBar);
	DockControlBar(&m_wndViewModeBar, AFX_IDW_DOCKBAR_RIGHT);
	DockControlBar(&m_wndTilesEditBar, AFX_IDW_DOCKBAR_RIGHT);
//    FloatControlBar(&m_wndTilesEditBar,CPoint(600, 100), CBRS_ALIGN_BOTTOM | CBRS_ALIGN_RIGHT );
	DockControlBar(&m_wndTilesEditToolBar, AFX_IDW_DOCKBAR_LEFT);
	DockControlBar(&m_wndDoodadsEditToolBar, AFX_IDW_DOCKBAR_LEFT);
    FloatControlBar(&m_wndDoodsEditBar,CPoint(608, 100), CBRS_ALIGN_BOTTOM | CBRS_ALIGN_RIGHT );
    FloatControlBar(&m_wndShadowsEditBar,CPoint(616, 100), CBRS_ALIGN_BOTTOM | CBRS_ALIGN_RIGHT );
    FloatControlBar(&m_wndPathEditBar,CPoint(624, 100), CBRS_ALIGN_BOTTOM | CBRS_ALIGN_RIGHT );
    FloatControlBar(&m_wndUnitsEditBar,CPoint(632, 100), CBRS_ALIGN_BOTTOM | CBRS_ALIGN_RIGHT );

	ShowControlBar(&m_wndDoodsEditBar, false, false);
	ShowControlBar(&m_wndUnitsEditBar, false, false);
	ShowControlBar(&m_wndPathEditBar, false, false);
	ShowControlBar(&m_wndShadowsEditBar, false, false);
	ShowControlBar(&m_wndDoodadsEditToolBar, false, false);

	//set displayed mode to the default mode
	m_wndEditModeBar.UpdateEditMode(MODE_DEFAULT);
//	m_wndViewModeBar.UpdateEditMode(MODE_DEFAULT);
	//init TilePoint Edit toolbar
	m_wndTilesEditBar.InitDilaogBar();

	m_wndDoodsEditBar.InitDialogBar();

	m_wndPathEditBar.InitDialogBar();

	SetPositionText("");
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::UpdateEditMode()
{

	m_wndEditModeBar.UpdateEditMode();

	CW3ZMapEditApp *pTheApp;
	pTheApp = (CW3ZMapEditApp *)AfxGetApp();
	CChildFrame		*pChildFrame;
	CW3ZMapEditView	*pActiveView;
	pChildFrame = (CChildFrame*)MDIGetActive();

	if (pTheApp)
	{
/*		if (pTheApp->GetEditMode() == MODE_DEFAULT)
		{

		}
		else
*/
		if (pTheApp->GetEditMode() == MODE_TILEPOINTS)
		{
			ShowControlBar(&m_wndTilesEditBar, true, false);
			ShowControlBar(&m_wndTilesEditToolBar, true, false);
			ShowControlBar(&m_wndDoodsEditBar, false, false);
			ShowControlBar(&m_wndDoodadsEditToolBar, false, false);
			ShowControlBar(&m_wndUnitsEditBar, false, false);
			ShowControlBar(&m_wndPathEditBar, false, false);
			ShowControlBar(&m_wndShadowsEditBar, false, false);
			if (pChildFrame)
			{
				pActiveView = pChildFrame->GetEditView();
//				pActiveView = (CW3ZMapEditView*)pChildFrame->GetActiveView();
				if (pActiveView)
				{
					pActiveView->m_iViewFlags = VFL_TILES | VFL_DOODADS | VFL_UNITS | VFL_WATER | VFL_BOUNDS | VFL_WEBOUNDS;
				}
				UpdateViewModeBar(pActiveView->m_iViewFlags);
				pActiveView->InvalidateBitmap();
				pActiveView->Invalidate();
			}
		}
		else if (pTheApp->GetEditMode() == MODE_DOODADS)
		{
			ShowControlBar(&m_wndTilesEditBar, false, false);
			ShowControlBar(&m_wndTilesEditToolBar, false, false);
			ShowControlBar(&m_wndDoodsEditBar, true, false);
			ShowControlBar(&m_wndDoodadsEditToolBar, true, false);
			ShowControlBar(&m_wndUnitsEditBar, false, false);
			ShowControlBar(&m_wndPathEditBar, false, false);
			ShowControlBar(&m_wndShadowsEditBar, false, false);
			if (pChildFrame)
			{
				pActiveView = pChildFrame->GetEditView();
				if (pActiveView)
				{
					pActiveView->m_iViewFlags = VFL_TILES | VFL_DOODADS | VFL_UNITS | VFL_WATER | VFL_BOUNDS | VFL_WEBOUNDS;
				}
				UpdateViewModeBar(pActiveView->m_iViewFlags);
				pActiveView->InvalidateBitmap();
				pActiveView->Invalidate();
			}
		}
		else if (pTheApp->GetEditMode() == MODE_PATH)
		{
			ShowControlBar(&m_wndTilesEditBar, false, false);
			ShowControlBar(&m_wndTilesEditToolBar, false, false);
			ShowControlBar(&m_wndDoodsEditBar, false, false);
			ShowControlBar(&m_wndDoodadsEditToolBar, false, false);
			ShowControlBar(&m_wndUnitsEditBar, false, false);
			ShowControlBar(&m_wndPathEditBar, true, false);
			ShowControlBar(&m_wndShadowsEditBar, false, false);
			if (pChildFrame)
			{
				pActiveView = pChildFrame->GetEditView();
				if (pActiveView)
				{
					pActiveView->m_iViewFlags = VFL_PATH | VFL_TILES | VFL_WATER | VFL_BOUNDS | VFL_WEBOUNDS;
				}
				UpdateViewModeBar(pActiveView->m_iViewFlags);
				pActiveView->InvalidateBitmap();
				pActiveView->Invalidate();
			}
		}
		else if (pTheApp->GetEditMode() == MODE_SHADOWS)
		{
			ShowControlBar(&m_wndTilesEditBar, false, false);
			ShowControlBar(&m_wndTilesEditToolBar, false, false);
			ShowControlBar(&m_wndDoodsEditBar, false, false);
			ShowControlBar(&m_wndDoodadsEditToolBar, false, false);
			ShowControlBar(&m_wndUnitsEditBar, false, false);
			ShowControlBar(&m_wndPathEditBar, false, false);
			ShowControlBar(&m_wndShadowsEditBar, true, false);
			if (pChildFrame)
			{
				pActiveView = pChildFrame->GetEditView();
				if (pActiveView)
				{
					pActiveView->m_iViewFlags = VFL_SHADOWS | VFL_TILES | VFL_DOODADS | VFL_WATER;
				}
				UpdateViewModeBar(pActiveView->m_iViewFlags);
				pActiveView->InvalidateBitmap();
				pActiveView->Invalidate();
			}
		}
		else if (pTheApp->GetEditMode() == MODE_UNITS)
		{
			ShowControlBar(&m_wndTilesEditBar, false, false);
			ShowControlBar(&m_wndTilesEditToolBar, false, false);
			ShowControlBar(&m_wndDoodsEditBar, false, false);
			ShowControlBar(&m_wndDoodadsEditToolBar, false, false);
			ShowControlBar(&m_wndUnitsEditBar, false, false);
			ShowControlBar(&m_wndPathEditBar, false, false);
			ShowControlBar(&m_wndShadowsEditBar, false, false);
			if (pChildFrame)
			{
				pActiveView = pChildFrame->GetEditView();
				if (pActiveView)
				{
					pActiveView->m_iViewFlags = VFL_UNITS | VFL_TILES | VFL_DOODADS | VFL_WATER | VFL_BOUNDS | VFL_WEBOUNDS;
				}
				UpdateViewModeBar(pActiveView->m_iViewFlags);
				pActiveView->InvalidateBitmap();
				pActiveView->Invalidate();
			}
		}
		else
		{
		
		}
	}

}

void CMainFrame::OnShadowsEdit() 
{
	ShowControlBar(&m_wndShadowsEditBar, !m_wndShadowsEditBar.IsVisible(), false);
}

void CMainFrame::OnUpdateShadowsEdit(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_wndShadowsEditBar.IsVisible());
}

void CMainFrame::OnTilesEdit() 
{
	// use only "m_wndTilesEditBar" state! 
	bool visible = !m_wndTilesEditBar.IsVisible();
	ShowControlBar(&m_wndTilesEditBar, visible, false);
	ShowControlBar(&m_wndTilesEditToolBar, visible, false);	
}

void CMainFrame::OnUpdateTilesEdit(CCmdUI* pCmdUI) 
{
	// use only "m_wndTilesEditBar" state! 
	pCmdUI->SetCheck(m_wndTilesEditBar.IsVisible());
}

void CMainFrame::OnUnitsEdit() 
{
	ShowControlBar(&m_wndUnitsEditBar, !m_wndUnitsEditBar.IsVisible(), false);
}

void CMainFrame::OnUpdateUnitsEdit(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_wndUnitsEditBar.IsVisible());
}

void CMainFrame::OnPathEdit() 
{
	ShowControlBar(&m_wndPathEditBar, !m_wndPathEditBar.IsVisible(), false);
}

void CMainFrame::OnUpdatePathEdit(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_wndPathEditBar.IsVisible());
}

void CMainFrame::OnDoodadsEdit() 
{
	// use only "m_wndDoodsEditBar" state! 
	bool visible = !m_wndDoodsEditBar.IsVisible();
	ShowControlBar(&m_wndDoodsEditBar, visible, false);
	ShowControlBar(&m_wndDoodadsEditToolBar, visible, false);
}

void CMainFrame::OnUpdateDoodadsEdit(CCmdUI* pCmdUI) 
{
	// use only "m_wndDoodsEditBar" state! 
	pCmdUI->SetCheck(m_wndDoodsEditBar.IsVisible());
}

void CMainFrame::OnViewEditModeBar() 
{
	ShowControlBar(&m_wndEditModeBar, !m_wndEditModeBar.IsVisible(), false);
}

void CMainFrame::OnUpdateViewEditModeBar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_wndEditModeBar.IsVisible());
}

void CMainFrame::OnViewEditToolbarsBar() 
{
	ShowControlBar(&m_wndToolBarsBar, !m_wndToolBarsBar.IsVisible(), false);
}

void CMainFrame::OnUpdateViewEditToolbarsBar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_wndToolBarsBar.IsVisible());
}

void CMainFrame::UpdateTilesEditBar(CTilePoint tilepoint)
{
	m_wndTilesEditBar.UpdateTilePoint(tilepoint);
}

CTilePoint CMainFrame::GetTilesEditBarNewTilePoint(CTilePoint oldtilepoint)
{
	return m_wndTilesEditBar.GetNewTilePoint(oldtilepoint);
}

void CMainFrame::UpdateViewModeBar(int iViewFlags)
{
	m_wndViewModeBar.UpdateViewFlags(iViewFlags);
}

void CMainFrame::OnViewViewModeBar() 
{
	ShowControlBar(&m_wndViewModeBar, !m_wndViewModeBar.IsVisible(), false);
}

void CMainFrame::OnUpdateViewViewModeBar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_wndViewModeBar.IsVisible());
}

void CMainFrame::UpdateDoodadsEditBar(CDoodad doodad)
{
	m_wndDoodsEditBar.UpdateDoodad(doodad);
}

void CMainFrame::SetStatusBarText(CString text)
{
	m_wndStatusBar.SetPaneText(0, text, TRUE);
}

void CMainFrame::OnViewZoomBar() 
{
	ShowControlBar(&m_wndZoomBar, !m_wndZoomBar.IsVisible(), false);
}

void CMainFrame::OnUpdateViewZoomBar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_wndZoomBar.IsVisible());
}

void CMainFrame::SetPositionText(CString text)
{
	m_wndStatusBar.SetPaneText(1, text, TRUE);
}

void CMainFrame::DockControlBarLeftOf(CToolBar *Bar, CToolBar *LeftOf)
{
	CRect rect;
	DWORD dw;
	UINT n;
	
	// get MFC to adjust the dimensions of all docked ToolBars
	// so that GetWindowRect will be accurate
	RecalcLayout(TRUE);
	
	LeftOf->GetWindowRect(&rect);
	rect.OffsetRect(1,0);
	dw=LeftOf->GetBarStyle();
	n = 0;
	n = (dw&CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
	n = (dw&CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = (dw&CBRS_ALIGN_LEFT && n==0) ? AFX_IDW_DOCKBAR_LEFT : n;
	n = (dw&CBRS_ALIGN_RIGHT && n==0) ? AFX_IDW_DOCKBAR_RIGHT : n;
	
	// When we take the default parameters on rect, DockControlBar will dock
	// each Toolbar on a seperate line. By calculating a rectangle, we
	// are simulating a Toolbar being dragged to that location and docked.
	DockControlBar(Bar,n,&rect);

}

CTilePoint CMainFrame::GetTilesEditBarTilePoint()
{
	return m_wndTilesEditBar.GetTilePoint();
}

CTilePoint CMainFrame::GetTilesEditBarRandTilePoint(CTilePoint oldtilepoint)
{
	return m_wndTilesEditBar.GetRandTilePoint(oldtilepoint);
}

CDoodad CMainFrame::GetDoodadsEditBarNewDoodad()
{
	return m_wndDoodsEditBar.GetNewDoodad();
}


void CMainFrame::OnPopupTPBProperties() 
{
	m_wndTilesEditBar.EditProperties();
}

void CMainFrame::OnPopupDBProperties() 
{
	m_wndDoodsEditBar.EditProperties();
	
}


CDoodad CMainFrame::GetDoodadsEditBarUpdatedDoodad(CDoodad doodad)
{
	return m_wndDoodsEditBar.GetUpdatedDoodad(doodad);
}

void CMainFrame::UpdateTilesEditBar(CW3ZMapEditDoc *pDoc)
{
	m_wndTilesEditBar.UpdateGroundTypes(pDoc->m_War3Map.mapGroundTiles);
	m_wndTilesEditBar.UpdateCliffTypes(pDoc->m_War3Map.mapCliffTiles);
}

void CMainFrame::UpdateDoodadsEditBar(CW3ZMapEditDoc *pDoc)
{

}

CDoodad CMainFrame::GetDoodadsEditBarDoodad()
{
	return m_wndDoodsEditBar.GetDoodad();
}

int CMainFrame::GetPathEditBarNewFlags(int oldflags)
{
	return m_wndPathEditBar.GetNewFlags(oldflags);
}

void CMainFrame::UpdatePathEditBarColors()
{
	m_wndPathEditBar.UpdateColors();
}



void CMainFrame::OnView3dview()
{
	CChildFrame		*pChildFrame;
	CW3ZMapEditDoc	*pDoc;
	pChildFrame = (CChildFrame*)MDIGetActive();
	if (NULL != pChildFrame)
	{
		pDoc = (CW3ZMapEditDoc *)pChildFrame->GetActiveDocument();
		pChildFrame = pDoc->Get3DChildFrame();
		if (NULL != pChildFrame)
		{
			if (TRUE == pChildFrame->IsWindowVisible())
			{
				pChildFrame->ShowWindow(SW_HIDE);
			}
			else
			{
				pChildFrame->ShowWindow(SW_SHOWNORMAL);
			}
		}
	}
}

void CMainFrame::OnUpdateView3dview(CCmdUI *pCmdUI)
{
	CChildFrame		*pChildFrame;
	CW3ZMapEditDoc	*pDoc;
	pChildFrame = (CChildFrame*)MDIGetActive();
	if (NULL != pChildFrame)
	{
		pDoc = (CW3ZMapEditDoc *)pChildFrame->GetActiveDocument();
		pChildFrame = pDoc->Get3DChildFrame();
		if (NULL != pChildFrame)
		{
			pCmdUI->SetCheck(pChildFrame->IsWindowVisible());
		}
	}
}

void CMainFrame::OnPlugins()
{
	if ((IDOK == m_dlgRunPluginDialog.DoModal()) && ("" != m_dlgRunPluginDialog.m_strSelectedPluginName))
	{
		CChildFrame		*pChildFrame;
		CW3ZMapEditDoc	*pDoc;
		pChildFrame = (CChildFrame*)MDIGetActive();
		if (NULL != pChildFrame)
		{
			pDoc = (CW3ZMapEditDoc *)pChildFrame->GetActiveDocument();
			pDoc->RunPlugin(m_dlgRunPluginDialog.m_strSelectedPluginName);
		}
	}
}
