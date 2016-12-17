// Archive.cpp: implementation of the CXArchive class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "File.h"
#include "Archive.h"
#include "Exception.h"

// Minimum buffer size
enum { nBufSizeMin = 128 };

// Verifies that the calling process has read or write access to the specified 
// range of memory. If the calling process has read or write access to all bytes 
// in the specified memory range, the return value is zero.
BOOL IsValidAddress(void* lp, UINT nBytes, BOOL bWrite = TRUE)
{
	if (bWrite)
		return IsBadWritePtr(lp, nBytes);
	else
		return IsBadReadPtr(lp, nBytes);
}

CXArchive::CXArchive(CXFile* pFile, UINT nMode, int nBufSize, void* lpBuf) :
					m_strFileName(pFile->GetFilePath())
{
	_ASSERT(pFile->m_hFile && ((pFile->m_hFile) != INVALID_HANDLE_VALUE));

	// Initialize members not dependent on allocated buffer
	m_nMode = nMode;
	m_pFile = pFile;

	// Initialize the buffer.  minimum size is 128
	m_lpBufStart = (BYTE*)lpBuf;
	m_bUserBuf = TRUE;
	m_bDirectBuffer = FALSE;
	m_bBlocking = m_pFile->GetBufferPtr(CXFile::bufferCheck) & CXFile::bufferBlocking;

	if (nBufSize < nBufSizeMin)
	{
		// Force use of private buffer of minimum size
		m_nBufSize = nBufSizeMin;
		m_lpBufStart = NULL;
	}
	else
		m_nBufSize = nBufSize;

	nBufSize = m_nBufSize;
	if (m_lpBufStart == NULL)
	{
		// Check for CFile providing buffering support
		m_bDirectBuffer = m_pFile->GetBufferPtr(CXFile::bufferCheck) & CXFile::bufferDirect;
		if (!m_bDirectBuffer)
		{
			// No support for direct buffering, allocate new buffer
			m_lpBufStart = new BYTE[m_nBufSize];
			m_bUserBuf = FALSE;
		}
		else
		{
			// CFile supports direct buffering!
			nBufSize = 0;   // will trigger initial FillBuffer
		}
	}

	if (!m_bDirectBuffer)
	{
		_ASSERT(m_lpBufStart != NULL);
		_ASSERT(!IsValidAddress(m_lpBufStart, nBufSize, IsStoring()));
	}

	m_lpBufMax = m_lpBufStart + nBufSize;
	m_lpBufCur = (IsLoading()) ? m_lpBufMax : m_lpBufStart;
}

CXArchive::~CXArchive()
{
	// Close makes m_pFile NULL. If it is not NULL, we must Close the CXArchive
	if (m_pFile != NULL && !(m_nMode & bNoFlushOnDelete))
		Close();

	Abort();    // abort completely shuts down the archive
}

void CXArchive::Abort()
{
	_ASSERT(m_bDirectBuffer || m_lpBufStart == NULL ||
		!IsValidAddress(m_lpBufStart, (UINT)(m_lpBufMax - m_lpBufStart), IsStoring()));
	_ASSERT(m_bDirectBuffer || m_lpBufCur == NULL ||
		!IsValidAddress(m_lpBufCur, (UINT)(m_lpBufMax - m_lpBufCur), IsStoring()));

	// Disconnect from the file
	m_pFile = NULL;

	if (!m_bUserBuf)
	{
		_ASSERT(!m_bDirectBuffer);
		delete[] m_lpBufStart;
		m_lpBufStart = NULL;
		m_lpBufCur = NULL;
	}
}

void CXArchive::Close()
{
	_ASSERT(m_pFile->m_hFile && ((m_pFile->m_hFile) != INVALID_HANDLE_VALUE));

	Flush();
	m_pFile = NULL;
}

UINT CXArchive::Read(void* lpBuf, UINT nMax)
{
	_ASSERT(m_pFile->m_hFile && ((m_pFile->m_hFile) != INVALID_HANDLE_VALUE));

	if (nMax == 0)
		return 0;

	_ASSERT(lpBuf != NULL);
	_ASSERT(!IsValidAddress(lpBuf, nMax));
	_ASSERT(m_bDirectBuffer || m_lpBufStart != NULL);
	_ASSERT(m_bDirectBuffer || m_lpBufCur != NULL);
	_ASSERT(m_lpBufStart == NULL ||
		!IsValidAddress(m_lpBufStart, (UINT)(m_lpBufMax - m_lpBufStart), FALSE));
	_ASSERT(m_lpBufCur == NULL ||
		!IsValidAddress(m_lpBufCur, (UINT)(m_lpBufMax - m_lpBufCur), FALSE));
	_ASSERT(IsLoading());

	// Try to fill from buffer first
	UINT nMaxTemp = nMax;
	UINT nTemp = min(nMaxTemp, (UINT)(m_lpBufMax - m_lpBufCur));
	memcpy(lpBuf, m_lpBufCur, nTemp);
	m_lpBufCur += nTemp;
	lpBuf = (BYTE*)lpBuf + nTemp;
	nMaxTemp -= nTemp;

	if (nMaxTemp != 0)
	{
		_ASSERT(m_lpBufCur == m_lpBufMax);

		// Read rest in buffer size chunks
		nTemp = nMaxTemp - (nMaxTemp % m_nBufSize);
		UINT nRead = 0;

		UINT nLeft = nTemp;
		UINT nBytes;
		do
		{
			nBytes = m_pFile->Read(lpBuf, nLeft);
			lpBuf = (BYTE*)lpBuf + nBytes;
			nRead += nBytes;
			nLeft -= nBytes;
		}
		while ((nBytes > 0) && (nLeft > 0));

		nMaxTemp -= nRead;

		if (nMaxTemp > 0)
		{
			// Read last chunk into buffer then copy
			if (nRead == nTemp)
			{
				_ASSERT(m_lpBufCur == m_lpBufMax);
				_ASSERT(nMaxTemp < (UINT)m_nBufSize);

				// Fill buffer (similar to CXArchive::FillBuffer, but no exception)
				if (!m_bDirectBuffer)
				{
					UINT nLeft;
					UINT nBytes;

					if (!m_bBlocking)
						nLeft = max(nMaxTemp, (UINT)m_nBufSize);
					else
						nLeft = nMaxTemp;
					BYTE* lpTemp = m_lpBufStart;
					nRead = 0;
					do
					{
						nBytes = m_pFile->Read(lpTemp, nLeft);
						lpTemp = lpTemp + nBytes;
						nRead += nBytes;
						nLeft -= nBytes;
					}
					while ((nBytes > 0) && (nLeft > 0) && nRead < nMaxTemp);

					m_lpBufCur = m_lpBufStart;
					m_lpBufMax = m_lpBufStart + nRead;
				}
				else
				{
					nRead = m_pFile->GetBufferPtr(CXFile::bufferRead, m_nBufSize,
						(void**)&m_lpBufStart, (void**)&m_lpBufMax);
					_ASSERT(nRead == size_t( m_lpBufMax-m_lpBufStart ));
					m_lpBufCur = m_lpBufStart;
				}

				// Use first part for rest of read
				nTemp = min(nMaxTemp, (UINT)(m_lpBufMax - m_lpBufCur));
				memcpy(lpBuf, m_lpBufCur, nTemp);
				m_lpBufCur += nTemp;
				nMaxTemp -= nTemp;
			}
		}
	}

	return nMax - nMaxTemp;
}

void CXArchive::Write(const void* lpBuf, UINT nMax)
{
	_ASSERT(m_pFile->m_hFile && ((m_pFile->m_hFile) != INVALID_HANDLE_VALUE));

	if (nMax == 0)
		return;

	_ASSERT(lpBuf != NULL);
	_ASSERT(!IsValidAddress(const_cast<void*>(lpBuf), nMax, FALSE));  // read-only access needed
	_ASSERT(m_bDirectBuffer || m_lpBufStart != NULL);
	_ASSERT(m_bDirectBuffer || m_lpBufCur != NULL);
	_ASSERT(m_lpBufStart == NULL ||
		!IsValidAddress(m_lpBufStart, (UINT)(m_lpBufMax - m_lpBufStart)));
	_ASSERT(m_lpBufCur == NULL ||
		!IsValidAddress(m_lpBufCur, (UINT)(m_lpBufMax - m_lpBufCur)));
	_ASSERT(IsStoring());

	// Copy to buffer if possible
	UINT nTemp = min(nMax, (UINT)(m_lpBufMax - m_lpBufCur));
	memcpy(m_lpBufCur, lpBuf, nTemp);
	m_lpBufCur += nTemp;
	lpBuf = (BYTE*)lpBuf + nTemp;
	nMax -= nTemp;

	if (nMax > 0)
	{
		Flush();    // flush the full buffer

		// Write rest of buffer size chunks
		nTemp = nMax - (nMax % m_nBufSize);
		m_pFile->Write(lpBuf, nTemp);
		lpBuf = (BYTE*)lpBuf + nTemp;
		nMax -= nTemp;

		if (m_bDirectBuffer)
		{
			// Sync up direct mode buffer to new file position
			m_pFile->GetBufferPtr(CXFile::bufferWrite, m_nBufSize,
								(void**)&m_lpBufStart, (void**)&m_lpBufMax);
			_ASSERT((UINT)m_nBufSize == (UINT)(m_lpBufMax - m_lpBufStart));
			m_lpBufCur = m_lpBufStart;
		}

		// Copy remaining to active buffer
		_ASSERT(nMax < (UINT)m_nBufSize);
		_ASSERT(m_lpBufCur == m_lpBufStart);
		memcpy(m_lpBufCur, lpBuf, nMax);
		m_lpBufCur += nMax;
	}
}

void CXArchive::Flush()
{
	_ASSERT(m_pFile->m_hFile && ((m_pFile->m_hFile) != INVALID_HANDLE_VALUE));
	_ASSERT(m_bDirectBuffer || m_lpBufStart != NULL);
	_ASSERT(m_bDirectBuffer || m_lpBufCur != NULL);
	_ASSERT(m_lpBufStart == NULL ||
		!IsValidAddress(m_lpBufStart, (UINT)(m_lpBufMax - m_lpBufStart), IsStoring()));
	_ASSERT(m_lpBufCur == NULL ||
		!IsValidAddress(m_lpBufCur, (UINT)(m_lpBufMax - m_lpBufCur), IsStoring()));

	if (IsLoading())
	{
		// Unget the characters in the buffer, seek back unused amount
		if (m_lpBufMax != m_lpBufCur)
			m_pFile->Seek(-(int(m_lpBufMax - m_lpBufCur)), CXFile::current);
		m_lpBufCur = m_lpBufMax;    // empty
	}
	else
	{
		if (!m_bDirectBuffer)
		{
			// Write out the current buffer to file
			if (m_lpBufCur != m_lpBufStart)
				m_pFile->Write(m_lpBufStart, ULONG(m_lpBufCur - m_lpBufStart));
		}
		else
		{
			// Commit current buffer
			if (m_lpBufCur != m_lpBufStart)
				m_pFile->GetBufferPtr(CXFile::bufferCommit, ULONG(m_lpBufCur - m_lpBufStart));
			// Get next buffer
			m_pFile->GetBufferPtr(CXFile::bufferWrite, m_nBufSize,
								(void**)&m_lpBufStart, (void**)&m_lpBufMax);
			_ASSERT((UINT)m_nBufSize == (UINT)(m_lpBufMax - m_lpBufStart));
		}
		m_lpBufCur = m_lpBufStart;
	}
}

void CXArchive::FillBuffer(UINT nBytesNeeded)
{
	_ASSERT(m_pFile->m_hFile && ((m_pFile->m_hFile) != INVALID_HANDLE_VALUE));
	_ASSERT(IsLoading());
	_ASSERT(m_bDirectBuffer || m_lpBufStart != NULL);
	_ASSERT(m_bDirectBuffer || m_lpBufCur != NULL);
	_ASSERT(nBytesNeeded > 0);
	_ASSERT(nBytesNeeded <= (UINT)m_nBufSize);
	_ASSERT(m_lpBufStart == NULL ||
		!IsValidAddress(m_lpBufStart, (UINT)(m_lpBufMax - m_lpBufStart), FALSE));
	_ASSERT(m_lpBufCur == NULL ||
		!IsValidAddress(m_lpBufCur, (UINT)(m_lpBufMax - m_lpBufCur), FALSE));

	UINT nUnused = (UINT)(m_lpBufMax - m_lpBufCur);
	ULONG nTotalNeeded = ((ULONG)nBytesNeeded) + nUnused;

	// Fill up the current buffer from file
	if (!m_bDirectBuffer)
	{
		_ASSERT(m_lpBufCur != NULL);
		_ASSERT(m_lpBufStart != NULL);
		_ASSERT(m_lpBufMax != NULL);

		if (m_lpBufCur > m_lpBufStart)
		{
			// Copy unused
			if ((int)nUnused > 0)
			{
				memmove(m_lpBufStart, m_lpBufCur, nUnused);
				m_lpBufCur = m_lpBufStart;
				m_lpBufMax = m_lpBufStart + nUnused;
			}

			// Read to satisfy nBytesNeeded or nLeft if possible
			UINT nRead = nUnused;
			UINT nLeft;
			UINT nBytes;

			// Only read what we have to, to avoid blocking waiting on data 
			// we don't need
			if (m_bBlocking)  
				nLeft = nBytesNeeded - nUnused;
			else
				nLeft = m_nBufSize - nUnused;
			BYTE* lpTemp = m_lpBufStart + nUnused;
			do
			{
				nBytes = m_pFile->Read(lpTemp, nLeft);
				lpTemp = lpTemp + nBytes;
				nRead += nBytes;
				nLeft -= nBytes;
			}
			while (nBytes > 0 && nLeft > 0 && nRead < nBytesNeeded);

			m_lpBufCur = m_lpBufStart;
			m_lpBufMax = m_lpBufStart + nRead;
		}
	}
	else
	{
		// Seek to unused portion and get the buffer starting there
		if (nUnused != 0)
			m_pFile->Seek(-(LONG)nUnused, CXFile::current);
		UINT nActual = m_pFile->GetBufferPtr(CXFile::bufferRead, m_nBufSize,
							(void**)&m_lpBufStart, (void**)&m_lpBufMax);
		_ASSERT(nActual == (UINT)(m_lpBufMax - m_lpBufStart));
		m_lpBufCur = m_lpBufStart;
	}

	// Not enough data to fill request?
	if ((ULONG)(m_lpBufMax - m_lpBufCur) < nTotalNeeded)
	{
		CXArchiveException ex(CXArchiveException::endOfFile, m_pFile->GetFileName());
		throw ex;
	}
}

void CXArchive::WriteString(LPCTSTR lpsz)
{
	_ASSERT(!IsBadStringPtr(lpsz, _tcslen(lpsz)));
	Write(lpsz, lstrlen(lpsz) * sizeof(TCHAR));
}

LPTSTR CXArchive::ReadString(LPTSTR lpsz, UINT nMax)
{
	// If nMax is negative (such a large number doesn't make sense given today's
	// 2gb address space), then assume it to mean "keep the newline".
	int nStop = (int)nMax < 0 ? -(int)nMax : (int)nMax;
	_ASSERT(!IsValidAddress(lpsz, (nStop+1) * sizeof(TCHAR), FALSE));

	_TUCHAR ch;
	int nRead = 0;

	try
	{
		while (nRead < nStop)
		{
			*this >> ch;

			// Stop and end-of-line (trailing '\n' is ignored)
			if (ch == '\n' || ch == '\r')
			{
				if (ch == '\r')
					*this >> ch;
				// Store the newline when called with negative nMax
				if ((int)nMax != nStop)
					lpsz[nRead++] = ch;
				break;
			}
			lpsz[nRead++] = ch;
		}
	}
	catch(CXArchiveException& Ex)
	{
		if (Ex.GetErrorNumber() == CXArchiveException::endOfFile)
		{
			if (nRead == 0)
				return NULL;
		}
		else
		{
			throw Ex;
		}
	}

	lpsz[nRead] = '\0';
	return lpsz;
}

BOOL CXArchive::ReadString(CString& rString)
{
	rString = _T("");    // empty wstring without deallocating
	const int nMaxSize = 128;
	LPTSTR lpsz = rString.GetBuffer(nMaxSize);
	LPTSTR lpszResult;
	int nLen;
	for (;;)
	{
		lpszResult = ReadString(lpsz, (UINT)-nMaxSize); // store the newline
		rString.ReleaseBuffer();

		// If wstring is read completely or EOF
		if (lpszResult == NULL || (nLen = (int)lstrlen(lpsz)) < nMaxSize || lpsz[nLen-1] == '\n')
		{
			break;
		}

		nLen = rString.GetLength();
		lpsz = rString.GetBuffer(nMaxSize + nLen) + nLen;
	}

	// Remove '\n' from end of wstring if present
	lpsz = rString.GetBuffer(0);
	nLen = rString.GetLength();
	if (nLen != 0 && lpsz[nLen-1] == '\n')
		rString.GetBufferSetLength(nLen-1);

	return lpszResult != NULL;
}

BOOL CXArchive::IsLoading() const
{ 
	return (m_nMode & CXArchive::load) != 0; 
}

BOOL CXArchive::IsStoring() const
{ 
	return (m_nMode & CXArchive::load) == 0; 
}

CXFile* CXArchive::GetFile() const
{ 
	return m_pFile; 
}

CXArchive& CXArchive::operator<<(int i)
{ 
	return CXArchive::operator<<((LONG)i); 
}

CXArchive& CXArchive::operator<<(unsigned u)
{ 
	return CXArchive::operator<<((LONG)u); 
}

CXArchive& CXArchive::operator<<(short w)
{ 
	return CXArchive::operator<<((WORD)w); 
}

CXArchive& CXArchive::operator<<(char ch)
{ 
	return CXArchive::operator<<((BYTE)ch); 
}

#ifdef _NATIVE_WCHAR_T_DEFINED
CXArchive& CXArchive::operator<<(wchar_t ch)
{
	return CXArchive::operator<<((WORD)ch); 
}
#endif

CXArchive& CXArchive::operator<<(bool b)
{ 
	return CXArchive::operator<<((BYTE)(b ? 1 : 0)); 
}

CXArchive& CXArchive::operator<<(BYTE by)
{ 
	if (m_lpBufCur + sizeof(BYTE) > m_lpBufMax) 
		Flush();

	*(UNALIGNED BYTE*)m_lpBufCur = by; 
	m_lpBufCur += sizeof(BYTE); 
	
	return *this; 
}

CXArchive& CXArchive::operator<<(LONGLONG dwdw)
{ 
	if (m_lpBufCur + sizeof(LONGLONG) > m_lpBufMax) 
		Flush();

	*(UNALIGNED LONGLONG*)m_lpBufCur = dwdw; 
	m_lpBufCur += sizeof(LONGLONG); 
	
	return *this; 
}

CXArchive& CXArchive::operator<<(ULONGLONG dwdw)
{ 
	if (m_lpBufCur + sizeof(ULONGLONG) > m_lpBufMax) 
		Flush();

	*(UNALIGNED ULONGLONG*)m_lpBufCur = dwdw; 
	m_lpBufCur += sizeof(ULONGLONG); 
	
	return *this; 
}

CXArchive& CXArchive::operator<<(WORD w)
{ 
	if (m_lpBufCur + sizeof(WORD) > m_lpBufMax) 
		Flush();

	*(UNALIGNED WORD*)m_lpBufCur = w; 
	m_lpBufCur += sizeof(WORD); 
	
	return *this; 
}

CXArchive& CXArchive::operator<<(LONG l)
{ 
	if (m_lpBufCur + sizeof(LONG) > m_lpBufMax) 
		Flush();
		
	*(UNALIGNED LONG*)m_lpBufCur = l; 
	m_lpBufCur += sizeof(LONG);

	return *this; 
}

CXArchive& CXArchive::operator<<(DWORD dw)
{ 
	if (m_lpBufCur + sizeof(DWORD) > m_lpBufMax) 
		Flush();

	*(UNALIGNED DWORD*)m_lpBufCur = dw; 
	m_lpBufCur += sizeof(DWORD); 
	
	return *this; 
}

CXArchive& CXArchive::operator<<(float f)
{ 
	if (m_lpBufCur + sizeof(float) > m_lpBufMax) 
		Flush();

	*(UNALIGNED float*)m_lpBufCur = f; 
	m_lpBufCur += sizeof(float); 
	
	return *this;
}

CXArchive& CXArchive::operator<<(double d)
{ 
	if (m_lpBufCur + sizeof(double) > m_lpBufMax) 
		Flush();

	*(UNALIGNED double*)m_lpBufCur = d; 
	m_lpBufCur += sizeof(double); 
	
	return *this; 
}

CXArchive& CXArchive::operator>>(int& i)
{ 
	return CXArchive::operator>>((LONG&)i); 
}

CXArchive& CXArchive::operator>>(unsigned& u)
{ 
	return CXArchive::operator>>((LONG&)u); 
}

CXArchive& CXArchive::operator>>(short& w)
{ 
	return CXArchive::operator>>((WORD&)w); 
}

CXArchive& CXArchive::operator>>(char& ch)
{ 
	return CXArchive::operator>>((BYTE&)ch); 
}

#ifdef _NATIVE_WCHAR_T_DEFINED
CXArchive& CXArchive::operator>>(wchar_t& ch)
{ 
	return CXArchive::operator>>((WORD&)ch); 
}
#endif

CXArchive& CXArchive::operator>>(bool& b)
{ 
	BYTE by; 
	CXArchive& ar = CXArchive::operator>>(by); 
	
	b = (by ? true : false); 
	
	return ar; 
}

CXArchive& CXArchive::operator>>(BYTE& by)
{ 
	if (m_lpBufCur + sizeof(BYTE) > m_lpBufMax)
		FillBuffer((UINT)(sizeof(BYTE) - (m_lpBufMax - m_lpBufCur)));
		
	by = *(UNALIGNED BYTE*)m_lpBufCur; 
	m_lpBufCur += sizeof(BYTE); 
	
	return *this; 
}

CXArchive& CXArchive::operator>>(LONGLONG& dwdw)
{ 
	if (m_lpBufCur + sizeof(LONGLONG) > m_lpBufMax)
		FillBuffer(sizeof(LONGLONG) - (UINT)(m_lpBufMax - m_lpBufCur));
	
	dwdw = *(UNALIGNED LONGLONG*)m_lpBufCur; 
	m_lpBufCur += sizeof(LONGLONG); 
	
	return *this; 
}

CXArchive& CXArchive::operator>>(ULONGLONG& dwdw)
{ 
	if (m_lpBufCur + sizeof(ULONGLONG) > m_lpBufMax)
		FillBuffer(sizeof(ULONGLONG) - (UINT)(m_lpBufMax - m_lpBufCur));
		
	dwdw = *(UNALIGNED ULONGLONG*)m_lpBufCur; 
	m_lpBufCur += sizeof(ULONGLONG); 
	
	return *this; 
}

CXArchive& CXArchive::operator>>(WORD& w)
{ 
	if (m_lpBufCur + sizeof(WORD) > m_lpBufMax)
		FillBuffer((UINT)(sizeof(WORD) - (m_lpBufMax - m_lpBufCur)));

	w = *(UNALIGNED WORD*)m_lpBufCur; 
	m_lpBufCur += sizeof(WORD); 
	
	return *this; 
}

CXArchive& CXArchive::operator>>(DWORD& dw)
{ 
	if (m_lpBufCur + sizeof(DWORD) > m_lpBufMax)
		FillBuffer((UINT)(sizeof(DWORD) - (m_lpBufMax - m_lpBufCur)));

	dw = *(UNALIGNED DWORD*)m_lpBufCur; 
	m_lpBufCur += sizeof(DWORD); 
	
	return *this; 
}

CXArchive& CXArchive::operator>>(float& f)
{ 
	if (m_lpBufCur + sizeof(float) > m_lpBufMax)
		FillBuffer((UINT)(sizeof(float) - (m_lpBufMax - m_lpBufCur)));
	
	f = *(UNALIGNED float*)m_lpBufCur; 
	m_lpBufCur += sizeof(float); 
	
	return *this; 
}

CXArchive& CXArchive::operator>>(double& d)
{ 
	if (m_lpBufCur + sizeof(double) > m_lpBufMax)
		FillBuffer((UINT)(sizeof(double) - (m_lpBufMax - m_lpBufCur)));

	d = *(UNALIGNED double*)m_lpBufCur; 
	m_lpBufCur += sizeof(double); 
	
	return *this; 
}

 CXArchive& CXArchive::operator>>(LONG& l)
{ 
	if (m_lpBufCur + sizeof(LONG) > m_lpBufMax)
		FillBuffer((UINT)(sizeof(LONG) - (m_lpBufMax - m_lpBufCur)));
		
	l = *(UNALIGNED LONG*)m_lpBufCur; 
	m_lpBufCur += sizeof(LONG); 
	
	return *this; 
}

/////////////////////////////////////////////////////////////////////////////
