// ChildFrm.h : interface of the TChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDFRM_H__FE33C08A_CBA2_11D1_9887_00504E02C39D__INCLUDED_)
#define AFX_CHILDFRM_H__FE33C08A_CBA2_11D1_9887_00504E02C39D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "stdafx.h"
//#include "docexp.hpp"
#include "autorc.hpp"
#include "opt.hpp"
#include "basview.hpp"
#include "rlistvw.h"
#include "toolparam.hpp"
#include "hint_typ.hpp"



class TAnalyzer;
class TPCConnect;

#include <afxcmn.h>

#include "..\\RUDAKOW\\SERVIS\\borctl.hpp"
#include "..\\RUDAKOW\\SERVIS\\dibapi.h"
#include "..\\RUDAKOW\\SERVIS\\mymenu.hpp"

#include <memory>
using namespace std;

#include "objtypes.hpp"

class TDiplomView;
typedef void (TDiplomView::* TPtrDrwView_Func)( bool );

struct TSnslDescr
 {
   UINT m_uiIdBmp;
   LPSTR m_lpName;
 };


#define MSGA_LEAVE  0
#define MSGA_ENTER  1
#define MSGA_DRAG   2
#define MSGA_ACCEPT 3

class TListStensil: public CListCtrl
 {
   DECLARE_DYNCREATE( TListStensil )

public:
  TListStensil(): CListCtrl() 
   {
     m_pStnsl = NULL;
	 m_bDragging = false;
	 m_hcDisbl = m_hcEmpty = m_hcPriv = NULL;
	 m_bLeave = m_bCursDisbl = false;
	 m_iMinW = m_iMinH = 50;
	 m_hLast = m_hwPriv = NULL;
   }
  virtual ~TListStensil();

  void SetStnsl( TSnslDescr *pStnsl )
   {
     m_pStnsl = pStnsl;
   }
  afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );  

  int MinH() const
   {
     return m_iMinH;
   }
  int MinW() const
   {
     return m_iMinW;
   }

protected:
  afx_msg void OnBeginDrag(LPNMHDR pnmhdr, LRESULT *pResult);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);  
  afx_msg void OnCancelMode();

  void TermDrag();
  LRESULT SendAcceptMsg( HWND );

  afx_msg LRESULT OnCommandHelp( WPARAM, LPARAM lParam );
  afx_msg LRESULT OnCommandHelpF1( WPARAM, LPARAM lParam );

private:
  auto_ptr<CImageList> m_apImgLst;
  TSnslDescr *m_pStnsl;
  bool m_bCursDisbl, m_bLeave;

  HCURSOR m_hcDisbl, m_hcEmpty, m_hcPriv;
  int m_iMinW, m_iMinH;

  bool			m_bDragging;
  int			m_iItemDrag;
  int			m_iItemDrop;
  CPoint		m_ptHotSpot;
  CPoint		m_ptOrigin;
  CSize			m_sizeDelta;
  auto_ptr<CImageList>	m_apImageListDrag;

  HWND m_hwPriv, m_hLast;

  DECLARE_MESSAGE_MAP()
 };


class CViewExSplitWnd2: public CSplitterWnd
 {
	DECLARE_DYNCREATE( CViewExSplitWnd2 )

friend class TChildFrame;

public:
	CViewExSplitWnd2();
	~CViewExSplitWnd2();

	virtual CWnd* GetActivePane( int* pRow = NULL, int* pCol = NULL );

	virtual BOOL CreateView( int row, int col, 
	  CRuntimeClass* pViewClass, SIZE sizeInit, 
	  CCreateContext* pContext );

	afx_msg void OnDestroy();

	virtual BOOL SplitRow( int cxBefore );

	virtual BOOL PreCreateWindow( CREATESTRUCT& cs );

	//virtual void TrackRowSize(int y, int row);
	virtual void TrackRowSize(int x, int col);		
	// starting and stopping tracking
	//virtual void StartTracking(int ht);
	//virtual void StopTracking(BOOL bAccept);	

	virtual void DeleteRow( int rowDelete );
	virtual void DeleteView( int row, int col );	

	afx_msg void OnSize( UINT nType, int cx, int cy );
	virtual void ActivateNext( BOOL bPrev = FALSE );
	virtual BOOL CanActivateNext( BOOL bPrev = FALSE );

	double m_dSplit;
    int m_iTopMin, m_iBottomMin;

	void OnShowWindow( BOOL bShow, UINT nStatus );
	bool m_bFirst;

protected:
    auto_ptr<TListStensil> m_apLstStnsl;
	auto_ptr<CRowListView> m_apListBr;
	auto_ptr<TMSStyleTooolBar> m_apTool;	

	DECLARE_MESSAGE_MAP()
 };

class CViewExSplitWnd1: public CSplitterWnd
 {
	DECLARE_DYNCREATE( CViewExSplitWnd1 )

friend class TChildFrame;

public:
	CViewExSplitWnd1();
	~CViewExSplitWnd1();

	virtual CWnd* GetActivePane( int* pRow = NULL, int* pCol = NULL );
	virtual BOOL SplitColumn( int cxBefore );

	//virtual void TrackRowSize(int y, int row);
	virtual void TrackColumnSize(int x, int col);		
	// starting and stopping tracking
	//virtual void StartTracking(int ht);
	//virtual void StopTracking(BOOL bAccept);

	virtual BOOL PreCreateWindow( CREATESTRUCT& cs );	

	virtual BOOL CreateView( int row, int col, 
	  CRuntimeClass* pViewClass, SIZE sizeInit, 
	  CCreateContext* pContext );

	virtual void DeleteColumn( int colDelete );
	virtual void DeleteView( int row, int col );	

	afx_msg void OnSize( UINT nType, int cx, int cy );
	virtual void ActivateNext( BOOL bPrev = FALSE );

	void MakeSplit2( bool );

	double m_dSplit, m_dLeftMax;
    int m_iLeftMin, m_iRightMin;

protected:
  auto_ptr<CViewExSplitWnd2> m_wndSplitter2;  

  DECLARE_MESSAGE_MAP()
 };



class CRcFile: public CFile
 {
public:
  CRcFile( HINSTANCE hinst, UINT uiIdRc, LPCSTR lpName );

  virtual ~CRcFile();

  virtual UINT Read( void* lpBuf, UINT nCount );  
  virtual DWORD GetLength() const;

private:
  HINSTANCE m_hHan;
  HRSRC m_hrc;
  HGLOBAL m_hg;
  LPSTR m_lpRc;
  DWORD m_dwSizeRc;

  LPSTR m_lpCuros;
  DWORD m_dwCnt;
  bool m_bEOF;
 };


#define MUL_DSTBUTTONS 2
struct TBtnItem //pPrivBtn устанавливается CMyDialogBar 
 {
   TBtnItem( CWnd *pBtn, bool bFixedDst )
	{
	  m_pBtn = pBtn; 
	  m_bFixedDst = bFixedDst;
	  m_pPrivBtn = NULL;
	}

   CWnd *m_pBtn;
   CWnd *m_pPrivBtn;
   bool m_bFixedDst;

   CRect r;
   int m_iW;
   int m_iH;
 };

class CMyDialogBar: public CDialogBar
 {

friend class TMDIFrame;

public:
  CMyDialogBar( bool bFlHoriz ): CDialogBar()
   {
     m_bFlHoriz = bFlHoriz;
	 m_imaxBounds = 50;
     //m_vievsCurrent = TV_Expert;
   }

  virtual ~CMyDialogBar();

  CMyDialogBar& operator+( TToolTipData *pDta )
   {
     m_vecToolTips.push_back( pDta );
	 return *this;
   }

  CMyDialogBar& operator+( TBtnItem *pBtnIt );
  CMyDialogBar& operator+( TBtnItem &rBtnIt )
   {
     return operator+( &rBtnIt );
   }

  void InsertHead( TBtnItem &rBtnIt );
  

  void ClearBtns();
  
  
  afx_msg void OnDestroy();

  virtual CSize CalcFixedLayout( BOOL bStretch, BOOL bHorz );

  afx_msg BOOL OnEraseBkgnd( CDC* );
  afx_msg void OnSize( UINT nType, int cx, int cy );
  afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );  
  //afx_msg void OnShowWindow( BOOL bShow, UINT nStatus );
  
  afx_msg LRESULT OnCommandHelp( WPARAM, LPARAM lParam );

  void MyLayout( bool bRedr, bool bFlCallDirectly = false, int iBnd = 0 );

  virtual BOOL PreCreateWindow( CREATESTRUCT& cs );

  LONG QPalette( LONG );

  virtual int OnToolHitTest( CPoint point, TOOLINFO* pTI ) const;

  void SetMaxBounds( int iMax ) { m_imaxBounds = iMax; }
  int GetMaxBounds() const { return m_imaxBounds; }
  bool IsHoris() const { return m_bFlHoriz; }

  bool TextureIsOn() const;  

  /*void SetCurVievClass( TV_Views vievsCurrent )
  {
    m_vievsCurrent = vievsCurrent;
  }*/

private:
  list<TToolTipData *> m_vecToolTips;
  bool m_bFlHoriz;
  //CBrush m_brBkGnd;

  int m_imaxBounds;

  //TV_Views m_vievsCurrent;

  static int m_iW, m_iH;
  static CPalette m_palPal;
  static HDIB m_hrcBmp;
  static int m_iUseCnt;
  static CRect m_rS;

  static int m_iWv, m_iHv;
  static CPalette m_palPalv;
  static HDIB m_hrcBmpv;
  static int m_iUseCntv;
  static CRect m_rSv;

  list<TBtnItem*> m_lstBtn;
 

  DECLARE_MESSAGE_MAP()
 };


struct TChfOpt
 {
   TChfOpt()
	{
      m_bFlBrig = m_bFlElem = m_bFlStepOnGrid =
	  m_bFlStepOnGuid = m_bFlGuids = m_bFlGrid = m_bFlGluePoints = true;
	  m_dScalePriv = m_dScale = 1;
	  m_dLines = true;

	  m_dSplit1 = 1.0/3.0;
	  m_dSplit2 = 2.0/3.0;

	  m_bFlModeBrig = false;

	  m_szPage = CSize( 850, 1100 );
	}

   bool m_bFlBrig, m_bFlElem, m_bFlStepOnGrid, 
	    m_bFlStepOnGuid, m_bFlGuids, m_bFlGrid, m_bFlGluePoints,
		m_dLines;
   double m_dScale, m_dScalePriv;

   double m_dSplit1, m_dSplit2;

   bool m_bFlModeBrig;

   CSize m_szPage;
 };


class TMyThrd: public CWinThread
 {
public: 
  TMyThrd( AFX_THREADPROC pfnThreadProc, LPVOID pParam, CEvent* pEvActive, CEvent* pEvNotActive, HWND hFram, UINT uiMsg ):
	CWinThread( pfnThreadProc, pParam )
	  {
	    m_pEvActive = pEvActive;
		m_evNotActive = pEvNotActive;
		m_hFram = hFram;
		m_uiMsg = uiMsg;
	  }

  virtual ~TMyThrd();
  
  BOOL CreateThread( DWORD dwCreateFlags = 0, UINT nStackSize = 0, LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL );

protected:
  CEvent *m_pEvActive, *m_evNotActive;
  HWND m_hFram;
  UINT m_uiMsg;
 };

class TChildFrame;
typedef void (TChildFrame::* TPtrCalc_Func)();

struct ThStart
 {
   TPtrCalc_Func m_pFn;   
   //TChildFrame *m_pObj;
 };

UINT ThreadProc( LPVOID );

class TCancelExc: public CException
 {
 public:
  TCancelExc(): CException()
   {
   }
  virtual ~TCancelExc();
  virtual BOOL GetErrorMessage( LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext = NULL );   
 };

enum TCalcErrors { TCA_No=0, TCA_Cancel, TCA_Common };

class TCalcData
 {
public:
  TCalcData()
   {
     m_shErr = TCA_No;
	 m_pTxt = NULL;
   }

  virtual ~TCalcData();   

  TCalcErrors m_shErr;
  CString m_csErr;
  TDrawTxt *m_pTxt;
 };

class TCalc_EQRestore: public TCalcData
 {
public:
  TCalc_EQRestore( TBrigada *p ): TCalcData()
   {
     m_iOA = p->m_iOA;
	 m_tdsDIS = p->m_tdsDIS;
	 m_iTotal = p->m_iTotal;
	 m_iLabel = p->m_iLabel;

	 m_id = p->ID();
	 m_dLamda = 0;

	 p->GetML( m_M, d_L, m_M2, d_L2 );
   }

  int m_iOA; //ремонтники
  TDisOfServis m_tdsDIS; //дисциплина  
  int m_iTotal, m_iLabel;
  double m_M, d_L;
  double m_M2, d_L2;

  double m_dLamda;

  int m_id;
 };


class TChildFrame : public CMDIChildWnd
{

friend class CViewExSplitWnd1;
friend class CViewExSplitWnd2;
friend class TDiplomView;
friend class TDiplomDoc;
friend inline bool IsRulesVisible( TDiplomDoc *pDoc );

friend UINT ThreadProc( LPVOID );

	DECLARE_DYNCREATE(TChildFrame)
public:
	TChildFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TChildFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);				
	virtual void Serialize(CArchive& ar);
	virtual BOOL DestroyWindow();
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);	
	//}}AFX_VIRTUAL

	void SerializeEMM(CArchive& ar);

public:
   afx_msg LONG OnGlobalQPal( UINT, LONG );
   afx_msg LONG OnGlobalPalCh( UINT, LONG );
   afx_msg LONG OnEndCalc( UINT, LONG );
   afx_msg LONG OnEndRun( UINT, LONG );
   afx_msg LONG OnNewStatusStr( UINT, LONG );

   void SwitchOnVerBar( bool bFl = true );
   
   void ChangeTitle();
   void UpdateLocToolbar();
   void UpdateTexture();

   void PageSizeChanged();

   void GetCtx( CCreateContext& );
   void RefreshDrawingPane();

   void CallDrawView_Func( TPtrDrwView_Func pF, bool );

   void ShowWindow_Init( BOOL bShow, UINT nStatus );

// Implementation
public:
	virtual ~TChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CMyDialogBar* GetHorDlgBarPtr()  { return &m_dbarHor; }

	auto_ptr<TMCildOpt> GetCHCfg();
	void SetCHCfg( TMCildOpt* pOpt );

    void SetXSplit1( double spl )
	 {
	   m_optX.m_dSplit1 = spl; 
	 }
	void SetXSplit2( double spl )
	 {
	   m_optX.m_dSplit2 = spl; 
	 }
	double GetXSplit1()
	 {
	   return m_optX.m_dSplit1;
	 }
	double GetXSplit2()
	 {
	   return m_optX.m_dSplit2;
	 }
	

	void PasteNative( COleDataObject& dataObject );
	static CLIPFORMAT m_cfDraw;

    void FillBrigList();	
	void EnableBrigEdit( bool bFl )
	 {
       m_bEnblBrigEdit = bFl;
	 }

    CEvent m_evCalcActive, m_evCalcStop, m_evCalcNotActive;
	CEvent m_evSingleStep;//automatic reset
	CEvent m_evSingleStepMode;
	CEvent m_evDataAccEnable;
	bool m_bFlRun;
	bool m_bFlCalc;
	auto_ptr<CBitmap> m_bmpCalc;
	BOOL MyCanClose();	
		

	auto_ptr<TAnalyzer> m_apAnalyzer;
    auto_ptr<TCalcData> m_apCD;
	ThStart m_thStart;
	void StartCalc();
	void AnalyzeCD();
	void Analyze_Run();
	void CheckCancel()
	 {
	   if( ::WaitForSingleObject(m_evCalcStop, 0) != WAIT_TIMEOUT )
		{
		  //m_evDataAccEnable.SetEvent();
	      throw new TCancelExc();
		}
	 }

	void CalcEQRest();
	void CalcRun();
	void GlobalSimplify( TAnalyzer *pAnalyzer, bool bFlSingleStep, bool bFNoRoot, TPCConnect* );

	auto_ptr<char> strMsgSimpl;
	CMutex m_mtxMsg;

	DWORD WaitInRun( TAnalyzer *pAnalyzer );
	void AsyncMsgStr( LPCSTR );
	void AssignEQRest( TBrigada *pBrig, TCalc_EQRestore* );

	void Quant();


	//auto_ptr<CVtChart> m_apChat;

// Generated message map functions
protected:
	//{{AFX_MSG(TChildFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnClose();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG

	afx_msg void OnCBNSCale();
	afx_msg void OnCBNScale_ENTER();	
	

	void SetupScaleCBN( double );

	void OnUpdate_ClcRun( CCmdUI* pCmdUI );
	void OnClcRun();

	
	bool CriticalRun()
	 {
	   bool bRes = (::WaitForSingleObject( m_evDataAccEnable, 0 ) != WAIT_TIMEOUT);
	   if( bRes == true ) m_evDataAccEnable.SetEvent();
	   return bRes;
	 }
	void OnUpd_DisableAtRun( CCmdUI* pCmdUI )
	 {
	   pCmdUI->Enable( !m_bFlRun );
	 }
	void OnUpd_EnableAtRun_Crit( CCmdUI* pCmdUI )
	 {
	   pCmdUI->Enable( CriticalRun() );
	 }	
	

    void OnUpdate_Calc( CCmdUI* pCmdUI ); 
	void OnUpdate_Stop( CCmdUI* pCmdUI ); 
	void OnUpdate_EqRest( CCmdUI* pCmdUI ); 
	void OnUpdate_Connect( CCmdUI* pCmdUI ); 
	void OnUpdate_ClcType( CCmdUI* pCmdUI ); 
	void OnUpdate_TSimpl( CCmdUI* pCmdUI );	
	void OnClcType();
	void OnClcInt();
	void On_Connect();
	void OnStop();
	void On_EqRest();
	void On_SinglStep();
    void On_StepMode();

	void AddReport();

	void OnSelectHigh();

	void OnNPaths();
	void OnTSimplify();
	void OnSofistSimplify();

	void OnARNG();

	void OnUpd_ViewGrid( CCmdUI* pCmdUI );
	void OnUpd_ViewGuidlines( CCmdUI* pCmdUI ); 
	void OnUpd_ViewCnctPoints( CCmdUI* pCmdUI ); 
	void OnUpd_ViewLine( CCmdUI* pCmdUI ); 
	void OnUpd_ViewBrig( CCmdUI* pCmdUI ); 
	void OnUpd_ViewElements( CCmdUI* pCmdUI ); 
	void OnUpdate_Always_NoCrit( CCmdUI* pCmdUI )
	 {
	   pCmdUI->Enable( CriticalRun() );
	 }
	void OnUpdate_Always_NoRun( CCmdUI* pCmdUI )
	 {
	   pCmdUI->Enable( !m_bFlRun );
	 }
	void OnUpdate_Always( CCmdUI* pCmdUI )
	 {
	   pCmdUI->Enable( 1 );
	 }
	void OnUpd_StepGrid( CCmdUI* pCmdUI ); 
	void OnUpd_StepGuid( CCmdUI* pCmdUI ); 
	void OnUpd_ModeBrig( CCmdUI* pCmdUI ); 

	void OnUpd_SimpleView_NoCrit( CCmdUI* pCmdUI );
	void OnUpd_Arrow_NoCrit( CCmdUI* pCmdUI ); 

	void OnUpd_Aligment_NoRun( CCmdUI* pCmdUI );
	void OnUpd_Center_NoCrit( CCmdUI* pCmdUI );
	void OnUpd_Back_NoRun( CCmdUI* pCmdUI );
	void OnUpd_Forward_NoRun( CCmdUI* pCmdUI ); 

	void OnUpdate_LocAdd( CCmdUI* pCmdUI ); 
	void OnUpdate_LocRemove( CCmdUI* pCmdUI ); 
	void OnUpdate_LocEdit( CCmdUI* pCmdUI ); 
	void OnUpdate_LocAccept( CCmdUI* pCmdUI ); 

	void OnUpdate_HaveSel_NoCrit( CCmdUI* pCmdUI );
	void OnUpdate_HaveSel( CCmdUI* pCmdUI );     
	void OnUpdate_HaveSel_NoRun( CCmdUI* pCmdUI );     
	void OnUpdate_Paste( CCmdUI* pCmdUI ); 
	void OnUpdate_Paste_NoRun( CCmdUI* pCmdUI ); 

	void OnUpdate_SelAll( CCmdUI* pCmdUI ); 
	void OnUpdate_SelAll_NoCrit( CCmdUI* pCmdUI ); 

	void OnUpd_Property_NoCrit( CCmdUI* pCmdUI ); 

	void OnUpdate_Servis( CCmdUI* pCmdUI ); 
	void OnUpdate_UnServis( CCmdUI* pCmdUI ); 
	void OnUpdate_Label( CCmdUI* pCmdUI ); 

	void On_Edit_Unhilite();
	void On_Edit_SelPC();
	void On_Edit_SelCnct();

	void On_Edit_Dup();
    void On_Edit_UnselAll();
	void On_Edit_SelAll();
    void On_Edit_SelInv();
	void On_Edit_Copy();
	void On_Edit_ClearAll();
	void On_Edit_Clear();
	void On_Edit_Cut();
	void On_Edit_Paste();
	void On_Edit_Scale();
	

	void OnModeBrig();
	void On_ViewGrid();
	void On_ViewGuidlines(); 
	void On_ViewCnctPoints(); 
	void On_ViewLine(); 
	void On_ViewBrig(); 
	void On_ViewElements(); 
	void On_Solve(); 
	void On_StepGrid(); 
	void On_StepGuid();

	void On_Aligment();
	void On_Center();
	void On_Back();
	void On_Forward();
	void On_FillLM();
	void On_CreRng();

	void On_SimpleView();
	void On_Arrow(); 

	void On_Property();

	void On_Servis(); 
	void On_UnServis(); 
	void On_Label(); 

	//--------------------------------
	void OnFont(); 
	void OnFntClr(); 
	void OnAlign(); 

	void OnLine(); 
	void OnLinClr(); 
	void OnLinThick(); 
	void OnLinTyp(); 

	void OnFil(); 
	void OnFilClr(); 
	void OnFilTyp(); 

	void OnCalc();

	//--------------------------
	void On_AligmentGuid();
	void OnUpdate_Revert( CCmdUI* pCmdUI ); 
	void On_Revert();
	void On_Export();
	//--------------------------

	void OnLocAdd();
	void OnLocRem(); 
	void OnLocEd();	
    void OnLocAccept();
	//--------------------------
	void OnView_PrivScale();
    void OnView_Scale100();
    void OnView_Pagew();
    void OnView_Page();
    void OnView_200();
    void OnView_50();
	//--------------------------
	void On_Edit_SelDBL();
	void On_Edit_SelDBL_Max();

	void On_SelSubch();
	void On_SelPCDp();
	void On_SelPCHigh();
	void On_SelCnctDP();
	void On_SelCnctHigh();

    CMyDialogBar m_dbarHor;
	TMCildOpt m_opt;
	bool m_bFlInit;
	bool m_bFirstShow;
	bool m_bMax_KEY;

	TChfOpt m_optX;

	TDiplomView *m_pMainView;

	bool HaveSelected( int = -1 );
	bool HaveObjects();
	bool DeniedProp();	

	void MakePropDialog( TH_HintPage htPage, TAP_PropEdit_Vec& rVecProp );

	CViewExSplitWnd1 m_wndSplitter1;
	//CViewExSplitWnd2 m_wndSplitter2;

	
	void AddObj_BrigLst( CListCtrl& lst, TBrigada* pUs, int iItem, bool bFlModify = false );
	void LoadBrigLst( int iItem );
	void StopBrigMode();

	CRowListView* GetBrigList();
	int CountSeletedInLstBrig();
	void OnDblClk_List( NMHDR * pNotifyStruct, LRESULT * result );
	void OnEnter_List( NMHDR * pNotifyStruct, LRESULT * result );
	void OnKeyDown_List( NMHDR * pNotifyStruct, LRESULT * result );
	void OnItemChg_List( NMHDR * pNotifyStruct, LRESULT * result );

	CPoint m_ptAlnDlgCorn, m_ptAlnProp_DlgCorn, m_ptBrigDlg,
	       m_ptScale;

	auto_ptr<EnumVal_Int> m_apLine_Pattern;
	auto_ptr<EnumVal_Int> m_apLine_Thick;
	auto_ptr<EnumVal_Int> m_apFill_Pattern;
	auto_ptr<EnumVal_Clr> m_ap_Clr;

	bool m_bEnblBrigEdit;

	void CopyPropVec( TAP_PropEdit_Vec& rDst, TAP_PropEdit_Vec& rSrc );
	void CmpPropVec( TAP_PropEdit_Vec& rDst, TAP_PropEdit_Vec& rSrc );

	int m_bAD_key;
    int m_iAln_H_key, m_iAln_V_key;
    int m_iDistr_HV_key;
   
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRM_H__FE33C08A_CBA2_11D1_9887_00504E02C39D__INCLUDED_)
