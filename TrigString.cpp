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
// TrigString.cpp: implementation of the CTrigString class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "War3TypesAndConstants.h"
#include "TrigString.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTrigString::CTrigString()
{
	m_sStrValue	= TSTR_UNKNOWN_VALUE;
	m_iTrigID	= TSTR_UNKNOWN_ID;
}

CTrigString::CTrigString(CString str, int ID)
{
	m_sStrValue	= str;
	m_iTrigID	= ID;
}

CTrigString::~CTrigString()
{

}


/*
GetTrigStringID
***************
 Returns the ID number of the trigger string.

In:
 none

Out:
 none

Return:
 int: ID value

*/
int CTrigString::GetTrigStringID()
{
	return m_iTrigID;
}

/*
SetTrigStringID
***************
 Sets the ID number of the trigger string to ID if ID is positive.

In:
 int ID: ID number

Out:
 none

Return:
 none

*/
void CTrigString::SetTrigStringID(int ID)
{
	if (ID >= 0)
	{m_iTrigID = ID;}
	else
	{TRACE("Warning: SetTrigID: invalid ID specified = %i\n", ID);}
}


/*
GetTrigStringName
*****************
 Returns a trigger string that can be used in a w3i or in jass file.

In:
 none

Out:
 none

Return:
 CString:
  -a string like "TRIGSTR_053" if ID is not negative (here =53) and the string is not null;
  -a null string if the string contains nothing;
  -the string content if it's not null and the ID is negative.

*/
CString CTrigString::GetTrigStringName()
{
	CString str;
	if (m_iTrigID >= 0)
	{
		str.Format("%s%03i", TSTR_TRIGSTR, m_iTrigID);
	}
	else
	{
		str = "";
	}
	return str;
}


/*
SetTrigStringValue
******************
Sets the trigger string string content.

In:
 CString str: new string value to put for this trigger string.

Out:
 none

Return:
 none

*/
void CTrigString::SetTrigStringValue(CString str)
{
	m_sStrValue = str;
}


/*
GetTrigStringValue
******************
Returns the trigger string content.

In:
 none

Out:
 none

Return:
 CString: the trigger string string value.

*/
CString CTrigString::GetTrigStringValue()
{
	return m_sStrValue;
}


/*
static GetTrigStringValue
*************************
This function intend to get the trigger string number from a string.
Returns a positive number which is the trigger ID number if the string passed as argument
is a trigger string, returns a negative value (-1) otherwise.

In:
 CString trigstr: a string

Out:
 none

Return:
 int: the trigger ID number (which can be 0) if str is a trigger string, -1 otherwise.

*/
int CTrigString::GetTrigStringID(CString trigstr)
{
	CString str;
	str = trigstr;
	int i = -1;

	if (str.GetLength() > 0)
	{
		TRY
		{
			// tirgger string command is case sensitive
			i = strncmp(TSTR_TRIGSTR, str.GetBuffer(TSTR_TRIGSTR_LEN), TSTR_TRIGSTR_LEN);
			str.ReleaseBuffer();
			if (!i)
			{
				str = str.Mid(TSTR_TRIGSTR_LEN);
				i = atoi(str.GetBuffer(str.GetLength()));
				str.ReleaseBuffer();
			}
			else
			{
				i = -1;
			}
		}
		CATCH(CMemoryException, e)
		{
			i = -1;
			TRACE("Error: GetTrigStringID: unable to create a buffer!\n");
		}
		END_CATCH
	}
	return i;
}

/*
UnsetTrigString
***************
Resets trigger string content and ID.

In:
 none

Out:
 none

Return:
 none

*/
void CTrigString::UnsetTrigString()
{
	m_sStrValue	= TSTR_UNKNOWN_VALUE;
	m_iTrigID	= TSTR_UNKNOWN_ID;
}


/*
IsTrigStringName
****************
Says if yes or no, a string is a trigger string name ("TRIGSTR_xxx").

In:
 CString str: the string to check

Out:
 none

Return:
 BOOL: TRUE if str is a trigger string name, FALSE otherwise

*/
BOOL CTrigString::IsTrigStringName(CString str)
{
	BOOL ret = FALSE;
	CString strtest = str;

	if (strtest.GetLength() > TSTR_TRIGSTR_LEN)
	{
		if (!strncmp(TSTR_TRIGSTR, strtest.GetBuffer(TSTR_TRIGSTR_LEN), TSTR_TRIGSTR_LEN))
		{
			ret = TRUE;
		}
		strtest.ReleaseBuffer();
	}
	return ret;
}
