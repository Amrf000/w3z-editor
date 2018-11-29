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
// ToolDoodadsEdit.cpp: implementation of the CToolDoodadsEdit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "W3ZMapEdit.h"
#include "W3ZMapEditDoc.h"
#include "W3ZMapEditView.h"
#include "MainFrm.h"
#include "ToolDoodadsEdit.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToolDoodadsEdit::CToolDoodadsEdit()
{
	m_fMinDist = TOOL_DOOD_DEFAULT_DIST;
	m_iSize = TOOL_DOOD_MIN_SIZE;
}

CToolDoodadsEdit::~CToolDoodadsEdit()
{

}

void CToolDoodadsEdit::OnLButtonDown(CW3ZMapEditView *pView, int nFlags, CPoint point)
{
	CTool::OnLButtonDown(pView, nFlags, point);
	if (m_bLeftTracking == TRUE)
	{
		CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pView->GetDocument();
		ASSERT_VALID(pDoc);
		CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
		ASSERT_VALID(pTheApp);
		CDoodad doodad;

		float x, y;
		m_ptFirst = m_ptLast = point;

		if ((point.x < pView->m_pBitmapInfo->bV4Width) && (point.y < pView->m_pBitmapInfo->bV4Height))
		{
			x = (float)((point.x * pDoc->m_War3Map.GetMapWidth()*128) / pView->m_pBitmapInfo->bV4Width);
			y = (float)(((pView->m_pBitmapInfo->bV4Height - point.y) * pDoc->m_War3Map.GetMapHeight()*128) / pView->m_pBitmapInfo->bV4Height);
			x += pDoc->m_War3Map.GetMapCenterX();
			y += pDoc->m_War3Map.GetMapCenterY();
			if (pDoc && pTheApp)
			{
				pDoc->OpenUndoObj();
				if (TOOL_MODE_DOOD_ADD == m_iToolMode)
				{			
					DoodadWrite(pTheApp, pDoc, pView, x, y);
				}
				else if (TOOL_MODE_DOOD_DELETE == m_iToolMode)
				{
					DoodadRemove(pTheApp, pDoc, pView, x, y);
				}
				else if (TOOL_MODE_DOOD_UPDATE == m_iToolMode)
				{
					DoodadUpdate(pTheApp, pDoc, pView, x, y);
				}
			}
		}
		if ((TOOL_MODE_DOOD_RECT == m_iToolMode) || (TOOL_MODE_DOOD_F_RECT == m_iToolMode))
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
		else if ((TOOL_MODE_DOOD_ELLIPSE == m_iToolMode) || (TOOL_MODE_DOOD_F_ELLIPSE == m_iToolMode))
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

void CToolDoodadsEdit::OnLButtonUp(CW3ZMapEditView *pView, int nFlags, CPoint point)
{
	if (m_bLeftTracking == TRUE)
	{
		CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pView->GetDocument();
		ASSERT_VALID(pDoc);
		CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
		ASSERT_VALID(pTheApp);
		CMainFrame *pMainFrame = NULL;
		CDoodad doodad;

		float x, y;
		if ((TOOL_MODE_DOOD_RECT == m_iToolMode) || (TOOL_MODE_DOOD_F_RECT == m_iToolMode))
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
		else if ((TOOL_MODE_DOOD_ELLIPSE == m_iToolMode) || (TOOL_MODE_DOOD_F_ELLIPSE == m_iToolMode))
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
		if ((point.x < pView->m_pBitmapInfo->bV4Width) && (point.y < pView->m_pBitmapInfo->bV4Height))
		{
			x = (float)((point.x * pDoc->m_War3Map.GetMapWidth()*128) / pView->m_pBitmapInfo->bV4Width);
			y = (float)(((pView->m_pBitmapInfo->bV4Height - point.y) * pDoc->m_War3Map.GetMapHeight()*128) / pView->m_pBitmapInfo->bV4Height);
			x += pDoc->m_War3Map.GetMapCenterX();
			y += pDoc->m_War3Map.GetMapCenterY();
			if (pDoc && pTheApp)
			{
				if (TOOL_MODE_DOOD_ADD == m_iToolMode)
				{			
					DoodadWrite(pTheApp, pDoc, pView, x, y);
				}
				else if (TOOL_MODE_DOOD_DELETE == m_iToolMode)
				{
					DoodadRemove(pTheApp, pDoc, pView, x, y);
				}
				else if (TOOL_MODE_DOOD_UPDATE == m_iToolMode)
				{
					DoodadUpdate(pTheApp, pDoc, pView, x, y);
				}
				else if (TOOL_MODE_DOOD_REPLACE == m_iToolMode)
				{
					CString oldtype, newtype;
					doodad = pDoc->m_War3Map.GetClosestDoodad(x, y);
					pMainFrame = pTheApp->GetMainFrame();
					if (pMainFrame) //NULL si pMainFrame n'a pas ete trouve
					{
						if (pDoc->m_War3Map.GetDistance(x, y, doodad.x, doodad.y) <= TOOL_DOOD_DEFAULT_DIST)
						{
							oldtype = doodad.type.GetStrID();
							newtype = pMainFrame->GetDoodadsEditBarDoodad().type.GetStrID();
							pDoc->m_War3Map.GoFirstDoodad();
							doodad = pDoc->m_War3Map.GetNextDoodad();
							while(doodad.type != "")
							{
								if (doodad.type == oldtype)
								{
									doodad.type = newtype;
									pDoc->AddToUndoObj(doodad, UNDO_DOOD_UPD);
									pDoc->m_War3Map.SetDoodad(doodad);
								}
							}
							pDoc->SetModifiedFlag();
							pView->InvalidateBitmap();
							pView->Invalidate();
						}
					}
				}
				else if ((TOOL_MODE_DOOD_RECT == m_iToolMode) || (TOOL_MODE_DOOD_F_RECT == m_iToolMode))
				{
					CWaitCursor wait;
					int x0, y0, minx, miny, maxx, maxy;
					int i, j;
					float fminx, fminy, fmaxx, fmaxy;
					if (point.x <= m_ptFirst.x)
					{
						minx = point.x;
						maxx = m_ptFirst.x;
					}
					else
					{
						minx = m_ptFirst.x;
						maxx = point.x;
					}
					if (point.y <= m_ptFirst.y)
					{
						miny = point.y;
						maxy = m_ptFirst.y;
					}
					else
					{
						miny = m_ptFirst.y;
						maxy = point.y;
					}

					fminx = (float)((minx * pDoc->m_War3Map.GetMapWidth()*128) / pView->m_pBitmapInfo->bV4Width);
					fminy = (float)(((pView->m_pBitmapInfo->bV4Height - miny) * pDoc->m_War3Map.GetMapHeight()*128) / pView->m_pBitmapInfo->bV4Height);
					fminx += pDoc->m_War3Map.GetMapCenterX();
					fminy += pDoc->m_War3Map.GetMapCenterY();

					fmaxx = (float)((maxx * pDoc->m_War3Map.GetMapWidth()*128) / pView->m_pBitmapInfo->bV4Width);
					fmaxy = (float)(((pView->m_pBitmapInfo->bV4Height - maxy) * pDoc->m_War3Map.GetMapHeight()*128) / pView->m_pBitmapInfo->bV4Height);
					fmaxx += pDoc->m_War3Map.GetMapCenterX();
					fmaxy += pDoc->m_War3Map.GetMapCenterY();

					if (TOOL_MODE_DOOD_RECT == m_iToolMode)
					{
						for (y0 = miny; y0 <= maxy; y0++)
						{
							y = (float)(((pView->m_pBitmapInfo->bV4Height - y0) * pDoc->m_War3Map.GetMapHeight()*128) / pView->m_pBitmapInfo->bV4Height);
							y += pDoc->m_War3Map.GetMapCenterY();
	
							DoodadWrite(pTheApp, pDoc, pView, fminx, y);
							DoodadWrite(pTheApp, pDoc, pView, fmaxx, y);
						}
						for (x0 = minx; x0 <= maxx; x0++)
						{
							x = (float)((x0 * pDoc->m_War3Map.GetMapWidth()*128) / pView->m_pBitmapInfo->bV4Width);
							x += pDoc->m_War3Map.GetMapCenterX();
	
							DoodadWrite(pTheApp, pDoc, pView, x, fminy);
							DoodadWrite(pTheApp, pDoc, pView, x, fmaxy);
						}
					}
					else if (TOOL_MODE_DOOD_F_RECT == m_iToolMode)
					{
						i = 1;
						j = 1;
						for (y0 = miny; y0 <= maxy; y0 += j)
						{
							for (x0 = minx; x0 <= maxx; x0 += i)
							{
								x = (float)((x0 * pDoc->m_War3Map.GetMapWidth()*128) / pView->m_pBitmapInfo->bV4Width);
								y = (float)(((pView->m_pBitmapInfo->bV4Height - y0) * pDoc->m_War3Map.GetMapHeight()*128) / pView->m_pBitmapInfo->bV4Height);
								x += pDoc->m_War3Map.GetMapCenterX();
								y += pDoc->m_War3Map.GetMapCenterY();
								DoodadWrite(pTheApp, pDoc, pView, x, y);
								i = 1 + rand()%2;
							}
							j = 1 + rand()%2;
						}
					}
				}
				else if ((TOOL_MODE_DOOD_ELLIPSE == m_iToolMode) || (TOOL_MODE_DOOD_F_ELLIPSE == m_iToolMode))
				{
					CWaitCursor wait;
					int x0, y0, minx, maxx, miny, maxy, a, b;
					int i, j;
					float ofsx, ofsy;

					if (point.x <= m_ptFirst.x)
					{
						minx = point.x;
						maxx = m_ptFirst.x;
					}
					else
					{
						minx = m_ptFirst.x;
						maxx = point.x;
					}
					if (point.y <= m_ptFirst.y)
					{
						miny = point.y;
						maxy = m_ptFirst.y;
					}
					else
					{
						miny = m_ptFirst.y;
						maxy = point.y;
					}
					if (TOOL_MODE_DOOD_ELLIPSE == m_iToolMode)
					{
						//x = a*cos(t)
						//y = b*sin(t)
						//x²/a² + y²/b² = 1
						//y= sqrt((1 - x²/a²)*b²)
						a = (maxx - minx)/2;
						b = (maxy - miny)/2;
						ofsx = (float)(maxx + minx)/2;
						ofsx = ((ofsx * pDoc->m_War3Map.GetMapWidth()*128) / pView->m_pBitmapInfo->bV4Width);
						ofsx += pDoc->m_War3Map.GetMapCenterX();
						ofsy = (float)(maxy + miny)/2;
						ofsy = (float)(((pView->m_pBitmapInfo->bV4Height - ofsy) * pDoc->m_War3Map.GetMapHeight()*128) / pView->m_pBitmapInfo->bV4Height);
						ofsy += pDoc->m_War3Map.GetMapCenterY();

						minx = 0;
						maxx = a; //(int)(a*sqrt(2)/2 + 0.5);
						for (x0 = minx; x0 <= maxx; x0++)
						{
							y0 = (int)(sqrt((1 - x0*x0/(double)(a*a))*b*b) + 0.5);

							x = (float)((x0 * pDoc->m_War3Map.GetMapWidth()*128) / pView->m_pBitmapInfo->bV4Width);
							y = (float)((y0 * pDoc->m_War3Map.GetMapHeight()*128) / pView->m_pBitmapInfo->bV4Height);
							DoodadWrite(pTheApp, pDoc, pView, ofsx + x, ofsy + y);
							DoodadWrite(pTheApp, pDoc, pView, ofsx + x, ofsy - y);
							DoodadWrite(pTheApp, pDoc, pView, ofsx - x, ofsy + y);
							DoodadWrite(pTheApp, pDoc, pView, ofsx - x, ofsy - y);
						}
						miny = 0;
						maxy = b;//(int)(b*sqrt(2)/2 + 0.5);
						for (y0 = miny; y0 <= maxy; y0++)
						{
							x0 = (int)(sqrt((1 - y0*y0/(double)(b*b))*a*a) + 0.5);

							x = (float)((x0 * pDoc->m_War3Map.GetMapWidth()*128) / pView->m_pBitmapInfo->bV4Width);
							y = (float)((y0 * pDoc->m_War3Map.GetMapHeight()*128) / pView->m_pBitmapInfo->bV4Height);
							DoodadWrite(pTheApp, pDoc, pView, ofsx + x, ofsy + y);
							DoodadWrite(pTheApp, pDoc, pView, ofsx + x, ofsy - y);
							DoodadWrite(pTheApp, pDoc, pView, ofsx - x, ofsy + y);
							DoodadWrite(pTheApp, pDoc, pView, ofsx - x, ofsy - y);
						}
					}
					else if (TOOL_MODE_DOOD_F_ELLIPSE == m_iToolMode)
					{
						//x = a*cos(t)
						//y = b*sin(t)
						//x²/a² + y²/b² = 1
						//y= sqrt((1 - x²/a²)*b²)
						a = (maxx - minx)/2;
						b = (maxy - miny)/2;
						ofsx = (float)(maxx + minx)/2;
						ofsx = ((ofsx * pDoc->m_War3Map.GetMapWidth()*128) / pView->m_pBitmapInfo->bV4Width);
						ofsx += pDoc->m_War3Map.GetMapCenterX();
						ofsy = (float)(maxy + miny)/2;
						ofsy = (float)(((pView->m_pBitmapInfo->bV4Height - ofsy) * pDoc->m_War3Map.GetMapHeight()*128) / pView->m_pBitmapInfo->bV4Height);
						ofsy += pDoc->m_War3Map.GetMapCenterY();

						minx = 0;
						maxx = a; //(int)(a*sqrt(2)/2 + 0.5);
						i = 1;
						j = 1;
						for (x0 = minx; x0 <= maxx; x0 += i)
						{
							maxy = (int)(sqrt((1 - x0*x0/(double)(a*a))*b*b) + 0.5);
							for (y0 = 0; y0 <= maxy; y0 += j)
							{
								x = (float)((x0 * pDoc->m_War3Map.GetMapWidth()*128) / pView->m_pBitmapInfo->bV4Width);
								y = (float)((y0 * pDoc->m_War3Map.GetMapHeight()*128) / pView->m_pBitmapInfo->bV4Height);
								DoodadWrite(pTheApp, pDoc, pView, ofsx + x, ofsy + y);
								DoodadWrite(pTheApp, pDoc, pView, ofsx + x, ofsy - y);
								DoodadWrite(pTheApp, pDoc, pView, ofsx - x, ofsy + y);
								DoodadWrite(pTheApp, pDoc, pView, ofsx - x, ofsy - y);
								j = 1+rand()%2;
							}
							i = 1+rand()%2;
						}
					}
				}
			}
		}
		if (pDoc)
		{pDoc->CloseUndoObj();}
		m_ptLast = point;
	}
	CTool::OnLButtonUp(pView, nFlags, point);
}

void CToolDoodadsEdit::OnRButtonDown(CW3ZMapEditView *pView, int nFlags, CPoint point)
{
	CTool::OnRButtonDown(pView, nFlags, point);
	CDoodad seldoodad;
	CRect updRect;

	//m_bRightTracking est true si l'appel du parent (OnRButtonDown) s'est effectue correctement
	if (m_bRightTracking == TRUE) // evite les conflits entre boutons droit et gauche
	{
		CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pView->GetDocument();
		ASSERT_VALID(pDoc);
		CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
		ASSERT_VALID(pTheApp);
		CMainFrame *pMainFrame = NULL;
		CDoodad doodad;

		if (pDoc)
		{
//			pDoc->m_War3Map.UnselectAllDoodads();
			seldoodad = pDoc->m_War3Map.GetSelectedDoodad();
//			seldoodad.selected = FALSE;
			pDoc->m_War3Map.UnselectDoodad(seldoodad);
			updRect.TopLeft().x = updRect.BottomRight().x = (int) ((seldoodad.x - pDoc->m_War3Map.GetMapCenterX()) * pView->m_pBitmapInfo->bV4Width / (pDoc->m_War3Map.GetMapWidth()*128));
			updRect.TopLeft().y = updRect.BottomRight().y = (int) (-1*((seldoodad.y - pDoc->m_War3Map.GetMapCenterY()) * pView->m_pBitmapInfo->bV4Height / (pDoc->m_War3Map.GetMapHeight()*128) - pView->m_pBitmapInfo->bV4Height));
			updRect.InflateRect(CR_WIDTH,CR_HEIGHT);
			pView->InvalidateDoodadsBitmap(updRect);
			pView->InvalidateBitmapRect(updRect);
		}

		float x, y;
		if ((point.x < pView->m_pBitmapInfo->bV4Width) && (point.y < pView->m_pBitmapInfo->bV4Height))
		{
			x = (float)((point.x * pDoc->m_War3Map.GetMapWidth()*128) / pView->m_pBitmapInfo->bV4Width);
			y = (float)(((pView->m_pBitmapInfo->bV4Height - point.y) * pDoc->m_War3Map.GetMapHeight()*128) / pView->m_pBitmapInfo->bV4Height);
			x += pDoc->m_War3Map.GetMapCenterX();
			y += pDoc->m_War3Map.GetMapCenterY();
			if (pDoc && pTheApp)
			{
				doodad = pDoc->m_War3Map.GetClosestDoodad(x, y);
				if (pDoc->m_War3Map.GetDistance(x, y, doodad.x, doodad.y) < TOOL_DOOD_DEFAULT_DIST)
				{// seulement si le doodad est proche
					doodad.selected = TRUE;
//					m_doodad = doodad;
					pDoc->m_War3Map.SetDoodad(doodad);
					pDoc->m_War3Map.SetSelectedDoodad(doodad);
					updRect.TopLeft().x = updRect.BottomRight().x = (int) ((doodad.x - pDoc->m_War3Map.GetMapCenterX()) * pView->m_pBitmapInfo->bV4Width / (pDoc->m_War3Map.GetMapWidth()*128));
					updRect.TopLeft().y = updRect.BottomRight().y = (int) (-1*((doodad.y - pDoc->m_War3Map.GetMapCenterY()) * pView->m_pBitmapInfo->bV4Height / (pDoc->m_War3Map.GetMapHeight()*128) - pView->m_pBitmapInfo->bV4Height));
					updRect.InflateRect(CR_WIDTH,CR_HEIGHT);
					pView->InvalidateDoodadsBitmap(updRect);
					pView->InvalidateBitmapRect(updRect);
					pMainFrame = pTheApp->GetMainFrame();
					if (pMainFrame)
					{
						pMainFrame->UpdateDoodadsEditBar(doodad);
					}
				}
			}
		}
		m_ptFirst = m_ptLast = point;
	}
}

void CToolDoodadsEdit::OnRButtonUp(CW3ZMapEditView *pView, int nFlags, CPoint point)
{
	CRect updRect;
	CDoodad seldoodad;

	if (m_bRightTracking == TRUE)
	{
		CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pView->GetDocument();
		ASSERT_VALID(pDoc);
		CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
		ASSERT_VALID(pTheApp);
		CMainFrame *pMainFrame = NULL;
		CDoodad doodad;
		if (pDoc)
		{
			seldoodad = pDoc->m_War3Map.GetSelectedDoodad();
		}

		float x, y;
		if ((point.x < pView->m_pBitmapInfo->bV4Width) && (point.y < pView->m_pBitmapInfo->bV4Height))
		{
			x = (float)((point.x * pDoc->m_War3Map.GetMapWidth()*128) / pView->m_pBitmapInfo->bV4Width);
			y = (float)(((pView->m_pBitmapInfo->bV4Height - point.y) * pDoc->m_War3Map.GetMapHeight()*128) / pView->m_pBitmapInfo->bV4Height);
			x += pDoc->m_War3Map.GetMapCenterX();
			y += pDoc->m_War3Map.GetMapCenterY();
			if (pDoc && pTheApp)
			{
				doodad = pDoc->m_War3Map.GetClosestDoodad(x, y);
				if (pDoc->m_War3Map.GetDistance(x,y,doodad.x, doodad.y) < TOOL_DOOD_DEFAULT_DIST)
				{// seulement si le doodad est proche
//					seldoodad.selected = FALSE;
					pDoc->m_War3Map.UnselectDoodad(seldoodad);
					updRect.TopLeft().x = updRect.BottomRight().x = (int) ((seldoodad.x - pDoc->m_War3Map.GetMapCenterX()) * pView->m_pBitmapInfo->bV4Width / (pDoc->m_War3Map.GetMapWidth()*128));
					updRect.TopLeft().y = updRect.BottomRight().y = (int) (-1*((seldoodad.y - pDoc->m_War3Map.GetMapCenterY()) * pView->m_pBitmapInfo->bV4Height / (pDoc->m_War3Map.GetMapHeight()*128) - pView->m_pBitmapInfo->bV4Height));
					updRect.InflateRect(CR_WIDTH,CR_HEIGHT);
					pView->InvalidateDoodadsBitmap(updRect);
					pView->InvalidateBitmapRect(updRect);
					doodad.selected = TRUE;
//					m_doodad = doodad;
					pDoc->m_War3Map.SetDoodad(doodad);
					pDoc->m_War3Map.SetSelectedDoodad(doodad);
					updRect.TopLeft().x = updRect.BottomRight().x = (int) ((doodad.x - pDoc->m_War3Map.GetMapCenterX()) * pView->m_pBitmapInfo->bV4Width / (pDoc->m_War3Map.GetMapWidth()*128));
					updRect.TopLeft().y = updRect.BottomRight().y = (int) (-1*((doodad.y - pDoc->m_War3Map.GetMapCenterY()) * pView->m_pBitmapInfo->bV4Height / (pDoc->m_War3Map.GetMapHeight()*128) - pView->m_pBitmapInfo->bV4Height));
					updRect.InflateRect(CR_WIDTH,CR_HEIGHT);
					pView->InvalidateDoodadsBitmap(updRect);
					pView->InvalidateBitmapRect(updRect);
					pMainFrame = pTheApp->GetMainFrame();
					if (pMainFrame)
					{
						pMainFrame->UpdateDoodadsEditBar(doodad);
					}
				}
			}
		}
		m_ptLast = point;
	}

	CTool::OnRButtonUp(pView, nFlags, point);
}

void CToolDoodadsEdit::OnMouseMove(CW3ZMapEditView *pView, int nFlags, CPoint point)
{
	CTool::OnMouseMove(pView, nFlags, point);
	CRect updRect;
	CDoodad seldoodad;

	if (m_bRightTracking == TRUE)
	{
		CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pView->GetDocument();
		ASSERT_VALID(pDoc);
		CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
		ASSERT_VALID(pTheApp);
		CMainFrame *pMainFrame = NULL;
		CDoodad doodad;

		if (pDoc)
		{
			seldoodad = pDoc->m_War3Map.GetSelectedDoodad();
		}

		float x, y;
		if ((point.x < pView->m_pBitmapInfo->bV4Width) && (point.y < pView->m_pBitmapInfo->bV4Height))
		{
			x = (float)((point.x * pDoc->m_War3Map.GetMapWidth()*128) / pView->m_pBitmapInfo->bV4Width);
			y = (float)(((pView->m_pBitmapInfo->bV4Height - point.y) * pDoc->m_War3Map.GetMapHeight()*128) / pView->m_pBitmapInfo->bV4Height);
			x += pDoc->m_War3Map.GetMapCenterX();
			y += pDoc->m_War3Map.GetMapCenterY();
			if (pDoc && pTheApp)
			{
				doodad = pDoc->m_War3Map.GetClosestDoodad(x, y);
				if ((pDoc->m_War3Map.GetDistance(x,y,doodad.x, doodad.y) < TOOL_DOOD_DEFAULT_DIST) && (doodad.ID != seldoodad.ID))
				{// seulement si le doodad est proche et qu'il n'est pas deja selectionne
//					seldoodad.selected = FALSE;
					pDoc->m_War3Map.UnselectDoodad(seldoodad);
					updRect.TopLeft().x = updRect.BottomRight().x = (int) ((seldoodad.x - pDoc->m_War3Map.GetMapCenterX()) * pView->m_pBitmapInfo->bV4Width / (pDoc->m_War3Map.GetMapWidth()*128));
					updRect.TopLeft().y = updRect.BottomRight().y = (int) (-1*((seldoodad.y - pDoc->m_War3Map.GetMapCenterY()) * pView->m_pBitmapInfo->bV4Height / (pDoc->m_War3Map.GetMapHeight()*128) - pView->m_pBitmapInfo->bV4Height));
					updRect.InflateRect(CR_WIDTH,CR_HEIGHT);
					pView->InvalidateDoodadsBitmap(updRect);
					pView->InvalidateBitmapRect(updRect);
					doodad.selected = TRUE;
//					m_doodad = doodad;
					pDoc->m_War3Map.SetDoodad(doodad);
					pDoc->m_War3Map.SetSelectedDoodad(doodad);
					updRect.TopLeft().x = updRect.BottomRight().x = (int) ((doodad.x - pDoc->m_War3Map.GetMapCenterX()) * pView->m_pBitmapInfo->bV4Width / (pDoc->m_War3Map.GetMapWidth()*128));
					updRect.TopLeft().y = updRect.BottomRight().y = (int) (-1*((doodad.y - pDoc->m_War3Map.GetMapCenterY()) * pView->m_pBitmapInfo->bV4Height / (pDoc->m_War3Map.GetMapHeight()*128) - pView->m_pBitmapInfo->bV4Height));
					updRect.InflateRect(CR_WIDTH,CR_HEIGHT);
					pView->InvalidateDoodadsBitmap(updRect);
					pView->InvalidateBitmapRect(updRect);
					pMainFrame = pTheApp->GetMainFrame();
					if (pMainFrame) //NULL si pMainFrame n'a pas ete trouve
					{
						pMainFrame->UpdateDoodadsEditBar(doodad);
					}
				}
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
		CDoodad doodad;

		float x, y;
		if ((point.x < pView->m_pBitmapInfo->bV4Width) && (point.y < pView->m_pBitmapInfo->bV4Height))
		{
			x = (float)((point.x * pDoc->m_War3Map.GetMapWidth()*128) / pView->m_pBitmapInfo->bV4Width);
			y = (float)(((pView->m_pBitmapInfo->bV4Height - point.y) * pDoc->m_War3Map.GetMapHeight()*128) / pView->m_pBitmapInfo->bV4Height);
			x += pDoc->m_War3Map.GetMapCenterX();
			y += pDoc->m_War3Map.GetMapCenterY();
			if (pDoc && pTheApp)
			{
				if (TOOL_MODE_DOOD_ADD == m_iToolMode)
				{			
					DoodadWrite(pTheApp, pDoc, pView, x, y);
				}
				else if (TOOL_MODE_DOOD_DELETE == m_iToolMode)
				{
					DoodadRemove(pTheApp, pDoc, pView, x, y);
				}
				else if (TOOL_MODE_DOOD_UPDATE == m_iToolMode)
				{
					DoodadUpdate(pTheApp, pDoc, pView, x, y);
				}

			}
		}
		if ((TOOL_MODE_DOOD_RECT == m_iToolMode) || (TOOL_MODE_DOOD_F_RECT == m_iToolMode))
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
		else if ((TOOL_MODE_DOOD_ELLIPSE == m_iToolMode) || (TOOL_MODE_DOOD_F_ELLIPSE == m_iToolMode))
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

void CToolDoodadsEdit::SetCursor()
{
	::SetCursor(AfxGetApp()->LoadCursor(IDC_DOODADS));
}

void CToolDoodadsEdit::DoodadWrite(CW3ZMapEditApp *pTheApp, CW3ZMapEditDoc *pDoc, CW3ZMapEditView *pView, float x, float y)
{
	CMainFrame *pMainFrame = NULL;
	CDoodad doodad, newdoodad;
	int i, j, unit;
	float x0, y0, halfsize;
	CRect updRect;

	halfsize = m_iSize/2;
	// find random position
	x0 = (x - halfsize) + rand()%((int)m_iSize);
	y0 = (y - halfsize) + rand()%((int)m_iSize);

	unit = (int)m_fMinDist/2;
	if (unit <= 0)
	{unit = 1;}
	// values to add to coordinates if another doodad is too close
	if (x0 > x)
	{i = -1;}
	else
	{i = 1;}

	if (y0 > y)
	{j = -1;}
	else
	{j = 1;}

	doodad = pDoc->m_War3Map.GetClosestDoodad(x0, y0);
	while ((pDoc->m_War3Map.GetDistance(x0, y0, doodad.x, doodad.y) < m_fMinDist)
			&& (x0 <= ( x + halfsize))
			&& (x0 >= ( x - halfsize))
			&& (y0 <= ( y + halfsize))
			&& (y0 >= ( y - halfsize)))
	{
		// try to randomly find another
		x0 += i*rand()%unit;
		y0 += j*rand()%unit;
		doodad = pDoc->m_War3Map.GetClosestDoodad(x0, y0);
	}

	pMainFrame = pTheApp->GetMainFrame();
	if (pMainFrame) //NULL si pMainFrame n'a pas ete trouve
	{
		if ((pDoc->m_War3Map.GetDistance(x0, y0, doodad.x, doodad.y) >= m_fMinDist)
			&& (x0 <= ( x + m_iSize/2))
			&& (x0 >= ( x - m_iSize/2))
			&& (y0 <= ( y + m_iSize/2))
			&& (y0 >= ( y - m_iSize/2)))
		{
			newdoodad = pMainFrame->GetDoodadsEditBarNewDoodad();
			newdoodad.x = x0;
			newdoodad.y = y0;
			newdoodad.z = pDoc->m_War3Map.GetGroundLevel(x0, y0);
			newdoodad.selected = false;
			newdoodad.ID = -1; // no ID

			pDoc->m_War3Map.AddDoodad(newdoodad);
			pDoc->AddToUndoObj(newdoodad, UNDO_DOOD_ADD);
			pDoc->SetModifiedFlag();
			updRect.TopLeft().x = updRect.BottomRight().x = (int) ((newdoodad.x - pDoc->m_War3Map.GetMapCenterX()) * pView->m_pBitmapInfo->bV4Width / (pDoc->m_War3Map.GetMapWidth()*128));
			updRect.TopLeft().y = updRect.BottomRight().y = (int) (-1*((newdoodad.y - pDoc->m_War3Map.GetMapCenterY()) * pView->m_pBitmapInfo->bV4Height / (pDoc->m_War3Map.GetMapHeight()*128) - pView->m_pBitmapInfo->bV4Height));
			updRect.InflateRect(CR_WIDTH,CR_HEIGHT);
			pView->InvalidateDoodadsBitmap(updRect);
			pView->InvalidateBitmapRect(updRect);
		}
	}
}

void CToolDoodadsEdit::DoodadRemove(CW3ZMapEditApp *pTheApp, CW3ZMapEditDoc *pDoc, CW3ZMapEditView *pView, float x, float y)
{
	CMainFrame *pMainFrame = NULL;
	CDoodad doodad;
	CRect updRect;

	doodad = pDoc->m_War3Map.GetClosestDoodad(x, y);
	pMainFrame = pTheApp->GetMainFrame();
	if (pMainFrame) //NULL si pMainFrame n'a pas ete trouve
	{
		if (pDoc->m_War3Map.GetDistance(x,y,doodad.x,doodad.y) <= TOOL_DOOD_DEFAULT_DIST)
		{
			pDoc->AddToUndoObj(doodad, UNDO_DOOD_DEL);
			pDoc->m_War3Map.DelDoodad(doodad);
			pDoc->SetModifiedFlag();
			updRect.TopLeft().x = updRect.BottomRight().x = (int) ((doodad.x - pDoc->m_War3Map.GetMapCenterX()) * pView->m_pBitmapInfo->bV4Width / (pDoc->m_War3Map.GetMapWidth()*128));
			updRect.TopLeft().y = updRect.BottomRight().y = (int) (-1*((doodad.y - pDoc->m_War3Map.GetMapCenterY()) * pView->m_pBitmapInfo->bV4Height / (pDoc->m_War3Map.GetMapHeight()*128) - pView->m_pBitmapInfo->bV4Height));
			updRect.InflateRect(CR_WIDTH,CR_HEIGHT);
			pView->InvalidateDoodadsBitmap(updRect);
			pView->InvalidateBitmapRect(updRect);
		}
	}
}

void CToolDoodadsEdit::DoodadUpdate(CW3ZMapEditApp *pTheApp, CW3ZMapEditDoc *pDoc, CW3ZMapEditView *pView, float x, float y)
{
	CMainFrame *pMainFrame = NULL;
	CDoodad doodad, newdoodad;
	CRect updRect;

	doodad = pDoc->m_War3Map.GetClosestDoodad(x, y);
	pMainFrame = pTheApp->GetMainFrame();
	if (pMainFrame) //NULL si pMainFrame n'a pas ete trouve
	{
		if (pDoc->m_War3Map.GetDistance(x,y,doodad.x,doodad.y) <= TOOL_DOOD_DEFAULT_DIST)
		{
			newdoodad = pMainFrame->GetDoodadsEditBarUpdatedDoodad(doodad);
//			newdoodad.x = x;
//			newdoodad.y = y;
			newdoodad.z = pDoc->m_War3Map.GetGroundLevel(x, y);
//			newdoodad.selected = false;
			pDoc->AddToUndoObj(doodad, UNDO_DOOD_UPD);
			pDoc->m_War3Map.SetDoodad(newdoodad);
			pDoc->SetModifiedFlag();
			updRect.TopLeft().x = updRect.BottomRight().x = (int) ((newdoodad.x - pDoc->m_War3Map.GetMapCenterX()) * pView->m_pBitmapInfo->bV4Width / (pDoc->m_War3Map.GetMapWidth()*128));
			updRect.TopLeft().y = updRect.BottomRight().y = (int) (-1*((newdoodad.y - pDoc->m_War3Map.GetMapCenterY()) * pView->m_pBitmapInfo->bV4Height / (pDoc->m_War3Map.GetMapHeight()*128) - pView->m_pBitmapInfo->bV4Height));
			updRect.InflateRect(CR_WIDTH,CR_HEIGHT);
			pView->InvalidateDoodadsBitmap(updRect);
			pView->InvalidateBitmapRect(updRect);
		}
	}

}

float CToolDoodadsEdit::GetToolSize()
{
	return m_iSize;
}

void CToolDoodadsEdit::SetToolSize(float size)
{
	if ((TOOL_DOOD_MIN_SIZE <= size ) && (size < TOOL_DOOD_MAX_SIZE))
	{
		m_iSize = size;
	}

}
