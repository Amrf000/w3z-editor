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
// EditMapPropSheet.cpp : implementation file
//
#include "stdafx.h"
#include "W3ZMapEdit.h"
#include "EditMapPropSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CEditMapPropSheet
IMPLEMENT_DYNAMIC(CEditMapPropSheet, CPropertySheet)

CEditMapPropSheet::CEditMapPropSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_iFeaturesFlag = W3Z_PROP_ALLENABLED;
	m_bIsInited = FALSE;
	AddPages();
}

CEditMapPropSheet::CEditMapPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_iFeaturesFlag = W3Z_PROP_ALLENABLED;
	m_bIsInited = FALSE;
	AddPages();
}

CEditMapPropSheet::~CEditMapPropSheet()
{
}


BEGIN_MESSAGE_MAP(CEditMapPropSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CEditMapPropSheet)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditMapPropSheet message handlers

void CEditMapPropSheet::AddPages()
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_psh.dwFlags |= PSP_USEHICON;
	m_psh.hIcon = m_hIcon;
	m_psh.dwFlags |= PSH_NOAPPLYNOW;    // Lose the Apply Now button
	m_psh.dwFlags &= ~PSH_HASHELP;  // Lose the Help button

	AddPage(&m_MapPropMainPage);
	AddPage(&m_MapPropSizePage);
	AddPage(&m_MapPropMinimapPage);
	AddPage(&m_MapPropMenuOptPage);
	AddPage(&m_MapPropPlayersPage);
	AddPage(&m_MapPropProtectPage);
}

BOOL CEditMapPropSheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	m_bIsInited	= TRUE;
	SetIcon(m_hIcon, FALSE);
	EnableFeatures(m_iFeaturesFlag);
	return bResult;
}

/*
EnableFeatures
**************
Enable or disable option pages.

In:
 int iFlags: page flags:
		const int W3Z_PROP_ALLDISABLED		= 0x00000000;
		const int W3Z_PROP_REGISTER			= 0x00000001;
		const int W3Z_PROP_PROTECT			= 0x00000002;
		const int W3Z_PROP_MAIN				= 0x00000004;
		const int W3Z_PROP_SIZE				= 0x00000008;
		const int W3Z_PROP_MINIMAP			= 0x00000010;
		const int W3Z_PROP_MENUOPT			= 0x00000020;
		const int W3Z_PROP_PLAYERS			= 0x00000040;
		const int W3Z_PROP_ALLNOPROTECT		= 0xFFFFFFFD;
		const int W3Z_PROP_ALLNOREGISTER	= 0xFFFFFFFE;
		const int W3Z_PROP_ALLENABLED		= 0xFFFFFFFF;

Out:
  nothing

Return:
  nothing

*/
void CEditMapPropSheet::EnableFeatures(int iFlags)
{
	if (TRUE == m_bIsInited)
	{
		// main page
		if (0 != (W3Z_PROP_MAIN & iFlags))
		{
			m_MapPropMainPage.EnablePageItems(TRUE);
		}
		else
		{
			m_MapPropMainPage.EnablePageItems(FALSE);
		}

		// size page
		if (0 != (W3Z_PROP_SIZE & iFlags))
		{
			m_MapPropSizePage.EnablePageItems(TRUE);
		}
		else
		{
			m_MapPropSizePage.EnablePageItems(FALSE);
		}

		// minimap page
		if (0 != (W3Z_PROP_MINIMAP & iFlags))
		{
			m_MapPropMinimapPage.EnablePageItems(TRUE);
		}
		else
		{
			m_MapPropMinimapPage.EnablePageItems(FALSE);
		}

		// menu option page
		if (0 != (W3Z_PROP_MENUOPT & iFlags))
		{
			m_MapPropMenuOptPage.EnablePageItems(TRUE);
		}
		else
		{
			m_MapPropMenuOptPage.EnablePageItems(FALSE);
		}

		// players page
		if (0 != (W3Z_PROP_PLAYERS & iFlags))
		{
			m_MapPropPlayersPage.EnablePageItems(TRUE);
		}
		else
		{
			m_MapPropPlayersPage.EnablePageItems(FALSE);
		}

		// protection page
		if (0 != (W3Z_PROP_PROTECT & iFlags))
		{
			m_MapPropProtectPage.EnablePageItems(TRUE);
		}
		else
		{
			m_MapPropProtectPage.EnablePageItems(FALSE);
		}
		if (0 != (W3Z_PROP_REGISTER & iFlags))
		{
			m_MapPropProtectPage.EnableRegisterBtn(TRUE);
		}
		else
		{
			m_MapPropProtectPage.EnableRegisterBtn(FALSE);
		}
	}
	else
	{
		// save status for next initialisation
		m_iFeaturesFlag = iFlags;
	}
}

void CEditMapPropSheet::OnDestroy() 
{
	CPropertySheet::OnDestroy();
	
	m_bIsInited	= FALSE;
	
}
