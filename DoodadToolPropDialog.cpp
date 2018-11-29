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
// DoodadToolPropDialog.cpp : implementation file
//
#include "stdafx.h"
#include "W3ZMapEdit.h"
#include "DoodadToolPropDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDoodadToolPropDialog dialog
CDoodadToolPropDialog::CDoodadToolPropDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDoodadToolPropDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDoodadToolPropDialog)
	m_iMaxLife = 0;
	m_fMaxScaleX = 0.0f;
	m_fMaxScaleY = 0.0f;
	m_fMaxScaleZ = 0.0f;
	m_iMinLife = 0;
	m_fMinScaleX = 0.0f;
	m_fMinScaleY = 0.0f;
	m_fMinScaleZ = 0.0f;
	m_fSpacing = 0.0f;
	m_sTypeList = _T("");
	m_iVariation = 0;
	m_iSize = 0;
	//}}AFX_DATA_INIT
}


void CDoodadToolPropDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDoodadToolPropDialog)
	DDX_Text(pDX, IDC_EDT_DOODADS_MAX_LIFE, m_iMaxLife);
	DDX_Text(pDX, IDC_EDT_DOODADS_MAXSCALE_X, m_fMaxScaleX);
	DDX_Text(pDX, IDC_EDT_DOODADS_MAXSCALE_Y, m_fMaxScaleY);
	DDX_Text(pDX, IDC_EDT_DOODADS_MAXSCALE_Z, m_fMaxScaleZ);
	DDX_Text(pDX, IDC_EDT_DOODADS_MIN_LIFE, m_iMinLife);
	DDX_Text(pDX, IDC_EDT_DOODADS_MINSCALE_X, m_fMinScaleX);
	DDX_Text(pDX, IDC_EDT_DOODADS_MINSCALE_Y, m_fMinScaleY);
	DDX_Text(pDX, IDC_EDT_DOODADS_MINSCALE_Z, m_fMinScaleZ);
	DDX_Text(pDX, IDC_EDT_DOODADS_SPACING, m_fSpacing);
	DDX_Text(pDX, IDC_EDT_DOODADS_TYPE_LIST, m_sTypeList);
	DDX_Text(pDX, IDC_EDT_DOODADS_VARIATION, m_iVariation);
	DDX_Text(pDX, IDC_EDT_DOODADS_PEN_SIZE, m_iSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDoodadToolPropDialog, CDialog)
	//{{AFX_MSG_MAP(CDoodadToolPropDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDoodadToolPropDialog message handlers
