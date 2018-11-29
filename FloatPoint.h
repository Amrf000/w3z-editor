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
// FloatPoint.h: interface for the CFloatPoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLOATPOINT_H__0D1C9E5E_666C_47CF_B957_D96E0CB29642__INCLUDED_)
#define AFX_FLOATPOINT_H__0D1C9E5E_666C_47CF_B957_D96E0CB29642__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFloatPoint  
{
public:
	CFloatPoint operator - (CFloatPoint fpoint) const;
	CFloatPoint operator + (CFloatPoint fpoint) const;
	void operator -= (CFloatPoint fpoint);
	void operator += (CFloatPoint fpoint);
	BOOL operator != (CFloatPoint fpoint) const;
	BOOL operator == (CFloatPoint fpoint) const;
	void Offset(float xOffset, float yOffset);
	float y;
	float x;
	CFloatPoint(float fx, float fy);
	CFloatPoint();
	virtual ~CFloatPoint();

};

#endif // !defined(AFX_FLOATPOINT_H__0D1C9E5E_666C_47CF_B957_D96E0CB29642__INCLUDED_)
