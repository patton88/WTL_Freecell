// Archive.h: interface for the CXArchive class.
//
///////////////////////////////////////////////////////////////////////////////

#if !defined ARCHIVE_WTL_H
#define ARCHIVE_WTL_H

#pragma warning (disable:4290)
#include "Exception.h"

//-----------------------------------------------------------------------------
// Class CXArchive implements a serialization logic to save and load a large
// number of objects to and from a disk storage that persists after those
// objects are deleted.
//-----------------------------------------------------------------------------
class CXArchive
{
public:
	// Flag values
	enum Mode { store = 0, load = 1, bNoFlushOnDelete = 2 };

	CXArchive(FILE* pFile, UINT nMode, int nBufSize = 4096, void* lpBuf = NULL);
	~CXArchive();

	// These functions access the mode to determine the state of the CXArchive object
	BOOL IsLoading() const;
	BOOL IsStoring() const;

	FILE* GetFile() const;

	// Operations
	UINT Read(void* lpBuf, UINT nMax);
	void Write(const void* lpBuf, UINT nMax);
	void Flush();
	void Close();
	void Abort();   // close and shutdown without exceptions
	UINT GetBufferPtr(UINT nCommand, UINT nCount = 0, void** ppBufStart = NULL, void** ppBufMax = NULL);

	// Reading and writing strings
	void WriteString(LPCTSTR lpsz);

	LPTSTR ReadString(LPTSTR lpsz, UINT nMax) throw(CXArchiveException);
	//在函数声明前添加加：#pragma warning (disable:4290)，可屏蔽warning C4290，但并不能保证编译器接受异常规范
	//warning C4290: C++ exception specification ignored except to indicate a function is not __declspec(nothrow)

	BOOL ReadString(WTL::CString& rString);

	// Insertion operations
	CXArchive& operator<<(BYTE by);
	CXArchive& operator<<(WORD w);
	CXArchive& operator<<(LONG l);
	CXArchive& operator<<(DWORD dw);
	CXArchive& operator<<(float f);
	CXArchive& operator<<(double d);
	CXArchive& operator<<(LONGLONG dwdw);
	CXArchive& operator<<(ULONGLONG dwdw);
	CXArchive& operator<<(int i);
	CXArchive& operator<<(short w);
	CXArchive& operator<<(char ch);
#ifdef _NATIVE_WCHAR_T_DEFINED
	CXArchive& operator<<(wchar_t ch);
#endif
	CXArchive& operator<<(unsigned u);
	CXArchive& operator<<(bool b);

	// Extraction operations
	CXArchive& operator>>(BYTE& by);
	CXArchive& operator>>(WORD& w);
	CXArchive& operator>>(DWORD& dw);
	CXArchive& operator>>(LONG& l);
	CXArchive& operator>>(float& f);
	CXArchive& operator>>(double& d);
	CXArchive& operator>>(LONGLONG& dwdw);
	CXArchive& operator>>(ULONGLONG& dwdw);
	CXArchive& operator>>(int& i);
	CXArchive& operator>>(short& w);
	CXArchive& operator>>(char& ch);
#ifdef _NATIVE_WCHAR_T_DEFINED
	CXArchive& operator>>(wchar_t& ch);
#endif
	CXArchive& operator>>(unsigned& u);
	CXArchive& operator>>(bool& b);

protected:
	// Archive objects can not be copied or assigned
	CXArchive(const CXArchive& arSrc) {}
	void operator=(const CXArchive& arSrc) {}

	void FillBuffer(UINT nBytesNeeded) throw(CXArchiveException);

	FILE* m_pFile;
	wstring m_strFileName;

	BOOL m_bDirectBuffer;   // TRUE if m_pFile supports direct buffering
	BOOL m_bBlocking;		// TRUE if m_pFile can block for unbounded periods of time
	BOOL m_nMode;			// specifies if the archive is reading or writing, also can be 
							// used to turn off flushing on delete
	BOOL m_bUserBuf;
	int m_nBufSize;
	BYTE * m_lpBufCur;
	BYTE * m_lpBufMax;
	BYTE * m_lpBufStart;

	enum BufferCommand { 
			bufferRead, 
			bufferWrite, 
			bufferCommit, 
			bufferCheck, 
			bufferBlocking,
			bufferDirect
	};

};

#endif	// !defined ARCHIVE_WTL_H