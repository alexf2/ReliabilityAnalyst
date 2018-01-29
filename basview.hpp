#if !defined(_VIEWBASE_)
#define _VIEWBASE_

#include "stdafx.h"
#include "diplomDoc.h"

#include <list>
using namespace std;

enum CH_Relation { CH_Top };


struct TChildItem
 {
   enum TS_Types { TS_RelationFrom, TS_RelationParent };

   TChildItem() { m_pCh = NULL; }
   TChildItem( CWnd *pCh,  CH_Relation rel, CWnd *pFrom )
	{
      m_pCh = pCh; m_pFrom = pFrom; m_rel = rel;

	  m_ts = TS_RelationFrom;
	}

   
   TChildItem( CWnd *pCh, double left, double top, double W, double H )
	{
      m_pCh = pCh;
	  m_left = left; m_top = top;
	  m_W = W; m_H = H;

	  m_ts = TS_RelationParent;
	}

   TS_Types m_ts;

   CWnd *m_pCh;
   
	struct {      
      double m_left, m_top; //if <= 1 then relative else absolute
      double m_W, m_H;
	 };
	struct {
      CWnd *m_pFrom;
	  CH_Relation m_rel;
	 };
 };


class TBaseView: public CView
 {


protected: // create from serialization only
  TBaseView();
  DECLARE_DYNCREATE( TBaseView )

public:
  
  virtual ~TBaseView();

  TDiplomDoc* GetDocument()
   {
     return static_cast<TDiplomDoc*>(m_pDocument);
   }

  TBaseView& operator+( TChildItem* pCh )
   {
     m_lstCh.push_back( pCh );
	 return *this;
   }
  
//__________ don't call in subclass __________
  //virtual void SetupHorizBar( CMyDialogBar& ); //use as pure virtual
  //virtual void SetupVerBar( CMyDialogBar& ); //use as pure virtual

  virtual void OnDraw( CDC* pDC ); //use as pure virtual
//____________________________________________

  //---------------
  virtual void OnActivateView( BOOL bActivate, CView* pActivateView, CView* pDeactiveView );
  virtual void OnActivateFrame( UINT nState, CFrameWnd* pFrameWnd );
  BOOL SaveFocusControl();
  afx_msg void OnSetFocus( CWnd* pOldWnd );
  virtual BOOL PreTranslateMessage( MSG* pMsg );
  //---------------


  virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
  afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
  virtual void OnUpdate( CView* pSender, LPARAM lHint, CObject* pHint );  
  virtual void OnInitialUpdate();

  virtual BOOL OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult );
  virtual BOOL OnCommand( WPARAM wParam, LPARAM lParam );  


  afx_msg void OnSize( UINT nType, int cx, int cy );
  void MakeLaoyt( bool bFlRedr = false );

  void OnUpdate_Always( CCmdUI* ); 
  
  afx_msg void OnDestroy();
  

  afx_msg BOOL OnEraseBkgnd( CDC* );  


  void SetMinWH( int W, int H )
   {
     m_iMinW = W; m_iMinH = H;
   }
  
  
protected:
  HWND m_hWndFocus;
  HWND m_hWndFocusCTL;  
  
private:    
  int m_iMinW, m_iMinH;    
  bool m_bFlScrollEnable;

  list<TChildItem*> m_lstCh;  
  bool m_bHScr, m_bVScr;
  int m_sX_, m_sY_;

  DECLARE_MESSAGE_MAP()
 };

#endif
