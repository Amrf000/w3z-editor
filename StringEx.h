/*
*******************
* CStringEx Class *
*******************

********************************************************************************

  This is an extension of the original MFC CString class made by Zafir Anjum.
  The sources are available from CodeGuru:
    http://www.codeguru.com/string/cstring_ext.shtml

********************************************************************************
This is a sample table of function use:
---------------------------------------

String				Function								Result
--------------------------------------------------------------------------------
xyz					Insert(1, ‘a’)							xayz
xyz					Insert(1, "ab")							xabyz
xyz					Insert(4, "ab")							xyz ab
Abcde				Delete(2,2)								Abe
Abcde				Replace(2,2,"xyz")						Abxyze
Abc					Find(‘b’)								1
Abcabc				Find(‘b’, 2)							4
AbcAbc				FindNoCase("aBC", 1)					3
AbcAbc				FindReplace("Ab","Xy")					XycXyc
AbcAbc				FindReplace("Ab","Xy", FALSE)			XycAbc
AbcAbc				FindReplaceNoCase("ab","Xy")			XycXyc
AbcAbc				FindReplaceNoCase("ab","Xy", FALSE)		XycAbc
AbcAbc				ReverseFind("bc")						4
AbcAbc				ReverseFindNoCase("abc")				3
AbcAbc				ReverseFindNoCase("abc", 2)				0
AbcAbc				GetField( ‘:’, 0 )						AbcAbc
AbcAbc				GetField( ‘:’, 1 )						(Blank string)
Abc:Def				GetField( ‘:’, 0 )						Abc
Abc:Def				GetField( ‘:’, 1 )						Def
Abc:Def				GetField( ‘:’, 2 )						(Blank string)
Name: Zafir			GetField("Name: ", 1)					Zafir
(415) 555-7777		GetDelimitedField("(", ")", 0 )			415

********************************************************************************
*/

//////////////////////////////////////////////////////////////////////
// StringEx.h
//

#ifndef __STRINGEX_H_
#define __STRINGEX_H_

class CStringEx : public CString
{
public:
	CStringEx() : CString( ){};
	CStringEx( const CString& stringSrc) : CString( stringSrc ){};
	CStringEx( const CStringEx& stringSrc) : CString( stringSrc ){};
	CStringEx( TCHAR ch, int nRepeat = 1 ) : CString( ch, nRepeat ){};
	CStringEx( LPCTSTR lpch, int nLength ) : CString( lpch, nLength ){};
	CStringEx( const unsigned char* psz ) : CString( psz ){};
	CStringEx( LPCWSTR lpsz ) : CString( lpsz ){};
	CStringEx( LPCSTR lpsz ) : CString( lpsz ){};

	CStringEx& Insert(int pos, LPCTSTR s);
	CStringEx& Insert(int pos, TCHAR c);

	CStringEx& Delete(int pos, int len);
	CStringEx& Replace(int pos, int len, LPCTSTR s);

	int Find( TCHAR ch, int startpos = 0 ) const;
	int Find( LPCTSTR lpszSub, int startpos = 0 ) const;
	int FindNoCase( TCHAR ch, int startpos = 0 ) const;
	int FindNoCase( LPCTSTR lpszSub, int startpos = 0 ) const;

	int FindReplace( LPCTSTR lpszSub, LPCTSTR lpszReplaceWith, BOOL bGlobal = TRUE );
	int FindReplaceNoCase( LPCTSTR lpszSub, LPCTSTR lpszReplaceWith, 
				BOOL bGlobal = TRUE );

	int ReverseFind( TCHAR ch ) const{ return CString::ReverseFind(ch);};
	int ReverseFind( LPCTSTR lpszSub, int startpos = -1 ) const;
	int ReverseFindNoCase( TCHAR ch, int startpos = -1  ) const;
	int ReverseFindNoCase( LPCTSTR lpszSub, int startpos = -1 ) const;

	CStringEx GetField( LPCTSTR delim, int fieldnum);
	CStringEx GetField( TCHAR delim, int fieldnum);
	int GetFieldCount( LPCTSTR delim );
	int GetFieldCount( TCHAR delim );

	CStringEx GetDelimitedField( LPCTSTR delimStart, LPCTSTR delimEnd, 
				int fieldnum = 0);
};


#endif
/////////////////////////////////////////////////////////////////////
