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
#if !defined(AFX_DOODADTOOLPROPDIALOG_H__A39D6001_01F7_4B31_9E01_BE48C8B56B2E__INCLUDED_)
#define AFX_DOODADTOOLPROPDIALOG_H__A39D6001_01F7_4B31_9E01_BE48C8B56B2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DoodadToolPropDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDoodadToolPropDialog dialog

class CDoodadToolPropDialog : public CDialog
{
// Construction
public:
	CDoodadToolPropDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDoodadToolPropDialog)
	enum { IDD = IDD_DLG_DOODADTOOL_PROP };
	int		m_iMaxLife;
	float	m_fMaxScaleX;
	float	m_fMaxScaleY;
	float	m_fMaxScaleZ;
	int		m_iMinLife;
	float	m_fMinScaleX;
	float	m_fMinScaleY;
	float	m_fMinScaleZ;
	float	m_fSpacing;
	CString	m_sTypeList;
	int		m_iVariation;
	float		m_iSize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDoodadToolPropDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDoodadToolPropDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOODADTOOLPROPDIALOG_H__A39D6001_01F7_4B31_9E01_BE48C8B56B2E__INCLUDED_)
