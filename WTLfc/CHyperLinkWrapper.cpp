// CHyperLinkWrapper.cpp
// 为避免Debug模式下系统没有默认浏览器、默认邮件程序时报错终止
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CHyperLinkWrapper.h"

bool CHyperLinkWrapper::Navigate()
{
	ATLASSERT(::IsWindow(m_hWnd));
	bool bRet = true;
	if (IsNotifyButton())
	{
		NMHDR nmhdr = { m_hWnd, (UINT_PTR)GetDlgCtrlID(), NM_CLICK };
		::SendMessage(GetParent(), WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&nmhdr);
	}
	else if (IsCommandButton())
	{
		::SendMessage(GetParent(), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)m_hWnd);
	}
	else
	{
		ATLASSERT(m_lpstrHyperLink != NULL);
#ifndef _WIN32_WCE
		DWORD_PTR dwRet = (DWORD_PTR)::ShellExecute(0, _T("open"), m_lpstrHyperLink, 0, 0, SW_SHOWNORMAL);
		bRet = (dwRet > 32);
#else // CE specific
		SHELLEXECUTEINFO shExeInfo = { sizeof(SHELLEXECUTEINFO), 0, 0, L"open", m_lpstrHyperLink, 0, 0, SW_SHOWNORMAL, 0, 0, 0, 0, 0, 0, 0 };
		::ShellExecuteEx(&shExeInfo);
		DWORD_PTR dwRet = (DWORD_PTR)shExeInfo.hInstApp;
		bRet = (dwRet == 0) || (dwRet > 32);
#endif // _WIN32_WCE
		//ATLASSERT(bRet);	//注视掉该句，是为避免Debug模式下系统没有默认浏览器、默认邮件程序时报错终止
		if (bRet)
		{
			m_bVisited = true;
			Invalidate();
		}
	}
	return bRet;
}
