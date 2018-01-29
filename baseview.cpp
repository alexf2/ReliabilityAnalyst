#include "basview.hpp"
#include "getgdimgr.hpp"

#include <afxcmn.h>


IMPLEMENT_DYNCREATE( TBaseView, CView )

BEGIN_MESSAGE_MAP( TBaseView, CView )	
  ON_WM_CREATE()  
  
  ON_WM_ERASEBKGND() 

  ON_WM_SIZE()
  ON_WM_SETFOCUS()

  ON_WM_DESTROY()

END_MESSAGE_MAP()

TBaseView::TBaseView(): CView()
 {        
   m_iMinW = m_iMinH = 100;

   m_hWndFocusCTL = m_hWndFocus = NULL;

   m_bHScr = m_bVScr = false;
   m_sX_ = m_sY_ = 0;

   m_bFlScrollEnable = false;
 }

TBaseView::~TBaseView()
 {      
   list<TChildItem*>::iterator it( m_lstCh.begin() );
   list<TChildItem*>::iterator itEnd( m_lstCh.end() );

   for( ; it != itEnd; ++it )
	  delete *it;

   m_lstCh.clear();
 }


BOOL TBaseView::OnEraseBkgnd( CDC *pDC )
 {
   //pDC->SetMapMode( MM_TEXT );
   //pDC->SetViewportOrg( 0, 0 );   

   CRect r; GetClientRect( r ); 
   pDC->FillSolidRect( r, RGB(255,255,255) );

 
   return TRUE;
   //return CView::OnEraseBkgnd( pDC );
 }

void TBaseView::OnUpdate( CView* pSender, LPARAM lHint, CObject* pHint )
 {
   // ...
   // Implement a GetDocSize( ) member function in 
   // your document class; it returns a CSize.
   //SetScrollSizes( MM_TEXT, ((TGrafDoc*)GetDocument())->GetDocSize( ) );
   //ResizeParentToFit( );   // Default bShrinkOnly argument
   // ...
   CView::OnUpdate( pSender, lHint, pHint );
 }




void TBaseView::OnDestroy()
 {
   CView::OnDestroy();
 }

int TBaseView::OnCreate( LPCREATESTRUCT lpCreateStruct )
 {
   if( CView::OnCreate(lpCreateStruct) == -1 )
     return -1;
             
   return 0; 
 }

void TBaseView::OnInitialUpdate()
 {
   CView::OnInitialUpdate();   
 }

inline double ConvertXY( double dVal, int iBound )
 {
   return  dVal > 1 ? dVal:double(iBound) * dVal;
 }

void TBaseView::MakeLaoyt( bool bFlRedr )
 {
   UINT uiRedr = (bFlRedr == false ? SWP_NOREDRAW:0);
   int iHD = 0, iVD = 0;

   /*if( m_bFlScrollEnable == true )
	{
      if( m_bHScr == true ) iHD = GetScrollPos( SB_HORZ );
      if( m_bVScr == true ) iVD = GetScrollPos( SB_VERT );
	}*/

   list<TChildItem*>::iterator it( m_lstCh.begin() );
   list<TChildItem*>::iterator itEnd( m_lstCh.end() );

   CRect r; GetClientRect( r );

   if( r.right < m_iMinW ) r.right = m_iMinW;
   if( r.bottom < m_iMinH ) r.bottom = m_iMinH;


   for( ; it != itEnd; ++it )
	if( (*it)->m_ts == TChildItem::TS_RelationParent )
	   (*it)->m_pCh->SetWindowPos( NULL, 
	     ConvertXY( (*it)->m_left, r.Width() ) - iHD, 
	     ConvertXY( (*it)->m_top, r.Height() ) - iVD,
	     ConvertXY( (*it)->m_W, r.Width() ),
	     ConvertXY( (*it)->m_H, r.Height() ),
		 SWP_NOZORDER|SWP_NOACTIVATE|uiRedr );


   it = m_lstCh.begin();
   itEnd = m_lstCh.end();
   for( ; it != itEnd; ++it )
	if( (*it)->m_ts == TChildItem::TS_RelationFrom )
	  {
        CRect rr; (*it)->m_pFrom->GetWindowRect( rr );
		ScreenToClient( rr );

		CRect rSelf; (*it)->m_pCh->GetWindowRect( rSelf );
		//ScreenToClient( rSelf );

		if( (*it)->m_rel == CH_Top )
		  (*it)->m_pCh->SetWindowPos( NULL, 
		    rr.left, 
			rr.top - rSelf.Height() - 1, 0, 0,
			SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE|uiRedr );
	  }
 }

void TBaseView::OnSize( UINT nType, int cx, int cy )
 {
   if( nType == SIZE_MINIMIZED ) 
	{
      CView::OnSize( nType, cx, cy );
	  return;
	}

   //if( cx < m_iMinW ) cx = m_iMinW;
   //if( cy < m_iMinH ) cy = m_iMinH;	
   CView::OnSize( nType, cx, cy );

   //CRect r; GetClientRect( r );
   //cx = r.Width(), cy = r.Height();

   MakeLaoyt();   

   if( m_bFlScrollEnable == false ) return;

   int iMaxX = 0, iMaxY = 0;
   int iMinX = 65535, iMinY = 65535;
   CWnd* pCh = GetWindow( GW_CHILD );
   if( pCh != NULL )
		pCh = pCh->GetWindow( GW_HWNDLAST );
	while( pCh )
	 {
	   CRect r; pCh->GetWindowRect( r ); ScreenToClient( r );
       iMaxX = max( iMaxX, r.right );
	   iMaxY = max( iMaxY, r.bottom );

	   iMinX = min( iMinX, r.left );
	   iMinY = min( iMinY, r.top );

	   pCh = pCh->GetWindow( GW_HWNDPREV );
	 } 

   iMaxX = abs( iMaxX - iMinX );
   iMaxY = abs( iMaxY - iMinY );


   DWORD dwRem = 0, dwAdd = 0;
   if( iMaxX-2 > cx && m_bHScr == false ) dwAdd |= WS_HSCROLL, m_bHScr = true, m_sX_ = 0;
   if( iMaxX-2 < cx && m_bHScr == true ) dwRem |= WS_HSCROLL, m_bHScr = false;

   if( iMaxY-2 > cy && m_bVScr == false ) dwAdd |= WS_VSCROLL, m_bVScr = true, m_sY_ = 0;
   if( iMaxY-2 < cy && m_bVScr == true ) dwRem |= WS_VSCROLL, m_bVScr = false;

   if( dwRem || dwAdd ) ModifyStyle( dwRem, dwAdd );
     
   if( m_bHScr == true )
	 SetScrollRange( SB_HORZ, 0, iMaxX - cx + 8, FALSE ); 
   if( m_bVScr == true )
	 SetScrollRange( SB_VERT, 0, iMaxY - cy + 8, FALSE ); 	 

   if( nType == SIZE_MAXIMIZED ) 
	{
	  CWnd* pCh = GetWindow( GW_CHILD );
	  if( pCh != NULL ) pCh = pCh->GetWindow( GW_HWNDLAST );
	  while( pCh )
	   {
		 pCh->InvalidateRect( NULL ); 
		 pCh = pCh->GetWindow( GW_HWNDPREV );
	   } 
	  InvalidateRect( NULL ); 
	}
 }



BOOL TBaseView::PreCreateWindow( CREATESTRUCT& cs )
 {      
   BOOL bRes = CView::PreCreateWindow( cs );
   if( !bRes ) return 0;
   /*static CString str;
   str =
	AfxRegisterWndClass(0, 
	  LoadCursor(NULL, IDC_ARROW), 
	  (HBRUSH)(COLOR_WINDOW + 1)); */

   cs.style |= WS_CLIPCHILDREN;   
   //cs.lpszClass = (LPCSTR)str;

   return TRUE;
 }


void TBaseView::OnDraw( CDC* pDC ) //use as pure virtual
 {
 }



void TBaseView::OnUpdate_Always( CCmdUI*  pCmdUI )
 {
   pCmdUI->Enable( TRUE );   
 }


void TBaseView::OnActivateView(
	BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
 {
	if (SaveFocusControl())
		return;     // don't call base class when focus is already set

	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
 }

void TBaseView::OnActivateFrame(UINT nState, CFrameWnd* /*pFrameWnd*/)
 {
	if (nState == WA_INACTIVE)
		SaveFocusControl();     // save focus when frame loses activation
 }

BOOL TBaseView::SaveFocusControl()
 {
	// save focus window if focus is on this window's controls
	HWND hWndFocus = ::GetFocus();
	if (hWndFocus != NULL && ::IsChild(m_hWnd, hWndFocus))
	{
		m_hWndFocus = hWndFocus;
		return TRUE;
	}
	return FALSE;
 }

void TBaseView::OnSetFocus(CWnd*)
 {
	if (!::IsWindow(m_hWndFocus) || !::IsChild(m_hWnd, m_hWndFocus))
	{
		// invalid or unknown focus window... let windows handle it
		m_hWndFocus = NULL;
		Default();
		return;
	}
	// otherwise, set focus to the last known focus window
	::SetFocus(m_hWndFocus);
 }

BOOL TBaseView::PreTranslateMessage(MSG* pMsg)
 {
	ASSERT(pMsg != NULL);
	ASSERT_VALID(this);
	ASSERT(m_hWnd != NULL);

	// allow tooltip messages to be filtered
	if (CView::PreTranslateMessage(pMsg))
		return TRUE;

	// don't translate dialog messages when in Shift+F1 help mode
	CFrameWnd* pFrameWnd = GetTopLevelFrame();
	if (pFrameWnd != NULL && pFrameWnd->m_bHelpMode)
		return FALSE;

	// since 'IsDialogMessage' will eat frame window accelerators,
	//   we call all frame windows' PreTranslateMessage first
	pFrameWnd = GetParentFrame();   // start with first parent frame
	while (pFrameWnd != NULL)
	{
		// allow owner & frames to translate before IsDialogMessage does
		if (pFrameWnd->PreTranslateMessage(pMsg))
			return TRUE;

		// try parent frames until there are no parent frames
		pFrameWnd = pFrameWnd->GetParentFrame();
	}

	// filter both messages to dialog and from children
	return PreTranslateInput(pMsg);
 }

BOOL TBaseView::OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult )
 { 
   NMHDR *ph = (NMHDR*)lParam;
   if( ph->code == NM_SETFOCUS ) 	
	 m_hWndFocus = m_hWndFocusCTL = ph->hwndFrom;	

   //return CView::OnNotify(wParam, lParam, pResult);

   if( CView::OnNotify(wParam, lParam, pResult) )
	 return TRUE;

   CFrameWnd* pFrameWnd = GetTopLevelFrame();
   if( pFrameWnd != NULL && pFrameWnd->m_bHelpMode )
	   return FALSE;

   pFrameWnd = GetParentFrame();   // start with first parent frame
   while( pFrameWnd != NULL )
    {
	  // allow owner & frames to translate before IsDialogMessage does
	  
	  if( pFrameWnd->SendMessage(WM_NOTIFY, wParam, lParam) )
		  return TRUE;

	  // try parent frames until there are no parent frames
	  pFrameWnd = pFrameWnd->GetParentFrame();
    }

   return FALSE;
 }

BOOL TBaseView::OnCommand( WPARAM wParam, LPARAM lParam )
 {
   if( HIWORD(wParam) == LBN_SETFOCUS )
	 m_hWndFocus = m_hWndFocusCTL = (HWND)lParam;

   
   if( CView::OnCommand(wParam, lParam) ) return TRUE;

   CFrameWnd* pFrameWnd = GetTopLevelFrame();
   if( pFrameWnd != NULL && pFrameWnd->m_bHelpMode )
	   return FALSE;
   

   // since 'IsDialogMessage' will eat frame window accelerators,
   //   we call all frame windows' PreTranslateMessage first
   pFrameWnd = GetParentFrame();   // start with first parent frame
   while( pFrameWnd != NULL )
    {
	  // allow owner & frames to translate before IsDialogMessage does
	  
	  if( pFrameWnd->SendMessage(WM_COMMAND, wParam, lParam) )
		  return TRUE;

	  // try parent frames until there are no parent frames
	  pFrameWnd = pFrameWnd->GetParentFrame();
    }

   // filter both messages to dialog and from children
   return FALSE;
 }
