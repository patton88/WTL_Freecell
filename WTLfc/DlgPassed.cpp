// PassedDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "JL.h"
#include "DlgPassed.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPassedDlg dialog


//CDlgPassed::CDlgPassed(CWnd* pParent /*=NULL*/)
//	: CDialog(CDlgPassed::IDD, pParent)
//CDlgPassed::CDlgPassed()
//{
//	//{{AFX_DATA_INIT(CPassedDlg)
//	m_nChoice = 0;
//	//}}AFX_DATA_INIT
//}

LRESULT CDlgPassed::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());
	m_nChoice = 0;
	return 0;
}

//void CDlgPassed::DoDataExchange(CDataExchange* pDX)
//{
//	CDialog::DoDataExchange(pDX);
//	//{{AFX_DATA_MAP(CPassedDlg)
//	//}}AFX_DATA_MAP
//}


/////////////////////////////////////////////////////////////////////////////
// CPassedDlg message handlers
//void CDlgPassed::OnPrevGame()
LRESULT CDlgPassed::OnPrevGame(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_nChoice = PrevGame;
	//CDialog::OnOK();
	EndDialog(0);
	return 0;
}

LRESULT CDlgPassed::OnNextGame(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_nChoice = NextGame;
	//CDialog::OnOK();
	EndDialog(0);
	return 0;
}

LRESULT CDlgPassed::OnRand(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_nChoice = RandGame;
	//CDialog::OnOK();
	EndDialog(0);
	return 0;
}

LRESULT CDlgPassed::OnAgain(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_nChoice = ThisGame;
	//CDialog::OnOK();
	EndDialog(0);
	return 0;
}

LRESULT CDlgPassed::OnSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_nChoice = SaveGame;
	//CDialog::OnOK();
	EndDialog(0);
	return 0;
}

LRESULT CDlgPassed::OnShow(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_nChoice = PlayShow;
	//CDialog::OnOK();
	EndDialog(0);
	return 0;
}

LRESULT CDlgPassed::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}
