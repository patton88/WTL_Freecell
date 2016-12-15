#if !defined(AFX_DLGAICAL_H__8B502F4B_EDE6_45C7_BC47_6995511B6004__INCLUDED_)
#define AFX_DLGAICAL_H__8B502F4B_EDE6_45C7_BC47_6995511B6004__INCLUDED_

#pragma once

// DlgAICal.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAICal dialog

//class CDlgAICal : public CDialog
class CDlgAICal : public CDialogImpl<CDlgAICal>, public CWinDataExchange<CDlgAICal>
{
// Construction
public:
	enum { IDD = IDD_AICAL };

	BEGIN_MSG_MAP(CDlgAICal)
		//MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		//COMMAND_ID_HANDLER(IDOK, OnOKCmd)
		//COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCHK_MAX_SPEED, OnMaxSpeed)
		COMMAND_ID_HANDLER(IDB_STOP, OnStop)
		//ON_WM_TIMER()
	END_MSG_MAP()

	//WTL 7.1中添加的一个新特性是DDX_CONTROL_HANDLE宏。在WTL 7.0里，如果你想对一个单纯的窗口接口类使用
	//DDX（像CWindow，CListViewCtrl等），你不能使用DDX_CONTROL，因为DDX_CONTROL只能和CWindowImpl的派生类一起工作。
	//除了所需基类不同之外，DDX_CONTROL_HANDLE和DDX_CONTROL一样地工作
	BEGIN_DDX_MAP(CDlgAICal)
		//DDX_INT(IDC_EDIT_WIDTH, m_nWidth)
		//DDX_INT(IDC_EDIT_HEIGHT, m_nHeight)
		DDX_CONTROL_HANDLE(IDCHK_MAX_SPEED, m_chkMaxSpeed)
		DDX_CONTROL_HANDLE(IDB_STOP, m_btnStop)
		DDX_CONTROL_HANDLE(IDC_PROGRESS_STATUS, m_ctrlProgress)
	END_DDX_MAP()

	LRESULT OnMaxSpeed(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnStop(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	void OnTimer(UINT nIDEvent);
	//virtual BOOL OnInitDialog();
	void StartThread();
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	//LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	//LRESULT OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	//CWinThread * m_hCalThread;
	HANDLE m_hCalThread;
	bool m_bSuccess;
	UINT m_nTimer;
	//CDlgAICal(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CDlgAICal)
	CButton	m_chkMaxSpeed;
	CButton	m_btnStop;
	CProgressBarCtrl m_ctrlProgress;
	//}}AFX_DATA

public:
	// Generated message map functions
	//{{AFX_MSG(CDlgAICal)
	//afx_msg void OnStop();
	//afx_msg void OnTimer(UINT nIDEvent);
	//virtual BOOL OnInitDialog();
	//afx_msg void OnMaxSpeed();
	////}}AFX_MSG
	//DECLARE_MESSAGE_MAP()


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGAICAL_H__8B502F4B_EDE6_45C7_BC47_6995511B6004__INCLUDED_)
