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
// War3File.h: interface for the CWar3File class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAR3FILE_H__C7A53EFC_83E4_4EF6_9772_07BD7933CB08__INCLUDED_)
#define AFX_WAR3FILE_H__C7A53EFC_83E4_4EF6_9772_07BD7933CB08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CWar3File : public CFile  
{
public:
	BOOL WriteString(CWar3String szStr);
	BOOL WriteFloat(CWar3Float f);
	BOOL WriteFlags(CWar3Flags flags);
	BOOL WriteInt(CWar3Int i);
	BOOL WriteStrID(CWar3StrID strID);
	BOOL WriteChar(CWar3Char c);
	BOOL ReadStrID(CWar3StrID &strID);
	BOOL ReadFlags(CWar3Flags &flags);
	BOOL ReadChar(CWar3Char &c);
	BOOL ReadFloat(CWar3Float &f);
	BOOL ReadString(CWar3String &szStr);
	BOOL ReadInt(CWar3Int &i);
	CWar3File();
	virtual ~CWar3File();
};


#endif // !defined(AFX_WAR3FILE_H__C7A53EFC_83E4_4EF6_9772_07BD7933CB08__INCLUDED_)
