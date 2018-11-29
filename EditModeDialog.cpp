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
// EditModeDialog.cpp : implementation file
//
#include "stdafx.h"
#include "W3ZMapEdit.h"
#include "EditModeDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CEditModeDialog dialog
CEditModeDialog::CEditModeDialog(CWnd* pParent /*=NULL*/)
	: CDialogBar(/*CEditModeDialog::IDD, pParent*/)
{
	//{{AFX_DATA_INIT(CEditModeDialog)
	m_iEditMode = 0;
	//}}AFX_DATA_INIT
    m_pTheApp = (CW3ZMapEditApp *)AfxGetApp();
}


void CEditModeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditModeDialog)
	DDX_CBIndex(pDX, IDC_EDITMODE, m_iEditMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditModeDialog, CDialogBar)
	//{{AFX_MSG_MAP(CEditModeDialog)
	ON_CBN_SELCHANGE(IDC_EDITMODE, OnSelChangeEditMode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditModeDialog message handlers


void CEditModeDialog::OnSelChangeEditMode() 
{
	UpdateData(TRUE);
	if (m_pTheApp)
	{m_pTheApp->SetEditMode(m_iEditMode);}
}

/*
Update displayed mode in the DropDownList from the mode used in the app
*/
void CEditModeDialog::UpdateEditMode()
{
	if (m_pTheApp)
	{
		m_iEditMode = m_pTheApp->GetEditMode();
		UpdateData(FALSE);
	}
}

/*
Update displayed mode in the DropDownList using the specified mode

Input:
-iMode: use constants defined in W3ZMapEdit.h
   (MODE_DEFAULT, MODE_TILEPOINTS, MODE_DOODADS,...)

*/
void CEditModeDialog::UpdateEditMode(int iMode)
{
		m_iEditMode = iMode;
		UpdateData(FALSE);
}











