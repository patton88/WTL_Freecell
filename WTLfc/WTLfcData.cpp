// WTLfcData.cpp : implementation of the CJLDoc class
//

#include "stdafx.h"

#include "WTLfcData.h"
#include "Datatype.h"
#include <time.h>
#include "WTLfcView.h"
#include "DlgPassed.h"
#include "DlgAIShow.h"

//用于支持洗牌的函数
#include <algorithm>
#include <functional>

/////////////////////////////////////////////////////////////////////////////
// WTLfcData

#define FitTrash(card,x) (TYPE(x)==TYPE(card)&&NUM(x)==NUM(card)-1)

CWTLfcData::CWTLfcData()
{
	m_nSel = 0;
	//m_pOps = new CObList;
	//m_pOps = new list<CMyObject*>;
	m_Hints.ClrHints();

	srand(UINT(time(NULL)));
	m_nCurGameNumber = Random();

	m_bEnableAlert = false;
	m_bQuickMove = false;
	m_bEnableDbClick = true;
	m_bMaxMove = true;
	m_bAICalRunning = false;
	m_bRealTimeUpdate = false;
	m_nDDASpeed = 75;
}

CWTLfcData::~CWTLfcData()
{
	//ClrOpsRecords();
	//delete m_pOps;
}


/////////////////////////////////////////////////////////////////////////////
// CJLDoc commands
bool CWTLfcData::IsCol(UINT col)
{
	return (col <= 16 && col >= 1);
}


// +-----Buf----+    +---Recycle---+
// | 9 10 11 12 | JL | 13 14 15 16 |
// +------------+    +-------------+
// +------------Cards--------------+
// | 1   2   3   4   5   6   7   8 |
// +-------------------------------+
//核心的移牌程序：将src列的n张牌移动到des列
void CWTLfcData::MoveCards(UINT des, UINT src, UINT n)
{
	ATLASSERT(IsCol(src) && !IsEmptyCol(src) //源列非空
		&& n <= CntSeriIn(src) //最多可移动全部序列牌
		&& IsCol(des));

	//取消当前选中
	if (!m_bAICalRunning) UnselectCardCol();

	CRect rSrc, rDes;
	UINT *pSrc, *pDes, *pTop;
	if (src <= 8) {
		pTop = &m_iCards[src - 1][0];
		pSrc = &pTop[pTop[19] - 1];//指向底牌
		//刷新移走的部分
		rSrc = RectOf(src, pTop[19] - n + 1, n);
		//改变牌的计数
		pTop[19] -= n;
		//刷新整列
		if (pTop[19] + n > 13) {
			rSrc.UnionRect(rSrc, RectOf(src, 1, pTop[19]));
		}
	}
	else if (src <= 12) {
		pSrc = &m_iBuffer[src - 9];
		rSrc = RectOf(src, 1, 1);
	}
	else {
		pTop = &m_iRecycle[src - 13][0];
		pSrc = &pTop[pTop[13] - 1];//指向底牌
		pTop[13] -= n;//改变牌的计数
		ATLASSERT(n == 1);
		rSrc = RectOf(src, 1, 1);
	}

	if (des <= 8) {
		pTop = &m_iCards[des - 1][0];
		pDes = &pTop[pTop[19]];//指向底牌之下
		//刷新移来的部分
		rDes = RectOf(des, pTop[19] + 1, n);
		//改变牌的计数
		pTop[19] += n;
		//刷新整列
		if (pTop[19] > 13) {
			rDes.UnionRect(rDes, RectOf(des, 1, pTop[19]));
		}
	}
	else if (des <= 12) {
		pDes = &m_iBuffer[des - 9];
		rDes = RectOf(des, 1, 1);
		ATLASSERT(!m_iBuffer[des - 9] && n == 1);
	}
	else {
		pTop = &m_iRecycle[des - 13][0];
		pDes = &pTop[pTop[13]];//指向底牌之下
		pTop[13] += n;//改变牌的计数
		ATLASSERT(n == 1);
		rDes = RectOf(des, 1, 1);
	}

	UINT *p = pSrc + 1 - n;//p指向最上面那张将要被移动的牌
	for (UINT i = 0; i < n; i++) {
		*pDes++ = *p;//移动到目标处
		*p++ = 0;//源牌清零
	}

	if (m_bAICalRunning && !m_bRealTimeUpdate) {
		return;
	}
	g_pView->InvalidateRect(RectOfStep());//刷新步数信息
	g_pView->InvalidateRect(rSrc);//刷新源列牌面
	g_pView->InvalidateRect(rDes);//刷新目标列牌面
}

//按照规则f的条件判断 a可放在b下 这一论断对两张牌a，b是否成立
//规则f如下：
//    红牌可以放在黑牌下，黑牌可以放在红牌下
//    但是必须保证大点数的牌在上，小点数的牌在下
//    且点数只能相差1点
//    例如：
//        照此规则，红桃5下只可以放黑桃4或者梅花4
//    
bool CWTLfcData::FitFormula(UINT b, UINT a)
{
	ATLASSERT(a <= 52 && a >= 1 && b <= 52 && b >= 1);
	//Type()   =  0 黑桃   1 红桃   2 梅花   3 方块
	//b，a不同花色且b的点数比a大一点
	return (TYPE(a) + TYPE(b)) % 2 == 1 && NUM(b) - NUM(a) == 1;
}
//洗牌
void CWTLfcData::Shuffle()
{
	/*
	//准备一副新牌，并洗牌
	using namespace std;
	vector<int> cards(52);
	for(int i = 1, *it = cards.begin(); it != cards.end(); *it++ = i++) ;

	srand(m_nCurGameNumber >> 16);
	random_shuffle(cards.begin(),cards.end());
	srand(m_nCurGameNumber & 0xFFFF);
	random_shuffle(cards.begin(),cards.end());
	*/
	//准备一副新牌，并洗牌
	int cards[52];
	for (int i = 1; i <= 52; ++i) cards[i - 1] = i;

	using namespace std;

	srand(m_nCurGameNumber >> 16);
	random_shuffle(cards, cards + 52);
	srand(m_nCurGameNumber & 0xFFFF);
	random_shuffle(cards, cards + 52);

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
	//发牌到牌列m_iCards
	for (int col = 0; col <= 3; col++) {
		UINT *pTop;

		pTop = &m_iCards[col][0];
		for (int i = 0; i < 7; i++) {
			UINT *pDes = &pTop[pTop[19]];//指向底牌之下
			pDes[i] = cards[col * 7 + i];
		}
		pTop[19] = 7;

		pTop = &m_iCards[col + 4][0];
		for (int i = 0; i < 6; i++) {
			UINT *pDes = &pTop[pTop[19]];//指向底牌之下
			pDes[i] = cards[28 + col * 6 + i];
		}
		pTop[19] = 6;
	}
}

void CWTLfcData::SelectCardCol(UINT col)
{
	ATLASSERT(IsCol(col) && !IsEmptyCol(col));

	//（如果有）取消当前选中
	UnselectCardCol();

	//选中另一列并刷新
	m_nSel = col;
	InvalidateRect(RectOf(col, CntCardsIn(col), 1));
}
//不选中此列
void CWTLfcData::UnselectCardCol()
{
	//游戏刚开始、玩家取消选中、有
	//牌移动等都会导致无任何列被选

	//如果没有选中任何列就不管
	if (!m_nSel) return;

	CRect r = RectOf(m_nSel, CntCardsIn(m_nSel), 1);
	//取消选中并刷新
	m_nSel = 0;
	InvalidateRect(r);
}
//看看此列是否为空
bool CWTLfcData::IsEmptyCol(UINT col)
{
	ATLASSERT(IsCol(col));
	if (col <= 8) {
		return !m_iCards[col - 1][19];
	}
	else if (col <= 12) {
		return !m_iBuffer[col - 9];
	}
	else {
		return !m_iRecycle[col - 13][13];
	}
}

// 计算实际允许从被选中列移动多少张纸牌到目标列
//（计算出来之后可以利用函数MoveCards来进行实际的移动）
UINT CWTLfcData::CntMaxMv(UINT desCol, UINT srcCol)
{
	ATLASSERT(IsCol(srcCol) && !ColInRecycle(srcCol) && !IsEmptyCol(srcCol));
	ATLASSERT(IsCol(desCol));

	UINT n = 0;
	//目标列是牌列
	if (desCol <= 8) {
		if (COL_IN_BUFF(srcCol)) { //源列是缓存列
			if (IsEmptyCol(desCol) ||
				FitFormula(BottCard(desCol), BottCard(srcCol)))
				n = 1;
		}
		else {
			//源列是牌列
			UINT nSeri = CntSeriIn(srcCol);//计算连续多少张牌
			if (IsEmptyCol(desCol)) { //目标列是空牌列
				UINT maxSuppliment = CntMaxSuppliment(true);
				//肯定可以移动
				n = min(maxSuppliment, nSeri);
			}
			else {
				UINT bottSrc = BottCard(srcCol);//源列最下面的牌
				UINT bottDes = BottCard(desCol);//目标列最下面的牌
				UINT numSrc = NUM(bottSrc);//牌点数
				UINT numDes = NUM(bottDes);//牌点数
				n = numDes - numSrc;
				UINT maxSuppliment = CntMaxSuppliment(false);
				//必须严格满足以下条件才可以移动：
				if ( 	//目标牌点数介于源序列牌之上的指定区间内 且
					numDes >= numSrc + 1 && numDes <= numSrc + nSeri &&
					//它比源牌大奇数点且红黑相异或大偶数点红黑相同 且
					n % 2 == (TYPE(bottSrc) + TYPE(bottDes)) % 2 &&
					//有足够空间来移动
					n <= maxSuppliment)
				{
					;
				}
				else {
					n = 0;
				}
			}
		}
	}
	else if (desCol <= 12) { //目标列是缓存列
		if (IsEmptyCol(desCol))
			n = 1;//缓存列无牌则可移动一张
	}
	else { //目标列是回收列
		int s = BottCard(srcCol);
		if (!IsEmptyCol(desCol)) {
			int d = BottCard(desCol);
			if (TYPE(s) == TYPE(d) && NUM(d) == NUM(s) - 1)
				n = 1;//花色相符，点数小一，则可以回收
		}
		else if (NUM(s) == 1 && TYPE(s) + 13 == desCol)
			n = 1;//是A且花色相符（且相应回收列中无牌）
	}

	return n;
}

//遍历各列并自动扔出1-12列中最小的牌直到无法扔出为止
void CWTLfcData::AutoThrow()
{
	UINT colSrc, cardSrc, numSrc, colDes, sons[2];
	while (true) { //直到没有牌可扔为止
		for (colSrc = 1; colSrc <= 12; colSrc++) { //寻找可扔的牌所在的列
			if (IsEmptyCol(colSrc)) continue;
			cardSrc = BottCard(colSrc);
			if (!Trashable(cardSrc)) continue;
			numSrc = NUM(cardSrc);
			colDes = TYPE(cardSrc) + 13;
			if (numSrc == 1 || numSrc == 2) break;
			if (m_bAICalRunning) break;//自动解答时废牌能扔就扔

			//考虑子牌是否已经回收
			sons[0] = sons[1] = colDes;
			sons[0] -= colDes > 13 ? 1 : -3;
			sons[1] += colDes < 16 ? 1 : -3;
			if (
				m_iRecycle[sons[0] - 13][13] && //子牌的回收列非空
				m_iRecycle[sons[1] - 13][13] && //子牌的回收列非空
				NUM(BottCard(sons[0])) >= numSrc - 1 &&
				NUM(BottCard(sons[1])) >= numSrc - 1
				) break;
		}
		if (colSrc > 12) break;
		if (!m_bQuickMove && ColInCard(colSrc)) { //快速移动的时候没有动画
			CRect rs = RectOf(colSrc, CntCardsIn(colSrc), 1);
			CRect rd = RectOf(colDes, 1, 1);
			::LineDDA(rs.left, rs.top, rd.left, rd.top, LineDDACallback, cardSrc);
			// 动画很慢死锁的原因是 g_fcData.m_nDDASpeed 没有初始化
		}
		MoveCards(colDes, colSrc, 1);

		//if (m_pOps->empty())
		if (m_OpsList.empty())
		{
			//Record(new COperations(colDes, colSrc, 1));
			Record(colDes, colSrc, 1);
		}
		else
		{ //扔牌后的自动扔牌动作必须和扔牌动作放在一起
			// GetTail	返回列表中的尾元素（列表不能为空）
			//((COperations*)m_pOps->back())->AddOperation(colDes, colSrc, 1);
			m_OpsList.back().AddOperation(colDes, colSrc, 1);
		}
	}
}

//测试是否游戏结束
bool CWTLfcData::GameOver()
{
	//如果所有牌都在回收列了则游戏结束
	for (UINT i = 13; i <= 16; i++) {
		if (m_iRecycle[i - 13][13] == 13) continue;
		return false;
	}
	return true;
}

//按照规则计算此列有几张牌是顺序存放的
UINT CWTLfcData::CntSeriIn(UINT col)
{
	//我们认为缓存列和回收列的序列牌长为1
	ATLASSERT(IsCol(col) && !IsEmptyCol(col));
	UINT nSeri = 1;//非空的缓存列序列牌数为1
	if (col <= 8) {
		UINT *pTop, *p1, *p2;
		pTop = &m_iCards[col - 1][0];
		p1 = &pTop[pTop[19] - 1];//指向底牌
		p2 = p1 - 1;//p2指向p1上面的牌
		while (p2 >= pTop && FitFormula(*p2--, *p1--)) ++nSeri;
	}
	return nSeri;
}

//计算给定的牌列中有多少张牌。Cnt 为 Count的缩写
UINT CWTLfcData::CntCardsIn(UINT col)
{
	ATLASSERT(IsCol(col));

	if (col <= 8) {
		return m_iCards[col - 1][19];
	}
	else if (col <= 12) {
		return m_iBuffer[col - 9] ? 1 : 0;
	}
	else {
		return m_iRecycle[col - 13][13];
	}
}


//  | 1 ... 13 | 14 ... 26 | 27 ... 39  | 40 ... 52 |
//  | A ...  K |  A ...  K |  A ...  K  |  A ...  K |
//  | －黑桃－ |  －红桃－ |  －梅花－  |  －方块－ |
UINT CWTLfcData::Num(UINT card) { return (card - 1) % 13 + 1; }//点数 1－13
UINT CWTLfcData::Type(UINT card) { return (card - 1) / 13; }//花色: 0黑桃 1红桃 2梅花 3方块

//假设目前有连续的无数张牌等待移动
//计算目前空出的无牌列(可用空间)最多可供一次性移动多少张牌
//OccupyAnEmptyCol指出在计算时是否使用全部可用空间
//如果否，则在计算最多允许一次性移动多少张牌时牌列中的可用
//空间会少计一个
UINT CWTLfcData::CntMaxSuppliment(bool OccupyAnEmptyCol)
{
	int a = 0, b = 0;
	//统计空牌列数 
	for (UINT i = 1; i <= 8; i++) {
		if (m_iCards[i - 1][19] == 0) ++b;
	}
	//统计空档数
	for (int i = 9; i <= 12; i++) {
		if (m_iBuffer[i - 9] == 0) ++a;
	}
	//有一个空列将会被作为目标牌列
	if (OccupyAnEmptyCol) {
		//可往空牌列移动的牌数在由人工来玩牌时只与空档有关
		if (!m_bAICalRunning) return a + b;
		//其他任何情况下都一样
		ATLASSERT(b);
		--b;
	}
	return a*(b + 1) + b*(b + 1) / 2 + 1;
}
//取出给定列的底牌
UINT CWTLfcData::BottCard(UINT col)
{
	ATLASSERT(IsCol(col) && !IsEmptyCol(col));

	if (col <= 8) {
		UINT *pTop = &m_iCards[col - 1][0];
		return pTop[pTop[19] - 1];//指向底牌
	}
	else if (col <= 12) {
		return m_iBuffer[col - 9];
	}
	else {
		UINT *pTop = &m_iRecycle[col - 13][0];
		return pTop[pTop[13] - 1];//指向底牌
	}
}

bool CWTLfcData::ColInCard(UINT col) {
	return (col <= 8 && col >= 1);
}

bool CWTLfcData::ColInBuf(UINT col) {
	return (col <= 12 && col >= 9);
}

bool CWTLfcData::ColInRecycle(UINT col) {
	return (col <= 16 && col >= 13);
}

bool CWTLfcData::IsCard(UINT card)
{
	return (card >= 1 && card <= 52);
}

void CWTLfcData::InvalidateRect(CRect r)
{
	//如果正在自动解答，那么有可能你最需要的是计算速度而不是
	//时实看到牌局状态，所以自动解答时，如果你关闭时实显示牌局
	//这一选项，就能大大加快解答速度

	//在时实显示牌局的条件下虽然解答速度较慢，但是
	//大部分的（大概有90%以上的）牌局通常都能在很短的时间内解答出来
	//“实时显示牌局状态”这一选项默认是打开的，毕竟自动解答时牌局的
	//演变过程看起来还是有点意思的。我们还是希望知道电脑都在干些什么。
	//计算出无效区域并刷新
	if (m_bAICalRunning && !m_bRealTimeUpdate)return;
	//CJLView *pView = GetView();
	g_pView->InvalidateRect(r);
	SendMessage(g_pView->m_hWnd, WM_PAINT, 0, 0);
}

// Record(new COperations(colDes, colSrc, 1));
//void CWTLfcData::Record(CMyObject *thisStep)
void CWTLfcData::Record(UINT des, UINT src, UINT n)
{
	//增加一步记录并刷新步数信息
	//m_pOps->push_back(thisStep);
	m_OpsList.push_back(COperations(des, src, n));
	InvalidateRect(RectOfStep());
}

//撤消
//void CWTLfcData::OnUndo()
//{
//	Undo();
//	GetHints();
//}
//执行撤消动作
void CWTLfcData::Undo()
{
	//::MessageBoxW(NULL, L"Undo Test", NULL, NULL);
	//if (m_pOps->IsEmpty()) return;
	if (m_OpsList.empty()) return;

	//撤销一步
	//COperations *pOpsLast = (COperations*)m_pOps->back();
	COperations& OpsLast = m_OpsList.back();
	// GetTail	返回列表中的尾元素（列表不能为空）
	//list<CMyObject*>* pOps = pOpsLast->m_pObjlist;
	list<COperation>& ops = OpsLast.m_OpList;

	for (list<COperation>::iterator it = ops.begin(); it != ops.end(); it++)
	{
		COperation& op = *it;
		MoveCards(op.src, op.des, op.cnt);
	}
	//pOpsLast->ClrOps();
	//delete pOpsLast;
	m_OpsList.pop_back();

	InvalidateRect(RectOfStep());

	//CObList
	//GetHeadPosition返回的是链表头元素的位置
	//CObList..GetNext(pos)先返回链表中pos所指元素，然后将pos指向下一个元素。调用GetNext后pos的值就改变了
	//微软脑壳有包！
		

	////撤销一步
	//COperations *pOpsLast = (COperations*)m_pOps->GetTail();
	//CObList *pOps = pOpsLast->pOps;
	//POSITION posMov = pOps->GetHeadPosition();
	//while (posMov) {
	//	COperation *pOp = (COperation*)pOps->GetNext(posMov);
	//	MoveCards(pOp->src, pOp->des, pOp->cnt);
	//}
	//pOpsLast->ClrOps();
	//delete pOpsLast;
	//m_pOps->RemoveTail();

	//InvalidateRect(RectOfStep());
}

//游戏返回到开头但是保留步骤记录，准备回放
void CWTLfcData::BackHome()
{
	int nSteps = m_OpsList.size();

	//还原牌局但保留步骤记录
	while (nSteps > 0)
	{
		COperations *pOpsLast = getAt(m_OpsList, --nSteps);
		list<COperation>& Ops = pOpsLast->m_OpList;

		list<COperation>::iterator it = Ops.begin();
		for (; it != Ops.end(); it++)
		{
			COperation& op = *it;
			MoveCards(op.src, op.des, op.cnt);
		}
		InvalidateRect(RectOfStep());
	}
}

//void WTLfcData::OnSetting()
//{
//	// TODO: Add your command handler code here
//
//	CDlgSettings dlg;
//	//set values of settings
//	dlg.m_bEnableAlert = m_bEnableAlert;
//	dlg.m_bEnableDBClick = m_bEnableDbClick;
//	dlg.m_bQuickMove = m_bQuickMove;
//	dlg.m_bMaxMove = m_bMaxMove;
//	dlg.m_nDDASpeed = m_nDDASpeed;
//
//	//prompt
//	dlg.DoModal();
//
//	//get settings
//	m_bEnableAlert = dlg.m_bEnableAlert;
//	m_bEnableDbClick = dlg.m_bEnableDBClick;
//	m_bQuickMove = dlg.m_bQuickMove;
//	m_bMaxMove = dlg.m_bMaxMove;
//	m_nDDASpeed = dlg.m_nDDASpeed;
//}
//
//void WTLfcData::DeleteContents()
//{
//	// TODO: Add your specialized code here and/or call the base class
//	ClrOpsRecords();
//
//	CDocument::DeleteContents();
//}
//
//计算第col列从第idx张牌开始的nCards张牌占据的矩形
//它可为选择，移动和重画函数提供绘图信息
CRect CWTLfcData::RectOf(UINT col, UINT idx, UINT nCards)
{
	ATLASSERT(IsCol(col));
	CPoint org = g_fcCcs.ptOrg;

	CRect r;
	if (col <= 8) {
		org.x += g_fcCcs.CARD_INT;
		org.y += g_fcCcs.CARD_HEI + g_fcCcs.PILE_VINT;
		r.left = org.x + (g_fcCcs.CARD_WID + g_fcCcs.CARD_INT)*(col - 1);
		r.right = r.left + g_fcCcs.CARD_WID;
		//超过十三张牌后每增加一张牌，露出部分的高度就减少1个象素
		int n = m_iCards[col - 1][19] - 13;
		if (n > 0)			//超过十三张牌后每增加一张牌，露出部分的高度就减少2*ccs.dRate个象素
		{
			n = int((n + 1) * g_fcCcs.dRate);
		}
		int h = g_fcCcs.CARD_UNCOVER - max(n, 0);
		r.top = org.y + (idx - 1)*h;
		r.bottom = r.top + (nCards - 1)*h + g_fcCcs.CARD_HEI;
	}
	else if (col <= 12) {
		r.left = org.x + (col - 9)*g_fcCcs.CARD_WID;
		r.top = org.y;
		r.right = r.left + g_fcCcs.CARD_WID;
		r.bottom = r.top + g_fcCcs.CARD_HEI;
	}
	else {
		org.x += 4 * g_fcCcs.CARD_WID + g_fcCcs.PILE_HINT;

		r.left = org.x + (col - 13)*g_fcCcs.CARD_WID;
		r.top = org.y;
		r.right = r.left + g_fcCcs.CARD_WID;
		r.bottom = r.top + g_fcCcs.CARD_HEI;
	}
	return r;
}

//#include "mainfrm.h"
//#include "JLView.h"

//绘制自动扔牌过程动画的函数LineDDACallback
VOID CALLBACK LineDDACallback(int x, int y, LPARAM lparam)
{
	//取得文档、视图指针，视图设备环境和动画牌
	//POSITION tmplPos = AfxGetApp()->GetFirstDocTemplatePosition();
	//CDocTemplate* pTmpl = AfxGetApp()->GetNextDocTemplate(tmplPos);
	//POSITION docPos = pTmpl->GetFirstDocPosition();
	//CJLDoc  *pDoc = (CJLDoc*)pTmpl->GetNextDoc(docPos);
	//CJLView *g_pView = pDoc->GetView();
	CClientDC cdc(g_pView->m_hWnd);
	UINT card = lparam;

	static CRect rbk(0, 0, 0, 0);//用于记录上次绘制的位置

	//当前位置
	CRect r(x, y, x + g_fcCcs.CARD_WID, y + g_fcCcs.CARD_HEI), R, rInter;

	//第一点处不绘制牌面，只记录位置
	if (rbk.IsRectEmpty()) {
		rbk = r;
		return;
	}
	//当两个矩形的重叠面积达到牌面积75%时并且将要绘制的
	//矩形不和回收牌列相交时绘制新的矩形
	//这一算法是经过多次摸索之后得到的，算法达到的效果还
	//不错，我不知道windows的空当接龙是什么算法。
	//
	//百分比和动画速度的关系：
	//
	//	动画速度：慢速 -----中速-----> 快
	//	百分比  ：100% ------50%-----> 1%
	//
	//上次绘制与本次绘制的两个矩形的重叠区域

#define SquareIsOK(a,b) (a*b <= g_fcCcs.CARD_WID*g_fcCcs.CARD_HEI*(1-g_fcData.m_nDDASpeed/100.))
	// 很慢死锁的原因是 g_fcData.m_nDDASpeed 没有初始化

	rInter.IntersectRect(r, rbk);//重叠矩形肯定不为空
	if (SquareIsOK(rInter.Width(), rInter.Height())) { //满足面积关系
		//擦去上次的绘制
		g_pView->InvalidateRect(rbk);
		//重叠的部分不必刷新，因为新绘制的矩形包括这部分
		g_pView->ValidateRect(rInter);
		//在当前位置重新绘制
		g_pView->DrawCard(r.TopLeft(), card, &cdc);
		//cdc.BitBlt(r.left,r.top,r.Width(),r.Height(),&cdc,rbk.left,rbk.top,SRCCOPY);
		//强制立即刷新无效区域
		g_pView->SendMessage(WM_PAINT, 0, 0);
		//绘制完毕。备份当前矩形坐标
		rbk = r;
	}
	//接近回收处时可能在最后一点前不再绘制
	R = g_fcData.RectOf(TYPE(card) + 13, 1, 1);
	rInter.IntersectRect(rbk, R);
	if (!SquareIsOK(R.Width(), R.Height())) {
		g_pView->InvalidateRect(rbk);//擦去上次的绘制
		//rbk.SetRectEmpty();
		return;
	}
}

//取第col列第idx张牌
UINT CWTLfcData::GetCard(UINT col, UINT idx)
{
	ATLASSERT(IsCol(col) && !IsEmptyCol(col) && idx > 0 && idx <= CntCardsIn(col));
	if (col <= 8) {
		UINT *pTop = &m_iCards[col - 1][0];
		return pTop[--idx];
	}
	else if (col <= 12) {
		ATLASSERT(idx == 1);
		return m_iBuffer[col - 9];//与idx无关
	}
	else {
		UINT *pTop = &m_iRecycle[col - 13][0];
		return pTop[--idx];
	}
}


//存档
//void WTLfcData::OnSave()
//{
//	// TODO: Add your command handler code here
//	CFileDialog dlg(FALSE, TEXT("rep"), GetTitle(), dwFlags, filter);
//	if (dlg.DoModal() == IDCANCEL) return;
//	CFile file(dlg.GetPathName(), modeCrWr);
//	CArchive ar(&file, CArchive::store);
//	Serialize(ar);
//	ar.Close();
//	file.Close();
//}
//
////读档
//void WTLfcData::OnLoad()
//{
//	// TODO: Add your command handler code here
//	if (!GiveUp()) return;
//
//	//选择文件
//	CFileDialog dlg(TRUE, TEXT("rep"), NULL, dwFlags, filter);
//	if (dlg.DoModal() == IDCANCEL) return;
//	CFile file(dlg.GetPathName(), modeRead);
//
//	/////////////////////////////////////////////////////////////////
//	//考虑存档文件有可能是一个不完整的自定义牌局
//	int nGameNumber;
//	file.Read(&nGameNumber, sizeof(int));
//	if (nGameNumber == -1) {
//		AfxMessageBox("请将自定义牌局【" + dlg.GetFileName() + "】编辑完整！\n");
//		return;
//	}
//	file.SeekToBegin();
//	/////////////////////////////////////////////////////////////////
//	m_dlgScore.UpdateScore();//记录战况
//
//	//读档
//	CArchive ar(&file, CArchive::load);
//	Serialize(ar);
//	ar.Close();
//	file.Close();
//
//	//刷新牌局
//	UpdateAllViews(NULL);
//
//	//设置窗框标题为当前牌局代号或自定义牌局的文件名
//	CString title;
//	if (m_nCurGameNumber > 0) {
//		title.Format(TEXT("%d"), m_nCurGameNumber);
//		SetTitle(title);
//	}
//	else {
//		SetTitle(dlg.GetFileName());
//	}
//
//	m_dlgScore.InitScore();//记录战况
//	CheckGame();//看看此局是否已经结束
//}

//void CWTLfcData::ClrOpsRecords()
//{
//	if (m_pOps == NULL) return;
//	////////////////////////////////
//	//清除原来的操作记录
//	while (!m_pOps->empty())
//	{
//		delete m_pOps->front();
//		m_pOps->pop_front();
//	}
//	m_pOps->clear();
//}
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

/*自动解答算法使用全局的递归算法和局部的回溯法算法相结合，步骤如下：
自动解答算法：
(1)  对1-4和9-16列中 每一个 有牌可以移动到其他列的 列 执行(2)
否则（无牌可以移动）看还有没有空档，没有则返回false（此路不通），
如果有空档则（表示1-4和9-16列全是空的，解答成功）返回true，
(2)  对此列牌的每一个目标列(目标列可能不止一列)都使用以下算法：
(3)  如果此列可以合并到其他列，则将此列合并到其他列。如果
有两个列可以合并它，则
(4)  自动扔牌
(5)  调用自动解答算法
如果它返回true则返回true
否则撤销本次所有的移动（即：(3)(4)中的移牌动作）并返回false
(6)  返回false
*/

//-----------------------------------------------
volatile bool g_bStopAICal;//玩家停止了自动解答
//-----------------------------------------------
//自动解答
bool CWTLfcData::AICal()
{
	//玩家等的不耐烦了
	if (g_bStopAICal) {
		//撤销过程的动画效果关闭，这样能在瞬间
		//撤销所有的解答步骤
		//m_bRealTimeUpdate = false;
		return false;
	}
	//自动解答得到的步数超过正常值，尽快结束这种局面
	//此局很可能导致算法无限死循环
	if (!m_OpsList.empty() && m_OpsList.size() > 200) {
		//g_bStopAICal = true;
		return false;
	}
	AutoThrow();
	return GameOver() || Combine() || Splite();//先合并，合不了就拆开
}
/*-------------------------------------------合-------------------------------------------
对每一除回收列之外的有目标列的非空列
{
如果没有目标列
如果此列是缓存列
如果有空牌列
如果拿下来能够节约空间
拿下来，继续解答
否则
如果目标列只在缓存列（因为可能有两个目标都在缓存列）
如果有空牌列
将（任一）目标牌列拿到空牌列中，继续解答         // 从上往下把目标牌列合并到空牌列中

否则如果此列是缓存列（至少有一个目标是牌列）
拿到（任一）牌列目标上来，继续解答

否则如果可以合并到牌列                                  （空间足够，缓存列和牌列合并到牌列）
如果能在目标上得到更长的序列牌			// 即使是长度相同也不行
或目标牌列完全是以K开始的序列牌
合并到目标牌列上，继续解答
}
返回假

继续解答：
记录此次动作
自动扔牌
如果执行自动解答成功返回真
撤销
返回假
----------------------------------------------------------------------------------------*/
/*-----------------------------------------扔---------------------------------------------
拆：非空才拆

拆缓存列：	有空牌列就直接拿下来，否则失败
拆牌列：	此列是非完全序列牌
先后拆到牌列空列空档，不够不拆。
否则就是完全序列牌，
全拆到空档，不够不拆。
拆完后再拆另一个牌列或空档列（此空档列不能是刚才拆上去的列，不然循环了）
如果找不到可以拆动的牌列，如果可以找到缓存列否则失败
----------------------------------------------------------------------------------------*/
//执行合并动作
bool CWTLfcData::Combine()
{
	for (UINT i = 1; i <= 12; i++) {
		if (IsEmptyCol(i)) continue;
		if (CombimeCol(i)) return true;
	}
	return false;
}
//执行拆分动作
bool CWTLfcData::Splite()
{
	//对每个可拆的非完全序列进行拆分
	UINT cols[8 + 1], *pFirst = cols, *pLast = SortByActivity(cols);
	for (; pFirst < pLast; ++pFirst) {
		if (SpliteCol(*pFirst)) return true;
	}
	for (UINT i = 9; i <= 12; i++) {
		if (IsEmptyCol(i)) continue;
		if (SpliteCol(i)) return true;
	}
	return false;
}
//合并牌列：将此列的（整个或部分）序列牌合并到其它列
bool CWTLfcData::CombimeCol(UINT col)
{
	//合并的对象只可能是缓存列或牌列且是非空列
	ATLASSERT(IsCol(col) && !ColInRecycle(col) && !IsEmptyCol(col));

	int desCol = 0, srcCol = 0, cntCards = 0;

	int tar[2];
	GetTarget(col, tar);//寻找此列的目标列

	UINT ntar = 0;//计算目标列数目
	bool bAllTarInBuf = false;//是否所有目标都在缓存列
	if (tar[0]) {
		++ntar;
		bAllTarInBuf = ColInBuf(tar[0]);
		if (tar[1]) {
			++ntar;
			bAllTarInBuf = bAllTarInBuf && ColInBuf(tar[1]);
		}
	}
	//----------
	//没有目标列
	//----------
	if (ntar == 0) {
		//此列是非空牌列没有目标列
		if (!ColInBuf(col)) return false;
		//此列是缓存列没有目标列
		int a = CntEmptyBufs();//a是空档数
		int b = CntEmptyCardCols();//b是空牌列数
		//如果没有空牌列
		if (b == 0) return false;
		//如果有空牌列
		int c = (2 * a + b)*(b + 1) / 2 + 1;//移动之前的空间
		++a, --b;//假设空档增加空牌列减少
		int d = (2 * a + b)*(b + 1) / 2 + 1;//移动之后的空间
		if (c >= d) return false;
		//能增加空间，拿到空牌列，继续解答
		srcCol = col;
		desCol = FindEmptyCardCol();
		cntCards = 1;
#ifdef DEBUG_ALERT
		ShowMessage("合并缓存列到空牌列，增加空间", srcCol, desCol, cntCards);
#endif
		goto doAI;
	}
	//如果目标列都在缓存列
	//--------------------
	else if (bAllTarInBuf) {
		int empCardCol = FindEmptyCardCol();
		if (!empCardCol)return false;//如果有空牌列
		//将（任一）目标牌列拿到空牌列中，继续解答
		srcCol = tar[0];
		desCol = empCardCol;
		cntCards = 1;
#ifdef DEBUG_ALERT
		ShowMessage("合并缓存列到空牌列，且其他牌可以合并到此牌",
			srcCol, desCol, cntCards);
#endif
		goto doAI;
	}
	//至少有一个目标列在牌列
	//------------
	//此列是缓存列
	else if (ColInBuf(col)) {
		//缓存列拿到牌列目标上来，继续解答
		srcCol = col;
		desCol = tar[ntar == 1 ? 0 : (ColInCard(tar[0]) ? 0 : 1)];
		ATLASSERT(tar[0]);
		cntCards = 1;
#ifdef DEBUG_ALERT
		ShowMessage("合并缓存列到牌列", srcCol, desCol, cntCards);
#endif
		goto doAI;
	}
	//否则此列是牌列
	else {
		srcCol = col;
		if (ntar == 1) { //仅有一个目标列则此目标列肯定是牌列
			desCol = tar[0];
		}
		else {
			//有两个目标列，则可能有一个或两个目标列是牌列
			if (!ColInCard(tar[0])) { //tar[0]不是牌列则tar[1]肯定是牌列
				desCol = tar[1];
			}
			else if (ColInCard(tar[1])) { //两个tar都是牌列
				desCol = tar[CntSeriIn(tar[0]) > CntSeriIn(tar[1]) ? 0 : 1];
				//先合并到序列牌长的目标列
			}
			else {
				desCol = tar[0];
			};
		}
		cntCards = CntMaxMv(desCol, srcCol);
		ATLASSERT(cntCards > 0);
		//if( cntCards + CntSeriIn(desCol) <= CntSeriIn(srcCol) ) return false;
		//由长序列合并到短序列必须是移动后源列露出废牌才行
		if (cntCards + CntSeriIn(desCol) <= CntSeriIn(srcCol))
			if (!Trashable(GetCard(srcCol, CntCardsIn(srcCol) - cntCards)))
				return false;
		//可以合并到牌列
#ifdef DEBUG_ALERT
		ShowMessage("合并牌列到牌列，且能得到更长序列牌", srcCol, desCol, cntCards);
#endif
		goto doAI;
	}
	return false;
doAI:	//有牌可以移动哦
MoveCards(desCol, srcCol, cntCards);//移动
//Record(new COperations(desCol, srcCol, cntCards));//记录移动动作
Record(desCol, srcCol, cntCards);//记录移动动作
AutoThrow();//自动扔牌（自动记录动作）
if (AICal())return true;//成功解答
Undo();
	return false;
}

/*
扔：牌列 | 缓存 ---> 回收
合：牌列 | 缓存 ---> 牌列
1. 序列牌整个拿到其他牌列露出空列或剩余牌
2. 序列牌部分拿到其他牌列露出废牌
3. 缓存牌拿到其他牌列
4. 缓存牌拿到空列成为责任牌

拆：牌列 ---> 牌列 | 空档
*/
bool CWTLfcData::SpliteCol(UINT col)
{
	//拆的对象只可能是缓存列或牌列且非空才拆
	ATLASSERT(IsCol(col) && !ColInRecycle(col) && !IsEmptyCol(col));
	if (ColInBuf(col))//拆缓存列
	{
		//没有空牌列则不能拿下来
		UINT empCardCol = FindEmptyCardCol();
		if (empCardCol == 0)return false;
		//有空牌列
		//如果拿下来能给别的列提供合的机会就拿下来
		int tar[2];
		GetTarget(col, tar);//寻找目标列
		if (!tar[0] && !tar[1])return false;
#ifdef DEBUG_ALERT
		ShowMessage("拆缓存列，有牌能合并到它上面", col, empCardCol, 1);
#endif
		MoveCards(empCardCol, col, 1);//记录移动动作
		//Record(new COperations(empCardCol, col, 1));
		Record(empCardCol, col, 1);
		if (AICal())return true;
		Undo();
		return false;
	}
	//拆牌列
	UINT empCardCol = FindEmptyCardCol();
	if (empCardCol)//能够直接移动到空牌列？
	{
		int nCntCards = CntCardsIn(col);
		int nFitFomula = CntSeriIn(col);
		int nMovableCards = CntMaxMv(empCardCol, col);
		//实际可移动的牌肯定不大于序列牌数
		if (nMovableCards == nFitFomula)
		{
			//完全序列牌列直接移到空牌列没有意义
			if (nFitFomula == nCntCards)return false;
#ifdef DEBUG_ALERT
			ShowMessage("拆序列牌到空牌列", col, empCardCol, nMovableCards);
#endif
			//非完全序列牌列的全部序列牌直接移到空牌列
			MoveCards(empCardCol, col, nMovableCards);
			//Record(new COperations(empCardCol, col, nMovableCards));
			Record(empCardCol, col, nMovableCards);
			if (AICal())return true;
			Undo();
			return false;
		}//else 序列牌不能直接拿到空牌列，则需要分批拆，看下面
	}
	else { //分批拆

		//先后拆到牌列，空列及空档，不够不拆。
		int inUse[12];//记录使用过的空间
		int steps = 0;//记录使用了多少空间
		int nMoved = 0;//记录移动过的牌数
		int nCntCard = CntCardsIn(col);
		int nCntSeri = CntSeriIn(col);
		int tarCol = 0, empCardCol = 0, empBufCol = 0;
		while (nMoved != nCntSeri)
		{
			int tar[2];
			GetTarget(col, tar);//寻找目标列
			bool t0 = ColInCard(tar[0]);
			bool t1 = ColInCard(tar[1]);
			//没有空牌列
			//如果可以部分合并到其他列时，合并到其他牌列
			if (t0 || t1)
				tarCol = tar[t0 ? 0 : 1];
			else if ((empCardCol = FindEmptyCardCol()) != 0)//否则如果还有空牌列
				tarCol = empCardCol;
			else if ((empBufCol = FindEmptyBuf()) != 0)//否则如果还有空档
				tarCol = empBufCol;
			else//否则（拆不开，看下一列）
			{
				while (steps--)Undo();//撤销
				return false;
			}
			int n = CntMaxMv(tarCol, col);
			ATLASSERT(n > 0);
#ifdef DEBUG_ALERT
			ShowMessage("拆序列牌到牌列，空列及空档！", col, tarCol, n);
#endif
			MoveCards(tarCol, col, n);//记录移动动作
			//Record(new COperations(tarCol, col, n));
			Record(tarCol, col, n);
			nMoved += n;//计算移走的牌数
			inUse[steps++] = tarCol;//记录当前使用的目标列
			//退出循环时，step记录使用了多少空间

			//部分序列拿走后露出废牌的这种情况在
			//合并函数中已经予以考虑过了，在此就不必再考虑了
			/*
			if(nMoved < nCntSeri) {
			UINT bc = BottCard(col);
			if(Trashable(bc)) {
			MoveCards(TYPE(bc)+13,col,1);
			Record(new COperations(TYPE(bc)+13,col,1));
			if(AICal()) return true;
			Undo();
			}
			}
			*/
		}
		//此列不完全是序列牌
		//------------------
		if (nCntCard != nCntSeri){
			if (AICal()) return true;
			while (steps--)Undo();//不成功则全部撤销
			return false;
		}
		//此列完全是序列牌
		//----------------
		//拆完后还再拆另一个列，它可能是牌列，也可能是缓存列，
		//但绝对不能是正在被使用的列，也不可能是拆掉了的当前列	
		for (int another = 1; another <= 12; another++) {
			ATLASSERT(IsEmptyCol(col));
			//col列此时为空所以可以被过滤掉
			if (IsEmptyCol(another))continue;
			bool isInUse = false;
			for (int j = 0; j < steps; j++){
				if (another != inUse[j])continue;
				isInUse = true;
				break;
			}
			//过滤掉刚刚被使用的列
			if (isInUse)continue;
			if (SpliteCol(another))return true;//成功解答
			while (steps--)Undo();//不成功则全部撤销
			return false;
		}
	}

	return false;
}

//找到一个空牌列
UINT CWTLfcData::FindEmptyCardCol()
{
	for (UINT i = 1; i <= 8; i++)
		if (m_iCards[i - 1][19] == 0) return i;
	return 0;
}

//找到一个空档
UINT CWTLfcData::FindEmptyBuf()
{
	for (UINT i = 9; i <= 12; i++)
		if (m_iBuffer[i - 9] == 0) return i;
	return 0;
}

//统计空牌列数
UINT CWTLfcData::CntEmptyCardCols()
{
	int cnt = 0;
	for (UINT i = 1; i <= 8; i++)
		if (m_iCards[i - 1][19] == 0) ++cnt;
	return cnt;
}

//统计空档数
UINT CWTLfcData::CntEmptyBufs()
{
	int cnt = 0;
	for (UINT i = 9; i <= 12; i++)
		if (m_iBuffer[i - 9] == 0) ++cnt;
	return cnt;
}


//为指定的源列寻找目标列，目标列可能有一个或两个，但是绝对不会超过两个
//如果没有目标列，则返回时，target[0]和target[1]都是零
//搜索非回收列来寻找目标列
void CWTLfcData::GetTarget(int col, int *target)
{
	ATLASSERT(IsCol(col) && !ColInRecycle(col) && !IsEmptyCol(col));

	int *p = target;
	p[0] = p[1] = 0;
	for (UINT i = 1; i <= 12; i++) {
		if (i > 8) {
			int d = m_iBuffer[i - 9];
			if (!IS_CARD(d)) continue;//忽略空档
			int s = BottCard(col);
			int n = NUM(d) - NUM(s);
			int nSeri = CntSeriIn(col);
			if (n > 0 && n <= nSeri && n % 2 == (TYPE(s) + TYPE(d)) % 2) {
				*p++ = i;
			}
		}
		else if (m_iCards[i - 1][19] && CntMaxMv(i, col)) {
			*p++ = i;//目标是牌列
		}
		if (p == target + 2) return;//目标列绝对不会超过两个
	}
}

#ifdef DEBUG_ALERT
//调试程序的时候，请将DEBUG_ALERT加入到预编译开关中
void CWTLfcData::ShowMessage(char* pMsg, int src, int des, int cnt)
{
	ATLASSERT(pMsg != NULL);

	CMsgDlg msgdlg;
	CString msg;
	msg.Format("%d--->%d(%d),%s", src, des, cnt, pMsg);

	msgdlg.m_strMsg = msg;
	msgdlg.DoModal();
}
#endif

//检查游戏是否结束
//如果没有结束则计算提示步骤
//否则就让玩家选择是否开局或回放存档
void CWTLfcData::CheckGame()
{
	if (!GameOver())
	{
		GetHints();
		if (m_Hints.IsEmpty())
		{
			MessageBoxW(g_pView->m_hWnd, _T("没法走下一步了！\n你可以撤销几步再试试。"), NULL, NULL);
			return;
		}
		return;
	}
	//本局结束了

	CDlgPassed dlg;
	BOOL b;
choice:	if (IDCANCEL == dlg.DoModal())
	return;
		switch (dlg.m_nChoice)
		{
		case RandGame:	OnRand();     break;
		case PrevGame:	g_pView->OnPrevGame(NULL, NULL, NULL, b); break;
		case NextGame:	g_pView->OnNextGame(NULL, NULL, NULL, b); break;
		case ThisGame:  g_pView->OnAgain(NULL, NULL, NULL, b);    break;
		//case SaveGame:	g_pView->OnSave(NULL, NULL, NULL, b);     goto choice;
		case PlayShow:
			//游戏返回到开头但是保留步骤记录
			BackHome();

			CDlgAIShow dlgAIShow;
			dlgAIShow.DoModal();//回放对话框
			goto choice;
		}
}

//执行自动解答
bool CWTLfcData::DoAICal(void)
{
	//备份“快速移动”选项的值
	BOOL quickmv_bk = m_bQuickMove;

	//自动扔牌时的动画效果必须暂时关闭
	m_bQuickMove = TRUE;

	//开始解答
	m_bAICalRunning = true;
	g_bStopAICal = false;
	m_bRealTimeUpdate = false;
	bool bSuccess = AICal();
	m_bAICalRunning = false;
	g_bStopAICal = true;

	//恢复“快速移动”选项的值
	m_bQuickMove = quickmv_bk;

	return bSuccess;
}

//计算出要显示步数信息的矩形
CRect CWTLfcData::RectOfStep()
{
	CRect r;
	r.UnionRect(RectOf(12, 1, 1), RectOf(13, 1, 1));

	int inflateY = r.Height() - g_fcCcs.stepFont * 4 / 3;
	r.InflateRect(-g_fcCcs.CARD_WID - 1, 0, -g_fcCcs.CARD_WID - 1, -inflateY);
	return r;
}

//牌索引按钮的矩形位置
CRect CWTLfcData::RectOf(UINT card)
{
	CRect r, r11, r14;
	r11 = RectOf(11, 1, 1);
	r14 = RectOf(14, 1, 1);
	r.left = r11.left + g_fcCcs.CARD_WID / 2;

	UINT rWid = (r14.right - r11.left - g_fcCcs.CARD_WID) / 13;
	r.left += (13 - NUM(card))*rWid;
	r.right = r.left + rWid + 1;
	r.top = 0;
	r.bottom = g_fcCcs.ptOrg.y - 1;
	return r;
}

//计算下一步的所有可能的动作并记录它（们）
void CWTLfcData::GetHints()
{
	//清除原来的记录
	m_Hints.ClrHints();
	if (GameOver()) return;

	UINT nMove, i, j;
	//考虑合并和回收废牌
	for (i = 1; i <= 12; i++) {
		if (IsEmptyCol(i)) continue;
		UINT bc = BottCard(i);
		if (Trashable(bc)) {
			m_Hints.AddOperation(Type(bc) + 13, i, 1);//提示回收底牌（废牌）
		}
	}
	for (i = 1; i <= 12; i++) {
		if (IsEmptyCol(i)) continue;
		for (j = 1; j <= 8; j++) {
			if (IsEmptyCol(j) || !(nMove = CntMaxMv(j, i))) continue;
			//合并要得到较长序列或有废牌可以扔掉
			if (CntSeriIn(i) - nMove < CntSeriIn(j) ||
				Trashable(GetCard(i, CntCardsIn(i) - nMove))) {
				m_Hints.AddOperation(j, i, nMove);
			}
		}
	}
	if (!m_Hints.IsEmpty()) return;

	//考虑拆掉非完全序列牌
	for (i = 1; i <= 8; i++) {
		if (IsEmptyCol(i) || CntSeriIn(i) == CntCardsIn(i)) continue;
		for (j = 1; j <= 12; j++) { //不考虑回收列因为无废牌
			if (!IsEmptyCol(j)) continue;
			m_Hints.AddOperation(j, i, CntMaxMv(j, i));
			break;//可能有多个空列，但只提示移动到其中之一就够了
		}
	}
}
/*
////////////////////////////////////////////////////////////////
能增加空间的列最先考虑拆掉：
////////////////////////////////////////////////////////////////

// 拆分兼具合并
·对每一非完全序列牌牌列：
·按照活牌所占剩余牌的比例由高到低进行排序，对排序后的每一列：
·如果序列牌能完全移走（通过合并到其他牌列，拆到空牌列，或移动到空档）
·全拆

·根据缓存列的牌从大到小的顺序
// 增加空间且增加责任牌
·如果此牌可以拿到牌列则拿下来
// 增加空间且增加责任牌
·否则如果此牌拿下来后能增加空间则拿下来
// 增加责任牌但减少空间
·如果此牌拿下来后能成为责任牌则拿下来

// 增加空间但减少责任牌
·对每一完全序列牌牌列：
·如果序列牌能完全移到空档和牌列
·能增加空间
·全拆
·如果没有空牌列
·拿到空档以留出一个空牌列

·调用自动解答
*/
//关于非空牌列的有关信息：牌列，牌数目，序列牌长，活牌数
struct COL_INF {
	UINT col, nCards, nSeri, act;
	void Set(UINT a, UINT b, UINT c, UINT d) {
		col = a; nCards = b; nSeri = c; act = d;
	}
};
/*
int CmpAct( const void *arg1, const void *arg2 )
{
COL_INF *p1 = (COL_INF*)arg1;
COL_INF *p2 = (COL_INF*)arg2;

int res = p1->act - p2->act;

if(p1->act == 0) {
if(p2->act == 0) { //都无活牌就先拆牌少的列
return p2->nCards - p1->nCards;
} else { //有活牌的列比无活牌的列先拆
return res;
}
} else {
if(p2->act == 0) { //有活牌的列比无活牌的列先拆
return res;
} else { //都有活牌
int diff = p1->act * (p2->nCards - p2->nSeri) - p2->act * (p1->nCards - p1->nSeri);
if(diff == 0) { //活牌比例一样大
if(p1->act == p2->act) { //活牌一样多
return p2->nSeri - p1->nSeri; //先拆序列牌短的列
} else { //活牌不一样多
return res; //拆活牌多的列
}
} else { //否则先拆活牌（占剩余牌的）比例较大的列
return diff;
}
}
}

return res;
}
*/
//对所有的非空牌列，根据活牌数对每个非空牌列进行排序
//活牌数最多的列排在最前
UINT* CWTLfcData::SortByActivity(UINT *pCols)
{
	char bc[8 + 1], rc[4 + 1], *p, i;
	for (p = bc, i = 1; i <= 8; i++) { //获取底牌集
		if (!IsEmptyCol(i)) *p++ = BottCard(i);
	} *p = 0;
	for (p = rc, i = 13; i <= 16; i++) { //获取废牌集
		if (!IsEmptyCol(i)) *p++ = BottCard(i);
		else *p++ = 0;
	} *p = 0;

	/*
	//统计非空牌列的有关信息：牌列，牌数目，序列牌长，活牌数
	COL_INF f[8+1],*pLast = f;
	for(i = 1 ; i <= 8 ; i++) {
	if(IsEmptyCol(i)) continue;

	UINT *pTop = &m_iCards[i-1][0];//pTop指向此列牌的顶部
	UINT nCards = pTop[19], nSeri = CntSeriIn(i);
	UINT *pCur = pTop + nCards-nSeri - 1 ;//pCur指向剩余牌的底牌

	char b[8+1], r[4+1]; //复制底牌集和废牌集分别到b，r
	strcpy(b,bc);
	strcpy(r,rc);

	UINT nAct = 0; //统计此列的活牌数目
	while(pCur >= pTop) { //遍历剩余牌以统计活牌数
	char *pTarg = FindActiveCard(*pCur,b,r);
	if(pTarg) {
	*pTarg = *pCur;//模拟此牌被拿开到其他列（拿到其他牌列或空档或回收列）
	++nAct;//统计此列的活牌数目
	}
	--pCur;
	}
	pLast++->Set(i,nCards,nSeri,nAct);//保存有关此列牌的信息
	} pLast->col = 0;
	//对牌列按照活牌数排序
	qsort( (void*)f, (pLast-f)/sizeof(COL_INF), sizeof(COL_INF), CmpAct);
	//拷贝排序后的牌列到参数数组中
	for(COL_INF * pFirst = f; pFirst < pLast; ) {
	*pCols++ = pFirst++->col;
	} *pCols = 0;
	return pCols;
	*/
	//统计非空牌列的有关信息：牌列，牌数目，序列牌长，活牌数
	COL_INF f[8 + 1], *pLast = f;
	for (i = 1; i <= 8; i++) {
		if (IsEmptyCol(i)) continue;

		UINT nCards = CntCardsIn(i);
		UINT nSeri = CntSeriIn(i);
		UINT nAct = 0;

		//统计此列的活牌数目
		char b[8 + 1], r[4 + 1];
		//strcpy(b, bc);
		//strcpy(r, rc);//复制底牌集和废牌集分别到b，r
		strcpy_s(b, bc);
		strcpy_s(r, rc);//复制底牌集和废牌集分别到b，r

		UINT *pTop = &m_iCards[i - 1][0];//pTop指向此列牌的顶部
		UINT *pCur = pTop + pTop[19] - 1 - nSeri;//pCur指向剩余牌的底牌
		while (pCur >= pTop) {
			char *pAct = FindActiveCard(*pCur, b, r);
			if (pAct) {
				*pAct = *pCur;
				++nAct;//统计此列的活牌数目
			}
			--pCur;
		}
		//保存有关此列牌的信息
		pLast++->Set(i, nCards, nSeri, nAct);
	}
	pLast->col = 0;

	//对牌列按照活牌数排序
	COL_INF *pFirst = f;
	while (pFirst < pLast) {
		COL_INF *p = pLast - 1;
		while (--p >= pFirst) {
			if (p->act < (p + 1)->act) {
				COL_INF t = p[0];
				p[0] = p[1];
				p[1] = t; //交换位置
			}
		}
		++pFirst;//活牌数最最多的列已经放到最前了
	}
	//拷贝排序后的牌列到参数数组中
	for (pFirst = f; pFirst < pLast;) {
		*pCols++ = pFirst++->col;
	} *pCols = 0;
	return pCols;
}

//看看此牌是否可以回收
bool CWTLfcData::Trashable(UINT card)
{
	ATLASSERT(IsCard(card));

	UINT type = TYPE(card);
	if (IsEmptyCol(type + 13)) return NUM(card) == 1;//只有A可以放入空列

	return FitTrash(card, BottCard(type + 13));//必须花色、点数都相符才行
}

//看看card是否为活牌
char * CWTLfcData::FindActiveCard(UINT card, char *b, char *r)
{
	ATLASSERT(IsCard(card));

	//查看底牌集，看是否可能拿到其他牌列
	for (; *b; ++b) if (FitFormula(*b, card)) return b;

	//查看废牌集，看是否符合回收规则
	UINT type = TYPE(card);
	UINT num = NUM(card);
	if (r[type] == 0) return num == 1 ? &r[type] : 0; //只有A可以放入空列

	return FitTrash(card, r[type]) ? &r[type] : 0;//必须花色、点数都相符才行
}

//利用clock与rand，返回一个随机数
//我们可以产生一个随机数，即使srand每次设置同一个种子，
//调用此函数后也不会得到相同的随机数，这样比较好，
//因为这才是真正的随机开局呀
int CWTLfcData::Random(void)
{
	int n = ((rand() << 16) | (clock() & 0xFFFF)) & MAX_GAME_NUMBER;
	return max(n, 1);
}

//BOOL WTLfcData::CanCloseFrame(CFrameWnd* pFrame)
//{
//	// TODO: Add your specialized code here and/or call the base class
//	if (!GameOver() && !m_pOps->IsEmpty())
//		if (IDNO == AfxMessageBox(TEXT("不玩了么？"), MB_YESNO))
//			return FALSE;
//
//	return CDocument::CanCloseFrame(pFrame);
//}

BOOL CWTLfcData::GiveUp()
{
	//当本局为新局或已结束的时候，可以开始下一局
	if (m_OpsList.empty() || GameOver()) return true;
	//否则要提醒玩家是否放弃当前局
	//return IDYES == AtlMessageBox((HWND)_Module.get_m_hInst(),TEXT("放弃当前游戏？"), MB_YESNO);
	//return IDYES == AtlMessageBox(g_pView->m_hWnd, TEXT("放弃当前游戏？"), MB_YESNO);
	return IDYES == ::MessageBox(g_pView->m_hWnd, TEXT("放弃当前游戏？"), TEXT("WTLfc"), MB_YESNO);
}

//随机开局，但不再产生已经出现过的局
void CWTLfcData::OnRand()
{
	// TODO: Add your command handler code here
	if (!GiveUp()) return;
	int nUniqueGame = Random();
	while (m_dlgScore.IsOldGameNumber(nUniqueGame = Random()));
	StartGame(nUniqueGame);
}

////上一局
//void CWTLfcData::OnPrevGame()
//{
//	// TODO: Add your command handler code here
//	if (!GiveUp()) return;
//	StartGame(max(m_nCurGameNumber - 1, MIN_GAME_NUMBER));
//}
////下一局
//void CWTLfcData::OnNextGame()
//{
//	if (!GiveUp()) return;
//	StartGame(min(m_nCurGameNumber + 1, MAX_GAME_NUMBER));
//}
////重玩
//void CWTLfcData::OnAgain()
//{
//	// TODO: Add your command handler code here
//	if (!GiveUp()) return;
//	if (m_nCurGameNumber > 0) {
//		StartGame(m_nCurGameNumber);
//	}
//	else {
//		while (!m_pOps->IsEmpty()) {
//			Undo();//撤销到开头
//		}
//	}
//}
//
////战况
//void WTLfcData::OnScore()
//{
//	m_dlgScore.DoModal();
//}
//
////选局
//void WTLfcData::OnSelectgamenumber()
//{
//	if (!GiveUp()) return;
//
//	CDlgSelGame dlg;
//	dlg.m_nGameNumber = m_nCurGameNumber;
//	if (dlg.DoModal() != IDOK) return;
//
//	StartGame(dlg.m_nGameNumber);
//}
//根据给定的牌局代号开始此局
void CWTLfcData::StartGame(int gameNumber)
{
	m_dlgScore.UpdateScore();//记录战况

	//ClrOpsRecords();			//清除动作记录
	UnselectCardCol();		//清除选中标志
	m_Hints.ClrHints();		//清除提示

	m_nCurGameNumber = gameNumber;
	Shuffle();				//洗牌发牌
	GetHints();
	//UpdateAllViews(NULL);	//绘制界面
	g_pView->RedrawWindow();

	CString title;			//设置窗框标题为当前牌局代号
	title.Format(TEXT("%10d"), m_nCurGameNumber);
	//SetTitle(title);

	m_dlgScore.InitScore();//记录战况
}

//根据给定的标签搜索此标签指定的牌
CARD_POS * CWTLfcData::FindCardForLabel(UINT cardLabel, CARD_POS * pos)
{
	for (UINT col = 1; col <= 8; col++) {
		UINT n = CntCardsIn(col);
		for (UINT idx = 1; idx <= n; idx++) {
			if (NUM(cardLabel) == NUM(GetCard(col, idx))) {
				pos->col = col;
				pos->idx = idx;
				++pos;
			}
		}
	}
	return pos;
}
////自定义牌局
//void CWTLfcData::OnEdit()
//{
//	CDlgDefGame dlg;
//	dlg.DoModal();
//}
