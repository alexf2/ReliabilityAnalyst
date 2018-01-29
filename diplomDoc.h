// diplomDoc.h : interface of the TDiplomDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIPLOMDOC_H__FE33C08C_CBA2_11D1_9887_00504E02C39D__INCLUDED_)
#define AFX_DIPLOMDOC_H__FE33C08C_CBA2_11D1_9887_00504E02C39D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <memory>
#include <list>
using namespace std;

#include "drawobj.h"
#include "toolparam.hpp"
#include "brigad.hpp"

#include "en_prop.hpp"
#include "doc_analyze.hpp"

class TDrawShape;


class TShpStoreTool_Dta
 {
public:
  TShpStoreTool_Dta( CArchive& rA, bool bFlSelectionOnly ):
    m_ca( rA ),
	m_bFlSelectionOnly( bFlSelectionOnly )
	 {
	 }

  void operator()( TAuto_DrawShape& arg )
   {
     if( m_bFlSelectionOnly && arg->IsSelected() == false ) return;
     m_ca << arg->isA();
	 arg->AR_Store( m_ca );
   }

  bool m_bFlSelectionOnly;
  CArchive& m_ca;
 };


class TShpHit_Dta
 {
public:
   TShpHit_Dta( const CPoint& rP, int& rI, int& rGlue, TDrawShape*& rC, TDiplomView *pV, TDrawShape* pExclude, short bSel ):
	 m_cpLocal( rP ),
	 nDragHandle( rI ),
	 nGlue( rGlue ),
	 m_rpChape( rC ),
	 m_pV( pV ),
	 m_pExclude( pExclude ),
	 m_bSel( bSel )
	  {
	  }

   //int operator()( TAuto_DrawShape& arg );
   int operator()( TAuto_DrawShape& arg )
	{
	  if( arg.get() == m_pExclude ) return FALSE;
	  if( m_bSel < 2 && arg->IsSelected() != bool(m_bSel) ) return FALSE;
	  return (m_rpChape = arg->HitTest( m_cpLocal, nDragHandle, nGlue, m_pV )) != NULL;
	}

   const CPoint& m_cpLocal;
   int& nDragHandle;
   int& nGlue;
   TDrawShape*& m_rpChape;
   TDiplomView *m_pV;
   TDrawShape* m_pExclude;
   short m_bSel;
 };

class TShpSelTool_Dta
 {
public:
  TShpSelTool_Dta( bool bFl, TDrawShapeList& rLst, TDiplomView *pV ):
    m_bFl( bFl ),
	m_rLst( rLst ),
	m_pMainView( pV )
	 {
	   m_iCnt = 1;
	 }

  void operator()( TAuto_DrawShape& arg );  

  bool m_bFl;
  TDrawShapeList& m_rLst;
  int m_iCnt;
  TDiplomView *m_pMainView;
 };

class TShpSelInveTool_Dta
 {
public:
  TShpSelInveTool_Dta( TDrawShapeList& rLst ):    
	m_rLst( rLst )
	 {
	 }

  void operator()( TAuto_DrawShape& arg )
   {
     arg->Select( m_rLst, !arg->IsSelected() );	 
   }
  
  TDrawShapeList& m_rLst;
 };

enum TAlnH { TL_Top=0, TL_Cent, TL_Bottom, TL_NoneH };
enum TAlnV { TL_Left=0, TL_CentV, TL_Right, TL_NoneV };
enum TDistrHV { TD_1=0, TD_2, TD_3, TD_4, TD_5, TD_6 };

class TShpAln_Tool
 {
public:
  TShpAln_Tool( TDrawShape *pShpBase, TAlnH h, TAlnV v, TDiplomView *pV );

  void operator()( TAuto_DrawShape& arg );   

  TAlnH m_h;
  TAlnV m_v;
  TDrawShape *m_pShpBase;
  TDiplomView *m_pV;

  int m_iParamH, m_iParamV;
 };




class TDiplomView;
struct TChfOpt;

enum TSEL_Select { TSEL_SelAll, TSEL_SelDp, TSEL_SelHigh };


class TDiplomDoc: public CDocument
{

friend class TDiplomApp;
friend class TDiplomView;
friend class TChildFrame;
friend class TCnct;
friend inline bool IsRulesVisible( TDiplomDoc *pDoc );
friend class TAnalyzer;
friend class TPCConnect;

protected: // create from serialization only
	TDiplomDoc();
	DECLARE_DYNCREATE(TDiplomDoc)

// Attributes
public:

// Operations
public:

   const CSize& GetSize() const { return m_size; }
   void ComputePageSize();
   int GetMapMode() const { return m_nMapMode; }

   void SelectAll( bool );
   void SelectHigh();
   void SelectInve();
   void NewScaleAll( double dSc );

   void SelectPC( TSEL_Select sel )
	{
	  Sel_Objs( sel, TT_PC );
	}
   void SelectCnct( TSEL_Select sel )
	{
	  Sel_Objs( sel, TT_Cnct );
	}
   void Sel_Objs( TSEL_Select sel, int );   

   void FillRanges();

   void Draw( CDC* pDrawDC, double dSc, const CRect& rClip );
   void DrawRulers( CDC* pDrawDC, double dSc, const CRect& rClip );
   void DrawSelected( CDC* pDrawDC );
   
   void UpdateMainView( CView* pSender, LPARAM lHint = 0L, CObject* pHint = NULL );   

   void ZSelection( bool bFlForward );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TDiplomDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL

	void Add( TDrawShape* pShp );	
	void Add( TDrawShape* pShp, TDrawShapeList& rL );
	void RemoveObj( TDrawShape* pObj );
	void RemoveSelected( bool bFlFirst );

	void AddBrig( TAP_Brigada ap );
	void RemoveBrig( DWORD );

	//void CreateInstuments();

	void RenumSelected();

	TDrawShapeList& RefLstDraw() 
	 {
       return m_lstDrw;
	 }
	bool HaveSelectedObj();

	CPoint GetSelectionCenter( bool, CRect *pROut = NULL );	

	TDiplomView* GetMainViev();

	TChfOpt* GetFrameOpt();
	void CenterDraw();	

	void ReadObjects( CArchive& ar );
	void WriteObjects( CArchive& ar, bool bFlSelectionOnly = false );
	int GetObjCnt( bool bFlSelectionOnly );		
	int CalcSelected( bool bFlWithGuids );
	int CalcSelectedAndGet( bool bFlWithGuids, TDrawShape*& );

	static int CalcPCAndPoints( TDrawShapeList& rLst );

	void AlignSelection( TAlnH iH, TAlnV iV );
	void DistribSelection( TDistrHV iHV );

	void RestoreGluPtrs();
	void RecalcAllGlued();
	void RecalcAllGlued_Selected(); //for selected PCs
	void CheckAllGlued();

	TDrawShape* HitTest2( const CPoint& pt, int& nDragHandle, int& nGlue, TDrawShape *pExc );
	TDrawShape* HitTest0( const CPoint& pt, int& nDragHandle, int& nGlue, TDrawShape *pExc );
	TDrawShape* FindInterect( const CPoint& pt );

	void RenumIDs();

	CBrush* MBrush_GlueSel()
	 {
       return m_tbBrush_GlueSel();
	 }

	void CreateVectorProp( TAP_PropEdit_Vec& );
	void AssNewProp( TAP_PropEdit_Vec&, TTypesDrawTool prTyp );
	TPropEdit* VecGetProp( TAP_PropEdit_Vec& rVec, TTypesDrawTool typ );

	TDrawShape* GetMinID();

	TBrigada* GetBrigPtr( DWORD dw )
	 {
       return (TBrigada*)TIndexedUnic::Find_ID( dw, (TIndexedList&)m_lstBrig );
	 }

	void ServisSelected( int );
	void ScaleSel( double, int );

	BOOL My_CanClose();

    void CountSelGuid( int& iCntV, int& iCntH, TDrawRuler*& pV, TDrawRuler*& pH);
	void AlignSelected_AtRuler( TDrawRuler*& pV, TDrawRuler*& pH );

	//void SelectDBL();

	
	void UnHiglight( TT_IndTypes );

	static void ClearDPolus( TDrawShapeList& lstDrw );
	static void ClearDPolusAndUsedFlag( TDrawShapeList& lstDrw );
	static void ClearUsedFlag( TDrawShapeList& lstDrw );
	void SelectDP( TDoublePolus&, int, bool bAdd );//0 - all, 1 - PC, 2 - Links

	bool CreAllDPs( TAP_TPDP_Lst& );//llll
	static TDrawPC *GetNoDPPc( TDrawShapeList& );

	void ShowCnctType( TDrawShape *pShp );
	void ShowInt( TDrawShape *pShp );

	void HiliteCnct( TPCConnect*, bool bFlHilite = true );
	void HiliteLstPCs( list<TDrawShape*>&, bool bFlHilite = true );

	void ShowNPaths( TDrawShape* );
	void SimplifyT( TDrawShape* );

	void LoadObjectsOnly( CString& );

	static void Get_GrafErr( TGrafExc& ex, CString& );
	void ReportExcStat( TGrafExc exc );

	void FlipObjects();
	void ShiftObjects( CPoint );

// Implementation
public:
	virtual ~TDiplomDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void CreRngs( list<TDrawPC*>& );
	void CreRngs_Back( list<TDrawPC*>& );
	void ClearLbl();
	void CreRng();
	void CreRngsXY( list<TDrawPC*>& );

	void UnHilite();
	short CheckConnect();
	bool FindPath( TDrawShape *pPc, TDrawShape *pPCSrc, TDrawShape *pStopPC, int& riOut );
	bool m_b_FndPath_Lbl;

protected:
  CSize m_size;
  short m_Albom;
  short m_sPageType;
  int m_nMapMode;

  CPoint m_ptCenter, m_ptCenterPriv;
  WINDOWPLACEMENT  m_wndPlac;

  double m_dSc;

  TDrawShapeList m_lstDrw;
  TLST_Brigada m_lstBrig;
  int m_iKeyChecked;

  bool m_bHaveSel, m_bHaveSel_G;
  int m_iCntSel;

  //auto_ptr<CBrush> m_apBrSelectFirst, m_apBrSelectSecond;
  //auto_ptr<CPen> m_apPenGlueNorm, m_apPenGlueSel;

  CPoint m_pLoadScroll;
  
  TChildFrame *m_pChFrame;

  void SetViewMode();
  bool m_bFlArrow, m_bFlSimpleView;  

  TTItem_Pen m_tpPen_GlueNorm, m_tpPen_GlueSel;
  TTItem_Brush m_tbBrush_GlueSel;
  TTItem_Brush m_tpBrush_Han1, m_tpBrush_Han2;
    

// Generated message map functions
private:
   bool m_bFlLoadObjOnly;
	//{{AFX_MSG(TDiplomDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIPLOMDOC_H__FE33C08C_CBA2_11D1_9887_00504E02C39D__INCLUDED_)
