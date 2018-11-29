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
// FloatPoint.cpp: implementation of the CFloatPoint class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "w3zmapedit.h"
#include "FloatPoint.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFloatPoint::CFloatPoint()
{
	x = 0;
	y = 0;
}

CFloatPoint::CFloatPoint(float fx, float fy)
{
	x = fx;
	y = fy;
}

CFloatPoint::~CFloatPoint()
{

}

void CFloatPoint::Offset(float xOffset, float yOffset)
{
	x += xOffset;
	y += yOffset;
}

BOOL CFloatPoint::operator ==(CFloatPoint fpoint) const
{
	BOOL bRet = FALSE;
	if ((x == fpoint.x) && (y == fpoint.y))
	{
		bRet = TRUE;
	}
	return bRet;
}

BOOL CFloatPoint::operator !=(CFloatPoint fpoint) const
{
	BOOL bRet = TRUE;
	if ((x == fpoint.x) && (y == fpoint.y))
	{
		bRet = FALSE;
	}
	return bRet;
}

void CFloatPoint::operator +=(CFloatPoint fpoint)
{
	x += fpoint.x;
	y += fpoint.y;
}

void CFloatPoint::operator -=(CFloatPoint fpoint)
{
	x -= fpoint.x;
	y -= fpoint.y;
}


CFloatPoint CFloatPoint::operator + (CFloatPoint fpoint) const
{
	CFloatPoint fpointRet;
	fpointRet.x = x + fpoint.x;
	fpointRet.y = y + fpoint.y;
	return fpointRet;
}

CFloatPoint CFloatPoint::operator - (CFloatPoint fpoint) const
{
	CFloatPoint fpointRet;
	fpointRet.x = x - fpoint.x;
	fpointRet.y = y - fpoint.y;
	return fpointRet;
}

