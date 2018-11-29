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
// DoodadDialog.cpp : implementation file
//
#include "stdafx.h"
#include "W3ZMapEdit.h"
#include "MainFrm.h"
#include "DoodadDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDoodadDialog dialog
CDoodadDialog::CDoodadDialog(CWnd* pParent /*=NULL*/)
	: CDialogBar(/*CDoodadDialog::IDD, pParent*/)
{
	//{{AFX_DATA_INIT(CDoodadDialog)
	m_fXCoord = 0.0f;
	m_fYCoord = 0.0f;
	m_fXScale = 0.0f;
	m_fZScale = 0.0f;
	m_fYScale = 0.0f;
	m_sDoodadType = _T("");
	m_fZCoord = 0.0f;
	m_fAngle = 0.0f;
	m_iID = 0;
	m_iLife = 0;
	m_iVariation = -1;
	m_chkNormal = FALSE;
	m_chkNonSolid = FALSE;
	m_sFlags = _T("");
	//}}AFX_DATA_INIT
	m_iMinLife = 80;
	m_iMaxLife = 100;
	m_fMinScaleX = (float)0.8;
	m_fMinScaleY = (float)0.8;
	m_fMinScaleZ = (float)0.8;
	m_fMaxScaleX = (float)1.2;
	m_fMaxScaleY = (float)1.2;
	m_fMaxScaleZ = (float)1.2;
	m_sTypeList = DOOD_DEFAULT_TREE;
	m_iVariationRange = 8;
}


void CDoodadDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDoodadDialog)
	DDX_Control(pDX, IDC_CHK_DOO_NORMAL, m_ctrlSetNormal);
	DDX_Control(pDX, IDC_CHK_DOO_NON_SOLID, m_ctrlSetNonSolid);
	DDX_Control(pDX, IDC_CHK_DOODAD_VARIATION, m_ctrlSetVariation);
	DDX_Control(pDX, IDC_CHK_DOODAD_TYPE, m_ctrlSetType);
	DDX_Control(pDX, IDC_CHK_DOODAD_SCALE_Z, m_ctrlSetScaleZ);
	DDX_Control(pDX, IDC_CHK_DOODAD_SCALE_Y, m_ctrlSetScaleY);
	DDX_Control(pDX, IDC_CHK_DOODAD_SCALE_X, m_ctrlSetScaleX);
	DDX_Control(pDX, IDC_CHK_DOODAD_LIFE, m_ctrlSetLife);
	DDX_Control(pDX, IDC_CHK_DOODAD_ANGLE, m_ctrlSetAngle);
	DDX_Control(pDX, IDC_EDT_DOO_FLAGS, m_ctrlEdtFlags);
	DDX_Text(pDX, IDC_EDT_DOO_X, m_fXCoord);
	DDX_Text(pDX, IDC_EDT_DOO_Y, m_fYCoord);
	DDX_Text(pDX, IDC_EDT_DOO_SC_X, m_fXScale);
	DDX_Text(pDX, IDC_EDT_DOO_SC_Z, m_fZScale);
	DDX_Text(pDX, IDC_EDT_DOO_SC_Y, m_fYScale);
	DDX_Text(pDX, IDC_EDT_DOO_TYPE, m_sDoodadType);
	DDV_MaxChars(pDX, m_sDoodadType, 4);
	DDX_Text(pDX, IDC_EDT_DOO_Z, m_fZCoord);
	DDX_Text(pDX, IDC_EDT_DOO_ANGLE, m_fAngle);
	DDX_Text(pDX, IDC_EDT_DOO_ID, m_iID);
	DDX_Text(pDX, IDC_EDT_DOO_LIFE, m_iLife);
	DDX_CBIndex(pDX, IDC_CMB_DOO_VARIATION, m_iVariation);
	DDX_Check(pDX, IDC_CHK_DOO_NORMAL, m_chkNormal);
	DDX_Check(pDX, IDC_CHK_DOO_NON_SOLID, m_chkNonSolid);
	DDX_Text(pDX, IDC_EDT_DOO_FLAGS, m_sFlags);
	DDV_MaxChars(pDX, m_sFlags, 4);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDoodadDialog, CDialogBar)
	//{{AFX_MSG_MAP(CDoodadDialog)
	ON_EN_KILLFOCUS(IDC_EDT_DOO_FLAGS, OnKillFocusEdtDooFlags)
	ON_BN_CLICKED(IDC_CHK_DOO_NON_SOLID, OnChkDooNonSolid)
	ON_BN_CLICKED(IDC_CHK_DOO_NORMAL, OnChkDooNormal)
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDoodadDialog message handlers

void CDoodadDialog::SetFlags(int flags)
{
	m_sFlags.Format("0x%02X", flags);
	UpdateData(FALSE);
}

int CDoodadDialog::GetFlags()
{
	BOOL invalid = false;
	int i;
	int result = 0;

	UpdateData(TRUE);
	if ((m_sFlags.GetLength() > 4)
		|| (m_sFlags.GetLength() <= 2)
		|| ('0' != m_sFlags[0])
		|| (('x' != m_sFlags[1]) && ('X' != m_sFlags[1])))
	{
		invalid = true;
	}
	if (!invalid)
	{
		for (i = 2; i < m_sFlags.GetLength(); i++)
		{
			if (!((m_sFlags[i] >= '0') && (m_sFlags[i] <= '9'))
				&& !((m_sFlags[i] >= 'a') && (m_sFlags[i] <= 'f'))
				&& !((m_sFlags[i] >= 'A') && (m_sFlags[i] <= 'F')))
			{
				invalid = true;
			}
		}
	}
	if (!invalid)
	{
		for (i = 0; i < 2;i++)
		{
			if ((m_sFlags[2+i] >= '0') && (m_sFlags[2+i] <= '9'))
			{
				result *= 0x010;
				result += (m_sFlags[2+i] - '0');
			}
			else if ((m_sFlags[2+i] >= 'a') && (m_sFlags[2+i] <= 'f'))
			{
				result *= 0x010;
				result += (m_sFlags[2+i] - 'a') + 10;
			}
			else if ((m_sFlags[2+i] >= 'A') && (m_sFlags[2+i] <= 'F'))
			{
				result *= 0x010;
				result += (m_sFlags[2+i] - 'A') + 10;
			}
		}
	}
	return result;
}

void CDoodadDialog::OnKillFocusEdtDooFlags() 
{
	BOOL invalid = false;
	int i;
	CString oldFlags = m_sFlags;

	UpdateData(TRUE);
	if ((m_sFlags.GetLength() > 4)
		|| (m_sFlags.GetLength() <= 2)
		|| ('0' != m_sFlags[0])
		|| (('x' != m_sFlags[1]) && ('X' != m_sFlags[1])))
	{
		invalid = true;
	}
	if (!invalid)
	{
		for (i = 2; i < m_sFlags.GetLength(); i++)
		{
			if (!((m_sFlags[i] >= '0') && (m_sFlags[i] <= '9'))
				&& !((m_sFlags[i] >= 'a') && (m_sFlags[i] <= 'f'))
				&& !((m_sFlags[i] >= 'A') && (m_sFlags[i] <= 'F')))
			{
				invalid = true;
			}
		}
	}
	if (invalid)
	{
		AfxMessageBox( "Invalid flags value!\r\nPlease enter a correct one.\r\nExample: 0x02", MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL);
		m_ctrlEdtFlags.SetFocus();
		m_sFlags = oldFlags;
	}
	else
	{
		int flags = GetFlags();
		SetFlags(flags);
		if (flags & DOOD_FL_NONSOLID)
		{m_chkNonSolid = true;}
		else
		{m_chkNonSolid = false;}
		if (flags & DOOD_FL_NORMAL)
		{m_chkNormal = true;}
		else
		{m_chkNormal = false;}
	}
	UpdateData(FALSE);
}

void CDoodadDialog::OnChkDooNonSolid() 
{
	int flags = 0;
	UpdateData(TRUE);
	flags = GetFlags();
	if (m_chkNonSolid)
	{
		flags |= DOOD_FL_NONSOLID;
	}
	else
	{
		flags = flags & ~DOOD_FL_NONSOLID;
	}
	SetFlags(flags);	
}

void CDoodadDialog::OnChkDooNormal() 
{
	int flags = 0;
	UpdateData(TRUE);
	flags = GetFlags();
	if (m_chkNormal)
	{
		flags |= DOOD_FL_NORMAL;
	}
	else
	{
		flags = flags & ~DOOD_FL_NORMAL;
	}
	SetFlags(flags);	
}

void CDoodadDialog::UpdateDoodad(CDoodad doodad)
{
	m_sDoodadType = doodad.type.GetStrID();
	m_iVariation = doodad.variation;
	m_fXCoord = doodad.x;
	m_fYCoord = doodad.y;
	m_fZCoord = doodad.z;
	m_fAngle = doodad.GetAngleValue();
	m_fXScale = doodad.scaleX;
	m_fYScale = doodad.scaleY;
	m_fZScale = doodad.scaleZ;
	SetFlags(doodad.flags);
	m_iLife = doodad.life;
	m_iID = doodad.ID;
	m_doodad = doodad;
	if (doodad.flags & DOOD_FL_NONSOLID)
	{m_chkNonSolid = true;}
	else
	{m_chkNonSolid = false;}
	if (doodad.flags & DOOD_FL_NORMAL)
	{m_chkNormal = true;}
	else
	{m_chkNormal = false;}

	UpdateData(FALSE);
}

CDoodad CDoodadDialog::GetNewDoodad()
{
	CDoodad doodad;
	int randi;
	double randd;

	UpdateData(TRUE);

	doodad.ID = m_iID;

	// doodad type
	if (EB_STATE2 == m_ctrlSetType.GetState())
	{
		randi = (rand()%(m_sTypeList.GetLength()/4))*4;
		doodad.type = m_sTypeList.Mid(randi,4);
	}
	else
	{
		doodad.type = m_sDoodadType;
	}

	//doodad life
	if (EB_STATE2 == m_ctrlSetLife.GetState())
	{
		randi = rand()%(m_iMaxLife - m_iMinLife);
		doodad.life = (byte)(m_iMinLife + randi);
	}
	else
	{
		doodad.life = (byte)m_iLife;
	}

	// doodad variation
	if (EB_STATE2 == m_ctrlSetVariation.GetState())
	{
		randi = rand()%m_iVariationRange;
		doodad.variation = randi;
	}
	else
	{
		doodad.variation = m_iVariation;
	}


	//m_ctrlSetAngle
//	doodad.angle = m_fAngle;
	doodad.SetAngleValue(m_fAngle);

	// doodad scale X
	if (EB_STATE2 == m_ctrlSetScaleX.GetState())
	{
		randi = rand();
		randd = (double)randi / (double)RAND_MAX;
		randd = randd*(m_fMaxScaleX - m_fMinScaleX);
		doodad.scaleX = (float)(m_fMinScaleX + randd);
	}
	else
	{
		doodad.scaleX = m_fXScale;
	}
	
	// doodad scale Y
	if (EB_STATE2 == m_ctrlSetScaleY.GetState())
	{
		randi = rand();
		randd = (double)randi / (double)RAND_MAX;
		randd = randd*(m_fMaxScaleY - m_fMinScaleY);
		doodad.scaleY = (float)(m_fMinScaleY + randd);
	}
	else
	{
		doodad.scaleY = m_fYScale;
	}
	
	// doodad scale Z
	if (EB_STATE2 == m_ctrlSetScaleZ.GetState())
	{
		randi = rand();
		randd = (double)randi / (double)RAND_MAX;
		randd = randd*(m_fMaxScaleZ - m_fMinScaleZ);
		doodad.scaleZ = (float)(m_fMinScaleZ + randd);
	}
	else
	{
		doodad.scaleZ = m_fZScale;
	}
	
	doodad.x = m_fXCoord;
	doodad.y = m_fYCoord;
	doodad.z = m_fZCoord;

	// doodad flags
	doodad.flags = 0;
	//m_ctrlSetNormal
	if (m_chkNormal && m_ctrlSetNormal.IsWriteEnabled())
	{doodad.flags |= DOOD_FL_NORMAL;}
	//m_ctrlSetNonSolid
	if (m_chkNonSolid && m_ctrlSetNonSolid.IsWriteEnabled())
	{doodad.flags |= DOOD_FL_NONSOLID;}
	//selected
	return doodad;
}

void CDoodadDialog::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CRect rect;

	// check if the right click is not handled by another object
	m_ctrlSetNormal.GetWindowRect(rect);
	if (rect.PtInRect(point))
	{
		return;
	}

	m_ctrlSetNonSolid.GetWindowRect(rect);
	if (rect.PtInRect(point))
	{
		return;
	}

	// make sure window is active
	GetParentFrame()->ActivateFrame();
	CPoint local = point;
	ScreenToClient(&local);

	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_POPUPS));
	CMenu* pPopup = menu.GetSubMenu(POPUP_DOODADS_BAR);
	ASSERT(pPopup != NULL);
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());
	
}

void CDoodadDialog::EditProperties()
{
	CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
	ASSERT_VALID(pTheApp);

	m_PropDialog.m_iMinLife = m_iMinLife;
	m_PropDialog.m_iMaxLife = m_iMaxLife ;
	m_PropDialog.m_fMinScaleX = m_fMinScaleX;
	m_PropDialog.m_fMinScaleY = m_fMinScaleY;
	m_PropDialog.m_fMinScaleZ = m_fMinScaleZ;
	m_PropDialog.m_fMaxScaleX = m_fMaxScaleX;
	m_PropDialog.m_fMaxScaleY = m_fMaxScaleY;
	m_PropDialog.m_fMaxScaleZ = m_fMaxScaleZ;
	m_PropDialog.m_sTypeList = m_sTypeList;
	m_PropDialog.m_iVariation = m_iVariationRange;
	if (pTheApp)
	{
		m_PropDialog.m_fSpacing = pTheApp->m_ToolDoodadsEdit.m_fMinDist;
		m_PropDialog.m_iSize = pTheApp->m_ToolDoodadsEdit.GetToolSize();
	}

	if (IDOK == m_PropDialog.DoModal())
	{
		int length;
		if (m_PropDialog.m_iMinLife < 0)
		{
			m_PropDialog.m_iMinLife = 0;
		}
		else if (m_PropDialog.m_iMinLife > 255)
		{
			m_PropDialog.m_iMinLife = 255;
		}
		if (m_PropDialog.m_iMaxLife < m_PropDialog.m_iMinLife)
		{
			m_PropDialog.m_iMaxLife = m_PropDialog.m_iMinLife;
		}
		else if (m_PropDialog.m_iMaxLife > 255)
		{
			m_PropDialog.m_iMaxLife = 255;
		}
		length = m_PropDialog.m_sTypeList.GetLength();
		if (length%4)
		{
			m_PropDialog.m_sTypeList = m_PropDialog.m_sTypeList.Left(length - length%4);
		}
		if (length < 4)
		{
			m_PropDialog.m_sTypeList = DOOD_DEFAULT_TREE;
		}
		if (m_PropDialog.m_iSize < TOOL_DOOD_MIN_SIZE)
		{
			m_PropDialog.m_iSize = TOOL_DOOD_MIN_SIZE;
		}
		else if (m_PropDialog.m_iSize > TOOL_DOOD_MAX_SIZE)
		{
			m_PropDialog.m_iSize = TOOL_DOOD_MAX_SIZE;
		}


		m_iMinLife = m_PropDialog.m_iMinLife;
		m_iMaxLife = m_PropDialog.m_iMaxLife;
		m_fMinScaleX = m_PropDialog.m_fMinScaleX;
		m_fMinScaleY = m_PropDialog.m_fMinScaleY;
		m_fMinScaleZ = m_PropDialog.m_fMinScaleZ;
		m_fMaxScaleX = m_PropDialog.m_fMaxScaleX;
		m_fMaxScaleY = m_PropDialog.m_fMaxScaleY;
		m_fMaxScaleZ = m_PropDialog.m_fMaxScaleZ;
		m_sTypeList = m_PropDialog.m_sTypeList;
		m_iVariationRange = m_PropDialog.m_iVariation;
		if (pTheApp)
		{
			pTheApp->m_ToolDoodadsEdit.SetToolSize(m_PropDialog.m_iSize);
			pTheApp->m_ToolDoodadsEdit.m_fMinDist = m_PropDialog.m_fSpacing;
		}
	}

}

void CDoodadDialog::OnUpdateCmdUI(CFrameWnd *pTarget, BOOL bDisableIfNoHndler)
{
 // disable auto-disabling buttons
}

void CDoodadDialog::InitDialogBar()
{
	CDoodad doodad;
	UpdateDoodad(doodad);

//	enable tri-states buttons
	m_ctrlSetVariation.SetTriState(TRUE);
	m_ctrlSetType.SetTriState(TRUE);
	m_ctrlSetScaleX.SetTriState(TRUE);
	m_ctrlSetScaleY.SetTriState(TRUE);
	m_ctrlSetScaleZ.SetTriState(TRUE);
	m_ctrlSetLife.SetTriState(TRUE);
//	m_ctrlSetAngle.SetTriState(TRUE);

// mets en mode texte seulement les boutons requis
	m_ctrlSetScaleX.SetTextOnly(TRUE);
	m_ctrlSetScaleY.SetTextOnly(TRUE);
	m_ctrlSetScaleZ.SetTextOnly(TRUE);

// met tous les boutons en mode ecriture
	m_ctrlSetVariation.SetState(EB_STATE2);
	m_ctrlSetType.SetState(EB_STATE1);
	m_ctrlSetScaleZ.SetState(EB_STATE2);
	m_ctrlSetScaleY.SetState(EB_STATE2);
	m_ctrlSetScaleX.SetState(EB_STATE2);
	m_ctrlSetLife.SetState(EB_STATE1);
	m_ctrlSetAngle.SetState(EB_STATE1);
	m_ctrlSetNormal.SetWriteState(TRUE);
	m_ctrlSetNonSolid.SetWriteState(TRUE);
}

CDoodad CDoodadDialog::GetUpdatedDoodad(CDoodad doodad)
{
	CDoodad newdoodad;
	int randi;
	double randd;

	UpdateData(TRUE);

	newdoodad = doodad;

	// doodad type
	if (EB_STATE1 == m_ctrlSetType.GetState())
	{
		newdoodad.type = m_sDoodadType;
	}
	else if (EB_STATE2 == m_ctrlSetType.GetState())
	{
		randi = (rand()%(m_sTypeList.GetLength()/4))*4;
		newdoodad.type = m_sTypeList.Mid(randi,4);
	}

	//doodad life
	if (EB_STATE1 == m_ctrlSetLife.GetState())
	{
		newdoodad.life = (byte)m_iLife;
	}
	else if (EB_STATE2 == m_ctrlSetLife.GetState())
	{
		randi = rand()%(m_iMaxLife - m_iMinLife);
		newdoodad.life = (byte)(m_iMinLife + randi);
	}

	// doodad variation
	if (EB_STATE1 == m_ctrlSetVariation.GetState())
	{
		newdoodad.variation = m_iVariation;
	}
	else if (EB_STATE2 == m_ctrlSetVariation.GetState())
	{
		randi = rand()%m_iVariationRange;
		newdoodad.variation = randi;
	}


	if (EB_STATE1 == m_ctrlSetAngle.GetState())
	{
//		newdoodad.angle = m_fAngle;
		newdoodad.SetAngleValue(m_fAngle);
	}

	// doodad scale X
	if (EB_STATE1 == m_ctrlSetScaleX.GetState())
	{
		newdoodad.scaleX = m_fXScale;
	}
	else if (EB_STATE2 == m_ctrlSetScaleX.GetState())
	{
		randi = rand();
		randd = (double)randi / (double)RAND_MAX;
		randd = randd*(m_fMaxScaleX - m_fMinScaleX);
		newdoodad.scaleX = (float)(m_fMinScaleX + randd);
	}
	
	// doodad scale Y
	if (EB_STATE1 == m_ctrlSetScaleY.GetState())
	{
		newdoodad.scaleY = m_fYScale;
	}
	else if (EB_STATE2 == m_ctrlSetScaleY.GetState())
	{
		randi = rand();
		randd = (double)randi / (double)RAND_MAX;
		randd = randd*(m_fMaxScaleY - m_fMinScaleY);
		newdoodad.scaleY = (float)(m_fMinScaleY + randd);
	}
	
	// doodad scale Z
	if (EB_STATE1 == m_ctrlSetScaleZ.GetState())
	{
		newdoodad.scaleZ = m_fZScale;
	}
	else if (EB_STATE2 == m_ctrlSetScaleZ.GetState())
	{
		randi = rand();
		randd = (double)randi / (double)RAND_MAX;
		randd = randd*(m_fMaxScaleZ - m_fMinScaleZ);
		newdoodad.scaleZ = (float)(m_fMinScaleZ + randd);
	}
	
//	newdoodad.x = m_fXCoord;
//	newdoodad.y = m_fYCoord;
//	newdoodad.z = m_fZCoord;

	// doodad flags
	if (m_chkNormal && m_ctrlSetNormal.IsWriteEnabled())
	{newdoodad.flags |= DOOD_FL_NORMAL;}

	if (m_chkNonSolid && m_ctrlSetNonSolid.IsWriteEnabled())
	{newdoodad.flags |= DOOD_FL_NONSOLID;}

	return newdoodad;

}

CDoodad CDoodadDialog::GetDoodad()
{
	CDoodad doodad;

	UpdateData(TRUE);

	doodad.ID = m_iID;

	// doodad type
	doodad.type = m_sDoodadType;

	//doodad life
	doodad.life = (byte)m_iLife;

	// doodad variation
	doodad.variation = m_iVariation;

	//m_ctrlSetAngle
//	doodad.angle = m_fAngle;
	doodad.SetAngleValue(m_fAngle);

	// doodad scale X
	doodad.scaleX = m_fXScale;
	
	// doodad scale Y
	doodad.scaleY = m_fYScale;

	// doodad scale Z
	doodad.scaleZ = m_fZScale;
	
	doodad.x = m_fXCoord;
	doodad.y = m_fYCoord;
	doodad.z = m_fZCoord;

	// doodad flags
	doodad.flags = 0;
	//m_ctrlSetNormal
	if (m_chkNormal)
	{doodad.flags |= DOOD_FL_NORMAL;}
	//m_ctrlSetNonSolid
	if (m_chkNonSolid)
	{doodad.flags |= DOOD_FL_NONSOLID;}
	//selected
	return doodad;
}
