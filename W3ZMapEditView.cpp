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
// W3ZMapEditView.cpp : implementation of the CW3ZMapEditView class
//

#include "stdafx.h"
#include "W3ZMapEdit.h"
#include "W3ZMapEditDoc.h"
#include "W3ZMapEditView.h"
#include "MainFrm.h"
#include "TilePoint.h"
#include "Doodad.h"
#include "Unit.h"
#include <math.h>
#include "ProgressWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CW3ZMapEditView

IMPLEMENT_DYNCREATE(CW3ZMapEditView, CScrollView)

BEGIN_MESSAGE_MAP(CW3ZMapEditView, CScrollView)
	//{{AFX_MSG_MAP(CW3ZMapEditView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_VIEW_REFRESH, OnViewRefresh)
	ON_COMMAND(ID_BTN_ZOOM_IN, OnBtnZoomIn)
	ON_COMMAND(ID_BTN_ZOOM_NO, OnBtnZoomNo)
	ON_COMMAND(ID_BTN_ZOOM_OUT, OnBtnZoomOut)
	ON_WM_MOUSEWHEEL()
	ON_WM_SETCURSOR()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CW3ZMapEditView construction/destruction

CW3ZMapEditView::CW3ZMapEditView()
{
	int i;

    CW3ZMapEditApp(* pTheApp) = (CW3ZMapEditApp *)AfxGetApp();
	m_pAppSettings = &(pTheApp->m_AppSettings);
	m_MRBtnDown = false;
	m_MLBtnDown = false;
	m_pImage = NULL;
	m_dZoom = 1;
	m_iViewFlags = VFL_TILES | VFL_DOODADS | VFL_UNITS | VFL_WATER | VFL_BOUNDS | VFL_WEBOUNDS;
	for (i=0; i < W3E_MAX_GROUNDTILES;i++)
	{
		m_TilesColors[i] = m_pAppSettings->GetTileColor(TS_STR_ID_UNDEFINED);
	}
	m_afWaterColor[2] = TS_WATER_COLOR & 0x0000FF; // red
	m_afWaterColor[1] = (TS_WATER_COLOR>>8) & 0x0000FF; // green
	m_afWaterColor[0] = (TS_WATER_COLOR>>16) & 0x0000FF; // blue
	m_afWaterColor[3] = TS_WATER_ALPHA;

	m_tRamp = 0;
	m_tBlight = 0;
	m_tBoundary = 0;
	m_fRampAlpha = 0;
	m_fBlightAlpha = 0;
	m_fBoundaryAlpha = 0;
	m_fLevelFactor = 1.0;

}

CW3ZMapEditView::~CW3ZMapEditView()
{
	if (m_pImage)
	{
		delete []m_pImage;
	}
    CW3ZMapEditApp(* pTheApp) = (CW3ZMapEditApp *)AfxGetApp();
	if (pTheApp)
	{
		CMainFrame *pMainFrame = pTheApp->GetMainFrame();
		if (pMainFrame)
		{
			pMainFrame->UpdateViewModeBar(0);
			pMainFrame->SetPositionText("");
		}
	}
}

BOOL CW3ZMapEditView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CW3ZMapEditView drawing

void CW3ZMapEditView::OnDraw(CDC* pDC)
{
	CW3ZMapEditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	int zmheight, zmwidth;
	zmwidth = (int)(m_pBitmapInfo->bV4Width * m_dZoom);
	zmheight = (int)(m_pBitmapInfo->bV4Height * m_dZoom);
	StretchDIBits(pDC->GetSafeHdc(), 0, 0 , zmwidth, zmheight, 0, 0, m_pBitmapInfo->bV4Width, m_pBitmapInfo->bV4Height, m_pMainBitmap, (const struct tagBITMAPINFO *) m_pBitmapInfo, DIB_RGB_COLORS,  SRCCOPY);

}

/////////////////////////////////////////////////////////////////////////////
// CW3ZMapEditView diagnostics

#ifdef _DEBUG
void CW3ZMapEditView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CW3ZMapEditView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CW3ZMapEditDoc* CW3ZMapEditView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CW3ZMapEditDoc)));
	return (CW3ZMapEditDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CW3ZMapEditView message handlers


void CW3ZMapEditView::OnInitialUpdate() 
{
	CScrollView::OnInitialUpdate();

	int BMPsize	= 0;
	CW3ZMapEditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	int u, i;
	CString str;
	CWar3StrID tileID;
	COLORREF tColor;

	// initialize color pallette
	u = pDoc->m_War3Map.mapGroundTiles.GetLength() / 4;
	if (u > W3E_MAX_GROUNDTILES)
	{
		u = W3E_MAX_GROUNDTILES;
	}
	str = pDoc->m_War3Map.mapGroundTiles;
	for (i=0; i < u; i++)
	{
/*
		tileID[0] = str[i*4];//str.Mid(i*4, 4),4);
		tileID[1] = str[i*4+1];
		tileID[2] = str[i*4+2];
		tileID[3] = str[i*4+3];
*/
		tileID = str.Mid(i*4, 4);
		m_TilesColors[i] = m_pAppSettings->GetTileColor(tileID);
	}

	tColor = m_pAppSettings->GetWaterColor(pDoc->m_War3Map.mapMainTileset);
	m_afWaterColor[2] = (float)(tColor & 0x0000FF); // red
	tColor >>= 8;
	m_afWaterColor[1] = (float)(tColor & 0x0000FF); // green
	tColor >>= 8;
	m_afWaterColor[0] = (float)(tColor & 0x0000FF); // blue
	m_afWaterColor[3] = m_pAppSettings->GetWaterAlpha(pDoc->m_War3Map.mapMainTileset);

	m_tRamp = m_pAppSettings->tRamp.tMaskColor;
	m_tBlight = m_pAppSettings->tBlight.tMaskColor;
	m_tBoundary = m_pAppSettings->tBoundary.tMaskColor;
	m_fRampAlpha = m_pAppSettings->tRamp.fAlpha;
	m_fBlightAlpha = m_pAppSettings->tBlight.fAlpha;
	m_fBoundaryAlpha = m_pAppSettings->tBoundary.fAlpha;
	m_fLevelFactor = m_pAppSettings->fLevelFactor;

	//create bitmap for display

	BMPsize = (pDoc->m_War3Map.GetMapWidth()*CR_WIDTH*pDoc->m_War3Map.GetMapHeight()*CR_HEIGHT*3);
	m_pImage = (unsigned char *) new char [sizeof(BITMAPV4HEADER) + BMPsize*6];
	m_pBitmapInfo = (BITMAPV4HEADER *) (m_pImage);
	m_pPalette = (RGBQUAD *)(m_pImage + sizeof(BITMAPV4HEADER));
	m_pMainBitmap = (m_pImage + sizeof (BITMAPV4HEADER));
	m_pTilesBitmap = (m_pMainBitmap + BMPsize);
	m_pDoodadsBitmap = (m_pTilesBitmap + BMPsize);
	m_pUnitsBitmap = (m_pDoodadsBitmap + BMPsize);
	m_pShadowsBitmap = (m_pUnitsBitmap + BMPsize);
	m_pPathBitmap = (m_pShadowsBitmap + BMPsize);


    m_pBitmapInfo->bV4Size = sizeof (BITMAPV4HEADER);
    m_pBitmapInfo->bV4Width = pDoc->m_War3Map.GetMapWidth()*CR_WIDTH;
    m_pBitmapInfo->bV4Height = pDoc->m_War3Map.GetMapHeight()*CR_HEIGHT;
    m_pBitmapInfo->bV4Planes = 1;
    m_pBitmapInfo->bV4BitCount = 24;
    m_pBitmapInfo->bV4V4Compression = BI_RGB;
    m_pBitmapInfo->bV4SizeImage = 0;
    m_pBitmapInfo->bV4XPelsPerMeter = 0;
    m_pBitmapInfo->bV4YPelsPerMeter = 0;
    m_pBitmapInfo->bV4ClrUsed = 0;
    m_pBitmapInfo->bV4ClrImportant = 0;
    m_pBitmapInfo->bV4RedMask = 0;
    m_pBitmapInfo->bV4GreenMask = 0;
    m_pBitmapInfo->bV4BlueMask = 0;
    m_pBitmapInfo->bV4AlphaMask = 0;
    m_pBitmapInfo->bV4CSType = LCS_sRGB;
    m_pBitmapInfo->bV4Endpoints = m_pCiexyzTriple;
    m_pBitmapInfo->bV4GammaRed = 0;
    m_pBitmapInfo->bV4GammaGreen = 0;
    m_pBitmapInfo->bV4GammaBlue = 0;

	memset(m_pMainBitmap, 0, BMPsize);
	memset(m_pTilesBitmap, 0, BMPsize);
	memset(m_pDoodadsBitmap, 0, BMPsize);
	memset(m_pUnitsBitmap, 0, BMPsize);
	memset(m_pShadowsBitmap, 0, BMPsize);
	memset(m_pPathBitmap, 0, BMPsize);

	InvalidateBitmap();

	SetScrollSizes(MM_TEXT,CSize((int)((double)m_pBitmapInfo->bV4Width * m_dZoom),(int)((double)m_pBitmapInfo->bV4Height * m_dZoom)),
		CSize((int)((double)pDoc->m_War3Map.GetMapWidth() * m_dZoom),(int)((double)pDoc->m_War3Map.GetMapHeight() * m_dZoom)),
		CSize((int)(CR_WIDTH * m_dZoom),(int)(CR_HEIGHT * m_dZoom)));
}




// redraw the bitmap in memory only, not on the display
void CW3ZMapEditView::InvalidateBitmap()
{
	CWaitCursor wait;
	int size;

	CProgressWnd wndProgress(NULL, "Updating display...", TRUE);
	// wndProgress.GoModal(); // Call this if you want a modal window
	wndProgress.SetRange(0, 100);
	wndProgress.SetPos(0);
	wndProgress.PeekAndPump();
	wndProgress.SetText("Redraw all...");

	//calcule le nombre de filtres actifs
	m_nbActiveFilters = 0;
	if (m_iViewFlags & (VFL_LEVELS | VFL_TILES))
	{
		m_nbActiveFilters++;
	}
	if (m_iViewFlags & VFL_PATH)
	{
		m_nbActiveFilters++;
	}
	if (m_iViewFlags & VFL_SHADOWS)
	{
		m_nbActiveFilters++;
	}

	// taille du bitmap de base
	size = m_pBitmapInfo->bV4Width*m_pBitmapInfo->bV4Height*3;
	memset(m_pMainBitmap, 0, size);
	CRect fullRec(0, 0, m_pBitmapInfo->bV4Width, m_pBitmapInfo->bV4Height);

	wndProgress.OffsetPos(10);
	wndProgress.PeekAndPump();
	wndProgress.SetText("Tilepoints...");
// tilepoints map
	if (m_iViewFlags & (VFL_LEVELS | VFL_TILES))
	{
		InvalidateTilesBitmap(fullRec);
		AddTilesFilter(fullRec);
	}

	wndProgress.OffsetPos(10);
	wndProgress.PeekAndPump();
	wndProgress.SetText("Path map...");
// path map
	if (m_iViewFlags & VFL_PATH)
	{
		InvalidatePathBitmap(fullRec);
		AddPathFilter(fullRec);
	}

	wndProgress.OffsetPos(10);
	wndProgress.PeekAndPump();
	wndProgress.SetText("Shadows...");
// shadows map
	if (m_iViewFlags & VFL_SHADOWS)
	{
		InvalidateShadowsBitmap(fullRec);
		AddShadowsFilter(fullRec);
	}

	wndProgress.OffsetPos(10);
	wndProgress.PeekAndPump();
	wndProgress.SetText("Doodads...");
//doodads map
	if (m_iViewFlags & VFL_DOODADS)
	{
		InvalidateDoodadsBitmap(fullRec);
		AddDoodadsFilter(fullRec);
	}

	wndProgress.OffsetPos(10);
	wndProgress.PeekAndPump();
	wndProgress.SetText("Units...");
//units map
	if (m_iViewFlags & VFL_UNITS)
	{
		InvalidateUnitsBitmap(fullRec);
		AddUnitsFilter(fullRec);
	}
	wndProgress.SetText("Done!");
	wndProgress.SetPos(100);
	wndProgress.PeekAndPump();
}


// display a tilepoint at specified coordinates
// coordinates are tilepoints map coordinates (.w3e)
void CW3ZMapEditView::DisplayTilePoint(int x, int y, CTilePoint tilepoint)
{
	byte* dibits;
	int ix, iy, xmin, xmax, ymin, ymax;
	int bmode = m_pBitmapInfo->bV4BitCount/8;
	int iWaterValue, iRampValue, iBlightValue, iBoundaryValue;
	double factor, newdibits;
	COLORREF color;

	CW3ZMapEditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// check coordinates
	// note: "length of a line of tilepoints" = pDoc->m_War3Map.GetMapWidth() + 1
	if ((x < 0) || (x > pDoc->m_War3Map.GetMapWidth()) || (y < 0) || (y > pDoc->m_War3Map.GetMapHeight()))
	{return;}

	xmin = x*CR_WIDTH - CR_WIDTH/2;
	xmax = x*CR_WIDTH + CR_WIDTH/2 - 1;
	if (xmin < 0)
	{xmin = 0;}
	if (xmax >= m_pBitmapInfo->bV4Width)
	{xmax = m_pBitmapInfo->bV4Width - 1;}

	ymin = y*CR_HEIGHT - CR_HEIGHT/2;
	ymax = y*CR_HEIGHT + CR_HEIGHT/2 - 1;
	if (ymin < 0)
	{ymin = 0;}
	if (ymax >= m_pBitmapInfo->bV4Height)
	{ymax = m_pBitmapInfo->bV4Height - 1;}


	for (iy = ymin; iy <= ymax; iy++)
	{
		for (ix = xmin; ix <= xmax; ix++)
		{
			dibits = m_pTilesBitmap + (ix + iy*m_pBitmapInfo->bV4Width)*bmode;
			//color choice...
			if ((tilepoint.selected) && (tilepoint.viewcolor.rgbReserved))
			{// selected and special ==> use selected color
				*(dibits  ) = tilepoint.viewcolor.rgbBlue; //B
				*(dibits+1) = tilepoint.viewcolor.rgbGreen; //G
				*(dibits+2) = tilepoint.viewcolor.rgbRed; //R
			}
			else if (VFL_TILES & m_iViewFlags)
			{// normal tile
				color = m_TilesColors[tilepoint.groundtype];
				*(dibits  ) = (byte)((color & 0x00FF0000)>>16); //B
				*(dibits+1) = (byte)((color & 0x0000FF00)>>8); //G
				*(dibits+2) = (byte)(color & 0x000000FF); //R
			}
			else if (VFL_LEVELS & m_iViewFlags)
			{// gray levels
				color = m_pAppSettings->GetTileColor(TS_STR_ID_UNDEFINED);
				*(dibits  ) = (byte)((color & 0x00FF0000)>>16); //B
				*(dibits+1) = (byte)((color & 0x0000FF00)>>8); //G
				*(dibits+2) = (byte)(color & 0x000000FF); //R
			}

			if ((tilepoint.selected) && !(tilepoint.viewcolor.rgbReserved))
			{// selected and non-special tilepoint ==> invert colors
				*(dibits  )  = (byte)(~*(dibits  )); //B
				*(dibits+1)  = (byte)(~*(dibits+1)); //V
				*(dibits+2)  = (byte)(~*(dibits+2)); //R
			}

			// compute gamma factor for 2D level rendering
			// min ground height is -256, max is 1536
			factor = tilepoint.GetGroundLevel()/4;
			if (factor < -256)
			{factor = -256;}
			else if (factor > 1536)
			{factor = 1536;}
			factor = factor/1536;

			newdibits =  *(dibits  );
			newdibits =  newdibits + (newdibits*m_fLevelFactor)*factor;
			if (newdibits > 255)
			{newdibits = 255;}
			else if (newdibits < 0)
			{newdibits = 0;}
			*(dibits  ) = (char)newdibits;

			newdibits =  *(dibits+1);
			newdibits =  newdibits + (newdibits*m_fLevelFactor)*factor;
			if (newdibits > 255)
			{newdibits = 255;}
			else if (newdibits < 0)
			{newdibits = 0;}
			*(dibits+1) = (char)newdibits;

			newdibits =  *(dibits+2);
			newdibits =  newdibits + (newdibits*m_fLevelFactor)*factor;
			if (newdibits > 255)
			{newdibits = 255;}
			else if (newdibits < 0)
			{newdibits = 0;}
			*(dibits+2) = (char)newdibits;

			if (!tilepoint.selected)
			{
				if ((VFL_WATER & m_iViewFlags) && tilepoint.IsUnderWater())
				{
					iWaterValue = (int)(*(dibits)*(1-m_afWaterColor[3]) + m_afWaterColor[0]*m_afWaterColor[3]);
					if (iWaterValue > 255)
					{
						iWaterValue = 255;
					}
					*(dibits  )  = (byte)iWaterValue;

					iWaterValue = (int)(*(dibits+1)*(1-m_afWaterColor[3]) + m_afWaterColor[1]*m_afWaterColor[3]);
					if (iWaterValue > 255)
					{
						iWaterValue = 255;
					}
					*(dibits+1)  = (byte)iWaterValue;

					iWaterValue = (int)(*(dibits+2)*(1-m_afWaterColor[3]) + m_afWaterColor[2]*m_afWaterColor[3]);
					if (iWaterValue > 255)
					{
						iWaterValue = 255;
					}
					*(dibits+2)  = (byte)iWaterValue;
				}
				if ((VFL_BLIGHTS & m_iViewFlags) && (tilepoint.flags & (TILE_FL_BLIGHT)))
				{
					iBlightValue = (int)(*(dibits)*(1-m_fBlightAlpha) + (m_tBlight>>16 & 0x00FF)*m_fBlightAlpha);
					if (iBlightValue > 255)
					{
						iBlightValue = 255;
					}
					*(dibits  )  = (byte)iBlightValue;

					iBlightValue = (int)(*(dibits+1)*(1-m_fBlightAlpha) + (m_tBlight>>8 & 0x00FF)*m_fBlightAlpha);
					if (iBlightValue > 255)
					{
						iBlightValue = 255;
					}
					*(dibits+1)  = (byte)iBlightValue;

					iBlightValue = (int)(*(dibits+2)*(1-m_fBlightAlpha) + (m_tBlight & 0x00FF)*m_fBlightAlpha);
					if (iBlightValue > 255)
					{
						iBlightValue = 255;
					}
					*(dibits+2)  = (byte)iBlightValue;
				}
				if ((VFL_BOUNDS & m_iViewFlags) && (tilepoint.flags & (TILE_FL_MINISHAD)))
				{
					iBoundaryValue = (int)(*(dibits)*(1-m_fBoundaryAlpha) + (m_tBoundary>>16 & 0x00FF)*m_fBoundaryAlpha);
					if (iBoundaryValue > 255)
					{
						iBoundaryValue = 255;
					}
					*(dibits  )  = (byte)iBoundaryValue;

					iBoundaryValue = (int)(*(dibits+1)*(1-m_fBoundaryAlpha) + (m_tBoundary>>8 & 0x00FF)*m_fBoundaryAlpha);
					if (iBoundaryValue > 255)
					{
						iBoundaryValue = 255;
					}
					*(dibits+1)  = (byte)iBoundaryValue;

					iBoundaryValue = (int)(*(dibits+2)*(1-m_fBoundaryAlpha) + (m_tBoundary & 0x00FF)*m_fBoundaryAlpha);
					if (iBoundaryValue > 255)
					{
						iBoundaryValue = 255;
					}
					*(dibits+2)  = (byte)iBoundaryValue;
				}
				if (((VFL_WEBOUNDS & m_iViewFlags) && (tilepoint.flags & (TILE_FL_SHADOW)))
					&& !((VFL_BOUNDS & m_iViewFlags) && (tilepoint.flags & (TILE_FL_MINISHAD))))
				{
					iBoundaryValue = (int)(*(dibits)*(1-m_fBoundaryAlpha) + (m_tBoundary>>16 & 0x00FF)*m_fBoundaryAlpha);
					if (iBoundaryValue > 255)
					{
						iBoundaryValue = 255;
					}
					*(dibits  )  = (byte)iBoundaryValue;

					iBoundaryValue = (int)(*(dibits+1)*(1-m_fBoundaryAlpha) + (m_tBoundary>>8 & 0x00FF)*m_fBoundaryAlpha);
					if (iBoundaryValue > 255)
					{
						iBoundaryValue = 255;
					}
					*(dibits+1)  = (byte)iBoundaryValue;

					iBoundaryValue = (int)(*(dibits+2)*(1-m_fBoundaryAlpha) + (m_tBoundary & 0x00FF)*m_fBoundaryAlpha);
					if (iBoundaryValue > 255)
					{
						iBoundaryValue = 255;
					}
					*(dibits+2)  = (byte)iBoundaryValue;
				}
				if ((VFL_RAMPS & m_iViewFlags) && (tilepoint.flags & (TILE_FL_RAMP)))
				{
					iRampValue = (int)(*(dibits)*(1-m_fRampAlpha) + (m_tRamp>>16 & 0x00FF)*m_fRampAlpha);
					if (iRampValue > 255)
					{
						iRampValue = 255;
					}
					*(dibits  )  = (byte)iRampValue;

					iRampValue = (int)(*(dibits+1)*(1-m_fRampAlpha) + (m_tRamp>>8 & 0x00FF)*m_fRampAlpha);
					if (iRampValue > 255)
					{
						iRampValue = 255;
					}
					*(dibits+1)  = (byte)iRampValue;

					iRampValue = (int)(*(dibits+2)*(1-m_fRampAlpha) + (m_tRamp & 0x00FF)*m_fRampAlpha);
					if (iRampValue > 255)
					{
						iRampValue = 255;
					}
					*(dibits+2)  = (byte)iRampValue;
				}
				if ((VFL_LAYERS & m_iViewFlags) && ((ix+iy)%2))
				{
					color = m_pAppSettings->LayersColors[tilepoint.layer];
					*(dibits  ) = (BYTE)((color & 0x00FF0000)>>16);
					*(dibits+1) = (BYTE)((color & 0x0000FF00)>>8);
					*(dibits+2) = (BYTE)(color & 0x000000FF);
				}
			}
		}
	}
}


void CW3ZMapEditView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture();
	m_MLBtnDown = true;
    CW3ZMapEditApp(* pTheApp) = (CW3ZMapEditApp *)AfxGetApp();
	CPoint bmpPoint;
	bmpPoint = point + GetScrollPosition();
	bmpPoint.x = (int)(bmpPoint.x / m_dZoom );
	bmpPoint.y = (int)(bmpPoint.y / m_dZoom );
	if (pTheApp->m_pCurrentTool)
	{
	 pTheApp->m_pCurrentTool->SetCursor();
	 pTheApp->m_pCurrentTool->OnLButtonDown(this, nFlags, bmpPoint);
	}

	CScrollView::OnLButtonDown(nFlags, point);
}


void CW3ZMapEditView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (!m_MRBtnDown)
	{
		ReleaseCapture();
	}
	m_MLBtnDown = false;
    CW3ZMapEditApp(* pTheApp) = (CW3ZMapEditApp *)AfxGetApp();
	CPoint bmpPoint;
	bmpPoint = point + GetScrollPosition();
	bmpPoint.x = (int)(bmpPoint.x / m_dZoom );
	bmpPoint.y = (int)(bmpPoint.y / m_dZoom );
	if (pTheApp->m_pCurrentTool)
	{
	 pTheApp->m_pCurrentTool->SetCursor();
	 pTheApp->m_pCurrentTool->OnLButtonUp(this, nFlags, bmpPoint);
	}
	
	CScrollView::OnLButtonUp(nFlags, point);
}

void CW3ZMapEditView::OnMouseMove(UINT nFlags, CPoint point) 
{
    CW3ZMapEditApp(* pTheApp) = (CW3ZMapEditApp *)AfxGetApp();
	CMainFrame *pMainFrame = pTheApp->GetMainFrame();
	CW3ZMapEditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CString StrPos;
	float x, y, z;
	CPoint bmpPoint;
	bmpPoint = point + GetScrollPosition();
	bmpPoint.x = (int)(bmpPoint.x / m_dZoom );
	bmpPoint.y = (int)(bmpPoint.y / m_dZoom );
	if (pTheApp->m_pCurrentTool)
	{
//	 pTheApp->m_pCurrentTool->SetCursor();
	 pTheApp->m_pCurrentTool->OnMouseMove(this, nFlags, bmpPoint);
	}

	if ((NULL != m_pBitmapInfo) && (m_pBitmapInfo->bV4Width > 0) && (m_pBitmapInfo->bV4Height > 0))
	{
		x = (float)((bmpPoint.x * pDoc->m_War3Map.GetMapWidth()*128) / m_pBitmapInfo->bV4Width);
		y = (float)(((m_pBitmapInfo->bV4Height - bmpPoint.y) * pDoc->m_War3Map.GetMapHeight()*128) / m_pBitmapInfo->bV4Height);
		x += pDoc->m_War3Map.GetMapCenterX();
		y += pDoc->m_War3Map.GetMapCenterY();
		z = pDoc->m_War3Map.GetGroundLevel(x,y);
	}
	else
	{
		x = y = z = 0;
	}

	StrPos.Format(szCOORDINATES, x, y, z);
	if (pMainFrame)
	{
		pMainFrame->SetPositionText(StrPos);
	}

	CScrollView::OnMouseMove(nFlags, point);
}

void CW3ZMapEditView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture();
	m_MRBtnDown = true;
    CW3ZMapEditApp(* pTheApp) = (CW3ZMapEditApp *)AfxGetApp();
	CPoint bmpPoint;
	bmpPoint = point + GetScrollPosition();
	bmpPoint.x = (int)(bmpPoint.x / m_dZoom );
	bmpPoint.y = (int)(bmpPoint.y / m_dZoom );
	if (pTheApp->m_pCurrentTool)
	{
	 pTheApp->m_pCurrentTool->SetCursor();
	 pTheApp->m_pCurrentTool->OnRButtonDown(this, nFlags, bmpPoint);
	}
	CScrollView::OnRButtonDown(nFlags, point);
}

void CW3ZMapEditView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	if (!m_MLBtnDown)
	{
		ReleaseCapture();
	}
	m_MRBtnDown = false;
    CW3ZMapEditApp(* pTheApp) = (CW3ZMapEditApp *)AfxGetApp();
	CPoint bmpPoint;
	bmpPoint = point + GetScrollPosition();
	bmpPoint.x = (int)(bmpPoint.x / m_dZoom );
	bmpPoint.y = (int)(bmpPoint.y / m_dZoom );
	if (pTheApp->m_pCurrentTool)
	{
	 pTheApp->m_pCurrentTool->SetCursor();
	 pTheApp->m_pCurrentTool->OnRButtonUp(this, nFlags, bmpPoint);
	}
	CScrollView::OnRButtonUp(nFlags, point);
}


// display provided doodad
void CW3ZMapEditView::DisplayDoodad(CDoodad doodad)
{
	BYTE* dibits = m_pDoodadsBitmap;
	int x, y;

	CW3ZMapEditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	x = (int) ((doodad.x - pDoc->m_War3Map.GetMapCenterX()) * m_pBitmapInfo->bV4Width / (pDoc->m_War3Map.GetMapWidth()*128));
	y = (int) ((doodad.y - pDoc->m_War3Map.GetMapCenterY()) * m_pBitmapInfo->bV4Height / (pDoc->m_War3Map.GetMapHeight()*128));

	// teste la validite des coordonnees
	if ((x < 0) || (x > (signed)m_pBitmapInfo->bV4Width) || (y < 0) || (y > (signed)m_pBitmapInfo->bV4Height))
	{return;}

	BYTE B,R,V;
	B = 1;
	V = 192;
	R = 1;
	if (doodad.selected)
	{
		B = 1;
		V = 1;
		R = 192;
	}

	if ((x >= 0)
		&& (x < m_pBitmapInfo->bV4Width)
		&& (y >= 0)
		&& (y < m_pBitmapInfo->bV4Height))
	{
		dibits = m_pDoodadsBitmap + (x)*3 + (y)*m_pBitmapInfo->bV4Width*3;
		*(dibits  )  = B; //B
		*(dibits+1)  = V; //V
		*(dibits+2)  = R; //R
	}

	if (((x+1) >= 0)
		&& ((x+1) < m_pBitmapInfo->bV4Width)
		&& (y >= 0)
		&& (y < m_pBitmapInfo->bV4Height))
	{
		dibits = m_pDoodadsBitmap + (x + 1)*3 + (y)*m_pBitmapInfo->bV4Width*3;
		*(dibits  )  = B; //B
		*(dibits+1)  = V; //V
		*(dibits+2)  = R; //R
	}

	if (((x-1) >= 0)
		&& ((x-1) < m_pBitmapInfo->bV4Width)
		&& (y >= 0)
		&& (y < m_pBitmapInfo->bV4Height))
	{
		dibits = m_pDoodadsBitmap + (x - 1)*3 + (y)*m_pBitmapInfo->bV4Width*3;
		*(dibits  )  = B; //B
		*(dibits+1)  = V; //V
		*(dibits+2)  = R; //R
	}

	if ((x >= 0)
		&& (x < m_pBitmapInfo->bV4Width)
		&& ((y+1) >= 0)
		&& ((y+1) < m_pBitmapInfo->bV4Height))
	{
		dibits = m_pDoodadsBitmap + (x)*3 + (y + 1)*m_pBitmapInfo->bV4Width*3;
		*(dibits  )  = B; //B
		*(dibits+1)  = V; //V
		*(dibits+2)  = R; //R
	}

	if ((x >= 0)
		&& (x < m_pBitmapInfo->bV4Width)
		&& ((y-1) >= 0)
		&& ((y-1) < m_pBitmapInfo->bV4Height))
	{
		dibits = m_pDoodadsBitmap + (x)*3 + (y - 1)*m_pBitmapInfo->bV4Width*3;
		*(dibits  )  = B; //B
		*(dibits+1)  = V; //V
		*(dibits+2)  = R; //R
	}

}

void CW3ZMapEditView::OnViewRefresh() 
{
	InvalidateBitmap();
	Invalidate();
}

void CW3ZMapEditView::InvalidateBitmapRect(CRect rect)
{
	//corrige les offsets lies a la scroll view et verifie les limites
	CPoint pt1, pt2;
	CRect newrect;

	pt1 = rect.TopLeft();
	pt1.x = pt1.x;
	pt1.y = pt1.y;
	if (pt1.x < 0)
	{pt1.x = 0;}
	if (pt1.x >= m_pBitmapInfo->bV4Width)
	{pt1.x = m_pBitmapInfo->bV4Width - 1;}
	if (pt1.y < 0)
	{pt1.y = 0;}
	if (pt1.y >= m_pBitmapInfo->bV4Height)
	{pt1.y = m_pBitmapInfo->bV4Height - 1;}

	pt2 = rect.BottomRight();
	pt2.x = pt2.x;
	pt2.y = pt2.y;
	if (pt2.x < 0)
	{pt2.x = 0;}
	if (pt2.x >= m_pBitmapInfo->bV4Width)
	{pt2.x = m_pBitmapInfo->bV4Width - 1;}
	if (pt2.y < 0)
	{pt2.y = 0;}
	if (pt2.y >= m_pBitmapInfo->bV4Height)
	{pt2.y = m_pBitmapInfo->bV4Height - 1;}

	newrect = CRect(pt1, pt2);
	if (VFL_LEVELS | VFL_TILES)
	{
		AddTilesFilter(newrect);
	}
	if (m_iViewFlags & VFL_PATH)
	{
		AddPathFilter(newrect);
	}
	if (m_iViewFlags & VFL_SHADOWS)
	{
		AddShadowsFilter(newrect);
	}
	if (m_iViewFlags & VFL_DOODADS)
	{
		AddDoodadsFilter(newrect);
	}
	if (m_iViewFlags & VFL_UNITS)
	{
		AddUnitsFilter(newrect);
	}

	pt1 = rect.TopLeft();
	pt1.x = (int)(pt1.x * m_dZoom);
	pt1.y = (int)(pt1.y * m_dZoom);
	pt1 -= GetScrollPosition();
	if (pt1.x < 0)
	{pt1.x = 0;}
//	if (pt1.x >= (long)(m_pBitmapInfo->bV4Width * m_dZoom))
//	{pt1.x = (long)(m_pBitmapInfo->bV4Width * m_dZoom) - 1;}
	if (pt1.y < 0)
	{pt1.y = 0;}
//	if (pt1.y >= (long)(m_pBitmapInfo->bV4Height * m_dZoom))
//	{pt1.y = (long)(m_pBitmapInfo->bV4Height * m_dZoom) - 1;}

	pt2 = rect.BottomRight();
	pt2.x = (int)(pt2.x * m_dZoom);
	pt2.y = (int)(pt2.y * m_dZoom);
	pt2 -= GetScrollPosition();
	if (pt2.x < 0)
	{pt2.x = 0;}
//	if (pt2.x >= (long)(m_pBitmapInfo->bV4Width * m_dZoom))
//	{pt2.x = (long)(m_pBitmapInfo->bV4Width * m_dZoom) - 1;}
	if (pt2.y < 0)
	{pt2.y = 0;}
//	if (pt2.y >= (long)(m_pBitmapInfo->bV4Height * m_dZoom))
//	{pt2.y = (long)(m_pBitmapInfo->bV4Height * m_dZoom) - 1;}

	newrect = CRect(pt1, pt2);
	newrect.NormalizeRect();

	InvalidateRect(newrect, FALSE); // FALSE= no erase
}

void CW3ZMapEditView::AddTilesFilter(CRect rect)
{
	int i, j, k;
	int minx, maxx, miny, maxy;

	if (rect.TopLeft().x < rect.BottomRight().x)
	{
		minx = rect.TopLeft().x;
		maxx = rect.BottomRight().x;
	}
	else
	{
		minx = rect.BottomRight().x;
		maxx = rect.TopLeft().x;
	}
	if (minx < 0)
	{minx = 0;}
	if (maxx < 0)
	{maxx = 0;}
	if (minx >= m_pBitmapInfo->bV4Width)
	{minx = m_pBitmapInfo->bV4Width - 1;}
	if (maxx >= m_pBitmapInfo->bV4Width)
	{maxx = m_pBitmapInfo->bV4Width - 1;}

	
	if (rect.BottomRight().y > rect.TopLeft().y)
	{
		miny = m_pBitmapInfo->bV4Height - 1 - rect.BottomRight().y;
		maxy = m_pBitmapInfo->bV4Height - 1 - rect.TopLeft().y;
	}
	else
	{
		miny = m_pBitmapInfo->bV4Height - 1 - rect.TopLeft().y;
		maxy = m_pBitmapInfo->bV4Height - 1 - rect.BottomRight().y;
	}
	if (miny < 0)
	{miny = 0;}
	if (maxy < 0)
	{maxy = 0;}
	if (miny >= m_pBitmapInfo->bV4Height)
	{miny = m_pBitmapInfo->bV4Height - 1;}
	if (maxy >= m_pBitmapInfo->bV4Height)
	{maxy = m_pBitmapInfo->bV4Height - 1;}

	// copies the tile bitmap in the main one
	for (j = miny;j <= maxy; j++)
	{
		for (i = minx;i <= maxx; i++)
		{
			k = (i + j*m_pBitmapInfo->bV4Width)*3;
			m_pMainBitmap[k] =  m_pTilesBitmap[k];
			m_pMainBitmap[k+1] = m_pTilesBitmap[k+1];
			m_pMainBitmap[k+2] = m_pTilesBitmap[k+2];
		}
	}
}

void CW3ZMapEditView::AddDoodadsFilter(CRect rect)
{
	int i, j, k;
	int minx, maxx, miny, maxy;

	if (rect.TopLeft().x < rect.BottomRight().x)
	{
		minx = rect.TopLeft().x;
		maxx = rect.BottomRight().x;
	}
	else
	{
		minx = rect.BottomRight().x;
		maxx = rect.TopLeft().x;
	}
	if (minx < 0)
	{minx = 0;}
	if (maxx < 0)
	{maxx = 0;}
	if (minx >= m_pBitmapInfo->bV4Width)
	{minx = m_pBitmapInfo->bV4Width - 1;}
	if (maxx >= m_pBitmapInfo->bV4Width)
	{maxx = m_pBitmapInfo->bV4Width - 1;}

	
	if (rect.BottomRight().y > rect.TopLeft().y)
	{
		miny = m_pBitmapInfo->bV4Height - 1 - rect.BottomRight().y;
		maxy = m_pBitmapInfo->bV4Height - 1 - rect.TopLeft().y;
	}
	else
	{
		miny = m_pBitmapInfo->bV4Height - 1 - rect.TopLeft().y;
		maxy = m_pBitmapInfo->bV4Height - 1 - rect.BottomRight().y;
	}
	if (miny < 0)
	{miny = 0;}
	if (maxy < 0)
	{maxy = 0;}
	if (miny >= m_pBitmapInfo->bV4Height)
	{miny = m_pBitmapInfo->bV4Height - 1;}
	if (maxy >= m_pBitmapInfo->bV4Height)
	{maxy = m_pBitmapInfo->bV4Height - 1;}

	// copies only non-black pixels of the doodad bitmap to the main one
	for (j = miny;j <= maxy; j++)
	{
		for (i = minx;i <= maxx; i++)
		{
			k = i*3 + j*m_pBitmapInfo->bV4Width*3;
			if (m_pDoodadsBitmap[k]
				|| m_pDoodadsBitmap[k+1]
				|| m_pDoodadsBitmap[k+2])
			{
				m_pMainBitmap[k] = m_pDoodadsBitmap[k];
				m_pMainBitmap[k+1] = m_pDoodadsBitmap[k+1];
				m_pMainBitmap[k+2] = m_pDoodadsBitmap[k+2];
			}
		}
	}
}


void CW3ZMapEditView::AddPathFilter(CRect rect)
{
	int i, j, k;
	int minx, maxx, miny, maxy;
	double mainFact;

	if (rect.TopLeft().x < rect.BottomRight().x)
	{
		minx = rect.TopLeft().x;
		maxx = rect.BottomRight().x;
	}
	else
	{
		minx = rect.BottomRight().x;
		maxx = rect.TopLeft().x;
	}
	if (minx < 0)
	{minx = 0;}
	if (maxx < 0)
	{maxx = 0;}
	if (minx >= m_pBitmapInfo->bV4Width)
	{minx = m_pBitmapInfo->bV4Width - 1;}
	if (maxx >= m_pBitmapInfo->bV4Width)
	{maxx = m_pBitmapInfo->bV4Width - 1;}

	
	if (rect.BottomRight().y > rect.TopLeft().y)
	{
		miny = m_pBitmapInfo->bV4Height - 1 - rect.BottomRight().y;
		maxy = m_pBitmapInfo->bV4Height - 1 - rect.TopLeft().y;
	}
	else
	{
		miny = m_pBitmapInfo->bV4Height - 1 - rect.TopLeft().y;
		maxy = m_pBitmapInfo->bV4Height - 1 - rect.BottomRight().y;
	}
	if (miny < 0)
	{miny = 0;}
	if (maxy < 0)
	{maxy = 0;}
	if (miny >= m_pBitmapInfo->bV4Height)
	{miny = m_pBitmapInfo->bV4Height - 1;}
	if (maxy >= m_pBitmapInfo->bV4Height)
	{maxy = m_pBitmapInfo->bV4Height - 1;}

	
	if (m_nbActiveFilters <= 0)
	{m_nbActiveFilters = 1;}
	mainFact = (double)(m_nbActiveFilters-1)/(double)m_nbActiveFilters;

	for (j = miny;j <= maxy; j++)
	{
		for (i = minx;i <= maxx; i++)
		{
			k = (i + j*m_pBitmapInfo->bV4Width)*3;
			m_pMainBitmap[k] =  (byte)((double)m_pMainBitmap[k]*mainFact + (double)m_pPathBitmap[k]/(double)m_nbActiveFilters);
			m_pMainBitmap[k+1] = (byte)((double)m_pMainBitmap[k+1]*mainFact + (double)m_pPathBitmap[k+1]/(double)m_nbActiveFilters);
			m_pMainBitmap[k+2] = (byte)((double)m_pMainBitmap[k+2]*mainFact + (double)m_pPathBitmap[k+2]/(double)m_nbActiveFilters);
		}
	}
}

void CW3ZMapEditView::AddShadowsFilter(CRect rect)
{
	int i, j, k;
	int minx, maxx, miny, maxy;
	double mainFact;

	if (rect.TopLeft().x < rect.BottomRight().x)
	{
		minx = rect.TopLeft().x;
		maxx = rect.BottomRight().x;
	}
	else
	{
		minx = rect.BottomRight().x;
		maxx = rect.TopLeft().x;
	}
	if (minx < 0)
	{minx = 0;}
	if (maxx < 0)
	{maxx = 0;}
	if (minx >= m_pBitmapInfo->bV4Width)
	{minx = m_pBitmapInfo->bV4Width - 1;}
	if (maxx >= m_pBitmapInfo->bV4Width)
	{maxx = m_pBitmapInfo->bV4Width - 1;}

	
	if (rect.BottomRight().y > rect.TopLeft().y)
	{
		miny = m_pBitmapInfo->bV4Height - 1 - rect.BottomRight().y;
		maxy = m_pBitmapInfo->bV4Height - 1 - rect.TopLeft().y;
	}
	else
	{
		miny = m_pBitmapInfo->bV4Height - 1 - rect.TopLeft().y;
		maxy = m_pBitmapInfo->bV4Height - 1 - rect.BottomRight().y;
	}
	if (miny < 0)
	{miny = 0;}
	if (maxy < 0)
	{maxy = 0;}
	if (miny >= m_pBitmapInfo->bV4Height)
	{miny = m_pBitmapInfo->bV4Height - 1;}
	if (maxy >= m_pBitmapInfo->bV4Height)
	{maxy = m_pBitmapInfo->bV4Height - 1;}


	if (m_nbActiveFilters <= 0)
	{m_nbActiveFilters = 1;}
	mainFact = (double)(m_nbActiveFilters-1)/(double)m_nbActiveFilters;

	for (j = miny;j <= maxy; j++)
	{
		for (i = minx;i <= maxx; i++)
		{
			k = i*3 + j*m_pBitmapInfo->bV4Width*3;
			m_pMainBitmap[k] =  (byte)((double)m_pMainBitmap[k]*mainFact + (double)m_pShadowsBitmap[k]/(double)m_nbActiveFilters);
			m_pMainBitmap[k+1] = (byte)((double)m_pMainBitmap[k+1]*mainFact + (double)m_pShadowsBitmap[k+1]/(double)m_nbActiveFilters);
			m_pMainBitmap[k+2] = (byte)((double)m_pMainBitmap[k+2]*mainFact + (double)m_pShadowsBitmap[k+2]/(double)m_nbActiveFilters);
		}
	}
}

void CW3ZMapEditView::AddUnitsFilter(CRect rect)
{
	int i, j, k;
	int minx, maxx, miny, maxy;

	if (rect.TopLeft().x < rect.BottomRight().x)
	{
		minx = rect.TopLeft().x;
		maxx = rect.BottomRight().x;
	}
	else
	{
		minx = rect.BottomRight().x;
		maxx = rect.TopLeft().x;
	}
	if (minx < 0)
	{minx = 0;}
	if (maxx < 0)
	{maxx = 0;}
	if (minx >= m_pBitmapInfo->bV4Width)
	{minx = m_pBitmapInfo->bV4Width - 1;}
	if (maxx >= m_pBitmapInfo->bV4Width)
	{maxx = m_pBitmapInfo->bV4Width - 1;}

	
	if (rect.BottomRight().y > rect.TopLeft().y)
	{
		miny = m_pBitmapInfo->bV4Height - 1 - rect.BottomRight().y;
		maxy = m_pBitmapInfo->bV4Height - 1 - rect.TopLeft().y;
	}
	else
	{
		miny = m_pBitmapInfo->bV4Height - 1 - rect.TopLeft().y;
		maxy = m_pBitmapInfo->bV4Height - 1 - rect.BottomRight().y;
	}
	if (miny < 0)
	{miny = 0;}
	if (maxy < 0)
	{maxy = 0;}
	if (miny >= m_pBitmapInfo->bV4Height)
	{miny = m_pBitmapInfo->bV4Height - 1;}
	if (maxy >= m_pBitmapInfo->bV4Height)
	{maxy = m_pBitmapInfo->bV4Height - 1;}

	// copies only non-black pixels of the doodad bitmap to the main one
	for (j = miny;j <= maxy; j++)
	{
		for (i = minx;i <= maxx; i++)
		{
			k = i*3 + j*m_pBitmapInfo->bV4Width*3;
			if (m_pUnitsBitmap[k]
				|| m_pUnitsBitmap[k+1]
				|| m_pUnitsBitmap[k+2])
			{
				m_pMainBitmap[k] = m_pUnitsBitmap[k];
				m_pMainBitmap[k+1] = m_pUnitsBitmap[k+1];
				m_pMainBitmap[k+2] = m_pUnitsBitmap[k+2];
			}
		}
	}
}

void CW3ZMapEditView::AddPlayersFilter(CRect rect)
{

}

void CW3ZMapEditView::SetZoom(double zoom)
{
	CPoint pos;
	double oldx, oldy;
	CW3ZMapEditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pos = GetScrollPosition();
	oldx = pos.x/m_dZoom;
	oldy= pos.y/m_dZoom;
	if (( 0.1 < zoom) && (zoom < 10))
	{
		m_dZoom = zoom;
	}

	SetScrollSizes(MM_TEXT,CSize((int)((double)m_pBitmapInfo->bV4Width * m_dZoom),(int)((double)m_pBitmapInfo->bV4Height * m_dZoom)),
		CSize((int)((double)pDoc->m_War3Map.GetMapWidth() * m_dZoom),(int)((double)pDoc->m_War3Map.GetMapHeight() * m_dZoom)),
		CSize((int)(CR_WIDTH * m_dZoom),(int)(CR_HEIGHT * m_dZoom)));
	pos = CPoint((int)(oldx*m_dZoom), (int)(oldy*m_dZoom));
	ScrollToPosition(pos);
	Invalidate(FALSE);
}

double CW3ZMapEditView::GetZoom()
{
	return m_dZoom;
}

void CW3ZMapEditView::OnBtnZoomIn() 
{
	SetZoom(m_dZoom + 0.1);
}

void CW3ZMapEditView::OnBtnZoomNo() 
{
	SetZoom(1);
}

void CW3ZMapEditView::OnBtnZoomOut() 
{
	SetZoom(m_dZoom - 0.1);
}

BOOL CW3ZMapEditView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if (zDelta < 0)
	{ // Zoom in
		SetZoom(m_dZoom + 0.1);
	}
	else if (zDelta > 0)
	{ // zoom out
		SetZoom(m_dZoom - 0.1);
	}

	return FALSE;
//	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}

void CW3ZMapEditView::InvalidateDoodadsBitmap(CRect rect)
{
	int x, y, minx, maxx, miny, maxy, linelength;
	int mapHeight, mapWidth;
	float fx, fy;
	BYTE* dibits = m_pDoodadsBitmap;
	CDoodad doodad;

	CW3ZMapEditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	linelength = m_pBitmapInfo->bV4Width*3;
	mapWidth = (int)pDoc->m_War3Map.GetMapWidth() + 1;
	mapHeight = (int)pDoc->m_War3Map.GetMapHeight() + 1;

	if (rect.TopLeft().x < rect.BottomRight().x)
	{
		minx = rect.TopLeft().x / CR_WIDTH - 1;
		maxx = rect.BottomRight().x / CR_WIDTH + 1;
	}
	else
	{
		minx = rect.BottomRight().x  / CR_WIDTH - 1;
		maxx = rect.TopLeft().x  / CR_WIDTH + 1;
	}
	if (minx < 0)
	{minx = 0;}
	if (maxx < 0)
	{maxx = 0;}
	if (minx >= mapWidth)
	{minx = mapWidth - 1;}
	if (maxx >= mapWidth)
	{maxx = mapWidth - 1;}
	
	if (rect.BottomRight().y > rect.TopLeft().y)
	{
		miny = (m_pBitmapInfo->bV4Height - 1 - rect.BottomRight().y) / CR_HEIGHT - 1;
		maxy = (m_pBitmapInfo->bV4Height - 1 - rect.TopLeft().y) / CR_HEIGHT + 1;
	}
	else
	{
		miny = (m_pBitmapInfo->bV4Height - 1 - rect.TopLeft().y) / CR_HEIGHT - 1;
		maxy = (m_pBitmapInfo->bV4Height - 1 - rect.BottomRight().y) / CR_HEIGHT + 1;
	}
	if (miny < 0)
	{miny = 0;}
	if (maxy < 0)
	{maxy = 0;}
	if (miny >= mapHeight)
	{miny = mapHeight - 1;}
	if (maxy >= mapHeight)
	{maxy = mapHeight - 1;}


	// clear bitmap rect
	x = (maxx - minx)*CR_WIDTH*3; // x contains the byte length of the width of the rect
	for(y=miny*CR_HEIGHT; y < maxy*CR_HEIGHT; y++)
	{
		dibits = m_pDoodadsBitmap + y*linelength + (minx*CR_WIDTH*3);
		memset(dibits, 0x00, x);
	}

	for(y=miny; y <= maxy; y++)
	{
		for(x=minx; x <= maxx; x++)
		{
			fx = (float)(x * 128 + pDoc->m_War3Map.GetMapCenterX());
			fy = (float)(y * 128 + pDoc->m_War3Map.GetMapCenterY());

			pDoc->m_War3Map.GoMapDoodad(fx, fy);
			doodad = pDoc->m_War3Map.GetNextMapDoodad();
			while(doodad.type != "")
			{
				DisplayDoodad(doodad);
				doodad = pDoc->m_War3Map.GetNextMapDoodad();
			}
		}
	}
}


void CW3ZMapEditView::InvalidateTilesBitmap(CRect rect)
{
	int x, y, minx, maxx, miny, maxy;
	int mapHeight, mapWidth;
	CTilePoint tilepoint;

	CW3ZMapEditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	mapWidth = (int)pDoc->m_War3Map.GetMapWidth() + 1;
	mapHeight = (int)pDoc->m_War3Map.GetMapHeight() + 1;

	if (rect.TopLeft().x < rect.BottomRight().x)
	{
		minx = rect.TopLeft().x / CR_WIDTH - 1;
		maxx = rect.BottomRight().x / CR_WIDTH + 1;
	}
	else
	{
		minx = rect.BottomRight().x  / CR_WIDTH - 1;
		maxx = rect.TopLeft().x  / CR_WIDTH + 1;
	}
	if (minx < 0)
	{minx = 0;}
	if (maxx < 0)
	{maxx = 0;}
	if (minx >= mapWidth)
	{minx = mapWidth - 1;}
	if (maxx >= mapWidth)
	{maxx = mapWidth - 1;}
	
	if (rect.BottomRight().y > rect.TopLeft().y)
	{
		miny = (m_pBitmapInfo->bV4Height - 1 - rect.BottomRight().y) / CR_HEIGHT - 1;
		maxy = (m_pBitmapInfo->bV4Height - 1 - rect.TopLeft().y) / CR_HEIGHT + 1;
	}
	else
	{
		miny = (m_pBitmapInfo->bV4Height - 1 - rect.TopLeft().y) / CR_HEIGHT - 1;
		maxy = (m_pBitmapInfo->bV4Height - 1 - rect.BottomRight().y) / CR_HEIGHT + 1;
	}
	if (miny < 0)
	{miny = 0;}
	if (maxy < 0)
	{maxy = 0;}
	if (miny >= mapHeight)
	{miny = mapHeight - 1;}
	if (maxy >= mapHeight)
	{maxy = mapHeight - 1;}

	for(y=miny; y <= maxy; y++)
	{
		for(x=minx; x <= maxx; x++)
		{
			tilepoint = pDoc->m_War3Map.GetTilePoint(x,y);
			DisplayTilePoint(x,y,tilepoint);
		}
	}
}

void CW3ZMapEditView::InvalidateShadowsBitmap(CRect rect)
{
	int x, y, i, x2, y2, minx, maxx, miny, maxy, linelength;
	int mapHeight, mapWidth;
	byte *pBytemap;

	CW3ZMapEditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//path bitmap line length
	linelength = m_pBitmapInfo->bV4Width*3;
	// TGA bitmap width and height
	mapWidth = (int)pDoc->m_War3Map.GetShadowMapWidth();
	mapHeight = (int)pDoc->m_War3Map.GetShadowMapHeight();
	// check coordinates
	if (rect.TopLeft().x < rect.BottomRight().x)
	{
		minx = rect.TopLeft().x;
		maxx = rect.BottomRight().x;
	}
	else
	{
		minx = rect.BottomRight().x;
		maxx = rect.TopLeft().x;
	}
	if (minx < 0)
	{minx = 0;}
	if (maxx < 0)
	{maxx = 0;}
	if (minx >= m_pBitmapInfo->bV4Width)
	{minx = m_pBitmapInfo->bV4Width - 1;}
	if (maxx >= m_pBitmapInfo->bV4Width)
	{maxx = m_pBitmapInfo->bV4Width - 1;}
	
	if (rect.BottomRight().y > rect.TopLeft().y)
	{
		miny = m_pBitmapInfo->bV4Height - 1 - rect.BottomRight().y;
		maxy = m_pBitmapInfo->bV4Height - 1 - rect.TopLeft().y;
	}
	else
	{
		miny = m_pBitmapInfo->bV4Height - 1 - rect.TopLeft().y;
		maxy = m_pBitmapInfo->bV4Height - 1 - rect.BottomRight().y;
	}
	if (miny < 0)
	{miny = 0;}
	if (maxy < 0)
	{maxy = 0;}
	if (miny >= m_pBitmapInfo->bV4Height)
	{miny = m_pBitmapInfo->bV4Height - 1;}
	if (maxy >= m_pBitmapInfo->bV4Height)
	{maxy = m_pBitmapInfo->bV4Height - 1;}

	pBytemap = pDoc->m_War3Map.GetShadowMapPtr();

	for (y = miny; y <= maxy; y++)
	{
		for (x = minx;x <= maxx; x++)
		{
			x2 = (int)(x*mapWidth/(double)m_pBitmapInfo->bV4Width);
			y2 = (int)(y*mapHeight/(double)m_pBitmapInfo->bV4Height);
			i = (x2 + y2*mapWidth);
			m_pShadowsBitmap[x*3+y*linelength] = (byte)(0xFF - pBytemap[i]);
			m_pShadowsBitmap[x*3+y*linelength+1] = (byte)(0xFF - pBytemap[i]);
			m_pShadowsBitmap[x*3+y*linelength+2] = (byte)(0xFF - pBytemap[i]);
		}
	}
}

void CW3ZMapEditView::InvalidatePathBitmap(CRect rect)
{
	int x, y, i, x2, y2, minx, maxx, miny, maxy,linelength;
	int mapHeight, mapWidth, iPixDepth;
	BYTE *pBitmap, *pBytemap;
	BOOL bNoBuildSet, bNoWalkSet,bNoFlySet, bNoWaterSet;

	CW3ZMapEditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//path bitmap line length
	linelength = m_pBitmapInfo->bV4Width*3;
	// TGA bitmap width and height
	mapWidth = (int)pDoc->m_War3Map.mapTGAPath.GetWidth();
	mapHeight = (int)pDoc->m_War3Map.mapTGAPath.GetHeight();
	iPixDepth = 4;
	// check coordinates
	if (rect.TopLeft().x < rect.BottomRight().x)
	{
		minx = rect.TopLeft().x;
		maxx = rect.BottomRight().x;
	}
	else
	{
		minx = rect.BottomRight().x;
		maxx = rect.TopLeft().x;
	}
	if (minx < 0)
	{minx = 0;}
	if (maxx < 0)
	{maxx = 0;}
	if (minx >= m_pBitmapInfo->bV4Width)
	{minx = m_pBitmapInfo->bV4Width - 1;}
	if (maxx >= m_pBitmapInfo->bV4Width)
	{maxx = m_pBitmapInfo->bV4Width - 1;}
	
	if (rect.BottomRight().y > rect.TopLeft().y)
	{
		miny = m_pBitmapInfo->bV4Height - 1 - rect.BottomRight().y;
		maxy = m_pBitmapInfo->bV4Height - 1 - rect.TopLeft().y;
	}
	else
	{
		miny = m_pBitmapInfo->bV4Height - 1 - rect.TopLeft().y;
		maxy = m_pBitmapInfo->bV4Height - 1 - rect.BottomRight().y;
	}
	if (miny < 0)
	{miny = 0;}
	if (maxy < 0)
	{maxy = 0;}
	if (miny >= m_pBitmapInfo->bV4Height)
	{miny = m_pBitmapInfo->bV4Height - 1;}
	if (maxy >= m_pBitmapInfo->bV4Height)
	{maxy = m_pBitmapInfo->bV4Height - 1;}

	pBitmap = pDoc->m_War3Map.mapTGAPath.GetBitmapPtr();
	pBytemap = pDoc->m_War3Map.GetPathMapPtr();
	for (y = miny; y <= maxy; y++)
	{
		for (x = minx;x <= maxx; x++)
		{
			x2 = (int)(x*mapWidth/(double)m_pBitmapInfo->bV4Width);
			y2 = mapHeight - 1 - (int)(y*mapHeight/(double)m_pBitmapInfo->bV4Height);
			//PathTGA: i = (x2 + y2*mapWidth)*iPixDepth;
			i = x2 + (mapHeight - y2 - 1)*mapWidth;
			bNoBuildSet = bNoWalkSet = bNoFlySet = bNoWaterSet = FALSE;

			m_pPathBitmap[x*3+y*linelength] = m_pAppSettings->pathNormal.clrBlue;
			m_pPathBitmap[x*3+y*linelength+1] = m_pAppSettings->pathNormal.clrGreen;
			m_pPathBitmap[x*3+y*linelength+2] = m_pAppSettings->pathNormal.clrRed;

			if ((pBytemap[i] & PATH_NOBUILD)
				&& (pBytemap[i] & PATH_NOWALK)
				&& (pBytemap[i] & PATH_NOFLY))
			{
				if (IsModeActive(m_pAppSettings->pathNoBuildNoWalkNoFly.mode, x, y))
				{
					m_pPathBitmap[x*3+y*linelength] = m_pAppSettings->pathNoBuildNoWalkNoFly.clrBlue;
					m_pPathBitmap[x*3+y*linelength+1] = m_pAppSettings->pathNoBuildNoWalkNoFly.clrGreen;
					m_pPathBitmap[x*3+y*linelength+2] = m_pAppSettings->pathNoBuildNoWalkNoFly.clrRed;
					bNoBuildSet = bNoWalkSet = bNoFlySet = TRUE;
				}
			}
			else if ((pBytemap[i] & PATH_NOBUILD)
					&& (pBytemap[i] & PATH_NOWALK)
					&& (pBytemap[i] & PATH_NOWATER))
			{
				if (IsModeActive(m_pAppSettings->pathNoBuildNoWalkNoWater.mode, x, y))
				{
					m_pPathBitmap[x*3+y*linelength] = m_pAppSettings->pathNoBuildNoWalkNoWater.clrBlue;
					m_pPathBitmap[x*3+y*linelength+1] = m_pAppSettings->pathNoBuildNoWalkNoWater.clrGreen;
					m_pPathBitmap[x*3+y*linelength+2] = m_pAppSettings->pathNoBuildNoWalkNoWater.clrRed;
					bNoBuildSet = bNoWalkSet = bNoWaterSet = TRUE;
				}
			}
			else if ((pBytemap[i] & PATH_NOBUILD)
					&& (pBytemap[i] & PATH_NOWALK))
			{
				if (IsModeActive(m_pAppSettings->pathNoBuildNoWalk.mode, x, y))
				{
					m_pPathBitmap[x*3+y*linelength] = m_pAppSettings->pathNoBuildNoWalk.clrBlue;
					m_pPathBitmap[x*3+y*linelength+1] = m_pAppSettings->pathNoBuildNoWalk.clrGreen;
					m_pPathBitmap[x*3+y*linelength+2] = m_pAppSettings->pathNoBuildNoWalk.clrRed;
					bNoBuildSet = bNoWalkSet = TRUE;
				}
			}

			if (!bNoBuildSet
				&& (pBytemap[i] & PATH_NOBUILD)
				&& IsModeActive(m_pAppSettings->pathNoBuild.mode, x, y))
			{
				m_pPathBitmap[x*3+y*linelength] = m_pAppSettings->pathNoBuild.clrBlue;
				m_pPathBitmap[x*3+y*linelength+1] = m_pAppSettings->pathNoBuild.clrGreen;
				m_pPathBitmap[x*3+y*linelength+2] = m_pAppSettings->pathNoBuild.clrRed;
			}

			if (!bNoWalkSet
				&& (pBytemap[i] & PATH_NOWALK)
				&& IsModeActive(m_pAppSettings->pathNoWalk.mode, x, y))
			{
				m_pPathBitmap[x*3+y*linelength] = m_pAppSettings->pathNoWalk.clrBlue;
				m_pPathBitmap[x*3+y*linelength+1] = m_pAppSettings->pathNoWalk.clrGreen;
				m_pPathBitmap[x*3+y*linelength+2] = m_pAppSettings->pathNoWalk.clrRed;
			}

			if (!bNoFlySet
				&& (pBytemap[i] & PATH_NOFLY)
				&& IsModeActive(m_pAppSettings->pathNoFly.mode, x, y))
			{
				m_pPathBitmap[x*3+y*linelength] = m_pAppSettings->pathNoFly.clrBlue;
				m_pPathBitmap[x*3+y*linelength+1] = m_pAppSettings->pathNoFly.clrGreen;
				m_pPathBitmap[x*3+y*linelength+2] = m_pAppSettings->pathNoFly.clrRed;
			}
			
			if (!bNoWaterSet
				&& (pBytemap[i] & PATH_NOWATER)
				&& IsModeActive(m_pAppSettings->pathNoWater.mode, x, y))
			{
				m_pPathBitmap[x*3+y*linelength] = m_pAppSettings->pathNoWater.clrBlue;
				m_pPathBitmap[x*3+y*linelength+1] = m_pAppSettings->pathNoWater.clrGreen;
				m_pPathBitmap[x*3+y*linelength+2] = m_pAppSettings->pathNoWater.clrRed;
			}

			if ((pBytemap[i] & PATH_BLIGHT)
				&& IsModeActive(m_pAppSettings->pathBlight.mode, x, y))
			{
				m_pPathBitmap[x*3+y*linelength] = m_pAppSettings->pathBlight.clrBlue;
				m_pPathBitmap[x*3+y*linelength+1] = m_pAppSettings->pathBlight.clrGreen;
				m_pPathBitmap[x*3+y*linelength+2] = m_pAppSettings->pathBlight.clrRed;
			}
			
			if ((pBytemap[i] & PATH_UNKNOWN)
				&& IsModeActive(m_pAppSettings->pathUnknown.mode, x, y))
			{
				m_pPathBitmap[x*3+y*linelength] = m_pAppSettings->pathUnknown.clrBlue;
				m_pPathBitmap[x*3+y*linelength+1] = m_pAppSettings->pathUnknown.clrGreen;
				m_pPathBitmap[x*3+y*linelength+2] = m_pAppSettings->pathUnknown.clrRed;
			}

			if ((pBytemap[i] & PATH_UNKN01)
				&& IsModeActive(m_pAppSettings->pathUnkn01.mode, x, y))
			{
				m_pPathBitmap[x*3+y*linelength] = m_pAppSettings->pathUnkn01.clrBlue;
				m_pPathBitmap[x*3+y*linelength+1] = m_pAppSettings->pathUnkn01.clrGreen;
				m_pPathBitmap[x*3+y*linelength+2] = m_pAppSettings->pathUnkn01.clrRed;
			}

			if ((pBytemap[i] & PATH_UNKN02)
				&& IsModeActive(m_pAppSettings->pathUnkn02.mode, x, y))
			{
				m_pPathBitmap[x*3+y*linelength] = m_pAppSettings->pathUnkn02.clrBlue;
				m_pPathBitmap[x*3+y*linelength+1] = m_pAppSettings->pathUnkn02.clrGreen;
				m_pPathBitmap[x*3+y*linelength+2] = m_pAppSettings->pathUnkn02.clrRed;
			}

/*
			m_pPathBitmap[x*3+y*linelength] = (m_pAppSettings->pathNormal & 0x00FF0000)/0x00010000 ; //B
			m_pPathBitmap[x*3+y*linelength+1] = (m_pAppSettings->pathNormal & 0x0000FF00)/0x00000100; //G
			m_pPathBitmap[x*3+y*linelength+2] = (m_pAppSettings->pathNormal & 0x000000FF); //R

			if (!(pBytemap[] & PATH_NOWATER) && !((x+y)%4))
			{
				m_pPathBitmap[x*3+y*linelength] = 0x80;
				m_pPathBitmap[x*3+y*linelength+1] = 0x80;
				m_pPathBitmap[x*3+y*linelength+2] = 0;
			}
			if ((pBytemap[x2 + (mapHeight - y2 - 1)*mapWidth] & PATH_BLIGHT) && !((x-y+2)%4) && !((x+y+2)%4))
			{
				m_pPathBitmap[x*3+y*linelength] = 0x40;
				m_pPathBitmap[x*3+y*linelength+1] = 0xC0;
				m_pPathBitmap[x*3+y*linelength+2] = 0xFF;
			}
			if ((pBytemap[x2 + (mapHeight - y2 - 1)*mapWidth] & PATH_UNKNOWN) && !((x-y)%4))
			{
				m_pPathBitmap[x*3+y*linelength] = 0x80;
				m_pPathBitmap[x*3+y*linelength+1] = 0x80;
				m_pPathBitmap[x*3+y*linelength+2] = 0x80;
			}
			if ((pBytemap[x2 + (mapHeight - y2 - 1)*mapWidth] & PATH_UNKN01) && !((x-y)%4))
			{
				m_pPathBitmap[x*3+y*linelength] = 0x80;
				m_pPathBitmap[x*3+y*linelength+1] = 0x80;
				m_pPathBitmap[x*3+y*linelength+2] = 0x80;
			}
			if ((pBytemap[x2 + (mapHeight - y2 - 1)*mapWidth] & PATH_UNKN02) && !((x-y)%4))
			{
				m_pPathBitmap[x*3+y*linelength] = 0x80;
				m_pPathBitmap[x*3+y*linelength+1] = 0x80;
				m_pPathBitmap[x*3+y*linelength+2] = 0x80;
			}
*/
		}
	}
}

void CW3ZMapEditView::InvalidateUnitsBitmap(CRect rect)
{
/*
	int x, y, minx, maxx, miny, maxy, linelength;
	int mapHeight, mapWidth;
	float fx, fy;
	BYTE* dibits = m_pUnitsBitmap;
*/
	CUnit unit;

	CW3ZMapEditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

/*
	linelength = m_pBitmapInfo->bV4Width*3;
	mapWidth = (int)pDoc->m_War3Map.GetMapWidth() + 1;
	mapHeight = (int)pDoc->m_War3Map.GetMapHeight() + 1;

	if (rect.TopLeft().x < rect.BottomRight().x)
	{
		minx = rect.TopLeft().x / CR_WIDTH - 1;
		maxx = rect.BottomRight().x / CR_WIDTH + 1;
	}
	else
	{
		minx = rect.BottomRight().x  / CR_WIDTH - 1;
		maxx = rect.TopLeft().x  / CR_WIDTH + 1;
	}
	if (minx < 0)
	{minx = 0;}
	if (maxx < 0)
	{maxx = 0;}
	if (minx >= mapWidth)
	{minx = mapWidth - 1;}
	if (maxx >= mapWidth)
	{maxx = mapWidth - 1;}
	
	if (rect.BottomRight().y > rect.TopLeft().y)
	{
		miny = (m_pBitmapInfo->bV4Height - 1 - rect.BottomRight().y) / CR_HEIGHT - 1;
		maxy = (m_pBitmapInfo->bV4Height - 1 - rect.TopLeft().y) / CR_HEIGHT + 1;
	}
	else
	{
		miny = (m_pBitmapInfo->bV4Height - 1 - rect.TopLeft().y) / CR_HEIGHT - 1;
		maxy = (m_pBitmapInfo->bV4Height - 1 - rect.BottomRight().y) / CR_HEIGHT + 1;
	}
	if (miny < 0)
	{miny = 0;}
	if (maxy < 0)
	{maxy = 0;}
	if (miny >= mapHeight)
	{miny = mapHeight - 1;}
	if (maxy >= mapHeight)
	{maxy = mapHeight - 1;}


	// clear bitmap rect
	x = (maxx - minx)*CR_WIDTH*3; // x contains the byte length of the width of the rect
	for(y=miny*CR_HEIGHT; y < maxy*CR_HEIGHT; y++)
	{
		dibits = m_pUnitsBitmap + y*linelength + (minx*CR_WIDTH*3);
		memset(dibits, 0x00, x);
	}
*/
	pDoc->m_War3Map.GoFirstUnit();
	unit = pDoc->m_War3Map.GetNextUnit();
	while(unit.type != "")
	{
		DisplayUnit(unit);
		unit = pDoc->m_War3Map.GetNextUnit();
	}
}

void CW3ZMapEditView::InvalidatePlayersBitmap(CRect rect)
{

}

BOOL CW3ZMapEditView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (nHitTest == HTCLIENT)
	{
		CW3ZMapEditApp(* pTheApp) = (CW3ZMapEditApp *)AfxGetApp();
		pTheApp->m_pCurrentTool->SetCursor();
		return true;
	}
	else
	{
		return CScrollView::OnSetCursor(pWnd, nHitTest, message);
	}
}

void CW3ZMapEditView::UpdateTilesColors()
{
	int u, i;
	CString str;
	char tile[5];
	CW3ZMapEditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// initialize color pallette
	u = pDoc->m_War3Map.mapGroundTiles.GetLength() / 4;
	if (u > W3E_MAX_GROUNDTILES)
	{
		u = W3E_MAX_GROUNDTILES;
	}
	tile[4] = '\0';
	str = pDoc->m_War3Map.mapGroundTiles;
	for (i=0; i < W3E_MAX_GROUNDTILES;i++)
	{
		if (i < u)
		{
			tile[0] = str[i*4];//str.Mid(i*4, 4),4);
			tile[1] = str[i*4+1];
			tile[2] = str[i*4+2];
			tile[3] = str[i*4+3];
			m_TilesColors[i] = m_pAppSettings->GetTileColor(tile);
		}
		else
		{
			m_TilesColors[i] = m_pAppSettings->GetTileColor(TS_STR_ID_UNDEFINED);
		}
	}

	if (m_iViewFlags & VFL_TILES)
	{
		InvalidateBitmap();
		Invalidate();
	}

}

void CW3ZMapEditView::RemoveDoodad(CDoodad doodad)
{
	BYTE* dibits = m_pDoodadsBitmap;
	int x, y;

	CW3ZMapEditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	x = (int) ((doodad.x - pDoc->m_War3Map.GetMapCenterX()) * m_pBitmapInfo->bV4Width / (pDoc->m_War3Map.GetMapWidth()*128));
	y = (int) ((doodad.y - pDoc->m_War3Map.GetMapCenterY()) * m_pBitmapInfo->bV4Height / (pDoc->m_War3Map.GetMapHeight()*128));

	// teste la validite des coordonnees
	if ((x < 0) || (x > (signed)m_pBitmapInfo->bV4Width) || (y < 0) || (y > (signed)m_pBitmapInfo->bV4Height))
	{return;}

	BYTE B,R,V;
	B = 0;
	V = 0;
	R = 0;
	dibits = m_pDoodadsBitmap + (x)*3 + (y)*m_pBitmapInfo->bV4Width*3;
	*(dibits  )  = B; //B
	*(dibits+1)  = V; //V
	*(dibits+2)  = R; //R

	dibits = m_pDoodadsBitmap + (x + 1)*3 + (y)*m_pBitmapInfo->bV4Width*3;
	*(dibits  )  = B; //B
	*(dibits+1)  = V; //V
	*(dibits+2)  = R; //R

	dibits = m_pDoodadsBitmap + (x - 1)*3 + (y)*m_pBitmapInfo->bV4Width*3;
	*(dibits  )  = B; //B
	*(dibits+1)  = V; //V
	*(dibits+2)  = R; //R

	dibits = m_pDoodadsBitmap + (x)*3 + (y + 1)*m_pBitmapInfo->bV4Width*3;
	*(dibits  )  = B; //B
	*(dibits+1)  = V; //V
	*(dibits+2)  = R; //R

	dibits = m_pDoodadsBitmap + (x)*3 + (y - 1)*m_pBitmapInfo->bV4Width*3;
	*(dibits  )  = B; //B
	*(dibits+1)  = V; //V
	*(dibits+2)  = R; //R


}

void CW3ZMapEditView::ChangeBitmapsSize()
{
	int BMPsize	= 0;
	CW3ZMapEditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//create bitmap for display
	if (m_pImage)
	{
		delete[] m_pImage;
	}

	BMPsize = (pDoc->m_War3Map.GetMapWidth()*CR_WIDTH*pDoc->m_War3Map.GetMapHeight()*CR_HEIGHT*3);
	m_pImage = (unsigned char *) new char [sizeof(BITMAPV4HEADER) + BMPsize*6];
	m_pBitmapInfo = (BITMAPV4HEADER *) (m_pImage);
	m_pPalette = (RGBQUAD *)(m_pImage + sizeof(BITMAPV4HEADER));
	m_pMainBitmap = (m_pImage + sizeof (BITMAPV4HEADER));
	m_pTilesBitmap = (m_pMainBitmap + BMPsize);
	m_pDoodadsBitmap = (m_pTilesBitmap + BMPsize);
	m_pUnitsBitmap = (m_pDoodadsBitmap + BMPsize);
	m_pShadowsBitmap = (m_pUnitsBitmap + BMPsize);
	m_pPathBitmap = (m_pShadowsBitmap + BMPsize);


    m_pBitmapInfo->bV4Size = sizeof (BITMAPV4HEADER);
    m_pBitmapInfo->bV4Width = pDoc->m_War3Map.GetMapWidth()*CR_WIDTH;
    m_pBitmapInfo->bV4Height = pDoc->m_War3Map.GetMapHeight()*CR_HEIGHT;
    m_pBitmapInfo->bV4Planes = 1;
    m_pBitmapInfo->bV4BitCount = 24;
    m_pBitmapInfo->bV4V4Compression = BI_RGB;
    m_pBitmapInfo->bV4SizeImage = 0;
    m_pBitmapInfo->bV4XPelsPerMeter = 0;
    m_pBitmapInfo->bV4YPelsPerMeter = 0;
    m_pBitmapInfo->bV4ClrUsed = 0;
    m_pBitmapInfo->bV4ClrImportant = 0;
    m_pBitmapInfo->bV4RedMask = 0;
    m_pBitmapInfo->bV4GreenMask = 0;
    m_pBitmapInfo->bV4BlueMask = 0;
    m_pBitmapInfo->bV4AlphaMask = 0;
    m_pBitmapInfo->bV4CSType = LCS_sRGB;
    m_pBitmapInfo->bV4Endpoints = m_pCiexyzTriple;
    m_pBitmapInfo->bV4GammaRed = 0;
    m_pBitmapInfo->bV4GammaGreen = 0;
    m_pBitmapInfo->bV4GammaBlue = 0;

	memset(m_pMainBitmap, 0, BMPsize);
	memset(m_pTilesBitmap, 0, BMPsize);
	memset(m_pDoodadsBitmap, 0, BMPsize);
	memset(m_pUnitsBitmap, 0, BMPsize);
	memset(m_pShadowsBitmap, 0, BMPsize);
	memset(m_pPathBitmap, 0, BMPsize);

	InvalidateBitmap();

	SetScrollSizes(MM_TEXT,CSize((int)((double)m_pBitmapInfo->bV4Width * m_dZoom),(int)((double)m_pBitmapInfo->bV4Height * m_dZoom)),
		CSize((int)((double)pDoc->m_War3Map.GetMapWidth() * m_dZoom),(int)((double)pDoc->m_War3Map.GetMapHeight() * m_dZoom)),
		CSize((int)(CR_WIDTH * m_dZoom),(int)(CR_HEIGHT * m_dZoom)));
}

void CW3ZMapEditView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    CW3ZMapEditApp(* pTheApp) = (CW3ZMapEditApp *)AfxGetApp();
	if (pTheApp->m_pCurrentTool)
	{
	 pTheApp->m_pCurrentTool->OnKeyDown(this, nChar, nRepCnt, nFlags);
	}
	
	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

CPoint CW3ZMapEditView::GetScrollPoint(CPoint point)
{
	CPoint pt1;
	pt1 = point;
	pt1.x = (int)(pt1.x * m_dZoom);
	pt1.y = (int)(pt1.y * m_dZoom);
	pt1 -= GetScrollPosition();
	return pt1;
}


void CW3ZMapEditView::DisplayUnit(CUnit unit)
{
	BYTE* dibits = m_pUnitsBitmap;
	int x, y, i;

	CW3ZMapEditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	x = (int) ((unit.x - pDoc->m_War3Map.GetMapCenterX()) * m_pBitmapInfo->bV4Width / (pDoc->m_War3Map.GetMapWidth()*128));
	y = (int) ((unit.y - pDoc->m_War3Map.GetMapCenterY()) * m_pBitmapInfo->bV4Height / (pDoc->m_War3Map.GetMapHeight()*128));

	// teste la validite des coordonnees
	if ((x < 0) || (x > (signed)m_pBitmapInfo->bV4Width) || (y < 0) || (y > (signed)m_pBitmapInfo->bV4Height))
	{return;}

	BYTE B,V,R;
	if (!unit.type.Compare(UNIT_STR_ID_STARTPOS))
	{// start location
		B = 0;
		V = 0;
		R = 255;
	}
	else if (!unit.type.Compare(UNIT_STR_ID_GOLDMINE))
	{// goldmine
		B = 0;
		V = 255;
		R = 255;
	}
	else
	{// other units
		B = 192;
		V = 255;
		R = 255;
	}
	for (i =0; i < 3; i++)
	{
		if (((x -1 +i) >= 0)
			&& ((x -1 +i) < m_pBitmapInfo->bV4Width)
			&& ((y-2) >= 0))
		{
			dibits = m_pUnitsBitmap + (x -1 +i)*3 + (y-2)*m_pBitmapInfo->bV4Width*3;
			*(dibits  )  = B; //B
			*(dibits+1)  = V; //V
			*(dibits+2)  = R; //R
		}

		if (((x -1 +i) >= 0)
			&& ((x -1 +i) < m_pBitmapInfo->bV4Width)
			&& ((y+2) < m_pBitmapInfo->bV4Height))
		{
			dibits = m_pUnitsBitmap + (x -1 +i)*3 + (y+2)*m_pBitmapInfo->bV4Width*3;
			*(dibits  )  = B; //B
			*(dibits+1)  = V; //V
			*(dibits+2)  = R; //R
		}

		if (((y -1 +i) >= 0)
			&& ((y -1 +i) < m_pBitmapInfo->bV4Height)
			&& ((x-2) >= 0))
		{
			dibits = m_pUnitsBitmap + (x-2)*3 + (y -1 +i)*m_pBitmapInfo->bV4Width*3;
			*(dibits  )  = B; //B
			*(dibits+1)  = V; //V
			*(dibits+2)  = R; //R
		}

		if (((y -1 +i) >= 0)
			&& ((y -1 +i) < m_pBitmapInfo->bV4Height)
			&& ((x+2) < m_pBitmapInfo->bV4Width))
		{
			dibits = m_pUnitsBitmap + (x+2)*3 + (y -1 +i)*m_pBitmapInfo->bV4Width*3;
			*(dibits  )  = B; //B
			*(dibits+1)  = V; //V
			*(dibits+2)  = R; //R
		}
	}
}

BOOL CW3ZMapEditView::IsModeActive(int mode, int x, int y)
{
	BOOL ret = TRUE;

/*
const int PATH_NOTHING	= -1;// nothing
const int PATH_PLAIN	= 0;// plain color
const int PATH_STRIPED1	= 1;// striped //  ((x-y)%4)
const int PATH_STRIPED2	= 2;// striped \\  ((x+y)%4)
const int PATH_STRIPED3	= 3;// striped ==  (y%4)
const int PATH_STRIPED4	= 4;// striped ||  (x%4)
const int PATH_STRIPED5	= 5;// striped ##  (((x+2)%4) && ((y+2)%4))
const int PATH_DOTS1	= 6;// dots ..  (((x+1)%4) || ((y+1)%4))
const int PATH_DOTS2	= 7;// more dots :: (((x+3)%2) || ((y+3)%2))
*/
	if (mode == PATH_PLAIN)
	{ret = TRUE;}
	else if (mode == PATH_NOTHING)
	{ret = FALSE;}
	else if (mode == PATH_STRIPED1)
	{
		if ((x-y)%4)
		{ret = FALSE;}
	}
	else if (mode == PATH_STRIPED2)
	{
		if ((x+y)%4)
		{ret = FALSE;}
	}
	else if (mode == PATH_STRIPED3)
	{
		if (y%4)
		{ret = FALSE;}
	}
	else if (mode == PATH_STRIPED4)
	{
		if (x%4)
		{ret = FALSE;}
	}
	else if (mode == PATH_STRIPED5)
	{
		if (((x+2)%4) && ((y+2)%4))
		{ret = FALSE;}
	}
	else if (mode == PATH_DOTS1)
	{
		if (((x+1)%4) || ((y+1)%4))
		{ret = FALSE;}
	}
	else if (mode == PATH_DOTS2)
	{
		if (((x)%2) || ((y)%2))
		{ret = FALSE;}
	}


	return ret;
}
