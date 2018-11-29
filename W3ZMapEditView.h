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
// W3ZMapEditView.h : interface of the CW3ZMapEditView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_W3ZMAPEDITVIEW_H__DB361DB7_DD99_4124_9B95_1DA0E8CBAB8A__INCLUDED_)
#define AFX_W3ZMAPEDITVIEW_H__DB361DB7_DD99_4124_9B95_1DA0E8CBAB8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTilePoint;
class CDoodad;
class CUnit;

class CW3ZMapEditView : public CScrollView
{
protected: // create from serialization only
	CW3ZMapEditView();
	DECLARE_DYNCREATE(CW3ZMapEditView)

// Attributes
public:
	CW3ZMapEditDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CW3ZMapEditView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL IsModeActive(int mode, int x, int y);
	void DisplayUnit(CUnit unit);
	CPoint GetScrollPoint(CPoint point);
	void ChangeBitmapsSize();
	void RemoveDoodad(CDoodad doodad);
	void UpdateTilesColors();
	void InvalidatePlayersBitmap(CRect rect);
	void InvalidateUnitsBitmap(CRect rect);
	void InvalidatePathBitmap(CRect rect);
	void InvalidateShadowsBitmap(CRect rect);
	void InvalidateTilesBitmap(CRect rect);
	int m_nbActiveFilters;
	void AddPlayersFilter(CRect rect);
	void AddUnitsFilter(CRect rect);
	void AddShadowsFilter(CRect rect);
	void AddPathFilter(CRect rect);
	void InvalidateDoodadsBitmap(CRect rect);
	double GetZoom();
	void SetZoom(double zoom);
	void AddDoodadsFilter(CRect rect);
	void AddTilesFilter(CRect rect);
	void InvalidateBitmapRect(CRect rect);
	void DisplayDoodad(CDoodad doodad);
	int m_iViewMode;
	int m_iViewFlags;
	void DisplayTilePoint(int x, int y, CTilePoint tilepoint);
	void InvalidateBitmap();
	virtual ~CW3ZMapEditView();
	// tileset bitmap
	unsigned char m_color;
	unsigned char m_backcolor;
	unsigned char * m_pMainBitmap;
	unsigned char * m_pTilesBitmap;
	unsigned char * m_pDoodadsBitmap;
	unsigned char * m_pUnitsBitmap;
	unsigned char * m_pShadowsBitmap;
	unsigned char * m_pPathBitmap;
	unsigned char * m_pImage;
	RGBQUAD * m_pPalette;
	BITMAPV4HEADER * m_pBitmapInfo;
	CIEXYZTRIPLE m_pCiexyzTriple;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	COLORREF m_TilesColors[W3E_MAX_GROUNDTILES];
	float m_afWaterColor[4];
	COLORREF m_tRamp;
	COLORREF m_tBlight;
	COLORREF m_tBoundary;
	float m_fRampAlpha;
	float m_fBlightAlpha;
	float m_fBoundaryAlpha;
	float m_fLevelFactor;
	CW3ZMapEditSettings * m_pAppSettings;
	bool m_MRBtnDown;
	bool m_MLBtnDown;
	double m_dZoom;
	//{{AFX_MSG(CW3ZMapEditView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnViewRefresh();
	afx_msg void OnBtnZoomIn();
	afx_msg void OnBtnZoomNo();
	afx_msg void OnBtnZoomOut();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in W3ZMapEditView.cpp
inline CW3ZMapEditDoc* CW3ZMapEditView::GetDocument()
   { return (CW3ZMapEditDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_W3ZMAPEDITVIEW_H__DB361DB7_DD99_4124_9B95_1DA0E8CBAB8A__INCLUDED_)
