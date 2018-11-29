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
#if !defined(AFX_PATHBUTTON_H__33FEBB65_A97D_4A22_B484_D99A6DFA7720__INCLUDED_)
#define AFX_PATHBUTTON_H__33FEBB65_A97D_4A22_B484_D99A6DFA7720__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PathButton.h : header file
//

#include "W3ZMapEditSettings.h"

/////////////////////////////////////////////////////////////////////////////
// CPathButton window

class CPathButton : public CButton
{
// Construction
public:
	CPathButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPathButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetFlags(int flags);
	BOOL IsModeActive(int mode, int x, int y);
	virtual ~CPathButton();

	// Generated message map functions
protected:
	CW3ZMapEditSettings * m_pAppSettings;
	int m_iFlags;
	//{{AFX_MSG(CPathButton)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATHBUTTON_H__33FEBB65_A97D_4A22_B484_D99A6DFA7720__INCLUDED_)
