// ChildFrm.cpp : implementation of the TChildFrame class
//

#include "stdafx.h"
#include "diplom.h"

#include "ChildFrm.h"
#include "globlmsg.hpp"
#include "mainfrm.h"
#include "diplomview.h"
#include "alndlg.hpp"

#include "getgdimgr.hpp"
#include "dlgprop.hpp"
#include "TBrigDlg.h"
#include "lock_update.hpp"
#include "dlg_scale.hpp"
#include "doc_analyze.hpp"
#include "dlg_sub.hpp"

#include <strstream>
#include <iomanip>

#include "resource.h"

#include <afxext.h>
#include <afxole.h>
#include <afxadv.h>
#include <afxpriv.h>

#include "HLP\\diplom.hh"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CLIPFORMAT TChildFrame::m_cfDraw = NULL;

UINT ThreadProc( LPVOID p );

/////////////////////////////////////////////////////////////////////////////
// TChildFrame


#define ID_LST_BRIG 10

IMPLEMENT_DYNCREATE(TChildFrame, CMDIChildWnd)

IMPLEMENT_DYNCREATE(CViewExSplitWnd1, CSplitterWnd)
IMPLEMENT_DYNCREATE(CViewExSplitWnd2, CSplitterWnd)

IMPLEMENT_DYNCREATE( TListStensil, CListCtrl )

BEGIN_MESSAGE_MAP( TListStensil, CListCtrl )
  ON_WM_CREATE()

  ON_NOTIFY_REFLECT( LVN_BEGINDRAG, OnBeginDrag )
  //ON_NOTIFY_REFLECT( LVN_BEGINRDRAG, OnBeginDrag )
  ON_WM_MOUSEMOVE()
  ON_WM_LBUTTONUP()

  ON_WM_CANCELMODE()

  ON_MESSAGE( WM_HELPHITTEST, OnCommandHelp )
  ON_MESSAGE( WM_COMMANDHELP, OnCommandHelpF1 )
  

END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(TChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(TChildFrame)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_WM_WINDOWPOSCHANGED()
	//}}AFX_MSG_MAP

	ON_REGISTERED_MESSAGE( globl_iMsgQNewPal, OnGlobalQPal )
    ON_REGISTERED_MESSAGE( globl_iMsgPalChanged, OnGlobalPalCh )  	
	ON_REGISTERED_MESSAGE( globl_iMsgEndCalc, OnEndCalc )
	ON_REGISTERED_MESSAGE( globl_iMsgEndRun, OnEndRun )
	ON_REGISTERED_MESSAGE( globl_iMsgNewStatusStr, OnNewStatusStr )
	    	

	ON_UPDATE_COMMAND_UI( ID_VIEW_GRID, OnUpd_ViewGrid )		
	ON_UPDATE_COMMAND_UI( ID_VIEW_GUIDLINES, OnUpd_ViewGuidlines )
	ON_UPDATE_COMMAND_UI( ID_VIEW_CNCTPOINTS, OnUpd_ViewCnctPoints )
	ON_UPDATE_COMMAND_UI( ID_VIEW_LINE, OnUpd_ViewLine )
	ON_UPDATE_COMMAND_UI( ID_VIEW_SIMPLEVIEW, OnUpd_SimpleView_NoCrit )
	ON_UPDATE_COMMAND_UI( ID_VIEW_ARROW, OnUpd_Arrow_NoCrit )
		
	ON_UPDATE_COMMAND_UI( ID_BR_BRIG, OnUpd_ViewBrig )
	ON_UPDATE_COMMAND_UI( ID_BR_ELEMENTS, OnUpd_ViewElements )	
	ON_UPDATE_COMMAND_UI( ID_BR_STEPGRID, OnUpd_StepGrid )
	ON_UPDATE_COMMAND_UI( ID_BR_STEPGUID, OnUpd_StepGuid )

	ON_UPDATE_COMMAND_UI( ID_SC_ALIGMENT, OnUpd_Aligment_NoRun )
	ON_UPDATE_COMMAND_UI( ID_SC_ALNGUID, OnUpd_Forward_NoRun )
	ON_UPDATE_COMMAND_UI( ID_SC_CENTER, OnUpd_Center_NoCrit )
	ON_UPDATE_COMMAND_UI( ID_SC_BACK, OnUpd_Back_NoRun )
	ON_UPDATE_COMMAND_UI( ID_SC_FORWARD, OnUpd_Forward_NoRun )
	ON_UPDATE_COMMAND_UI( ID_SC_FILL_LM, OnUpd_Forward_NoRun )
	ON_UPDATE_COMMAND_UI( ID_SC_CRERNG, OnUpdate_Always_NoRun )

	ON_UPDATE_COMMAND_UI( ID_MODE_BRIG, OnUpd_ModeBrig )	

	ON_UPDATE_COMMAND_UI( ID_CTX_PROPERTY, OnUpd_Property_NoCrit )

	ON_COMMAND( ID_MODE_BRIG, OnModeBrig )	
	ON_COMMAND( ID_VIEW_GRID, On_ViewGrid )	
	ON_COMMAND( ID_VIEW_GUIDLINES, On_ViewGuidlines )	
	ON_COMMAND( ID_VIEW_CNCTPOINTS, On_ViewCnctPoints )	
	ON_COMMAND( ID_VIEW_LINE, On_ViewLine )	
	ON_COMMAND( ID_VIEW_SIMPLEVIEW, On_SimpleView )
	ON_COMMAND( ID_VIEW_ARROW, On_Arrow )

	ON_COMMAND( ID_SC_ALIGMENT, On_Aligment )
	ON_COMMAND( ID_SC_ALNGUID, On_AligmentGuid )
	ON_COMMAND( ID_SC_CENTER, On_Center )
	ON_COMMAND( ID_SC_BACK, On_Back )
	ON_COMMAND( ID_SC_FORWARD, On_Forward )
	ON_COMMAND( ID_SC_FILL_LM, On_FillLM )
	ON_COMMAND( ID_SC_CRERNG, On_CreRng )

	ON_COMMAND( ID_BR_BRIG, On_ViewBrig )	
	ON_COMMAND( ID_BR_ELEMENTS, On_ViewElements )		
	ON_COMMAND( ID_BR_STEPGRID, On_StepGrid )	
	ON_COMMAND( ID_BR_STEPGUID, On_StepGuid )		

	ON_COMMAND( ID_CTX_PROPERTY, On_Property )
	ON_COMMAND( ID_CTX_REPORT, AddReport )


	//------------------------------------
	ON_UPDATE_COMMAND_UI( ICM_ED_FONT, OnUpdate_Always )
	ON_UPDATE_COMMAND_UI( ICM_ED_FNTCLR, OnUpdate_Always )	
	ON_UPDATE_COMMAND_UI( ICM_ED_ALIGN, OnUpdate_Always )		

	ON_UPDATE_COMMAND_UI( ICM_ED_LIN, OnUpdate_Always )
	ON_UPDATE_COMMAND_UI( ICM_ED_LINCLR, OnUpdate_Always )
	ON_UPDATE_COMMAND_UI( ICM_ED_LINTHICK, OnUpdate_Always )
	ON_UPDATE_COMMAND_UI( ICM_ED_LINTYP, OnUpdate_Always )

	ON_UPDATE_COMMAND_UI( ICM_ED_FIL, OnUpdate_Always )
	ON_UPDATE_COMMAND_UI( ICM_ED_FILCLR, OnUpdate_Always )
	ON_UPDATE_COMMAND_UI( ICM_ED_FILTYP, OnUpdate_Always )

	ON_UPDATE_COMMAND_UI( ICM_ED_CALC, OnUpdate_Calc )
	ON_UPDATE_COMMAND_UI( ICM_ED_STOP, OnUpdate_Stop )
	ON_UPDATE_COMMAND_UI( ICM_ED_SINGLESTEP, OnUpdate_Stop )
    ON_UPDATE_COMMAND_UI( ICM_ED_STEPMODE, OnUpdate_Stop )
	ON_UPDATE_COMMAND_UI( ID_CLC_EQREST, OnUpdate_EqRest )
	ON_UPDATE_COMMAND_UI( ID_CLC_CONNECT,  OnUpdate_Connect )
	ON_UPDATE_COMMAND_UI( ID_CLC_CTYPE,  OnUpdate_ClcType )
	ON_UPDATE_COMMAND_UI( ID_CLC_CINT,  OnUpdate_ClcType )
	ON_UPDATE_COMMAND_UI( ID_ED_SELECT_HIGH, OnUpdate_Always_NoCrit )
	ON_UPDATE_COMMAND_UI( ID_CLC_NPATHS, OnUpdate_ClcType )
    ON_UPDATE_COMMAND_UI( ID_CLC_TSIMPL, OnUpdate_TSimpl )
	ON_UPDATE_COMMAND_UI( ID_CLC_TSOFIST, OnUpdate_TSimpl )
	ON_UPDATE_COMMAND_UI( ID_CLC_RUN, OnUpdate_ClcRun )
	ON_UPDATE_COMMAND_UI( ID_CLC_ARNG, OnUpdate_ClcRun )

	ON_COMMAND( ID_CLC_RUN, OnClcRun )
	ON_COMMAND( ID_CLC_NPATHS, OnNPaths )
	ON_COMMAND( ID_CLC_TSIMPL, OnTSimplify )
	ON_COMMAND( ID_CLC_TSOFIST, OnSofistSimplify )
	ON_COMMAND( ID_CLC_ARNG, OnARNG )
	ON_COMMAND( ID_ED_SELECT_HIGH, OnSelectHigh )
	ON_COMMAND( ICM_ED_FONT, OnFont )
	ON_COMMAND( ICM_ED_FNTCLR, OnFntClr )
	ON_COMMAND( ICM_ED_ALIGN, OnAlign )
	ON_COMMAND( ID_CLC_CTYPE, OnClcType )
	ON_COMMAND( ID_CLC_CINT, OnClcInt )

	ON_COMMAND( ICM_ED_LIN, OnLine )
	ON_COMMAND( ICM_ED_LINCLR, OnLinClr )
	ON_COMMAND( ICM_ED_LINTHICK, OnLinThick )
	ON_COMMAND( ICM_ED_LINTYP, OnLinTyp )

	ON_COMMAND( ICM_ED_FIL, OnFil )
	ON_COMMAND( ICM_ED_FILCLR, OnFilClr )
	ON_COMMAND( ICM_ED_FILTYP, OnFilTyp )

	ON_COMMAND( ICM_ED_CALC, OnCalc )
	ON_COMMAND( ICM_ED_STOP, OnStop )
	ON_COMMAND( ID_CLC_EQREST, On_EqRest )
	ON_COMMAND( ID_CLC_CONNECT, On_Connect )
	ON_COMMAND( ICM_ED_SINGLESTEP, On_SinglStep )
    ON_COMMAND( ICM_ED_STEPMODE, On_StepMode )

	//------------------------
	ON_UPDATE_COMMAND_UI( ID_LOC_ADD, OnUpdate_LocAdd )
	ON_UPDATE_COMMAND_UI( ID_LOC_REM, OnUpdate_LocRemove )
	ON_UPDATE_COMMAND_UI( ID_LOC_ED, OnUpdate_LocEdit )	
	ON_UPDATE_COMMAND_UI( ID_LOC_ACCEPT, OnUpdate_LocAccept )		

	ON_COMMAND( ID_LOC_ADD, OnLocAdd )
	ON_COMMAND( ID_LOC_REM, OnLocRem )
	ON_COMMAND( ID_LOC_ED, OnLocEd )
	ON_COMMAND( ID_LOC_ACCEPT, OnLocAccept )

	ON_CBN_SELENDOK( ID_CBN_SCALE, OnCBNSCale )    	
	ON_COMMAND( 1, OnCBNScale_ENTER )

	//-----------------------    
    ON_UPDATE_COMMAND_UI( ID_VIEW_PRIVSCALE, OnUpdate_Always )
    ON_UPDATE_COMMAND_UI( ID_VIEW_100, OnUpdate_Always )      
    ON_UPDATE_COMMAND_UI( ID_VIEW_PAGEW, OnUpdate_Always )    
    ON_UPDATE_COMMAND_UI( ID_VIEW_PAGE, OnUpdate_Always )     
    ON_UPDATE_COMMAND_UI( ID_VIEW_200, OnUpdate_Always )      
    ON_UPDATE_COMMAND_UI( ID_VIEW_50, OnUpdate_Always )       
	
	ON_COMMAND( ID_VIEW_PRIVSCALE, OnView_PrivScale )
    ON_COMMAND( ID_VIEW_100, OnView_Scale100 )      
    ON_COMMAND( ID_VIEW_PAGEW, OnView_Pagew )    
    ON_COMMAND( ID_VIEW_PAGE, OnView_Page )     
    ON_COMMAND( ID_VIEW_200, OnView_200 )      
    ON_COMMAND( ID_VIEW_50, OnView_50 )       

	//-----------------------    
	ON_UPDATE_COMMAND_UI( ID_EDIT_DUP, OnUpdate_HaveSel_NoRun )
	ON_UPDATE_COMMAND_UI( ID_EDIT_SELECT_ALL, OnUpdate_SelAll_NoCrit )
	ON_UPDATE_COMMAND_UI( ID_ED_SELECTPC, OnUpdate_SelAll_NoCrit )
	ON_UPDATE_COMMAND_UI( ID_ED_SELECTCNCT, OnUpdate_SelAll_NoCrit )
    ON_UPDATE_COMMAND_UI( ID_EDIT_UNSELECTALL, OnUpdate_HaveSel_NoCrit )             
    ON_UPDATE_COMMAND_UI( ID_EDIT_INV, OnUpdate_HaveSel_NoCrit )                     
	ON_UPDATE_COMMAND_UI( ID_EDIT_COPY, OnUpdate_HaveSel_NoCrit )
	ON_UPDATE_COMMAND_UI( ID_EDIT_CLEAR_ALL, OnUpdate_HaveSel_NoRun )
	ON_UPDATE_COMMAND_UI( ID_EDIT_CLEAR, OnUpdate_HaveSel_NoRun )
	ON_UPDATE_COMMAND_UI( ID_EDIT_CUT, OnUpdate_HaveSel_NoRun )
	ON_UPDATE_COMMAND_UI( ID_EDIT_PASTE, OnUpdate_Paste_NoRun )
	ON_UPDATE_COMMAND_UI( ID_EDIT_SCALE, OnUpdate_HaveSel_NoCrit )
	ON_UPDATE_COMMAND_UI( ID_ED_SELDBL, OnUpdate_HaveSel_NoRun )
	ON_UPDATE_COMMAND_UI( ID_ED_SELDBL_MAX, OnUpdate_HaveSel_NoRun )
	ON_UPDATE_COMMAND_UI( ID_ED_UNHILITE, OnUpdate_Always_NoCrit )

	ON_UPDATE_COMMAND_UI( ID_ED_SEL_SUBSCH, OnUpdate_Always_NoRun )
	ON_UPDATE_COMMAND_UI( ID_ED_SELECTPC_DP, OnUpdate_HaveSel_NoRun )
	ON_UPDATE_COMMAND_UI( ID_ED_SELECTPC_HIGH, OnUpdate_SelAll_NoCrit )
	ON_UPDATE_COMMAND_UI( ID_ED_SEL_CDP, OnUpdate_HaveSel_NoRun )
	ON_UPDATE_COMMAND_UI( ID_ED_SEL_HIGH, OnUpdate_SelAll_NoCrit )

	ON_COMMAND( ID_ED_SEL_SUBSCH, On_SelSubch )
	ON_COMMAND( ID_ED_SELECTPC_DP, On_SelPCDp )
	ON_COMMAND( ID_ED_SELECTPC_HIGH, On_SelPCHigh )
	ON_COMMAND( ID_ED_SEL_CDP, On_SelCnctDP )
	ON_COMMAND( ID_ED_SEL_HIGH, On_SelCnctHigh )
	ON_COMMAND( ID_ED_SELDBL, On_Edit_SelDBL )
    ON_COMMAND( ID_ED_SELDBL_MAX, On_Edit_SelDBL_Max )

	ON_COMMAND( ID_EDIT_DUP, On_Edit_Dup )
    ON_COMMAND( ID_EDIT_UNSELECTALL, On_Edit_UnselAll )             
	ON_COMMAND( ID_EDIT_SELECT_ALL, On_Edit_SelAll )             
    ON_COMMAND( ID_EDIT_INV, On_Edit_SelInv )                     
	ON_COMMAND( ID_EDIT_COPY, On_Edit_Copy )
	ON_COMMAND( ID_EDIT_CLEAR_ALL, On_Edit_ClearAll )
	ON_COMMAND( ID_EDIT_CLEAR, On_Edit_Clear )
	ON_COMMAND( ID_EDIT_CUT, On_Edit_Cut )
	ON_COMMAND( ID_EDIT_PASTE, On_Edit_Paste )
	ON_COMMAND( ID_EDIT_SCALE, On_Edit_Scale )
	

	ON_COMMAND( ID_ED_SELECTPC, On_Edit_SelPC )
	ON_COMMAND( ID_ED_SELECTCNCT, On_Edit_SelCnct )

	ON_COMMAND( ID_ED_UNHILITE, On_Edit_Unhilite )

	//--------------------------
	ON_UPDATE_COMMAND_UI( ID_BR_SERVIS, OnUpdate_Servis )
	ON_UPDATE_COMMAND_UI( ID_BR_UNSERVIS, OnUpdate_UnServis )
	ON_UPDATE_COMMAND_UI( ID_BR_LABEL, OnUpdate_Label )
	
	ON_COMMAND( ID_BR_SERVIS, On_Servis )
	ON_COMMAND( ID_BR_UNSERVIS, On_UnServis )
	ON_COMMAND( ID_BR_LABEL, On_Label )

	//--------------------------
	
	ON_UPDATE_COMMAND_UI( ID_FILE_SAVE, OnUpd_DisableAtRun )
	ON_UPDATE_COMMAND_UI( ID_FILE_SAVE_AS, OnUpd_DisableAtRun )
	ON_UPDATE_COMMAND_UI( ID_FILE_REVERT, OnUpd_DisableAtRun )
	ON_UPDATE_COMMAND_UI( ID_FILE_PRINT, OnUpd_DisableAtRun )
	ON_UPDATE_COMMAND_UI( ID_FILE_PRINT_SETUP, OnUpd_DisableAtRun )
	ON_UPDATE_COMMAND_UI( ID_FILE_PRINT_PREVIEW, OnUpd_DisableAtRun )	
	ON_UPDATE_COMMAND_UI( ID_FILE_EXPORT, OnUpd_DisableAtRun )

	ON_COMMAND( ID_FILE_REVERT, On_Revert )
	ON_COMMAND( ID_FILE_EXPORT, On_Export )

	ON_NOTIFY( NM_DBLCLK, ID_LST_BRIG, OnDblClk_List )
	ON_NOTIFY( NM_RETURN, ID_LST_BRIG, OnEnter_List )
	ON_NOTIFY( LVN_KEYDOWN, ID_LST_BRIG, OnKeyDown_List )
	ON_NOTIFY( LVN_ITEMCHANGED, ID_LST_BRIG, OnItemChg_List )

  	
END_MESSAGE_MAP()


BEGIN_MESSAGE_MAP( CMyDialogBar, CDialogBar )
  ON_WM_ERASEBKGND()
  ON_WM_SIZE()
  ON_WM_CREATE()

  //ON_MESSAGE( WM_HELPHITTEST, OnCommandHelp )

  ON_WM_DESTROY()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// TChildFrame construction/destruction

TChildFrame::TChildFrame():
  m_dbarHor( true ),
  m_ptAlnDlgCorn( SHRT_MIN, SHRT_MIN ),
  m_ptAlnProp_DlgCorn( SHRT_MIN, SHRT_MIN ),
  m_ptBrigDlg( SHRT_MIN, SHRT_MIN ),
  m_ptScale( SHRT_MIN, SHRT_MIN ),
  m_evCalcActive( FALSE, TRUE ),
  m_evCalcStop( FALSE, TRUE ),
  m_evCalcNotActive( TRUE, TRUE ),
  m_evSingleStep( FALSE, FALSE ),
  m_evSingleStepMode( FALSE, TRUE ),
  m_evDataAccEnable( TRUE, FALSE ),
  m_mtxMsg( FALSE ),
  m_apAnalyzer( NULL ),
  m_apCD( NULL ),
  strMsgSimpl( new char[1024] )
 {
	// TODO: add member initialization code here
  m_bFlInit = false;	
  m_bFirstShow = true;
  m_pMainView = NULL;  

  m_bMax_KEY = false;

  m_bFlCalc = true;
  m_bFlRun = false;
  
  if( TChildFrame::m_cfDraw == NULL )
	TChildFrame::m_cfDraw = (CLIPFORMAT)::RegisterClipboardFormat( _T("AS Clip") );

  m_apLine_Pattern = Line_EnumPattern();
  m_apLine_Thick = Line_EnumThick();
  m_apFill_Pattern = Fill_EnumPattern();
  m_ap_Clr = Common_EnumColor();

  m_bEnblBrigEdit = true;

  m_bAD_key = 0;
  m_iAln_H_key = 1;
  m_iAln_V_key = 3;
  m_iDistr_HV_key = 0;
 }

TChildFrame::~TChildFrame()
 {
 }

BOOL TChildFrame::DestroyWindow()
 {
   auto_ptr<CSingleLock> apLock( new CSingleLock(&m_evCalcNotActive) );
   m_evCalcStop.SetEvent();
   apLock->Lock();   
      
   delete apLock.release();

   m_dbarHor.DestroyWindow();   

   return CMDIChildWnd::DestroyWindow();
 }


BOOL TChildFrame::PreCreateWindow(CREATESTRUCT& cs)
 {
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

   BOOL bFl = CMDIChildWnd::PreCreateWindow( cs );
   if( !bFl ) return FALSE;

   //cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
   /*static CString str;
   str =
	AfxRegisterWndClass( CS_VREDRAW, 
	  LoadCursor(NULL, IDC_ARROW), 
	  (HBRUSH)(COLOR_WINDOW + 1)); 

   cs.lpszClass = (LPCSTR)str;*/

   return TRUE;
 }

BOOL CViewExSplitWnd1::PreCreateWindow( CREATESTRUCT& cs )
 {
   BOOL bFl = CSplitterWnd::PreCreateWindow( cs );
   if( !bFl ) return FALSE;

   //cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
   /*static CString str;
   str =
	AfxRegisterWndClass( CS_VREDRAW, 
	  LoadCursor(NULL, IDC_ARROW), 
	  (HBRUSH)(COLOR_WINDOW + 1)); 

   cs.lpszClass = (LPCSTR)str;*/

   return TRUE;
 }
BOOL CViewExSplitWnd2::PreCreateWindow( CREATESTRUCT& cs )
 {
   BOOL bFl = CSplitterWnd::PreCreateWindow( cs );
   if( !bFl ) return FALSE;

   //cs.style |= WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
   /*static CString str;
   str =
	AfxRegisterWndClass( CS_VREDRAW, 
	  LoadCursor(NULL, IDC_ARROW), 
	  (HBRUSH)(COLOR_WINDOW + 1)); 

   cs.lpszClass = (LPCSTR)str;*/

   return TRUE;
 }
/////////////////////////////////////////////////////////////////////////////
// TChildFrame diagnostics

#ifdef _DEBUG
void TChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void TChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// TChildFrame message handlers

void TChildFrame::UpdateTexture()
 {
   if( !m_dbarHor.GetSafeHwnd() ) return;
   m_dbarHor.InvalidateRect( NULL );
 }

void TChildFrame::UpdateLocToolbar()
 {   
   if( !m_dbarHor.GetSafeHwnd() ) return;

   m_dbarHor.ModifyStyle( 
	!static_cast<TMainFrame*>(AfxGetApp()->m_pMainWnd)->GetOpt().d.m_bFlLocToolOn ? WS_VISIBLE:0,
	static_cast<TMainFrame*>(AfxGetApp()->m_pMainWnd)->GetOpt().d.m_bFlLocToolOn ? WS_VISIBLE:0,
	0 );
 }

auto_ptr<TMCildOpt> TChildFrame::GetCHCfg()
 {
   GetWindowPlacement( &m_opt.m_wndPlac );
   CDocument *pDoc = GetActiveDocument();
   if( pDoc )
     m_opt.m_csPath = pDoc->GetPathName();
   auto_ptr<TMCildOpt> opt( new TMCildOpt(m_opt) );   
   return opt;
 }

void TChildFrame::SetCHCfg( TMCildOpt* pOpt )
 {
   m_opt = *pOpt;
 }

int TChildFrame::OnCreate(LPCREATESTRUCT lpC) 
{
	if( CMDIChildWnd::OnCreate(lpC) == -1 )
	  return -1;	 
         
   if( !m_dbarHor.Create(this, IDD_DLG_DLGBARH, CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY,
	                     IDD_DLG_DLGBARH) )
	{
	  MessageBox( "Не удалось создать горизонтальный DialogBar", "Ошибка", MB_OK|MB_ICONEXCLAMATION );
      return -1;
	}

   DWORD dwS = m_dbarHor.GetBarStyle();
   m_dbarHor.SetBarStyle( (dwS & ~CBRS_BORDER_BOTTOM)|CBRS_BORDER_LEFT|CBRS_BORDER_RIGHT );
   

   BOOL bFlag;
   OnGlobalQPal( 0, (LONG)&bFlag );

   ChangeTitle();
   UpdateLocToolbar();   

   m_pMainView = dynamic_cast<TDiplomView*>( m_wndSplitter1.GetPane(0, 0) );
	if( !m_pMainView )
	  m_pMainView = dynamic_cast<TDiplomView*>( m_wndSplitter1.GetPane(0, 1) );
   
   //UpdateWindow();

   return 0;
}


LONG TChildFrame::OnGlobalPalCh( UINT lPtrRes, LONG lPtr )
 {
   if( (CWnd*)lPtr != this ) 
	{
	  OnGlobalQPal( 0, 0 );
	  *(BOOL*)lPtrRes = TRUE;
	}
   return 0;
 }

LONG TChildFrame::OnGlobalQPal( UINT, LONG lPtr )
 {
   LONG r1 = m_dbarHor.QPalette( 0 );
   LONG r2;
   //if( m_dbarVer.GetSafeHwnd() ) r2 = m_dbarVer.QPalette( r1 );
   //else r2 = 0;
       

   if( lPtr )
     *(BOOL*)lPtr = max( *(BOOL*)lPtr, max(r1, r2) );

   return 0;   
 }

void TChildFrame::ChangeTitle()
 {
   /*TBaseView *pView = (TBaseView*)GetActiveView();
   if( !pView ) return;
   CString str;      
   str.Format( "%s - \"%s\"", pView->GetShortFileName(), pView->GetName() );

   if( pView->GetDocument() )
     pView->GetDocument()->SetTitle( str );
   else
     SetWindowText( str );*/
 }


BOOL TChildFrame::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CMDIChildWnd::OnEraseBkgnd(pDC);
}


BOOL CMyDialogBar::PreCreateWindow( CREATESTRUCT& cs )
 {
   BOOL bRes = CDialogBar::PreCreateWindow( cs );
   if( !bRes ) return bRes;

   cs.style |= WS_CLIPCHILDREN;
   return TRUE;
 }

void CMyDialogBar::OnDestroy()
 {
   if( m_bFlHoriz == true )
	{
      if( CMyDialogBar::m_iUseCnt > 1 ) CMyDialogBar::m_iUseCnt--;
      else
	   {
         CMyDialogBar::m_iUseCnt = 0;
	     ::GlobalFree( CMyDialogBar::m_hrcBmp );
	     CMyDialogBar::m_hrcBmp = NULL;
	     CMyDialogBar::m_palPal.DeleteObject();
	   }
	}
   else
	{
      if( CMyDialogBar::m_iUseCntv > 1 ) CMyDialogBar::m_iUseCntv--;
      else
	   {
         CMyDialogBar::m_iUseCntv = 0;
	     ::GlobalFree( CMyDialogBar::m_hrcBmpv );
	     CMyDialogBar::m_hrcBmpv = NULL;
	     CMyDialogBar::m_palPalv.DeleteObject();
	   }
	}   

   CDialogBar::OnDestroy();
 }

int CMyDialogBar::OnToolHitTest( CPoint point, TOOLINFO* pTI ) const
 {
   list<TToolTipData *>::iterator itSta( m_vecToolTips.begin() );
   list<TToolTipData *>::iterator itEnd( m_vecToolTips.end() );

   TToolTipData *pDta = NULL;
   CRect r;
   HWND hw;
   for( ; itEnd != itSta; ++itSta )
	{
	  hw = ::GetDlgItem( m_hWnd, (*itSta)->m_uiId );
	  if( !hw ) continue;
	  ::GetWindowRect( hw, &r );
	  ScreenToClient( &r );
	  if( r.PtInRect(point) == TRUE ) 
	   {
         pDta = *itSta;
		 break;
	   }
	}   

   if( !pDta ) return -1;

   //pTI->uFlags = TTF_IDISHWND | TTF_SUBCLASS;
   if( pTI )
	{
      pTI->uFlags = TTF_IDISHWND;
      pTI->hwnd = m_hWnd;
      pTI->uId = (UINT)hw;
      pTI->rect = r;
      //pTI->hinst = AfxGetResourceHandle();
      pTI->hinst = 0;
      //pTI->lpszText = MAKEINTRESOURCE( m_uiCommand );
      pTI->lpszText = _strdup( (LPCSTR)(pDta->m_csTxt) );
	}
   else return ::GetDlgCtrlID( hw );

   return 1;
 }

CMyDialogBar::~CMyDialogBar()
 {
   ClearBtns();   
 }

void CMyDialogBar::ClearBtns()
 {
   m_imaxBounds = 50;

   list<TToolTipData *>::iterator itSta( m_vecToolTips.begin() );
   list<TToolTipData *>::iterator itEnd( m_vecToolTips.end() );

   for( ; itEnd != itSta; ++itSta )
     if( *itSta ) delete *itSta;
	  
   m_vecToolTips.clear();


   list<TBtnItem*>::iterator itSta2( m_lstBtn.begin() );
   list<TBtnItem*>::iterator itEnd2( m_lstBtn.end() );

   for( ; itEnd2 != itSta2; ++itSta2 )
     if( *itSta2 ) 
	  {
	    //delete (*itSta2)->m_pBtn; - not owned button
	    delete *itSta2;
	  }
	  
   m_lstBtn.clear();   
 }

int CMyDialogBar::OnCreate( LPCREATESTRUCT lpC )
 {
   if( CDialogBar::OnCreate(lpC) == -1 ) return -1;

   if( m_bFlHoriz == true && !CMyDialogBar::m_iUseCnt )
	{ 
      CRcFile frc( AfxGetResourceHandle(), IDB_DIB_BKGNDH, "BITMAP256" );
      CMyDialogBar::m_hrcBmp = ReadDIBFile( frc );
	  CreateDIBPalette( (HDIB)CMyDialogBar::m_hrcBmp, &m_palPal );

	  LPVOID lpDIB = ::GlobalLock( CMyDialogBar::m_hrcBmp );
	  m_iW = DIBWidth( (LPSTR)lpDIB );
      m_iH = DIBHeight( (LPSTR)lpDIB );
      ::GlobalUnlock( CMyDialogBar::m_hrcBmp );	  

	  m_rS = CRect( 0, 0, CMyDialogBar::m_iW, CMyDialogBar::m_iH );
	}

   if( m_bFlHoriz == false && !CMyDialogBar::m_iUseCntv )
	{ 
      CRcFile frc( AfxGetResourceHandle(), IDB_DIB_BKGNDV, "BITMAP256" );
      CMyDialogBar::m_hrcBmpv = ReadDIBFile( frc );
	  CreateDIBPalette( (HDIB)CMyDialogBar::m_hrcBmpv, &m_palPalv );

	  LPVOID lpDIB = ::GlobalLock( CMyDialogBar::m_hrcBmpv );
	  m_iWv = DIBWidth( (LPSTR)lpDIB );
      m_iHv = DIBHeight( (LPSTR)lpDIB );
      ::GlobalUnlock( CMyDialogBar::m_hrcBmpv );	  

	  m_rSv = CRect( 0, 0, CMyDialogBar::m_iWv, CMyDialogBar::m_iHv );
	}
	
   if( m_bFlHoriz == true ) CMyDialogBar::m_iUseCnt++;	   
   else CMyDialogBar::m_iUseCntv++;	   

   EnableToolTips( TRUE );
   
   //BOOL b = m_brBkGnd.CreateDIBPatternBrush( CMyDialogBar::m_hrcBmp, DIB_RGB_COLORS );

   return 0;
 }

bool CMyDialogBar::TextureIsOn() const
 {
   return ((TMainFrame*)(AfxGetApp()->m_pMainWnd))->GetOpt().d.m_bFlFactureOn;
   //return true;
 }

BOOL CMyDialogBar::OnEraseBkgnd( CDC *pDC )
 {      
   if( TextureIsOn() == false ) return CDialogBar::OnEraseBkgnd( pDC );

   CRect r; GetClientRect( r );     

   CPalette *pOld;

   if( m_bFlHoriz == true )
	{
      pOld = pDC->SelectPalette( &CMyDialogBar::m_palPal, FALSE );

      int iY = 0;
      for( ; iY < r.bottom; iY += CMyDialogBar::m_iH )
	   {
         int iX = 0;
         for( ; iX < r.right; iX += CMyDialogBar::m_iW )
		  {
	        CRect r2( iX, iY, iX + CMyDialogBar::m_iW, iY + CMyDialogBar::m_iH );
	        PaintDIB( pDC->m_hDC, r2, CMyDialogBar::m_hrcBmp, CMyDialogBar::m_rS, 
		              &CMyDialogBar::m_palPal );
		  }
	   }
	}
   else
	{
      pOld = pDC->SelectPalette( &CMyDialogBar::m_palPalv, FALSE );

	  int iY = 0;
      for( ; iY < r.bottom; iY += CMyDialogBar::m_iHv )
	   {
         int iX = 0;
         for( ; iX < r.right; iX += CMyDialogBar::m_iWv )
		  {
	        CRect r2( iX, iY, iX + CMyDialogBar::m_iWv, iY + CMyDialogBar::m_iHv );
	        PaintDIB( pDC->m_hDC, r2, CMyDialogBar::m_hrcBmpv, CMyDialogBar::m_rSv, 
		              &CMyDialogBar::m_palPalv );
		  }
	   }
	}
   //pDC->FillRect( r, &m_brBkGnd );

   pDC->SelectPalette( pOld, FALSE );

   return TRUE;
 }



CMyDialogBar& CMyDialogBar::operator+( TBtnItem *pBtnIt )
 {
   if( m_lstBtn.size() )
	 pBtnIt->m_pPrivBtn = (*--m_lstBtn.end())->m_pBtn;

   m_lstBtn.push_back( pBtnIt );

   pBtnIt->m_pBtn->GetWindowRect( pBtnIt->r );
   ScreenToClient( pBtnIt->r );      

   pBtnIt->m_iW = pBtnIt->r.Width();
   pBtnIt->m_iH = pBtnIt->r.Height();
   
   return *this;
 }

void CMyDialogBar::InsertHead( TBtnItem &rBtnIt )
 {
   if( m_lstBtn.size() ) (*m_lstBtn.begin())->m_pPrivBtn = rBtnIt.m_pBtn;

   m_lstBtn.push_front( &rBtnIt );

   rBtnIt.m_pBtn->GetWindowRect( rBtnIt.r );
   ScreenToClient( rBtnIt.r );      

   rBtnIt.m_iW = rBtnIt.r.Width();
   rBtnIt.m_iH = rBtnIt.r.Height();
 }

CSize CMyDialogBar::CalcFixedLayout( BOOL bStretch, BOOL bHorz )
 {
   CSize sz = CDialogBar::CalcFixedLayout( bStretch, bHorz );

   if( bHorz )	
     sz.cy = m_imaxBounds + 4*GetSystemMetrics( SM_CYDLGFRAME );
   else	
	 sz.cx = m_imaxBounds + 4*GetSystemMetrics( SM_CXDLGFRAME );

   return sz;
 }
//m_imaxBounds = 50;

void CMyDialogBar::MyLayout( bool bRedr, bool bFlCallDirectly, int iBnd )
 {
   int iDst = 
	 MUL_DSTBUTTONS * GetSystemMetrics( m_bFlHoriz == true ? SM_CXDLGFRAME:SM_CYDLGFRAME );
   int iCurX = 2 * iDst; 
   CRect r; GetClientRect( r );
 
   if( bFlCallDirectly == true )
	{
      if( IsHoris() == true ) r.bottom = iBnd;
	  else r.right = iBnd;
	}

   int iFixedSize = 2 * iCurX;
   list<TBtnItem*>::iterator itSta2( m_lstBtn.begin() );
   list<TBtnItem*>::iterator itEnd2( m_lstBtn.end() );

   int iCntVar = 0;
   bool bFlSta = true;
   for( ; itEnd2 != itSta2; ++itSta2 )	 
	{
      (*itSta2)->m_pBtn->GetWindowRect( (*itSta2)->r );
	  ScreenToClient( (*itSta2)->r );

      iFixedSize += m_bFlHoriz == true ? (*itSta2)->m_iW:(*itSta2)->m_iH;

	  if( bFlSta == true )
	   {
         bFlSta = false; continue;
	   }
	  if( (*itSta2)->m_bFixedDst == true ) iFixedSize += iDst;
	  else iCntVar++;
	}

   int iVarDst;
   if( m_bFlHoriz == true )
	  iVarDst = iCntVar ? (r.Width() - iFixedSize) / iCntVar:0;
   else
	  iVarDst = iCntVar ? (r.Height() - iFixedSize) / iCntVar:0;

   if( iVarDst < iCurX ) iVarDst = iCurX;

   itSta2 = m_lstBtn.begin();
   itEnd2 = m_lstBtn.end();
   bFlSta = true;
   CRect rP; GetClientRect( rP ); 
   int iPH = m_bFlHoriz == true  ? rP.Height():rP.Width();
   for( ; itEnd2 != itSta2; ++itSta2 )	 
	{
      if( bFlSta == false ) 
	    iCurX += (*itSta2)->m_bFixedDst == true ? iDst:iVarDst;
	  else bFlSta = false;

	  CRect rKey( (*itSta2)->r );
	  if( m_bFlHoriz == true )
	   {
         (*itSta2)->r.left = iCurX;
	     (*itSta2)->r.right = (*itSta2)->r.left + (*itSta2)->m_iW;

		 if( rKey != (*itSta2)->r )
	       (*itSta2)->m_pBtn->SetWindowPos( NULL, (*itSta2)->r.left,
	       (iPH - (*itSta2)->m_iH + 1) / 2, 0, 0, 
		   SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE|(bRedr ? 0:SWP_NOREDRAW) );
		 

	     iCurX += (*itSta2)->m_iW;
	   }
	  else
	   {
         (*itSta2)->r.top = iCurX;
	     (*itSta2)->r.bottom = (*itSta2)->r.top + (*itSta2)->m_iH;

		 if( rKey != (*itSta2)->r )
	       (*itSta2)->m_pBtn->SetWindowPos( NULL, (iPH - (*itSta2)->m_iW + 1) / 2,
	       (*itSta2)->r.top, 0, 0, 
		   SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE|(bRedr ? 0:SWP_NOREDRAW) );

	      iCurX += (*itSta2)->m_iH;
	   }	  
	}
 }

void CMyDialogBar::OnSize( UINT nType, int cx, int cy )
 {
   CDialogBar::OnSize( nType, cx, cy );

   if( nType == SIZE_MINIMIZED ) return;

   MyLayout( nType == SIZE_RESTORED );   
 }

LONG CMyDialogBar::QPalette( LONG )
 {
	HPALETTE HpalOld;
	int i;
	HPALETTE hp = m_bFlHoriz == true ? CMyDialogBar::m_palPal:CMyDialogBar::m_palPalv;
					  //TPalette
	if( !hp ) return 0;

	HDC hdc = ::GetDC( m_hWnd );
	HpalOld = ::SelectPalette( hdc, hp, FALSE );
	
	i = ::RealizePalette( hdc );

	::SelectPalette( hdc, HpalOld, FALSE );

	if( i > 0 )
	  InvalidateRect( NULL, TRUE );	

	::ReleaseDC( m_hWnd, hdc );

	return i;
 }

int CMyDialogBar::m_iW = 0,  CMyDialogBar::m_iH = 0;
CPalette CMyDialogBar::m_palPal;
HDIB CMyDialogBar::m_hrcBmp = NULL;
int CMyDialogBar::m_iUseCnt = 0;
CRect CMyDialogBar::m_rS;

int CMyDialogBar::m_iWv = 0,  CMyDialogBar::m_iHv = 0;
CPalette CMyDialogBar::m_palPalv;
HDIB CMyDialogBar::m_hrcBmpv = NULL;
int CMyDialogBar::m_iUseCntv = 0;
CRect CMyDialogBar::m_rSv;


CRcFile::CRcFile( HINSTANCE hinst, UINT uiIdRc, LPCSTR lpName ): CFile()
   {
     m_hHan = hinst;
	 m_hrc = NULL;
	 m_hrc = FindResource( hinst, MAKEINTRESOURCE(uiIdRc), lpName );
     if( !m_hrc ) AfxThrowResourceException();

	 m_hg = NULL;
	 m_hg =  LoadResource( m_hHan, m_hrc );	
     if( !m_hg ) AfxThrowResourceException();

	 m_lpCuros = m_lpRc = (LPSTR)LockResource( m_hg );
     m_dwSizeRc = SizeofResource( m_hHan, m_hrc );
	 
	 m_dwCnt = 0;
	 m_bEOF = false;
   }


UINT CRcFile::Read( void* lpBuf, UINT nCount )
 {
   if( m_bEOF ) return 0;   

   DWORD dwRead = (m_dwCnt + nCount >= m_dwSizeRc) ? 
	 m_dwSizeRc-m_dwCnt:nCount;

   if( dwRead < nCount ) m_bEOF = true;

   memcpy( lpBuf, m_lpCuros, dwRead );
   m_lpCuros += dwRead;
   m_dwCnt += dwRead;

   return dwRead;
 }

DWORD CRcFile::GetLength() const
 {
   return m_dwSizeRc;
 }

CRcFile::~CRcFile()
 {
   ::FreeResource( m_hrc );
 }

void TChildFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CMDIChildWnd::OnClose();
}

void TChildFrame::ShowWindow_Init( BOOL bShow, UINT nStatus )
 {
   if( bShow && m_bFirstShow )
	{
      m_bFirstShow = false;

      TDiplomDoc *pDoc = (TDiplomDoc*)GetActiveDocument();

	  TMCildOpt *pChOpt = 
		static_cast<TMainFrame*>(AfxGetApp()->m_pMainWnd)->GetOptPtr( 
		  pDoc->GetPathName() );
	  if( pChOpt )
	   {
		 m_opt = *pChOpt;
		 m_bFlInit = true;
	   }

	   //if( m_bFlInit == true )
		 //SetWindowPlacement( &m_opt.m_wndPlac );
	  if( pDoc )
	   {
		 WINDOWPLACEMENT  wndPlac; memset( &wndPlac, 0, sizeof(wndPlac) );
		 if( memcmp(&wndPlac, &pDoc->m_wndPlac, sizeof(wndPlac)) )
		  {
		    SetWindowPlacement( &pDoc->m_wndPlac );
			pDoc->GetMainViev()->SetPageSize( pDoc->GetSize(), false );
			if( pDoc->m_pLoadScroll.x  && pDoc->m_pLoadScroll.y )
			  pDoc->GetMainViev()->ScrollToPosition( pDoc->m_pLoadScroll );			
		  }
		 else if( m_bFlInit == true )
		   SetWindowPlacement( &m_opt.m_wndPlac );
	   }



	   m_wndSplitter1.m_dSplit = m_optX.m_dSplit1;

	   if( m_optX.m_bFlBrig ||
		   m_optX.m_bFlElem
		 )   
		{
		  CRect r; m_wndSplitter1.GetClientRect( r );	  
		  m_wndSplitter1.SplitColumn( r.Width()*m_optX.m_dSplit1 );
		}
	}
 }

void TChildFrame::OnShowWindow(BOOL bShow, UINT nStatus) 
 {
   CMDIChildWnd::OnShowWindow(bShow, nStatus);

   UpdateWindow();   	
	// TODO: Add your message handler code here
 }




CViewExSplitWnd1::CViewExSplitWnd1()
 {
   m_dSplit = 1.0/3.0;
   m_dLeftMax = 2.0/3.0;
   m_iLeftMin = 80;
   m_iRightMin = 1.7*GetSystemMetrics( SM_CXMIN );
 }

CViewExSplitWnd1::~CViewExSplitWnd1()
 {
 } 

void CViewExSplitWnd1::ActivateNext( BOOL bPrev )
 {
   //CSplitterWnd::ActivateNext( bPrev );
   if( m_wndSplitter2.get() ) 
	m_wndSplitter2->SetActivePane( 0, 0 );
 }

CWnd* CViewExSplitWnd1::GetActivePane( int* pRow, int* pCol )
 {
   ASSERT_VALID(this);

   // attempt to use active view of frame window
   CWnd* pView = NULL;
   CFrameWnd* pFrameWnd = GetParentFrame();
   ASSERT_VALID(pFrameWnd);
   pView = pFrameWnd->GetActiveView();

   // failing that, use the current focus
   if (pView == NULL)
	   pView = GetFocus();

   return pView;
 }



void TChildFrame::GetCtx( CCreateContext& rC )
 {
   rC.m_pNewViewClass = RUNTIME_CLASS( TBaseView );
   //rC.m_pCurrentDoc = GetActiveView()->GetDocument();
   rC.m_pCurrentDoc = m_pMainView->GetDocument();   
   rC.m_pNewDocTemplate = rC.m_pCurrentDoc->GetDocTemplate();
   //rC.m_pLastView = GetActiveView();
   /*rC.m_pLastView = m_pMainView;
   rC.m_pCurrentFrame = this;*/
   rC.m_pLastView = NULL;
   rC.m_pCurrentFrame = NULL;
 }


void CViewExSplitWnd1::DeleteColumn( int colDelete )
 {
   //if( colDelete != 1 ) CSplitterWnd::DeleteColumn( colDelete );
   CSplitterWnd::DeleteColumn( colDelete );

   if( true/*colDelete == 1*/ )
	{
      TChildFrame *pFr = reinterpret_cast<TChildFrame*>( GetParentFrame() );
	  if( pFr ) 
		pFr->m_optX.m_bFlBrig = false,
		pFr->m_optX.m_bFlElem = false;

	  if( pFr->m_optX.m_bFlModeBrig )
	    pFr->StopBrigMode();
	}
 }

void CViewExSplitWnd1::DeleteView( int row, int col )
 {
    if( GetColumnCount() > 1 )
	 {
       int x, m;
	   GetColumnInfo( 0, x, m );
	   CRect r; GetClientRect( r );
	   TChildFrame *pFr = reinterpret_cast<TChildFrame*>( GetParentFrame() );
	   if( pFr && r.Width() ) 
		 pFr->SetXSplit1( double(x) / double(r.Width()) );
	 }

	ASSERT_VALID(this);

	// if active child is being deleted - activate next
	CWnd* pPane = GetPane(row, col);
	//ASSERT_KINDOF(CView, pPane);
	if (GetActivePane() == pPane)
		ActivateNext(FALSE);

	// default implementation assumes view will auto delete in PostNcDestroy	

	pPane->DestroyWindow();

	if( long(pPane) == long(m_wndSplitter2.get()) )
	  delete m_wndSplitter2.release();
 }


void CViewExSplitWnd1::TrackColumnSize( int x, int col )
 {
   CRect r; GetClientRect( r );
   if( x < m_iLeftMin ) x = m_iLeftMin;
   else if( x > r.Width()*m_dLeftMax ) x = r.Width()*m_dLeftMax;

   m_dSplit = double(x) / double(r.Width());

   reinterpret_cast<TChildFrame*>(GetParentFrame())->m_optX.m_dSplit1 = m_dSplit;

   CSplitterWnd::TrackColumnSize( x, col );
 }

BEGIN_MESSAGE_MAP( CViewExSplitWnd1, CSplitterWnd )
  ON_WM_SIZE()
END_MESSAGE_MAP()

void CViewExSplitWnd1::OnSize( UINT nType, int cx, int cy )
 {   
   CSplitterWnd::OnSize( nType, cx, cy );      

   if( m_pColInfo /*&& nType != SIZE_MINIMIZED*/ )
	{
	  int iLeft = cx * m_dSplit,
	      iRight = cx - iLeft;

	  if( iLeft < m_iLeftMin )
	    m_dSplit = double(m_iLeftMin) / double(cx),
		iRight = cx - m_iLeftMin;
	  if( m_iRightMin > iRight ) 	   
 	    m_dSplit = double(cx - m_iRightMin) / double(cx);
	   

	  int cx_, cM;
	  GetColumnInfo( 0, cx_, cM );
	  if( cx*m_dSplit >= cM )
	    SetColumnInfo( 0, cx*m_dSplit, cM );	  
	  else if( GetColumnCount() > 1 )
	   {
	     int id = IdFromRowCol( 0, 0 );
		 GetPane( 0, 0 )->SetDlgCtrlID( IdFromRowCol(0,1) );      
		 GetPane( 0, 1 )->SetDlgCtrlID( id );
	     DeleteColumn( 1 );

		 SetActivePane( 0, 0 );
	   }	  

	  //CSplitterWnd::TrackColumnSize( cx * m_dSplit, 0 );
	}            
 }

void CViewExSplitWnd1::MakeSplit2( bool bFl )
 {
   CRect r;  m_wndSplitter2->GetClientRect( r );

   if( bFl && m_wndSplitter2->GetRowCount() == 1 )
	 m_wndSplitter2->SplitRow( m_wndSplitter2->m_dSplit * r.Height() );
   else if( !bFl && m_wndSplitter2->GetRowCount() == 2 )
	 m_wndSplitter2->DeleteRow( 1 );
 }

BOOL CViewExSplitWnd1::SplitColumn( int cxBefore )
 {
   CRect r; GetClientRect( r );
   //m_dSplit = double(cxBefore) / double(r.Width());
   TChildFrame *pFr = reinterpret_cast<TChildFrame*>( GetParentFrame() );
   if( pFr  ) 
	{
      m_dSplit = pFr->GetXSplit1();
	  cxBefore = m_dSplit * r.Width();
	}

   BOOL bRes = CSplitterWnd::SplitColumn( cxBefore );
   if( !bRes ) return 0;

   if( !m_wndSplitter2.get() ) return bRes;

   //TChildFrame *pFr = reinterpret_cast<TChildFrame*>( GetParentFrame() );
   m_wndSplitter2->m_dSplit = pFr->m_optX.m_dSplit2;
   if( pFr->m_optX.m_bFlBrig )	 	 
	{
	  CRect r; m_wndSplitter2->GetClientRect( r );		 
	  m_wndSplitter2->SplitRow( r.Height()*pFr->m_optX.m_dSplit2 );
	}

   return bRes;
 }

BOOL CViewExSplitWnd1::CreateView( int row, int col,
	CRuntimeClass* pViewClass,
	SIZE sizeInit, CCreateContext* pContext )
 {
	if( row == 0 && col == 0 )
	{	  
	  BOOL bRes = CSplitterWnd::CreateView( row, col,
			RUNTIME_CLASS(TDiplomView),
			sizeInit, pContext );
	  //GetPane(0,0)->SetDlgCtrlID( IdFromRowCol(0,1) );      
	  return bRes;
	}
	else
	{		
		//return CSplitterWnd::CreateView( row, col,
		//	RUNTIME_CLASS(CViewExSplitWnd2),	sizeInit, pContext );
      col = 0;

	  m_wndSplitter2 = auto_ptr<CViewExSplitWnd2>( new CViewExSplitWnd2() );
	  CCreateContext cc;	  
	  reinterpret_cast<TChildFrame*>( GetParentFrame() )->GetCtx( cc );
	  
	  if( !m_wndSplitter2->Create( this,
			2, 1,                 // TODO: adjust the number of rows, columns
			CSize( 40, 20 ),      // TODO: adjust the minimum pane size
			&cc,
			WS_CHILD|WS_VISIBLE|WS_BORDER|SPLS_DYNAMIC_SPLIT,
			IdFromRowCol(0,0)
		  )
		)
	   {
		 MessageBox( "Не удалось создать Splitter2", "Ошибка", MB_OK|MB_ICONEXCLAMATION );
		 return 0;
	   }

	  GetPane(0,0)->SetDlgCtrlID( IdFromRowCol(0,1) );      
	  

	  return TRUE;
	}
 }


BEGIN_MESSAGE_MAP( CViewExSplitWnd2, CSplitterWnd )
  ON_WM_DESTROY()
  ON_WM_SIZE()
  ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

void CViewExSplitWnd2::OnDestroy()
 {
   CSplitterWnd::OnDestroy();
 }

void CViewExSplitWnd2::ActivateNext( BOOL bPrev )
 {
   //CSplitterWnd::ActivateNext( bPrev );
   int iRow, iCol;
   GetActivePane( &iRow, &iCol );
   if( iRow == 0 && GetRowCount() == 2 ) 
	 SetActivePane( 1, 0 );
   else 
	 reinterpret_cast<CSplitterWnd*>( GetParent() )->SetActivePane( 0, 1 );
 }

BOOL CViewExSplitWnd2::CanActivateNext( BOOL bPrev )
 {
   return TRUE;
 }


CViewExSplitWnd2::CViewExSplitWnd2()
 {
   m_dSplit = 2.0/3.0;   
   m_iTopMin = 40;
   m_iBottomMin = 80;

   m_bFirst = true;
 }

CViewExSplitWnd2::~CViewExSplitWnd2()
 {
 } 

void CViewExSplitWnd2::OnShowWindow( BOOL bShow, UINT nStatus )
 {
   CSplitterWnd::OnShowWindow( bShow, nStatus );

   if( m_bFirst == true )
	{
      m_bFirst = false;
	  
	}
 }

CWnd* CViewExSplitWnd2::GetActivePane( int* pRow, int* pCol )
 {
   return CSplitterWnd::GetActivePane( pRow, pCol );

   /*ASSERT_VALID(this);

   // attempt to use active view of frame window
   CWnd* pView = NULL;
   CFrameWnd* pFrameWnd = GetParentFrame();
   ASSERT_VALID(pFrameWnd);
   pView = pFrameWnd->GetActiveView();

   // failing that, use the current focus
   if( pView == NULL ) pView = GetFocus();

   return pView;*/
 }

static TSnslDescr sdStensils[] = 
 {
  {IDB_SNSL_1, "Уголковый" },
  {IDB_SNSL_2, "Прямой" },
  {IDB_SNSL_3, "С плечом" },
  {IDB_SNSL_4, "Ступенчатый" },
  {IDB_SNSL_PC, "Устройство" },
  {IDB_SNSL_CNCT, "Cвязь" },  
  {IDB_SNSL_TXT, "Текст" },
  {0, 0}
 };

static TBtnDescr ap_buttons[] =
 {   
   { ID_LOC_ADD, false, true },	    		  
   { ID_LOC_ED, false, true },

   { ID_SEPARATOR,	false, true },   
   { ID_LOC_ACCEPT, false, true },

   { ID_LOC_REM, true, true }
 };
 
static UINT ap_buttonsRC[] =
 {        
   IDB_LOC_ADD,
   IDB_LOC_ED,
     0,   
   IDB_LOC_ACCEPT,
   IDB_LOC_REM
 };

static char* const ap_lpButtonsText[] =
 {          
   "",
   "",
     "",
   "",
   ""
	/*"\nСоздать",
	"\nУдалить",
	"\nОтредактировать",
	  ""*/
 };

void TChildFrame::OnLocAdd()
 {
   CListCtrl& rLst = GetBrigList()->GetListCtrl();
   TDiplomDoc *pDoc = m_pMainView->GetDocument();

   TAP_Brigada ap( new TBrigada() );
   TBrigDlg dlg( this, ap.get(), m_ptBrigDlg );
   if( dlg.DoModal() == IDOK )
	{
	  TBrigada *p = ap.get();
      pDoc->AddBrig( ap );
	  AddObj_BrigLst( rLst, p, rLst.GetItemCount() ? rLst.GetItemCount():0 );

      for( int i = rLst.GetItemCount() - 1; i >= 0; --i )	   
        if( rLst.GetItemState(i, LVIS_SELECTED)&LVIS_SELECTED )
	      rLst.SetItemState( i,
	        0, LVIS_SELECTED ); 

	  rLst.SetItemState( rLst.GetItemCount() - 1,
	    LVIS_SELECTED, LVIS_SELECTED );
	}
 }
void TChildFrame::OnLocRem()
 {
   CListCtrl& rLst = GetBrigList()->GetListCtrl();
   TDiplomDoc *pDoc = m_pMainView->GetDocument();

   int iKey = rLst.GetNextItem( -1, LVNI_SELECTED );
   for( int iItem = -1; (iItem = rLst.GetNextItem(-1, LVNI_SELECTED)) != -1; )
	{
	  CString str = rLst.GetItemText( iItem, 0 );
	  LPSTR ptr;
	  DWORD dw = strtoul( (LPCSTR)str, &ptr, 10 );
	  rLst.DeleteItem( iItem );

	  pDoc->RemoveBrig( dw );
	}
   
   iKey = max(0, min( iKey, rLst.GetItemCount()-1 ));
   rLst.SetItemState( iKey, LVIS_SELECTED, LVIS_SELECTED );
   rLst.Invalidate();
 }

void TChildFrame::OnLocAccept()
 {
   CListCtrl& rLst = GetBrigList()->GetListCtrl();
   TDiplomDoc *pDoc = m_pMainView->GetDocument();

   int iItem = rLst.GetNextItem( -1, LVNI_SELECTED );
   CString str = rLst.GetItemText( iItem, 0 );
   LPSTR ptr;
   DWORD dw = strtoul( (LPCSTR)str, &ptr, 10 );

   TBrigada *p = pDoc->GetBrigPtr( dw );
   
   if( !p->CheckingDIS(pDoc->RefLstDraw(), str) )
	{
      MessageBox( (LPCSTR)str, "Ошибка", MB_OK|MB_ICONEXCLAMATION );
	  return;
	}
   p->ReadServisedPCs( pDoc->RefLstDraw() );

   AddObj_BrigLst( rLst, p, iItem, true );
 }

void TChildFrame::OnLocEd()
 {
   CListCtrl& rLst = GetBrigList()->GetListCtrl();
   TDiplomDoc *pDoc = m_pMainView->GetDocument();

   int iItem = rLst.GetNextItem( -1, LVNI_SELECTED );
   CString str = rLst.GetItemText( iItem, 0 );
   LPSTR ptr;
   DWORD dw = strtoul( (LPCSTR)str, &ptr, 10 );

   TBrigada *p = pDoc->GetBrigPtr( dw );
   TBrigDlg dlg( this, p, m_ptBrigDlg );
   if( dlg.DoModal() == IDOK )
	 AddObj_BrigLst( rLst, p, iItem, true );
 }

void TChildFrame::OnEnter_List( NMHDR * pNotifyStruct, LRESULT * result )
 {
   if( CountSeletedInLstBrig() == 1 ) OnLocEd();
	
   *result = TRUE;
 }

void TChildFrame::OnDblClk_List( NMHDR * pNotifyStruct, LRESULT * result )
 {
   if( CountSeletedInLstBrig() == 1 ) OnLocEd();
	
   *result = TRUE;
 }

void TChildFrame::OnKeyDown_List( NMHDR * pNotifyStruct, LRESULT * result )
 {
   *result = TRUE;

   if( CountSeletedInLstBrig() <= 0 ) return;
   
   LV_KEYDOWN* pKd = (LV_KEYDOWN*)pNotifyStruct;
   if( pKd->wVKey == VK_DELETE )  OnLocRem();
 }
void TChildFrame::OnItemChg_List( NMHDR * pNotifyStruct, LRESULT * result )
 {
   *result = TRUE;

   if( !m_optX.m_bFlModeBrig ) return;

   NM_LISTVIEW *pIt = (NM_LISTVIEW *)pNotifyStruct;
   if( (pIt->uChanged&LVIF_STATE) && (pIt->uNewState&LVIS_SELECTED) 
	   //CountSeletedInLstBrig() == 1
	 )	
	 LoadBrigLst( pIt->iItem );		  
 }

void TChildFrame::FillBrigList()
 {
   CRowListView *pLst = GetBrigList();
   if( !pLst ) return;   
   CListCtrl& rLst = pLst->GetListCtrl();

   rLst.DeleteAllItems();
   TDiplomDoc *pDoc = m_pMainView->GetDocument();

   TLockRedraw lock( &rLst );

   TLST_Brigada::iterator itSta( pDoc->m_lstBrig.begin() );
   TLST_Brigada::iterator itEnd( pDoc->m_lstBrig.end() );
   for( int i = 0; itSta != itEnd; ++itSta, ++i )
	 AddObj_BrigLst( rLst, (*itSta).get(), i );

   if( pDoc->m_iKeyChecked != -1 ) 
	 pLst->CheckItem( pDoc->m_iKeyChecked );

 }

void TChildFrame::AddObj_BrigLst( CListCtrl& lst, TBrigada* pUs, int iItem, bool bFlModify )
 {
   LV_ITEM lvIt;
   memset( &lvIt, 0, sizeof(LV_ITEM) );
   lvIt.mask = LVIF_TEXT;   
   
   char cBuf[ 100 ];
   lvIt.pszText = cBuf;   
   
   lvIt.iItem = iItem;
   
   sprintf( cBuf, "%u", (int)pUs->ID() );
   lvIt.iSubItem = 0;    
   if( bFlModify == false ) lvIt.iItem = lst.InsertItem( &lvIt );   	  
	 lst.SetItem( &lvIt );	

   sprintf( cBuf, "%u", (int)pUs->m_iOA );
   lvIt.iSubItem = 1;      
   lst.SetItem( &lvIt );

   sprintf( cBuf, "%s", pUs->GetDISDescribe() );
   lvIt.iSubItem = 2; 	     
   lst.SetItem( &lvIt );	    
   
   /*if( pUs->m_tdsDIS == TDS_NoPriorety )
	 sprintf( cBuf, "%u", pUs->m_iTotal );
   else*/
     sprintf( cBuf, "%u/%u", pUs->m_iTotal, pUs->m_iLabel );
   lvIt.iSubItem = 3; 	     
   lst.SetItem( &lvIt );	    
 }


BOOL CViewExSplitWnd2::CreateView(int row, int col,
	CRuntimeClass* pViewClass,
	SIZE sizeInit, CCreateContext* pContext)
 {
	if( row == 0 && col == 0 )
	{
	  BOOL bRes = CSplitterWnd::CreateView( row, col,
			RUNTIME_CLASS(TBaseView),	
			sizeInit, pContext );
	  if( !bRes ) return 0;

	  TBaseView *pBs = reinterpret_cast<TBaseView*>( GetPane(0,0) );
	  
	  m_apLstStnsl = auto_ptr<TListStensil>( new TListStensil() );
	  m_apLstStnsl->SetStnsl( sdStensils );	  
	  CRect r( 0, 0, 100, 100 );
	  if( !m_apLstStnsl->Create( 
		    WS_CHILD|WS_VISIBLE|WS_TABSTOP|LVS_AUTOARRANGE|LVS_ICON|LVS_SINGLESEL,
		     r, pBs, 1)
	    ) return 0;
	  m_apLstStnsl->SetBkColor( RGB(128,255,128) );
	  m_apLstStnsl->SetTextBkColor( RGB(128,255,128) );

	  pBs->SetMinWH( m_apLstStnsl->MinW(), m_apLstStnsl->MinH() );
	
	  //pBs->GetClientRect( r );
	  *pBs + new TChildItem( m_apLstStnsl.get(), 0, 0, 1, 1 );	

      pBs->MakeLaoyt();

	  return bRes;
	}
	else
	{
      CCreateContext cc;	  
	  reinterpret_cast<TChildFrame*>( GetParentFrame() )->GetCtx( cc );
	  cc.m_pNewViewClass = RUNTIME_CLASS(TBaseView);

	  BOOL bRes = CSplitterWnd::CreateView( row, col,
			RUNTIME_CLASS(TBaseView),
			sizeInit, &cc );
	  if( !bRes ) return 0;

	  TBaseView *pBs = reinterpret_cast<TBaseView*>( GetPane(1,0) );

	  m_apListBr = auto_ptr<CRowListView>( new CRowListView() );
	  m_apListBr.release();

      
	  cc.m_pNewViewClass = RUNTIME_CLASS( CRowListView );

	  CRect r( 0, 0, 100, 100 );
	  if( !m_apListBr->Create( NULL, "BrList", 
	         WS_CHILD|WS_VISIBLE|WS_TABSTOP,
	         r, pBs, ID_LST_BRIG, &cc )		    
	    ) return 0;

	  pBs->SetMinWH( 50, 50 );

	  CBitmap bm; bm.LoadBitmap( ap_buttonsRC[1] );
	  BITMAP bmp;
	  ::GetObject( bm, sizeof(BITMAP), &bmp );
	  int iY = float(bmp.bmHeight) * 1.9;
	  

	  m_apTool = auto_ptr<TMSStyleTooolBar>( new TMSStyleTooolBar(this) );
		
	  for( int i = 0; i < sizeof(ap_buttons) / sizeof(TBtnDescr); i++ )
	   {                      
		 TMSItem *pMsIt;
		 pMsIt = new TMSItem(); 
		 pMsIt->Init( ap_lpButtonsText[i], ap_buttonsRC[i], 
		   ap_buttons[i].uCmd, ap_buttons[i].bRight, ap_buttons[i].bPush );		 
		
		 *m_apTool, pMsIt;		 
	   }
	  
	  LOGFONT lf = { 8, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET,
		OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
		VARIABLE_PITCH | FF_DONTCARE, "MS Sans Serif"
	  };

	  CFont *pFnt = new CFont();
	  pFnt->CreateFontIndirect( &lf );
	  m_apTool->SetFont( pFnt );
	  
	  m_apTool->Init( true ); 

	  if( !m_apTool->Create(pBs, WS_CHILD | WS_VISIBLE |
			  CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY,  AFX_IDW_TOOLBAR+1 ) 
		  //!m_arcTool()->LoadBitmap(IDR_MAINFRAME) 		
		)
	  {
		  //AfxMessageBox( "Failed to create toolbar\n", MB_OK | MB_ICONSTOP );
		  ((TDiplomApp*)AfxGetApp())->bwcc.BWCCMessageBox( m_hWnd, "Ошибка при создании локального тулбара", "Ошибка", MB_OK | MB_ICONSTOP );
		  return 0;      
	  }

	  /*m_apTool->GetToolBarCtrl().SetBarStyle( 
	     m_apTool->GetToolBarCtrl().GetBarStyle()& 
		   (CBRS_BORDER_TOP|
		    CBRS_BORDER_BOTTOM|
			CBRS_BORDER_LEFT|
			CBRS_BORDER_RIGHT
		   )
		 );*/
	  m_apTool->ModifyStyle( WS_BORDER, 0 );

	  m_apTool->SetBarStyle( m_apTool->GetBarStyle() &
	    ~(CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM |
	    CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT) );

	  *pBs + new TChildItem( m_apListBr.get(), 0, iY + 1, 1, 1 ) +
	         new TChildItem( m_apTool.get(), 0, 0, 1, iY );
      
	  m_apListBr->OnInitialUpdate();
	  pBs->MakeLaoyt();


      ((TChildFrame*)GetParentFrame())->FillBrigList();

	  return bRes;
	}
 }


void CViewExSplitWnd2::DeleteRow( int colDelete )
 {
   //if( colDelete != 1 ) CSplitterWnd::DeleteColumn( colDelete );
   CSplitterWnd::DeleteRow( colDelete );

   if( colDelete == 1 )
	{
      TChildFrame *pFr = reinterpret_cast<TChildFrame*>( GetParentFrame() );
	  if( pFr ) pFr->m_optX.m_bFlBrig = false;
	}
 }

void CViewExSplitWnd2::DeleteView( int row, int col )
 {
    if( GetRowCount() > 1 )
	 {
       int y, m;

	   GetRowInfo( 0, y, m );
	   CRect r; GetClientRect( r );
	   TChildFrame *pFr = reinterpret_cast<TChildFrame*>( GetParentFrame() );
	   if( pFr && r.Height() ) 
		 pFr->SetXSplit2( double(y) / double(r.Height()) );
	 }

    CSplitterWnd::DeleteView( row, col );	
 }


void CViewExSplitWnd2::TrackRowSize( int x, int col )
 {
   CRect r; GetClientRect( r );
   if( x < m_iTopMin ) x = m_iTopMin;
   else if( r.Height() - x < m_iBottomMin ) 
	 x = r.Height() - m_iBottomMin;

   m_dSplit = double(x) / double(r.Height());

   reinterpret_cast<TChildFrame*>(GetParentFrame())->m_optX.m_dSplit2 = m_dSplit;

   CSplitterWnd::TrackRowSize( x, col );
 }


void CViewExSplitWnd2::OnSize( UINT nType, int cx, int cy )
 {
   CSplitterWnd::OnSize( nType, cx, cy );   

   if( m_pColInfo /*&& nType != SIZE_MINIMIZED*/ )
	{
	  int iLeft = cy * m_dSplit,
	      iRight = cy - iLeft;

	  if( iLeft < m_iTopMin )
	    m_dSplit = double(m_iTopMin) / double(cy),
		iRight = cy - m_iTopMin;
	  if( m_iBottomMin > iRight ) 	   
 	    m_dSplit = double(cy - m_iBottomMin) / double(cy);
	   
      if( GetRowCount() > 1 )
	   {
		 int cx_, cM;
		 GetRowInfo( 0, cx_, cM );
		 if( cy*m_dSplit >= cM )
		   SetRowInfo( 0, cy*m_dSplit, cM );	  
		 else 
		  {
			/*int id = IdFromRowCol( 0, 0 );
			GetPane( 0, 0 )->SetDlgCtrlID( IdFromRowCol(1,0) );      
			GetPane( 1, 0 )->SetDlgCtrlID( id );*/
			DeleteRow( 1 );
		  }	  
	   }

	  //CSplitterWnd::TrackColumnSize( cx * m_dSplit, 0 );
	}      
 }

BOOL CViewExSplitWnd2::SplitRow( int cxBefore )
 {
   CRect r; GetClientRect( r );
   //m_dSplit = double(cxBefore) / double(r.Height());
   TChildFrame *pFr = reinterpret_cast<TChildFrame*>( GetParentFrame() );
   if( pFr  ) 
	{
      m_dSplit = pFr->GetXSplit2();
	  cxBefore = m_dSplit * r.Height();
	}

   return CSplitterWnd::SplitRow( cxBefore );
 }


BOOL TChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
 {
	// TODO: Add your specialized code here and/or call the base class
	
	//return CMDIChildWnd::OnCreateClient(lpcs, pContext);
	if( !m_wndSplitter1.Create( this,
		  1, 2,                 // TODO: adjust the number of rows, columns
		  CSize( 40, 40 ),      // TODO: adjust the minimum pane size
		  pContext,
		  WS_CHILD|WS_VISIBLE|SPLS_DYNAMIC_SPLIT
		)
	  )
	 {
       MessageBox( "Не удалось создать Splitter1", "Ошибка", MB_OK|MB_ICONEXCLAMATION );
	   return 0;
	 }    	

	return TRUE;
 }


void TChildFrame::OnUpdate_Servis( CCmdUI* pCmdUI )
 {
   OnUpdate_HaveSel( pCmdUI );
   if( !m_optX.m_bFlModeBrig || !m_bEnblBrigEdit ) 
	 pCmdUI->Enable( FALSE );
 }
void TChildFrame::OnUpdate_UnServis( CCmdUI* pCmdUI )
 {
   OnUpdate_HaveSel( pCmdUI );
   if( !m_optX.m_bFlModeBrig || !m_bEnblBrigEdit ) 
	 pCmdUI->Enable( FALSE );
 }
void TChildFrame::OnUpdate_Label( CCmdUI* pCmdUI )
 {
   OnUpdate_HaveSel( pCmdUI );
   if( !m_optX.m_bFlModeBrig || !m_bEnblBrigEdit ) 
	 pCmdUI->Enable( FALSE );
 }


void TChildFrame::On_Servis()
 {
   m_pMainView->GetDocument()->ServisSelected( 1 );
 }
void TChildFrame::On_UnServis()
 {
   m_pMainView->GetDocument()->ServisSelected( 0 );
 }
void TChildFrame::On_Label()
 {
   m_pMainView->GetDocument()->ServisSelected( 2 );
 }


void TChildFrame::OnUpd_Property_NoCrit( CCmdUI* pCmdUI )
 {
   OnUpdate_HaveSel_NoCrit( pCmdUI );
 }

void TChildFrame::OnUpd_ViewGrid( CCmdUI* pCmdUI )
 {
   pCmdUI->SetCheck( m_optX.m_bFlGrid );
 }
void TChildFrame::OnUpd_ViewGuidlines( CCmdUI* pCmdUI )
 {
   pCmdUI->SetCheck( m_optX.m_bFlGuids );
 }
void TChildFrame::OnUpd_ViewCnctPoints( CCmdUI* pCmdUI )
 {
   pCmdUI->SetCheck( m_optX.m_bFlGluePoints );
 }
void TChildFrame::OnUpd_ViewLine( CCmdUI* pCmdUI )
 {
   pCmdUI->SetCheck( m_optX.m_dLines );
 }
void TChildFrame::OnUpd_ViewBrig( CCmdUI* pCmdUI )
 {
   pCmdUI->SetCheck( m_optX.m_bFlBrig );
 }
void TChildFrame::OnUpd_ViewElements( CCmdUI* pCmdUI )
 {
   pCmdUI->SetCheck( m_optX.m_bFlElem );
 }

void TChildFrame::OnUpd_StepGrid( CCmdUI* pCmdUI )
 {
   pCmdUI->SetCheck( m_optX.m_bFlStepOnGrid );
 }
void TChildFrame::OnUpd_StepGuid( CCmdUI* pCmdUI )
 {
   pCmdUI->SetCheck( m_optX.m_bFlStepOnGuid );
 }
void TChildFrame::OnUpd_ModeBrig( CCmdUI* pCmdUI )
 {
   //pCmdUI->SetCheck( m_optX.m_bFlModeBrig );
   CRowListView* pLV = GetBrigList();
   if( m_bFlRun || !pLV || pLV->GetListCtrl().GetNextItem(-1, LVNI_SELECTED) == -1 )
	{
	  if( m_optX.m_bFlModeBrig ) StopBrigMode();
      pCmdUI->Enable( FALSE );
	}
   else
	{
	  pCmdUI->Enable( TRUE ); 
      reinterpret_cast<TMainFrame*>(AfxGetApp()->m_pMainWnd)->CheckBrig( m_optX.m_bFlModeBrig );
	}

   if( pLV )	
      m_pMainView->GetDocument()->m_iKeyChecked = pLV->m_nCheckedItem;	
   
 }

void TChildFrame::OnUpdate_Revert( CCmdUI* pCmdUI )
 {
   pCmdUI->Enable( m_pMainView->GetDocument() != NULL );
 }
void TChildFrame::On_Export()
 {
   CFileDialog dlgFile( FALSE, _T(".wmf"), NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST,
		_T("Windows метафайл (*.wmf) | *.wmf | All Files (*.*) | *.* ||"), 
		this );   
   
   char drive[ _MAX_DRIVE ];
   char dir[ _MAX_DIR ];
   char fname[ _MAX_FNAME ];
   char ext[ _MAX_EXT ];
   char fNam[ _MAX_PATH ];

   _splitpath( (LPCSTR)m_pMainView->GetDocument()->GetPathName(), drive, dir, fname, ext );
   _makepath( fNam, "", "", fname, ".wmf" );

   dlgFile.m_ofn.lpstrFile = fNam;
   dlgFile.m_ofn.nMaxFile = 2048;
   dlgFile.m_ofn.nFilterIndex = 1;
   dlgFile.m_ofn.nMaxCustFilter = 2;

   if( dlgFile.DoModal() == IDOK )
	{
      UpdateWindow();

	  CMetaFileDC dc;
	  CString str = dlgFile.GetPathName();
	  CClientDC dcR( m_pMainView );
	  m_pMainView->OnPrepareDC_Met( &dcR );
	  if( !dc.CreateEnhanced(NULL, str, NULL, NULL) )
	   {
	     CString str2;
		 str2.Format( "Ошибка создания файла \"%s\"", (LPCSTR)str );
	     MessageBox( str2, "Ошибка", MB_OK|MB_ICONSTOP );
		 return;
	   }
	  double  dKeyScale;
	  CRect rCC;	  
	  m_pMainView->GetDocument()->FlipObjects();
	  m_pMainView->GetDocument()->GetSelectionCenter( false, &rCC );
	  CPoint pt( rCC.left, rCC.top );
	  m_pMainView->GetDocument()->ShiftObjects( -pt );
	  try {
	    m_pMainView->OnPrepareDC_Met( &dc );

		dKeyScale = m_optX.m_dScale;
	    m_optX.m_dScale = 1;
	    m_pMainView->GetDocument()->NewScaleAll( 1 );

		CRect rClip( SHRT_MIN, SHRT_MIN, SHRT_MAX, SHRT_MAX );
	    m_pMainView->GetDocument()->Draw( &dc, 1, rClip );		
	   }
	  catch( CException *pE )
	   {
	     pE->ReportError(); 
		 pE->Delete();
		 //::DeleteMetaFile( dc.CloseEnhanced() );
		 //return;
	   }
	  catch( exception e )
	   {
	     MessageBox( e.what(), "Ошибка", MB_OK|MB_ICONSTOP );
		 //::DeleteMetaFile( dc.Close() );
		 //return;
	   }
	  catch(...)
	   {
	     MessageBox( "Общая ошибка", "Ошибка", MB_OK|MB_ICONSTOP );
		 //::DeleteMetaFile( dc.Close() );
		 //return;
	   }

	  m_optX.m_dScale = dKeyScale;
	  m_pMainView->GetDocument()->NewScaleAll( dKeyScale );
	  m_pMainView->GetDocument()->ShiftObjects( pt );
	  m_pMainView->GetDocument()->FlipObjects();

	  ::DeleteEnhMetaFile( dc.CloseEnhanced() );
	}
 }
void TChildFrame::On_Revert()
 {
   CString str( m_pMainView->GetDocument()->GetPathName() );

   if( m_pMainView->GetDocument()->IsModified() )
	{
      char drive[ _MAX_DRIVE ];
      char dir[ _MAX_DIR ];
      char fname[ _MAX_FNAME ];
      char ext[ _MAX_EXT ];
	  char fNam[ _MAX_PATH ];

	  _splitpath( (LPCSTR)str, drive, dir, fname, ext ); 
	  _makepath( fNam, "", "", fname, ext );
	  CString str2; 
	  str2.Format( "Восстановить схему [%s] из файла ?", fNam );
	  if( MessageBox(str2, "Предупреждение", 
	       MB_YESNO|MB_ICONQUESTION) != IDYES
		)
	    return;
	}
  
   double dScKey = m_pMainView->GetDocument()->m_dSc;

   m_pMainView->GetDocument()->DeleteContents();
   m_pMainView->GetDocument()->LoadObjectsOnly( str );

   //m_pMainView->GetDocument()->m_dSc = dScKey;
   m_pMainView->GetDocument()->NewScaleAll( m_pMainView->GetDocument()->m_dSc );

   //m_pMainView->SetPageSize( m_pMainView->GetDocument()->GetSize(), false );
   //m_pMainView->SetupScaleCBN( m_optX.m_dScale, m_dbarHor );

   FillBrigList();

   RefreshDrawingPane();
 }

void TChildFrame::LoadBrigLst( int iItem )
 {
   CListCtrl& rLst = GetBrigList()->GetListCtrl();
   CString str = rLst.GetItemText( iItem, 0 );
   LPSTR ptr;
   DWORD dw = strtoul( (LPCSTR)str, &ptr, 10 );
   TDiplomDoc *pDoc = m_pMainView->GetDocument();
   TBrigada *p = pDoc->GetBrigPtr( dw );

   if( !TBrigada::WirePCPtrs(p->m_lstPC, m_pMainView->GetDocument()->RefLstDraw()) )
	{
      TDrawShapeList::iterator itSta( m_pMainView->GetDocument()->RefLstDraw().begin() );
	  TDrawShapeList::iterator itEnd( m_pMainView->GetDocument()->RefLstDraw().end() );
	  for( ;itSta != itEnd; ++itSta )
		if( (*itSta)->isA() == TT_PC )
		 {
		   short key = ((TDrawPC*)(*itSta).get())->m_sServis_Brig;
		   ((TDrawPC*)(*itSta).get())->m_sServis_Brig = 0;
		   if( key != 0 )
			 m_pMainView->InvalObj( (*itSta).get() );
		 }

	  CString str;
      str.Format( "У бригады #%u нарушен список обслуживаемых устройств", (int)p->ID() );
	  MessageBox( str, "Предупреждение", 
	       MB_OK|MB_ICONINFORMATION );
	  return;
	}	  	

   p->ShowServisedPCs( pDoc->RefLstDraw(), m_pMainView );
 }

void TChildFrame::OnUpd_SimpleView_NoCrit( CCmdUI* pCmdUI )
 {   
   pCmdUI->SetCheck( m_pMainView && m_pMainView->GetDocument()->m_bFlSimpleView );
 }

void TChildFrame::OnUpd_Arrow_NoCrit( CCmdUI* pCmdUI )
 {
   pCmdUI->SetCheck( m_pMainView && m_pMainView->GetDocument()->m_bFlArrow );
 }
      
void TChildFrame::OnModeBrig()
 {
   m_optX.m_bFlModeBrig = !m_optX.m_bFlModeBrig;

   TDiplomDoc *pDoc = m_pMainView->GetDocument();

   if( m_optX.m_bFlModeBrig )
	{   
	  CListCtrl& rLst = GetBrigList()->GetListCtrl();      
	  int iItem = rLst.GetNextItem( -1, LVNI_SELECTED );
	  if( iItem == -1 ) return;
	  LoadBrigLst( iItem );
	}
   else
	 StopBrigMode();

   m_pMainView->Invalidate();
 }
void TChildFrame::StopBrigMode()
 {
   TDiplomDoc *pDoc = m_pMainView->GetDocument();
   TBrigada::ClearServisedPCs( pDoc->RefLstDraw() );

   m_optX.m_bFlModeBrig = false;
   reinterpret_cast<TMainFrame*>(AfxGetApp()->m_pMainWnd)->CheckBrig( false );

   CRowListView *pLV;
   if( (pLV=GetBrigList()) )	
     pDoc->m_iKeyChecked = pLV->m_nCheckedItem;	
 }
void TChildFrame::On_ViewGrid()
 {
   m_optX.m_bFlGrid = !m_optX.m_bFlGrid;
   RefreshDrawingPane();
 }
void TChildFrame::On_ViewGuidlines()
 {
   m_optX.m_bFlGuids = !m_optX.m_bFlGuids;
   RefreshDrawingPane();
 }
void TChildFrame::On_ViewCnctPoints()
 {
   m_optX.m_bFlGluePoints = !m_optX.m_bFlGluePoints;  
   RefreshDrawingPane();   
 }
void TChildFrame::On_ViewLine()
 {
   m_optX.m_dLines = !m_optX.m_dLines;
   RefreshDrawingPane();   
 }

void TChildFrame::On_SimpleView()
 {
   if( !m_pMainView ) return;
   if( !CriticalRun() )
	{
      MessageBeep( -1 ); return;
	}

   m_evDataAccEnable.ResetEvent();

   try {
	 m_pMainView->GetDocument()->m_bFlSimpleView = !m_pMainView->GetDocument()->m_bFlSimpleView;
	 m_pMainView->GetDocument()->SetViewMode();
	 RefreshDrawingPane();   
	}
   catch(...)
	{
	  m_evDataAccEnable.SetEvent();
	  throw;
	}
   m_evDataAccEnable.SetEvent();
 }

void TChildFrame::On_Arrow()
 {
   if( !m_pMainView ) return;
   if( !CriticalRun() )
	{
      MessageBeep( -1 ); return;
	}

   m_evDataAccEnable.ResetEvent();
   try {
	 m_pMainView->GetDocument()->m_bFlArrow = !m_pMainView->GetDocument()->m_bFlArrow;
	 RefreshDrawingPane();   
	}
   catch(...)
	{
      m_evDataAccEnable.SetEvent();
	  throw;
	}
   m_evDataAccEnable.SetEvent();
 }

void TChildFrame::OnCBNScale_ENTER()
 {
   if( !CriticalRun() )
	{
      MessageBeep( -1 ); return;
	}

   m_evDataAccEnable.ResetEvent();
   try {
     m_pMainView->SetCenterViewToSelection();
     CallDrawView_Func( &TDiplomView::OnCBNScale, true );   
	}
   catch(...)
	{
      m_evDataAccEnable.SetEvent();
	  throw;
	}
   m_evDataAccEnable.SetEvent();
 }

void TChildFrame::OnCBNSCale()
 {
   if( !CriticalRun() )
	{
      MessageBeep( -1 ); return;
	}

   m_evDataAccEnable.ResetEvent();
   try {
     m_pMainView->SetCenterViewToSelection();
     CallDrawView_Func( &TDiplomView::OnCBNScale, false );   
   }
   catch(...)
	{
      m_evDataAccEnable.SetEvent();
	  throw;
	}
   m_evDataAccEnable.SetEvent();
 }

void TChildFrame::OnView_PrivScale()
 {
   CComboBox *pComb = reinterpret_cast<CComboBox*>( m_dbarHor.GetDlgItem(ID_CBN_SCALE) );
   pComb->SetCurSel( pComb->FindStringExact(-1, "Предидущий") );
   OnCBNSCale();
 }
void TChildFrame::OnView_Scale100()
 {
   CComboBox *pComb = reinterpret_cast<CComboBox*>( m_dbarHor.GetDlgItem(ID_CBN_SCALE) );
   pComb->SetCurSel( pComb->FindStringExact(-1, "100") );
   OnCBNSCale();
 }
void TChildFrame::OnView_Pagew()
 {
   CComboBox *pComb = reinterpret_cast<CComboBox*>( m_dbarHor.GetDlgItem(ID_CBN_SCALE) );
   pComb->SetCurSel( pComb->FindStringExact(-1, "Ширина") );
   OnCBNSCale();
 }
void TChildFrame::OnView_Page()
 {
   CComboBox *pComb = reinterpret_cast<CComboBox*>( m_dbarHor.GetDlgItem(ID_CBN_SCALE) );
   pComb->SetCurSel( pComb->FindStringExact(-1, "Страница") );
   OnCBNSCale();
 }
void TChildFrame::OnView_200()
 {
   CComboBox *pComb = reinterpret_cast<CComboBox*>( m_dbarHor.GetDlgItem(ID_CBN_SCALE) );
   pComb->SetCurSel( pComb->FindStringExact(-1, "200") );
   OnCBNSCale();
 }
void TChildFrame::OnView_50()
 {
   CComboBox *pComb = reinterpret_cast<CComboBox*>( m_dbarHor.GetDlgItem(ID_CBN_SCALE) );
   pComb->SetCurSel( pComb->FindStringExact(-1, "50") );
   OnCBNSCale();
 }

void TChildFrame::OnUpdate_LocAdd( CCmdUI* pCmdUI )
 {
   OnUpdate_Always( pCmdUI );
   //pCmdUI->Enable( TRUE );
 }

int TChildFrame::CountSeletedInLstBrig()
 {
   CRowListView *pL = GetBrigList();
   if( pL )
     return pL->GetListCtrl().GetSelectedCount();	  
   else return 0;
 }

void TChildFrame::OnUpdate_LocRemove( CCmdUI* pCmdUI )
 {      
   pCmdUI->Enable( CountSeletedInLstBrig() > 0 );
 }
void TChildFrame::OnUpdate_LocEdit( CCmdUI* pCmdUI )
 {
   pCmdUI->Enable( CountSeletedInLstBrig() == 1 );
 }

void TChildFrame::OnUpdate_LocAccept( CCmdUI* pCmdUI )
 {
   pCmdUI->Enable( 
	 CountSeletedInLstBrig() == 1 && m_optX.m_bFlModeBrig );
 }


CRowListView* TChildFrame::GetBrigList()
 {
   if( m_wndSplitter1.GetColumnCount() == 2 &&
	   m_wndSplitter1.m_wndSplitter2.get() &&
	   m_wndSplitter1.m_wndSplitter2->GetRowCount() == 2 &&
	   m_wndSplitter1.m_wndSplitter2->m_apListBr.get()
	 ) return m_wndSplitter1.m_wndSplitter2->m_apListBr.get();

   return NULL;
 }

void TChildFrame::OnUpdate_SelAll_NoCrit( CCmdUI* pCmdUI )
 {
   pCmdUI->Enable( HaveObjects() && CriticalRun() );
 }

void TChildFrame::OnUpdate_SelAll( CCmdUI* pCmdUI )
 {
   pCmdUI->Enable( HaveObjects() );
 }

void TChildFrame::OnUpdate_HaveSel_NoCrit( CCmdUI* pCmdUI )
 {
   pCmdUI->Enable( CriticalRun() && m_pMainView && m_pMainView->GetDocument() &&
	 m_pMainView->GetDocument()->m_bHaveSel );
 }
void TChildFrame::OnUpdate_HaveSel_NoRun( CCmdUI* pCmdUI )
 {
   pCmdUI->Enable( !m_bFlRun && m_pMainView && m_pMainView->GetDocument() &&
	 m_pMainView->GetDocument()->m_bHaveSel );
 }
void TChildFrame::OnUpdate_HaveSel( CCmdUI* pCmdUI )
 {
   pCmdUI->Enable( m_pMainView && m_pMainView->GetDocument() &&
	 m_pMainView->GetDocument()->m_bHaveSel );
 }
void TChildFrame::OnUpdate_Paste_NoRun( CCmdUI* pCmdUI )
 {
   if( m_bFlRun )
	{
	  pCmdUI->Enable( 0 );
	  return;
	}

   COleDataObject dataObject;
   BOOL bEnable = dataObject.AttachClipboard() &&
	   ( dataObject.IsDataAvailable(m_cfDraw) );
		
   pCmdUI->Enable( bEnable );
 }
void TChildFrame::OnUpdate_Paste( CCmdUI* pCmdUI )
 {
   COleDataObject dataObject;
   BOOL bEnable = dataObject.AttachClipboard() &&
	   ( dataObject.IsDataAvailable(m_cfDraw) );
		
   pCmdUI->Enable( bEnable );
 }

void TChildFrame::On_Edit_Dup()
 {
   m_pMainView->CloneShpSelection();
 }

void TChildFrame::On_Edit_Unhilite()
 {
   if( !CriticalRun() )
	{
      MessageBeep( -1 ); return;
	}

   m_evDataAccEnable.ResetEvent();
   try {
     m_pMainView->GetDocument()->UnHilite();
	}
   catch(...)
	{
	  m_evDataAccEnable.SetEvent();
	  throw;
	}
   m_evDataAccEnable.SetEvent();
 }

void TChildFrame::On_Edit_SelPC()
 {
   if( !CriticalRun() )
	{
      MessageBeep( -1 ); return;
	}

   m_evDataAccEnable.ResetEvent();
   try {
	 m_pMainView->GetDocument()->SelectPC( TSEL_SelAll );   
	 RefreshDrawingPane();
   }
   catch(...)
	{
	  m_evDataAccEnable.SetEvent();
	  throw;
	}
   m_evDataAccEnable.SetEvent();
 }
void TChildFrame::On_Edit_SelCnct()
 {
   if( !CriticalRun() )
	{
      MessageBeep( -1 ); return;
	}

   m_evDataAccEnable.ResetEvent();
   try {
     m_pMainView->GetDocument()->SelectCnct( TSEL_SelAll );   
     RefreshDrawingPane();
   }
   catch(...)
	{
	  m_evDataAccEnable.SetEvent();
	  throw;
	}
   m_evDataAccEnable.SetEvent();
 }


/*class TFndDPForIndDTa
 {
public:
   TFndDPForIndDTa( int iInd )
	{
	  m_ind = iInd;
	}
   bool operator()( const TAP_TPDP& rA )
	{
	  return rA->m_iNum == m_ind;
	}
   int m_ind;
 };*/

void TChildFrame::On_SelSubch()
 {
   if( m_bFlRun ) return;

   TAP_TPDP_Lst lstDP;
   CWaitCursor wc;
   if( !m_pMainView->GetDocument()->CreAllDPs(lstDP) ) return;
   wc.Restore();
   
   TSubDlg dlg( this );
   TAP_TPDP_Lst::iterator itSta( lstDP.begin() );
   TAP_TPDP_Lst::iterator itEnd( lstDP.end() );
   int i = 0;
   for( ; itSta != itEnd; ++itSta, ++i )
	{
      CString str;
	  str.Format( "%3u - (%u)", i, (int)(*itSta)->m_iOutRng );
	  dlg.m_lstFill.push_back( str );
	}
   if( dlg.DoModal() == IDOK && dlg.m_lstSel.size() )
	{
	  list<short>::iterator itSta( dlg.m_lstSel.begin() );
	  list<short>::iterator itEnd( dlg.m_lstSel.end() );

	  CWaitCursor wc;
	  m_pMainView->GetDocument()->SelectAll( false );
	  
	  for( ; itSta != itEnd; ++itSta )
	   {
	     /*TFndDPForIndDTa dta( *itSta );
         TAP_TPDP_Lst::iterator it = find_if( lstDP.begin(), lstDP.end(), dta );
		 */
		 TAP_TPDP_Lst::iterator it = lstDP.begin();
		 advance( it, *itSta );

		 m_pMainView->GetDocument()->SelectDP( **it, 0, true );		  
	   }

	  m_pMainView->GetDocument()->RenumSelected();
	  RefreshDrawingPane();   
	}
 }
void TChildFrame::On_SelPCDp()
 {
   if( m_bFlRun ) return;
   
   m_pMainView->GetDocument()->Sel_Objs( TSEL_SelDp, 1 );
   RefreshDrawingPane();   
 }
void TChildFrame::On_SelPCHigh()
 {
   if( !CriticalRun() )
	{
      MessageBeep( -1 ); return;
	}

   m_evDataAccEnable.ResetEvent();
   try {
     m_pMainView->GetDocument()->SelectPC( TSEL_SelHigh );
     RefreshDrawingPane();
   }
   catch(...)
	{
	  m_evDataAccEnable.SetEvent();
	  throw;
	}
   m_evDataAccEnable.SetEvent();
 }
void TChildFrame::On_SelCnctDP()
 {
   if( m_bFlRun ) return;
   
   m_pMainView->GetDocument()->Sel_Objs( TSEL_SelDp, 2 );
   RefreshDrawingPane();   
 }
void TChildFrame::On_SelCnctHigh()
 {
   if( !CriticalRun() )
	{
      MessageBeep( -1 ); return;
	}

   m_evDataAccEnable.ResetEvent();
   try {
     m_pMainView->GetDocument()->SelectCnct( TSEL_SelHigh );   
     RefreshDrawingPane();
   }
   catch(...)
	{
	  m_evDataAccEnable.SetEvent();
	  throw;
	}
   m_evDataAccEnable.SetEvent();
 }
void TChildFrame::On_Edit_SelAll()
 {
   if( !CriticalRun() )
	{
      MessageBeep( -1 ); return;
	}

   m_evDataAccEnable.ResetEvent();
   try {
     m_pMainView->GetDocument()->SelectAll( true );   
     RefreshDrawingPane();
   }
   catch(...)
	{
	  m_evDataAccEnable.SetEvent();
	  throw;
	}
   m_evDataAccEnable.SetEvent();
 }

void TChildFrame::OnSelectHigh()
 {
   if( !CriticalRun() )
	{
      MessageBeep( -1 ); return;
	}

   m_evDataAccEnable.ResetEvent();
   try {
	 if( m_pMainView && m_pMainView->GetDocument() )
	   m_pMainView->GetDocument()->SelectHigh();   
    }
   catch(...)
	{
	  m_evDataAccEnable.SetEvent();
	  throw;
	}
   m_evDataAccEnable.SetEvent();
 }
void TChildFrame::On_Edit_UnselAll()
 {
   if( !CriticalRun() )
	{
      MessageBeep( -1 ); return;
	}

   m_evDataAccEnable.ResetEvent();
   try {
     m_pMainView->GetDocument()->SelectAll( false );   
	}
   catch(...)
	{
	  m_evDataAccEnable.SetEvent();
	  throw;
	}
   m_evDataAccEnable.SetEvent();
 }
void TChildFrame::On_Edit_SelInv()
 {
   if( !CriticalRun() )
	{
      MessageBeep( -1 ); return;
	}

   m_evDataAccEnable.ResetEvent();
   try {
	 m_pMainView->GetDocument()->SelectInve();
	 RefreshDrawingPane();   
   }
   catch(...)
	{
	  m_evDataAccEnable.SetEvent();
	  throw;
	}
   m_evDataAccEnable.SetEvent();
 }

void TChildFrame::On_Edit_Copy()
 {
    if( !CriticalRun() )
	 {
       MessageBeep( -1 ); return;
	 }

    ASSERT_VALID( this );
	ASSERT( m_cfDraw != NULL );

	// Create a shared file and associate a CArchive with it
	CSharedFile file;
	CArchive ar( &file, CArchive::store );

	// Serialize selected objects to the archive
	m_pMainView->GetDocument()->WriteObjects( ar, true );
	//m_selection.Serialize(ar);
	ar.Close();

	m_evDataAccEnable.ResetEvent();

	COleDataSource* pDataSource = NULL;	
	TRY
	{
		pDataSource = new COleDataSource;
		// put on local format instead of or in addation to
		pDataSource->CacheGlobalData( m_cfDraw, file.Detach() );

		// if only one item and it is a COleClientItem then also
		// paste in that format
		/*CDrawObj* pDrawObj = m_selection.GetHead();
		if (m_selection.GetCount() == 1 &&
			pDrawObj->IsKindOf(RUNTIME_CLASS(CDrawOleObj)))
		{
			CDrawOleObj* pDrawOle = (CDrawOleObj*)pDrawObj;
			pDrawOle->m_pClientItem->GetClipboardData(pDataSource, FALSE);
		}*/
		
		pDataSource->SetClipboard();		
	}
	CATCH_ALL(e)
	{
	   delete pDataSource;						
	   m_evDataAccEnable.SetEvent();
	   THROW_LAST();
	}
	END_CATCH_ALL

   m_evDataAccEnable.SetEvent();
 }
void TChildFrame::On_Edit_ClearAll()
 {
   m_pMainView->GetDocument()->RemoveSelected( false );
 }
void TChildFrame::On_Edit_Clear()
 {
   m_pMainView->GetDocument()->RemoveSelected( true );
 }
void TChildFrame::On_Edit_Cut()
 {
   On_Edit_Copy();
   m_pMainView->GetDocument()->RemoveSelected( false );
 }
void TChildFrame::On_Edit_Paste()
 {
   COleDataObject dataObject;
   dataObject.AttachClipboard();

   if( dataObject.IsDataAvailable(m_cfDraw) )   
	  PasteNative(dataObject);
	   
   m_pMainView->GetDocument()->SetModifiedFlag();   
   m_pMainView->Invalidate( FALSE );
 }

void TChildFrame::On_Edit_SelDBL_Max()
 {
   if( m_pMainView->GetDocument()->CalcSelected( false ) != 1 )
	{
      MessageBox( "Для выделения двухполюсника надо выделить любое принадлежащее ему устройство", "Ошибка", 
	       MB_OK|MB_ICONINFORMATION );
	  return;
	}
   //m_pMainView->GetDocument()->SelectDBL();
   m_pMainView->GetDocument()->Sel_Objs( TSEL_SelDp, 5 );
 }

void TChildFrame::On_Edit_SelDBL()
 {
   if( m_pMainView->GetDocument()->CalcSelected( false ) != 1 )
	{
      MessageBox( "Для выделения двухполюсника надо выделить любое принадлежащее ему устройство", "Ошибка", 
	       MB_OK|MB_ICONINFORMATION );
	  return;
	}
   //m_pMainView->GetDocument()->SelectDBL();
   m_pMainView->GetDocument()->Sel_Objs( TSEL_SelDp, 0 );
 }

void TChildFrame::On_Edit_Scale()
 {  
   if( !CriticalRun() )
	{
      MessageBeep( -1 ); return;
	}

   TScaleDlg dlg( this, m_ptScale );
   if( dlg.DoModal() == IDOK )
	{
	  m_evDataAccEnable.ResetEvent();
	  try {
	    m_pMainView->GetDocument()->ScaleSel( dlg.m_dRes, dlg.m_iCenter );
	    RefreshDrawingPane();
	   }
      catch(...)
	   {
	     m_evDataAccEnable.SetEvent();
	     throw;
	   }
	  m_evDataAccEnable.SetEvent();
	}
 }

bool TChildFrame::HaveSelected( int iCnt )
 {
   if( iCnt == -1 )
     return m_pMainView && m_pMainView->GetDocument() &&
	   m_pMainView->GetDocument()->m_bHaveSel;
   else
	return m_pMainView && m_pMainView->GetDocument() &&
	   m_pMainView->GetDocument()->m_iCntSel >= iCnt;
 }
bool TChildFrame::HaveObjects()
 {
   return m_pMainView && m_pMainView->GetDocument() &&
	 m_pMainView->GetDocument()->RefLstDraw().size() > 0;
 }

void TChildFrame::OnUpd_Aligment_NoRun( CCmdUI* pCmdUI )
 {
   pCmdUI->Enable( !m_bFlRun && HaveSelected(2) );
 }
void TChildFrame::OnUpd_Center_NoCrit( CCmdUI* pCmdUI )
 {   
   pCmdUI->Enable( CriticalRun() && HaveObjects() );
 }
void TChildFrame::OnUpd_Back_NoRun( CCmdUI* pCmdUI )
 {
   pCmdUI->Enable( !m_bFlRun && HaveSelected() );
 }
void TChildFrame::OnUpd_Forward_NoRun( CCmdUI* pCmdUI )
 {
   pCmdUI->Enable( !m_bFlRun && HaveSelected() );
 }

void TChildFrame::On_AligmentGuid()
 {
   int m_iGSelCntV = 0, m_iGSelCntH = 0;
   TDrawRuler *p1, *p2;

   m_pMainView->GetDocument()->CountSelGuid( m_iGSelCntV, m_iGSelCntH, p1, p2 );
   if( m_iGSelCntV > 1 || m_iGSelCntH > 1 )
	{
	  MessageBox( "Для выравнивания по направляющим требуется одна выделенная вертикальная или одна горизонтальная направляющая или и то и другое", "Предупреждение", 
	       MB_OK|MB_ICONINFORMATION );
	  return;
	}
   if( !m_iGSelCntV && !m_iGSelCntH )
	{
      MessageBox( "Нет выделенных направляющих", "Предупреждение", 
	       MB_OK|MB_ICONINFORMATION );
	  return;
	}

   m_pMainView->GetDocument()->AlignSelected_AtRuler( p1, p2 );   
 }


void TChildFrame::On_Aligment()
 {
   /*LONG xx = GetDialogBaseUnits();
   WORD x = LOWORD(xx);
   WORD y = HIWORD(xx);*/

   TAlnDlg dlg( this, m_ptAlnDlgCorn, m_pMainView->GetDocument()->m_iCntSel );

   dlg.m_bAD = m_bAD_key;  
   dlg.m_iAln_H = m_iAln_H_key;  
   dlg.m_iAln_V = m_iAln_V_key;     
   dlg.m_iDistr_HV = m_iDistr_HV_key;  
   

   if( dlg.DoModal() == IDOK )
	{
      UpdateWindow();

	  if( dlg.m_bAD == 0 )
        m_pMainView->GetDocument()->AlignSelection( (TAlnH)dlg.m_iAln_H, (TAlnV)dlg.m_iAln_V );
	  else
	    m_pMainView->GetDocument()->DistribSelection( (TDistrHV)dlg.m_iDistr_HV );

	  m_bAD_key = dlg.m_bAD;
	  m_iAln_H_key = dlg.m_iAln_H;
	  m_iAln_V_key = dlg.m_iAln_V;
	  m_iDistr_HV_key = dlg.m_iDistr_HV;
	}   

 }
void TChildFrame::On_Center()
 {  
   if( !CriticalRun() )
	{
      MessageBeep( -1 ); return;
	}

   m_evDataAccEnable.ResetEvent();
   try {
     m_pMainView->GetDocument()->CenterDraw();
     m_pMainView->Invalidate( FALSE );
	}
   catch(...)
	{
	  m_evDataAccEnable.SetEvent();
	  throw;
	}
   m_evDataAccEnable.SetEvent();
 }
void TChildFrame::On_Back()
 {
   m_pMainView->GetDocument()->ZSelection( false );
 }
void TChildFrame::On_Forward()
 {
   m_pMainView->GetDocument()->ZSelection( true );
 }

void TChildFrame::On_FillLM()
 {
   m_pMainView->GetDocument()->FillRanges();
 }

void TChildFrame::On_CreRng()
 {
   if( m_pMainView->GetDocument() )
	{
      m_pMainView->GetDocument()->CreRng();
      RefreshDrawingPane();
	}
 }

void TChildFrame::CallDrawView_Func( TPtrDrwView_Func pF, bool bParam )
 {
   CWnd *pW = NULL;

   if( m_wndSplitter1.GetColumnCount() == 2 && 
	   m_wndSplitter1.GetPane(0,1) 
	 )	
	  pW = m_wndSplitter1.GetPane( 0, 1 );	
   else
	if( m_wndSplitter1.GetColumnCount() == 1 && 
	   m_wndSplitter1.GetPane(0,0) 
	 )	
	  pW = m_wndSplitter1.GetPane( 0, 0 );

	ASSERT( typeid(*pW) == typeid(TDiplomView) );

	(reinterpret_cast<TDiplomView*>(pW)->*pF)( bParam );	 
 }

void TChildFrame::RefreshDrawingPane()
 {
   if( m_wndSplitter1.GetColumnCount() == 2 && 
	   m_wndSplitter1.GetPane(0,1) 
	 )
	 m_wndSplitter1.GetPane( 0, 1 )->Invalidate( FALSE );
   else
	if( m_wndSplitter1.GetColumnCount() == 1 && 
	   m_wndSplitter1.GetPane(0,0) 
	 )
	 m_wndSplitter1.GetPane( 0, 0 )->Invalidate( FALSE );
 }

void TChildFrame::On_ViewBrig()
 {
   m_optX.m_bFlBrig = !m_optX.m_bFlBrig;

   CRect r;  m_wndSplitter1.GetClientRect( r );
   if( m_optX.m_bFlBrig && m_wndSplitter1.GetColumnCount() == 1 )
	 m_wndSplitter1.SplitColumn( m_wndSplitter1.m_dSplit * r.Width() ),
     m_optX.m_bFlElem = true;
	 
   //else if( !m_optX.m_bFlBrig && m_wndSplitter1.GetColumnCount() == 2 )
	 //m_wndSplitter1.MakeSplit2( m_optX.m_bFlBrig );

   m_wndSplitter1.MakeSplit2( m_optX.m_bFlBrig );
 }

void TChildFrame::On_ViewElements()
 {
   m_optX.m_bFlElem = !m_optX.m_bFlElem;

   if( m_optX.m_bFlElem )
	{
	  if( m_wndSplitter1.GetColumnCount() == 1 )
	   {
         CRect r;  m_wndSplitter1.GetClientRect( r );
		 m_wndSplitter1.SplitColumn( m_wndSplitter1.m_dSplit * r.Width() );		 
	   }	  	  
	}
   else if( !m_optX.m_bFlElem && m_wndSplitter1.GetColumnCount() == 2 )
	{
	  m_wndSplitter1.DeleteColumn( 0 );   
	  m_wndSplitter1.SetActivePane( 0, 0 );
	}
 }
void TChildFrame::On_Solve()
 {
   //m_optX. = !m_optX.;
 }
void TChildFrame::On_StepGrid()
 {
   m_optX.m_bFlStepOnGrid = !m_optX.m_bFlStepOnGrid;
 }
void TChildFrame::On_StepGuid()
 {
   m_optX.m_bFlStepOnGuid = !m_optX.m_bFlStepOnGuid;
 }

bool TChildFrame::DeniedProp()
 {
   if( !HaveSelected() )
	{
      MessageBeep( -1 ); return true;
	}
   return false;
 }

//--------------------------------
void TChildFrame::OnFont()
 {
   if( DeniedProp() ) return;

   TDiplomDoc *pDoc = m_pMainView->GetDocument();

   TAP_PropEdit_Vec vec;
   pDoc->CreateVectorProp( vec );
   TPropEdit *pEd = pDoc->VecGetProp( vec, TDR_Font );
	
   if( !pEd )
	{
      MessageBeep( -1 );
	  return;
	}

   ASSERT( pEd->m_ttEdit.m_ap.get() != NULL );
   TFontDrTool *p = (TFontDrTool*)pEd->m_ttEdit.m_ap.get();   

   CFontDialog dlgF( &p->Data()->m_lf, CF_EFFECTS|CF_SCREENFONTS, NULL, this );
   dlgF.m_cf.rgbColors = p->Data()->m_clr;
   if( dlgF.DoModal() == IDOK )
	{
      dlgF.GetCurrentFont( &p->Data()->m_lf );	  
	  p->Data()->m_clr = dlgF.m_cf.rgbColors;
	  
	  pDoc->AssNewProp( vec, TDR_Font );	  
	}
 }
void TChildFrame::OnFntClr()
 {
   if( DeniedProp() ) return;

   TDiplomDoc *pDoc = m_pMainView->GetDocument();

   TAP_PropEdit_Vec vec;
   pDoc->CreateVectorProp( vec );
   TPropEdit *pEd = pDoc->VecGetProp( vec, TDR_Font );
	
   if( !pEd )
	{
      MessageBeep( -1 );
	  return;
	}

   ASSERT( pEd->m_ttEdit.m_ap.get() != NULL );
   TFontDrTool *p = (TFontDrTool*)pEd->m_ttEdit.m_ap.get();

   m_ap_Clr->SetIter( p->Data()->m_clr );
   p->Data()->m_clr = m_ap_Clr->Next();
   
   pDoc->AssNewProp( vec, TDR_Font );
 }
void TChildFrame::OnAlign() 
 {
   if( !HaveSelected(2) )
	{
      MessageBeep( -1 ); return;
	}

   On_Aligment();
 }

void TChildFrame::OnLine() 
 {
   if( DeniedProp() ) return;

   TDiplomDoc *pDoc = m_pMainView->GetDocument();

   TAP_PropEdit_Vec vec;
   pDoc->CreateVectorProp( vec );

   TPropEdit *pEd = pDoc->VecGetProp( vec, TDR_Pen );
   if( !pEd )
	{
      MessageBeep( -1 ); return;
	}

   MakePropDialog( TH_Line, vec );
 }
void TChildFrame::OnLinClr()
 {
   if( DeniedProp() ) return;
   
   TDiplomDoc *pDoc = m_pMainView->GetDocument();

   TAP_PropEdit_Vec vec;
   pDoc->CreateVectorProp( vec );
   TPropEdit *pEd = pDoc->VecGetProp( vec, TDR_Pen );
	
   if( !pEd )
	{
      MessageBeep( -1 );
	  return;
	}

   ASSERT( pEd->m_ttEdit.m_ap.get() != NULL );
   TPenDrTool *p = (TPenDrTool*)pEd->m_ttEdit.m_ap.get();

   m_ap_Clr->SetIter( p->Data()->m_clr );
   p->Data()->m_clr = m_ap_Clr->Next();

   pDoc->AssNewProp( vec, TDR_Pen );
 }
void TChildFrame::OnLinThick()
 {
   if( DeniedProp() ) return;

   TDiplomDoc *pDoc = m_pMainView->GetDocument();

   TAP_PropEdit_Vec vec;
   pDoc->CreateVectorProp( vec );
   TPropEdit *pEd = pDoc->VecGetProp( vec, TDR_Pen );
	
   if( !pEd )
	{
      MessageBeep( -1 );
	  return;
	}

   ASSERT( pEd->m_ttEdit.m_ap.get() != NULL );
   TPenDrTool *p = (TPenDrTool*)pEd->m_ttEdit.m_ap.get();

   m_apLine_Thick->SetIter( p->Data()->m_iWidth );
   p->Data()->m_iWidth = m_apLine_Thick->Next();

   pDoc->AssNewProp( vec, TDR_Pen );
 }
void TChildFrame::OnLinTyp()
 {
   if( DeniedProp() ) return;

   TDiplomDoc *pDoc = m_pMainView->GetDocument();

   TAP_PropEdit_Vec vec;
   pDoc->CreateVectorProp( vec );
   TPropEdit *pEd = pDoc->VecGetProp( vec, TDR_Pen );
	
   if( !pEd )
	{
      MessageBeep( -1 );
	  return;
	}

   ASSERT( pEd->m_ttEdit.m_ap.get() != NULL );
   TPenDrTool *p = (TPenDrTool*)pEd->m_ttEdit.m_ap.get();

   m_apLine_Pattern->SetIter( p->Data()->m_iStyle );
   p->Data()->m_iStyle = m_apLine_Pattern->Next();

   pDoc->AssNewProp( vec, TDR_Pen );
 }

void TChildFrame::OnFil()
 {
   if( DeniedProp() ) return;   

   TDiplomDoc *pDoc = m_pMainView->GetDocument();

   TAP_PropEdit_Vec vec;
   pDoc->CreateVectorProp( vec );

   TPropEdit *pEd = pDoc->VecGetProp( vec, TDR_Brush );
   if( !pEd )
	{
      MessageBeep( -1 ); return;
	}

   MakePropDialog( TH_Fill, vec );
 }
void TChildFrame::OnFilClr()
 {
   if( DeniedProp() ) return;

   TDiplomDoc *pDoc = m_pMainView->GetDocument();

   TAP_PropEdit_Vec vec;
   pDoc->CreateVectorProp( vec );
   TPropEdit *pEd = pDoc->VecGetProp( vec, TDR_Brush );
	
   if( !pEd )
	{
      MessageBeep( -1 );
	  return;
	}

   ASSERT( pEd->m_ttEdit.m_ap.get() != NULL );
   TBrushDrTool *p = (TBrushDrTool*)pEd->m_ttEdit.m_ap.get();

   m_ap_Clr->SetIter( p->Data()->m_clr );
   p->Data()->m_clr = m_ap_Clr->Next();

   pDoc->AssNewProp( vec, TDR_Brush );
 }
void TChildFrame::OnFilTyp()
 {
   if( DeniedProp() ) return;

   TDiplomDoc *pDoc = m_pMainView->GetDocument();

   TAP_PropEdit_Vec vec;
   pDoc->CreateVectorProp( vec );
   TPropEdit *pEd = pDoc->VecGetProp( vec, TDR_Brush );
	
   if( !pEd )
	{
      MessageBeep( -1 );
	  return;
	}

   ASSERT( pEd->m_ttEdit.m_ap.get() != NULL );
   TBrushDrTool *p = (TBrushDrTool*)pEd->m_ttEdit.m_ap.get();

   int ind;
   if( p->Data()->m_solid == BS_HATCHED ) ind = p->Data()->m_iIndex;
   else if( p->Data()->m_solid == BS_SOLID ) ind = -1;
   else ind = -2;

   m_apFill_Pattern->SetIter( ind );
   ind = m_apFill_Pattern->Next();

   if( ind == -1 ) 
	p->Data()->m_solid = BS_SOLID,
	p->Data()->m_iIndex = HS_CROSS;
   else if( ind == -2 ) 
	 p->Data()->m_solid = BS_NULL,
	 p->Data()->m_iIndex = HS_CROSS;
   else p->Data()->m_solid = BS_HATCHED,
	 p->Data()->m_iIndex = ind;

   pDoc->AssNewProp( vec, TDR_Brush );
 }

void TChildFrame::OnUpdate_ClcRun( CCmdUI* pCmdUI )
 {
   pCmdUI->Enable( !m_bFlRun && HaveObjects() );
 }
void TChildFrame::OnClcRun()
 {   
   if( m_pMainView->GetDocument()->IsModified() )
	{
	  CString str;
	  str.Format( "Документ [%s] модифицирован. При расчёте схема будет уничтожена. Сохранить ?",
	    (LPCSTR)m_pMainView->GetDocument()->GetPathName() );
	  if( MessageBox(str, "Предупреждение", MB_YESNO|MB_ICONQUESTION) == IDYES )
	     m_pMainView->GetDocument()->OnSaveDocument( m_pMainView->GetDocument()->GetPathName() );
	}

   m_apAnalyzer = 
	 auto_ptr<TAnalyzer>( 
	     new TAnalyzer(m_pMainView->GetDocument()->RefLstDraw(), m_pMainView->GetDocument()) 
	   );
   CWaitCursor wc;

   try {	  
	  m_apAnalyzer->Create();
	}
   catch( TGrafExc exc )
	{
	  wc.Restore();
      m_pMainView->GetDocument()->ReportExcStat( exc );
	  m_apAnalyzer = auto_ptr<TAnalyzer>( NULL );
	  return;
	}
   wc.Restore();

   if( m_optX.m_bFlModeBrig )
	    StopBrigMode();

   m_thStart.m_pFn = &TChildFrame::CalcRun;
   m_apCD = auto_ptr<TCalcData>( new TCalcData() );
   m_pMainView->ResetupBar( 1 );
   m_dbarHor.CheckDlgButton( ICM_ED_STEPMODE, 1 );

   m_evCalcActive.ResetEvent(); m_evCalcStop.ResetEvent();
   m_evCalcNotActive.SetEvent();

   m_evSingleStep.ResetEvent();
   m_evSingleStepMode.ResetEvent();
   m_evDataAccEnable.SetEvent();	
   
   m_bFlRun = true;
   TMyThrd *pThrd = new TMyThrd( ThreadProc, (LPVOID)this, &m_evCalcActive, &m_evCalcNotActive, m_hWnd, globl_iMsgEndRun );
   if( !pThrd->CreateThread() )
	{
      delete pThrd;
	  ASSERT( FALSE );
	  m_bFlRun = false;
	  return;
	}   
   //CalcRun();//lllll
 }

void TChildFrame::OnUpdate_TSimpl( CCmdUI* pCmdUI )
 {
   pCmdUI->Enable( !m_bFlRun && m_pMainView->GetDocument()->m_iCntSel == 1 );
 }
void TChildFrame::OnUpdate_ClcType( CCmdUI* pCmdUI )
 {
   pCmdUI->Enable( CriticalRun() && m_pMainView->GetDocument()->m_iCntSel == 1 );
 }

void TChildFrame::OnClcInt()
 {
   if( !CriticalRun() )
	{
      MessageBeep( -1 ); return;
	}

   TDrawShape *pShp;
   int iC = m_pMainView->GetDocument()->CalcSelectedAndGet( false, pShp );
   if( iC != 1 || pShp->isA() != TT_PC )
	{
      MessageBox( "Для определения пересечения требуется выделить одно устройство", "Ошибка", 
	       MB_OK|MB_ICONEXCLAMATION );
	  return;
	}

   m_evDataAccEnable.ResetEvent();
   try {
     m_pMainView->GetDocument()->ShowInt( pShp );
	}
   catch(...)
	{
	  m_evDataAccEnable.SetEvent();
	  throw;
	}
   m_evDataAccEnable.SetEvent();
 }

void TChildFrame::OnNPaths()
 {
   if( !CriticalRun() )
	{
      MessageBeep( -1 ); return;
	}

   TDrawShape *pShp;
   int iC = m_pMainView->GetDocument()->CalcSelectedAndGet( false, pShp );
   if( iC != 1 || (pShp->isA() != TT_Cnct && pShp->isA() != TT_Point) )
	{
      MessageBox( "Требуется выделить одну связь или точку соединения", "Ошибка", 
	       MB_OK|MB_ICONEXCLAMATION );
	  return;
	}

   m_evDataAccEnable.ResetEvent();
   try {
     m_pMainView->GetDocument()->ShowNPaths( pShp );
	}
   catch(...)
	{
	  m_evDataAccEnable.SetEvent();
	  throw;
	}
   m_evDataAccEnable.SetEvent();
 }
void TChildFrame::OnARNG()
 {
   
   /*m_apChat = auto_ptr<CVtChart>( new CVtChart() );
   CRect r( 0, 0, 600, 500 );
   m_apChat->Create( NULL, WS_CHILD|WS_VISIBLE, r, m_pMainView, 991 );	
   m_apChat->ToDefaults();
   //m_apChat->SetChart3d( TRUE );
   //m_apChat->ShowWindow( SW_SHOW );
   //m_apChat->SetChartType( VtChChartType3dBar )
   m_apChat->SetChartType( 0 );
   //m_apChat->SetRandomFill( FALSE );
   
   m_apChat->SetColumnCount( 20 );
   //m_apChat->SetRowCount( 10 );
   //m_apChat->SetColumnLabelCount( 2 );
   //m_apChat->SetRowLabelCount( 3 );

   //LPDISPATCH lpDSP = m_apChat->GetDataGrid();

   m_apChat->SetColumn( 2 );
   m_apChat->SetColumnLabelIndex( 2 );

   m_apChat->SetRow( 2 );
   m_apChat->SetRowLabelIndex( 2 );

   m_apChat->SetColumnLabel( "My Label Col" );
   m_apChat->SetRowLabel( "My Label Row" );
   
   m_apChat->SetData( "100" );

   m_apChat->SetActiveSeriesCount( 5 );
   */
 }
void TChildFrame::OnSofistSimplify()
 {
   TDrawShape *pShp;
   int iC = m_pMainView->GetDocument()->CalcSelectedAndGet( false, pShp );
   if( iC != 1 || pShp->isA() != TT_PC )
	{
      MessageBox( "Для сведения требуется выделить одно устройство", "Ошибка", 
	       MB_OK|MB_ICONEXCLAMATION );
	  return;
	}

   CWaitCursor wc;
   TAnalyzer an( m_pMainView->GetDocument()->RefLstDraw(), m_pMainView->GetDocument() ); 
   
   try {	  
	  an.Create();
	}
   catch( TGrafExc exc )
	{
	  wc.Restore();
      m_pMainView->GetDocument()->ReportExcStat( exc );	  
	  return;
	}

   m_apCD = auto_ptr<TCalcData>( new TCalcData() );
   m_evDataAccEnable.SetEvent();
   m_evCalcStop.ResetEvent();

   try {
      GlobalSimplify( &an, true, false, ((TDrawPC*)pShp)->m_pTPC );	   
	}
   catch( CException *pE )
	{
	  if( typeid(*pE) == typeid(TCancelExc) )
	   {
         m_apCD->m_shErr = TCA_Cancel;		 
		 m_apCD->m_csErr = "Прерывание пользователем";
	   }
	  else
	   {
         m_apCD->m_shErr = TCA_Common;
		 LPSTR lp = m_apCD->m_csErr.GetBuffer( 1024 );
		 pE->GetErrorMessage( lp, 1023 );
		 lp[ 1023 ] = 0;
		 m_apCD->m_csErr.ReleaseBuffer();
	   }

	  pE->Delete();
	}
   catch( exception e )
	{
	  m_apCD->m_shErr = TCA_Common;	
	  m_apCD->m_csErr = e.what();
	}
   catch( TGrafExc ex )
	{
	  m_apCD->m_shErr = TCA_Common;	
      TDiplomDoc::Get_GrafErr( ex, m_apCD->m_csErr );
	}

   m_evDataAccEnable.SetEvent();
   Analyze_Run();   
 }//ppppppppppp

void TChildFrame::OnTSimplify()
 {
   TDrawShape *pShp;
   int iC = m_pMainView->GetDocument()->CalcSelectedAndGet( false, pShp );
   if( iC != 1 || pShp->isA() != TT_PC )
	{
      MessageBox( "Для сведения требуется выделить одно устройство", "Ошибка", 
	       MB_OK|MB_ICONEXCLAMATION );
	  return;
	}

   m_pMainView->GetDocument()->SimplifyT( pShp );
 }

void TChildFrame::OnClcType()
 {
   if( !CriticalRun() )
	{
      MessageBeep( -1 ); return;
	}

   TDrawShape *pShp;
   int iC = m_pMainView->GetDocument()->CalcSelectedAndGet( false, pShp );
   if( iC != 1 || pShp->isA() != TT_PC )
	{
      MessageBox( "Для определения типа связи требуется выделить одно устройство", "Ошибка", 
	       MB_OK|MB_ICONEXCLAMATION );
	  return;
	}

   m_evDataAccEnable.ResetEvent();
   try {
     m_pMainView->GetDocument()->ShowCnctType( pShp );
	}
   catch(...)
	{
	  m_evDataAccEnable.SetEvent();
	  throw;
	}
   m_evDataAccEnable.SetEvent();
 }

void TChildFrame::OnUpdate_Connect( CCmdUI* pCmdUI )
 {
   pCmdUI->Enable( !m_bFlRun && HaveObjects() );
 }
void TChildFrame::On_Connect()
 {
   short res = m_pMainView->GetDocument()->CheckConnect();

   if( res == 0 )
	{
	  m_pMainView->UpdateWindow();
      MessageBox( "Имеются оторванные объекты", "Ошибка", 
	       MB_OK|MB_ICONEXCLAMATION );
	}
   else if( res == 1 )
	 MessageBox( "Тест прошёл успешно", "Сообщение", 
	       MB_OK|MB_ICONINFORMATION );
 }

void TChildFrame::OnUpdate_EqRest( CCmdUI* pCmdUI )
 {
   CRowListView *pLst = GetBrigList();
   if( !pLst || m_bFlRun ) 
	{
      pCmdUI->Enable( FALSE ); 
	  return;
	}
   
   pCmdUI->Enable( pLst->m_nCheckedItem != -1 );    
 }//?????????

void TChildFrame::On_EqRest()
 {
   CRowListView *pLst = GetBrigList();
   if( !pLst || pLst->m_nCheckedItem < 0 ) return;      

   CString str = pLst->GetListCtrl().GetItemText( pLst->m_nCheckedItem, 0 );
   LPSTR ptr;
   DWORD dw = strtoul( (LPCSTR)str, &ptr, 10 );

   TBrigada *p = m_pMainView->GetDocument()->GetBrigPtr( dw );
   ASSERT( p != NULL );

   str = "";
   if( !p->CheckAllData(str) )
	{
      MessageBox( (LPCSTR)str, "Ошибка", 
	       MB_OK|MB_ICONEXCLAMATION );
	  return;
	}

   m_thStart.m_pFn = &TChildFrame::CalcEQRest;   
   m_apCD = auto_ptr<TCalcData>( (TCalcData*)new TCalc_EQRestore(p) );

   StartCalc();
 }

LONG TChildFrame::OnNewStatusStr( UINT, LONG )
 {
   CSingleLock lock( &m_mtxMsg, TRUE );   
   ((CFrameWnd*)AfxGetApp()->m_pMainWnd)->SetMessageText( strMsgSimpl.get() );
   return 0;
 }

LONG TChildFrame::OnEndRun( UINT, LONG )
 {         
   m_evDataAccEnable.SetEvent();	

   Analyze_Run();
   m_bFlRun = false;
   m_pMainView->ResetupBar( 0 );
   return 0;
 }

LONG TChildFrame::OnEndCalc( UINT, LONG )
 {   
   m_evDataAccEnable.SetEvent();	

   if( !m_bFlCalc )
	{
	  m_bFlCalc = true;
	  CButton *p = (CButton*)m_dbarHor.GetDlgItem( ICM_ED_STOP );
	  auto_ptr<CBitmap> apTmp( new CBitmap() );
	  apTmp->LoadBitmap( IDB_BMP_ED_CALC );
	  p->SetBitmap( *apTmp );
	  m_bmpCalc = apTmp;
	  
	  //pCmdUI->Enable( FALSE );
	  p->SetDlgCtrlID( ICM_ED_CALC );

	  AnalyzeCD();
	}

   return 0;
 }

void TChildFrame::OnUpdate_Stop( CCmdUI* pCmdUI )
 {
 }
void TChildFrame::OnStop()
 {
   if( m_bFlRun ) m_evCalcStop.SetEvent();
 }
void TChildFrame::On_SinglStep()
 {
   if( m_bFlRun ) m_evSingleStep.SetEvent();
 }
void TChildFrame::On_StepMode()
 {
   if( !m_bFlRun ) return;

   bool bFl;
   if( ::WaitForSingleObject(m_evSingleStepMode, 0) == WAIT_TIMEOUT )
	 m_evSingleStepMode.SetEvent(), bFl = false;
   else m_evSingleStepMode.ResetEvent(), bFl = true;

   m_dbarHor.CheckDlgButton( ICM_ED_STEPMODE, bFl );
 }

void TChildFrame::OnUpdate_Calc( CCmdUI* pCmdUI )
 {   		 
 }

void TChildFrame::Analyze_Run()
 {
 //llllllllll
   if( m_apCD.get() )
	{	  
      if( m_apCD->m_shErr != TCA_No )
	   {
	     ((CFrameWnd*)AfxGetApp()->m_pMainWnd)->SetMessageText( "Преобразование прервано" );
         MessageBox( (LPCSTR)m_apCD->m_csErr, 
		   m_apCD->m_shErr == TCA_Cancel ? "Сообщение":"Ошибка",
	       MB_OK|MB_ICONEXCLAMATION );
	   }
	  else
	    ((CFrameWnd*)AfxGetApp()->m_pMainWnd)->SetMessageText( "Преобразование закончено" );

	  m_apCD = auto_ptr<TCalcData>( NULL );
	}
   if( m_apAnalyzer.get() )
	 m_apAnalyzer = auto_ptr<TAnalyzer>( NULL );   

   m_pMainView->GetDocument()->SetModifiedFlag( FALSE );
 }

void TChildFrame::AnalyzeCD()
 {
   if( m_apCD.get() )
	{
	  m_apCD->m_pTxt->Invalidate();
      m_apCD->m_pTxt->BoundsRound();	  

      if( m_apCD->m_shErr != TCA_No )
	   {
         MessageBox( (LPCSTR)m_apCD->m_csErr, 
		   m_apCD->m_shErr == TCA_Cancel ? "Сообщение":"Ошибка",
	       MB_OK|MB_ICONEXCLAMATION );
	   }

	  m_apCD = auto_ptr<TCalcData>( NULL );
	}
 }

BOOL TChildFrame::MyCanClose()
 {
   CSingleLock lock( &m_evCalcActive );
   if( lock.Lock(0) )
	{
      MessageBox( "Перед закрытием нужно прервать вычисления", "Ошибка", 
	     MB_OK|MB_ICONEXCLAMATION );
	  return FALSE;
	}
   return TRUE;
 }

void TChildFrame::OnCalc()
 {     
   CRect r;
   CWnd *p = m_dbarHor.GetDlgItem( ICM_ED_CALC );
   if( !p ) return;
   p->GetWindowRect( r );
   m_pMainView->ScreenToClient( r );
   CPoint pt( r.left, r.bottom );
   m_pMainView->ShowContextMenu( pt, 1 );


   //ID_CLC_EQREST
 }

TListStensil::~TListStensil()
 {
   TermDrag();
 }



int TListStensil::OnCreate( LPCREATESTRUCT lpC )
 {
   if( CListCtrl::OnCreate(lpC) == -1 )
	  return -1;	 

   m_apImageListDrag = auto_ptr<CImageList>( NULL );
   m_apImgLst = auto_ptr<CImageList>( new CImageList() );

   ASSERT( m_pStnsl  != NULL );

   CBitmap bmBmp;
   bmBmp.LoadBitmap( m_pStnsl[0].m_uiIdBmp );      
	  
   BITMAP bm;
   ::GetObject( (HBITMAP)bmBmp, sizeof(bm), &bm );

   m_iMinW = bm.bmWidth;
   m_iMinH = bm.bmHeight;

   m_apImgLst->Create( bm.bmWidth, bm.bmHeight, 
	                ILC_MASK | ILC_COLOR4, 20, 5 );

   m_apImgLst->Add( &bmBmp, RGB(0,0,128) );	     

   for( int i = 1; m_pStnsl[i].m_uiIdBmp && m_pStnsl[i].m_lpName; ++i )
	{
	  CBitmap bmBmp;
      bmBmp.LoadBitmap( m_pStnsl[i].m_uiIdBmp );   

      m_apImgLst->Add( &bmBmp, RGB(0,0,128) );	  	  
	}      
   m_apImgLst->SetBkColor( CLR_NONE );
   
   SetImageList( m_apImgLst.get(), LVSIL_NORMAL );
   for( i = 0; m_pStnsl[i].m_uiIdBmp && m_pStnsl[i].m_lpName; ++i )
	 InsertItem( i, m_pStnsl[i].m_lpName, i );
   //m_lstFold.SetImageList( m_ilstSmall(), LVSIL_SMALL );  
   ModifyStyle( 0, LVS_ICON );

   m_hcDisbl = AfxGetApp()->LoadStandardCursor( IDC_NO );   

   //UpdateWindow();

   return 0;
 }

void TListStensil::OnBeginDrag( LPNMHDR pnmhdr, LRESULT *pResult )
 {
	CPoint			ptItem, ptAction, ptImage;
	NM_LISTVIEW		*pnmListView = (NM_LISTVIEW *)pnmhdr;

	ASSERT( m_bDragging==false );
	m_bDragging = true;
	m_iItemDrag = pnmListView->iItem;
	ptAction = pnmListView->ptAction;
	GetItemPosition(m_iItemDrag, &ptItem);  // ptItem is relative to (0,0) and not the view origin
	GetOrigin(&m_ptOrigin);

	//ASSERT(m_apImageListDrag.get() == NULL);
	m_apImageListDrag = auto_ptr<CImageList>( CreateDragImage(m_iItemDrag, &ptImage) );
	//m_apImageListDrag = auto_ptr<CImageList>( m_apImgLst.get() );
	  
	m_sizeDelta = ptAction - ptImage;   // difference between cursor pos and image pos
	m_ptHotSpot = ptAction - ptItem + m_ptOrigin;  // calculate hotspot for the cursor
	m_apImageListDrag->DragShowNolock( TRUE );  // lock updates and show drag image
	m_apImageListDrag->SetDragCursorImage( 0, CPoint(0,0) );  // define the hot spot for the new cursor image
	m_apImageListDrag->BeginDrag( 0, CPoint(0, 0) );
	
	ptAction -= m_sizeDelta;
	m_apImageListDrag->DragEnter( this, ptAction );
	m_apImageListDrag->DragMove( ptAction );  // move image to overlap original icon

	SetCapture();	
	m_hcPriv = SetCursor( m_hcEmpty );
	m_bLeave = m_bCursDisbl = false;
	 
	m_hLast = m_hwPriv = NULL;
 }

LRESULT TListStensil::SendAcceptMsg( HWND hw )
 {
   m_hLast = hw;

   if( hw != m_hwPriv )
	{
	  if( m_hwPriv )
	    ::SendMessage( m_hwPriv, globl_iMsgTestAcceptDrag, MSGA_LEAVE, 0 );
	  if( hw == m_hWnd ) 
	   {
         m_hwPriv = NULL; return 0L;   
	   }
      m_hwPriv = hw;
	  return ::SendMessage( hw, globl_iMsgTestAcceptDrag, MSGA_ENTER, m_iItemDrag );
	}

   if( hw == m_hWnd ) return 0L;
   return ::SendMessage( hw, globl_iMsgTestAcceptDrag, MSGA_DRAG, 0 );
 }

void TListStensil::OnMouseMove( UINT nFlags, CPoint point )
 {	
	if( m_bDragging )
	{
	  //ASSERT( m_apImageListDrag.get() != NULL );
	  CPoint pt( point ); ClientToScreen( &pt );
	  HWND hw = ::WindowFromPoint( pt );
	  if( hw )
	   {	     
		 if( !SendAcceptMsg(hw) && hw != m_hWnd )
		  {
		    if( m_bCursDisbl == false )
		      SetCursor( m_hcDisbl ), m_bCursDisbl = true;
		  }
		 else if( m_bCursDisbl == true )
		   SetCursor( m_hcEmpty ), m_bCursDisbl = false;
	   }

      if( hw == m_hWnd )
	   {
	     if( m_bLeave ) m_bLeave = false, 
		   m_apImageListDrag->DragEnter( this, point - m_sizeDelta );
	     m_apImageListDrag->DragMove(point - m_sizeDelta);  // move the image		
	   }
	  else if( !m_bLeave ) m_bLeave = true, 
	    m_apImageListDrag->DragLeave( this );	   
	}

	CListCtrl::OnMouseMove( nFlags, point );
 }

void TListStensil::OnLButtonUp( UINT nFlags, CPoint point )
 {  
    if( m_bDragging )
	 {
	   //CPoint pt; GetCursorPos( &pt );
	   //HWND hw = ::WindowFromPoint( pt );
       ::SendMessage( m_hLast, globl_iMsgTestAcceptDrag, MSGA_ACCEPT, 0 );
	 }
    TermDrag();

	m_hLast = NULL;

	CListCtrl::OnLButtonUp(nFlags, point);
 }

void TListStensil::OnCancelMode()
 {   
   if( m_bDragging )
	{
	  //CPoint pt; GetCursorPos( &pt );
	  //HWND hw = ::WindowFromPoint( pt );
      ::SendMessage( m_hLast, globl_iMsgTestAcceptDrag, MSGA_LEAVE, 0 );
	}

   TermDrag();

   CListCtrl::OnCancelMode();
 }

void TListStensil::TermDrag()
 {
   if( m_bDragging )
	{
      m_bDragging = false;

	  ASSERT( m_apImageListDrag.get() != NULL );

	  if( !m_bLeave ) m_apImageListDrag->DragLeave(this),
	    m_bLeave = false;
	  m_apImageListDrag->EndDrag();
	  if( m_apImageListDrag.get() )
	    delete m_apImageListDrag.release();											
	  m_apImageListDrag = auto_ptr<CImageList>( NULL );

	  ::ReleaseCapture();
	  SetCursor( m_hcPriv );
	}
 }

void TChildFrame::PageSizeChanged()
 {   	
   CWnd *pW = NULL;

   if( m_wndSplitter1.GetColumnCount() == 2 && 
	   m_wndSplitter1.GetPane(0,1) 
	 )	
	  pW = m_wndSplitter1.GetPane( 0, 1 );	
   else
	if( m_wndSplitter1.GetColumnCount() == 1 && 
	   m_wndSplitter1.GetPane(0,0) 
	 )	
	  pW = m_wndSplitter1.GetPane( 0, 0 );

	ASSERT( typeid(*pW) == typeid(TDiplomView) );

	reinterpret_cast<TDiplomView*>(pW)->GetDocument()->ComputePageSize();
	m_optX.m_szPage = reinterpret_cast<TDiplomView*>(pW)->GetDocument()->GetSize();
	reinterpret_cast<TDiplomView*>(pW)->SetPageSize( m_optX.m_szPage, true );
 }

void TChildFrame::SerializeEMM( CArchive& ar )
 {
   auto_ptr<TChfOpt> apOpt( new TChfOpt() );

   if( ar.IsStoring() )
	 {
	   memset( apOpt.get(), 0, sizeof(TChfOpt) );
	   ar.Write( apOpt.get(), sizeof(TChfOpt) );
	 }
	else
	 {	// loading code
	   ar.Read( apOpt.get(), sizeof(TChfOpt) );		
	 }
 }

void TChildFrame::Serialize( CArchive& ar ) 
 {
	if( ar.IsStoring() )
	 {	// storing code
	   /*int cx, cxM;
	   m_wndSplitter1.GetColumnInfo( 0, cx, cxM )
       m_optX.m_dSplit1 =*/

	   ar.Write( &m_optX, sizeof(m_optX) );
	 }
	else
	 {	// loading code
	   ar.Read( &m_optX, sizeof(m_optX) );		
	 }
 }


void TChildFrame::PasteNative( COleDataObject& dataObject )
 { 
	// get file refering to clipboard data
    m_pMainView->GetDocument()->SelectAll( false );

	auto_ptr<CFile> apFile( dataObject.GetFileData(m_cfDraw) );
	if( apFile.get() == NULL ) return;

	// connect the file to the archive
	CArchive ar( apFile.get(), CArchive::load );
	TRY
	{
		ar.m_pDocument = m_pMainView->GetDocument(); // set back-pointer in archive

		// read the selection
		//m_selection.Serialize(ar);
		m_pMainView->GetDocument()->ReadObjects( ar );		
		m_pMainView->GetDocument()->RestoreGluPtrs();
	}
	CATCH_ALL(e)
	{
		ar.Close();		
		THROW_LAST();
	}
	END_CATCH_ALL

	ar.Close();
 }

void TChildFrame::OnSize(UINT nType, int cx, int cy) 
 {
	CMDIChildWnd::OnSize(nType, cx, cy);

	if( nType == SIZE_MAXIMIZED ) m_bMax_KEY = true;
	else if( nType == SIZE_RESTORED && m_bMax_KEY )	
	  m_wndSplitter1.RecalcLayout(), m_bMax_KEY = false;
	else m_bMax_KEY = false;

	/*if( IsWindowVisible() ) 
	  ShowWindow_Init( true, 0 );*/
	
	// TODO: Add your message handler code here	
 }

void TChildFrame::OnWindowPosChanged( WINDOWPOS FAR* lp ) 
 {
	CMDIChildWnd::OnWindowPosChanged( lp );

	if( lp->flags & SWP_SHOWWINDOW )
	  ShowWindow_Init( true, 0 );	
	// TODO: Add your message handler code here
	
 }

void TChildFrame::On_Property()
 {
   if( !CriticalRun() )
	{
      MessageBeep( -1 ); return;
	}

   TDiplomDoc *pDoc = m_pMainView->GetDocument();

   TAP_PropEdit_Vec vec;
   m_evDataAccEnable.ResetEvent();
   try {
     pDoc->CreateVectorProp( vec );
	}
   catch(...)
	{
	  m_evDataAccEnable.SetEvent();
	  throw;
	}
   m_evDataAccEnable.SetEvent();

   MakePropDialog( TH_Property, vec );
 }

void TChildFrame::CopyPropVec( TAP_PropEdit_Vec& rDst, TAP_PropEdit_Vec& rSrc )
 {
   rDst.assign( rSrc.size() );
   for( int i = rSrc.size() - 1; i >= 0; --i )
     rDst[ i ] = TAP_PropEdit(new TPropEdit(*rSrc[i])) ;
 }

void TChildFrame::CmpPropVec( TAP_PropEdit_Vec& rDst, TAP_PropEdit_Vec& rSrc )
 {
   for( int i = rSrc.size() - 1; i >= 0; --i )
     if( rDst[ i ]->m_ttEdit.CmpProp(rSrc[ i ]->m_ttEdit) == true &&
	     rSrc[ i ]->m_bFlAvaliable
	   )
	   rSrc[ i ]->m_bFlAvaliable = false;
 }

void TChildFrame::MakePropDialog( TH_HintPage htPage, TAP_PropEdit_Vec& rVecProp )
 {
   if( !CriticalRun() )
	{
      MessageBeep( -1 ); return;
	}
   
   m_evDataAccEnable.ResetEvent();
   try {
	 TDiplomDoc *pDoc = m_pMainView->GetDocument();
   
	 TDrawShape *pFirst;
	 int iSel = pDoc->CalcSelectedAndGet( false, pFirst );

	 bool bFlHaveAvl = false;
	 for( int k = rVecProp.size() - 1; k >= 0; --k )
	  if( rVecProp[k]->m_bFlAvaliable )
	   {
		 bFlHaveAvl = true; break;
	   }

	  if( bFlHaveAvl == false )
	   {	   
	     m_evDataAccEnable.SetEvent();
		 MessageBox( iSel ? "Множества свойств объектов не пересекаются":"Нет доступных свойств", 
		   "Информация", MB_OK|MB_ICONINFORMATION );
		 return;
	   }
   

	 auto_ptr<TPRopPage_PC> ap_pagPC( NULL );
	 auto_ptr<TPRopPage_Text> ap_pagTxt( NULL );
	 auto_ptr<TPRopPage_Line> ap_pagLine( NULL );
	 auto_ptr<TPRopPage_Fill> ap_pagFill( NULL );

	 ASSERT( (iSel != 0 && pFirst != NULL) );
	 if( iSel == 1 )
	  switch( pFirst->isA() )
	  {
		case TT_PC:
		  ap_pagPC = auto_ptr<TPRopPage_PC>( new TPRopPage_PC((TDrawPC*)pFirst) );
		  break;
		case TT_Text:
		  ap_pagTxt = auto_ptr<TPRopPage_Text>( new TPRopPage_Text((TDrawTxt*)pFirst) );
		  break;
	  }

	 for( int i = rVecProp.size() - 1; i >= 0; --i )
	  if( rVecProp[i]->m_bFlAvaliable )
		switch( rVecProp[i]->m_ttEdit.TypeIs() )
		 {
		   case TDR_Pen:
			 ap_pagLine = auto_ptr<TPRopPage_Line>( new TPRopPage_Line(rVecProp[i].get()) );
			 break;
		   case TDR_Brush:
			 ap_pagFill = auto_ptr<TPRopPage_Fill>( new TPRopPage_Fill(rVecProp[i].get()) );
			 break;
		 }

	 if( !ap_pagPC.get() && !ap_pagTxt.get() && !ap_pagLine.get() && !ap_pagFill.get() )
	  {
	    m_evDataAccEnable.SetEvent();
		MessageBox( "Нельзя изменить свойства этой группы: множества свойств выделенных объектов не пересекаются ", 
		   "Ошибка", MB_OK|MB_ICONINFORMATION );
		return;
	  }

	 TPropSheet propSheet( m_ptAlnProp_DlgCorn, htPage, "Параметры", this );   

	 if( ap_pagPC.get() ) propSheet.AddPage( ap_pagPC.get() );
	 if( ap_pagTxt.get() ) propSheet.AddPage( ap_pagTxt.get() );
	 if( ap_pagLine.get() ) propSheet.AddPage( ap_pagLine.get() );
	 if( ap_pagFill.get() ) propSheet.AddPage( ap_pagFill.get() );

	 m_evDataAccEnable.SetEvent();

	 TAP_PropEdit_Vec vKey; CopyPropVec( vKey, rVecProp );
	 if( propSheet.DoModal() == IDOK )
	  { 
	    ::WaitForSingleObject( m_evDataAccEnable, INFINITE );
	    //m_evDataAccEnable.ResetEvent();  

		CmpPropVec( vKey, rVecProp );
		pDoc->AssNewProp( rVecProp, TDR_All );
		if( pFirst ) m_pMainView->InvalObj( pFirst, true );

		pDoc->SetModifiedFlag();
	  }
	}
   catch(...)
	{
	  m_evDataAccEnable.SetEvent();
	  throw;
	}
   m_evDataAccEnable.SetEvent();
 }

BOOL TCancelExc::GetErrorMessage( LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext )
 {
   strncpy( lpszError, "Прерывание пользователем", nMaxError );
   return TRUE;
 }

TCancelExc::~TCancelExc()
 {
 }

UINT ThreadProc( LPVOID p )
 {
   TChildFrame *pFr = (TChildFrame *)p;

   try {
      (pFr->*(pFr->m_thStart.m_pFn))();
	}
   catch( CException *pE )
	{
	  if( typeid(*pE) == typeid(TCancelExc) )
	   {
         pFr->m_apCD->m_shErr = TCA_Cancel;		 
		 pFr->m_apCD->m_csErr = "Прерывание пользователем";
	   }
	  else
	   {
         pFr->m_apCD->m_shErr = TCA_Common;
		 LPSTR lp = pFr->m_apCD->m_csErr.GetBuffer( 1024 );
		 pE->GetErrorMessage( lp, 1023 );
		 lp[ 1023 ] = 0;
		 pFr->m_apCD->m_csErr.ReleaseBuffer();
	   }

	  pE->Delete();
	}
   catch( exception e )
	{
	  pFr->m_apCD->m_shErr = TCA_Common;	
	  pFr->m_apCD->m_csErr = e.what();
	}
   catch( TGrafExc ex )
	{
	  pFr->m_apCD->m_shErr = TCA_Common;	
      TDiplomDoc::Get_GrafErr( ex, pFr->m_apCD->m_csErr );
	}
   
   if( pFr->m_apAnalyzer.get() && pFr->m_apAnalyzer->m_bRedraw == false )
	 pFr->m_pMainView->Invalidate( FALSE );

   pFr->m_evDataAccEnable.SetEvent();
   return 0;
 }

void TChildFrame::AddReport()
 {
   if( !CriticalRun() )
	{
      MessageBeep( -1 ); return;
	}

   CPoint pt( m_pMainView->m_ptPopup );
   m_pMainView->ClientToDoc_SC( &pt );
   TDrawShape *pObj = m_pMainView->GetDocument()->FindInterect( pt );
   if( !pObj || pObj->isA() != TT_PC )
	{
      MessageBox( "Это доступно только для устройств", "Ошибка", MB_OK|MB_ICONEXCLAMATION );
	  return;
	}

   TDrawPC *pPC = (TDrawPC*)pObj;

   CRect r; m_pMainView->GetClientRect( &r );
   pt = r.CenterPoint();	  
   m_pMainView->ClientToDoc_SC( &pt );

   TDrawTxt *pCh = NULL;
   pCh = new TDrawTxt();
   ((TDrawTxt*)pCh)->SetBounds( 25, 5 );
   
	
   CRect position = pCh->m_position;
   CPoint ptCObj( position.left + abs(position.Width())/2,
				  position.top - abs(position.Height())/2
				);
   position.OffsetRect( pt - ptCObj );
   pCh->m_position = position;   

   pCh->SetAln( DT_LEFT );
   pCh->SetML( true );   
   

   m_pMainView->GetDocument()->Add( pCh );		    
   
   CString str;
   unsigned int uiSize = pPC->m_csName.GetLength() + 10*1024;
   LPSTR lp = str.GetBuffer( uiSize );

   ostrstream strm( lp, uiSize - 1 );
   //long lFl = strm.flags();

   //lFl |= ios::fixed|ios::scientific;
   //lFl |= ios::scientific;
   //lFl &= ~ios::scientific;

   strm << "Устройство [" << pPC->m_sRang << '.' << pPC->m_sNum << "] \"" << (LPCSTR)pPC->m_csName << "\"" << "\r\n" <<
	  "Интенсивность потока отказов: " << pPC->m_dLambda << "\r\n" <<
	  "Математическое ожидание времени ремонта: " << pPC->m_dM << "\r\n" <<
	  "Дисперсия времени ремонта: " << pPC->m_dD << "\r\n" <<
	  "Коэффициент готовности: " << pPC->m_dKReady << "\r\n" <<
	  "Первый момент функции готовности: " << pPC->m_dKC << "\r\n";

   str.ReleaseBuffer( strm.pcount() );
   
   pCh->SetTxt( str );

   pCh->BoundsRound();
 }//llllllllll

void TChildFrame::StartCalc()
 {   
   m_evCalcActive.ResetEvent(); m_evCalcStop.ResetEvent();
   m_evCalcNotActive.SetEvent();   

   CRect r; m_pMainView->GetClientRect( &r );
   CPoint pt( r.CenterPoint() );	  
   m_pMainView->ClientToDoc_SC( &pt );

   TDrawTxt *pCh = NULL;
   pCh = new TDrawTxt();
   ((TDrawTxt*)pCh)->SetBounds( 25, 5 );
   
	
   CRect position = pCh->m_position;
   CPoint ptCObj( position.left + abs(position.Width())/2,
				  position.top - abs(position.Height())/2
				);
   position.OffsetRect( pt - ptCObj );
   pCh->m_position = position;   

   m_pMainView->GetDocument()->Add( pCh );		    
   
   pCh->SetTxt( "Идёт расчёт..." );

   //pCh->SetInve();
   
   pCh->SetAln( DT_CENTER );
   pCh->SetML( true );
   pCh->SetLock( true );

   m_pMainView->SelectShp( pCh, false );
   //m_pMainView->InvalObj( pCh );

   m_apCD->m_pTxt = pCh;

   TMyThrd *pThrd = new TMyThrd( ThreadProc, (LPVOID)this, &m_evCalcActive, &m_evCalcNotActive, m_hWnd, globl_iMsgEndCalc );
   if( !pThrd->CreateThread() )
	{
      delete pThrd;
	  ASSERT( FALSE );
	  m_bFlCalc = true;
	  return;
	}
   else
	{
      m_bFlCalc = false;
	  CButton *p = (CButton*)m_dbarHor.GetDlgItem( ICM_ED_CALC );
	  auto_ptr<CBitmap> apTmp( new CBitmap() );
	  apTmp->LoadBitmap( IDB_BMP_STOP );
	  p->SetBitmap( *apTmp );
	  m_bmpCalc = apTmp;
	  
	  //pCmdUI->Enable( FALSE );
	  p->SetDlgCtrlID( ICM_ED_STOP );
	}
 }

TMyThrd::~TMyThrd()
 {
   m_pEvActive->ResetEvent();
   m_evNotActive->SetEvent();
   ::PostMessage( m_hFram, m_uiMsg, 0, 0 );
 }


BOOL TMyThrd::CreateThread( DWORD dwCreateFlags, UINT nStackSize, LPSECURITY_ATTRIBUTES lpSecurityAttrs )
 {
   BOOL bRes = CWinThread::CreateThread( dwCreateFlags, nStackSize, lpSecurityAttrs );
   if( !bRes ) return FALSE;

   m_pEvActive->SetEvent();
   m_evNotActive->ResetEvent();
   return TRUE;
   
 }

TCalcData::~TCalcData()
 {
   if( m_pTxt )
	{
      m_pTxt->SetLock( false );
	  m_pTxt = NULL;
	}
 }

LRESULT TListStensil::OnCommandHelp( WPARAM wp, LPARAM lParam )
 {   
   return IDH_EditorSchemes;
 }

LRESULT TListStensil::OnCommandHelpF1( WPARAM wp, LPARAM lParam )
 {   
   AfxGetApp()->CWinApp::WinHelp( IDH_EditorSchemes );    
   return TRUE;
 }
