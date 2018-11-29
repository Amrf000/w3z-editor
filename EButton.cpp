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
// EButton.cpp : implementation file
//
#include "stdafx.h"
#include "W3ZMapEdit.h"
#include "EButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CEButton
CEButton::CEButton()
{
	m_crTextColor = GetSysColor(COLOR_WINDOWTEXT);
	m_crBkColor = GetSysColor(COLOR_3DFACE);
	m_crBkColorState0 = GetSysColor(COLOR_3DFACE);//RGB(0,191,0);  // green
	m_crBkColorState1 = RGB(255,0,0);  // red
	m_crBkColorState2 = RGB(255,159,0);  // yellow
	m_iOldState = m_iNewState = m_iState = EB_STATE0;
	m_bTextOnly = FALSE;
	m_bFocus = FALSE;
	m_bKbTracking = FALSE;
	m_bLTracking = FALSE;
	m_bMouseOver = FALSE;
	m_bTriState = FALSE;
}

CEButton::~CEButton()
{
}


BEGIN_MESSAGE_MAP(CEButton, CButton)
	//{{AFX_MSG_MAP(CEButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEButton message handlers

void CEButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{

	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);     //Get device context object
	CRect rt, rtofs, rtCircle;
	CPoint pt1,pt2;
	CBrush brush;
	CBrush* pOldBrush;
	CPen pen;
	CString strTemp;
	int space;

	rt = lpDrawItemStruct->rcItem;        // get button rect
	rtofs = rt;
	if ((m_bMouseOver && m_bLTracking) || (m_bKbTracking))  // check for an offset
	{
		rtofs.OffsetRect(1,1);
	}

	if (!m_bTextOnly)
	{
		dc.FillSolidRect(rt, m_crBkColor);  // erase button background
		// set position of the state dot
		space = (int)((rtofs.TopLeft().y + rtofs.BottomRight().y - EB_CIRCLE_DIAM + 0.5) / 2.);
		pt1.x = rtofs.TopLeft().x + space;
		pt1.y = space + 1;
		pt2.x = pt1.x + EB_CIRCLE_DIAM;
		pt2.y = pt1.y + EB_CIRCLE_DIAM;
		rtCircle.SetRect(pt1, pt2);

		if (EB_STATE1 == m_iState)
		{
			brush.CreateSolidBrush(m_crBkColorState1);  // create the dot brush
		}
		else if (EB_STATE2 == m_iState)
		{
			brush.CreateSolidBrush(m_crBkColorState2);  // create the dot brush
		}
		else
		{
			brush.CreateSolidBrush(m_crBkColorState0);  // create the dot brush
		}
		pOldBrush = dc.GetCurrentBrush(); // save old brush
		if (m_bFocus)       // If the button is focused, make the dot larger
		{rtCircle.InflateRect(1,1);}
		pen.CreatePen(PS_SOLID, 1, m_crTextColor);
		dc.SelectObject(&brush);
		dc.SelectObject(pen);
		if  (EB_STATE0 != m_iState)
		{
			dc.Ellipse(rtCircle);  // draws the dot
		}
		dc.SelectObject(pOldBrush);
		rtofs.DeflateRect(EB_CIRCLE_DIAM + 2*space,0,0,0);
	}
	else
	{
		if (EB_STATE1 == m_iState)
		{
			dc.FillSolidRect(rt, m_crBkColorState1);  // erase button background
		}
		else if (EB_STATE2 == m_iState)
		{
			dc.FillSolidRect(rt, m_crBkColorState2);  // erase button background
		}
		else
		{
			dc.FillSolidRect(rt, m_crBkColorState0);  // erase button background
		}
	}

	GetWindowText(strTemp); // Get the caption which have been set
	// Set the color of the caption
/*	if (m_bTextOnly)
	{
		if (EB_STATE1 == m_iState)
		{
			dc.SetTextColor(m_crBkColorState1);
		}
		else if (EB_STATE2 == m_iState)
		{
			dc.SetTextColor(m_crBkColorState2);
		}
		else
		{
			dc.SetTextColor(m_crBkColorState0);
		}
	}
	else
	{
*/		dc.SetTextColor(m_crTextColor);
//	}
	dc.SetBkMode(TRANSPARENT);
	if (m_bTextOnly)
	{
		dc.DrawText(strTemp, rtofs, DT_CENTER|DT_VCENTER|DT_SINGLELINE); // Draw out the caption
	}
	else
	{
		dc.DrawText(strTemp, rtofs, DT_LEFT|DT_VCENTER|DT_SINGLELINE); // Draw out the caption
	}

	if ((EB_STATE1 == m_iState) || (EB_STATE2 == m_iState) || (m_bMouseOver && m_bLTracking)) // If it is pressed
	{
		dc.DrawEdge(rt,EDGE_SUNKEN,BF_RECT);    // Draw a sunken face
	}
	else
	{
		dc.DrawEdge(rt,EDGE_RAISED,BF_RECT);    // Draw a raised face
	}

	dc.Detach();
}


void CEButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture();
	m_bLTracking = TRUE;
	m_bFocus = TRUE;
	m_iOldState = ToggleState();
	m_iNewState = m_iState;
	CButton::OnLButtonDown(nFlags, point);
}


void CEButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bLTracking)
	{
		if (!m_bMouseOver)
		{
			m_iState = m_iOldState;
		}
		m_iOldState = m_iNewState = m_iState;
		m_bLTracking = FALSE;
	}
	CButton::OnLButtonUp(nFlags, point);
	RedrawWindow();
	ReleaseCapture();
}


void CEButton::OnMouseMove(UINT nFlags, CPoint point) 
{

	CRect rect;
	GetClientRect(rect);
	if (rect.PtInRect(point))
	{
		if (m_bLTracking)
		{m_iState = m_iNewState;}
		m_bMouseOver = TRUE;
	}
	else
	{
		if (m_bLTracking)
		{m_iState = m_iOldState;}
		m_bMouseOver = FALSE;
	}
	CButton::OnMouseMove(nFlags, point);
}


void CEButton::OnSetFocus(CWnd* pOldWnd) 
{
	CButton::OnSetFocus(pOldWnd);
	m_bFocus = TRUE;
	RedrawWindow();
}
void CEButton::OnKillFocus(CWnd* pNewWnd) 
{
	CButton::OnKillFocus(pNewWnd);
	m_bFocus = FALSE;
	RedrawWindow();
}


UINT CEButton::ToggleState()
{
	UINT oldState = m_iState;
	if (EB_STATE0 == m_iState)
	{
		m_iState = EB_STATE1;
	}
	else if ((EB_STATE1 == m_iState) && (m_bTriState))
	{
		m_iState = EB_STATE2;
	}
	else
	{
		m_iState = EB_STATE0;
	}
	return oldState;
}


UINT CEButton::GetState()
{
	return m_iState;
}


void CEButton::SetState(UINT state)
{
	m_iOldState = m_iNewState = m_iState = state;
}

void CEButton::SetTriState(BOOL tristate)
{
	m_bTriState	= tristate;
}


void CEButton::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (!m_bLTracking && !m_bKbTracking)
	{
		if (nChar == 0x20)
		{
			m_iOldState = ToggleState();
			m_bKbTracking = TRUE;
		}
		RedrawWindow();
	}
	CButton::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CEButton::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (!m_bLTracking)
	{
		m_bKbTracking = FALSE;
		m_iOldState = m_iNewState = m_iState;
	}
	RedrawWindow();	
	CButton::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CEButton::SetTextOnly(BOOL textonly)
{
	m_bTextOnly = textonly;
}

void CEButton::SetColorState0(COLORREF newcolor)
{
	m_crBkColorState0 = newcolor;
}

void CEButton::SetColorState1(COLORREF newcolor)
{
	m_crBkColorState1 = newcolor;
}

void CEButton::SetColorState2(COLORREF newcolor)
{
	m_crBkColorState2 = newcolor;
}
