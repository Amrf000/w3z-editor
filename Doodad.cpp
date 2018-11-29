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
// Doodad.cpp: implementation of the CDoodad class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "War3TypesAndConstants.h"
#include "Doodad.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDoodad::CDoodad()
{
	ID = -1;
	life = 100;
	flags = DOOD_FL_NORMAL;
	scaleZ = 1;
	scaleY = 1;
	scaleX = 1;
	angle = DOOD_DEFAULT_ANGLE;
	z = 0;
	y = 0;
	x = 0;
	variation = DOOD_DEFAULT_VARIATION;
	type = DOOD_DEFAULT_TREE;
	selected = FALSE;
}

CDoodad::~CDoodad()
{

}

float CDoodad::GetAngleValue()
{
	return angle * DOOD_ANGLE_CONST;
}

void CDoodad::SetAngleValue(float newangle)
{
	angle = newangle / DOOD_ANGLE_CONST;
}
