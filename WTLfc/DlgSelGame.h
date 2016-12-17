#if !defined(AFX_DLGSELGAME_H__BC990402_2A29_11D8_9B84_00055D5E1EBB__INCLUDED_)
#define AFX_DLGSELGAME_H__BC990402_2A29_11D8_9B84_00055D5E1EBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSelGame.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CDlgSelGame dialog

class CDlgSelGame : public CDialogImpl<CDlgSelGame>, public CWinDataExchange<CDlgSelGame>
{
// Construction
public:
	//CDlgSelGame(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSelGame)
	enum { IDD = IDD_SELGAME };
	//UINT m_nGameNumber;
	int m_nGameNumber;
	WTL::CString m_strGameNumber;
	//}}AFX_DATA

	BEGIN_MSG_MAP(CWndSizeDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOKCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CWndSizeDlg)
		//DDV_MinMaxUInt(m_nGameNumber, 1, 2147483647);
		//DDX_INT(IDC_EDIT_WIDTH, m_nWidth)
		//DDX_INT(IDC_EDIT_HEIGHT, m_nHeight)
		DDX_TEXT(IDC_EDIT_GAMENUMBER, m_strGameNumber)
		DDX_UINT_RANGE(IDC_EDIT_GAMENUMBER, m_nGameNumber, 1, 2147483647)
	END_DDX_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSelGame)
	protected:
	//virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSelGame)
	//}}AFX_MSG
	//DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSELGAME_H__BC990402_2A29_11D8_9B84_00055D5E1EBB__INCLUDED_)
