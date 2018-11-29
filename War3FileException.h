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
// War3FileException.h: interface for the CWar3FileException class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAR3FILEEXCEPTION_H__2C9C9EB2_6AD7_411B_BDBB_DF556BEA98B7__INCLUDED_)
#define AFX_WAR3FILEEXCEPTION_H__2C9C9EB2_6AD7_411B_BDBB_DF556BEA98B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWar3FileException
{

public:
	void ReportError();
	void SetMessage(const CString &strMessage);
	CString GetMessage() const;
	enum
	{
		none,
		unknown,
		invalidFileID,
		unsupportedVersion,
		unknownVersion,
		unhandledData,
		erroneousData,
		missingData
	};
	int m_cause;
	CWar3FileException();
	CWar3FileException(int war3FileExceptionCause, LPCSTR szMessage = NULL);
	virtual ~CWar3FileException();

protected:
	CString m_strMessage;
};

#endif // !defined(AFX_WAR3FILEEXCEPTION_H__2C9C9EB2_6AD7_411B_BDBB_DF556BEA98B7__INCLUDED_)
