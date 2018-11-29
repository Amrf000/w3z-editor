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
#if !defined(AFX_VIEWMODEDIALOG_H__8112137E_995A_424F_8C5E_1E01DAE68198__INCLUDED_)
#define AFX_VIEWMODEDIALOG_H__8112137E_995A_424F_8C5E_1E01DAE68198__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewModeDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewModeDialog dialog

class CViewModeDialog : public CDialogBar
{
// Construction
public:
	void UpdateViewFlags(int iViewFlags);
	CViewModeDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CViewModeDialog)
	enum { IDD = IDD_BAR_VIEWMODE };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewModeDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CViewModeDialog)
	afx_msg void OnRadTiles();
	afx_msg void OnRadLevel();
	afx_msg void OnChkDoodads();
	afx_msg void OnChkPath();
	afx_msg void OnChkShadows();
	afx_msg void OnChkPlayers();
	afx_msg void OnChkTilesBlights();
	afx_msg void OnChkTilesBoundaries();
	afx_msg void OnChkTilesLayers();
	afx_msg void OnChkTilesRamps();
	afx_msg void OnChkTilesWater();
	afx_msg void OnChkTilesWebounds();
	afx_msg void OnChkUnits();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWMODEDIALOG_H__8112137E_995A_424F_8C5E_1E01DAE68198__INCLUDED_)
