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
// W3ZMapOpenGLView.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "W3ZMapEdit.h"
#include "W3ZMapEditDoc.h"
#include "W3ZMapOpenGLView.h"
#include "W3ZMapEditView.h"
#include "TilePoint.h"
#include "w3zmapopenglview.h"
#include "GLCamera.h"
#include "w3zmapopenglview.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
// M_PI may not be defined in some "math.h"!
#ifndef M_PI
	#define M_PI	3.1415926535897932384626433832795
#endif


// CW3ZMapOpenGLView

IMPLEMENT_DYNCREATE(CW3ZMapOpenGLView, CView)

CW3ZMapOpenGLView::CW3ZMapOpenGLView()
{
    CW3ZMapEditApp(* pTheApp) = (CW3ZMapEditApp *)AfxGetApp();
	m_pAppSettings = &(pTheApp->m_AppSettings);
	COLORREF tColor;
//	m_bMRBtnDown = false;
//	m_bMLBtnDown = false;
	tColor = m_pAppSettings->GetTileColor(TS_STR_ID_UNDEFINED);
	float fRed, fGreen, fBlue;
	fRed = (tColor & 0x0000FF) / 255.0f; // red
	tColor >>= 8;
	fGreen = (tColor & 0x0000FF) / 255.0f; // green
	tColor >>= 8;
	fBlue = (tColor & 0x0000FF) / 255.0f; // blue
	for (int i=0; i < W3E_MAX_GROUNDTILES;i++)
	{
		m_afTilesColors[i][0] = fRed; // red
		m_afTilesColors[i][1] = fGreen; // green
		m_afTilesColors[i][2] = fBlue; // blue
	}
	m_afWaterColor[0] = (TS_WATER_COLOR & 0x0000FF) / 255.0f; // red
	m_afWaterColor[1] = ((TS_WATER_COLOR>>8) & 0x0000FF) / 255.0f; // green
	m_afWaterColor[2] = ((TS_WATER_COLOR>>16) & 0x0000FF) / 255.0f; // blue
	m_afWaterColor[3] = TS_WATER_ALPHA;

	m_pDC = NULL;
	m_bRightMouseTrack = FALSE;
	m_tFOV = 45.0;
	m_tAspect = 1.0;
	m_tZNear = 0.01;
	m_tZFar = 1000.0;

	m_bUpdateViewPort = FALSE;
}

CW3ZMapOpenGLView::~CW3ZMapOpenGLView()
{
	glDeleteLists(m_iIDGround, (GLsizei)1);
	glDeleteLists(m_iIDWater, (GLsizei)1);
	glDeleteLists(m_iIDShadow, (GLsizei)1);
	glDeleteLists(m_iIDPathing, (GLsizei)1);
}

BEGIN_MESSAGE_MAP(CW3ZMapOpenGLView, CView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
//	ON_WM_KEYUP()
ON_WM_MOUSEWHEEL()
ON_COMMAND(ID_VIEW_REFRESH, OnViewRefresh)
//ON_WM_CLOSE()
//ON_WM_CLOSE()
END_MESSAGE_MAP()


void CW3ZMapOpenGLView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	COLORREF tColor;
	CW3ZMapEditDoc* pDoc = (CW3ZMapEditDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	CString str;
	CWar3StrID tileID;

//checks OpenGL1.1...
#ifdef GL_VERSION_1_1
	const char *szVersion;
	int iMajor, iMinor;

	szVersion = (char *) glGetString(GL_VERSION);
	if (sscanf(szVersion, "%d.%d", &iMajor, &iMinor) != 2)
	{ // malformed version string
		iMajor = 0;
		iMinor = 0;
	}
	if ((iMajor > 1) || ((iMajor == 1) && (iMinor >= 1)))
	{
		m_iPolygonOffsetVersion = W3ZMEGL_POFS_1_1;
		glPolygonOffset(-2.0, -1.0);
	}
	else
#endif
//...+checks 1.1
	{
//+polygon offset feature...
#ifdef GL_EXT_polygon_offset
		// check for the polygon offset extension
		if (glutExtensionSupported("GL_EXT_polygon_offset"))
		{
			m_iPolygonOffsetVersion = W3ZMEGL_POFS_EXTENSION;
			glPolygonOffsetEXT(-0.1, -0.002);
		}
		else
#endif
//...+polygon offset feature
			m_iPolygonOffsetVersion = W3ZMEGL_POFS_MISSING;
	}
//...checks OpenGL1.1

	// initialize color pallette
	int iNbTilesets = pDoc->m_War3Map.mapGroundTiles.GetLength() / 4;
	if (iNbTilesets > W3E_MAX_GROUNDTILES)
	{
		iNbTilesets = W3E_MAX_GROUNDTILES;
	}
	str = pDoc->m_War3Map.mapGroundTiles;
	for (int i=0; i < iNbTilesets; i++)
	{
		tileID = str.Mid(i*4, 4);
		tColor = m_pAppSettings->GetTileColor(tileID);
		m_afTilesColors[i][0] = (tColor & 0x0000FF) / 255.0f; // red
		tColor >>= 8;
		m_afTilesColors[i][1] = (tColor & 0x0000FF) / 255.0f; // green
		tColor >>= 8;
		m_afTilesColors[i][2] = (tColor & 0x0000FF) / 255.0f; // blue
	}

	tColor = m_pAppSettings->GetWaterColor(pDoc->m_War3Map.mapMainTileset);
	m_afWaterColor[0] = (tColor & 0x0000FF) / 255.0f; // red
	tColor >>= 8;
	m_afWaterColor[1] = (tColor & 0x0000FF) / 255.0f; // green
	tColor >>= 8;
	m_afWaterColor[2] = (tColor & 0x0000FF) / 255.0f; // blue
	m_afWaterColor[3] = m_pAppSettings->GetWaterAlpha(pDoc->m_War3Map.mapMainTileset);

	// -gets IDs
	m_iIDGround = glGenLists(1);
	m_iIDWater = glGenLists(1);
	m_iIDShadow = glGenLists(1);
	m_iIDPathing = glGenLists(1);
	// creates models
	Update3DView();
}

// Dessin CW3ZMapOpenGLView

void CW3ZMapOpenGLView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	DrawScene();
}


// Diagnostics CW3ZMapOpenGLView

#ifdef _DEBUG
void CW3ZMapOpenGLView::AssertValid() const
{
	CView::AssertValid();
}

void CW3ZMapOpenGLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


// Gestionnaires de messages CW3ZMapOpenGLView

int CW3ZMapOpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	Init(); // initialize OpenGL

	return 0;
}

BOOL CW3ZMapOpenGLView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	return CView::PreCreateWindow(cs);
}

void CW3ZMapOpenGLView::OnDestroy()
{
	::wglMakeCurrent(NULL,  NULL);

	if (m_tHrc)
		::wglDeleteContext(m_tHrc);

	if (m_pDC)
		delete m_pDC;

	CView::OnDestroy();
}


void CW3ZMapOpenGLView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (cy > 0)
	{
		m_tAspect = (GLdouble)cx/cy;
		m_bUpdateViewPort = TRUE;
		if((m_oldRect.right > cx) || (m_oldRect.bottom > cy))
		{
			RedrawWindow();
		}
		m_oldRect.right = cx;
		m_oldRect.bottom = cy;
	}
}


void CW3ZMapOpenGLView::Init()
{
	PIXELFORMATDESCRIPTOR pfd;
	int         n;

	m_pDC = new CClientDC(this);

	ASSERT(m_pDC != NULL);

	if (!bSetupPixelFormat())
		return;

	n = ::GetPixelFormat(m_pDC->GetSafeHdc());
	::DescribePixelFormat(m_pDC->GetSafeHdc(), n, sizeof(pfd), &pfd);

	m_tHrc = wglCreateContext(m_pDC->GetSafeHdc());
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_tHrc);

	GetClientRect(&m_oldRect);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	if (m_oldRect.bottom)
	{
		m_tAspect = (GLfloat)m_oldRect.right/m_oldRect.bottom;
	}
	else    // don't divide by zero, not that we should ever run into that...
	{
		m_tAspect = 1.0f;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(m_tFOV, m_tAspect, m_tZNear, m_tZFar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// camera init
	GLdouble agldPos[3];
	agldPos[0] = 0;
	agldPos[1] = 1;
	agldPos[2] = 0;
	m_tCamera.SetUpVector(agldPos);
	agldPos[0] = 0;
	agldPos[1] = 20;
	agldPos[2] = 0;
	m_tCamera.SetPosition(agldPos);
	agldPos[0] = 50;
	agldPos[1] = 0;
	agldPos[2] = 50;
	m_tCamera.SetTarget(agldPos);
	m_tCamera.SavePosition();
	m_tCamera.Watch();

}

BOOL CW3ZMapOpenGLView::bSetupPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
		1,                              // version number
		PFD_DRAW_TO_WINDOW |            // support window
		  PFD_SUPPORT_OPENGL |          // support OpenGL
		  PFD_DOUBLEBUFFER,             // double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		24,                             // 24-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		32,                             // 32-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};
	int pixelformat;

	if ( (pixelformat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd)) == 0 )
	{
		MessageBox("ChoosePixelFormat failed");
		return FALSE;
	}

	if (SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd) == FALSE)
	{
		MessageBox("SetPixelFormat failed");
		return FALSE;
	}

	return TRUE;
}


void CW3ZMapOpenGLView::DrawScene(void)
{
	static BOOL     bBusy = FALSE;
	GLenum gleBlendSrc, gleBlendDst;

	if (bBusy)
	{
		return;
	}
	bBusy = TRUE;

//+zep...
	ASSERT(m_pDC != NULL);
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_tHrc);
//...+zep
	CW3ZMapEditDoc* pDoc = (CW3ZMapEditDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	POSITION pos = pDoc->GetFirstViewPosition();
	CW3ZMapEditView* pView = (CW3ZMapEditView *)pDoc->GetNextView(pos);

	if (TRUE == m_bUpdateViewPort)
	{
		CRect tClientRect;
		GetClientRect(&tClientRect);
		glViewport(0, 0, tClientRect.right, tClientRect.bottom);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(m_tFOV, m_tAspect, m_tZNear, m_tZFar);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
		m_tCamera.Watch();

		// ground
		if ((NULL != pView) && (0 != (pView->m_iViewFlags & VFL_TILES)))
		{
//			glShadeModel(GL_FLAT);
			glCallList(m_iIDGround);
		}

		// shadows
		if ((NULL != pView) && (0 != (pView->m_iViewFlags & VFL_SHADOWS)))
		{
			// To eliminate depth buffer artifacts, I use polygon offset
			// to raise the depth of the projected shadow slightly so
			// that it does not depth buffer alias with the floor.
			switch (m_iPolygonOffsetVersion)
			{
			case W3ZMEGL_POFS_EXTENSION:
#ifdef GL_EXT_polygon_offset
				{
					glEnable(GL_POLYGON_OFFSET_EXT);
					break;
				}
#endif
			case W3ZMEGL_POFS_1_1:
#ifdef GL_VERSION_1_1
				{
					glEnable(GL_POLYGON_OFFSET_FILL);
					break;
				}
#endif
			case W3ZMEGL_POFS_MISSING:
				{
					break;
				}
			}
			glGetIntegerv(GL_BLEND_SRC, (GLint*)&gleBlendSrc);
			glGetIntegerv(GL_BLEND_DST, (GLint*)&gleBlendDst);
			glEnable(GL_BLEND); // enables blending
			glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
//			glTranslatef(0.0f, 0.1f, 0.0f);
			glCallList(m_iIDShadow);
			glDisable(GL_BLEND); // disables blending
			//restore blending function
			glBlendFunc(gleBlendSrc, gleBlendDst);
			switch (m_iPolygonOffsetVersion)
			{
#ifdef GL_EXT_polygon_offset
			case W3ZMEGL_POFS_EXTENSION:
				{
					glDisable(GL_POLYGON_OFFSET_EXT);
					break;
				}
#endif
			case W3ZMEGL_POFS_1_1:
#ifdef GL_VERSION_1_1
				{
					glDisable(GL_POLYGON_OFFSET_FILL);
					break;
				}
#endif
			case W3ZMEGL_POFS_MISSING:
				{
					break;
				}
			}
		}

		// pathing
		if ((NULL != pView) && (0 != (pView->m_iViewFlags & VFL_PATH)))
		{
			// To eliminate depth buffer artifacts, I use polygon offset
			// to raise the depth of the projected shadow slightly so
			// that it does not depth buffer alias with the floor.
			switch (m_iPolygonOffsetVersion)
			{
			case W3ZMEGL_POFS_EXTENSION:
#ifdef GL_EXT_polygon_offset
				{
					glEnable(GL_POLYGON_OFFSET_EXT);
					break;
				}
#endif
			case W3ZMEGL_POFS_1_1:
#ifdef GL_VERSION_1_1
				{
					glEnable(GL_POLYGON_OFFSET_FILL);
					break;
				}
#endif
			case W3ZMEGL_POFS_MISSING:
				{
					break;
				}
			}
			glGetIntegerv(GL_BLEND_SRC, (GLint*)&gleBlendSrc);
			glGetIntegerv(GL_BLEND_DST, (GLint*)&gleBlendDst);
			glEnable(GL_BLEND); // enables blending
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // transparency
			glPushMatrix();
				glTranslatef(0.0f, 0.1f, 0.0f);
				glCallList(m_iIDPathing);
			glPopMatrix();
			glDisable(GL_BLEND); // disables blending
			//restore blending function
			glBlendFunc(gleBlendSrc, gleBlendDst);
			switch (m_iPolygonOffsetVersion)
			{
#ifdef GL_EXT_polygon_offset
			case W3ZMEGL_POFS_EXTENSION:
				{
					glDisable(GL_POLYGON_OFFSET_EXT);
					break;
				}
#endif
			case W3ZMEGL_POFS_1_1:
#ifdef GL_VERSION_1_1
				{
					glDisable(GL_POLYGON_OFFSET_FILL);
					break;
				}
#endif
			case W3ZMEGL_POFS_MISSING:
				{
					break;
				}
			}
		}

		//water
		if ((NULL != pView) && (0 != (pView->m_iViewFlags & VFL_WATER)))
		{
			// To eliminate depth buffer artifacts, I use polygon offset
			// to raise the depth of the projected shadow slightly so
			// that it does not depth buffer alias with the floor.
			switch (m_iPolygonOffsetVersion)
			{
			case W3ZMEGL_POFS_EXTENSION:
#ifdef GL_EXT_polygon_offset
				{
					glEnable(GL_POLYGON_OFFSET_EXT);
					break;
				}
#endif
			case W3ZMEGL_POFS_1_1:
#ifdef GL_VERSION_1_1
				{
					glEnable(GL_POLYGON_OFFSET_FILL);
					break;
				}
#endif
			case W3ZMEGL_POFS_MISSING:
				{
					break;
				}
			}
			glGetIntegerv(GL_BLEND_SRC, (GLint*)&gleBlendSrc);
			glGetIntegerv(GL_BLEND_DST, (GLint*)&gleBlendDst);
			glEnable(GL_BLEND); // enables blending
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // transparency
			glCallList(m_iIDWater);
			glDisable(GL_BLEND); // disables blending
			//restore blending function
			glBlendFunc(gleBlendSrc, gleBlendDst);
			switch (m_iPolygonOffsetVersion)
			{
#ifdef GL_EXT_polygon_offset
			case W3ZMEGL_POFS_EXTENSION:
				{
					glDisable(GL_POLYGON_OFFSET_EXT);
					break;
				}
#endif
			case W3ZMEGL_POFS_1_1:
#ifdef GL_VERSION_1_1
				{
					glDisable(GL_POLYGON_OFFSET_FILL);
					break;
				}
#endif
			case W3ZMEGL_POFS_MISSING:
				{
					break;
				}
			}
		}
	glPopMatrix();
	glFinish();
	SwapBuffers(wglGetCurrentDC());

	bBusy = FALSE;
}


BOOL CW3ZMapOpenGLView::OnEraseBkgnd(CDC* pDC)
{
	//CView::OnEraseBkgnd(pDC);
	return TRUE; 
}

void CW3ZMapOpenGLView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	SHORT siShift = GetKeyState(VK_SHIFT);
	SHORT siControl = GetKeyState(VK_CONTROL);
	switch(nChar)
	{
		case VK_UP:
		{
			m_tCamera.ZoomIn(10.0);
			RedrawWindow();
			break;
		}
	case VK_DOWN:
		{
			m_tCamera.ZoomOut(10.0);
			RedrawWindow();
			break;
		}
	case VK_LEFT:
		{
			m_tCamera.TurnLeft(M_PI/20.0);
			RedrawWindow();
			break;
		}
	case VK_RIGHT:
		{
			m_tCamera.TurnRight(M_PI/20.0);
			RedrawWindow();
			break;
		}
	case VK_PRIOR:
		{
			m_tCamera.MoveUp(2.0);
			RedrawWindow();
			break;
		}
	case VK_NEXT:
		{
			m_tCamera.MoveDown(2.0);
			RedrawWindow();
			break;
		}
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CW3ZMapOpenGLView::OnMouseMove(UINT nFlags, CPoint point)
{
	const double OPENGL_VIEW_SENSITIVITY = 50.;
	double dSensitivity = OPENGL_VIEW_SENSITIVITY;
	if (TRUE == m_bRightMouseTrack)
	{
		SHORT siShift = GetKeyState(VK_SHIFT);
		SHORT siControl = GetKeyState(VK_CONTROL);
		if ((0 != (siShift & 0x80)) && (0 != (siControl & 0x80)))
		{
//			m_tZFar += ((point.y - m_tMousePos.y)/OPENGL_VIEW_SENSITIVITY);
//			gluPerspective(m_tFOV, m_tAspect, m_tZNear, m_tZFar);
		}
		else if (0 != (siShift & 0x80))
		{
			m_tCamera.ZoomOut((point.y - m_tMousePos.y));
		}
		else if (0 != (siControl & 0x80))
		{
			GLdouble agldCoords[3];
			m_tCamera.GetPosition(agldCoords);
			dSensitivity *= (10/agldCoords[1]);
			m_tCamera.TurnDown((point.y - m_tMousePos.y)/(OPENGL_VIEW_SENSITIVITY*10));
			m_tCamera.MoveUp((point.y - m_tMousePos.y)/dSensitivity);
			m_tCamera.TurnRight((point.x - m_tMousePos.x)/(OPENGL_VIEW_SENSITIVITY*10));
			m_tCamera.StrafeLeft((point.x - m_tMousePos.x)/dSensitivity);
		}
		else
		{
			GLdouble agldCoords[3];
			m_tCamera.GetPosition(agldCoords);
			dSensitivity *= (10/agldCoords[1]);
			m_tCamera.MoveForward((point.y - m_tMousePos.y)/dSensitivity);
			m_tCamera.StrafeLeft((point.x - m_tMousePos.x)/dSensitivity);
		}
		m_tMousePos = point;
		RedrawWindow();
	}

	CView::OnMouseMove(nFlags, point);
}

void CW3ZMapOpenGLView::OnRButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	m_bRightMouseTrack = TRUE;
	m_tMousePos = point;
	CView::OnRButtonDown(nFlags, point);
}

void CW3ZMapOpenGLView::OnRButtonUp(UINT nFlags, CPoint point)
{
	m_bRightMouseTrack = FALSE;
	ReleaseCapture();

	CView::OnRButtonUp(nFlags, point);
}

void CW3ZMapOpenGLView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO : ajoutez ici le code de votre gestionnaire de messages et/ou les paramètres par défaut des appels

	CView::OnLButtonUp(nFlags, point);
}

void CW3ZMapOpenGLView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO : ajoutez ici le code de votre gestionnaire de messages et/ou les paramètres par défaut des appels

	CView::OnLButtonDown(nFlags, point);
}

BOOL CW3ZMapOpenGLView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
//	SHORT siShift = GetKeyState(VK_SHIFT);
//	SHORT siControl = GetKeyState(VK_CONTROL);
//	if (0 != (siControl & 0x80))
//	{
//			m_tZFar += ((point.y - m_tMousePos.y)/OPENGL_VIEW_SENSITIVITY);
//			gluPerspective(m_tFOV, m_tAspect, m_tZNear, m_tZFar);
//	}
	m_tCamera.ZoomIn(zDelta/10.);
	RedrawWindow();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CW3ZMapOpenGLView::Update3DView(void)
{
//	glPushMatrix();
		int x, y, i, j;
		float fX, fY, fZ, fX2, fZ2, fY2, fX3, fZ3, fY3, fX4, fZ4, fY4;
		float fTPY1, fTPY2, fTPY3, fTPY4;
		double dInterY;
		int mapHeight, mapWidth;
		CTilePoint tilepoint;
		BOOL bShowBounds;
		const float fLayerHeight = (float)TILE_LAYER_HEIGHT;

		CW3ZMapEditDoc* pDoc = (CW3ZMapEditDoc*)GetDocument();
		ASSERT_VALID(pDoc);
		POSITION pos = pDoc->GetFirstViewPosition();
		CW3ZMapEditView* pView = (CW3ZMapEditView *)pDoc->GetNextView(pos);
		if ((NULL != pView) && (0 != (pView->m_iViewFlags & (VFL_BOUNDS | VFL_WEBOUNDS))))
		{bShowBounds= TRUE;}
		else
		{bShowBounds= FALSE;}

		mapWidth = (int)pDoc->m_War3Map.GetMapWidth() + 1;
		mapHeight = (int)pDoc->m_War3Map.GetMapHeight() + 1;

		// generates ground surface
		glNewList(m_iIDGround, GL_COMPILE);
			glBegin(GL_QUADS);
				for(y=0; y < mapHeight-1; y++)
				{
					for(x=0; x < mapWidth-1; x++)
					{
						fX = (float)x;
						fZ = (float)(mapHeight - 1 - y);

						tilepoint = pDoc->m_War3Map.GetTilePoint(x,y);
						fY = (float)tilepoint.GetGroundLevel();
						if (tilepoint.flags & (TILE_FL_MINISHAD | TILE_FL_SHADOW) && bShowBounds)
						{
							glColor3f(m_afTilesColors[tilepoint.groundtype][0]/4, m_afTilesColors[tilepoint.groundtype][1]/4, m_afTilesColors[tilepoint.groundtype][2]/4);
						}
						else
						{
							glColor3f(m_afTilesColors[tilepoint.groundtype][0], m_afTilesColors[tilepoint.groundtype][1], m_afTilesColors[tilepoint.groundtype][2]);
						}
						glVertex3f(fX, fY/fLayerHeight, fZ);

						tilepoint = pDoc->m_War3Map.GetTilePoint(x+1,y);
						fY = (float)tilepoint.GetGroundLevel();
						if (tilepoint.flags & (TILE_FL_MINISHAD | TILE_FL_SHADOW) && bShowBounds)
						{
							glColor3f(m_afTilesColors[tilepoint.groundtype][0]/4, m_afTilesColors[tilepoint.groundtype][1]/4, m_afTilesColors[tilepoint.groundtype][2]/4);
						}
						else
						{
							glColor3f(m_afTilesColors[tilepoint.groundtype][0], m_afTilesColors[tilepoint.groundtype][1], m_afTilesColors[tilepoint.groundtype][2]);
						}
						glVertex3f(fX+1, fY/fLayerHeight, fZ);

						tilepoint = pDoc->m_War3Map.GetTilePoint(x+1,y+1);
						fY = (float)tilepoint.GetGroundLevel();
						if (tilepoint.flags & (TILE_FL_MINISHAD | TILE_FL_SHADOW) && bShowBounds)
						{
							glColor3f(m_afTilesColors[tilepoint.groundtype][0]/4, m_afTilesColors[tilepoint.groundtype][1]/4, m_afTilesColors[tilepoint.groundtype][2]/4);
						}
						else
						{
							glColor3f(m_afTilesColors[tilepoint.groundtype][0], m_afTilesColors[tilepoint.groundtype][1], m_afTilesColors[tilepoint.groundtype][2]);
						}
						glVertex3f(fX+1, fY/fLayerHeight, fZ-1);

						tilepoint = pDoc->m_War3Map.GetTilePoint(x,y+1);
						fY = (float)tilepoint.GetGroundLevel();
						if (tilepoint.flags & (TILE_FL_MINISHAD | TILE_FL_SHADOW) && bShowBounds)
						{
							glColor3f(m_afTilesColors[tilepoint.groundtype][0]/4, m_afTilesColors[tilepoint.groundtype][1]/4, m_afTilesColors[tilepoint.groundtype][2]/4);
						}
						else
						{
							glColor3f(m_afTilesColors[tilepoint.groundtype][0], m_afTilesColors[tilepoint.groundtype][1], m_afTilesColors[tilepoint.groundtype][2]);
						}
						glVertex3f(fX, fY/fLayerHeight, fZ-1);
					}
				}
			glEnd();
		glEndList();

		// generates water surface
		glNewList(m_iIDWater, GL_COMPILE);
			glBegin(GL_QUADS);
				BOOL bHasWater;
				glColor4f(m_afWaterColor[0], m_afWaterColor[1], m_afWaterColor[2], m_afWaterColor[3]);
				for(y=0; y < mapHeight-1; y++)
				{
					for(x=0; x < mapWidth-1; x++)
					{
						bHasWater = FALSE;

						tilepoint = pDoc->m_War3Map.GetTilePoint(x,y);
						fX = (float)x;
						fZ = (float)(mapHeight - 1 - y);
						fY = (float)tilepoint.GetWaterLevel()/fLayerHeight;
						if (tilepoint.flags & TILE_FL_WATER)
						{bHasWater = TRUE;}

						tilepoint = pDoc->m_War3Map.GetTilePoint(x+1,y);
						fX2 = (float)x+1;
						fZ2 = (float)(mapHeight - 1 - y);
						fY2 = (float)tilepoint.GetWaterLevel()/fLayerHeight;
						if (tilepoint.flags & TILE_FL_WATER)
						{bHasWater = TRUE;}

						tilepoint = pDoc->m_War3Map.GetTilePoint(x+1,y+1);
						fX3 = (float)x+1;
						fZ3 = (float)(mapHeight - 2 - y);
						fY3 = (float)tilepoint.GetWaterLevel()/fLayerHeight;
						if (tilepoint.flags & TILE_FL_WATER)
						{bHasWater = TRUE;}

						tilepoint = pDoc->m_War3Map.GetTilePoint(x,y+1);
						fX4 = (float)x;
						fZ4 = (float)(mapHeight - 2 - y);
						fY4 = (float)tilepoint.GetWaterLevel()/fLayerHeight;
						if (tilepoint.flags & TILE_FL_WATER)
						{bHasWater = TRUE;}

						if (TRUE == bHasWater)
						{
							glVertex3f(fX, fY, fZ);
							glVertex3f(fX2, fY2, fZ2);
							glVertex3f(fX3, fY3, fZ3);
							glVertex3f(fX4, fY4, fZ4);
						}
					}
				}
			glEnd();
		glEndList();

		// Path map
		// generates water surface
		glNewList(m_iIDPathing, GL_COMPILE);
			glBegin(GL_QUADS);
				byte* pabyPathMap = pDoc->m_War3Map.GetPathMapPtr();
				byte byPathData;
				for(y=0; y < mapHeight-1; y++)
				{
					for(x=0; x < mapWidth-1; x++)
					{
						//+FIXME: need code and memory optimisation
						tilepoint = pDoc->m_War3Map.GetTilePoint(x,y); // integer divisions
						fTPY1 = (float)tilepoint.GetGroundLevel()/fLayerHeight;
						tilepoint = pDoc->m_War3Map.GetTilePoint(x+1,y); // integer divisions
						fTPY2 = (float)tilepoint.GetGroundLevel()/fLayerHeight;
						tilepoint = pDoc->m_War3Map.GetTilePoint(x+1,y+1); // integer divisions
						fTPY3 = (float)tilepoint.GetGroundLevel()/fLayerHeight;
						tilepoint = pDoc->m_War3Map.GetTilePoint(x,y+1); // integer divisions
						fTPY4 = (float)tilepoint.GetGroundLevel()/fLayerHeight;

						for (j=0; j<PATH_MAP_SUBDIV; j++)
						{
							for (i=0; i<PATH_MAP_SUBDIV; i++)
							{
								if (i == (PATH_MAP_SUBDIV-1-j))
								{
									fX = (float)i/(float)PATH_MAP_SUBDIV;
									fZ = (float)j/(float)PATH_MAP_SUBDIV;
									dInterY = fTPY2*(double)fX/(1.-(double)fZ) + fTPY1*(1.-(double)fX-(double)fZ)/(1.-(double)fZ); //fZ always != 1
									fY = (float)(fZ*fTPY4 + (1-fZ)*dInterY);

									fX2 = (float)(i+1)/(float)PATH_MAP_SUBDIV;
									fZ2 = (float)j/(float)PATH_MAP_SUBDIV;
									fY2 = fTPY4*(1-fX2) + fTPY2*fX2;

									fX3 = (float)(i+1)/(float)PATH_MAP_SUBDIV;
									fZ3 = (float)(j+1)/(float)PATH_MAP_SUBDIV;
									dInterY = fTPY3*((double)fX3+(double)fZ3-1.)/(double)fZ3 + (double)fTPY4*(1.-(double)fX3)/(double)fZ3; //fZ3 always != 0
									fY3 = (float)((1-(double)fZ3)*(double)fTPY2 + (double)fZ3*dInterY);

									fX4 = (float)i/(float)PATH_MAP_SUBDIV;
									fZ4 = (float)(j+1)/(float)PATH_MAP_SUBDIV;
									fY4 = fTPY4*(1-fX4) + fTPY2*fX4;
								}
								else if (i < (PATH_MAP_SUBDIV-1-j))
								{
									fX = (float)i/(float)PATH_MAP_SUBDIV;
									fZ = (float)j/(float)PATH_MAP_SUBDIV;
									dInterY = fTPY2*(double)fX/(1.-(double)fZ) + fTPY1*(1.-(double)fX-(double)fZ)/(1.-(double)fZ); //fZ always != 1
									fY = (float)(fZ*fTPY4 + (1-fZ)*dInterY);

									fX2 = (float)(i+1)/(float)PATH_MAP_SUBDIV;
									fZ2 = (float)j/(float)PATH_MAP_SUBDIV;
									dInterY = fTPY2*(double)fX2/(1.-(double)fZ2) + fTPY1*(1.-(double)fX2-(double)fZ2)/(1.-(double)fZ2); //fZ2 always != 1
									fY2 = (float)(fZ2*fTPY4 + (1-fZ2)*dInterY);

									fX3 = (float)(i+1)/(float)PATH_MAP_SUBDIV;
									fZ3 = (float)(j+1)/(float)PATH_MAP_SUBDIV;
									dInterY = fTPY2*(double)fX3/(1.-(double)fZ3) + fTPY1*(1.-(double)fX3-(double)fZ3)/(1.-(double)fZ3); //
									fY3 = (float)(fZ3*fTPY4 + (1-fZ3)*dInterY);

									fX4 = (float)i/(float)PATH_MAP_SUBDIV;
									fZ4 = (float)(j+1)/(float)PATH_MAP_SUBDIV;
									dInterY = fTPY2*(double)fX4/(1.-(double)fZ4) + fTPY1*(1.-(double)fX4-(double)fZ4)/(1.-(double)fZ4); //
									fY4 = (float)(fZ4*fTPY4 + (1-fZ4)*dInterY);
								}
								else
								{
									fX = (float)i/(float)PATH_MAP_SUBDIV;
									fZ = (float)j/(float)PATH_MAP_SUBDIV;
									dInterY = fTPY3*((double)fX+(double)fZ-1.)/(double)fZ + (double)fTPY4*(1.-(double)fX)/(double)fZ; //
									fY = (float)((1-(double)fZ)*(double)fTPY2 + (double)fZ*dInterY);

									fX2 = (float)(i+1)/(float)PATH_MAP_SUBDIV;
									fZ2 = (float)j/(float)PATH_MAP_SUBDIV;
									dInterY = fTPY3*((double)fX2+(double)fZ2-1.)/(double)fZ2 + (double)fTPY4*(1.-(double)fX2)/(double)fZ2; //
									fY2 = (float)((1-(double)fZ2)*(double)fTPY2 + (double)fZ2*dInterY);

									fX3 = (float)(i+1)/(float)PATH_MAP_SUBDIV;
									fZ3 = (float)(j+1)/(float)PATH_MAP_SUBDIV;
									dInterY = fTPY3*((double)fX3+(double)fZ3-1.)/(double)fZ3 + (double)fTPY4*(1.-(double)fX3)/(double)fZ3; //fZ3 always != 0
									fY3 = (float)((1-(double)fZ3)*(double)fTPY2 + (double)fZ3*dInterY);

									fX4 = (float)i/(float)PATH_MAP_SUBDIV;
									fZ4 = (float)(j+1)/(float)PATH_MAP_SUBDIV;
									dInterY = fTPY3*((double)fX4+(double)fZ4-1.)/(double)fZ4 + (double)fTPY4*(1.-(double)fX4)/(double)fZ4; //fZ4 always != 0
									fY4 = (float)((1-(double)fZ4)*(double)fTPY2 + (double)fZ4*dInterY);
								}
								byPathData = pabyPathMap[x*PATH_MAP_SUBDIV + i + (y*PATH_MAP_SUBDIV + j)*(mapWidth-1)*PATH_MAP_SUBDIV];

								if ((byPathData & PATH_NOBUILD)
									&& (byPathData & PATH_NOWALK)
									&& (byPathData & PATH_NOFLY))
								{
									glColor4f(m_pAppSettings->pathNoBuildNoWalkNoFly.clrRed/255.0f, m_pAppSettings->pathNoBuildNoWalkNoFly.clrGreen/255.0f, m_pAppSettings->pathNoBuildNoWalkNoFly.clrBlue/255.0f, 0.5f);
								}
								else if ((byPathData & PATH_NOBUILD)
										&& (byPathData & PATH_NOWALK)
										&& (byPathData & PATH_NOWATER))
								{
									glColor4f(m_pAppSettings->pathNoBuildNoWalkNoWater.clrRed/255.0f, m_pAppSettings->pathNoBuildNoWalkNoWater.clrGreen/255.0f, m_pAppSettings->pathNoBuildNoWalkNoWater.clrBlue/255.0f, 0.5f);
								}
								else if ((byPathData & PATH_NOBUILD)
										&& (byPathData & PATH_NOWALK))
								{
									glColor4f(m_pAppSettings->pathNoBuildNoWalk.clrRed/255.0f, m_pAppSettings->pathNoBuildNoWalk.clrGreen/255.0f, m_pAppSettings->pathNoBuildNoWalk.clrBlue/255.0f, 0.5f);
								}
								else
								{
									glColor4f(m_pAppSettings->pathNormal.clrRed/255.0f, m_pAppSettings->pathNormal.clrGreen/255.0f, m_pAppSettings->pathNormal.clrBlue/255.0f, 0.1f);
								}

								//glColor4f(byPathData/255.0f, byPathData/255.0f, byPathData/255.0f, 0.1f);
								glVertex3f(x+fX, fY, mapHeight - 1 - y-fZ);
								glVertex3f(x+fX2, fY2, mapHeight - 1 - y-fZ2);
								glVertex3f(x+fX3, fY3, mapHeight - 1 - y-fZ3);
								glVertex3f(x+fX4, fY4, mapHeight - 1 - y-fZ4);
							}
						}
					}
				}
			glEnd();
		glEndList();

		// Shadow map
		// generates water surface
		glNewList(m_iIDShadow, GL_COMPILE);
			glBegin(GL_QUADS);
				byte* pabyShadowMap = pDoc->m_War3Map.GetShadowMapPtr();
				byte byShadow;
				for(y=0; y < mapHeight-1; y++)
				{
					for(x=0; x < mapWidth-1; x++)
					{
						//+FIXME: need code and memory optimisation
						tilepoint = pDoc->m_War3Map.GetTilePoint(x,y); // integer divisions
						fTPY1 = (float)tilepoint.GetGroundLevel()/fLayerHeight;
						tilepoint = pDoc->m_War3Map.GetTilePoint(x+1,y); // integer divisions
						fTPY2 = (float)tilepoint.GetGroundLevel()/fLayerHeight;
						tilepoint = pDoc->m_War3Map.GetTilePoint(x+1,y+1); // integer divisions
						fTPY3 = (float)tilepoint.GetGroundLevel()/fLayerHeight;
						tilepoint = pDoc->m_War3Map.GetTilePoint(x,y+1); // integer divisions
						fTPY4 = (float)tilepoint.GetGroundLevel()/fLayerHeight;

						for (j=0; j<SHAD_MAP_SUBDIV; j++)
						{
							for (i=0; i<SHAD_MAP_SUBDIV; i++)
							{
								if (i == (SHAD_MAP_SUBDIV-1-j))
								{
									fX = (float)i/(float)SHAD_MAP_SUBDIV;
									fZ = (float)j/(float)SHAD_MAP_SUBDIV;
									dInterY = fTPY2*(double)fX/(1.-(double)fZ) + fTPY1*(1.-(double)fX-(double)fZ)/(1.-(double)fZ); //fZ always != 1
									fY = (float)(fZ*fTPY4 + (1-fZ)*dInterY);

									fX2 = (float)(i+1)/(float)SHAD_MAP_SUBDIV;
									fZ2 = (float)j/(float)SHAD_MAP_SUBDIV;
									fY2 = fTPY4*(1-fX2) + fTPY2*fX2;

									fX3 = (float)(i+1)/(float)SHAD_MAP_SUBDIV;
									fZ3 = (float)(j+1)/(float)SHAD_MAP_SUBDIV;
									dInterY = fTPY3*((double)fX3+(double)fZ3-1.)/(double)fZ3 + (double)fTPY4*(1.-(double)fX3)/(double)fZ3; //fZ3 always != 0
									fY3 = (float)((1-(double)fZ3)*(double)fTPY2 + (double)fZ3*dInterY);

									fX4 = (float)i/(float)SHAD_MAP_SUBDIV;
									fZ4 = (float)(j+1)/(float)SHAD_MAP_SUBDIV;
									fY4 = fTPY4*(1-fX4) + fTPY2*fX4;
								}
								else if (i < (SHAD_MAP_SUBDIV-1-j))
								{
									fX = (float)i/(float)SHAD_MAP_SUBDIV;
									fZ = (float)j/(float)SHAD_MAP_SUBDIV;
									dInterY = fTPY2*(double)fX/(1.-(double)fZ) + fTPY1*(1.-(double)fX-(double)fZ)/(1.-(double)fZ); //fZ always != 1
									fY = (float)(fZ*fTPY4 + (1-fZ)*dInterY);

									fX2 = (float)(i+1)/(float)SHAD_MAP_SUBDIV;
									fZ2 = (float)j/(float)SHAD_MAP_SUBDIV;
									dInterY = fTPY2*(double)fX2/(1.-(double)fZ2) + fTPY1*(1.-(double)fX2-(double)fZ2)/(1.-(double)fZ2); //fZ2 always != 1
									fY2 = (float)(fZ2*fTPY4 + (1-fZ2)*dInterY);

									fX3 = (float)(i+1)/(float)SHAD_MAP_SUBDIV;
									fZ3 = (float)(j+1)/(float)SHAD_MAP_SUBDIV;
									dInterY = fTPY2*(double)fX3/(1.-(double)fZ3) + fTPY1*(1.-(double)fX3-(double)fZ3)/(1.-(double)fZ3); //
									fY3 = (float)(fZ3*fTPY4 + (1-fZ3)*dInterY);

									fX4 = (float)i/(float)SHAD_MAP_SUBDIV;
									fZ4 = (float)(j+1)/(float)SHAD_MAP_SUBDIV;
									dInterY = fTPY2*(double)fX4/(1.-(double)fZ4) + fTPY1*(1.-(double)fX4-(double)fZ4)/(1.-(double)fZ4); //
									fY4 = (float)(fZ4*fTPY4 + (1-fZ4)*dInterY);
								}
								else
								{
									fX = (float)i/(float)SHAD_MAP_SUBDIV;
									fZ = (float)j/(float)SHAD_MAP_SUBDIV;
									dInterY = fTPY3*((double)fX+(double)fZ-1.)/(double)fZ + (double)fTPY4*(1.-(double)fX)/(double)fZ; //
									fY = (float)((1-(double)fZ)*(double)fTPY2 + (double)fZ*dInterY);

									fX2 = (float)(i+1)/(float)SHAD_MAP_SUBDIV;
									fZ2 = (float)j/(float)SHAD_MAP_SUBDIV;
									dInterY = fTPY3*((double)fX2+(double)fZ2-1.)/(double)fZ2 + (double)fTPY4*(1.-(double)fX2)/(double)fZ2; //
									fY2 = (float)((1-(double)fZ2)*(double)fTPY2 + (double)fZ2*dInterY);

									fX3 = (float)(i+1)/(float)SHAD_MAP_SUBDIV;
									fZ3 = (float)(j+1)/(float)SHAD_MAP_SUBDIV;
									dInterY = fTPY3*((double)fX3+(double)fZ3-1.)/(double)fZ3 + (double)fTPY4*(1.-(double)fX3)/(double)fZ3; //fZ3 always != 0
									fY3 = (float)((1-(double)fZ3)*(double)fTPY2 + (double)fZ3*dInterY);

									fX4 = (float)i/(float)SHAD_MAP_SUBDIV;
									fZ4 = (float)(j+1)/(float)SHAD_MAP_SUBDIV;
									dInterY = fTPY3*((double)fX4+(double)fZ4-1.)/(double)fZ4 + (double)fTPY4*(1.-(double)fX4)/(double)fZ4; //fZ4 always != 0
									fY4 = (float)((1-(double)fZ4)*(double)fTPY2 + (double)fZ4*dInterY);
								}
								byShadow = 255 - pabyShadowMap[x*SHAD_MAP_SUBDIV + i + (y*SHAD_MAP_SUBDIV + j)*(mapWidth-1)*SHAD_MAP_SUBDIV];
								glColor4f(byShadow/255.0f, byShadow/255.0f, byShadow/255.0f, 0.1f);
								glVertex3f(x+fX, fY, mapHeight - 1 - y-fZ);
								glVertex3f(x+fX2, fY2, mapHeight - 1 - y-fZ2);
								glVertex3f(x+fX3, fY3, mapHeight - 1 - y-fZ3);
								glVertex3f(x+fX4, fY4, mapHeight - 1 - y-fZ4);
							}
						}
					}
				}
			glEnd();
		glEndList();
//	glPopMatrix();
}

void CW3ZMapOpenGLView::OnViewRefresh()
{
	Update3DView();
	RedrawWindow();
}
