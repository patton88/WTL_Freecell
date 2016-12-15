// WTLfc.cpp : main source file for WTLfc.exe
//

#include "stdafx.h"

#include "resource.h"

#include "WTLfcView.h"
#include "aboutdlg.h"
#include "MainFrm.h"

CAppModule _Module;
CCardSize g_fcCcs;
CWTLfcData g_fcData;
CWTLfcView* g_pView;
CMainFrame* g_pMain;
bool g_bMaxOnly = false;

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainFrame wndMain;

	g_pMain = &wndMain;

	if(wndMain.CreateEx() == NULL)
	{
		ATLTRACE(_T("Main window creation failed!\n"));
		return 0;
	}

	wndMain.CenterWindow();

	//if (1)
	if (GetSystemMetrics(SM_CXSCREEN) < 1213 || GetSystemMetrics(SM_CYSCREEN) < 850)
	{
		// 若屏幕尺寸小于缺省窗口大小，便禁用窗口还原菜单 
		// 通过WTL的BOOL CMainFrame::OnIdle()的UIEnable只能处理工具栏和二级菜单项的启用禁用，不能处理顶级菜单
		// 必须在通过WTL的BOOL CMainFrame::OnIdle()中使用EnableMenuItem才行
		g_bMaxOnly = true;
		wndMain.ShowWindow(SW_SHOWMAXIMIZED);

		// EnableMenuItem放在在这里无效，只能放在BOOL CMainFrame::OnIdle()中才有效
		//::EnableMenuItem(g_pMain->GetMenu(), ID_WND_RESTORE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		//::EnableMenuItem(g_pMain->GetMenu(), ID_WND_RESTORE_TOP, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		// 在VS2013中，即使在CMainFrame::CMainFrame()中加入m_bAutoMenuEnable = FALSE;一句，禁用菜单也不行。VC6中可以
		//g_pMain->GetMenu()->GetSubMenu(0)->EnableMenuItem(ID_WND_RESTORE, MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);
		// 在VS2013中，使用CMainFrame::OnUpdateWndRestore中使用pCmdUI->Enable(FALSE);能够禁用菜单项
	}
	else
	{
		g_bMaxOnly = false;
		wndMain.ShowWindow(nCmdShow);
		g_pView->m_bWndRestored = false;
		wndMain.OnWndRestore(0, 0, 0);	//避免启动后就单击菜单“窗口还原”发生变动
	}

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
