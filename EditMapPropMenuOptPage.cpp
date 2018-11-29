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
// EditMapPropMenuOptPage.cpp : implementation file
//
#include "stdafx.h"
#include "w3zmapedit.h"
#include "EditMapPropMenuOptPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CEditMapPropMenuOptPage property page
IMPLEMENT_DYNCREATE(CEditMapPropMenuOptPage, CPropertyPage)

CEditMapPropMenuOptPage::CEditMapPropMenuOptPage() : CPropertyPageEnh(CEditMapPropMenuOptPage::IDD)
{
	//{{AFX_DATA_INIT(CEditMapPropMenuOptPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CEditMapPropMenuOptPage::~CEditMapPropMenuOptPage()
{
}

void CEditMapPropMenuOptPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEnh::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditMapPropMenuOptPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditMapPropMenuOptPage, CPropertyPageEnh)
	//{{AFX_MSG_MAP(CEditMapPropMenuOptPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditMapPropMenuOptPage message handlers



