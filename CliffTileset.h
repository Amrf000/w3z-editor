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
// CliffTileset.h: interface for the CCliffTileset class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIFFTILESET_H__297FC291_9524_4B6D_8621_AA5A11AFB350__INCLUDED_)
#define AFX_CLIFFTILESET_H__297FC291_9524_4B6D_8621_AA5A11AFB350__INCLUDED_

#include "War3TypesAndConstants.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCliffTileset  
{
public:
	CString cliffName;
	CWar3StrID associatedTileID;
	CWar3StrID CliffID;
	CCliffTileset();
	virtual ~CCliffTileset();

};

#endif // !defined(AFX_CLIFFTILESET_H__297FC291_9524_4B6D_8621_AA5A11AFB350__INCLUDED_)
