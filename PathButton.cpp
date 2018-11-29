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
// PathButton.cpp : implementation file
//

#include "stdafx.h"
#include "w3zmapedit.h"
#include "PathButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPathButton

CPathButton::CPathButton()
{
    CW3ZMapEditApp(* pTheApp) = (CW3ZMapEditApp *)AfxGetApp();
	m_pAppSettings = &(pTheApp->m_AppSettings);
}

CPathButton::~CPathButton()
{
}


BEGIN_MESSAGE_MAP(CPathButton, CButton)
	//{{AFX_MSG_MAP(CPathButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPathButton message handlers


BOOL CPathButton::IsModeActive(int mode, int x, int y)
{
	BOOL ret = TRUE;

/*
const int PATH_NOTHING	= -1;// nothing
const int PATH_PLAIN	= 0;// plain color
const int PATH_STRIPED1	= 1;// striped //  ((x-y)%4)
const int PATH_STRIPED2	= 2;// striped \\  ((x+y)%4)
const int PATH_STRIPED3	= 3;// striped ==  (y%4)
const int PATH_STRIPED4	= 4;// striped ||  (x%4)
const int PATH_STRIPED5	= 5;// striped ##  (((x+2)%4) && ((y+2)%4))
const int PATH_DOTS1	= 6;// dots ..  (((x+1)%4) || ((y+1)%4))
const int PATH_DOTS2	= 7;// more dots :: (((x+3)%2) || ((y+3)%2))
*/
	if (mode == PATH_PLAIN)
	{ret = TRUE;}
	else if (mode == PATH_NOTHING)
	{ret = FALSE;}
	else if (mode == PATH_STRIPED1)
	{
		if ((x-y)%4)
		{ret = FALSE;}
	}
	else if (mode == PATH_STRIPED2)
	{
		if ((x+y)%4)
		{ret = FALSE;}
	}
	else if (mode == PATH_STRIPED3)
	{
		if (y%4)
		{ret = FALSE;}
	}
	else if (mode == PATH_STRIPED4)
	{
		if (x%4)
		{ret = FALSE;}
	}
	else if (mode == PATH_STRIPED5)
	{
		if (((x+2)%4) && ((y+2)%4))
		{ret = FALSE;}
	}
	else if (mode == PATH_DOTS1)
	{
		if (((x+1)%4) || ((y+1)%4))
		{ret = FALSE;}
	}
	else if (mode == PATH_DOTS2)
	{
		if (((x)%2) || ((y)%2))
		{ret = FALSE;}
	}
	return ret;
}

void CPathButton::SetFlags(int flags)
{
	m_iFlags = flags;
	Invalidate();
}

void CPathButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);     //Get device context object
	CRect rt;
	COLORREF crColor ;
	BOOL bNoBuildSet, bNoWalkSet,bNoFlySet, bNoWaterSet;
	int x, y, xmin, xmax, ymin, ymax;

	rt = lpDrawItemStruct->rcItem;        // get button rect

	if (rt.TopLeft().x < rt.BottomRight().x)
	{
		xmin = rt.TopLeft().x;
		xmax = rt.BottomRight().x;
	}
	else
	{
		xmax = rt.TopLeft().x;
		xmin = rt.BottomRight().x;
	}

	if (rt.TopLeft().y < rt.BottomRight().y)
	{
		ymin = rt.TopLeft().y;
		ymax = rt.BottomRight().y;
	}
	else
	{
		ymax = rt.TopLeft().y;
		ymin = rt.BottomRight().y;
	}

	//	dc.FillSolidRect(rt, m_crBkColorState1);  // erase button background
	for (y = ymin; y < ymax; y++)
	for (x = xmin; x < xmax; x++)
	{
		crColor = m_pAppSettings->pathNormal.clrBlue * 0x00010000
				  + m_pAppSettings->pathNormal.clrGreen * 0x00000100
				  + m_pAppSettings->pathNormal.clrRed;
		bNoBuildSet = bNoWalkSet = bNoFlySet = bNoWaterSet = FALSE;

		if ((m_iFlags & PATH_NOBUILD)
			&& (m_iFlags & PATH_NOWALK)
			&& (m_iFlags & PATH_NOFLY))
		{
			if (IsModeActive(m_pAppSettings->pathNoBuildNoWalkNoFly.mode, x, y))
			{
				crColor = m_pAppSettings->pathNoBuildNoWalkNoFly.clrBlue * 0x00010000
						  + m_pAppSettings->pathNoBuildNoWalkNoFly.clrGreen * 0x00000100
						  + m_pAppSettings->pathNoBuildNoWalkNoFly.clrRed;
				bNoBuildSet = bNoWalkSet = bNoFlySet = TRUE;
			}
		}
		else if ((m_iFlags & PATH_NOBUILD)
				&& (m_iFlags & PATH_NOWALK)
				&& (m_iFlags & PATH_NOWATER))
		{
			if (IsModeActive(m_pAppSettings->pathNoBuildNoWalkNoWater.mode, x, y))
			{
				crColor = m_pAppSettings->pathNoBuildNoWalkNoWater.clrBlue * 0x00010000
						  + m_pAppSettings->pathNoBuildNoWalkNoWater.clrGreen * 0x00000100
						  + m_pAppSettings->pathNoBuildNoWalkNoWater.clrRed;
				bNoBuildSet = bNoWalkSet = bNoWaterSet = TRUE;
			}
		}
		else if ((m_iFlags & PATH_NOBUILD)
				&& (m_iFlags & PATH_NOWALK))
		{
			if (IsModeActive(m_pAppSettings->pathNoBuildNoWalk.mode, x, y))
			{
				crColor = m_pAppSettings->pathNoBuildNoWalk.clrBlue * 0x00010000
						  + m_pAppSettings->pathNoBuildNoWalk.clrGreen * 0x00000100
						  + m_pAppSettings->pathNoBuildNoWalk.clrRed;
				bNoBuildSet = bNoWalkSet = TRUE;
			}
		}

		if (!bNoBuildSet
			&& (m_iFlags & PATH_NOBUILD)
			&& IsModeActive(m_pAppSettings->pathNoBuild.mode, x, y))
		{
			crColor = m_pAppSettings->pathNoBuild.clrBlue * 0x00010000
					  + m_pAppSettings->pathNoBuild.clrGreen * 0x00000100
					  + m_pAppSettings->pathNoBuild.clrRed;
		}

		if (!bNoWalkSet
			&& (m_iFlags & PATH_NOWALK)
			&& IsModeActive(m_pAppSettings->pathNoWalk.mode, x, y))
		{
			crColor = m_pAppSettings->pathNoWalk.clrBlue * 0x00010000
					  + m_pAppSettings->pathNoWalk.clrGreen * 0x00000100
					  + m_pAppSettings->pathNoWalk.clrRed;
		}

		if (!bNoFlySet
			&& (m_iFlags & PATH_NOFLY)
			&& IsModeActive(m_pAppSettings->pathNoFly.mode, x, y))
		{
			crColor = m_pAppSettings->pathNoFly.clrBlue * 0x00010000
					  + m_pAppSettings->pathNoFly.clrGreen * 0x00000100
					  + m_pAppSettings->pathNoFly.clrRed;
		}
			
		if (!bNoWaterSet
			&& (m_iFlags & PATH_NOWATER)
			&& IsModeActive(m_pAppSettings->pathNoWater.mode, x, y))
		{
			crColor = m_pAppSettings->pathNoWater.clrBlue * 0x00010000
					  + m_pAppSettings->pathNoWater.clrGreen * 0x00000100
					  + m_pAppSettings->pathNoWater.clrRed;
		}

		if ((m_iFlags & PATH_BLIGHT)
			&& IsModeActive(m_pAppSettings->pathBlight.mode, x, y))
		{
			crColor = m_pAppSettings->pathBlight.clrBlue * 0x00010000
					  + m_pAppSettings->pathBlight.clrGreen * 0x00000100
					  + m_pAppSettings->pathBlight.clrRed;
		}
		
		if ((m_iFlags & PATH_UNKNOWN)
			&& IsModeActive(m_pAppSettings->pathUnknown.mode, x, y))
		{
			crColor = m_pAppSettings->pathUnknown.clrBlue * 0x00010000
					  + m_pAppSettings->pathUnknown.clrGreen * 0x00000100
					  + m_pAppSettings->pathUnknown.clrRed;
		}

		if ((m_iFlags & PATH_UNKN01)
			&& IsModeActive(m_pAppSettings->pathUnkn01.mode, x, y))
		{
			crColor = m_pAppSettings->pathUnkn01.clrBlue * 0x00010000
					  + m_pAppSettings->pathUnkn01.clrGreen * 0x00000100
					  + m_pAppSettings->pathUnkn01.clrRed;
		}

		if ((m_iFlags & PATH_UNKN02)
			&& IsModeActive(m_pAppSettings->pathUnkn02.mode, x, y))
		{
			crColor = m_pAppSettings->pathUnkn02.clrBlue * 0x00010000
					  + m_pAppSettings->pathUnkn02.clrGreen * 0x00000100
					  + m_pAppSettings->pathUnkn02.clrRed;
		}
		dc.SetPixel( x, y, crColor );
	}

	dc.DrawEdge(rt,EDGE_SUNKEN,BF_RECT);    // Draw a sunken face
	dc.Detach();
}



