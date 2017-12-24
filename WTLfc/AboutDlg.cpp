// aboutdlg.cpp : implementation of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"

CAboutDlg::CAboutDlg(COLORREF clrText) : m_bMsgHandled(FALSE)
{
	//默认颜色
	//m_brushBkgnd.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
	//m_brushBkgnd.CreateSolidBrush(RGB(218, 218, 218));
	//m_brushBkgnd.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
	m_brushBkgnd.CreateSolidBrush(RGB(0, 128, 0));
	m_clrText = clrText;
}

LRESULT CAboutDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());

	//将版权信息放入列表框中
	wchar_t *msg[] =
	{
		TEXT("--------------------------------------------"),
		TEXT("WTL_Freecell v1.0 (WTL空档接龙v1.0) 20180110"),
		TEXT("--------------------------------------------"),
		TEXT("By        ：MinJun"),
		TEXT("E-mail    ：ybmj@vip.163.com"),
		TEXT("QQ        ：153248043"),
		TEXT("Blog      ：http://blog.csdn.net/shuren8/"),
		TEXT("--------------------------------------------"),
	};

	//CListBox *plb = (CListBox *)GetDlgItem(IDLB_ABOUT);
	CListBox lb(::GetDlgItem(m_hWnd, IDLB_ABOUT));
	int cnt = 0;
	while (cnt < sizeof(msg) / sizeof(char*))
		lb.AddString(msg[cnt++]);

	// Set up the hyperlink
	m_wndLink.SubclassWindow(GetDlgItem(IDC_CP_LINK));
	DWORD linkStyle = m_wndLink.GetHyperLinkExtendedStyle() | HLINK_COMMANDBUTTON | HLINK_NOTIFYBUTTON;
	m_wndLink.SetHyperLinkExtendedStyle(linkStyle);
	m_wndLink.SetHyperLink(L"http://blog.csdn.net/shuren8/");

	return TRUE;		// return TRUE unless you set the focus to a control
					// EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CAboutDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

// 改变对话框背景色
LRESULT CAboutDlg::OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	//CBrush brushBkgnd;    //背景色,
	//brushBkgnd.CreateSolidBrush(RGB(192,192,192));

	//T* pThis = static_cast<T*>(this);
	HDC hDc = (HDC)wParam;
	RECT rcRect;
	//pThis->GetClientRect(&rcRect);
	GetClientRect(&rcRect);
	FillRect(hDc, &rcRect, m_brushBkgnd);
	return 1;
}

LRESULT CAboutDlg::OnCtlColorDlg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	HDC hDc = (HDC)wParam;
	switch (uMsg)
	{
	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLORBTN:
	case WM_CTLCOLORMSGBOX:
	case WM_CTLCOLORLISTBOX:
	{
		::SetBkMode(hDc, TRANSPARENT);
		::SetTextColor(hDc, m_clrText);
		bHandled = TRUE;
		return (LRESULT)m_brushBkgnd.m_hBrush;
	}
	break;
	case WM_CTLCOLORDLG:
	case WM_CTLCOLOREDIT:
	default:
		break;
	}
	return 0;
}

LRESULT CAboutDlg::OnLinkIntro(int wParam, LPNMHDR lpNMHdr, BOOL& bHandled)
{
	::ShellExecute(NULL, L"open", L"http://blog.csdn.net/shuren8/",
		NULL, NULL, SW_SHOWNORMAL);
	return 0;
}
