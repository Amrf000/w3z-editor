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
// MiniMap.h: interface for the CMiniMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MINIMAP_H__09A03A60_11C7_4CF9_BEEA_3B265F9FD025__INCLUDED_)
#define AFX_MINIMAP_H__09A03A60_11C7_4CF9_BEEA_3B265F9FD025__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "MMPIcon.h"

class CMiniMap  
{
public:
	CMMPIcon GetClosestIcon(int v_iX, int v_iY);
	void DelIcon(CMMPIcon mmpIcon);
	void ClearList();
	CMMPIcon GetNext();
	void GoFirstIcon();
	int GetIconsCount();
	void AddIcon(CMMPIcon mmpIcon);
	CByteArray m_baUnsupportedBuffer;
	CMiniMap();
	virtual ~CMiniMap();

protected:
	POSITION m_tposCurrent;
	CList<CMMPIcon, CMMPIcon&> m_tlistIconList; // list of icons
	int m_iMMPVersion; // MMP format version

};

#endif // !defined(AFX_MINIMAP_H__09A03A60_11C7_4CF9_BEEA_3B265F9FD025__INCLUDED_)
