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
// ListedUndoObj.h: interface for the CListedUndoObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LISTEDUNDOOBJ_H__40132ED9_700D_4853_B6B3_B5EC26E187B5__INCLUDED_)
#define AFX_LISTEDUNDOOBJ_H__40132ED9_700D_4853_B6B3_B5EC26E187B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UndoObj.h"

class CListedUndoObj  
{
public:
	CListedUndoObj* next;
	CListedUndoObj* prev;
	CUndoObj m_UndoObj;
	CListedUndoObj();
	virtual ~CListedUndoObj();

};

#endif // !defined(AFX_LISTEDUNDOOBJ_H__40132ED9_700D_4853_B6B3_B5EC26E187B5__INCLUDED_)
