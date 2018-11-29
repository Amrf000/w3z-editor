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
// TilePointsTable.cpp: implementation of the CTilePointsTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "War3TypesAndConstants.h"
#include "TilePoint.h"
#include "TilePointsTable.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTilePointsTable::CTilePointsTable()
{
	m_iW3EVersion = W3E_VERSION_DEFAULT;
	m_pTilePoints = NULL;
	m_width = 0;
	m_height = 0;
}

CTilePointsTable::~CTilePointsTable()
{
	ClearTable();
}


CTilePointsTable::CTilePointsTable(int width, int height)
{
	CTilePointsTable();
	SetSize(width, height);
}


void CTilePointsTable::SetSize(int width, int height)
{
	int size, i, j;
	CTilePoint* m_pNewTable;
	

	if ((width > 0) && (height > 0))
	{
		size = width * height;
		if (m_pTilePoints)
		{// copy old map
			m_pNewTable = new CTilePoint[size];
			j = 0;
			while ((j < m_height) && (j < height))
			{
				i = 0;
				while ((i < m_width) && (i < width))
				{
					m_pNewTable[j*width +i] = m_pTilePoints[j*m_width +i];
					i++;
				}
				j++;
			}
			ClearTable(); // clear old map
			m_pTilePoints = m_pNewTable;
		}
		else
		{// create a new one
			ClearTable();
			m_pTilePoints = new CTilePoint[size];
		}
		m_width = width;
		m_height = height;
	}

}


void CTilePointsTable::ClearTable()
{
	if (m_pTilePoints)
	{
		delete []m_pTilePoints;
	}
	m_pTilePoints = NULL;
	m_width = 0;
	m_height = 0;
}


CTilePoint CTilePointsTable::GetTilePoint(int num)
{
	CTilePoint tilepoint;
	if ((num >= 0) && (num < (m_width*m_height)))
	{
		tilepoint = m_pTilePoints[num];
	}
	return tilepoint;
}


void CTilePointsTable::SetTilePoint(int num, CTilePoint tilepoint)
{
	if ((num >= 0) && (num < (m_width*m_height)))
	{
		m_pTilePoints[num] = tilepoint;
	}
}


int CTilePointsTable::GetHeight()
{
 return m_height;
}


int CTilePointsTable::GetWidth()
{
 return m_width;
}

void CTilePointsTable::UnselectAll()
{
	int i, size;
	size = m_height*m_width;
	for (i=0; i < size; i++)
	{
		m_pTilePoints[i].selected = false;
		m_pTilePoints[i].viewcolor = RGB_BLACK;
	}
}

void CTilePointsTable::ReplaceGroundType(int tileold, int tilenew, bool selectedonly)
{
	if ((tilenew >= 0) && (tilenew < 16))
	{
		int i, size;
		size = m_height*m_width;
		if (selectedonly)
		{
			for (i=0; i < size; i++)
			{
				if (m_pTilePoints[i].selected && (m_pTilePoints[i].groundtype == tileold))
				{
					m_pTilePoints[i].groundtype = (byte)tilenew;
				}
			}
		}
		else
		{
			for (i=0; i < size; i++)
			{
				if (m_pTilePoints[i].groundtype == tileold)
				{
					m_pTilePoints[i].groundtype = (byte)tilenew;
				}
			}
		}
	}
}

void CTilePointsTable::SetVersion(int iVersion)
{
	m_iW3EVersion = iVersion;
}

int CTilePointsTable::GetVersion()
{
	return m_iW3EVersion;
}
