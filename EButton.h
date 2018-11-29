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
#if !defined(AFX_EBUTTON_H__25174420_CC04_40F7_9EA3_822252D10982__INCLUDED_)
#define AFX_EBUTTON_H__25174420_CC04_40F7_9EA3_822252D10982__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EButton.h : header file
//

// states values
const UINT EB_STATE0 = 0;
const UINT EB_STATE1 = 1;
const UINT EB_STATE2 = 2;

// diameter of the state dot
const int EB_CIRCLE_DIAM = 6;

/////////////////////////////////////////////////////////////////////////////
// CEButton window
// Class designed to handle color functions for checkbox and buttons used
// as checkboxes.

class CEButton : public CButton
{
// Construction
public:
	CEButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetColorState2(COLORREF newcolor);
	void SetColorState1(COLORREF newcolor);
	void SetColorState0(COLORREF newcolor);
	void SetTextOnly(BOOL textonly=TRUE);
	void SetTriState(BOOL tristate = TRUE);
	void SetState(UINT state);
	UINT GetState();
	UINT ToggleState();
	virtual ~CEButton();

	// Generated message map functions
protected:
	BOOL m_bTextOnly;
	BOOL m_bKbTracking;
	BOOL m_bLTracking;
	BOOL m_bMouseOver;
	BOOL m_bFocus;
	BOOL m_bTriState;
	UINT m_iOldState;
	UINT m_iNewState;
	UINT m_iState;
	COLORREF	m_crBkColorState0;
	COLORREF	m_crBkColorState1;
	COLORREF	m_crBkColorState2;
	COLORREF	m_crBkColor;
	COLORREF	m_crTextColor;
	//{{AFX_MSG(CEButton)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EBUTTON_H__25174420_CC04_40F7_9EA3_822252D10982__INCLUDED_)
