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
#if !defined(AFX_EDITMAPPROPMAINPAGE_H__931892E0_47AB_4118_A62B_948B5AEA26B4__INCLUDED_)
#define AFX_EDITMAPPROPMAINPAGE_H__931892E0_47AB_4118_A62B_948B5AEA26B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditMapPropMainPage.h : header file
//
#include "PropertyPageEnh.h"

/////////////////////////////////////////////////////////////////////////////
// CEditMapPropMainPage dialog

class CEditMapPropMainPage : public CPropertyPageEnh
{
	DECLARE_DYNCREATE(CEditMapPropMainPage)

// Construction
public:
//	void EnablePage(BOOL bEnable = TRUE);
	char m_cTilesetGroup;
	CEditMapPropMainPage();
	virtual ~CEditMapPropMainPage();

// Dialog Data
	//{{AFX_DATA(CEditMapPropMainPage)
	enum { IDD = IDD_PAGE_MAPPROP };
	CComboBox	m_cmbMapType;
	CString	m_sAuthor;
	CString	m_sCliff;
	CString	m_sDesc;
	CString	m_sGround;
	CString	m_sName;
	CString	m_sPlayText;
	int		m_iEditorVer;
	int		m_iMapVer;
	BOOL	m_bCustTilesets;
	int		m_iNbPlayers;
	BOOL	m_bWavesCliff;
	BOOL	m_bWavesRolling;
	BOOL	m_bUnknown0;
	BOOL	m_bUnknown1;
	BOOL	m_bMelee;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CEditMapPropMainPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_bIsEnabled;
	BOOL m_bIsInited;
	CString m_sOrgCliff;
	CString m_sOrgGround;

	// Generated message map functions
	//{{AFX_MSG(CEditMapPropMainPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmbMapType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITMAPPROPMAINPAGE_H__931892E0_47AB_4118_A62B_948B5AEA26B4__INCLUDED_)
