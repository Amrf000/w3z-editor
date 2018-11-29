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
// DoodadsList.cpp: implementation of the CDoodadsList class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "War3TypesAndConstants.h"
#include "Doodad.h"
#include "IDList.h"
#include "DoodadsList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


/********************************************************************************/
// CListedDoodad class
/********************************************************************************/
/*
Construction/destruction
*/
CDoodadsList::CListedDoodad::CListedDoodad()
{
	next		= NULL;
	mapnext		= NULL;
	radiusnext	= NULL;
}

CDoodadsList::CListedDoodad::~CListedDoodad()
{

}
/********************************************************************************/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDoodadsList::CDoodadsList()
{
	m_iNbDoodads = 0;
	m_iDoodadsMapSize = 0;
	head = NULL;
	last = NULL;
	current = NULL;
	m_pDoodadsMap = NULL;
	m_pMapCurrent = NULL;
	m_fMapWidth = 0;
	m_fMapHeight = 0;
	m_fMapOrgX = 0;
	m_fMapOrgY = 0;
	m_iHighestID = -1;
}

CDoodadsList::~CDoodadsList()
{
	ClearList();
	if (m_pDoodadsMap)
	{
		delete[] m_pDoodadsMap;
	}
}

void CDoodadsList::ClearList()
{
	PCListedDoodad pDoodad;
	current = NULL;
	// clear doodads list
	while(head)
	{
		pDoodad = head;
		head = head->next;
		delete pDoodad;
	}
	last = NULL;
	m_iNbDoodads = 0;
	// clear Free IDs list
	m_FreeIDList.ClearList();
	m_iHighestID = -1;
	//clear doodads map
	SetSize(m_fMapWidth, m_fMapHeight);
}

void CDoodadsList::AddDoodad(CDoodad &doodad)
{
	PCListedDoodad pLDoodad;

	doodad.ID = m_FreeIDList.GetAndRemoveID();
	if (doodad.ID < 0)
	{// empty Free ID list
		m_iHighestID++;
		doodad.ID = m_iHighestID;
	}
	pLDoodad = new CListedDoodad();
	pLDoodad->doodad = doodad;
	pLDoodad->next = NULL;
	pLDoodad->mapnext = NULL;

	if (last)
	{// not empty list
		last->next = pLDoodad;
		last = pLDoodad;
	}
	else
	{// empty list
		head = pLDoodad;
		last = pLDoodad;
	}
	m_iNbDoodads++;

	PlaceDoodad(pLDoodad);
}

void CDoodadsList::DelDoodads(CRect area)
{
	AfxMessageBox("Not implemented yet!", MB_OK | MB_APPLMODAL | MB_ICONSTOP);
// m_iNbDoodads--;
// next changes
// mapnext changes
}

int CDoodadsList::GetNbDoodads()
{
	return m_iNbDoodads;
}

void CDoodadsList::GoFirstDoodad()
{
	current = head;
}

// returns next doodad in the list
// if the last element has been reached, the doodad returned has an empty type ("")
CDoodad CDoodadsList::GetNext()
{
	CDoodad doodad;
	if(current)
	{
		doodad = current->doodad;
		current = current->next;
	}
	else
	{
		current = head;
		doodad.type = "";
	}
	return doodad;
}

CDoodad CDoodadsList::GetDoodad(int ID)
{
	PCListedDoodad pLDoodad;
	CDoodad doodad;
	pLDoodad = head;
	while(pLDoodad && (pLDoodad->doodad.ID != ID))
	{
		pLDoodad = pLDoodad->next;
	}
	if (pLDoodad)
	{
		doodad = pLDoodad->doodad;
	}

	return doodad;
}

void CDoodadsList::SetDoodad(CDoodad doodad)
{
	PCListedDoodad pLDoodad;

	pLDoodad = head;
	while(pLDoodad)
	{
		if (pLDoodad->doodad.ID == doodad.ID)
		{
			pLDoodad->doodad = doodad;
			PlaceDoodad(pLDoodad);
			pLDoodad = NULL;
		}
		else
		{
			pLDoodad = pLDoodad->next;
		}
	}
}

CDoodad CDoodadsList::GetClosestDoodad(float x, float y)
{
	PCListedDoodad pLDoodad;
	CDoodad closestDoodad;
	float distance, best;

	best = -1;
	pLDoodad = head;
	while(pLDoodad)
	{
		distance = (pLDoodad->doodad.x - x)*(pLDoodad->doodad.x - x) + (pLDoodad->doodad.y - y)*(pLDoodad->doodad.y - y);
		if ((best < 0) || (distance < best))
		{
			best = distance;
			closestDoodad = pLDoodad->doodad;		
		}
		pLDoodad = pLDoodad->next;
	}
	return closestDoodad;

}

void CDoodadsList::DelDoodad(CDoodad doodad)
{
	PCListedDoodad pLDoodad;
	PCListedDoodad pLDoodadPrev;

	pLDoodad = head;
	pLDoodadPrev = NULL;
	while(pLDoodad)
	{
		if (pLDoodad->doodad.ID == doodad.ID)
		{
			// check for list head
			if (pLDoodad == head)
			{// is list head
				head = pLDoodad->next;
			}
			else
			{// otherwise, remove elements from the list
				pLDoodadPrev->next = pLDoodad->next;
			}
			// update list tail if needed
			if (pLDoodad == last)
			{ // works for both tail and head
				last = pLDoodadPrev;
			}
			m_iNbDoodads--;
			// updates free ID list if needed
			m_FreeIDList.AddID(pLDoodad->doodad.ID);
			// updates doodads table
			UnplaceDoodad(pLDoodad);
			// updates radius table
			RemoveFromRadiusList(pLDoodad);
			delete pLDoodad;
			pLDoodad = NULL;
		}
		else
		{
			pLDoodadPrev = pLDoodad;
			pLDoodad = pLDoodad->next;
		}
	}
}


void CDoodadsList::UnselectAll()
{
	PCListedDoodad pLDoodad;
	pLDoodad = head;
	while(pLDoodad)
	{
		if (pLDoodad->doodad.selected)
		{
			pLDoodad->doodad.selected = false;
		}
		pLDoodad = pLDoodad->next;
	}
}


// set doodads map width and height
// if a value is 0 or negative, the value is unchanged
void CDoodadsList::SetSize(float width, float height)
{
	int i,size;
	if (width > 0)
	{
		m_fMapWidth = width;
	}
	if (height > 0)
	{
		m_fMapHeight = height;
	}

	if (m_pDoodadsMap)
	{
		delete[] m_pDoodadsMap;
	}

	size = (int) (m_fMapWidth/DOOD_MAP_SUBDIV * m_fMapHeight/DOOD_MAP_SUBDIV);

	if (size > 0)
	{
		m_pDoodadsMap = new PCListedDoodad[size];
		m_iDoodadsMapSize = size;
		for (i = 0; i < size; i++)
		{
			m_pDoodadsMap[i] = NULL;
		}
		RePlaceAllDoodads();
	}
}

// sets origin offsets
void CDoodadsList::SetDoodadMapOrigin(float x, float y)
{
	m_fMapOrgX = x;
	m_fMapOrgY = y;
	RePlaceAllDoodads();
}

// place an existing listed doodad on the doodads map
void CDoodadsList::PlaceDoodad(PCListedDoodad pLDoodad)
{
	int posx, posy, pos;

	// remove the doodad from the doodad map
	UnplaceDoodad(pLDoodad);
	posx = (int)((pLDoodad->doodad.x - m_fMapOrgX)/128);
	posy = (int)((pLDoodad->doodad.y - m_fMapOrgY)/128);
	if ((0 <= posx) && (0 <= posy) && (posx*DOOD_MAP_SUBDIV <= m_fMapWidth) && (posy*DOOD_MAP_SUBDIV <= m_fMapHeight))
	{
		pos = (int)(posx + posy*m_fMapWidth/DOOD_MAP_SUBDIV);
		if ((m_pDoodadsMap)
			&& ( pos >= 0)
			&& (pos < m_iDoodadsMapSize))
		{
			pLDoodad->mapnext = m_pDoodadsMap[pos];
			m_pDoodadsMap[pos] = pLDoodad;
		}
	}

}

// remove a doodad from the doodad map but not from the doodad list
void CDoodadsList::UnplaceDoodad(PCListedDoodad pUpLDoodad)
{
	PCListedDoodad pLDoodad;
	int i;

	if (!pUpLDoodad)
	{return;}

	if (pUpLDoodad->mapnext == pUpLDoodad)
	{pUpLDoodad->mapnext = NULL;}

	for (i=0; i < m_iDoodadsMapSize;i++)
	{
		if (m_pDoodadsMap[i] == pUpLDoodad)
		{m_pDoodadsMap[i] = pUpLDoodad->mapnext;}
	}
	pLDoodad = head;
	while(pLDoodad)
	{
		if (pLDoodad->mapnext == pUpLDoodad)
		{pLDoodad->mapnext = pUpLDoodad->mapnext;}
		pLDoodad = pLDoodad->next;
	}
}

/*
Place again all the doodads on the doodads map
*/
void CDoodadsList::RePlaceAllDoodads()
{
	PCListedDoodad pLDoodad;
	int i;
	for (i=0; i < m_iDoodadsMapSize;i++)
	{
		m_pDoodadsMap[i] = NULL;
	}
	pLDoodad = head;
	while(pLDoodad)
	{
		pLDoodad->mapnext = NULL;
		pLDoodad = pLDoodad->next;
	}
	pLDoodad = head;
	while (pLDoodad)
	{
		PlaceDoodad(pLDoodad);
		pLDoodad = pLDoodad->next;
	}
}

void CDoodadsList::GoMapDoodad(float fx, float fy)
{
	int posx, posy, pos;

	posx = (int)((fx - m_fMapOrgX)/DOOD_MAP_SUBDIV);
	posy = (int)((fy - m_fMapOrgY)/DOOD_MAP_SUBDIV);
	if ((0 <= posx) && (0 <= posy) && (posx*DOOD_MAP_SUBDIV < m_fMapWidth) && (posy*DOOD_MAP_SUBDIV < m_fMapHeight))
	{
		pos = (int)(posx + posy*m_fMapWidth/DOOD_MAP_SUBDIV);
		if ((m_pDoodadsMap)
			&& ( pos >= 0)
			&& (pos < m_iDoodadsMapSize))
		{
			m_pMapCurrent = m_pDoodadsMap[pos];
		}
	}
}

// return next doodad in the doodad map or a doodad with a null type ("")
CDoodad CDoodadsList::GetMapNext()
{
	CDoodad doodad;
	doodad.type = "";
	if (m_pMapCurrent)
	{
		doodad = m_pMapCurrent->doodad;
		m_pMapCurrent = m_pMapCurrent->mapnext;
	}
	return doodad;
}

void CDoodadsList::FindRadius(float fx, float fy, float fradius)
{
	AfxMessageBox("Not implemented yet!", MB_OK | MB_APPLMODAL | MB_ICONSTOP);
//radiusnext
}

CDoodad CDoodadsList::GetRadiusNext()
{
	CDoodad doodad;
	AfxMessageBox("Not implemented yet!", MB_OK | MB_APPLMODAL | MB_ICONSTOP);

	return doodad;
}

void CDoodadsList::RemoveFromRadiusList(PCListedDoodad pRadLDoodad)
{
	PCListedDoodad pLDoodad;

	if (!pRadLDoodad)
	{return;}

	if (pRadLDoodad->radiusnext == pRadLDoodad)
	{pRadLDoodad->mapnext = NULL;}

	pLDoodad = head;
	while(pLDoodad)
	{
		if (pLDoodad->radiusnext == pRadLDoodad)
		{pLDoodad->radiusnext = pRadLDoodad->radiusnext;}
		pLDoodad = pLDoodad->next;
	}

}

void CDoodadsList::UnselectDoodad(CDoodad doodad)
{
	PCListedDoodad pLDoodad;

	pLDoodad = head;
	while(pLDoodad)
	{
		if (pLDoodad->doodad.ID == doodad.ID)
		{
			pLDoodad->doodad.selected = FALSE;
			PlaceDoodad(pLDoodad);
			pLDoodad = NULL;
		}
		else
		{
			pLDoodad = pLDoodad->next;
		}
	}
}

