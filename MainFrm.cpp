// MainFrm.cpp : implementation of the TMainFrame class
//

#include "stdafx.h"
#include "diplom.h"

#include "MainFrm.h"
#include "about.hpp"
#include "globlmsg.hpp"
#include "childfrm.h"
#include "diplomView.h"

#include "resource.h"

#include <algorithm>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TMainFrame

IMPLEMENT_DYNAMIC(TMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(TMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(TMainFrame)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
	ON_WM_PALETTECHANGED()
	ON_WM_QUERYNEWPALETTE()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_ACTIVATE()
	//}}AFX_MSG_MAP
	
	ON_UPDATE_COMMAND_UI( AFX_IDW_TOOLBAR, CMDIFrameWnd::OnUpdateControlBarMenu )
	ON_COMMAND_EX( AFX_IDW_TOOLBAR, CMDIFrameWnd::OnBarCheck )
	

	ON_COMMAND( ID_HELP_FINDER, CMDIFrameWnd::OnHelpFinder )
	ON_COMMAND( ID_HELP, CMDIFrameWnd::OnHelp )
	ON_COMMAND( ID_CONTEXT_HELP, CMDIFrameWnd::OnContextHelp )
	ON_COMMAND( ID_DEFAULT_HELP, CMDIFrameWnd::OnHelpFinder )
			
	ON_UPDATE_COMMAND_UI( ID_WINDOW_CLOSEALL, OnUserWnd )	
	ON_UPDATE_COMMAND_UI( ID_WINDOW_NEXT, OnUserWnd )
	ON_UPDATE_COMMAND_UI( ID_WINDOW_PRIV, OnUserWnd )
	ON_UPDATE_COMMAND_UI( ID_APP_WINDOW, OnUserWnd )		

	ON_UPDATE_COMMAND_UI( ID_VIEW_LOCALTOOL, OnLocalToolUpd )	
	ON_UPDATE_COMMAND_UI( ID_VIEW_TEXTURE, OnLocalToolTextureUpd )		
	ON_UPDATE_COMMAND_UI( ID_VIEW_SAVEOPT, OnVievSaveOptUpd )		
	ON_UPDATE_COMMAND_UI( ID_VIEV_SVEDESKTOP, OnVievSaveDesktopUpd )		
	

	ON_UPDATE_COMMAND_UI( ID_APPABOUT, OnAlways )			
	
	ON_UPDATE_COMMAND_UI( AFX_IDM_FIRST_MDICHILD, OnAlways )	

	ON_COMMAND( ID_APPABOUT, OnAbout )	
	ON_COMMAND( ID_APP_WINDOW, OnAppWindow )
	ON_COMMAND( ID_WINDOW_CLOSEALL, OnCloseAll )
	ON_COMMAND( ID_WINDOW_NEXT, OnNext )
	ON_COMMAND( ID_WINDOW_PRIV, OnPriv )			

	ON_COMMAND( ID_VIEW_LOCALTOOL, OnLocalTool )
	ON_COMMAND( ID_VIEW_TEXTURE, OnLocalTexture )	
	ON_COMMAND( ID_VIEW_SAVEOPT, OnVievSaveOpt )		
	ON_COMMAND( ID_VIEV_SVEDESKTOP, OnVievSaveDesktop )		

	ON_REGISTERED_MESSAGE( globl_iMsgOpenDoc, OnGLOpenDoc )

	//ON_WM_QUERYNEWPALETTE()
    //ON_WM_PALETTECHANGED()

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR
};


static TBtnDescr buttons[] =
 {	
  { ID_FILE_NEW, false, true },	    		  
  { ID_FILE_OPEN, false, true },
  { ID_FILE_SAVE, false, true },  

	{ ID_SEPARATOR,	false, true },

  { ID_EDIT_CLEAR, false, true },
  { ID_EDIT_DUP, false, true },
  { ID_EDIT_UNDO, false, true },
  { ID_EDIT_REDO, false, true },  

	{ ID_SEPARATOR,	false, true },
    { ID_MODE_BRIG, false, false }, 
	{ ID_SEPARATOR,	false, true },
	  
  { ID_CONTEXT_HELP, true, true }
 };
 
static UINT buttonsRC[] =
 {   
   IDB_NEW,
   IDB_OPEN,
   IDB_SAVE,   
    0,
   IDB_CLEAR,
   IDB_DUP,	
   IDB_UNDO,
   IDB_REDO,
    0,
	IDB_PROP,
    0,
   IDB_HELP
 };

static char* const lpButtonsText[] =
 {
	"Создать/~N~",
	"Открыть/~O~",
	"Сохранить/~S~",	

	"",

	"Удалить/~E~",
	"Дублировать/~D~",	
	"Отменить/~O~",
	"Повторить/~R~",
	
	"",
	"Бригады/~B~",
	"",
    
	"Справка/~H~",	
 };


void TMainFrame::OnUserWnd( CCmdUI* pCmdUI )
 {
   pCmdUI->Enable( MDIGetActive() != NULL );
 }

void TMainFrame::OnAlways( CCmdUI* pCmdUI )
{
  pCmdUI->Enable( TRUE );
}

void TMainFrame::OnVievSaveOptUpd( CCmdUI* pCmdUI )
 {
   CMenu *mnu = GetMenu();	 
   /*mnu->CheckMenuItem( ID_VIEW_SAVEOPT, 
     MF_BYCOMMAND|(GetOpt().d.m_bSaveOptOnExit ? MF_CHECKED:MF_UNCHECKED) );
*/
   pCmdUI->Enable( TRUE );
   pCmdUI->SetCheck( GetOpt().d.m_bSaveOptOnExit );
 }
void TMainFrame::OnVievSaveDesktopUpd( CCmdUI* pCmdUI )
 {
   CMenu *mnu = GetMenu();	 
   /*mnu->CheckMenuItem( ID_VIEV_SVEDESKTOP, 
     MF_BYCOMMAND|(GetOpt().d.m_bSaveDesktopOnExit ? MF_CHECKED:MF_UNCHECKED) );
   */
   pCmdUI->Enable( TRUE );
   pCmdUI->SetCheck( GetOpt().d.m_bSaveDesktopOnExit );
 }

void TMainFrame::OnLocalToolTextureUpd( CCmdUI* pCmdUI )
 {   
   CMenu *mnu = GetMenu();	 
   /*mnu->CheckMenuItem( ID_VIEW_TEXTURE, 
     MF_BYCOMMAND|(GetOpt().d.m_bFlFactureOn ? MF_CHECKED:MF_UNCHECKED) );
*/
   OnUserWnd( pCmdUI );
   pCmdUI->SetCheck( GetOpt().d.m_bFlFactureOn );
 }

void TMainFrame::OnLocalToolUpd( CCmdUI* pCmdUI )
 {   
   CMenu *mnu = GetMenu();	 
   /*mnu->CheckMenuItem( ID_VIEW_LOCALTOOL, 
     MF_BYCOMMAND|(GetOpt().d.m_bFlLocToolOn ? MF_CHECKED:MF_UNCHECKED) );
*/
   OnUserWnd( pCmdUI );
   pCmdUI->SetCheck( GetOpt().d.m_bFlLocToolOn );
 }

/////////////////////////////////////////////////////////////////////////////
// TMainFrame construction/destruction

TMainFrame::TMainFrame()
{
	// TODO: add member initialization code here
  globl_iMsgQNewPal = RegisterWindowMessage( "WM_GL_QUERYNEWPAL" );
  globl_iMsgPalChanged = RegisterWindowMessage( "WM_GL_PALCHANGED" );
  globl_iMsgOpenDoc = RegisterWindowMessage( "WM_GL_OPENDOC" );
  globl_iMsgTestAcceptDrag = RegisterWindowMessage( "WM_LST_ACCEPTDRAG" );
  globl_iMsgReqvestLocalMenu = RegisterWindowMessage( "WM_DVIEW_REQMENU" );
  globl_iMsgEndCalc = RegisterWindowMessage( "WM_CH_ENDCALC" );
  globl_iMsgEndRun = RegisterWindowMessage( "WM_CH_ENDRUN" );
  globl_iMsgNewStatusStr = RegisterWindowMessage( "WM_CH_NEWSTATUSSTR" );

  m_bFirstShow = true;
  m_pKeyBrigIt = NULL;

  m_bActive = false;
}

TMainFrame::~TMainFrame()
{
}


void TMainFrame::SaveCfg( CString& csPath )
 {
   fstream fStrm( (LPCSTR)csPath, ios::out|ios::binary|ios::trunc );
   fStrm << m_opt;

   int iL = m_lstChOpt.size();
   fStrm.write( (char*)&iL, sizeof(iL) );
   TSaveCfgData dta( fStrm );
   for_each( m_lstChOpt.begin(), m_lstChOpt.end(), dta );
 }

void TMainFrame::LoadCfg( CString& csPath )
 {
   fstream fStrm( (LPCSTR)csPath, ios::in|ios::binary );
   fStrm >> m_opt;   

   if( m_opt.d.m_bSaveDesktopOnExit )
	{
	  int iL = 0;
	  fStrm.read( (char*)&iL, sizeof(iL) );
	  for( int i = iL; i; --i )
	   {
		 TMCildOpt_AP ap( new TMCildOpt() );
		 fStrm >> *ap;
		 m_lstChOpt.push_back( ap );
	   }
	}
 }

void TMainFrame::OnAbout()
 {
   try {
     AboutShow( this, IDB_DIB_ABOUT256 );
	}
   catch( CException *pE )
	{	  
	  pE->ReportError( MB_OK | MB_ICONSTOP );
	  pE->Delete();  	
	  return;
	}   
 }

void TMainFrame::OnAppWindow()
 {
   WindowProc( WM_COMMAND, 0xFF09, 0 );
 }

void TMainFrame::OnNext()
 {
   MDINext();
 }

void TMainFrame::OnPriv()
 {
   CMDIChildWnd* pCh = MDIGetActive();
   if( !pCh ) return;

   //pCh->MDIDestroy(); return;

   HWND hwPriv = ::GetWindow( pCh->m_hWnd, GW_HWNDLAST );
	//pCh->m_hWnd;
   HWND hw = ::GetWindow( hwPriv, GW_HWNDNEXT );
   while( hw != pCh->m_hWnd && hw )
	{
      hwPriv = hw;
	  hw = ::GetWindow( hwPriv, GW_HWNDNEXT );
	}

   if( !hwPriv || hwPriv == pCh->m_hWnd ) return;

   CWnd *pWnd = CWnd::FromHandle( hwPriv );
   MDIActivate( pWnd );
 }

void TMainFrame::OnCloseAll()
 {
   CWinApp *pApp = AfxGetApp();

   POSITION ps = pApp->GetFirstDocTemplatePosition();
   CDocTemplate *pDocTmpl;
   while( ps )
	{
      pDocTmpl = pApp->GetNextDocTemplate( ps );

      POSITION pos = pDocTmpl->GetFirstDocPosition();
	  CDocument *pDoc;
	  while( pos ) 
	   {
         pDoc = pDocTmpl->GetNextDoc( pos );
	     if( ((TDiplomDoc*)pDoc)->My_CanClose() == FALSE ) return;
	   }
	}

   ps = pApp->GetFirstDocTemplatePosition();   
   while( ps )
	{
      pDocTmpl = pApp->GetNextDocTemplate( ps );

      POSITION pos = pDocTmpl->GetFirstDocPosition();
	  CDocument *pDoc;
	  while( pos ) 
	   {
         pDoc = pDocTmpl->GetNextDoc( pos );
	     pDoc->OnCloseDocument();
	   }
	}   
 }

int TMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	
	if( !m_wndStatusBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_BOTTOM , AFX_IDW_STATUS_BAR) ||	
	    !m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT))
	  )
	 {
		 //TRACE0("Failed to create status bar\n");
		 ((TDiplomApp*)AfxGetApp())->bwcc.BWCCMessageBox( m_hWnd, "Ошибка при создании строки статуса", "Ошибка", MB_OK | MB_ICONSTOP );
		 return -1;      
	 }

	UINT nID, nStyle;
	int cxWidth;
	m_wndStatusBar.GetPaneInfo( 0, nID, nStyle, cxWidth );
	m_wndStatusBar.SetPaneInfo( 0, nID, SBPS_STRETCH | SBPS_NORMAL, cxWidth );	


	m_apTool = auto_ptr<TMSStyleTooolBar>( new TMSStyleTooolBar(this) );
		
	for( int i = 0; i < sizeof(buttons) / sizeof(TBtnDescr); i++ )
	 {                      
	   TMSItem *pMsIt;
	   pMsIt = new TMSItem(); 
	   pMsIt->Init( lpButtonsText[i], buttonsRC[i], 
		 buttons[i].uCmd, buttons[i].bRight, buttons[i].bPush );

	   if( buttons[i].uCmd == ID_MODE_BRIG ) m_pKeyBrigIt = pMsIt;
	  
	   *m_apTool, pMsIt;
	   //void Init( LPSTR lpStr, UINT uiIdRcBmp, UINT m_uiCommand );
	 }
	
    LOGFONT lf = { 8, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET,
      OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
	  VARIABLE_PITCH | FF_DONTCARE, "MS Sans Serif"
	};

	CFont *pFnt = new CFont();
	pFnt->CreateFontIndirect( &lf );
    m_apTool->SetFont( pFnt );
	
    m_apTool->Init( false ); 

    if( !m_apTool->Create(this, WS_CHILD | WS_VISIBLE |
			CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY,  AFX_IDW_TOOLBAR ) 
		//!m_arcTool()->LoadBitmap(IDR_MAINFRAME) 		
	  )
	{
		//AfxMessageBox( "Failed to create toolbar\n", MB_OK | MB_ICONSTOP );
	    ((TDiplomApp*)AfxGetApp())->bwcc.BWCCMessageBox( m_hWnd, "Ошибка при создании тулбара", "Ошибка", MB_OK | MB_ICONSTOP );
		return -1;      
	}

	return 0;
}

BOOL TMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
   cs.style |= WS_HSCROLL | WS_HSCROLL;   
   BOOL bRes = CMDIFrameWnd::PreCreateWindow( cs );
   if( !bRes ) return bRes;
    
   return bRes;
}


/////////////////////////////////////////////////////////////////////////////
// TMainFrame diagnostics

#ifdef _DEBUG
void TMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void TMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// TMainFrame message handlers

void TMainFrame::OnDestroy() 
 {    
	

	CMDIFrameWnd::OnDestroy();
	
	// TODO: Add your message handler code here	
 }

void TMainFrame::OnClose() 
 {
	// TODO: Add your message handler code here and/or call default
   CMDIChildWnd* pMDIChild = MDIGetActive();
   if( pMDIChild != NULL )
	   pMDIChild = (CMDIChildWnd*)pMDIChild->GetWindow( GW_HWNDLAST );
   while( pMDIChild != NULL )
	{	
	  if( !((TChildFrame*)pMDIChild)->MyCanClose() ) return;
	  pMDIChild = (CMDIChildWnd*)pMDIChild->GetWindow( GW_HWNDPREV );
	} 
	
	CMDIFrameWnd::OnClose();
 }

void TMainFrame::My_ShowControlBar( int iId, int iFl )
 {
   CControlBar *pBar = GetControlBar( iId );
   if( pBar )
     ShowControlBar( pBar, iFl, FALSE );
 }

   

void TMainFrame::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CMDIFrameWnd::OnShowWindow(bShow, nStatus);

	/*if( bShow == false )
	 {
	m_opt = TMFrOpt( this );

	CControlBar *pBar = GetControlBar( AFX_IDW_STATUS_BAR );   
	m_opt.m_bFlStatusOn = pBar ? pBar->IsWindowVisible():1;
	pBar = GetControlBar( AFX_IDW_TOOLBAR );   
	m_opt.m_bFlToolOn = pBar ? pBar->IsWindowVisible():1;
	 }*/

    if( bShow == false || m_bFirstShow == false ) return;
	m_bFirstShow = false;

	try {
	   LoadCfg( static_cast<TDiplomApp*>(AfxGetApp())->GetConfig() );

	   if( !m_opt.d.m_bSaveOptOnExit )
		{
          m_opt = TMFrOpt();
		  m_opt.d.m_bSaveOptOnExit = m_opt.d.m_bSaveDesktopOnExit = 0;
		}
	   else
		{
		  int iScrW = GetSystemMetrics( SM_CXSCREEN );
		  int iScrH = GetSystemMetrics( SM_CYSCREEN );

		  if( m_opt.d.m_iSCX != iScrW || m_opt.d.m_iSCY != iScrH )
			DefaultPos();
		  else 
		   {
			 SetWindowPlacement( &m_opt.m_wndPlac );
			 My_ShowControlBar( AFX_IDW_STATUS_BAR, m_opt.d.m_bFlStatusOn );
			 My_ShowControlBar( AFX_IDW_TOOLBAR, m_opt.d.m_bFlToolOn );
		   }
		  
		  if( !m_opt.d.m_bSaveDesktopOnExit ) m_lstChOpt.clear();
		  else
		   {
			 TCreData dta( this );
			 for_each( m_lstChOpt.begin(), m_lstChOpt.end(), dta );
		   }
		}
	 }
	catch( CException *pE )
	 {
       pE->ReportError();
	   pE->Delete();

	   DefaultPos();
	   m_opt = TMFrOpt();
	 }	
	catch( exception ex )
	 {
       MessageBox( ex.what(), "Ошибка", MB_OK|MB_ICONEXCLAMATION );

	   DefaultPos();
	 }
	
	// TODO: Add your message handler code here	
}

void TMainFrame::StoreCfg()
 {   
    //m_opt = TMFrOpt( this );		
    if( !m_opt.d.m_bSaveOptOnExit )
	 {
       m_lstChOpt.clear();
	   m_opt = TMFrOpt();
	   m_opt.d.m_bSaveOptOnExit = 0;
	   m_opt.d.m_bSaveDesktopOnExit = 0;
	   return;
	 }
    GetWindowPlacement( &m_opt.m_wndPlac );

	CControlBar *pBar = GetControlBar( AFX_IDW_STATUS_BAR );   
	m_opt.d.m_bFlStatusOn = pBar ? pBar->IsWindowVisible():1;
	pBar = GetControlBar( AFX_IDW_TOOLBAR );   
	m_opt.d.m_bFlToolOn = pBar ? pBar->IsWindowVisible():1;

	/*CMenu *mnu = GetMenu();
	m_opt.d.m_bFlLocToolOn = 
	  (mnu->GetMenuState(ID_VIEW_LOCALTOOL, MF_BYCOMMAND) & MF_CHECKED);
	m_opt.d.m_bFlFactureOn =
	  (mnu->GetMenuState(ID_VIEW_TEXTURE, MF_BYCOMMAND) & MF_CHECKED);	
*/
	m_lstChOpt.clear();

	if( m_opt.d.m_bSaveDesktopOnExit )
	 {
	   CMDIChildWnd* pMDIChild = MDIGetActive();
	   if( pMDIChild != NULL )
		   pMDIChild = (CMDIChildWnd*)pMDIChild->GetWindow( GW_HWNDLAST );
	   while( pMDIChild != NULL )
		{	
		  try {
		    m_lstChOpt.push_back(
		      reinterpret_cast<TChildFrame*>(pMDIChild)->GetCHCfg() );		 
		   }
		  catch( exception e )
		   {
              MessageBox( e.what(), "Ошибка", MB_OK | MB_ICONASTERISK );
			  return;
		   }
  
		  pMDIChild = (CMDIChildWnd*)pMDIChild->GetWindow( GW_HWNDPREV );
		} 
	 }
 }

void TMainFrame::DefaultPos()
 {
   TMSStyleTooolBar	*pBar = (TMSStyleTooolBar*)GetDescendantWindow( AFX_IDW_TOOLBAR );
   if( !pBar || typeid(*pBar) != typeid(TMSStyleTooolBar) ) return;

   int iWidth = pBar->GetReqvWidth() + 20 + 2*GetSystemMetrics( SM_CXBORDER );
   int iScrW = GetSystemMetrics( SM_CXSCREEN );
   int iScrH = GetSystemMetrics( SM_CYSCREEN );
   int iWOpt = float(iScrW) * (2.0/3.0);
   int iHOpt = float(iScrH) * (2.5/3.0);

   int iW = min( max(iWidth, iWOpt), iScrW);

   SetWindowPos( NULL, (iScrW - iW)/2, (iScrH - iHOpt)/2, iW, iHOpt, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOREDRAW );
 }


void TMainFrame::OnPaletteChanged( CWnd* pFocusWnd ) 
{
   if( !m_opt.d.m_bFlFactureOn )
	{
	  CMDIFrameWnd::OnPaletteChanged( pFocusWnd );
	  return;
	}

   if( pFocusWnd->GetSafeHwnd() == GetSafeHwnd() || IsChild(pFocusWnd) )
     CMDIFrameWnd::OnPaletteChanged( pFocusWnd );
   else
     SendMessageToDescendants( globl_iMsgQNewPal, 0, 0, TRUE, FALSE );
	
	// TODO: Add your message handler code here
	
}

BOOL TMainFrame::OnQueryNewPalette() 
{
	// TODO: Add your message handler code here and/or call default

   if( !m_opt.d.m_bFlFactureOn )
	 return CMDIFrameWnd::OnQueryNewPalette();
	
   BOOL bRes = 0;
   SendMessageToDescendants( globl_iMsgQNewPal, 0, (LPARAM)&bRes, TRUE, FALSE );

   return bRes;
}


void TMainFrame::OnVievSaveOpt()
 {
   CMenu *mnu = GetMenu();
   if( mnu->GetMenuState(ID_VIEW_SAVEOPT, MF_BYCOMMAND) & MF_CHECKED )
	 m_opt.d.m_bSaveOptOnExit = 0,
     mnu->CheckMenuItem( ID_VIEW_SAVEOPT, MF_BYCOMMAND|MF_UNCHECKED );
   else m_opt.d.m_bSaveOptOnExit = 1,
	 mnu->CheckMenuItem( ID_VIEW_SAVEOPT, MF_BYCOMMAND|MF_CHECKED );   
 }
void TMainFrame::OnVievSaveDesktop()
 {
   CMenu *mnu = GetMenu();
   if( mnu->GetMenuState(ID_VIEV_SVEDESKTOP, MF_BYCOMMAND) & MF_CHECKED )
	 m_opt.d.m_bSaveDesktopOnExit = 0,
     mnu->CheckMenuItem( ID_VIEV_SVEDESKTOP, MF_BYCOMMAND|MF_UNCHECKED );
   else m_opt.d.m_bSaveDesktopOnExit = 1,
	 mnu->CheckMenuItem( ID_VIEV_SVEDESKTOP, MF_BYCOMMAND|MF_CHECKED );   
 }

void TMainFrame::OnLocalTexture()
 {
   CMenu *mnu = GetMenu();
   if( mnu->GetMenuState(ID_VIEW_TEXTURE, MF_BYCOMMAND) & MF_CHECKED )
	 m_opt.d.m_bFlFactureOn = 0,
     mnu->CheckMenuItem( ID_VIEW_TEXTURE, MF_BYCOMMAND|MF_UNCHECKED );
   else m_opt.d.m_bFlFactureOn = 1,
	 mnu->CheckMenuItem( ID_VIEW_TEXTURE, MF_BYCOMMAND|MF_CHECKED );   

   CMDIChildWnd* pMDIChild = MDIGetActive();
   if( pMDIChild != NULL )
	   pMDIChild = (CMDIChildWnd*)pMDIChild->GetWindow( GW_HWNDLAST );
   while( pMDIChild != NULL )
	{
	  /*if( pMDIChild->GetActiveView() &&
	     ((TBaseView*)(pMDIChild->GetActiveView()))->CanClose() == false
	    ) return;	 */

	  reinterpret_cast<TChildFrame*>(pMDIChild)->UpdateTexture();	  
  
	  pMDIChild = (CMDIChildWnd*)pMDIChild->GetWindow( GW_HWNDPREV );
	} 
 }

void TMainFrame::OnLocalTool()
 {
   CMenu *mnu = GetMenu();
   if( mnu->GetMenuState(ID_VIEW_LOCALTOOL, MF_BYCOMMAND) & MF_CHECKED )
	 m_opt.d.m_bFlLocToolOn = 0,
     mnu->CheckMenuItem( ID_VIEW_LOCALTOOL, MF_BYCOMMAND|MF_UNCHECKED );
   else m_opt.d.m_bFlLocToolOn = 1,
	 mnu->CheckMenuItem( ID_VIEW_LOCALTOOL, MF_BYCOMMAND|MF_CHECKED );    

   RefreshChildLocalTool();
 }

void TMainFrame::RefreshChildLocalTool()
 {
   CMDIChildWnd* pMDIChild = MDIGetActive();
   if( pMDIChild != NULL )
	   pMDIChild = (CMDIChildWnd*)pMDIChild->GetWindow( GW_HWNDLAST );
   while( pMDIChild != NULL )
	{
	  /*if( pMDIChild->GetActiveView() &&
	     ((TBaseView*)(pMDIChild->GetActiveView()))->CanClose() == false
	    ) return;	 */

	  reinterpret_cast<TChildFrame*>(pMDIChild)->UpdateLocToolbar();
	  pMDIChild->RecalcLayout();
  
	  pMDIChild = (CMDIChildWnd*)pMDIChild->GetWindow( GW_HWNDPREV );
	} 
 }

TMCildOpt* TMainFrame::GetOptPtr( LPCSTR lp )
 {
   TFindOptData dta( lp );
   list<TMCildOpt_AP>::iterator it = 
	 find_if( m_lstChOpt.begin(), m_lstChOpt.end(), dta );

   if( it != m_lstChOpt.end() ) return (*it).get();
   return NULL;
 }

LONG TMainFrame::OnGLOpenDoc( UINT wPar, LONG lPar )
 {
   TMCildOpt *pChCfg = reinterpret_cast<TMCildOpt*>( lPar );
   if( pChCfg->m_csPath.IsEmpty() ) return 0;

   try {
     AfxGetApp()->OpenDocumentFile( pChCfg->m_csPath );
	}
   catch( CException *pE )
	{
	  pE->ReportError();
	  pE->Delete();
	}

   return 0;
 }


void TMainFrame::OnSysColorChange() 
 {
	CMDIFrameWnd::OnSysColorChange();

	m_gdiMgr.RecreateTools( true, true, false );

	POSITION ps = AfxGetApp()->GetFirstDocTemplatePosition();
	CDocTemplate *pDocTmpl;
	while( ps )
	 {
	   pDocTmpl = AfxGetApp()->GetNextDocTemplate( ps );

	   POSITION pos = pDocTmpl->GetFirstDocPosition();
	   CDocument *pDoc;
	   while( pos ) 
		{
		  pDoc = pDocTmpl->GetNextDoc( pos );
		  POSITION ps2 = pDoc->GetFirstViewPosition();
		  while( ps2 )
		   {
             CView *pV = pDoc->GetNextView( ps2 );

			 if( typeid(*pV) == typeid(TDiplomView) )
			   reinterpret_cast<TDiplomView*>( pV )->SysClrChange();
		   }
		}
	 }
 }


void TMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CMDIFrameWnd::OnActivate(nState, pWndOther, bMinimized);

	m_bActive = (nState == WA_INACTIVE) ? false:true;
	// TODO: Add your message handler code here
	
}

