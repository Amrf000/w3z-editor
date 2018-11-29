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
// RunPluginDialog.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "W3ZMapEdit.h"
#include "RunPluginDialog.h"
#include ".\runplugindialog.h"


// Boîte de dialogue CRunPluginDialog

IMPLEMENT_DYNAMIC(CRunPluginDialog, CDialog)
CRunPluginDialog::CRunPluginDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRunPluginDialog::IDD, pParent)
	, m_strSelectedPluginName(_T(""))
{
}

CRunPluginDialog::~CRunPluginDialog()
{
}

void CRunPluginDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_PLUGIN_LIST, m_cmbPluginList);
	DDX_CBString(pDX, IDC_CMB_PLUGIN_LIST, m_strSelectedPluginName);
	DDX_Control(pDX, IDOK, m_btnRun);
}


BEGIN_MESSAGE_MAP(CRunPluginDialog, CDialog)
END_MESSAGE_MAP()


// Messages handler CRunPluginDialog

BOOL CRunPluginDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

    CW3ZMapEditApp(* pTheApp) = (CW3ZMapEditApp *)AfxGetApp();
	CString strList, strItem;
	int i=0;

	m_cmbPluginList.ResetContent();
	strList = pTheApp->m_AppSettings.GetMenuPluginList();
	strItem = strList.Tokenize("\n", i);
	while ("" != strItem)
	{
		m_cmbPluginList.AddString(strItem);
		strItem = strList.Tokenize("\n", i);
	}
	if (0 >= m_cmbPluginList.GetCount())
	{
		// no items
		m_cmbPluginList.EnableWindow(FALSE);
		m_btnRun.EnableWindow(FALSE);
	}
	else
	{
		m_cmbPluginList.EnableWindow(TRUE);
		m_btnRun.EnableWindow(TRUE);
		m_cmbPluginList.SetCurSel(0);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}
