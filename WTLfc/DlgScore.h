#if !defined(AFX_DLGSCORE_H__C602AC74_2F0C_4988_8304_95D5115AEF42__INCLUDED_)
#define AFX_DLGSCORE_H__C602AC74_2F0C_4988_8304_95D5115AEF42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgScore.h : header file
//

#include "resource.h"
#include "Datatype.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgScore dialog

class CDlgScore : public CDialogImpl<CDlgScore>, public CWinDataExchange<CDlgScore>
{
// Construction
public:
	bool IsOldGameNumber(int gameNum);
	void InitScore();
	void UpdateScore();

	//CObList *m_pOps;
	//list<CMyObject*>* m_pOps;

	//CObList m_score;
	list<CMyObject*> m_score;

	//CDlgScore(CWnd* pParent = NULL);   // standard constructor
	CDlgScore();
	~CDlgScore();
// Dialog Data
	//{{AFX_DATA(CDlgScore)
	enum { IDD = IDD_SCORE };
	CListViewCtrl	m_lcScoreInfo;
	CListViewCtrl	m_lcScore;
	//}}AFX_DATA

	BEGIN_MSG_MAP(CDlgScore)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOKCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CDlgScore)
		//DDV_MinMaxUInt(m_nGameNumber, 1, 2147483647);
		//DDX_INT(IDC_EDIT_WIDTH, m_nWidth)
		//DDX_INT(IDC_EDIT_HEIGHT, m_nHeight)
		//DDX_TEXT(IDC_EDIT_GAMENUMBER, m_strGameNumber)
		//DDX_UINT_RANGE(IDC_EDIT_GAMENUMBER, m_nGameNumber, 1, 2147483647)
		//DDX_Control(pDX, IDLC_SCORE_INFO, m_lcScoreInfo)
		//DDX_Control(pDX, IDLC_SCORE, m_lcScore)
		//DDX_CONTROL(IDLC_SCORE_INFO, m_lcScoreInfo) // error C2039: 'SubclassWindow' : is not a member of 'WTL::CListViewCtrlT<ATL::CWindow>'
		//DDX_CONTROL(IDLC_SCORE, m_lcScore);//error C2039: 'SubclassWindow' : is not a member of 'WTL::CListViewCtrlT<ATL::CWindow>'
		DDX_CONTROL_HANDLE(IDLC_SCORE_INFO, m_lcScoreInfo)	// 编译通过
		DDX_CONTROL_HANDLE(IDLC_SCORE, m_lcScore)			// 编译通过
	END_DDX_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgScore)
	protected:
	//virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgScore)
	//virtual BOOL OnInitDialog();
	//}}AFX_MSG
	//DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSCORE_H__C602AC74_2F0C_4988_8304_95D5115AEF42__INCLUDED_)
