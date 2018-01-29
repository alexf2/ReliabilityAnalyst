#include <typeinfo.h>

#include "dlgprop.hpp"
#include "drawobj.h"

BEGIN_MESSAGE_MAP( TPropSheet, TBWCCPropertySheet )  
  //ON_WM_CREATE()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP( TPRopPage_Line, CPropPageTooltips )
  ON_CBN_SELENDOK( IDC_COMBO_CLR, OnSelClr )
  ON_BN_CLICKED( IDC_BTN_CLREXT, OnClrExt )
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP( TPRopPage_Fill, CPropPageTooltips )
  ON_CBN_SELENDOK( IDC_COMBO_FONCLR, OnSelClrBk )
  ON_CBN_SELENDOK( IDC_COMBO_CLR, OnSelClr )
  ON_BN_CLICKED( IDC_BTN_CLREXT, OnClrExt )
  ON_BN_CLICKED( IDC_BTN_CLREXTBK, OnClrExtBk )
END_MESSAGE_MAP()


void TColorCombo::DrawContent( CDC& dc, CRect& r, LPDRAWITEMSTRUCT lpD )
 {
   int iSav = dc.SaveDC();

   DWORD dwDta = GetItemData( lpD->itemID );
   int iMid = (r.top + r.bottom) / 2;
   dc.SetBkMode( TRANSPARENT );

   CPen   pen;
   CBrush br;

   switch( m_typ )
	{
	  case TCC_LinePattern:
	   {
	     pen.CreatePen( dwDta, 0, (COLORREF)m_dwExtra1 );
		 dc.SelectObject( &pen );
		 dc.MoveTo( r.left, iMid );
		 dc.LineTo( r.right, iMid );
	   };
	   break;
	  case TCC_LineWeight:
	   {
	     LOGBRUSH lbr = {
		   BS_SOLID,
		   (COLORREF)m_dwExtra1,
		   0
		  };
	     pen.CreatePen( PS_SOLID|PS_GEOMETRIC|PS_ENDCAP_FLAT|PS_JOIN_MITER, 
		    dwDta, &lbr );
		 dc.SelectObject( &pen );
		 dc.BeginPath();
		 dc.MoveTo( r.left, iMid );
		 dc.LineTo( r.right, iMid );
		 dc.EndPath();
         dc.StrokePath();
	   };
	   break;
	  case TCC_Color:
	   {
	     LOGBRUSH logBr =
		  {
		    int(dwDta) == -1 ? BS_NULL:BS_SOLID,
			dwDta,
			0
		  };		 
	     br.CreateBrushIndirect( &logBr );		 
	     dc.FillRect( r, &br );		 

		 if( int(dwDta) == -1 )
		  {
			dc.SelectStockObject( ANSI_VAR_FONT );
			CString cs( "Прозрачный" );
			dc.SetBkMode( TRANSPARENT );
			dc.SetTextColor( 0 );
			dc.DrawText( cs, r, DT_SINGLELINE|DT_CENTER|DT_VCENTER|DT_NOPREFIX );
		  }
	   };
	   break;
	  case TCC_FillPattern:
	   {
	     LOGBRUSH logBr =
		  {
		    int(dwDta) == -1 ? BS_SOLID:(int(dwDta) == -2 ? BS_NULL:BS_HATCHED),
            (COLORREF)m_dwExtra1,
			int(dwDta) >= 0 ? dwDta:0
		  };			 
	     br.CreateBrushIndirect( &logBr );
		 if( int(m_dwExtra2) == -1 )
		  {
		    dc.FillSolidRect( r, RGB(255,255,255) );
		    dc.SetBkMode( TRANSPARENT );
		  }
		 else
		   dc.SetBkMode( OPAQUE ),
		   dc.SetBkColor( m_dwExtra2 );
	     dc.FillRect( r, &br );
          
		 if( int(dwDta) == -2 )
		  {
			dc.SelectStockObject( ANSI_VAR_FONT );
			CString cs( "Прозрачный" );
			dc.SetBkMode( TRANSPARENT );
			dc.SetTextColor( 0 );
			dc.DrawText( cs, r, DT_SINGLELINE|DT_CENTER|DT_VCENTER|DT_NOPREFIX );
		  }
	   };
	   break;
	};

   dc.RestoreDC( iSav );
 }

void TColorCombo::DrawItem( LPDRAWITEMSTRUCT lpD )
 {
   CDC dc; dc.Attach( lpD->hDC );

   CRect r( lpD->rcItem );   
   //CRect rC(r); //GetClientRect( rC );
   //r.right = rC.right;
   r.DeflateRect( 4, 2 );

   //CBrush br( (COLORREF)GetItemData(lpD->itemID) );   
   //dc.FillRect( r, &br );   
   DrawContent( dc, r, lpD );   

   CRect r2( r ); //r2.bottom++;

   CBrush blackBr( RGB(0,0,0) );
   if( m_typ != TCC_LineWeight )
     dc.FrameRect( r2, &blackBr );

   if( lpD->itemState & (ODS_FOCUS|ODS_SELECTED) )
	{
	  CBrush rbFr( RGB(0,0,255) );
	  r.InflateRect( 2, 2 );
      dc.FrameRect( r, &rbFr );
	}
   else
	{ 
      CBrush rbFr( RGB(0xFF,0xFF,0xFF) );
	  r.InflateRect( 2, 2 );
      dc.FrameRect( r, &rbFr );
	}

   dc.Detach();
 }

void TColorCombo::MeasureItem( LPMEASUREITEMSTRUCT lpM )
 {
   CWindowDC dc( NULL );
   CFont *pFOld = (CFont*)dc.SelectStockObject( SYSTEM_FONT );

   CSize sz = dc.GetTextExtent( "ABC", 3 );

   dc.SelectObject( pFOld );

   lpM->itemWidth  = sz.cx;
   lpM->itemHeight = sz.cy;
 }


TPRopPage_PC::TPRopPage_PC( TDrawPC* pPC ): 
 CPropPageTooltips( IDD_PROPPAGE_PC ) 
  {	
    m_sRang = pPC->m_sRang;
	m_sNum = pPC->m_sNum;
    m_dLambda = pPC->m_dLambda;
	m_dM = pPC->m_dM;
	m_dD = pPC->m_dD;
    m_csName = pPC->m_csName;

    m_pPC = pPC;

	*this + 
	   new TToolTipData( IDC_ED_NAME, "Название устройства" ) +
	   new TToolTipData( IDC_ED_RANG, "Ранг устройства" ) +
	   new TToolTipData( IDC_ED_NUMBER, "Номер устройства" ) +
	   new TToolTipData( IDC_ED_L, "Интенсивность потока отказов" ) +
	   new TToolTipData( IDC_ED_M, "Математическое ожидание времени ремонта" ) +
	   new TToolTipData( IDC_ED_D, "Дисперсия времени ремонта" );   		
  }
TPRopPage_PC::~TPRopPage_PC()
 {
 }


BOOL TPRopPage_PC::OnInitDialog()
 {
   CPropPageTooltips::OnInitDialog();
   	
   m_sp1.SetRange( 1, UD_MAXVAL );
   m_sp1.SetPos( m_sRang );
   m_sp2.SetRange( 1, UD_MAXVAL );
   m_sp2.SetPos( m_sNum );

   HWND hw1 = ::GetDlgItem( m_hWnd, IDC_ED_RANG );
   PostMessage( WM_NEXTDLGCTL, (WPARAM)hw1, 1 );	              

   return FALSE;
 }

void TPRopPage_PC::OnOK()
 {   
   CPropPageTooltips::OnOK();

   m_pPC->m_sRang = m_sRang;
   m_pPC->m_sNum = m_sNum;
   m_pPC->m_dLambda = m_dLambda;
   m_pPC->m_dM = m_dM;
   m_pPC->m_dD = m_dD;
   m_pPC->m_csName = m_csName;   

   m_pPC->CalcExtraParams();

   m_pPC->GenerateTTY();
 }

void TPRopPage_PC::DoDataExchange( CDataExchange* pDX )
 {
   CPropertyPage::DoDataExchange( pDX );
   
   DDX_Control( pDX, IDC_SPIN_PASS, m_sp1 );
   DDX_Control( pDX, IDC_SPIN_PASS2, m_sp2 );
   
   
   DDX_Text( pDX, IDC_ED_NAME, m_csName );
   DDV_MaxChars( pDX, m_csName, 255 );    
   
   DDX_Text( pDX, IDC_ED_RANG, m_sRang );
   DDV_MinMaxInt( pDX, m_sRang, 1, USHRT_MAX );
   DDX_Text( pDX, IDC_ED_NUMBER, m_sNum );
   DDV_MinMaxInt( pDX, m_sNum, 1, USHRT_MAX );

   DDX_Text( pDX, IDC_ED_L, m_dLambda );
   DDV_MinMaxDouble( pDX, m_dLambda, 0, ULONG_MAX );
   DDX_Text( pDX, IDC_ED_M, m_dM );
   DDV_MinMaxDouble( pDX, m_dM, 0, ULONG_MAX );
   DDX_Text( pDX, IDC_ED_D, m_dD );
   DDV_MinMaxDouble( pDX, m_dD, 0, ULONG_MAX );   
 }


TPRopPage_Text::TPRopPage_Text( TDrawTxt* pPC ):
  CPropPageTooltips( IDD_PROPPAGE_TXT ),
  m_csTxt( pPC->m_csTxt )
 {
   m_pPC = pPC;   

   m_iMulti = pPC->m_bMultiLine;

   *this + 
	   new TToolTipData( IDC_ED_TEXT, "Текст, который будет выводиться" );
 }
	
TPRopPage_Text::~TPRopPage_Text()
 {
 }

BOOL TPRopPage_Text::OnInitDialog()
 {
   CPropPageTooltips::OnInitDialog();

   HWND hw1 = ::GetDlgItem( m_hWnd, IDC_ED_TEXT );
   PostMessage( WM_NEXTDLGCTL, (WPARAM)hw1, 1 );	           

   return FALSE;
 }
void TPRopPage_Text::DoDataExchange( CDataExchange* pDX )
 {
   CPropertyPage::DoDataExchange( pDX );
   
   DDX_Text( pDX, IDC_ED_TEXT, m_csTxt );
   DDV_MaxChars( pDX, m_csTxt, 255 );    

   DDX_Check( pDX, IDC_CHECK1, m_iMulti );
 }
void TPRopPage_Text::OnOK()
 {
   CPropPageTooltips::OnOK();

   m_pPC->m_csTxt = m_csTxt;   
   m_pPC->m_bMultiLine = m_iMulti;
 }
  


TPropSheet::~TPropSheet()
 {
 }

TPropSheet::TPropSheet( CPoint& rPt, TH_HintPage hnt, LPCTSTR pszCaption, CWnd *pParentWnd  ):
  TBWCCPropertySheet( pszCaption, pParentWnd, 0 ),
  m_rptIni( rPt ),
  m_hnt( hnt )
 {
   /*AddPage( &m_pagUser ); 
   AddPage( &m_pagExp ); 
   AddPage( &m_pagKognit ); 
   AddPage( &m_pagAdm );*/
 }

  
BOOL TPropSheet::DestroyWindow()
 {
   CRect r; GetWindowRect( r );
   m_rptIni.x = r.left; m_rptIni.y = r.top;

   return TBWCCPropertySheet::DestroyWindow();
 }

int TPropSheet::FindPage( TH_HintPage hp )
 {
   for( int i = GetPageCount() - 1; i >= 0; --i )
	 if( GetPage(i) )
	  {
        if( hp == TH_Line && 
		    typeid(*GetPage(i)) == typeid(TPRopPage_Line) 
		  ) return i;
		if( hp == TH_Fill && 
		    typeid(*GetPage(i)) == typeid(TPRopPage_Fill) 
		  ) return i;
	  }

   return 0;
 }


//int TPropSheet::OnCreate( LPCREATESTRUCT lpC )
BOOL TPropSheet::OnInitDialog()
 {
   TBWCCPropertySheet::OnInitDialog();	  

   CRect r; GetDesktopWindow()->GetWindowRect( r );
   if( !r.PtInRect(m_rptIni) )
     CenterWindow( GetParent() );
   else
	 SetWindowPos( NULL, m_rptIni.x, m_rptIni.y, 0, 0, SWP_NOZORDER|SWP_NOSIZE|SWP_NOREDRAW ); 

   int iSel = 0;
   switch( m_hnt )
	{
	  case TH_Property:
	    iSel = 0;
		break;
	  case TH_Line:
	    iSel = FindPage( TH_Line );
		break;
	  case TH_Fill:
	    iSel = FindPage( TH_Fill );
		break;
	};
   SetActivePage( iSel );

   return FALSE;
 }


TPRopPage_Line::TPRopPage_Line( TPropEdit* pPC ): 
 CPropPageTooltips( IDD_PROPPAGE_LINE ),
  m_cbnPat( TCC_LinePattern ),
  m_cbnWeight( TCC_LineWeight ),
  m_cbnClr( TCC_Color )
  {	    
    m_pPC = pPC;
	m_patt = (*(TTItem_Pen&)pPC->m_ttEdit).m_iStyle;
	m_w = (*(TTItem_Pen&)pPC->m_ttEdit).m_iWidth;
    m_clr = (*(TTItem_Pen&)pPC->m_ttEdit).m_clr;	
	   
	*this + 
	   new TToolTipData( IDC_COMBO_PATTERN, "Тип линии" ) +
	   new TToolTipData( IDC_COMBO_WEIGHT, "Толщина линии" ) +
	   new TToolTipData( IDC_COMBO_CLR, "Цвет линии" ) +
	   new TToolTipData( IDC_BTN_CLREXT, "Дополнительные цвета" );

	apPattern = Line_EnumPattern();
	apThick = Line_EnumThick();
	apClr = Common_EnumColor();

	m_iInd_patt = apPattern->IndexOf( m_patt );
	m_iInd_w = apThick->IndexOf( m_w ); 
	m_iInd_clr = apClr->IndexOf( m_clr );
  }
TPRopPage_Line::~TPRopPage_Line()
 {
 }

void TPRopPage_Line::OnClrExt()
 {
   CColorDialog dlgColor( (COLORREF)m_cbnClr.GetItemData(m_cbnClr.GetCurSel()), 0, this );
   if( dlgColor.DoModal() == IDOK )
	{ 
	  int iInd = m_cbnClr.AddString( "" );
	  m_cbnClr.SetItemData( iInd, (DWORD)dlgColor.GetColor() );
	  m_cbnClr.SetCurSel( iInd );	  
	  apClr->IndexOf( dlgColor.GetColor() );
	  OnSelClr();
    }
 }

BOOL TPRopPage_Line::OnInitDialog()
 {
   CPropPageTooltips::OnInitDialog();
    
   int iSlct = 0;
   m_cbnPat.InitStorage( apPattern->GetSize(), 4 );
   m_cbnPat.SetRedraw( FALSE );
   for( int i = 0; i < apPattern->GetSize(); ++i )
	{
      m_cbnPat.SetItemData( m_cbnPat.InsertString(i,""), (*apPattern)[i] );
	  if( (*apPattern)[i] == m_patt ) iSlct = i;
	}
   m_cbnPat.SetRedraw( TRUE );

   m_cbnPat.SetCurSel( iSlct );   
   m_iInd_patt = iSlct;
	

   iSlct = 0;
   m_cbnWeight.InitStorage( apThick->GetSize(), 4 );
   m_cbnWeight.SetRedraw( FALSE );
   for( i = 0; i < apThick->GetSize(); ++i )
	{
      m_cbnWeight.SetItemData( m_cbnWeight.InsertString(i,""), (*apThick)[i] );
	  if( (*apThick)[i] == m_w ) iSlct = i;
	}
   m_cbnWeight.SetRedraw( TRUE );

   m_cbnWeight.SetCurSel( iSlct );   
   m_iInd_w = iSlct;	

   iSlct = 0;
   m_cbnClr.InitStorage( apClr->GetSize(), 4 );
   m_cbnClr.SetRedraw( FALSE );
   for( i = 0; i < apClr->GetSize(); ++i )
	{
      m_cbnClr.SetItemData( m_cbnClr.InsertString(i,""), (*apClr)[i] );
	  if( (*apClr)[i] == m_clr ) iSlct = i;
	}
   m_cbnClr.SetRedraw( TRUE );

   m_cbnClr.SetCurSel( iSlct );
   m_cbnWeight.m_dwExtra1 = m_cbnPat.m_dwExtra1 = 
	 m_cbnClr.m_dwExtra1 = (*apClr)[iSlct];
   m_iInd_clr = iSlct;
   
      
   HWND hw1 = ::GetDlgItem( m_hWnd, IDC_COMBO_PATTERN );
   PostMessage( WM_NEXTDLGCTL, (WPARAM)hw1, 1 );	           

   return FALSE;
 }

void TPRopPage_Line::OnOK()
 {   
   CPropPageTooltips::OnOK();

   (*(TTItem_Pen&)m_pPC->m_ttEdit).m_iStyle = m_patt;
   (*(TTItem_Pen&)m_pPC->m_ttEdit).m_iWidth = m_w;
   (*(TTItem_Pen&)m_pPC->m_ttEdit).m_clr = m_clr;
 }

void TPRopPage_Line::DoDataExchange( CDataExchange* pDX )
 {
   CPropertyPage::DoDataExchange( pDX );

   DDX_Control( pDX, IDC_COMBO_PATTERN, m_cbnPat );
   DDX_Control( pDX, IDC_COMBO_WEIGHT, m_cbnWeight );
   DDX_Control( pDX, IDC_COMBO_CLR, m_cbnClr );

   DDX_CBIndex( pDX, IDC_COMBO_PATTERN, m_iInd_patt );
   DDX_CBIndex( pDX, IDC_COMBO_WEIGHT, m_iInd_w );
   DDX_CBIndex( pDX, IDC_COMBO_CLR, m_iInd_clr );      

   if( pDX->m_bSaveAndValidate )
	 m_patt = (*apPattern)[ m_iInd_patt ],
	 m_w = (*apThick)[ m_iInd_w ],
	 m_clr = (*apClr)[ m_iInd_clr ];
 }

void TPRopPage_Line::OnSelClr()
 {   
   if( m_cbnClr.GetCurSel() == CB_ERR ) return;

   m_cbnWeight.m_dwExtra1 = m_cbnPat.m_dwExtra1 = 
	 m_cbnClr.m_dwExtra1 = (*apClr)[ m_cbnClr.GetCurSel() ];

   m_cbnWeight.Invalidate();
   m_cbnPat.Invalidate();
 }




TPRopPage_Fill::TPRopPage_Fill( TPropEdit* pPC ): 
 CPropPageTooltips( IDD_PROPPAGE_FILL ),
  m_cbnInd( TCC_FillPattern ),
  m_cbnClr( TCC_Color ),
  m_cbnClrBk( TCC_Color )
  {	    
    m_pPC = pPC;
	m_iIndex = (*(TTItem_Brush&)pPC->m_ttEdit).m_iIndex;
	m_clr = (*(TTItem_Brush&)pPC->m_ttEdit).m_clr;
    m_clrBk = (*(TTItem_Brush&)pPC->m_ttEdit).m_clrBk;	

	if( (*(TTItem_Brush&)pPC->m_ttEdit).m_solid == BS_SOLID )
	  m_iIndex = -1;
	else if( (*(TTItem_Brush&)pPC->m_ttEdit).m_solid == BS_NULL )
	 m_iIndex = -2;
	   
	*this + 
	   new TToolTipData( IDC_COMBO_PATTERN, "Тип закраски" ) +
	   new TToolTipData( IDC_COMBO_FONCLR, "Цвет фона" ) +
	   new TToolTipData( IDC_COMBO_CLR, "Цвет закраски" ) +
	   new TToolTipData( IDC_BTN_CLREXT, "Дополнительные цвета" ) +
	   new TToolTipData( IDC_BTN_CLREXTBK, "Дополнительные цвета" );

	apIndex = Fill_EnumPattern();
	apClr = Common_EnumColor();
	apClrBk = BkGnd_EnumColor();

	m_iInd_ind = apIndex->IndexOf( m_iIndex );
	m_iInd_clr = apClr->IndexOf( m_clr ); 
	m_iInd_clrBk = apClrBk->IndexOf( m_clrBk );
  }
TPRopPage_Fill::~TPRopPage_Fill()
 {
 }

void TPRopPage_Fill::OnClrExt()
 {
   CColorDialog dlgColor( (COLORREF)m_cbnClr.GetItemData(m_cbnClr.GetCurSel()), 0, this );
   if( dlgColor.DoModal() == IDOK )
	{ 
	  int iInd = m_cbnClr.AddString( "" );
	  m_cbnClr.SetItemData( iInd, (DWORD)dlgColor.GetColor() );
	  m_cbnClr.SetCurSel( iInd );	  
	  apClr->IndexOf( dlgColor.GetColor() );
	  OnSelClr();
    }
 }
void TPRopPage_Fill::OnClrExtBk()
 {
   CColorDialog dlgColor( (COLORREF)m_cbnClrBk.GetItemData(m_cbnClrBk.GetCurSel()), 0, this );
   if( dlgColor.DoModal() == IDOK )
	{ 
	  int iInd = m_cbnClrBk.AddString( "" );
	  m_cbnClrBk.SetItemData( iInd, (DWORD)dlgColor.GetColor() );
	  m_cbnClrBk.SetCurSel( iInd );	  
	  apClrBk->IndexOf( dlgColor.GetColor() );
	  OnSelClrBk();
    }
 }

BOOL TPRopPage_Fill::OnInitDialog()
 {
   CPropPageTooltips::OnInitDialog();
    
   int iSlct = 0;
   m_cbnInd.InitStorage( apIndex->GetSize(), 4 );
   m_cbnInd.SetRedraw( FALSE );
   for( int i = 0; i < apIndex->GetSize(); ++i )
	{
      m_cbnInd.SetItemData( m_cbnInd.InsertString(i,""), (*apIndex)[i] );
	  if( (*apIndex)[i] == m_iIndex ) iSlct = i;
	}
   m_cbnInd.SetRedraw( TRUE );

   m_cbnInd.SetCurSel( iSlct );   
   m_iInd_ind = iSlct;
	

   iSlct = 0;
   m_cbnClr.InitStorage( apClr->GetSize(), 4 );
   m_cbnClr.SetRedraw( FALSE );
   for( i = 0; i < apClr->GetSize(); ++i )
	{
      m_cbnClr.SetItemData( m_cbnClr.InsertString(i,""), (*apClr)[i] );
	  if( (*apClr)[i] == m_clr ) iSlct = i;
	}
   m_cbnClr.SetRedraw( TRUE );

   m_cbnClr.SetCurSel( iSlct );   
   m_iInd_clr = iSlct;	
   m_cbnInd.m_dwExtra1 = (*apClr)[iSlct];

   iSlct = 0;
   m_cbnClrBk.InitStorage( apClrBk->GetSize(), 4 );
   m_cbnClrBk.SetRedraw( FALSE );
   for( i = 0; i < apClrBk->GetSize(); ++i )
	{
      m_cbnClrBk.SetItemData( m_cbnClrBk.InsertString(i,""), (*apClrBk)[i] );
	  if( (*apClrBk)[i] == m_clrBk ) iSlct = i;
	}
   m_cbnClrBk.SetRedraw( TRUE );

   m_cbnClrBk.SetCurSel( iSlct );
   m_cbnInd.m_dwExtra2 = (*apClrBk)[iSlct];
   m_iInd_clrBk = iSlct;
   
      
   HWND hw1 = ::GetDlgItem( m_hWnd, IDC_COMBO_PATTERN );
   PostMessage( WM_NEXTDLGCTL, (WPARAM)hw1, 1 );	           

   return FALSE;
 }

void TPRopPage_Fill::OnOK()
 {   
   CPropPageTooltips::OnOK();

   (*(TTItem_Brush&)m_pPC->m_ttEdit).m_iIndex = m_iIndex;
   (*(TTItem_Brush&)m_pPC->m_ttEdit).m_clr = m_clr;
   (*(TTItem_Brush&)m_pPC->m_ttEdit).m_clrBk = m_clrBk;   

	if( m_iIndex == -1 )
     (*(TTItem_Brush&)m_pPC->m_ttEdit).m_solid = BS_SOLID,
	 (*(TTItem_Brush&)m_pPC->m_ttEdit).m_iIndex = HS_BDIAGONAL;
	else if( m_iIndex == -2 )
	  (*(TTItem_Brush&)m_pPC->m_ttEdit).m_solid = BS_NULL,
	  (*(TTItem_Brush&)m_pPC->m_ttEdit).m_iIndex = HS_BDIAGONAL;
	else (*(TTItem_Brush&)m_pPC->m_ttEdit).m_solid = BS_HATCHED;
 }

void TPRopPage_Fill::DoDataExchange( CDataExchange* pDX )
 {
   CPropertyPage::DoDataExchange( pDX );

   DDX_Control( pDX, IDC_COMBO_PATTERN, m_cbnInd );
   DDX_Control( pDX, IDC_COMBO_FONCLR, m_cbnClrBk );
   DDX_Control( pDX, IDC_COMBO_CLR, m_cbnClr );

   DDX_CBIndex( pDX, IDC_COMBO_PATTERN, m_iInd_ind );
   DDX_CBIndex( pDX, IDC_COMBO_FONCLR, m_iInd_clrBk );
   DDX_CBIndex( pDX, IDC_COMBO_CLR, m_iInd_clr );      

   if( pDX->m_bSaveAndValidate )
	 m_iIndex = (*apIndex)[ m_iInd_ind ],
	 m_clr = (*apClr)[ m_iInd_clr ],
	 m_clrBk = (*apClrBk)[ m_iInd_clrBk ];
 }

void TPRopPage_Fill::OnSelClr()
 {   
   if( m_cbnClr.GetCurSel() == CB_ERR ) return;

   m_cbnInd.m_dwExtra1 = (*apClr)[ m_cbnClr.GetCurSel() ];

   m_cbnInd.Invalidate();   
 }

void TPRopPage_Fill::OnSelClrBk()
 {   
   if( m_cbnClrBk.GetCurSel() == CB_ERR ) return;

   m_cbnInd.m_dwExtra2 = (*apClrBk)[ m_cbnClrBk.GetCurSel() ];

   m_cbnInd.Invalidate();   
 }