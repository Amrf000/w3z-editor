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
// ToolPathEdit.h: interface for the CToolPathEdit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLPATHEDIT_H__E076089E_763A_434B_87C4_AB4787FCBD17__INCLUDED_)
#define AFX_TOOLPATHEDIT_H__E076089E_763A_434B_87C4_AB4787FCBD17__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Tool.h"
class CW3ZMapEditApp;
class CW3ZMapEditDoc;
class CW3ZMapEditView;

class CToolPathEdit : public CTool  
{
public:
	int GetToolSize();
	void SetToolSize(int size);
	void PathWrite(CW3ZMapEditApp *pTheApp, CW3ZMapEditDoc *pDoc, CW3ZMapEditView *pView, int x, int y);
	void SetCursor();
	void OnMouseMove(CW3ZMapEditView *pView, int nFlags, CPoint point);
	void OnLButtonUp(CW3ZMapEditView *pView, int nFlags, CPoint point);
	void OnLButtonDown(CW3ZMapEditView *pView, int nFlags, CPoint point);
	CToolPathEdit();
	virtual ~CToolPathEdit();

protected:
	int m_iSize;
};

#endif // !defined(AFX_TOOLPATHEDIT_H__E076089E_763A_434B_87C4_AB4787FCBD17__INCLUDED_)
