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
// PluginRunningDialog.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "W3ZMapEdit.h"
#include "PluginRunningDialog.h"
#include ".\pluginrunningdialog.h"


// Boîte de dialogue CPluginRunningDialog

IMPLEMENT_DYNAMIC(CPluginRunningDialog, CDialog)
CPluginRunningDialog::CPluginRunningDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPluginRunningDialog::IDD, pParent)
	, m_strPluginName(_T(""))
	, m_strFileName(_T(""))
{
	m_hPluginHandle = NULL;
	m_iTimer = 0;
}

CPluginRunningDialog::~CPluginRunningDialog()
{
}

void CPluginRunningDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_PLUG_PLUGNAME, m_strPluginName);
	DDX_Text(pDX, IDC_EDT_PLUG_FILENAME, m_strFileName);
}


BEGIN_MESSAGE_MAP(CPluginRunningDialog, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()


void CPluginRunningDialog::OnTimer(UINT nIDEvent)
{
	DWORD dwExitCode;

	// gets process status
	GetExitCodeProcess(m_hPluginHandle, &dwExitCode);
	// check if process ended
	if (STILL_ACTIVE != dwExitCode)
	{
		// process ended, remove timer and end dialog
		KillTimer(m_iTimer);
		EndDialog(IDOK);
	}
	CDialog::OnTimer(nIDEvent);
}

INT_PTR CPluginRunningDialog::DoModal(HANDLE hPluginHandle, CString strPluginName, CString strFileName)
{
	if (NULL == hPluginHandle)
	{
		// no valid plugin handle given, cancel
		return IDCANCEL;
	}
	m_hPluginHandle = hPluginHandle;
	m_strPluginName = strPluginName;
	m_strFileName = strFileName;
	return CDialog::DoModal();
}

BOOL CPluginRunningDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (NULL != m_hPluginHandle)
	{
		// check every 100ms if the plugin is done
		m_iTimer = SetTimer(ID_PLUGIN_TIMER, 100, NULL);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX property pages should return FALSE
}
