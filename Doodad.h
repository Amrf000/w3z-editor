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
// Doodad.h: interface for the CDoodad class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOODAD_H__B1488A97_D9C8_43EF_8A59_C6DFC3DEE4F2__INCLUDED_)
#define AFX_DOODAD_H__B1488A97_D9C8_43EF_8A59_C6DFC3DEE4F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDoodad  
{
public:
	void SetAngleValue(float newangle);
	float GetAngleValue();
	bool selected;
	int ID;
	byte life;
	byte flags;
	float scaleZ;
	float scaleY;
	float scaleX;
	float angle;
	float z;
	float y;
	float x;
	int variation;
	CWar3StrID type;
	CDoodad();
	virtual ~CDoodad();

};

#endif // !defined(AFX_DOODAD_H__B1488A97_D9C8_43EF_8A59_C6DFC3DEE4F2__INCLUDED_)
