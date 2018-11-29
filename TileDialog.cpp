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
// TileDialog.cpp : implementation file
//

#include "stdafx.h"
#include "W3ZMapEdit.h"
#include "TileDialog.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "W3ZMapEditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTileDialog dialog


CTileDialog::CTileDialog(CWnd* pParent /*=NULL*/)
	: CDialogBar(/*CTileDialog::IDD, pParent*/)
{
	//{{AFX_DATA_INIT(CTileDialog)
	m_bShadow = FALSE;
	m_bWater = FALSE;
	m_iDetails = 0;
	m_iHeight = 0;
	m_iLayer = 0;
	m_sFlags = _T("");
	m_bRamp = FALSE;
	m_bMinimapShadow = FALSE;
	m_bBlight = FALSE;
	m_iWaterLevel = 0;
	m_fWEWaterLevel = 0.0f;
	m_fWEGroundLevel = 0.0f;
	//}}AFX_DATA_INIT
	m_iGroundHeightAmp = 64;
	m_iWaterHeightAmp = 0;
	m_iDetailsRange = 255;
}


void CTileDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTileDialog)
	DDX_Control(pDX, IDC_CHK_WATER_LEVEL, m_ctrlSetWaterLevel);
	DDX_Control(pDX, IDC_CHK_TILEPOINT_HEIGHT, m_ctrlSetTilePointHeight);
	DDX_Control(pDX, IDC_CHK_LAYER, m_ctrlSetLayer);
	DDX_Control(pDX, IDC_CHK_GROUND_TYPE, m_ctrlSetGroundType);
	DDX_Control(pDX, IDC_CHK_DETAILS, m_ctrlSetDetails);
	DDX_Control(pDX, IDC_CHK_CLIFF_TYPE, m_ctrlSetCliffType);
	DDX_Control(pDX, IDC_CHK_TILE_WATER, m_ctrlChkWater);
	DDX_Control(pDX, IDC_CHK_TILE_MINIMAPSHD, m_ctrlChkMinimapShadow);
	DDX_Control(pDX, IDC_CHK_TILE_BLIGHT, m_ctrlChkBlight);
	DDX_Control(pDX, IDC_CHK_TILE_SHADOW, m_ctrlChkShadow);
	DDX_Control(pDX, IDC_CHK_TILE_RAMP, m_ctrlChkRamp);
	DDX_Control(pDX, IDC_SPN_WATER_LEVEL, m_spnWaterLevel);
	DDX_Control(pDX, IDC_CMB_TILE_CLIFF, m_ctrlEdtCliff);
	DDX_Control(pDX, IDC_CMB_TILE_GROUND, m_ctrlEdtGround);
	DDX_Control(pDX, IDC_EDTFLAGS, m_ctrlEdtFlags);
	DDX_Control(pDX, IDC_SPN_LAYER, m_spnLayer);
	DDX_Control(pDX, IDC_SPN_HEIGHT, m_spnHeight);
	DDX_Control(pDX, IDC_SPN_DETAILS, m_spnDetails);
	DDX_Check(pDX, IDC_CHK_TILE_SHADOW, m_bShadow);
	DDX_Check(pDX, IDC_CHK_TILE_WATER, m_bWater);
	DDX_Text(pDX, IDC_EDT_TILE_DETAILS, m_iDetails);
	DDX_Text(pDX, IDC_EDT_TILE_HEIGHT, m_iHeight);
	DDX_Text(pDX, IDC_EDT_TILE_LAYER, m_iLayer);
	DDX_Text(pDX, IDC_EDTFLAGS, m_sFlags);
	DDV_MaxChars(pDX, m_sFlags, 6);
	DDX_Check(pDX, IDC_CHK_TILE_RAMP, m_bRamp);
	DDX_Check(pDX, IDC_CHK_TILE_MINIMAPSHD, m_bMinimapShadow);
	DDX_Check(pDX, IDC_CHK_TILE_BLIGHT, m_bBlight);
	DDX_Text(pDX, IDC_EDT_TILE_WATER_LEVEL, m_iWaterLevel);
	DDX_Text(pDX, IDC_EDT_WATER_LEVEL, m_fWEWaterLevel);
	DDX_Text(pDX, IDC_EDT_GROUND_LEVEL, m_fWEGroundLevel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTileDialog, CDialogBar)
	//{{AFX_MSG_MAP(CTileDialog)
	ON_BN_CLICKED(IDC_CHK_TILE_SHADOW, OnChkTileShadow)
	ON_BN_CLICKED(IDC_CHK_TILE_WATER, OnChkTileWater)
	ON_EN_KILLFOCUS(IDC_EDTFLAGS, OnKillFocusEdtFlags)
	ON_EN_KILLFOCUS(IDC_EDT_TILE_LAYER, OnKillFocusEdtTileLayer)
	ON_EN_KILLFOCUS(IDC_EDT_TILE_HEIGHT, OnKillFocusEdtTileHeight)
	ON_EN_KILLFOCUS(IDC_EDT_TILE_DETAILS, OnKillFocusEdtTileDetails)
	ON_BN_CLICKED(IDC_CHK_TILE_RAMP, OnChkTileRamp)
	ON_BN_CLICKED(IDC_CHK_TILE_MINIMAPSHD, OnChkTileMiniMapShd)
	ON_BN_CLICKED(IDC_CHK_TILE_BLIGHT, OnChkTileBlight)
	ON_EN_KILLFOCUS(IDC_EDT_TILE_WATER_LEVEL, OnKillFocusEdtTileWaterLevel)
	ON_EN_KILLFOCUS(IDC_EDT_GROUND_LEVEL, OnKillFocusEdtGroundLevel)
	ON_EN_KILLFOCUS(IDC_EDT_WATER_LEVEL, OnKillFocusEdtWaterLevel)
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTileDialog message handlers


void CTileDialog::OnChkTileShadow() 
{

	int flags = 0;
	UpdateData(TRUE);
	flags = GetFlags();
	if (m_bShadow)
	{
		flags |= TILE_FL_SHADOW;
	}
	else
	{
		flags = flags & ~TILE_FL_SHADOW;
	}
	SetFlags(flags);

}

void CTileDialog::OnChkTileWater() 
{

	int flags = 0;
	UpdateData(TRUE);
	flags = GetFlags();
	if (m_bWater)
	{
		flags |= TILE_FL_WATER;
	}
	else
	{
		flags = flags & ~TILE_FL_WATER;
	}
	SetFlags(flags);

}

void CTileDialog::OnKillFocusEdtFlags() 
{
	BOOL invalid = false;
	int i;
	CString oldFlags = m_sFlags;

	UpdateData(TRUE);
	if ((m_sFlags.GetLength() > 6)
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
		AfxMessageBox( "Invalid flags value!\r\nPlease enter a correct one.\r\nExample: 0x4020", MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL);
		m_ctrlEdtFlags.SetFocus();
		m_sFlags = oldFlags;
	}
	else
	{
		int flags = GetFlags();
		SetFlags(flags);
		if (flags & TILE_FL_WATER)
		{m_bWater = true;}
		else
		{m_bWater = false;}
		if (flags & TILE_FL_SHADOW)
		{m_bShadow = true;}
		else
		{m_bShadow = false;}
		if (flags & TILE_FL_MINISHAD)
		{m_bMinimapShadow = true;}
		else
		{m_bMinimapShadow = false;}
		if (flags & TILE_FL_RAMP)
		{m_bRamp = true;}
		else
		{m_bRamp = false;}
		if (flags & TILE_FL_BLIGHT)
		{m_bBlight = true;}
		else
		{m_bBlight = false;}
	}
	UpdateData(FALSE);
}

void CTileDialog::OnKillFocusEdtTileDetails() 
{
	UpdateData(TRUE);
	if (m_iDetails < 0)
	{
		m_iDetails = 0;
		UpdateData(FALSE);
	}
	else if (m_iDetails >= 256)
	{
		m_iDetails = 256;
		UpdateData(FALSE);
	}
}

int CTileDialog::GetFlags()
{
	BOOL invalid = false;
	int i;
	int result = 0;

	UpdateData(TRUE);
	if ((m_sFlags.GetLength() > 6)
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
		for (i = 0; i < 4;i++)
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
	result = result & TILE_MSK_FLAGS;
	return result;
}

void CTileDialog::SetFlags(int flags)
{
	m_sFlags.Format("0x%04X", (flags & TILE_MSK_FLAGS));
	UpdateData(FALSE);
}

void CTileDialog::OnChkTileRamp() 
{

	int flags = 0;
	UpdateData(TRUE);
	flags = GetFlags();
	if (m_bRamp)
	{
		flags |= TILE_FL_RAMP;
//		m_ctrlChkRamp.SetWindowText("Ramp *");
	}
	else
	{
		flags = flags & ~TILE_FL_RAMP;
//		m_ctrlChkRamp.SetWindowText("Ramp");
	}
	SetFlags(flags);

}

void CTileDialog::OnChkTileMiniMapShd() 
{

	int flags = 0;
	UpdateData(TRUE);
	flags = GetFlags();
	if (m_bMinimapShadow)
	{
		flags |= TILE_FL_MINISHAD;
	}
	else
	{
		flags = flags & ~TILE_FL_MINISHAD;
	}
	SetFlags(flags);
	
}

void CTileDialog::OnChkTileBlight() 
{

	int flags = 0;
	UpdateData(TRUE);
	flags = GetFlags();
	if (m_bBlight)
	{
		flags |= TILE_FL_BLIGHT;
	}
	else
	{
		flags = flags & ~TILE_FL_BLIGHT;
	}
	SetFlags(flags);

}

void CTileDialog::OnKillFocusEdtTileLayer() 
{
	UpdateData(TRUE);
	if (m_iLayer < 0)
	{
		m_iLayer = 0;
	}
	else if (m_iLayer > TILE_MAX_LAYER)
	{
		m_iLayer = TILE_MAX_LAYER;
	}
	m_fWEGroundLevel = (float)(m_iHeight - TILE_HEIGHT_LEVEL_ZERO + (m_iLayer - TILE_LAYER_LEVEL_ZERO)*0x0200) / (float)4.0;
	UpdateData(FALSE);
}

void CTileDialog::OnKillFocusEdtTileHeight() 
{
	UpdateData(TRUE);
	if (m_iHeight < 0)
	{
		m_iHeight = 0;
	}
	else if (m_iHeight >= 0xFFFF)
	{
		m_iHeight = 0xFFFF;
	}
	m_fWEGroundLevel = (float)(m_iHeight - TILE_HEIGHT_LEVEL_ZERO + (m_iLayer - TILE_LAYER_LEVEL_ZERO)*0x0200) / (float)4.0;
	UpdateData(FALSE);
}

void CTileDialog::OnKillFocusEdtTileWaterLevel() 
{
	UpdateData(TRUE);
	if (m_iWaterLevel < 0)
	{
		m_iWaterLevel = 0;
	}
	else if (m_iWaterLevel >= 0xFFFF)
	{
		m_iWaterLevel = 0xFFFF;
	}
	m_fWEWaterLevel = (float)(m_iWaterLevel - TILE_WATER_OFFSET) / (float)4.0;
	UpdateData(FALSE);
}

void CTileDialog::OnKillFocusEdtGroundLevel() 
{
	UpdateData(TRUE);
	m_iHeight = (int) (m_fWEGroundLevel * 4) - ((m_iLayer - TILE_LAYER_LEVEL_ZERO)*0x0200) + TILE_HEIGHT_LEVEL_ZERO;
	if (m_iHeight < 0)
	{
		m_iHeight = 0;
	}
	else if (m_iHeight >= 0xFFFF)
	{
		m_iHeight = 0xFFFF;
	}
	m_fWEGroundLevel = (float)(m_iHeight - TILE_HEIGHT_LEVEL_ZERO + (m_iLayer - TILE_LAYER_LEVEL_ZERO)*0x0200) / (float)4.0;
	UpdateData(FALSE);
}

void CTileDialog::OnKillFocusEdtWaterLevel() 
{
	UpdateData(TRUE);
	m_iWaterLevel = (int) (m_fWEWaterLevel * 4) + TILE_WATER_OFFSET;
	if (m_iWaterLevel < 0)
	{
		m_iWaterLevel = 0;
	}
	else if (m_iWaterLevel >= 0xFFFF)
	{
		m_iWaterLevel = 0xFFFF;
	}
	m_fWEWaterLevel = (float)(m_iWaterLevel - TILE_WATER_OFFSET) / (float)4.0;
	UpdateData(FALSE);
}


void CTileDialog::InitDilaogBar()
{
	CTilePoint tilepoint;
	UpdateTilePoint(tilepoint);

	// l'intilaisation des Spins doit imperativement avoir lieu
	// APRES l'initialisation des champs qu'elles concenrnent.
	m_spnLayer.SetBase(10);
	m_spnLayer.SetRange32(0, 15);
	m_spnLayer.SetPos(m_iLayer);

	m_spnHeight.SetBase(10);
	m_spnHeight.SetRange32(0, 65535);
	m_spnHeight.SetPos(m_iHeight);

	m_spnWaterLevel.SetBase(10);
	m_spnWaterLevel.SetRange32(0, 16383);
	m_spnWaterLevel.SetPos(m_iWaterLevel);

	m_spnDetails.SetBase(10);
	m_spnDetails.SetRange32(0, 255);
	m_spnDetails.SetPos(m_iDetails);

//	enable tri-states buttons
	m_ctrlSetTilePointHeight.SetTriState(TRUE);
	m_ctrlSetWaterLevel.SetTriState(TRUE);
	m_ctrlSetDetails.SetTriState(TRUE);

// met tous les boutons en mode ecriture ou random suivant les cas
	m_ctrlSetWaterLevel.SetState(EB_STATE1);
	m_ctrlSetTilePointHeight.SetState(EB_STATE2);
	m_ctrlSetLayer.SetState(EB_STATE1);
	m_ctrlSetGroundType.SetState(EB_STATE1);
	m_ctrlSetDetails.SetState(EB_STATE2);
	m_ctrlSetCliffType.SetState(EB_STATE1);
	m_ctrlChkWater.SetWriteState(TRUE);
	m_ctrlChkMinimapShadow.SetWriteState(TRUE);
	m_ctrlChkBlight.SetWriteState(TRUE);
	m_ctrlChkShadow.SetWriteState(TRUE);
	m_ctrlChkRamp.SetWriteState(TRUE);

	Invalidate(); // a cause des EButtons (owner-drawn)
}


void CTileDialog::UpdateTilePoint(CTilePoint tilepoint)
{
	UpdateData(FALSE); // updates to get back the "set/not-to-set" status

	m_iHeight = tilepoint.height;
	m_iLayer = tilepoint.layer;
//	m_iGroundType = tilepoint.groundtype;
	m_ctrlEdtGround.SetCurSel(tilepoint.groundtype);
//	m_iCliffType = tilepoint.clifftype;
	m_ctrlEdtCliff.SetCurSel(tilepoint.clifftype);
	m_iDetails = tilepoint.details;

	if (tilepoint.flags & TILE_FL_SHADOW)
	{m_bShadow = true;}
	else
	{m_bShadow = false;}
	if (tilepoint.flags & TILE_FL_WATER)
	{m_bWater = true;}
	else
	{m_bWater = false;}
	if (tilepoint.flags & TILE_FL_MINISHAD)
	{m_bMinimapShadow = true;}
	else
	{m_bMinimapShadow = false;}
	if (tilepoint.flags & TILE_FL_RAMP)
	{m_bRamp = true;}
	else
	{m_bRamp = false;}
	if (tilepoint.flags & TILE_FL_BLIGHT)
	{m_bBlight = true;}
	else
	{m_bBlight = false;}

	SetFlags(tilepoint.flags);
	
	m_iWaterLevel = tilepoint.waterlevel;

	m_fWEGroundLevel = (float)tilepoint.GetGroundLevel() / (float)4.0;
	m_fWEWaterLevel = (float)tilepoint.GetWaterLevel() / (float)4.0;

	UpdateData(FALSE);
}


CTilePoint CTileDialog::GetTilePoint()
{
	CTilePoint tilepoint;

	UpdateData(TRUE);

	tilepoint.layer = (byte)m_iLayer;
	tilepoint.height = m_iHeight;
	tilepoint.waterlevel = m_iWaterLevel;
	tilepoint.groundtype = (byte)m_ctrlEdtGround.GetCurSel();//m_iGroundType;
	tilepoint.clifftype = (byte)m_ctrlEdtCliff.GetCurSel();//m_iCliffType;
	tilepoint.details = (byte)m_iDetails;
	tilepoint.flags = GetFlags();
	return tilepoint;

}


// sets only randomizable values
CTilePoint CTileDialog::GetRandTilePoint(CTilePoint oldtilepoint)
{
	CTilePoint tilepoint;
	int randi;

	tilepoint = oldtilepoint;

	UpdateData(TRUE);

	if (m_iGroundHeightAmp)
	{
		randi = (rand()%(m_iGroundHeightAmp*2))-m_iGroundHeightAmp;
		tilepoint.height = tilepoint.height + randi;
	}

	if (m_iWaterHeightAmp)
	{
		randi = (rand()%(m_iWaterHeightAmp*2))-m_iWaterHeightAmp;
		tilepoint.waterlevel = tilepoint.waterlevel + randi;
	}

	if (m_iDetailsRange)
	{
		randi = rand()%m_iDetailsRange;
		tilepoint.details = (byte)randi;
	}

	return tilepoint;
}


CTilePoint CTileDialog::GetNewTilePoint(CTilePoint oldtilepoint)
{
	CTilePoint tilepoint;
	int randi;

	tilepoint = oldtilepoint;

	UpdateData(TRUE);

	if (m_ctrlSetLayer.GetState())
	{
		tilepoint.layer = (byte)m_iLayer;
	}
	if (1 == m_ctrlSetTilePointHeight.GetState())
	{// tri state
		tilepoint.height = m_iHeight;
	}
	else if (2 == m_ctrlSetTilePointHeight.GetState())
	{
		if (m_iGroundHeightAmp)
		{
			randi = (rand()%(m_iGroundHeightAmp*2))-m_iGroundHeightAmp;
			tilepoint.height = m_iHeight + randi;
		}
		else
		{
			tilepoint.height = m_iHeight;
		}
	}
	if (1 == m_ctrlSetWaterLevel.GetState())
	{// tri state
		tilepoint.waterlevel = m_iWaterLevel;
	}
	else if (2 == m_ctrlSetWaterLevel.GetState())
	{
		if (m_iWaterHeightAmp)
		{
			randi = (rand()%(m_iWaterHeightAmp*2))-m_iWaterHeightAmp;
			tilepoint.waterlevel = m_iWaterLevel + randi;
		}
		else
		{
			tilepoint.waterlevel = m_iWaterLevel;
		}
	}
	if (m_ctrlSetGroundType.GetState())
	{
		tilepoint.groundtype = (byte)m_ctrlEdtGround.GetCurSel();//m_iGroundType;
	}
	if (m_ctrlSetCliffType.GetState())
	{
		tilepoint.clifftype = (byte)m_ctrlEdtCliff.GetCurSel();//m_iCliffType;
	}
	if (1 == m_ctrlSetDetails.GetState())
	{// tri state
		tilepoint.details = (byte)m_iDetails;
	}
	else if (m_ctrlSetDetails.GetState())
	{
		if (m_iDetailsRange)
		{
			randi = rand()%m_iDetailsRange;
			tilepoint.details = (byte)randi;
		}
		else
		{
			tilepoint.details = (byte)m_iDetails;
		}
	}

	//shadow
	if (m_ctrlChkShadow.IsWriteEnabled())
	{
		if (m_bShadow)
		{tilepoint.flags |= TILE_FL_SHADOW;}
		else
		{tilepoint.flags &= ~TILE_FL_SHADOW;}
	}
	//ramp
	if (m_ctrlChkRamp.IsWriteEnabled())
	{
		if (m_bRamp)
		{tilepoint.flags |= TILE_FL_RAMP;}
		else
		{tilepoint.flags &= ~TILE_FL_RAMP;}
	}
	//shadow
	if (m_ctrlChkWater.IsWriteEnabled())
	{
		if (m_bWater)
		{tilepoint.flags |= TILE_FL_WATER;}
		else
		{tilepoint.flags &= ~TILE_FL_WATER;}
	}
	//minimap shadow
	if (m_ctrlChkMinimapShadow.IsWriteEnabled())
	{
		if (m_bMinimapShadow)
		{tilepoint.flags |= TILE_FL_MINISHAD;}
		else
		{tilepoint.flags &= ~TILE_FL_MINISHAD;}
	}
	//blight
	if (m_ctrlChkBlight.IsWriteEnabled())
	{
		if (m_bBlight)
		{tilepoint.flags |= TILE_FL_BLIGHT;}
		else
		{tilepoint.flags &= ~TILE_FL_BLIGHT;}
	}
	return tilepoint;
}


void CTileDialog::OnUpdateCmdUI(CFrameWnd *pTarget, BOOL bDisableIfNoHndler)
{
 // disable auto-disabling buttons
}

void CTileDialog::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CRect rect;

	// check if the right click is not handled by another object
	
	m_ctrlChkWater.GetWindowRect(rect);
	if (rect.PtInRect(point))
	{
		return;
	}
	m_ctrlChkMinimapShadow.GetWindowRect(rect);
	if (rect.PtInRect(point))
	{
		return;
	}
	m_ctrlChkBlight.GetWindowRect(rect);
	if (rect.PtInRect(point))
	{
		return;
	}
	m_ctrlChkShadow.GetWindowRect(rect);
	if (rect.PtInRect(point))
	{
		return;
	}
	m_ctrlChkRamp.GetWindowRect(rect);
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
	CMenu* pPopup = menu.GetSubMenu(POPUP_TILEPOINTS_BAR);
	ASSERT(pPopup != NULL);
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());
}


void CTileDialog::EditProperties()
{
	CW3ZMapEditApp *pTheApp;
	pTheApp = (CW3ZMapEditApp *)AfxGetApp();

	m_PropDialog.m_iDetailsRange = m_iDetailsRange;
	m_PropDialog.m_iGrndHeightAmp = m_iGroundHeightAmp;
	m_PropDialog.m_iWaterHeightAmp = m_iWaterHeightAmp;
	if (pTheApp)
	{
		m_PropDialog.m_iSize = pTheApp->m_ToolTilePointsEdit.GetToolSize();
	}
	else
	{
		m_PropDialog.m_iSize = 1;
	}
	if (IDOK == m_PropDialog.DoModal())
	{
		// check details range
		if (m_PropDialog.m_iDetailsRange < 0)
		{
			m_PropDialog.m_iDetailsRange = 0;
		}
		else if (m_PropDialog.m_iDetailsRange > 255)
		{
			m_PropDialog.m_iDetailsRange = 255;
		}
		// check ground height amplitude range
		if (m_PropDialog.m_iGrndHeightAmp < 0)
		{
			m_PropDialog.m_iGrndHeightAmp = 0;
		}
		else if (m_PropDialog.m_iGrndHeightAmp > 32767)
		{
			m_PropDialog.m_iGrndHeightAmp = 32767;
		}
		// check water height amplitude range
		if (m_PropDialog.m_iWaterHeightAmp < 0)
		{
			m_PropDialog.m_iWaterHeightAmp = 0;
		}
		else if (m_PropDialog.m_iWaterHeightAmp > 32767)
		{
			m_PropDialog.m_iWaterHeightAmp = 32767;
		}
		if (m_PropDialog.m_iSize < TOOL_TILE_MIN_SIZE)
		{
			m_PropDialog.m_iSize = TOOL_TILE_MIN_SIZE;
		}
		else if (m_PropDialog.m_iSize > TOOL_TILE_MAX_SIZE)
		{
			m_PropDialog.m_iSize = TOOL_TILE_MAX_SIZE;
		}

		m_iDetailsRange = m_PropDialog.m_iDetailsRange;
		m_iGroundHeightAmp = m_PropDialog.m_iGrndHeightAmp;
		m_iWaterHeightAmp = m_PropDialog.m_iWaterHeightAmp;
		if (pTheApp)
		{
			pTheApp->m_ToolTilePointsEdit.SetToolSize(m_PropDialog.m_iSize);
		}
	}
}

void CTileDialog::UpdateGroundTypes(CString tilesets)
{
	UpdateData(TRUE);
	int cursel;
	cursel = m_ctrlEdtGround.GetCurSel();
	m_ctrlEdtGround.ResetContent();
	if (tilesets.GetLength())
	{
		int i, nbItems;
		nbItems = tilesets.GetLength()/4;
		for (i = 0; i < nbItems;i++)
		{
			m_ctrlEdtGround.AddString(tilesets.Mid(i*4, 4));
		}
		if ((cursel < 0) || (cursel > nbItems))
		{
			m_ctrlEdtGround.SetCurSel(0);
		}
		else
		{
			m_ctrlEdtGround.SetCurSel(cursel);
		}
		UpdateData(FALSE);
	}
}

void CTileDialog::UpdateCliffTypes(CString tilesets)
{
	UpdateData(TRUE);
	int cursel;
	cursel = m_ctrlEdtCliff.GetCurSel();
	m_ctrlEdtCliff.ResetContent();
	if (tilesets.GetLength())
	{
		int i, nbItems;
		nbItems = tilesets.GetLength()/4;
		for (i = 0; i < nbItems;i++)
		{
			m_ctrlEdtCliff.AddString(tilesets.Mid(i*4, 4));
		}
		if ((cursel < 0) || (cursel > nbItems))
		{
			m_ctrlEdtCliff.SetCurSel(0);
		}
		else
		{
			m_ctrlEdtCliff.SetCurSel(cursel);
		}
		UpdateData(FALSE);
	}
}
