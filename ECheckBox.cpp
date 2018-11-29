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
// ECheckBox.cpp : implementation file
//
#include "stdafx.h"
#include "W3ZMapEdit.h"
#include "ECheckBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CECheckBox
CECheckBox::CECheckBox()
{
	m_bWriteState = FALSE;
	m_bMouseOver = FALSE;
	m_bRTracking = FALSE;
	m_bKbTracking = FALSE;
	m_crTextColor = GetSysColor(COLOR_WINDOWTEXT);
	m_hBrush = ::CreateSolidBrush(GetSysColor(COLOR_3DFACE));
}

CECheckBox::~CECheckBox()
{
	::DeleteObject(m_hBrush);
}


BEGIN_MESSAGE_MAP(CECheckBox, CButton)
	//{{AFX_MSG_MAP(CECheckBox)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CECheckBox message handlers

HBRUSH CECheckBox::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	if (CTLCOLOR_STATIC == nCtlColor)
	{
		pDC->SetTextColor(m_crTextColor);
		pDC->SetBkMode(TRANSPARENT);
	}
	return m_hBrush;
}

void CECheckBox::OnRButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture();
	m_bRTracking = TRUE;
	ToggleWriteState();
	CButton::OnRButtonDown(nFlags, point);
}

void CECheckBox::OnMouseMove(UINT nFlags, CPoint point) 
{
	CRect rect;
	GetClientRect(rect);
	if (rect.PtInRect(point))
	{
		if (!m_bMouseOver)
		{
			if (m_bRTracking)
			{ToggleWriteState();}
			m_bMouseOver = TRUE;
		}
	}
	else
	{
		if (m_bMouseOver)
		{
			if (m_bRTracking)
			{ToggleWriteState();}
			m_bMouseOver = FALSE;
		}
	}
	CButton::OnMouseMove(nFlags, point);
}

void CECheckBox::OnRButtonUp(UINT nFlags, CPoint point) 
{
	m_bRTracking = FALSE;
	CButton::OnRButtonUp(nFlags, point);
	ReleaseCapture();
}

void CECheckBox::ToggleWriteState()
{
	if (m_bWriteState)
	{
		m_bWriteState = FALSE;
		m_crTextColor = GetSysColor(COLOR_WINDOWTEXT);
	}
	else
	{
		m_bWriteState = TRUE;
		m_crTextColor = RGB(255,0,0);
	}	
	RedrawWindow();
}

BOOL CECheckBox::IsWriteEnabled()
{
	return m_bWriteState;
}

void CECheckBox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	bool bCtlrLState,bCtlrRState;
	if (GetKeyState(VK_LCONTROL) & 0x80)
	{bCtlrLState = TRUE;}
	else
	{bCtlrLState = FALSE;}
	if (GetKeyState(VK_RCONTROL) & 0x80)
	{bCtlrRState = TRUE;}
	else
	{bCtlrRState = FALSE;}
	if (!m_bRTracking && !m_bKbTracking
		&& (bCtlrLState || bCtlrRState)
		&& (nChar == 0x20))
	{
		m_bKbTracking = TRUE;
		ToggleWriteState();
	}
	else
	{
		CButton::OnKeyDown(nChar, nRepCnt, nFlags);
	}
}

void CECheckBox::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	m_bKbTracking = FALSE;
	CButton::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CECheckBox::SetWriteState(BOOL state)
{
	if (state)
	{
		m_bWriteState = TRUE;
		m_crTextColor = RGB(255,0,0);
	}
	else
	{
		m_bWriteState = FALSE;
		m_crTextColor = GetSysColor(COLOR_WINDOWTEXT);
	}	
	RedrawWindow();
}
