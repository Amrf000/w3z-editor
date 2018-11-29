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
// ToolPathEdit.cpp: implementation of the CToolPathEdit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "W3ZMapEdit.h"
#include "W3ZMapEditDoc.h"
#include "W3ZMapEditView.h"
#include "MainFrm.h"
#include "ToolPathEdit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToolPathEdit::CToolPathEdit()
{
	m_iSize = TOOL_PATH_DEFAULT_SIZE;
}

CToolPathEdit::~CToolPathEdit()
{

}

void CToolPathEdit::OnLButtonDown(CW3ZMapEditView *pView, int nFlags, CPoint point)
{
	CTool::OnLButtonDown(pView, nFlags, point);

	//m_bLeftTracking est ture si l'appel du parent (OnLButtonDown) s'est effectue correctement
	if (m_bLeftTracking == TRUE) // evite les conflits entre boutons droit et gauche
	{
		CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pView->GetDocument();
		ASSERT_VALID(pDoc);
		CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
		ASSERT_VALID(pTheApp);

		int x,y;
		m_ptFirst = m_ptLast = point;
		if ((point.x < pView->m_pBitmapInfo->bV4Width) && (point.y < pView->m_pBitmapInfo->bV4Height))
		{
			pDoc->OpenUndoObj();
			x = (point.x) / (CR_WIDTH/4);
			y = (point.y) / (CR_HEIGHT/4);
			if (pDoc && pTheApp)
			{
				PathWrite(pTheApp, pDoc, pView, x, y);
			}
		}
		CRect rect(m_ptLast, point);
		rect.NormalizeRect();
		rect.InflateRect(m_iSize*CR_WIDTH/4+2*CR_WIDTH, m_iSize*CR_HEIGHT/4+2*CR_HEIGHT);
		pView->InvalidatePathBitmap(rect);
		pView->InvalidateBitmapRect(rect);
	}
}

void CToolPathEdit::OnMouseMove(CW3ZMapEditView *pView, int nFlags, CPoint point)
{
	CTool::OnMouseMove(pView, nFlags, point);

	if (m_bLeftTracking == TRUE) // evite les conflits entre boutons droit et gauche
	{
		CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pView->GetDocument();
		ASSERT_VALID(pDoc);
		CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
		ASSERT_VALID(pTheApp);

		double m, p;
		int x1, y1, x2, y2, i;
		if ((point.x < pView->m_pBitmapInfo->bV4Width) && (point.y < pView->m_pBitmapInfo->bV4Height))
		{
			x1 = (m_ptLast.x) / (CR_WIDTH/4);
			y1 = (m_ptLast.y) / (CR_HEIGHT/4);
			x2 = (point.x) / (CR_WIDTH/4);
			y2 = (point.y) / (CR_HEIGHT/4);
			if (pDoc && pTheApp)
			{
				if (x1 != x2)
				{
					m = (double)(y1 - y2) / (double)(x1 - x2);
					p = y2 - m*x2;
					if (m*m <= 1)
					{// suivant les x
						if (x1 > x2)
						{
							i = x1;
							x1 = x2;
							x2 = i;
						}
						for (i = x1; i <= x2; i++)
						{
							y1 = (int) (m*i + p + 0.5);
							PathWrite(pTheApp, pDoc, pView, i, y1);
						}
					}
					else
					{ // suivant les y
						if (y1 > y2)
						{
							i = y1;
							y1 = y2;
							y2 = i;
						}
						for (i = y1; i <= y2; i++)
						{
							x1 = (int) ((i - p) / m + 0.5);
							PathWrite(pTheApp, pDoc, pView, x1, i);
						}
					}
				}
				else if (y1 != y2)
				{
					if (y1 > y2)
					{
						i = y1;
						y1 = y2;
						y2 = i;
					}
					for (i = y1; i <= y2; i++)
					{
						PathWrite(pTheApp, pDoc, pView, x1, i);
					}
				}
				else
				{
					PathWrite(pTheApp, pDoc, pView, x1, y1);
				}
			}
		}
		CRect rect(m_ptLast, point);
		rect.NormalizeRect();
		rect.InflateRect(m_iSize*CR_WIDTH/4+2*CR_WIDTH, m_iSize*CR_HEIGHT/4+2*CR_HEIGHT);
		pView->InvalidatePathBitmap(rect);
		pView->InvalidateBitmapRect(rect);
		m_ptLast = point;
	}

}

void CToolPathEdit::OnLButtonUp(CW3ZMapEditView *pView, int nFlags, CPoint point)
{
	if (m_bLeftTracking == TRUE)
	{
		CW3ZMapEditDoc *pDoc = (CW3ZMapEditDoc *)pView->GetDocument();
		ASSERT_VALID(pDoc);
		CW3ZMapEditApp *pTheApp = (CW3ZMapEditApp *)AfxGetApp();
		ASSERT_VALID(pTheApp);

		double m, p;
		int x1, y1, x2, y2, i;
		if ((point.x < pView->m_pBitmapInfo->bV4Width) && (point.y < pView->m_pBitmapInfo->bV4Height))
		{
			x1 = (m_ptLast.x) / (CR_WIDTH/4);
			y1 = (m_ptLast.y) / (CR_HEIGHT/4);
			x2 = (point.x) / (CR_WIDTH/4);
			y2 = (point.y) / (CR_HEIGHT/4);
			if (pDoc && pTheApp)
			{
				if (x1 != x2)
				{
					m = (double)(y1 - y2) / (double)(x1 - x2);
					p = y2 - m*x2;
					if (m*m <= 1)
					{// suivant les x
						if (x1 > x2)
						{
							i = x1;
							x1 = x2;
							x2 = i;
						}
						for (i = x1; i <= x2; i++)
						{
							y1 = (int) (m*i + p + 0.5);
							PathWrite(pTheApp, pDoc, pView, i, y1);
						}
					}
					else
					{ // suivant les y
						if (y1 > y2)
						{
							i = y1;
							y1 = y2;
							y2 = i;
						}
						for (i = y1; i <= y2; i++)
						{
							x1 = (int) ((i - p) / m + 0.5);
							PathWrite(pTheApp, pDoc, pView, x1, i);
						}
					}
				}
				else if (y1 != y2)
				{
					if (y1 > y2)
					{
						i = y1;
						y1 = y2;
						y2 = i;
					}
					for (i = y1; i <= y2; i++)
					{
						PathWrite(pTheApp, pDoc, pView, x1, i);
					}
				}
				else
				{
					PathWrite(pTheApp, pDoc, pView, x1, y1);
				}
			}
		}
		if (pDoc)
		{pDoc->CloseUndoObj();}
		CRect rect(m_ptLast, point);
		rect.NormalizeRect();
		rect.InflateRect(m_iSize*CR_WIDTH/4+2*CR_WIDTH, m_iSize*CR_HEIGHT/4+2*CR_HEIGHT);
		pView->InvalidatePathBitmap(rect);
		pView->InvalidateBitmapRect(rect);
		m_ptLast = point;
	}
	CTool::OnLButtonUp(pView, nFlags, point);
}


void CToolPathEdit::SetCursor()
{
	::SetCursor(AfxGetApp()->LoadCursor(IDC_PATH));
}


void CToolPathEdit::PathWrite(CW3ZMapEditApp *pTheApp, CW3ZMapEditDoc *pDoc, CW3ZMapEditView *pView, int x, int y)
{
	int i, j, x0, y0, x1, y1;
	int oldflag;
	byte* pBitmap = pDoc->m_War3Map.mapTGAPath.GetBitmapPtr();
	byte* pBytemap = pDoc->m_War3Map.GetPathMapPtr();

	CMainFrame *pMainFrame = NULL;

	pMainFrame = pTheApp->GetMainFrame();
	if (pMainFrame) //if the Main Frame exists
	{
		if (m_iSize >= TOOL_PATH_DEFAULT_SIZE)
		{
			x0 = x - (x + TOOL_PATH_DEFAULT_SIZE/2)%TOOL_PATH_DEFAULT_SIZE;
			y0 = y - (y + TOOL_PATH_DEFAULT_SIZE/2)%TOOL_PATH_DEFAULT_SIZE;
		}
		else
		{
			x0 = x;
			y0 = y;
		}

		for (y1 = y0; y1 < y0 + m_iSize; y1++)
		for (x1 = x0; x1 < x0 + m_iSize; x1++)
		if ((x1 >= 0) && (y1 >= 0) && (x1 < pDoc->m_War3Map.GetPathMapWidth()) && (y1 < pDoc->m_War3Map.GetPathMapHeight()))
		{
			// WPM
			i = (((pDoc->m_War3Map.GetPathMapHeight() - y1 - 1)*pDoc->m_War3Map.GetPathMapWidth()) + x1);
			oldflag = (int)(pBytemap[i]);
			pDoc->AddPathToUndoObj(oldflag, x1, y1); 
			pBytemap[i] = (byte)(pMainFrame->GetPathEditBarNewFlags(oldflag));

			// Path.TGA
			if ((x1 < pDoc->m_War3Map.mapTGAPath.GetWidth())
				&& (y1 < pDoc->m_War3Map.mapTGAPath.GetHeight()))
			{
				j = (y1*pDoc->m_War3Map.mapTGAPath.GetWidth() + x1) * 4;
				if (pBytemap[i] & PATH_NOWALK)
				{
					pBitmap[j+2] = 0xFF; // red
				}
				else
				{
					pBitmap[j+2] = 0; // red
				}

				if (pBytemap[i] & PATH_NOBUILD)
				{
					pBitmap[j] = 0xFF; // blue
				}
				else
				{
					pBitmap[j] = 0; // blue
				}

				if (pBytemap[i] & PATH_NOFLY)
				{
					pBitmap[j+1] = 0xFF; // green
				}
				else
				{
					pBitmap[j+1] = 0; // green
				}

				if (pBytemap[i] & PATH_BLIGHT)
				{
					pBitmap[j+3] = 0xFF; // alpha
				}
				else
				{
					pBitmap[j+3] = 0; // alpha
				}
			
			}
			pDoc->SetModifiedFlag();
		}
	}
}

void CToolPathEdit::SetToolSize(int size)
{
 if ((size >= TOOL_PATH_MIN_SIZE) && (size <= TOOL_PATH_MAX_SIZE))
 {m_iSize = size;}
}

int CToolPathEdit::GetToolSize()
{
	return m_iSize;
}
