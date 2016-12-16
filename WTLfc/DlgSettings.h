#if !defined(AFX_DLGSETTINGS_H__C1523382_2B1F_11D8_9B84_00055D5E1EBB__INCLUDED_)
#define AFX_DLGSETTINGS_H__C1523382_2B1F_11D8_9B84_00055D5E1EBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSettings.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CDlgSettings dialog

class CDlgSettings : public CDialogImpl<CDlgSettings>, public CWinDataExchange<CDlgSettings>
{
// Construction
public:
	//CDlgSettings(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSettings)
	enum { IDD = IDD_SETTING };
	// CTrackBarCtrl（MFC中为CSliderCtrl）
	CTrackBarCtrl m_sliDDASpeed;
	BOOL m_bEnableAlert;
	BOOL m_bQuickMove;
	BOOL	 m_bEnableDBClick;
	BOOL	 m_bMaxMove;
	int	m_nDDASpeed;
	//}}AFX_DATA

	BEGIN_MSG_MAP(CDlgSettings)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOKCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CDlgSettings)
		//DDV_MinMaxUInt(m_nGameNumber, 1, 2147483647);
		//DDX_INT(IDC_EDIT_WIDTH, m_nWidth)
		//DDX_INT(IDC_EDIT_HEIGHT, m_nHeight)
		//DDX_TEXT(IDC_EDIT_GAMENUMBER, m_strGameNumber)
		//DDX_UINT_RANGE(IDC_EDIT_GAMENUMBER, m_nGameNumber, 1, 2147483647)

		//DDX_Control(pDX, IDSLI_DDASPEED, m_sliDDASpeed);
		//DDX_Check(pDX, IDCHK_CHECKALERT, m_bEnableAlert);
		//DDX_Check(pDX, IDCHK_CHECKQUICKLY, m_bQuickMove);
		//DDX_Check(pDX, IDCHK_ENABLELBDCLICK, m_bEnableDBClick);
		//DDX_Check(pDX, IDCHK_MOVEMAX, m_bMaxMove);
		//DDX_Slider(pDX, IDSLI_DDASPEED, m_nDDASpeed);

		DDX_INT(IDCHK_CHECKALERT, m_bEnableAlert);
		DDX_INT(IDCHK_CHECKQUICKLY, m_bQuickMove);
		DDX_INT(IDCHK_ENABLELBDCLICK, m_bEnableDBClick);
		DDX_INT(IDCHK_MOVEMAX, m_bMaxMove);

		//DDX_CONTROL_HANDLE(IDSLI_DDASPEED, m_sliDDASpeed);
		//DDX_Slider(IDSLI_DDASPEED, m_nDDASpeed);	// WTL 中没有对应命令
	END_DDX_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSettings)
	protected:
	//virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSettings)
	//virtual BOOL OnInitDialog();
	//}}AFX_MSG
	//DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETTINGS_H__C1523382_2B1F_11D8_9B84_00055D5E1EBB__INCLUDED_)
