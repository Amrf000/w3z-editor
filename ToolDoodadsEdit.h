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
// ToolDoodadsEdit.h: interface for the CToolDoodadsEdit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLDOODADSEDIT_H__A1D4EEA1_8DA9_4A3C_8B3E_00B298B15C76__INCLUDED_)
#define AFX_TOOLDOODADSEDIT_H__A1D4EEA1_8DA9_4A3C_8B3E_00B298B15C76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Tool.h"
#include "Doodad.h"
class CW3ZMapEditApp;
class CW3ZMapEditDoc;
class CW3ZMapEditView;

class CToolDoodadsEdit : public CTool  
{
public:
	void SetToolSize(float size);
	float GetToolSize();
	void DoodadUpdate(CW3ZMapEditApp *pTheApp, CW3ZMapEditDoc *pDoc, CW3ZMapEditView *pView, float x, float y);
	void DoodadRemove(CW3ZMapEditApp *pTheApp, CW3ZMapEditDoc *pDoc, CW3ZMapEditView *pView, float x, float y);
	float m_fMinDist;
	void DoodadWrite(CW3ZMapEditApp *pTheApp, CW3ZMapEditDoc *pDoc, CW3ZMapEditView *pView, float x, float y);
	void SetCursor();
	void OnMouseMove(CW3ZMapEditView *pView, int nFlags, CPoint point);
	void OnRButtonUp(CW3ZMapEditView *pView, int nFlags, CPoint point);
	void OnRButtonDown(CW3ZMapEditView *pView, int nFlags, CPoint point);
	void OnLButtonUp(CW3ZMapEditView *pView, int nFlags, CPoint point);
	void OnLButtonDown(CW3ZMapEditView *pView, int nFlags, CPoint point);
	CToolDoodadsEdit();
	virtual ~CToolDoodadsEdit();

protected:
	float m_iSize;

};

#endif // !defined(AFX_TOOLDOODADSEDIT_H__A1D4EEA1_8DA9_4A3C_8B3E_00B298B15C76__INCLUDED_)
