// File.h: interface for the CXFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined FILE_WTL_H
#define FILE_WTL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning( disable : 4290 ) // a function is declared using exception specification, 
								  // which Visual C++ accepts but does not implement

#include "Exception.h"

//-----------------------------------------------------------------------------
// Class CXFile implements basic nonbuffered file access that is a wrapper 
// around the Windows file APIs.
//-----------------------------------------------------------------------------
class CXFile
{
public:
	enum SeekPosition { begin = 0x0, current = 0x1, end = 0x2 };

	// Constructors, destructor
	CXFile();
	CXFile(HANDLE hFile);
	virtual ~CXFile();

	// Attributes
	HANDLE m_hFile;

	// Operations
	virtual DWORD GetPosition() const throw(CXFileException);
	virtual wstring GetFileName() const;
	virtual wstring GetFilePath() const;
	virtual void SetFilePath(wstring strNewName);

	virtual void Open(wstring strFileName, DWORD dwAccess, DWORD dwShareMode, 
		LPSECURITY_ATTRIBUTES lpSA, DWORD dwCreationDisposition, DWORD dwAttributes,
		HANDLE hTemplateFile) throw(CXFileException);

	static void Rename(wstring strOldName, wstring strNewName) throw(CXFileException);
	static void Remove(wstring strFileName) throw(CXFileException);

	DWORD SeekToEnd() throw(CXFileException);
	DWORD SeekToBegin() throw(CXFileException);

	virtual CXFile * Duplicate() const throw(CXFileException);

	virtual DWORD Seek(LONG lOff, UINT nFrom) throw(CXFileException);
	virtual void SetLength(DWORD dwNewLen) throw(CXFileException);
	virtual DWORD GetLength() const throw(CXFileException);

	virtual DWORD Read(void* lpBuf, DWORD nCount, LPOVERLAPPED lpOverlapped = NULL) throw(CXFileException);
	virtual void Write(const void* lpBuf, DWORD nCount) throw(CXFileException);
	void WriteEndOfLine() throw(CXFileException);

	virtual void LockRange(DWORD dwPos, DWORD dwCount) throw(CXFileException);
	virtual void UnlockRange(DWORD dwPos, DWORD dwCount) throw(CXFileException);

	virtual void Abort() throw();
	virtual void Flush() throw(CXFileException);
	virtual void Close() throw(CXFileException);

	enum BufferCommand { 
				bufferRead, 
				bufferWrite, 
				bufferCommit, 
				bufferCheck, 
				bufferBlocking,
				bufferDirect
	};
	virtual UINT GetBufferPtr(UINT nCommand, UINT nCount = 0, void** ppBufStart = NULL, void** ppBufMax = NULL);

protected:
	wstring m_strFileName;	// stores the file name for the current file
};

#endif // !defined FILE_WTL_H
