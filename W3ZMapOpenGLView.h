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
#pragma once

// Vue CW3ZMapOpenGLView
#include "GLCamera.h"

class CW3ZMapOpenGLView : public CView
{
	DECLARE_DYNCREATE(CW3ZMapOpenGLView)

protected:
	int			m_iPolygonOffsetVersion;
	BOOL		m_bUpdateViewPort;
	GLint		m_iIDWater;
	GLint		m_iIDGround;
	GLint		m_iIDShadow;
	GLint		m_iIDPathing;
	GLclampd	m_tFOV, m_tAspect;
	GLclampd	m_tZNear, m_tZFar;
	CPoint		m_tMousePos;
	BOOL		m_bRightMouseTrack;
	CGLCamera   m_tCamera;
	float       m_afTilesColors[W3E_MAX_GROUNDTILES][3];
	float       m_afWaterColor[4];
	CRect       m_oldRect;
	CClientDC   *m_pDC;
	HGLRC       m_tHrc;
	CW3ZMapEditSettings * m_pAppSettings;
	CW3ZMapOpenGLView();           // constructeur protégé utilisé par la création dynamique
	virtual ~CW3ZMapOpenGLView();
	void Init();
	BOOL bSetupPixelFormat();
	void DrawScene(void);

public:
	// Polygon Offset Version
	enum { W3ZMEGL_POFS_MISSING, W3ZMEGL_POFS_EXTENSION, W3ZMEGL_POFS_1_1 };
	virtual void OnDraw(CDC* pDC);      // substitué pour dessiner cette vue
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnInitialUpdate();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	void Update3DView(void);
	afx_msg void OnViewRefresh();
};


