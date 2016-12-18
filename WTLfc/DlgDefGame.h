#if !defined(AFX_DLGDEFGAME_H__5609046B_6116_4BF2_A965_0974BD952BDE__INCLUDED_)
#define AFX_DLGDEFGAME_H__5609046B_6116_4BF2_A965_0974BD952BDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDefGame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
class CDGWnd;
// CDlgDefGame dialog
class CDlgDefGame : public CDialogImpl<CDlgDefGame>
{
// Construction
public:
	CDGWnd *m_pDGWnd;
	//CDlgDefGame(CWnd* pParent = NULL);   // standard constructor
	CDlgDefGame();
	~CDlgDefGame();

// Dialog Data
	//{{AFX_DATA(CDlgDefGame)
	enum { IDD = IDD_DEF_GAME };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	BEGIN_MSG_MAP(CWndSizeDlg)
		//MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
		//MESSAGE_HANDLER(WM_SIZING, OnSizeing)
		//MESSAGE_HANDLER(WM_CREATE, OnCreate)
		//MESSAGE_HANDLER(WM_CLOSE, OnClose)

		//MESSAGE_HANDLER(IDM_SAVE_DEF, OnSaveDef)
		//MESSAGE_HANDLER(IDM_LOAD_DEF, OnLoadDef)
		//MESSAGE_HANDLER(IDM_SAVE_DEF_AS, OnSaveDefAs)
		//MESSAGE_HANDLER(IDM_NEW_DEF, OnNewDef)

		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOKCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

	LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSizeing(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnSaveDef(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLoadDef(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSaveDefAs(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnNewDef(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	//ON_WM_CREATE()
	//ON_WM_SETFOCUS()
	//ON_WM_SIZING()
	//ON_WM_CLOSE()
	//ON_COMMAND(IDM_SAVE_DEF, OnSaveDef)
	//ON_COMMAND(IDM_LOAD_DEF, OnLoadDef)
	//ON_COMMAND(IDM_SAVE_DEF_AS, OnSaveDefAs)
	//ON_COMMAND(IDM_NEW_DEF, OnNewDef)

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDefGame)
	protected:
	//virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDefGame)
	//afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//afx_msg void OnSetFocus(CWnd* pOldWnd);
	//afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	//virtual BOOL OnInitDialog();
	//afx_msg void OnSaveDef();
	//afx_msg void OnLoadDef();
	//afx_msg void OnClose();
	//afx_msg void OnSaveDefAs();
	//afx_msg void OnNewDef();
	//}}AFX_MSG
	//DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDEFGAME_H__5609046B_6116_4BF2_A965_0974BD952BDE__INCLUDED_)
