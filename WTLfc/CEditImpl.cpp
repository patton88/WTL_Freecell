// CEditImpl.cpp
//
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "resource.h"
#include "CEditImpl.h"

CEditImpl::CEditImpl()
{
    m_bHovering = FALSE;
}

CEditImpl::~CEditImpl()
{
}

void CEditImpl::SetTooltipText(LPCTSTR lpszText, BOOL bActivate)
{
    if(NULL == lpszText)
        return;
	
    InitToolTip();
    if (m_toolTip.GetToolCount() == 0)
	{
        CRect rectBtn; 
        GetClientRect(rectBtn);
        m_toolTip.AddTool(*this, lpszText, rectBtn, 1);
    }
    m_toolTip.UpdateTipText(lpszText, *this, 1);
	m_toolTip.SetDelayTime(TTDT_AUTOPOP, 30000);//最大30秒。出现提示前的延迟时间，非必需。缺省是500ms
	m_toolTip.SetDelayTime(TTDT_RESHOW, 1000);	//鼠标停留再1秒弹出提示。解决提示出现又消失、再弹出的问题
							//MSG_WM_SETCURSOR消息响应也可能会导致提示信息闪烁消失，必须去除
    m_toolTip.Activate(bActivate);
}

void CEditImpl::InitToolTip()
{
    if(NULL == m_toolTip.m_hWnd)
	{
        m_toolTip.Create(*this);
        m_toolTip.Activate(FALSE);
        m_toolTip.SetMaxTipWidth(400);
    }
}

LRESULT CEditImpl::OnMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    // track hover msg and leave msg
    if(WM_MOUSEMOVE == uMsg)
	{
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(TRACKMOUSEEVENT);
        tme.dwFlags = TME_HOVER | TME_LEAVE;
        tme.dwHoverTime = 20;
        tme.hwndTrack = m_hWnd;
        _TrackMouseEvent(&tme);
    }
	
    if(m_toolTip.IsWindow())
        m_toolTip.RelayEvent((LPMSG)m_pCurrentMsg);
	
    bHandled = FALSE;
    return FALSE;
}

void CEditImpl::OnMouseHover(WPARAM wParam, CPoint point)
{
    if(!m_bHovering)
	{
        m_bHovering = TRUE;
        //Invalidate();
    }
	SetMsgHandled(FALSE);
}

void CEditImpl::OnMouseLeave()
{
	// 就是这两句：要先判断下是否存在m_toolTip窗口，否则未添加toolTip的button会出错。。。
	if(m_toolTip.IsWindow())
	{
		m_toolTip.Activate(FALSE);
		m_toolTip.Activate(TRUE);
	}
	
	m_bHovering = FALSE;
	//Invalidate();
	SetMsgHandled(FALSE);
}

LRESULT CEditImpl::OnLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetSel(0, -1);	//SetSel(0,-1), 表示全选edit框内容
	//this->SetSel(0, -1);//SetSel(0,-1),表示全选edit框内容
	//::PostMessage(m_hWnd, EM_SETSEL, 0, -1); 
	
	return 0L;
}
