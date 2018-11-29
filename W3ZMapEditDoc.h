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
// W3ZMapEditDoc.h : interface of the CW3ZMapEditDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_W3ZMAPEDITDOC_H__C8D2BF34_9D7F_45FC_BF49_9C76346ACE7F__INCLUDED_)
#define AFX_W3ZMAPEDITDOC_H__C8D2BF34_9D7F_45FC_BF49_9C76346ACE7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SFmpqapi.h"
#include "War3Map.h"
#include "War3File.h"
#include "EditMapPropSheet.h"
#include "War3TypesAndConstants.h"
//#include "StringEx.h"
#include "StringCountList.h"
#include "HashCountList.h"
#include "3DChildFrame.h"

class CListedUndoObj;
class CUndoObj;

class CW3ZMapEditDoc : public CDocument
{
protected: // create from serialization only
	CW3ZMapEditDoc();
	DECLARE_DYNCREATE(CW3ZMapEditDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CW3ZMapEditDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void SetTitle(LPCTSTR lpszTitle);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE);
	protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:
	void UnprotectW3Zv1Level3();
	void DecryptData(const CString &strPassword, byte *pbyData, int iDataSize);
	void CryptData(const CString &strPassword, byte *pbyData, int iDataSize);
	BOOL m_bSaved;
	BOOL IsImportedFile(const CString & strFilename);
	BOOL IsReadOnly();
	BOOL QuickBackup();
	CString GetProtectionName() const;
	void SetProtected();
	BOOL SetPublicPassword(const CString &strPassword);
	BOOL SetAuthorPassword(const CString &strPassword);
	BOOL IsPublicRegistered();
	BOOL IsAuthorRegistered();
	double GetAuthorNameRatio();
	void SetProtection(int iProtectType);
	int GetProtection();
	unsigned int GetAshedPublicPassword();
	unsigned int GetAshedAuthorPassword();
	CString GetAuthorName() const;
	void DecryptString(CString &strCryptedString);
	void CryptString(CString &strTheString, int iLength = -1);
	BOOL RegisterPublicPassword(const CString &strPassword);
	BOOL RegisterAuthorPassword(const CString &strPassword);
	BOOL IsProtected();
	void SerializePreviewTGA(BOOL IsStoring);
	void UpdateMapPropertiesDialog();
	void UpdateFromMapPropertiesDialog();
	void AddPathToUndoObj(int oldpath, int x, int y);
	void AddToUndoObj(CDoodad doodad, int flag);
	void AddToUndoObj(CTilePoint tilepoint,int x, int y);
	void DoUndo(int n=1);
	void OpenUndoObj();
	void CloseUndoObj();
	void EmptyTemporaryDirectory(CString &sPath);
	void SerializeWPM(BOOL IsStoring);
	void SerializeW3E(BOOL IsStoring);
	void SerializeW3I(BOOL IsStoring);
	void SerializeWTG(BOOL IsStoring);
	void SerializeWCT(BOOL IsStoring);
	void SerializeWTS(BOOL IsStoring);
	void SerializeJASS(BOOL IsStoring);
	void SerializeSHD(BOOL IsStoring);
	void SerializeMapB00(BOOL IsStoring);
	void SerializeMapBLP(BOOL IsStoring);
	void SerializeMapTGA(BOOL IsStoring);
	void SerializeMMP(BOOL IsStoring);
	void SerializePathTGA(BOOL IsStoring);
	void SerializeDOO(BOOL IsStoring);
	void SerializeUnitsDOO(BOOL IsStoring);
	void SerializeW3R(BOOL IsStoring);
	void SerializeW3C(BOOL IsStoring);
	void SerializeW3U(BOOL IsStoring);
	void SerializeW3S(BOOL IsStoring);
	void SerializeIMP(BOOL IsStoring);
	void SerializeListfile(BOOL IsStoring);
	CWar3Map m_War3Map;

	virtual ~CW3ZMapEditDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	C3DChildFrame* m_pt3DChildFrame;
	BOOL m_bPPassCorrupted;
	BOOL m_bAPassCorrupted;
	W3ZPROTECTDATA02 m_tW3ZProtectData02;
	W3ZPROTECTDATA01 m_tW3ZProtectData01;
	BOOL m_bAuthorMsg;
	BOOL m_bMakeBackup;
	BOOL m_bUnpRec;
	CString m_strPublicPassword;
	CString m_strAuthorPassword;
	int m_iProtectionType;
//	unsigned int m_uHashPublicPassword;
//	unsigned int m_uHashAuthorPassword;
	__int64 m_iFilesProtectStatus; // flags of protected files
	__int64 m_iFilesInitStatus; // flags of initialized files
	__int64 m_iFilesSaveStatus; // flags of saved files
	BOOL m_bAuthorRegistered; // says if the author password has been checked OK
	BOOL m_bPublicRegistered; // says if the public password has been checked OK
	BOOL m_bMapProtected; // says if the map is protected or not
	CEditMapPropSheet *m_pEditMapPropSheet;
	char* m_pUndoShadowsMap;
	char* m_pUndoPathMap;
	int m_iMapWidth;
	int m_iMapHeight;
	char* m_pUndoTPMap;
	int m_iMaxUndos;
	int m_iNbUndos;
	bool m_bOpenedUndoObj;
	CListedUndoObj* m_pUndoListHead;
	CListedUndoObj* m_pUndoListTail;
	CString m_strTempDir;
	BOOL m_bTmpFilesCreated;
	CFile m_fiListfile;
	CWar3File m_fiCHK;
	CWar3File m_fiW3E;
	CWar3File m_fiW3I;
	CWar3File m_fiWTG;
	CWar3File m_fiWCT;
	CFile m_fiWTS;
	CFile m_fiJASS;
	CFile m_fiSHD;
	CFile m_fiMapTGA;
	CFile m_fiPreviewTGA;
	CFile m_fiPathTGA;
	CWar3File m_fiMapBLP;
	CWar3File m_fiMapB00;
	CWar3File m_fiMMP;
	CWar3File m_fiWPM;
	CWar3File m_fiDOO;
	CWar3File m_fiUnitsDOO;
	CWar3File m_fiW3R;
	CWar3File m_fiW3C;
	CWar3File m_fiW3U;
	CWar3File m_fiW3S;
	CWar3File m_fiIMP;
	CWar3File m_fiUNP;

// Generated message map functions
protected:
	CHashCountList m_tPublicHashCountList;
	CHashCountList m_tAuthorHashCountList;
	CStringCountList m_tStrCountListAuthorName;
	CString m_strOriginalMPQFilename;
	CString m_strListfile;
	CFile m_fiEmptyFile;
	BOOL m_bEnableProtect;
	void InitProtectDataStructs();
	void AddAuthorName(const CString &strAuthorName);
	BOOL WriteEmbededFile(MPQHANDLE hMPQ, CString srcFilename, CString destFilename);
	BOOL ReadEmbededFile(MPQHANDLE hMPQ, CString filename, CFile &file);
	void CloseAndDeleteFile(CFile &file);
	BOOL OpenTemporaryFile(CFile &file);

	//{{AFX_MSG(CW3ZMapEditDoc)
	afx_msg void OnEditMapProperties();
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditMapResizeMap();
	afx_msg void OnEditMapMenuOptions();
	afx_msg void OnEditMapProtection();
	afx_msg void OnViewStatistics();
	afx_msg void OnFileQuickBackup();
	afx_msg void OnEditMinimap();
	afx_msg void OnEditPlayersProperties();
	afx_msg void OnBtnPathReset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	C3DChildFrame* Get3DChildFrame(void);
	void RunPlugin(CString strPluginName);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_W3ZMAPEDITDOC_H__C8D2BF34_9D7F_45FC_BF49_9C76346ACE7F__INCLUDED_)
