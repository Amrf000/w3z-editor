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
// ViewModeDialog.cpp : implementation file
//

#include "stdafx.h"
#include "W3ZMapEdit.h"
#include "ViewModeDialog.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "W3ZMapEditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewModeDialog dialog


CViewModeDialog::CViewModeDialog(CWnd* pParent /*=NULL*/)
	: CDialogBar(/*CViewModeDialog::IDD, pParent*/)
{
	//{{AFX_DATA_INIT(CViewModeDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CViewModeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewModeDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewModeDialog, CDialogBar)
	//{{AFX_MSG_MAP(CViewModeDialog)
	ON_BN_CLICKED(IDC_RAD_TILES, OnRadTiles)
	ON_BN_CLICKED(IDC_RAD_LEVEL, OnRadLevel)
	ON_BN_CLICKED(IDC_CHK_DOODADS, OnChkDoodads)
	ON_BN_CLICKED(IDC_CHK_PATH, OnChkPath)
	ON_BN_CLICKED(IDC_CHK_SHADOWS, OnChkShadows)
	ON_BN_CLICKED(IDC_CHK_PLAYERS, OnChkPlayers)
	ON_BN_CLICKED(IDC_CHK_TILES_BLIGHTS, OnChkTilesBlights)
	ON_BN_CLICKED(IDC_CHK_TILES_BOUNDARIES, OnChkTilesBoundaries)
	ON_BN_CLICKED(IDC_CHK_TILES_LAYERS, OnChkTilesLayers)
	ON_BN_CLICKED(IDC_CHK_TILES_RAMPS, OnChkTilesRamps)
	ON_BN_CLICKED(IDC_CHK_TILES_WATER, OnChkTilesWater)
	ON_BN_CLICKED(IDC_CHK_TILES_WEBOUNDS, OnChkTilesWebounds)
	ON_BN_CLICKED(IDC_CHK_UNITS, OnChkUnits)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewModeDialog message handlers

void CViewModeDialog::OnRadTiles() 
{
	CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
	ASSERT_VALID(pTheApp);

	CButton *pRadTiles;
	pRadTiles = (CButton *)GetDlgItem(IDC_RAD_TILES);

	if (pTheApp)
	{
		CMainFrame		*pMainFrame;
		CChildFrame		*pChildFrame;
		CW3ZMapEditView	*pActiveView;
		pMainFrame = (CMainFrame*) pTheApp->GetMainFrame();
		if (pMainFrame)
		{
			pChildFrame = (CChildFrame*)pMainFrame->MDIGetActive();
			if (pChildFrame)
			{
				pActiveView = pChildFrame->GetEditView();
				if (pActiveView)
				{
					if (pActiveView->m_iViewFlags & VFL_LEVELS)
					{
						pActiveView->m_iViewFlags ^= VFL_LEVELS;
						pActiveView->m_iViewFlags |= VFL_TILES;
					}
					else if (pActiveView->m_iViewFlags & VFL_TILES)
					{
						pActiveView->m_iViewFlags ^= VFL_TILES;
						pRadTiles->SetCheck(0);
					}
					else
					{
						pActiveView->m_iViewFlags |= VFL_TILES;
					}
					pActiveView->InvalidateBitmap();
					pActiveView->Invalidate();
				}
			}
			else
			{
				if (pRadTiles)
				{pRadTiles->SetCheck(0);}
			}

		}
	}
}

void CViewModeDialog::OnRadLevel() 
{
	CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
	ASSERT_VALID(pTheApp);

	CButton *pRadLevel;
	pRadLevel = (CButton *)GetDlgItem(IDC_RAD_LEVEL);

	if (pTheApp)
	{
		CMainFrame		*pMainFrame;
		CChildFrame		*pChildFrame;
		CW3ZMapEditView	*pActiveView;
		pMainFrame = (CMainFrame*) pTheApp->GetMainFrame();
		if (pMainFrame)
		{
			pChildFrame = (CChildFrame*)pMainFrame->MDIGetActive();
			if (pChildFrame)
			{
				pActiveView = pChildFrame->GetEditView();
				if (pActiveView)
				{
					if (pActiveView->m_iViewFlags & VFL_TILES)
					{
						pActiveView->m_iViewFlags ^= VFL_TILES;
						pActiveView->m_iViewFlags |= VFL_LEVELS;
					}
					else if (pActiveView->m_iViewFlags & VFL_LEVELS)
					{
						pActiveView->m_iViewFlags ^= VFL_LEVELS;
						pRadLevel->SetCheck(0);
					}
					else
					{
						pActiveView->m_iViewFlags |= VFL_LEVELS;
					}
					pActiveView->InvalidateBitmap();
					pActiveView->Invalidate();
				}
			}
			else
			{
				if (pRadLevel)
				{pRadLevel->SetCheck(0);}
			}
		}
	}

}

void CViewModeDialog::OnChkDoodads() 
{
	CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
	ASSERT_VALID(pTheApp);

	CButton *pChkDoodads;
	pChkDoodads = (CButton *)GetDlgItem(IDC_CHK_DOODADS);

	if (pTheApp)
	{
		CMainFrame		*pMainFrame;
		CChildFrame		*pChildFrame;
		CW3ZMapEditView	*pActiveView;
		pMainFrame = (CMainFrame*) pTheApp->GetMainFrame();
		if (pMainFrame)
		{
			pChildFrame = (CChildFrame*)pMainFrame->MDIGetActive();
			if (pChildFrame)
			{
				pActiveView = pChildFrame->GetEditView();
				if (pActiveView)
				{
					if ((pChkDoodads) && (pChkDoodads->GetCheck()))
					{// boutons actif
						pActiveView->m_iViewFlags |= VFL_DOODADS;
					}
					else
					{// bouton inactif
						if (pActiveView->m_iViewFlags & VFL_DOODADS)
						{pActiveView->m_iViewFlags ^= VFL_DOODADS;}
					}
					pActiveView->InvalidateBitmap();
					pActiveView->Invalidate();
				}
			}
			else
			{
				if (pChkDoodads)
				{pChkDoodads->SetCheck(0);}
			}
		}
	}

}

void CViewModeDialog::OnChkPath() 
{
	CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
	ASSERT_VALID(pTheApp);

	CButton *pChkPath;
	pChkPath = (CButton *)GetDlgItem(IDC_CHK_PATH);

	if (pTheApp)
	{
		CMainFrame		*pMainFrame;
		CChildFrame		*pChildFrame;
		CW3ZMapEditView	*pActiveView;
		pMainFrame = (CMainFrame*) pTheApp->GetMainFrame();
		if (pMainFrame)
		{
			pChildFrame = (CChildFrame*)pMainFrame->MDIGetActive();
			if (pChildFrame)
			{
				pActiveView = pChildFrame->GetEditView();
				if (pActiveView)
				{
					if ((pChkPath) && (pChkPath->GetCheck()))
					{// boutons actif
						pActiveView->m_iViewFlags |= VFL_PATH;
					}
					else
					{// bouton inactif
						if (pActiveView->m_iViewFlags & VFL_PATH)
						{pActiveView->m_iViewFlags ^= VFL_PATH;}
					}
					pActiveView->InvalidateBitmap();
					pActiveView->Invalidate();
				}
			}
			else
			{
				if (pChkPath)
				{pChkPath->SetCheck(0);}
			}
		}
	}
}

void CViewModeDialog::OnChkShadows() 
{
	CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
	ASSERT_VALID(pTheApp);

	CButton *pChkShadows;
	pChkShadows = (CButton *)GetDlgItem(IDC_CHK_SHADOWS);

	if (pTheApp)
	{
		CMainFrame		*pMainFrame;
		CChildFrame		*pChildFrame;
		CW3ZMapEditView	*pActiveView;
		pMainFrame = (CMainFrame*) pTheApp->GetMainFrame();
		if (pMainFrame)
		{
			pChildFrame = (CChildFrame*)pMainFrame->MDIGetActive();
			if (pChildFrame)
			{
				pActiveView = pChildFrame->GetEditView();
				if (pActiveView)
				{
					if ((pChkShadows) && (pChkShadows->GetCheck()))
					{// boutons actif
						pActiveView->m_iViewFlags |= VFL_SHADOWS;
					}
					else
					{// bouton inactif
						if (pActiveView->m_iViewFlags & VFL_SHADOWS)
						{pActiveView->m_iViewFlags ^= VFL_SHADOWS;}
					}
					pActiveView->InvalidateBitmap();
					pActiveView->Invalidate();
				}
			}
			else
			{
				if (pChkShadows)
				{pChkShadows->SetCheck(0);}
			}
		}
	}
}

void CViewModeDialog::OnChkPlayers() 
{
	CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
	ASSERT_VALID(pTheApp);

	CButton *pChkPlayers;
	pChkPlayers = (CButton *)GetDlgItem(IDC_CHK_PLAYERS);

	if (pTheApp)
	{
		CMainFrame		*pMainFrame;
		CChildFrame		*pChildFrame;
		CW3ZMapEditView	*pActiveView;
		pMainFrame = (CMainFrame*) pTheApp->GetMainFrame();
		if (pMainFrame)
		{
			pChildFrame = (CChildFrame*)pMainFrame->MDIGetActive();
			if (pChildFrame)
			{
				pActiveView = pChildFrame->GetEditView();
				if (pActiveView)
				{
					if ((pChkPlayers) && (pChkPlayers->GetCheck()))
					{// boutons actif
						pActiveView->m_iViewFlags |= VFL_PLAYERS;
					}
					else
					{// bouton inactif
						if (pActiveView->m_iViewFlags & VFL_PLAYERS)
						{pActiveView->m_iViewFlags ^= VFL_PLAYERS;}
					}
					pActiveView->InvalidateBitmap();
					pActiveView->Invalidate();
				}
			}
			else
			{
				if (pChkPlayers)
				{pChkPlayers->SetCheck(0);}
			}
		}
	}
	
}

void CViewModeDialog::OnChkUnits() 
{
	CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
	ASSERT_VALID(pTheApp);

	CButton *pChkUnits;
	pChkUnits = (CButton *)GetDlgItem(IDC_CHK_UNITS);

	if (pTheApp)
	{
		CMainFrame		*pMainFrame;
		CChildFrame		*pChildFrame;
		CW3ZMapEditView	*pActiveView;
		pMainFrame = (CMainFrame*) pTheApp->GetMainFrame();
		if (pMainFrame)
		{
			pChildFrame = (CChildFrame*)pMainFrame->MDIGetActive();
			if (pChildFrame)
			{
				pActiveView = pChildFrame->GetEditView();
				if (pActiveView)
				{
					if ((pChkUnits) && (pChkUnits->GetCheck()))
					{// boutons actif
						pActiveView->m_iViewFlags |= VFL_UNITS;
					}
					else
					{// bouton inactif
						if (pActiveView->m_iViewFlags & VFL_UNITS)
						{pActiveView->m_iViewFlags ^= VFL_UNITS;}
					}
					pActiveView->InvalidateBitmap();
					pActiveView->Invalidate();
				}
			}
			else
			{
				if (pChkUnits)
				{pChkUnits->SetCheck(0);}
			}
		}
	}
	
}

void CViewModeDialog::OnChkTilesBlights() 
{
	CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
	ASSERT_VALID(pTheApp);

	CButton *pChkBlights;
	pChkBlights = (CButton *)GetDlgItem(IDC_CHK_TILES_BLIGHTS);

	if (pTheApp)
	{
		CMainFrame		*pMainFrame;
		CChildFrame		*pChildFrame;
		CW3ZMapEditView	*pActiveView;
		pMainFrame = (CMainFrame*) pTheApp->GetMainFrame();
		if (pMainFrame)
		{
			pChildFrame = (CChildFrame*)pMainFrame->MDIGetActive();
			if (pChildFrame)
			{
				pActiveView = pChildFrame->GetEditView();
				if (pActiveView)
				{
					if ((pChkBlights) && (pChkBlights->GetCheck()))
					{// boutons actif
						pActiveView->m_iViewFlags |= VFL_BLIGHTS;
					}
					else
					{// bouton inactif
						if (pActiveView->m_iViewFlags & VFL_BLIGHTS)
						{pActiveView->m_iViewFlags ^= VFL_BLIGHTS;}
					}
					pActiveView->InvalidateBitmap();
					pActiveView->Invalidate();
				}
			}
			else
			{
				if (pChkBlights)
				{pChkBlights->SetCheck(0);}
			}
		}
	}
}

void CViewModeDialog::OnChkTilesBoundaries() 
{
	CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
	ASSERT_VALID(pTheApp);

	CButton *pChkBoundaries;
	pChkBoundaries = (CButton *)GetDlgItem(IDC_CHK_TILES_BOUNDARIES);

	if (pTheApp)
	{
		CMainFrame		*pMainFrame;
		CChildFrame		*pChildFrame;
		CW3ZMapEditView	*pActiveView;
		pMainFrame = (CMainFrame*) pTheApp->GetMainFrame();
		if (pMainFrame)
		{
			pChildFrame = (CChildFrame*)pMainFrame->MDIGetActive();
			if (pChildFrame)
			{
				pActiveView = pChildFrame->GetEditView();
				if (pActiveView)
				{
					if ((pChkBoundaries) && (pChkBoundaries->GetCheck()))
					{// boutons actif
						pActiveView->m_iViewFlags |= VFL_BOUNDS;
					}
					else
					{// bouton inactif
						if (pActiveView->m_iViewFlags & VFL_BOUNDS)
						{pActiveView->m_iViewFlags ^= VFL_BOUNDS;}
					}
					pActiveView->InvalidateBitmap();
					pActiveView->Invalidate();
				}
			}
			else
			{
				if (pChkBoundaries)
				{pChkBoundaries->SetCheck(0);}
			}
		}
	}
	
}

void CViewModeDialog::OnChkTilesLayers() 
{
	CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
	ASSERT_VALID(pTheApp);

	CButton *pChkLayers;
	pChkLayers = (CButton *)GetDlgItem(IDC_CHK_TILES_LAYERS);

	if (pTheApp)
	{
		CMainFrame		*pMainFrame;
		CChildFrame		*pChildFrame;
		CW3ZMapEditView	*pActiveView;
		pMainFrame = (CMainFrame*) pTheApp->GetMainFrame();
		if (pMainFrame)
		{
			pChildFrame = (CChildFrame*)pMainFrame->MDIGetActive();
			if (pChildFrame)
			{
				pActiveView = pChildFrame->GetEditView();
				if (pActiveView)
				{
					if ((pChkLayers) && (pChkLayers->GetCheck()))
					{// boutons actif
						pActiveView->m_iViewFlags |= VFL_LAYERS;
					}
					else
					{// bouton inactif
						if (pActiveView->m_iViewFlags & VFL_LAYERS)
						{pActiveView->m_iViewFlags ^= VFL_LAYERS;}
					}
					pActiveView->InvalidateBitmap();
					pActiveView->Invalidate();
				}
			}
			else
			{
				if (pChkLayers)
				{pChkLayers->SetCheck(0);}
			}
		}
	}
	
}

void CViewModeDialog::OnChkTilesRamps() 
{
	CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
	ASSERT_VALID(pTheApp);

	CButton *pChkRamps;
	pChkRamps = (CButton *)GetDlgItem(IDC_CHK_TILES_RAMPS);

	if (pTheApp)
	{
		CMainFrame		*pMainFrame;
		CChildFrame		*pChildFrame;
		CW3ZMapEditView	*pActiveView;
		pMainFrame = (CMainFrame*) pTheApp->GetMainFrame();
		if (pMainFrame)
		{
			pChildFrame = (CChildFrame*)pMainFrame->MDIGetActive();
			if (pChildFrame)
			{
				pActiveView = pChildFrame->GetEditView();
				if (pActiveView)
				{
					if ((pChkRamps) && (pChkRamps->GetCheck()))
					{// boutons actif
						pActiveView->m_iViewFlags |= VFL_RAMPS;
					}
					else
					{// bouton inactif
						if (pActiveView->m_iViewFlags & VFL_RAMPS)
						{pActiveView->m_iViewFlags ^= VFL_RAMPS;}
					}
					pActiveView->InvalidateBitmap();
					pActiveView->Invalidate();
				}
			}
			else
			{
				if (pChkRamps)
				{pChkRamps->SetCheck(0);}
			}
		}
	}
	
}

void CViewModeDialog::OnChkTilesWater() 
{
	CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
	ASSERT_VALID(pTheApp);

	CButton *pChkWater;
	pChkWater = (CButton *)GetDlgItem(IDC_CHK_TILES_WATER);

	if (pTheApp)
	{
		CMainFrame		*pMainFrame;
		CChildFrame		*pChildFrame;
		CW3ZMapEditView	*pActiveView;
		pMainFrame = (CMainFrame*) pTheApp->GetMainFrame();
		if (pMainFrame)
		{
			pChildFrame = (CChildFrame*)pMainFrame->MDIGetActive();
			if (pChildFrame)
			{
				pActiveView = pChildFrame->GetEditView();
				if (pActiveView)
				{
					if ((pChkWater) && (pChkWater->GetCheck()))
					{// boutons actif
						pActiveView->m_iViewFlags |= VFL_WATER;
					}
					else
					{// bouton inactif
						if (pActiveView->m_iViewFlags & VFL_WATER)
						{pActiveView->m_iViewFlags ^= VFL_WATER;}
					}
					pActiveView->InvalidateBitmap();
					pActiveView->Invalidate();
				}
			}
			else
			{
				if (pChkWater)
				{pChkWater->SetCheck(0);}
			}
		}
	}
	
}

void CViewModeDialog::OnChkTilesWebounds() 
{
	CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
	ASSERT_VALID(pTheApp);

	CButton *pChkWEBounds;
	pChkWEBounds = (CButton *)GetDlgItem(IDC_CHK_TILES_WEBOUNDS);

	if (pTheApp)
	{
		CMainFrame		*pMainFrame;
		CChildFrame		*pChildFrame;
		CW3ZMapEditView	*pActiveView;
		pMainFrame = (CMainFrame*) pTheApp->GetMainFrame();
		if (pMainFrame)
		{
			pChildFrame = (CChildFrame*)pMainFrame->MDIGetActive();
			if (pChildFrame)
			{
				pActiveView = pChildFrame->GetEditView();
				if (pActiveView)
				{
					if ((pChkWEBounds) && (pChkWEBounds->GetCheck()))
					{// boutons actif
						pActiveView->m_iViewFlags |= VFL_WEBOUNDS;
					}
					else
					{// bouton inactif
						if (pActiveView->m_iViewFlags & VFL_WEBOUNDS)
						{pActiveView->m_iViewFlags ^= VFL_WEBOUNDS;}
					}
					pActiveView->InvalidateBitmap();
					pActiveView->Invalidate();
				}
			}
			else
			{
				if (pChkWEBounds)
				{pChkWEBounds->SetCheck(0);}
			}
		}
	}
	
}


void CViewModeDialog::UpdateViewFlags(int iViewFlags)
{

	CButton *pRadTiles;
	CButton *pRadLevel;
	CButton *pChkDoodads;
	CButton *pChkPath;
	CButton *pChkShadows;
	CButton *pChkPlayers;
	CButton *pChkUnits;
	CButton *pChkBlights;
	CButton *pChkBoundaries;
	CButton *pChkLayers;
	CButton *pChkRamps;
	CButton *pChkWater;
	CButton *pChkWEBounds;

	pRadTiles = (CButton *)GetDlgItem(IDC_RAD_TILES);
	pRadLevel = (CButton *)GetDlgItem(IDC_RAD_LEVEL);
	pChkDoodads = (CButton *)GetDlgItem(IDC_CHK_DOODADS);
	pChkPath = (CButton *)GetDlgItem(IDC_CHK_PATH);
	pChkShadows = (CButton *)GetDlgItem(IDC_CHK_SHADOWS);
	pChkPlayers = (CButton *)GetDlgItem(IDC_CHK_PLAYERS);
	pChkUnits = (CButton *)GetDlgItem(IDC_CHK_UNITS);
	pChkBlights = (CButton *)GetDlgItem(IDC_CHK_TILES_BLIGHTS);
	pChkBoundaries = (CButton *)GetDlgItem(IDC_CHK_TILES_BOUNDARIES);
	pChkLayers = (CButton *)GetDlgItem(IDC_CHK_TILES_LAYERS);
	pChkRamps = (CButton *)GetDlgItem(IDC_CHK_TILES_RAMPS);
	pChkWater = (CButton *)GetDlgItem(IDC_CHK_TILES_WATER);
	pChkWEBounds = (CButton *)GetDlgItem(IDC_CHK_TILES_WEBOUNDS);


	if (pRadTiles && pRadLevel)
	{
		if (iViewFlags & VFL_TILES)
		{
			pRadTiles->SetCheck(1);
			pRadLevel->SetCheck(0);
		}
		else if(iViewFlags & VFL_LEVELS)
		{
			pRadTiles->SetCheck(0);
			pRadLevel->SetCheck(1);
		}
		else
		{
			pRadTiles->SetCheck(0);
			pRadLevel->SetCheck(0);
		}
	}
	if (pChkDoodads)
	{
		if (iViewFlags & VFL_DOODADS)
		{
			pChkDoodads->SetCheck(1);
		}
		else
		{
			pChkDoodads->SetCheck(0);
		}
	}
	if (pChkPath)
	{
		if (iViewFlags & VFL_PATH)
		{
			pChkPath->SetCheck(1);
		}
		else
		{
			pChkPath->SetCheck(0);
		}
	}
	if (pChkShadows)
	{
		if (iViewFlags & VFL_SHADOWS)
		{
			pChkShadows->SetCheck(1);
		}
		else
		{
			pChkShadows->SetCheck(0);
		}
	}
	if (pChkPlayers)
	{
		if (iViewFlags & VFL_PLAYERS)
		{
			pChkPlayers->SetCheck(1);
		}
		else
		{
			pChkPlayers->SetCheck(0);
		}
	}
	if (pChkUnits)
	{
		if (iViewFlags & VFL_UNITS)
		{
			pChkUnits->SetCheck(1);
		}
		else
		{
			pChkUnits->SetCheck(0);
		}
	}
	if (pChkBlights)
	{
		if (iViewFlags & VFL_BLIGHTS)
		{
			pChkBlights->SetCheck(1);
		}
		else
		{
			pChkBlights->SetCheck(0);
		}
	}
	if (pChkBoundaries)
	{
		if (iViewFlags & VFL_BOUNDS)
		{
			pChkBoundaries->SetCheck(1);
		}
		else
		{
			pChkBoundaries->SetCheck(0);
		}
	}
	if (pChkLayers)
	{
		if (iViewFlags & VFL_LAYERS)
		{
			pChkLayers->SetCheck(1);
		}
		else
		{
			pChkLayers->SetCheck(0);
		}
	}
	if (pChkRamps)
	{
		if (iViewFlags & VFL_RAMPS)
		{
			pChkRamps->SetCheck(1);
		}
		else
		{
			pChkRamps->SetCheck(0);
		}
	}
	if (pChkWater)
	{
		if (iViewFlags & VFL_WATER)
		{
			pChkWater->SetCheck(1);
		}
		else
		{
			pChkWater->SetCheck(0);
		}
	}
	if (pChkWEBounds)
	{
		if (iViewFlags & VFL_WEBOUNDS)
		{
			pChkWEBounds->SetCheck(1);
		}
		else
		{
			pChkWEBounds->SetCheck(0);
		}
	}
}

