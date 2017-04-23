// DlgDefGame.cpp : implementation file
//

#include "stdafx.h"
//#include "JL.h"
//#include "JLDoc.h"
#include "DlgDefGame.h"


//#ifdef _DEBUG
//#define new DEBUG_NEW
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#endif
//
//
//extern CJLDoc * AfxGetDocument();
//extern CJLView * AfxGetView();

/////////////////////////////////////////////////////////////////////////////
// CDlgDefGame dialog


//CDlgDefGame::CDlgDefGame(CWnd* pParent /*=NULL*/)
//	: CDialog(CDlgDefGame::IDD, pParent)
CDlgDefGame::CDlgDefGame() : m_bMsgHandled(FALSE)
{
	//{{AFX_DATA_INIT(CDlgDefGame)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	//在客户区创建一个窗口用于编辑牌局，所有的编辑功能都由此窗口完成
	//m_pDGWnd = new CDGWnd;
}

//删除窗口
CDlgDefGame::~CDlgDefGame()
{
	//delete m_pDGWnd;
}


//void CDlgDefGame::DoDataExchange(CDataExchange* pDX)
//{
//	CDialog::DoDataExchange(pDX);
//	//{{AFX_DATA_MAP(CDlgDefGame)
//		// NOTE: the ClassWizard will add DDX and DDV calls here
//	//}}AFX_DATA_MAP
//}


//BEGIN_MESSAGE_MAP(CDlgDefGame, CDialog)
//	//{{AFX_MSG_MAP(CDlgDefGame)
//	ON_WM_CREATE()
//	ON_WM_SETFOCUS()
//	ON_WM_SIZING()
//	ON_COMMAND(IDM_SAVE_DEF, OnSaveDef)
//	ON_COMMAND(IDM_LOAD_DEF, OnLoadDef)
//	ON_WM_CLOSE()
//	ON_COMMAND(IDM_SAVE_DEF_AS, OnSaveDefAs)
//	ON_COMMAND(IDM_NEW_DEF, OnNewDef)
//	//}}AFX_MSG_MAP
//END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDefGame message handlers

//int CDlgDefGame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
LRESULT CDlgDefGame::OnCreate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	//if (CDialog::OnCreate(lpCreateStruct) == -1)
	//	return -1;
	
	//wndIE.Create(*this, rc, _T(""), WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN);

	// TODO: Add your specialized creation code here
	////创建一个窗口占据整个客户区
	//CRect r;
	//GetClientRect(r);
	////m_pDGWnd->Create(NULL,NULL,WS_CHILD|WS_VISIBLE,r,this,210);
	//m_pDGWnd->Create(m_hWnd, r, NULL, WS_CHILD | WS_VISIBLE, WS_EX_CLIENTEDGE);
	////m_view.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);

	//return CWindowImplBaseT< TBase, TWinTraits >::Create(hWndParent, rect, szWindowName,
	//	dwStyle, dwExStyle, MenuOrID, atom, lpCreateParam);

	return 0;
}

//void CDlgDefGame::OnSetFocus(CWnd* pOldWnd) 
LRESULT CDlgDefGame::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//CDialog::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	m_DGWnd.SetFocus(); 

	return 0;
}

//void CDlgDefGame::OnSizing(UINT fwSide, LPRECT pRect) 
LRESULT CDlgDefGame::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//CDialog::OnSizing(fwSide, pRect);
	
	// TODO: Add your message handler code here
	CRect r;
	GetClientRect(r);
	m_DGWnd.MoveWindow(r);
	m_DGWnd.Invalidate();

	return 0;
}
//调整对话框大小和主程序窗口一样大，让编辑牌局的窗口占据对话框的整个客户区
//BOOL CDlgDefGame::OnInitDialog() 
LRESULT CDlgDefGame::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	//CDialog::OnInitDialog();
	
	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME),
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME),
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	//创建一个窗口占据整个客户区
	CRect r;
	GetClientRect(r);
	//m_pDGWnd->Create(NULL,NULL,WS_CHILD|WS_VISIBLE,r,this,210);
	m_DGWnd.Create(m_hWnd, r, NULL, WS_CHILD | WS_VISIBLE, WS_EX_CLIENTEDGE);
	//m_view.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);


	// TODO: Add extra initialization here
	//CRect r;
	//CWnd *pParent = GetParent();
	//pParent->GetWindowRect(r);
	::GetWindowRect(GetParent(), r);
	MoveWindow(r);

	GetClientRect(r);
	m_DGWnd.MoveWindow(r);

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//void CDlgDefGame::OnClose() 
LRESULT CDlgDefGame::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// TODO: Add your message handler code here and/or call default
	if (!m_DGWnd.GiveUp()) return 0;

	EndDialog(0);
	//CDialog::OnClose();

	return 0;
}

//void CDlgDefGame::OnSaveDef() 
LRESULT CDlgDefGame::OnSaveDef(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_DGWnd.SaveDefGame();

	return 0;
}

//void CDlgDefGame::OnLoadDef() 
LRESULT CDlgDefGame::OnLoadDef(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_DGWnd.LoadDefGame();

	return 0;
}

//void CDlgDefGame::OnSaveDefAs() 
LRESULT CDlgDefGame::OnSaveDefAs(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_DGWnd.SaveDefGameAs();

	return 0;
}

//void CDlgDefGame::OnNewDef() 
LRESULT CDlgDefGame::OnNewDef(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_DGWnd.NewDefGame();

	return 0;
}

LRESULT CDlgDefGame::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

LRESULT CDlgDefGame::OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// First DDX call, hooks up controls to variables.
	//DoDataExchange(true);

	EndDialog(wID);
	return 0;
}
