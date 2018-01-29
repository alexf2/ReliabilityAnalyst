// diplomView.h : interface of the TDiplomView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIPLOMVIEW_H__FE33C08E_CBA2_11D1_9887_00504E02C39D__INCLUDED_)
#define AFX_DIPLOMVIEW_H__FE33C08E_CBA2_11D1_9887_00504E02C39D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "stdafx.h"
#include "childfrm.h"
#include "diplomDoc.h"
#include "drtool.hpp"


#define HINT_UPDATE_WINDOW      0
#define HINT_UPDATE_DRAWOBJ     1
#define HINT_UPDATE_SELECTION   2
#define HINT_DELETE_SELECTION   3


enum TB_BtnTypes { TB_Bmp, TB_Normal, TB_EndDescr };
struct TBtnTool
 {
   TB_BtnTypes m_typ;
   int m_iIDCmd;      

   LPSTR m_lpTtl;
   int m_iIdBmp;

   bool m_bShiftMode;

   LPSTR m_lpHint;

   DWORD m_dwExtraStyle;
 };

void CreateBtns( CMyDialogBar& rB, TBtnTool* pDescr, bool bFlTxtAln, 
				 list<CBitmap*>& rLst, list<CWnd*>& rLstBtn );


class TDiplomView : public CScrollView
{

friend class TChildFrame;
friend class TDiplomDoc;

protected: // create from serialization only
	TDiplomView();
	DECLARE_DYNCREATE(TDiplomView)

// Attributes
public:
	TDiplomDoc* GetDocument();
	void SetPageSize( CSize size, bool bUpdate = true );
	void DocToClient( CRect* rect, int iCnt = 1 );
	void DocToClient( CPoint* point, int iCnt = 1  );
	void ClientToDoc( CPoint* point, int iCnt = 1  );
	void ClientToDoc( CRect* rect, int iCnt = 1  );

	void DocToClient_SC( CRect* rect, int iCnt = 1 );
	void DocToClient_SC( CPoint* point, int iCnt = 1  );
	void ClientToDoc_SC( CPoint* point, int iCnt = 1  );
	void ClientToDoc_SC( CRect* rect, int iCnt = 1  );

	void SysClrChange();	

	TDrawShape* HitTest( const CPoint& local, int& nDragHandle, int& nGlue, TDrawShape* pExclude=NULL );
	bool IsShpSelected( TDrawShape* );
	void SelectShp( TDrawShape*, bool = false );
	void CloneShpSelection();
	TDrawShape* ObjectAt( const CPoint& );
	void Deselect( TDrawShape* );
	void EditText( TDrawShape* );
	void SelectWithinRect( const CRect&, bool );
	int GetCountSelected();
	bool HaveSelected() 
	 {
	   return GetDocument() ? GetDocument()->m_bHaveSel:false;
	 }
	int CalcSelected( bool bFlWithGuids )
	 {
	   return GetDocument()->CalcSelected( bFlWithGuids );
	 }
	void CastOnSelection_MoveTo( const CPoint& );	
	void CastSelectionInve( bool, bool );
	void MoveHandleTo( int nDragHandle, const CPoint& local, TDrawShape*, bool );
	//TDrawShape* GetHeadSelected();	

	void InvalObj( TDrawShape* pObj, bool bFlWithSel = false );

	void RemoveObj( TDrawShape* pObj );

	void SetCenterViewToSelection();

// Operations
public:

   afx_msg LRESULT OnCommandHelp( WPARAM, LPARAM lParam );
   afx_msg LRESULT OnCommandHelpF1( WPARAM, LPARAM lParam );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TDiplomView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView);	
	//}}AFX_VIRTUAL

	void OnPrepareDC_Met( CDC* );

	virtual void OnActivateFrame( UINT nState, CFrameWnd* pFrameWnd );

	virtual void OnInitialUpdate();
	void SetupScaleCBN( double dSc, CMyDialogBar& rB );
	int CalcRulerStep( double dScale );

	void DrawPaper( CDC*, CRect& );
	void DrawRullers( CDC*, CRect& );

	bool IsActiveWnd();
	

// Implementation
public:
	virtual ~TDiplomView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
   void OnUpdate_Always( CCmdUI*  pCmdUI );
   void SetupHorizBar( CMyDialogBar& rB, TBtnTool* btns );
   void ResetupBar( short sCfg ); 

   void OnCBNScale( bool bFlEnter );

   void CalcCenter();
   void CenteringPage();

// Generated message map functions
protected:
	//{{AFX_MSG(TDiplomView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnCancelMode();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	afx_msg void OnFilePrintPreview();
	afx_msg LONG OnMyAcceptDrag( UINT, LONG );
	afx_msg LONG OnReqMenu( UINT, LONG );

	afx_msg LRESULT OnDisplayChange( WPARAM, LPARAM );

    list<CBitmap*> m_lstBmp;
    list<CWnd*> m_lstBtn;
    void ClearBtnsLists();

	CComboBox m_cbnScale;

	double m_dWPaper, m_dHPaper;
	int m_iRulerStepH, m_iRulerStepV;
	double m_dRulerStepH_pix, m_dRulerStepV_pix;
	CRect m_rectPaper;

	TChildFrame *m_pFram;
	CMyDialogBar *m_pBar;
	

	bool m_bActive;

	CRect m_rRulH, m_rRulV, m_rRulInt;
	void CalcRulRects( bool bFlClient = false );
	void InvalidateRules();

	TTItem_Pen m_ttPenGridV, m_ttPenGridH, m_ttPenPaperShadow,
	  m_ttPenGridV2, m_ttPenGridH2;
    TTItem_Brush m_ttBrushTable;
	TTItem_Font m_ttFntRules, m_ttFntRulesV;
//???
	/*auto_ptr<CFont> m_apFntRules, m_apFntRulesV;
	auto_ptr<CBrush> m_apBrushTable;
	auto_ptr<CPen> m_apPenGridV, m_apPenGridH, m_apPenPaperShadow;
	auto_ptr<CPen> m_apPenGridV2, m_apPenGridH2;*/
	int m_iRulerHHeight, m_iRulerVWidth;	

	//void CreateFonts();
	//void CreateOthers();

	double CalcScaleH( int iSize );
    double CalcScaleV( int iSize );
	bool CheckDigits( LPCSTR );
	void UpdateCBNScale();
	bool CheckMax( int );	

	int m_iXOffset, m_iYOffset;
	void GetClientRectS( CRect& rR )
	 {
	   GetClientRect( rR );
	   rR.left += m_iRulerVWidth;
	   rR.top += m_iRulerHHeight;	   
	 }
	void InvalidateRulers();

	double m_dKeyScale;

	TDrawRuler *m_pRulAdd;

	void ShowContextMenu( CPoint point, short shMnu = 0 );
	CMenu PopMenu, m_menBrig, m_menCalc;
    BOOL m_bFlPopupCreated, m_bFlPopupCreated_Br, m_bFlPopupCreated_Calc;
	CPoint m_ptPopup;
	TDrawShape *m_pLocalClicked;

	DECLARE_MESSAGE_MAP()
};

class TShpObjTool_Dta
 {
public:
  TShpObjTool_Dta( TDiplomView* pV ):
    m_pView( pV )
	 {
	 }

  void operator()( TAuto_DrawShape& arg )
   {
     if( arg->IsSelected() )
       m_pView->InvalObj( arg.get() );
   }

  TDiplomView* m_pView;
 };


class TShpSelectRectTool_Dta
 {
public:
  TShpSelectRectTool_Dta( TDiplomView* pV, const CRect& rect, TDrawShapeList& rLst ):
    m_pView( pV ),
	m_rect( rect ),
	m_rLst( rLst )
	 {
	 }

  void operator()( TAuto_DrawShape& arg )
   {
     if( arg->Intersects(m_rect) && !arg->IsSelected() && arg->isA() != TT_Ruler )
       arg->Select( m_rLst, true ),
	   m_pView->InvalObj( arg.get() );
   }

  TDiplomView* m_pView;
  const CRect& m_rect;
  TDrawShapeList& m_rLst;
 };

class TShpCountSelect_Dta
 {
public:
  TShpCountSelect_Dta( int& iCnt ):
	m_iCnt( iCnt )  
	 {
	   m_iCnt = 0;
	 }

  void operator()( TAuto_DrawShape& arg )
   {
     if( arg->IsSelected() ) m_iCnt++;
   }

  int& m_iCnt;
 };


#ifndef _DEBUG  // debug version in diplomView.cpp
inline TDiplomDoc* TDiplomView::GetDocument()
   { return (TDiplomDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIPLOMVIEW_H__FE33C08E_CBA2_11D1_9887_00504E02C39D__INCLUDED_)
