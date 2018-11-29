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
// PathDialog.cpp : implementation file
//

#include "stdafx.h"
#include "w3zmapedit.h"
#include "PathDialog.h"
#include ".\pathdialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPathDialog dialog


CPathDialog::CPathDialog(CWnd* pParent /*=NULL*/)
	: CDialogBar(/*CPathDialog::IDD, pParent*/)
{
	//{{AFX_DATA_INIT(CPathDialog)
	m_iPenSize = 0;
	//}}AFX_DATA_INIT
}


void CPathDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPathDialog)
	DDX_Control(pDX, IDC_STC_PATH_CURRENT, m_staCurrent);
	DDX_Control(pDX, IDC_STC_PATH_NOFLY, m_staNoFly);
	DDX_Control(pDX, IDC_STC_PATH_UNKNOWN, m_staUnknown);
	DDX_Control(pDX, IDC_STC_PATH_UNKN01, m_staUnkn01);
	DDX_Control(pDX, IDC_STC_PATH_UNKN02, m_staUnkn02);
	DDX_Control(pDX, IDC_STC_PATH_BLIGHT, m_staBlight);
	DDX_Control(pDX, IDC_STA_PATH_NOWATER, m_staNoWater);
	DDX_Control(pDX, IDC_STA_PATH_NOWALK, m_staNoWalk);
	DDX_Control(pDX, IDC_STA_PATH_NOBUILD, m_staNoBuild);
	DDX_Control(pDX, IDC_BTN_PATH_UNKNOWN, m_btnUnknown);
	DDX_Control(pDX, IDC_BTN_PATH_UNKN01, m_btnUnkn01);
	DDX_Control(pDX, IDC_BTN_PATH_UNKN02, m_btnUnkn02);
	DDX_Control(pDX, IDC_BTN_PATH_NOWATER, m_btnNoWater);
	DDX_Control(pDX, IDC_BTN_PATH_NOWALK, m_btnNoWalk);
	DDX_Control(pDX, IDC_BTN_PATH_NOFLY, m_btnNoFly);
	DDX_Control(pDX, IDC_BTN_PATH_NOBUILD, m_btnNoBuild);
	DDX_Control(pDX, IDC_BTN_PATH_BLIGHT, m_btnBlight);
	DDX_Text(pDX, IDC_EDT_PATH_PEN_SIZE, m_iPenSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPathDialog, CDialogBar)
	//{{AFX_MSG_MAP(CPathDialog)
	ON_BN_CLICKED(IDC_BTN_PATH_NOWALK, OnBtnNoWalk)
	ON_BN_CLICKED(IDC_BTN_PATH_NOBUILD, OnBtnNoBuild)
	ON_BN_CLICKED(IDC_BTN_PATH_NOFLY, OnBtnNoFly)
	ON_BN_CLICKED(IDC_BTN_PATH_UNKNOWN, OnBtnUnknown)
	ON_BN_CLICKED(IDC_BTN_PATH_UNKN01, OnBtnUnkn01)
	ON_BN_CLICKED(IDC_BTN_PATH_UNKN02, OnBtnUnkn02)
	ON_BN_CLICKED(IDC_BTN_PATH_NOWATER, OnBtnNoWater)
	ON_BN_CLICKED(IDC_BTN_PATH_BLIGHT, OnBtnBlight)
	ON_EN_KILLFOCUS(IDC_EDT_PATH_PEN_SIZE, OnKillFocusEdtPathPenSize)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPathDialog message handlers

void CPathDialog::InitDialogBar()
{
	CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
	ASSERT_VALID(pTheApp);
	m_iPenSize = pTheApp->m_ToolPathEdit.GetToolSize();
	UpdateData(FALSE);

	m_btnNoWalk.SetTriState(TRUE);
	m_btnNoWalk.SetTextOnly(TRUE);
	m_btnNoWalk.SetWindowText(STR_PATH_NOWALK);
	m_btnNoWalk.SetState(EB_STATE0);
	m_btnNoWalk.SetColorState2(0x0000C000);

	m_btnNoBuild.SetTriState(TRUE);
	m_btnNoBuild.SetTextOnly(TRUE);
	m_btnNoBuild.SetState(EB_STATE0);
	m_btnNoBuild.SetColorState2(0x0000C000);

	m_btnNoFly.SetTriState(TRUE);
	m_btnNoFly.SetTextOnly(TRUE);
	m_btnNoFly.SetState(EB_STATE0);
	m_btnNoFly.SetColorState2(0x0000C000);

	m_btnBlight.SetTriState(TRUE);
	m_btnBlight.SetTextOnly(TRUE);
	m_btnBlight.SetState(EB_STATE0);
	m_btnBlight.SetColorState2(0x0000C000);

	m_btnNoWater.SetTriState(TRUE);
	m_btnNoWater.SetTextOnly(TRUE);
	m_btnNoWater.SetState(EB_STATE0);
	m_btnNoWater.SetColorState2(0x0000C000);

	m_btnUnknown.SetTriState(TRUE);
	m_btnUnknown.SetTextOnly(TRUE);
	m_btnUnknown.SetState(EB_STATE0);
	m_btnUnknown.SetColorState2(0x0000C000);

	m_btnUnkn01.SetTriState(TRUE);
	m_btnUnkn01.SetTextOnly(TRUE);
	m_btnUnkn01.SetState(EB_STATE0);
	m_btnUnkn01.SetColorState2(0x0000C000);

	m_btnUnkn02.SetTriState(TRUE);
	m_btnUnkn02.SetTextOnly(TRUE);
	m_btnUnkn02.SetState(EB_STATE0);
	m_btnUnkn02.SetColorState2(0x0000C000);

	UpdateColors();
	UpdateCurrentColor();
}

void CPathDialog::OnUpdateCmdUI(CFrameWnd *pTarget, BOOL bDisableIfNoHndler)
{
 // disable auto-disabling buttons
}


void CPathDialog::OnBtnNoWalk() 
{
	CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
	ASSERT_VALID(pTheApp);
	pTheApp->OnBtnPathBar();

	if (m_btnNoWalk.GetState() == EB_STATE2)
	{
		m_btnNoWalk.SetWindowText(STR_PATH_WALK);
	}
	else
	{
		m_btnNoWalk.SetWindowText(STR_PATH_NOWALK);
	}
	UpdateCurrentColor();
}

void CPathDialog::OnBtnNoBuild() 
{
	CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
	ASSERT_VALID(pTheApp);
	pTheApp->OnBtnPathBar();

	if (m_btnNoBuild.GetState() == EB_STATE2)
	{
		m_btnNoBuild.SetWindowText(STR_PATH_BUILD);
	}
	else
	{
		m_btnNoBuild.SetWindowText(STR_PATH_NOBUILD);
	}
	UpdateCurrentColor();
}

void CPathDialog::OnBtnNoFly() 
{
	CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
	ASSERT_VALID(pTheApp);
	pTheApp->OnBtnPathBar();

	if (m_btnNoFly.GetState() == EB_STATE2)
	{
		m_btnNoFly.SetWindowText(STR_PATH_FLY);
	}
	else
	{
		m_btnNoFly.SetWindowText(STR_PATH_NOFLY);
	}
	UpdateCurrentColor();
}

void CPathDialog::OnBtnUnknown() 
{
	CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
	ASSERT_VALID(pTheApp);
	pTheApp->OnBtnPathBar();

	if (m_btnUnknown.GetState() == EB_STATE2)
	{
		m_btnUnknown.SetWindowText(STR_PATH_NOUNK);
	}
	else
	{
		m_btnUnknown.SetWindowText(STR_PATH_UNK);
	}
	UpdateCurrentColor();
}

void CPathDialog::OnBtnUnkn01() 
{
	CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
	ASSERT_VALID(pTheApp);
	pTheApp->OnBtnPathBar();

	if (m_btnUnkn01.GetState() == EB_STATE2)
	{
		m_btnUnkn01.SetWindowText(STR_PATH_NOUNK01);
	}
	else
	{
		m_btnUnkn01.SetWindowText(STR_PATH_UNK01);
	}
	UpdateCurrentColor();
}

void CPathDialog::OnBtnUnkn02() 
{
	CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
	ASSERT_VALID(pTheApp);
	pTheApp->OnBtnPathBar();

	if (m_btnUnkn02.GetState() == EB_STATE2)
	{
		m_btnUnkn02.SetWindowText(STR_PATH_NOUNK02);
	}
	else
	{
		m_btnUnkn02.SetWindowText(STR_PATH_UNK02);
	}
	UpdateCurrentColor();
}

void CPathDialog::OnBtnNoWater() 
{
	CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
	ASSERT_VALID(pTheApp);
	pTheApp->OnBtnPathBar();

	if (m_btnNoWater.GetState() == EB_STATE2)
	{
		m_btnNoWater.SetWindowText(STR_PATH_WATER);
	}
	else
	{
		m_btnNoWater.SetWindowText(STR_PATH_NOWATER);
	}
	UpdateCurrentColor();
}

void CPathDialog::OnBtnBlight() 
{
	CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
	ASSERT_VALID(pTheApp);
	pTheApp->OnBtnPathBar();

	if (m_btnBlight.GetState() == EB_STATE2)
	{
		m_btnBlight.SetWindowText(STR_PATH_NOBLIGHT);
	}
	else
	{
		m_btnBlight.SetWindowText(STR_PATH_BLIGHT);
	}
	UpdateCurrentColor();
}

int CPathDialog::GetNewFlags(int oldflags)
{
	int newflags;
	newflags = oldflags;

	if (m_btnNoWalk.GetState() == EB_STATE1)
	{
		newflags |= PATH_NOWALK;
	}
	else if (m_btnNoWalk.GetState() == EB_STATE2)
	{
		newflags &= ~PATH_NOWALK;
	}

	if (m_btnNoBuild.GetState() == EB_STATE1)
	{
		newflags |= PATH_NOBUILD;
	}
	else if (m_btnNoBuild.GetState() == EB_STATE2)
	{
		newflags &= ~PATH_NOBUILD;
	}

	if (m_btnNoFly.GetState() == EB_STATE1)
	{
		newflags |= PATH_NOFLY;
	}
	else if (m_btnNoFly.GetState() == EB_STATE2)
	{
		newflags &= ~PATH_NOFLY;
	}

	if (m_btnBlight.GetState() == EB_STATE1)
	{
		newflags |= PATH_BLIGHT;
	}
	else if (m_btnBlight.GetState() == EB_STATE2)
	{
		newflags &= ~PATH_BLIGHT;
	}

	if (m_btnNoWater.GetState() == EB_STATE1)
	{
		newflags |= PATH_NOWATER;
	}
	else if (m_btnNoWater.GetState() == EB_STATE2)
	{
		newflags &= ~PATH_NOWATER;
	}

	if (m_btnUnknown.GetState() == EB_STATE1)
	{
		newflags |= PATH_UNKNOWN;
	}
	else if (m_btnUnknown.GetState() == EB_STATE2)
	{
		newflags &= ~PATH_UNKNOWN;
	}

	if (m_btnUnkn01.GetState() == EB_STATE1)
	{
		newflags |= PATH_UNKN01;
	}
	else if (m_btnUnkn01.GetState() == EB_STATE2)
	{
		newflags &= ~PATH_UNKN01;
	}

	if (m_btnUnkn02.GetState() == EB_STATE1)
	{
		newflags |= PATH_UNKN02;
	}
	else if (m_btnUnkn02.GetState() == EB_STATE2)
	{
		newflags &= ~PATH_UNKN02;
	}

	return newflags;
}


void CPathDialog::UpdateColors()
{
	COLORREF color;
	CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
	ASSERT_VALID(pTheApp);

	color = pTheApp->m_AppSettings.pathNoWalk.clrBlue * 0x0010000
		+ pTheApp->m_AppSettings.pathNoWalk.clrGreen * 0x00000100
		+ pTheApp->m_AppSettings.pathNoWalk.clrRed;
	m_staNoWalk.SetBkColor(color);

	color = pTheApp->m_AppSettings.pathNoBuild.clrBlue * 0x0010000
		+ pTheApp->m_AppSettings.pathNoBuild.clrGreen * 0x00000100
		+ pTheApp->m_AppSettings.pathNoBuild.clrRed;
	m_staNoBuild.SetBkColor(color);

	color = pTheApp->m_AppSettings.pathNoFly.clrBlue * 0x0010000
		+ pTheApp->m_AppSettings.pathNoFly.clrGreen * 0x00000100
		+ pTheApp->m_AppSettings.pathNoFly.clrRed;
	m_staNoFly.SetBkColor(color);

	color = pTheApp->m_AppSettings.pathBlight.clrBlue * 0x0010000
		+ pTheApp->m_AppSettings.pathBlight.clrGreen * 0x00000100
		+ pTheApp->m_AppSettings.pathBlight.clrRed;
	m_staBlight.SetBkColor(color);

	color = pTheApp->m_AppSettings.pathNoWater.clrBlue * 0x0010000
		+ pTheApp->m_AppSettings.pathNoWater.clrGreen * 0x00000100
		+ pTheApp->m_AppSettings.pathNoWater.clrRed;
	m_staNoWater.SetBkColor(color);

	color = pTheApp->m_AppSettings.pathUnknown.clrBlue * 0x0010000
		+ pTheApp->m_AppSettings.pathUnknown.clrGreen * 0x00000100
		+ pTheApp->m_AppSettings.pathUnknown.clrRed;
	m_staUnknown.SetBkColor(color);

	color = pTheApp->m_AppSettings.pathUnkn01.clrBlue * 0x0010000
		+ pTheApp->m_AppSettings.pathUnkn01.clrGreen * 0x00000100
		+ pTheApp->m_AppSettings.pathUnkn01.clrRed;
	m_staUnkn01.SetBkColor(color);

	color = pTheApp->m_AppSettings.pathUnkn02.clrBlue * 0x0010000
		+ pTheApp->m_AppSettings.pathUnkn02.clrGreen * 0x00000100
		+ pTheApp->m_AppSettings.pathUnkn02.clrRed;
	m_staUnkn02.SetBkColor(color);
}

void CPathDialog::UpdateCurrentColor()
{
	m_staCurrent.SetFlags(GetNewFlags(0));
}

void CPathDialog::OnKillFocusEdtPathPenSize() 
{
	CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
	ASSERT_VALID(pTheApp);
	UpdateData(TRUE);
	pTheApp->m_ToolPathEdit.SetToolSize(m_iPenSize);
	m_iPenSize = pTheApp->m_ToolPathEdit.GetToolSize();
	UpdateData(FALSE);
}
