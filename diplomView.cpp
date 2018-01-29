// diplomView.cpp : implementation of the TDiplomView class
//

#include "stdafx.h"
#include "diplom.h"

#include "diplomDoc.h"
#include "diplomView.h"
#include "childfrm.h"
#include "drawobj.h"
#include "drawtool.hpp"
#include "mainfrm.h"

#include "getgdimgr.hpp"
#include "lock_update.hpp"

#include "HLP\\diplom.hh"

#include <math.h>
#include <afxpriv.h>

#include <algorithm>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// TDiplomView

IMPLEMENT_DYNCREATE(TDiplomView, CScrollView)

BEGIN_MESSAGE_MAP(TDiplomView, CScrollView)
	//{{AFX_MSG_MAP(TDiplomView)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_CONTEXTMENU()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CANCELMODE()
	ON_WM_SETCURSOR()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)

	ON_MESSAGE( WM_DISPLAYCHANGE, OnDisplayChange )	

	ON_MESSAGE( WM_HELPHITTEST, OnCommandHelp )
    ON_MESSAGE( WM_COMMANDHELP, OnCommandHelpF1 )

	ON_REGISTERED_MESSAGE( globl_iMsgTestAcceptDrag, OnMyAcceptDrag )
	ON_REGISTERED_MESSAGE( globl_iMsgReqvestLocalMenu, OnReqMenu )
	
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// TDiplomView construction/destruction

static LOGFONT lfRuls = { -9, 0, 0, 0, FW_NORMAL, 0, 0, 0, RUSSIAN_CHARSET,
		OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
		VARIABLE_PITCH | FF_DONTCARE, "Arial"
	  };

TDiplomView::TDiplomView():
   m_ttPenGridV( *GDIMgr() ),
   m_ttPenGridH( *GDIMgr() ),
   m_ttPenPaperShadow( *GDIMgr() ),
   m_ttPenGridV2( *GDIMgr() ),
   m_ttPenGridH2( *GDIMgr() ),
   m_ttBrushTable( *GDIMgr() ),
   m_ttFntRules( *GDIMgr() ),
   m_ttFntRulesV( *GDIMgr() )
{
	// TODO: add construction code here
  m_dWPaper = m_dHPaper = 100;
  m_iRulerStepH = m_iRulerStepV = 10;
  m_dRulerStepV_pix = m_dRulerStepH_pix = 10;
  m_pBar = NULL;

  m_bActive = false;  
  

  m_iXOffset = m_iYOffset = 25;

  m_pRulAdd = NULL;
  m_bFlPopupCreated_Calc = m_bFlPopupCreated_Br = m_bFlPopupCreated = FALSE;

  m_pLocalClicked = NULL;

  (*m_ttPenGridV).m_iStyle = PS_SOLID;
  (*m_ttPenGridV).m_iWidth = 0;
  (*m_ttPenGridV).m_clr = RGB(192, 192, 192);
  m_ttPenGridV.Create();

  (*m_ttPenGridH).m_iStyle = PS_SOLID;
  (*m_ttPenGridH).m_iWidth = 0;
  (*m_ttPenGridH).m_clr = RGB(192, 192, 192);
  m_ttPenGridH.Create();

  (*m_ttPenPaperShadow).m_iStyle = PS_SOLID;
  (*m_ttPenPaperShadow).m_iWidth = 0;
  (*m_ttPenPaperShadow).m_clr = RGB(192, 192, 192);
  m_ttPenPaperShadow.Create();

  (*m_ttPenGridV2).m_iStyle = PS_DOT;
  (*m_ttPenGridV2).m_iWidth = 0;
  (*m_ttPenGridV2).m_clr = RGB(192, 192, 192);
  m_ttPenGridV2.Create();

  (*m_ttPenGridH2).m_iStyle = PS_DOT;
  (*m_ttPenGridH2).m_iWidth = 0;
  (*m_ttPenGridH2).m_clr = RGB(192, 192, 192);
  m_ttPenGridH2.Create();

  (*m_ttBrushTable).m_clr = RGB(192,255,255);  	  	  
  m_ttBrushTable.Create();

  memcpy( &(*m_ttFntRules).m_lf, &lfRuls, sizeof(LOGFONT) );
  m_ttFntRules.Create();

  memcpy( &(*m_ttFntRulesV).m_lf, &lfRuls, sizeof(LOGFONT) );
  (*m_ttFntRulesV).m_lf.lfOrientation = 
    (*m_ttFntRulesV).m_lf.lfEscapement = 3600 - 900;	   
  m_ttFntRulesV.Create();
}

TDiplomView::~TDiplomView()
{
  ClearBtnsLists();

  /*list<TChildItem*>::iterator it( m_lstCh.begin() );
  list<TChildItem*>::iterator itEnd( m_lstCh.end() );

  for( ; it != itEnd; ++it )
	 delete *it;

  m_lstCh.clear();*/
}

BOOL TDiplomView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// TDiplomView drawing
		

/*void TDiplomView::CreateFonts()
 {
   LOGFONT lf = { -9, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET,
		OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
		VARIABLE_PITCH | FF_DONTCARE, "Arial"
	  };

   m_apFntRules = auto_ptr<CFont>( new CFont() );	  
   m_apFntRules->CreateFontIndirect( &lf );

   m_apFntRulesV = auto_ptr<CFont>( new CFont() );	  
   lf.lfOrientation = lf.lfEscapement = 3600 - 900;	   
   m_apFntRulesV->CreateFontIndirect( &lf );
 }
void TDiplomView::CreateOthers()
 {
   m_apBrushTable = auto_ptr<CBrush>( new CBrush(RGB(192,255,255)) );

   //CBitmap bmp;
   //bmp.LoadBitmap( IDB_HCH );
 
  
   m_apPenGridV = auto_ptr<CPen>( 
	  new CPen(PS_SOLID, 0, RGB(192, 192, 192)) );
   m_apPenGridH = auto_ptr<CPen>( 
	  new CPen(PS_SOLID, 0, RGB(192, 192, 192)) );

   m_apPenGridV2 = auto_ptr<CPen>( 
	  new CPen(PS_DOT, 0, RGB(192, 192, 192)) );
   m_apPenGridH2 = auto_ptr<CPen>( 
	  new CPen(PS_DOT, 0, RGB(192, 192, 192)) );


   m_apPenPaperShadow = 
	 auto_ptr<CPen>( 
	   new CPen(PS_SOLID, 0, RGB(192, 192, 192)) );
 }*/

void TDiplomView::SysClrChange()
 {
   //CreateOthers();
   //if( GetDocument() ) GetDocument()->CreateInstuments();
   Invalidate( FALSE );
 }


LRESULT TDiplomView::OnDisplayChange( WPARAM wp, LPARAM lp )
 {
    LRESULT res = CScrollView::OnDisplayChange( wp, lp );

	SetPageSize( GetDocument()->GetSize(), true );

	return res;
 }

void TDiplomView::OnDraw( CDC* pDC )
{
	TDiplomDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
		 

	CDC dc;
	CDC* pDrawDC = pDC;
	CBitmap bitmap;
	CBitmap* pOldBitmap;	
	bool bSel = false;

    /*if( !pDC->IsPrinting() && pFram->m_optX.m_dLines )
	 {
	   CalcRulRects();
	   pDC->SetBoundsRect( m_rRulH, DCB_ACCUMULATE );
	   pDC->SetBoundsRect( m_rRulV, DCB_ACCUMULATE );
	   pDC->SetBoundsRect( m_rRulInt, DCB_ACCUMULATE );	   
	 }*/

	// only paint the rect that needs repainting
	CRect client;
	pDC->GetClipBox( client );
	CRect rect = client;
	DocToClient( &rect );

	try {
	  if( !pDC->IsPrinting() )
	   {
		  // draw to offscreen bitmap for fast looking repaints
		  if( dc.CreateCompatibleDC(pDC) )
		   {
			  if( bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height()) )
			   {
				  OnPrepareDC( &dc, NULL );
				  pDrawDC = &dc;

				  // offset origin more because bitmap is just piece of the whole drawing
				  dc.OffsetViewportOrg( -rect.left, -rect.top );
				  pOldBitmap = dc.SelectObject( &bitmap );
				  bSel = true;
				  dc.SetBrushOrg( rect.left % 8, rect.top % 8 );
				  				  
				  // might as well clip to the same rectangle
				  dc.IntersectClipRect( client );
			   }
		   }
	   }	

	  if( !pDC->IsPrinting() )
	   {	     			
		 DrawPaper( pDrawDC, client );
	   }

	  pDoc->Draw( pDrawDC, m_pFram->m_optX.m_dScale, client );

	  if( !pDC->IsPrinting() && m_pFram->m_optX.m_dLines )
	   {	     
	     CalcRulRects();
		 DrawRullers( pDrawDC, client );
	   }

	  if( pDrawDC != pDC )
	  {
		  pDC->SetViewportOrg(0, 0);
		  pDC->SetWindowOrg(0,0);
		  pDC->SetMapMode(MM_TEXT);
		  dc.SetViewportOrg(0, 0);
		  dc.SetWindowOrg(0,0);
		  dc.SetMapMode(MM_TEXT);
		  pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(),
			  &dc, 0, 0, SRCCOPY);

		  dc.SelectObject( pOldBitmap );
	  }
	}
	catch( CException *e )
	 {
       if( bSel ) dc.SelectObject( pOldBitmap );
	   e->ReportError();
	   e->Delete();
	 }
	catch( exception e )
	 {	  
       MessageBox( e.what(), "Ошибка", MB_OK|MB_ICONEXCLAMATION );
	 }
}

void TDiplomView::DrawPaper( CDC* pDC, CRect& rRect )
 {
   pDC->FillRect( rRect, m_ttBrushTable() );
   //::FillRect( pDC->m_hDC, &m_rectPaper, (HBRUSH)::GetStockObject(WHITE_BRUSH) );   

   CRect r( m_rectPaper ); r.NormalizeRect();
   CPen *pOld = (CPen*)pDC->SelectStockObject( BLACK_PEN );
   CBrush *pBrOld = (CBrush*)pDC->SelectStockObject( WHITE_BRUSH );
   ::Rectangle( pDC->m_hDC, r.left, r.top, r.right, r.bottom );
   pDC->SelectObject( pBrOld );

   pDC->SelectObject( m_ttPenPaperShadow() );

   pDC->MoveTo( r.right + 3, r.bottom - 6 );
   pDC->LineTo( r.right + 3, r.top - 2 );

   pDC->MoveTo( r.right + 2, r.bottom - 6 );
   pDC->LineTo( r.right + 2, r.top - 2 );

   pDC->MoveTo( r.right + 1, r.bottom - 6 );
   pDC->LineTo( r.right + 1, r.top - 2 );

   pDC->MoveTo( r.right + 0, r.bottom - 6 );
   pDC->LineTo( r.right + 0, r.top - 2 );


   //pDC->MoveTo( r.right + 3, r.top - 4 );
   //pDC->LineTo( r.left + 6, r.top - 4 );

   pDC->MoveTo( r.right + 3, r.top - 3 );
   pDC->LineTo( r.left + 6, r.top - 3 );

   pDC->MoveTo( r.right + 3, r.top - 2 );
   pDC->LineTo( r.left + 6, r.top - 2 );

   pDC->MoveTo( r.right + 3, r.top - 1 );
   pDC->LineTo( r.left + 6, r.top - 1 );

   pDC->MoveTo( r.right + 3, r.top - 0 );
   pDC->LineTo( r.left + 6, r.top - 0 );

   if( m_pFram->m_optX.m_bFlGrid )
	{      
	  CRect r2( rRect ); r2.NormalizeRect();
	  CRect rI1; r.DeflateRect( 2, 2 );
	  if( rI1.IntersectRect(r, r2) )
	   {
         CPen *pFOld = pDC->SelectObject( m_ttPenGridV() );

		 int iA = rI1.left, iB = rI1.right;
		 double dStep = double(m_dRulerStepH_pix) * m_pFram->m_optX.m_dScale;	  
		 int iK = floor( double(iA) / double(dStep) );	  

		 double dStep2 = dStep / 2;
		 int iK2 = floor( double(iA) / double(dStep2) );	  
	  		 		 		 		 
		 int iCX = dStep * double(iK);
		 int iCX2 = dStep2 * double(iK2);
		 if( iCX == iCX2 ) iCX2 = dStep2 * double(--iK2);
		 
		 do
		  {      	                 
		    if( iCX >= r.left && iCX <= r.right )
			 {
			   pDC->SelectObject( m_ttPenGridV() );
			   pDC->MoveTo( iCX, r.top );
			   pDC->LineTo( iCX, r.bottom );
			 }
            if( iCX2 >= r.left && iCX2 <= r.right )
			 {
			   pDC->SelectObject( m_ttPenGridV2() );
			   pDC->MoveTo( iCX2, r.top );
			   pDC->LineTo( iCX2, r.bottom );		  		  
			 }
		  
			++iK, iK2 += 2;
			iCX = dStep * iK;
			iCX2 = dStep2 * iK2;
       
		  } while( iCX <= iB || iCX2 <= iB );		       	   
		 
		 pDC->SelectObject( m_ttPenGridH() );

		 iA = rI1.top, iB = rI1.bottom;
		 dStep = double(m_dRulerStepV_pix) * m_pFram->m_optX.m_dScale;	  
		 iK = floor( double(iA) / double(dStep) );	  

		 dStep2 = dStep / 2;
		 iK2 = floor( double(iA) / double(dStep2) );	  
	  		 		 		 		 
		 iCX = dStep * double(iK);
		 iCX2 = dStep2 * double(iK2);
		 if( iCX == iCX2 ) iCX2 = dStep2 * double(--iK2);
		 	  	  		 
		 do
		  {      
			if( iCX >= r.top && iCX <= r.bottom )
			 {
			   pDC->SelectObject( m_ttPenGridH() );
			   pDC->MoveTo( r.left, iCX );
			   pDC->LineTo( r.right, iCX );
			 }
            if( iCX2 >= r.top && iCX2 <= r.bottom )
			 {
			   pDC->SelectObject( m_ttPenGridH2() );
			   pDC->MoveTo( r.left, iCX2 );
			   pDC->LineTo( r.right, iCX2 );		  		  
			 }

			++iK, iK2 += 2;			
			iCX = dStep * iK;
			iCX2 = dStep2 * iK2;
       
		  } while( iCX <= iB || iCX2 <= iB );		       	   

		 pDC->SelectObject( pFOld );
	   }
	}

   pDC->SelectObject( pOld );
 }
void TDiplomView::CalcRulRects( bool bFlClient )
 {
   CRect r; GetClientRect( r );
   CRect rr[3] = 
	{
      CRect( m_iRulerVWidth, 0, r.right, m_iRulerHHeight ),
	  CRect( 0, m_iRulerHHeight, m_iRulerVWidth, r.bottom ),
	  CRect( 0, 0, m_iRulerVWidth, m_iRulerHHeight )
	};

   if( !bFlClient )  ClientToDoc( rr, 3 );

   m_rRulH = rr[ 0 ];
   m_rRulV = rr[ 1 ];
   m_rRulInt = rr[ 2 ];   
   //m_rRulInt.right = m_rRulH.left; 
   //m_rRulInt.bottom = m_rRulV.top; 
 }

static int Nearst( double d )
 {
   double f = floor( d ), c = ceil( d );
   return fabs(f-d) < fabs(c-d) ? f:c;
 }

void TDiplomView::DrawRullers( CDC* pDC, CRect& rRect )
 {    	    
   CRect rI1, rI2;

   pDC->SetTextColor( RGB(0,0,0) );

   CRect r;
   CRect r2( m_rRulH ); r2.NormalizeRect();
   CRect r3( rRect ); r3.NormalizeRect(); r3.InflateRect( 1, 1 );
   BOOL bFlH = rI1.IntersectRect( r2, r3 );

   r2 = m_rRulV; r2.NormalizeRect();
   BOOL bFlV = rI2.IntersectRect( r2, r3 );   
   
   CPen *pOld = (CPen*)pDC->SelectStockObject( BLACK_PEN );
   CBrush *pBrOld = (CBrush*)pDC->SelectStockObject( LTGRAY_BRUSH );
   CFont *pF = pDC->SelectObject( m_ttFntRules() );

   r = m_rRulInt; 
   r.left -= 2, r.top += 2, r.right += 1, r.bottom -= 1;
   //r.NormalizeRect();
   ::Rectangle( pDC->m_hDC, r.left, r.top, r.right, r.bottom );

   if( bFlH )
	{
	  r = m_rRulH; 
	  r.left, r.top += 2, r.right += 2, r.bottom -= 1;
	  //r.NormalizeRect();
	  ::Rectangle( pDC->m_hDC, r.left, r.top, r.right, r.bottom );
	}

   if( bFlV )
	{
	  r = m_rRulV; 
	  r.left -= 2, r.top, r.right += 1, r.bottom -= 2;
	  //r.NormalizeRect();
	  ::Rectangle( pDC->m_hDC, r.left, r.top, r.right, r.bottom );
	}


   if( bFlH || bFlV )  pDC->ExcludeClipRect( m_rRulInt );

   CPoint ppp[ 2 ] =
	{
      CPoint( m_iRulerVWidth * 0.6, m_iRulerHHeight * 0.6 ),
      CPoint( m_iRulerVWidth * 0.7, m_iRulerHHeight * 0.7 ),	 
	};   
   ClientToDoc( ppp, 2 );   
   
   
   char cBuf[ 100 ];
   if( bFlH )
	{
	  int iA = rI1.left, iB = rI1.right;
	  double dStep = double(m_dRulerStepH_pix) * m_pFram->m_optX.m_dScale;	  
	  int iK = floor( double(iA) / double(dStep) );	  

	  double dStep2 = dStep / 2;
	  int iK2 = floor( double(iA) / double(dStep2) );	  
	  	  
	  pDC->SetBkMode( TRANSPARENT );
	  pDC->SetTextAlign( TA_LEFT|TA_TOP );

      //dStep = floor( dStep );

	  //if( iK * dStep < iA ) ++iK;
	  CRect rDr( 0, m_rRulH.top, dStep, m_rRulH.bottom );
	  //int iCX = dStep * double(iK);	  
	  int iCX = dStep * double(iK);
	  int iCX2 = dStep2 * double(iK2);
	  if( iCX == iCX2 ) iCX2 = dStep2 * double(--iK2);

	  //int iTop2 = m_rRulH.top - abs(m_rRulH.Height()/1.8);	  
	  int iTop0 = ppp[1].y;
	  int iTop2 = ppp[0].y;
	  	  	  
	  do
	   {      	                 
		 pDC->MoveTo( iCX, m_rRulH.top );
		 pDC->LineTo( iCX, m_rRulH.bottom );

		 sprintf( cBuf, "%d", int(iK * m_iRulerStepH) );
		 rDr.left = iCX + 3; rDr.right = rDr.left + dStep;
		 pDC->DrawText( cBuf, strlen(cBuf), rDr, 
		   DT_LEFT|DT_NOCLIP|DT_NOPREFIX|DT_SINGLELINE|DT_TOP );
		 //pDC->TextOut( rDr.left, rDr.top, cBuf, strlen(cBuf) );

		 pDC->MoveTo( iCX2, iTop2 );
		 pDC->LineTo( iCX2, m_rRulH.bottom );		  		  
	   
		 ++iK, iK2 += 2;
		 //iCX = dStep * double(iK);
		 iCX = dStep * iK;
		 iCX2 = dStep2 * iK2;
       
	   } while( iCX <= iB || iCX2 <= iB );		 
      
	}

   if( bFlV )
	{
      pDC->SelectObject( m_ttFntRulesV() );

	  int iA = rI2.top, iB = rI2.bottom;
	  double dStep = double(m_dRulerStepV_pix) * m_pFram->m_optX.m_dScale;	  
	  int iK = floor( double(iA) / double(dStep) );	  

	  double dStep2 = dStep / 2;
	  int iK2 = floor( double(iA) / double(dStep2) );	  
	  	  
	  pDC->SetBkMode( TRANSPARENT );
	  pDC->SetTextAlign( TA_LEFT|TA_TOP );      

	  //if( iK * dStep < iA ) ++iK;
	  CRect rDr( m_rRulV.left, 0, m_rRulH.right, dStep );
	  //int iCX = dStep * double(iK);	  
	  int iCX = dStep * double(iK);
	  int iCX2 = dStep2 * double(iK2);
	  if( iCX == iCX2 ) iCX2 = dStep2 * double(--iK2);

	  //int iTop2 = m_rRulH.top - abs(m_rRulH.Height()/1.8);	  
	  int iTop0 = ppp[1].x;
	  int iTop2 = ppp[0].x;
	  	  	  
	  do
	   {      
		  pDC->MoveTo( m_rRulV.left, iCX );
		  pDC->LineTo( m_rRulV.right, iCX );

		  sprintf( cBuf, "%d", int(iK * m_iRulerStepV) );
		  rDr.top = iCX + 3; rDr.bottom = rDr.top - dStep;
		  pDC->DrawText( cBuf, strlen(cBuf), rDr, 
			DT_LEFT|DT_NOCLIP|DT_NOPREFIX|DT_SINGLELINE|DT_TOP );
		  

          pDC->MoveTo( iTop2, iCX2 );
		  pDC->LineTo( m_rRulV.right, iCX2 );	

		 ++iK, iK2 += 2;
		 //iCX = dStep * double(iK);
		 iCX = dStep * iK;
		 iCX2 = dStep2 * iK2;
       
	   } while( iCX <= iB || iCX2 <= iB );		       
	}

   pDC->SelectObject( pF );
   pDC->SelectObject( pBrOld );
   pDC->SelectObject( pOld );
 }

/////////////////////////////////////////////////////////////////////////////
// TDiplomView printing

BOOL TDiplomView::OnPreparePrinting( CPrintInfo* pInfo )
 {
	// default preparation    
	BOOL bFl = DoPreparePrinting(pInfo);
	if( bFl )
	 {
	   m_dKeyScale = m_pFram->m_optX.m_dScale;
	   m_pFram->m_optX.m_dScale = 1;
	   GetDocument()->NewScaleAll( 1 );
	 }
	return bFl;
 }

void TDiplomView::OnBeginPrinting( CDC* /*pDC*/, CPrintInfo* /*pInfo*/ )
 {
	// TODO: add extra initialization before printing
 }

void TDiplomView::OnEndPrinting( CDC* /*pDC*/, CPrintInfo* /*pInfo*/ )
 {
   m_pFram->m_optX.m_dScale = m_dKeyScale;
   GetDocument()->NewScaleAll( m_dKeyScale );
	// TODO: add cleanup after printing
 }

/////////////////////////////////////////////////////////////////////////////
// TDiplomView diagnostics

#ifdef _DEBUG
void TDiplomView::AssertValid() const
{
	CScrollView::AssertValid();
}

void TDiplomView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

TDiplomDoc* TDiplomView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(TDiplomDoc)));
	return (TDiplomDoc*)m_pDocument;
}
#endif //_DEBUG

static TBtnTool btnHoriz[] =
 {
  { TB_Bmp, ICM_ED_FONT, NULL, IDB_BMP_ED_FONT, false, "Изменить шрифт", BS_PUSHBUTTON },
  { TB_Bmp, ICM_ED_FNTCLR, NULL, IDB_BMP_ED_FNTCLR, true, "Изменить цвет шрифта", BS_PUSHBUTTON },

  { TB_Bmp, ICM_ED_LIN, NULL, IDB_BMP_ED_LIN, false, "Линия", BS_PUSHBUTTON },
  { TB_Bmp, ICM_ED_LINCLR, NULL, IDB_BMP_ED_LINCLR, true, "Цвет линии", BS_PUSHBUTTON },
  { TB_Bmp, ICM_ED_LINTHICK, NULL, IDB_BMP_ED_LINTHICK, true, "Толщина линии", BS_PUSHBUTTON },
  { TB_Bmp, ICM_ED_LINTYP, NULL, IDB_BMP_ED_LINTYP, true, "Тип линии", BS_PUSHBUTTON },

  { TB_Bmp, ICM_ED_FIL, NULL, IDB_BMP_ED_FIL, false, "Заливка", BS_PUSHBUTTON },
  { TB_Bmp, ICM_ED_FILCLR, NULL, IDB_BMP_ED_FILCLR, true, "Цвет заливки", BS_PUSHBUTTON },
  { TB_Bmp, ICM_ED_FILTYP, NULL, IDB_BMP_ED_FILTYP, true, "Тип заливки", BS_PUSHBUTTON },

  { TB_Bmp, ICM_ED_ALIGN, NULL, IDB_BMP_ED_ALIGN, false, "Выравнивание", BS_PUSHBUTTON },

  { TB_Bmp, ICM_ED_CALC, NULL, IDB_BMP_ED_CALC, false, "Расчитать надёжностные характеристики", BS_PUSHBUTTON },
  { TB_EndDescr, 0, NULL, 0, true, "", 0 }
 };

static TBtnTool btnHoriz2[] =
 {
   { TB_Bmp, ICM_ED_SINGLESTEP, NULL, IDB_BMP_ED_STEP, false, "Шаг преобразования", BS_PUSHBUTTON },
   { TB_Bmp, ICM_ED_STEPMODE, NULL, IDB_BMP_ED_STEPMODE, true, "Шаговый режим", BS_AUTOCHECKBOX|BS_PUSHLIKE },

   { TB_Bmp, ICM_ED_STOP, NULL, IDB_BMP_STOP, false, "Прекратить преобразование", BS_PUSHBUTTON },
   { TB_Bmp, ICM_ED_CALC, NULL, IDB_BMP_ED_CALC, true, "Расчитать надёжностные характеристики", BS_PUSHBUTTON },

   { TB_EndDescr, 0, NULL, 0, true, "", 0 }
 };

void TDiplomView::ResetupBar( short sCfg )
 {
   TLockUpdate lockUpd( m_pBar );

   m_pBar->ClearBtns();
   ClearBtnsLists();

   *m_pBar + *new TBtnItem( &m_cbnScale, true );
   *m_pBar + new TToolTipData( ID_CBN_SCALE, "Масштаб" );

   if( sCfg == 0 )
	{
      *m_pBar + new TToolTipData( ICM_ED_STOP, "Останов расчёта" );

	  CRect r; m_cbnScale.GetWindowRect( r );
	  CreateBtns( *m_pBar, btnHoriz, false, m_lstBmp, m_lstBtn );
	  //m_pBar->SetMaxBounds( max(m_pBar->GetMaxBounds(), r.Height()) );
	}
   else
	{
	  CRect r; m_cbnScale.GetWindowRect( r );
	  CreateBtns( *m_pBar, btnHoriz2, false, m_lstBmp, m_lstBtn );
	  //m_pBar->SetMaxBounds( max(m_pBar->GetMaxBounds(), r.Height()) );
	}//llll

   //((TChildFrame*)GetParentFrame())->RecalcLayout();
   m_pBar->MyLayout( true );
 }

void TDiplomView::SetupHorizBar( CMyDialogBar& rB, TBtnTool* btns )
 {           
   m_pBar = &rB;

   ClearBtnsLists(); 
   DWORD dwS = rB.GetBarStyle();
   rB.SetBarStyle( (dwS & ~CBRS_BORDER_BOTTOM)|CBRS_BORDER_LEFT|CBRS_BORDER_RIGHT|CBRS_BORDER_TOP );

   //CreateBtns( rB, btns, false, m_lstBmp, m_lstBtn );

   rB + *new TBtnItem( &m_cbnScale, true );
   rB + new TToolTipData( ID_CBN_SCALE, "Масштаб" );
   rB + new TToolTipData( ICM_ED_STOP, "Останов расчёта" );

   m_cbnScale.SetDlgCtrlID( 999 );

   CRect r; m_cbnScale.GetWindowRect( r );

   CreateBtns( rB, btns, false, m_lstBmp, m_lstBtn );

   rB.SetMaxBounds( max(rB.GetMaxBounds(), r.Height()) );
   
   SetupScaleCBN( ((TChildFrame*)GetParentFrame())->m_optX.m_dScale, 
	 rB );
 }

void CreateBtns( CMyDialogBar& rB, TBtnTool* pDescr, bool bFlTxtAln, 
				 list<CBitmap*>& rLst, list<CWnd*>& rLstBtn )
 {
   TBtnTool *p = pDescr;

   int iMaxBoundX = 0, iMaxBoundY = 0;

   CClientDC dc( (CWnd*)&rB );
   int iTxtMaxW = 0;
   CSize szAdd = dc.GetTextExtent( CString("A") );
   for( ; p->m_typ != TB_EndDescr; p++ )
	 if( p->m_typ == TB_Normal )
	  {
        CSize sz = dc.GetTextExtent( p->m_lpTtl, strlen(p->m_lpTtl) );
		iTxtMaxW = max( iTxtMaxW, sz.cx );
	  }

   iTxtMaxW += 2 * szAdd.cx;
   int iHTxt = 2 * szAdd.cy;

   p = pDescr;
   int iBmpBtnW, iBmpBtnH;
   for( ; p->m_typ != TB_EndDescr; p++ )
	 if( p->m_typ == TB_Bmp )
	  {
        BITMAP bmp;
		CBitmap bm; bm.LoadBitmap( p->m_iIdBmp );
		::GetObject( bm, sizeof(BITMAP), &bmp );
		iBmpBtnW = bmp.bmWidth + 0.2*bmp.bmWidth;
		iBmpBtnH = bmp.bmHeight + 0.2*bmp.bmHeight;
		break;
	  }

   iMaxBoundX = iBmpBtnW;
   iMaxBoundY = iBmpBtnH;

   p = pDescr;
   for( ; p->m_typ != TB_EndDescr; p++ )
	{
      if( p->m_typ == TB_Bmp )
	   {
	     CBitmap *pBmp = new CBitmap();
		 pBmp->LoadBitmap( p->m_iIdBmp );
		 rLst.push_back( pBmp );

		 CButton *pBtn = new CButton();
		 rLstBtn.push_back( pBtn );
		 CRect rr( 0, 0, iBmpBtnW, iBmpBtnH );
		 pBtn->Create( "", 		   
		   WS_CHILD|WS_VISIBLE|BS_BITMAP|BS_CENTER|BS_VCENTER|WS_TABSTOP|p->m_dwExtraStyle,
		   rr, (CWnd*)&rB, p->m_iIDCmd );
		 pBtn->SetBitmap( *pBmp );

		 rB + *new TBtnItem( pBtn, p->m_bShiftMode );
		 rB + new TToolTipData( p->m_iIDCmd, p->m_lpHint );
	   }
	  else
	   {
	     CButton *pBtn = new CButton();
		 rLstBtn.push_back( pBtn );
		 CRect rr( 0, 0, iTxtMaxW, iHTxt );
		 if( bFlTxtAln == false )
		  {
            CSize sz = dc.GetTextExtent( p->m_lpTtl, strlen(p->m_lpTtl) );
			rr.right = sz.cx + 2 * szAdd.cx;
		  }
		 pBtn->Create( p->m_lpTtl, 
		   WS_CHILD|WS_VISIBLE|BS_CENTER|BS_VCENTER|WS_TABSTOP|p->m_dwExtraStyle, 
		   rr, (CWnd*)&rB, p->m_iIDCmd );

		 rB + *new TBtnItem( pBtn, p->m_bShiftMode );
		 rB + new TToolTipData( p->m_iIDCmd, p->m_lpHint );

		 iMaxBoundX = max( iMaxBoundX, rr.Width() );
		 iMaxBoundY = max( iMaxBoundY, rr.Height() );
	   }
	}

   int iM;
   rB.SetMaxBounds( (iM=rB.IsHoris() == true ? iMaxBoundY:iMaxBoundX) );
      

   /*rB.MyLayout( true, rB.IsHoris() == true ?
	  iM + 4*GetSystemMetrics( SM_CYDLGFRAME ): 
	  iM + 4*GetSystemMetrics( SM_CXDLGFRAME )
    );*/
 }


void TDiplomView::ClearBtnsLists()
 {
   list<CWnd*>::iterator it( m_lstBtn.begin() );
   list<CWnd*>::iterator itEnd( m_lstBtn.end() );

   for( ; it != itEnd; ++it )
	  delete *it;

   list<CBitmap*>::iterator it2( m_lstBmp.begin() );
   list<CBitmap*>::iterator itEnd2( m_lstBmp.end() );

   for( ; it2 != itEnd2; ++it2 )
	  delete *it2;

   m_lstBmp.clear();
   m_lstBtn.clear();
 }

void TDiplomView::OnUpdate_Always( CCmdUI*  pCmdUI )
 {
   pCmdUI->Enable( TRUE );   
} 

static LPSTR cCbn[] =
   {
	 "400",
	 "200",
	 "150",
	 "100",
	 "75",
	 "50",
	 "Предидущий",
	 "Ширина",
	 "Страница",
	 NULL
   };

bool TDiplomView::CheckMax( int iSc )
 {
    //return  !( (DWORD)m_rectPaper.Width()*double(iSc)/100.0 > INT_MAX ||
	  // (DWORD)m_rectPaper.Height()*double(iSc)/100.0 > INT_MAX || iSc > 400 );
   return  !(iSc <= 400);
 }

void TDiplomView::UpdateCBNScale()
 {      
   SetupScaleCBN( ((TChildFrame*)GetParentFrame())->m_optX.m_dScale, 
	 *m_pBar );
 }

bool TDiplomView::CheckDigits( LPCSTR lp )
 {
   for( ; *lp; ++lp )
	 if( !isdigit(*lp) && *lp != ' ' ) return false;

   return true;
 }

double TDiplomView::CalcScaleH( int iSize )
 {
   CRect r; GetClientRect( r );   
   ClientToDoc( &r );

   return double(abs(r.Width())) / double(iSize);
 }

double TDiplomView::CalcScaleV( int iSize )
 {
   CRect r; GetClientRect( r );   
   ClientToDoc( &r );

   return double(abs(r.Height())) / double(iSize);
 }

void TDiplomView::OnCBNScale( bool bFlEnter )
 {
   CString str;
   if( bFlEnter )
     m_cbnScale.GetWindowText( str );
   else
	{
	  if( m_cbnScale.GetCurSel()!= CB_ERR )
		m_cbnScale.GetLBText( m_cbnScale.GetCurSel(), str );
	  else str = "100";
	}   

   TChildFrame *pFrm = reinterpret_cast<TChildFrame*>( GetParentFrame() );

   double dSc = pFrm->m_optX.m_dScale;
   CPoint ptCent( GetDocument()->m_ptCenter );

   if( str == "Предидущий" )	
     pFrm->m_optX.m_dScale = pFrm->m_optX.m_dScalePriv,
	 GetDocument()->m_ptCenter = GetDocument()->m_ptCenterPriv;
   else if( str == "Ширина" )		  
	{
	  CSize sz = GetDocument()->GetSize();
	  pFrm->m_optX.m_dScale = 0.8 * CalcScaleH( sz.cx );	  	
      CenteringPage();
	}
   else if( str == "Страница" )	     
	{
      CSize sz = GetDocument()->GetSize();
	  pFrm->m_optX.m_dScale = 
		0.9 * min(
		  CalcScaleH( sz.cx ), 
		   CalcScaleV( sz.cy ) );	 
	  CenteringPage();
	}
   else
	{
      if( !CheckDigits(str) ) 
	   {
         MessageBox( "Ошибочный масштаб", "Ошибка", MB_OK|MB_ICONEXCLAMATION );
		 UpdateCBNScale();
		 return;
	   }
	  
	  int iSc = atoi( (LPCSTR)str );
	  if( iSc < 1 || CheckMax(iSc) ) 
	   {
         MessageBox( "Выход масштаба за допустимые границы", "Ошибка", MB_OK|MB_ICONEXCLAMATION );
		 UpdateCBNScale();
		 return;
	   }
	  
	  pFrm->m_optX.m_dScale = double(iSc) / 100.0;
	}

   if( GetDocument() ) GetDocument()->NewScaleAll( pFrm->m_optX.m_dScale );

   pFrm->m_optX.m_dScalePriv = dSc;
   GetDocument()->m_ptCenterPriv = ptCent;
   UpdateCBNScale();
      
   SetPageSize( GetDocument()->GetSize(), true );   
 }

void TDiplomView::OnInitialUpdate()
{       
   GetDocument()->m_pChFrame = reinterpret_cast<TChildFrame*>(GetParentFrame());

   //CSize size = GetDocument()->GetSize();
   SetPageSize( GetDocument()->GetSize(), false );
   /*CClientDC dc( NULL );
   size.cx = MulDiv( size.cx, dc.GetDeviceCaps(LOGPIXELSX), 100 );
   size.cy = MulDiv( size.cy, dc.GetDeviceCaps(LOGPIXELSY), 100 );
   SetScrollSizes( MM_TEXT, size );
     */             
   
   CClientDC dc( GetDesktopWindow() );
   CFont *pFOld = (CFont*)dc.SelectStockObject( ANSI_VAR_FONT );

   LPSTR *lp = cCbn;
   int iMax = 0;
   CSize sz;
   for( ; *lp; ++lp )
	{
      sz = dc.GetTextExtent( *lp, strlen(*lp) );
	  iMax = max( iMax, sz.cx );
    }

   sz = dc.GetTextExtent( "ABC", 2 );
   dc.SelectObject( pFOld );

   CRect r( 0, 0, iMax + sz.cx, 11*sz.cy );

   //TChildFrame *ppp = (TChildFrame*)GetParentFrame();

   m_cbnScale.Create( WS_VSCROLL|WS_CHILD|WS_VISIBLE|CBS_AUTOHSCROLL|CBS_DROPDOWN|CBS_NOINTEGRALHEIGHT, 
	  r, ((TChildFrame*)GetParentFrame())->GetHorDlgBarPtr(), ID_CBN_SCALE );
	 //DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID );

   lp = cCbn;
   for( ; *lp; ++lp )
	 m_cbnScale.AddString( *lp );

   m_cbnScale.SendMessage( WM_SETFONT, (WPARAM)::GetStockObject(ANSI_VAR_FONT), 0 );

   SetupHorizBar( *((TChildFrame*)GetParentFrame())->GetHorDlgBarPtr(), btnHoriz );
   

   ((TChildFrame*)GetParentFrame())->RecalcLayout();

   /*CRect rp( 100, 150, 250, 50 );
   GetDocument()->Add( new TDrawPC(rp) );*/

   //CScrollView::OnInitialUpdate();

   /*TChildFrame *pChWnd = reinterpret_cast<TChildFrame*>( GetParentFrame() );
   //if( pChWnd && pChWnd->IsZoomed() )
     pChWnd->ShowWindow_Init( TRUE, 0 ) ;*/

   //UpdateWindow();
}

int TDiplomView::CalcRulerStep( double dScale )
 {
   static int iTbl[ 7 ] =
	{
      1, 2, 5, 10, 20, 50, 100
	};

   dScale *= 10;   
   //int iMul = pow( 10, floor(dScale / double(iTbl[6])) );
   int iMul = 1;
   
   int iDiff = INT_MAX, iInd = 0;
   for( int i = 0; i < 7; ++i )
	 if( abs(dScale - iTbl[i]*iMul) < iDiff )
	   iDiff = abs(dScale - iTbl[i]*iMul), iInd = i;	    
      

   return iTbl[ 6 - iInd ] * iMul;
 }

void TDiplomView::SetPageSize( CSize size, bool bUpdate )
 {
   TLockUpdate lock( this );

   CSize sizeKey( size );

   double dSc = reinterpret_cast<TChildFrame*>(GetParentFrame())->m_optX.m_dScale;

   m_dWPaper = ceil( double(size.cx) * dSc );
   m_dHPaper = ceil( double(size.cy) * dSc );

   m_rectPaper = CRect( 0, m_dHPaper, m_dWPaper, 0 );
   //m_rectPaper.NormalizeRect();
   m_rectPaper.InflateRect( 1, 1 );
   
   m_iRulerStepH = CalcRulerStep( dSc );
   m_iRulerStepV = CalcRulerStep( dSc );      

   CClientDC dc( NULL );

   //m_dRulerStepH_pix = double(dc.GetDeviceCaps(LOGPIXELSX) * m_iRulerStepH) / 25.4;   
   m_dRulerStepH_pix = double(m_iRulerStepH) * 100.0 / 25.4;
   m_dRulerStepV_pix = double(m_iRulerStepV) * 100.0 / 25.4;   
   //double dScMul = 100.0 / double(dc.GetDeviceCaps(LOGPIXELSX));
   //m_dRulerStepH_pix /= dScMul;

   m_iRulerHHeight = double(dc.GetDeviceCaps( LOGPIXELSY )) / 2.54 / 2;
   m_iRulerVWidth  = double(dc.GetDeviceCaps( LOGPIXELSX )) / 2.54 / 2;   

   m_iXOffset = 4 * m_iRulerVWidth;
   m_iYOffset = 4 * m_iRulerHHeight;
   
   size.cx = MulDiv( 3.0*m_dWPaper, dc.GetDeviceCaps(LOGPIXELSX), 100 );
   size.cy = MulDiv( 3.0*m_dHPaper, dc.GetDeviceCaps(LOGPIXELSY), 100 );   
   CSize szLineScroll( 
	 dSc * double(m_iRulerStepH) * double(dc.GetDeviceCaps( LOGPIXELSX )) / 25.4 / 2, 
	 dSc * double(m_iRulerStepV) * double(dc.GetDeviceCaps( LOGPIXELSY )) / 25.4 / 2 );
   szLineScroll.cx = max( szLineScroll.cx, 1 );
   szLineScroll.cy = max( szLineScroll.cy, 1 );
   SetScrollSizes( MM_TEXT, size, sizeDefault, szLineScroll );

   CPoint ptC( GetDocument()->m_ptCenter );
   ScrollToPosition( CPoint(0, 0) );
   ptC.x = double(ptC.x) * dSc;// + m_iRulerVWidth; 
   ptC.y = double(ptC.y) * dSc;// + m_iRulerHHeight;
   DocToClient( &ptC );
   CRect r; GetClientRect( r );
   /*if( ptC.x < 0 || ptC.y < 0 || ptC.x > size.cx || ptC.y > size.cy )
	{
      GetDocument()->m_ptCenter = CPoint( sizeKey.cx / 2, sizeKey.cy / 2 );
	  ptC = GetDocument()->m_ptCenter;
	  DocToClient( &ptC );
	}*/
   int iX = ptC.x - r.Width()/2.0 - GetSystemMetrics(SM_CXVSCROLL) + m_iRulerVWidth/2, 
	   iY = ptC.y - r.Height()/2.0 - GetSystemMetrics(SM_CYHSCROLL) + m_iRulerHHeight/2;
   if( iX < 0 ) iX = 0; if( iY < 0 ) iY = 0;
   ScrollToPosition( CPoint(iX, iY) );
   

   if( bUpdate )
     GetDocument()->UpdateMainView( NULL, HINT_UPDATE_WINDOW, NULL );
 }

void TDiplomView::DocToClient( CRect* rect, int iCnt )
 {   
   CClientDC dc( this );
   OnPrepareDC( &dc, NULL );

   for( ; iCnt; --iCnt, ++rect )
	{
      dc.LPtoDP( *rect );

      rect->NormalizeRect();
	}
 }
void TDiplomView::DocToClient( CPoint* point, int iCnt )
 {   
   CClientDC dc( this );
   OnPrepareDC( &dc, NULL );
   
   for( ; iCnt; --iCnt, ++point )		
     dc.LPtoDP( point );        	
 }
void TDiplomView::ClientToDoc( CPoint* point, int iCnt )
 {   
   CClientDC dc(this);
   OnPrepareDC(&dc, NULL);
   
   for( ; iCnt; --iCnt, ++point )	
     dc.DPtoLP( point );      
 }
void TDiplomView::ClientToDoc( CRect* rect, int iCnt )
 {   
   CClientDC dc(this);
   OnPrepareDC(&dc, NULL);
      
   for( ; iCnt; --iCnt, ++rect )
	{
      dc.DPtoLP( *rect );
	  
      ASSERT(rect->left <= rect->right);
      ASSERT(rect->bottom <= rect->top);   	  
	}
 }


void TDiplomView::DocToClient_SC( CRect* rect, int iCnt )
 {
   double dSc = m_pFram->m_optX.m_dScale;

   CClientDC dc( this );
   OnPrepareDC( &dc, NULL );

   for( ; iCnt; --iCnt, ++rect )
	{      
	  rect->left = double(rect->left) * dSc, 
	  rect->right = double(rect->right) * dSc, 
	  rect->top = double(rect->top) * dSc,
	  rect->bottom = double(rect->bottom) * dSc;

	  dc.LPtoDP( *rect );

      rect->NormalizeRect();
	}
 }
void TDiplomView::DocToClient_SC( CPoint* point, int iCnt )
 {
   double dSc = m_pFram->m_optX.m_dScale;

   CClientDC dc( this );
   OnPrepareDC( &dc, NULL );
   
   for( ; iCnt; --iCnt, ++point )	
	{      
      point->x = double(point->x) * dSc, 
	  point->y = double(point->y) * dSc;

	  dc.LPtoDP( point );
	}
 }
void TDiplomView::ClientToDoc_SC( CPoint* point, int iCnt )
 {
   double dSc = m_pFram->m_optX.m_dScale;

   CClientDC dc(this);
   OnPrepareDC(&dc, NULL);
   
   for( ; iCnt; --iCnt, ++point )	
	{
      dc.DPtoLP( point );      

      point->x = double(point->x) / dSc, 
	  point->y = double(point->y) / dSc;
	}
 }
void TDiplomView::ClientToDoc_SC( CRect* rect, int iCnt )
 {
   double dSc = m_pFram->m_optX.m_dScale;

   CClientDC dc(this);
   OnPrepareDC(&dc, NULL);
      
   for( ; iCnt; --iCnt, ++rect )
	{
      dc.DPtoLP( *rect );
	  
      ASSERT(rect->left <= rect->right);
      ASSERT(rect->bottom <= rect->top);   

	  rect->left = double(rect->left) / dSc, 
	  rect->right = double(rect->right) / dSc, 
	  rect->top = double(rect->top) / dSc,
	  rect->bottom = double(rect->bottom) / dSc;
	}
 }

/////////////////////////////////////////////////////////////////////////////
// TDiplomView message handlers

int TDiplomView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
 {
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;	

	m_pFram = reinterpret_cast<TChildFrame*>( GetParentFrame() );
	
	//CreateFonts();
	//CreateOthers();			

	m_bActive = true;

	//UpdateWindow();
	
	return 0;
 }

void TDiplomView::SetupScaleCBN( double dSc, CMyDialogBar& rB )
 {
   CComboBox *pCbn = (CComboBox*)rB.GetDlgItem( ID_CBN_SCALE );
   //cbn.Attach( ::GetDlgItem(m_dbarHor.m_hWnd, ID_CBN_SCALE) );   
   
   CString str; str.Format( "%1.0f", dSc*100.0 );
   

   int iInd = pCbn->FindStringExact( -1, str );
   if( iInd != CB_ERR ) pCbn->SetCurSel( iInd );
   else pCbn->SetCurSel( pCbn->InsertString(-1, str) );   
/*
   int iInd = pCbn->SelectString( -1, str );
   if( iInd == CB_ERR )
	 pCbn->SetCurSel( pCbn->InsertString(-1, str) );   
   else 
	{
	  CString str2;
      pCbn->GetLBText( iInd, str2 );
	  if( str2 != str ) 
	    pCbn->SetCurSel( pCbn->InsertString(-1, str) );   
	}	 */
 }

void TDiplomView::OnPrepareDC_Met( CDC* pDC )
 {
   pDC->SetMapMode( MM_LOENGLISH );
	
	/*int i1 = pDC->GetDeviceCaps(LOGPIXELSX),
	 i2 = pDC->GetDeviceCaps(LOGPIXELSY); //!!!!!!!*/

	//pDC->SetViewportExt( pDC->GetDeviceCaps(LOGPIXELSX),
	//	pDC->GetDeviceCaps(LOGPIXELSY) );
	//pDC->SetWindowExt( 100, -100 );	

	// set the origin of the coordinate system to the center of the page
	CPoint ptOrg;
	//ptOrg.x = GetDocument()->GetSize().cx;
	//ptOrg.y = 2 * GetDocument()->GetSize().cy;
	ptOrg.x = m_dWPaper;
	ptOrg.y = 2 * m_dHPaper;	

	// ptOrg is in logical coordinates
	//pDC->OffsetWindowOrg( -ptOrg.x, ptOrg.y );	
 }

void TDiplomView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CScrollView::OnPrepareDC( pDC, pInfo );

	// mapping mode is MM_ANISOTROPIC
	// these extents setup a mode similar to MM_LOENGLISH
	// MM_LOENGLISH is in .01 physical inches
	// these extents provide .01 logical inches

	pDC->SetMapMode( MM_ANISOTROPIC );
	
	/*int i1 = pDC->GetDeviceCaps(LOGPIXELSX),
	 i2 = pDC->GetDeviceCaps(LOGPIXELSY); //!!!!!!!*/

	pDC->SetViewportExt( pDC->GetDeviceCaps(LOGPIXELSX),
		pDC->GetDeviceCaps(LOGPIXELSY) );
	pDC->SetWindowExt( 100, -100 );	

	// set the origin of the coordinate system to the center of the page
	CPoint ptOrg;
	//ptOrg.x = GetDocument()->GetSize().cx;
	//ptOrg.y = 2 * GetDocument()->GetSize().cy;
	ptOrg.x = m_dWPaper;
	ptOrg.y = 2 * m_dHPaper;	

	// ptOrg is in logical coordinates
	if( pDC->IsPrinting() )
	 {
	   int iX = MulDiv( pDC->GetDeviceCaps(HORZSIZE), 1000, 254 );
	   int iY = MulDiv( pDC->GetDeviceCaps(VERTSIZE), 1000, 254 );
	   //iX = MulDiv( iX, pDC->GetDeviceCaps(LOGPIXELSX), 100 );
	   //iY = MulDiv( iY, pDC->GetDeviceCaps(LOGPIXELSY), 100 );
	   CSize sz = GetDocument()->GetSize();
	   iX = (sz.cx - iX) / 2;
	   iY = (sz.cy - iY) / 2;

	   pDC->OffsetWindowOrg( iX, GetDocument()->m_size.cy - iY );		 
	 }
	else
	  pDC->OffsetWindowOrg( -ptOrg.x, ptOrg.y );		
    

	/*CPen pp( PS_SOLID, 3, RGB(255,0,0) );
	CPen pp2( PS_SOLID, 3, RGB(0,255,0) );
	CPen *pO = (CPen*)pDC->SelectObject( &pp );
	pDC->MoveTo( 0, 0 );
	pDC->LineTo( pDC->GetDeviceCaps(LOGPIXELSX), 0 );
	pDC->MoveTo( 100, 100 );
	pDC->SelectObject( &pp2 );
	pDC->LineTo( 110, 110 );
	pDC->SelectObject( pO ); //!!!!!!!*/
 }

void TDiplomView::InvalObj( TDrawShape* pObj, bool bFlWithSel )
 {
   //CRect rect = pObj->m_position;
   CRect rect( pObj->GetPosition(bFlWithSel || IsShpSelected(pObj) || m_pFram->m_optX.m_bFlGluePoints) );
   DocToClient_SC( &rect );
   if( bFlWithSel || IsShpSelected(pObj) || m_pFram->m_optX.m_bFlGluePoints ) 
	 rect.InflateRect( 10, 10 );
    /*{
	  rect.left -= 4;
	  rect.top -= 5;
	  rect.right += 5;
	  rect.bottom += 4;
    }
   rect.InflateRect( 1, 1 ); // handles CDrawOleObj objects
*/
   InvalidateRect( rect, FALSE );   
 }


void TDiplomView::OnUpdate( CView* pSender, LPARAM lHint, CObject* pHint ) 
 {
	// TODO: Add your specialized code here and/or call the base class
   switch( lHint )
	{
	  case HINT_UPDATE_WINDOW:    // redraw entire window
		  Invalidate( FALSE );
		  break;

	  case HINT_UPDATE_DRAWOBJ:   // a single object has changed
		  InvalObj( reinterpret_cast<TDrawShape*>(pHint) );
		  break;

	  case HINT_UPDATE_SELECTION: // an entire selection has changed
		  {
			  /*CDrawObjList* pList = pHint != NULL ?
				  (CDrawObjList*)pHint : &m_selection;
			  POSITION pos = pList->GetHeadPosition();
			  while (pos != NULL)
				  InvalObj(pList->GetNext(pos));*/
		    TShpObjTool_Dta dta( this );
		    for_each( GetDocument()->m_lstDrw.begin(),
			  GetDocument()->m_lstDrw.end(), dta );
		  }
		 break;

	  case HINT_DELETE_SELECTION: // an entire selection has been removed
		  /*if (pHint != &m_selection)
		  {
			  CDrawObjList* pList = (CDrawObjList*)pHint;
			  POSITION pos = pList->GetHeadPosition();
			  while (pos != NULL)
			  {
				  CDrawObj* pObj = pList->GetNext(pos);
				  InvalObj(pObj);
				  Remove(pObj);   // remove it from this view's selection
			  }
		  }*/
		  break;	

	  default:
		  ASSERT(FALSE);
		  break;
	}	
 }

void TDiplomView::OnLButtonDown( UINT nFlags, CPoint point ) 
 {
	// TODO: Add your message handler code here and/or call default	
	if( !IsActiveWnd() || !m_pFram->CriticalRun() ) return;

	TDrawRuler *pCh = NULL;
    CalcRulRects( true );
	//CRect rHH( m_rRulH ); rHH.NormalizeRect();
	if( !m_pFram->m_bFlRun )
	 {
	   if( m_rRulH.PtInRect(point) )
		{
		  CPoint pt( point ); ClientToDoc_SC( &pt );

		  pCh = new TDrawRuler();
		  pCh->SetVert( false, pt );
		}
	   //rHH = m_rRulV; rHH.NormalizeRect();
	   else
	   if( m_rRulV.PtInRect(point) )
		{
		  CPoint pt( point ); ClientToDoc_SC( &pt );

		  pCh = new TDrawRuler();
		  pCh->SetVert( true, pt );
		}   
	 }

	if( pCh )
	 {		 	   
	   GetDocument()->Add( pCh );		    

	   pCh->SetInve();
	   InvalObj( pCh );

	   m_pRulAdd = pCh;
	   SetCapture();

	   return;
	 }


	if( !(m_pFram->m_bFlRun && (nFlags&MK_CONTROL)) )
	 {
	   CDrawTool* pTool = CDrawTool::FindTool( CDrawTool::c_drawShape );
	   if( pTool != NULL )
		 pTool->OnLButtonDown( this, nFlags, point );
	 }
 }

void TDiplomView::OnLButtonUp( UINT nFlags, CPoint point ) 
 {
	// TODO: Add your message handler code here and/or call default	
	if( !IsActiveWnd() || !m_pFram->CriticalRun() ) return;

	if( m_pRulAdd )
	 {
	   CalcRulRects( true );	   
	   if( (m_rRulH.PtInRect(point) && !m_pRulAdd->GetVert()) ||		
	       (m_rRulV.PtInRect(point) && m_pRulAdd->GetVert())
		 )
	     RemoveObj( m_pRulAdd );
	   else GetDocument()->SetModifiedFlag();
	
	   m_pRulAdd = NULL;
	   ReleaseCapture();
	   return;
	 }

	CDrawTool* pTool = CDrawTool::FindTool( CDrawTool::c_drawShape );
	if( pTool != NULL )
	  pTool->OnLButtonUp( this, nFlags, point );
 }


void TDiplomView::OnMouseMove( UINT nFlags, CPoint point ) 
 {
	// TODO: Add your message handler code here and/or call default    

    if( !IsActiveWnd() || !m_pFram->CriticalRun() ) return;

	CScrollView::OnMouseMove( nFlags, point ); 

    if( m_pRulAdd )
	 {
       CPoint pt( point ); ClientToDoc_SC( &pt );	   
	   InvalObj( m_pRulAdd );
	   m_pRulAdd->SetVert( m_pRulAdd->GetVert(), pt );
	   InvalObj( m_pRulAdd );
	   return;
	 }

	if( GetCapture() != this )
	 {
	   CalcRulRects( true );
	   //CRect rHH( m_rRulH ); rHH.NormalizeRect();
	   if( m_rRulH.PtInRect(point) )
		{
		  if( !m_pFram->m_bFlRun )
		   {
			 HCURSOR hc = AfxGetApp()->LoadStandardCursor( IDC_SIZENS );
			 if( ::GetCursor() != hc ) ::SetCursor( hc );
		   }
		  return;
		}
	   //rHH = m_rRulV; rHH.NormalizeRect();
	   if( m_rRulV.PtInRect(point) )
		{
		  if( !m_pFram->m_bFlRun )
		   {
			 HCURSOR hc = AfxGetApp()->LoadStandardCursor( IDC_SIZEWE );
			 if( ::GetCursor() != hc ) ::SetCursor( hc );
		   }
		  return;
		}   
	 }

	if( !m_pFram->m_bFlRun || (GetDocument() && GetDocument()->m_iCntSel < 1) )
	 {
	   CDrawTool* pTool = CDrawTool::FindTool( CDrawTool::c_drawShape );
	   if( pTool != NULL )
		{
		  bool bFlScrolled = false;

		  if( pTool->IsDragging() )
		   {
			 CRect r; GetClientRectS( r ); 
			 CPoint pt = GetScrollPosition();
			 CPoint pt1 = pt;
			 
			 if( point.x <= r.left ) pt.Offset( -m_iXOffset, 0 );
			 else if( point.x >= r.right ) pt.Offset( m_iXOffset, 0 );
			 if( point.y <= r.top ) pt.Offset( 0, -m_iYOffset );
			 else if( point.y >= r.bottom ) pt.Offset( 0, m_iYOffset );

			 CSize sz( GetTotalSize() );
			 if( pt.x < 0 ) pt.x = 0;
			 else if( pt.x > sz.cx ) pt.x = sz.cx;
			 if( pt.y < 0 ) pt.y = 0;
			 else if( pt.y > sz.cy ) pt.y = sz.cy;

			 if( pt != pt1 ) 
			  {				     		     
				bFlScrolled = true;
				CPoint ptOff = pt1 - pt;
				pTool->PreScroll( this, ptOff );			 

				point.Offset( -pt.x + pt1.x, -pt.y + pt1.y );
				CPoint point2( point ); ClientToScreen( &point2 );
				//CPoint ppt;
				//GetCursorPos( &ppt );
				SetCursorPos( point2.x, point2.y );

				InvalidateRulers();
				ScrollToPosition( pt );
				UpdateWindow();
				CalcCenter();
			  }
         
			GetCursorPos( &pt ); ScreenToClient( &pt ); pt1 = pt;
			if( pt.x < 0 ) pt.x = 0;
			else if( pt.x > r.right ) pt.x = r.right;
			if( pt.y < 0 ) pt.y = 0;
			else if( pt.y > r.bottom ) pt.y = r.bottom;

			if( pt1 != pt ) 
			 {
			   ClientToScreen( &pt );
			   SetCursorPos( pt.x, pt.y );
			 }
		   }

		  pTool->OnMouseMove( this, nFlags, point );
		  if( bFlScrolled ) pTool->PostScroll( this );
		}
	 }
 }

BOOL TDiplomView::OnEraseBkgnd( CDC* pDC ) 
{
	// TODO: Add your message handler code here and/or call default
	
	return TRUE;
}

void TDiplomView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	
}

void TDiplomView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
		
	InvalidateRulers();

	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);	

	CalcCenter();
}

void TDiplomView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
		
	InvalidateRulers();

	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);	

	CalcCenter();	
}

void TDiplomView::InvalidateRulers()
 {
   CalcRulRects( true );	    

   CRect r( m_rRulH ); r.InflateRect( 2, 2 );
   InvalidateRect( r, FALSE );     
   r = m_rRulInt; r.InflateRect( 2, 2 );
   InvalidateRect( r, FALSE );   
   r = m_rRulV; r.InflateRect( 2, 2 );
   InvalidateRect( r, FALSE );   	
 }

void TDiplomView::CalcCenter()
 {
   CRect r; GetClientRect( r );
   CPoint pt( r.Width() / 2, r.Height() / 2 );
   ClientToDoc_SC( &pt );
   GetDocument()->m_ptCenter = pt;
 }

void TDiplomView::CenteringPage()
 {
   TChildFrame *pFrm = reinterpret_cast<TChildFrame*>( GetParentFrame() );

   GetDocument()->m_ptCenter = CPoint(
	 double(GetDocument()->GetSize().cx) / 2,
	 double(GetDocument()->GetSize().cy) / 2 );
 }

void TDiplomView::InvalidateRules()
 {
   InvalidateRect( m_rRulH, FALSE ); 
   InvalidateRect( m_rRulV, FALSE );
   InvalidateRect( m_rRulInt, FALSE );   
 }

void TDiplomView::OnSize(UINT nType, int cx, int cy) 
 {
	CScrollView::OnSize(nType, cx, cy);

	if( GetSafeHwnd() && IsWindowVisible() && !IsIconic() ) CalcCenter();	
 }

void TDiplomView::OnFilePrintPreview()
 {   
   if( ((TDiplomApp*)AfxGetApp())->GetDevMode_() == NULL )
	{
      if( m_pFram )
	    m_pFram->MessageBox( "Требуется установленный принтер", "Ошибка", MB_OK|MB_ICONEXCLAMATION );
	  else AfxMessageBox( "Требуется установленный принтер", MB_OK|MB_ICONEXCLAMATION );
	  return;
	}

   if( m_pBar ) m_pBar->ShowWindow( SW_HIDE );   
   /*m_dKeyScale = m_pFram->m_optX.m_dScale;
   m_pFram->m_optX.m_dScale = 1;
   GetDocument()->NewScaleAll( 1 );*/

   CScrollView::OnFilePrintPreview();   
 }


void TDiplomView::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView) 
 {
	// TODO: Add your specialized code here and/or call the base class

    //GetDocument()->NewScaleAll( m_pFram->m_optX.m_dScale );
	m_pFram->m_optX.m_dScale = m_dKeyScale;
    GetDocument()->NewScaleAll( m_dKeyScale );
	
	CScrollView::OnEndPrintPreview(pDC, pInfo, point, pView);

	if( m_pBar ) 
	 {
	   m_pBar->ShowWindow( SW_SHOW );
       GetParentFrame()->RecalcLayout();
	 }
 }

void TDiplomView::OnRButtonUp( UINT nFlags, CPoint point ) 
 {
	// TODO: Add your message handler code here and/or call default	
   CScrollView::OnRButtonUp( nFlags, point );

   if( nFlags & MK_CONTROL ) return;

   CPoint pt( point );
   ClientToDoc_SC( &pt );
   //int nDragHandle, nGlue;
   //TDrawShape *pObj = HitTest( pt, nDragHandle, nGlue );	 
   TDrawShape *pObj = GetDocument()->FindInterect( pt );

   if( pObj ) 
	{
	  m_ptPopup = point,
	  SelectShp( pObj, false );
	  UpdateWindow();
	  PostMessage( globl_iMsgReqvestLocalMenu, 0, (LPARAM)pObj );
	}
	/*{
	  CDrawTool* pTool = CDrawTool::FindTool( CDrawTool::c_drawShape );
	  if( pTool != NULL )
	   {
	     pTool->OnLButtonDown( this, nFlags, point );
		 pTool->OnLButtonUp( this, nFlags, point );
		 m_ptPopup = point;
		 UpdateWindow();
		 PostMessage( globl_iMsgReqvestLocalMenu, 0, (LPARAM)pObj );
	   }
	}*/
 }

struct TMenuEntry
 {
   UINT  m_Attr;
   UINT  m_ID;
   char* m_Txt;
 };

static TMenuEntry mnuMain[] =
 {
  { MF_STRING | MF_ENABLED, ID_EDIT_COPY, "Копировать/&C" },
  { MF_STRING | MF_ENABLED, ID_EDIT_CUT, "Вырезать/&t" },
  { MF_STRING | MF_ENABLED, ID_EDIT_CLEAR_ALL, "Удалить/&R" },  
  { MF_STRING | MF_ENABLED, ID_EDIT_DUP, "Дублировать/&D" },  
  { MF_SEPARATOR, 0, "" },
  { MF_STRING | MF_ENABLED, ID_SC_ALIGMENT, "Выравнять/&A" },
  { MF_STRING | MF_ENABLED, ID_SC_FORWARD, "Поместить вперёд/&F" },
  { MF_STRING | MF_ENABLED, ID_SC_BACK, "Поместить назад/&B" },
  { MF_SEPARATOR, 0, "" },
  { MF_STRING | MF_ENABLED, ID_CTX_PROPERTY, "Свойства/&O" },
  { MF_SEPARATOR, 0, "" },
  { MF_STRING | MF_ENABLED, ID_CTX_REPORT, "Отчёт" },  
  { 0, 0, NULL }
 };

static TMenuEntry mnuBrigEdit[] =
 {
   { MF_STRING | MF_ENABLED, ID_BR_SERVIS, "Обслуживаемые/&S" },
   { MF_STRING | MF_ENABLED, ID_BR_UNSERVIS, "Не обслуживаемые/&N" },
   { MF_STRING | MF_ENABLED, ID_BR_LABEL, "Меченые/&L" },
   { 0, 0, NULL }
 };

static TMenuEntry mnuCalc[] =
 {
   { MF_STRING | MF_ENABLED, ID_CLC_EQREST, "Эквив. интенсивность" },   
   { MF_STRING | MF_ENABLED, ID_CLC_CONNECT, "Проверить связанность" },
   { MF_STRING | MF_ENABLED, ID_CLC_CTYPE, "Показать тип связи" },
   { MF_STRING | MF_ENABLED, ID_CLC_CINT, "Показать пересечение" },
   { MF_STRING | MF_ENABLED, ID_CLC_NPATHS, "Показать число путей" },
   { MF_STRING | MF_ENABLED, ID_CLC_TSIMPL, "Тривиальное сведение" },
   { MF_STRING | MF_ENABLED, ID_CLC_TSOFIST, "Шаг сложного сведения" },
   { MF_SEPARATOR, 0, "" },
   { MF_STRING | MF_ENABLED, ID_CLC_RUN, "Расчитать схему" },
   //{ MF_STRING | MF_ENABLED, ID_CLC_ARNG, "Анализ рангов" },
   { 0, 0, NULL }
 };

static void FillMenu( CMenu& PopMenu, TMenuEntry* pE )
 {   
   for( ; pE->m_Txt;  pE++ )
     PopMenu.AppendMenu( pE->m_Attr, pE->m_ID, pE->m_Txt );
 }

void TDiplomView::ShowContextMenu( CPoint point, short shMnu )
 {	
	CPoint	ClientPoint( point );	

    CMenu *pMen = NULL;

	try
	 {	   
	   if( !shMnu )
		{
		  if( m_pFram->m_optX.m_bFlModeBrig )
		   {
			 pMen = &m_menBrig;
			 if( !m_bFlPopupCreated_Br )
			  {
				m_bFlPopupCreated_Br = m_menBrig.CreatePopupMenu();
				if( !m_bFlPopupCreated_Br )
				  AfxThrowUserException();	   
				FillMenu( m_menBrig, mnuBrigEdit );
			  }

			 UINT uiFl = 0;
			 TDrawShapeList::iterator itSta( GetDocument()->RefLstDraw().begin() );
			 TDrawShapeList::iterator itEnd( GetDocument()->RefLstDraw().end() );

			 for( ; itSta != itEnd; ++itSta )
			   if( (*itSta)->IsSelected() && (*itSta)->isA() == TT_PC )
				{
				  uiFl = 1;
				  break;
				}

			 m_pFram->EnableBrigEdit( uiFl );
		   }
		  else
		   {
			 pMen = &PopMenu;
			 if( !m_bFlPopupCreated )
			  {
				m_bFlPopupCreated = PopMenu.CreatePopupMenu();
				if( !m_bFlPopupCreated )
				  AfxThrowUserException();	   			  
				FillMenu( PopMenu, mnuMain );
			  }		  		   
		   }		
		}
	   else
		{
          switch( shMnu )
		   {
		     case 1:
			  {
			    pMen = &m_menCalc;
				if( !m_bFlPopupCreated_Calc )
				 {
				   m_bFlPopupCreated_Calc = m_menCalc.CreatePopupMenu();
				   if( !m_bFlPopupCreated_Calc )
					 AfxThrowUserException();	   			  
				   FillMenu( m_menCalc, mnuCalc );
				 }		  		   
			  };
			  break;
			 default:
			  break;
		   };
		}

	   //PopMenu.EnableMenuItem( ID_CMD_LINE, MF_DISABLED | MF_GRAYED );		
		     
   	   if( !pMen ) return;

	   ClientToScreen( &ClientPoint );		

	   if( !pMen->TrackPopupMenu(TPM_LEFTALIGN,
		   ClientPoint.x, ClientPoint.y, m_pFram) 
	 	 )
		   AfxThrowUserException();
	 }
	catch( CUserException* pEx )
	 {
		AfxMessageBox( "Cann't create popup menu" );	
		pEx->Delete();
	 } 

	//if( bExists )
	  //PopMenu.DestroyMenu();
 }


TDrawShape* TDiplomView::HitTest( const CPoint& local, int& nDragHandle, int& nGlue, TDrawShape* pExclude )
 {
   TDrawShape *pCh = NULL;
   TShpHit_Dta dta( local, nDragHandle, nGlue, pCh, this, pExclude, 2 );
   find_if( GetDocument()->m_lstDrw.rbegin(), GetDocument()->m_lstDrw.rend(), dta );

   if( !pCh || (pCh && pCh->IsSelected()) ) return pCh;

   TDrawShape *pCh2 = NULL;
   int nDragHandle2, nGlue2;
   TShpHit_Dta dta2( local, nDragHandle2, nGlue2, pCh2, this, pExclude, 1 );
   find_if( GetDocument()->m_lstDrw.rbegin(), GetDocument()->m_lstDrw.rend(), dta2 );
   
   if( pCh2 && nDragHandle2 )
	{
      nDragHandle = nDragHandle2; nGlue = nGlue2;
	  return pCh2;
	}

   return pCh;
 }
bool TDiplomView::IsShpSelected( TDrawShape* pShp )
 {
   return pShp->IsSelected();
 }
void TDiplomView::SelectShp( TDrawShape* pShp, bool bAdd )
 {
   if( !bAdd && pShp && IsShpSelected(pShp) ) return;

   if( !bAdd ) 
	{      
      OnUpdate( NULL, HINT_UPDATE_SELECTION, NULL );
	  GetDocument()->SelectAll( false );
	}
	  
   if( pShp == NULL ) return;

   pShp->Select( GetDocument()->RefLstDraw(), !bAdd ? true:!pShp->IsSelected() );
   if( bAdd && !pShp->IsSelected() ) 
	{
      TDrawShapeList::iterator itSta( GetDocument()->RefLstDraw().begin() );
      TDrawShapeList::iterator itEnd( GetDocument()->RefLstDraw().end() );

	  int iMinNum = INT_MAX;
	  TDrawShape *pDrCh = NULL;
      for( ; itSta != itEnd; ++itSta )
	    if( (*itSta)->IsSelected() && (*itSta)->m_iSelCount < iMinNum )
          iMinNum = (*itSta)->m_iSelCount,
		  pDrCh = (*itSta).get();

	   if( pDrCh ) 
		{
		  pDrCh->m_iSelCount = 1;
		  InvalObj( pDrCh, true );
		}
	}
   
   InvalObj( pShp, true );
 }

void TDiplomView::CloneShpSelection()
 {
   list<TDrawShape*> lstKey;
typedef list<TDrawShape*>::iterator TLstIT;

   TDrawShapeList::iterator itSta( GetDocument()->m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( GetDocument()->m_lstDrw.end() );

   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->IsSelected() ) lstKey.push_back( (*itSta).get() );

   TLstIT itSta2( lstKey.begin() );
   TLstIT itEnd2( lstKey.end() );

   for( ; itSta2 != itEnd2; ++itSta2 )
	  (*itSta2)->Clone( GetDocument() );
 }

TDrawShape* TDiplomView::ObjectAt( const CPoint& point )
 {   
   CRect rect( point, CSize(1, 1) );

   TDrawShapeList::reverse_iterator itSta( GetDocument()->m_lstDrw.rbegin() );
   TDrawShapeList::reverse_iterator itEnd( GetDocument()->m_lstDrw.rend() );

   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->Intersects(rect) ) return (*itSta).get();
   
	return NULL;
 }
void TDiplomView::Deselect( TDrawShape* pSh )
 {
   pSh->Select( GetDocument()->RefLstDraw(), false );
   InvalObj( pSh );
 }
void TDiplomView::EditText( TDrawShape* pSh )
 {
 }
void TDiplomView::SelectWithinRect( const CRect& rect_, bool bAdd )
 {
   if( !bAdd ) SelectShp( NULL );

   CRect rect( rect_ );
   ClientToDoc_SC( &rect );
   /*CDrawObjList* pObList = GetDocument()->GetObjects();
   POSITION posObj = pObList->GetHeadPosition();
   while (posObj != NULL)
    {
	   CDrawObj* pObj = pObList->GetNext(posObj);
	   if (pObj->Intersects(rect))
		   Select(pObj, TRUE);
    }*/
   TShpSelectRectTool_Dta dta = TShpSelectRectTool_Dta( this, rect, GetDocument()->RefLstDraw() );
   for_each( GetDocument()->m_lstDrw.begin(),
	 GetDocument()->m_lstDrw.end(), dta );
 }


int TDiplomView::GetCountSelected()
 {
   int iCnt = 0;
   TShpCountSelect_Dta dta( iCnt );
   for_each( GetDocument()->m_lstDrw.begin(),
	 GetDocument()->m_lstDrw.end(), dta );

   return iCnt;
 }
void TDiplomView::CastOnSelection_MoveTo( const CPoint& delta )
 {
   TDrawShapeList::iterator itSta( GetDocument()->m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( GetDocument()->m_lstDrw.end() );

   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->IsSelected() ) 
	  {
	    CRect position = (*itSta)->m_position;
	    position += delta;
		(*itSta)->SetInve();
	    (*itSta)->MoveTo( position, this );
	  }
 }

void TDiplomView::CastSelectionInve( bool bFl, bool bUpdate )
 {
   TDrawShapeList::iterator itSta( GetDocument()->m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( GetDocument()->m_lstDrw.end() );

   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->IsSelected() ) 
	  {
	    (*itSta)->SetInve( bFl );	  
		if( bUpdate ) InvalObj( (*itSta).get(), true );
	  }
 }

void TDiplomView::MoveHandleTo( int nDragHandle, const CPoint& local, TDrawShape* pCh, bool bFlEQ )
 {
   pCh->SetInve();
   pCh->MoveHandleTo( nDragHandle, local, this, bFlEQ );
	//int nHandle, CPoint point, TDiplomView* pView = NULL );
 }

/*TDrawShape* TDiplomView::GetHeadSelected()
 {
   TDrawShapeList::iterator itSta( GetDocument()->m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( GetDocument()->m_lstDrw.end() );

   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->IsSelected() ) return (*itSta).get();

   return NULL;
 }*/

void TDiplomView::OnLButtonDblClk( UINT nFlags, CPoint point ) 
 {
   // TODO: Add your message handler code here and/or call default	
   if( !IsActiveWnd() ) return;
   CDrawTool* pTool = CDrawTool::FindTool( CDrawTool::c_drawShape );
   if( pTool != NULL )
	 pTool->OnLButtonDblClk( this, nFlags, point );
 }

void TDiplomView::OnCancelMode() 
 {
   CScrollView::OnCancelMode();
	
   if( !IsActiveWnd() ) return;

   if( m_pRulAdd )
	{
	  RemoveObj( m_pRulAdd );
	  m_pRulAdd = NULL;
      return;
	}

   CDrawTool* pTool = CDrawTool::FindTool( CDrawTool::c_drawShape );
   if( pTool != NULL ) pTool->OnCancel( this );
 }

bool TDiplomView::IsActiveWnd()
 {
   return 
	 reinterpret_cast<CMDIFrameWnd*>(AfxGetApp()->m_pMainWnd)->MDIGetActive() == m_pFram && 
	 reinterpret_cast<TMainFrame*>(AfxGetApp()->m_pMainWnd)->IsActive();
 }

void TDiplomView::OnActivateFrame( UINT nState, CFrameWnd* pFrameWnd )
 {
   CScrollView::OnActivateFrame( nState, pFrameWnd );
    
   m_bActive = (nState == WA_INACTIVE) ? false:true;

   /*if( m_bActive != bActivate )
	{
	  if (bActivate)  // if becoming active update as if active
		  m_bActive = bActivate;
	  
	  m_bActive = bActivate;
	}*/
 }

BOOL TDiplomView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
 {
	// TODO: Add your message handler code here and/or call default   

   if( nHitTest == HTCLIENT && IsActiveWnd() ) 
	{
	  return TRUE;
	  /*CDrawTool* pTool = CDrawTool::FindTool( CDrawTool::c_drawShape );
	  if( pTool != NULL )
		 pTool->OnLButtonDown( this, nFlags, point );*/
	}
	
	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
 }

void TDiplomView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
 {
	// TODO: Add your message handler code here and/or call default
    if( nChar == VK_CONTROL && IsActiveWnd() )
	 {
       CDrawTool* pTool = CDrawTool::FindTool( CDrawTool::c_drawShape );
	   if( pTool != NULL ) pTool->OnKeyDown( nChar, this );
	 }
	
	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
 }

void TDiplomView::OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags ) 
{
	// TODO: Add your message handler code here and/or call default

    if( nChar == VK_CONTROL && IsActiveWnd() )
	 {
       CDrawTool* pTool = CDrawTool::FindTool( CDrawTool::c_drawShape );
	   if( pTool != NULL ) pTool->OnKeyUp( nChar, this );
	 }	
	else if( nChar == VK_DELETE )
	 {
	   if( !m_pFram->m_bFlRun && GetDocument()->m_bHaveSel )
         GetDocument()->RemoveSelected( true );
	 }

	CScrollView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void TDiplomView::RemoveObj( TDrawShape* pObj )
 {
   InvalObj( pObj );
   GetDocument()->RemoveObj( pObj );   
 }

LONG TDiplomView::OnReqMenu( UINT wp, LONG lp )
 {
   try { 
	  m_pLocalClicked = (TDrawShape*)lp;
	  ShowContextMenu( m_ptPopup );	 
	}
   catch( CException *pE )
	{
	  m_pLocalClicked = NULL;
	  throw;
	}
   catch( exception e )
	{ 
	  m_pLocalClicked = NULL;
	  throw;
	}
   m_pLocalClicked = NULL;

   return 0;
 }

LONG TDiplomView::OnMyAcceptDrag( UINT wp, LONG lp )
 {
   if( m_pFram->m_bFlRun ) return 0;

   if( wp == MSGA_LEAVE )
	{
      TDrawShapeList& rLst = GetDocument()->RefLstDraw();
	  RemoveObj( (*--rLst.end()).get() );

	  TDrawShape::m_iGlueLevel = 0;
	}
   else 
	{
      CPoint pt; GetCursorPos( &pt );
	  ScreenToClient( &pt );
      ClientToDoc_SC( &pt );

	  if( wp == MSGA_ENTER )
	   {
	     TDrawShape *pCh = NULL;
	     switch( (TObjTypes)lp )
		  {
		    case TO_Cnct_Corn:			  
			  pCh = new TCnct( TO_Cnct_Corn );
			  break;
			case TO_Cnct_Simple:
			  pCh = new TCnct( TO_Cnct_Simple );
			  break;
			case TO_Cnct_Sq:
			  pCh = new TCnct( TO_Cnct_Sq );
			  break;
			case TO_Cnct_Cln:
			  pCh = new TCnct( TO_Cnct_Cln );
			  break;
			case TO_PC:
			  pCh = new TDrawPC();
			  if( GetDocument()->m_bFlSimpleView )
			    reinterpret_cast<TDrawPC*>(pCh)->SwitchModes( true );
			  break;
			case TO_Point:
			  pCh = new TDrawPoint();
			  break;
			case TO_Text:			 
			 {
			   pCh = new TDrawTxt();
			   ((TDrawTxt*)pCh)->SetBounds();
			   break;
			 }
		  };

		 if( pCh ) 
		  {			
			CRect position = pCh->m_position;
			CPoint ptCObj( position.left + abs(position.Width())/2,
						   position.top - abs(position.Height())/2
						 );
			position.OffsetRect( pt - ptCObj );
			pCh->m_position = position;

			GetDocument()->Add( pCh );		    

			pCh->SetInve();
			InvalObj( pCh );
		  }

		 TDrawShape::m_iGlueLevel = 2;
	   }
	  else if( wp == MSGA_DRAG )
	   {
	     TDrawShapeList& rLst = GetDocument()->RefLstDraw();
	     TDrawShape *pShp = (*--rLst.end()).get();

		 CRect position = pShp->m_position;
		 CPoint ptCObj( position.left + abs(position.Width())/2,
		                position.top - abs(position.Height())/2
					  );
		 position.OffsetRect( pt - ptCObj );
		 
		 pShp->SetInve();
		 pShp->MoveTo( position, this );
		 InvalObj( pShp );
	   }
	  else if( wp == MSGA_ACCEPT )
	   {
         TDrawShapeList& rLst = GetDocument()->RefLstDraw();
	     (*--rLst.end())->SetInve( false );
		 SelectShp( (*--rLst.end()).get() );
		 InvalObj( (*--rLst.end()).get() );

		 GetDocument()->SetModifiedFlag();
		 m_pFram->SetActiveView( this );

		 TDrawShape::m_iGlueLevel = 0;
		 GetDocument()->RecalcAllGlued();
	   }
	}

   return TRUE;
 }

void TDiplomView::SetCenterViewToSelection()
 {
   if( GetDocument() && GetDocument()->HaveSelectedObj() )	
     GetDocument()->m_ptCenter = GetDocument()->GetSelectionCenter( true );	
 }

void TDiplomView::OnDestroy() 
 {    
	CScrollView::OnDestroy();
	
	if( m_bFlPopupCreated )
	  m_bFlPopupCreated = FALSE,
	  PopMenu.DestroyMenu();	

	if( m_bFlPopupCreated_Br )
	  m_bFlPopupCreated_Br = FALSE,
	  m_menBrig.DestroyMenu();		

	if( m_bFlPopupCreated_Calc )
	  m_bFlPopupCreated_Calc = FALSE,
	  m_menCalc.DestroyMenu();			
 }

LRESULT TDiplomView::OnCommandHelp( WPARAM wp, LPARAM lParam )
 {
   if( m_pFram->m_optX.m_bFlModeBrig ) return IDH_BrigEditor;
   return m_pFram->m_bFlRun ? IDH_SimplifyMode:IDH_EditorSchemes;
 }

LRESULT TDiplomView::OnCommandHelpF1( WPARAM wp, LPARAM lParam )
 {
   if( m_pFram->m_optX.m_bFlModeBrig )
     AfxGetApp()->CWinApp::WinHelp( IDH_BrigEditor );
   AfxGetApp()->CWinApp::WinHelp( m_pFram->m_bFlRun ? IDH_SimplifyMode:IDH_EditorSchemes );    
   return TRUE;
 }
