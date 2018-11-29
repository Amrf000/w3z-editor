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
// UndoObj.h: interface for the CUndoObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UNDOOBJ_H__3E4EFF8B_B6F2_4A9F_8F56_6EE0D9E0C2A9__INCLUDED_)
#define AFX_UNDOOBJ_H__3E4EFF8B_B6F2_4A9F_8F56_6EE0D9E0C2A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CW3ZMapEditDoc;
class CListedDoodad;
class CListedTilePoint;
class CTilePoint;
class CDoodad;

class CUndoObj  
{
	class CListedInt
	{
		public:
			int i;
			int x;
			int y;
			CListedInt* next;
	};

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
	bool IsEmpty();
	void AddPath(int oldpath, int x, int y);
	CString m_sUndoMsg;
	void DoUndo(CW3ZMapEditDoc *pDoc);
	void AddAddDoodad(CDoodad doodad);
	void AddDelDoodad(CDoodad doodad);
	void AddUpdDoodad(CDoodad doodad);
	void AddTilePoint(CTilePoint tilepoint, int x, int y);
	CUndoObj();
	virtual ~CUndoObj();

protected:
	CListedInt*	m_pPathList;
	CListedDoodad* m_pDoodadsUpdList;
	CListedDoodad* m_pDoodadsDelList;
	CListedDoodad* m_pDoodadsAddList;
	CListedTilePoint* m_pTilePointsList;
};

#endif // !defined(AFX_UNDOOBJ_H__3E4EFF8B_B6F2_4A9F_8F56_6EE0D9E0C2A9__INCLUDED_)
