// WndSizeDlg.cpp : implementation of the CWndSizeDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "WndSizeDlg.h"

LRESULT CWndSizeDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());

	CRect wr;
	//::GetWindowRect(GetParent(), wr);	// 得到CMainFrame窗口矩形
	g_pView->GetClientRect(wr);	// 得到View窗口客户区矩形
	m_nWidth = wr.Width();
	m_nHeight = wr.Height();

	// First DDX call, hooks up variables to controls.
	DoDataExchange(false);

	// Set the focus, and we're done.
	HWND hEdit = GetDlgItem(IDC_EDIT_WIDTH);	// 定义控件句柄hEdit变量，并赋值
	CEdit editWidth(hEdit);		// 1、使用构造函数，将窗口变量 editWidth 关联控件句柄 hEdit
	editWidth.SetFocus();
	editWidth.SendMessage(WM_KEYDOWN, VK_END, 0);

	return FALSE; // 不能返回TRUE
}

LRESULT CWndSizeDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

LRESULT CWndSizeDlg::OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int nWidth = m_nWidth;
	int nHight = m_nHeight;
	DoDataExchange(true);

	//if (nWidth != m_nWidth || nHight != m_nHeight)
	if (abs(nWidth - m_nWidth) > 5 || abs(nHight - m_nHeight) > 5)		// 为避免计算误差，设置取值范围
	{
		g_fcCcs.SetCardSize(m_nWidth);
		g_pView->m_bWndRestored = false;		// 否则AdjustFrameToFixedSize()会将窗口设置为缺省大小
		g_pView->AdjustFrameToFixedSize();
	}
	
	EndDialog(wID);
	return 0;
}
