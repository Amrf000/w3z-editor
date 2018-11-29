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
// ShadowMap.h: interface for the CShadowMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHADOWMAP_H__0FDBF045_E6F7_4149_9972_5245C45EBC0C__INCLUDED_)
#define AFX_SHADOWMAP_H__0FDBF045_E6F7_4149_9972_5245C45EBC0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CShadowMap  
{
public:
	byte* GetBytemapPtr();
	int GetHeight();
	int GetWidth();
	void RemoveShadowAt(CRect rect);
	void RemoveShadowAt(int x, int y);
	void SetShadowAt(CRect rect);
	void SetShadowAt(int x, int y);
	void SetSize(int width, int height);
	CShadowMap();
	virtual ~CShadowMap();

protected:
	byte* m_pByteMap;
	int m_Height;
	int m_Width;
};

#endif // !defined(AFX_SHADOWMAP_H__0FDBF045_E6F7_4149_9972_5245C45EBC0C__INCLUDED_)
