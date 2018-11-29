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
// EditMapPropProtectPage.cpp : implementation file
//
#include "stdafx.h"
#include "w3zmapedit.h"
#include "EditMapPropProtectPage.h"
#include "PasswordDialog.h"
#include "W3ZMapEditDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CEditMapPropProtectPage property page
IMPLEMENT_DYNCREATE(CEditMapPropProtectPage, CPropertyPage)

CEditMapPropProtectPage::CEditMapPropProtectPage() : CPropertyPageEnh(CEditMapPropProtectPage::IDD)
{
	//{{AFX_DATA_INIT(CEditMapPropProtectPage)
	m_bEnableProtect = FALSE;
	m_bAuthorMsg = FALSE;
	m_bMakeBackup = FALSE;
	m_bUnpRec = FALSE;
	m_sAuthorPassword = _T("");
	m_sPublicPassword = _T("");
	m_sProtectVer = _T("");
	m_sProtectStat = _T("");
	m_sOrgAuthorName = _T("");
	m_strRegisteredStatus = _T("");
	m_iProtectLvl = -1;
	m_iTypeSelected = -1;
	//}}AFX_DATA_INIT
	m_pDoc = NULL;
	m_bIsRegisterEnabled = TRUE;
//	m_bEnableW3ZLevelRadios = FALSE;
	m_bTooManyTries = FALSE;
}

CEditMapPropProtectPage::~CEditMapPropProtectPage()
{
}

void CEditMapPropProtectPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPageEnh::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditMapPropProtectPage)
	DDX_Control(pDX, IDC_BTN_REGISTER, m_btnRegister);
	DDX_Control(pDX, IDC_CHK_ENABLE_PROTECT, m_chkEnableProtect);
	DDX_Control(pDX, IDC_EDT_ORG_AUTHOR_NAME, m_edtOrgAuthorName);
	DDX_Control(pDX, IDC_EDT_PROTECTPPASS, m_edtPublicPassword);
	DDX_Control(pDX, IDC_EDT_PROTECTAPASS, m_edtAuthorPassword);
	DDX_Control(pDX, IDC_CHK_PROTECTAUTHORMSG, m_chkAuthorMsg);
	DDX_Control(pDX, IDC_CHK_PROTECTBACKUP, m_chkMakeBackup);
	DDX_Control(pDX, IDC_CHK_PROTECTUNPREC, m_chkUnpRec);
	DDX_Control(pDX, IDC_CMB_PROTECTTYPE, m_cmbProtectType);
	DDX_Check(pDX, IDC_CHK_ENABLE_PROTECT, m_bEnableProtect);
	DDX_Check(pDX, IDC_CHK_PROTECTAUTHORMSG, m_bAuthorMsg);
	DDX_Check(pDX, IDC_CHK_PROTECTBACKUP, m_bMakeBackup);
	DDX_Check(pDX, IDC_CHK_PROTECTUNPREC, m_bUnpRec);
	DDX_Text(pDX, IDC_EDT_PROTECTAPASS, m_sAuthorPassword);
	DDV_MaxChars(pDX, m_sAuthorPassword, 16);
	DDX_Text(pDX, IDC_EDT_PROTECTPPASS, m_sPublicPassword);
	DDV_MaxChars(pDX, m_sPublicPassword, 16);
	DDX_Text(pDX, IDC_EDT_PROTECTVER, m_sProtectVer);
	DDX_Text(pDX, IDC_STC_PROTECTSTATUS, m_sProtectStat);
	DDX_Text(pDX, IDC_EDT_ORG_AUTHOR_NAME, m_sOrgAuthorName);
	DDV_MaxChars(pDX, m_sOrgAuthorName, 15);
	DDX_Text(pDX, IDC_STA_REGISTERED, m_strRegisteredStatus);
	DDX_Radio(pDX, IDC_RAD_PROTECTLVL1, m_iProtectLvl);
	DDX_CBIndex(pDX, IDC_CMB_PROTECTTYPE, m_iTypeSelected);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditMapPropProtectPage, CPropertyPageEnh)
	//{{AFX_MSG_MAP(CEditMapPropProtectPage)
	ON_BN_CLICKED(IDC_BTN_REGISTER, OnBtnRegister)
	ON_CBN_SELCHANGE(IDC_CMB_PROTECTTYPE, OnSelChangeCmbProtectType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditMapPropProtectPage message handlers


/*
OnBtnRegister
*************
Displays the password windows that allows the user to register as author or
public user.

*/
void CEditMapPropProtectPage::OnBtnRegister() 
{
	CPasswordDialog dlgPassDialog;
	BOOL bDone;
	int iTries;

	if (TRUE == m_bTooManyTries)
	{
		W3Z_WARNING(STR_WARNING_REGISTER00);
		return;
	}
	iTries = 0;
	if ((NULL != m_pDoc) && (TRUE == m_pDoc->IsProtected()))
	{// the document exists and the map is protected
		bDone = FALSE;
		while (FALSE == bDone) // asks the password
		{
			if (IDCANCEL == dlgPassDialog.DoModal())
			{
				bDone = TRUE;
			}
			else
			{
				// try to register the author password
				if (TRUE == m_pDoc->RegisterAuthorPassword(dlgPassDialog.m_sPassword))
				{// the author password has been verified
					m_strRegisteredStatus = STR_MAP_PROT_AUTHORREGGED;
					bDone = TRUE;
				}
				else if (TRUE == m_pDoc->RegisterPublicPassword(dlgPassDialog.m_sPassword))
				// try to register the public password
				{// the public password has been verified
					m_strRegisteredStatus = STR_MAP_PROT_PUBLICREGGED;
					bDone = TRUE;
				}
				else
				{// none of the password matched
					m_strRegisteredStatus = STR_MAP_PROT_NOTREGGED;
					iTries++;
					if (iTries >= W3Z_MAX_TRIES)
					{//too many tries
						W3Z_WARNING(STR_WARNING_REGISTER00);
						m_bTooManyTries = TRUE;
						bDone = TRUE;
					}
					else
					{
						bDone = FALSE;
					}
				}
			}
		}
	}
	UpdateData(FALSE);// updates register status display
}


/*
OnSelChangeCmbProtectType
*************************
Updates W3Z protection level radio.

*/
void CEditMapPropProtectPage::OnSelChangeCmbProtectType()
{
	UpdateData(TRUE);
	/*
	ComboBox Items Order (m_iTypeSelected):
		0: No protection type set
		1: Unlucky_Footman-Weaaddar Protection
		2: W3Z Map Protection
		3: Unknown protection type
	*/
	if (2 == m_iTypeSelected)
	{
		EnableW3ZLevelRadios(TRUE);
	}
	else
	{
		EnableW3ZLevelRadios(FALSE);
	}
}


/*
EnableW3ZLevelRadios
********************
Enable or disable W3Z Protection Level radios.
Note: it only enables the radios if the selected protection type is W3Z.

In:
 BOOL bEnable: TRUE if the radio must be enabled, FALSE if they must be disabled

Out:
 nothing

Retrun:
 nothing

*/
void CEditMapPropProtectPage::EnableW3ZLevelRadios(BOOL bEnable)
{
	if (TRUE == m_bIsPageInited)
	{
		CButton *pRadProtectLvl1;
		CButton *pRadProtectLvl2;
		CButton *pRadProtectLvl3;
		// gets the radion controls
		pRadProtectLvl1 = (CButton *)GetDlgItem(IDC_RAD_PROTECTLVL1);
		pRadProtectLvl2 = (CButton *)GetDlgItem(IDC_RAD_PROTECTLVL2);
		pRadProtectLvl3 = (CButton *)GetDlgItem(IDC_RAD_PROTECTLVL3);

		// and enable or disable the radios
		if ((NULL !=pRadProtectLvl1) && (NULL != pRadProtectLvl2) && (NULL != pRadProtectLvl3))
		{
			UpdateData(TRUE);
			if ((TRUE == bEnable) && (2 == m_iTypeSelected))
			{
				pRadProtectLvl1->EnableWindow(m_cmbProtectType.IsWindowEnabled());
				pRadProtectLvl2->EnableWindow(m_cmbProtectType.IsWindowEnabled());
				pRadProtectLvl3->EnableWindow(m_cmbProtectType.IsWindowEnabled());
			}
			else
			{
				pRadProtectLvl1->EnableWindow(FALSE);
				pRadProtectLvl2->EnableWindow(FALSE);
				pRadProtectLvl3->EnableWindow(FALSE);
			}
		}
	}
/*
	else
	{
		m_bEnableW3ZLevelRadios = bEnable;
	}
*/
}



BOOL CEditMapPropProtectPage::OnInitDialog() 
{
	CPropertyPageEnh::OnInitDialog();

	// add register button independant support
	EnableW3ZLevelRadios(m_bIsEnabled);
	EnableRegisterBtn(m_bIsRegisterEnabled);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/*
EnablePageItems
***************
Enable or disable the dialog items.

In:
  BOOL bEnable: set to TRUE enables the dialog items, set to FALSE disables them

Out:
  nothing

Return:
  nothing

*/
void CEditMapPropProtectPage::EnablePageItems(BOOL bEnable)
{
	CPropertyPageEnh::EnablePageItems(bEnable);
	// handles W3Z protection level radios...
	if (TRUE == m_bIsPageInited)
	{
		if (2 == m_iTypeSelected)
		{
			EnableW3ZLevelRadios(bEnable);
		}
		else
		{
			EnableW3ZLevelRadios(FALSE);
		}
	}
}


/*
EnableRegisterBtn
*****************
Enables or disable the register user button.

In:
  BOOL bEnable: enable button if TRUE, disable the button otherwise

Out:
  nothing

Return:
  nothing

*/
void CEditMapPropProtectPage::EnableRegisterBtn(BOOL bEnable)
{
	if (TRUE == m_bIsPageInited)
	{
		m_btnRegister.EnableWindow(bEnable);
	}
	else
	{
		m_bIsRegisterEnabled = bEnable;
	}
}
