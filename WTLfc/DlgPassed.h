#if !defined(AFX_PASSEDDLG_H__67586726_CDE2_4B9F_8E2C_CBFB09C64585__INCLUDED_)
#define AFX_PASSEDDLG_H__67586726_CDE2_4B9F_8E2C_CBFB09C64585__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PassedDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPassedDlg dialog
enum { RandGame = 1, PrevGame, NextGame, ThisGame, SaveGame, PlayShow };

//class CDlgPassed : public CDialog
class CDlgPassed : public CDialogImpl<CDlgPassed>
	//, public CWinDataExchange<CDlgPassed>
{
// Construction
public:
	//CDlgPassed(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPassedDlg)
	enum { IDD = IDD_PASSED };
	int		m_nChoice;
	//}}AFX_DATA

	//BEGIN_MESSAGE_MAP(CDlgPassed, CDialog)
	//	//{{AFX_MSG_MAP(CPassedDlg)
	//	ON_BN_CLICKED(IDB_PREV_GAME, OnPrevGame)
	//	ON_BN_CLICKED(IDB_NEXT_GAME, OnNextGame)
	//	ON_BN_CLICKED(IDB_RAND, OnRand)
	//	ON_BN_CLICKED(IDB_SHOW, OnShow)
	//	ON_BN_CLICKED(IDB_AGAIN, OnAgain)
	//	ON_BN_CLICKED(IDB_SAVE, OnSave)
	//	//}}AFX_MSG_MAP
	//END_MESSAGE_MAP()

	//CDlgPassed();

	BEGIN_MSG_MAP(CDlgAICal)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		//COMMAND_ID_HANDLER(IDOK, OnOKCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		COMMAND_ID_HANDLER(IDB_PREV_GAME, OnPrevGame)
		COMMAND_ID_HANDLER(IDB_NEXT_GAME, OnNextGame)
		COMMAND_ID_HANDLER(IDB_RAND, OnRand)
		COMMAND_ID_HANDLER(IDB_SHOW, OnShow)
		COMMAND_ID_HANDLER(IDB_AGAIN, OnAgain)
		COMMAND_ID_HANDLER(IDB_SAVE, OnSave)
		//ON_WM_TIMER()
	END_MSG_MAP()

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnPrevGame(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnNextGame(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnRand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnShow(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAgain(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	//virtual BOOL OnInitDialog();
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	//WTL 7.1中添加的一个新特性是DDX_CONTROL_HANDLE宏。在WTL 7.0里，如果你想对一个单纯的窗口接口类使用
	//DDX（像CWindow，CListViewCtrl等），你不能使用DDX_CONTROL，因为DDX_CONTROL只能和CWindowImpl的派生类一起工作。
	//除了所需基类不同之外，DDX_CONTROL_HANDLE和DDX_CONTROL一样地工作
	//BEGIN_DDX_MAP(CDlgAICal)
	//	//DDX_INT(IDC_EDIT_WIDTH, m_nWidth)
	//	//DDX_INT(IDC_EDIT_HEIGHT, m_nHeight)
	//	DDX_CONTROL_HANDLE(IDCHK_MAX_SPEED, m_chkMaxSpeed)
	//	DDX_CONTROL_HANDLE(IDB_STOP, m_btnStop)
	//	DDX_CONTROL_HANDLE(IDC_PROGRESS_STATUS, m_ctrlProgress)
	//END_DDX_MAP()


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPassedDlg)
	//protected:
	//virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
//protected:

	// Generated message map functions
	//{{AFX_MSG(CPassedDlg)
	//afx_msg void OnPrevGame();
	//afx_msg void OnNextGame();
	//afx_msg void OnRand();
	//afx_msg void OnShow();
	//afx_msg void OnAgain();
	//afx_msg void OnSave();
	////}}AFX_MSG
	//DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PASSEDDLG_H__67586726_CDE2_4B9F_8E2C_CBFB09C64585__INCLUDED_)
