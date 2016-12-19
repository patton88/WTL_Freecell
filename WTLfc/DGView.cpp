// DGView.cpp : implementation file
//

#include "stdafx.h"
//#include "JL.h"
//#include "JLDoc.h"
//#include "JLView.h"
#include "DGView.h"
#include "DlgDefGame.h"
#include "CardSize.h"
//用于支持洗牌的函数
#include <algorithm>
#include <functional>

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#endif

/////////////////////////////////////////////////////////////////////////////
// CDGWnd
//extern CJLDoc * AfxGetDocument();
//extern CJLView * AfxGetView();
//extern CCardSize g_fcCcs;

//IMPLEMENT_DYNCREATE(CDGWnd, CWnd)
//IMPLEMENT_DYNCREATE(CDGWnd, CWindowImpl)

//BOOL CDGWnd::PreTranslateMessage(MSG* pMsg)
//{
//	if (CFrameWindowImpl<CDGWnd>::PreTranslateMessage(pMsg))
//		return TRUE;
//
//	return FALSE;
//}
//
//BOOL CDGWnd::OnIdle()
//{
//	return FALSE;
//}

CDGWnd::CDGWnd()
{
	Init();
}

CDGWnd::~CDGWnd()
{
}


//BEGIN_MESSAGE_MAP(CDGWnd, CWnd)
//	//{{AFX_MSG_MAP(CDGWnd)
//	ON_WM_LBUTTONDOWN()
//	ON_WM_LBUTTONDBLCLK()
//	ON_WM_ERASEBKGND()
//	ON_WM_PAINT()
//	//}}AFX_MSG_MAP
//END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDGWnd diagnostics

//#ifdef _DEBUG
//void CDGWnd::AssertValid() const
//{
//	CWnd::AssertValid();
//}
//
//void CDGWnd::Dump(CDumpContext& dc) const
//{
//	CWnd::Dump(dc);
//}
//#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDGWnd drawing

LRESULT CDGWnd::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//void CDGWnd::OnPaint()
{
	//CPaintDC dc(this);
	//CJLView *pView = AfxGetView();
	CPaintDC dc(m_hWnd);

	//创建内存DC及位图
	CDC dcMem;
	dcMem.CreateCompatibleDC(dc);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(dc, g_fcCcs.CARD_WID, g_fcCcs.CARD_HEI);
	//绘制牌盒子
	//CBitmap *pOldBmp = dcMem.SelectObject(&bmp);
	HBITMAP hBmpOld = dcMem.SelectBitmap(bmp);
	//如果此局已编辑完整就在牌盒子的区域中显示提示
	if (GameIsValid()) {
		CFont font;
		font.CreatePointFont(20 * 10, TEXT("Arial"), dc);
		HFONT prevFont = dc.SelectFont(font);
		int prevMode = dc.SetBkMode(TRANSPARENT);
		WTL::CString str(TEXT("此局编辑完毕，请存档！"));
		dc.DrawText(str, str.GetLength(), RectOfBox(), DT_CENTER);
		dc.SetBkMode(prevMode);
		dc.SelectFont(prevFont);
	}
	for (UINT i = 52; i >= 1; --i) {
		CRect r = RectOf(i);
		UINT card = Get(i);
		if (!card) continue;
		//在内存DC中绘制此牌并贴到窗口中此牌的矩形位置上
		g_pView->DrawCard(CPoint(0, 0), card, &dcMem);
		dc.BitBlt(r.left, r.top, r.Width(), r.Height(), dcMem, 0, 0, SRCCOPY);
		if (m_iSrcCol == i) {
			dc.InvertRect(r);        //如果是选中状态则反色
		}
	}
	dcMem.SelectBitmap(hBmpOld);
	//绘制桌面
	//共八列牌，前四列每列七张，后四列每列六张，共计52张
	for (int i = 1; i <= 8; i++) {
		UINT m = (i <= 4 ? 7 : 6);
		for (UINT j = 1; j <= m; j++) {
			int idx = (i << 8) + j, card = Get(idx);
			CRect r = RectOf(idx);
			//如果无牌就画空框
			if (!card) {
				if (j < m) r.bottom += 5;
				CBrush OldBrush = dc.SelectBrush(g_pView->m_brushBkgnd);		// 画刷的变换放在这里，问题解决
				dc.RoundRect(r, CPoint(5, 5));
				dc.SelectBrush(OldBrush);	// 画刷的变换放在这里，问题解决
			}
			//否则绘制此牌 
			else {
				g_pView->DrawCard(r.TopLeft(), card, &dc);	 // 变换画刷导致该句调用始终报错。所以在前面进行画刷变换
				if (m_iSrcCol == idx) dc.InvertRect(r);
			}
		}
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CDGWnd message handlers
//双击桌面上的牌将会把该牌回收到牌盒子中
//void CDGWnd::OnLButtonDblClk(UINT nFlags, CPoint point) 
void CDGWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	//测试双击了哪张牌
	UINT hit = HitTest(point);
	//如果双击了桌面上的牌
	if (hit && !IS_CARD(hit) && Get(hit)) {
		if (m_iSrcCol) { //选中某牌
			CRect rBack = RectOf(m_iSrcCol);//当前选中的列的矩形的备份
			m_iSrcCol = 0;//取消选中
			InvalidateRect(rBack);
		}
		UINT card = Get(hit);
		Set(hit, 0);
		InvalidateRect(RectOf(hit));
		Set(card, card);//回收此列
		InvalidateRect(RectOf(card));
		m_iSrcCol = 0;
		m_bModified = true;
		SetTitle();
	}

	CheckGame();

	//CWnd::OnLButtonDblClk(nFlags, point);
}

//void CDGWnd::OnLButtonDown(UINT nFlags, CPoint point) 
void CDGWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	UINT hit = HitTest(point);
	if (!hit) {
		;
	}
	//无源牌
	else if (!m_iSrcCol) {
		if (Get(hit)) { //击中牌
			m_iSrcCol = hit;//选中此牌
			InvalidateRect(RectOf(hit));
		}
	}
	//有源牌
	else if (!Get(hit)) {
		//击中空桌 
		if (!IS_CARD(hit)) {
			//移入空桌
			UINT card = Get(m_iSrcCol);
			Set(m_iSrcCol, 0);
			InvalidateRect(RectOf(m_iSrcCol));
			Set(hit, card);
			InvalidateRect(RectOf(hit));

			m_iSrcCol = 0;
			m_bModified = true;

			SetTitle();
		}
		//击中空盒子，且源在桌面上
		else if (!IS_CARD(m_iSrcCol)) {
			UINT card = Get(m_iSrcCol);
			Set(m_iSrcCol, 0);
			InvalidateRect(RectOf(m_iSrcCol));
			Set(card, card);//回收此列
			InvalidateRect(RectOf(card));

			m_iSrcCol = 0;
			m_bModified = true;

			SetTitle();
		}
	}
	//击中牌
	else {
		CRect rBack = RectOf(m_iSrcCol);//当前选中的列的矩形的备份
		//击中源牌
		if (hit == m_iSrcCol) {
			m_iSrcCol = 0;//取消选中
		}
		else {
			m_iSrcCol = hit;//选中击中的牌
			InvalidateRect(RectOf(hit));
		}
		InvalidateRect(rBack);
	}

	CheckGame();

	//CWnd::OnLButtonDown(nFlags, point);
}
//击中测试，看看点击了哪张牌
UINT CDGWnd::HitTest(const CPoint &pt)
{
	//测试是否在牌盒子的某牌上点击
	for (UINT i = 1; i <= 52; i++) {
		CRect r = RectOf(i);
		if (r.PtInRect(pt)) return i;
	}
	//测试是否在桌面上放牌区点击
	for (int i = 1; i <= 8; i++) {
		UINT x = i <= 4 ? 7 : 6;//索引上限
		for (UINT j = 1; j <= x; j++) {
			UINT idx = (i << 8) + j;
			CRect r = RectOf(idx);
			if (r.PtInRect(pt)) return idx;
		}
	}
	return 0;
}
//计算指定的牌占据的矩形位置
CRect CDGWnd::RectOf(UINT pos)
{
	CRect cr;
	GetClientRect(cr);
	int wid = cr.Width() / 13;
	int hei = (cr.Height() - g_fcCcs.CARD_HEI) / (6 + 4 + 1);
	//牌盒子中的牌每张所在的矩形
	CRect r(0, 0, 0, 0);
	if (IS_CARD(pos)) {
		r.left = (13 - NUM(pos))*wid;
		r.right = r.left + wid;
		r.top = TYPE(pos)*hei;
		r.bottom = r.top + hei;
	}
	//桌面上每张牌所占据的矩形
	else {
		UINT col = pos >> 8;
		UINT idx = pos & 0x000000FF;
		ATLASSERT(col <= 8 && col >= 1 && idx >= 1);
		ATLASSERT(col <= 4 && idx <= 7 || col >= 4 && idx <= 6);

		CPoint p(0, hei * 5);
		int inter = (cr.Width() - 8 * g_fcCcs.CARD_WID) / 9;
		int x = (col - 1) * (inter + g_fcCcs.CARD_WID) + inter;
		int y = (4 + idx) * hei;
		r.SetRect(
			x,
			y,
			x + g_fcCcs.CARD_WID,
			y + ((col <= 4 && idx == 7 || col > 4 && idx == 6) ? g_fcCcs.CARD_HEI : hei)
			);
	}
	return r;
}
//在指定的位置存放给定的牌
void CDGWnd::Set(UINT pos, UINT card)
{
	//如果位置是在牌盒子中
	if (IS_CARD(pos)) {
		m_iBox[pos - 1] = card;
	}
	else {
		UINT col = pos >> 8;
		UINT idx = pos & 0x000000FF;
		ATLASSERT(col <= 8 && col >= 1 && idx >= 1);
		ATLASSERT(col <= 4 && idx <= 7 || col >= 4 && idx <= 6);

		if (col <= 4) {
			m_iTbl[(col - 1) * 7 + idx - 1] = card;
		}
		else {
			m_iTbl[28 + (col - 5) * 6 + idx - 1] = card;
		}
	}
}

//根据给定的索引位置取出此处的牌值
UINT CDGWnd::Get(UINT pos)
{
	//如果是牌盒子中的牌
	if (IS_CARD(pos)) {
		return m_iBox[pos - 1];
	}
	//否则就是桌面上的牌
	else {
		UINT col = pos >> 8;
		UINT idx = pos & 0x000000FF;
		ATLASSERT(col <= 8 && col >= 1 && idx >= 1);
		ATLASSERT(col <= 4 && idx <= 7 || col >= 4 && idx <= 6);

		if (col <= 4) {
			return m_iTbl[(col - 1) * 7 + idx - 1];
		}
		else {
			return m_iTbl[28 + (col - 5) * 6 + idx - 1];
		}
	}
}

//BOOL CDGWnd::OnEraseBkgnd(CDC* pDC) 
LRESULT CDGWnd::OnEraseBkgnd(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	HDC hDc = (HDC)wParam;
	CRect cr;
	GetClientRect(cr);
	FillRect(hDc, cr, g_pView->m_brushBkgnd);
	return 1;
}


void CDGWnd::NewDefGame()
{
	if (!GiveUp()) return;

	Init();
	InvalidateRect(NULL);
	SetTitle();
}

void CDGWnd::SaveDefGameAs()
{
	WTL::CString name = m_strFile;
	bool bModi = m_bModified;

	m_strFile.Empty();
	m_bModified = true;

	SaveDefGame();

	if (m_strFile.IsEmpty()) {
		m_strFile = name;
		m_bModified = bModi;
	}
}

void CDGWnd::SaveDefGame()
{
	struct SIZE_INF { UINT size, *pAddr; } cols[3] = {
		{ sizeof(m_iCards) / sizeof(UINT), &m_iCards[0][0] },
		{ sizeof(m_iBuffer) / sizeof(UINT), &m_iBuffer[0] },
		{ sizeof(m_iRecycle) / sizeof(UINT), &m_iRecycle[0][0] }
	};
	if (!m_bModified) return;

	if (m_strFile.IsEmpty()) {
		//CFileDialog dlg(FALSE, TEXT("rep"), TEXT("自定义牌局"), dwFlags, filter);
		//if (dlg.DoModal() == IDCANCEL) return;
		//m_strFile = dlg.GetPathName();
		CFileDialog fd(FALSE, L"txt", TEXT("自定义牌局"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0\0", m_hWnd);
		if (fd.DoModal() == IDCANCEL) return;
		fd.m_ofn.lpstrInitialDir = L".";
		m_strFile = fd.m_szFileName;

		SetTitle();
	}
	//CFile file(m_strFile, modeCrWr);
	//CArchive ar(&file, CArchive::store);

	// Storing
	CXFile file1;
	file1.Open(wstring(m_strFile),						// file name
		GENERIC_WRITE | GENERIC_READ,			// access mode 
		FILE_SHARE_READ | FILE_SHARE_WRITE,		// share mode 
		NULL,									// no security 
		CREATE_ALWAYS,							// create a new file, overwrite if it exists
		FILE_ATTRIBUTE_NORMAL,					// file attributes
		NULL);									// no template file

	CXArchive ar1(&file1, CXArchive::store);
	g_fcData.Serialize(ar1);

	CTList<COperations> ops;
	CDlgScore dlgScore;

	if (GameIsValid()) {
		////////////////////////////////////////////////////////////
		//完整的自定义牌局，其牌局代号为0，
		//保存时存为一个完整的牌局存档文件
		////////////////////////////////////////////////////////////
		Shuffle();//洗牌
		m_nCurGameNumber = 0;
		ar1 << m_nCurGameNumber;			//1、保存本局代号
		//CObList ops;
		//m_OpsList.Serialize(ar);//保存步骤记录
		ops.Serialize(ar1);				//2、保存空的步骤记录
		for (UINT k = 0; k < 3; ++k)		//3、保存牌局
			for (UINT i = 0; i < cols[k].size; i++)
				ar1 << cols[k].pAddr[i];

		//m_dlgScore.m_score.Serialize(ar);//保存战况记录
		dlgScore.m_score.Serialize(ar1);//4、保存空的战况记录
	}
	else {
		////////////////////////////////////////////////////////////
		//不完整的自定义牌局，其牌局代号为-1，
		//保存时只保存当前编辑时有用的信息，便于下次再编辑
		////////////////////////////////////////////////////////////
		m_nCurGameNumber = -1;
		ar1 << m_nCurGameNumber;			//1、保存本局代号
		for (UINT i = 0; i < 52; i++) { ar1 << m_iTbl[i]; }	//2、保存桌面
		for (int i = 0; i < 52; i++) { ar1 << m_iBox[i]; }	//3、保存牌盒子
		dlgScore.m_score.Serialize(ar1);//4、保存空的战况记录
	}
	//ar.Close();
	//file.Close();

	ar1.Close();
	// CXFile file1 由 ~CXFile 关闭

	m_bModified = false;

	SetTitle();
}
//读档，根据牌局代号决定如何读档：
//如果牌局代号为0则说明是完整的自定义牌局，
//	此时根据步骤记录判断此局是否走过若干步，如果是则提示玩家先撤消到开局状态并存档后编辑，
//	否则就说明牌局是完整的且没有走过，那么就读出牌盒子和桌面；
//如果牌局代号大于0则说明是标准牌局的存档文件，
//	此时就重新生成一个原始牌局；
//如果牌局代号为-1则说明是未编辑完整的自定义牌局，
//	此时就读出牌盒子和桌面。
void CDGWnd::LoadDefGame()
{
	struct SIZE_INF { UINT size, *pAddr; } cols[3] = {
		{ sizeof(m_iCards) / sizeof(UINT), &m_iCards[0][0] },
		{ sizeof(m_iBuffer) / sizeof(UINT), &m_iBuffer[0] },
		{ sizeof(m_iRecycle) / sizeof(UINT), &m_iRecycle[0][0] }
	};

	if (!GiveUp()) return;

	//CFileDialog dlg(TRUE, TEXT("rep"), NULL, dwFlags, filter);
	//if (dlg.DoModal() == IDCANCEL) return;
	CFileDialog fd(TRUE, L"txt", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0\0", m_hWnd);
	fd.m_ofn.lpstrTitle = L"ReadFileTest";
	fd.m_ofn.lpstrInitialDir = L".";

	if (fd.DoModal() == IDCANCEL) return;

	Init();

	//CFile file(dlg.GetPathName(), modeRead);
	//CArchive ar(&file, CArchive::load);

	// Loading
	CXFile file2;
	file2.Open(fd.m_szFileName,					// file name
		GENERIC_WRITE | GENERIC_READ,			// access mode 
		FILE_SHARE_READ | FILE_SHARE_WRITE,		// share mode 
		NULL,									// no security 
		//CREATE_ALWAYS,							// create a new file, overwrite if it exists
		OPEN_EXISTING,							// open the file, if it exists
		FILE_ATTRIBUTE_NORMAL,					// file attributes
		NULL);									// no template file

	CXArchive ar2(&file2, CXArchive::load);

	CTList<COperations> ops;
	CDlgScore dlgScore;

	//读取本局代号
	ar2 >> m_nCurGameNumber;					//1、读取牌局代号

	//如果是未编辑完成的牌局
	if (m_nCurGameNumber == -1)
	{
		for (UINT i = 0; i < 52; i++) { ar2 >> m_iTbl[i]; }	//2、读取桌面
		for (int i = 0; i < 52; i++) { ar2 >> m_iBox[i]; }	//3、读取牌盒子
		dlgScore.m_score.Serialize(ar2);		//4、读取战况记录
	}
	//如果是标准牌局的存档
	else if (m_nCurGameNumber > 0)
	{
		//准备一副新牌，并洗牌
		int cards[52];
		for (int i = 1; i <= 52; ++i) cards[i - 1] = i;

		srand(m_nCurGameNumber >> 16);
		random_shuffle(cards, cards + 52);
		srand(m_nCurGameNumber & 0xFFFF);
		random_shuffle(cards, cards + 52);

		for (int i = 0; i < 52; i++) { m_iTbl[i] = cards[i]; }	//发牌到桌面
		for (int i = 0; i < 52; i++) { m_iBox[i] = 0; }			//准备空的牌盒子
	}
	//已经完成的自定义牌局
	else
	{
		ops.Serialize(ar2);						//2、读取步骤记录
		UINT nSteps = ops.m_tList.size();
		//ClrOpsRecords(&ops);		//清除步骤记录
		ops.m_tList.clear();			//清除步骤记录
		if (nSteps == 0) {
			for (UINT k = 0; k < 3; ++k)			//3、读取牌局
				for (UINT i = 0; i < cols[k].size; i++)
					ar2 >> cols[k].pAddr[i];

			for (int col = 0; col < 4; col++)//准备牌局前4列
				for (int idx = 0; idx < 7; idx++)
					m_iTbl[col * 7 + idx] = m_iCards[col][idx];
			for (int col = 4; col < 8; col++)//准备牌局后4列
				for (int idx = 0; idx < 6; idx++)
					m_iTbl[28 + (col - 4) * 6 + idx] = m_iCards[col][idx];

			for (UINT i = 0; i < 52; i++) { m_iBox[i] = 0; }//准备空的牌盒子

			dlgScore.m_score.Serialize(ar2);		//4、读取战况记录
			dlgScore.m_score.m_tList.clear();	//清除战况记录

		}
		else
		{
			ar2.Close();//关闭文件
			//file.Close();
			WTL::CString errStr;
			errStr.LoadString(IDS_RESON_UNEDITABLE);
			MessageBox(errStr);
			return;
		}
	}
	//ar.Close();//关闭文件
	//file.Close();

	ar2.Close();
	// CXFile file2 由 ~CXFile 关闭

	m_strFile = fd.m_szFileName;
	m_bGameIsValid = GameIsValid();

	InvalidateRect(NULL);
	SetTitle();
}
//洗牌并发牌到桌面
void CDGWnd::Shuffle()
{
	//清空缓存列、回收列和牌列
	struct SIZE_INF { UINT size, *pAddr; };
	const SIZE_INF cols[3] = {
		{ sizeof(m_iCards) / sizeof(UINT), &m_iCards[0][0] },
		{ sizeof(m_iBuffer) / sizeof(UINT), &m_iBuffer[0] },
		{ sizeof(m_iRecycle) / sizeof(UINT), &m_iRecycle[0][0] },
	};
	for (UINT k = 0; k < 3; ++k)
		for (UINT i = 0; i < cols[k].size; i++)
			cols[k].pAddr[i] = 0;
	//发牌
	for (int col = 0; col <= 3; col++) {
		UINT *pTop;

		pTop = &m_iCards[col][0];
		for (UINT i = 0; i < 7; i++) {
			UINT *pDes = &pTop[pTop[19]];//指向底牌之下
			pDes[i] = m_iTbl[col * 7 + i];
		}
		pTop[19] = 7;

		pTop = &m_iCards[col + 4][0];
		for (int i = 0; i < 6; i++) {
			UINT *pDes = &pTop[pTop[19]];//指向底牌之下
			pDes[i] = m_iTbl[28 + col * 6 + i];
		}
		pTop[19] = 6;
	}
}

//////////////////////////////////////////////////////////////////////
//看看牌局是否编辑完全
//牌盒子中的牌全部拿到桌面上了，则说明编辑完成，牌局是完整的了
//////////////////////////////////////////////////////////////////////
bool CDGWnd::GameIsValid()
{
	for (UINT i = 0; i < 52; i++) {
		if (m_iBox[i]) return false;
	}
	return true;
}

//根据牌局是否修改过决定是否提示存档
BOOL CDGWnd::GiveUp()
{
	return !m_bModified || IDYES ==
		MessageBox(L"牌局已经更改，尚未存档，要放弃它吗？", L"自定义牌局", MB_YESNO);
}
//清除步骤记录
//void CDGWnd::ClrOpsRecords(CObList *pList)
//{
//	POSITION p = pList->GetHeadPosition();
//	while(p) {
//		COperations *pOps = (COperations *)pList->GetNext(p);
//		pOps->ClrOps();
//		delete pOps;
//	}
//	pList->RemoveAll();
//}

void CDGWnd::Init()
{
	m_bGameIsValid = false;
	m_strFile.Empty();
	m_iSrcCol = 0;
	for (UINT i = 0; i < 52; i++) {
		m_iTbl[i] = 0;//桌面上无牌
		m_iBox[i] = i + 1;//准备一副新牌放入牌盒子中
	}
	m_bModified = false;
}
//根据程序当前状态决定在标题栏中显示什么
//如果没有存档且已改动则在文件名后加上*号
void CDGWnd::SetTitle()
{
	WTL::CString title = L"自定义牌局";
	const WTL::CString strModi = "*";

	if (!m_strFile.IsEmpty()) {
		int idx = m_strFile.ReverseFind('\\');
		title = m_strFile.Right(m_strFile.GetLength() - idx - 1);
	}
	if (m_bModified) {
		title += strModi;
	}
	//CDlgDefGame *pDlg = (CDlgDefGame *)GetParent();
	//pDlg->SetWindowText(title);
	SetWindowText(title);
}

void CDGWnd::CheckGame()
{
	//根据当前牌面判断自定义牌局是否编辑完整
	bool isValidNow = GameIsValid();
	//如果当前状态和点击前的状态不同则刷新牌盒子
	//也就是说决定是否显示牌局是否编辑完整的提示信息
	if (m_bGameIsValid ^ isValidNow) {
		m_bGameIsValid = isValidNow;
		InvalidateRect(RectOfBox());
	}
}
//计算整个牌盒子的矩形
CRect CDGWnd::RectOfBox()
{
	CRect r;
	r.UnionRect(RectOf(1), RectOf(52));
	return r;
}

LRESULT CDGWnd::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//EndDialog(wID);
	return 0;
}

LRESULT CDGWnd::OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// First DDX call, hooks up controls to variables.
	//DoDataExchange(true);

	//EndDialog(wID);
	return 0;
}

LRESULT CDGWnd::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	//CenterWindow(GetParent());

	//m_strGameNumber.Format(L"%d", m_nGameNumber);

	//// First DDX call, hooks up variables to controls.
	//DoDataExchange(false);

	return 0;
}
