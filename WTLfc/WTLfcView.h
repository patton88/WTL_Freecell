// WTLfcView.h : interface of the CWTLfcView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

//需要增加该行，否则报错：
//g:\myvccode3\wtlfc\wtlfc\wtlfcview.h(33): error C2065: 'IDM_UNDO' : undeclared identifier
//#define IDM_UNDO                        32774
//#define IDM_RAND                        32798
#include "resource.h"

class CWTLfcView : public CWindowImpl<CWTLfcView>
{
public:
	DECLARE_WND_CLASS(NULL)

	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP(CWTLfcView)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
//		MESSAGE_HANDLER(WM_SIZE, OnSize)
		//MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDblClk)
		//MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		//MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		//MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		//MESSAGE_HANDLER(WM_RBUTTONDOWN, OnRButtonDown)
		//MESSAGE_HANDLER(WM_RBUTTONUP, OnRButtonUp)
		MSG_WM_LBUTTONDBLCLK(OnLButtonDblClk)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_RBUTTONDOWN(OnRButtonDown)
		MSG_WM_RBUTTONUP(OnRButtonUp)
		COMMAND_ID_HANDLER(IDM_UNDO, OnUndo)		// 需要在CMainFrame.h中加入CHAIN_COMMANDS_MEMBER(m_view)才行
		COMMAND_ID_HANDLER(IDM_RAND, OnRand)
	END_MSG_MAP()

	CWTLfcView();

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

public:
	CBitmap* replaceColor(CBitmap *pBmpBefore, COLORREF res, COLORREF aim, CClientDC *dc);
	UINT CardLabelHitTest(const CPoint &point);
	void AdjustFrameToFixedSize();
	void HitAt(CPoint point);
	HCURSOR	m_hDown;
	HICON	m_hIcon;
	CBitmap m_AllBmps;
	CBitmap m_AllBmps_Fixed;
	CBrush  m_brushBk;
	CBrush  m_brushBkgnd;
	CRect   m_rectRBDown;
	UINT m_nCardLabelHit;
	void DrawCard(CPoint point, UINT card, CDC *pDC);
	UINT CardColHitTest(const CPoint& point);
	int m_nFirst;
	//bool m_bDisableOnSize;
	bool m_bWndRestored;
	CRect m_rectOrg;

	LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
//	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	BOOL MyCenterWindow(CRect rw);

	//LRESULT OnLButtonDblClk(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//LRESULT OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//LRESULT OnRButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//LRESULT OnRButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	void OnLButtonDblClk(UINT nFlags, CPoint point);
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
	void OnMouseMove(UINT nFlags, CPoint point);
	void OnRButtonDown(UINT nFlags, CPoint point);
	void OnRButtonUp(UINT nFlags, CPoint point);
	LRESULT OnUndo(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnRand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
