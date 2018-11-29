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
#if !defined(AFX_EDITMAPPROPMINIMAPPAGE_H__26774C14_5083_4518_893D_6A472F6C77C4__INCLUDED_)
#define AFX_EDITMAPPROPMINIMAPPAGE_H__26774C14_5083_4518_893D_6A472F6C77C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditMapPropMinimapPage.h : header file
//
#include "PropertyPageEnh.h"

/////////////////////////////////////////////////////////////////////////////
// CEditMapPropMinimapPage dialog

class CEditMapPropMinimapPage : public CPropertyPageEnh
{
	DECLARE_DYNCREATE(CEditMapPropMinimapPage)

// Construction
public:
	CEditMapPropMinimapPage();
	virtual ~CEditMapPropMinimapPage();

// Dialog Data
	//{{AFX_DATA(CEditMapPropMinimapPage)
	enum { IDD = IDD_PAGE_MINIMAP };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CEditMapPropMinimapPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CEditMapPropMinimapPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITMAPPROPMINIMAPPAGE_H__26774C14_5083_4518_893D_6A472F6C77C4__INCLUDED_)
