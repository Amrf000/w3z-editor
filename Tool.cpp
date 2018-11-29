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
// Tool.cpp: implementation of the CTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "W3ZMapEdit.h"
#include "W3ZMapEditView.h"
#include "Tool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTool::CTool()
{
	m_bLeftTracking = m_bRightTracking = FALSE;
	m_ptFirst = m_ptLast = CPoint(0,0);
	m_iToolMode = 0;
}

CTool::~CTool()
{
	m_bLeftTracking = m_bRightTracking = FALSE;
}

void CTool::SetCursor()
{
	::SetCursor(AfxGetApp()->LoadCursor(IDC_PEN));
}

void CTool::OnMouseMove(CW3ZMapEditView *pView, int nFlags, CPoint point)
{

}

void CTool::OnLButtonDown(CW3ZMapEditView *pView, int nFlags, CPoint point)
{
	if (!m_bRightTracking) // s'assure de tracker qu'1 bouton a la fois
	{m_bLeftTracking = TRUE;}
}

void CTool::OnLButtonUp(CW3ZMapEditView *pView, int nFlags, CPoint point)
{
	m_bLeftTracking = FALSE;
}

void CTool::OnRButtonDown(CW3ZMapEditView *pView, int nFlags, CPoint point)
{
	if (!m_bLeftTracking) // s'assure de tracker qu'1 bouton a la fois
	{m_bRightTracking = TRUE;}
}

void CTool::OnRButtonUp(CW3ZMapEditView *pView, int nFlags, CPoint point)
{
	m_bRightTracking = FALSE;
}

void CTool::SetToolMode(int toolmode)
{
	m_iToolMode = toolmode;
}

int CTool::GetToolMode()
{
	return m_iToolMode;
}

void CTool::OnKeyDown(CW3ZMapEditView *pView, UINT nChar, UINT nRepCnt, UINT nFlags)
{

}
