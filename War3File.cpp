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
// War3File.cpp: implementation of the CWar3File class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "War3TypesAndConstants.h"
#include "War3File.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWar3File::CWar3File() :CFile()
{
}

CWar3File::~CWar3File()
{
}


/*
ReadInt
*******
Reads an integer from the file into i.

In:
 none

Out:
 CWar3Int &i: integer read

Return:
 BOOL: TRUE if successfull, FALSE otherwise

*/
BOOL CWar3File::ReadInt(CWar3Int &i)
{
	BOOL ret = FALSE;
	TRY
	{
		Read(&i, 4);
		ret = TRUE;
	}
	CATCH( CFileException, e )
	{
		ret = FALSE;
	}
	END_CATCH
	return ret;
}


/*
ReadString
**********
Reads a string from the file into str.

In:
 none

Out:
 CWar3String &szStr: string read

Return:
 BOOL: TRUE if successfull, FALSE otherwise

*/
BOOL CWar3File::ReadString(CWar3String &szStr)
{
	BOOL ret = FALSE;
	int i = 0, size = 0, pos = 0;
	char* pBuf;

	TRY
	{
		pos = (int) GetPosition();
		size = (int)(GetLength() - pos);
		if (size > 0)
		{
			pBuf = new char[size];
			if (pBuf)
			{
				Read(pBuf, size);
				i = 0;
				szStr = "";
				while ((i < size) && pBuf[i])
				{
					szStr += pBuf[i++];
				}
				delete []pBuf;
				pos = pos + szStr.GetLength() + 1;
				Seek(pos, CFile::begin);
				ret = TRUE;
			}
			else
			{
				ret = FALSE;
			}
		}
	}
	CATCH( CFileException, e )
	{
		ret = FALSE;
	}
	END_CATCH

	return ret;
}


/*
ReadFloat
*********
Reads a float from the file into f.

In:
 none

Out:
 CWar3Float &f: float read

Return:
 BOOL: TRUE if successfull, FALSE otherwise

*/
BOOL CWar3File::ReadFloat(CWar3Float &f)
{
	BOOL ret = FALSE;
	TRY
	{
		Read(&f, 4);
		ret = TRUE;
	}
	CATCH( CFileException, e )
	{
		ret = FALSE;
	}
	END_CATCH
	return ret;
}


/*
ReadChar
********
Reads a char from the file into c.

In:
 none

Out:
 CWar3Char &c: char read

Return:
 BOOL: TRUE if successfull, FALSE otherwise

*/
BOOL CWar3File::ReadChar(CWar3Char &c)
{
	BOOL ret = FALSE;
	TRY
	{
		Read(&c, 1);
		ret = TRUE;
	}
	CATCH( CFileException, e )
	{
		ret = FALSE;
	}
	END_CATCH
	return ret;
}


/*
ReadFlags
*********
Reads an integer from the file into flags.

In:
 none

Out:
 CWar3Flags &flags: flags read

Return:
 BOOL: TRUE if successfull, FALSE otherwise

*/
BOOL CWar3File::ReadFlags(CWar3Flags &flags)
{
	BOOL ret = FALSE;
	TRY
	{
		Read(&flags, 4);
		ret = TRUE;
	}
	CATCH( CFileException, e )
	{
		ret = FALSE;
	}
	END_CATCH
	return ret;
}


/*
ReadStrID
*********
Reads a String ID from the file into strID.

In:
 CWar3StrID strID: a pointer to the destination char[4] buffer

Out:
 *(CWar3StrID strID): string ID read

Return:
 BOOL: TRUE if successfull, FALSE otherwise

*/
BOOL CWar3File::ReadStrID(CWar3StrID &strID)
{
	BOOL ret = FALSE;
	char acBuf[4];

	TRY
	{
		Read(acBuf, 4);
		strID = acBuf;
		ret = TRUE;
	}
	CATCH( CFileException, e )
	{
		ret = FALSE;
	}
	END_CATCH
	return ret;
}


/*
WriteChar
*********
Write the char c to the file.

In:
 CWar3Char c: char to be Written

Out:
 nothing

Return:
 BOOL: TRUE if successfull, FALSE otherwise

*/
BOOL CWar3File::WriteChar(CWar3Char c)
{
	BOOL ret = FALSE;
	TRY
	{
		Write(&c, 1);
		ret = TRUE;
	}
	CATCH( CFileException, e )
	{
		ret = FALSE;
	}
	END_CATCH
	return ret;
}


/*
WriteStrID
**********
Write the string ID strID to the file.

In:
 CWar3StrID strID: string ID to be written

Out:
 nothing

Return:
 BOOL: TRUE if successfull, FALSE otherwise

*/
BOOL CWar3File::WriteStrID(CWar3StrID strID)
{
	BOOL ret = FALSE;
	TRY
	{
		Write(strID.GetStrID(), 4);
		ret = TRUE;
	}
	CATCH( CFileException, e )
	{
		ret = FALSE;
	}
	END_CATCH
	return ret;
}


/*
WriteInt
********
Write the int i to the file.

In:
 CWar3Int i: int to write

Out:
 nothing

Return:
 BOOL: TRUE if successfull, FALSE otherwise

*/
BOOL CWar3File::WriteInt(CWar3Int i)
{
	BOOL ret = FALSE;
	TRY
	{
		Write(&i, 4);
		ret = TRUE;
	}
	CATCH( CFileException, e )
	{
		ret = FALSE;
	}
	END_CATCH
	return ret;
}


/*
WriteFlags
**********
Write the flags "flags" to the file.

In:
 CWar3Flags flags: flags to be written

Out:
 nothing

Return:
 BOOL: TRUE if successfull, FALSE otherwise

*/
BOOL CWar3File::WriteFlags(CWar3Flags flags)
{
	BOOL ret = FALSE;
	TRY
	{
		Write(&flags, 4);
		ret = TRUE;
	}
	CATCH( CFileException, e )
	{
		ret = FALSE;
	}
	END_CATCH
	return ret;
}


/*
WriteFloat
**********
Write the float f to the file.

In:
 CWar3Float f: the float to write

Out:
 nothing

Return:
 BOOL: TRUE if successfull, FALSE otherwise

*/
BOOL CWar3File::WriteFloat(CWar3Float f)
{
	BOOL ret = FALSE;
	TRY
	{
		Write(&f, 4);
		ret = TRUE;
	}
	CATCH( CFileException, e )
	{
		ret = FALSE;
	}
	END_CATCH
	return ret;
}


/*
WriteString
***********
Write the string szStr to the file.

In:
 CWar3String szStr: string to write

Out:
 nothing

Return:
 BOOL: TRUE if successfull, FALSE otherwise

*/
BOOL CWar3File::WriteString(CWar3String szStr)
{
	BOOL ret = FALSE;
	CWar3String szLocalStr;
	szLocalStr = szStr;
	int iStrSize;
	char cNull ='\0';

	TRY
	{
		// gets string size
		iStrSize = szLocalStr.GetLength();
		// write the string content
		Write(szLocalStr.GetBuffer(iStrSize), iStrSize);
		szLocalStr.ReleaseBuffer();
		// write the ending null char
		Write(&cNull, 1);
		ret = TRUE;
	}
	CATCH( CFileException, e )
	{
		ret = FALSE;
	}
	END_CATCH
	return ret;
}
