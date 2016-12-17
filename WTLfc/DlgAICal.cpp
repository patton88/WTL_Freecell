// DlgAICal.cpp : implementation file
//

#include "stdafx.h"
//#include "JL.h"
//#include "JLDoc.h"
#include "DlgAICal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAICal dialog
#define ID_TIMER_STEP 300
//extern CJLDoc * AfxGetDocument();
//extern CJLView * AfxGetView();
extern volatile bool g_bStopAICal;

//CDlgAICal::CDlgAICal(CWnd* pParent /*=NULL*/)
//	: CDialog(CDlgAICal::IDD, pParent)
//{
//	//{{AFX_DATA_INIT(CDlgAICal)
//	//}}AFX_DATA_INIT
//}
//
//void CDlgAICal::DoDataExchange(CDataExchange* pDX)
//{
//	CDialog::DoDataExchange(pDX);
//	//{{AFX_DATA_MAP(CDlgAICal)
//	DDX_Control(pDX, IDCHK_MAX_SPEED, m_chkMaxSpeed);
//	DDX_Control(pDX, IDB_STOP, m_btnStop);
//	DDX_Control(pDX, IDC_PROGRESS_STATUS, m_ctrlProgress);
//	//}}AFX_DATA_MAP
//}

//BEGIN_MESSAGE_MAP(CDlgAICal, CDialog)
//	//{{AFX_MSG_MAP(CDlgAICal)
//	ON_BN_CLICKED(IDCHK_MAX_SPEED, OnMaxSpeed)
//	ON_BN_CLICKED(IDB_STOP, OnStop)
//	ON_WM_TIMER()
//	//}}AFX_MSG_MAP
//END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAICal message handlers
//开启一个线程执行自动解答
//UINT AICalThread(void * para)
unsigned __stdcall AICalThread(void * para)
{
	CDlgAICal* pDlg = (CDlgAICal*)para;
	//CJLDoc *pDoc = AfxGetDocument();
	pDlg->m_bSuccess = g_fcData.DoAICal();

	pDlg->KillTimer(pDlg->m_nTimer);
	//pDlg->EndDialog(0);
	//pDlg->EndDialog(0);

	return 0;
}

//void CDlgAICal::OnStop() 
LRESULT CDlgAICal::OnStop(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add extra cleanup here
	//点击“停止”按扭后该按扭将变成灰色，必须等待牌局还原后对话框才会关闭
	m_btnStop.EnableWindow(FALSE);
	SetWindowText(TEXT("正在还原牌局，请等待......"));
	g_bStopAICal = true;//停止解答

	return 0;
}

void CDlgAICal::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	//CJLDoc *pDoc = AfxGetDocument();
	//实时显示计算到了哪一步，通常成功的解答步数介于30-70步之间
	//如果接近80步说明此局可能解不开，玩家可停止自动解答
	if (!g_fcData.m_OpsList.m_tList.empty()) {
		int cnt = g_fcData.m_OpsList.m_tList.size();
		m_ctrlProgress.SetPos(1+cnt%100);
	}
	//CDialog::OnTimer(nIDEvent);
}

void CDlgAICal::StartThread()
{
	HANDLE   hth1;
	unsigned  uiThread2ID;

	hth1 = (HANDLE)_beginthreadex(
		NULL,				// security
		0,					// stack size
		AICalThread,
		this,				// arg list
		0,	// so we can later call ResumeThread()
		&uiThread2ID);

	WaitForSingleObject(hth1, INFINITE);

	DWORD   dwExitCode;
	GetExitCodeThread(hth1, &dwExitCode);

	// The handle returned by _beginthreadex() has to be closed
	// by the caller of _beginthreadex().

	CloseHandle(hth1);
}

//BOOL CDlgAICal::OnInitDialog() 
LRESULT CDlgAICal::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	//CDialog::OnInitDialog();
	// First DDX call, hooks up variables to controls.
	DoDataExchange(false);

	////进度每秒刷新5次
	m_nTimer = SetTimer(ID_TIMER_STEP,1000/5,NULL);
	////通常不会超过100步
	m_ctrlProgress.SetRange(1,100);
	////默认打开“快速解答”选项
	m_chkMaxSpeed.SetCheck(1);
	BOOL b;
	OnMaxSpeed(NULL, NULL, NULL, b);

	StartThread();
	//m_hCalThread = AfxBeginThread(AICalThread,this,THREAD_PRIORITY_LOWEST,0,0);
	EndDialog(0);

	return TRUE;
}
//单击“快速解答”后将执行此函数
//void CDlgAICal::OnMaxSpeed()
LRESULT CDlgAICal::OnMaxSpeed(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//CJLDoc *pDoc = AfxGetDocument();
	g_fcData.m_bRealTimeUpdate = m_chkMaxSpeed.GetCheck() ? false : true;

	return 0;
}
