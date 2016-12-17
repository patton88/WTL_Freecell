// DlgScore.cpp : implementation file
//

#include "stdafx.h"
//#include "JL.h"
//#include "JLDoc.h"
#include "DlgScore.h"

//extern CJLDoc * AfxGetDocument();

static const wchar_t * statusStr[4] = { TEXT("未玩"), TEXT("通过"), TEXT("放弃"), TEXT("挂了") };
static const wchar_t * fmts = TEXT("%H:%M:%S");

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

	// First DDX call, hooks up variables to controls.
	DoDataExchange(false);

	//每局的信息
	m_lcScore.SetBkColor(RGB(0, 128, 0));
	m_lcScore.SetTextBkColor(RGB(0, 128, 0));
	m_lcScore.SetTextColor(RGB(218, 218, 218));

	int charWid = m_lcScore.GetStringWidth(TEXT("9"));
	m_lcScore.InsertColumn(0, TEXT("牌局"), LVCFMT_LEFT, charWid * (11 + 2));
	m_lcScore.InsertColumn(1, TEXT("步数"), LVCFMT_LEFT, charWid * (4 + 2));
	m_lcScore.InsertColumn(2, TEXT("状态"), LVCFMT_LEFT, charWid * (4 + 2));
	m_lcScore.InsertColumn(3, TEXT("耗时"), LVCFMT_LEFT, charWid * (8 + 2));
	m_lcScore.InsertColumn(4, TEXT("时间"), LVCFMT_LEFT, charWid * (20 + 2));

	int nTotal = 0;
	int nPassed = 0;
	int nGiveUp = 0;
	int nDead = 0;
	int nStepsPassed = 0;

	//WTL::CString s;	//WTL::CString.Format 不支持 %f
	ATL::CString s;		//ATL::CString.Format 支持 %f
	int i = 0;

	// list<CMyObject*> m_score;
	//for (POSITION pos = m_score.GetTailPosition(); pos != NULL; ++i )
	for (list<CScore>::iterator it = m_score.m_tList.end(); it != m_score.m_tList.begin(); i++)
	{
		//最后一局还没有解，所以状态不必显示
		it--;	//	必须放在这里，否则运行报错终止。COblist 与 STL list的不同
		//if(pos == m_score.m_tList.GetHeadPosition()) break;
		if (m_score.m_tList.begin() == it)
			break;

		//CScore *p = (CScore*)m_score.m_tList.GetPrev(pos);
		CScore& cs = *it;	// it = m_score.m_tList.end()，必须先it--，才能*it，否则运行时报错终止

		s.Format(TEXT("%d"), cs.gameNumber);
		m_lcScore.InsertItem(i, s);

		s.Format(TEXT("%d"), cs.steps);
		m_lcScore.SetItemText(i, 1, s);

		m_lcScore.SetItemText(i, 2, statusStr[cs.gameStatus]);	// 该句报错终止

		CTimeSpan ts = cs.tmEnd - cs.tmStart;	// #include <atltime.h>
		s.Format(TEXT("%2dm%2ds"), ts.GetMinutes(), ts.GetSeconds());
		m_lcScore.SetItemText(i, 3, s);

		m_lcScore.SetItemText(i, 4,
			cs.tmStart.Format(fmts) + TEXT("/") + cs.tmEnd.Format(fmts));

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

	return TRUE;
}

//更新战况
void CDlgScore::UpdateScore()
{
	if (m_score.m_tList.empty()) return;

	//CScore *p = (CScore*)m_score.m_tList.GetHead();
	CScore& cs = m_score.m_tList.front();
	cs.tmEnd = CTime::GetCurrentTime();

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
