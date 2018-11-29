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
// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__9E50B617_45F8_42F8_9269_F177E9009A93__INCLUDED_)
#define AFX_MAINFRM_H__9E50B617_45F8_42F8_9269_F177E9009A93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DoodadDialog.h"
#include "TileDialog.h"
#include "UnitDialog.h"
#include "PathDialog.h"
#include "EditModeDialog.h"
#include "ViewModeDialog.h"
#include "TilePoint.h"
#include "Doodad.h"
#include "runplugindialog.h"

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void UpdatePathEditBarColors();
	int GetPathEditBarNewFlags(int oldflags);
	CDoodad GetDoodadsEditBarDoodad();
	CTilePoint GetTilesEditBarRandTilePoint(CTilePoint oldtilepoint);
	void UpdateDoodadsEditBar(CW3ZMapEditDoc * pDoc);
	void UpdateTilesEditBar(CW3ZMapEditDoc * pDoc);
	CDoodad GetDoodadsEditBarUpdatedDoodad(CDoodad doodad);
	CDoodad GetDoodadsEditBarNewDoodad();
	void DockControlBarLeftOf(CToolBar* Bar, CToolBar* LeftOf);
	void SetPositionText(CString text);
	void SetStatusBarText(CString text);
	void UpdateDoodadsEditBar(CDoodad doodad);
	void UpdateViewModeBar(int iViewMode);
	CTilePoint GetTilesEditBarNewTilePoint(CTilePoint oldtilepoint);
	CTilePoint GetTilesEditBarTilePoint();
	void UpdateTilesEditBar(CTilePoint tilepoint);
	void UpdateEditMode();
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar		m_wndStatusBar;
	CToolBar		m_wndToolBar;
	CToolBar		m_wndToolBarsBar;
	CEditModeDialog	m_wndEditModeBar;
	CViewModeDialog	m_wndViewModeBar;
	CTileDialog		m_wndTilesEditBar;
	CDoodadDialog	m_wndDoodsEditBar;
	CUnitDialog		m_wndUnitsEditBar;
	CToolBar		m_wndShadowsEditBar;
	CPathDialog		m_wndPathEditBar;
	CToolBar		m_wndTilesEditToolBar;
	CToolBar		m_wndDoodadsEditToolBar;
	CToolBar		m_wndZoomBar;
	CRunPluginDialog m_dlgRunPluginDialog;


// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShadowsEdit();
	afx_msg void OnUpdateShadowsEdit(CCmdUI* pCmdUI);
	afx_msg void OnTilesEdit();
	afx_msg void OnUpdateTilesEdit(CCmdUI* pCmdUI);
	afx_msg void OnUnitsEdit();
	afx_msg void OnUpdateUnitsEdit(CCmdUI* pCmdUI);
	afx_msg void OnPathEdit();
	afx_msg void OnUpdatePathEdit(CCmdUI* pCmdUI);
	afx_msg void OnDoodadsEdit();
	afx_msg void OnUpdateDoodadsEdit(CCmdUI* pCmdUI);
	afx_msg void OnViewEditModeBar();
	afx_msg void OnUpdateViewEditModeBar(CCmdUI* pCmdUI);
	afx_msg void OnViewEditToolbarsBar();
	afx_msg void OnUpdateViewEditToolbarsBar(CCmdUI* pCmdUI);
	afx_msg void OnViewViewModeBar();
	afx_msg void OnUpdateViewViewModeBar(CCmdUI* pCmdUI);
	afx_msg void OnViewZoomBar();
	afx_msg void OnUpdateViewZoomBar(CCmdUI* pCmdUI);
	afx_msg void OnPopupTPBProperties();
	afx_msg void OnPopupDBProperties();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnView3dview();
	afx_msg void OnUpdateView3dview(CCmdUI *pCmdUI);
public:
	afx_msg void OnPlugins();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__9E50B617_45F8_42F8_9269_F177E9009A93__INCLUDED_)
