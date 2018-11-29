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
// EditMapPropSizePage.cpp : implementation file
//
#include "stdafx.h"
#include "w3zmapedit.h"
#include "EditMapPropSizePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CEditMapPropSizePage property page
IMPLEMENT_DYNCREATE(CEditMapPropSizePage, CPropertyPage)

CEditMapPropSizePage::CEditMapPropSizePage() : CPropertyPageEnh(CEditMapPropSizePage::IDD)
{
	//{{AFX_DATA_INIT(CEditMapPropSizePage)
	m_iHeight = 0;
	m_iWidth = 0;
	m_iPlayableHeight = 0;
	m_iPlayableWidth = 0;
	m_fCBblx = 0.0f;
	m_fCBbly = 0.0f;
	m_fCBbrx = 0.0f;
	m_fCBbry = 0.0f;
	m_fCBtlx = 0.0f;
	m_fCBtly = 0.0f;
	m_fCBtrx = 0.0f;
	m_fCBtry = 0.0f;
	//}}AFX_DATA_INIT
}

CEditMapPropSizePage::~CEditMapPropSizePage()
{
}

void CEditMapPropSizePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEnh::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditMapPropSizePage)
	DDX_Text(pDX, IDC_EDT_MAPHEIGHT, m_iHeight);
	DDV_MinMaxUInt(pDX, m_iHeight, 16, 512);
	DDX_Text(pDX, IDC_EDT_MAPWIDTH, m_iWidth);
	DDV_MinMaxUInt(pDX, m_iWidth, 16, 512);
	DDX_Text(pDX, IDC_EDT_PLAY_HEIGHT, m_iPlayableHeight);
	DDX_Text(pDX, IDC_EDT_PLAY_WIDTH, m_iPlayableWidth);
	DDX_Text(pDX, IDC_EDT_BOTTOM_LEFT_X, m_fCBblx);
	DDX_Text(pDX, IDC_EDT_BOTTOM_LEFT_Y, m_fCBbly);
	DDX_Text(pDX, IDC_EDT_BOTTOM_RIGHT_X, m_fCBbrx);
	DDX_Text(pDX, IDC_EDT_BOTTOM_RIGHT_Y, m_fCBbry);
	DDX_Text(pDX, IDC_EDT_TOP_LEFT_X, m_fCBtlx);
	DDX_Text(pDX, IDC_EDT_TOP_LEFT_Y, m_fCBtly);
	DDX_Text(pDX, IDC_EDT_TOP_RIGHT_X, m_fCBtrx);
	DDX_Text(pDX, IDC_EDT_TOP_RIGHT_Y, m_fCBtry);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditMapPropSizePage, CPropertyPageEnh)
	//{{AFX_MSG_MAP(CEditMapPropSizePage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditMapPropSizePage message handlers





