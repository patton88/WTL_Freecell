// WndSizeDlg.h : interface of the CWndSizeDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CWndSizeDlg : public CDialogImpl<CWndSizeDlg>, public CWinDataExchange<CWndSizeDlg>
{
public:
	enum { IDD = IDD_WND_SIZE };

	BEGIN_MSG_MAP(CWndSizeDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOKCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CWndSizeDlg)
		DDX_INT(IDC_EDIT_WIDTH, m_nWidth)
		DDX_INT(IDC_EDIT_HEIGHT, m_nHeight)
	END_DDX_MAP()

	int m_nWidth;
	int m_nHeight;

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
