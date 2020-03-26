//Download by http://www.NewXing.com
// Mahjong.h : main header file for the MAHJONG application
//

#if !defined(AFX_MAHJONG_H__DA775C04_B2A9_45BA_9C9C_4C551479E759__INCLUDED_)
#define AFX_MAHJONG_H__DA775C04_B2A9_45BA_9C9C_4C551479E759__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMahjongApp:
// See Mahjong.cpp for the implementation of this class
//

class CMahjongApp : public CWinApp
{
public:
	CMahjongApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMahjongApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMahjongApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAHJONG_H__DA775C04_B2A9_45BA_9C9C_4C551479E759__INCLUDED_)
