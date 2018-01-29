// MainFrm.h : interface of the TMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__FE33C088_CBA2_11D1_9887_00504E02C39D__INCLUDED_)
#define AFX_MAINFRM_H__FE33C088_CBA2_11D1_9887_00504E02C39D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "..\\RUDAKOW\\SERVIS\\mymenu.hpp"
#include "opt.hpp"
#include "globlmsg.hpp"
#include "drtool.hpp"

using namespace std;
#include <memory>
#include <list>

struct TBtnDescr
 {
   UINT uCmd;
   bool bRight, bPush;
 };


class TSaveCfgData: public TClassFunc<fstream&, auto_ptr<TMCildOpt>&>
 {
public:
  TSaveCfgData( fstream& rS ): TClassFunc<fstream&, auto_ptr<TMCildOpt>&>( rS )
   {
   }

  void operator()( auto_ptr<TMCildOpt>& rArg )
   {
     m_data << *rArg;
   }
 };


typedef auto_ptr<TMCildOpt> TMCildOpt_AP;

class TMainFrame : public CMDIFrameWnd
 {
	DECLARE_DYNAMIC(TMainFrame)

public:
	TMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~TMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void OnUserWnd( CCmdUI* pCmdUI );
	void OnAlways( CCmdUI* pCmdUI );
	void OnLocalToolUpd( CCmdUI* pCmdUI );
	void OnLocalToolTextureUpd( CCmdUI* pCmdUI );
	void OnVievSaveOptUpd( CCmdUI* pCmdUI );
	void OnVievSaveDesktopUpd( CCmdUI* pCmdUI );

	void OnAbout(); 
    void OnAppWindow(); 
    void OnNext();

	void OnPriv();
	void OnCloseAll();	

	void SaveCfg( CString& );
    void LoadCfg( CString& );
    void DefaultPos();
	void My_ShowControlBar( int iId, int iFl );
	void StoreCfg();

	const TMFrOpt& GetOpt() const { return m_opt; }
	TMCildOpt* GetOptPtr( LPCSTR );

	void CheckBrig( bool bFl )
	 {
       m_pKeyBrigIt->SetCheckMy( bFl );
	 }

	bool IsActive() const
	 {
	   return m_bActive;
	 }

	TDrToolsManeger m_gdiMgr;
	

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;	
	auto_ptr<TMSStyleTooolBar> m_apTool;	

	TMSItem *m_pKeyBrigIt;

// Generated message map functions
protected:

    void RefreshChildLocalTool();

	//{{AFX_MSG(TMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg BOOL OnQueryNewPalette();	
	afx_msg void OnSysColorChange();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	//}}AFX_MSG

	LONG OnGLOpenDoc( UINT ui, LONG ll );

	afx_msg void OnLocalTool();
	afx_msg void OnLocalTexture();
	afx_msg void OnVievSaveOpt();
	afx_msg void OnVievSaveDesktop();	

	DECLARE_MESSAGE_MAP()

private:
    TMFrOpt m_opt;

	bool m_bActive;
	
	list<TMCildOpt_AP> m_lstChOpt;
	bool m_bFirstShow;
 };

class TCreData: public TClassFunc<TMainFrame*, auto_ptr<TMCildOpt>&>
 {
public:
  TCreData( TMainFrame* pF ): 
    TClassFunc<TMainFrame*, auto_ptr<TMCildOpt>&>( pF )
   {
   }

  void operator()( auto_ptr<TMCildOpt>& rArg )
   {
     m_data->PostMessage( globl_iMsgOpenDoc, 0, (LPARAM)rArg.get() );
   }
 };

class TFindOptData
 {
public:
  TFindOptData( LPCSTR lp ): m_lp( lp ) {};
    
  int operator()( auto_ptr<TMCildOpt>& rArg )
   {
     return !_stricmp( m_lp, (LPCSTR)rArg->m_csPath );
   }

protected:
  LPCSTR m_lp;
 };

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__FE33C088_CBA2_11D1_9887_00504E02C39D__INCLUDED_)
