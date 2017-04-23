#pragma once

#include "stdafx.h"

// DlgColorDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgScore dialog

//WTL CFileDialog 文件选择对话框居中问题
//标签： nullclass
//2008-11-30 10:54 2609人阅读 评论(1) 收藏 举报
//版权声明：本文为博主原创文章，未经博主允许不得转载。
//
//CFileDialog 及其基类 CFileDialogImpl 用于显示文件打开和文件保存对话框。
//通过分析WTL源代码发现CFileDialog 的功能做的很不够；在弹出文件打开和文件保存对话框的时候没有居中，
//为了美观需要实现居中，方法是自己派生一个新类来实现：

//class CColorDialog : public CColorDialogImpl<CColorDialog>
//{
//public:
//	CColorDialog(COLORREF clrInit = 0, DWORD dwFlags = 0, HWND hWndParent = NULL)
//		: CColorDialogImpl<CColorDialog>(clrInit, dwFlags, hWndParent)
//	{ }
//
//	// override base class map and references to handlers
//	DECLARE_EMPTY_MSG_MAP()
//};

// 通过子类化解决 WTL CColorDialog 颜色选择对话框居中问题
class CDlgColorDialog : public CColorDialogImpl<CDlgColorDialog>
{
public:
	//CDlgColorDialog(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
	//                         LPCTSTR lpszDefExt = NULL,
	//                         LPCTSTR lpszFileName = NULL,
	//                         DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
	//                         LPCTSTR lpszFilter = NULL,
	//                         HWND hWndParent = NULL)
	//                         : CFileDialogImpl<CDlgColorDialog>(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, hWndParent){ }

	CDlgColorDialog(COLORREF clrInit = 0, DWORD dwFlags = 0, HWND hWndParent = NULL)
		: CColorDialogImpl<CDlgColorDialog>(clrInit, dwFlags, hWndParent), m_bMsgHandled(FALSE)
	{ }

	// Maps
	BEGIN_MSG_MAP(CDlgColorDialog)
		//CHAIN_MSG_MAP(CFileDialogImpl<CDlgColorDialog>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		CHAIN_MSG_MAP(CColorDialogImpl<CDlgColorDialog>)
	END_MSG_MAP()

	// override base class map and references to handlers
	//DECLARE_EMPTY_MSG_MAP()

	//重载此函数；实现居中
	//void OnInitDone( LPOFNOTIFY lpon )
	//{
	//	//GetFileDialogWindow().CenterWindow(lpon->lpOFN->hwndOwner);

	//}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		//CDialog::OnInitDialog();
		CenterWindow(GetParent());

		bHandled = false;	// 让基类继续处理
		return false;
	}
};
