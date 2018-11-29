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
#if !defined(AFX_EDITMAPPROPPLAYERSPAGE_H__6ABD2107_E56A_418D_A043_F1E044D634AB__INCLUDED_)
#define AFX_EDITMAPPROPPLAYERSPAGE_H__6ABD2107_E56A_418D_A043_F1E044D634AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditMapPropPlayersPage.h : header file
//
#include "PropertyPageEnh.h"

/////////////////////////////////////////////////////////////////////////////
// CEditMapPropPlayersPage dialog

class CEditMapPropPlayersPage : public CPropertyPageEnh
{
	DECLARE_DYNCREATE(CEditMapPropPlayersPage)

// Construction
public:
	CEditMapPropPlayersPage();
	virtual ~CEditMapPropPlayersPage();

// Dialog Data
	//{{AFX_DATA(CEditMapPropPlayersPage)
	enum { IDD = IDD_PAGE_PLAYERSPROP };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CEditMapPropPlayersPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CEditMapPropPlayersPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITMAPPROPPLAYERSPAGE_H__6ABD2107_E56A_418D_A043_F1E044D634AB__INCLUDED_)
