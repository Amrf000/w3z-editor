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
// PathMap.cpp: implementation of the CPathMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "War3TypesAndConstants.h"
#include "PathMap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPathMap::CPathMap()
{
	m_iHeight = 0;
	m_iWidth = 0;
	m_pbyPathMap = NULL;
}

CPathMap::~CPathMap()
{
	if (m_pbyPathMap)
	{delete []m_pbyPathMap;}
}


/*
SetSize
*******
Sets path map size.

In:
 int width: with of the path map (must be > 0 and < PATH_MAX_WIDTH)
 int height: height of the path map (must be > 0 and < PATH_MAX_HEIGHT)

Out:
 nothing

Return:
 BOOL: TRUE if successfull, FALSE otherwise

*/
BOOL CPathMap::SetSize(int v_iWidth, int v_iHeight)
{
	BOOL bRet = TRUE;

	if ((v_iWidth > 0) && (v_iHeight > 0)
		&& (v_iWidth < PATH_MAX_WIDTH) && (v_iHeight < PATH_MAX_HEIGHT))
	{
		if ((v_iWidth != m_iWidth) && (v_iHeight != m_iHeight))
		{// only if the size has changed
			if (NULL != m_pbyPathMap)
			{// deletes old path
				delete []m_pbyPathMap;
			}
			m_pbyPathMap = new byte[v_iWidth*v_iHeight];
			if (m_pbyPathMap)
			{ // memory allocated
				m_iHeight = v_iHeight;
				m_iWidth = v_iWidth;
				memset( m_pbyPathMap, PATH_DEFAULT, v_iWidth*v_iHeight);
			}
			else
			{ // failled to allocate memory
				m_iHeight = 0;
				m_iWidth = 0;
				bRet = FALSE;
			}
		}
		else
		{// if the size has not changed, just reset the data
			memset( m_pbyPathMap, PATH_DEFAULT, v_iWidth*v_iHeight);
		}
	}
	else
	{
		bRet = FALSE;
	}
	return bRet;
}


/*
GetWidth
********
Returns path map width.

In:
 nothing

Out:
 nothing

Return:
 int: path map width.

*/
int CPathMap::GetWidth()
{
	return m_iWidth;
}


/*
GetHeight
*********
Returns path map height.

In:
 nothing

Out:
 nothing

Return:
 int: path map height.

*/
int CPathMap::GetHeight()
{
	return m_iHeight;
}


/*
GetLength
*********
Retruns the length of the path map (in byte).

In:
 nothing

Out:
 nothing

Return:
 int: path map lenght (0 if size not set or empty path map)

*/
int CPathMap::GetLength()
{
	return m_iWidth*m_iHeight;
}


/*
GetGetData
**********
Returns a pointer to the path map (a byte map).

In:
 nothing

Out:
 nothing

Return:
 byte*: a pointer to the path map (byte map), NULL if there is no path map

*/
byte* CPathMap::GetData()
{
	return m_pbyPathMap;
}
