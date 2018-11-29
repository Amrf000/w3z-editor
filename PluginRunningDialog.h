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
#pragma once


// Boîte de dialogue CPluginRunningDialog

class CPluginRunningDialog : public CDialog
{
	DECLARE_DYNAMIC(CPluginRunningDialog)

public:
	CPluginRunningDialog(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CPluginRunningDialog();

// Données de boîte de dialogue
	enum { IDD = IDD_DLG_PLUGIN_RUN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	HANDLE m_hPluginHandle;
	UINT m_iTimer;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT nIDEvent);
	CString m_strPluginName;
	CString m_strFileName;
	INT_PTR DoModal(HANDLE hPluginHandle, CString strPluginName, CString strFileName);
	virtual BOOL OnInitDialog();
};
