#if !defined(AFX_DGVIEW_H__D14344CB_FAAB_4330_9CC3_CD7FB6999F79__INCLUDED_)
#define AFX_DGVIEW_H__D14344CB_FAAB_4330_9CC3_CD7FB6999F79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DGView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDGWnd view
class CDGWnd : public CWindowImpl<CDGWnd>
	// public CWindowImpl<CDGWnd>
	// public CDialogImpl<CDGWnd>
{
//protected:
public:
	//DECLARE_DYNCREATE(CDGWnd)
	DECLARE_WND_CLASS(NULL)

// Attributes
public:
	CDGWnd();           // protected constructor used by dynamic creation
	virtual ~CDGWnd();

	BEGIN_MSG_MAP(CDGWnd)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_LBUTTONDBLCLK(OnLButtonDblClk)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)

		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOKCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonDblClk(UINT nFlags, CPoint point);
	LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	//ON_WM_LBUTTONDOWN()
	//ON_WM_LBUTTONDBLCLK()
	//ON_WM_ERASEBKGND()
	//ON_WM_PAINT()

// Operations
public:
	CRect RectOfBox();
	void CheckGame();
	bool m_bGameIsValid;
	void SetTitle();
	void Init();
	void NewDefGame();
	void SaveDefGameAs();
	//void ClrOpsRecords(CObList *pList);
	CString m_strFile;
	BOOL GiveUp();
	bool GameIsValid();
	void Shuffle();
	void LoadDefGame();
	void SaveDefGame(void);
	void Set(UINT pos,UINT card);
	UINT Get(UINT pos);
	CRect RectOf(UINT pos);
	UINT HitTest(const CPoint& pt);

	bool m_bModified;
	UINT  m_iSrcCol;
	UCHAR m_iTbl[52];
	UCHAR m_iBox[52];

	int m_nCurGameNumber;
	UINT m_iCards[8][20];//6+13=19
	UINT m_iBuffer[4];
	UINT m_iRecycle[4][14];
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDGWnd)
	//public:
	//protected:
	//virtual void OnPaint();      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
//protected:
//#ifdef _DEBUG
//	//virtual void AssertValid() const;
//	//virtual void Dump(CDumpContext& dc) const;
//#endif

	// Generated message map functions
//protected:
	////{{AFX_MSG(CDGWnd)
	//afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	////}}AFX_MSG
	//DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGVIEW_H__D14344CB_FAAB_4330_9CC3_CD7FB6999F79__INCLUDED_)
