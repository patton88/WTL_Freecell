// DlgSelGame.cpp : implementation file
//

#include "stdafx.h"
//#include "JL.h"
#include "DlgSelGame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSelGame dialog

LRESULT CDlgSelGame::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());

	m_strGameNumber.Format(L"%d", m_nGameNumber);

	// First DDX call, hooks up variables to controls.
	DoDataExchange(false);

	return 0;
}

LRESULT CDlgSelGame::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

LRESULT CDlgSelGame::OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// First DDX call, hooks up controls to variables.
	DoDataExchange(true);

	EndDialog(wID);
	return 0;
}

//CDlgSelGame::CDlgSelGame(CWnd* pParent /*=NULL*/)
//	: CDialog(CDlgSelGame::IDD, pParent)
//{
//	//{{AFX_DATA_INIT(CDlgSelGame)
//	//}}AFX_DATA_INIT
//}

//void CDlgSelGame::DoDataExchange(CDataExchange* pDX)
//{
//	CDialog::DoDataExchange(pDX);
//	//{{AFX_DATA_MAP(CDlgSelGame)
//	DDX_Text(pDX, IDC_EDIT_GAMENUMBER, m_nGameNumber);
//	DDV_MinMaxUInt(pDX, m_nGameNumber, 1, 2147483647);
//	//}}AFX_DATA_MAP
//}


//BEGIN_MESSAGE_MAP(CDlgSelGame, CDialog)
//	//{{AFX_MSG_MAP(CDlgSelGame)
//	//}}AFX_MSG_MAP
//END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSelGame message handlers
