// CHyperLinkWrapper.h
// 为避免Debug模式下系统没有默认浏览器、默认邮件程序时报错终止
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHyperLinkWrapper_H__C84_E69_d49_i41_t05_I29_00007733__INCLUDED_)
#define AFX_CHyperLinkWrapper_H__C84_E69_d49_i41_t05_I29_00007733__INCLUDED_

#if _MSC_VER >= 1000
	#pragma once
#endif // _MSC_VER >= 1000

// CHyperLinkWrapper - CWindowImpl-derived class that implements a button.  We need a
// class like this to do subclassing or DDX.
class CHyperLinkWrapper : public CHyperLinkImpl<CHyperLinkWrapper>
{
public:
	DECLARE_WND_CLASS(_T("WTL_HyperLink"))
	bool Navigate();
};

#endif // !defined(AFX_CHyperLinkWrapper_H__C84_E69_d49_i41_t05_I29_00007733__INCLUDED_)

//////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
