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
#if !defined(AFX_DOODADDIALOG_H__62B79103_00F4_4EA0_A6C4_D7AE9E2EFBEF__INCLUDED_)
#define AFX_DOODADDIALOG_H__62B79103_00F4_4EA0_A6C4_D7AE9E2EFBEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Doodad.h"
#include "EButton.h"
#include "ECheckBox.h"
#include "DoodadToolPropDialog.h"

// DoodadDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDoodadDialog dialog

class CDoodadDialog : public CDialogBar
{
// Construction
public:
	CDoodad GetDoodad();
	CDoodad GetUpdatedDoodad(CDoodad doodad);
	void InitDialogBar();
	void OnUpdateCmdUI(CFrameWnd *pTarget, BOOL bDisableIfNoHndler);
	void EditProperties();
	CDoodad GetNewDoodad();
	void UpdateDoodad(CDoodad doodad);
	int GetFlags();
	void SetFlags(int flags);
	CDoodad m_doodad;
	CDoodadDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDoodadDialog)
	enum { IDD = IDD_BAR_DOODADS };
	CECheckBox	m_ctrlSetNormal;
	CECheckBox	m_ctrlSetNonSolid;
	CEButton	m_ctrlSetVariation;
	CEButton	m_ctrlSetType;
	CEButton	m_ctrlSetScaleZ;
	CEButton	m_ctrlSetScaleY;
	CEButton	m_ctrlSetScaleX;
	CEButton	m_ctrlSetLife;
	CEButton	m_ctrlSetAngle;
	CEdit	m_ctrlEdtFlags;
	float	m_fXCoord;
	float	m_fYCoord;
	float	m_fXScale;
	float	m_fZScale;
	float	m_fYScale;
	CString	m_sDoodadType;
	float	m_fZCoord;
	float	m_fAngle;
	int		m_iID;
	int		m_iLife;
	int		m_iVariation;
	BOOL	m_chkNormal;
	BOOL	m_chkNonSolid;
	CString	m_sFlags;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDoodadDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDoodadToolPropDialog m_PropDialog;
	int		m_iMinLife;
	int		m_iMaxLife;
	float	m_fMinScaleX;
	float	m_fMinScaleY;
	float	m_fMinScaleZ;
	float	m_fMaxScaleX;
	float	m_fMaxScaleY;
	float	m_fMaxScaleZ;
	CString	m_sTypeList;
	int		m_iVariationRange;

	// Generated message map functions
	//{{AFX_MSG(CDoodadDialog)
	afx_msg void OnKillFocusEdtDooFlags();
	afx_msg void OnChkDooNonSolid();
	afx_msg void OnChkDooNormal();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOODADDIALOG_H__62B79103_00F4_4EA0_A6C4_D7AE9E2EFBEF__INCLUDED_)
