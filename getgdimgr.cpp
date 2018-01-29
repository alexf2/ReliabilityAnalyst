#include "getgdimgr.hpp"
#include "mainfrm.h"

#include "stdafx.h"

TDrToolsManeger* GDIMgr()
 {
   return &reinterpret_cast<TMainFrame*>(AfxGetApp()->m_pMainWnd)->m_gdiMgr;
 }

static CString strKey;

void GL_SetMsgStr( LPCSTR lp )
 {
   //((CStatusBar*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetDescendantWindow(AFX_IDW_STATUS_BAR, TRUE))->GetPaneText( ID_SEPARATOR, strKey );
   ((CStatusBar*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetMessageBar())->GetPaneText( ID_SEPARATOR, strKey );   
   ((CFrameWnd*)AfxGetApp()->m_pMainWnd)->SetMessageText( lp );
 }
void GL_RestoreMsg()
 {
   ((CFrameWnd*)AfxGetApp()->m_pMainWnd)->SetMessageText( (LPCSTR)strKey );
   strKey = "Выход Alt+X";
 }
