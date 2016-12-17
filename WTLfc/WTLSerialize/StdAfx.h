// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__AC4D2145_1689_11D6_9A95_00A0C9D5029B__INCLUDED_)
#define AFX_STDAFX_H__AC4D2145_1689_11D6_9A95_00A0C9D5029B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define WINVER		0x0400
#define _WIN32_WINNT	0x0400
#define _WIN32_IE    0x0400		// atlapp.h(28) : fatal error C1189: #error :  WTL requires IE version 3.0 or higher


#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlmisc.h>
#include <tchar.h>

#include <iostream>
#include <iomanip>
#include <string>
using namespace std;
#include <windows.h>

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__AC4D2145_1689_11D6_9A95_00A0C9D5029B__INCLUDED_)
