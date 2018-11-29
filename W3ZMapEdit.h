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
// W3ZMapEdit.h : main header file for the W3ZMAPEDIT application
//

#if !defined(AFX_W3ZMAPEDIT_H__6645DD9C_3E84_431C_B0A0_E5C97CC25070__INCLUDED_)
#define AFX_W3ZMAPEDIT_H__6645DD9C_3E84_431C_B0A0_E5C97CC25070__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h" // main symbols
#include "War3TypesAndConstants.h" // all the typedefs and constants needed
#include "W3ZMapEditSettings.h"
#include "ToolTilePointsEdit.h"
#include "ToolDoodadsEdit.h"
#include "ToolUnitsEdit.h"
#include "ToolPathEdit.h"
#include "ToolShadowsEdit.h"

class CMainFrame;
/////////////////////////////////////////////////////////////////////////////
// CW3ZMapEditApp:
// See W3ZMapEdit.cpp for the implementation of this class
//

class CW3ZMapEditApp : public CWinApp
{
public:
	void OnBtnPathBar();
	CW3ZMapEditSettings m_AppSettings;
	CString GetAppPath();
	BOOL IsMPQInit();
	CMainFrame* GetMainFrame();
	int GetEditMode();
	void SetEditMode(int iEditMode);
	CToolTilePointsEdit m_ToolTilePointsEdit;
	CToolDoodadsEdit m_ToolDoodadsEdit;
	CToolUnitsEdit m_ToolUnitsEdit;
	CToolPathEdit m_ToolPathEdit;
	CToolShadowsEdit m_ToolShadowsEdit;
	CTool * m_pCurrentTool;
	CW3ZMapEditApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CW3ZMapEditApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CW3ZMapEditApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileOpen();
	afx_msg void OnBtnTilePen();
	afx_msg void OnUpdateBtnTilePen(CCmdUI* pCmdUI);
	afx_msg void OnBtnTileSelect();
	afx_msg void OnUpdateBtnTileSelect(CCmdUI* pCmdUI);
	afx_msg void OnBtnTileBendDown();
	afx_msg void OnUpdateBtnTileBendDown(CCmdUI* pCmdUI);
	afx_msg void OnBtnTileBendDownWater();
	afx_msg void OnUpdateBtnTileBendDownWater(CCmdUI* pCmdUI);
	afx_msg void OnBtnTileBendUp();
	afx_msg void OnUpdateBtnTileBendUp(CCmdUI* pCmdUI);
	afx_msg void OnBtnTileBendUpWater();
	afx_msg void OnUpdateBtnTileBendUpWater(CCmdUI* pCmdUI);
	afx_msg void OnBtnTileEllipse();
	afx_msg void OnUpdateBtnTileEllipse(CCmdUI* pCmdUI);
	afx_msg void OnBtnTileFill();
	afx_msg void OnUpdateBtnTileFill(CCmdUI* pCmdUI);
	afx_msg void OnBtnTileFlat();
	afx_msg void OnUpdateBtnTileFlat(CCmdUI* pCmdUI);
	afx_msg void OnBtnTileFlatWater();
	afx_msg void OnUpdateBtnTileFlatWater(CCmdUI* pCmdUI);
	afx_msg void OnBtnTileRand();
	afx_msg void OnUpdateBtnTileRand(CCmdUI* pCmdUI);
	afx_msg void OnBtnTileRandWater();
	afx_msg void OnUpdateBtnTileRandWater(CCmdUI* pCmdUI);
	afx_msg void OnBtnTileReplace();
	afx_msg void OnUpdateBtnTileReplace(CCmdUI* pCmdUI);
	afx_msg void OnBtnTileRotate();
	afx_msg void OnUpdateBtnTileRotate(CCmdUI* pCmdUI);
	afx_msg void OnBtnTileSlope();
	afx_msg void OnUpdateBtnTileSlope(CCmdUI* pCmdUI);
	afx_msg void OnBtnTileSlopeWater();
	afx_msg void OnUpdateBtnTileSlopeWater(CCmdUI* pCmdUI);
	afx_msg void OnBtnTileRect();
	afx_msg void OnUpdateBtnTileRect(CCmdUI* pCmdUI);
	afx_msg void OnBtnTileHMirror();
	afx_msg void OnUpdateBtnTileHMirror(CCmdUI* pCmdUI);
	afx_msg void OnBtnTileVMirror();
	afx_msg void OnUpdateBtnTileVMirror(CCmdUI* pCmdUI);
	afx_msg void OnBtnDoodAdd();
	afx_msg void OnUpdateBtnDoodAdd(CCmdUI* pCmdUI);
	afx_msg void OnBtnDoodDelete();
	afx_msg void OnUpdateBtnDoodDelete(CCmdUI* pCmdUI);
	afx_msg void OnBtnDoodEllipse();
	afx_msg void OnUpdateBtnDoodEllipse(CCmdUI* pCmdUI);
	afx_msg void OnBtnDoodEllipseFill();
	afx_msg void OnUpdateBtnDoodEllipseFill(CCmdUI* pCmdUI);
	afx_msg void OnBtnDoodReplace();
	afx_msg void OnUpdateBtnDoodReplace(CCmdUI* pCmdUI);
	afx_msg void OnBtnDoodSelect();
	afx_msg void OnUpdateBtnDoodSelect(CCmdUI* pCmdUI);
	afx_msg void OnBtnDoodSpacing();
	afx_msg void OnUpdateBtnDoodSpacing(CCmdUI* pCmdUI);
	afx_msg void OnBtnDoodRect();
	afx_msg void OnUpdateBtnDoodRect(CCmdUI* pCmdUI);
	afx_msg void OnBtnDoodRectFill();
	afx_msg void OnUpdateBtnDoodRectFill(CCmdUI* pCmdUI);
	afx_msg void OnBtnDoodUpdt();
	afx_msg void OnUpdateBtnDoodUpdt(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	CString m_strAppPath;
	BOOL m_MPQInit;
	int m_iEditMode;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_W3ZMAPEDIT_H__6645DD9C_3E84_431C_B0A0_E5C97CC25070__INCLUDED_)
