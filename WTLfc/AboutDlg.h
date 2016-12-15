// aboutdlg.h : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CAboutDlg : public CDialogImpl<CAboutDlg>
{
public:
	enum { IDD = IDD_ABOUTBOX };

	CBrush      m_brushBkgnd;    //±³¾°É«,
	COLORREF    m_clrText;       //ÎÄ×ÖÑÕÉ«

	CAboutDlg(COLORREF clrText = RGB(0, 0, 255));

	BEGIN_MSG_MAP(CAboutDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)

		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		//MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorDlg)
		//MESSAGE_HANDLER(WM_CTLCOLORDLG, OnCtlColorDlg)
		//MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorDlg)
		//MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorDlg)
		//MESSAGE_HANDLER(WM_CTLCOLORMSGBOX, OnCtlColorDlg)
		MESSAGE_HANDLER(WM_CTLCOLORLISTBOX, OnCtlColorDlg)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
	LRESULT OnCtlColorDlg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};
