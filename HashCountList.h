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
// HashCountList.h: interface for the CHashCountList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HASHCOUNTLIST_H__CFC766B3_0968_4CB6_9BEF_958EFB295740__INCLUDED_)
#define AFX_HASHCOUNTLIST_H__CFC766B3_0968_4CB6_9BEF_958EFB295740__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHashCountList  
{
	class CHashCount
	{
	public:
		int count;
		unsigned int uHash;
		CHashCount();
		virtual ~CHashCount();	
	};

	class CListedHashCount
	{
	public:
		CHashCount tHashCount;
		CListedHashCount *next;
		CListedHashCount();
		virtual ~CListedHashCount();	
	};

	typedef CListedHashCount* PCListedHashCount;

public:
	void ClearList();
	PCListedHashCount m_pHead;
	double GetBestCountRatio();
	unsigned int GetBestCountHash();
	void AddHash(unsigned int uHash);
	CHashCountList();
	virtual ~CHashCountList();
};

#endif // !defined(AFX_HASHCOUNTLIST_H__CFC766B3_0968_4CB6_9BEF_958EFB295740__INCLUDED_)
