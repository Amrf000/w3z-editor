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
// ShadowMap.cpp: implementation of the CShadowMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "War3TypesAndConstants.h"
#include "ShadowMap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShadowMap::CShadowMap()
{
	m_pByteMap = NULL;
}

CShadowMap::~CShadowMap()
{
	if (m_pByteMap)
	{delete []m_pByteMap;}
}

void CShadowMap::SetSize(int width, int height)
{
	byte* pOldByteMap;
	int oldwidth, oldheight, x, y, maxx, maxy;

	if ((width > 0) && (height > 0) && (width <= SHAD_MAX_WIDTH) && (height <= SHAD_MAX_HEIGHT))
	{
		pOldByteMap = m_pByteMap;
		oldwidth = m_Width;
		oldheight = m_Height;
		m_Height = height;
		m_Width = width;
		m_pByteMap = new byte[width*height];
		memset( m_pByteMap, 0, width*height);
		if (pOldByteMap)
		{
			if (m_Width < oldwidth)
			{
				maxx = m_Width;
			}
			else
			{
				maxx = oldwidth;
			}
			if (m_Height < oldheight)
			{
				maxy = m_Height;
			}
			else
			{
				maxy = oldheight;
			}
			for (y=0; y < maxy; y++)
			for (x=0; x < maxx; x++)
			{
				m_pByteMap[y*m_Width + x] = pOldByteMap[y*oldwidth + x];
			}
			delete []pOldByteMap;
		}

	}
}

void CShadowMap::SetShadowAt(int x, int y)
{
	if ((x >= 0) && (x < m_Width) && (y >= 0) && (y < m_Height))
	{m_pByteMap[y*m_Width+x] = 0x0000;}
}

void CShadowMap::SetShadowAt(CRect rect)
{
	AfxMessageBox("Not implemented yet!", MB_OK | MB_APPLMODAL | MB_ICONSTOP);
}

void CShadowMap::RemoveShadowAt(int x, int y)
{
	if ((x >= 0) && (x < m_Width) && (y >= 0) && (y < m_Height))
	{m_pByteMap[y*m_Width+x] = 0x0000;}
}

void CShadowMap::RemoveShadowAt(CRect rect)
{
	AfxMessageBox("Not implemented yet!", MB_OK | MB_APPLMODAL | MB_ICONSTOP);
}

int CShadowMap::GetWidth()
{
	return m_Width;
}

int CShadowMap::GetHeight()
{
	return m_Height;
}

byte* CShadowMap::GetBytemapPtr()
{
	return m_pByteMap;
}
