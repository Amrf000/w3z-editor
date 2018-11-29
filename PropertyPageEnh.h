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
#if !defined(AFX_PROPERTYPAGEENH_H__A2BC2FBE_B9E1_4E76_8CDA_1DE38FF84AF3__INCLUDED_)
#define AFX_PROPERTYPAGEENH_H__A2BC2FBE_B9E1_4E76_8CDA_1DE38FF84AF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyPageEnh.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageEnh dialog

class CPropertyPageEnh : public CPropertyPage
{
//	DECLARE_DYNCREATE(CPropertyPageEnh)

// Construction
public:
	BOOL IsEnabled();
	void EnablePageItems(BOOL bEnable = TRUE);
	CPropertyPageEnh(UINT uIDTemplate);
	virtual ~CPropertyPageEnh();

// Dialog Data
	//{{AFX_DATA(CPropertyPageEnh)
		// NOTE - m_bIsPageInitedWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	enum { IDD = NULL };
	//}}AFX_DATA


// Overrides
	// m_bIsPageInitedWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropertyPageEnh)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_bIsPageInited;
	BOOL m_bIsEnabled;
	// Generated message map functions
	//{{AFX_MSG(CPropertyPageEnh)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYPAGEENH_H__A2BC2FBE_B9E1_4E76_8CDA_1DE38FF84AF3__INCLUDED_)
