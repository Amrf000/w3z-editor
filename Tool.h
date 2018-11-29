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
// Tool.h: interface for the CTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOL_H__550550B9_244C_4616_A639_D3E880355586__INCLUDED_)
#define AFX_TOOL_H__550550B9_244C_4616_A639_D3E880355586__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CW3ZMapEditView;

class CTool  
{
public:
	int GetToolMode();
	void SetToolMode(int toolmode);
	CPoint m_ptLast;
	CPoint m_ptFirst;
	BOOL m_bLeftTracking;
	BOOL m_bRightTracking;

	virtual void SetCursor();
	virtual void OnKeyDown(CW3ZMapEditView *pView, UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnMouseMove(CW3ZMapEditView* pView, int nFlags, CPoint point);
	virtual void OnRButtonUp(CW3ZMapEditView *pView, int nFlags, CPoint point);
	virtual void OnRButtonDown(CW3ZMapEditView *pView, int nFlags, CPoint point);
	virtual void OnLButtonUp(CW3ZMapEditView* pView,int nFlags, CPoint point);
	virtual void OnLButtonDown(CW3ZMapEditView* pView,int nFlags, CPoint point);
	CTool();
	virtual ~CTool();

protected:
	int m_iToolMode;
};

#endif // !defined(AFX_TOOL_H__550550B9_244C_4616_A639_D3E880355586__INCLUDED_)
