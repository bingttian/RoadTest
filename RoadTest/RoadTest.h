// RoadTest.h : main header file for the RoadTest application
//

#if !defined(AFX_FAST_H__24A06FF8_0429_4F63_B0F4_1155E4C27755__INCLUDED_)
#define AFX_FAST_H__24A06FF8_0429_4F63_B0F4_1155E4C27755__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFastApp:
// See RoadTest.cpp for the implementation of this class
//

class CFastApp : public CWinApp
{
public:
	CFastApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFastApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFastApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FAST_H__24A06FF8_0429_4F63_B0F4_1155E4C27755__INCLUDED_)
