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
#if !defined(AFX_ESTATIC_H__81866E24_3847_4512_9EC0_D1213E85F7E6__INCLUDED_)
#define AFX_ESTATIC_H__81866E24_3847_4512_9EC0_D1213E85F7E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEStatic window

class CEStatic : public CStatic
{
// Construction
public:
	CEStatic& SetBkColor(COLORREF crBkgndColor);
	CEStatic& SetTextColor(COLORREF crTextColor);
	CEStatic();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEStatic();

	// Generated message map functions
protected:
	COLORREF	m_crTextColor;
	COLORREF	m_crBkColor;
	HBRUSH		m_hBrush;
	HBRUSH		m_hwndBrush;
	//{{AFX_MSG(CEStatic)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ESTATIC_H__81866E24_3847_4512_9EC0_D1213E85F7E6__INCLUDED_)
