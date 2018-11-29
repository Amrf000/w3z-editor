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
#if !defined(AFX_TILEDIALOG_H__33F6C1E1_FC21_4370_B7ED_9D5342AA18A1__INCLUDED_)
#define AFX_TILEDIALOG_H__33F6C1E1_FC21_4370_B7ED_9D5342AA18A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TileDialog.h : header file
//

#include "TilePoint.h"
#include "EButton.h"
#include "ECheckBox.h"
#include "TilePointToolPropDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CTileDialog dialog

class CTileDialog : public CDialogBar
{
// Construction
public:
	CTilePoint GetRandTilePoint(CTilePoint oldtilepoint);
	void UpdateCliffTypes(CString tilesets);
	void UpdateGroundTypes(CString tilesets);
	void EditProperties();
	void OnUpdateCmdUI( CFrameWnd* pTarget, BOOL bDisableIfNoHndler );
	CTilePoint GetTilePoint();
	CTilePoint GetNewTilePoint(CTilePoint oldtilepoint);
	void UpdateTilePoint(CTilePoint tilepoint);
	void InitDilaogBar();
	void SetFlags(int flags);
	int GetFlags();
	CTileDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTileDialog)
	enum { IDD = IDD_BAR_TILE };
	CEButton	m_ctrlSetWaterLevel;
	CEButton	m_ctrlSetTilePointHeight;
	CEButton	m_ctrlSetLayer;
	CEButton	m_ctrlSetGroundType;
	CEButton	m_ctrlSetDetails;
	CEButton	m_ctrlSetCliffType;
	CECheckBox	m_ctrlChkWater;
	CECheckBox	m_ctrlChkMinimapShadow;
	CECheckBox	m_ctrlChkBlight;
	CECheckBox	m_ctrlChkShadow;
	CECheckBox	m_ctrlChkRamp;
	CSpinButtonCtrl	m_spnWaterLevel;
	CComboBox	m_ctrlEdtCliff;
	CComboBox	m_ctrlEdtGround;
	CEdit		m_ctrlEdtFlags;
	CSpinButtonCtrl	m_spnLayer;
	CSpinButtonCtrl	m_spnHeight;
	CSpinButtonCtrl	m_spnDetails;
	BOOL	m_bShadow;
	BOOL	m_bWater;
	int		m_iDetails;
	int		m_iHeight;
	int		m_iLayer;
	CString	m_sFlags;
	BOOL	m_bRamp;
	BOOL	m_bMinimapShadow;
	BOOL	m_bBlight;
	int		m_iWaterLevel;
	float	m_fWEWaterLevel;
	float	m_fWEGroundLevel;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTileDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CTilePointToolPropDialog m_PropDialog;
	int m_iGroundHeightAmp;
	int m_iWaterHeightAmp;
	int m_iDetailsRange;

	// Generated message map functions
	//{{AFX_MSG(CTileDialog)
	afx_msg void OnChkTileShadow();
	afx_msg void OnChkTileWater();
	afx_msg void OnKillFocusEdtFlags();
	afx_msg void OnKillFocusEdtTileLayer();
	afx_msg void OnKillFocusEdtTileHeight();
	afx_msg void OnKillFocusEdtTileDetails();
	afx_msg void OnChkTileRamp();
	afx_msg void OnChkTileMiniMapShd();
	afx_msg void OnChkTileBlight();
	afx_msg void OnKillFocusEdtTileWaterLevel();
	afx_msg void OnKillFocusEdtGroundLevel();
	afx_msg void OnKillFocusEdtWaterLevel();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TILEDIALOG_H__33F6C1E1_FC21_4370_B7ED_9D5342AA18A1__INCLUDED_)
