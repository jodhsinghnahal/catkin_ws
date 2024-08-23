// XBDll.h : main header file for the XBDll DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


// CTestDllApp
// See TestDll.cpp for the implementation of this class
//

class CXBDllApp : public CWinApp
{
public:
	CXBDllApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
