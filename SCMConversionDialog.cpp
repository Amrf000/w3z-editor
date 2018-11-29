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
// SCMConversionDialog.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "W3ZMapEdit.h"
#include "SCMConversionDialog.h"


// CSCMConversionDialog dialog box

IMPLEMENT_DYNAMIC(CSCMConversionDialog, CDialog)
CSCMConversionDialog::CSCMConversionDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSCMConversionDialog::IDD, pParent)
	, m_bRotate45Deg(FALSE)
	, m_bAddBoundaries(FALSE)
	, m_iRatio(0)
	, m_bSmooth(FALSE)
{
}

CSCMConversionDialog::~CSCMConversionDialog()
{
}

void CSCMConversionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHK_SCM_45DEG, m_bRotate45Deg);
	DDX_Check(pDX, IDC_CHK_SCM_BOUNDS, m_bAddBoundaries);
	DDX_Radio(pDX, IDC_RAD_SCM_SAMEDIM, m_iRatio);
	DDX_Check(pDX, IDC_CHK_SCM_SMOOTH, m_bSmooth);
	DDX_Control(pDX, IDC_CHK_SCM_45DEG, m_btn45Deg);
}


BEGIN_MESSAGE_MAP(CSCMConversionDialog, CDialog)
	ON_BN_CLICKED(IDC_RAD_SCM_ANGLES, OnBnClickedRadScmAngles)
	ON_BN_CLICKED(IDC_RAD_SCM_EVEN, OnBnClickedRadScmEven)
	ON_BN_CLICKED(IDC_RAD_SCM_SAMEDIM, OnBnClickedRadScmSamedim)
END_MESSAGE_MAP()


// Gestionnaires de messages CSCMConversionDialog

void CSCMConversionDialog::OnBnClickedRadScmAngles()
{
	m_btn45Deg.EnableWindow(TRUE);
}

void CSCMConversionDialog::OnBnClickedRadScmEven()
{
	m_btn45Deg.EnableWindow(FALSE);
}

void CSCMConversionDialog::OnBnClickedRadScmSamedim()
{
	m_btn45Deg.EnableWindow(FALSE);
}

BOOL CSCMConversionDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (1 == m_iRatio)
	{
		m_btn45Deg.EnableWindow(TRUE);
	}
	else
	{
		m_btn45Deg.EnableWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}
