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
// EStatic.cpp : implementation file
//
#include "stdafx.h"
#include "w3zmapedit.h"
#include "EStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CEStatic
CEStatic::CEStatic()
{
	m_crTextColor = GetSysColor(COLOR_WINDOWTEXT);
	m_hBrush = ::CreateSolidBrush(GetSysColor(COLOR_3DFACE));
	m_hwndBrush = ::CreateSolidBrush(GetSysColor(COLOR_3DFACE));
}

CEStatic::~CEStatic()
{
	::DeleteObject(m_hBrush);
}


BEGIN_MESSAGE_MAP(CEStatic, CStatic)
	//{{AFX_MSG_MAP(CEStatic)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEStatic message handlers


HBRUSH CEStatic::CtlColor(CDC* pDC, UINT nCtlColor)
{
	if (CTLCOLOR_STATIC == nCtlColor)
	{
		pDC->SetTextColor(m_crTextColor);
		pDC->SetBkMode(TRANSPARENT);
	}

/*	if (m_Type == Background)
	{
		return m_hwndBrush;
	}
*/
	return m_hBrush;
}

CEStatic& CEStatic::SetTextColor(COLORREF crTextColor)
{
	m_crTextColor = crTextColor;
	RedrawWindow();
	return *this;
}

CEStatic& CEStatic::SetBkColor(COLORREF crBkgndColor)
{
	if (m_hBrush)
		::DeleteObject(m_hBrush);
	m_hBrush = ::CreateSolidBrush(crBkgndColor);
	return *this;
}

