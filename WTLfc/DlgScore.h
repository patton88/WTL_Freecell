#if !defined(AFX_DLGSCORE_H__C602AC74_2F0C_4988_8304_95D5115AEF42__INCLUDED_)
#define AFX_DLGSCORE_H__C602AC74_2F0C_4988_8304_95D5115AEF42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgScore.h : header file
//

#include "resource.h"
#include "Datatype.h"
class CScore;
#include <atltime.h>

/*
CTime与DWORD之间的转换
2010-04-07 11:02:33
标签：CTime DWORD office，word 休闲 职场
原创作品，允许转载，转载时请务必以超链接形式标明文章 原始出处 、作者信息和本声明。否则将追究法律责任。
http://wanxin.blog.51cto.com/966660/291927
CTime t(2009,7,1,16,58,50); //时间类型赋值
DWORD dwTime = t.GetTime(); //将CTime状换为DWORD类型
CTime time(dwTime);//将DWORD转换为CTime
time.GetDay();
time.GetYear();
.......................
time.Format("%Y %M %M %H %M %S",t.GetYear(), ........);
//时间类型的赋值
本文出自 “数据恢复技术-Thinking..” 博客，请务必保留此出处http://wanxin.blog.51cto.com/966660/291927
*/

class CScore : public CMyObject
{
public:
	int gameNumber;
	int steps;
	CTime tmStart, tmEnd;
	enum { gameInit = 0, gamePassed, gameGiveUp, gameDead } gameStatus;

	void Serialize(CXArchive &ar)	// 类的串行化支持都先注释掉
	{
		//CObject does not serialize anything by default
		//CObject::Serialize(ar);
		long long llStart, llEnd;
		if (ar.IsStoring()) {
			ar << gameNumber << steps << tmStart.GetTime() << tmEnd.GetTime();
		}
		else {
			ar >> gameNumber >> steps >> llStart >> llEnd;
			tmStart = llStart;	tmEnd = llEnd;
		}
	}
};

// 1>g:\myvccode3\wtlfc\wtlfc\dlgscore.h(32): error C2079: 'CDlgScore::m_score' uses undefined class 'CScoreList'
//////////////////////////////////////////////////////////////
// CScoreList用于实现 list<CScore> m_scList; 的串行化
class CScoreList : public CMyObject
{
public:
	list<CScore> m_scList;
	CScoreList(int n = 0) { m_scList.resize(n); }

	void Serialize(CXArchive& ar)
	{
		if (ar.IsStoring())
		{
			ar << m_scList.size();
		}
		else
		{
			int n;	ar >> n;		m_scList.resize(n);
		}

		for (list<CScore>::iterator it = m_scList.begin(); it != m_scList.end(); it++)
			(*it).Serialize(ar);
	}
};
/////////////////////////////////////////////////////////////////////////////
// CDlgScore dialog

class CDlgScore : public CDialogImpl<CDlgScore>, public CWinDataExchange<CDlgScore>
{
// Construction
public:
	bool IsOldGameNumber(int gameNum);
	void InitScore();
	void UpdateScore();

	//CObList *m_pOps;
	//list<CMyObject*>* m_pOps;

	//CObList m_score;
	//list<CMyObject*> m_score;
	//list<CScore> m_score;
	CScoreList m_score;

	//CDlgScore(CWnd* pParent = NULL);   // standard constructor
	CDlgScore();
	~CDlgScore();
// Dialog Data
	//{{AFX_DATA(CDlgScore)
	enum { IDD = IDD_SCORE };
	CListViewCtrl	m_lcScoreInfo;
	CListViewCtrl	m_lcScore;
	//}}AFX_DATA

	BEGIN_MSG_MAP(CDlgScore)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOKCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CDlgScore)
		//DDV_MinMaxUInt(m_nGameNumber, 1, 2147483647);
		//DDX_INT(IDC_EDIT_WIDTH, m_nWidth)
		//DDX_INT(IDC_EDIT_HEIGHT, m_nHeight)
		//DDX_TEXT(IDC_EDIT_GAMENUMBER, m_strGameNumber)
		//DDX_UINT_RANGE(IDC_EDIT_GAMENUMBER, m_nGameNumber, 1, 2147483647)
		//DDX_Control(pDX, IDLC_SCORE_INFO, m_lcScoreInfo)
		//DDX_Control(pDX, IDLC_SCORE, m_lcScore)
		//DDX_CONTROL(IDLC_SCORE_INFO, m_lcScoreInfo) // error C2039: 'SubclassWindow' : is not a member of 'WTL::CListViewCtrlT<ATL::CWindow>'
		//DDX_CONTROL(IDLC_SCORE, m_lcScore);//error C2039: 'SubclassWindow' : is not a member of 'WTL::CListViewCtrlT<ATL::CWindow>'
		DDX_CONTROL_HANDLE(IDLC_SCORE_INFO, m_lcScoreInfo)	// 编译通过
		DDX_CONTROL_HANDLE(IDLC_SCORE, m_lcScore)			// 编译通过
	END_DDX_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgScore)
	protected:
	//virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgScore)
	//virtual BOOL OnInitDialog();
	//}}AFX_MSG
	//DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSCORE_H__C602AC74_2F0C_4988_8304_95D5115AEF42__INCLUDED_)
