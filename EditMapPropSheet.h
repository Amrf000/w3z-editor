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
#if !defined(AFX_EDITMAPPROPSHEET_H__78208D6E_865C_4343_BA6B_BACAAC6B1493__INCLUDED_)
#define AFX_EDITMAPPROPSHEET_H__78208D6E_865C_4343_BA6B_BACAAC6B1493__INCLUDED_

#include "EditMapPropPlayersPage.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditMapPropSheet.h : header file
//

#include "EditMapPropMainPage.h"
#include "EditMapPropSizePage.h"
#include "EditMapPropProtectPage.h"
#include "EditMapPropMenuOptPage.h"
#include "EditMapPropPlayersPage.h"
#include "EditMapPropMinimapPage.h"

/////////////////////////////////////////////////////////////////////////////
// CEditMapPropSheet

class CEditMapPropSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CEditMapPropSheet)

// Construction
public:
	CEditMapPropSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CEditMapPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditMapPropSheet)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	CEditMapPropMinimapPage m_MapPropMinimapPage;
	CEditMapPropPlayersPage m_MapPropPlayersPage;
	int m_iFeaturesFlag;
	void EnableFeatures(int iFlags);
	CEditMapPropMenuOptPage m_MapPropMenuOptPage;
	CEditMapPropProtectPage m_MapPropProtectPage;
	CEditMapPropMainPage m_MapPropMainPage;
	CEditMapPropSizePage m_MapPropSizePage;
	virtual ~CEditMapPropSheet();

	// Generated message map functions
protected:
	void AddPages();
	BOOL m_bIsInited;
	HICON m_hIcon;
	//{{AFX_MSG(CEditMapPropSheet)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITMAPPROPSHEET_H__78208D6E_865C_4343_BA6B_BACAAC6B1493__INCLUDED_)
