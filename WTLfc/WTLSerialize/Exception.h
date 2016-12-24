// Exception.h: implementation of the exception classes.
//
//////////////////////////////////////////////////////////////////////

#if !defined EXCEPTION_WTL_H
#define EXCEPTION_WTL_H

//-----------------------------------------------------------------------------
// Class CXException - root class for all exception classes
//-----------------------------------------------------------------------------
class CXException
{
public:
	virtual long GetErrorNumber() = 0;
	virtual wstring GetErrorDesc() = 0;

protected:
	long m_lErrorNo;		// error number
	wstring m_strError;		// error description
};

//-----------------------------------------------------------------------------
// Class CArchiveException implements the exception thrown by CXArchive methods
//-----------------------------------------------------------------------------
class CXArchiveException : public CXException
{
public:
	CXArchiveException(int nCause = CXArchiveException::none, 
		wstring strArchiveName = _T("")) : m_strFileName(strArchiveName)
	{
		m_lErrorNo = nCause;
		m_strError = ErrNumberToDescription(nCause);
	}

	~CXArchiveException() {}

	virtual long GetErrorNumber()
	{
		return m_lErrorNo;
	}

	virtual wstring GetErrorDesc()
	{
		return m_strError;
	}

	wstring GetArchiveFile()
	{
		return m_strFileName;
	}

	enum {
		none,
		generic,
		readOnly,
		endOfFile,
		writeOnly
	};

private:
	wstring m_strFileName;

	// Maps error number to its textual description
	wstring ErrNumberToDescription(int lErr)
	{
		wstring strError;

		switch (lErr)
		{
		case none:
			strError =  _T("None");
			break;
		case generic:
			strError =  _T("Generic archive exception");
			break;
		case readOnly:
			strError =  _T("Archive file is read-only");
			break;
		case endOfFile:
			strError =  _T("End of archive file exception");
			break;
		case writeOnly:
			strError =  _T("Archive file is write-only");
			break;
		default:
			strError =  _T("Unknown archive exception");
		}

		return strError;
	}
};

#endif // !defined EXCEPTION_WTL_H