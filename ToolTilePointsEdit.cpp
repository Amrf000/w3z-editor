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
// ToolTilePointsEdit.cpp: implementation of the CToolTilePointsEdit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "W3ZMapEdit.h"
#include "W3ZMapEditDoc.h"
#include "W3ZMapEditView.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "ToolTilePointsEdit.h"
#include "ListedPoint.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToolTilePointsEdit::CToolTilePointsEdit()
{
	ground = 255;
	m_iSize = TOOL_TILE_DEFAULT_SIZE;
	m_ptFillMin.x = 0;
	m_ptFillMin.y = 0;
	m_ptFillMax.x = 0;
	m_ptFillMax.y = 0;
}


CToolTilePointsEdit::~CToolTilePointsEdit()
{
}


void CToolTilePointsEdit::OnRButtonDown(CW3ZMapEditView *pView, int nFlags, CPoint point)
{
	CTool::OnRButtonDown(pView, nFlags, point);

	//m_bRightTracking is true if parent call (OnRButtonDown) did well
	if (m_bRightTracking == TRUE) // prenvents conflicts with right and left mouse buttons
	{
		CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pView->GetDocument();
		ASSERT_VALID(pDoc);
		CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
		ASSERT_VALID(pTheApp);
		CMainFrame *pMainFrame = NULL;
		CTilePoint tilepoint;

		int x,y;
		if ((point.x < pView->m_pBitmapInfo->bV4Width) && (0 <= point.x) && (point.y < pView->m_pBitmapInfo->bV4Height) && (0 <= point.y))
		{
			x = (point.x + CR_WIDTH/2) / CR_WIDTH;
			y = (point.y + CR_HEIGHT/2) / CR_HEIGHT;
			if (pDoc && pTheApp)
			{
				tilepoint = pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y);
				pMainFrame = pTheApp->GetMainFrame();
			}
			if (pMainFrame) //NULL if pMainFrame wasn't found
			{
				pMainFrame->UpdateTilesEditBar(tilepoint);
			}
		}
		m_ptFirst = m_ptLast = point;
	}

}


void CToolTilePointsEdit::OnLButtonDown(CW3ZMapEditView *pView, int nFlags, CPoint point)
{
	CTool::OnLButtonDown(pView, nFlags, point);

	//m_bLeftTracking is true parent call (OnRButtonDown) did well
	if (m_bLeftTracking == TRUE)
	{
		CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pView->GetDocument();
		ASSERT_VALID(pDoc);
		CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
		ASSERT_VALID(pTheApp);
		CTilePoint tilepoint;

		int x,y;
		m_ptFirst = m_ptLast = point;

		// check bitmap coordinates
		if ((point.x < pView->m_pBitmapInfo->bV4Width) && (0 <= point.x) && (point.y < pView->m_pBitmapInfo->bV4Height) && (0 <= point.y))
		{
			// computes tilepoint coordinates
			x = (point.x + CR_WIDTH/2) / CR_WIDTH;
			y = (point.y + CR_HEIGHT/2) / CR_HEIGHT;
			if (pDoc && pTheApp)
			{
				pDoc->OpenUndoObj();
				if (TOOL_MODE_TILE_SELECT == m_iToolMode)
				{
				}
				else if (TOOL_MODE_TILE_PEN == m_iToolMode)
				{
					TileWrite(pTheApp, pDoc, pView,x, y);
					CRect rect(point, point);
					rect.NormalizeRect();
					rect.InflateRect(CR_WIDTH*(1 + m_iSize%2 + m_iSize/2),CR_HEIGHT*(1 + m_iSize%2 + m_iSize/2));
					pView->InvalidateTilesBitmap(rect);
					pView->InvalidateBitmapRect(rect);
				}
				else if (TOOL_MODE_TILE_RECT == m_iToolMode)
				{
				}
				else if (TOOL_MODE_TILE_ELLIPSE == m_iToolMode)
				{
				}
				else if (TOOL_MODE_TILE_FILL == m_iToolMode)
				{
					m_ptFillMin = m_ptFillMax = CPoint(x, y);
					pDoc->SetModifiedFlag();
					TileFill(pTheApp, pDoc, pView,x, y);
					m_ptFillMin.x *= CR_WIDTH;
					m_ptFillMin.y *= CR_HEIGHT;
					m_ptFillMax.x *= CR_WIDTH;
					m_ptFillMax.y *= CR_HEIGHT;
					CRect rect(m_ptFillMin, m_ptFillMax);
					rect.NormalizeRect();
					rect.InflateRect(CR_WIDTH*(1 + m_iSize%2 + m_iSize/2),CR_HEIGHT*(1 + m_iSize%2 + m_iSize/2));
					pView->InvalidateTilesBitmap(rect);
					pView->InvalidateBitmapRect(rect);					
				}
				else if (TOOL_MODE_TILE_REPLACE == m_iToolMode)
				{
					SetGround(pTheApp, pDoc, pView,x, y);
					ReplaceTool(pTheApp, pDoc, pView,x, y);
					CRect rect(point, point);
					rect.NormalizeRect();
					rect.InflateRect(CR_WIDTH*(1 + m_iSize%2 + m_iSize/2),CR_HEIGHT*(1 + m_iSize%2 + m_iSize/2));
					pView->InvalidateTilesBitmap(rect);
					pView->InvalidateBitmapRect(rect);
				}
			}
		}
		if (TOOL_MODE_TILE_RECT == m_iToolMode)
		{
			CClientDC dc(pView);
			int oldrop = dc.SetROP2(R2_NOTXORPEN);
			CRect rect(pView->GetScrollPoint(m_ptFirst), pView->GetScrollPoint(point));
			rect.NormalizeRect();
			rect.right++;
			rect.bottom++;
			dc.Rectangle(rect);
			dc.SetROP2(oldrop);
		}
		else if (TOOL_MODE_TILE_ELLIPSE == m_iToolMode)
		{
			CClientDC dc(pView);
			int oldrop = dc.SetROP2(R2_NOTXORPEN);
			CRect rect(pView->GetScrollPoint(m_ptFirst), pView->GetScrollPoint(point));
			rect.NormalizeRect();
			rect.right++;
			rect.bottom++;
			dc.Ellipse(rect);
			dc.SetROP2(oldrop);
		}
	}
}


void CToolTilePointsEdit::OnMouseMove(CW3ZMapEditView *pView, int nFlags, CPoint point)
{
	CTool::OnMouseMove(pView, nFlags, point);

	if (m_bRightTracking == TRUE)
	{
		CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pView->GetDocument();
		ASSERT_VALID(pDoc);
		CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
		ASSERT_VALID(pTheApp);
		CMainFrame *pMainFrame = NULL;
		CTilePoint tilepoint;

		int x,y;
		if ((point.x < pView->m_pBitmapInfo->bV4Width) && (0 <= point.x) && (point.y < pView->m_pBitmapInfo->bV4Height) && (0 <= point.y))
		{
			x = (point.x + CR_WIDTH/2) / CR_WIDTH;
			y = (point.y + CR_HEIGHT/2) / CR_HEIGHT;
			if (pDoc && pTheApp)
			{
				tilepoint = pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y);
				pMainFrame = pTheApp->GetMainFrame();
			}
			if (pMainFrame) //NULL if pMainFrame wasn't found
			{
				pMainFrame->UpdateTilesEditBar(tilepoint);
			}
		}
		m_ptLast = point;
	}
	else if (m_bLeftTracking == TRUE)
	{
		CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pView->GetDocument();
		ASSERT_VALID(pDoc);
		CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
		ASSERT_VALID(pTheApp);
		CTilePoint tilepoint;

		int x,y;
		if ((point.x < pView->m_pBitmapInfo->bV4Width) && (0 <= point.x) && (point.y < pView->m_pBitmapInfo->bV4Height) && (0 <= point.y))
		{
			x = (point.x + CR_WIDTH/2) / CR_WIDTH;
			y = (point.y + CR_HEIGHT/2) / CR_HEIGHT;
			if (pDoc && pTheApp)
			{
				if (TOOL_MODE_TILE_SELECT == m_iToolMode)
				{
				}
				else if (TOOL_MODE_TILE_PEN == m_iToolMode)
				{
					TileWrite(pTheApp, pDoc, pView,x, y);
					ReplaceTool(pTheApp, pDoc, pView,x, y);
					CRect rect(m_ptLast, point);
					rect.NormalizeRect();
					rect.InflateRect(CR_WIDTH*(1 + m_iSize%2 + m_iSize/2),CR_HEIGHT*(1 + m_iSize%2 + m_iSize/2));
					pView->InvalidateTilesBitmap(rect);
					pView->InvalidateBitmapRect(rect);
				}
				else if (TOOL_MODE_TILE_RECT == m_iToolMode)
				{
				}
				else if (TOOL_MODE_TILE_ELLIPSE == m_iToolMode)
				{
				}
				else if (TOOL_MODE_TILE_FILL == m_iToolMode)
				{
				}
				else if (TOOL_MODE_TILE_REPLACE == m_iToolMode)
				{
					ReplaceTool(pTheApp, pDoc, pView,x, y);
					CRect rect(m_ptLast, point);
					rect.NormalizeRect();
					rect.InflateRect(CR_WIDTH*(1 + m_iSize%2 + m_iSize/2),CR_HEIGHT*(1 + m_iSize%2 + m_iSize/2));
					pView->InvalidateTilesBitmap(rect);
					pView->InvalidateBitmapRect(rect);
				}
			}
		}
		if (TOOL_MODE_TILE_RECT == m_iToolMode)
		{
			CClientDC dc(pView);
			int oldrop = dc.SetROP2(R2_NOTXORPEN);  
			CRect rect(pView->GetScrollPoint(m_ptFirst), pView->GetScrollPoint(m_ptLast));
			rect.NormalizeRect();
			rect.right++;
			rect.bottom++;
			dc.Rectangle(rect);

			rect.SetRect(pView->GetScrollPoint(m_ptFirst), pView->GetScrollPoint(point));
			rect.NormalizeRect();
			rect.right++;
			rect.bottom++;
			dc.Rectangle(rect);
			dc.SetROP2(oldrop);
		}
		else if (TOOL_MODE_TILE_ELLIPSE == m_iToolMode)
		{
			CClientDC dc(pView);
			int oldrop = dc.SetROP2(R2_NOTXORPEN);  
			CRect rect(pView->GetScrollPoint(m_ptFirst), pView->GetScrollPoint(m_ptLast));
			rect.NormalizeRect();
			rect.right++;
			rect.bottom++;
			dc.Ellipse(rect);

			rect.SetRect(pView->GetScrollPoint(m_ptFirst), pView->GetScrollPoint(point));
			rect.NormalizeRect();
			rect.right++;
			rect.bottom++;
			dc.Ellipse(rect);
			dc.SetROP2(oldrop);
		}
		m_ptLast = point;
	}

}


void CToolTilePointsEdit::OnRButtonUp(CW3ZMapEditView *pView, int nFlags, CPoint point)
{
	if (m_bRightTracking == TRUE)
	{
		CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pView->GetDocument();
		ASSERT_VALID(pDoc);
		CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
		ASSERT_VALID(pTheApp);
		CMainFrame *pMainFrame = NULL;
		CTilePoint tilepoint;

		int x,y;
		if ((point.x < pView->m_pBitmapInfo->bV4Width) && (0 <= point.x) && (point.y < pView->m_pBitmapInfo->bV4Height) && (0 <= point.y))
		{
			x = (point.x + CR_WIDTH/2) / CR_WIDTH;
			y = (point.y + CR_HEIGHT/2) / CR_HEIGHT;
			if (pDoc && pTheApp)
			{
				tilepoint = pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y);
				pMainFrame = pTheApp->GetMainFrame();
			}
			if (pMainFrame) //NULL si pMainFrame n'a pas ete trouve
			{
				pMainFrame->UpdateTilesEditBar(tilepoint);
			}
		}
		m_ptLast = point;
	}
	CTool::OnRButtonUp(pView, nFlags, point);
}


void CToolTilePointsEdit::OnLButtonUp(CW3ZMapEditView *pView, int nFlags, CPoint point)
{
	if (m_bLeftTracking == TRUE)
	{
		CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pView->GetDocument();
		ASSERT_VALID(pDoc);
		CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
		ASSERT_VALID(pTheApp);
		CTilePoint tilepoint;

		int x,y;

		if (TOOL_MODE_TILE_RECT == m_iToolMode)
		{
			CClientDC dc(pView);
			int oldrop = dc.SetROP2(R2_NOTXORPEN);  
			CRect rect(pView->GetScrollPoint(m_ptFirst), pView->GetScrollPoint(m_ptLast));
			rect.NormalizeRect();
			rect.right++;
			rect.bottom++;
			dc.Rectangle(rect);
			dc.SetROP2(oldrop);
		}
		else if (TOOL_MODE_TILE_ELLIPSE == m_iToolMode)
		{
			CClientDC dc(pView);
			int oldrop = dc.SetROP2(R2_NOTXORPEN);  
			CRect rect(pView->GetScrollPoint(m_ptFirst), pView->GetScrollPoint(m_ptLast));
			rect.NormalizeRect();
			rect.right++;
			rect.bottom++;
			dc.Ellipse(rect);
			dc.SetROP2(oldrop);
		}

		if ((point.x < pView->m_pBitmapInfo->bV4Width) && (0 <= point.x)&& (point.y < pView->m_pBitmapInfo->bV4Height) && (0 <= point.y))
		{
			x = (point.x + CR_WIDTH/2) / CR_WIDTH;
			y = (point.y + CR_HEIGHT/2) / CR_HEIGHT;
			if (pDoc && pTheApp)
			{
				if (TOOL_MODE_TILE_SELECT == m_iToolMode)
				{
					SelectTool(pTheApp, pDoc, pView,x, y);
					CRect rect(point, point);
					rect.NormalizeRect();
					rect.InflateRect(CR_WIDTH*(1 + m_iSize%2 + m_iSize/2),CR_HEIGHT*(1 + m_iSize%2 + m_iSize/2));
					pView->InvalidateTilesBitmap(rect);
					pView->InvalidateBitmapRect(rect);
				}
				else if (TOOL_MODE_TILE_PEN == m_iToolMode)
				{
					TileWrite(pTheApp, pDoc, pView,x, y);
					CRect rect(m_ptLast, point);
					rect.NormalizeRect();
					rect.InflateRect(CR_WIDTH*(1 + m_iSize%2 + m_iSize/2),CR_HEIGHT*(1 + m_iSize%2 + m_iSize/2));
					pView->InvalidateTilesBitmap(rect);
					pView->InvalidateBitmapRect(rect);
				}
				else if (TOOL_MODE_TILE_RECT == m_iToolMode)
				{
					int minx, maxx, miny, maxy;
					if (point.x <= m_ptFirst.x)
					{
						minx = (point.x + CR_WIDTH/2) / CR_WIDTH;
						maxx = (m_ptFirst.x + CR_WIDTH/2) / CR_WIDTH;
					}
					else
					{
						minx = (m_ptFirst.x + CR_WIDTH/2) / CR_WIDTH;
						maxx = (point.x + CR_WIDTH/2) / CR_WIDTH;
					}
					if (point.y <= m_ptFirst.y)
					{
						miny = (point.y + CR_HEIGHT/2) / CR_HEIGHT;
						maxy = (m_ptFirst.y + CR_HEIGHT/2) / CR_HEIGHT;
					}
					else
					{
						miny = (m_ptFirst.y + CR_HEIGHT/2) / CR_HEIGHT;
						maxy = (point.y + CR_HEIGHT/2) / CR_HEIGHT;
					}
					for (y = miny; y <= maxy; y++)
					{
						TileWrite(pTheApp, pDoc, pView, minx, y);
						TileWrite(pTheApp, pDoc, pView, maxx, y);
					}
					for (x = minx; x <= maxx; x++)
					{
						TileWrite(pTheApp, pDoc, pView, x, miny);
						TileWrite(pTheApp, pDoc, pView, x, maxy);
					}
					CRect rect(m_ptFirst, point);
					rect.NormalizeRect();
					rect.InflateRect(CR_WIDTH*(1 + m_iSize%2 + m_iSize/2),CR_HEIGHT*(1 + m_iSize%2 + m_iSize/2));
					pView->InvalidateTilesBitmap(rect);
					pView->InvalidateBitmapRect(rect);
				}
				else if (TOOL_MODE_TILE_ELLIPSE == m_iToolMode)
				{
					int minx, maxx, miny, maxy, a, b, ofsx, ofsy;
					if (point.x <= m_ptFirst.x)
					{
						minx = (point.x + CR_WIDTH/2) / CR_WIDTH;
						maxx = (m_ptFirst.x + CR_WIDTH/2) / CR_WIDTH;
					}
					else
					{
						minx = (m_ptFirst.x + CR_WIDTH/2) / CR_WIDTH;
						maxx = (point.x + CR_WIDTH/2) / CR_WIDTH;
					}
					if (point.y <= m_ptFirst.y)
					{
						miny = (point.y + CR_HEIGHT/2) / CR_HEIGHT;
						maxy = (m_ptFirst.y + CR_HEIGHT/2) / CR_HEIGHT;
					}
					else
					{
						miny = (m_ptFirst.y + CR_HEIGHT/2) / CR_HEIGHT;
						maxy = (point.y + CR_HEIGHT/2) / CR_HEIGHT;
					}
					//x = a*cos(t)
					//y = b*sin(t)
					//x²/a² + y²/b² = 1
					//y= sqrt((1 - x²/a²)*b²)
					a = (maxx - minx)/2;
					b = (maxy - miny)/2;
					ofsx = (maxx + minx) / 2;
					ofsy = (maxy + miny) / 2;
					minx = 0;
					maxx = a;//(int)(a*sqrt(2)/2 + 0.5);
					for (x = minx; x <= maxx; x++)
					{
						y = (int)(sqrt((1 - x*x/double(a*a))*b*b) + 0.5);
						TileWrite(pTheApp, pDoc, pView, ofsx + x, ofsy + y);
						TileWrite(pTheApp, pDoc, pView, ofsx + x, ofsy - y);
						TileWrite(pTheApp, pDoc, pView, ofsx - x, ofsy + y);
						TileWrite(pTheApp, pDoc, pView, ofsx - x, ofsy - y);
					}
					miny = 0;
					maxy = b;//(int)(b*sqrt(2)/2 + 0.5);
					for (y = miny; y <= maxy; y++)
					{
						x = (int)(sqrt((1 - y*y/double(b*b))*a*a) + 0.5);
						TileWrite(pTheApp, pDoc, pView, ofsx + x, ofsy + y);
						TileWrite(pTheApp, pDoc, pView, ofsx + x, ofsy - y);
						TileWrite(pTheApp, pDoc, pView, ofsx - x, ofsy + y);
						TileWrite(pTheApp, pDoc, pView, ofsx - x, ofsy - y);
					}
					CRect rect(m_ptFirst, point);
					rect.NormalizeRect();
					rect.InflateRect(CR_WIDTH*(1 + m_iSize%2 + m_iSize/2),CR_HEIGHT*(1 + m_iSize%2 + m_iSize/2));
					pView->InvalidateTilesBitmap(rect);
					pView->InvalidateBitmapRect(rect);
				}
				else if (TOOL_MODE_TILE_FILL == m_iToolMode)
				{
				}
				else if (TOOL_MODE_TILE_REPLACE == m_iToolMode)
				{
					ReplaceTool(pTheApp, pDoc, pView,x, y);
					ground = 255;
					CRect rect(m_ptLast, point);
					rect.NormalizeRect();
					rect.InflateRect(CR_WIDTH*(1 + m_iSize%2 + m_iSize/2),CR_HEIGHT*(1 + m_iSize%2 + m_iSize/2));
					pView->InvalidateTilesBitmap(rect);
					pView->InvalidateBitmapRect(rect);
				}
			}
		}
		if (pDoc)
		{pDoc->CloseUndoObj();}
		m_ptLast = point;
	}
	CTool::OnLButtonUp(pView, nFlags, point);
}

void CToolTilePointsEdit::SetCursor()
{
	if (TOOL_MODE_TILE_SELECT == m_iToolMode)
	{
		::SetCursor(AfxGetApp()->LoadCursor(IDC_TILEPOINTS_SEL));
	}
	else if (TOOL_MODE_TILE_PEN == m_iToolMode)
	{
		::SetCursor(AfxGetApp()->LoadCursor(IDC_TILEPOINTS));
	}
	else if (TOOL_MODE_TILE_RECT == m_iToolMode)
	{
		::SetCursor(AfxGetApp()->LoadCursor(IDC_TILEPOINTS));
	}
	else if (TOOL_MODE_TILE_ELLIPSE == m_iToolMode)
	{
		::SetCursor(AfxGetApp()->LoadCursor(IDC_TILEPOINTS));
	}
	else if (TOOL_MODE_TILE_FILL == m_iToolMode)
	{
		::SetCursor(AfxGetApp()->LoadCursor(IDC_TILEPOINTS_FILL));
	}
	else if (TOOL_MODE_TILE_REPLACE == m_iToolMode)
	{
		::SetCursor(AfxGetApp()->LoadCursor(IDC_TILEPOINTS_REPLACE));
	}
	else
	{
		::SetCursor(AfxGetApp()->LoadCursor(IDC_TILEPOINTS));
	}
}


void CToolTilePointsEdit::TileWrite(CW3ZMapEditApp *pTheApp, CW3ZMapEditDoc *pDoc, CW3ZMapEditView *pView, int x, int y)
{
	CMainFrame *pMainFrame = NULL;
	CTilePoint tilepoint;
	CTilePoint undoTilePoint;
	int x0,y0, minx, maxx, miny, maxy;
	minx = x - m_iSize/2;
	maxx = x + m_iSize%2 + m_iSize/2;
	miny = y - m_iSize/2;
	maxy = y + m_iSize%2 + m_iSize/2;
	if (minx < 0)
	{minx = 0;}
	if (maxx > pDoc->m_War3Map.GetMapWidth() + 1)
	{maxx = pDoc->m_War3Map.GetMapWidth() + 1;}
	if (miny < 0)
	{miny = 0;}
	if (maxy > pDoc->m_War3Map.GetMapHeight() + 1)
	{maxy = pDoc->m_War3Map.GetMapHeight() + 1;}

	for (y0 = miny; y0 < maxy; y0++)
	for (x0 = minx; x0 < maxx; x0++)
	{
		undoTilePoint = tilepoint = pDoc->m_War3Map.GetTilePoint(x0,pDoc->m_War3Map.GetMapHeight() - y0);
		if (!pDoc->m_War3Map.SelectedTilePointsOnly || (pDoc->m_War3Map.SelectedTilePointsOnly && (tilepoint.selected)))
		{
			pMainFrame = pTheApp->GetMainFrame();
			if (pMainFrame) //NULL if pMainFrame wasn't found
			{
				tilepoint = pMainFrame->GetTilesEditBarNewTilePoint(tilepoint);
			}
			// stores old tilepoint
			pDoc->AddToUndoObj(undoTilePoint, x0, pDoc->m_War3Map.GetMapHeight() - y0);
			// writes new tilepoint
			pDoc->m_War3Map.SetTilePoint(x0, pDoc->m_War3Map.GetMapHeight() - y0, tilepoint);
			pDoc->SetModifiedFlag();
			pView->DisplayTilePoint(x0, pDoc->m_War3Map.GetMapHeight() - y0, tilepoint);
		}
	}
}


void CToolTilePointsEdit::SelectTool(CW3ZMapEditApp *pTheApp, CW3ZMapEditDoc *pDoc, CW3ZMapEditView *pView, int x, int y)
{
	CTilePoint tilepoint;
	int minx, maxx, miny, maxy;
	CRect rect;

	tilepoint = pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y);
	if (0 == pDoc->m_War3Map.mapTilePointSelState)
	{
		pDoc->m_War3Map.mapTilePointSelFirst.x = x;
		pDoc->m_War3Map.mapTilePointSelFirst.y = y;
		pDoc->m_War3Map.mapTilePointSelState = 1;
		tilepoint.viewcolor.rgbRed = 255;
		tilepoint.viewcolor.rgbGreen = 0;
		tilepoint.viewcolor.rgbBlue = 0;
		tilepoint.viewcolor.rgbReserved = 255;
		tilepoint.selected = true;
		pDoc->m_War3Map.SetTilePoint(x, pDoc->m_War3Map.GetMapHeight() - y, tilepoint);
		pView->DisplayTilePoint(x, pDoc->m_War3Map.GetMapHeight() - y, tilepoint);
	}
	else if (1 == pDoc->m_War3Map.mapTilePointSelState)
	{
		pDoc->m_War3Map.mapTilePointSelSecond.x = x;
		pDoc->m_War3Map.mapTilePointSelSecond.y = y;
		pDoc->m_War3Map.mapTilePointSelState = 2;
		tilepoint.viewcolor.rgbRed = 255;
		tilepoint.viewcolor.rgbGreen = 0;
		tilepoint.viewcolor.rgbBlue = 0;
		tilepoint.viewcolor.rgbReserved = 255;
		tilepoint.selected = true;
		pDoc->m_War3Map.SetTilePoint(x, pDoc->m_War3Map.GetMapHeight() - y, tilepoint);
		pView->DisplayTilePoint(x, pDoc->m_War3Map.GetMapHeight() - y, tilepoint);
	}
	else if (2 == pDoc->m_War3Map.mapTilePointSelState)
	{
		pDoc->m_War3Map.mapTilePointSelThird.x = x;
		pDoc->m_War3Map.mapTilePointSelThird.y = y;
		pDoc->m_War3Map.mapTilePointSelState = 3;
		tilepoint.viewcolor.rgbRed = 0;
		tilepoint.viewcolor.rgbGreen = 0;
		tilepoint.viewcolor.rgbBlue = 255;
		tilepoint.viewcolor.rgbReserved = 255;
		tilepoint.selected = true;
		pDoc->m_War3Map.SetTilePoint(x, pDoc->m_War3Map.GetMapHeight() - y, tilepoint);
		pView->DisplayTilePoint(x, pDoc->m_War3Map.GetMapHeight() - y, tilepoint);
	}
	else if (3 == pDoc->m_War3Map.mapTilePointSelState)
	{
		CWaitCursor wait;
		pDoc->m_War3Map.mapTilePointSelForth.x = x;
		pDoc->m_War3Map.mapTilePointSelForth.y = y;
		pDoc->m_War3Map.mapTilePointSelState = 4;
		tilepoint.viewcolor.rgbRed = 0;
		tilepoint.viewcolor.rgbGreen = 0;
		tilepoint.viewcolor.rgbBlue = 255;
		tilepoint.viewcolor.rgbReserved = 255;
		tilepoint.selected = true;
		pDoc->m_War3Map.SetTilePoint(x, pDoc->m_War3Map.GetMapHeight() - y, tilepoint);
		pDoc->m_War3Map.SelectedTilePointsOnly = true;
		pDoc->m_War3Map.SelectTilePoints(pDoc->m_War3Map.mapTilePointSelFirst, pDoc->m_War3Map.mapTilePointSelSecond, pDoc->m_War3Map.mapTilePointSelThird, pDoc->m_War3Map.mapTilePointSelForth);

		minx = pDoc->m_War3Map.mapTilePointSelFirst.x;
		if (pDoc->m_War3Map.mapTilePointSelSecond.x < minx)
		{
			minx = pDoc->m_War3Map.mapTilePointSelSecond.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.x < minx)
		{
			minx = pDoc->m_War3Map.mapTilePointSelThird.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.x < minx)
		{
			minx = pDoc->m_War3Map.mapTilePointSelForth.x;
		}
		maxx = pDoc->m_War3Map.mapTilePointSelFirst.x;
		if (pDoc->m_War3Map.mapTilePointSelSecond.x > maxx)
		{
			maxx = pDoc->m_War3Map.mapTilePointSelSecond.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.x > maxx)
		{
			maxx = pDoc->m_War3Map.mapTilePointSelThird.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.x > maxx)
		{
			maxx = pDoc->m_War3Map.mapTilePointSelForth.x;
		}
		miny = pDoc->m_War3Map.mapTilePointSelFirst.y;
		if (pDoc->m_War3Map.mapTilePointSelSecond.y < miny)
		{
			miny = pDoc->m_War3Map.mapTilePointSelSecond.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.y < miny)
		{
			miny = pDoc->m_War3Map.mapTilePointSelThird.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.y < miny)
		{
			miny = pDoc->m_War3Map.mapTilePointSelForth.y;
		}
		maxy = pDoc->m_War3Map.mapTilePointSelFirst.y;
		if (pDoc->m_War3Map.mapTilePointSelSecond.y > maxy)
		{
			maxy = pDoc->m_War3Map.mapTilePointSelSecond.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.y > maxy)
		{
			maxy = pDoc->m_War3Map.mapTilePointSelThird.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.y > maxy)
		{
			maxy = pDoc->m_War3Map.mapTilePointSelForth.y;
		}

		rect.SetRect((minx - 1)*CR_WIDTH, (miny - 1)*CR_HEIGHT, (maxx + 1)*CR_WIDTH, (maxy + 1)*CR_HEIGHT);
		pView->InvalidateTilesBitmap(rect);
		pView->InvalidateBitmapRect(rect);
	}
	else if (4 == pDoc->m_War3Map.mapTilePointSelState)
	{// unselect all
		CWaitCursor wait;
		pDoc->m_War3Map.mapTilePointSelState = 0;
		pDoc->m_War3Map.SelectedTilePointsOnly = false;
		pDoc->m_War3Map.UnselectAllTilePoints();
		minx = pDoc->m_War3Map.mapTilePointSelFirst.x;
		if (pDoc->m_War3Map.mapTilePointSelSecond.x < minx)
		{
			minx = pDoc->m_War3Map.mapTilePointSelSecond.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.x < minx)
		{
			minx = pDoc->m_War3Map.mapTilePointSelThird.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.x < minx)
		{
			minx = pDoc->m_War3Map.mapTilePointSelForth.x;
		}
		maxx = pDoc->m_War3Map.mapTilePointSelFirst.x;
		if (pDoc->m_War3Map.mapTilePointSelSecond.x > maxx)
		{
			maxx = pDoc->m_War3Map.mapTilePointSelSecond.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.x > maxx)
		{
			maxx = pDoc->m_War3Map.mapTilePointSelThird.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.x > maxx)
		{
			maxx = pDoc->m_War3Map.mapTilePointSelForth.x;
		}
		miny = pDoc->m_War3Map.mapTilePointSelFirst.y;
		if (pDoc->m_War3Map.mapTilePointSelSecond.y < miny)
		{
			miny = pDoc->m_War3Map.mapTilePointSelSecond.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.y < miny)
		{
			miny = pDoc->m_War3Map.mapTilePointSelThird.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.y < miny)
		{
			miny = pDoc->m_War3Map.mapTilePointSelForth.y;
		}
		maxy = pDoc->m_War3Map.mapTilePointSelFirst.y;
		if (pDoc->m_War3Map.mapTilePointSelSecond.y > maxy)
		{
			maxy = pDoc->m_War3Map.mapTilePointSelSecond.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.y > maxy)
		{
			maxy = pDoc->m_War3Map.mapTilePointSelThird.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.y > maxy)
		{
			maxy = pDoc->m_War3Map.mapTilePointSelForth.y;
		}

		rect.SetRect((minx - 1)*CR_WIDTH, (miny - 1)*CR_HEIGHT, (maxx + 1)*CR_WIDTH, (maxy + 1)*CR_HEIGHT);
		pView->InvalidateTilesBitmap(rect);
		pView->InvalidateBitmapRect(rect);
	}

}

void CToolTilePointsEdit::ReplaceTool(CW3ZMapEditApp *pTheApp, CW3ZMapEditDoc *pDoc, CW3ZMapEditView *pView, int x, int y)
{
	CMainFrame *pMainFrame = NULL;
	CTilePoint tilepoint;
	CTilePoint undoTilePoint;
	int x0,y0;

	for (y0 = y - m_iSize/2; y0 < y + m_iSize%2 + m_iSize/2;y0++)
	for (x0 = x - m_iSize/2; x0 < x + m_iSize%2 + m_iSize/2;x0++)
	{
		undoTilePoint = tilepoint = pDoc->m_War3Map.GetTilePoint(x0,pDoc->m_War3Map.GetMapHeight() - y0);
		if (!pDoc->m_War3Map.SelectedTilePointsOnly || (pDoc->m_War3Map.SelectedTilePointsOnly && (tilepoint.selected)))
		{
			if (ground == tilepoint.groundtype)
			{
				pMainFrame = pTheApp->GetMainFrame();
				if (pMainFrame) //NULL if pMainFrame wasn't found
				{
					tilepoint.groundtype = pMainFrame->GetTilesEditBarTilePoint().groundtype;
				}
				// stores old tilepoint
				pDoc->AddToUndoObj(undoTilePoint, x0, pDoc->m_War3Map.GetMapHeight() - y0);
				// writes new tilepoint
				pDoc->m_War3Map.SetTilePoint(x0, pDoc->m_War3Map.GetMapHeight() - y0,tilepoint);
				pDoc->SetModifiedFlag();
				pView->DisplayTilePoint(x0, pDoc->m_War3Map.GetMapHeight() - y0,tilepoint);
			}
		}
	}
}

int CToolTilePointsEdit::GetToolSize()
{
	return m_iSize;
}

void CToolTilePointsEdit::SetToolSize(int size)
{
	if ((TOOL_TILE_MIN_SIZE <= size ) && (size < TOOL_TILE_MAX_SIZE))
	{
		m_iSize = size;
	}
}


void CToolTilePointsEdit::GroundSlope()
{
	CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
	ASSERT_VALID(pTheApp);
	if (!pTheApp)
	{return;}
	CMainFrame *pMainFrame = pTheApp->GetMainFrame();
	if (!pMainFrame)
	{return;}
	CChildFrame *pChildFrame = (CChildFrame*)pMainFrame->GetActiveFrame();
	if (!pChildFrame)
	{return;}
	CW3ZMapEditView *pView = pChildFrame->GetEditView();
	if (!pView)
	{return;}
	CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pChildFrame->GetActiveDocument();
	if (!pDoc)
	{return;}
	ASSERT_VALID(pDoc);

	CTilePoint tilepoint;
	CTilePoint undoTilePoint;
	int x, y, minx, maxx, miny, maxy;
	CTilePoint gTP1, gTP2, gTP3, gTP4;
	CRect rect;
	double dist1, dist2, dist3, dist4, a, b, c, d;

	if (4 == pDoc->m_War3Map.mapTilePointSelState)
	{// check if we got selected tilepoints
		CWaitCursor wait;
		pDoc->OpenUndoObj();
		// gets the coordinates of the rectangle that contains the selected tilepoints
		minx = pDoc->m_War3Map.mapTilePointSelFirst.x;
		if (pDoc->m_War3Map.mapTilePointSelSecond.x < minx)
		{
			minx = pDoc->m_War3Map.mapTilePointSelSecond.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.x < minx)
		{
			minx = pDoc->m_War3Map.mapTilePointSelThird.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.x < minx)
		{
			minx = pDoc->m_War3Map.mapTilePointSelForth.x;
		}
		maxx = pDoc->m_War3Map.mapTilePointSelFirst.x;
		if (pDoc->m_War3Map.mapTilePointSelSecond.x > maxx)
		{
			maxx = pDoc->m_War3Map.mapTilePointSelSecond.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.x > maxx)
		{
			maxx = pDoc->m_War3Map.mapTilePointSelThird.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.x > maxx)
		{
			maxx = pDoc->m_War3Map.mapTilePointSelForth.x;
		}
		miny = pDoc->m_War3Map.mapTilePointSelFirst.y;
		if (pDoc->m_War3Map.mapTilePointSelSecond.y < miny)
		{
			miny = pDoc->m_War3Map.mapTilePointSelSecond.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.y < miny)
		{
			miny = pDoc->m_War3Map.mapTilePointSelThird.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.y < miny)
		{
			miny = pDoc->m_War3Map.mapTilePointSelForth.y;
		}
		maxy = pDoc->m_War3Map.mapTilePointSelFirst.y;
		if (pDoc->m_War3Map.mapTilePointSelSecond.y > maxy)
		{
			maxy = pDoc->m_War3Map.mapTilePointSelSecond.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.y > maxy)
		{
			maxy = pDoc->m_War3Map.mapTilePointSelThird.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.y > maxy)
		{
			maxy = pDoc->m_War3Map.mapTilePointSelForth.y;
		}
		// gets corner points of the selected shape
		gTP1 = pDoc->m_War3Map.GetTilePoint(pDoc->m_War3Map.mapTilePointSelFirst.x,pDoc->m_War3Map.GetMapHeight() - pDoc->m_War3Map.mapTilePointSelFirst.y);
		gTP2 = pDoc->m_War3Map.GetTilePoint(pDoc->m_War3Map.mapTilePointSelSecond.x,pDoc->m_War3Map.GetMapHeight() - pDoc->m_War3Map.mapTilePointSelSecond.y);
		gTP3 = pDoc->m_War3Map.GetTilePoint(pDoc->m_War3Map.mapTilePointSelThird.x,pDoc->m_War3Map.GetMapHeight() - pDoc->m_War3Map.mapTilePointSelThird.y);
		gTP4 = pDoc->m_War3Map.GetTilePoint(pDoc->m_War3Map.mapTilePointSelForth.x,pDoc->m_War3Map.GetMapHeight() - pDoc->m_War3Map.mapTilePointSelForth.y);
		for (y = miny; y <= maxy; y++)
		{
			for (x = minx; x <= maxx; x++)
			{
				tilepoint = pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y);
				if (tilepoint.selected)
				{ // only selected tilepoints
						pDoc->AddToUndoObj(tilepoint, x, pDoc->m_War3Map.GetMapHeight() - y);
						if ((pDoc->m_War3Map.mapTilePointSelFirst.x != pDoc->m_War3Map.mapTilePointSelSecond.x) || (pDoc->m_War3Map.mapTilePointSelFirst.y != pDoc->m_War3Map.mapTilePointSelSecond.y))
						{// c != 0
							//a = distance between the point and gTP1
							//b = distance between the point and gTP2
							//c = distance between the gTP1 and gTP2
							//d = distance between gTP1 and the normal to (gTP1;gTP2) that passes by the point
							//dist1 = distance between the point and the ligne (gTP1;gTP2)
							//dist2 = calculated height of the intersection of (gTP1;gTP2) and the normal that passes by the point
							a = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelFirst.x)*(x - pDoc->m_War3Map.mapTilePointSelFirst.x) + (y - pDoc->m_War3Map.mapTilePointSelFirst.y)*(y - pDoc->m_War3Map.mapTilePointSelFirst.y)));
							b = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelSecond.x)*(x - pDoc->m_War3Map.mapTilePointSelSecond.x) + (y - pDoc->m_War3Map.mapTilePointSelSecond.y)*(y - pDoc->m_War3Map.mapTilePointSelSecond.y)));
							c = sqrt((double)((pDoc->m_War3Map.mapTilePointSelFirst.x - pDoc->m_War3Map.mapTilePointSelSecond.x)*(pDoc->m_War3Map.mapTilePointSelFirst.x - pDoc->m_War3Map.mapTilePointSelSecond.x) + (pDoc->m_War3Map.mapTilePointSelFirst.y - pDoc->m_War3Map.mapTilePointSelSecond.y)*(pDoc->m_War3Map.mapTilePointSelFirst.y - pDoc->m_War3Map.mapTilePointSelSecond.y)));
							d = (a*a - b*b + c*c) / (2*c);
							dist1 = sqrt(a*a - d*d);
							dist2 = gTP1.height*(c-d)/c + gTP2.height*d/c;
						}
						else
						{// gTP1 = gTP2
							//dist1 = distance between gTP1 and the point
							//dist2 = height of gTP1
							dist1 = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelFirst.x)*(x - pDoc->m_War3Map.mapTilePointSelFirst.x) + (y - pDoc->m_War3Map.mapTilePointSelFirst.y)*(y - pDoc->m_War3Map.mapTilePointSelFirst.y)));
							dist2 = gTP1.height;
						}
						if ((pDoc->m_War3Map.mapTilePointSelThird.x != pDoc->m_War3Map.mapTilePointSelForth.x) || (pDoc->m_War3Map.mapTilePointSelThird.y != pDoc->m_War3Map.mapTilePointSelForth.y))
						{// c != 0
							//a = distance between the point and gTP3
							//b = distance between the point and gTP4
							//c = distance between the gTP3 and gTP4
							//d = distance between gTP3 and the normal to (gTP3;gTP4) that passes by the point
							//dist1 = distance between the point and the ligne (gTP3;gTP4)
							//dist2 = calculated height of the intersection of (gTP3;gTP4) and the normal that passes by the point
							a = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelThird.x)*(x - pDoc->m_War3Map.mapTilePointSelThird.x) + (y - pDoc->m_War3Map.mapTilePointSelThird.y)*(y - pDoc->m_War3Map.mapTilePointSelThird.y)));
							b = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelForth.x)*(x - pDoc->m_War3Map.mapTilePointSelForth.x) + (y - pDoc->m_War3Map.mapTilePointSelForth.y)*(y - pDoc->m_War3Map.mapTilePointSelForth.y)));
							c = sqrt((double)((pDoc->m_War3Map.mapTilePointSelThird.x - pDoc->m_War3Map.mapTilePointSelForth.x)*(pDoc->m_War3Map.mapTilePointSelThird.x - pDoc->m_War3Map.mapTilePointSelForth.x) + (pDoc->m_War3Map.mapTilePointSelThird.y - pDoc->m_War3Map.mapTilePointSelForth.y)*(pDoc->m_War3Map.mapTilePointSelThird.y - pDoc->m_War3Map.mapTilePointSelForth.y)));
							d = (a*a - b*b + c*c) / (2*c);
							dist3 = sqrt(a*a - d*d);
							dist4 = gTP3.height*(c-d)/c + gTP4.height*d/c;
						}
						else
						{// gTP3 = gTP4
							//dist3 = distance between gTP3 and the point
							//dist4 = height of gTP3
							dist3 = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelThird.x)*(x - pDoc->m_War3Map.mapTilePointSelThird.x) + (y - pDoc->m_War3Map.mapTilePointSelThird.y)*(y - pDoc->m_War3Map.mapTilePointSelThird.y)));
							dist4 = gTP3.height;
						}
						if ((dist1) && (dist3))
						{// point is neither between gTP1 and gTP2 nor between gTP3 and gTP4
							// inverts factors and calculate the height
							dist1 = 1/dist1;
							dist3 = 1/dist3;
							tilepoint.height = (int)((dist1*dist2 + dist3*dist4)/(dist1+dist3));
						}
						else if (dist3)
						{// point between gTP1 and gTP2 only
							dist1 = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelFirst.x)*(x - pDoc->m_War3Map.mapTilePointSelFirst.x) + (y - pDoc->m_War3Map.mapTilePointSelFirst.y)*(y - pDoc->m_War3Map.mapTilePointSelFirst.y)));
							dist2 = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelSecond.x)*(x - pDoc->m_War3Map.mapTilePointSelSecond.x) + (y - pDoc->m_War3Map.mapTilePointSelSecond.y)*(y - pDoc->m_War3Map.mapTilePointSelSecond.y)));
							if ((dist1) && (dist2))
							{// point != gTP1 && point != gTP2
								dist1 = 1/dist1;
								dist2 = 1/dist2;
								tilepoint.height = (int)((gTP1.height*dist1 + gTP2.height*dist2)/(dist1+dist2));
							}// else don't change the point height because it's gTP1 or gTP2
						}
						else if (dist1)
						{ // point between gTP3 and gTP4 only
							dist3 = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelThird.x)*(x - pDoc->m_War3Map.mapTilePointSelThird.x) + (y - pDoc->m_War3Map.mapTilePointSelThird.y)*(y - pDoc->m_War3Map.mapTilePointSelThird.y)));
							dist4 = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelForth.x)*(x - pDoc->m_War3Map.mapTilePointSelForth.x) + (y - pDoc->m_War3Map.mapTilePointSelForth.y)*(y - pDoc->m_War3Map.mapTilePointSelForth.y)));
							if ((dist3) && (dist4))
							{// point != gTP3 && point != gTP4
								dist3 = 1/dist3;
								dist4 = 1/dist4;
								tilepoint.height = (int)((gTP3.height*dist3 + gTP4.height*dist4)/(dist3+dist4));
							}// else don't change the point height because it's gTP3 or gTP4
						}//	else gTP1 = gTP2 = gTP3 = gTP4 so do nothing
				}
				pDoc->SetModifiedFlag();
				pDoc->m_War3Map.SetTilePoint(x, pDoc->m_War3Map.GetMapHeight() - y, tilepoint);
			}			
		}

		rect.SetRect((minx - 1)*CR_WIDTH, (miny - 1)*CR_HEIGHT, (maxx + 1)*CR_WIDTH, (maxy + 1)*CR_HEIGHT);
		pView->InvalidateTilesBitmap(rect);
		pView->InvalidateBitmapRect(rect);
		pDoc->CloseUndoObj();
	}
	else
	{// no selected tilepoints
		AfxMessageBox("You need to select tilepoints first to use this tool!", MB_OK | MB_APPLMODAL | MB_ICONWARNING);
	}
}

void CToolTilePointsEdit::GroundBendUp()
{
	CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
	ASSERT_VALID(pTheApp);
	if (!pTheApp)
	{return;}
	CMainFrame *pMainFrame = pTheApp->GetMainFrame();
	if (!pMainFrame)
	{return;}
	CChildFrame *pChildFrame = (CChildFrame*)pMainFrame->GetActiveFrame();
	if (!pChildFrame)
	{return;}
	CW3ZMapEditView *pView = pChildFrame->GetEditView();
	if (!pView)
	{return;}
	CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pChildFrame->GetActiveDocument();
	if (!pDoc)
	{return;}
	ASSERT_VALID(pDoc);

	CTilePoint tilepoint;
	CTilePoint undoTilePoint;
	int x, y, minx, maxx, miny, maxy;
	CTilePoint gTP1, gTP2, gTP3, gTP4;
	CRect rect;
	double dist1, dist2, dist3, dist4, a, b, c, d;

	if (4 == pDoc->m_War3Map.mapTilePointSelState)
	{// check if we got selected tilepoints
		CWaitCursor wait;
		pDoc->OpenUndoObj();
		// gets the coordinates of the rectangle that contains the selected tilepoints
		minx = pDoc->m_War3Map.mapTilePointSelFirst.x;
		if (pDoc->m_War3Map.mapTilePointSelSecond.x < minx)
		{
			minx = pDoc->m_War3Map.mapTilePointSelSecond.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.x < minx)
		{
			minx = pDoc->m_War3Map.mapTilePointSelThird.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.x < minx)
		{
			minx = pDoc->m_War3Map.mapTilePointSelForth.x;
		}
		maxx = pDoc->m_War3Map.mapTilePointSelFirst.x;
		if (pDoc->m_War3Map.mapTilePointSelSecond.x > maxx)
		{
			maxx = pDoc->m_War3Map.mapTilePointSelSecond.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.x > maxx)
		{
			maxx = pDoc->m_War3Map.mapTilePointSelThird.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.x > maxx)
		{
			maxx = pDoc->m_War3Map.mapTilePointSelForth.x;
		}
		miny = pDoc->m_War3Map.mapTilePointSelFirst.y;
		if (pDoc->m_War3Map.mapTilePointSelSecond.y < miny)
		{
			miny = pDoc->m_War3Map.mapTilePointSelSecond.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.y < miny)
		{
			miny = pDoc->m_War3Map.mapTilePointSelThird.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.y < miny)
		{
			miny = pDoc->m_War3Map.mapTilePointSelForth.y;
		}
		maxy = pDoc->m_War3Map.mapTilePointSelFirst.y;
		if (pDoc->m_War3Map.mapTilePointSelSecond.y > maxy)
		{
			maxy = pDoc->m_War3Map.mapTilePointSelSecond.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.y > maxy)
		{
			maxy = pDoc->m_War3Map.mapTilePointSelThird.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.y > maxy)
		{
			maxy = pDoc->m_War3Map.mapTilePointSelForth.y;
		}
		// gets corner points of the selected shape
		gTP1 = pDoc->m_War3Map.GetTilePoint(pDoc->m_War3Map.mapTilePointSelFirst.x,pDoc->m_War3Map.GetMapHeight() - pDoc->m_War3Map.mapTilePointSelFirst.y);
		gTP2 = pDoc->m_War3Map.GetTilePoint(pDoc->m_War3Map.mapTilePointSelSecond.x,pDoc->m_War3Map.GetMapHeight() - pDoc->m_War3Map.mapTilePointSelSecond.y);
		gTP3 = pDoc->m_War3Map.GetTilePoint(pDoc->m_War3Map.mapTilePointSelThird.x,pDoc->m_War3Map.GetMapHeight() - pDoc->m_War3Map.mapTilePointSelThird.y);
		gTP4 = pDoc->m_War3Map.GetTilePoint(pDoc->m_War3Map.mapTilePointSelForth.x,pDoc->m_War3Map.GetMapHeight() - pDoc->m_War3Map.mapTilePointSelForth.y);
		for (y = miny; y <= maxy; y++)
		{
			for (x = minx; x <= maxx; x++)
			{
				tilepoint = pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y);
				if (tilepoint.selected)
				{ // only selected tilepoints
						pDoc->AddToUndoObj(tilepoint, x, pDoc->m_War3Map.GetMapHeight() - y);
						if ((pDoc->m_War3Map.mapTilePointSelFirst.x != pDoc->m_War3Map.mapTilePointSelSecond.x) || (pDoc->m_War3Map.mapTilePointSelFirst.y != pDoc->m_War3Map.mapTilePointSelSecond.y))
						{// c != 0
							//a = distance between the point and gTP1
							//b = distance between the point and gTP2
							//c = distance between the gTP1 and gTP2
							//d = distance between gTP1 and the normal to (gTP1;gTP2) that passes by the point
							//dist1 = distance between the point and the ligne (gTP1;gTP2)
							//dist2 = calculated height of the intersection of (gTP1;gTP2) and the normal that passes by the point
							a = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelFirst.x)*(x - pDoc->m_War3Map.mapTilePointSelFirst.x) + (y - pDoc->m_War3Map.mapTilePointSelFirst.y)*(y - pDoc->m_War3Map.mapTilePointSelFirst.y)));
							b = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelSecond.x)*(x - pDoc->m_War3Map.mapTilePointSelSecond.x) + (y - pDoc->m_War3Map.mapTilePointSelSecond.y)*(y - pDoc->m_War3Map.mapTilePointSelSecond.y)));
							c = sqrt((double)((pDoc->m_War3Map.mapTilePointSelFirst.x - pDoc->m_War3Map.mapTilePointSelSecond.x)*(pDoc->m_War3Map.mapTilePointSelFirst.x - pDoc->m_War3Map.mapTilePointSelSecond.x) + (pDoc->m_War3Map.mapTilePointSelFirst.y - pDoc->m_War3Map.mapTilePointSelSecond.y)*(pDoc->m_War3Map.mapTilePointSelFirst.y - pDoc->m_War3Map.mapTilePointSelSecond.y)));
							d = (a*a - b*b + c*c) / (2*c);
							dist1 = sqrt(a*a - d*d);
							dist2 = gTP1.height*(c-d)/c + gTP2.height*d/c;
						}
						else
						{// gTP1 = gTP2
							//dist1 = distance between gTP1 and the point
							//dist2 = height of gTP1
							dist1 = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelFirst.x)*(x - pDoc->m_War3Map.mapTilePointSelFirst.x) + (y - pDoc->m_War3Map.mapTilePointSelFirst.y)*(y - pDoc->m_War3Map.mapTilePointSelFirst.y)));
							dist2 = gTP1.height;
						}
						if ((pDoc->m_War3Map.mapTilePointSelThird.x != pDoc->m_War3Map.mapTilePointSelForth.x) || (pDoc->m_War3Map.mapTilePointSelThird.y != pDoc->m_War3Map.mapTilePointSelForth.y))
						{// c != 0
							//a = distance between the point and gTP3
							//b = distance between the point and gTP4
							//c = distance between the gTP3 and gTP4
							//d = distance between gTP3 and the normal to (gTP3;gTP4) that passes by the point
							//dist1 = distance between the point and the ligne (gTP3;gTP4)
							//dist2 = calculated height of the intersection of (gTP3;gTP4) and the normal that passes by the point
							a = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelThird.x)*(x - pDoc->m_War3Map.mapTilePointSelThird.x) + (y - pDoc->m_War3Map.mapTilePointSelThird.y)*(y - pDoc->m_War3Map.mapTilePointSelThird.y)));
							b = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelForth.x)*(x - pDoc->m_War3Map.mapTilePointSelForth.x) + (y - pDoc->m_War3Map.mapTilePointSelForth.y)*(y - pDoc->m_War3Map.mapTilePointSelForth.y)));
							c = sqrt((double)((pDoc->m_War3Map.mapTilePointSelThird.x - pDoc->m_War3Map.mapTilePointSelForth.x)*(pDoc->m_War3Map.mapTilePointSelThird.x - pDoc->m_War3Map.mapTilePointSelForth.x) + (pDoc->m_War3Map.mapTilePointSelThird.y - pDoc->m_War3Map.mapTilePointSelForth.y)*(pDoc->m_War3Map.mapTilePointSelThird.y - pDoc->m_War3Map.mapTilePointSelForth.y)));
							d = (a*a - b*b + c*c) / (2*c);
							dist3 = sqrt(a*a - d*d);
							dist4 = gTP3.height*(c-d)/c + gTP4.height*d/c;
						}
						else
						{// gTP3 = gTP4
							//dist3 = distance between gTP3 and the point
							//dist4 = height of gTP3
							dist3 = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelThird.x)*(x - pDoc->m_War3Map.mapTilePointSelThird.x) + (y - pDoc->m_War3Map.mapTilePointSelThird.y)*(y - pDoc->m_War3Map.mapTilePointSelThird.y)));
							dist4 = gTP3.height;
						}
						if ((dist1) && (dist3))
						{// point is neither between gTP1 and gTP2 nor between gTP3 and gTP4
							// inverts factors and calculate the height
							dist1 = 1/dist1;
							dist3 = 1/(dist3*dist3); // bend up
							tilepoint.height = (int)((dist1*dist2 + dist3*dist4)/(dist1+dist3));
						}
						else if (dist3)
						{// point between gTP1 and gTP2 only
							dist1 = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelFirst.x)*(x - pDoc->m_War3Map.mapTilePointSelFirst.x) + (y - pDoc->m_War3Map.mapTilePointSelFirst.y)*(y - pDoc->m_War3Map.mapTilePointSelFirst.y)));
							dist2 = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelSecond.x)*(x - pDoc->m_War3Map.mapTilePointSelSecond.x) + (y - pDoc->m_War3Map.mapTilePointSelSecond.y)*(y - pDoc->m_War3Map.mapTilePointSelSecond.y)));
							if ((dist1) && (dist2))
							{// point != gTP1 && point != gTP2
								dist1 = 1/dist1;
								dist2 = 1/dist2;
								tilepoint.height = (int)((gTP1.height*dist1 + gTP2.height*dist2)/(dist1+dist2));
							}// else don't change the point height because it's gTP1 or gTP2
						}
						else if (dist1)
						{ // point between gTP3 and gTP4 only
							dist3 = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelThird.x)*(x - pDoc->m_War3Map.mapTilePointSelThird.x) + (y - pDoc->m_War3Map.mapTilePointSelThird.y)*(y - pDoc->m_War3Map.mapTilePointSelThird.y)));
							dist4 = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelForth.x)*(x - pDoc->m_War3Map.mapTilePointSelForth.x) + (y - pDoc->m_War3Map.mapTilePointSelForth.y)*(y - pDoc->m_War3Map.mapTilePointSelForth.y)));
							if ((dist3) && (dist4))
							{// point != gTP3 && point != gTP4
								dist3 = 1/dist3;
								dist4 = 1/dist4;
								tilepoint.height = (int)((gTP3.height*dist3 + gTP4.height*dist4)/(dist3+dist4));
							}// else don't change the point height because it's gTP3 or gTP4
						}//	else gTP1 = gTP2 = gTP3 = gTP4 so do nothing
				}
				pDoc->SetModifiedFlag();
				pDoc->m_War3Map.SetTilePoint(x, pDoc->m_War3Map.GetMapHeight() - y, tilepoint);
			}			
		}

		rect.SetRect((minx - 1)*CR_WIDTH, (miny - 1)*CR_HEIGHT, (maxx + 1)*CR_WIDTH, (maxy + 1)*CR_HEIGHT);
		pView->InvalidateTilesBitmap(rect);
		pView->InvalidateBitmapRect(rect);
		pDoc->CloseUndoObj();
	}
	else
	{// no selected tilepoints
		AfxMessageBox("You need to select tilepoints first to use this tool!", MB_OK | MB_APPLMODAL | MB_ICONWARNING);
	}
}

void CToolTilePointsEdit::GroundBendDown()
{
	CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
	ASSERT_VALID(pTheApp);
	if (!pTheApp)
	{return;}
	CMainFrame *pMainFrame = pTheApp->GetMainFrame();
	if (!pMainFrame)
	{return;}
	CChildFrame *pChildFrame = (CChildFrame*)pMainFrame->GetActiveFrame();
	if (!pChildFrame)
	{return;}
	CW3ZMapEditView *pView = pChildFrame->GetEditView();
	if (!pView)
	{return;}
	CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pChildFrame->GetActiveDocument();
	if (!pDoc)
	{return;}
	ASSERT_VALID(pDoc);

	CTilePoint tilepoint;
	CTilePoint undoTilePoint;
	int x, y, minx, maxx, miny, maxy;
	CTilePoint gTP1, gTP2, gTP3, gTP4;
	CRect rect;
	double dist1, dist2, dist3, dist4, a, b, c, d;

	if (4 == pDoc->m_War3Map.mapTilePointSelState)
	{// check if we got selected tilepoints
		CWaitCursor wait;
		pDoc->OpenUndoObj();
		// gets the coordinates of the rectangle that contains the selected tilepoints
		minx = pDoc->m_War3Map.mapTilePointSelFirst.x;
		if (pDoc->m_War3Map.mapTilePointSelSecond.x < minx)
		{
			minx = pDoc->m_War3Map.mapTilePointSelSecond.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.x < minx)
		{
			minx = pDoc->m_War3Map.mapTilePointSelThird.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.x < minx)
		{
			minx = pDoc->m_War3Map.mapTilePointSelForth.x;
		}
		maxx = pDoc->m_War3Map.mapTilePointSelFirst.x;
		if (pDoc->m_War3Map.mapTilePointSelSecond.x > maxx)
		{
			maxx = pDoc->m_War3Map.mapTilePointSelSecond.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.x > maxx)
		{
			maxx = pDoc->m_War3Map.mapTilePointSelThird.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.x > maxx)
		{
			maxx = pDoc->m_War3Map.mapTilePointSelForth.x;
		}
		miny = pDoc->m_War3Map.mapTilePointSelFirst.y;
		if (pDoc->m_War3Map.mapTilePointSelSecond.y < miny)
		{
			miny = pDoc->m_War3Map.mapTilePointSelSecond.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.y < miny)
		{
			miny = pDoc->m_War3Map.mapTilePointSelThird.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.y < miny)
		{
			miny = pDoc->m_War3Map.mapTilePointSelForth.y;
		}
		maxy = pDoc->m_War3Map.mapTilePointSelFirst.y;
		if (pDoc->m_War3Map.mapTilePointSelSecond.y > maxy)
		{
			maxy = pDoc->m_War3Map.mapTilePointSelSecond.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.y > maxy)
		{
			maxy = pDoc->m_War3Map.mapTilePointSelThird.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.y > maxy)
		{
			maxy = pDoc->m_War3Map.mapTilePointSelForth.y;
		}
		// gets corner points of the selected shape
		gTP1 = pDoc->m_War3Map.GetTilePoint(pDoc->m_War3Map.mapTilePointSelFirst.x,pDoc->m_War3Map.GetMapHeight() - pDoc->m_War3Map.mapTilePointSelFirst.y);
		gTP2 = pDoc->m_War3Map.GetTilePoint(pDoc->m_War3Map.mapTilePointSelSecond.x,pDoc->m_War3Map.GetMapHeight() - pDoc->m_War3Map.mapTilePointSelSecond.y);
		gTP3 = pDoc->m_War3Map.GetTilePoint(pDoc->m_War3Map.mapTilePointSelThird.x,pDoc->m_War3Map.GetMapHeight() - pDoc->m_War3Map.mapTilePointSelThird.y);
		gTP4 = pDoc->m_War3Map.GetTilePoint(pDoc->m_War3Map.mapTilePointSelForth.x,pDoc->m_War3Map.GetMapHeight() - pDoc->m_War3Map.mapTilePointSelForth.y);
		for (y = miny; y <= maxy; y++)
		{
			for (x = minx; x <= maxx; x++)
			{
				tilepoint = pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y);
				if (tilepoint.selected)
				{ // only selected tilepoints
						pDoc->AddToUndoObj(tilepoint, x, pDoc->m_War3Map.GetMapHeight() - y);
						if ((pDoc->m_War3Map.mapTilePointSelFirst.x != pDoc->m_War3Map.mapTilePointSelSecond.x) || (pDoc->m_War3Map.mapTilePointSelFirst.y != pDoc->m_War3Map.mapTilePointSelSecond.y))
						{// c != 0
							//a = distance between the point and gTP1
							//b = distance between the point and gTP2
							//c = distance between the gTP1 and gTP2
							//d = distance between gTP1 and the normal to (gTP1;gTP2) that passes by the point
							//dist1 = distance between the point and the ligne (gTP1;gTP2)
							//dist2 = calculated height of the intersection of (gTP1;gTP2) and the normal that passes by the point
							a = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelFirst.x)*(x - pDoc->m_War3Map.mapTilePointSelFirst.x) + (y - pDoc->m_War3Map.mapTilePointSelFirst.y)*(y - pDoc->m_War3Map.mapTilePointSelFirst.y)));
							b = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelSecond.x)*(x - pDoc->m_War3Map.mapTilePointSelSecond.x) + (y - pDoc->m_War3Map.mapTilePointSelSecond.y)*(y - pDoc->m_War3Map.mapTilePointSelSecond.y)));
							c = sqrt((double)((pDoc->m_War3Map.mapTilePointSelFirst.x - pDoc->m_War3Map.mapTilePointSelSecond.x)*(pDoc->m_War3Map.mapTilePointSelFirst.x - pDoc->m_War3Map.mapTilePointSelSecond.x) + (pDoc->m_War3Map.mapTilePointSelFirst.y - pDoc->m_War3Map.mapTilePointSelSecond.y)*(pDoc->m_War3Map.mapTilePointSelFirst.y - pDoc->m_War3Map.mapTilePointSelSecond.y)));
							d = (a*a - b*b + c*c) / (2*c);
							dist1 = sqrt(a*a - d*d);
							dist2 = gTP1.height*(c-d)/c + gTP2.height*d/c;
						}
						else
						{// gTP1 = gTP2
							//dist1 = distance between gTP1 and the point
							//dist2 = height of gTP1
							dist1 = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelFirst.x)*(x - pDoc->m_War3Map.mapTilePointSelFirst.x) + (y - pDoc->m_War3Map.mapTilePointSelFirst.y)*(y - pDoc->m_War3Map.mapTilePointSelFirst.y)));
							dist2 = gTP1.height;
						}
						if ((pDoc->m_War3Map.mapTilePointSelThird.x != pDoc->m_War3Map.mapTilePointSelForth.x) || (pDoc->m_War3Map.mapTilePointSelThird.y != pDoc->m_War3Map.mapTilePointSelForth.y))
						{// c != 0
							//a = distance between the point and gTP3
							//b = distance between the point and gTP4
							//c = distance between the gTP3 and gTP4
							//d = distance between gTP3 and the normal to (gTP3;gTP4) that passes by the point
							//dist1 = distance between the point and the ligne (gTP3;gTP4)
							//dist2 = calculated height of the intersection of (gTP3;gTP4) and the normal that passes by the point
							a = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelThird.x)*(x - pDoc->m_War3Map.mapTilePointSelThird.x) + (y - pDoc->m_War3Map.mapTilePointSelThird.y)*(y - pDoc->m_War3Map.mapTilePointSelThird.y)));
							b = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelForth.x)*(x - pDoc->m_War3Map.mapTilePointSelForth.x) + (y - pDoc->m_War3Map.mapTilePointSelForth.y)*(y - pDoc->m_War3Map.mapTilePointSelForth.y)));
							c = sqrt((double)((pDoc->m_War3Map.mapTilePointSelThird.x - pDoc->m_War3Map.mapTilePointSelForth.x)*(pDoc->m_War3Map.mapTilePointSelThird.x - pDoc->m_War3Map.mapTilePointSelForth.x) + (pDoc->m_War3Map.mapTilePointSelThird.y - pDoc->m_War3Map.mapTilePointSelForth.y)*(pDoc->m_War3Map.mapTilePointSelThird.y - pDoc->m_War3Map.mapTilePointSelForth.y)));
							d = (a*a - b*b + c*c) / (2*c);
							dist3 = sqrt(a*a - d*d);
							dist4 = gTP3.height*(c-d)/c + gTP4.height*d/c;
						}
						else
						{// gTP3 = gTP4
							//dist3 = distance between gTP3 and the point
							//dist4 = height of gTP3
							dist3 = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelThird.x)*(x - pDoc->m_War3Map.mapTilePointSelThird.x) + (y - pDoc->m_War3Map.mapTilePointSelThird.y)*(y - pDoc->m_War3Map.mapTilePointSelThird.y)));
							dist4 = gTP3.height;
						}
						if ((dist1) && (dist3))
						{// point is neither between gTP1 and gTP2 nor between gTP3 and gTP4
							// inverts factors and calculate the height
							dist1 = 1/(dist1*dist1); // bend down
							dist3 = 1/dist3;
							tilepoint.height = (int)((dist1*dist2 + dist3*dist4)/(dist1+dist3));
						}
						else if (dist3)
						{// point between gTP1 and gTP2 only
							dist1 = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelFirst.x)*(x - pDoc->m_War3Map.mapTilePointSelFirst.x) + (y - pDoc->m_War3Map.mapTilePointSelFirst.y)*(y - pDoc->m_War3Map.mapTilePointSelFirst.y)));
							dist2 = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelSecond.x)*(x - pDoc->m_War3Map.mapTilePointSelSecond.x) + (y - pDoc->m_War3Map.mapTilePointSelSecond.y)*(y - pDoc->m_War3Map.mapTilePointSelSecond.y)));
							if ((dist1) && (dist2))
							{// point != gTP1 && point != gTP2
								dist1 = 1/dist1;
								dist2 = 1/dist2;
								tilepoint.height = (int)((gTP1.height*dist1 + gTP2.height*dist2)/(dist1+dist2));
							}// else don't change the point height because it's gTP1 or gTP2
						}
						else if (dist1)
						{ // point between gTP3 and gTP4 only
							dist3 = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelThird.x)*(x - pDoc->m_War3Map.mapTilePointSelThird.x) + (y - pDoc->m_War3Map.mapTilePointSelThird.y)*(y - pDoc->m_War3Map.mapTilePointSelThird.y)));
							dist4 = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelForth.x)*(x - pDoc->m_War3Map.mapTilePointSelForth.x) + (y - pDoc->m_War3Map.mapTilePointSelForth.y)*(y - pDoc->m_War3Map.mapTilePointSelForth.y)));
							if ((dist3) && (dist4))
							{// point != gTP3 && point != gTP4
								dist3 = 1/dist3;
								dist4 = 1/dist4;
								tilepoint.height = (int)((gTP3.height*dist3 + gTP4.height*dist4)/(dist3+dist4));
							}// else don't change the point height because it's gTP3 or gTP4
						}//	else gTP1 = gTP2 = gTP3 = gTP4 so do nothing
				}
				pDoc->SetModifiedFlag();
				pDoc->m_War3Map.SetTilePoint(x, pDoc->m_War3Map.GetMapHeight() - y, tilepoint);
			}			
		}

		rect.SetRect((minx - 1)*CR_WIDTH, (miny - 1)*CR_HEIGHT, (maxx + 1)*CR_WIDTH, (maxy + 1)*CR_HEIGHT);
		pView->InvalidateTilesBitmap(rect);
		pView->InvalidateBitmapRect(rect);
		pDoc->CloseUndoObj();
	}
	else
	{// no selected tilepoints
		AfxMessageBox("You need to select tilepoints first to use this tool!", MB_OK | MB_APPLMODAL | MB_ICONWARNING);
	}
}


void CToolTilePointsEdit::GroundFlat()
{
	CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
	ASSERT_VALID(pTheApp);
	if (!pTheApp)
	{return;}
	CMainFrame *pMainFrame = pTheApp->GetMainFrame();
	if (!pMainFrame)
	{return;}
	CChildFrame *pChildFrame = (CChildFrame*)pMainFrame->GetActiveFrame();
	if (!pChildFrame)
	{return;}
	CW3ZMapEditView *pView = pChildFrame->GetEditView();	
	if (!pView)
	{return;}
	CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pChildFrame->GetActiveDocument();
	if (!pDoc)
	{return;}
	ASSERT_VALID(pDoc);

	CTilePoint tilepoint;
	int x, y, minx, maxx, miny, maxy;
	CRect rect;

	if (4 == pDoc->m_War3Map.mapTilePointSelState)
	{// check if we got selected tilepoints
		CWaitCursor wait;
		pDoc->OpenUndoObj();
		// gets the coordinates of the rectangle that contains the selected tilepoints
		minx = pDoc->m_War3Map.mapTilePointSelFirst.x;
		if (pDoc->m_War3Map.mapTilePointSelSecond.x < minx)
		{
			minx = pDoc->m_War3Map.mapTilePointSelSecond.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.x < minx)
		{
			minx = pDoc->m_War3Map.mapTilePointSelThird.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.x < minx)
		{
			minx = pDoc->m_War3Map.mapTilePointSelForth.x;
		}
		maxx = pDoc->m_War3Map.mapTilePointSelFirst.x;
		if (pDoc->m_War3Map.mapTilePointSelSecond.x > maxx)
		{
			maxx = pDoc->m_War3Map.mapTilePointSelSecond.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.x > maxx)
		{
			maxx = pDoc->m_War3Map.mapTilePointSelThird.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.x > maxx)
		{
			maxx = pDoc->m_War3Map.mapTilePointSelForth.x;
		}
		miny = pDoc->m_War3Map.mapTilePointSelFirst.y;
		if (pDoc->m_War3Map.mapTilePointSelSecond.y < miny)
		{
			miny = pDoc->m_War3Map.mapTilePointSelSecond.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.y < miny)
		{
			miny = pDoc->m_War3Map.mapTilePointSelThird.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.y < miny)
		{
			miny = pDoc->m_War3Map.mapTilePointSelForth.y;
		}
		maxy = pDoc->m_War3Map.mapTilePointSelFirst.y;
		if (pDoc->m_War3Map.mapTilePointSelSecond.y > maxy)
		{
			maxy = pDoc->m_War3Map.mapTilePointSelSecond.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.y > maxy)
		{
			maxy = pDoc->m_War3Map.mapTilePointSelThird.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.y > maxy)
		{
			maxy = pDoc->m_War3Map.mapTilePointSelForth.y;
		}
		// reduced range
		if ((minx+1 < maxx) && (miny+1 < maxy))
		{
			for (y = miny+1; y < maxy; y++)
			{
				for (x = minx+1; x < maxx; x++)
				{
					tilepoint = pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y);
					if (tilepoint.selected)
					{ // only selected tilepoints
						pDoc->AddToUndoObj(tilepoint, x, pDoc->m_War3Map.GetMapHeight() - y);
						tilepoint.height = (tilepoint.height + pDoc->m_War3Map.GetTilePoint(x+1,pDoc->m_War3Map.GetMapHeight() - y).height + pDoc->m_War3Map.GetTilePoint(x-1,pDoc->m_War3Map.GetMapHeight() - y).height + pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y - 1).height + pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y + 1).height)/5;
						pDoc->SetModifiedFlag();
						pDoc->m_War3Map.SetTilePoint(x, pDoc->m_War3Map.GetMapHeight() - y, tilepoint);
					}
				}			
			}
		}
		else if (minx+1 < maxx)
		{
			for (y = miny; y <= maxy; y++)
			{
				for (x = minx+1; x < maxx; x++)
				{
					tilepoint = pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y);
					if (tilepoint.selected)
					{ // only selected tilepoints
						pDoc->AddToUndoObj(tilepoint, x, pDoc->m_War3Map.GetMapHeight() - y);
						if (y > miny)
						{
							tilepoint.height = (tilepoint.height + pDoc->m_War3Map.GetTilePoint(x+1,pDoc->m_War3Map.GetMapHeight() - y).height + pDoc->m_War3Map.GetTilePoint(x-1,pDoc->m_War3Map.GetMapHeight() - y).height + pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y - 1).height)/4;
						}
						else if (y < maxy)
						{
							tilepoint.height = (tilepoint.height + pDoc->m_War3Map.GetTilePoint(x+1,pDoc->m_War3Map.GetMapHeight() - y).height + pDoc->m_War3Map.GetTilePoint(x-1,pDoc->m_War3Map.GetMapHeight() - y).height + pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y + 1).height)/4;
						}
						else
						{
							tilepoint.height = (tilepoint.height + pDoc->m_War3Map.GetTilePoint(x+1,pDoc->m_War3Map.GetMapHeight() - y).height + pDoc->m_War3Map.GetTilePoint(x-1,pDoc->m_War3Map.GetMapHeight() - y).height)/3;
						}
						pDoc->SetModifiedFlag();
						pDoc->m_War3Map.SetTilePoint(x, pDoc->m_War3Map.GetMapHeight() - y, tilepoint);
					}
				}			
			}
		}
		else if (miny+1 < maxy)
		{
			for (y = miny+1; y < maxy; y++)
			{
				for (x = minx; x <= maxx; x++)
				{
					tilepoint = pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y);
					if (tilepoint.selected)
					{ // only selected tilepoints
						pDoc->AddToUndoObj(tilepoint, x, pDoc->m_War3Map.GetMapHeight() - y);
						if (x > minx)
						{
							tilepoint.height = (tilepoint.height + pDoc->m_War3Map.GetTilePoint(x-1,pDoc->m_War3Map.GetMapHeight() - y).height + pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y - 1).height + pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y + 1).height)/4;
						}
						else if (x < maxx)
						{
							tilepoint.height = (tilepoint.height + pDoc->m_War3Map.GetTilePoint(x+1,pDoc->m_War3Map.GetMapHeight() - y).height + pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y - 1).height + pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y + 1).height)/4;
						}
						else
						{
							tilepoint.height = (tilepoint.height + pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y - 1).height + pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y + 1).height)/3;
						}
						pDoc->SetModifiedFlag();
						pDoc->m_War3Map.SetTilePoint(x, pDoc->m_War3Map.GetMapHeight() - y, tilepoint);
					}
				}			
			}
		}

		rect.SetRect((minx - 1)*CR_WIDTH, (miny - 1)*CR_HEIGHT, (maxx + 1)*CR_WIDTH, (maxy + 1)*CR_HEIGHT);
		pView->InvalidateTilesBitmap(rect);
		pView->InvalidateBitmapRect(rect);
		pDoc->CloseUndoObj();
	}
	else
	{// no selected tilepoints
		AfxMessageBox("You need to select tilepoints first to use this tool!", MB_OK | MB_APPLMODAL | MB_ICONWARNING);
	}
}

void CToolTilePointsEdit::GroundRand()
{
	CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
	ASSERT_VALID(pTheApp);
	if (!pTheApp)
	{return;}
	CMainFrame *pMainFrame = pTheApp->GetMainFrame();
	if (!pMainFrame)
	{return;}
	CChildFrame *pChildFrame = (CChildFrame*)pMainFrame->GetActiveFrame();
	if (!pChildFrame)
	{return;}
	CW3ZMapEditView *pView = pChildFrame->GetEditView();	
	if (!pView)
	{return;}
	CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pChildFrame->GetActiveDocument();
	if (!pDoc)
	{return;}
	ASSERT_VALID(pDoc);

	CTilePoint tilepoint;
	int x, y, minx, maxx, miny, maxy;
	CRect rect;

	if (4 == pDoc->m_War3Map.mapTilePointSelState)
	{// check if we got selected tilepoints
		CWaitCursor wait;
		pDoc->OpenUndoObj();
		// gets the coordinates of the rectangle that contains the selected tilepoints
		minx = pDoc->m_War3Map.mapTilePointSelFirst.x;
		if (pDoc->m_War3Map.mapTilePointSelSecond.x < minx)
		{
			minx = pDoc->m_War3Map.mapTilePointSelSecond.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.x < minx)
		{
			minx = pDoc->m_War3Map.mapTilePointSelThird.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.x < minx)
		{
			minx = pDoc->m_War3Map.mapTilePointSelForth.x;
		}
		maxx = pDoc->m_War3Map.mapTilePointSelFirst.x;
		if (pDoc->m_War3Map.mapTilePointSelSecond.x > maxx)
		{
			maxx = pDoc->m_War3Map.mapTilePointSelSecond.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.x > maxx)
		{
			maxx = pDoc->m_War3Map.mapTilePointSelThird.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.x > maxx)
		{
			maxx = pDoc->m_War3Map.mapTilePointSelForth.x;
		}
		miny = pDoc->m_War3Map.mapTilePointSelFirst.y;
		if (pDoc->m_War3Map.mapTilePointSelSecond.y < miny)
		{
			miny = pDoc->m_War3Map.mapTilePointSelSecond.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.y < miny)
		{
			miny = pDoc->m_War3Map.mapTilePointSelThird.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.y < miny)
		{
			miny = pDoc->m_War3Map.mapTilePointSelForth.y;
		}
		maxy = pDoc->m_War3Map.mapTilePointSelFirst.y;
		if (pDoc->m_War3Map.mapTilePointSelSecond.y > maxy)
		{
			maxy = pDoc->m_War3Map.mapTilePointSelSecond.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.y > maxy)
		{
			maxy = pDoc->m_War3Map.mapTilePointSelThird.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.y > maxy)
		{
			maxy = pDoc->m_War3Map.mapTilePointSelForth.y;
		}
		for (y = miny; y <= maxy; y++)
		{
			for (x = minx; x <= maxx; x++)
			{
				tilepoint = pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y);
				if (tilepoint.selected)
				{ // only selected tilepoints
					pDoc->AddToUndoObj(tilepoint, x, pDoc->m_War3Map.GetMapHeight() - y);
					tilepoint.height = pMainFrame->GetTilesEditBarRandTilePoint(tilepoint).height;
					pDoc->SetModifiedFlag();
					pDoc->m_War3Map.SetTilePoint(x, pDoc->m_War3Map.GetMapHeight() - y, tilepoint);
				}
			}			
		}

		rect.SetRect((minx - 1)*CR_WIDTH, (miny - 1)*CR_HEIGHT, (maxx + 1)*CR_WIDTH, (maxy + 1)*CR_HEIGHT);
		pView->InvalidateTilesBitmap(rect);
		pView->InvalidateBitmapRect(rect);
		pDoc->CloseUndoObj();
	}
	else
	{// no selected tilepoints
		AfxMessageBox("You need to select tilepoints first to use this tool!", MB_OK | MB_APPLMODAL | MB_ICONWARNING);
	}
}


void CToolTilePointsEdit::WaterSlope()
{
	CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
	ASSERT_VALID(pTheApp);
	if (!pTheApp)
	{return;}
	CMainFrame *pMainFrame = pTheApp->GetMainFrame();
	if (!pMainFrame)
	{return;}
	CChildFrame *pChildFrame = (CChildFrame*)pMainFrame->GetActiveFrame();
	if (!pChildFrame)
	{return;}
	CW3ZMapEditView *pView = pChildFrame->GetEditView();	
	if (!pView)
	{return;}
	CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pChildFrame->GetActiveDocument();
	if (!pDoc)
	{return;}
	ASSERT_VALID(pDoc);

	CTilePoint tilepoint;
	CTilePoint undoTilePoint;
	int x, y, minx, maxx, miny, maxy;
	CTilePoint wTP1, wTP2, wTP3, wTP4;
	CRect rect;
	double dist1, dist2, dist3, dist4, a, b, c, d;

	if (4 == pDoc->m_War3Map.mapTilePointSelState)
	{// check if we got selected tilepoints
		CWaitCursor wait;
		pDoc->OpenUndoObj();
		// gets the coordinates of the rectangle that contains the selected tilepoints
		minx = pDoc->m_War3Map.mapTilePointSelFirst.x;
		if (pDoc->m_War3Map.mapTilePointSelSecond.x < minx)
		{
			minx = pDoc->m_War3Map.mapTilePointSelSecond.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.x < minx)
		{
			minx = pDoc->m_War3Map.mapTilePointSelThird.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.x < minx)
		{
			minx = pDoc->m_War3Map.mapTilePointSelForth.x;
		}
		maxx = pDoc->m_War3Map.mapTilePointSelFirst.x;
		if (pDoc->m_War3Map.mapTilePointSelSecond.x > maxx)
		{
			maxx = pDoc->m_War3Map.mapTilePointSelSecond.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.x > maxx)
		{
			maxx = pDoc->m_War3Map.mapTilePointSelThird.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.x > maxx)
		{
			maxx = pDoc->m_War3Map.mapTilePointSelForth.x;
		}
		miny = pDoc->m_War3Map.mapTilePointSelFirst.y;
		if (pDoc->m_War3Map.mapTilePointSelSecond.y < miny)
		{
			miny = pDoc->m_War3Map.mapTilePointSelSecond.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.y < miny)
		{
			miny = pDoc->m_War3Map.mapTilePointSelThird.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.y < miny)
		{
			miny = pDoc->m_War3Map.mapTilePointSelForth.y;
		}
		maxy = pDoc->m_War3Map.mapTilePointSelFirst.y;
		if (pDoc->m_War3Map.mapTilePointSelSecond.y > maxy)
		{
			maxy = pDoc->m_War3Map.mapTilePointSelSecond.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.y > maxy)
		{
			maxy = pDoc->m_War3Map.mapTilePointSelThird.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.y > maxy)
		{
			maxy = pDoc->m_War3Map.mapTilePointSelForth.y;
		}
		// gets corner points of the selected shape
		wTP1 = pDoc->m_War3Map.GetTilePoint(pDoc->m_War3Map.mapTilePointSelFirst.x,pDoc->m_War3Map.GetMapHeight() - pDoc->m_War3Map.mapTilePointSelFirst.y);
		wTP2 = pDoc->m_War3Map.GetTilePoint(pDoc->m_War3Map.mapTilePointSelSecond.x,pDoc->m_War3Map.GetMapHeight() - pDoc->m_War3Map.mapTilePointSelSecond.y);
		wTP3 = pDoc->m_War3Map.GetTilePoint(pDoc->m_War3Map.mapTilePointSelThird.x,pDoc->m_War3Map.GetMapHeight() - pDoc->m_War3Map.mapTilePointSelThird.y);
		wTP4 = pDoc->m_War3Map.GetTilePoint(pDoc->m_War3Map.mapTilePointSelForth.x,pDoc->m_War3Map.GetMapHeight() - pDoc->m_War3Map.mapTilePointSelForth.y);
		for (y = miny; y <= maxy; y++)
		{
			for (x = minx; x <= maxx; x++)
			{
				tilepoint = pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y);
				if (tilepoint.selected)
				{ // only selected tilepoints
						pDoc->AddToUndoObj(tilepoint, x, pDoc->m_War3Map.GetMapHeight() - y);
						if ((pDoc->m_War3Map.mapTilePointSelFirst.x != pDoc->m_War3Map.mapTilePointSelSecond.x) || (pDoc->m_War3Map.mapTilePointSelFirst.y != pDoc->m_War3Map.mapTilePointSelSecond.y))
						{// c != 0
							//a = distance between the point and wTP1
							//b = distance between the point and wTP2
							//c = distance between the wTP1 and wTP2
							//d = distance between wTP1 and the normal to (wTP1;wTP2) that passes by the point
							//dist1 = distance between the point and the ligne (wTP1;wTP2)
							//dist2 = calculated height of the intersection of (wTP1;wTP2) and the normal that passes by the point
							a = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelFirst.x)*(x - pDoc->m_War3Map.mapTilePointSelFirst.x) + (y - pDoc->m_War3Map.mapTilePointSelFirst.y)*(y - pDoc->m_War3Map.mapTilePointSelFirst.y)));
							b = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelSecond.x)*(x - pDoc->m_War3Map.mapTilePointSelSecond.x) + (y - pDoc->m_War3Map.mapTilePointSelSecond.y)*(y - pDoc->m_War3Map.mapTilePointSelSecond.y)));
							c = sqrt((double)((pDoc->m_War3Map.mapTilePointSelFirst.x - pDoc->m_War3Map.mapTilePointSelSecond.x)*(pDoc->m_War3Map.mapTilePointSelFirst.x - pDoc->m_War3Map.mapTilePointSelSecond.x) + (pDoc->m_War3Map.mapTilePointSelFirst.y - pDoc->m_War3Map.mapTilePointSelSecond.y)*(pDoc->m_War3Map.mapTilePointSelFirst.y - pDoc->m_War3Map.mapTilePointSelSecond.y)));
							d = (a*a - b*b + c*c) / (2*c);
							dist1 = sqrt(a*a - d*d);
							dist2 = wTP1.waterlevel*(c-d)/c + wTP2.waterlevel*d/c;
						}
						else
						{// wTP1 = wTP2
							//dist1 = distance between wTP1 and the point
							//dist2 = height of wTP1
							dist1 = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelFirst.x)*(x - pDoc->m_War3Map.mapTilePointSelFirst.x) + (y - pDoc->m_War3Map.mapTilePointSelFirst.y)*(y - pDoc->m_War3Map.mapTilePointSelFirst.y)));
							dist2 = wTP1.waterlevel;
						}
						if ((pDoc->m_War3Map.mapTilePointSelThird.x != pDoc->m_War3Map.mapTilePointSelForth.x) || (pDoc->m_War3Map.mapTilePointSelThird.y != pDoc->m_War3Map.mapTilePointSelForth.y))
						{// c != 0
							//a = distance between the point and wTP3
							//b = distance between the point and wTP4
							//c = distance between the wTP3 and wTP4
							//d = distance between wTP3 and the normal to (wTP3;wTP4) that passes by the point
							//dist1 = distance between the point and the ligne (wTP3;wTP4)
							//dist2 = calculated height of the intersection of (wTP3;wTP4) and the normal that passes by the point
							a = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelThird.x)*(x - pDoc->m_War3Map.mapTilePointSelThird.x) + (y - pDoc->m_War3Map.mapTilePointSelThird.y)*(y - pDoc->m_War3Map.mapTilePointSelThird.y)));
							b = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelForth.x)*(x - pDoc->m_War3Map.mapTilePointSelForth.x) + (y - pDoc->m_War3Map.mapTilePointSelForth.y)*(y - pDoc->m_War3Map.mapTilePointSelForth.y)));
							c = sqrt((double)((pDoc->m_War3Map.mapTilePointSelThird.x - pDoc->m_War3Map.mapTilePointSelForth.x)*(pDoc->m_War3Map.mapTilePointSelThird.x - pDoc->m_War3Map.mapTilePointSelForth.x) + (pDoc->m_War3Map.mapTilePointSelThird.y - pDoc->m_War3Map.mapTilePointSelForth.y)*(pDoc->m_War3Map.mapTilePointSelThird.y - pDoc->m_War3Map.mapTilePointSelForth.y)));
							d = (a*a - b*b + c*c) / (2*c);
							dist3 = sqrt(a*a - d*d);
							dist4 = wTP3.waterlevel*(c-d)/c + wTP4.waterlevel*d/c;
						}
						else
						{// wTP3 = wTP4
							//dist3 = distance between wTP3 and the point
							//dist4 = height of wTP3
							dist3 = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelThird.x)*(x - pDoc->m_War3Map.mapTilePointSelThird.x) + (y - pDoc->m_War3Map.mapTilePointSelThird.y)*(y - pDoc->m_War3Map.mapTilePointSelThird.y)));
							dist4 = wTP3.waterlevel;
						}
						if ((dist1) && (dist3))
						{// point is neither between wTP1 and wTP2 nor between wTP3 and wTP4
							// inverts factors and calculate the height
							dist1 = 1/dist1;
							dist3 = 1/dist3;
							tilepoint.waterlevel = (int)((dist1*dist2 + dist3*dist4)/(dist1+dist3));
						}
						else if (dist3)
						{// point between wTP1 and wTP2 only
							dist1 = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelFirst.x)*(x - pDoc->m_War3Map.mapTilePointSelFirst.x) + (y - pDoc->m_War3Map.mapTilePointSelFirst.y)*(y - pDoc->m_War3Map.mapTilePointSelFirst.y)));
							dist2 = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelSecond.x)*(x - pDoc->m_War3Map.mapTilePointSelSecond.x) + (y - pDoc->m_War3Map.mapTilePointSelSecond.y)*(y - pDoc->m_War3Map.mapTilePointSelSecond.y)));
							if ((dist1) && (dist2))
							{// point != wTP1 && point != wTP2
								dist1 = 1/dist1;
								dist2 = 1/dist2;
								tilepoint.waterlevel = (int)((wTP1.waterlevel*dist1 + wTP2.waterlevel*dist2)/(dist1+dist2));
							}// else don't change the point height because it's wTP1 or wTP2
						}
						else if (dist1)
						{ // point between wTP3 and wTP4 only
							dist3 = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelThird.x)*(x - pDoc->m_War3Map.mapTilePointSelThird.x) + (y - pDoc->m_War3Map.mapTilePointSelThird.y)*(y - pDoc->m_War3Map.mapTilePointSelThird.y)));
							dist4 = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelForth.x)*(x - pDoc->m_War3Map.mapTilePointSelForth.x) + (y - pDoc->m_War3Map.mapTilePointSelForth.y)*(y - pDoc->m_War3Map.mapTilePointSelForth.y)));
							if ((dist3) && (dist4))
							{// point != wTP3 && point != wTP4
								dist3 = 1/dist3;
								dist4 = 1/dist4;
								tilepoint.waterlevel = (int)((wTP3.waterlevel*dist3 + wTP4.waterlevel*dist4)/(dist3+dist4));
							}// else don't change the point height because it's wTP3 or wTP4
						}//	else wTP1 = wTP2 = wTP3 = wTP4 so do nothing
				}
				tilepoint.flags |= TILE_FL_WATER;
				pDoc->SetModifiedFlag();
				pDoc->m_War3Map.SetTilePoint(x, pDoc->m_War3Map.GetMapHeight() - y, tilepoint);
			}			
		}

		rect.SetRect((minx - 1)*CR_WIDTH, (miny - 1)*CR_HEIGHT, (maxx + 1)*CR_WIDTH, (maxy + 1)*CR_HEIGHT);
		pView->InvalidateTilesBitmap(rect);
		pView->InvalidateBitmapRect(rect);
		pDoc->CloseUndoObj();
	}
	else
	{// no selected tilepoints
		AfxMessageBox("You need to select tilepoints first to use this tool!", MB_OK | MB_APPLMODAL | MB_ICONWARNING);
	}
}

void CToolTilePointsEdit::WaterBendUp()
{
	CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
	ASSERT_VALID(pTheApp);
	if (!pTheApp)
	{return;}
	CMainFrame *pMainFrame = pTheApp->GetMainFrame();
	if (!pMainFrame)
	{return;}
	CChildFrame *pChildFrame = (CChildFrame*)pMainFrame->GetActiveFrame();
	if (!pChildFrame)
	{return;}
	CW3ZMapEditView *pView = pChildFrame->GetEditView();	
	if (!pView)
	{return;}
	CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pChildFrame->GetActiveDocument();
	if (!pDoc)
	{return;}
	ASSERT_VALID(pDoc);

	CTilePoint tilepoint;
	CTilePoint undoTilePoint;
	int x, y, minx, maxx, miny, maxy;
	CTilePoint wTP1, wTP2, wTP3, wTP4;
	CRect rect;
	double dist1, dist2, dist3, dist4, a, b, c, d;

	if (4 == pDoc->m_War3Map.mapTilePointSelState)
	{// check if we got selected tilepoints
		CWaitCursor wait;
		pDoc->OpenUndoObj();
		// gets the coordinates of the rectangle that contains the selected tilepoints
		minx = pDoc->m_War3Map.mapTilePointSelFirst.x;
		if (pDoc->m_War3Map.mapTilePointSelSecond.x < minx)
		{
			minx = pDoc->m_War3Map.mapTilePointSelSecond.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.x < minx)
		{
			minx = pDoc->m_War3Map.mapTilePointSelThird.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.x < minx)
		{
			minx = pDoc->m_War3Map.mapTilePointSelForth.x;
		}
		maxx = pDoc->m_War3Map.mapTilePointSelFirst.x;
		if (pDoc->m_War3Map.mapTilePointSelSecond.x > maxx)
		{
			maxx = pDoc->m_War3Map.mapTilePointSelSecond.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.x > maxx)
		{
			maxx = pDoc->m_War3Map.mapTilePointSelThird.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.x > maxx)
		{
			maxx = pDoc->m_War3Map.mapTilePointSelForth.x;
		}
		miny = pDoc->m_War3Map.mapTilePointSelFirst.y;
		if (pDoc->m_War3Map.mapTilePointSelSecond.y < miny)
		{
			miny = pDoc->m_War3Map.mapTilePointSelSecond.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.y < miny)
		{
			miny = pDoc->m_War3Map.mapTilePointSelThird.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.y < miny)
		{
			miny = pDoc->m_War3Map.mapTilePointSelForth.y;
		}
		maxy = pDoc->m_War3Map.mapTilePointSelFirst.y;
		if (pDoc->m_War3Map.mapTilePointSelSecond.y > maxy)
		{
			maxy = pDoc->m_War3Map.mapTilePointSelSecond.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.y > maxy)
		{
			maxy = pDoc->m_War3Map.mapTilePointSelThird.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.y > maxy)
		{
			maxy = pDoc->m_War3Map.mapTilePointSelForth.y;
		}
		// gets corner points of the selected shape
		wTP1 = pDoc->m_War3Map.GetTilePoint(pDoc->m_War3Map.mapTilePointSelFirst.x,pDoc->m_War3Map.GetMapHeight() - pDoc->m_War3Map.mapTilePointSelFirst.y);
		wTP2 = pDoc->m_War3Map.GetTilePoint(pDoc->m_War3Map.mapTilePointSelSecond.x,pDoc->m_War3Map.GetMapHeight() - pDoc->m_War3Map.mapTilePointSelSecond.y);
		wTP3 = pDoc->m_War3Map.GetTilePoint(pDoc->m_War3Map.mapTilePointSelThird.x,pDoc->m_War3Map.GetMapHeight() - pDoc->m_War3Map.mapTilePointSelThird.y);
		wTP4 = pDoc->m_War3Map.GetTilePoint(pDoc->m_War3Map.mapTilePointSelForth.x,pDoc->m_War3Map.GetMapHeight() - pDoc->m_War3Map.mapTilePointSelForth.y);
		for (y = miny; y <= maxy; y++)
		{
			for (x = minx; x <= maxx; x++)
			{
				tilepoint = pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y);
				if (tilepoint.selected)
				{ // only selected tilepoints
						pDoc->AddToUndoObj(tilepoint, x, pDoc->m_War3Map.GetMapHeight() - y);
						if ((pDoc->m_War3Map.mapTilePointSelFirst.x != pDoc->m_War3Map.mapTilePointSelSecond.x) || (pDoc->m_War3Map.mapTilePointSelFirst.y != pDoc->m_War3Map.mapTilePointSelSecond.y))
						{// c != 0
							//a = distance between the point and wTP1
							//b = distance between the point and wTP2
							//c = distance between the wTP1 and wTP2
							//d = distance between wTP1 and the normal to (wTP1;wTP2) that passes by the point
							//dist1 = distance between the point and the ligne (wTP1;wTP2)
							//dist2 = calculated height of the intersection of (wTP1;wTP2) and the normal that passes by the point
							a = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelFirst.x)*(x - pDoc->m_War3Map.mapTilePointSelFirst.x) + (y - pDoc->m_War3Map.mapTilePointSelFirst.y)*(y - pDoc->m_War3Map.mapTilePointSelFirst.y)));
							b = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelSecond.x)*(x - pDoc->m_War3Map.mapTilePointSelSecond.x) + (y - pDoc->m_War3Map.mapTilePointSelSecond.y)*(y - pDoc->m_War3Map.mapTilePointSelSecond.y)));
							c = sqrt((double)((pDoc->m_War3Map.mapTilePointSelFirst.x - pDoc->m_War3Map.mapTilePointSelSecond.x)*(pDoc->m_War3Map.mapTilePointSelFirst.x - pDoc->m_War3Map.mapTilePointSelSecond.x) + (pDoc->m_War3Map.mapTilePointSelFirst.y - pDoc->m_War3Map.mapTilePointSelSecond.y)*(pDoc->m_War3Map.mapTilePointSelFirst.y - pDoc->m_War3Map.mapTilePointSelSecond.y)));
							d = (a*a - b*b + c*c) / (2*c);
							dist1 = sqrt(a*a - d*d);
							dist2 = wTP1.waterlevel*(c-d)/c + wTP2.waterlevel*d/c;
						}
						else
						{// wTP1 = wTP2
							//dist1 = distance between wTP1 and the point
							//dist2 = height of wTP1
							dist1 = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelFirst.x)*(x - pDoc->m_War3Map.mapTilePointSelFirst.x) + (y - pDoc->m_War3Map.mapTilePointSelFirst.y)*(y - pDoc->m_War3Map.mapTilePointSelFirst.y)));
							dist2 = wTP1.waterlevel;
						}
						if ((pDoc->m_War3Map.mapTilePointSelThird.x != pDoc->m_War3Map.mapTilePointSelForth.x) || (pDoc->m_War3Map.mapTilePointSelThird.y != pDoc->m_War3Map.mapTilePointSelForth.y))
						{// c != 0
							//a = distance between the point and wTP3
							//b = distance between the point and wTP4
							//c = distance between the wTP3 and wTP4
							//d = distance between wTP3 and the normal to (wTP3;wTP4) that passes by the point
							//dist1 = distance between the point and the ligne (wTP3;wTP4)
							//dist2 = calculated height of the intersection of (wTP3;wTP4) and the normal that passes by the point
							a = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelThird.x)*(x - pDoc->m_War3Map.mapTilePointSelThird.x) + (y - pDoc->m_War3Map.mapTilePointSelThird.y)*(y - pDoc->m_War3Map.mapTilePointSelThird.y)));
							b = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelForth.x)*(x - pDoc->m_War3Map.mapTilePointSelForth.x) + (y - pDoc->m_War3Map.mapTilePointSelForth.y)*(y - pDoc->m_War3Map.mapTilePointSelForth.y)));
							c = sqrt((double)((pDoc->m_War3Map.mapTilePointSelThird.x - pDoc->m_War3Map.mapTilePointSelForth.x)*(pDoc->m_War3Map.mapTilePointSelThird.x - pDoc->m_War3Map.mapTilePointSelForth.x) + (pDoc->m_War3Map.mapTilePointSelThird.y - pDoc->m_War3Map.mapTilePointSelForth.y)*(pDoc->m_War3Map.mapTilePointSelThird.y - pDoc->m_War3Map.mapTilePointSelForth.y)));
							d = (a*a - b*b + c*c) / (2*c);
							dist3 = sqrt(a*a - d*d);
							dist4 = wTP3.waterlevel*(c-d)/c + wTP4.waterlevel*d/c;
						}
						else
						{// wTP3 = wTP4
							//dist3 = distance between wTP3 and the point
							//dist4 = height of wTP3
							dist3 = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelThird.x)*(x - pDoc->m_War3Map.mapTilePointSelThird.x) + (y - pDoc->m_War3Map.mapTilePointSelThird.y)*(y - pDoc->m_War3Map.mapTilePointSelThird.y)));
							dist4 = wTP3.waterlevel;
						}
						if ((dist1) && (dist3))
						{// point is neither between wTP1 and wTP2 nor between wTP3 and wTP4
							// inverts factors and calculate the height
							dist1 = 1/dist1;
							dist3 = 1/(dist3*dist3); // bend up
							tilepoint.waterlevel = (int)((dist1*dist2 + dist3*dist4)/(dist1+dist3));
						}
						else if (dist3)
						{// point between wTP1 and wTP2 only
							dist1 = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelFirst.x)*(x - pDoc->m_War3Map.mapTilePointSelFirst.x) + (y - pDoc->m_War3Map.mapTilePointSelFirst.y)*(y - pDoc->m_War3Map.mapTilePointSelFirst.y)));
							dist2 = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelSecond.x)*(x - pDoc->m_War3Map.mapTilePointSelSecond.x) + (y - pDoc->m_War3Map.mapTilePointSelSecond.y)*(y - pDoc->m_War3Map.mapTilePointSelSecond.y)));
							if ((dist1) && (dist2))
							{// point != wTP1 && point != wTP2
								dist1 = 1/dist1;
								dist2 = 1/dist2;
								tilepoint.waterlevel = (int)((wTP1.waterlevel*dist1 + wTP2.waterlevel*dist2)/(dist1+dist2));
							}// else don't change the point height because it's wTP1 or wTP2
						}
						else if (dist1)
						{ // point between wTP3 and wTP4 only
							dist3 = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelThird.x)*(x - pDoc->m_War3Map.mapTilePointSelThird.x) + (y - pDoc->m_War3Map.mapTilePointSelThird.y)*(y - pDoc->m_War3Map.mapTilePointSelThird.y)));
							dist4 = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelForth.x)*(x - pDoc->m_War3Map.mapTilePointSelForth.x) + (y - pDoc->m_War3Map.mapTilePointSelForth.y)*(y - pDoc->m_War3Map.mapTilePointSelForth.y)));
							if ((dist3) && (dist4))
							{// point != wTP3 && point != wTP4
								dist3 = 1/dist3;
								dist4 = 1/dist4;
								tilepoint.waterlevel = (int)((wTP3.waterlevel*dist3 + wTP4.waterlevel*dist4)/(dist3+dist4));
							}// else don't change the point height because it's wTP3 or wTP4
						}//	else wTP1 = wTP2 = wTP3 = wTP4 so do nothing
				}
				tilepoint.flags |= TILE_FL_WATER;
				pDoc->SetModifiedFlag();
				pDoc->m_War3Map.SetTilePoint(x, pDoc->m_War3Map.GetMapHeight() - y, tilepoint);
			}			
		}

		rect.SetRect((minx - 1)*CR_WIDTH, (miny - 1)*CR_HEIGHT, (maxx + 1)*CR_WIDTH, (maxy + 1)*CR_HEIGHT);
		pView->InvalidateTilesBitmap(rect);
		pView->InvalidateBitmapRect(rect);
		pDoc->CloseUndoObj();
	}
	else
	{// no selected tilepoints
		AfxMessageBox("You need to select tilepoints first to use this tool!", MB_OK | MB_APPLMODAL | MB_ICONWARNING);
	}
}

void CToolTilePointsEdit::WaterBendDown()
{
	CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
	ASSERT_VALID(pTheApp);
	if (!pTheApp)
	{return;}
	CMainFrame *pMainFrame = pTheApp->GetMainFrame();
	if (!pMainFrame)
	{return;}
	CChildFrame *pChildFrame = (CChildFrame*)pMainFrame->GetActiveFrame();
	if (!pChildFrame)
	{return;}
	CW3ZMapEditView *pView = pChildFrame->GetEditView();	
	if (!pView)
	{return;}
	CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pChildFrame->GetActiveDocument();
	if (!pDoc)
	{return;}
	ASSERT_VALID(pDoc);

	CTilePoint tilepoint;
	CTilePoint undoTilePoint;
	int x, y, minx, maxx, miny, maxy;
	CTilePoint wTP1, wTP2, wTP3, wTP4;
	CRect rect;
	double dist1, dist2, dist3, dist4, a, b, c, d;

	if (4 == pDoc->m_War3Map.mapTilePointSelState)
	{// check if we got selected tilepoints
		CWaitCursor wait;
		pDoc->OpenUndoObj();
		// gets the coordinates of the rectangle that contains the selected tilepoints
		minx = pDoc->m_War3Map.mapTilePointSelFirst.x;
		if (pDoc->m_War3Map.mapTilePointSelSecond.x < minx)
		{
			minx = pDoc->m_War3Map.mapTilePointSelSecond.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.x < minx)
		{
			minx = pDoc->m_War3Map.mapTilePointSelThird.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.x < minx)
		{
			minx = pDoc->m_War3Map.mapTilePointSelForth.x;
		}
		maxx = pDoc->m_War3Map.mapTilePointSelFirst.x;
		if (pDoc->m_War3Map.mapTilePointSelSecond.x > maxx)
		{
			maxx = pDoc->m_War3Map.mapTilePointSelSecond.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.x > maxx)
		{
			maxx = pDoc->m_War3Map.mapTilePointSelThird.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.x > maxx)
		{
			maxx = pDoc->m_War3Map.mapTilePointSelForth.x;
		}
		miny = pDoc->m_War3Map.mapTilePointSelFirst.y;
		if (pDoc->m_War3Map.mapTilePointSelSecond.y < miny)
		{
			miny = pDoc->m_War3Map.mapTilePointSelSecond.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.y < miny)
		{
			miny = pDoc->m_War3Map.mapTilePointSelThird.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.y < miny)
		{
			miny = pDoc->m_War3Map.mapTilePointSelForth.y;
		}
		maxy = pDoc->m_War3Map.mapTilePointSelFirst.y;
		if (pDoc->m_War3Map.mapTilePointSelSecond.y > maxy)
		{
			maxy = pDoc->m_War3Map.mapTilePointSelSecond.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.y > maxy)
		{
			maxy = pDoc->m_War3Map.mapTilePointSelThird.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.y > maxy)
		{
			maxy = pDoc->m_War3Map.mapTilePointSelForth.y;
		}
		// gets corner points of the selected shape
		wTP1 = pDoc->m_War3Map.GetTilePoint(pDoc->m_War3Map.mapTilePointSelFirst.x,pDoc->m_War3Map.GetMapHeight() - pDoc->m_War3Map.mapTilePointSelFirst.y);
		wTP2 = pDoc->m_War3Map.GetTilePoint(pDoc->m_War3Map.mapTilePointSelSecond.x,pDoc->m_War3Map.GetMapHeight() - pDoc->m_War3Map.mapTilePointSelSecond.y);
		wTP3 = pDoc->m_War3Map.GetTilePoint(pDoc->m_War3Map.mapTilePointSelThird.x,pDoc->m_War3Map.GetMapHeight() - pDoc->m_War3Map.mapTilePointSelThird.y);
		wTP4 = pDoc->m_War3Map.GetTilePoint(pDoc->m_War3Map.mapTilePointSelForth.x,pDoc->m_War3Map.GetMapHeight() - pDoc->m_War3Map.mapTilePointSelForth.y);
		for (y = miny; y <= maxy; y++)
		{
			for (x = minx; x <= maxx; x++)
			{
				tilepoint = pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y);
				if (tilepoint.selected)
				{ // only selected tilepoints
						pDoc->AddToUndoObj(tilepoint, x, pDoc->m_War3Map.GetMapHeight() - y);
						if ((pDoc->m_War3Map.mapTilePointSelFirst.x != pDoc->m_War3Map.mapTilePointSelSecond.x) || (pDoc->m_War3Map.mapTilePointSelFirst.y != pDoc->m_War3Map.mapTilePointSelSecond.y))
						{// c != 0
							//a = distance between the point and wTP1
							//b = distance between the point and wTP2
							//c = distance between the wTP1 and wTP2
							//d = distance between wTP1 and the normal to (wTP1;wTP2) that passes by the point
							//dist1 = distance between the point and the ligne (wTP1;wTP2)
							//dist2 = calculated height of the intersection of (wTP1;wTP2) and the normal that passes by the point
							a = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelFirst.x)*(x - pDoc->m_War3Map.mapTilePointSelFirst.x) + (y - pDoc->m_War3Map.mapTilePointSelFirst.y)*(y - pDoc->m_War3Map.mapTilePointSelFirst.y)));
							b = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelSecond.x)*(x - pDoc->m_War3Map.mapTilePointSelSecond.x) + (y - pDoc->m_War3Map.mapTilePointSelSecond.y)*(y - pDoc->m_War3Map.mapTilePointSelSecond.y)));
							c = sqrt((double)((pDoc->m_War3Map.mapTilePointSelFirst.x - pDoc->m_War3Map.mapTilePointSelSecond.x)*(pDoc->m_War3Map.mapTilePointSelFirst.x - pDoc->m_War3Map.mapTilePointSelSecond.x) + (pDoc->m_War3Map.mapTilePointSelFirst.y - pDoc->m_War3Map.mapTilePointSelSecond.y)*(pDoc->m_War3Map.mapTilePointSelFirst.y - pDoc->m_War3Map.mapTilePointSelSecond.y)));
							d = (a*a - b*b + c*c) / (2*c);
							dist1 = sqrt(a*a - d*d);
							dist2 = wTP1.waterlevel*(c-d)/c + wTP2.waterlevel*d/c;
						}
						else
						{// wTP1 = wTP2
							//dist1 = distance between wTP1 and the point
							//dist2 = height of wTP1
							dist1 = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelFirst.x)*(x - pDoc->m_War3Map.mapTilePointSelFirst.x) + (y - pDoc->m_War3Map.mapTilePointSelFirst.y)*(y - pDoc->m_War3Map.mapTilePointSelFirst.y)));
							dist2 = wTP1.waterlevel;
						}
						if ((pDoc->m_War3Map.mapTilePointSelThird.x != pDoc->m_War3Map.mapTilePointSelForth.x) || (pDoc->m_War3Map.mapTilePointSelThird.y != pDoc->m_War3Map.mapTilePointSelForth.y))
						{// c != 0
							//a = distance between the point and wTP3
							//b = distance between the point and wTP4
							//c = distance between the wTP3 and wTP4
							//d = distance between wTP3 and the normal to (wTP3;wTP4) that passes by the point
							//dist1 = distance between the point and the ligne (wTP3;wTP4)
							//dist2 = calculated height of the intersection of (wTP3;wTP4) and the normal that passes by the point
							a = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelThird.x)*(x - pDoc->m_War3Map.mapTilePointSelThird.x) + (y - pDoc->m_War3Map.mapTilePointSelThird.y)*(y - pDoc->m_War3Map.mapTilePointSelThird.y)));
							b = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelForth.x)*(x - pDoc->m_War3Map.mapTilePointSelForth.x) + (y - pDoc->m_War3Map.mapTilePointSelForth.y)*(y - pDoc->m_War3Map.mapTilePointSelForth.y)));
							c = sqrt((double)((pDoc->m_War3Map.mapTilePointSelThird.x - pDoc->m_War3Map.mapTilePointSelForth.x)*(pDoc->m_War3Map.mapTilePointSelThird.x - pDoc->m_War3Map.mapTilePointSelForth.x) + (pDoc->m_War3Map.mapTilePointSelThird.y - pDoc->m_War3Map.mapTilePointSelForth.y)*(pDoc->m_War3Map.mapTilePointSelThird.y - pDoc->m_War3Map.mapTilePointSelForth.y)));
							d = (a*a - b*b + c*c) / (2*c);
							dist3 = sqrt(a*a - d*d);
							dist4 = wTP3.waterlevel*(c-d)/c + wTP4.waterlevel*d/c;
						}
						else
						{// wTP3 = wTP4
							//dist3 = distance between wTP3 and the point
							//dist4 = height of wTP3
							dist3 = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelThird.x)*(x - pDoc->m_War3Map.mapTilePointSelThird.x) + (y - pDoc->m_War3Map.mapTilePointSelThird.y)*(y - pDoc->m_War3Map.mapTilePointSelThird.y)));
							dist4 = wTP3.waterlevel;
						}
						if ((dist1) && (dist3))
						{// point is neither between wTP1 and wTP2 nor between wTP3 and wTP4
							// inverts factors and calculate the height
							dist1 = 1/(dist1*dist1); // bend down
							dist3 = 1/dist3;
							tilepoint.waterlevel = (int)((dist1*dist2 + dist3*dist4)/(dist1+dist3));
						}
						else if (dist3)
						{// point between wTP1 and wTP2 only
							dist1 = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelFirst.x)*(x - pDoc->m_War3Map.mapTilePointSelFirst.x) + (y - pDoc->m_War3Map.mapTilePointSelFirst.y)*(y - pDoc->m_War3Map.mapTilePointSelFirst.y)));
							dist2 = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelSecond.x)*(x - pDoc->m_War3Map.mapTilePointSelSecond.x) + (y - pDoc->m_War3Map.mapTilePointSelSecond.y)*(y - pDoc->m_War3Map.mapTilePointSelSecond.y)));
							if ((dist1) && (dist2))
							{// point != wTP1 && point != wTP2
								dist1 = 1/dist1;
								dist2 = 1/dist2;
								tilepoint.waterlevel = (int)((wTP1.waterlevel*dist1 + wTP2.waterlevel*dist2)/(dist1+dist2));
							}// else don't change the point height because it's wTP1 or wTP2
						}
						else if (dist1)
						{ // point between wTP3 and wTP4 only
							dist3 = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelThird.x)*(x - pDoc->m_War3Map.mapTilePointSelThird.x) + (y - pDoc->m_War3Map.mapTilePointSelThird.y)*(y - pDoc->m_War3Map.mapTilePointSelThird.y)));
							dist4 = sqrt((double)((x - pDoc->m_War3Map.mapTilePointSelForth.x)*(x - pDoc->m_War3Map.mapTilePointSelForth.x) + (y - pDoc->m_War3Map.mapTilePointSelForth.y)*(y - pDoc->m_War3Map.mapTilePointSelForth.y)));
							if ((dist3) && (dist4))
							{// point != wTP3 && point != wTP4
								dist3 = 1/dist3;
								dist4 = 1/dist4;
								tilepoint.waterlevel = (int)((wTP3.waterlevel*dist3 + wTP4.waterlevel*dist4)/(dist3+dist4));
							}// else don't change the point height because it's wTP3 or wTP4
						}//	else wTP1 = wTP2 = wTP3 = wTP4 so do nothing
				}
				tilepoint.flags |= TILE_FL_WATER;
				pDoc->SetModifiedFlag();
				pDoc->m_War3Map.SetTilePoint(x, pDoc->m_War3Map.GetMapHeight() - y, tilepoint);
			}			
		}

		rect.SetRect((minx - 1)*CR_WIDTH, (miny - 1)*CR_HEIGHT, (maxx + 1)*CR_WIDTH, (maxy + 1)*CR_HEIGHT);
		pView->InvalidateTilesBitmap(rect);
		pView->InvalidateBitmapRect(rect);
		pDoc->CloseUndoObj();
	}
	else
	{// no selected tilepoints
		AfxMessageBox("You need to select tilepoints first to use this tool!", MB_OK | MB_APPLMODAL | MB_ICONWARNING);
	}
}

void CToolTilePointsEdit::WaterFlat()
{
	CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
	ASSERT_VALID(pTheApp);
	if (!pTheApp)
	{return;}
	CMainFrame *pMainFrame = pTheApp->GetMainFrame();
	if (!pMainFrame)
	{return;}
	CChildFrame *pChildFrame = (CChildFrame*)pMainFrame->GetActiveFrame();
	if (!pChildFrame)
	{return;}
	CW3ZMapEditView *pView = pChildFrame->GetEditView();	
	if (!pView)
	{return;}
	CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pChildFrame->GetActiveDocument();
	if (!pDoc)
	{return;}
	ASSERT_VALID(pDoc);

	CTilePoint tilepoint;
	int x, y, minx, maxx, miny, maxy;
	CRect rect;

	if (4 == pDoc->m_War3Map.mapTilePointSelState)
	{// check if we got selected tilepoints
		CWaitCursor wait;
		pDoc->OpenUndoObj();
		// gets the coordinates of the rectangle that contains the selected tilepoints
		minx = pDoc->m_War3Map.mapTilePointSelFirst.x;
		if (pDoc->m_War3Map.mapTilePointSelSecond.x < minx)
		{
			minx = pDoc->m_War3Map.mapTilePointSelSecond.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.x < minx)
		{
			minx = pDoc->m_War3Map.mapTilePointSelThird.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.x < minx)
		{
			minx = pDoc->m_War3Map.mapTilePointSelForth.x;
		}
		maxx = pDoc->m_War3Map.mapTilePointSelFirst.x;
		if (pDoc->m_War3Map.mapTilePointSelSecond.x > maxx)
		{
			maxx = pDoc->m_War3Map.mapTilePointSelSecond.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.x > maxx)
		{
			maxx = pDoc->m_War3Map.mapTilePointSelThird.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.x > maxx)
		{
			maxx = pDoc->m_War3Map.mapTilePointSelForth.x;
		}
		miny = pDoc->m_War3Map.mapTilePointSelFirst.y;
		if (pDoc->m_War3Map.mapTilePointSelSecond.y < miny)
		{
			miny = pDoc->m_War3Map.mapTilePointSelSecond.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.y < miny)
		{
			miny = pDoc->m_War3Map.mapTilePointSelThird.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.y < miny)
		{
			miny = pDoc->m_War3Map.mapTilePointSelForth.y;
		}
		maxy = pDoc->m_War3Map.mapTilePointSelFirst.y;
		if (pDoc->m_War3Map.mapTilePointSelSecond.y > maxy)
		{
			maxy = pDoc->m_War3Map.mapTilePointSelSecond.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.y > maxy)
		{
			maxy = pDoc->m_War3Map.mapTilePointSelThird.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.y > maxy)
		{
			maxy = pDoc->m_War3Map.mapTilePointSelForth.y;
		}
		// reduced range
		if ((minx+1 < maxx) && (miny+1 < maxy))
		{
			for (y = miny+1; y < maxy; y++)
			{
				for (x = minx+1; x < maxx; x++)
				{
					tilepoint = pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y);
					if (tilepoint.selected)
					{ // only selected tilepoints
						pDoc->AddToUndoObj(tilepoint, x, pDoc->m_War3Map.GetMapHeight() - y);
						tilepoint.waterlevel = (tilepoint.waterlevel + pDoc->m_War3Map.GetTilePoint(x+1,pDoc->m_War3Map.GetMapHeight() - y).waterlevel + pDoc->m_War3Map.GetTilePoint(x-1,pDoc->m_War3Map.GetMapHeight() - y).waterlevel + pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y - 1).waterlevel + pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y + 1).waterlevel)/5;
						pDoc->SetModifiedFlag();
						pDoc->m_War3Map.SetTilePoint(x, pDoc->m_War3Map.GetMapHeight() - y, tilepoint);
					}
				}			
			}
		}
		else if (minx+1 < maxx)
		{
			for (y = miny; y <= maxy; y++)
			{
				for (x = minx+1; x < maxx; x++)
				{
					tilepoint = pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y);
					if (tilepoint.selected)
					{ // only selected tilepoints
						pDoc->AddToUndoObj(tilepoint, x, pDoc->m_War3Map.GetMapHeight() - y);
						if (y > miny)
						{
							tilepoint.waterlevel = (tilepoint.waterlevel + pDoc->m_War3Map.GetTilePoint(x+1,pDoc->m_War3Map.GetMapHeight() - y).waterlevel + pDoc->m_War3Map.GetTilePoint(x-1,pDoc->m_War3Map.GetMapHeight() - y).waterlevel + pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y - 1).waterlevel)/4;
						}
						else if (y < maxy)
						{
							tilepoint.waterlevel = (tilepoint.waterlevel + pDoc->m_War3Map.GetTilePoint(x+1,pDoc->m_War3Map.GetMapHeight() - y).waterlevel + pDoc->m_War3Map.GetTilePoint(x-1,pDoc->m_War3Map.GetMapHeight() - y).waterlevel + pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y + 1).waterlevel)/4;
						}
						else
						{
							tilepoint.waterlevel = (tilepoint.waterlevel + pDoc->m_War3Map.GetTilePoint(x+1,pDoc->m_War3Map.GetMapHeight() - y).waterlevel + pDoc->m_War3Map.GetTilePoint(x-1,pDoc->m_War3Map.GetMapHeight() - y).waterlevel)/3;
						}
						pDoc->SetModifiedFlag();
						pDoc->m_War3Map.SetTilePoint(x, pDoc->m_War3Map.GetMapHeight() - y, tilepoint);
					}
				}			
			}
		}
		else if (miny+1 < maxy)
		{
			for (y = miny+1; y < maxy; y++)
			{
				for (x = minx; x <= maxx; x++)
				{
					tilepoint = pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y);
					if (tilepoint.selected)
					{ // only selected tilepoints
						pDoc->AddToUndoObj(tilepoint, x, pDoc->m_War3Map.GetMapHeight() - y);
						if (x > minx)
						{
							tilepoint.waterlevel = (tilepoint.waterlevel + pDoc->m_War3Map.GetTilePoint(x-1,pDoc->m_War3Map.GetMapHeight() - y).waterlevel + pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y - 1).waterlevel + pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y + 1).waterlevel)/4;
						}
						else if (x < maxx)
						{
							tilepoint.waterlevel = (tilepoint.waterlevel + pDoc->m_War3Map.GetTilePoint(x+1,pDoc->m_War3Map.GetMapHeight() - y).waterlevel + pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y - 1).waterlevel + pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y + 1).waterlevel)/4;
						}
						else
						{
							tilepoint.waterlevel = (tilepoint.waterlevel + pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y - 1).waterlevel + pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y + 1).waterlevel)/3;
						}
						pDoc->SetModifiedFlag();
						pDoc->m_War3Map.SetTilePoint(x, pDoc->m_War3Map.GetMapHeight() - y, tilepoint);
					}
				}			
			}
		}

		rect.SetRect((minx - 1)*CR_WIDTH, (miny - 1)*CR_HEIGHT, (maxx + 1)*CR_WIDTH, (maxy + 1)*CR_HEIGHT);
		pView->InvalidateTilesBitmap(rect);
		pView->InvalidateBitmapRect(rect);
		pDoc->CloseUndoObj();
	}
	else
	{// no selected tilepoints
		AfxMessageBox("You need to select tilepoints first to use this tool!", MB_OK | MB_APPLMODAL | MB_ICONWARNING);
	}
}

void CToolTilePointsEdit::WaterRand()
{
	CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
	ASSERT_VALID(pTheApp);
	if (!pTheApp)
	{return;}
	CMainFrame *pMainFrame = pTheApp->GetMainFrame();
	if (!pMainFrame)
	{return;}
	CChildFrame *pChildFrame = (CChildFrame*)pMainFrame->GetActiveFrame();
	if (!pChildFrame)
	{return;}
	CW3ZMapEditView *pView = pChildFrame->GetEditView();
	if (!pView)
	{return;}
	CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pChildFrame->GetActiveDocument();
	if (!pDoc)
	{return;}
	ASSERT_VALID(pDoc);

	CTilePoint tilepoint;
	int x, y, minx, maxx, miny, maxy;
	CRect rect;

	if (4 == pDoc->m_War3Map.mapTilePointSelState)
	{// check if we got selected tilepoints
		CWaitCursor wait;
		pDoc->OpenUndoObj();
		// gets the coordinates of the rectangle that contains the selected tilepoints
		minx = pDoc->m_War3Map.mapTilePointSelFirst.x;
		if (pDoc->m_War3Map.mapTilePointSelSecond.x < minx)
		{
			minx = pDoc->m_War3Map.mapTilePointSelSecond.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.x < minx)
		{
			minx = pDoc->m_War3Map.mapTilePointSelThird.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.x < minx)
		{
			minx = pDoc->m_War3Map.mapTilePointSelForth.x;
		}
		maxx = pDoc->m_War3Map.mapTilePointSelFirst.x;
		if (pDoc->m_War3Map.mapTilePointSelSecond.x > maxx)
		{
			maxx = pDoc->m_War3Map.mapTilePointSelSecond.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.x > maxx)
		{
			maxx = pDoc->m_War3Map.mapTilePointSelThird.x;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.x > maxx)
		{
			maxx = pDoc->m_War3Map.mapTilePointSelForth.x;
		}
		miny = pDoc->m_War3Map.mapTilePointSelFirst.y;
		if (pDoc->m_War3Map.mapTilePointSelSecond.y < miny)
		{
			miny = pDoc->m_War3Map.mapTilePointSelSecond.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.y < miny)
		{
			miny = pDoc->m_War3Map.mapTilePointSelThird.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.y < miny)
		{
			miny = pDoc->m_War3Map.mapTilePointSelForth.y;
		}
		maxy = pDoc->m_War3Map.mapTilePointSelFirst.y;
		if (pDoc->m_War3Map.mapTilePointSelSecond.y > maxy)
		{
			maxy = pDoc->m_War3Map.mapTilePointSelSecond.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelThird.y > maxy)
		{
			maxy = pDoc->m_War3Map.mapTilePointSelThird.y;
		}
		if (pDoc->m_War3Map.mapTilePointSelForth.y > maxy)
		{
			maxy = pDoc->m_War3Map.mapTilePointSelForth.y;
		}
		for (y = miny; y <= maxy; y++)
		{
			for (x = minx; x <= maxx; x++)
			{
				tilepoint = pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y);
				if (tilepoint.selected)
				{ // only selected tilepoints
					pDoc->AddToUndoObj(tilepoint, x, pDoc->m_War3Map.GetMapHeight() - y);
					tilepoint.waterlevel = pMainFrame->GetTilesEditBarRandTilePoint(tilepoint).waterlevel;
					pDoc->SetModifiedFlag();
					pDoc->m_War3Map.SetTilePoint(x, pDoc->m_War3Map.GetMapHeight() - y, tilepoint);
				}
			}			
		}

		rect.SetRect((minx - 1)*CR_WIDTH, (miny - 1)*CR_HEIGHT, (maxx + 1)*CR_WIDTH, (maxy + 1)*CR_HEIGHT);
		pView->InvalidateTilesBitmap(rect);
		pView->InvalidateBitmapRect(rect);
		pDoc->CloseUndoObj();
	}
	else
	{// no selected tilepoints
		AfxMessageBox("You need to select tilepoints first to use this tool!", MB_OK | MB_APPLMODAL | MB_ICONWARNING);
	}

}

void CToolTilePointsEdit::SetGround(CW3ZMapEditApp *pTheApp, CW3ZMapEditDoc *pDoc, CW3ZMapEditView *pView, int x, int y)
{
	CTilePoint tilepoint;
	tilepoint = pDoc->m_War3Map.GetTilePoint(x,pDoc->m_War3Map.GetMapHeight() - y);
	ground = tilepoint.groundtype;
}

void CToolTilePointsEdit::OnKeyDown(CW3ZMapEditView *pView, UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pView->GetDocument();
	ASSERT_VALID(pDoc);

	CTilePoint tilepoint;
	int minx, maxx, miny, maxy;
	CRect rect;

	if ( VK_ESCAPE == nChar)
	{
		CWaitCursor wait;
		if ((1 <= pDoc->m_War3Map.mapTilePointSelState) && (4 > pDoc->m_War3Map.mapTilePointSelState))
		{
			tilepoint = pDoc->m_War3Map.GetTilePoint(pDoc->m_War3Map.mapTilePointSelFirst.x, pDoc->m_War3Map.GetMapHeight() - pDoc->m_War3Map.mapTilePointSelFirst.y);
			tilepoint.viewcolor.rgbRed = 0;
			tilepoint.viewcolor.rgbGreen = 0;
			tilepoint.viewcolor.rgbBlue = 0;
			tilepoint.viewcolor.rgbReserved = 0;
			tilepoint.selected = false;
			pDoc->m_War3Map.SetTilePoint(pDoc->m_War3Map.mapTilePointSelFirst.x, pDoc->m_War3Map.GetMapHeight() - pDoc->m_War3Map.mapTilePointSelFirst.y, tilepoint);
			pView->DisplayTilePoint(pDoc->m_War3Map.mapTilePointSelFirst.x, pDoc->m_War3Map.GetMapHeight() - pDoc->m_War3Map.mapTilePointSelFirst.y, tilepoint);
			rect.SetRect(pDoc->m_War3Map.mapTilePointSelFirst.x*CR_WIDTH, pDoc->m_War3Map.mapTilePointSelFirst.y*CR_HEIGHT, pDoc->m_War3Map.mapTilePointSelFirst.x*CR_WIDTH, pDoc->m_War3Map.mapTilePointSelFirst.y*CR_HEIGHT);
			rect.NormalizeRect();
			rect.InflateRect(CR_WIDTH*(1 + m_iSize%2 + m_iSize/2),CR_HEIGHT*(1 + m_iSize%2 + m_iSize/2));
			pView->InvalidateTilesBitmap(rect);
			pView->InvalidateBitmapRect(rect);
		}
		if ((2 <= pDoc->m_War3Map.mapTilePointSelState) && (4 > pDoc->m_War3Map.mapTilePointSelState))
		{
			tilepoint = pDoc->m_War3Map.GetTilePoint(pDoc->m_War3Map.mapTilePointSelSecond.x, pDoc->m_War3Map.GetMapHeight() - pDoc->m_War3Map.mapTilePointSelSecond.y);
			tilepoint.viewcolor.rgbRed = 0;
			tilepoint.viewcolor.rgbGreen = 0;
			tilepoint.viewcolor.rgbBlue = 0;
			tilepoint.viewcolor.rgbReserved = 0;
			tilepoint.selected = false;
			pDoc->m_War3Map.SetTilePoint(pDoc->m_War3Map.mapTilePointSelSecond.x, pDoc->m_War3Map.GetMapHeight() - pDoc->m_War3Map.mapTilePointSelSecond.y, tilepoint);
			pView->DisplayTilePoint(pDoc->m_War3Map.mapTilePointSelSecond.x, pDoc->m_War3Map.GetMapHeight() - pDoc->m_War3Map.mapTilePointSelSecond.y, tilepoint);
			rect.SetRect(pDoc->m_War3Map.mapTilePointSelSecond.x*CR_WIDTH, pDoc->m_War3Map.mapTilePointSelSecond.y*CR_HEIGHT, pDoc->m_War3Map.mapTilePointSelSecond.x*CR_WIDTH, pDoc->m_War3Map.mapTilePointSelSecond.y*CR_HEIGHT);
			rect.NormalizeRect();
			rect.InflateRect(CR_WIDTH*(1 + m_iSize%2 + m_iSize/2),CR_HEIGHT*(1 + m_iSize%2 + m_iSize/2));
			pView->InvalidateTilesBitmap(rect);
			pView->InvalidateBitmapRect(rect);
		}
		if (3 == pDoc->m_War3Map.mapTilePointSelState)
		{
			tilepoint = pDoc->m_War3Map.GetTilePoint(pDoc->m_War3Map.mapTilePointSelThird.x, pDoc->m_War3Map.GetMapHeight() - pDoc->m_War3Map.mapTilePointSelThird.y);
			tilepoint.viewcolor.rgbRed = 0;
			tilepoint.viewcolor.rgbGreen = 0;
			tilepoint.viewcolor.rgbBlue = 0;
			tilepoint.viewcolor.rgbReserved = 0;
			tilepoint.selected = false;
			pDoc->m_War3Map.SetTilePoint(pDoc->m_War3Map.mapTilePointSelThird.x, pDoc->m_War3Map.GetMapHeight() - pDoc->m_War3Map.mapTilePointSelThird.y, tilepoint);
			pView->DisplayTilePoint(pDoc->m_War3Map.mapTilePointSelThird.x, pDoc->m_War3Map.GetMapHeight() - pDoc->m_War3Map.mapTilePointSelThird.y, tilepoint);
			rect.SetRect(pDoc->m_War3Map.mapTilePointSelThird.x*CR_WIDTH, pDoc->m_War3Map.mapTilePointSelThird.y*CR_HEIGHT, pDoc->m_War3Map.mapTilePointSelThird.x*CR_WIDTH, pDoc->m_War3Map.mapTilePointSelThird.y*CR_HEIGHT);
			rect.NormalizeRect();
			rect.InflateRect(CR_WIDTH*(1 + m_iSize%2 + m_iSize/2),CR_HEIGHT*(1 + m_iSize%2 + m_iSize/2));
			pView->InvalidateTilesBitmap(rect);
			pView->InvalidateBitmapRect(rect);
		}
		if (4 == pDoc->m_War3Map.mapTilePointSelState)
		{// unselect all
			pDoc->m_War3Map.UnselectAllTilePoints();
			minx = pDoc->m_War3Map.mapTilePointSelFirst.x;
			if (pDoc->m_War3Map.mapTilePointSelSecond.x < minx)
			{
				minx = pDoc->m_War3Map.mapTilePointSelSecond.x;
			}
			if (pDoc->m_War3Map.mapTilePointSelThird.x < minx)
			{
				minx = pDoc->m_War3Map.mapTilePointSelThird.x;
			}
			if (pDoc->m_War3Map.mapTilePointSelForth.x < minx)
			{
				minx = pDoc->m_War3Map.mapTilePointSelForth.x;
			}
			maxx = pDoc->m_War3Map.mapTilePointSelFirst.x;
			if (pDoc->m_War3Map.mapTilePointSelSecond.x > maxx)
				{
			maxx = pDoc->m_War3Map.mapTilePointSelSecond.x;
			}
			if (pDoc->m_War3Map.mapTilePointSelThird.x > maxx)
			{
				maxx = pDoc->m_War3Map.mapTilePointSelThird.x;
			}
			if (pDoc->m_War3Map.mapTilePointSelForth.x > maxx)
			{
				maxx = pDoc->m_War3Map.mapTilePointSelForth.x;
			}
			miny = pDoc->m_War3Map.mapTilePointSelFirst.y;
			if (pDoc->m_War3Map.mapTilePointSelSecond.y < miny)
			{
				miny = pDoc->m_War3Map.mapTilePointSelSecond.y;
			}
			if (pDoc->m_War3Map.mapTilePointSelThird.y < miny)
			{
				miny = pDoc->m_War3Map.mapTilePointSelThird.y;
			}
			if (pDoc->m_War3Map.mapTilePointSelForth.y < miny)
			{
				miny = pDoc->m_War3Map.mapTilePointSelForth.y;
			}
				maxy = pDoc->m_War3Map.mapTilePointSelFirst.y;
			if (pDoc->m_War3Map.mapTilePointSelSecond.y > maxy)
			{
				maxy = pDoc->m_War3Map.mapTilePointSelSecond.y;
			}
			if (pDoc->m_War3Map.mapTilePointSelThird.y > maxy)
			{
				maxy = pDoc->m_War3Map.mapTilePointSelThird.y;
			}
			if (pDoc->m_War3Map.mapTilePointSelForth.y > maxy)
			{
				maxy = pDoc->m_War3Map.mapTilePointSelForth.y;
			}

			rect.SetRect((minx - 1)*CR_WIDTH, (miny - 1)*CR_HEIGHT, (maxx + 1)*CR_WIDTH, (maxy + 1)*CR_HEIGHT);
			pView->InvalidateTilesBitmap(rect);
			pView->InvalidateBitmapRect(rect);
		}
		pDoc->m_War3Map.mapTilePointSelState = 0;
		pDoc->m_War3Map.SelectedTilePointsOnly = false;
	}
}

void CToolTilePointsEdit::TileFill(CW3ZMapEditApp *pTheApp, CW3ZMapEditDoc *pDoc, CW3ZMapEditView *pView, int x, int y)
{
	CMainFrame *pMainFrame = NULL;
	CTilePoint tilepoint;
	CListedPoint* pLPoint = NULL;
	CListedPoint* pLPoint2 = NULL;
	int x0, y0;

	CWaitCursor wait;
	tilepoint = pDoc->m_War3Map.GetTilePoint(x, pDoc->m_War3Map.GetMapHeight() - y);
	ground = tilepoint.groundtype;

	// check if same ground as the tilepoint bar
	pMainFrame = pTheApp->GetMainFrame();
	if ((!pMainFrame) || (ground == pMainFrame->GetTilesEditBarTilePoint().groundtype))
	{return;}

	pLPoint = new CListedPoint;
	pLPoint->next = NULL;
	pLPoint->point.x = x;
	pLPoint->point.y = y;

	while(pLPoint)
	{
		// scan right
		x0 = pLPoint->point.x + 1;
		y0 = pLPoint->point.y;
		while((x0 <= pDoc->m_War3Map.GetMapWidth()) && (pDoc->m_War3Map.GetTilePoint(x0, pDoc->m_War3Map.GetMapHeight() - y0).groundtype == ground))
		{x0++;}
		x0--;
		pLPoint->point.x = x0;

		// fixes max X coordinate
		if (x0 > m_ptFillMax.x)
		{m_ptFillMax.x = x0;}

		tilepoint = pDoc->m_War3Map.GetTilePoint(x0, pDoc->m_War3Map.GetMapHeight() - y0);
		// store old tilepoint
		pDoc->AddToUndoObj(tilepoint, x0, pDoc->m_War3Map.GetMapHeight() - y0);
		// updates new
		tilepoint.groundtype = pMainFrame->GetTilesEditBarTilePoint().groundtype;
		pDoc->m_War3Map.SetTilePoint(x0, pDoc->m_War3Map.GetMapHeight() - y0, tilepoint);
		pView->DisplayTilePoint(x0, pDoc->m_War3Map.GetMapHeight() - y0, tilepoint);
		// to scan above
		if (((y0 - 1) >= 0) && (pDoc->m_War3Map.GetTilePoint(x0, pDoc->m_War3Map.GetMapHeight() - (y0 - 1)).groundtype == ground))
		{
			pLPoint2 = new CListedPoint;
			pLPoint2->next = pLPoint->next;
			pLPoint2->point.x = x0;
			pLPoint2->point.y = y0 - 1;
			pLPoint->next = pLPoint2;
			if ((y0 - 1) < m_ptFillMin.y)
			{m_ptFillMin.y = y0 - 1;}
		}
		// to scan before
		if (((x0 - 1) >= 0) && (pDoc->m_War3Map.GetTilePoint((x0 - 1), pDoc->m_War3Map.GetMapHeight() - y0).groundtype == ground))
		{
				pLPoint2 = new CListedPoint;
				pLPoint2->next = pLPoint->next;
				pLPoint2->point.x = x0 - 1;
				pLPoint2->point.y = y0;
				pLPoint->next = pLPoint2;
				if ((x0 - 1) < m_ptFillMin.x)
				{m_ptFillMin.x = x0 - 1;}
		}
		// to scan below
		if (((y0+1) <= pDoc->m_War3Map.GetMapHeight()) && (pDoc->m_War3Map.GetTilePoint(x0, pDoc->m_War3Map.GetMapHeight() - (y0 + 1)).groundtype == ground))
		{
			pLPoint2 = new CListedPoint;
			pLPoint2->next = pLPoint->next;
			pLPoint2->point.x = x0;
			pLPoint2->point.y = y0 + 1;
			pLPoint->next = pLPoint2;
			if ((y0 + 1) > m_ptFillMax.y)
			{m_ptFillMax.y = y0 + 1;}
		}

		pLPoint2 = pLPoint;
		pLPoint = pLPoint->next;
		delete pLPoint2;
	}	
}