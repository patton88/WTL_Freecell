#if !defined(AFX_DLGAISHOW_H__25A0161F_208A_44D5_9481_2F372C3B31F2__INCLUDED_)
#define AFX_DLGAISHOW_H__25A0161F_208A_44D5_9481_2F372C3B31F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAIShow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAIShow dialog

//class CDlgAIShow : public CDialog
//class CDlgAIShow : public CDialogImpl<CDlgAIShow>
class CDlgAIShow : public CDialogImpl<CDlgAIShow>
{
// Construction
public:
	void CancelCurMv(void);
	UINT m_bPlaying;
	UINT m_nElapse;
public:
	void ShowStepInfo(void);
	unsigned m_pCurPos;
	//CDlgAIShow(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAIShow)
	enum { IDD = IDD_AI_SHOW };
	//}}AFX_DATA

	//BEGIN_MESSAGE_MAP(CDlgAIShow, CDialog)
	//	//{{AFX_MSG_MAP(CDlgAIShow)
	//	ON_BN_CLICKED(IDB_NEXT, OnNext)
	//	ON_BN_CLICKED(IDB_PREV, OnPrev)
	//	ON_BN_CLICKED(IDB_FIRST, OnFirst)
	//	ON_BN_CLICKED(IDB_LAST, OnLast)
	//	ON_BN_CLICKED(IDB_AUTO_PLAY, OnAutoPlay)
	//	ON_WM_TIMER()
	//	//}}AFX_MSG_MAP
	//END_MESSAGE_MAP()

	BEGIN_MSG_MAP(CDlgAICal)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		//COMMAND_ID_HANDLER(IDOK, OnOKCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		COMMAND_ID_HANDLER(IDB_NEXT, OnNext)
		COMMAND_ID_HANDLER(IDB_PREV, OnPrev)
		COMMAND_ID_HANDLER(IDB_FIRST, OnFirst)
		COMMAND_ID_HANDLER(IDB_LAST, OnLast)
		COMMAND_ID_HANDLER(IDB_AUTO_PLAY, OnAutoPlay)
		//ON_WM_TIMER()
		MSG_WM_TIMER(OnTimer)
	END_MSG_MAP()

	void OnTimer(UINT uTimerID);
	LRESULT OnNext(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnPrev(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFirst(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnLast(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAutoPlay(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAIShow)
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAIShow)
	//afx_msg void OnNext();
	//afx_msg void OnPrev();
	//virtual BOOL OnInitDialog();
	//afx_msg void OnFirst();
	//afx_msg void OnLast();
	//afx_msg void OnAutoPlay();
	//afx_msg void OnTimer(UINT nIDEvent);
	//virtual void OnCancel();
	////}}AFX_MSG
	//DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGAISHOW_H__25A0161F_208A_44D5_9481_2F372C3B31F2__INCLUDED_)
