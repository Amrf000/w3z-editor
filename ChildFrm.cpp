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
// ChildFrm.cpp : implementation of the CChildFrame class
//
#include "stdafx.h"
#include "W3ZMapEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "ChildFrm.h"
#include "MainFrm.h"
#include "W3ZMapEditDoc.h"
#include "W3ZMapEditView.h"
#include "W3ZMapOpenGLView.h"


/////////////////////////////////////////////////////////////////////////////
// CChildFrame
IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_SETFOCUS()
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_QUICKBACKUP, OnUpdateFileQuickBackup)
	//}}AFX_MSG_MAP
//	ON_WM_CLOSE()
ON_WM_DESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

void CChildFrame::OnSetFocus(CWnd* pOldWnd) 
{
	CMDIChildWnd::OnSetFocus(pOldWnd);

	CMainFrame *pMainFrame = (CMainFrame*)GetMDIFrame();
	CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)GetActiveDocument();
	POSITION pos = pDoc->GetFirstViewPosition();
	CW3ZMapEditView *pView = (CW3ZMapEditView*)pDoc->GetNextView(pos);
	ASSERT_VALID(pDoc);

	if (pMainFrame)
	{
		if (pView && pDoc)
		{
			pMainFrame->UpdateViewModeBar(pView->m_iViewFlags);
			pMainFrame->UpdateTilesEditBar(pDoc);
			pMainFrame->UpdateDoodadsEditBar(pDoc);
		}
	}
}

void CChildFrame::OnUpdateFileSave(CCmdUI* pCmdUI) 
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

void CChildFrame::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
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

void CChildFrame::OnUpdateFileQuickBackup(CCmdUI* pCmdUI) 
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

CW3ZMapOpenGLView* CChildFrame::GetOpenGLView(void)
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

CW3ZMapEditView* CChildFrame::GetEditView(void)
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

void CChildFrame::OnDestroy()
{
	//+FIXME: active document can not be associated to this frame? --> check!
	CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)GetActiveDocument();
	if (NULL != pDoc)
	{
		ASSERT_VALID(pDoc);
		if (this != pDoc->Get3DChildFrame())
		{
			pDoc->Get3DChildFrame()->DestroyWindow();
		}
	}
	CMDIChildWnd::OnDestroy();
}
