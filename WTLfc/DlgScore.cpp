// DlgScore.cpp : implementation file
//

#include "stdafx.h"
//#include "JL.h"
//#include "JLDoc.h"
#include "DlgScore.h"

//extern CJLDoc * AfxGetDocument();

static const wchar_t * statusStr[4] = { TEXT("未玩"), TEXT("通过"), TEXT("放弃"), TEXT("挂了") };
static const wchar_t * fmts = TEXT("%H:%M:%S");
static const wchar_t * fmtsYMD = TEXT("%Y.%m.%d-%H:%M:%S");

/////////////////////////////////////////////////////////////////////////////
// CDlgScore dialog

//CDlgScore::CDlgScore(CWnd* pParent /*=NULL*/)
//	: CDialog(CDlgScore::IDD, pParent)
//{
//	//{{AFX_DATA_INIT(CDlgScore)
//	//}}AFX_DATA_INIT
//}

CDlgScore::CDlgScore()
{

}

CDlgScore::~CDlgScore()
{
	//while (!m_score.empty()) {
	//	CScore *pScore = (CScore*)m_score.back();
	//	delete pScore;
	//	m_score.pop_front();
	//}

	//for (list<CMyObject*>::iterator it = m_score.end(); it != m_score.begin();)
	//{
	//	//最后一局还没有解，所以状态不必显示
	//	it--;	//	必须放在这里，否则运行报错终止。COblist 与 STL list的不同

	//	//CScore *p = (CScore*)m_score.GetPrev(pos);
	//	CScore *pScore = (CScore*)(*it);	// it = m_score.end()，必须先it--，才能*it，否则运行时报错终止
	//	delete pScore;

	//	//if(pos == m_score.GetHeadPosition()) break;
	//	//if (m_score.begin() == it)
	//	//	break;
	//}
}


//void CDlgScore::DoDataExchange(CDataExchange* pDX)
//{
//	CDialog::DoDataExchange(pDX);
//	//{{AFX_DATA_MAP(CDlgScore)
//	DDX_Control(pDX, IDLC_SCORE_INFO, m_lcScoreInfo);
//	DDX_Control(pDX, IDLC_SCORE, m_lcScore);
//	//}}AFX_DATA_MAP
//}


//BEGIN_MESSAGE_MAP(CDlgScore, CDialog)
//	//{{AFX_MSG_MAP(CDlgScore)
//	//}}AFX_MSG_MAP
//END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgScore message handlers

//BOOL CDlgScore::OnInitDialog() 
LRESULT CDlgScore::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	//CDialog::OnInitDialog();
	CenterWindow(GetParent());

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME),
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME),
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	// First DDX call, hooks up variables to controls.
	DoDataExchange(false);

	//每局的信息
	m_lcScore.SetBkColor(RGB(0, 128, 0));
	m_lcScore.SetTextBkColor(RGB(0, 128, 0));
	m_lcScore.SetTextColor(RGB(218, 218, 218));

	int charWid = m_lcScore.GetStringWidth(TEXT("9"));
	m_lcScore.InsertColumn(0, TEXT("序号"), LVCFMT_RIGHT, charWid * (13 + 2));	// 这种方式设置0列对齐不行
	m_lcScore.InsertColumn(1, TEXT("牌局"), LVCFMT_RIGHT, charWid * (13 + 2));
	m_lcScore.InsertColumn(2, TEXT("步数"), LVCFMT_RIGHT, charWid * (5 + 2));
	m_lcScore.InsertColumn(3, TEXT("状态"), LVCFMT_RIGHT, charWid * (5 + 2));
	m_lcScore.InsertColumn(4, TEXT("耗时"), LVCFMT_LEFT, charWid * (17 + 2));
	m_lcScore.InsertColumn(5, TEXT("时间"), LVCFMT_LEFT, charWid * (43 + 2));

	// 设置0列对齐，需要使用这种方式
	LVCOLUMN colTest;
	colTest.mask = LVCF_FMT;
	m_lcScore.GetColumn(0, &colTest);
	colTest.fmt = LVCFMT_RIGHT;
	m_lcScore.SetColumn(0, &colTest);
	// 设置0列对齐，需要使用这种方式

	int nTotal = 0;
	int nPassed = 0;
	int nGiveUp = 0;
	int nDead = 0;
	int nStepsPassed = 0;

	//WTL::CString s;	//WTL::CString.Format 不支持 %f
	ATL::CString s;		//ATL::CString.Format 支持 %f
	ATL::CString st;	
	int i = 0;
	int nSize = m_score.m_tList.size();

	// list<CMyObject*> m_score;
	//for (POSITION pos = m_score.GetTailPosition(); pos != NULL; ++i )
	//for (list<CScore>::iterator it = m_score.m_tList.end(); it != m_score.m_tList.begin(); i++)
	//战况从最后一局开始显示
	for (list<CScore>::iterator it = m_score.m_tList.begin(); ;i++)
	{
		// m_score.m_tList.begin()存放的最后一局还没有解，所以状态不必显示
		it++;	//	必须放在这里，否则运行报错终止。跳过 m_score.m_tList.begin()
		if (m_score.m_tList.end() == it)
			break;

		//it--;	//	必须放在这里，否则运行报错终止。COblist 与 STL list的不同
		////if(pos == m_score.m_tList.GetHeadPosition()) break;
		//if (m_score.m_tList.begin() == it)
		//	break;

		//CScore *p = (CScore*)m_score.m_tList.GetPrev(pos);
		CScore& cs = *it;	// it = m_score.m_tList.end()，必须先it--，才能*it，否则运行时报错终止

		s.Format(TEXT("%d"), nSize - (i + 1));	// 当前一局没有战况信息，所以要多减1
		m_lcScore.InsertItem(i, s);

		s.Format(TEXT("%d"), cs.gameNumber);
		m_lcScore.SetItemText(i, 1, s);

		s.Format(TEXT("%d"), cs.steps);
		m_lcScore.SetItemText(i, 2, s);

		m_lcScore.SetItemText(i, 3, statusStr[cs.gameStatus]);	// 该句报错终止

		CTimeSpan ts = cs.tmEnd - cs.tmStart;	// #include <atltime.h>
		//s.Format(TEXT("%2dm%2ds"), ts.GetMinutes(), ts.GetSeconds());

		// 按照这种方式不能正确获取。可能是ATL::CString.Format函数的毛病
		//s.Format(TEXT("%2dD%2dh%2dm%2ds"), ts.GetDays(), ts.GetHours(), ts.GetMinutes(), ts.GetSeconds());

		// 必须按照这样的方式才能正确获取，否则不对。可能是ATL::CString.Format函数的毛病
		s.Format(TEXT("%3dD"), ts.GetDays());
		st.Format(TEXT("%2dH - "), ts.GetHours());
		s += st;
		st.Format(TEXT("%2dm%2ds"), ts.GetMinutes(), ts.GetSeconds());
		s += st;
		m_lcScore.SetItemText(i, 4, s);

		m_lcScore.SetItemText(i, 5,
			cs.tmStart.Format(fmtsYMD) + TEXT(" / ") + cs.tmEnd.Format(fmtsYMD));

		++nTotal;

		if (cs.gameStatus == CScore::gamePassed) {
			++nPassed;
			nStepsPassed += cs.steps;
		}
		else if (cs.gameStatus == CScore::gameGiveUp) {
			++nGiveUp;
		}
		else if (cs.gameStatus == CScore::gameDead) {
			++nDead;
		}
	}

	//统计战况
	m_lcScoreInfo.SetBkColor(RGB(0, 128, 0));
	m_lcScoreInfo.SetTextBkColor(RGB(0, 128, 0));
	m_lcScoreInfo.SetTextColor(RGB(218, 218, 218));

	// 用空格代替\t吧这个问题第2个回答：	listbox好像是这样，但是取出来的内容还是正确的、
	m_lcScoreInfo.InsertColumn(0, TEXT("项目"), LVCFMT_LEFT, charWid * 13);
	m_lcScoreInfo.InsertColumn(1, TEXT("统计"), LVCFMT_LEFT, charWid * 15);
	m_lcScoreInfo.InsertItem(0, TEXT("总计(局) ："));
	m_lcScoreInfo.InsertItem(1, TEXT("通过(局) ："));
	m_lcScoreInfo.InsertItem(2, TEXT("胜率     ："));
	m_lcScoreInfo.InsertItem(3, TEXT("玩通一局 ："));
	m_lcScoreInfo.InsertItem(4, TEXT("放弃(局) ："));
	m_lcScoreInfo.InsertItem(5, TEXT("挂了(局) ："));

	s.Format(TEXT("%d"), nTotal);
	m_lcScoreInfo.SetItemText(0, 1, s);

	s.Format(TEXT("%d"), nPassed);
	m_lcScoreInfo.SetItemText(1, 1, s);

	if (nTotal > 0)
	{
		s.Format(TEXT("%3.1f%%"), 100.*nPassed / nTotal);
		m_lcScoreInfo.SetItemText(2, 1, s);
	}
	else
		m_lcScoreInfo.SetItemText(2, 1, L"0");

	if (nPassed > 0)
	{
		s.Format(TEXT("平均%3.1f步"), float(nStepsPassed) / nPassed);
		m_lcScoreInfo.SetItemText(3, 1, s);
	}
	else
		m_lcScoreInfo.SetItemText(3, 1, L"0");

	s.Format(TEXT("%d"), nGiveUp);
	m_lcScoreInfo.SetItemText(4, 1, s);

	s.Format(TEXT("%d"), nDead);
	m_lcScoreInfo.SetItemText(5, 1, s);

	CRect rect;
	GetClientRect(&rect);	//取客户区大小  
	Old.x = rect.right - rect.left;
	Old.y = rect.bottom - rect.top;

	m_lcScore.GetWindowRect(&rect);
	ww = rect.left;
	m_lcScoreInfo.GetWindowRect(&rect);
	ww -= rect.right;			// 得到中间分隔栏宽度

	return TRUE;
}

//更新战况
void CDlgScore::UpdateScore()
{
	if (m_score.m_tList.empty()) return;

	//CScore *p = (CScore*)m_score.m_tList.GetHead();
	CScore& cs = m_score.m_tList.front();
	cs.tmEnd = CTime::GetCurrentTime();

	//WTL::CString str = cs.tmStart.Format(fmts) + TEXT("/") + cs.tmEnd.Format(fmts);
	//::MessageBox(NULL, str, L"", NULL);

	//十五秒以内解开的局认为是使用了自动解答的帮助，将不予记录
	if ((cs.tmEnd - cs.tmStart).GetTotalSeconds() < 15) {
		//m_score.RemoveHead();
		m_score.m_tList.pop_front();
		//delete p;
		return;
	}

	//CJLDoc *pDoc = AfxGetDocument();
	cs.gameStatus =
		g_fcData.GameOver() ? CScore::gamePassed :
		g_fcData.m_Hints.IsEmpty() ? CScore::gameDead :
		CScore::gameGiveUp;
	cs.steps = g_fcData.m_OpsList.m_tList.size();
}

void CDlgScore::InitScore()
{
	//CScore *p = new CScore;
	//CScore *p = new CScore;
	//ATLASSERT(p);
	m_score.m_tList.push_front(CScore());

	//CJLDoc *pDoc = AfxGetDocument();

	// STL list front() 返回第一个元素的引用，back() 返回最后一元素的引用
	m_score.m_tList.front().gameNumber = g_fcData.m_nCurGameNumber;
	m_score.m_tList.front().tmStart = CTime::GetCurrentTime();
}

//看看此局是否曾经玩过
bool CDlgScore::IsOldGameNumber(int gameNum)
{
	//for(POSITION pos = m_score.GetHeadPosition(); pos != NULL; )
	//	if(gameNum == ((CScore*)m_score.GetNext(pos))->gameNumber)
	//		return true;
	for (list<CScore>::iterator it = m_score.m_tList.begin(); it != m_score.m_tList.begin(); it++)
	{
		if (gameNum == (*it).gameNumber)
			return true;
	}

	return false;
}

LRESULT CDlgScore::OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

LRESULT CDlgScore::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// 添加以下2行代码，解决第2次打开战况对话框报错终止问题
	m_lcScore.Detach();
	m_lcScoreInfo.Detach();

	EndDialog(wID);
	return 0;
}

LRESULT CDlgScore::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//CDialog::OnSizing(fwSide, pRect);

	// TODO: Add your message handler code here
	//CRect r;
	//GetClientRect(r);
	//MoveWindow(r);

	resize();

	return 0;
}

void CDlgScore::resize()
{
	float fsp[2];
	POINT Newp; //获取现在对话框的大小
	CRect recta;
	GetClientRect(&recta);     //取客户区大小  

	Newp.x = recta.right - recta.left;
	Newp.y = recta.bottom - recta.top;
	fsp[0] = (float)Newp.x / Old.x;
	fsp[1] = (float)Newp.y / Old.y;

	CRect Rect;
	//int idWoc;		// woc ID of Window of Control
	CPoint OldTLPoint, TLPoint; //左上角
	CPoint OldBRPoint, BRPoint; //右下角

	m_lcScoreInfo.GetWindowRect(Rect);		// 取得控件原来的位置 
	ScreenToClient(&Rect);

	OldTLPoint = Rect.TopLeft();
	//TLPoint.x = long(OldTLPoint.x*fsp[0]);	// 计算出空间新位置的左上角
	TLPoint.x = recta.left;					// 这样更准确。计算出空间新位置的左上角
	TLPoint.y = long(OldTLPoint.y*fsp[1]);

	OldBRPoint = Rect.BottomRight();
	BRPoint.x = long(OldBRPoint.x *fsp[0]);	// 计算出空间新位置的右下角
	BRPoint.y = long(OldBRPoint.y *fsp[1]);

	Rect.SetRect(TLPoint, BRPoint);			// 设置新位置
	//Invalidate();
	::MoveWindow(m_lcScoreInfo.m_hWnd, Rect.left, Rect.top, Rect.Width(), Rect.Height(), TRUE);	// 移动控件
	//::InvalidateRect(GetDlgItem(idWoc), Rect, TRUE);


	m_lcScore.GetWindowRect(Rect);			// 取得控件原来的位置 
	ScreenToClient(&Rect);

	OldTLPoint = Rect.TopLeft();
	//TLPoint.x = long(OldTLPoint.x*fsp[0]);	// 计算出空间新位置的左上角
	TLPoint.x = BRPoint.x + ww;				// 计算出空间新位置的左上角
	TLPoint.y = long(OldTLPoint.y*fsp[1]);

	OldBRPoint = Rect.BottomRight();
	//BRPoint.x = long(OldBRPoint.x *fsp[0]);// 计算出空间新位置的右下角
	BRPoint.x = recta.right;					// 这样更准确。计算出空间新位置的右下角
	BRPoint.y = long(OldBRPoint.y *fsp[1]);

	Rect.SetRect(TLPoint, BRPoint);			// 设置新位置
	//Invalidate();
	::MoveWindow(m_lcScore.m_hWnd, Rect.left, Rect.top, Rect.Width(), Rect.Height(), TRUE);	// 移动控件
	//::InvalidateRect(GetDlgItem(idWoc), Rect, TRUE);

	//HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);		//列出所有控件  
	//while (hwndChild)
	//{
	//	idWoc = ::GetDlgCtrlID(hwndChild);				// 取得控件ID
	//	//if ( IDC_LIST1 == idWoc ) break;

	//	::GetWindowRect(GetDlgItem(idWoc), Rect);		// 取得控件原来的位置 
	//	ScreenToClient(&Rect);

	//	OldTLPoint = Rect.TopLeft();
	//	TLPoint.x = long(OldTLPoint.x*fsp[0]);			// 计算出空间新位置的左上角
	//	TLPoint.y = long(OldTLPoint.y*fsp[1]);

	//	OldBRPoint = Rect.BottomRight();
	//	BRPoint.x = long(OldBRPoint.x *fsp[0]);			// 计算出空间新位置的右下角
	//	BRPoint.y = long(OldBRPoint.y *fsp[1]);

	//	Rect.SetRect(TLPoint, BRPoint);					// 设置新位置
	//	Invalidate();
	//	::MoveWindow(GetDlgItem(idWoc), Rect.left, Rect.top, Rect.Width(), Rect.Height(), TRUE);	// 移动控件
	//	//::InvalidateRect(GetDlgItem(idWoc), Rect, TRUE);

	//	hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);	// 取下一个控件句柄
	//}
	Old = Newp;
}
