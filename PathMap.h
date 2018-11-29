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
// PathMap.h: interface for the CPathMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PATHMAP_H__CD856562_6D70_4611_9461_5A8158364ABA__INCLUDED_)
#define AFX_PATHMAP_H__CD856562_6D70_4611_9461_5A8158364ABA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
CPathMap
********
Class used to store Warcraft 3 WPM file data.

*/
class CPathMap
{
public:
	CByteArray m_abyUnsupportedBuffer; //unsupported data buffer
	byte* GetData();
	int GetLength();
	int GetHeight();
	int GetWidth();
	BOOL SetSize(int width, int height);
	CPathMap();
	virtual ~CPathMap();

protected:
	byte* m_pbyPathMap;
	int m_iHeight;
	int m_iWidth;

};

#endif // !defined(AFX_PATHMAP_H__CD856562_6D70_4611_9461_5A8158364ABA__INCLUDED_)
