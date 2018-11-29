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
#if !defined(AFX_ECHECKBOX_H__3A351C67_13BD_4F71_8C06_003E95F19A9B__INCLUDED_)
#define AFX_ECHECKBOX_H__3A351C67_13BD_4F71_8C06_003E95F19A9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ECheckBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CECheckBox window

class CECheckBox : public CButton
{
// Construction
public:
	CECheckBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CECheckBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetWriteState(BOOL state=TRUE);
	BOOL IsWriteEnabled();
	void ToggleWriteState();
	virtual ~CECheckBox();

	// Generated message map functions
protected:
	BOOL		m_bWriteState;
	BOOL		m_bMouseOver;
	BOOL		m_bKbTracking;
	BOOL		m_bRTracking;
	COLORREF	m_crTextColor;
	HBRUSH		m_hBrush;
	//{{AFX_MSG(CECheckBox)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ECHECKBOX_H__3A351C67_13BD_4F71_8C06_003E95F19A9B__INCLUDED_)
