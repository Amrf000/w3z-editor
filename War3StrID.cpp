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
// War3StrID.cpp: implementation of the CWar3StrID class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "War3TypesAndConstants.h"
#include "War3StrID.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWar3StrID::CWar3StrID()
{
	int i;
	for (i = 0; i <= WAR3STRID_MAXCHAR; i++)
	{
		strID[i] = '\0';
	}
}

CWar3StrID::~CWar3StrID()
{

}


char* CWar3StrID::GetStrID()
{
	return strID;
}


void CWar3StrID::SetStrID(const char acNewID[])
{
	int i;
	for (i = 0; i < WAR3STRID_MAXCHAR; i++)
	{
		strID[i] = acNewID[i];
	}
	strID[WAR3STRID_MAXCHAR] = '\0';
}


BOOL CWar3StrID::operator ==(const CWar3StrID &otherStrID) const
{
	BOOL ret = TRUE;
	int i = 0;
	while ((i < WAR3STRID_MAXCHAR) && (TRUE == ret))
	{
		if (strID[i] != otherStrID.strID[i])
		{
			ret = FALSE;
		}
		i++;
	}
	return ret;
}


BOOL CWar3StrID::operator ==(const char acStrID[]) const
{
	BOOL ret = TRUE;
	int i = 0;
	while ((i < WAR3STRID_MAXCHAR) && (TRUE == ret))
	{
		if (strID[i] != acStrID[i])
		{
			ret = FALSE;
		}
		i++;
	}
	return ret;
}


BOOL CWar3StrID::operator !=(const CWar3StrID &otherStrID) const
{
	BOOL ret = FALSE;
	int i = 0;
	while ((i < WAR3STRID_MAXCHAR) && (FALSE == ret))
	{
		if (strID[i] != otherStrID.strID[i])
		{
			ret = TRUE;
		}
		i++;
	}
	return ret;
}


BOOL CWar3StrID::operator !=(const char acStrID[]) const
{
	BOOL ret = FALSE;
	int i = 0;
	while ((i < WAR3STRID_MAXCHAR) && (FALSE == ret))
	{
		if (strID[i] != acStrID[i])
		{
			ret = TRUE;
		}
		i++;
	}
	return ret;
}


void CWar3StrID::operator =(const char acStrID[])
{
	SetStrID(acStrID);
}


char CWar3StrID::operator [](int nIndex) const
{
	if ((nIndex < WAR3STRID_MAXCHAR) && (nIndex >= 0))
	{
		return strID[nIndex];
	}
	else
	{
		return '\0';
	}
}

CWar3StrID::CWar3StrID(const char acStrID[])
{
	SetStrID(acStrID);
}
