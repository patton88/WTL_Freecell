// CEditImpl.h
//
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CEditImpl_H__C84_E69_d49_i41_t05_I29_00007733__INCLUDED_)
#define AFX_CEditImpl_H__C84_E69_d49_i41_t05_I29_00007733__INCLUDED_

#if _MSC_VER >= 1000
	#pragma once
#endif // _MSC_VER >= 1000

// CEditImpl - CWindowImpl-derived class that implements a button.  We need a
// class like this to do subclassing or DDX.
class CEditImpl : public CWindowImpl<CEditImpl, CEdit>
{
public:
    CEditImpl();
    virtual ~CEditImpl();
    
    void SetTooltipText(LPCTSTR lpszText, BOOL bActivate = TRUE);
    
	BEGIN_MSG_MAP(CEditImpl)
		MESSAGE_RANGE_HANDLER(WM_MOUSEFIRST, WM_MOUSELAST, OnMouseMessage)
		MSG_WM_MOUSEHOVER(OnMouseHover)
        MSG_WM_MOUSELEAVE(OnMouseLeave)
		MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDblClk)
	END_MSG_MAP()
	
	LRESULT OnLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		
	LRESULT OnMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void OnMouseHover(WPARAM wParam, CPoint point);
    void OnMouseLeave();

private:
    CToolTipCtrl m_toolTip;
    BOOL m_bHovering;
    void InitToolTip();

};

#endif // !defined(AFX_CEditImpl_H__C84_E69_d49_i41_t05_I29_00007733__INCLUDED_)

//////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
