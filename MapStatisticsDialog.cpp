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
// MapStatisticsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "w3zmapedit.h"
#include "MapStatisticsDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapStatisticsDialog dialog


CMapStatisticsDialog::CMapStatisticsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMapStatisticsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMapStatisticsDialog)
	m_sStats = _T("");
	//}}AFX_DATA_INIT
}


void CMapStatisticsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMapStatisticsDialog)
	DDX_Text(pDX, IDC_EDT_STATISTICS, m_sStats);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMapStatisticsDialog, CDialog)
	//{{AFX_MSG_MAP(CMapStatisticsDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapStatisticsDialog message handlers
