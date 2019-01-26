// WTLfcData.h : interface of the CJLDoc class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DlgScore.h"
#include <list>
using namespace std;
#include "Datatype.h"

VOID CALLBACK LineDDACallback(int x, int y, LPARAM lparam);
//class HINTS;
//struct CARD_POS;
//class CMyObject;

class CWTLfcData
{
protected: // create from serialization only

// Attributes
public:
	// WTLfc.cpp(14): error C2248: 'CWTLfcData::CWTLfcData' : cannot access protected member declared in class 'CWTLfcData'
	CWTLfcData();

	UINT m_nSel;
	HINTS m_Hints;

	int m_nCurGameNumber;
	//CObList *m_pOps;
	//list<CMyObject*>* m_pOps;
	//list<COperations> m_OpsList;
	//CMyOpsList m_OpsList;
	CTList<COperations> m_OpsList;

	UINT m_iCards[8][20];//6+13=19
	UINT m_iBuffer[4];
	UINT m_iRecycle[4][14];

	BOOL m_bMaxMove;
	BOOL m_bQuickMove;
	BOOL m_bEnableDbClick;
	BOOL m_bEnableAlert;

	bool m_bAICalRunning;
	bool m_bRealTimeUpdate;

// Implementation
public:
	void wtlSetTitle(CString strTitle = L"");
	void Serialize(CXArchive& ar);
	void BackHome();
	BOOL GiveUp();
	CRect RectOfStep();
	//void Record(CMyObject * thisStep);
	void Record(UINT des, UINT src, UINT n);
	void InvalidateRect(CRect r);
	UINT FindEmptyBuf();
	bool IsCard(UINT card);
	bool IsCol(UINT col);
	bool ColInRecycle(UINT col);
	bool ColInCard(UINT col);
	bool ColInBuf(UINT col);
	UINT BottCard(UINT col);
	UINT CntMaxSuppliment(bool OccupyAnEmptyCol = false);
	UINT Num(UINT card);
	UINT Type(UINT card);
	UINT CntCardsIn(UINT col);
	UINT CntSeriIn(UINT col);
	bool GameOver(void);
	void AutoThrow();
	UINT CntMaxMv(UINT desCol, UINT srcCol);
	void SelectCardCol(UINT col);
	bool IsEmptyCol(UINT col);
	void UnselectCardCol();
	void Shuffle();
	void MoveCards(UINT des, UINT src, UINT n);
	virtual ~CWTLfcData();

	CDlgScore m_dlgScore;
	void Undo();
	CARD_POS * FindCardForLabel(UINT cardLabel, CARD_POS * pos);
	CRect RectOf(UINT card);
	int m_nDDASpeed;
	int Random(void);
	void StartGame(int gameNumber);
	char * FindActiveCard(UINT card, char * b, char *r);
	bool Trashable(UINT card);
	UINT* SortByActivity(UINT *pCols);
	void GetHints(void);
	bool DoAICal(void);
	void CheckGame();
	//CWTLfcView* GetView(void);
	bool CombimeCol(UINT col);
	bool SpliteCol(UINT col);
	void GetTarget(int col, int *target);
	UINT CntEmptyCardCols(void);
	UINT CntEmptyBufs(void);
	UINT FindEmptyCardCol(void);
	bool FitFormula(UINT b, UINT a);
	bool Splite();
	bool Combine();
	bool AICal();
	//void ClrOpsRecords(void);
	UINT GetCard(UINT col, UINT idx);
	CRect RectOf(UINT col, UINT idx, UINT nCards);

	void OnRand();

	////////////////////////////////////////////////////////////
	// 获取STL list中的指定元素只能用遍历了
	//CMyObject* getAt(list<CMyObject*>* pList, unsigned n)
	template <typename T>
	T* getAt(list<T>& xList, unsigned n)
	{
		list<T>::iterator it = xList.begin();
		for (unsigned i = 0; i < n; i++)
			++it;

		return &(*it);
	}

	//COperation* getAt(list<COperation>& opList, unsigned n)
	//{
	//	list<COperation>::iterator it = opList.begin();
	//	for (unsigned i = 0; i < n; i++)
	//		it++;

	//	return &(*it);
	//}

	//COperations* getAt(list<COperations>& opsList, unsigned n)
	//{
	//	list<COperations>::iterator it = opsList.begin();
	//	for (unsigned i = 0; i < n; i++)
	//		it++;

	//	return &(*it);
	//}

	// 获取STL list中的指定元素只能用遍历了。在头文件中定义的模板函数不好实例化
	//template <typename T>
	//T listget(list<T>* pList, unsigned n)
	//{
	//	list<T>::iterator it = pList.begin();
	//	for (unsigned i = 0; i < n; i++)
	//		it++;
	//	
	//	return *it;
	//}
// Generated message map functions
public:

};

