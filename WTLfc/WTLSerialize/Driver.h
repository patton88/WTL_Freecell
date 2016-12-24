// Driver.h: implementation of the CDriver classes used to test CXArchive
//
//////////////////////////////////////////////////////////////////////

class CDriverString
{
	TCHAR szString[64];
	TCHAR szString1[32];
	CString strString;
	
	TCHAR szResult[64];
	TCHAR szResult1[32];
	CString strResult;
	
public:
	CDriverString()
	{
		_tcscpy(szString, L"Error happened during the program execution.");
		_tcscpy(szString1, L"Blue screen of death.");
		strString = L"Test CString class.";
	}
	
	void Serialize(CXArchive& ar)
	{
		if (ar.IsStoring())
		{
			ar.WriteString(strString);
			ar.WriteString(L"\n");
			ar.WriteString(szString);
			ar.WriteString(L"\n");
			ar.WriteString(szString1);
			ar.WriteString(L"\n");
		}
		else
		{
			ar.ReadString(strResult);
			ar.ReadString(szResult, (sizeof(szResult)-1)/sizeof(TCHAR));
			ar.ReadString(szResult1, (sizeof(szResult1)-1)/sizeof(TCHAR));
		}
	}
	
	void Display()
	{
		wcout << setiosflags(ios::left) << setw(15) << L"CString:" << setw(50) << strResult.GetBuffer(0) << endl;
		wcout << setiosflags(ios::left) << setw(15) << L"LPCTSTR:" << setw(50) << szResult << endl;
		wcout << setiosflags(ios::left) << setw(15) << L"LPCTSTR1:" << setw(50) << szResult1 << endl;
	}
};

class CDriver
{
	BYTE m_byte;
	WORD m_word;
	LONG m_long;
	DWORD m_dword;
	float m_float;
	double m_double;
	LONGLONG m_dwdw;
	ULONGLONG m_udwdw;
	int m_int;
	short m_short;
	char m_char;
	unsigned m_uint;
	bool m_bool;

	CDriverString testStr;

public:
	CDriver()
	{
		m_byte = 'A';
		m_word = -32768;
		m_long = -1000000000;
		m_dword = 56789;
		m_float = 123.23F;
		m_double = -987654321;
		m_dwdw = -116444736000000009;
		m_udwdw = 116444736999999991;
		m_int = 32767;
		m_short = 12345;
		m_char = 'T';
		m_uint = 32768;
		m_bool = true;
	}

	void Serialize(CXArchive& ar)
	{
		if (ar.IsStoring())
		{
			ar << m_byte;
			ar << m_word;
			ar << m_long;
			ar << m_dword;
			ar << m_float;
			ar << m_double;
			ar << m_dwdw;
			ar << m_udwdw;
			ar << m_int;
			ar << m_short;
			ar << m_char;
			ar << m_uint;
			ar << m_bool;
		}
		else
		{
			ar >> m_byte;
			ar >> m_word;
			ar >> m_long;
			ar >> m_dword;
			ar >> m_float;
			ar >> m_double;
			ar >> m_dwdw;
			ar >> m_udwdw;
			ar >> m_int;
			ar >> m_short;
			ar >> m_char;
			ar >> m_uint;
			ar >> m_bool;
		}

		testStr.Serialize(ar);
	}

	void Display()
	{
		cout << setiosflags(ios::left) << setw(15) << "BYTE:" << setw(50) << m_byte << endl;
		cout << setiosflags(ios::left) << setw(15) << "WORD:" << setw(50) << m_word << endl;
		cout << setiosflags(ios::left) << setw(15) << "LONG:" << setw(50) << m_long << endl;
		cout << setiosflags(ios::left) << setw(15) << "DWORD:" << setw(50) << m_dword << endl;
		cout << setiosflags(ios::left) << setw(15) << "float:" << setw(50) << m_float << endl;
		cout << setiosflags(ios::left) << setw(15) << "double:" << setw(50) << m_double << endl;
		//cout << setiosflags(ios::left) << setw(15) << "LONGLONG:" << setw(50) << m_dwdw << endl;
		//cout << setiosflags(ios::left) << setw(15) << "ULONGLONG:" << setw(50) << m_udwdw << endl;
		cout << setiosflags(ios::left) << setw(15) << "int:" << setw(50) << m_int << endl;
		cout << setiosflags(ios::left) << setw(15) << "short:" << setw(50) << m_short << endl;
		cout << setiosflags(ios::left) << setw(15) << "char:" << setw(50) << m_char << endl;
		cout << setiosflags(ios::left) << setw(15) << "uint:" << setw(50) << m_uint << endl;
		cout << setiosflags(ios::left) << setw(15) << "bool:" << setw(50) << m_bool << endl;

		testStr.Display();
	}
};

