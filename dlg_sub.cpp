// TBrigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "diplom.h"
#include "dlg_sub.hpp"
#include "lock_update.hpp"


void TSubDlg::DoDataExchange( CDataExchange* pDX )
 {
   CDialog::DoDataExchange( pDX );
   
   DDX_Control( pDX, IDC_LIST_DP, m_lstBox );      
 }

void TSubDlg::OnOK()
 {  
   int iSelCnt = m_lstBox.GetSelCount();

   if( iSelCnt > 0 && iSelCnt != LB_ERR )
	{
	  auto_ptr<int> apInt( new int[iSelCnt] );
	  m_lstBox.GetSelItems( iSelCnt, apInt.get() );
	  
	  for( int i = 0; i < iSelCnt; ++i )
		m_lstSel.push_back( apInt.get()[i] );
	}
   
   CDialog::OnOK();   
 }
BOOL TSubDlg::OnInitDialog()
 {
   CDialog::OnInitDialog();
 
   TLockUpdate lock( &m_lstBox );

   list<CString>::iterator itSta( m_lstFill.begin() );
   list<CString>::iterator itEnd( m_lstFill.end() );

   for( ; itSta != itEnd; ++itSta )
     m_lstBox.InsertString( -1, *itSta );

   m_lstBox.SendMessage( WM_SETFONT, 
	  (WPARAM)GetStockObject(SYSTEM_FONT), 0 );
   
   return FALSE;
 }
/////////////////////////////////////////////////////////////////////////////
// TBrigDlg message handlers
