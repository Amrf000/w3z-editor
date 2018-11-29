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
// MMPIcon.h: interface for the CMMPIcon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MMPICON_H__B416FA63_3D85_4AC4_8B1A_673F7C35009D__INCLUDED_)
#define AFX_MMPICON_H__B416FA63_3D85_4AC4_8B1A_673F7C35009D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMMPIcon  
{
public:
	int m_iIconType;
	int m_iX;
	int m_iY;
	COLORREF m_Color;
	BOOL operator ==(const CMMPIcon other) const;
	CMMPIcon();
	virtual ~CMMPIcon();

};

#endif // !defined(AFX_MMPICON_H__B416FA63_3D85_4AC4_8B1A_673F7C35009D__INCLUDED_)
