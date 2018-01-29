#include "alndlg.hpp"

BEGIN_MESSAGE_MAP( TAlnDlg, CDialog )
  ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


void TAlnDlg::DoDataExchange( CDataExchange* pDX )
 {
   CDialog::DoDataExchange( pDX );         

   DDX_Control( pDX, IDC_BTN_A1_H, m_bA_Hor[0] );
   DDX_Control( pDX, IDC_BTN_A2_H, m_bA_Hor[1] );
   DDX_Control( pDX, IDC_BTN_A3_H, m_bA_Hor[2] );
   DDX_Control( pDX, IDC_BTN_A4_H, m_bA_Hor[3] );

   DDX_Control( pDX, IDC_BTN_A1_V, m_bA_Ver[0] );
   DDX_Control( pDX, IDC_BTN_A2_V, m_bA_Ver[1] );
   DDX_Control( pDX, IDC_BTN_A3_V, m_bA_Ver[2] );
   DDX_Control( pDX, IDC_BTN_A4_V, m_bA_Ver[3] );

   DDX_Control( pDX, IDC_BTN_D1_H, m_bD_Hor[0] );
   DDX_Control( pDX, IDC_BTN_D2_H, m_bD_Hor[1] );
   DDX_Control( pDX, IDC_BTN_D3_H, m_bD_Hor[2] );   

   DDX_Control( pDX, IDC_BTN_D1_V, m_bD_Ver[0] );
   DDX_Control( pDX, IDC_BTN_D2_V, m_bD_Ver[1] );
   DDX_Control( pDX, IDC_BTN_D3_V, m_bD_Ver[2] );
   

   DDX_Radio( pDX, IDC_RAD_A, m_bAD );
        
   DDX_Radio( pDX, IDC_BTN_A1_V, m_iAln_V );
   DDX_Radio( pDX, IDC_BTN_A1_H, m_iAln_H );
   DDX_Radio( pDX, IDC_BTN_D1_H, m_iDistr_HV );   
 }

void TAlnDlg::OnOK()
 {
   CRect r; GetWindowRect( r );
   m_rptIni.x = r.left; m_rptIni.y = r.top;

   CDialog::OnOK();
 }

BOOL TAlnDlg::OnInitDialog()
 {
   static UINT bmpS[ 14 ] =
	{
      IDB_BMP_A1_H,
	  IDB_BMP_A2_H,
	  IDB_BMP_A3_H,
	  IDB_BMP_A4_H,

	  IDB_BMP_A1_V,
	  IDB_BMP_A2_V,
	  IDB_BMP_A3_V,
	  IDB_BMP_A4_H,	  	  

	  IDB_BMP_D1_H,
	  IDB_BMP_D2_H,
	  IDB_BMP_D3_H,	  

	  IDB_BMP_D1_V,
	  IDB_BMP_D2_V,
	  IDB_BMP_D3_V	  
	};

   CDialog::OnInitDialog();
 
   CRect r; GetDesktopWindow()->GetWindowRect( r );
   if( !r.PtInRect(m_rptIni) )
     CenterWindow( GetParent() );
   else
	 SetWindowPos( NULL, m_rptIni.x, m_rptIni.y, 0, 0, SWP_NOZORDER|SWP_NOSIZE|SWP_NOREDRAW );                  

   for( int i = 0; i < 8; ++i )
     m_bmpsA[i].LoadBitmap( bmpS[i] );
   for( i = 0; i < 6; ++i )
     m_bmpsD[i].LoadBitmap( bmpS[i+8] );

   for( i = 0; i < 4; ++i )
     m_bA_Hor[i].SetBitmap( m_bmpsA[i] ),
	 m_bA_Ver[i].SetBitmap( m_bmpsA[i+4] );

   for( i = 0; i < 3; ++i )
     m_bD_Hor[i].SetBitmap( m_bmpsD[i] ),
	 m_bD_Ver[i].SetBitmap( m_bmpsD[i+3] );   

   if( m_iCntSel < 3 ) 
	{
	  ((CButton*)GetDlgItem( IDC_RAD_D ))->SetCheck( 0 );
	  ((CButton*)GetDlgItem( IDC_RAD_D ))->EnableWindow( FALSE );
	  ((CButton*)GetDlgItem( IDC_RAD_A ))->SetCheck( 1 );
	}

   HWND hw1 = ::GetDlgItem( m_hWnd, IDOK );
   PostMessage( WM_NEXTDLGCTL, (WPARAM)hw1, 1 );	        

   return FALSE;
 }

HBRUSH TAlnDlg::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
 {
   HBRUSH hbr = CDialog::OnCtlColor( pDC, pWnd, nCtlColor );
   
   int id = pWnd->GetDlgCtrlID();
   if( id == IDC_RAD_A || id == IDC_RAD_D )
	 pDC->SetTextColor( RGB(0,0,255) );

   return hbr;
 }
