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
// DoodadsList.h: interface for the CDoodadsList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOODADSLIST_H__6B9367FE_16EF_4498_9827_6FD321F8836D__INCLUDED_)
#define AFX_DOODADSLIST_H__6B9367FE_16EF_4498_9827_6FD321F8836D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Doodad.h"
#include "IDList.h"

class CDoodadsList  
{
	// listed doodad type
	class CListedDoodad  
	{
	public:
		CListedDoodad();
		virtual ~CListedDoodad();
		CListedDoodad *next; // next in the linear list
		CListedDoodad *mapnext; // next in the square of the map where the doodad is
		CListedDoodad *radiusnext; // next when a findradius occured
		CDoodad doodad;
	};

	typedef CListedDoodad (*PCListedDoodad);


public:
	CByteArray m_baUnsupportedBuffer;
	void UnselectDoodad(CDoodad doodad);
	void UnselectAll();
	void RePlaceAllDoodads();
	void SetDoodadMapOrigin(float x, float y);
	void SetSize(float width, float height);
	CDoodad GetRadiusNext();
	void FindRadius(float fx, float fy, float fradius);
	CDoodad GetClosestDoodad(float x, float y);
	CDoodad GetMapNext();
	void GoMapDoodad(float fx, float fy);
	CDoodad GetNext();
	void GoFirstDoodad();
	void SetDoodad(CDoodad doodad);
	CDoodad GetDoodad(int ID);
	int GetNbDoodads();
	void DelDoodads(CRect area);
	void DelDoodad(CDoodad doodad);
	void AddDoodad(CDoodad &doodad);
	void ClearList();
	CDoodadsList();
	virtual ~CDoodadsList();

protected:
	void RemoveFromRadiusList(PCListedDoodad pRadLDoodad);
	void UnplaceDoodad(PCListedDoodad pUpLDoodad);
	void PlaceDoodad(PCListedDoodad pLDoodad);
	CIDList m_FreeIDList;
	PCListedDoodad *m_pDoodadsMap;
	PCListedDoodad m_pMapCurrent;
	PCListedDoodad current;
	PCListedDoodad last;
	PCListedDoodad head;
	float m_fMapWidth;
	float m_fMapHeight;
	float m_fMapOrgX;
	float m_fMapOrgY;
	int m_iDoodadsMapSize;
	int m_iNbDoodads;
	int m_iHighestID;



};

#endif // !defined(AFX_DOODADSLIST_H__6B9367FE_16EF_4498_9827_6FD321F8836D__INCLUDED_)
