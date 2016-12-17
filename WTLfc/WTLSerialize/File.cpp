// File.cpp: implementation of the CXFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "File.h"
			
//-----------------------------------------------------------------------------
// Construction/Destruction
//-----------------------------------------------------------------------------
CXFile::CXFile()
{
	m_hFile = NULL;
}

CXFile::CXFile(HANDLE hFile)
{
	m_hFile = hFile;
}

CXFile::~CXFile()
{
	if (m_hFile != NULL)
		Close();
}

//-----------------------------------------------------------------------------
// Constructs a duplicate CXFile object for a given file.
//-----------------------------------------------------------------------------
CXFile* CXFile::Duplicate() const
{
	_ASSERT(m_hFile && ((m_hFile) != INVALID_HANDLE_VALUE));

	CXFile * pFile = new CXFile();

	HANDLE hFile;
	if (!::DuplicateHandle(::GetCurrentProcess(), m_hFile,
		::GetCurrentProcess(), &hFile, 0, FALSE, DUPLICATE_SAME_ACCESS))
	{
		delete pFile;
		CXFileException ex((long)::GetLastError());
		throw ex;
	}

	pFile->m_hFile = hFile;
	_ASSERT(pFile->m_hFile);
	
	return pFile;
}

//-----------------------------------------------------------------------------
// Creates or opens a file
//-----------------------------------------------------------------------------
void CXFile::Open(wstring strFileName,			// file name 
				 DWORD dwAccess,				// access mode
				 DWORD dwShareMode,				// share mode
				 LPSECURITY_ATTRIBUTES lpSA,	// poiner to SECURITY_ATTRIBUTES
				 DWORD dwCreationDisposition,	// how to create
				 DWORD dwAttributes,			// file attributes
				 HANDLE hTemplateFile)			// handle to template file
{
	_ASSERT(!strFileName.empty());				// file name should not be empty
	_ASSERT(strFileName.length() <= MAX_PATH);	// the file name is limited to 
												// MAX_PATH charactes

	m_hFile = NULL;
	m_strFileName.erase(m_strFileName.begin(), m_strFileName.end());

	// Retrieve the full path and file name 
	TCHAR szTemp[MAX_PATH];
	LPTSTR lpszFilePart;

	if (!::GetFullPathName(strFileName.c_str(), MAX_PATH, szTemp, &lpszFilePart))
	{
		_ASSERT(FALSE);
		m_strFileName = strFileName;
	}

	m_strFileName = szTemp;

	// Attempt file creation
	HANDLE hFile = ::CreateFile(m_strFileName.c_str(), 
								dwAccess, 
								dwShareMode, 
								lpSA,
								dwCreationDisposition, 
								dwAttributes, 
								hTemplateFile);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		CXFileException ex((long)::GetLastError());
		throw ex;
	}

	// See if the file already exists and if it does move the file pointer 
	// to the end of the file
	if (::GetLastError() == ERROR_ALREADY_EXISTS)
		::SetFilePointer(hFile, 0, 0, FILE_END);

	m_hFile = hFile;
}

//-----------------------------------------------------------------------------
// Reads data from a file associated with the CXFile object, 
// starting at the position indicated by the file pointer.
//-----------------------------------------------------------------------------
DWORD CXFile::Read(void* lpBuf, DWORD nCount, LPOVERLAPPED lpOverlapped)
{
	_ASSERT(m_hFile && ((m_hFile) != INVALID_HANDLE_VALUE));

	// Avoid a null read operation, since the behavior of a null
	// read operation depends on the underlying file system
	if (nCount == 0)
		return 0;   

	_ASSERT(lpBuf);

	// Read data from the file
	DWORD dwRead;
	if (!::ReadFile(m_hFile, lpBuf, nCount, &dwRead, lpOverlapped))
	{
		CXFileException ex((long)::GetLastError());
		throw ex;
	}

	return dwRead;
}

//-----------------------------------------------------------------------------
// Writes data from a buffer to the file associated with the CXFile object
//-----------------------------------------------------------------------------
void CXFile::Write(const void* lpBuf, DWORD nCount)
{
	_ASSERT(m_hFile && ((m_hFile) != INVALID_HANDLE_VALUE));
	_ASSERT(lpBuf);

	// Avoid a null write operation, since the behavior of a null
	// write operation depends on the underlying file system
	if (nCount == 0)
		return;

	// Write data to a file
	DWORD dwWritten;
	if (!::WriteFile(m_hFile, lpBuf, nCount, &dwWritten, NULL))
	{
		CXFileException ex((long)::GetLastError());
		throw ex;
	}
}

//-----------------------------------------------------------------------------
// Writes "\r\n" combination at the end of the line
//-----------------------------------------------------------------------------
void CXFile::WriteEndOfLine()
{
	_ASSERT(m_hFile && ((m_hFile) != INVALID_HANDLE_VALUE));

	DWORD dwWritten;
	wstring strCRLF = _T("\r\n");	// \r\n at the end of each line

	// Append CR-LF pair 
	if (!::WriteFile(m_hFile, strCRLF.c_str(), strCRLF.length(), &dwWritten, NULL))
	{
		CXFileException ex((long)::GetLastError());
		throw ex;
	}
}

//-----------------------------------------------------------------------------
// Repositions the pointer in a previously opened file.
//-----------------------------------------------------------------------------
DWORD CXFile::Seek(LONG lOff, UINT nFrom)
{
	_ASSERT(m_hFile && ((m_hFile) != INVALID_HANDLE_VALUE));
	_ASSERT(nFrom == begin || nFrom == end || nFrom == current);
	_ASSERT(begin == FILE_BEGIN && end == FILE_END && current == FILE_CURRENT);

	DWORD dwNew = ::SetFilePointer(m_hFile, lOff, NULL, (DWORD)nFrom);
	if (dwNew  == (DWORD)-1)
	{
		CXFileException ex((long)::GetLastError());
		throw ex;
	}

	return dwNew;
}

//-----------------------------------------------------------------------------
// Obtains the current value of the file pointer, which can be used in 
// subsequent calls to Seek.
//-----------------------------------------------------------------------------
DWORD CXFile::GetPosition() const
{
	_ASSERT(m_hFile && ((m_hFile) != INVALID_HANDLE_VALUE));

	DWORD dwPos = ::SetFilePointer(m_hFile, 0, NULL, FILE_CURRENT);
	if (dwPos  == (DWORD)-1)
	{
		CXFileException ex((long)::GetLastError());
		throw ex;
	}

	return dwPos;
}

//-----------------------------------------------------------------------------
// Flushes any data yet to be written
//-----------------------------------------------------------------------------
void CXFile::Flush()
{
	if (m_hFile == NULL)
		return;

	if (!::FlushFileBuffers(m_hFile))
	{
		CXFileException ex((long)::GetLastError());
		throw ex;
	}
}

//-----------------------------------------------------------------------------
// Closes the file associated with CXFile object and makes the file 
// unavailable for reading or writing.
//-----------------------------------------------------------------------------
void CXFile::Close()
{
	_ASSERT(m_hFile && ((m_hFile) != INVALID_HANDLE_VALUE));

	BOOL bError = FALSE;
	if (m_hFile != NULL)
	{
		bError = !::CloseHandle(m_hFile);
		m_hFile = NULL;
	}

	// Clear the file name
	m_strFileName.erase(m_strFileName.begin(), m_strFileName.end());

	if (bError)
	{
		CXFileException ex((long)::GetLastError());
		throw ex;
	}
}

//-----------------------------------------------------------------------------
// Closes the file associated with CXFile object and makes the file 
// unavailable for reading or writing.
// Will not throw an exception.
//-----------------------------------------------------------------------------
void CXFile::Abort()
{
	if (m_hFile != NULL)
	{
		// Close but ignore errors
		::CloseHandle(m_hFile);
		m_hFile = NULL;
	}

	// Clear the file name
	m_strFileName.erase(m_strFileName.begin(), m_strFileName.end());
}

//-----------------------------------------------------------------------------
// Locks a range of bytes in an open file, throwing an exception if the file is 
// already locked. Locking bytes in a file prevents access to those bytes by other processes.
//-----------------------------------------------------------------------------
void CXFile::LockRange(DWORD dwPos, DWORD dwCount)
{
	_ASSERT(m_hFile && ((m_hFile) != INVALID_HANDLE_VALUE));

	if (!::LockFile(m_hFile, dwPos, 0, dwCount, 0))
	{
		CXFileException ex((long)::GetLastError());
		throw ex;
	}
}

//-----------------------------------------------------------------------------
// Unlocks a range of bytes in an open file. 
//-----------------------------------------------------------------------------
void CXFile::UnlockRange(DWORD dwPos, DWORD dwCount)
{
	_ASSERT(m_hFile && ((m_hFile) != INVALID_HANDLE_VALUE));

	if (!::UnlockFile(m_hFile, dwPos, 0, dwCount, 0))
	{
		CXFileException ex((long)::GetLastError());
		throw ex;
	}
}

//-----------------------------------------------------------------------------
// Changes the length of the file.
//-----------------------------------------------------------------------------
void CXFile::SetLength(DWORD dwNewLen)
{
	_ASSERT(m_hFile && ((m_hFile) != INVALID_HANDLE_VALUE));

	Seek(dwNewLen, CXFile::begin);

	// Move the end-of-file (EOF) position for the file to the current position 
	// of the file pointer. 
	if (!::SetEndOfFile(m_hFile))
	{
		CXFileException ex((long)::GetLastError());
		throw ex;
	}
}

//-----------------------------------------------------------------------------
// Obtains the current logical length of the file in bytes
//-----------------------------------------------------------------------------
DWORD CXFile::GetLength() const
{
	CXFile * pFile = (CXFile*)this;

	return pFile->SeekToEnd();
}

//-----------------------------------------------------------------------------
// Renames the specified file. Directories cannot be renamed
//-----------------------------------------------------------------------------
void CXFile::Rename(wstring strOldName, wstring strNewName)
{
	if (!::MoveFile(strOldName.c_str(), strNewName.c_str()))
	{
		CXFileException ex((long)::GetLastError());
		throw ex;
	}
}

//-----------------------------------------------------------------------------
// Deletes the specified file.
//-----------------------------------------------------------------------------
void CXFile::Remove(wstring strFileName)
{
	if (!::DeleteFile(strFileName.c_str()))
	{
		CXFileException ex((long)::GetLastError());
		throw ex;
	}
}

//-----------------------------------------------------------------------------
// Sets the value of the file pointer to the end of the file.
//-----------------------------------------------------------------------------
DWORD CXFile::SeekToEnd()
{ 
	return Seek(0, CXFile::end); 
}

//-----------------------------------------------------------------------------
// Sets the value of the file pointer to the beginning of the file.
//-----------------------------------------------------------------------------
DWORD CXFile::SeekToBegin()
{ 
	return Seek(0, CXFile::begin);
}

//-----------------------------------------------------------------------------
// Sets the full file path of the selected file, for example, if the path of a 
// file is not available when a CXFile object is constructed, call SetFilePath to 
// provide it.
// SetFilePath does not open the file or create the file; it simply associates the 
// CXFile object with a path name, which can then be used.
//-----------------------------------------------------------------------------
void CXFile::SetFilePath(wstring strNewName)
{
	_ASSERT(m_hFile && ((m_hFile) != INVALID_HANDLE_VALUE));

	// Make sure it's a file path
	int nPos = strNewName.find(_T(":\\"));
	if (nPos == -1)
		_ASSERT(FALSE);	

	m_strFileName = strNewName;
}

//-----------------------------------------------------------------------------
// Retrieves the full file path of the selected file.
//-----------------------------------------------------------------------------
wstring CXFile::GetFilePath() const
{
	_ASSERT(m_hFile && ((m_hFile) != INVALID_HANDLE_VALUE));
	_ASSERT(!m_strFileName.empty());		// file name should not be empty

	return m_strFileName;
}

//-----------------------------------------------------------------------------
// Retrieves the filename of the selected file.
//-----------------------------------------------------------------------------
wstring CXFile::GetFileName() const
{
	_ASSERT(m_hFile && ((m_hFile) != INVALID_HANDLE_VALUE));
	_ASSERT(!m_strFileName.empty());		// file name should not be empty

	// Always capture the complete file name including extension
	LPTSTR lpszTemp = const_cast<LPTSTR>(m_strFileName.c_str());
	for (LPCTSTR lpsz = m_strFileName.c_str(); *lpsz != '\0'; lpsz = _tcsinc(lpsz))
	{
		// Remember last directory/drive separator
		if (*lpsz == '\\' || *lpsz == '/' || *lpsz == ':')
			lpszTemp = const_cast<LPTSTR>(_tcsinc(lpsz));
	}

	return wstring(lpszTemp);
}

//-----------------------------------------------------------------------------
// CXFile does not support direct buffering
//-----------------------------------------------------------------------------
UINT CXFile::GetBufferPtr(UINT nCommand, UINT nCount, void** ppBufStart, void** ppBufMax)
{
	_ASSERT(nCommand == bufferCheck);

	return 0;   // no support
}
