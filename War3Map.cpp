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
// War3Map.cpp: implementation of the CWar3Map class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "War3TypesAndConstants.h"
#include "War3Map.h"
#include "W3ZMapEdit.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWar3Map::CWar3Map()
{
	mapTilePointSelState = 0;
	mapGroundTiles = W3E_TILESET_DEFAULT;
	mapCliffTiles = W3E_CLIFFSET_DEFAULT;
	mapMainTileset = W3E_MAINTILE_DEFAULT;
	mapWidth = 0;
	mapHeight = 0;
	mapCustomTilesets = 0;
	SelectedTilePointsOnly = FALSE;

	CW3ZMapEditApp(* pTheApp) = (CW3ZMapEditApp *)AfxGetApp();
	m_pAppSettings = &(pTheApp->m_AppSettings);
}

CWar3Map::~CWar3Map()
{
}

/*
SetMapSize
**********
Sets map size and initialize all the required structures (ground, doodads, pathing, shadows,...)
(even value like 64x64)

In:
  int v_iWidth: new width (must be > W3E_MIN_WIDTH and < W3E_MAX_WIDTH)
  int v_iHeight: new height (must be > W3E_MIN_HEIGHT and < W3E_MAX_HEIGHT)

Out:
 nothing

Return:
 BOOL: TRUE if successfull, FALSE otherwise

Note:
 If width and height values specified are outside the range, values are truncated to feet the range.

*/
BOOL CWar3Map::SetMapSize(int v_iWidth, int v_iHeight)
{
	int iWidth, iHeight;
	BOOL bRet = TRUE;
	
	iWidth = v_iWidth;
	iHeight = v_iHeight;

	if (iWidth < W3E_MIN_WIDTH)
	{
		iWidth = W3E_MIN_WIDTH;
	}
	else if(iWidth > W3E_MAX_WIDTH)
	{
		iWidth = W3E_MAX_WIDTH;
	}

	if (iHeight < W3E_MIN_HEIGHT)
	{
		iHeight = W3E_MIN_HEIGHT;
	}
	else if(iHeight > W3E_MAX_HEIGHT)
	{
		iHeight = W3E_MAX_HEIGHT;
	}

	if ((iWidth != mapWidth) || (iHeight != mapHeight))
	{// only if size changes
		CWaitCursor wait;
		mapTilePointsTable.SetSize(iWidth+1, iHeight+1); // cree les tilepoints
		mapDoodadsList.SetSize((float)iWidth*DOOD_MAP_SUBDIV, (float)iHeight*DOOD_MAP_SUBDIV);
		mapShadowMap.SetSize(iWidth*SHAD_MAP_SUBDIV, iHeight*SHAD_MAP_SUBDIV);
		mapPathMap.SetSize(iWidth*PATH_MAP_SUBDIV, iHeight*PATH_MAP_SUBDIV);
		mapTGAPath.SetSize(iWidth*PATH_MAP_SUBDIV, iHeight*PATH_MAP_SUBDIV);
		mapWidth = iWidth;
		mapHeight = iHeight;
	}
	//+FIXME: handle errors (bRet)
	return bRet;
}

/*
GetTilePoint
************
Returns the tilepoint at the specified coordinate

In:
  int x: x coordinate
  int y: y coordinate

Out:
 none

Retrun:
 CTilePoint: a copy of the tilepoint

*/
CTilePoint CWar3Map::GetTilePoint(int x, int y)
{
	int i, x0, y0;
	if (x < 0)
	{
		x0 = 0;
	}
	else if (x >= mapTilePointsTable.GetWidth())
	{
		x0 = mapTilePointsTable.GetWidth() - 1;
	}
	else
	{
		x0 = x;
	}
	if (y < 0)
	{
		y0 = 0;
	}
	else if (y >= mapTilePointsTable.GetHeight())
	{
		y0 = mapTilePointsTable.GetHeight() - 1;
	}
	else
	{
		y0 = y;
	}

	i = mapTilePointsTable.GetWidth() * y0 + x0;
	return mapTilePointsTable.GetTilePoint(i);
}


int CWar3Map::GetMapWidth()
{
	return mapWidth;
}

int CWar3Map::GetMapHeight()
{
	return mapHeight;
}

void CWar3Map::SetTilePoint(int num, CTilePoint tilepoint)
{
	mapTilePointsTable.SetTilePoint(num, tilepoint);
}

CTilePoint CWar3Map::GetTilePoint(int num)
{
	return mapTilePointsTable.GetTilePoint(num);
}


void CWar3Map::SetTilePoint(int x, int y, CTilePoint tilepoint)
{
	int i;
	if ((x >= 0) && (y >= 0) && (x < mapTilePointsTable.GetWidth()) && (y < mapTilePointsTable.GetHeight()))
	{
		i = mapTilePointsTable.GetWidth() * y + x;
		mapTilePointsTable.SetTilePoint(i, tilepoint);
	}
}

void CWar3Map::ReplaceGroundType(int tileold, int tilenew, bool selectedonly)
{
	mapTilePointsTable.ReplaceGroundType(tileold, tilenew, selectedonly);
}


void CWar3Map::AddDoodad(CDoodad &doodad)
{
	mapDoodadsList.AddDoodad(doodad);
}

int CWar3Map::GetNbDoodads()
{
	return mapDoodadsList.GetNbDoodads();
}

void CWar3Map::GoFirstDoodad()
{
	mapDoodadsList.GoFirstDoodad();
}

CDoodad CWar3Map::GetNextDoodad()
{
	return mapDoodadsList.GetNext();
}


CDoodad CWar3Map::GetClosestDoodad(float x, float y)
{
	return mapDoodadsList.GetClosestDoodad(x,y);
}

CDoodad CWar3Map::GetDoodad(int ID)
{
	return mapDoodadsList.GetDoodad(ID);
}

void CWar3Map::SetDoodad(CDoodad doodad)
{
	mapDoodadsList.SetDoodad(doodad);
}

void CWar3Map::DelDoodad(CDoodad doodad)
{
	mapDoodadsList.DelDoodad(doodad);
}

float CWar3Map::GetDistance(float x1, float y1, float x2, float y2)
{
	float distance;
	float deltaX, deltaY;
	deltaX = x1 - x2;
	deltaY = y1 - y2;
	distance = (float)sqrt(deltaX*deltaX + deltaY*deltaY);
	return distance;
}


void CWar3Map::UnselectAllDoodads()
{
	mapDoodadsList.UnselectAll();
}

void CWar3Map::UnselectAllTilePoints()
{
	mapTilePointsTable.UnselectAll();
}

void CWar3Map::SelectTilePoints(CPoint pt1, CPoint pt2, CPoint pt3, CPoint pt4)
{
	int x, y, xmin, xmax, ymin, ymax;
	bool isselected;
	CTilePoint tilepoint;

	xmin = pt1.x;
	xmax = pt1.x;
	if (pt2.x < xmin)
	{xmin = pt2.x;}
	else if (pt2.x > xmax)
	{xmax = pt2.x;}
	if (pt3.x < xmin)
	{xmin = pt3.x;}
	else if (pt3.x > xmax)
	{xmax = pt3.x;}
	if (pt4.x < xmin)
	{xmin = pt4.x;}
	else if (pt4.x > xmax)
	{xmax = pt4.x;}

	ymin = pt1.y;
	ymax = pt1.y;
	if (pt2.y < ymin)
	{ymin = pt2.y;}
	else if (pt2.y > ymax)
	{ymax = pt2.y;}
	if (pt3.y < ymin)
	{ymin = pt3.y;}
	else if (pt3.y > ymax)
	{ymax = pt3.y;}
	if (pt4.y < ymin)
	{ymin = pt4.y;}
	else if (pt4.y > ymax)
	{ymax = pt4.y;}

	for (y = ymin; y <= ymax; y++)
	{
		for (x = xmin; x <= xmax; x++)
		{
			tilepoint = GetTilePoint(x, mapHeight - y);
			if (!tilepoint.selected)
			{
				tilepoint.selected = true;
				SetTilePoint(x, mapHeight - y, tilepoint);
			}
		}
	}
	SelectTilePoints(pt1, pt2, RGB_GREEN);
	SelectTilePoints(pt2, pt3, RGB_GREEN);
	SelectTilePoints(pt3, pt4, RGB_GREEN);
	SelectTilePoints(pt4, pt1, RGB_GREEN);

	// grignotte les cotes...
	// cote gauche
	for (y = ymin; y <= ymax; y++)
	{
		isselected = false;
		x = xmin;
		do
		{
			tilepoint = GetTilePoint(x, mapHeight - y);
			if (tilepoint.viewcolor.rgbGreen)
			{
//				tilepoint.viewcolor.rgbGreen = 0;
//				SetTilePoint(x, mapHeight - y, tilepoint);
				isselected = true;
			}
			else
			{
				tilepoint.selected = false;	
				SetTilePoint(x, mapHeight - y, tilepoint);
			}
			x++;
		}
		while (!isselected && (x < xmax));
	}
	// cote droit
	for (y = ymin; y <= ymax; y++)
	{
		isselected = false;
		x = xmax;
		do
		{
			tilepoint = GetTilePoint(x, mapHeight - y);
			if (tilepoint.viewcolor.rgbGreen)
			{
//				tilepoint.viewcolor.rgbGreen = 0;
//				SetTilePoint(x, mapHeight - y, tilepoint);
				isselected = true;
			}
			else
			{
				tilepoint.selected = false;	
				SetTilePoint(x, mapHeight - y, tilepoint);
			}
			x--;
		}
		while (!isselected && (x > xmin));
	}
	//dessus
	for (x = xmin; x <= xmax; x++)
	{
		isselected = false;
		y = ymin;
		do
		{
			tilepoint = GetTilePoint(x, mapHeight - y);
			if (tilepoint.viewcolor.rgbGreen)
			{
//				tilepoint.viewcolor.rgbGreen = 0;
//				SetTilePoint(x, mapHeight - y, tilepoint);
				isselected = true;
			}
			else
			{
				tilepoint.selected = false;	
				SetTilePoint(x, mapHeight - y, tilepoint);
			}
			y++;
		}
		while (!isselected && (y < ymax));
	}
	//dessous
	for (x = xmin; x <= xmax; x++)
	{
		isselected = false;
		y = ymax;
		do
		{
			tilepoint = GetTilePoint(x, mapHeight - y);
			if (tilepoint.viewcolor.rgbGreen)
			{
//				tilepoint.viewcolor.rgbGreen = 0;
//				SetTilePoint(x, mapHeight - y, tilepoint);
				isselected = true;
			}
			else
			{
				tilepoint.selected = false;	
				SetTilePoint(x, mapHeight - y, tilepoint);
			}
			y--;
		}
		while (!isselected && (y > ymin));
	}

	SelectTilePoints(pt1, pt2, RGB_SRED);
	SelectTilePoints(pt3, pt4, RGB_SBLUE);
	
/*	for (y = ymin; y <= ymax; y++)
	{
		isselected = false;
		x = xmin;
		while ()
		/*
		for (x = xmin; x <= xmax; x++)
		{
			tilepoint = GetTilePoint(x, mapHeight - y);
			if (isselected)
			{
				if (tilepoint.selected)
				{isselected = false;}
				else if(x == xmax)
				{ // end of line and no other selected, remove previous selection
					i = xmax - 1;
					tilepoint = GetTilePoint(i, mapHeight - y);
					while(tilepoint.selected)
					{
						tilepoint.selected = false;
						SetTilePoint(i, mapHeight - y, tilepoint);
						i--;
						tilepoint = GetTilePoint(i, mapHeight - y);
					}
					// reselect the first one
					i++;
					tilepoint = GetTilePoint(i, mapHeight - y);
					tilepoint.selected = true;
					SetTilePoint(i, mapHeight - y, tilepoint);
				}
				else
				{// select the tilepoint
					tilepoint.selected = true;
					SetTilePoint(x, mapHeight - y, tilepoint);
				}
			}
			else
			{// not isselect
				if (tilepoint.selected)
				{isselected = true;}
			}
		}
		*/
//	}


}

void CWar3Map::SelectTilePoints(CPoint pt1, CPoint pt2, RGBQUAD color)
{
	int i, j, xdelta, ydelta;
	double m, p;
	CTilePoint tilepoint;

	//y = mx + p
	xdelta = pt1.x - pt2.x;
	ydelta = pt1.y - pt2.y;
	if ((!xdelta) && (!ydelta))
	{ // 1 seul point de selection
	}
	if (!xdelta)
	{ // vertical
		i = pt1.x;
		if (pt1.y < pt2.y)
		{
			for (j = pt1.y; j <= pt2.y; j++)
			{
				tilepoint = GetTilePoint(i, mapHeight - j);
				tilepoint.selected = true;
				tilepoint.viewcolor = color;
				SetTilePoint(i, mapHeight - j, tilepoint);
			}
		}
		else
		{
			for (j = pt2.y; j <= pt1.y; j++)
			{
				tilepoint = GetTilePoint(i, mapHeight - j);
				tilepoint.selected = true;
				tilepoint.viewcolor = color;
				SetTilePoint(i, mapHeight - j, tilepoint);
			}
		}
	}
	else if (!ydelta)
	{ // horizontal
		j = pt1.y;
		if (pt1.x < pt2.x)
		{
			for (i = pt1.x; i <= pt2.x; i++)
			{
				tilepoint = GetTilePoint(i, mapHeight - j);
				tilepoint.selected = true;
				tilepoint.viewcolor = color;
				SetTilePoint(i, mapHeight - j, tilepoint);
			}
		}
		else
		{
			for (i = pt2.x; i <= pt1.x; i++)
			{
				tilepoint = GetTilePoint(i, mapHeight - j);
				tilepoint.selected = true;
				tilepoint.viewcolor = color;
				SetTilePoint(i, mapHeight - j, tilepoint);
			}
		}
	}
	else
	{// diagonale
		m = (double)ydelta/(double)xdelta;
		p = (double)pt1.y - m*pt1.x;
		if (xdelta < 0)
		{xdelta *= -1;}
		if (ydelta < 0)
		{ydelta *= -1;}

		if (xdelta >= ydelta)
		{//suivant les x
			if (pt1.x < pt2.x)
			{
				for (i = pt1.x; i <= pt2.x; i++)
				{
					j = (int)(m*i+p + 0.5);
					tilepoint = GetTilePoint(i,mapHeight - j);
					tilepoint.selected = true;
					tilepoint.viewcolor = color;
					SetTilePoint(i, mapHeight - j, tilepoint);
				}
			}
			else
			{
				for (i = pt2.x; i <= pt1.x; i++)
				{
					j = (int)(m*i+p + 0.5);
					tilepoint = GetTilePoint(i, mapHeight - j);
					tilepoint.selected = true;
					tilepoint.viewcolor = color;
					SetTilePoint(i, GetMapHeight() - j, tilepoint);
				}
			}
		}
		else
		{// suivant les y
			if (pt1.y < pt2.y)
			{
				for (j = pt1.y; j <= pt2.y; j++)
				{
					i = (int)((j-p)/m + 0.5);
					tilepoint = GetTilePoint(i, mapHeight - j);
					tilepoint.selected = true;
					tilepoint.viewcolor = color;
					SetTilePoint(i, mapHeight - j, tilepoint);
				}
			}
			else
			{
				for (j = pt2.y; j <= pt1.y; j++)
				{
					i = (int)((j-p)/m + 0.5);
					tilepoint = GetTilePoint(i, mapHeight - j);
					tilepoint.selected = true;
					tilepoint.viewcolor = color;
					SetTilePoint(i, mapHeight - j, tilepoint);
				}
			}
		}
	}
}

float CWar3Map::GetGroundLevel(float x, float y)
{

	float z;
	CTilePoint tilepoint;
	tilepoint = GetTilePoint((int)((x - mapCenterX)/128 + 0.5), (int)((y - mapCenterY)/128 + 0.5));
	z = (float)tilepoint.GetGroundLevel() / (float)4.0;

	return z;
}


void CWar3Map::SetMapCenter(float fCenterX, float fCenterY)
{
	mapCenterX = fCenterX;
	mapCenterY = fCenterY;
	mapDoodadsList.SetDoodadMapOrigin(fCenterX, fCenterY);
}

float CWar3Map::GetMapCenterX()
{
	return mapCenterX;
}

float CWar3Map::GetMapCenterY()
{
	return mapCenterY;
}

void CWar3Map::GoMapDoodad(float fx, float fy)
{
	mapDoodadsList.GoMapDoodad(fx,fy);
}

CDoodad CWar3Map::GetNextMapDoodad()
{
	return mapDoodadsList.GetMapNext();
}

CDoodad CWar3Map::GetSelectedDoodad()
{
	return	mapSelectedDoodad;

}

void CWar3Map::SetSelectedDoodad(CDoodad doodad)
{
	mapSelectedDoodad = doodad;
}

void CWar3Map::GoFirstUnit()
{
	mapUnitsList.GoFirstUnit();
}

CUnit CWar3Map::GetNextUnit()
{
	return mapUnitsList.GetNext();
}

void CWar3Map::AddUnit(CUnit unit)
{
	mapUnitsList.AddUnit(unit);
}

int CWar3Map::GetNbUnits()
{
	return mapUnitsList.GetNbUnits();
}

void CWar3Map::UnselectDoodad(CDoodad doodad)
{
	mapDoodadsList.UnselectDoodad(doodad);
}


BYTE* CWar3Map::GetW3EUnsupportedBuffer()
{
	return mapTilePointsTable.m_baUnsupportedBuffer.GetData();
}

int CWar3Map::GetW3EUnsupportedBufferSize()
{
	return mapTilePointsTable.m_baUnsupportedBuffer.GetSize();
}


BOOL CWar3Map::SetW3EUnsupportedBufferSize(int v_iSize)
{
	BOOL boSetW3EUnsupportedBufferSize = TRUE;
	TRY
	{
		mapTilePointsTable.m_baUnsupportedBuffer.SetSize(v_iSize);
		boSetW3EUnsupportedBufferSize = TRUE;
	}
	CATCH (CMemoryException, e)
	{//+FIXME
		boSetW3EUnsupportedBufferSize = FALSE;
	}
	END_CATCH
	return boSetW3EUnsupportedBufferSize;
}


BYTE* CWar3Map::GetDOOUnsupportedBuffer()
{
	return mapDoodadsList.m_baUnsupportedBuffer.GetData();
}

int CWar3Map::GetDOOUnsupportedBufferSize()
{
	return mapDoodadsList.m_baUnsupportedBuffer.GetSize();
}


BOOL CWar3Map::SetDOOUnsupportedBufferSize(int v_iSize)
{
	BOOL boSetW3EUnsupportedBufferSize = TRUE;
	TRY
	{
		mapDoodadsList.m_baUnsupportedBuffer.SetSize(v_iSize);
		boSetW3EUnsupportedBufferSize = TRUE;
	}
	CATCH (CMemoryException, e)
	{//+FIXME
		boSetW3EUnsupportedBufferSize = FALSE;
	}
	END_CATCH
	return boSetW3EUnsupportedBufferSize;
}


BYTE* CWar3Map::GetW3IUnsupportedBuffer()
{
	return mapW3Info.m_baUnsupportedBuffer.GetData();
}

int CWar3Map::GetW3IUnsupportedBufferSize()
{
	return mapW3Info.m_baUnsupportedBuffer.GetSize();
}

BOOL CWar3Map::SetW3IUnsupportedBufferSize(int v_iSize)
{
	BOOL boSetW3IUnsupportedBufferSize = TRUE;
	TRY
	{
		mapW3Info.m_baUnsupportedBuffer.SetSize(v_iSize);
		boSetW3IUnsupportedBufferSize = TRUE;
	}
	CATCH (CMemoryException, e)
	{//+FIXME
		boSetW3IUnsupportedBufferSize = FALSE;
	}
	END_CATCH
	return boSetW3IUnsupportedBufferSize;
}


/*
SetPathMapSize
**************
Sets path map (WPM) size.

In:
 int v_iWidth: the new width
 int v_iHeight: the new height

Out:
 nothing

Return:
 BOOL: TRUE if Successfull, FALSE otherwise

*/
BOOL CWar3Map::SetPathMapSize(int v_iWidth, int v_iHeight)
{
	return mapPathMap.SetSize(v_iWidth, v_iHeight);
}

/*
GetPathMapPtr
*************
Returns path map byte map pointer (byte*).

Int:
 nothing

Out:
 nothing

Return:
 byte*: a pointer to the path map.

*/
byte* CWar3Map::GetPathMapPtr()
{
	return mapPathMap.GetData();
}

/*
GetPathMapLength
****************
Returns the length of the path map (array of bytes)

In:
 nothing

Out:
 nothing

Return:
 int: length of the array of byte (path map length), 0 if the path map was not set.

*/
int CWar3Map::GetPathMapLength()
{
	return mapPathMap.GetLength();
}

/*
GetPathMapWidth
****************
Returns the width of the path map.

In:
 nothing

Out:
 nothing

Return:
 int: width of the array of byte (path map length), 0 if the path map was not set.

*/
int CWar3Map::GetPathMapWidth()
{
	return mapPathMap.GetWidth();
}

/*
GetPathMapHeight
****************
Returns the height of the path map.

In:
 nothing

Out:
 nothing

Return:
 int: height of the array of byte (path map length), 0 if the path map was not set.

*/
int CWar3Map::GetPathMapHeight()
{
	return mapPathMap.GetHeight();
}


/*
GetShadowMapWidth
*****************
Returns the width of the shadow map.

In:
 nothing

Out:
 nothing

Return:
 int: width of the array of byte (path map length), 0 if the shadow map was not set.

*/
int CWar3Map::GetShadowMapWidth()
{
	return mapShadowMap.GetWidth();
}

/*
GetShadowMapHeight
******************
Returns the height of the shadow map.

In:
 nothing

Out:
 nothing

Return:
 int: height of the array of byte (path map length), 0 if the shadow map was not set.

*/
int CWar3Map::GetShadowMapHeight()
{
	return mapShadowMap.GetHeight();
}

/*
GetShadowMapPtr
***************
Returns a pointer to the shadow map (array of bytes).

In:
  nothing

Out:
  nothing

Return:
  byte*: pointer to the shadow map (array of bytes), NULL if the shadow map was not set

*/
byte* CWar3Map::GetShadowMapPtr()
{
	return mapShadowMap.GetBytemapPtr();
}

/*
SetShadowMapSize
****************
Set the shadow map size.

In: 
  int iWidth: new width
  int iHeight: new height

Out:
  nothing

Return:
  nothing

*/
void CWar3Map::SetShadowMapSize(int iWidth, int iHeight)
{
	mapShadowMap.SetSize(iWidth, iHeight);
}


/*
PathTGA2WPM
***********
Initialize the WPM (path map) using the PathTGA data.

In:
  nothing

Out:
  nothing

Return:
  BOOL: TRUE if the data convertion has been done successfully, FALSE otherwise

*/
BOOL CWar3Map::PathTGA2WPM()
{
	CWaitCursor wait;
	byte* pBitmap;
	byte* pBytemap;
	int x, y,j ,i;
	int iWidth, iHeight;

	iWidth = mapTGAPath.GetWidth();
	iHeight = mapTGAPath.GetHeight();

	// check if TGA path is ok
	if ((iWidth <= 0) || (iHeight <= 0))
	{//invalid size
		return FALSE;
	}

	pBitmap = mapTGAPath.GetBitmapPtr();
	// allocate memory
	SetPathMapSize(iWidth, iHeight);
	pBytemap = GetPathMapPtr();

	// check if memory has been allocated
	if ((NULL == pBitmap) || (NULL == pBytemap))
	{// allocation failed
		return FALSE;
	}

	// start data convertion
	j = 0;
	for (y = iHeight-1; y >=0 ; y--)
	for (x = 0; x < iWidth; x++)
	{
		i = x + (y*iWidth);
		j += 4; // size of a pixel in the TGA
		// clear path info
		pBytemap[i] = PATH_DEFAULT;
		if (0 != pBitmap[j])
		{ // check for blue = no build
			pBytemap[i] |= PATH_NOBUILD;
		}
		if (0 != pBitmap[j+1])
		{ // check for green = no fly
			pBytemap[i] |= PATH_NOFLY;
		}
		if (0 != pBitmap[j+2])
		{ // check for red = no walk
			pBytemap[i] |= PATH_NOWALK;
		}
		if (0 != pBitmap[j+3])
		{ // check for blight
			pBytemap[i] |= PATH_BLIGHT;
		}
		if (FALSE == GetTilePoint((x+PATH_MAP_SUBDIV/2)/PATH_MAP_SUBDIV, (y+PATH_MAP_SUBDIV/2)/PATH_MAP_SUBDIV).IsUnderWater())
		{ // check for water
			pBytemap[i] |= PATH_NOWATER;
		}//if
	}// for
	return TRUE;
}


/*
WPM2PathTGA
***********
Initialize the PathTGA using the WPM (path map) data.

In:
  nothing

Out:
  nothing

Return:
  BOOL: TRUE if the data convertion has been done successfully, FALSE otherwise

*/
BOOL CWar3Map::WPM2PathTGA()
{
	CWaitCursor wait;
	byte* pBitmap;
	byte* pBytemap;
	int x, y,j ,i;
	int iWidth, iHeight;

	iWidth = GetPathMapWidth();
	iHeight = GetPathMapHeight();

	// check if TGA path is ok
	if ((iWidth <= 0) || (iHeight <= 0))
	{// invalid size
		return FALSE;
	}

	pBytemap = GetPathMapPtr();
	// allocate memory
	mapTGAPath.SetSize(iWidth, iHeight);
	pBitmap = mapTGAPath.GetBitmapPtr();

	// check if memory has been allocated
	if ((NULL == pBitmap) || (NULL == pBytemap))
	{// allocation failed
		return FALSE;
	}

	j = 0; // used to index pixels of the TGA
	for (y = iHeight-1; y >= 0 ; y--)
	for (x = 0; x < iWidth; x++)
	{
		// gets the path map WPM index i
		i = x + (y*iWidth);
		// resets pathing colors
		pBitmap[j] = 0; //B
		pBitmap[j+1] = 0; //G
		pBitmap[j+2] = 0; //R
		pBitmap[j+3] = 0; //X
		if (PATH_NOBUILD & pBytemap[i])
		{ // blue
			pBitmap[j] = 0xFF;
		}
		if (PATH_NOFLY & pBytemap[i])
		{ // green
			pBitmap[j+1] = 0xFF;
		}
		if (PATH_NOWALK & pBytemap[i])
		{ // red
			pBitmap[j+2] = 0xFF;
		}
		if (PATH_BLIGHT & pBytemap[i])
		{ // alpha
			pBitmap[j+3] = 0xFF;
		}
		j += 4; // 4 == pixel size (32bit mode)
	}
	return TRUE;
}

/*
ResetPathing
************
Set WPM (path map) data using the tilepoints data

In:
  nothing

Out:
  nothing

Return:
  nothing

*/
void CWar3Map::ResetPathing()
{
	CWaitCursor wait;
	byte* pBytemap;
	int x, y, i;
	int iWidth, iHeight;

	iWidth = GetPathMapWidth();
	iHeight = GetPathMapHeight();

	if ((GetMapWidth()*PATH_MAP_SUBDIV != iWidth) || (GetMapHeight()*PATH_MAP_SUBDIV != iHeight))
	{
		// reset path map size
		SetPathMapSize(GetMapWidth()*PATH_MAP_SUBDIV, GetMapHeight()*PATH_MAP_SUBDIV);
	}

	// check if TGA path is ok
	if ((iWidth <= 0) || (iHeight <= 0))
	{// invalid size
		return;
	}

	pBytemap = GetPathMapPtr();

	// check if memory has been allocated
	if (NULL == pBytemap)
	{// allocation failed
		return;
	}

	for (y = iHeight-1; y >=0 ; y--)
	for (x = 0; x < iWidth; x++)
	{
		i = x + (y*iWidth);
		// clear path info
		pBytemap[i] = 0;
		/*
const byte PATH_UNK01				= 0x01;
const byte PATH_NOWALK				= 0x02;
const byte PATH_NOFLY				= 0x04;
const byte PATH_NOBUILD				= 0x08;
const byte PATH_UNK02				= 0x10;
const byte PATH_BLIGHT				= 0x20;
const byte PATH_NOWATER				= 0x40;
const byte PATH_UNKNOWN				= 0x80;
		*/
		if ((GetTilePoint((x+PATH_MAP_SUBDIV/2)/PATH_MAP_SUBDIV, (y+PATH_MAP_SUBDIV/2)/PATH_MAP_SUBDIV).flags & TILE_FL_MINISHAD)
			|| (GetTilePoint((x+PATH_MAP_SUBDIV/2)/PATH_MAP_SUBDIV, (y+PATH_MAP_SUBDIV/2)/PATH_MAP_SUBDIV).flags & TILE_FL_SHADOW))
		{// check for blight
			pBytemap[i] |= PATH_UNKNOWN | PATH_NOBUILD | PATH_NOWALK | PATH_NOFLY;
		}// if

		if (FALSE == IsBuildable((float)(x+PATH_MAP_SUBDIV/2)*(float)DOOD_MAP_SUBDIV/(float)PATH_MAP_SUBDIV + mapCenterX,(float)(y+PATH_MAP_SUBDIV/2)*(float)DOOD_MAP_SUBDIV/(float)PATH_MAP_SUBDIV + mapCenterY))
		{
			pBytemap[i] |= PATH_NOBUILD;
		}

		if (TRUE == IsCliff((float)(x+PATH_MAP_SUBDIV/2)*(float)DOOD_MAP_SUBDIV/(float)PATH_MAP_SUBDIV + mapCenterX,(float)(y+PATH_MAP_SUBDIV/2)*(float)DOOD_MAP_SUBDIV/(float)PATH_MAP_SUBDIV + mapCenterY))
		{
			pBytemap[i] |= PATH_NOWALK | PATH_NOBUILD;
		}

		if (TRUE == GetTilePoint((x+PATH_MAP_SUBDIV/2)/PATH_MAP_SUBDIV, (y+PATH_MAP_SUBDIV/2)/PATH_MAP_SUBDIV).IsBlight())
		{// check for blight
			pBytemap[i] |= PATH_BLIGHT;
		}// if

		// check for water
		if (TRUE == GetTilePoint((x+PATH_MAP_SUBDIV/2)/PATH_MAP_SUBDIV, (y+PATH_MAP_SUBDIV/2)/PATH_MAP_SUBDIV).IsUnderWater())
		{// is under water
			pBytemap[i] |= PATH_NOBUILD;
		}// if
		else
		{// is not under water
			pBytemap[i] |= PATH_NOWATER;
		}
	}// for

}


/*
IsCliff
*******
Says if a cliff is set or not at a specific coordinate on the map.

In:
  float fx: x coorinate (float coordinate)
  float fy: y coorinate (float coordinate)

Out:
  nothing

Return:
  BOOL: TRUE if there is a cliff, FALSE otherwise

*/
BOOL CWar3Map::IsCliff(float fx, float fy)
{
	BOOL bRet = FALSE;
	BOOL b2Layers = FALSE;
	int ix, iy;
	CPoint pt;
	int iLayer = 0;
	int iNbRamp = 0;
	CTilePoint tTilepoint;

	// check coordinates
//+FIXME	if (fx ...)

	//get the top left tilepoint layer
	ix = (int)((fx - mapCenterX) / DOOD_MAP_SUBDIV);
	iy = (int)((fy - mapCenterY) / DOOD_MAP_SUBDIV);
	tTilepoint = GetTilePoint(ix, iy);
	iLayer = tTilepoint.layer;
	if (TILE_FL_RAMP & tTilepoint.flags)
	{iNbRamp++;}

	// top right
	ix = (int)((fx - mapCenterX) / DOOD_MAP_SUBDIV) + 1;
	iy = (int)((fy - mapCenterY) / DOOD_MAP_SUBDIV);
	tTilepoint = GetTilePoint(ix, iy);
	if (tTilepoint.layer != iLayer)
	{
		bRet = TRUE;
	}
	if ((tTilepoint.layer - iLayer)*(tTilepoint.layer - iLayer) > 1)
	{
		b2Layers = TRUE;
	}
	iLayer = tTilepoint.layer;
	if (TILE_FL_RAMP & tTilepoint.flags)
	{iNbRamp++;}

	// bottom left
	ix = (int)((fx - mapCenterX) / DOOD_MAP_SUBDIV);
	iy = (int)((fy - mapCenterY) / DOOD_MAP_SUBDIV) + 1;
	tTilepoint = GetTilePoint(ix, iy);
	if (tTilepoint.layer != iLayer)
	{
		bRet = TRUE;
	}
	if ((tTilepoint.layer - iLayer)*(tTilepoint.layer - iLayer) > 1)
	{
		b2Layers = TRUE;
	}
	iLayer = tTilepoint.layer;
	if (TILE_FL_RAMP & tTilepoint.flags)
	{iNbRamp++;}

	// bottom right
	ix = (int)((fx - mapCenterX) / DOOD_MAP_SUBDIV) + 1;
	iy = (int)((fy - mapCenterY) / DOOD_MAP_SUBDIV) + 1;
	tTilepoint = GetTilePoint(ix, iy);
	if (tTilepoint.layer != iLayer)
	{
		bRet = TRUE;
	}
	if ((tTilepoint.layer - iLayer)*(tTilepoint.layer - iLayer) > 1)
	{
		b2Layers = TRUE;
	}
	iLayer = tTilepoint.layer;
	if (TILE_FL_RAMP & tTilepoint.flags)
	{iNbRamp++;}

	if ((iNbRamp > 3) && (FALSE == b2Layers))
	{
		bRet = FALSE;
	}

	return bRet;
}



// Coordinates convertions
//*************************

CFloatPoint CWar3Map::TilePointCoord2FloatCoord(int x, int y)
{
	CFloatPoint fptRet;
	fptRet.x = (float)(x*DOOD_MAP_SUBDIV);
	fptRet.y = (float)(y*DOOD_MAP_SUBDIV);
	return AbsFloatCoord2FloatCoord(fptRet);
}


CFloatPoint CWar3Map::TilePointCoord2FloatCoord(CPoint pt)
{
	CFloatPoint fptRet;
	fptRet.x = (float)(pt.x*DOOD_MAP_SUBDIV);
	fptRet.y = (float)(pt.y*DOOD_MAP_SUBDIV);
	return AbsFloatCoord2FloatCoord(fptRet);
}


CPoint CWar3Map::TilePointCoord2SubMapCoord(int x, int y)
{
	CPoint ptRet;
	ptRet.x = x*PATH_MAP_SUBDIV;
	ptRet.y = y*PATH_MAP_SUBDIV;
	return ptRet;
}


CPoint CWar3Map::TilePointCoord2SubMapCoord(CPoint pt)
{
	CPoint ptRet;
	ptRet.x = pt.x*PATH_MAP_SUBDIV;
	ptRet.y = pt.y*PATH_MAP_SUBDIV;
	return ptRet;
}


CPoint CWar3Map::FloatCoord2SubMapCoord(float fx, float fy)
{
	CPoint ptRet;
	ptRet.x = (int)(fx*PATH_MAP_SUBDIV/DOOD_MAP_SUBDIV);
	ptRet.y = (int)(fy*PATH_MAP_SUBDIV/DOOD_MAP_SUBDIV);
	return ptRet;
}


CPoint CWar3Map::FloatCoord2SubMapCoord(CFloatPoint fpt)
{
	CPoint ptRet;
	ptRet.x = (int)(fpt.x*PATH_MAP_SUBDIV/DOOD_MAP_SUBDIV);
	ptRet.y = (int)(fpt.y*PATH_MAP_SUBDIV/DOOD_MAP_SUBDIV);
	return ptRet;
}


CPoint CWar3Map::FloatCoord2TilePointCoord(float fx, float fy)
{
	CPoint ptRet;
	ptRet.x = (int)(fx/DOOD_MAP_SUBDIV);
	ptRet.y = (int)(fy/DOOD_MAP_SUBDIV);
	return ptRet;
}


CPoint CWar3Map::FloatCoord2TilePointCoord(CFloatPoint fpt)
{
	CPoint ptRet;
	ptRet.x = (int)(fpt.x/DOOD_MAP_SUBDIV);
	ptRet.y = (int)(fpt.y/DOOD_MAP_SUBDIV);
	return ptRet;
}


CFloatPoint CWar3Map::SubMapCoord2FloatCoord(int x, int y)
{
	CFloatPoint fptRet;
	fptRet.x = (float)(x*DOOD_MAP_SUBDIV/PATH_MAP_SUBDIV);
	fptRet.y = (float)(y*DOOD_MAP_SUBDIV/PATH_MAP_SUBDIV);
	return AbsFloatCoord2FloatCoord(fptRet);
}


CFloatPoint CWar3Map::SubMapCoord2FloatCoord(CPoint pt)
{
	CFloatPoint fptRet;
	fptRet.x = (float)(pt.x*DOOD_MAP_SUBDIV/PATH_MAP_SUBDIV);
	fptRet.y = (float)(pt.y*DOOD_MAP_SUBDIV/PATH_MAP_SUBDIV);
	return AbsFloatCoord2FloatCoord(fptRet);
}


CPoint CWar3Map::SubMapCoord2TilePointCoord(int x, int y)
{
	CPoint ptRet;
	ptRet.x = x/PATH_MAP_SUBDIV;
	ptRet.y = y/PATH_MAP_SUBDIV;
	return ptRet;
}


CPoint CWar3Map::SubMapCoord2TilePointCoord(CPoint pt)
{
	CPoint ptRet;
	ptRet.x = pt.x/PATH_MAP_SUBDIV;
	ptRet.y = pt.y/PATH_MAP_SUBDIV;
	return ptRet;
}


CFloatPoint CWar3Map::FloatCoord2AbsFloatCoord(float fx, float fy)
{
	CFloatPoint fptRet;
	fptRet.x = fx - mapCenterX;
	fptRet.y = fy - mapCenterY;
	return fptRet;
}


CFloatPoint CWar3Map::FloatCoord2AbsFloatCoord(CFloatPoint fpt)
{
	CFloatPoint fptRet;
	fptRet.x = fpt.x - mapCenterX;
	fptRet.y = fpt.y - mapCenterY;
	return fptRet;
}


CFloatPoint CWar3Map::AbsFloatCoord2FloatCoord(float fx, float fy)
{
	CFloatPoint fptRet;
	fptRet.x = fx + mapCenterX;
	fptRet.y = fy + mapCenterY;
	return fptRet;
}


CFloatPoint CWar3Map::AbsFloatCoord2FloatCoord(CFloatPoint fpt)
{
	CFloatPoint fptRet;
	fptRet.x = fpt.x + mapCenterX;
	fptRet.y = fpt.y + mapCenterY;
	return fptRet;
}


/*
IsBuildable
***********
Says if it is possible or not to build at a specific coordinate on the map.
It uses tilepoints data to tell if it's possible or not and doesn't use path data.

In:
  float fx:
  float fy:

Out:
  nothing

Return:
  BOOL: TRUE if it's a buildable place, FALSE otherwise

*/
BOOL CWar3Map::IsBuildable(float fx, float fy)
{
	int ix, iy;
	float fHeigh1, fHeigh2, fHeigh3, fHeigh4, fHDiff;
	CTilePoint tTilepoint;

	if (TRUE == IsCliff(fx, fy))
	{
		return FALSE;
	}

	//get the bottom left tilepoint layer
	ix = (int)((fx - mapCenterX) / DOOD_MAP_SUBDIV);
	iy = (int)((fy - mapCenterY) / DOOD_MAP_SUBDIV);
	tTilepoint = GetTilePoint(ix, iy);
	fHeigh1 = (float)tTilepoint.GetGroundLevel();

	//check the ground type
	if (FALSE == m_pAppSettings->IsTileBuildable(CWar3StrID(mapGroundTiles.Mid(tTilepoint.groundtype*4, 4))))
	{
		return FALSE;
	}

	// bottom right
	tTilepoint = GetTilePoint(ix+1, iy);
	fHeigh2 = (float)tTilepoint.GetGroundLevel();

	// top left
	tTilepoint = GetTilePoint(ix, iy+1);
	fHeigh3 = (float)tTilepoint.GetGroundLevel();

	// top right
	tTilepoint = GetTilePoint(ix+1, iy+1);
	fHeigh4 = (float)tTilepoint.GetGroundLevel();

	//check the slope
	fHDiff = fabs(fHeigh1 - fHeigh2);
	if (fHDiff >= m_pAppSettings->pathNoBuildHeightDelta)
	{
		return FALSE;
	}
	fHDiff = fabs(fHeigh1 - fHeigh3);
	if (fHDiff >= m_pAppSettings->pathNoBuildHeightDelta)
	{
		return FALSE;
	}
	fHDiff = fabs(fHeigh1 - fHeigh4);
	if (fHDiff >= m_pAppSettings->pathNoBuildHeightDelta)
	{
		return FALSE;
	}
	fHDiff = fabs(fHeigh2 - fHeigh3);
	if (fHDiff >= m_pAppSettings->pathNoBuildHeightDelta)
	{
		return FALSE;
	}
	fHDiff = fabs(fHeigh2 - fHeigh4);
	if (fHDiff >= m_pAppSettings->pathNoBuildHeightDelta)
	{
		return FALSE;
	}
	fHDiff = fabs(fHeigh3 - fHeigh4);
	if (fHDiff >= m_pAppSettings->pathNoBuildHeightDelta)
	{
		return FALSE;
	}


	return TRUE;
}

/*
FixMap
******
Corrects any map big issue or unconsistency and fill missing fields

*/
void CWar3Map::FixMap()
{
	CWaitCursor wait;

}

