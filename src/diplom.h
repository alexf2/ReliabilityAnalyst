// diplom.h : main header file for the DIPLOM application
//

#if !defined(AFX_DIPLOM_H__FE33C084_CBA2_11D1_9887_00504E02C39D__INCLUDED_)
#define AFX_DIPLOM_H__FE33C084_CBA2_11D1_9887_00504E02C39D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// TDiplomApp:
// See diplom.cpp for the implementation of this class
//

#include "stdafx.h"
#include "..\\RUDAKOW\\SERVIS\\bwcc.hpp"

#define STR_CONFIGNAME "as.cfg"

class TDiplomApp : public CWinApp
{
public:
	TBWCC bwcc;
	TDiplomApp();

	CString& GetConfig() { return m_csConfig; }
	
	virtual BOOL SaveAllModified();	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TDiplomApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(TDiplomApp)
	afx_msg void OnFileNew();
	//}}AFX_MSG
	
	afx_msg void OnFilePrintSetup();

	LPDEVMODE GetDevMode_()
	 {
	   return (LPDEVMODE)m_hDevMode;
	 }

	virtual void WinHelp( DWORD dwData, UINT nCmd = HELP_CONTEXT );

protected:
    CString m_csConfig, m_csStartPath;
	bool m_bFlNoFileNew;

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIPLOM_H__FE33C084_CBA2_11D1_9887_00504E02C39D__INCLUDED_)
