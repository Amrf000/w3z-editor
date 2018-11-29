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
// War3FileException.cpp: implementation of the CWar3FileException class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "War3FileException.h"
#include "War3TypesAndConstants.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWar3FileException::CWar3FileException()
{
	m_cause = unknown;
	m_strMessage = "";
}

CWar3FileException::CWar3FileException(int war3FileExceptionCause, LPCSTR szMessage)
{
	m_cause = war3FileExceptionCause;
	if (NULL != szMessage)
	{
		m_strMessage = szMessage;
	}
	else
	{
		m_strMessage = "";
	}
}

CWar3FileException::~CWar3FileException()
{

}


CString CWar3FileException::GetMessage() const
{
	return m_strMessage;
}

void CWar3FileException::SetMessage(const CString &strMessage)
{
	m_strMessage = strMessage;
}

void CWar3FileException::ReportError()
{
	W3Z_WARNING(m_strMessage);
}
