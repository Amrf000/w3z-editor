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
// TilePointToolPropDialog.cpp : implementation file
//

#include "stdafx.h"
#include "W3ZMapEdit.h"
#include "TilePointToolPropDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTilePointToolPropDialog dialog


CTilePointToolPropDialog::CTilePointToolPropDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTilePointToolPropDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTilePointToolPropDialog)
	m_iDetailsRange = 0;
	m_iGrndHeightAmp = 0;
	m_iWaterHeightAmp = 0;
	m_iSize = 0;
	//}}AFX_DATA_INIT
}


void CTilePointToolPropDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTilePointToolPropDialog)
	DDX_Text(pDX, IDC_EDT_DETAILS_RANGE, m_iDetailsRange);
	DDX_Text(pDX, IDC_EDT_GRND_HEIGHT_AMP, m_iGrndHeightAmp);
	DDX_Text(pDX, IDC_EDT_WATER_HEIGHT_AMP, m_iWaterHeightAmp);
	DDX_Text(pDX, IDC_EDT_TILEPOINT_PEN_SIZE, m_iSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTilePointToolPropDialog, CDialog)
	//{{AFX_MSG_MAP(CTilePointToolPropDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTilePointToolPropDialog message handlers
