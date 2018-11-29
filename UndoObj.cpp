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
// UndoObj.cpp: implementation of the CUndoObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "W3ZMapEdit.h"
#include "W3ZMapEditDoc.h"
#include "UndoObj.h"
#include "TilePoint.h"
#include "Doodad.h"
#include "ListedTilePoint.h"

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
CUndoObj::CListedDoodad::CListedDoodad()
{
	next		= NULL;
	mapnext		= NULL;
	radiusnext	= NULL;
}

CUndoObj::CListedDoodad::~CListedDoodad()
{

}
/********************************************************************************/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUndoObj::CUndoObj()
{
	m_sUndoMsg = "";
	m_pDoodadsAddList = NULL;
	m_pDoodadsDelList = NULL;
	m_pDoodadsUpdList = NULL;
	m_pTilePointsList = NULL;
	m_pPathList = NULL;
}

CUndoObj::~CUndoObj()
{
	CListedTilePoint* pLTilePoint;
	CListedDoodad* pLDoodad;
	CListedInt *pLInt;

	while (m_pTilePointsList)
	{
		pLTilePoint = m_pTilePointsList;
		m_pTilePointsList = m_pTilePointsList->next;
		delete pLTilePoint;
	}
	while(m_pDoodadsAddList)
	{
		pLDoodad = m_pDoodadsAddList;
		m_pDoodadsAddList = m_pDoodadsAddList->next;
		delete pLDoodad;
	}
	while(m_pDoodadsDelList)
	{
		pLDoodad = m_pDoodadsDelList;
		m_pDoodadsDelList = m_pDoodadsDelList->next;
		delete pLDoodad ;
	}
	while(m_pDoodadsUpdList)
	{
		pLDoodad = m_pDoodadsUpdList;
		m_pDoodadsUpdList = m_pDoodadsUpdList->next;
		delete pLDoodad ;
	}
	while (m_pPathList)
	{
		pLInt = m_pPathList;
		m_pPathList = m_pPathList->next;
		delete pLInt;
	}
}

void CUndoObj::AddTilePoint(CTilePoint tilepoint, int x, int y)
{
	CListedTilePoint* pLTilePoint;

	pLTilePoint = m_pTilePointsList;
/*	while ((pLTilePoint) && (pLTilePoint->x != x || pLTilePoint->y != y))
	{
		pLTilePoint = pLTilePoint->next;
	}
*/
	// n'ajoute que si le tilepoint a (x,y) n'est pas deja present
//	if (!pLTilePoint)
//	{
		pLTilePoint = new CListedTilePoint;
		pLTilePoint->x = x;
		pLTilePoint->y = y;
		pLTilePoint->tilepoint = tilepoint;
		pLTilePoint->tilepoint.viewcolor.rgbBlue = 0;
		pLTilePoint->tilepoint.viewcolor.rgbGreen = 0;
		pLTilePoint->tilepoint.viewcolor.rgbRed = 0;
		pLTilePoint->tilepoint.viewcolor.rgbReserved = 0;
		pLTilePoint->tilepoint.selected = false;
		pLTilePoint->next = m_pTilePointsList;
		m_pTilePointsList = pLTilePoint;
//	}
}

void CUndoObj::AddAddDoodad(CDoodad doodad)
{
	CListedDoodad* pLDoodad;
	pLDoodad = new CListedDoodad;
	pLDoodad->doodad = doodad;
	pLDoodad->doodad.selected = false;
	pLDoodad->next = m_pDoodadsAddList;
	m_pDoodadsAddList = pLDoodad;
}

void CUndoObj::AddDelDoodad(CDoodad doodad)
{
	CListedDoodad* pLDoodad;
	pLDoodad = new CListedDoodad;
	pLDoodad->doodad = doodad;
	pLDoodad->doodad.selected = false;
	pLDoodad->next = m_pDoodadsDelList;
	m_pDoodadsDelList = pLDoodad;
}

void CUndoObj::AddUpdDoodad(CDoodad doodad)
{
	CListedDoodad* pLDoodad;
	pLDoodad = new CListedDoodad;
	pLDoodad->doodad = doodad;
	pLDoodad->doodad.selected = false;
	pLDoodad->next = m_pDoodadsUpdList;
	m_pDoodadsUpdList = pLDoodad;
}

void CUndoObj::DoUndo(CW3ZMapEditDoc* pDoc)
{
	int i;
	CListedTilePoint* pLTilePoint;
	CListedDoodad* pLDoodad;
	CListedInt *pLInt;
//	byte* pBitmap;
	byte* pBytemap;

	while (m_pTilePointsList)
	{
		pLTilePoint = m_pTilePointsList;
		m_pTilePointsList = m_pTilePointsList->next;
		pLTilePoint->tilepoint.selected = pDoc->m_War3Map.GetTilePoint(pLTilePoint->x, pLTilePoint->y).selected;
		pLTilePoint->tilepoint.viewcolor = pDoc->m_War3Map.GetTilePoint(pLTilePoint->x, pLTilePoint->y).viewcolor;
		pDoc->m_War3Map.SetTilePoint(pLTilePoint->x, pLTilePoint->y, pLTilePoint->tilepoint);
		delete pLTilePoint;
	}
	while(m_pDoodadsAddList)
	{// doodad(s) adde(s) a enlever
		pLDoodad = m_pDoodadsAddList;
		m_pDoodadsAddList = m_pDoodadsAddList->next;
		pDoc->m_War3Map.DelDoodad(pLDoodad->doodad);
		delete pLDoodad;
	}
	while(m_pDoodadsDelList)
	{// doodad(s) enleve(s) a remettre
		pLDoodad = m_pDoodadsDelList;
		m_pDoodadsDelList = m_pDoodadsDelList->next;
		pDoc->m_War3Map.AddDoodad(pLDoodad->doodad);
		delete pLDoodad;
	}
	while(m_pDoodadsUpdList)
	{// remet a jour tous doodads modifies
		pLDoodad = m_pDoodadsUpdList;
		m_pDoodadsUpdList = m_pDoodadsUpdList->next;
		pDoc->m_War3Map.SetDoodad(pLDoodad->doodad);
		delete pLDoodad;
	}
	while (m_pPathList)
	{
//		pBitmap = pDoc->m_War3Map.mapTGAPath.GetBitmapPtr();
		pBytemap = pDoc->m_War3Map.GetPathMapPtr();
		pLInt = m_pPathList;
		m_pPathList = m_pPathList->next;
//		i = (pLInt->y*pDoc->m_War3Map.mapTGAPath.GetWidth() + pLInt->x) * 4;
//		pBitmap[i] = (pLInt->i & 0xFF0000)/0x010000; // blue
//		pBitmap[i+1] = (pLInt->i & 0x00FF00)/0x0100; // green
//		pBitmap[i+2] = (pLInt->i & 0x0000FF); // red
		i = ((pDoc->m_War3Map.GetPathMapHeight() - pLInt->y - 1)*pDoc->m_War3Map.GetPathMapWidth() + pLInt->x);
		pBytemap[i] = (byte)(pLInt->i);
		delete pLInt;
	}
}

void CUndoObj::AddPath(int oldpath, int x, int y)
{
	CListedInt *pLInt;

	pLInt = m_pPathList;
	pLInt = new CListedInt;
	pLInt->i = oldpath;
	pLInt->x = x;
	pLInt->y = y;
	pLInt->next = m_pPathList;
	m_pPathList = pLInt;
}

bool CUndoObj::IsEmpty()
{
	bool ret = true;
	if (m_pTilePointsList
		|| m_pDoodadsAddList
		|| m_pDoodadsDelList
		|| m_pDoodadsUpdList
		|| m_pPathList)
	{
		ret = false;
	}

	return ret;
}
