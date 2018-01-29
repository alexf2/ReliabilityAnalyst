// diplom.cpp : Defines the class behaviors for the application.
//
#include "stdafx.h"
#include "diplom.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "diplomDoc.h"
#include "diplomView.h"
#include "drawtool.hpp"

#include "..\\RUDAKOW\\SERVIS\\texcept.hpp"

//#include "locrc.hpp"

#include "..\\RUDAKOW\\SERVIS\\servis.hpp"
#include "..\\RUDAKOW\\SERVIS\\mydib.hpp"
#include "..\\RUDAKOW\\SERVIS\\pathex.hpp"

#include <locale.h>
#include <io.h>
#include <direct.h>
#include <afxdisp.h>

#include "globlmsg.hpp"
#include "mainfrm.h"

#include "HLP\\diplom.hh"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TDiplomApp

BEGIN_MESSAGE_MAP(TDiplomApp, CWinApp)
	//{{AFX_MSG_MAP(TDiplomApp)	
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	//}}AFX_MSG_MAP
	// Standard file based document commands	
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, OnFilePrintSetup)
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TDiplomApp construction

TDiplomApp::TDiplomApp()
{
  m_bFlNoFileNew = false;
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only TDiplomApp object

TDiplomApp theApp;

/////////////////////////////////////////////////////////////////////////////
// TDiplomApp initialization

void TDiplomApp::OnFilePrintSetup()
 {
   BOOL bFl;
   CMDIChildWnd *pCh = NULL;
   if( m_pMainWnd ) pCh = ((CMDIFrameWnd*)m_pMainWnd)->MDIGetActive( &bFl );

   if( !pCh || !pCh->GetActiveView() || !pCh->GetActiveView()->GetDocument() )
	{
      m_pMainWnd->MessageBox( "Нет активного CView связанного с документом", 
	     "Предупреждение", MB_OK|MB_ICONEXCLAMATION );
	  return;
	}

   TDiplomDoc *pDoc = (TDiplomDoc*)(pCh->GetActiveView()->GetDocument());

   if( !m_hDevMode )
	{
      if( m_pMainWnd )
	    m_pMainWnd->MessageBox( "Требуется установленный принтер", "Ошибка", MB_OK|MB_ICONEXCLAMATION );
	  else AfxMessageBox( "Требуется установленный принтер", MB_OK|MB_ICONEXCLAMATION );
	  return;
	}
   
   CPrintDialog pd( TRUE );

   
   DEVMODE *pMdod = (DEVMODE*)::GlobalLock( m_hDevMode );
   pMdod->dmOrientation = pDoc->m_Albom;
   pMdod->dmPaperSize = pDoc->m_sPageType;
   pMdod->dmPaperWidth = pDoc->m_size.cx;
   pMdod->dmPaperLength = pDoc->m_size.cy;
   //::GlobalUnlock( m_hDevMode );

   if( DoPrintDialog( &pd ) == IDOK )
	{	  

	  if( pCh && typeid(*pCh) == typeid(TChildFrame) ) 
	    reinterpret_cast<TChildFrame*>( pCh )->PageSizeChanged();
	}
 }

bool CheckExist( LPCSTR strName )
 {
   _finddata_t fnd;
   memset( &fnd, 0, sizeof(_finddata_t) );
   fnd.attrib = _A_ARCH | _A_SYSTEM | _A_HIDDEN | _A_RDONLY | _A_NORMAL;

   long lHandl = _findfirst( (LPSTR)(LPCSTR)strName, &fnd );
   if( lHandl != -1 ) 
	{
      _findclose( lHandl );
	  if( !(fnd.attrib & _A_SUBDIR) )  return true;	   
	}
   
   return false;
 }

BOOL TDiplomApp::SaveAllModified()
 {
   BOOL bRes = CWinApp::SaveAllModified();
   if( !bRes ) return bRes;

   try {
	 if( m_pMainWnd /*&& static_cast<TMainFrame*>(m_pMainWnd)->GetOpt().d.m_bSaveOptOnExit*/ )
	  {
	    static_cast<TMainFrame*>(m_pMainWnd)->StoreCfg();
		static_cast<TMainFrame*>(m_pMainWnd)->SaveCfg( m_csConfig );
	  }
	}
   catch( CException *pE )
	{
      pE->ReportError();
	  pE->Delete();
	}	
   catch( exception ex )
	{
      m_pMainWnd->MessageBox( ex.what(), "Ошибка", MB_OK|MB_ICONEXCLAMATION );
	}

   return bRes;
 }


BOOL TDiplomApp::InitInstance()
{
    setlocale( LC_ALL, "" );

	//CoInitialize( NULL );
	AfxEnableControlContainer();

	if( !AfxOleInit() )
	 {
	   AfxMessageBox( "Ошибка инициализации OLE" );
	   return FALSE;
	 }

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

    CMultiDocTemplate* pDocTemplate = NULL;

    try {

     InitServisDLL();
	 InitMyDIBDLL();

     #ifdef _AFXDLL
	    Enable3dControls();			// Call this when using MFC in a shared DLL
	 #else
		Enable3dControlsStatic();	// Call this when linking to MFC statically
	 #endif

	 // Change the registry key under which our settings are stored.
	 // You should modify this string to be something appropriate
	 // such as the name of your company or organization.
	 SetRegistryKey(_T("DIPLOM.exe"));

	 LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	 // Register the application's document templates.  Document templates
	 //  serve as the connection between documents, frame windows and views.
	 
     bwcc.EnableBWCC( m_hInstance, TRUE );

     pDocTemplate = new CMultiDocTemplate(
		IDR_DIPLOMTYPE,
		RUNTIME_CLASS(TDiplomDoc),
		RUNTIME_CLASS(TChildFrame), // custom MDI child frame
		RUNTIME_CLASS(TDiplomView) );
	}
   catch( TExcept rExept )
    {
      rExept.ReportExcept();
	  rExept.Delete();

	  if( pDocTemplate ) delete pDocTemplate;
	  return FALSE;
	}
   catch( CException* pE )
	{
      pE->ReportError();
	  pE->Delete();

	  if( pDocTemplate ) delete pDocTemplate;
	  return FALSE;
	}
   catch(...)
    {
      AfxMessageBox( "Any exception", MB_OK ); 

	  if( pDocTemplate ) delete pDocTemplate;
	  return FALSE;
	}

    LPSTR lpBuf = m_csStartPath.GetBuffer( _MAX_PATH );    
	::GetModuleFileName( m_hInstance, lpBuf, _MAX_PATH );
   
    char drive[ _MAX_DRIVE ];
    char dir[ _MAX_DIR ];
    char fname[ _MAX_FNAME ];
    char ext[ _MAX_EXT ];

	_splitpath( lpBuf, drive, dir, fname, ext );
	_makepath( lpBuf, drive, dir, "", "" );	  	 

	m_csStartPath.ReleaseBuffer();

	lpBuf = m_csConfig.GetBuffer( _MAX_PATH );    
	_getcwd( lpBuf, _MAX_PATH - 1 );
	m_csConfig.ReleaseBuffer();
	m_csConfig += CString("\\") + STR_CONFIGNAME;		

	if( CheckExist(m_csConfig) == false )
	 {
	   m_csConfig = m_csStartPath + STR_CONFIGNAME;
       	
	   //if( CheckExist(m_csConfig) == false ) CreateConfig();
	 }
	 
	//m_Startpath + "VES\\";

	
		
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	TMainFrame* pMainFrame = new TMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	m_bFlNoFileNew = true;
	try {
	  if (!ProcessShellCommand(cmdInfo))
		 return FALSE;
	  m_bFlNoFileNew = false;
	 }
	catch( CException *pE )
	 {
	   m_bFlNoFileNew = false;
	   throw;
	 }

    new CSelectTool();    

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// TDiplomApp commands

void TDiplomApp::OnFileNew() 
{
	// TODO: Add your command handler code here

  if( m_bFlNoFileNew == true ) return;
  CWinApp::OnFileNew();
}


int TDiplomApp::ExitInstance() 
 {
	// TODO: Add your specialized code here and/or call the base class
	//CoUninitialize();
    CDrawTool::m_lstTools.clear();
 
	return CWinApp::ExitInstance();
 }

void TDiplomApp::WinHelp( DWORD dwData, UINT nCmd )
 {
   switch( dwData ) 
	{
	  case 131200: case 262146:
	    dwData = IDH_Contents;
	    break;	 
	  case 387073: case 131201:
	    dwData = IDH_Interface;
	    break;	 
	  case 66236: case 66238: case 66239: case 66237: 
	    dwData = IDH_BrigEditor;
		break;
    };
    
    CWinApp::WinHelp( dwData, nCmd );    
 }
