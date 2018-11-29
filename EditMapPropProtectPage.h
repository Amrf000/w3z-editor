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
#if !defined(AFX_EDITMAPPROPPROTECTPAGE_H__035AA49F_8F70_462C_8F79_9506DB9DCB7A__INCLUDED_)
#define AFX_EDITMAPPROPPROTECTPAGE_H__035AA49F_8F70_462C_8F79_9506DB9DCB7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditMapPropProtectPage.h : header file
//
#include "PropertyPageEnh.h"

/////////////////////////////////////////////////////////////////////////////
// CEditMapPropProtectPage dialog

class CW3ZMapEditDoc;

class CEditMapPropProtectPage : public CPropertyPageEnh
{
	DECLARE_DYNCREATE(CEditMapPropProtectPage)

// Construction
public:
	void EnableRegisterBtn(BOOL bEnable=TRUE);
	void EnablePageItems(BOOL bEnable = TRUE);
	void EnableW3ZLevelRadios(BOOL bEnable=TRUE);
	CW3ZMapEditDoc* m_pDoc;
	CEditMapPropProtectPage();
	virtual ~CEditMapPropProtectPage();

// Dialog Data
	//{{AFX_DATA(CEditMapPropProtectPage)
	enum { IDD = IDD_PAGE_MAPPROTECT };
	CButton	m_btnRegister;
	CButton	m_chkEnableProtect;
	CEdit	m_edtOrgAuthorName;
	CEdit	m_edtPublicPassword;
	CEdit	m_edtAuthorPassword;
	CButton	m_chkAuthorMsg;
	CButton	m_chkMakeBackup;
	CButton	m_chkUnpRec;
	CComboBox	m_cmbProtectType;
	BOOL	m_bEnableProtect;
 	BOOL	m_bAuthorMsg;
	BOOL	m_bMakeBackup;
	BOOL	m_bUnpRec;
	CString	m_sAuthorPassword;
	CString	m_sPublicPassword;
	CString	m_sProtectVer;
	CString	m_sProtectStat;
	CString	m_sOrgAuthorName;
	CString	m_strRegisteredStatus;
	int		m_iProtectLvl;
	int		m_iTypeSelected;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CEditMapPropProtectPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_bTooManyTries;
	BOOL m_bIsRegisterEnabled;
//	BOOL m_bEnableW3ZLevelRadios;
	// Generated message map functions
	//{{AFX_MSG(CEditMapPropProtectPage)
	afx_msg void OnBtnRegister();
	afx_msg void OnSelChangeCmbProtectType();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITMAPPROPPROTECTPAGE_H__035AA49F_8F70_462C_8F79_9506DB9DCB7A__INCLUDED_)
