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
// MiniMap.cpp: implementation of the CMiniMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <afxtempl.h>
#include "War3TypesAndConstants.h"
#include "MMPIcon.h"
#include "MiniMap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMiniMap::CMiniMap()
{
	m_iMMPVersion = MMP_VERSION_DEFAULT;
	m_tposCurrent = NULL;
}

CMiniMap::~CMiniMap()
{

}

/*
AddIcon
*******
Add a minimap icons in the list. It copies the icon provided.

In:
  const CMMPIcon &mmpIcon: icon to add

Out:
  nothing

Return:
  nothing

*/
void CMiniMap::AddIcon(CMMPIcon mmpIcon)
{
	m_tlistIconList.AddTail(mmpIcon);
}

int CMiniMap::GetIconsCount()
{
	return m_tlistIconList.GetCount();
}


/*
GoFirstIcon
***********
Place the current possition to the head.

In:
  nothing

Out:
  nothing

Return:
  nothing

*/
void CMiniMap::GoFirstIcon()
{
	m_tposCurrent = m_tlistIconList.GetHeadPosition();
}


/*
GetNext
*******
Returns the next element in the list.

In:
  nothing

Out:
  nothing

Return:
  CMMPIcon: the next element in the list
*/
CMMPIcon CMiniMap::GetNext()
{
	if (NULL == m_tposCurrent)
	{
		GoFirstIcon();
	}
	return m_tlistIconList.GetNext(m_tposCurrent);
}


/*
ClearList
*********
Remove all the elements from the list.

In:
  nothing

Out:
  nothing

Return:
  nothing

*/
void CMiniMap::ClearList()
{
	m_tlistIconList.RemoveAll();
}


/*
DelIcon
*******
Remove an icon from the list.

In:
  const CMMPIcon &mmpIcon: icon to find an remove.

Out:
  nothing

Return:
  nothing

*/
void CMiniMap::DelIcon(CMMPIcon mmpIcon)
{
	POSITION pos;
	pos = m_tlistIconList.Find(mmpIcon);
	if (NULL != pos)
	{
		m_tlistIconList.RemoveAt(pos);
	}
}


/*
GetClosestIcon
**************
Returns the closets icons of the passed coordinates.
Coordinates must be between 0 and 255; middle of the map is 128.
Top left is (0,0), bottom right (255,255).
If no icons was found, the icon returned has an undefined icon type.

In:
  int v_iX: X coordiante (0-255)
  int v_iY: Y coordiante (0-255)

Out:
  nothing

Return:
  nothing

*/
CMMPIcon CMiniMap::GetClosestIcon(int v_iX, int v_iY)
{
	CMMPIcon retMmpIcon, mmpIcon;
	POSITION pos;
	double dDist;
	double dDistMin;

	// dDistMin set to max distance
	dDistMin = MMP_COORD_MAX*MMP_COORD_MAX;
	// start the search from the first element
	pos = m_tlistIconList.GetHeadPosition();
	// process to all the element of the list
	while(NULL != pos)
	{
		mmpIcon = m_tlistIconList.GetNext(pos);
		dDist = (v_iX - mmpIcon.m_iX)*(v_iX - mmpIcon.m_iX) + (v_iY - mmpIcon.m_iY)*(v_iY - mmpIcon.m_iY);
		if (dDist <= dDistMin)
		{
			retMmpIcon = mmpIcon;
			dDistMin = dDist;
		}
	}
	return retMmpIcon;
}
