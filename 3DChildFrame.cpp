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
// ChildFrm.cpp : implementation of the C3DChildFrame class
//
#include "stdafx.h"
#include "W3ZMapEdit.h"
#include "3DChildFrame.h"
#include "ChildFrm.h"
#include "MainFrm.h"
#include "W3ZMapEditDoc.h"
#include "W3ZMapEditView.h"
#include "W3ZMapOpenGLView.h"


/////////////////////////////////////////////////////////////////////////////
// C3DChildFrame
IMPLEMENT_DYNCREATE(C3DChildFrame, CChildFrame)

BEGIN_MESSAGE_MAP(C3DChildFrame, CChildFrame)
	//{{AFX_MSG_MAP(C3DChildFrame)
//	ON_WM_SETFOCUS()
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_QUICKBACKUP, OnUpdateFileQuickBackup)
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
//	ON_COMMAND(ID_VIEW_3DVIEW, OnView3dview)
//	ON_UPDATE_COMMAND_UI(ID_VIEW_3DVIEW, OnUpdateView3dview)
//ON_WM_CREATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3DChildFrame construction/destruction

C3DChildFrame::C3DChildFrame()
{
}

C3DChildFrame::~C3DChildFrame()
{
}

BOOL C3DChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CChildFrame::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// C3DChildFrame diagnostics

#ifdef _DEBUG
void C3DChildFrame::AssertValid() const
{
	CChildFrame::AssertValid();
}

void C3DChildFrame::Dump(CDumpContext& dc) const
{
	CChildFrame::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// C3DChildFrame message handlers
void C3DChildFrame::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)GetActiveDocument();
	ASSERT_VALID(pDoc);
	// if the map is protected and the user is not registered as the author, disable save
	if (pDoc && (pDoc->IsProtected()) && (FALSE == pDoc->IsAuthorRegistered()))
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

void C3DChildFrame::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)GetActiveDocument();
	ASSERT_VALID(pDoc);
	// if the map is protected and the user is not registered as the author, disable save
	if (pDoc && (pDoc->IsProtected()) && (FALSE == pDoc->IsAuthorRegistered()))
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

void C3DChildFrame::OnUpdateFileQuickBackup(CCmdUI* pCmdUI) 
{
	CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)GetActiveDocument();
	ASSERT_VALID(pDoc);
	// if the map is protected and the user is not registered as the author, disable save
	if (pDoc && (pDoc->IsProtected()) && (FALSE == pDoc->IsAuthorRegistered()))
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

CW3ZMapOpenGLView* C3DChildFrame::GetOpenGLView(void)
{
	CW3ZMapEditView	*pActiveView = NULL;
	CW3ZMapOpenGLView *pOpenGLView = NULL;
	CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)GetActiveDocument();
	if (NULL != pDoc)
	{
		ASSERT_VALID(pDoc);
		POSITION pos = pDoc->GetFirstViewPosition();
		pActiveView = (CW3ZMapEditView*)pDoc->GetNextView(pos);
		pOpenGLView = (CW3ZMapOpenGLView*)pDoc->GetNextView(pos);
	}
	return pOpenGLView;
}

CW3ZMapEditView* C3DChildFrame::GetEditView(void)
{
	CW3ZMapEditView	*pActiveView = NULL;
	CW3ZMapOpenGLView *pOpenGLView = NULL;
	CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)GetActiveDocument();
	if (NULL != pDoc)
	{
		ASSERT_VALID(pDoc);
		POSITION pos = pDoc->GetFirstViewPosition();
		pActiveView = (CW3ZMapEditView*)pDoc->GetNextView(pos);
	}
	return pActiveView;
}

void C3DChildFrame::OnClose()
{
// disables "x" button
// CChildFrame::OnClose();
// hides instead
	ShowWindow(SW_HIDE);
}