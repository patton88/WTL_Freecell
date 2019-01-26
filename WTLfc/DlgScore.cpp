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

CDlgScore::CDlgScore()	// -V730
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

	//CString s;	//CString.Format 不支持 %f
	ATL::CString s;		//ATL::CString.Format 支持 %f
	ATL::CString st;
	int i = 0;
	int nSize = m_score.m_tList.size();

	// list<CMyObject*> m_score;
	//for (POSITION pos = m_score.GetTailPosition(); pos != NULL; ++i )
	//for (list<CScore>::iterator it = m_score.m_tList.end(); it != m_score.m_tList.begin(); i++)
	//战况从最后一局开始显示
	for (list<CScore>::iterator it = m_score.m_tList.begin(); ; i++)
	{
		// m_score.m_tList.begin()存放的最后一局还没有解，所以状态不必显示
		++it;	//	必须放在这里，否则运行报错终止。跳过 m_score.m_tList.begin()
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
		s.Format(TEXT("%3dD"), (int)ts.GetDays());
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

	// 用于实现战况窗口缩放时，控件自动

	//CRect rect1, rect2, rect3;
	//GetClientRect(&rect1);		// 取客户区大小  

	//m_lcScoreInfo.GetWindowRect(&rect2);
	//ScreenToClient(rect2);

	//m_lcScore.GetWindowRect(&rect3);
	//ScreenToClient(rect3);

	//+ rect1{ top = 0 bottom = 495 left = 0 right = 943 }	WTL::CRect
	//+ rect2{ top = 0 bottom = 495 left = 0 right = 259 }	WTL::CRect
	//+ rect3{ top = 0 bottom = 495 left = 266 right = 941 }	WTL::CRect


	CRect rect;
	m_lcScore.GetWindowRect(&rect);
	m_nInterval = rect.left;
	m_lcScoreInfo.GetWindowRect(&rect);
	m_nInterval -= rect.right;	// 得到m_lcScoreInfo、m_lcScore 控件之间的间隔宽度

	m_dScale = rect.Width();		// 得到 m_lcScoreInfo 的窗口宽度
	GetClientRect(&rect);		// 取客户区大小  
	m_dScale /= rect.Width();	// 得到 m_lcScore 控件宽度占窗口客户区宽度的比例

	// 用于实现战况窗口缩放时，控件自动

	m_sortstyl = 1;
	m_selectCol = -1;

	return TRUE;
}

//更新战况
void CDlgScore::UpdateScore()
{
	if (m_score.m_tList.empty()) return;

	//CScore *p = (CScore*)m_score.m_tList.GetHead();
	CScore& cs = m_score.m_tList.front();
	cs.tmEnd = CTime::GetCurrentTime();

	//CString str = cs.tmStart.Format(fmts) + TEXT("/") + cs.tmEnd.Format(fmts);
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
	for (list<CScore>::iterator it = m_score.m_tList.begin(); it != m_score.m_tList.end(); ++it)
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
	CRect rectc;
	GetClientRect(&rectc);     //取客户区大小  

	CRect rect(0, 0, int(rectc.right * m_dScale), rectc.bottom);
	::MoveWindow(m_lcScoreInfo.m_hWnd, rect.left, rect.top, rect.Width(), rect.Height(), TRUE);	// 移动控件

	rect.left = rect.right + m_nInterval;
	rect.top = 0;
	rect.right = rectc.right - 2;
	rect.bottom = rectc.bottom;
	::MoveWindow(m_lcScore.m_hWnd, rect.left, rect.top, rect.Width(), rect.Height(), TRUE);		// 移动控件

}

//-------------------------------------------------------------------
//单击列表框列表头排序
LRESULT CDlgScore::OnColumnClick(int/*idCtrl*/, LPNMHDR pnmh, BOOL&/*bHandled*/)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pnmh;
	//NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	//MessageBox(L"Test");

	/*
	VC CListCtrl控件单击列排序  2009-09-24 14:26:07|  分类： VC技术 |  标签： |字号大中小 订阅
	http://alifens198345.blog.163.com/blog/static/11679917720098242267804/
	网上很多都是调用了自己写的回调函数后排序的！我这个是直接响应单击列消息来进行排序，本来也是网上找的一段代码！
	但是只能做字符串的排序！如果是字符串数字，比如“10”，“1”“2”“3”“11”按数字大小排序就不正确了！
	显示为1，10，11，2，3的顺序！但是实际顺序应该是1，2，3，10，11。我自己修改了一下，
	现在字符串和字符串数字都能正确排序了!只要把下面的代码贴到OnColumnclickList()函数里就可以了！

	2010-09-03 11:37sd
	double dbStrA = atof(strA);//这个地方有问题(其中strA是CString类型)不信，你自己可以试一下，
	顺便说一句，我用的是visual studio 2010。谢谢回复
	2010-11-14 22:54有个贱人在我身边 回复 sd
	atof((LPSTR)(LPCTSTR)strA);
	编译器不同,编码也不同,自己转一下就好了.不能凡事靠别人.回复
	*/

	//SetRedraw(FALSE);

	int selectCol = pNMListView->iSubItem;
	int listRows, listColumns, i, j, n;
	//DWORD ddA, ddB;

	if (m_selectCol != selectCol)	// 若不是上次单击的表头列，便重设排序为升序
		m_sortstyl = 1;

	m_selectCol = selectCol;			// 保存单击的表头列

	//取得控件的行数和列数
	//listRows = GetItemCount() - m_nTreeRec;
	listRows = m_score.m_tList.size() - 1;	// 当前一局没有战况信息，所以要多减1
	listColumns = m_lcScore.GetHeader().GetItemCount();
	CString strA, strB;
	double dbStrA, dbStrB;
	wchar_t *wpSrc, *wpEnd;

	//m_bSortStatusOfLV = (100 + selectCol) * m_sortstyl;
	//if (g_pMainFrame->m_bSaveSortStatusOfLV)
	//{
	//	g_pMainFrame->m_bSaveSortStatusOfLV = m_bSortStatusOfLV;
	//	WritePrivateProfileStringW(L"Main", L"SaveSortStatusOfLV", itos(m_bSortStatusOfLV), g_pMainFrame->m_wzIniName);
	//}

	//构造函数中m_sortstyl = 1。每次改变目录后初始化排序类型为升序排列m_sortstyl = 1
	//m_sortstyl == 1 升序排列，m_sortstyl == -1 降序排列。每次排序后都反转排序类型
	if (m_sortstyl == 1)
	{
		for (j = 1; j <= listRows; j++)
		{
			for (i = 0; i < listRows - j; i++)
			{
				m_lcScore.GetItemText(i, selectCol, strA.GetBuffer(255), 255); //m_List2为控件绑定的变量，根据自己的修改
				m_lcScore.GetItemText(i + 1, selectCol, strB.GetBuffer(255), 255);
				strA.ReleaseBuffer();
				strB.ReleaseBuffer();

				wpSrc = strA.GetBuffer(strA.GetLength());	//由于WTL中，m_lcScore.GetItemText使用CString最方便，所以这里还是用strA
				dbStrA = wcstod(wpSrc, &wpEnd);
				strA.ReleaseBuffer();

				wpSrc = strB.GetBuffer(strB.GetLength());
				dbStrB = wcstod(wpSrc, &wpEnd);
				strB.ReleaseBuffer();

				//根据返回值判断是字符串还是字符串数字
				//if (dbStrA != dbStrB && dbStrA != 0 && dbStrB != 0)		// 若是字符串数字
				if (fabs(dbStrA - dbStrB) > 0 && fabs(dbStrA) > 0 && fabs(dbStrB) > 0)		// 若是字符串数字
				{
					if (dbStrA > dbStrB)
					{
						for (n = 0; n < listColumns; n++)
						{
							m_lcScore.GetItemText(i, n, strA.GetBuffer(255), 255);
							m_lcScore.GetItemText(i + 1, n, strB.GetBuffer(255), 255);
							strA.ReleaseBuffer();
							strB.ReleaseBuffer();

							m_lcScore.SetItemText(i, n, strB);
							m_lcScore.SetItemText(i + 1, n, strA);
						}

						//保存在记录附加数据中的nRecNO只需交换1次
						//ddA = GetItemData(i);
						//ddB = GetItemData(i + 1);
						//SetItemData(i, ddB);
						//SetItemData(i + 1, ddA);

						//m_lcScore.SetItemText(m, 2, itos(ddB));		//测试用，Show Data
						//m_lcScore.SetItemText(m + 1, 2, itos(ddA));	//测试用，Show Data
					}
				}
				else
				{
					if (strA > strB)							//若是字符串
					{
						for (n = 0; n < listColumns; n++)		//12为你控件的列的数量，根据自己的实际情况修改
						{
							m_lcScore.GetItemText(i, n, strA.GetBuffer(255), 255);
							m_lcScore.GetItemText(i + 1, n, strB.GetBuffer(255), 255);
							strA.ReleaseBuffer();
							strB.ReleaseBuffer();

							m_lcScore.SetItemText(i, n, strB);
							m_lcScore.SetItemText(i + 1, n, strA);
						}

						//保存在记录附加数据中的iRecNO只需交换1次
						//ddA = GetItemData(i);
						//ddB = GetItemData(i + 1);
						//SetItemData(i, ddB);
						//SetItemData(i + 1, ddA);

						//m_lcScore.SetItemText(m, 2, itos(ddB));		//测试用，Show Data
						//m_lcScore.SetItemText(m + 1, 2, itos(ddA));	//测试用，Show Data
					}
				}

			}
		}
	}
	//构造函数中m_sortstyl = 1。每次改变目录后初始化排序类型为升序排列m_sortstyl = 1
	//m_sortstyl == 1 升序排列，m_sortstyl == -1 降序排列。每次排序后都反转排序类型
	else if (m_sortstyl == -1)
	{
		for (j = 1; j <= listRows; j++)
		{
			for (i = 0; i < listRows - j; i++)
			{
				m_lcScore.GetItemText(i, selectCol, strA.GetBuffer(255), 255);
				m_lcScore.GetItemText(i + 1, selectCol, strB.GetBuffer(255), 255);
				strA.ReleaseBuffer();
				strB.ReleaseBuffer();

				wpSrc = strA.GetBuffer(strA.GetLength());
				dbStrA = wcstod(wpSrc, &wpEnd);
				strA.ReleaseBuffer();

				wpSrc = strB.GetBuffer(strB.GetLength());
				dbStrB = wcstod(wpSrc, &wpEnd);
				strB.ReleaseBuffer();

				//if (dbStrA != dbStrB && dbStrA != 0 && dbStrB != 0)		// 若是字符串数字
				if (fabs(dbStrA - dbStrB) > 0 && fabs(dbStrA) > 0 && fabs(dbStrB) > 0)		// 若是字符串数字
				{
					if (dbStrB > dbStrA)
					{
						for (n = 0; n < listColumns; n++)
						{
							m_lcScore.GetItemText(i, n, strA.GetBuffer(255), 255);
							m_lcScore.GetItemText(i + 1, n, strB.GetBuffer(255), 255);
							strA.ReleaseBuffer();
							strB.ReleaseBuffer();

							m_lcScore.SetItemText(i, n, strB);
							m_lcScore.SetItemText(i + 1, n, strA);
						}

						//保存在记录附加数据中的iRecNO只需交换1次
						//ddA = GetItemData(i);
						//ddB = GetItemData(i + 1);
						//SetItemData(i, ddB);
						//SetItemData(i + 1, ddA);

						//m_lcScore.SetItemText(m, 2, itos(ddB));		//测试用，Show Data
						//m_lcScore.SetItemText(m + 1, 2, itos(ddA));	//测试用，Show Data
					}
				}
				else
				{
					if (strB > strA)
					{
						for (n = 0; n < listColumns; n++)
						{
							m_lcScore.GetItemText(i, n, strA.GetBuffer(255), 255);
							m_lcScore.GetItemText(i + 1, n, strB.GetBuffer(255), 255);
							strA.ReleaseBuffer();
							strB.ReleaseBuffer();

							m_lcScore.SetItemText(i, n, strB);
							m_lcScore.SetItemText(i + 1, n, strA);
						}

						//保存在记录附加数据中的iRecNO只需交换1次
						//ddA = GetItemData(i);
						//ddB = GetItemData(i + 1);
						//SetItemData(i, ddB);
						//SetItemData(i + 1, ddA);

						//m_lcScore.SetItemText(m, 2, itos(ddB));		//测试用，Show Data
						//m_lcScore.SetItemText(m + 1, 2, itos(ddA));	//测试用，Show Data
					}
				}
			}
		}
	}

	//构造函数中m_sortstyl = 1。每次改变目录后初始化排序类型为升序排列m_sortstyl = 1
	//m_sortstyl == 1 升序排列，m_sortstyl == -1 降序排列。每次排序后都反转排序类型
	m_sortstyl = m_sortstyl * (-1);

	//每次排序后自动更新序列号
	//for (j = 0; j < listRows; j++)
	//	m_lcScore.SetItemText(j, 1, itos(j + 1));	//Set Number

	//SetRedraw(TRUE);
	//CRect rect;
	//::GetWindowRect(m_lcScore.m_hWnd, &rect);
	//rect.top -= 8;
	//InvalidateRect(rect);
	//m_lcScore.Invalidate();
	//Invalidate();

	//*pResult = 0;


	/*
	EnsureVisible---调用该函数以确保指定的列表视图条目可见，其原型为：
	BOOL EnsureVisible（int nItem,BOOL bPartialOK）
	　　 返回值：Nonzero if successful; otherwise zero.
	   　　 nItem：指定了必须可见的条目索引。
		  　　 bPartialOK：指定了是否允许部分可见。
			 　　 列表视图控件将在必要时进行滚动，以确保指定条目可见。如果bPartialOK参数为非零值，
				则当条目部分可见时，列表视图控件不进行滚动。

				注意：如果ListView指定LVS_NOSCROLL,调用Scroll和EnsureVisible都是FALSE
				*/

				/*
				1、设置CListCtrl选中行

				m_list.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);

				注意：这句代码只是让指定行高亮显示，如果使用后再用GetSelectionMark函数来得到选中行，结果往往是错误的。
				比如，先用鼠标点击选中第5行，调用GetSelectionMark函数得到的是4(第5行)，再调用SetItemState函数选中第2行，
				然后调用GetSelectionMark函数得到的还是4(第5行)，所以，需要在设置选中行高亮显示时，一般与SetSelectionMark函数连用：
				m_list.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,LVIS_FOCUSED | LVIS_SELECTED);   //选中行
				m_list.SetSelectionMark(nIndexs);

				*/

	return 0L;
}

//-------------------------------------------------------------------
//单击列表框列表头排序
//LRESULT CMainFrame::OnLVItemDblclk(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
//LRESULT CMainFrame::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult)
//LRESULT CRMlistView::OnColumnClick(int/*idCtrl*/, LPNMHDR pnmh, BOOL&/*bHandled*/)
//{
//	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pnmh;
//	//NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
//
//	/*
//	VC CListCtrl控件单击列排序  2009-09-24 14:26:07|  分类： VC技术 |  标签： |字号大中小 订阅
//	http://alifens198345.blog.163.com/blog/static/11679917720098242267804/
//	网上很多都是调用了自己写的回调函数后排序的！我这个是直接响应单击列消息来进行排序，本来也是网上找的一段代码！
//	但是只能做字符串的排序！如果是字符串数字，比如“10”，“1”“2”“3”“11”按数字大小排序就不正确了！
//	显示为1，10，11，2，3的顺序！但是实际顺序应该是1，2，3，10，11。我自己修改了一下，
//	现在字符串和字符串数字都能正确排序了!只要把下面的代码贴到OnColumnclickList()函数里就可以了！
//
//	2010-09-03 11:37sd
//	double dbStrA = atof(strA);//这个地方有问题(其中strA是CString类型)不信，你自己可以试一下，
//	顺便说一句，我用的是visual studio 2010。谢谢回复
//	2010-11-14 22:54有个贱人在我身边 回复 sd
//	atof((LPSTR)(LPCTSTR)strA);
//	编译器不同,编码也不同,自己转一下就好了.不能凡事靠别人.回复
//	*/
//
//	SetRedraw(FALSE);
//
//	int selectCol = pNMListView->iSubItem;
//	int listRows, listColumns, i, j, m, n;
//	DWORD ddA, ddB;
//
//	//取得控件的行数和列数
//	listRows = GetItemCount() - m_nTreeRec;
//	listColumns = GetHeader().GetItemCount();
//	CString strA, strB;
//	double dbStrA, dbStrB;
//	wchar_t *wpSrc, *wpEnd;
//
//	m_bSortStatusOfLV = (100 + selectCol) * m_sortstyl;
//	if (g_pMainFrame->m_bSaveSortStatusOfLV)
//	{
//		g_pMainFrame->m_bSaveSortStatusOfLV = m_bSortStatusOfLV;
//		WritePrivateProfileStringW(L"Main", L"SaveSortStatusOfLV", itos(m_bSortStatusOfLV), g_pMainFrame->m_wzIniName);
//	}
//
//	//构造函数中m_sortstyl = 1。每次改变目录后初始化排序类型为升序排列m_sortstyl = 1
//	//m_sortstyl == 1 升序排列，m_sortstyl == -1 降序排列。每次排序后都反转排序类型
//	if (m_sortstyl == 1)
//	{
//		for (j = 1; j <= listRows; j++)
//		{
//			for (i = 0; i < listRows - j; i++)
//			{
//				m = i + m_nTreeRec;
//				GetItemText(m, selectCol, strA); //m_List2为控件绑定的变量，根据自己的修改
//				GetItemText(m + 1, selectCol, strB);
//
//				wpSrc = strA.GetBuffer(strA.GetLength());	//由于WTL中，GetItemText使用CString最方便，所以这里还是用strA
//				dbStrA = wcstod(wpSrc, &wpEnd);
//				strA.ReleaseBuffer();
//
//				wpSrc = strB.GetBuffer(strB.GetLength());
//				dbStrB = wcstod(wpSrc, &wpEnd);
//				strB.ReleaseBuffer();
//
//				//根据返回值判断是字符串还是字符串数字
//				if (dbStrA != dbStrB && dbStrA != 0 && dbStrB != 0)		//若是字符串数字
//				{
//					if (dbStrA > dbStrB)
//					{
//						for (n = 0; n < listColumns; n++)
//						{
//							GetItemText(m, n, strA);
//							GetItemText(m + 1, n, strB);
//							SetItemText(m, n, strB);
//							SetItemText(m + 1, n, strA);
//						}
//						//保存在记录附加数据中的nRecNO只需交换1次
//						ddA = GetItemData(m);
//						ddB = GetItemData(m + 1);
//						SetItemData(m, ddB);
//						SetItemData(m + 1, ddA);
//
//						//SetItemText(m, 2, itos(ddB));		//测试用，Show Data
//						//SetItemText(m + 1, 2, itos(ddA));	//测试用，Show Data
//					}
//				}
//				else
//				{
//					if (strA > strB)							//若是字符串
//					{
//						for (n = 0; n < listColumns; n++)		//12为你控件的列的数量，根据自己的实际情况修改
//						{
//							GetItemText(m, n, strA);
//							GetItemText(m + 1, n, strB);
//							SetItemText(m, n, strB);
//							SetItemText(m + 1, n, strA);
//						}
//						//保存在记录附加数据中的iRecNO只需交换1次
//						ddA = GetItemData(m);
//						ddB = GetItemData(m + 1);
//						SetItemData(m, ddB);
//						SetItemData(m + 1, ddA);
//
//						//SetItemText(m, 2, itos(ddB));		//测试用，Show Data
//						//SetItemText(m + 1, 2, itos(ddA));	//测试用，Show Data
//					}
//				}
//
//			}
//		}
//	}
//	//构造函数中m_sortstyl = 1。每次改变目录后初始化排序类型为升序排列m_sortstyl = 1
//	//m_sortstyl == 1 升序排列，m_sortstyl == -1 降序排列。每次排序后都反转排序类型
//	else if (m_sortstyl == -1)
//	{
//		for (j = 1; j <= listRows; j++)
//		{
//			for (i = 0; i < listRows - j; i++)
//			{
//				m = i + m_nTreeRec;
//				GetItemText(m, selectCol, strA);
//				GetItemText(m + 1, selectCol, strB);
//
//				wpSrc = strA.GetBuffer(strA.GetLength());
//				dbStrA = wcstod(wpSrc, &wpEnd);
//				strA.ReleaseBuffer();
//
//				wpSrc = strB.GetBuffer(strB.GetLength());
//				dbStrB = wcstod(wpSrc, &wpEnd);
//				strB.ReleaseBuffer();
//
//				if (dbStrA != dbStrB && dbStrA != 0 && dbStrB != 0)
//				{
//					if (dbStrB > dbStrA)
//					{
//						for (n = 0; n < listColumns; n++)
//						{
//							GetItemText(m, n, strA);
//							GetItemText(m + 1, n, strB);
//							SetItemText(m, n, strB);
//							SetItemText(m + 1, n, strA);
//						}
//						//保存在记录附加数据中的iRecNO只需交换1次
//						ddA = GetItemData(m);
//						ddB = GetItemData(m + 1);
//						SetItemData(m, ddB);
//						SetItemData(m + 1, ddA);
//
//						//SetItemText(m, 2, itos(ddB));		//测试用，Show Data
//						//SetItemText(m + 1, 2, itos(ddA));	//测试用，Show Data
//					}
//				}
//				else
//				{
//					if (strB > strA)
//					{
//						for (n = 0; n < listColumns; n++)
//						{
//							GetItemText(m, n, strA);
//							GetItemText(m + 1, n, strB);
//							SetItemText(m, n, strB);
//							SetItemText(m + 1, n, strA);
//						}
//						//保存在记录附加数据中的iRecNO只需交换1次
//						ddA = GetItemData(m);
//						ddB = GetItemData(m + 1);
//						SetItemData(m, ddB);
//						SetItemData(m + 1, ddA);
//
//						//SetItemText(m, 2, itos(ddB));		//测试用，Show Data
//						//SetItemText(m + 1, 2, itos(ddA));	//测试用，Show Data
//					}
//				}
//			}
//		}
//	}
//
//	//构造函数中m_sortstyl = 1。每次改变目录后初始化排序类型为升序排列m_sortstyl = 1
//	//m_sortstyl == 1 升序排列，m_sortstyl == -1 降序排列。每次排序后都反转排序类型
//	m_sortstyl = m_sortstyl * (-1);
//
//	//每次排序后自动更新序列号
//	for (j = 0; j < listRows; j++)
//		SetItemText(j + m_nTreeRec, 1, itos(j + 1));	//Set Number
//
//	SetRedraw(TRUE);
//
//	//*pResult = 0;
//
//
//	/*
//	EnsureVisible---调用该函数以确保指定的列表视图条目可见，其原型为：
//	BOOL EnsureVisible（int nItem,BOOL bPartialOK）
//	　　 返回值：Nonzero if successful; otherwise zero.
//	   　　 nItem：指定了必须可见的条目索引。
//		  　　 bPartialOK：指定了是否允许部分可见。
//			 　　 列表视图控件将在必要时进行滚动，以确保指定条目可见。如果bPartialOK参数为非零值，
//				则当条目部分可见时，列表视图控件不进行滚动。
//
//				注意：如果ListView指定LVS_NOSCROLL,调用Scroll和EnsureVisible都是FALSE
//				*/
//
//	/*
//	1、设置CListCtrl选中行
//
//	m_list.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
//
//	注意：这句代码只是让指定行高亮显示，如果使用后再用GetSelectionMark函数来得到选中行，结果往往是错误的。
//	比如，先用鼠标点击选中第5行，调用GetSelectionMark函数得到的是4(第5行)，再调用SetItemState函数选中第2行，
//	然后调用GetSelectionMark函数得到的还是4(第5行)，所以，需要在设置选中行高亮显示时，一般与SetSelectionMark函数连用：
//	m_list.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,LVIS_FOCUSED | LVIS_SELECTED);   //选中行
//	m_list.SetSelectionMark(nIndexs);
//
//	*/
//
//	return 0L;
//}
