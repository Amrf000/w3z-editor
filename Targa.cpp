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
// Targa.cpp: implementation of the CTarga class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Targa.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTarga::CTarga()
{
// Image Data
	m_pBitmap = NULL;
// Color Map Data
	m_pColorMap = NULL;
// Image ID 
	m_pImageID = NULL;

// Image Spec 10 bytes
	m_ImgDesc = TGA_IMG_WAR3_FORMAT;
	m_PixelDepth = 32;
	m_Height = 0;
	m_Width = 0;
	m_YOrg = 0;
	m_XOrg = 0;

//	Color Map Spec 5 bytes
	m_ColorMapEntrySize = 0;
	m_ColorMapLength = 0;
	m_FirstEntryIndex = 0;

	m_ImageType = TGA_TYPE_UNCOMP_RGB;
	m_ColorMapType = 0;
	m_IDLength = 0;
}

CTarga::~CTarga()
{

	if (m_pBitmap)
	{delete m_pBitmap;}
// Color Map Data
	if (m_pColorMap)
	{delete m_pColorMap;}
// Image ID 
	if (m_pImageID)
	{delete m_pImageID;}

}

void CTarga::SetSize(int width, int height)
{
	if ((width > 0) && (height > 0) && (width <= TGA_MAX_WIDTH) && (height <= TGA_MAX_HEIGHT))
	{
		int oldwidth, oldheight, x, y, maxx, maxy, size;
		byte* pOldBitmap;
		oldwidth = m_Width;
		oldheight = m_Height;
		pOldBitmap = m_pBitmap;

		// cree un nouveau TGA
		m_IDLength = 0;
		m_ColorMapType = 0;
		m_ImageType = TGA_TYPE_UNCOMP_RGB;
		//	Color Map Spec 5 bytes
		m_FirstEntryIndex = 0;
		m_ColorMapLength = 0;
		m_ColorMapEntrySize = 0;
		// Image Spec 10 bytes
		m_XOrg = 0;
		m_YOrg = 0;
		m_Width = width;
		m_Height = height;
		m_PixelDepth = 32;
		m_ImgDesc = TGA_IMG_WAR3_FORMAT;
		size = m_Width * m_Height * 4; // 4: bytes par pixels
		m_pBitmap = new byte[size];
		memset(m_pBitmap, 0, size);

		if (pOldBitmap) // delete old PathTGA
		{
			if (m_Width < oldwidth)
			{
				maxx = m_Width;
			}
			else
			{
				maxx = oldwidth;
			}
			if (m_Height < oldheight)
			{
				maxy = m_Height;
			}
			else
			{
				maxy = oldheight;
			}
			for (y=0; y < maxy; y++)
			for (x=0; x < maxx; x++)
			{
				m_pBitmap[((m_Height - 1 - y)*m_Width + x)*4]   = pOldBitmap[((oldheight - 1 - y)*oldwidth + x)*4];
				m_pBitmap[((m_Height - 1 - y)*m_Width + x)*4+1] = pOldBitmap[((oldheight - 1 - y)*oldwidth + x)*4+1];
				m_pBitmap[((m_Height - 1 - y)*m_Width + x)*4+2] = pOldBitmap[((oldheight - 1 - y)*oldwidth + x)*4+2];
				m_pBitmap[((m_Height - 1 - y)*m_Width + x)*4+3] = pOldBitmap[((oldheight - 1 - y)*oldwidth + x)*4+3];
			}
			delete []pOldBitmap;
		}
	}
}

int CTarga::GetWidth()
{
	return m_Width;

}

int CTarga::GetHeight()
{
	return m_Height;
}

byte* CTarga::GetBitmapPtr()
{
	return m_pBitmap;
}
