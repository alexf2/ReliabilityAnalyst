// TBrigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "diplom.h"
#include "TBrigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TBrigDlg dialog

//enum TDisOfServis { TDS_Priorety, TDS_NoPriorety, TDS_LabeledPC };
int TBrigDlg::IDtoInd( TDisOfServis ds )
 {
   switch( ds )
	{
	  case TDS_Priorety:
	    return 0;
	  case TDS_NoPriorety:
	    return 1;
	  case TDS_LabeledPC:
	    return 2;
	  default:
	   return 0;
	};
 }
TDisOfServis TBrigDlg::IndtoID( int iInd )
 {
   switch( iInd )
	{
	  case 0:
	    return TDS_Priorety;
	  case 1:
	    return TDS_NoPriorety;
	  case 2:
	    return TDS_LabeledPC;
	  default:
	    return TDS_Priorety;
	};
 }

void TBrigDlg::DoDataExchange( CDataExchange* pDX )
 {
   CDialog::DoDataExchange( pDX );

   DDX_Control( pDX, IDC_SPIN1, m_spNumb );
   
   DDX_Radio( pDX, IDC_RAD_PRIOR, m_iDISId );
   DDX_Text( pDX, IDC_EDIT_NUMBER, m_iOA );
   DDV_MinMaxInt( pDX, m_iOA, 1, UD_MAXVAL );
 }

void TBrigDlg::OnOK()
 {
   CRect r; GetWindowRect( r );
   m_rPt.x = r.left; m_rPt.y = r.top;

   CDialog::OnOK();

   m_pBr->m_iOA = m_iOA;
   m_pBr->m_tdsDIS = IndtoID( m_iDISId );
 }
BOOL TBrigDlg::OnInitDialog()
 {
   CDialog::OnInitDialog();
 
   CRect r; GetDesktopWindow()->GetWindowRect( r );
   if( !r.PtInRect(m_rPt) )
     CenterWindow( GetParent() );
   else
	 SetWindowPos( NULL, m_rPt.x, m_rPt.y, 0, 0, SWP_NOZORDER|SWP_NOSIZE|SWP_NOREDRAW );

   CString str;
   str.Format( "Бригада #%u", (int)m_pBr->ID() );
   SetWindowText( str );   

   m_spNumb.SetRange( 1, UD_MAXVAL );
   m_spNumb.SetPos( m_iOA );

   return FALSE;
 }

/////////////////////////////////////////////////////////////////////////////
// TBrigDlg message handlers
