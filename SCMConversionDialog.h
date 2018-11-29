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
#include "afxwin.h"


// Boîte de dialogue CSCMConversionDialog

class CSCMConversionDialog : public CDialog
{
	DECLARE_DYNAMIC(CSCMConversionDialog)

public:
	CSCMConversionDialog(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CSCMConversionDialog();

// Données de boîte de dialogue
	enum { IDD = IDD_DLG_SCM_CONVOPT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bRotate45Deg;
	BOOL m_bAddBoundaries;
	int m_iRatio;
	BOOL m_bSmooth;
	afx_msg void OnBnClickedRadScmAngles();
	afx_msg void OnBnClickedRadScmEven();
	afx_msg void OnBnClickedRadScmSamedim();
	CButton m_btn45Deg;
	virtual BOOL OnInitDialog();
};
