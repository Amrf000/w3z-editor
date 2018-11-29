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
// PropertyPageEnh.cpp : implementation file
//

#include "stdafx.h"
#include "PropertyPageEnh.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageEnh property page

//IMPLEMENT_DYNCREATE(CPropertyPageEnh, CPropertyPage)

CPropertyPageEnh::CPropertyPageEnh(UINT uIDTemplate) : CPropertyPage(uIDTemplate)
{
	m_bIsPageInited = FALSE;
	m_bIsEnabled = TRUE;
}

CPropertyPageEnh::~CPropertyPageEnh()
{
}

void CPropertyPageEnh::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyPageEnh)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPropertyPageEnh, CPropertyPage)
	//{{AFX_MSG_MAP(CPropertyPageEnh)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageEnh message handlers

BOOL CPropertyPageEnh::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	//this code hides the dialog items disabled and not implemented
	CWnd* pWnd;
	pWnd = GetTopWindow();
	while (NULL != pWnd)
	{
		if (FALSE == pWnd->IsWindowEnabled())
		{
			pWnd->ShowWindow(SW_HIDE);
		}
		pWnd = pWnd->GetNextWindow();
	}
	// dialog has just been inited
	m_bIsPageInited = TRUE;
	EnablePageItems(m_bIsEnabled);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropertyPageEnh::OnDestroy() 
{
	CPropertyPage::OnDestroy();
	
	// dialog is destroyed so the it'll need to be inited again
	m_bIsPageInited = FALSE;
	
}

/*
EnablePageItems
***************
Enable (bEnable=TRUE) or disable(bEnable=FALSE) all the property page items.

In:
 BOOL bEnable: enable items if set to TRUE, disable them otherwise

Out:
 nothing

Return:
 nothing

*/
void CPropertyPageEnh::EnablePageItems(BOOL bEnable)
{
	if (TRUE == m_bIsPageInited)
	{
		CWnd* pWnd;
		pWnd = GetTopWindow();
		while (NULL != pWnd)
		{
			pWnd->EnableWindow(bEnable);
			pWnd = pWnd->GetNextWindow();
		}
	}
	else
	{
		m_bIsEnabled = bEnable;
	}
}


/*
IsEnabled
*********
Says if the property page is enabled or not.

In:
 nothing

Out:
 nothing

Return:
 BOOL: TRUE if the page is enabled

*/
BOOL CPropertyPageEnh::IsEnabled()
{
	return m_bIsEnabled;
}
