// WTLfcView.cpp : implementation of the CWTLfcView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "WTLfcView.h"
#include "MainFrm.h"
#include "DlgAICal.h"
#include "DlgSelGame.h"

BOOL CWTLfcView::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}

CWTLfcView::CWTLfcView()
{
	g_pView = this;
	//m_bDisableOnSize = false;

	//HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	//SetIcon(hIcon, TRUE);
	m_hDown = AtlLoadCursor(IDCSR_DOWN_ARROW);
	m_hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	m_brushBk.CreateSolidBrush(RGB(255, 255, 255));	//card bk
	m_brushBkgnd.CreateSolidBrush(RGB(0, 128, 0));	//client bk

	//装载位图资源
	//HANDLE bmp = ::LoadImage(pApp->m_hInstance, name, IMAGE_BITMAP, 596, 86, LR_LOADFROMFILE);
	//HANDLE bmp_Fixed = ::LoadImage(pApp->m_hInstance, name, IMAGE_BITMAP, 596, 86, LR_LOADFROMFILE);
	//m_AllBmps.Attach(bmp);
	//m_AllBmps_Fixed.Attach(bmp_Fixed);
	m_AllBmps.LoadBitmapW(IDB_JL_SUB);
	m_AllBmps_Fixed.LoadBitmapW(IDB_JL_SUB);

	// 另一种方式：可用全局函数加载位图
	//m_AllBmps_Fixed = LoadBitmap(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDB_JL_SUB));

	m_nCardLabelHit = 0;
	m_nFirst = 0;
	m_bWndRestored = true;
}

//CJLView::CJLView()
//{
//	// TODO: add construction code here
//	CWinApp *pApp = AfxGetApp();
//	m_hDown = pApp->LoadCursor(IDCSR_DOWN_ARROW);
//	m_hIcon = pApp->LoadIcon(IDR_MAINFRAME);
//	m_brushBk.CreateSolidBrush(RGB(255, 255, 255));//card bk
//	m_brushBkgnd.CreateSolidBrush(RGB(0, 128, 0));//client bk
//	//装载位图资源
//	{
//		//压缩后的位图文件被作为自定义资源，这里先装载资源
//		HRSRC handle = FindResource(NULL, TEXT("IDR_BIN_ZIP_BMP"), TEXT("BIN"));
//		HGLOBAL hGlobal = LoadResource(NULL, handle);
//		BYTE * pByte = (BYTE *)LockResource(hGlobal);
//		DWORD size = SizeofResource(NULL, handle);
//
//		CMemFile srcFile;
//		srcFile.Attach(pByte, size, 0);
//
//		//准备将压缩的位图解码到临时文件
//		wchar_t path[MAX_PATH], name[MAX_PATH];
//		DWORD len = GetTempPath(MAX_PATH, path);
//		if (len == 0 || len >= MAX_PATH || GetTempFileName(path, TEXT("bmp"), 0, name) == 0) {
//			MessageBox(TEXT("无法创建临时文件!"), NULL, MB_ICONERROR | MB_OK);
//		}
//
//		CFile desFile(name, modeCrWr);
//
//		//解码
//		Decoding(desFile, srcFile);
//		srcFile.Detach();
//
//		//装载位图资源之前要保证位图文件已经关闭
//		desFile.Flush();//这里两步是必不可少的步骤！
//		desFile.Close();
//		HANDLE bmp = ::LoadImage(pApp->m_hInstance, name, IMAGE_BITMAP, 596, 86, LR_LOADFROMFILE);
//		HANDLE bmp_Fixed = ::LoadImage(pApp->m_hInstance, name, IMAGE_BITMAP, 596, 86, LR_LOADFROMFILE);
//		m_AllBmps.Attach(bmp);
//		m_AllBmps_Fixed.Attach(bmp_Fixed);
//
//		//删除临时位图文件
//		CFile::Remove(name);
//		m_nFirst = 0;
//	}
//
//	m_nCardLabelHit = 0;
//}


//擦除窗口背景
LRESULT CWTLfcView::OnEraseBkgnd(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	// TODO: Add your message handler code here and/or call default

	CRect r;
	GetClientRect(r);
	//pDC->FillRect(r, &m_brushBkgnd);

	CDCHandle dc((HDC)wParam);
	dc.FillRect(r, m_brushBkgnd);

	bHandled = TRUE;

	return TRUE;
}

LRESULT CWTLfcView::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CPaintDC dc(m_hWnd);

	//TODO: Add your drawing code here

	////// 添加的处理 窗口还原 功能的相关代码
	//static int bFisrt = 0;

	//if (bFisrt < 1 && m_bWndRestored)
	if (1 == m_nFirst && m_bWndRestored)	// 优化代码。省略变量bFisrt
	{
		//bFisrt++;
		m_nFirst++;
		g_pMain->GetWindowRect(m_rectOrg);
		CRect vcr; GetClientRect(vcr);
		g_fcCcs.m_nWinWidth = vcr.Width();
	}

	CRect wr, cr;
	g_pMain->GetWindowRect(wr);
	g_pMain->GetClientRect(cr);
	int ww = wr.Width();
	int cw = cr.Width();

	//g_fcCcs.SetCardSize(wr.Width());	// 在 OnSize消息响应函数中，g_pView->GetClientRect(wr)数值不准确
	g_fcCcs.SetCardSize(cr.Width());		// 放到这里，g_pView->GetClientRect(wr)数值是准确的


	//bool b = g_pMain->IsZoomed();
	//如果当前为最大化。不是由void CJLView::OnWndRestore()触发
	if (g_pMain->IsZoomed() || wr != m_rectOrg)
		//abs(wr.Width() - g_rectOrg.Width()) > 5 || abs(wr.Height() - g_rectOrg.Height()) > 5)
	{
		m_bWndRestored = false;
		//g_pMain->GetMenu()->ModifyMenu(ID_WND_RESTORE, MF_BYCOMMAND, ID_WND_RESTORE, _T("窗口还原(F8)"));
		ModifyMenuW(g_pMain->GetMenu(), ID_WND_RESTORE, MF_BYCOMMAND, ID_WND_RESTORE, _T("窗口还原(F8)"));
		ModifyMenuW(g_pMain->GetMenu(), ID_WND_RESTORE_TOP, MF_BYCOMMAND, ID_WND_RESTORE_TOP, _T("窗口还原(F8)"));
		g_pMain->DrawMenuBar();
	}
	else
	{
		m_bWndRestored = true;
		//g_pMain->GetMenu()->ModifyMenu(ID_WND_RESTORE, MF_BYCOMMAND, ID_WND_RESTORE, _T("窗口最大化(F8)"));
		ModifyMenuW(g_pMain->GetMenu(), ID_WND_RESTORE, MF_BYCOMMAND, ID_WND_RESTORE, _T("窗口最大化(F8)"));
		ModifyMenuW(g_pMain->GetMenu(), ID_WND_RESTORE_TOP, MF_BYCOMMAND, ID_WND_RESTORE_TOP, _T("窗口最大化(F8)"));
		g_pMain->DrawMenuBar();
	}
	////// 添加的处理 窗口还原 功能的相关代码

	//BITMAP bmp;
	//::GetObject(m_AllBmps_Fixed, sizeof(BITMAP), &bmp);
	//SIZE size = { bmp.bmWidth, bmp.bmHeight };

	//CDC dcMem;
	//dcMem.CreateCompatibleDC(dc);
	//HBITMAP hBmpOld = dcMem.SelectBitmap(m_AllBmps_Fixed);
	//dc.BitBlt(0, 0, size.cx, size.cy, dcMem, 0, 0, SRCCOPY);
	//dcMem.SelectBitmap(hBmpOld);

	// 1、显示步数信息
	CString strSteps;
	CRect r = g_fcData.RectOfStep();
	// ...
	if (!g_fcData.m_pOps->empty()) {
		strSteps.Format(TEXT("%d"), g_fcData.m_pOps->size());

		CFont font;
		//font.CreatePointFont(ccs.stepFont * 10, "Arial", pDC);
		font.CreatePointFont(g_fcCcs.stepFont * 6, TEXT("Arial"), dc);
		HFONT prevFont = dc.SelectFont(font);
		int prevMode = dc.SetBkMode(TRANSPARENT);

		dc.SetTextColor(RGB(0, 255, 0));
		r.left -= 3;
		//	int DrawText(LPTSTR lpstrText, int cchText, LPRECT lpRect, UINT uFormat)
		dc.DrawText(strSteps, strSteps.GetLength(), r, DT_CENTER);

		dc.SetBkMode(prevMode);
		dc.SelectFont(prevFont);
	}

	// 2、在步数信息下面显示图标
	CPoint p = r.CenterPoint();
	//p.x -= 16;
	p.x -= 23;
	//p.y += 16;
	//p.y += 30;
	p.y += int(30 * g_fcCcs.dRate);
	dc.DrawIcon(p, m_hIcon);

	// 3、绘制牌面提示按钮(8*12)   A-K
	CDC dcCard;
	dcCard.CreateCompatibleDC(dc);
	//CBitmap *poldbmp = memDC.SelectObject(&m_AllBmps);
	HBITMAP hBmpOld = dcCard.SelectBitmap(m_AllBmps_Fixed);

	//CBrush brush(RGB(0,0,0));
	//CBrush brush = CreateSolidBrush(RGB(0, 255, 0));
	CBrush brush;
	brush.CreateSolidBrush(RGB(0, 255, 0));

	for (UINT c = 1; c <= 13; c++) {
		CRect r = g_fcData.RectOf(c);
		dc.FrameRect(r, brush);
		CPoint p = r.CenterPoint();
		//p.x -= 5; p.y -= 5;
		p.x -= int(2.2 * g_fcCcs.dRate); p.y -= int(4 * g_fcCcs.dRate);
		//pDC->BitBlt(p.x,p.y,8,12,&memDC,(c-1)*8+32,0,SRCAND);
		//pDC->BitBlt(p.x,p.y,8,12,&memDC,(c-1)*8+32,0,NOTSRCCOPY | SRCAND);
		dc.StretchBlt(p.x, p.y, int(5 * g_fcCcs.dRate), int(8 * g_fcCcs.dRate), dcCard, (c - 1) * 8 + 32, 0, 8, 12, NOTSRCCOPY | SRCAND);
	}
	//memDC.SelectObject(poldbmp);
	dcCard.SelectBitmap(hBmpOld);

	// 4、绘制牌局
	p.x = p.y = 0;
	for (UINT i = 1; i <= 16; i++)
	{
		UINT nCards = g_fcData.CntCardsIn(i);
		if (!nCards && i > 8)	//此列没有牌则只绘制方框，只绘制空档列和回收列方框
		{
			//HPEN penBlack = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
			CPen penBlack;
			penBlack.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));

			HPEN penOld = dc.SelectPen(penBlack);

			//pDC->FrameRect(pDoc->RectOf(i,1,1),&brush);
			//CBrush *pPrevBrush = pDC->SelectObject(&m_brushBkgnd);
			CBrush brushOld = dc.SelectBrush(m_brushBkgnd);
			//pDC->RoundRect(pDoc->RectOf(i,1,1),CPoint(0,0));
			dc.Rectangle(g_fcData.RectOf(i, 1, 1));
			dc.SelectBrush(brushOld);
			//pDC->SelectObject(&brush);

			//HPEN penBlack2 = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			CPen penBlack2;
			penBlack2.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

			dc.SelectPen(penBlack2);

			CPoint p;
			CPoint pTopLeft = g_fcData.RectOf(i, 1, 1).TopLeft();
			CPoint BottomRight = g_fcData.RectOf(i, 1, 1).BottomRight();
			p.x = pTopLeft.x;
			p.y = BottomRight.y;

			dc.MoveTo(p);
			dc.LineTo(pTopLeft);

			p.x = BottomRight.x;
			p.y = pTopLeft.y;

			dc.LineTo(p);

			dc.SelectPen(penOld);
		}
		else if (i <= 8)
		{
			for (UINT j = 1; j <= nCards; j++)
			{
				UINT card = g_fcData.GetCard(i, j);
				if (card == 0) continue;		//这里本不该出现牌点数为 0 的情况，都是多线程惹的祸
				r = g_fcData.RectOf(i, j, 1);
				DrawCard(r.TopLeft(), card, &dc);
				if (g_fcData.m_nSel == i && j == nCards)		 //被选中列的底牌需要反色
				{
					dc.InvertRect(r);
				}
			}
		}
		else		//绘制空档列和回收列
		{
			r = g_fcData.RectOf(i, 1, 1);
			DrawCard(r.TopLeft(), g_fcData.BottCard(i), &dc);
			if (g_fcData.m_nSel == i)	 //如果此列是被选中状态，则此列牌被反色
			{
				dc.InvertRect(r);
			}
		}
	}

	return 0;
}

//void CJLView::OnDraw(CDC* pDC)
//{
//	CJLDoc* pDoc = GetDocument();
//	ASSERT_VALID(pDoc);
//
//	//显示步数信息
//	CString strSteps;
//	CRect r = pDoc->RectOfStep();
//	if (!pDoc->m_pOps->IsEmpty()) {
//		strSteps.Format(TEXT("%d"), pDoc->m_pOps->GetCount());
//
//		CFont font;
//		//font.CreatePointFont(ccs.stepFont * 10, "Arial", pDC);
//		font.CreatePointFont(ccs.stepFont * 6, TEXT("Arial"), pDC);
//		CFont * prevFont = pDC->SelectObject(&font);
//		int     prevMode = pDC->SetBkMode(TRANSPARENT);
//
//		pDC->SetTextColor(RGB(0, 255, 0));
//		r.left -= 3;
//		pDC->DrawText(strSteps, r, DT_CENTER);
//
//		pDC->SetBkMode(prevMode);
//		pDC->SelectObject(prevFont);
//	}
//	//在步数信息下面显示图标
//	CPoint p = r.CenterPoint();
//	//p.x -= 16;
//	p.x -= 23;
//	//p.y += 16;
//	//p.y += 30;
//	p.y += int(30 * ccs.dRate);
//	pDC->DrawIcon(p, m_hIcon);
//
//	//绘制牌面提示按钮(8*12)   A-K
//	CDC memDC;
//	memDC.CreateCompatibleDC(pDC);
//	//CBitmap *poldbmp = memDC.SelectObject(&m_AllBmps);
//	CBitmap *poldbmp = memDC.SelectObject(&m_AllBmps_Fixed);
//	//CBrush brush(RGB(0,0,0));
//	CBrush brush(RGB(0, 255, 0));
//	for (UINT c = 1; c <= 13; c++) {
//		CRect r = pDoc->RectOf(c);
//		pDC->FrameRect(r, &brush);
//		CPoint p = r.CenterPoint();
//		//p.x -= 5; p.y -= 5;
//		p.x -= int(2.2 * ccs.dRate); p.y -= int(4 * ccs.dRate);
//		//pDC->BitBlt(p.x,p.y,8,12,&memDC,(c-1)*8+32,0,SRCAND);
//		//pDC->BitBlt(p.x,p.y,8,12,&memDC,(c-1)*8+32,0,NOTSRCCOPY | SRCAND);
//		pDC->StretchBlt(p.x, p.y, int(5 * ccs.dRate), int(8 * ccs.dRate), &memDC, (c - 1) * 8 + 32, 0, 8, 12, NOTSRCCOPY | SRCAND);
//	}
//	memDC.SelectObject(poldbmp);
//
//	//绘制牌局
//	p.x = p.y = 0;
//	for (UINT i = 1; i <= 16; i++) {
//		UINT nCards = pDoc->CntCardsIn(i);
//		if (!nCards && i > 8) { //此列没有牌则只绘制方框，只绘制空档列和回收列方框
//			CPen penBlack(PS_SOLID, 1, RGB(0, 255, 0));
//			CPen *oldPen = pDC->SelectObject(&penBlack);
//
//			//pDC->FrameRect(pDoc->RectOf(i,1,1),&brush);
//			CBrush *pPrevBrush = pDC->SelectObject(&m_brushBkgnd);
//			//pDC->RoundRect(pDoc->RectOf(i,1,1),CPoint(0,0));
//			pDC->Rectangle(pDoc->RectOf(i, 1, 1));
//			pDC->SelectObject(&pPrevBrush);
//			//pDC->SelectObject(&brush);
//
//			CPen penBlack2(PS_SOLID, 1, RGB(0, 0, 0));
//			pDC->SelectObject(&penBlack2);
//
//			CPoint p;
//			CPoint pTopLeft = pDoc->RectOf(i, 1, 1).TopLeft();
//			CPoint BottomRight = pDoc->RectOf(i, 1, 1).BottomRight();
//			p.x = pTopLeft.x;
//			p.y = BottomRight.y;
//
//			pDC->MoveTo(p);
//			pDC->LineTo(pTopLeft);
//
//			p.x = BottomRight.x;
//			p.y = pTopLeft.y;
//
//			pDC->LineTo(p);
//
//			pDC->SelectObject(oldPen);
//		}
//		else if (i <= 8) {
//			for (UINT j = 1; j <= nCards; j++) {
//				UINT card = pDoc->GetCard(i, j);
//				if (card == 0) continue; //这里本不该出现牌点数为0 的情况，都是多线程惹的祸
//				r = pDoc->RectOf(i, j, 1);
//				DrawCard(r.TopLeft(), card, pDC);
//				if (pDoc->m_nSel == i && j == nCards) {
//					pDC->InvertRect(r); //被选中列的底牌需要反色
//				}
//			}
//		}
//		else { //绘制空档列和回收列
//			r = pDoc->RectOf(i, 1, 1);
//			DrawCard(r.TopLeft(), pDoc->BottCard(i), pDC);
//			if (pDoc->m_nSel == i) { //如果此列是被选中状态，则此列牌被反色
//				pDC->InvertRect(r);
//			}
//		}
//	}
//}

//调整窗口到合适的大小
void CWTLfcView::AdjustFrameToFixedSize()
{
	// 解决非缺省大小窗口最大化后窗口还原的问题
	// 这两句必须放在pWnd->MoveWindow(wr);之前，不然会有问题：
	//   当窗口不是缺省大小位置->最大化->窗口还原，就会是系统的恢复窗口大小，而不能将 窗口还原 到缺省大小位置
	//if (g_pMain->IsZoomed())
	//	g_pMain->SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);

	//要求不管在什么时候窗口要正好能显示所有的牌
	CRect wr, cr;

	//if (m_bWndRestored && m_rectOrg.Width() > 0)
	if (m_bWndRestored && m_nFirst > 0)		// 已完成初始化处理，缺省窗口大小已经保存
	{
		wr = m_rectOrg;
	}
	else
	{
		GetClientRect(cr);
		g_pMain->GetWindowRect(wr);

		// 窗口左右两个边的宽度和上下边框的高度
		int xEdge = (wr.Width() - cr.Width()) / 2 + 1;
		int yEdge = (wr.Height() - cr.Height()) / 2;

		CRect r = g_fcData.RectOf(8, 1, 1);//第八列第一张
		ClientToScreen(r);

		//GetSystemMetrics(SM_CXFRAME)
		//GetSystemMetrics(SM_CYFRAME)
		//wr.left = wr.top = 0;
		wr.right = r.right + g_fcCcs.CARD_INT + g_fcCcs.ptOrg.x + xEdge;
		wr.bottom = r.top + g_fcCcs.CARD_UNCOVER * 11 + g_fcCcs.CARD_HEI + xEdge;

		//pWnd->MoveWindow(wr);
		//pWnd->CenterWindow();
	}

	MyCenterWindow(wr);		// 避免窗口闪烁

	if (0 == m_nFirst)
		m_nFirst = 1;
}

BOOL CWTLfcView::MyCenterWindow(CRect rcDlg)
{
	CRect rcArea;
	CRect rcScreen;

	rcScreen.left = rcScreen.top = 0;
	rcScreen.right = GetSystemMetrics(SM_CXSCREEN);
	rcScreen.bottom = GetSystemMetrics(SM_CYSCREEN);

	::SystemParametersInfo(SPI_GETWORKAREA, NULL, (PVOID)&rcArea, 0);	//获取桌面工作区大小

	int xLeft = (rcScreen.left + rcScreen.right) / 2 - rcDlg.Width() / 2;
	int yTop = (rcScreen.top + rcScreen.bottom) / 2 - rcDlg.Height() / 2;

	// if the dialog is outside the screen, move it inside
	if (xLeft < rcArea.left)		xLeft = rcArea.left;
	if (yTop < rcArea.top)	yTop = rcArea.top;

	CRect rcRet(xLeft, yTop, xLeft + rcDlg.Width(), yTop + rcDlg.Height());

	if (rcRet.Width() > rcArea.Width())
		rcRet.right = rcArea.right;
	if (rcRet.Height() > rcArea.Height())
		rcRet.bottom = rcArea.bottom;

	// 解决非缺省大小窗口最大化后窗口还原的问题
	// 这两句必须放在pWnd->MoveWindow(wr);、SetWindowPos等之前，不然会有问题：
	//   当窗口不是缺省大小位置->最大化->窗口还原，就会是系统的恢复窗口大小，而不能将 窗口还原 到缺省大小位置
	if (g_pMain->IsZoomed())
	{
		//g_pMain->SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
		// 这样可以进一步减少窗口大小改变时的闪烁
		long lWnd = ::GetWindowLong(g_pMain->m_hWnd, GWL_STYLE);
		//lWnd = lWnd & ~WS_MAXIMIZEBOX;		//lWnd = lWnd & ~WS_MINIMIZEBOX;
		lWnd = lWnd & ~(WS_MAXIMIZE | WS_MINIMIZE);
		::SetWindowLong(g_pMain->m_hWnd, GWL_STYLE, lWnd);
	}

	return g_pMain->SetWindowPos(NULL, xLeft, yTop,
		rcRet.Width(), rcRet.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
}

//调整窗口到合适的大小
//void CWTLfcView::AdjustFrameToFixedSize()
//{
//	//CenterWindow(GetParent());		// WTL 中这样无效
//	//GetParent().CenterWindow();		// WTL 中这样有效
//
//	//要求不管在什么时候窗口要正好能显示所有的牌
//	//HWND hwnd = GetParent();
//	CRect wr, cr;
//	//g_pView->GetClientRect(cr);		// 得到View窗口客户区矩形
//	//g_pView->GetWindowRect(wr);		// 得到View窗口的矩形
//	//::GetClientRect(m_hWnd, cr);		// 得到View窗口客户区矩形。这种方式从类外调用时，会有问题
//	//::GetWindowRect(m_hWnd, wr);		// 得到View窗口的矩形。这种方式从类外调用时，会有问题
//
//	//g_pMain->GetWindowRect(wr);		// 得到CMainFrame窗口的矩形
//	//g_pMain->GetClientRect(cr);		// 得到CMainFrame窗口客户区矩形
//
//	// 窗口左右两个边的宽度和上下边框的高度
//	//int edge = (wr.Width() - cr.Width()) / 2 + 1;
//
//	CRect r = g_fcData.RectOf(8, 1, 1);//得到第八列第一张的客户区矩形。第八列也即最后、最右面一列
//	//ClientToScreen(r);		// 这种方式从类外调用时，会有问题
//	//g_pMain->ClientToScreen(r);	// 这种方式是错误的
//	//g_pView->ClientToScreen(r);		// 这种方式才是正确的
//
//	// g_fcCcs.CARD_INT 是两列牌之间的间隔，g_fcCcs.ptOrg.x 是左右两边牌列离客户区边框的距离
//	//wr.right = r.right + g_fcCcs.CARD_INT + g_fcCcs.ptOrg.x + edge;
//	//wr.bottom = r.top + g_fcCcs.CARD_UNCOVER*12 + g_fcCcs.CARD_HEI + edge;
//	//wr.bottom = r.top + g_fcCcs.CARD_UNCOVER * 11 + g_fcCcs.CARD_HEI + edge;
//	wr.top = wr.left = 0;
//	wr.right = r.right + g_fcCcs.CARD_INT + g_fcCcs.ptOrg.x + 2 * (GetSystemMetrics(SM_CXFRAME)) + 1;
//	wr.bottom = r.top + g_fcCcs.CARD_UNCOVER * 13 + g_fcCcs.CARD_HEI + 2 * (GetSystemMetrics(SM_CYFRAME)) + 1;
//
//	//CRect rcArea;
//	//rcArea.left = rcArea.top = 0;
//	//rcArea.right = GetSystemMetrics(SM_CXSCREEN);
//	//rcArea.bottom = GetSystemMetrics(SM_CYSCREEN);
//
//	//int DlgWidth = wr.right - wr.left;
//	//int DlgHeight = wr.bottom - wr.top;
//
//	//// find dialog's upper left based on rcCenter
//	//int xLeft = (rcArea.left + rcArea.right) / 2 - DlgWidth / 2;
//	//int yTop = (rcArea.top + rcArea.bottom) / 2 - DlgHeight / 2;
//
//	//wr.Height();
//	//::MoveWindow(hwnd, wr.left, wr.top, wr.Width(), wr.Height(), TRUE);
//	//::MoveWindow(hwnd, xLeft, yTop, wr.Width(), wr.Height(), TRUE);
//	//GetParent().CenterWindow();		// WTL 中这样有效
//	//GetParent().UpdateWindow();		// WTL 中这样有效
//	MyCenterWindow(wr);
//	//g_pMain->SetWindowPos(NULL, wr.left, wr.top, wr.Width(), wr.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
//	//if (g_pMain->IsZoomed())
//	//	g_pMain->PostMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
//	//g_pMain->CenterWindow();
//
//	//if (0 == m_nFirst)
//	//	m_nFirst = 1;
//}

////调整窗口到合适的大小
//void CJLView::AdjustFrameToFixedSize()
//{
//	CJLDoc * pDoc = GetDocument();
//
//	//要求不管在什么时候窗口要正好能显示所有的牌
//	CRect wr, cr;
//	GetClientRect(cr);
//	CWnd *pWnd = GetParent();
//	pWnd->GetWindowRect(wr);
//
//	// 窗口左右两个边的宽度和上下边框的高度
//	int edge = (wr.Width() - cr.Width()) / 2 + 1;
//
//	CRect r = pDoc->RectOf(8, 1, 1);//第八列第一张
//	ClientToScreen(r);
//
//	wr.right = r.right + ccs.CARD_INT + ccs.ptOrg.x + edge;
//	//wr.bottom = r.top + ccs.CARD_UNCOVER*12 + ccs.CARD_HEI + edge;
//	wr.bottom = r.top + ccs.CARD_UNCOVER * 11 + ccs.CARD_HEI + edge;
//
//	//wr.Height();
//	pWnd->MoveWindow(wr);
//	pWnd->CenterWindow();
//
//	if (0 == m_nFirst)
//		m_nFirst = 1;
//}


LRESULT CWTLfcView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// TODO: Add your message handler code here and/or call default

	//如果没有文档打开就自动设置牌局
	//if (g_fcData.GetPathName().IsEmpty())
	{ g_fcData.OnRand(); }

	//AdjustFrameToFixedSize();	//调整窗口到合适的大小

	//OK
	CClientDC dc(m_hWnd);
	//CClientDC pDC(this);
	//CDC *pDC = GetDC();
	//replaceColor(&m_AllBmps_Fixed, RGB(0,0,0), RGB(0,255,0), &pDC);
	replaceColor(&m_AllBmps_Fixed, RGB(0, 0, 0), RGB(255, 0, 255), &dc);

	return 0;
}


//颜色替换函数
//输入:待替换的图像指针,替换前颜色,替换后的颜色,CDC容器
//输出:替换后的图像指针
CBitmap* CWTLfcView::replaceColor(CBitmap *pBmpBefore, COLORREF res, COLORREF aim, CClientDC *dc)
{
	CDC dcMask, dcBefore;
	CBitmap bmpMask;
	BITMAP bitmap;
	pBmpBefore->GetBitmap(&bitmap);
	CRect rect(0, 0, bitmap.bmWidth, bitmap.bmHeight);
	//创建适配的遮罩层图像,设备,并装入
	bmpMask.CreateBitmap(bitmap.bmWidth, bitmap.bmHeight, 1, 1, NULL);
	dcMask.CreateCompatibleDC(*dc);
	dcBefore.CreateCompatibleDC(*dc);
	dcMask.SelectBitmap(bmpMask);
	dcBefore.SelectBitmap(pBmpBefore->m_hBitmap);
	//将替换前颜色设置为黑色
	dcBefore.SetBkColor(res);
	dcMask.BitBlt(0, 0, bitmap.bmWidth, bitmap.bmHeight, dcBefore, 0, 0, SRCCOPY);
	dcBefore.SetBkColor(RGB(0, 0, 0));
	dcBefore.SetTextColor(RGB(255, 255, 255));
	dcBefore.BitBlt(0, 0, bitmap.bmWidth, bitmap.bmHeight, dcMask, 0, 0, SRCAND);
	//将替换前颜色替换为目标颜色
	dcBefore.SetBkColor(aim);
	dcBefore.SetTextColor(RGB(0, 0, 0));
	dcBefore.BitBlt(0, 0, bitmap.bmWidth, bitmap.bmHeight, dcMask, 0, 0, SRCPAINT);
	return	pBmpBefore;
}

//颜色替换函数
//输入:待替换的图像指针,替换前颜色,替换后的颜色,CDC容器
//输出:替换后的图像指针
//CBitmap* CJLView::replaceColor(CBitmap *b_bmp, COLORREF res, COLORREF aim, CDC *cdc)
//{
//	CDC m_cdc, b_cdc;
//	CBitmap m_bmp;
//	BITMAP bitmap;
//	b_bmp->GetBitmap(&bitmap);
//	CRect rect(0, 0, bitmap.bmWidth, bitmap.bmHeight);
//	//创建适配的遮罩层图像,设备,并装入
//	m_bmp.CreateBitmap(bitmap.bmWidth, bitmap.bmHeight, 1, 1, NULL);
//	m_cdc.CreateCompatibleDC(cdc);
//	b_cdc.CreateCompatibleDC(cdc);
//	m_cdc.SelectObject(&m_bmp);
//	b_cdc.SelectObject(b_bmp);
//	//将替换前颜色设置为黑色
//	b_cdc.SetBkColor(res);
//	m_cdc.BitBlt(0, 0, bitmap.bmWidth, bitmap.bmHeight, &b_cdc, 0, 0, SRCCOPY);
//	b_cdc.SetBkColor(RGB(0, 0, 0));
//	b_cdc.SetTextColor(RGB(255, 255, 255));
//	b_cdc.BitBlt(0, 0, bitmap.bmWidth, bitmap.bmHeight, &m_cdc, 0, 0, SRCAND);
//	//将替换前颜色替换为目标颜色
//	b_cdc.SetBkColor(aim);
//	b_cdc.SetTextColor(RGB(0, 0, 0));
//	b_cdc.BitBlt(0, 0, bitmap.bmWidth, bitmap.bmHeight, &m_cdc, 0, 0, SRCPAINT);
//	return	b_bmp;
//}


//draw all things in memory dc!!!!!!!!!!!!!
//所有的绘牌动作必须都在内存中进行否则会造成闪动
void CWTLfcView::DrawCard(CPoint point, UINT card, CDC *pDC)
{
	//CJLDoc* pDoc = GetDocument();
	ATLASSERT(g_fcData.IsCard(card));

	//HPEN penBlack = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen penBlack;
	penBlack.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

	HPEN penOld = pDC->SelectPen(penBlack);
	CBrush brushOld = pDC->SelectBrush(m_brushBk);
	CRect r(point, CSize(g_fcCcs.CARD_WID, g_fcCcs.CARD_HEI));
	pDC->RoundRect(r, CPoint(5, 5));//画牌的背景与边框
	pDC->SelectPen(penOld);
	pDC->SelectBrush(brushOld);

	CDC memDC;
	memDC.CreateCompatibleDC(*pDC);
	HBITMAP bmpOld = memDC.SelectBitmap(m_AllBmps);
	//画左上角 & 右下角数字(8*12)和数字下/上面的图标(8*8)
	int type = TYPE(card), num = NUM(card);
	int idxBS = type % 2 * 26, idxUL = idxBS + num - 1, idxBR = idxBS + 26 - num;
	/*
	pDC->BitBlt(r.left+3    , r.top+3       , 8 , 12 , &memDC , idxUL*8+32 , 0 , SRCAND);
	pDC->BitBlt(r.right-3-8 , r.bottom-3-12 , 8 , 12 , &memDC , idxBR*8+32 , 0 , SRCAND);
	pDC->BitBlt(r.left+3    , r.top+3+12+1,       8, 8, &memDC , ccs.clr[ccs.SML][type][0][0], ccs.clr[ccs.SML][type][0][1] , SRCAND);
	pDC->BitBlt(r.right-3-8 , r.bottom-3-12-1-8 , 8, 8, &memDC , ccs.clr[ccs.SML][type][1][0], ccs.clr[ccs.SML][type][1][1] , SRCAND);
	*/

	//基本思路和模式，是将调用StretchBlt函数时，前4个参数末尾的数字 * ccs.dRate。一般每个参数只最末数字乘1次
	//将运算结果强制转int，以免警告。warning C4244: 'argument' : conversion from 'float' to 'int', possible loss of data
	int nEdg = int(4.5*g_fcCcs.dRate);
	pDC->StretchBlt(r.left + nEdg, r.top + nEdg, int(8.8*g_fcCcs.dRate), int(13.2*g_fcCcs.dRate), memDC, idxUL * 8 + 32, 0, 8, 12, SRCAND);
	pDC->StretchBlt(int(r.right - nEdg - 8.5*g_fcCcs.dRate), int(r.bottom - nEdg - 13 * g_fcCcs.dRate), int(8.8*g_fcCcs.dRate), int(13.2*g_fcCcs.dRate), memDC, idxBR * 8 + 32, 0, 8, 12, SRCAND);
	//下面两行作了特殊处理，是画左上角 & 右下角图标的(8*8)的
	pDC->StretchBlt(int(r.left + nEdg + 1.5*g_fcCcs.dRate), int(r.top + nEdg + 15 * g_fcCcs.dRate + 1 * g_fcCcs.dRate), int(8 * g_fcCcs.dRate), int(8 * g_fcCcs.dRate), memDC, g_fcCcs.clr[g_fcCcs.SML][type][0][0], g_fcCcs.clr[g_fcCcs.SML][type][0][1], 8, 8, SRCAND);
	pDC->StretchBlt(int(r.right - nEdg - 8.5*g_fcCcs.dRate), int(r.bottom - nEdg - 12 * g_fcCcs.dRate - 1 * g_fcCcs.dRate - 10.5*g_fcCcs.dRate), int(8 * g_fcCcs.dRate), int(8 * g_fcCcs.dRate), memDC, g_fcCcs.clr[g_fcCcs.SML][type][1][0], g_fcCcs.clr[g_fcCcs.SML][type][1][1], 8, 8, SRCAND);

	//在牌的主要部分画花色
	const UCHAR *p, *data[] = { g_fcCcs.cA, g_fcCcs.c2, g_fcCcs.c3, g_fcCcs.c4, g_fcCcs.c5, g_fcCcs.c6, g_fcCcs.c7, g_fcCcs.c8, g_fcCcs.c9, g_fcCcs.c10 };
	if (num <= 10) {
		p = (card == CARD(8, 3) ? g_fcCcs.c8FK : data[num - 1]);
		UINT n = num * 3;
		for (UINT j = 0; j < n; j += 3) {
			//pDC->BitBlt(point.x + p[j] - 8, point.y + p[j+1] - 8, 16, 16, &memDC, clr[BIG][type][!p[j+2]][0], clr[BIG][type][!p[j+2]][1], SRCAND);
			//原来时原始大小绘制牌片，不用缩放，所以用BitBlt，参数为8个。注意这里用的是MFC的CDC成员函数BitBlt，而非平台SDK API的BitBlt(参数为9个，多第一个HDC)
			//这里需要进行缩放，所以用StretchBlt，参数为10个。注意这里用的是MFC的CDC成员函数StretchBlt，而非平台SDK API的StretchBlt(参数为11个，多第一个HDC)
			pDC->StretchBlt(int(point.x + p[j] - 5.5*g_fcCcs.dRate), int(point.y + p[j + 1] - 7 * g_fcCcs.dRate), int(12 * g_fcCcs.dRate), int(16 * g_fcCcs.dRate), memDC,
				g_fcCcs.clr[g_fcCcs.BIG][type][!p[j + 2]][0], g_fcCcs.clr[g_fcCcs.BIG][type][!p[j + 2]][1], 16, 16, SRCAND);

			//pDC->StretchBlt(int(point.x + p[j] - 12*g_fcCcs.fRate), int(point.y + p[j+1] - 16*g_fcCcs.fRate), int(26*g_fcCcs.fRate), int(32*g_fcCcs.fRate), &memDC, 
			//	ccs.clr[ccs.BIG][type][!p[j+2]][0], ccs.clr[ccs.BIG][type][!p[j+2]][1], 16, 16, SRCAND);


		}
	}
	else { //画J Q K
		CPoint p = r.CenterPoint();
		//pDC->BitBlt(p.x-23, p.y-37, 47, 74, &memDC, ( (num-11)*4 + type )*47 + 32, 12, SRCAND);
		pDC->StretchBlt(int(p.x - 20 * g_fcCcs.dRate), int(p.y - 33 * g_fcCcs.dRate), int(41 * g_fcCcs.dRate), int(67 * g_fcCcs.dRate), memDC, ((num - 11) * 4 + type) * 47 + 32, 12, 47, 74, SRCAND);

	}
	memDC.SelectBitmap(bmpOld);
}

//draw all things in memory dc!!!!!!!!!!!!!
//所有的绘牌动作必须都在内存中进行否则会造成闪动
//void CJLView::DrawCard(CPoint point, UINT card, CDC *pDC)
//{
//	CJLDoc* pDoc = GetDocument();
//	ASSERT(pDoc->IsCard(card));
//
//	CPen penBlack(PS_SOLID, 1, RGB(0, 0, 0));
//	CPen *oldPen = pDC->SelectObject(&penBlack);
//	CBrush *oldBrush = pDC->SelectObject(&m_brushBk);
//	CRect r(point, CSize(ccs.CARD_WID, ccs.CARD_HEI));
//	pDC->RoundRect(r, CPoint(5, 5));//画牌的背景与边框
//	pDC->SelectObject(oldPen);
//	pDC->SelectObject(oldBrush);
//	CDC memDC;
//	memDC.CreateCompatibleDC(pDC);
//	CBitmap *poldbmp = memDC.SelectObject(&m_AllBmps);
//	//画左上角 & 右下角数字(8*12)和数字下/上面的图标(8*8)
//	int type = TYPE(card), num = NUM(card);
//	int idxBS = type % 2 * 26, idxUL = idxBS + num - 1, idxBR = idxBS + 26 - num;
//	/*
//	pDC->BitBlt(r.left+3    , r.top+3       , 8 , 12 , &memDC , idxUL*8+32 , 0 , SRCAND);
//	pDC->BitBlt(r.right-3-8 , r.bottom-3-12 , 8 , 12 , &memDC , idxBR*8+32 , 0 , SRCAND);
//	pDC->BitBlt(r.left+3    , r.top+3+12+1,       8, 8, &memDC , ccs.clr[ccs.SML][type][0][0], ccs.clr[ccs.SML][type][0][1] , SRCAND);
//	pDC->BitBlt(r.right-3-8 , r.bottom-3-12-1-8 , 8, 8, &memDC , ccs.clr[ccs.SML][type][1][0], ccs.clr[ccs.SML][type][1][1] , SRCAND);
//	*/
//
//	//基本思路和模式，是将调用StretchBlt函数时，前4个参数末尾的数字 * ccs.dRate。一般每个参数只最末数字乘1次
//	//将运算结果强制转int，以免警告。warning C4244: 'argument' : conversion from 'float' to 'int', possible loss of data
//	int nEdg = int(4.5*ccs.dRate);
//	pDC->StretchBlt(r.left + nEdg, r.top + nEdg, int(8.8*ccs.dRate), int(13.2*ccs.dRate), &memDC, idxUL * 8 + 32, 0, 8, 12, SRCAND);
//	pDC->StretchBlt(int(r.right - nEdg - 8.5*ccs.dRate), int(r.bottom - nEdg - 13 * ccs.dRate), int(8.8*ccs.dRate), int(13.2*ccs.dRate), &memDC, idxBR * 8 + 32, 0, 8, 12, SRCAND);
//	//下面两行作了特殊处理，是画左上角 & 右下角图标的(8*8)的
//	pDC->StretchBlt(int(r.left + nEdg + 1.5*ccs.dRate), int(r.top + nEdg + 15 * ccs.dRate + 1 * ccs.dRate), int(8 * ccs.dRate), int(8 * ccs.dRate), &memDC, ccs.clr[ccs.SML][type][0][0], ccs.clr[ccs.SML][type][0][1], 8, 8, SRCAND);
//	pDC->StretchBlt(int(r.right - nEdg - 8.5*ccs.dRate), int(r.bottom - nEdg - 12 * ccs.dRate - 1 * ccs.dRate - 10.5*ccs.dRate), int(8 * ccs.dRate), int(8 * ccs.dRate), &memDC, ccs.clr[ccs.SML][type][1][0], ccs.clr[ccs.SML][type][1][1], 8, 8, SRCAND);
//
//	//在牌的主要部分画花色
//	const UCHAR *p, *data[] = { ccs.cA, ccs.c2, ccs.c3, ccs.c4, ccs.c5, ccs.c6, ccs.c7, ccs.c8, ccs.c9, ccs.c10 };
//	if (num <= 10) {
//		p = (card == CARD(8, 3) ? ccs.c8FK : data[num - 1]);
//		UINT n = num * 3;
//		for (UINT j = 0; j < n; j += 3) {
//			//pDC->BitBlt(point.x + p[j] - 8, point.y + p[j+1] - 8, 16, 16, &memDC, clr[BIG][type][!p[j+2]][0], clr[BIG][type][!p[j+2]][1], SRCAND);
//			//原来时原始大小绘制牌片，不用缩放，所以用BitBlt，参数为8个。注意这里用的是MFC的CDC成员函数BitBlt，而非平台SDK API的BitBlt(参数为9个，多第一个HDC)
//			//这里需要进行缩放，所以用StretchBlt，参数为10个。注意这里用的是MFC的CDC成员函数StretchBlt，而非平台SDK API的StretchBlt(参数为11个，多第一个HDC)
//			pDC->StretchBlt(int(point.x + p[j] - 5.5*ccs.dRate), int(point.y + p[j + 1] - 7 * ccs.dRate), int(12 * ccs.dRate), int(16 * ccs.dRate), &memDC,
//				ccs.clr[ccs.BIG][type][!p[j + 2]][0], ccs.clr[ccs.BIG][type][!p[j + 2]][1], 16, 16, SRCAND);
//
//			//pDC->StretchBlt(int(point.x + p[j] - 12*ccs.fRate), int(point.y + p[j+1] - 16*ccs.fRate), int(26*ccs.fRate), int(32*ccs.fRate), &memDC, 
//			//	ccs.clr[ccs.BIG][type][!p[j+2]][0], ccs.clr[ccs.BIG][type][!p[j+2]][1], 16, 16, SRCAND);
//
//
//		}
//	}
//	else { //画J Q K
//		CPoint p = r.CenterPoint();
//		//pDC->BitBlt(p.x-23, p.y-37, 47, 74, &memDC, ( (num-11)*4 + type )*47 + 32, 12, SRCAND);
//		pDC->StretchBlt(int(p.x - 20 * ccs.dRate), int(p.y - 33 * ccs.dRate), int(41 * ccs.dRate), int(67 * ccs.dRate), &memDC, ((num - 11) * 4 + type) * 47 + 32, 12, 47, 74, SRCAND);
//
//	}
//	memDC.SelectObject(poldbmp);
//}


//LRESULT CWTLfcView::OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
//{
//	// TODO: Add your message handler code here and/or call default
//
//	bHandled = FALSE;
//	//if (0 == m_nFirst)
//	//	return 0;
//
//	////if (m_bDisableOnSize)
//	////	return 0;
//
//	//CRect wr;
//	////GetClientRect(wr);	// 得到View窗口客户区矩形
//	////GetWindowRect(wr);	// 得到View窗口矩形
//
//	//g_pView->GetClientRect(wr);		// 得到View窗口客户区矩形。这样当窗口被部分遮挡时，会出现错位现象
//	////g_pMain->GetWindowRect(wr);		// 得到CMainFrame窗口客户区矩形。这样当窗口被部分遮挡时，不会出现错位现象
//
//	////g_pView->GetWindowRect(wr);		// 得到View窗口的矩形
//	////::GetClientRect(m_hWnd, wr);		// 得到View窗口客户区矩形。这种方式从类外调用时，会有问题
//	////::GetWindowRect(m_hWnd, wr);		// 得到View窗口的矩形。这种方式从类外调用时，会有问题
//
//	//if (wParam != SIZE_MAXIMIZED)	//如果事件为最大化
//	//{
//	//	//ccs.fRate = (double)cr.Width() / (double)g_nWindowWdith;
//	//	//ShowWindow(SW_HIDE);//隐藏主窗口 
//	//	//Shell_NotifyIcon(NIM_ADD,&m_nid);//在托盘区添加图标
//	//}
//
//	////if (wr.Width() > 1195 || wr.Width() < 1185)		// 为避免计算误差，设置取值范围
//	//g_fcCcs.SetCardSize(wr.Width());	// 在 OnSize消息响应函数中，g_pView->GetClientRect(wr)数值不准确
//	////AdjustFrameToFixedSize();
//
//	return 0;
//}

//BOOL CWTLfcView::MyCenterWindow(CRect rcDlg)
//{
//	//int with = GetSystemMetrics(SM_CXFULLSCREEN);
//	//int heigh = GetSystemMetrics(SM_CYFULLSCREEN);
//	//通过上边两个函数获取的是显示屏幕的大小，及不包括任务栏等区域。
//
//	//int   cx = GetSystemMetrics(SM_CXSCREEN);
//	//int   cy = GetSystemMetrics(SM_CYSCREEN);
//	//这两个函数获取的是真正屏幕的大小。
//
//	CRect rcArea;
//	CRect rcScreen;
//
//	rcScreen.left = rcScreen.top = 0;
//	rcScreen.right = GetSystemMetrics(SM_CXSCREEN);
//	rcScreen.bottom = GetSystemMetrics(SM_CYSCREEN);
//
//	::SystemParametersInfo(SPI_GETWORKAREA, NULL, (PVOID)&rcArea, 0);	//获取桌面工作区大小
//
//	//int DlgWidth = rcDlg.right - rcDlg.left;
//	//int DlgHeight = rcDlg.bottom - rcDlg.top;
//
//	// find dialog's upper left based on rcCenter
//	int xLeft = (rcScreen.left + rcScreen.right) / 2 - rcDlg.Width() / 2;
//	int yTop = (rcScreen.top + rcScreen.bottom) / 2 - rcDlg.Height() / 2;
//
//	// if the dialog is outside the screen, move it inside
//	if (xLeft < rcArea.left)		xLeft = rcArea.left;
//	if (yTop < rcArea.top)	yTop = rcArea.top;
//
//	//CRect rcRet(xLeft, yTop, rcDlg.right, rcDlg.bottom);
//	CRect rcRet(xLeft, yTop, xLeft + rcDlg.Width(), yTop + rcDlg.Height());
//
//	if (rcRet.Width() > rcArea.Width())
//		rcRet.right = rcArea.right;
//	if (rcRet.Height() > rcArea.Height())
//		rcRet.bottom = rcArea.bottom;
//
//	// map screen coordinates to child coordinates
//	//return ::SetWindowPos(GetParent(), NULL, xLeft, yTop, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
//	if (g_pMain->IsZoomed())
//		g_pMain->SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
//	//g_pMain->PostMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
//
//	return g_pMain->SetWindowPos(NULL, xLeft, yTop,
//		rcRet.Width(), rcRet.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
//}

// atlwin.h of WTL91_5321_Final
//BOOL CenterWindow(_Inout_opt_ HWND hWndCenter = NULL) throw()
//{
//	ATLASSERT(::IsWindow(m_hWnd));
//
//	// determine owner window to center against
//	DWORD dwStyle = GetStyle();
//	if (hWndCenter == NULL)
//	{
//		if (dwStyle & WS_CHILD)
//			hWndCenter = ::GetParent(m_hWnd);
//		else
//			hWndCenter = ::GetWindow(m_hWnd, GW_OWNER);
//	}
//
//	// get coordinates of the window relative to its parent
//	RECT rcDlg;
//	::GetWindowRect(m_hWnd, &rcDlg);
//	RECT rcArea;
//	RECT rcCenter;
//	HWND hWndParent;
//	if (!(dwStyle & WS_CHILD))
//	{
//		// don't center against invisible or minimized windows
//		if (hWndCenter != NULL)
//		{
//			DWORD dwStyleCenter = ::GetWindowLong(hWndCenter, GWL_STYLE);
//			if (!(dwStyleCenter & WS_VISIBLE) || (dwStyleCenter & WS_MINIMIZE))
//				hWndCenter = NULL;
//		}
//
//		// center within screen coordinates
//		HMONITOR hMonitor = NULL;
//		if (hWndCenter != NULL)
//		{
//			hMonitor = ::MonitorFromWindow(hWndCenter, MONITOR_DEFAULTTONEAREST);
//		}
//		else
//		{
//			hMonitor = ::MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST);
//		}
//		ATLENSURE_RETURN_VAL(hMonitor != NULL, FALSE);
//
//		MONITORINFO minfo;
//		minfo.cbSize = sizeof(MONITORINFO);
//		BOOL bResult = ::GetMonitorInfo(hMonitor, &minfo);
//		ATLENSURE_RETURN_VAL(bResult, FALSE);
//
//		rcArea = minfo.rcWork;
//
//		if (hWndCenter == NULL)
//			rcCenter = rcArea;
//		else
//			::GetWindowRect(hWndCenter, &rcCenter);
//	}
//	else
//	{
//		// center within parent client coordinates
//		hWndParent = ::GetParent(m_hWnd);
//		ATLASSERT(::IsWindow(hWndParent));
//
//		::GetClientRect(hWndParent, &rcArea);
//		ATLASSERT(::IsWindow(hWndCenter));
//		::GetClientRect(hWndCenter, &rcCenter);
//		::MapWindowPoints(hWndCenter, hWndParent, (POINT*)&rcCenter, 2);
//	}
//
//	int DlgWidth = rcDlg.right - rcDlg.left;
//	int DlgHeight = rcDlg.bottom - rcDlg.top;
//
//	// find dialog's upper left based on rcCenter
//	int xLeft = (rcCenter.left + rcCenter.right) / 2 - DlgWidth / 2;
//	int yTop = (rcCenter.top + rcCenter.bottom) / 2 - DlgHeight / 2;
//
//	// if the dialog is outside the screen, move it inside
//	if (xLeft + DlgWidth > rcArea.right)
//		xLeft = rcArea.right - DlgWidth;
//	if (xLeft < rcArea.left)
//		xLeft = rcArea.left;
//
//	if (yTop + DlgHeight > rcArea.bottom)
//		yTop = rcArea.bottom - DlgHeight;
//	if (yTop < rcArea.top)
//		yTop = rcArea.top;
//
//	// map screen coordinates to child coordinates
//	return ::SetWindowPos(m_hWnd, NULL, xLeft, yTop, -1, -1,
//		SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
//}

//LRESULT CWTLfcView::OnLButtonDblClk(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
void CWTLfcView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//bHandled = false;

	//long nXPos = static_cast<short>(LOWORD(lParam));
	//long nYPos = static_cast<short>(HIWORD(lParam));
	//long nColumn = -1;
	//POINT ptMouse = { nXPos, nYPos };

	//允许双击功能且有空档则双击有效
	if (g_fcData.m_bEnableDbClick)
	{
		//取消选中列
		g_fcData.UnselectCardCol();
		//测试击中哪列
		UINT curHit = CardColHitTest(point), emptyCol;
		if (
			g_fcData.ColInBuf(curHit)
			&&
			g_fcData.ColInCard(emptyCol = g_fcData.FindEmptyCardCol())

			||

			g_fcData.ColInCard(curHit)
			&&
			(
			g_fcData.ColInBuf(emptyCol = g_fcData.FindEmptyBuf())
			||
			g_fcData.ColInCard(emptyCol = g_fcData.FindEmptyCardCol())
			)
			)
		{
			HitAt(point);//假设发生两个单击动作，第一个动作击中当前双击的地方
			HitAt(g_fcData.RectOf(emptyCol, 1, 1).CenterPoint());//第二个单击动作击中某空档中央
		}
	}

	//return 0;
}

//测试击中了哪一列
UINT CWTLfcView::CardColHitTest(const CPoint &point)
{
	for (UINT i = 1; i <= 16; i++)
		if (g_fcData.RectOf(i, 1, 19).PtInRect(point))
			return i;
	return 0;
}

//测试当前单击点并根据击中的列的情况采取对应动作
//这个函数的代码本来是应该在OnLButtonDown中的，但
//考虑到双击动作可以用单击来模拟，而直接在OnLBtttonDblClk中调用
//OnLButtonDown不太合适，所以提取出来作为一个函数
//供OnLButtonDown和OnLBtttonDblClk两者调用
void CWTLfcView::HitAt(CPoint point)
{
	UINT nMv = 0, hit;
	//测试当前点中了哪一列牌
	if (!(hit = CardColHitTest(point)))
		goto ret;//没有击中任何一列

	//本次单击之前未选中任何列
	if (!g_fcData.m_nSel) {
		if (hit <= 12 && g_fcData.CntCardsIn(hit))//击中牌列或缓冲区且该列非空
			g_fcData.SelectCardCol(hit);//选中此列
		goto ret;
	}
	//已经选中某列，击中缓存列或牌列
	if (hit <= 12) {
		if (hit == g_fcData.m_nSel) { //单击了被选中列则取消当前选中
			g_fcData.UnselectCardCol();
			goto ret;
		}
		//单击的列不是选中列，可以移动几张？
		if (!(nMv = g_fcData.CntMaxMv(hit, g_fcData.m_nSel))) {
			if (!g_fcData.m_bEnableAlert) {
				//选中当前被击中的列（此列必定非空）
				g_fcData.SelectCardCol(hit);
			}
			else {
				MessageBox(TEXT("可能空档不够或不合规则。"),
					TEXT("不能这样移动"), MB_ICONWARNING | MB_OK);
			}
			goto ret;
		}
		//=================================-============================
		//可以移动若干张到目标列，目标列可以是空列也可以不是，源列正确性
		//（源列中一定有牌）这一点由SelectCardCol和CntMaxMv保证
		//=================================-============================
		if (g_fcData.ColInCard(g_fcData.m_nSel) && //选中牌列
			g_fcData.ColInCard(hit) &&
			g_fcData.IsEmptyCol(hit) && //击中空牌列
			nMv > 1) { //可移动多张
			//如果没有设置“尽可能移动多的牌”选项就询问
			if (!g_fcData.m_bMaxMove && IDCANCEL == MessageBox(
				TEXT("整列牌都移到此处？"), TEXT("移动"), MB_OKCANCEL))
				nMv = 1;
		}
		//一定要备份g_fcData.m_nSel，因为MoveCards会清除当前选中标记，
		//导致g_fcData.m_nSel为0，这样Record中就不能正确记录当前选中列
	Move:		UINT colSel = g_fcData.m_nSel;
		g_fcData.MoveCards(hit, colSel, nMv);
		g_fcData.Record(new COperations(hit, colSel, nMv));
	}
	//本次单击之前已经选中某列，现在击中回收列
	else if ((nMv = g_fcData.CntMaxMv(hit, g_fcData.m_nSel)) == 1) {
		goto Move;
	}
ret:	g_fcData.AutoThrow();//自动扔牌
	g_fcData.CheckGame();//游戏是否结束？
	//Invalidate();		// 有效，有闪烁
	//UpdateWindow();	// 无效
}


//LRESULT CWTLfcView::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
void CWTLfcView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//击中提示牌对应的按钮则在此处理
	bool bInverted = false;
	UINT hit = CardLabelHitTest(point);
	if (hit) {
		//CJLDoc *pDoc = GetDocument();
		CARD_POS pos[5], *p = pos, *pEnd = g_fcData.FindCardForLabel(hit, pos);
		if (p < pEnd) { //如果有牌对应牌标
			SetCapture();//捕获鼠标输入
			m_nCardLabelHit = hit;//记录此牌标

			CClientDC cdc(m_hWnd);
			cdc.InvertRect(g_fcData.RectOf(hit));//牌标反色

			//让反色牌标图形中的文字醒目
			CDC memDC;
			memDC.CreateCompatibleDC(cdc);
			//CBitmap *poldbmp = memDC.SelectObject(&m_AllBmps);
			HBITMAP hBmpOld = memDC.SelectBitmap(m_AllBmps);
			//CBrush brush(RGB(0, 255, 0));
			//CBrush brush = CreateSolidBrush(RGB(0, 255, 0));
			CBrush brush;
			brush.CreateSolidBrush(RGB(0, 255, 0));

			CRect r = g_fcData.RectOf(hit);
			cdc.FrameRect(r, brush);
			CPoint pt = r.CenterPoint();
			//pt.x -= 5; pt.y -= 7;
			//cdc.StretchBlt(pt.x,pt.y,9,15,&memDC,(hit-1)*8+32,0,8,12,NOTSRCCOPY | SRCAND);

			pt.x -= int(2.2 * g_fcCcs.dRate); pt.y -= int(4 * g_fcCcs.dRate);
			//pDC->BitBlt(p.x,p.y,8,12,&memDC,(c-1)*8+32,0,SRCAND);
			//pDC->BitBlt(p.x,p.y,8,12,&memDC,(c-1)*8+32,0,NOTSRCCOPY | SRCAND);
			//pDC->StretchBlt(p.x,p.y,int(5 * ccs.dRate),int(10 * ccs.dRate),&memDC,(c-1)*8+32,0,8,12,NOTSRCCOPY | SRCAND);
			cdc.StretchBlt(pt.x, pt.y, int(5 * g_fcCcs.dRate), int(8 * g_fcCcs.dRate), memDC, (hit - 1) * 8 + 32, 0, 8, 12, NOTSRCCOPY | SRCAND);

			memDC.SelectBitmap(hBmpOld);

			//CBrush brRed(RGB(255, 0, 0));
			//CBrush brRed = CreateSolidBrush(RGB(255, 0, 0));
			CBrush brRed;
			brRed.CreateSolidBrush(RGB(255, 0, 0));

			//CBrush brBlk(RGB(0,0,0));
			//CBrush brBlk(RGB(0, 0, 255));
			//CBrush brBlk = CreateSolidBrush(RGB(0, 0, 255));
			CBrush brBlk;
			brBlk.CreateSolidBrush(RGB(0, 0, 255));

			//WTL中必须在此定义
			CBrush hBr;
			while (p < pEnd) { //将与此牌标对应的牌显示出来并反色
				CRect r = g_fcData.RectOf(p->col, p->idx, 1);
				UINT card = g_fcData.GetCard(p->col, p->idx);
				DrawCard(r.TopLeft(), card, &cdc);

				//WTL中这样定义，只会初始化一次，重新赋值会导致hBr失效
				//CBrush hBr = TYPE(card) % 2 == 0 ? brBlk : brRed;
				hBr = TYPE(card) % 2 == 0 ? brBlk : brRed;

				cdc.FrameRect(r, hBr);
				r.InflateRect(-1, -1);
				cdc.FrameRect(r, hBr);
				r.InflateRect(-1, -1);
				cdc.FrameRect(r, hBr);

				++p;
			}
		}
	}
	else {
		HitAt(point);//击中牌列则在这里处理
	}

	//return 0;
}


//LRESULT CWTLfcView::OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
void CWTLfcView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	UINT hit = m_nCardLabelHit;
	m_nCardLabelHit = 0;
	if (hit) {
		ReleaseCapture();
		//CJLDoc *pDoc = GetDocument();
		InvalidateRect(g_fcData.RectOf(hit));//牌标反色
		CARD_POS pos[5], *p = pos, *pEnd = g_fcData.FindCardForLabel(hit, pos);
		while (p < pEnd) { //将与此牌标对应的牌正常显示出来
			CRect r = g_fcData.RectOf(p->col, p->idx, 1);
			InvalidateRect(r);
			++p;
		}
	}

	//return 0;
}


//LRESULT CWTLfcView::OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
void CWTLfcView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	UINT curHit = CardColHitTest(point);
	if
		(	//有选中列
		g_fcData.IsCol(g_fcData.m_nSel)
		//在牌列上移动
		&& g_fcData.IsCol(curHit)
		//被选中的牌列不是目前光标所在的列
		&& g_fcData.m_nSel != curHit
		//空档足够则设置光标以提示可以移动若干张到目标列
		&& g_fcData.CntMaxMv(curHit, g_fcData.m_nSel)
		)
	{
		SetCursor(m_hDown);
	}

	//return 0;
}

//测试击中了哪一个标签
UINT CWTLfcView::CardLabelHitTest(const CPoint &point)
{
	for (UINT i = 1; i <= 13; i++)
		if (g_fcData.RectOf(i).PtInRect(point))
			return i;
	return 0;
}


//LRESULT CWTLfcView::OnRButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
void CWTLfcView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//测试点了哪张牌
	UINT hit = CardColHitTest(point);
	//击中牌列
	if (g_fcData.ColInCard(hit)) {
		for (UINT idx = 1; idx < g_fcData.CntCardsIn(hit); idx++) {
			//击中被压住的牌
			if (g_fcData.RectOf(hit, idx, 1).PtInRect(point) &&
				!g_fcData.RectOf(hit, idx + 1, 1).PtInRect(point)) {
				m_rectRBDown = g_fcData.RectOf(hit, idx, 1);
				CClientDC cdc(m_hWnd);
				DrawCard(m_rectRBDown.TopLeft(), g_fcData.GetCard(hit, idx), &cdc);
				SetCapture();
				break;
			}
		}
	}

	//return 0;
}


//LRESULT CWTLfcView::OnRButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
void CWTLfcView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//刷新被右键点击的牌占据的矩形区域
	if (!m_rectRBDown.IsRectEmpty()) {
		InvalidateRect(m_rectRBDown);
		m_rectRBDown.SetRectEmpty();
		ReleaseCapture();
	}

	//return 0;
}

LRESULT CWTLfcView::OnUndo(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your message handler code here and/or call default

	g_fcData.Undo();
	g_fcData.GetHints();

	return 0;
}

LRESULT CWTLfcView::OnRand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your command handler code here

	if (!g_fcData.GiveUp()) return 0;
	int nUniqueGame = g_fcData.Random();
	//while (m_dlgScore.IsOldGameNumber(nUniqueGame = Random()));
	g_fcData.StartGame(nUniqueGame);

	return 0;
}

//自动解答
LRESULT CWTLfcView::OnAi(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your command handler code here
	g_fcData.UnselectCardCol();//取消选中状态
	g_fcData.m_Hints.ClrHints();//清除提示的记录

	CDlgAICal dlgAICal;//自动解答
	dlgAICal.DoModal();
	//UpdateAllViews(NULL);//可能使用了快速解答，所以要刷新界面
	g_pView->RedrawWindow();

	if (!dlgAICal.m_bSuccess) {
		//AfxMessageBox(TEXT("抱歉，自动解答未能成功!"));
		::MessageBox(g_pView->m_hWnd, TEXT("抱歉，自动解答未能成功!"), TEXT("WTLfc"), MB_OK);
	}
	g_fcData.CheckGame();//答案已经找到，从头开始演示

	return 0;
}

//自动解答
//void WTLfcData::OnAi()
//{
//	// TODO: Add your command handler code here
//	UnselectCardCol();//取消选中状态
//	m_Hints.ClrHints();//清除提示的记录
//
//	CDlgAICal dlgAICal;//自动解答
//	dlgAICal.DoModal();
//	UpdateAllViews(NULL);//可能使用了快速解答，所以要刷新界面
//
//	if (!dlgAICal.m_bSuccess) {
//		AfxMessageBox(TEXT("抱歉，自动解答未能成功!"));
//	}
//	CheckGame();//答案已经找到，从头开始演示
//}

//上一局
//void CWTLfcView::OnPrevGame()
LRESULT CWTLfcView::OnPrevGame(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your command handler code here
	if (!g_fcData.GiveUp()) return 0;
	g_fcData.StartGame(max(g_fcData.m_nCurGameNumber - 1, MIN_GAME_NUMBER));
	return 0;
}

//下一局
//void CWTLfcView::OnNextGame()
LRESULT CWTLfcView::OnNextGame(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (!g_fcData.GiveUp()) return 0;
	g_fcData.StartGame(min(g_fcData.m_nCurGameNumber + 1, MAX_GAME_NUMBER));
	return 0;
}

//重玩
//void CWTLfcView::OnAgain()
LRESULT CWTLfcView::OnAgain(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your command handler code here
	if (!g_fcData.GiveUp()) return 0;
	if (g_fcData.m_nCurGameNumber > 0) {
		g_fcData.StartGame(g_fcData.m_nCurGameNumber);
	}
	else {
		while (!g_fcData.m_pOps->empty()) {
			g_fcData.Undo();//撤销到开头
		}
	}
	return 0;
}

//战况
//void WTLfcData::OnScore()
LRESULT CWTLfcView::OnScore(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	g_fcData.m_dlgScore.DoModal();
	return 0;
}

LRESULT CWTLfcView::OnEdit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	return 0;
}

//选局
LRESULT CWTLfcView::OnSelectgamenumber(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (!g_fcData.GiveUp()) return 0;

	CDlgSelGame dlg;
	dlg.m_nGameNumber = g_fcData.m_nCurGameNumber;
	if (dlg.DoModal() != IDOK) return 0;

	g_fcData.StartGame(dlg.m_nGameNumber);

	return 0;
}

//提示下一步
//void CWTLfcView::OnHelpNextstep()
LRESULT CWTLfcView::OnHelpNextstep(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your command handler code here
	if (g_fcData.m_Hints.IsEmpty()) return 0;

	//提示前取消选中状态
	g_fcData.UnselectCardCol();

	//取出下一步动做的记录并提示玩家
	const COperation *pOp = g_fcData.m_Hints.NextHint();

	UINT cntSrc = g_fcData.CntCardsIn(pOp->src);
	UINT cntDes = g_fcData.CntCardsIn(pOp->des);
	CRect sR = g_fcData.RectOf(pOp->src, cntSrc - pOp->cnt + 1, pOp->cnt);
	CRect dR = g_fcData.RectOf(pOp->des, max(cntDes, 1), 1);

	//CJLView *pView = GetView();
	CClientDC cdc(g_pView->m_hWnd);
	//提示过程就是闪烁源列和目列的牌
	cdc.InvertRect(sR); cdc.InvertRect(dR);//反色
	SendMessage(g_pView->m_hWnd, WM_PAINT, 0, 0);
	::Sleep(200);
	cdc.InvertRect(sR); cdc.InvertRect(dR);//还原
	SendMessage(g_pView->m_hWnd, WM_PAINT, 0, 0);
	::Sleep(200);
	cdc.InvertRect(sR); cdc.InvertRect(dR);//反色
	SendMessage(g_pView->m_hWnd, WM_PAINT, 0, 0);
	::Sleep(200);
	cdc.InvertRect(sR); cdc.InvertRect(dR);//还原

	return 0;
}
