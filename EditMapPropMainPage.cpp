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
// EditMapPropMainPage.cpp : implementation file
//
#include "stdafx.h"
#include "W3ZMapEdit.h"
#include "EditMapPropMainPage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CEditMapPropMainPage property page
IMPLEMENT_DYNCREATE(CEditMapPropMainPage, CPropertyPage)

CEditMapPropMainPage::CEditMapPropMainPage() : CPropertyPageEnh(CEditMapPropMainPage::IDD)
{
	//{{AFX_DATA_INIT(CEditMapPropMainPage)
	m_sAuthor = _T("");
	m_sCliff = _T("");
	m_sDesc = _T("");
	m_sGround = _T("");
	m_sName = _T("");
	m_sPlayText = _T("");
	m_iEditorVer = 0;
	m_iMapVer = 0;
	m_bCustTilesets = FALSE;
	m_iNbPlayers = 0;
	m_bWavesCliff = FALSE;
	m_bWavesRolling = FALSE;
	m_bUnknown0 = FALSE;
	m_bUnknown1 = FALSE;
	m_bMelee = FALSE;
	//}}AFX_DATA_INIT
	m_cTilesetGroup = TG_CHAR_ID_UNDEFINED;
	m_sOrgGround = "";
	m_sOrgCliff = "";
	m_bIsInited = FALSE;
	m_bIsEnabled = TRUE;
}

CEditMapPropMainPage::~CEditMapPropMainPage()
{
}

void CEditMapPropMainPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEnh::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditMapPropMainPage)
	DDX_Control(pDX, IDC_CMB_MAPTYPE, m_cmbMapType);
	DDX_Text(pDX, IDC_EDT_MAPAUTHOR, m_sAuthor);
	DDX_Text(pDX, IDC_EDT_MAPCLIFF, m_sCliff);
	DDV_MaxChars(pDX, m_sCliff, 64);
	DDX_Text(pDX, IDC_EDT_MAPDESC, m_sDesc);
	DDX_Text(pDX, IDC_EDT_MAPGROUND, m_sGround);
	DDV_MaxChars(pDX, m_sGround, 64);
	DDX_Text(pDX, IDC_EDT_MAPNAME, m_sName);
	DDX_Text(pDX, IDC_EDT_MAPPLAYREC, m_sPlayText);
	DDX_Text(pDX, IDC_EDT_EDITOR_VER, m_iEditorVer);
	DDX_Text(pDX, IDC_EDT_MAP_VER, m_iMapVer);
	DDX_Check(pDX, IDC_CHK_CUSTOM_TILESETS, m_bCustTilesets);
	DDX_Text(pDX, IDC_EDT_MAPNBPLAYERS, m_iNbPlayers);
	DDX_Check(pDX, IDC_CHK_WAVESCLIFF, m_bWavesCliff);
	DDX_Check(pDX, IDC_CHK_WAVESROLLING, m_bWavesRolling);
	DDX_Check(pDX, IDC_CHK_FL_UNKNOWN0, m_bUnknown0);
	DDX_Check(pDX, IDC_CHK_FL_UNKNOWN1, m_bUnknown1);
	DDX_Check(pDX, IDC_CHK_MELEE, m_bMelee);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditMapPropMainPage, CPropertyPageEnh)
	//{{AFX_MSG_MAP(CEditMapPropMainPage)
	ON_CBN_SELCHANGE(IDC_CMB_MAPTYPE, OnSelchangeCmbMapType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditMapPropMainPage message handlers


BOOL CEditMapPropMainPage::OnInitDialog() 
{
	CPropertyPageEnh::OnInitDialog();
	
    CW3ZMapEditApp(* pTheApp) = (CW3ZMapEditApp *)AfxGetApp();
	CTilesetGroup tilesetGroup;
	CString str;
	int i;


	m_sOrgGround = m_sGround;
	m_sOrgCliff = m_sCliff;

	m_cmbMapType.ResetContent();
	str = tilesetGroup.groupName;
	m_cmbMapType.AddString(str);
	m_cmbMapType.SetItemData(0, (DWORD)tilesetGroup.groupID);
	m_cmbMapType.SetCurSel(0);

	pTheApp->m_AppSettings.GoFirstTilesetGroup();
	tilesetGroup = pTheApp->m_AppSettings.GetNextTilesetGroup();
	while (tilesetGroup.groupID != TG_CHAR_ID_UNDEFINED)
	{
		str.Format("%s (%c)", tilesetGroup.groupName, tilesetGroup.groupID);
		m_cmbMapType.AddString(str);
		m_cmbMapType.SetItemData(m_cmbMapType.GetCount() - 1, (DWORD)tilesetGroup.groupID);
		tilesetGroup = pTheApp->m_AppSettings.GetNextTilesetGroup();
	}

	if (m_cTilesetGroup != TG_CHAR_ID_UNDEFINED)
	{
		i = 0;
		while ((i < m_cmbMapType.GetCount()) && (m_cmbMapType.GetItemData(i) != (unsigned)m_cTilesetGroup))
		{i++;}
		if (i < m_cmbMapType.GetCount())
		{
			m_cmbMapType.SetCurSel(i);
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditMapPropMainPage::OnSelchangeCmbMapType() 
{
    CW3ZMapEditApp(* pTheApp) = (CW3ZMapEditApp *)AfxGetApp();
	CTilesetGroup tilesetGroup;
	char groupID;
	int i;
	CString str;

	UpdateData(TRUE);

	i = m_cmbMapType.GetCurSel();
	if (i != CB_ERR)
	{
		groupID = (char) m_cmbMapType.GetItemData(i);
		tilesetGroup = pTheApp->m_AppSettings.GetTilesetGroup(groupID);
		m_cTilesetGroup = tilesetGroup.groupID;
		if (groupID != TG_CHAR_ID_UNDEFINED)
		{
			m_sGround = tilesetGroup.tiles;
			m_sCliff = tilesetGroup.cliffs;
		}
		else
		{
			m_sGround = m_sOrgGround;
			m_sCliff = m_sOrgCliff;
		}
	}

	UpdateData(FALSE);
}

