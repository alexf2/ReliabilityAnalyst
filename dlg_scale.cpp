// TBrigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "diplom.h"
#include "dlg_scale.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP( TScaleDlg, CDialog )
  ON_WM_HSCROLL()
END_MESSAGE_MAP()

void TScaleDlg::DoDataExchange( CDataExchange* pDX )
 {
   CDialog::DoDataExchange( pDX );

   if( pDX->m_bSaveAndValidate )
	 m_iSc = m_sl.GetPos();
   DDX_Control( pDX, IDC_SLIDER1, m_sl );   
   DDX_Radio( pDX, IDC_RADIO1, m_iUpDownd );   
   DDX_Control( pDX, IDC_EDIT1, m_edSc );
   DDX_Radio( pDX, IDC_RAD_SELF, m_iCenter );
   DDX_Text(  pDX, IDC_EDIT1, m_dRes );
   DDV_MinMaxDouble(  pDX, m_dRes, 1, 100 );
 }

void TScaleDlg::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
 {
   m_dRes = m_iSc = m_sl.GetPos();
   char cBuf[ 100 ];
   sprintf( cBuf, "%u", m_iSc );
   m_edSc.SetWindowText( cBuf );

   CDialog::OnHScroll( nSBCode, nPos, pScrollBar );
 }

void TScaleDlg::OnOK()
 {
   CRect r; GetWindowRect( r );
   m_rPt.x = r.left; m_rPt.y = r.top;

   CDialog::OnOK();

   m_dRes = m_dRes > 0 ? m_dRes:1;
   if( m_iUpDownd == 1 ) m_dRes = 1.0 / m_dRes;
 }
BOOL TScaleDlg::OnInitDialog()
 {
   CDialog::OnInitDialog();
 
   CRect r; GetDesktopWindow()->GetWindowRect( r );
   if( !r.PtInRect(m_rPt) )
     CenterWindow( GetParent() );
   else
	 SetWindowPos( NULL, m_rPt.x, m_rPt.y, 0, 0, SWP_NOZORDER|SWP_NOSIZE|SWP_NOREDRAW );

   m_sl.SetRange( 1, 10 );
   m_sl.SetPos( m_iSc );
   m_sl.SetTicFreq( 1 );

   return FALSE;
 }

/////////////////////////////////////////////////////////////////////////////
// TBrigDlg message handlers
