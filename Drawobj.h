#ifndef __DRAWOBJ_H__
#define __DRAWOBJ_H__

class TDiplomView;
class TDiplomDoc;

/////////////////////////////////////////////////////////////////////////////
// CDrawObj - base class for all 'drawable objects'



#include <memory>
#include <list>
#include <vector>
#include <algorithm>
using namespace std;
#include <afxmt.h>

#include "objtypes.hpp"
#include "autorc.hpp"
#include "indexeditem.hpp"
#include "drtool.hpp"
#include "getgdimgr.hpp"

class TDrawShape;
class TCnct;

struct TGlueDescr
 {
   TGlueDescr()
	{
	  m_nGlu = m_iID = -1;
	  m_pShp = NULL;
	}
   TGlueDescr( int nGlu, TDrawShape *pShp );	

   bool IsGlued() const
	{
      return m_iID != -1;
	}

   int m_nGlu, m_iID;
   TDrawShape *m_pShp;
 };

enum TGTypes { TG_In, TG_Out, TG_InOut, TG_NoGlue };

class TFndGluDta
 {
public:
   TFndGluDta( int nGlu, int iID )
	{
      m_nGlu = nGlu; m_iID = iID;
	}
   bool operator()( const TGlueDescr& rD )
	{
      return rD.m_nGlu == m_nGlu && rD.m_iID == m_iID;
	}
   int m_nGlu, m_iID;
 };

class TGlue
 {

public:
  TGlue()
   {
     m_tgTyp = TG_In;
   }
  TGlue( TGTypes tgFl ): m_tgTyp( tgFl ) {};

  bool Glu( TDrawShape* pShp, int nGluForin, TDrawShape* pOwner, int nGluNative );//active
  bool DeGlu( TDrawShape* pOwner, int nGluNative, TDiplomView *pView );//active
  bool AddGlu( TDrawShape* pShp, int nGluForin );
  bool RemoveGlu( TDrawShape* pShp, int nGluForin );

  void RecalcGlued( TDrawShape* pShpOwn, int nOwn, TDiplomView *pView );

  TGTypes m_tgTyp;
  list<TGlueDescr> m_lstGlu;
 };


typedef auto_ptr<TGlue> TAuto_Glue;
typedef vector<TAuto_Glue> TVec_Glue;

CArchive& operator<<( CArchive& rA, TAuto_Glue& rCh );
CArchive& operator>>( CArchive& rA, TAuto_Glue& rCh );


typedef auto_ptr<TDrawShape> TAuto_DrawShape;
typedef list<TAuto_DrawShape> TDrawShapeList;


inline void MulRect( CRect& rp, const CRect& rSrc, double dSc )
 {
   rp.left = double(rSrc.left) * dSc;
   rp.right = double(rSrc.right) * dSc;
   rp.top = double(rSrc.top) * dSc;
   rp.bottom = double(rSrc.bottom) * dSc;
 }
inline void DivRect( CRect& rp, const CRect& rSrc, double dSc )
 {
   rp.left = double(rSrc.left) / dSc;
   rp.right = double(rSrc.right) / dSc;
   rp.top = double(rSrc.top) / dSc;
   rp.bottom = double(rSrc.bottom) / dSc;
 }
inline void MulPt( CPoint& rp, const CPoint& rSrc, double dSc )
 {
   rp.x = double(rSrc.x) * dSc;
   rp.y = double(rSrc.y) * dSc;
 }
inline void DivPt( CPoint& rp, const CPoint& rSrc, double dSc )
 {
   rp.x = double(rSrc.x) / dSc;
   rp.y = double(rSrc.y) / dSc;
 }


template<class T> class TVarHolderVal
 {
public:
  TVarHolderVal( T& ref, T val ):
     m_ref( ref )
	{
	  m_valKey = ref; ref = val;
	}
  ~TVarHolderVal()
	{
      m_ref = m_valKey;
	}

private:
  T& m_ref;
  T m_valKey;
 };

class TDrawShape: public TIndexedUnic
 {

friend CArchive& operator<<( CArchive& rA, TDrawShape& rCh );
friend CArchive& operator>>( CArchive& rA, TDrawShape& rCh );
friend class TShpSelTool_Dta;
friend class TGlue;
friend class TDiplomDoc;
friend class TDoublePolus;
friend class TCnct;
friend class TDrawPoint;
friend class TAnalyzer;
friend class TPCConnect;
friend void AddTo( TDrawShapeList& lstInt, TDiplomDoc *pDoc );

 // Constructors
public:
	TDrawShape();
	TDrawShape( const CRect& position );
	TDrawShape( const TDrawShape& );
	virtual ~TDrawShape();

// Attributes
	int DeltaTracker() const
	 {
       return 3;
	 }
    virtual CRect GetPosition( bool bFlWithTracker = false ) const;
	void FixPos()
	 {
       m_rFixRect = m_position;
	 }
	CRect m_position;
	TDiplomDoc* m_pDocument;
	double m_dSc;

	virtual void SetScaled( double dSc );
	 

	virtual int GetHandleCount();
	virtual CPoint GetHandle( int nHandle );
	virtual int GetGlueCount();
	virtual CPoint GetGlue( int nHandle );
	virtual CPoint GetGlue_Muled( int nHandle );
	CRect GetHandleRect( int nHandleID, TDiplomView* pView );
	CRect GetGlueRect( int nHandleID, TDiplomView* pView );
	virtual HCURSOR GetHandleCursor( int nHandle, bool bFlDrag );	

// Operations
	virtual void Draw( CDC* pDC, double dSc );
	enum TrackerState { normal, selected, active };
	virtual void DrawTracker( CDC* pDC, TrackerState state );
	virtual void DrawGlue( CDC* pDC, TrackerState state, CPen* pNorm, CPen* pSel );
	virtual void MoveTo( const CRect& positon, TDiplomView* pView = NULL );
	//virtual int HitTest( CPoint point, TDiplomView* pView, BOOL bSelected );
	virtual TDrawShape* HitTest( const CPoint& point, int& nHint, int& nGlue, TDiplomView* pView, bool bGlueOnly = false );	
	virtual BOOL Intersects( const CRect& rect );
	virtual void MoveHandleTo( int nHandle, CPoint point, TDiplomView* pView = NULL, bool bFlEQ = false );	
	virtual void OnEditProperties();
	virtual TDrawShape* Clone( TDiplomDoc* pDoc = NULL );
	virtual void Remove();
	
	void Invalidate();

	virtual void MakeOffset( const CPoint& );

	virtual void ScaleUser( double dSc, CPoint& pt );

	bool IsSelected() const { return m_bSelected; }
	void SetSel_Internal( bool bFl )
	 {
       m_bSelected = bFl;
	 }
	void Select( TDrawShapeList& rL, bool bFl = true );	 

	virtual void AR_Store( CArchive& rA );
	virtual void AR_Load( CArchive& rA );
	void ShiftIDs( int );
	virtual int isA() const
	 {
	   return TT_Shape;
	 }

	void SetInve( bool bFl = true )
	 {
       m_bInve = bFl;
	 }

	bool IsInve() const { return m_bInve; }

	virtual void ExtendRect( CRect& r, double ) const
	 {
	 }

	virtual void ChCursor( HCURSOR& )
	 {
	 }

	virtual bool IsActiveGlue() const
	 {
	   return false;
	 }

	virtual int HToGlu( int )
	 {
	   return -1;
	 }

	void RecalcGlued( TDiplomView *pView, bool bFlEnd = false );
	void CheckGlued( TDiplomView *pView );
	void CheckGlued0( TDiplomView *pView );

	virtual int GetGDICount() const
	 {
       return 0;
	 }

	virtual TDrToolItem* GetDrTool( TTypesDrawTool typ );

	virtual bool IsLocked() const
	 {
	   return false;
	 }

	virtual void HighLight( bool bRedraw = true )
	 {
	 }
	virtual void UnHighLight( bool bRedraw = true )
	 {
	 }
	virtual bool IsHighLighted()
	 {
	   return false;
	 }

// Implementation
#ifdef _DEBUG
	void AssertValid();
#endif

	int m_iSelCount;
	int m_iActiveGlue;

	bool MakeGlue( int nGlu, TDrawShape* pShp, int nGluForin, bool& rMade );	
	bool DeGlue( int nGlu, TDiplomView *pView );	 
	void DeGlueAll( TDiplomView *pView );

	bool CompatibleGlu( int iInvGlu_Ind, TDrawShape *pShp, int nGluForin_Ind );
	TGTypes GetGluTyp( int iInvGlu_Ind ) //only active GL
	 {       
       if( !m_vecGlu[ iInvGlu_Ind ]->m_lstGlu.size() ) 
		 return TG_NoGlue;

       TGlueDescr &p = m_vecGlu[ iInvGlu_Ind ]->m_lstGlu.front(); 

	   if( !p.IsGlued() || !p.m_pShp ) return TG_NoGlue;	  

	   return p.m_pShp->m_vecGlu[ p.m_nGlu - 1 ]->m_tgTyp;
	 }

	static int m_iGlueLevel; //0 - off, 1 - check break, 2 - full;

	virtual TDrawShape* GoTop( TCnct*& rP, TDrawShape* pExc, bool bFlLbl, bool& bFlNoEnter, bool bMarkUsed ) 
	 {
	   return NULL;
	 }
	virtual TDrawShape* GoBottom( TCnct*& rP, TDrawShape* pExc, bool bFlLbl, bool bMarkUsed )
	 {
	   return NULL;
	 }
	TCnct* FindNotUsedCnct( int iPoint_Ind, int& iglCnct );
	TCnct* FindNextCnct( int iPoint_Ind, int& iglCnct, TCnct* );
	

	TCnct *GetLinkTo( TDrawShape* );
	void SelectEmptyLink( int& iCnct );

	short IsUsed() const
	 {
	   return m_iUsed;
	 }
	void SetUsedFlag( bool bFl )
	 {
	   if( bFl ) m_iUsed++;
	   else m_iUsed = 0;
	 }
	void ResUsedFlag()
	 {
	   m_iUsed = 0;
	 }

	void LockUsed( bool bFl )
	 {
	   if( bFl ) m_iLockCnt++;
	   else if( m_iLockCnt > 0 ) m_iLockCnt--;
	 }
	void ResLockUsed()
	 {
	   m_iLockCnt = 0;
	 }
	int GetLocked() const
	 {
	   return m_iLockCnt;
	 }

	bool IsConnectedTo( TDrawShape*, int inGlu );

	bool m_bEnableMultiIn;
	// implementation data
protected:
  bool m_bSelected, m_bInve;  
  int m_iUsed;

  int m_iLockCnt;

  virtual int PenX() const
   {
     return 0;
   }
  virtual int PenY() const
   {
     return 0;
   }

  CRect m_rFixRect;

  TVec_Glue m_vecGlu;
 };

CArchive& operator<<( CArchive& rA, TDrawShape& rCh );
CArchive& operator>>( CArchive& rA, TDrawShape& rCh );

inline TGlueDescr::TGlueDescr( int nGlu, TDrawShape *pShp )
 { 
   ASSERT( nGlu != -1 );
   m_nGlu = nGlu;
   m_iID = pShp->ID(); m_pShp = pShp;
 }

struct TParamCalc
 {
   double m_dLambda, m_dM, m_dD;   
   double m_dKReady, m_dKC;
 };

class TDrawPC: public TDrawShape
 {

friend class TPRopPage_PC;
friend class TBrigada;
friend class TServisPC;
friend class TDiplomDoc;
friend struct std::greater<class TDrawPC*>;
//friend struct std::less<class TDrawShape*>;
friend struct std::greater<class TDrawShape*>;
friend struct TGreaterRng;
friend class TDoublePolus;
friend class TAnalyzer;
friend class TPCPath;
friend class TPCConnect;
friend class TChildFrame;


friend class TCnctFnd_Dta_Parall;
friend class TCnctFnd_Dta_FAcc;
friend class TCnctFnd_Dta_NDAcc;


friend CArchive& operator<<( CArchive& rA, TDrawPC& rCh );
friend CArchive& operator>>( CArchive& rA, TDrawPC& rCh );

public:
	TDrawPC();
	TDrawPC( const CRect& position );
	TDrawPC( const TDrawPC& );
	virtual ~TDrawPC();

// Implementation
public:	
	virtual void Draw( CDC* pDC, double dSc );
	virtual int GetHandleCount();
	virtual CPoint GetHandle( int nHandle );
	virtual int GetGlueCount();
	virtual CPoint GetGlue( int nHandle );
	virtual CPoint GetGlue_Muled( int nHandle );
	virtual HCURSOR GetHandleCursor( int nHandle, bool bFlDrag );	
	virtual BOOL Intersects( const CRect& rect );
	virtual TDrawShape* Clone( TDiplomDoc* pDoc );

	virtual void MoveTo( const CRect& positon, TDiplomView* pView = NULL );
	virtual void MoveHandleTo( int nHandle, CPoint point, TDiplomView* pView = NULL, bool bFlEQ = false );

	virtual void AR_Store( CArchive& rA );
	virtual void AR_Load( CArchive& rA );
	virtual int isA() const
	 {
	   return TT_PC;
	 }


	virtual void ScaleUser( double dSc, CPoint& pt );
	
	virtual void SetScaled( double dSc );

	void SwitchModes( bool bShort );
	int GetDefaultRadius();

	virtual int HToGlu( int iH )
	 {
	   return iH == 2 ? 1:(iH == 6 ? 2:-1);
	 }

	bool ShortView() const { return m_bShortView; }

	virtual int GetGDICount() const
	 {
       return 3;
	 }
	void InitGDI();

	virtual TDrToolItem* GetDrTool( TTypesDrawTool typ );

	virtual int PenX() const
	 {
	   return const_cast<TDrawPC*>(this)->m_tpPen.GetData().m_iWidth;
	 }
    virtual int PenY() const
	 {
	   return const_cast<TDrawPC*>(this)->m_tpPen.GetData().m_iWidth;
	 }

	void GenerateTTY();
	CString& GetInfoStr()
	 {
       return m_strInfo;
	 }
	 
	
	void AssSameMD( TDrawPC *p )
	 {
	   m_dLambda = p->m_dLambda;
	   m_dM = p->m_dM;
	   m_dD = p->m_dD;

	   m_dKReady = p->m_dKReady;
	   m_dKC = p->m_dKC;
	 }

	void CalcExtraParams();

	virtual TDrawShape* GoTop( TCnct*& rP, TDrawShape* pExc, bool bFlLbl, bool& bFlNoEnter, bool bMarkUsed );
	virtual TDrawShape* GoBottom( TCnct*& rP, TDrawShape* pExc, bool bFlLbl, bool bMarkUsed );	

	virtual void HighLight( bool bRedraw = true );	 
	virtual void UnHighLight( bool bRedraw = true );
	virtual bool IsHighLighted()
	 {
	   return m_bHighLight;
	 }

	bool IsInBiPolus( short sNum )
	 {
	   vector<short>::iterator it = find( m_vecBiPolus.begin(), m_vecBiPolus.end(), sNum );
	   return it != m_vecBiPolus.end();
	 }
	void ClearBiPolus()
	 {
       m_vecBiPolus.clear();
	   m_vecBiPolus.resize( 0 );
	 }
	bool MarkBiPolus( short sNum )
	 {
       if( !IsInBiPolus(sNum) )
		{
		  m_vecBiPolus.push_back( sNum );
		  return true;
		}
	   return false;
	 }

  TPCConnect *m_pTPC;
  bool m_bCTypeValid;

  short m_sRang, m_sNum;

protected:	
  CPoint m_roundness;
  CSize m_szKeep;
  bool m_bShortView;
  

  COLORREF m_clrKey;
  bool m_bHighLight;

  TTItem_Pen    m_tpPen;
  TTItem_Brush  m_brBrush;
  TTItem_Font   m_fnFont;  

  TTItem_Pen m_tPenLbl, m_tPenLblBk;
  
  double m_dLambda, m_dM, m_dD;
  CString m_csName;

  CString m_csTTYFull, m_csTTYShort, m_strInfo;

  short m_sServis_Brig;//0-нет; 1-низкий; 2-высокий/меченое.

  bool m_bLbl;
  double m_dKReady, m_dKC;

  void SetDefaultParam();

  vector<short> m_vecBiPolus;  
 };

CArchive& operator<<( CArchive& rA, TDrawPC& rCh );
CArchive& operator>>( CArchive& rA, TDrawPC& rCh );


class TCnct: public TDrawShape
 {

friend CArchive& operator<<( CArchive& rA, TCnct& rCh );
friend CArchive& operator>>( CArchive& rA, TCnct& rCh );

public:
    TCnct();
	TCnct( TObjTypes );
	TCnct( TObjTypes, const CRect& position );
	TCnct( const TCnct& );
	virtual ~TCnct();

// Implementation
public:	
	virtual void Draw( CDC* pDC, double dSc );
	virtual int GetHandleCount();
	virtual CPoint GetHandle( int nHandle );
	virtual int GetGlueCount();
	virtual CPoint GetGlue( int nHandle );
	virtual CPoint GetGlue_Muled( int nHandle );
	virtual HCURSOR GetHandleCursor( int nHandle, bool bFlDrag );	
	virtual BOOL Intersects( const CRect& rect );
	virtual TDrawShape* Clone( TDiplomDoc* pDoc );

	virtual void MoveTo( const CRect& positon, TDiplomView* pView = NULL );
	virtual void MoveHandleTo( int nHandle, CPoint point, TDiplomView* pView = NULL, bool bFlEQ = false );

	virtual void AR_Store( CArchive& rA );
	virtual void AR_Load( CArchive& rA );
	virtual int isA() const
	 {
	   return TT_Cnct;
	 }

	virtual void ScaleUser( double dSc, CPoint& pt );

	virtual CRect GetPosition( bool bFlWithTracker = false ) const;

	virtual bool IsActiveGlue() const
	 {
	   return true;
	 }

	virtual int HToGlu( int iH );	 

	virtual int GetGDICount() const
	 {
       return 1;
	 }
	void InitGDI();

	virtual TDrToolItem* GetDrTool( TTypesDrawTool typ );

	virtual int PenX() const
	 {
	   return const_cast<TCnct*>(this)->m_tpPen.GetData().m_iWidth;
	 }
    virtual int PenY() const
	 {
	   return const_cast<TCnct*>(this)->m_tpPen.GetData().m_iWidth;
	 }

	bool IsHaveGlu()
	 {
       return 
		 (m_vecGlu[0]->m_lstGlu.size() && m_vecGlu[0]->m_lstGlu.front().IsGlued()) ||
		 (m_vecGlu[1]->m_lstGlu.size() && m_vecGlu[1]->m_lstGlu.front().IsGlued());
	 }
	
	

	virtual TDrawShape* GoTop( TCnct*& rP, TDrawShape* pExc, bool bFlLbl, bool& bFlNoEnter, bool bMarkUsed );
	virtual TDrawShape* GoBottom( TCnct*& rP, TDrawShape* pExc, bool bFlLbl, bool bMarkUsed );
	int Another( int iCnctGL )
	 {
	   return iCnctGL == 1 ? 2:1;
	 }
	TDrawShape *GoLink( int, bool& bFlNoEnter, bool bMarkUsed );
	TDrawShape *GoLinkBack( int, bool bMarkUsed );
	

	virtual void HighLight( bool bRedraw = true );	 
	virtual void UnHighLight( bool bRedraw = true );
	virtual bool IsHighLighted()
	 {
	   return m_bHighLight;
	 }

	bool m_bFlMark;

protected:	
  TObjTypes m_typ;  

  COLORREF m_clrKey;
  bool m_bHighLight;

  void CreLineRgn( CRgn& rRgn, const CRect& fixed );  
  void DrawCorn( CRect& );

  short m_sNPoints;
  TAutoMemoryRc_AR<CPoint*> m_arcPoints;
  void LastSeg( CRect& r )
   {
     r = CRect( m_arcPoints()[m_sNPoints - 2], m_arcPoints()[m_sNPoints - 1] );
   }
  void LastSeg2( CRect& r )
   {
     r = CRect( m_arcPoints()[m_sNPoints - 3], m_arcPoints()[m_sNPoints - 2] );
   }
  void FirstSeg( CRect& r )
   {
     r = CRect( m_arcPoints()[0], m_arcPoints()[1] );
   }
  void FirstSeg2( CRect& r )
   {
     r = CRect( m_arcPoints()[1], m_arcPoints()[2] );
   }
  void InitPointsArray( CRect& rp, double );
  short GetPointsForTyp( TObjTypes );

  virtual void ExtendRect( CRect& r, double ) const;  

  double m_dKSq;
  int m_iKln;

  TTItem_Pen    m_tpPen;    
 };

CArchive& operator<<( CArchive& rA, TCnct& rCh );
CArchive& operator>>( CArchive& rA, TCnct& rCh );


class TDrawPoint: public TDrawShape
 {

friend CArchive& operator<<( CArchive& rA, TDrawPoint& rCh );
friend CArchive& operator>>( CArchive& rA, TDrawPoint& rCh );

public:
	TDrawPoint();
	TDrawPoint( const CRect& position );
	TDrawPoint( const TDrawPoint& );
	virtual ~TDrawPoint();

// Implementation
public:	
	virtual void Draw( CDC* pDC, double dSc );
	virtual int GetHandleCount();
	virtual CPoint GetHandle( int nHandle );
	virtual int GetGlueCount();
	virtual CPoint GetGlue( int nHandle );
	virtual CPoint GetGlue_Muled( int nHandle );
	virtual HCURSOR GetHandleCursor( int nHandle, bool bFlDrag );	
	virtual BOOL Intersects( const CRect& rect );
	virtual TDrawShape* Clone( TDiplomDoc* pDoc );

	virtual void MoveTo( const CRect& positon, TDiplomView* pView = NULL );
	virtual void MoveHandleTo( int nHandle, CPoint point, TDiplomView* pView = NULL, bool bFlEQ = false );

	virtual void AR_Store( CArchive& rA );
	virtual void AR_Load( CArchive& rA );
	virtual int isA() const
	 {
	   return TT_Point;
	 }

	

	virtual void SetScaled( double dSc );

	virtual int HToGlu( int iH )
	 {
	   return iH == 1 ? 1:-1;
	 }

	virtual int GetGDICount() const
	 {
       return 2;
	 }
	void InitGDI();

	virtual TDrToolItem* GetDrTool( TTypesDrawTool typ );

	virtual int PenX() const
	 {
	   return const_cast<TDrawPoint*>(this)->m_tpPen.GetData().m_iWidth;
	 }
    virtual int PenY() const
	 {
	   return const_cast<TDrawPoint*>(this)->m_tpPen.GetData().m_iWidth;
	 }

	virtual TDrawShape* GoTop( TCnct*& rP, TDrawShape* pExc, bool bFlLbl, bool& bFlNoEnter, bool bMarkUsed );
	virtual TDrawShape* GoBottom( TCnct*& rP, TDrawShape* pExc, bool bFlLbl, bool bMarkUsed );

	bool IsInBiPolus( short sNum )
	 {
	   vector<short>::iterator it = find( m_vecBiPolus.begin(), m_vecBiPolus.end(), sNum );
	   return it != m_vecBiPolus.end();
	 }
	void ClearBiPolus()
	 {
       m_vecBiPolus.clear();
	   m_vecBiPolus.resize( 0 );
	 }
	bool MarkBiPolus( short sNum )
	 {
       if( !IsInBiPolus(sNum) )
		{
		  m_vecBiPolus.push_back( sNum );
		  return true;
		}
	   return false;
	 }
	

	virtual void HighLight( bool bRedraw = true );	 
	virtual void UnHighLight( bool bRedraw = true );
	virtual bool IsHighLighted()
	 {
	   return m_bHighLight;
	 }

protected:
  TTItem_Pen    m_tpPen;
  TTItem_Brush  m_brBrush;

  vector<short> m_vecBiPolus;
  

  COLORREF m_clrKey;
  bool m_bHighLight;
 };

CArchive& operator<<( CArchive& rA, TDrawPoint& rCh );
CArchive& operator>>( CArchive& rA, TDrawPoint& rCh );

class TDrawTxt: public TDrawShape
 {

friend CArchive& operator<<( CArchive& rA, TDrawTxt& rCh );
friend CArchive& operator>>( CArchive& rA, TDrawTxt& rCh );
friend class TPRopPage_Text;

public:
	TDrawTxt();
	TDrawTxt( const CRect& position );
	TDrawTxt( const TDrawTxt& );
	virtual ~TDrawTxt();

// Implementation
public:	
	virtual void Draw( CDC* pDC, double dSc );
	virtual int GetHandleCount();
	virtual CPoint GetHandle( int nHandle );
	virtual int GetGlueCount();
	virtual CPoint GetGlue( int nHandle );
	virtual CPoint GetGlue_Muled( int nHandle );
	virtual HCURSOR GetHandleCursor( int nHandle, bool bFlDrag );	
	virtual BOOL Intersects( const CRect& rect );
	virtual TDrawShape* Clone( TDiplomDoc* pDoc );

	virtual void MoveTo( const CRect& positon, TDiplomView* pView = NULL );
	virtual void MoveHandleTo( int nHandle, CPoint point, TDiplomView* pView = NULL, bool bFlEQ = false );

	virtual void AR_Store( CArchive& rA );
	virtual void AR_Load( CArchive& rA );
	virtual int isA() const
	 {
	   return TT_Text;
	 }

	

	virtual void SetScaled( double dSc );

	LPCSTR InitTxt() const { return "Текст"; }
	void SetBounds();
	void SetBounds( int iWCh, int iHCh );

	virtual int GetGDICount() const
	 {
       return 1;
	 }
	void InitGDI();

	virtual TDrToolItem* GetDrTool( TTypesDrawTool typ );

	void SetML( bool bFl )
	 {
	   m_bMultiLine = bFl;
	 }
	void SetLock( bool bFl )
	 {
	   m_bLock = bFl;
	 }
	virtual bool IsLocked() const
	 {
	   return m_bLock;
	 }
	void SetAln( UINT ui )
	 {
	   m_uiAln = ui;
	 }

	void SetTxt( LPCSTR lp );
	void BoundsRound();

protected:	    
  TTItem_Font   m_fnFont;  
  CString m_csTxt;
  CMutex m_mtx;

  bool m_bMultiLine, m_bLock;
  UINT m_uiAln;
 };

CArchive& operator<<( CArchive& rA, TDrawTxt& rCh );
CArchive& operator>>( CArchive& rA, TDrawTxt& rCh );


class TDrawRuler: public TDrawShape
 {

friend CArchive& operator<<( CArchive& rA, TDrawRuler& rCh );
friend CArchive& operator>>( CArchive& rA, TDrawRuler& rCh );

public:
	TDrawRuler();
	TDrawRuler( const CRect& position );
	TDrawRuler( const TDrawRuler& );
	virtual ~TDrawRuler();

// Implementation
public:	
	virtual void Draw( CDC* pDC, double dSc );
	virtual int GetHandleCount();
	virtual CPoint GetHandle( int nHandle );
	virtual int GetGlueCount();
	virtual CPoint GetGlue( int nHandle );
	virtual CPoint GetGlue_Muled( int nHandle );
	virtual HCURSOR GetHandleCursor( int nHandle, bool bFlDrag );	
	virtual BOOL Intersects( const CRect& rect );
	virtual TDrawShape* Clone( TDiplomDoc* pDoc );

	virtual void MoveTo( const CRect& positon, TDiplomView* pView = NULL );
	virtual void MoveHandleTo( int nHandle, CPoint point, TDiplomView* pView = NULL, bool bFlEQ = false );

	virtual void AR_Store( CArchive& rA );
	virtual void AR_Load( CArchive& rA );
	virtual int isA() const
	 {
	   return TT_Ruler;
	 }
	
	void SetVert( bool bFl, const CPoint& rPt );
	 
	bool GetVert() const
	 {
       return m_bFlVert;
	 }

	virtual void ScaleUser( double dSc, CPoint& pt );

	virtual void ChCursor( HCURSOR& );	 

	virtual int GetGDICount() const
	 {
       return 2;
	 }	
	void InitGDI();

	virtual int PenX() const
	 {
	   return 1;
	 }
    virtual int PenY() const
	 {
	   return 1;
	 }

	bool GetFlag() const
	 {
	   return m_bFlVert;
	 }

protected:	  
  bool m_bFlVert;

  TTItem_Pen    m_tpPen, m_tpPenSel; 
 };

CArchive& operator<<( CArchive& rA, TDrawRuler& rCh );
CArchive& operator>>( CArchive& rA, TDrawRuler& rCh );


#endif // __DRAWOBJ_H__
