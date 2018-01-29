#if !defined(_TSUB_DLG_)
#define _TSUB_DLG_


#include "stdafx.h"
#include "resource.h"

#include <list>
using namespace std;

class TSubDlg: public CDialog
{
// Construction
public:
	TSubDlg( CWnd* pParent ):
	  CDialog( IDD_DIALOG_SCSUB, pParent )	  
	   { 	     
	   }


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual void OnOK();
	virtual BOOL OnInitDialog();	

public:    	
   CListBox m_lstBox;
   list<short> m_lstSel;
   list<CString> m_lstFill;
 };

#endif
