#include "stdafx.h"
#include "drawobj.h"
#include "diplomView.h"
#include "diplomDoc.h"
#include "childfrm.h"
#include "resource.h"

#include "clip_sz.hpp"

#include <math.h>
#include <strstream>
#include <iomanip>

#include <algorithm>
using namespace std;

int TDrawShape::m_iGlueLevel = 0;

inline bool IsRulesVisible( TDiplomDoc *pDoc ) 
 {
   return pDoc->m_pChFrame->m_optX.m_dLines;
 }

TDrawShape::TDrawShape():
  m_position( 0, 75, 150, 0 )
 {
   m_bEnableMultiIn = m_bSelected = false;   
   m_dSc = 1;
   m_iSelCount = 0;
   m_iActiveGlue = -1;
   m_bInve = false;      

   m_iUsed = 0;

   m_iLockCnt = 0;
 }

TDrawShape::~TDrawShape()
 {
 }

TDrawShape::TDrawShape( const CRect& position )
 {
	m_position = position;	
	m_pDocument = NULL;
	m_bEnableMultiIn = m_bSelected = false;
	m_dSc = 1;
	m_iSelCount = 0;
	m_iActiveGlue = -1;
	m_bInve = false;

	m_iUsed = 0;

	m_iLockCnt = 0;
 }

TDrawShape::TDrawShape( const TDrawShape& rShp )
 {
   m_bEnableMultiIn = rShp.m_bEnableMultiIn;
   m_position = rShp.m_position;   
   m_pDocument = rShp.m_pDocument;
   m_dSc = rShp.m_dSc;

   m_iSelCount = 0;

   m_iActiveGlue = -1;
   m_bInve = false;

   m_iUsed = rShp.m_iUsed;

   //m_bSelected
   //m_iID

   m_iLockCnt = 0;
 }


CRect TDrawShape::GetPosition( bool bFlWithTracker ) const
 {
   CRect r( m_position ); 
   r.NormalizeRect();
   //r.InflateRect( PenX(), PenY() );
   if( bFlWithTracker )
	 r.InflateRect( DeltaTracker()+1, DeltaTracker()+1 );   

   return r;
 }

bool TDrawShape::IsConnectedTo( TDrawShape* pS, int inGlu )
 {
   list<TGlueDescr>::iterator itSta( m_vecGlu[inGlu - 1]->m_lstGlu.begin() );
   list<TGlueDescr>::iterator itEnd( m_vecGlu[inGlu - 1]->m_lstGlu.end() );
   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta).IsGlued() )
	  {
        TGlue *pGlue = ((TCnct*)(*itSta).m_pShp)->m_vecGlu[ ((TCnct*)(*itSta).m_pShp)->Another((*itSta).m_nGlu) - 1 ].get();
		if( (!pGlue->m_lstGlu.size() || !pGlue->m_lstGlu.front().IsGlued()) && !pS ) return true;
        if( pGlue->m_lstGlu.size() && pGlue->m_lstGlu.front().IsGlued() && 
		    pS == pGlue->m_lstGlu.front().m_pShp
		  ) return true;
	  }

   return false;
 }

void TDrawShape::Remove()
 {
	delete this;
 }

void TDrawShape::Draw( CDC*, double dSc )
 {
 }

TCnct* TDrawShape::FindNotUsedCnct( int iPoint_Ind, int& iglCnct )
 {
   list<TGlueDescr>::iterator itSta( m_vecGlu[ iPoint_Ind ]->m_lstGlu.begin() );
   list<TGlueDescr>::iterator itEnd( m_vecGlu[ iPoint_Ind ]->m_lstGlu.end() );

   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta).IsGlued() )
	  {
	    ASSERT( (*itSta).m_pShp->isA() == TT_Cnct );
		if( ((TCnct*)(*itSta).m_pShp)->IsUsed() == 0 ) 
		 {
		   iglCnct = (*itSta).m_nGlu;
		   return (TCnct*)(*itSta).m_pShp;
		 }
	  }
   return NULL;
 }

void TDrawShape::SelectEmptyLink( int& iCnt )
 {
   if( isA() == TT_PC || isA() == TT_Point )	
     for( int i = m_vecGlu.size() - 1;  i >= 0; --i )
	  {
        list<TGlueDescr>::iterator itSta( m_vecGlu[ i ]->m_lstGlu.begin() );
        list<TGlueDescr>::iterator itEnd( m_vecGlu[ i ]->m_lstGlu.end() ); 
		for( ; itSta != itEnd; ++itSta )
		  if( (*itSta).IsGlued() )
		   {
             TCnct *p = (TCnct*)(*itSta).m_pShp;
			 TGlue *pD = p->m_vecGlu[p->Another( (*itSta).m_nGlu ) - 1].get();
			 if( !pD->m_lstGlu.size() || !pD->m_lstGlu.front().IsGlued() )
			  {
                p->SetSel_Internal( true );	  
			    p->m_iSelCount = iCnt++;
			  }
		   }
	  }
 }

TCnct *TDrawShape::GetLinkTo( TDrawShape* pSh )
 {
   for( int i = m_vecGlu.size() - 1; i >= 0; --i )
	{
      list<TGlueDescr>::iterator itSta( m_vecGlu[ i ]->m_lstGlu.begin() );
      list<TGlueDescr>::iterator itEnd( m_vecGlu[ i ]->m_lstGlu.end() );

	  for( ; itSta != itEnd; ++itSta )
	    if( (*itSta).IsGlued() && (*itSta).m_pShp->isA() == TT_Cnct )
		 {
           TCnct *p = (TCnct*)(*itSta).m_pShp;
		   TGlue *pD = p->m_vecGlu[p->Another( (*itSta).m_nGlu ) - 1].get();
		   if( pD->m_lstGlu.size() && pD->m_lstGlu.front().IsGlued() &&
			   pD->m_lstGlu.front().m_pShp == pSh
			 )
			return p;
		 }
	}

   return NULL;
 }

TCnct* TDrawShape::FindNextCnct( int iPoint_Ind, int& iglCnct, TCnct *pCnct )
 {
   list<TGlueDescr>::iterator itSta( m_vecGlu[ iPoint_Ind ]->m_lstGlu.begin() );
   list<TGlueDescr>::iterator itEnd( m_vecGlu[ iPoint_Ind ]->m_lstGlu.end() );

   bool bFlFnd = pCnct == NULL ? true:false;

   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta).IsGlued() )
	  {
	    ASSERT( (*itSta).m_pShp->isA() == TT_Cnct );
		if( bFlFnd )
		 {
		   iglCnct = (*itSta).m_nGlu;
		   return (TCnct*)(*itSta).m_pShp;
		 }
		else if( pCnct == (TCnct*)(*itSta).m_pShp )
		  bFlFnd = true;
	  }
   return NULL;
 }

TDrawShape* TDrawPC::GoBottom( TCnct*& rP, TDrawShape* pExc, bool bFlLbl, bool bMarkUsed )
 {
   int iCnct;
   if( bFlLbl )
	 rP = FindNotUsedCnct( 0, iCnct );
   else
     rP = FindNextCnct( 0, iCnct, rP );
   //if( m_vecGlu[1]->m_lstGlu.size() ) bFlNoEnter = true;
   //else bFlNoEnter = false;
   if( !rP ) return NULL;
     
   if( bMarkUsed ) rP->SetUsedFlag( true );
             /*rP->HighLight();//??????????
			 m_pDocument->GetMainViev()->UpdateWindow();
			 Sleep( 2000 );*/   
   TDrawShape *pFnd = rP->GoLinkBack( rP->Another(iCnct) - 1, bMarkUsed );
   
   return pFnd;
 }

TDrawShape *TCnct::GoLinkBack( int iInd, bool bMarkUsed )
 {
   if( !m_vecGlu[ iInd ]->m_lstGlu.size() || 
	   !m_vecGlu[ iInd ]->m_lstGlu.front().IsGlued()
	 ) 
	{	  
	  if( bMarkUsed ) SetUsedFlag( true );
	  return NULL;
	}

   if( m_vecGlu[ iInd ]->m_lstGlu.front().m_pShp->m_vecGlu[m_vecGlu[iInd]->m_lstGlu.front().m_nGlu - 1]->m_tgTyp == TG_In )
	{
	  return NULL;
	}   
   
   return m_vecGlu[ iInd ]->m_lstGlu.front().m_pShp;
 }

TDrawShape* TDrawPC::GoTop( TCnct*& rP, TDrawShape*, bool bFlLbl, bool& bFlNoEnter, bool bMarkUsed )
 {
   int iCnct;
   if( bFlLbl )
	 rP = FindNotUsedCnct( 1, iCnct );
   else
     rP = FindNextCnct( 1, iCnct, rP );
   if( m_vecGlu[1]->m_lstGlu.size() ) bFlNoEnter = true;
   else bFlNoEnter = false;
   if( !rP ) return NULL;
     
   if( bMarkUsed ) rP->SetUsedFlag( true );
             /*rP->HighLight();//??????????
			 m_pDocument->GetMainViev()->UpdateWindow();
			 Sleep( 2000 );*/
   TDrawShape *pFnd = rP->GoLink( rP->Another(iCnct) - 1, bFlNoEnter, bMarkUsed );
   
   return pFnd;
 }
TDrawShape *TCnct::GoLink( int iInd, bool& bFlNoEnter, bool bMarkUsed )
 {   
   if( !m_vecGlu[ iInd ]->m_lstGlu.size() || 
	   !m_vecGlu[ iInd ]->m_lstGlu.front().IsGlued()
	 ) 
	{
	  bFlNoEnter = false;
	  if( bMarkUsed ) SetUsedFlag( true );
	  return NULL;
	}

   if( m_vecGlu[ iInd ]->m_lstGlu.front().m_pShp->m_vecGlu[m_vecGlu[iInd]->m_lstGlu.front().m_nGlu - 1]->m_tgTyp == TG_Out )
	{
	  bFlNoEnter = true;
	  return NULL;
	}
   
   
   return m_vecGlu[ iInd ]->m_lstGlu.front().m_pShp;
 }
TDrawShape* TCnct::GoTop( TCnct*& rP, TDrawShape*, bool, bool& bFlNoEnter, bool bMarkUsed )
 {
   return NULL;
 }
TDrawShape* TCnct::GoBottom( TCnct*& rP, TDrawShape* pExc, bool bFlLbl, bool bMarkUsed )
 {
   return NULL;
 }
TDrawShape* TDrawPoint::GoBottom( TCnct*& rP, TDrawShape* pExc, bool bFlLbl, bool bMarkUsed )
 {
   list<TGlueDescr>::iterator itSta( m_vecGlu[ 0 ]->m_lstGlu.begin() );
   list<TGlueDescr>::iterator itEnd( m_vecGlu[ 0 ]->m_lstGlu.end() );

   bool bFlFound = rP == NULL ? true:false;   

   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta).IsGlued() )
	  {
        
	    ASSERT( (*itSta).m_pShp->isA() == TT_Cnct );
		if( bFlFound ) 
		 {
		   rP = (TCnct*)(*itSta).m_pShp;		   
		   TDrawShape *pFnd = rP->GoLinkBack( rP->Another((*itSta).m_nGlu) - 1, bMarkUsed );
		   if( pFnd && pFnd != pExc ) 
			{
              if( bMarkUsed ) rP->SetUsedFlag( true );
			      /*rP->HighLight(); //??????????
				  m_pDocument->GetMainViev()->UpdateWindow();
			      Sleep( 2000 );*/
			  return pFnd;
			}
		 }
		else if( (TCnct*)(*itSta).m_pShp == rP )
		  bFlFound = true;
	  }
   
   rP = NULL;
   return NULL;
 }
TDrawShape* TDrawPoint::GoTop( TCnct*& rP, TDrawShape* pExc, bool, bool& bFlNoEnter, bool bMarkUsed )
 {
   list<TGlueDescr>::iterator itSta( m_vecGlu[ 0 ]->m_lstGlu.begin() );
   list<TGlueDescr>::iterator itEnd( m_vecGlu[ 0 ]->m_lstGlu.end() );

   bool bFlFound = rP == NULL ? true:false;
 
   //if( m_vecGlu[0]->m_lstGlu.size() > 1 ) bFlNoEnter = true;
   //else bFlNoEnter = false;

   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta).IsGlued() )
	  {
        
	    ASSERT( (*itSta).m_pShp->isA() == TT_Cnct );
		if( bFlFound ) 
		 {
		   rP = (TCnct*)(*itSta).m_pShp;		   
		   TDrawShape *pFnd = rP->GoLink( rP->Another((*itSta).m_nGlu) - 1, bFlNoEnter, bMarkUsed );
		   if( pFnd && pFnd != pExc ) 
			{
              if( bMarkUsed ) rP->SetUsedFlag( true );
			      /*rP->HighLight(); //??????????
				  m_pDocument->GetMainViev()->UpdateWindow();
			      Sleep( 2000 );*/
			  return pFnd;
			}
		 }
		else if( (TCnct*)(*itSta).m_pShp == rP )
		  bFlFound = true;
	  }

   itSta = m_vecGlu[ 0 ]->m_lstGlu.begin();
   itEnd = m_vecGlu[ 0 ]->m_lstGlu.end();
   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta).IsGlued() )
	  {
	    TCnct *pS = (TCnct*)(*itSta).m_pShp;
	    if( !pS->m_vecGlu[pS->Another((*itSta).m_nGlu) - 1]->m_lstGlu.size() )
		 {
           bFlNoEnter = false;
		   break;
		 }
	  }

   rP = NULL;
   return NULL;
 }

TDrToolItem* TDrawShape::GetDrTool( TTypesDrawTool typ )
 {
   return NULL;
 }

class TDtaCntSel
 {
public:
  TDtaCntSel( int& iCnt ): m_iCnt( iCnt ) 
   {
     m_iCnt = 0;
   }

  void operator()( TAuto_DrawShape& rCh )
   {
     if( rCh->IsSelected() ) ++m_iCnt;
   }
protected:
  int& m_iCnt;
 };

inline void Scale_Pt( int& x, int& y, double dSc, CPoint& pt )
 {
   x = double(x)*dSc + double(pt.x)*(1.0 - dSc);
   y = double(y)*dSc + double(pt.y)*(1.0 - dSc);
 }

inline void Scale_Pt( long& x, long& y, double dSc, CPoint& pt )
 {
   x = double(x)*dSc + double(pt.x)*(1.0 - dSc);
   y = double(y)*dSc + double(pt.y)*(1.0 - dSc);
 }

void TDrawShape::ScaleUser( double dSc, CPoint& pt )
 {   
   Scale_Pt( m_position.left, m_position.top, dSc, pt );
   Scale_Pt( m_position.right, m_position.bottom, dSc, pt );
 }
void TDrawPC::ScaleUser( double dSc, CPoint& pt )
 {
   TDrawShape::ScaleUser( dSc, pt );
   //Scale_Pt( m_roundness.x, m_roundness.y, dSc, pt );   
   m_roundness.x = double(m_roundness.x) * dSc;
   m_roundness.y = double(m_roundness.y) * dSc;
 }
void TCnct::ScaleUser( double dSc, CPoint& pt )
 {
   TDrawShape::ScaleUser( dSc, pt );
   int ii = 0;
   //Scale_Pt( m_iKln, ii, dSc, pt );      
   m_iKln = double(m_iKln) * dSc;
 }
void TDrawRuler::ScaleUser( double dSc, CPoint& pt )
 {
 }

void TDrawShape::Select( TDrawShapeList& rL, bool bFl )
 {
   if( bFl == false ) m_iSelCount = 0;
   else
	{
	  int iCnt = 0;
      TDtaCntSel dta( iCnt );
	  for_each( rL.begin(), rL.end(), dta );
	  m_iSelCount = iCnt + 1;
	}
   m_bSelected = bFl;   
 }

bool TGlue::Glu( TDrawShape* pShp, int nGluForin, TDrawShape* pOwner, int nGluNative )
 {
   ASSERT( nGluForin != -1 );
   ASSERT( nGluNative != -1 );

   if( !m_lstGlu.size() ) m_lstGlu.push_back( TGlueDescr() );
   TGlueDescr &rDscr = m_lstGlu.front();
   if( rDscr.m_iID == pShp->ID() && rDscr.m_nGlu == nGluForin ) return false;
   
   rDscr.m_iID = pShp->ID();
   rDscr.m_nGlu = nGluForin;
   rDscr.m_pShp = pShp;
   pShp->m_vecGlu[ nGluForin - 1 ]->AddGlu( pOwner, nGluNative );
   if( TDrawShape::m_iGlueLevel == 2 ) pShp->m_iActiveGlue = nGluForin;

   return true;
 }
bool TGlue::DeGlu( TDrawShape* pOwner, int nGluNative, TDiplomView *pView )
 {
   ASSERT( nGluNative != -1 );

   //if( !m_lstGlu.size() || m_lstGlu.front().m_iID == -1 ) return false;   
   bool bFl = false;
   list<TGlueDescr>::iterator itSta( m_lstGlu.begin() );
   list<TGlueDescr>::iterator itEnd( m_lstGlu.end() );
   for( ; itSta != itEnd; )
	{
      TGlueDescr &rDscr = *itSta;
	  if( rDscr.m_iID == -1 ) 
	   {
         ++itSta; continue;
	   }	  

	  bFl = true;
	  if( pView ) pView->InvalObj( rDscr.m_pShp, true );

	  rDscr.m_pShp->m_vecGlu[ rDscr.m_nGlu - 1 ]->RemoveGlu( pOwner, nGluNative );
	  rDscr.m_pShp->m_iActiveGlue = -1;
	  rDscr.m_pShp = NULL;
	  rDscr.m_nGlu = rDscr.m_iID = -1;   

	  m_lstGlu.erase( itSta++ );
	}
   
   return bFl;
 }

bool TGlue::RemoveGlu( TDrawShape* pShp, int nGluForin )
 {   
   ASSERT( nGluForin != -1 );

   TFndGluDta dta( nGluForin, pShp->ID() );
   list<TGlueDescr>::iterator it = find_if( m_lstGlu.begin(), m_lstGlu.end(), dta );
   if( it == m_lstGlu.end() ) return false;
	
   m_lstGlu.erase( it );

   return true;	   
 }

bool TGlue::AddGlu( TDrawShape* pShp, int nGluForin )
 {
   ASSERT( nGluForin != -1 );

   TFndGluDta dta( nGluForin, pShp->ID() );
   list<TGlueDescr>::iterator it = find_if( m_lstGlu.begin(), m_lstGlu.end(), dta );
   if( it == m_lstGlu.end() )
	{
	  m_lstGlu.push_back( TGlueDescr(nGluForin, pShp) );
	  return true;
	}

   return false;
 }

void TDrawShape::DrawGlue( CDC* pDC, TrackerState state, CPen* pNorm, CPen* pSel )
 {
   ASSERT_VALID(this);
   
   int nHandleCount = GetGlueCount();
   CBrush* pBrOld = (CBrush*)pDC->SelectStockObject( NULL_BRUSH );    
   CPen *penOld = pDC->SelectObject( pNorm );
	   
   bool bDraw = m_pDocument->GetFrameOpt()->m_bFlGluePoints && !IsSelected();

   int iDelt = DeltaTracker();
   int iWH = 2 * iDelt;

   for( int nHandle = 1; nHandle <= nHandleCount; nHandle += 1 )
	{
	  CPoint handle = GetGlue_Muled( nHandle ); 
	  //MulPt( handle, handle, m_dSc );

	  if( bDraw )
	   {         		 
		 pDC->SelectObject( pNorm );
		 pDC->MoveTo( handle.x - iDelt, handle.y - iDelt );
		 pDC->LineTo( handle.x + iDelt, handle.y + iDelt );
		 pDC->MoveTo( handle.x + iDelt, handle.y - iDelt );
		 pDC->LineTo( handle.x - iDelt, handle.y + iDelt );		  		 
	   }

	  if( m_iActiveGlue == nHandle )
	   {
         pDC->SelectObject( pSel );
         pDC->Rectangle( handle.x - iWH, handle.y - iWH, handle.x + iWH, handle.y + iWH );
	   }	   
	}

   pDC->SelectObject( penOld );
   pDC->SelectObject( pBrOld );
 }

void TDrawShape::MakeOffset( const CPoint& rPt )
 {
   m_position.OffsetRect( rPt );
 }

void TDrawShape::DrawTracker( CDC* pDC, TrackerState state )
 {
	ASSERT_VALID(this);

	//CBrush brGlu( RGB(255,0,0) );

	switch (state)
	{
	  case normal:
		  break;

	  case selected:
	  case active:
	   {
		 int nHandleCount = GetHandleCount();
		 int iDelt = DeltaTracker();
		 int iWH = 2 * iDelt + 1;
		 //CBrush *pBr = (CBrush*)pDC->SelectStockObject( BLACK_BRUSH );
		 for( int nHandle = 1; nHandle <= nHandleCount; nHandle += 1 )
		  {
			CPoint handle = GetHandle(nHandle);
			MulPt( handle, handle, m_dSc );
			int iIndGlu = HToGlu( nHandle );
			bool bFlGl = iIndGlu == -1 ? false:
			  (m_vecGlu[iIndGlu-1]->m_lstGlu.size() && 
			   m_vecGlu[iIndGlu-1]->m_lstGlu.front().IsGlued());
			CBrush *pOldBr;
			if( bFlGl ) 
			  pOldBr = pDC->SelectObject( m_pDocument->MBrush_GlueSel() );
			pDC->PatBlt( handle.x - iDelt, handle.y - iDelt, iWH, iWH, PATCOPY );
			if( bFlGl ) pDC->SelectObject( pOldBr );
			/*C->Rectangle( handle.x - 4, handle.y - 4, 
			  handle.x + 4, handle.y + 4 );*/
		  }
		 /*pDC->SelectObject( pBr );
		 for( nHandle = 1; nHandle <= nHandleCount; nHandle += 1 )
		  {
			CPoint handle = GetHandle( nHandle );
			pDC->PatBlt( handle.x - 2, handle.y - 2, 5, 5, PATCOPY );		
		  }*/
	   }
	  break;
	}
 }

void TGlue::RecalcGlued( TDrawShape* pShpOwn, int nOwn, TDiplomView *pView )
 {   
   CPoint pt( pShpOwn->GetGlue(nOwn) );

   list<TGlueDescr>::iterator itSta( m_lstGlu.begin() ); 
   list<TGlueDescr>::iterator itEnd( m_lstGlu.end() );
   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta).IsGlued() )
	   (*itSta).m_pShp->MoveHandleTo( (*itSta).m_nGlu, pt, pView, false );
 }

void TDrawShape::ShiftIDs( int iShft )
 {
   for( int i = m_vecGlu.size() - 1; i >= 0; --i )
	{
	  list<TGlueDescr>::iterator itSta( m_vecGlu[i]->m_lstGlu.begin() );
	  list<TGlueDescr>::iterator itEnd( m_vecGlu[i]->m_lstGlu.end() );
	  for( ; itSta != itEnd; ++itSta )
	    (*itSta).m_iID += iShft;
	}
 }

void TDrawShape::RecalcGlued( TDiplomView *pView, bool bFlEnd )//passive
 {
   TVarHolderVal<int> hold( TDrawShape::m_iGlueLevel, 0 );

   TVec_Glue::iterator itSta( m_vecGlu.begin() );
   TVec_Glue::iterator itEnd( m_vecGlu.end() );
   for( int i = 1; itSta != itEnd; ++itSta, ++i )	 
	   (*itSta)->RecalcGlued( this, i, pView );

   if( bFlEnd && m_iActiveGlue != -1 )
	{
	  m_iActiveGlue = -1;
	  if( pView ) pView->InvalObj( this, true );
	}
 }
void TDrawShape::CheckGlued( TDiplomView *pView )//active
 {   
   bool bFlRefr = false;

   for( int i = m_vecGlu.size(); i > 0; --i )
	{
      CPoint pt( GetGlue(i) );
	  int nGlue, nDragHandle;
	  //TDrawShape* pObj = pView->HitTest( pt, nDragHandle, nGlue, this );
	  TDrawShape* pObj = pView->GetDocument()->HitTest2( pt, nDragHandle, nGlue, this );

	  bool bFl = false;
	  if( !nGlue ) bFl = DeGlue( i, pView );
	  else if( pObj && TDrawShape::m_iGlueLevel == 2 ) 
	    MakeGlue( i, pObj, nGlue, bFl );

	  if( bFl )
	   {
	     if( pObj ) pView->InvalObj( pObj, true );
	     bFlRefr = true;
	   }
	}

   if( bFlRefr ) 
	 pView->InvalObj( this, true );
 }

void TDrawShape::CheckGlued0( TDiplomView *pView )//active
 {   
   bool bFlRefr = false;

   for( int i = m_vecGlu.size(); i > 0; --i )
	{
      CPoint pt( GetGlue(i) );
	  int nGlue, nDragHandle;
	  //TDrawShape* pObj = pView->HitTest0( pt, nDragHandle, nGlue, this );
	  TDrawShape* pObj = pView->GetDocument()->HitTest0( pt, nDragHandle, nGlue, this );

	  bool bFl = false;
	  if( !nGlue ) bFl = DeGlue( i, pView );
	  else if( pObj && TDrawShape::m_iGlueLevel == 2 ) 
	    MakeGlue( i, pObj, nGlue, bFl );

	  if( bFl )
	   {
	     if( pObj ) pView->InvalObj( pObj, true );
	     bFlRefr = true;
	   }
	}

   if( bFlRefr ) 
	 pView->InvalObj( this, true );
 }

bool TDrawShape::CompatibleGlu( int iInvGlu_Ind, TDrawShape *pShp, int nGluForin_Ind )
 {
   if( !m_vecGlu[iInvGlu_Ind]->m_lstGlu.size() )
	 return true;

   TGlueDescr &p = m_vecGlu[ iInvGlu_Ind ]->m_lstGlu.front();

   if( !p.IsGlued() || !p.m_pShp ) return true;
   if( p.m_pShp == pShp ) return false;
   
   TGTypes t1 = p.m_pShp->m_vecGlu[p.m_nGlu - 1]->m_tgTyp;
   TGTypes t2 = pShp->m_vecGlu[ nGluForin_Ind ]->m_tgTyp;
   return t1 != t2 || t1 == TG_InOut || t2 == TG_InOut;
 }

bool TDrawShape::MakeGlue( int nGlu, TDrawShape* pShp, int nGluForin, bool& rMade )
 {
   ASSERT( IsActiveGlue() );
   
   rMade = false;

   /*if( !(IsActiveGlue() != pShp->IsActiveGlue() && 
		m_vecGlu[nGlu - 1]->m_tgTyp != pShp->m_vecGlu[nGluForin - 1]->m_tgTyp)
	 ) return false;*/
   int iInvGlu = nGlu == 1 ? 1:0;
   if( pShp->IsActiveGlue() || 
	   !CompatibleGlu(iInvGlu, pShp, nGluForin - 1)	||
	   (pShp->m_vecGlu[ nGluForin - 1 ]->m_tgTyp == TG_In && 
	    (pShp->m_bEnableMultiIn == false && pShp->m_vecGlu[ nGluForin - 1 ]->m_lstGlu.size() > 1))
	 ) return false;

   rMade = 
	 m_vecGlu[nGlu - 1]->Glu( pShp, nGluForin, this, nGlu );         

   return true;
 }

bool TDrawShape::DeGlue( int nGlu, TDiplomView *pView )
 {
   //ASSERT( IsActiveGlue() );
  
   return  m_vecGlu[ nGlu - 1 ]->DeGlu( this, nGlu, pView );
 }
void TDrawShape::DeGlueAll( TDiplomView *pView )
 {
   //if( !IsActiveGlue() ) return;

   for( int i = GetGlueCount(); i > 0; --i )
	 DeGlue( i, pView );
 }

// position is in logical
void TDrawShape::MoveTo( const CRect& position, TDiplomView* pView )
 {
	ASSERT_VALID(this);

	if( position == m_position )
		return;

	if( pView == NULL )
	 {
	   Invalidate();
	   m_position = position;
	   Invalidate();
	 }
	else
	 {
	   pView->InvalObj(this);
	   m_position = position;
	   pView->InvalObj(this);
	 }
	m_pDocument->SetModifiedFlag();

	SetScaled( m_dSc );

	if( !TDrawShape::m_iGlueLevel ) return;
	
	if( !IsActiveGlue() )  RecalcGlued( pView );
	else CheckGlued( pView );
 }

// Note: if bSelected, hit-codes start at one for the top-left
// and increment clockwise, 0 means no hit.
// If !bSelected, 0 = no hit, 1 = hit (anywhere)

// point is in logical coordinates

//int TDrawShape::HitTest( CPoint point, TDiplomView* pView, BOOL bSelected )
TDrawShape* TDrawShape::HitTest( const CPoint& point, int& nHint, int& nGlue, TDiplomView* pView, bool bGlueOnly )
 {
	ASSERT_VALID(this);
	//ASSERT(pView != NULL);    

	nGlue = nHint = 0;

	int iDt = min( 5, max( 1, 2.0/m_dSc ) ); 
	//int iDt = 10; //kkkkkkkkkk
    CRect rDelta( point.x-iDt, point.y-iDt, point.x+iDt, point.y+iDt );
	/*CRect rKey( m_position ); 
	m_position.left -= 3; m_position.right += 3;
	m_position.top += 3; m_position.bottom -= 3;
	bool bIntTst = !Intersects( rDelta );
	m_position = rKey;
	if( bIntTst ) return NULL;*/

	bool bFullTst = IsSelected() && isA()==TT_PC && ((TDrawPC*)this)->ShortView();
	//bool bFullTst = IsSelected();
	if( !bFullTst && !Intersects(rDelta) ) return NULL;

	int nHandleCount, nHandle;
	if( bGlueOnly )
	 {
	   nHandleCount = GetGlueCount();
	   for( nHandle = 1; nHandle <= nHandleCount; nHandle += 1 )
		{
		  CRect rc = GetGlueRect( nHandle, pView );
		  if( point.x >= rc.left && point.x < rc.right &&
			  point.y <= rc.top && point.y > rc.bottom )
		   {
			 nGlue = nHandle;
			 break;
		   }
		}
	 }
    else if( m_bSelected )
	 {
	   nHandleCount = GetHandleCount();
	   for( nHandle = 1; nHandle <= nHandleCount; nHandle += 1 )
		{
		   // GetHandleRect returns in logical coords
		   CRect rc = GetHandleRect(nHandle,pView);
		   if (point.x >= rc.left && point.x < rc.right &&
			   point.y <= rc.top && point.y > rc.bottom)
			{
			   nHint = nHandle;        			   
			   break;
			}
		}	   	
	 }

	if( nHint || nGlue ) return this;	

	if( bFullTst && !Intersects(rDelta) ) return NULL;

	return this;	
 }

// rect must be in logical coordinates
BOOL TDrawShape::Intersects( const CRect& rect )
 {
	ASSERT_VALID(this);

	//CRect fixed = m_position;
	//fixed.NormalizeRect();
	CRect fixed = GetPosition();
	CRect rectT = rect;
	rectT.NormalizeRect();
	return !(rectT & fixed).IsRectEmpty();
 }

int TDrawShape::GetGlueCount()
 {
   ASSERT_VALID(this);
   return 2;
 }
   
CPoint TDrawShape::GetGlue_Muled( int nHandle )
 {
   ASSERT_VALID(this);
   int x, y, xCenter, yCenter;   

   // this gets the center regardless of left/right and top/bottom ordering
   CRect rp; MulRect( rp, m_position, m_dSc );
   xCenter = rp.left + rp.Width() / 2;
   yCenter = rp.top + rp.Height() / 2;

   switch( nHandle )
   {
   default:
	   ASSERT(FALSE);   

   case 1:
	   x = xCenter;
	   y = rp.top;
	   break;      

   case 2:
	   x = xCenter;
	   y = rp.bottom + 1;
	   break;   
   }

   return CPoint(x, y);
 }

CPoint TDrawShape::GetGlue( int nHandle )
 {
   ASSERT_VALID(this);
   int x, y, xCenter, yCenter;   

   // this gets the center regardless of left/right and top/bottom ordering
   xCenter = m_position.left + m_position.Width() / 2;
   yCenter = m_position.top + m_position.Height() / 2;

   switch( nHandle )
   {
   default:
	   ASSERT(FALSE);   

   case 1:
	   x = xCenter;
	   y = m_position.top;
	   break;      

   case 2:
	   x = xCenter;
	   y = m_position.bottom + 1;
	   break;   
   }

   return CPoint(x, y);
 }

int TDrawShape::GetHandleCount()
 {
   ASSERT_VALID(this);
   return 8;
 }

// returns logical coords of center of handle
CPoint TDrawShape::GetHandle( int nHandle )
 {
   ASSERT_VALID(this);
   int x, y, xCenter, yCenter;
   

   // this gets the center regardless of left/right and top/bottom ordering
   xCenter = m_position.left + m_position.Width() / 2;
   yCenter = m_position.top + m_position.Height() / 2;

   switch( nHandle )
   {
   default:
	   ASSERT(FALSE);

   case 1:
	   x = m_position.left;
	   y = m_position.top;
	   break;

   case 2:
	   x = xCenter;
	   y = m_position.top;
	   break;

   case 3:
	   x = m_position.right;
	   y = m_position.top;
	   break;

   case 4:
	   x = m_position.right;
	   y = yCenter;
	   break;

   case 5:
	   x = m_position.right;
	   y = m_position.bottom;
	   break;

   case 6:
	   x = xCenter;
	   y = m_position.bottom;
	   break;

   case 7:
	   x = m_position.left;
	   y = m_position.bottom;
	   break;

   case 8:
	   x = m_position.left;
	   y = yCenter;
	   break;
   }

   return CPoint(x, y);
 }

CRect TDrawShape::GetGlueRect( int nHandleID, TDiplomView* pView )
 {
   const int HANDLE_DELTA = 5;

   ASSERT_VALID(this);
   ASSERT(pView != NULL);

   CRect rect;
   // get the center of the handle in logical coords
   CPoint point = GetGlue( nHandleID );
   // convert to client/device coords
   pView->DocToClient_SC( &point );
   // return CRect of handle in device coords
   rect.SetRect( point.x-HANDLE_DELTA, point.y-HANDLE_DELTA, 
	             point.x+HANDLE_DELTA, point.y+HANDLE_DELTA );
   pView->ClientToDoc_SC( &rect );

   return rect;
 }

// return rectange of handle in logical coords
CRect TDrawShape::GetHandleRect( int nHandleID, TDiplomView* pView )
 {
   const int HANDLE_DELTA = 3;

   ASSERT_VALID(this);
   ASSERT(pView != NULL);

   CRect rect;
   // get the center of the handle in logical coords
   CPoint point = GetHandle( nHandleID );
   // convert to client/device coords
   pView->DocToClient_SC( &point );
   // return CRect of handle in device coords
   rect.SetRect( point.x-HANDLE_DELTA, point.y-HANDLE_DELTA, 
	             point.x+HANDLE_DELTA, point.y+HANDLE_DELTA );
   pView->ClientToDoc_SC( &rect );

   return rect;
 }

HCURSOR TDrawShape::GetHandleCursor( int nHandle, bool bFlDrag )
 {
   ASSERT_VALID(this);

   LPCTSTR id;
   switch (nHandle)
   {
   default:
	   ASSERT(FALSE);

   case 1:
   case 5:
	   id = IDC_SIZENWSE;
	   break;

   case 2:
   case 6:
	   id = IDC_SIZENS;
	   break;

   case 3:
   case 7:
	   id = IDC_SIZENESW;
	   break;

   case 4:
   case 8:
	   id = IDC_SIZEWE;
	   break;
   }

   return AfxGetApp()->LoadStandardCursor(id);
 }

inline int sign( int x )
 {
   return x < 0 ? -1:1;
 }

static void ScaleFrom( int iX, int iY, const CRect& rP1, CRect& rP2, double dSc )
 {
   rP2 = rP1; rP2.OffsetRect( -iX, -iY ); 

   rP2.left = double(rP2.left) * dSc;
   rP2.right = double(rP2.right) * dSc;
   rP2.top = double(rP2.top) * dSc;
   rP2.bottom = double(rP2.bottom) * dSc;

   rP2.OffsetRect( iX, iY ); 
 }

// point must be in logical
void TDrawShape::MoveHandleTo( int nHandle, CPoint point, TDiplomView* pView, bool bFlEQ )
 {
	ASSERT_VALID(this);

	CRect position = m_position;	
	long iX  = 0, iY = 0;
	switch( nHandle )
	 {
	  default:
		  ASSERT(FALSE);

	  case 1:
		  position.left = point.x;
		  position.top = point.y;		  
		  iX = position.right; iY = position.bottom;
		  break;

	  case 2:
		  position.top = point.y;		  		  
		  iX = position.right; iY = position.bottom;
		  break;

	  case 3:
		  position.right = point.x;
		  position.top = point.y;		  		  
		  iX = position.left; iY = position.bottom;
		  break;

	  case 4:
		  position.right = point.x;		  		  
		  iX = position.left; iY = position.bottom;
		  break;

	  case 5:
		  position.right = point.x;
		  position.bottom = point.y;		  	
		  iX = position.left; iY = position.top;
		  break;

	  case 6:
		  position.bottom = point.y;		  	
		  iX = position.left; iY = position.top;
		  break;

	  case 7:
		  position.left = point.x;
		  position.bottom = point.y;		  	
		  iX = position.right; iY = position.top;
		  break;

	  case 8:
		  position.left = point.x;		  		 
		  iX = position.right; iY = position.top;
		  break;
	 }

	if( bFlEQ )
	 {
       int w1 = abs(m_rFixRect.Width()), w2 = abs(position.Width());
	   int h1 = abs(m_rFixRect.Height()), h2 = abs(position.Height());
	   double dSc1 = w1 ? double(w2)/double(w1):1, 
		      dSc2 = h1 ? double(h2)/double(h1):1;
	   double dSc;
	   switch( nHandle )
		{	  
	      case 1:case 3:case 5:case 7:
		    dSc = max( dSc1, dSc2 );       
			break;
		  case 2:case 6:
		    dSc = dSc2;
			break;
		  case 8:case 4:
		    dSc = dSc1;
			break;
		}

	   ScaleFrom( iX, iY, m_rFixRect, position, dSc );
	 }
	MoveTo( position, pView );
 }

void TDrawShape::Invalidate()
 {
   ASSERT_VALID(this);
   if( m_pDocument )
     m_pDocument->UpdateMainView( NULL, HINT_UPDATE_DRAWOBJ, this );
 }

TDrawShape* TDrawShape::Clone( TDiplomDoc* pDoc )
 {
   ASSERT_VALID(this);

   TDrawShape* pClone = new TDrawShape( *this );   
   ASSERT_VALID(pClone);

   if( pDoc != NULL )
	  pDoc->Add( pClone );

   return pClone;
 }

void TDrawShape::OnEditProperties()
 {
   ASSERT_VALID(this);
   
   Invalidate();
   m_pDocument->SetModifiedFlag();
 }

CArchive& operator<<( CArchive& rA, TAuto_Glue& rCh )
 {
   rA << int(rCh->m_tgTyp) << int(rCh->m_lstGlu.size());

   list<TGlueDescr>::iterator itSta( rCh->m_lstGlu.begin() );
   list<TGlueDescr>::iterator itEnd( rCh->m_lstGlu.end() );
   for( ; itSta != itEnd; ++itSta )
	 rA.Write( &*itSta, sizeof(TGlueDescr) );

   return rA;
 }
CArchive& operator>>( CArchive& rA, TAuto_Glue& rCh )
 {
   int iT; rA >> iT; rCh->m_tgTyp = (TGTypes)iT;
   rA >> iT;

   if( iT )	
     for( ; iT > 0; --iT )
	  {
        TGlueDescr dscr;
		rA.Read( &dscr, sizeof(TGlueDescr) );
		dscr.m_pShp = NULL;
		rCh->m_lstGlu.push_back( dscr );
	  }

   return rA;
 }

CArchive& operator<<( CArchive& rA, TDrawShape& rCh )
 {
   operator<<( rA, (TIndexedUnic&)rCh );

   rA << int(rCh.m_bSelected) << rCh.m_iSelCount;
   rA.Write( &rCh.m_position, sizeof(rCh.m_position) ); 

   if( rCh.IsActiveGlue() )
	{
	  rA << int(rCh.m_vecGlu.size());
	  TVec_Glue::iterator itSta( rCh.m_vecGlu.begin() );
	  TVec_Glue::iterator itEnd( rCh.m_vecGlu.end() );

	  int iCnt = 0;
	  for( ; itSta != itEnd; ++itSta )
		rA << *itSta;
	}   

   return rA;
 }

CArchive& operator>>( CArchive& rA, TDrawShape& rCh )
 {
   operator>>( rA, (TIndexedUnic&)rCh );

   int iRd;
   rA >> iRd >> rCh.m_iSelCount; rCh.m_bSelected = iRd;
   rA.Read( &rCh.m_position, sizeof(rCh.m_position) ); 

   if( rCh.IsActiveGlue() )
	{
	  rA >> iRd; 
	  if( iRd )
	   {
		 rCh.m_vecGlu.assign( iRd );
		 for( int i = 0; iRd > 0; --iRd, ++i )
		  {
			TAuto_Glue ap( new TGlue() );
			rA >> ap;
			rCh.m_vecGlu[ i ] = ap;
		  }
	   }
	}   

   return rA;
 }


#ifdef _DEBUG
void TDrawShape::AssertValid()
{
	ASSERT(m_position.left <= m_position.right);
	ASSERT(m_position.bottom <= m_position.top);
}
#endif

CArchive& operator<<( CArchive& rA, TDrawPC& rCh )
 {
   operator<<( rA, (TDrawShape&)rCh );
   rA << rCh.m_szKeep.cx << rCh.m_szKeep.cy << int(rCh.m_bShortView);

   rA << rCh.m_sRang << rCh.m_sNum << rCh.m_dLambda << rCh.m_dM << rCh.m_dD << rCh.m_csName;
   
   bool bKeyH = rCh.IsHighLighted();
   if( bKeyH ) rCh.UnHighLight( false );
	
   rCh.m_tpPen.AR_Store( rA );
   rCh.m_brBrush.AR_Store( rA );
   rCh.m_fnFont.AR_Store( rA );

   if( bKeyH ) rCh.HighLight( false );

   return rA;
 }

CArchive& operator>>( CArchive& rA, TDrawPC& rCh )
 {
   operator>>( rA, (TDrawShape&)rCh );
   int iT;
   rA >> rCh.m_szKeep.cx >> rCh.m_szKeep.cy >> iT;
   rCh.m_bShortView = bool( iT );

   rA >> rCh.m_sRang >> rCh.m_sNum >> rCh.m_dLambda >> rCh.m_dM >> rCh.m_dD >> rCh.m_csName;
   rCh.CalcExtraParams();
   rCh.GenerateTTY();

   rCh.m_tpPen.AR_Load( rA );
   rCh.m_brBrush.AR_Load( rA );
   rCh.m_fnFont.AR_Load( rA );

   return rA;
 }


TDrToolItem* TDrawPC::GetDrTool( TTypesDrawTool typ )
 {
   switch( typ )
	{
	  case TDR_Pen:
	    return &m_tpPen;
	  case TDR_Brush:
	    return &m_brBrush;
	  case TDR_Font:
	    return &m_fnFont;
	  default:
	    return NULL;
	};
 }

void TDrawPC::InitGDI()
 {    
   (*m_tpPen).m_iStyle = PS_SOLID;
   (*m_tpPen).m_iWidth = 1;
   (*m_tpPen).m_clr = RGB( 0, 0, 0 );
   m_tpPen.Create();
   
   (*m_brBrush).m_clr = RGB(255,255,255);
   m_brBrush.Create();
   
   m_fnFont.Create();   

   (*m_tPenLbl).m_iStyle = PS_SOLID;
   (*m_tPenLbl).m_iWidth = 3;
   (*m_tPenLbl).m_clr = RGB( 0, 255, 0 );
   m_tPenLbl.Create();

   (*m_tPenLblBk).m_iStyle = PS_SOLID;
   (*m_tPenLblBk).m_iWidth = 3;
   (*m_tPenLblBk).m_clr = RGB( 0, 0, 0 );
   m_tPenLblBk.Create();   
 }

void TDrawPC::SetDefaultParam()
 {
   m_sNum = m_sRang = 1;   
   m_dLambda = 0.5;
   m_dM = 0.2;
   m_dD = 0.02;
   m_csName = "Устройство";

   CalcExtraParams();

   GenerateTTY();
 }

void TDrawPC::CalcExtraParams()
 {
   m_dKReady = 1.0 / (1.0 + m_dLambda*m_dM );
   m_dKC = m_dLambda/2.0 * (m_dD + m_dM*m_dM) * m_dKReady*m_dKReady;
 }

TDrawPC::TDrawPC():
  TDrawShape(),
  m_roundness( 16, 16 ),
  m_tpPen( *GDIMgr() ),
  m_brBrush( *GDIMgr() ),
  m_fnFont( *GDIMgr() ),
  m_tPenLbl( *GDIMgr() ),
  m_tPenLblBk( *GDIMgr() )
 {
   m_bShortView = false;
   m_vecGlu.assign( 2 );
   m_vecGlu[ 0 ] = TAuto_Glue( new TGlue(TG_In) );
   m_vecGlu[ 1 ] = TAuto_Glue( new TGlue(TG_Out) );

   m_sServis_Brig = 0;

   m_bLbl = false;

   SetDefaultParam();

   InitGDI();

   
   m_bHighLight = false;

   m_pTPC = NULL;
   m_bCTypeValid = false;
 }

TDrawPC::TDrawPC( const CRect& position ): 
  TDrawShape( position ),
  m_roundness( 16, 16 ),
  m_tpPen( *GDIMgr() ),
  m_brBrush( *GDIMgr() ),
  m_fnFont( *GDIMgr() ),
  m_tPenLbl( *GDIMgr() ),
  m_tPenLblBk( *GDIMgr() )
 {   
   m_bShortView = false;
   m_vecGlu.assign( 2 );
   m_vecGlu[ 0 ] = TAuto_Glue( new TGlue(TG_In) );
   m_vecGlu[ 1 ] = TAuto_Glue( new TGlue(TG_Out) );

   m_sServis_Brig = 0;

   m_bLbl = false;

   SetDefaultParam();

   InitGDI();

   
   m_bHighLight = false;

   m_pTPC = NULL;
   m_bCTypeValid = false;

   ASSERT_VALID( this );	
 }

TDrawPC::TDrawPC( const TDrawPC& rPC ):
  TDrawShape( rPC ),
  m_tpPen( *GDIMgr() ),
  m_brBrush( *GDIMgr() ),
  m_fnFont( *GDIMgr() ),
  m_tPenLbl( *GDIMgr() ),
  m_tPenLblBk( *GDIMgr() )
 {   
   m_roundness = rPC.m_roundness;
   m_bShortView = rPC.m_bShortView;
   m_szKeep = rPC.m_szKeep;   

   m_vecBiPolus = rPC.m_vecBiPolus;

   m_vecGlu.assign( 2 );
   m_vecGlu[ 0 ] = TAuto_Glue( new TGlue(TG_In) );
   m_vecGlu[ 1 ] = TAuto_Glue( new TGlue(TG_Out) );

   m_sServis_Brig = rPC.m_sServis_Brig;

   m_sRang = rPC.m_sRang;
   m_sNum = rPC.m_sNum;
   m_dLambda = rPC.m_dLambda;
   m_dM = rPC.m_dM;
   m_dD = rPC.m_dD;
   m_csName = rPC.m_csName;

   m_dKReady = rPC.m_dKReady;
   m_dKC = rPC.m_dKC;

   m_csTTYFull  = rPC.m_csTTYFull;
   m_csTTYShort = rPC.m_csTTYShort;
   m_strInfo = rPC.m_strInfo;

   m_tpPen = rPC.m_tpPen;
   m_brBrush = rPC.m_brBrush;
   m_fnFont = rPC.m_fnFont;

   m_tPenLbl = rPC.m_tPenLbl;
   m_tPenLblBk = rPC.m_tPenLblBk;

   m_bLbl = false;

   
   m_bHighLight = rPC.m_bHighLight;
   m_clrKey = rPC.m_clrKey;

   m_pTPC = NULL;
   m_bCTypeValid = rPC.m_bCTypeValid;
 }

TDrawPC::~TDrawPC()
 {
 }

void TDrawPC::HighLight( bool bRedraw )
 {
   if( m_bHighLight ) return;

   m_bHighLight = true;
   m_tpPen.Recreate();
   m_clrKey = (*m_tpPen).m_clr;
   (*m_tpPen).m_clr = RGB( 255, 0, 0 );
   m_tpPen.Create();
   if( bRedraw ) Invalidate();
 }
void TDrawPC::UnHighLight( bool bRedraw )
 {
   if( !m_bHighLight ) return;

   m_bHighLight = false;
   m_tpPen.Recreate();
   (*m_tpPen).m_clr = m_clrKey;   
   m_tpPen.Create();
   if( bRedraw ) Invalidate();
 }

int TDrawPC::GetDefaultRadius()
 {
   CWindowDC dc( NULL );
   CFont *pFOld = (CFont*)dc.SelectStockObject( SYSTEM_FONT );
   CString str( 99.5 );

   CSize sz = dc.GetTextExtent( str );
   sz.cx = MulDiv( sz.cx, GetDeviceCaps(dc, LOGPIXELSX), 72 ) + 1;
   sz.cy = MulDiv( sz.cy, GetDeviceCaps(dc, LOGPIXELSY), 72 ) + 1;

   dc.SelectObject( pFOld );

   return max( sz.cx + PenX(), sz.cy + PenY() ) + 2;
 }

void TDrawPC::SwitchModes( bool bShort )
 {
   if( bShort )
	{
      m_bShortView = true;
	  int iSQ = GetDefaultRadius();
	  m_szKeep.cx = abs(m_position.Width()) / 2;
	  m_szKeep.cy = abs(m_position.Height()) / 2;
      CPoint ptC( m_position.CenterPoint() );
      m_position = CRect( ptC.x - iSQ, ptC.y + iSQ, ptC.x + iSQ, ptC.y - iSQ );
	}
   else
	{
      m_bShortView = false;
	  CPoint ptC( m_position.CenterPoint() );
	  if( !m_szKeep.cx && !m_szKeep.cy )
	    m_szKeep = CSize( abs(m_position.Width()), abs(m_position.Width()) );
	  m_position = CRect( ptC.x - m_szKeep.cx, ptC.y + m_szKeep.cy, 
	    ptC.x + m_szKeep.cx, ptC.y - m_szKeep.cy );
	}
 }

static void DrawLbl( CDC* pDC, CRect& r, CPen* pPen )
 {
   CPen* pOldPen = pDC->SelectObject( pPen );  

   pDC->MoveTo( r.left, r.bottom - r.Height()/3 );
   pDC->LineTo( r.left + r.Width()/3, r.bottom );   
   pDC->LineTo( r.right, r.top );

   pDC->SelectObject( pOldPen );
 }

void TDrawPC::Draw( CDC* pDC, double dSc )
 {
   ASSERT_VALID(this);

   //CBrush brush( RGB(255,0,0) );   
   //CPen pen( PS_SOLID, 0, RGB(0,0,0) );	   

   CBrush* pOldBrush = pDC->SelectObject( m_brBrush() );
   CPen* pOldPen = pDC->SelectObject( m_tpPen() );  

   //m_brBrush()->UnrealizeObject();
   
   if( m_brBrush.GetData().m_clrBk != -1 )
     pDC->SetBkColor( m_brBrush.GetData().m_clrBk ),
     pDC->SetBkMode( OPAQUE );
   else pDC->SetBkMode( TRANSPARENT );
      
      
   /*CRect rOb = m_position;

   rOb.left = double(rOb.left) * dSc, rOb.right = double(rOb.right) * dSc,
   rOb.top = double(rOb.top) * dSc, rOb.bottom = double(rOb.bottom) * dSc;
*/ 
   CRect rp; MulRect( rp, m_position, dSc );
   CRect rpKey( rp );

   if( !m_bShortView )
	{
	  CPoint pt; MulPt( pt, m_roundness, dSc );
	  pDC->RoundRect( rp, pt );
	}
   else pDC->Ellipse( rp );

   pDC->SetBkMode( TRANSPARENT );
   CFont *pFOld = pDC->SelectObject( m_fnFont() );
   pDC->SetTextColor( m_fnFont.GetData().m_clr );

   if( !m_bShortView )	
	{		  
	  CRect rr( rp ); 
	  int iHT = pDC->DrawText( m_csTTYFull, rr, DT_CENTER|DT_VCENTER|DT_NOPREFIX|DT_WORDBREAK|DT_EDITCONTROL|DT_CALCRECT );
	  int iShft = (abs(rp.Height()) - abs(iHT)) / 2;
	  if( iShft > 0 ) rp.OffsetRect( 0, -iShft );	  
	  //rp.NormalizeRect();
      pDC->DrawText( m_csTTYFull, rp, DT_CENTER|DT_VCENTER|DT_NOPREFIX|DT_WORDBREAK|DT_EDITCONTROL );	
	}
   else	  
	{
	 //rp.NormalizeRect();
	 pDC->DrawText( m_csTTYShort, rp, DT_CENTER|DT_VCENTER|DT_NOPREFIX|DT_SINGLELINE );	
	}

   
   if( m_sServis_Brig )
	{	  
	  rpKey.NormalizeRect();
	  int iHW = min( rpKey.Height(), rpKey.Width() ) / 3;
	  CRect rLbl( 0, iHW, iHW, 0 );
	  rLbl.OffsetRect( rpKey.CenterPoint() - rLbl.CenterPoint() );

	  if( m_sServis_Brig == 1 )
	   {
	     DrawLbl( pDC, rLbl, m_tPenLblBk() );
		 rLbl.OffsetRect( -2, 2 );
	     DrawLbl( pDC, rLbl, m_tPenLbl() );
	   }
	  else
	   {
	     int iW = rLbl.Width() / 2;
         rLbl.OffsetRect( -iW, 0 );
		 DrawLbl( pDC, rLbl, m_tPenLblBk() );
		 rLbl.OffsetRect( -2, 2 );
		 DrawLbl( pDC, rLbl, m_tPenLbl() );
		 rLbl.OffsetRect( 2*iW+2, -2 );
		 DrawLbl( pDC, rLbl, m_tPenLblBk() );
		 rLbl.OffsetRect( -2, 2 );
		 DrawLbl( pDC, rLbl, m_tPenLbl() );
	   }
	}
   
   pDC->SelectObject( pFOld );
   pDC->SelectObject( pOldBrush );
   pDC->SelectObject( pOldPen );
 }

   
void TDrawPC::GenerateTTY()
 {
   //CString str;
   unsigned int uiSize = m_csName.GetLength() + 1024;
   LPSTR lp = m_csTTYFull.GetBuffer( uiSize );

   ostrstream strm( lp, uiSize - 1 );
   //long lFl = strm.flags();

   //lFl |= ios::fixed|ios::scientific;
   //lFl |= ios::scientific;
   //lFl &= ~ios::scientific;

   strm << setprecision(2) <<
	  m_sRang << '.' << m_sNum << endl << (LPCSTR)m_csName << endl <<
	  '(' << m_dLambda << ';' << m_dM << ';' << m_dD << ')';

   m_csTTYFull.ReleaseBuffer( strm.pcount() );

   lp = m_csTTYShort.GetBuffer( 1024 );
   ostrstream strm2( lp, 1023 );   

   strm2 <<  
	 m_sRang << '.' << m_sNum;

   m_csTTYShort.ReleaseBuffer( strm2.pcount() );

   /*m_csTTYFull.Format( "%d.%d\n%s\n(%1.1f; %1.1f; %1.1f)", 
	   int(m_sRang), int(m_sNum), 
	   (LPCSTR)m_csName, m_dLambda, m_dM, m_dD );	   

   m_csTTYShort.Format( "%d.%d", int(m_sRang), int(m_sNum) );*/

   lp = m_strInfo.GetBuffer( uiSize );
   ostrstream strm3( lp, uiSize - 1 );   

   strm3 << /*setprecision(2) <<*/ "Устройство " << m_sRang << '.' <<
	 m_sNum << "  \"" << (LPCSTR)m_csName << "\":  (L=" <<
	 m_dLambda << ", M=" << m_dM << ", D=" << m_dD << ") - Кг=" <<
	 m_dKReady << ", Кс=" << m_dKC;

   m_strInfo.ReleaseBuffer( strm3.pcount() );

   /*rS.Format( "Устройство %u.%u \"%s\": (L=%1.3f, M=%1.3f, D=%1.3f) - Кг=%1.3f, Кс=%1.3f",
	 int(m_sRang), int(m_sNum), 
	 (LPCSTR)m_csName, m_dLambda, m_dM, m_dD, m_dKReady, m_dKC );*/
 }

int TDrawPC::GetGlueCount()
 {
   ASSERT_VALID(this);
   return TDrawShape::GetGlueCount();
 }
CPoint TDrawPC::GetGlue( int nHandle )
 {
   ASSERT_VALID(this);
   return TDrawShape::GetGlue(nHandle);
 }

CPoint TDrawPC::GetGlue_Muled( int nHandle )
 {
   ASSERT_VALID(this);
   return TDrawShape::GetGlue_Muled( nHandle );
 }

int TDrawPC::GetHandleCount()
 {
   ASSERT_VALID(this);
   return TDrawShape::GetHandleCount();
 }

// returns center of handle in logical coordinates
CPoint TDrawPC::GetHandle( int nHandle )
 {
   ASSERT_VALID(this);
   return TDrawShape::GetHandle(nHandle);
 }

HCURSOR TDrawPC::GetHandleCursor( int nHandle, bool bFlDrag )
 {
   ASSERT_VALID(this);
   
   return TDrawShape::GetHandleCursor( nHandle, bFlDrag );
 }

// point is in logical coordinates
void TDrawPC::MoveHandleTo( int nHandle, CPoint point, TDiplomView* pView, bool bFlEQ )
 {
   ASSERT_VALID(this);

   CRect r = m_position;
   TDrawShape::MoveHandleTo( nHandle, point, pView, bFlEQ );

   int iW = m_position.Width(),
	   iH = m_position.Height();
   if( iW < 25 )
	 m_position.left = r.left, m_position.right = r.right;
   if( -iH < 25 )
	 m_position.top = r.top, m_position.bottom = r.bottom;
   
   iH = abs( iH ), iW = abs( iW );
   if( m_bShortView && iW != iH )
	{
	  int iSQ = max( iW, iH ) / 2;
	  CPoint ptC( m_rFixRect.CenterPoint() );
	  m_position = CRect( ptC.x - iSQ, ptC.y + iSQ, ptC.x + iSQ, ptC.y - iSQ );
	  //CPoint ptC( m_position.CenterPoint() );
      m_position.OffsetRect( m_position.CenterPoint() - ptC );
	  Invalidate();
	}
 }

void TDrawPC::MoveTo( const CRect& positon, TDiplomView* pView )
 {
   TDrawShape::MoveTo( positon, pView );
 }

// rect must be in logical coordinates
BOOL TDrawPC::Intersects( const CRect& rect )
{
	ASSERT_VALID(this);

	CRect rectT = rect;
	rectT.NormalizeRect();

	//CRect fixed = m_position;
	//fixed.NormalizeRect();
	CRect fixed( GetPosition() );
	//fixed.left -= 10, fixed.right += 10, fixed.top -= 10,
	 //fixed.bottom += 10;//kkkkkkkk
	if( (rectT & fixed).IsRectEmpty() )
	  return FALSE;
	
	if( !m_bShortView )  return TRUE;
	  //rgn.CreateRoundRectRgn( fixed.left, fixed.top, fixed.right, fixed.bottom,
		 //m_roundness.x, m_roundness.y );
	CRgn rgn;	
	fixed.NormalizeRect(); fixed.InflateRect( 2, 2 );
	rgn.CreateEllipticRgn( fixed.left, fixed.top, fixed.right, fixed.bottom );

	return rgn.RectInRegion( rectT );	
}

TDrawShape* TDrawPC::Clone( TDiplomDoc* pDoc )
 {
	ASSERT_VALID(this);

	TDrawPC* pClone = new TDrawPC( *this );	
	ASSERT_VALID(pClone);
    
	Invalidate();
	int iSelKey = m_iSelCount;
	Select( m_pDocument->RefLstDraw(), false );

    pClone->m_position.OffsetRect( 10, -10 );
	pClone->SetScaled( pClone->m_dSc );

	if( pDoc != NULL )
	 {
	   pClone->Select( m_pDocument->RefLstDraw(), true );
	   pDoc->Add( pClone );
	   pClone->m_iSelCount = iSelKey;
	   pClone->Invalidate();
	 }

    pDoc->SetModifiedFlag();

	ASSERT_VALID(pClone);
	return pClone;
}


void TDrawShape::AR_Store( CArchive& rA )
 { 
   TIndexedUnic::AR_Store( rA );
   rA << *this;
 }
void TDrawShape::AR_Load( CArchive& rA )
 {
   TIndexedUnic::AR_Load( rA );
   rA >> *this;
 }
void TDrawPC::AR_Store( CArchive& rA )
 { 
   TDrawShape::AR_Store( rA );
   rA << *this;
 }
void TDrawPC::AR_Load( CArchive& rA )
 {
   TDrawShape::AR_Load( rA );
   rA >> *this;
 }

void TDrawShape::SetScaled( double dSc )
 {
   m_dSc = dSc;   
   TTItem_Font *p = (TTItem_Font*)GetDrTool( TDR_Font );
   if( p )
	{
      p->Recreate();	  
	  (**p).m_dSc = dSc;
	  p->Create();
	}
 }
void TDrawPC::SetScaled( double dSc )
 {
   TDrawShape::SetScaled( dSc );   
 }


//-----------------------------------------------------

TDrToolItem* TCnct::GetDrTool( TTypesDrawTool typ )
 {
   switch( typ )
	{
	  case TDR_Pen:	    
	    return &m_tpPen;			  
	  default:
	    return NULL;
	};
 }

void TCnct::InitGDI()
 {         
   (*m_tpPen).m_iStyle = PS_SOLID;
   (*m_tpPen).m_iWidth = 1;
   (*m_tpPen).m_clr = RGB(0,0,0);
   m_tpPen.Create();      
 }

TCnct::TCnct():
  m_tpPen( *GDIMgr() )
 {
   m_typ = TO_Cnct_Simple;
   m_sNPoints = GetPointsForTyp( m_typ );
   m_arcPoints = new CPoint[ m_sNPoints ];
   m_dKSq = 0.5;
   m_iKln = 50;
   m_vecGlu.assign( 2 );
   m_vecGlu[ 0 ] = TAuto_Glue( new TGlue(TG_In) );
   m_vecGlu[ 1 ] = TAuto_Glue( new TGlue(TG_Out) );

   InitGDI();
   
   m_bHighLight = false;
   m_bFlMark = false;
 }
TCnct::TCnct( TObjTypes typ ):
  m_typ( typ ),
  m_tpPen( *GDIMgr() )
 {
   m_sNPoints = GetPointsForTyp( m_typ );
   m_arcPoints = new CPoint[ m_sNPoints ];
   m_dKSq = 0.5;
   m_iKln = 50;
   m_vecGlu.assign( 2 );
   m_vecGlu[ 0 ] = TAuto_Glue( new TGlue(TG_In) );
   m_vecGlu[ 1 ] = TAuto_Glue( new TGlue(TG_Out) );

   InitGDI();

   
   m_bHighLight = false;
   m_bFlMark = false;
 }

TCnct::TCnct( TObjTypes typ, const CRect& position ): 
  TDrawShape( position ),  
  m_typ( typ ),
  m_tpPen( *GDIMgr() )
 {   
   m_sNPoints = GetPointsForTyp( m_typ );
   m_arcPoints = new CPoint[ m_sNPoints ];
   m_dKSq = 0.5;
   m_iKln = 50;
   m_vecGlu.assign( 2 );
   m_vecGlu[ 0 ] = TAuto_Glue( new TGlue(TG_In) );
   m_vecGlu[ 1 ] = TAuto_Glue( new TGlue(TG_Out) );

   InitGDI();

   
   m_bHighLight = false;
   m_bFlMark = false;

   ASSERT_VALID( this );	
 }

TCnct::TCnct( const TCnct& rPC ):
  TDrawShape( rPC ),
   m_tpPen( *GDIMgr() )
 {
   m_typ = rPC.m_typ;
   m_sNPoints = GetPointsForTyp( m_typ );
   m_arcPoints = new CPoint[ m_sNPoints ];
   m_dKSq = rPC.m_dKSq;
   m_iKln = rPC.m_iKln;  

   m_vecGlu.assign( 2 );
   m_vecGlu[ 0 ] = TAuto_Glue( new TGlue(TG_In) );
   m_vecGlu[ 1 ] = TAuto_Glue( new TGlue(TG_Out) );

   m_tpPen = rPC.m_tpPen;

      
   m_bHighLight = rPC.m_bHighLight;
   m_clrKey = rPC.m_clrKey;
   m_bFlMark = false;
 }

TCnct::~TCnct()
 {
 }

void TCnct::HighLight( bool bRedraw )
 {
   if( m_bHighLight ) return;

   m_bHighLight = true;
   m_tpPen.Recreate();
   m_clrKey = (*m_tpPen).m_clr;
   (*m_tpPen).m_clr = RGB( 255, 0, 0 );
   m_tpPen.Create();
   if( bRedraw ) Invalidate();
 }
void TCnct::UnHighLight( bool bRedraw )
 {
   if( !m_bHighLight ) return;

   m_bHighLight = false;
   m_tpPen.Recreate();
   (*m_tpPen).m_clr = m_clrKey;   
   m_tpPen.Create();
   if( bRedraw ) Invalidate();
 }

int TCnct::HToGlu( int iH )
 {
   switch( m_typ )
	{
	  case TO_Cnct_Corn:			  
	    if( iH == 1 ) return 1;
		if( iH == 2 ) return 2;
		return -1;		
	  case TO_Cnct_Simple:
		return iH;		
	  case TO_Cnct_Sq:
		if( iH == 1 ) return 1;
		if( iH == 2 ) return 2;
		return -1;		
	  case TO_Cnct_Cln:
		if( iH == 1 ) return 1;
		if( iH == 2 ) return 2;
		return -1;		
	  default:
	    return -1;
	};
 }

#define GRAD_TO_RAD(grad) (double(grad) / 180.0 * 3.14)
#define RAD_TO_GRAD(rad) (double(rad) * 180.0 / 3.14)

#define  MDI_d_DrawArrowMinDistance  0.001
#define  MDI_i_DrawArrowLenght 15

static void DrawArrow( CDC* pDC, int iX1, int iY1, int iX2, int iY2, double dScale )
 {
   const double dMinDistance = MDI_d_DrawArrowMinDistance;
   const double dCorner = GRAD_TO_RAD(180 - 20);
   //const int iLength = 15.0 / ((GetScale() < 1 && GetScale()) ? GetScale():1);
   const int iLength = MDI_i_DrawArrowLenght * dScale;

   double dXs = iLength * cos( dCorner ),
	     dYs = iLength * sin( dCorner );
   int iDx = iX2 - iX1, iDy = iY2 - iY1;
   double dD = sqrt( double(iDx) * double(iDx) + double(iDy) * double(iDy) );
   if( dD < dMinDistance ) return;
   double dCosM = double(iDx) / dD,
          dSinM = double(iDy) / dD;

   double dX = dXs * dCosM - dYs * dSinM,
	  dY = dXs * dSinM + dYs * dCosM;

   int x1 = iX2 + dX, y1 = iY2 + dY;

   dXs = iLength * cos( -dCorner );
   dYs = iLength * sin( -dCorner );
   dX = dXs * dCosM - dYs * dSinM;
   dY = dXs * dSinM + dYs * dCosM;

   int x2 = iX2 + dX, y2 = iY2 + dY;

   pDC->MoveTo( iX2, iY2 );
   pDC->LineTo( x1, y1 );
   pDC->MoveTo( iX2, iY2 );
   pDC->LineTo( x2, y2 );
 }

void TCnct::Draw( CDC* pDC, double dSc )
 {
   ASSERT_VALID(this);
      
   //CPen pen( PS_SOLID, 0, RGB(0,0,0) );	

   CPen* pOldPen = pDC->SelectObject( m_tpPen() );           
   pDC->SetBkMode( TRANSPARENT );
      
   CRect rp; MulRect( rp, m_position, dSc );   

   InitPointsArray( rp, dSc );
   pDC->Polyline( m_arcPoints(), m_sNPoints );
   if( m_pDocument->m_bFlArrow )
	{      	 
      if( GetGluTyp(1) == TG_In )
       {
		 CRect rp; LastSeg( rp );
		 if( m_sNPoints > 2 && rp.left == rp.right && rp.top == rp.bottom ) 
			LastSeg2( rp );
		 pDC->SetBkColor( m_tpPen.GetData().m_clr );
		 DrawArrow( pDC, rp.left, rp.top, rp.right, rp.bottom, m_dSc );
	   }
	  else
	  if( GetGluTyp(0) == TG_In )
	   {
         CRect rp; FirstSeg( rp );
		 if( m_sNPoints > 2 && rp.left == rp.right && rp.top == rp.bottom ) 
			FirstSeg2( rp );
		 pDC->SetBkColor( m_tpPen.GetData().m_clr );
		 DrawArrow( pDC, rp.right, rp.bottom, rp.left, rp.top, m_dSc );
	   }
	}   
      
   pDC->SelectObject( pOldPen );
 }

void TCnct::InitPointsArray( CRect& rp, double dSc )
 {
   if( m_typ == TO_Cnct_Simple )
	{
	  m_arcPoints()[0] = CPoint( rp.left, rp.top );
	  m_arcPoints()[1] = CPoint( rp.right, rp.bottom );
	}
   else if( m_typ == TO_Cnct_Corn ) 
	{
	  m_arcPoints()[0] = CPoint( rp.left, rp.top );
	  m_arcPoints()[1] = CPoint( rp.left, rp.bottom );
	  m_arcPoints()[2] = CPoint( rp.right, rp.bottom );
	}
   else if( m_typ == TO_Cnct_Sq )
	{
	  int iY = double(rp.Height()) * m_dKSq;
      m_arcPoints()[0] = CPoint( rp.left, rp.top );
	  m_arcPoints()[1] = CPoint( rp.left, rp.top + iY );
	  m_arcPoints()[2] = CPoint( rp.right, rp.top + iY );
	  m_arcPoints()[3] = CPoint( rp.right, rp.bottom );
	}
   else
	{
      int iY = double(rp.Height()) * m_dKSq;
	  int k = double(m_iKln) * dSc;
      m_arcPoints()[0] = CPoint( rp.left, rp.top );
	  m_arcPoints()[1] = CPoint( rp.left, rp.top + iY );
	  m_arcPoints()[2] = CPoint( rp.right - k, rp.top + iY );
	  m_arcPoints()[3] = CPoint( rp.right - k, rp.bottom );
	  m_arcPoints()[4] = CPoint( rp.right, rp.bottom );
	}
 }


short TCnct::GetPointsForTyp( TObjTypes typ )
 {
   switch( typ )
	{
	  case TO_Cnct_Corn:			  
		return 3;		
	  case TO_Cnct_Simple:
		return 2;		
	  case TO_Cnct_Sq:
		return 4;
	  case TO_Cnct_Cln:
		return 5;
	  default:
	    return 2;
	};
 }

int TCnct::GetGlueCount()
 {
   ASSERT_VALID(this);   
   return 2;
 }
CPoint TCnct::GetGlue( int nHandle )
 {
   ASSERT_VALID(this);
   ASSERT( nHandle == 1 || nHandle == 2 );

   if( nHandle == 2 ) nHandle = 5;
   return TDrawShape::GetHandle( nHandle );
 }

CPoint TCnct::GetGlue_Muled( int nHandle )
 {
   ASSERT_VALID(this);
   ASSERT( nHandle == 1 || nHandle == 2 );

   int x, y;//, xCenter, yCenter;   

   // this gets the center regardless of left/right and top/bottom ordering
   CRect rp; MulRect( rp, m_position, m_dSc );
   //xCenter = rp.left + rp.Width() / 2;
   //yCenter = rp.top + rp.Height() / 2;

   switch( nHandle )
    {
	  default:
		  ASSERT(FALSE);   

	  case 1:
		  x = rp.left;
		  y = rp.top;
		  break;      

	  case 2:
		  x = rp.right;
		  y = rp.bottom + 1;
		  break;   
    }

   return CPoint(x, y);
 }

int TCnct::GetHandleCount()
 {
   ASSERT_VALID(this);
   if( m_typ == TO_Cnct_Sq ) return 3;
   else if( m_typ == TO_Cnct_Cln ) return 4;
   else return 2;
 }

// returns center of handle in logical coordinates
CPoint TCnct::GetHandle( int nHandle )
 {
   ASSERT_VALID(this);
   //ASSERT( nHandle == 1 || nHandle == 2 );

   CPoint pt; 
   if( nHandle == 3 )
	{	  
	  InitPointsArray( m_position, 1 );	  
	  pt.x = (m_arcPoints()[1].x + m_arcPoints()[2].x) / 2;
	  pt.y = m_arcPoints()[1].y;      
	  
	  return pt;
	}
   if( nHandle == 4 )
	{	  
	  InitPointsArray( m_position, 1 );	  
	  pt.x = m_arcPoints()[3].x;
	  pt.y = (m_arcPoints()[2].y + m_arcPoints()[3].y) / 2;
	  
	  return pt;
	}

   if( nHandle == 2 ) nHandle = 5;   
   return TDrawShape::GetHandle(nHandle);
 }

HCURSOR TCnct::GetHandleCursor( int nHandle, bool bFlDrag )
 {
   ASSERT_VALID(this);

   LPCTSTR lpCursor;

   if( nHandle == 3 ) lpCursor = IDC_SIZENS;	
   else if( nHandle == 4 ) lpCursor = IDC_SIZEWE;	
   else if( bFlDrag ) return AfxGetApp()->LoadCursor( IDC_CURSOR_GLUMOV );
   else lpCursor = IDC_SIZEALL;
   
   //return TDrawShape::GetHandleCursor( nHandle );
   return AfxGetApp()->LoadStandardCursor( lpCursor );
 }

// point is in logical coordinates
void TCnct::MoveHandleTo( int nHandle, CPoint point, TDiplomView* pView, bool bFlEQ )
 {
   ASSERT_VALID(this);
   //ASSERT( nHandle == 1 || nHandle == 2 );

   if( nHandle == 3 )
	{	  
	  int iDP = abs( m_position.Height() );
	  if( iDP < 5 ) return;
	  bool bFl = m_position.top < m_position.bottom;
	  int iMY = bFl ? m_position.bottom:m_position.top;
	  int iMiY = !bFl ? m_position.bottom:m_position.top;
	  if( point.y >= iMY ) m_dKSq = bFl ? 1:0;
	  else if( point.y <= iMiY ) m_dKSq = !bFl ? 1:0;
	  else m_dKSq = fabs( double(point.y-m_position.top)/double(iDP) );	     
	  
	  Invalidate();
	  return;
	}
   else if( nHandle == 4 )
	{	  
      Invalidate();
	  m_iKln = -point.x + m_position.right;
	  return;
	}

   if( nHandle == 2 ) nHandle = 5;   
   
   TDrawShape::MoveHandleTo( nHandle, point, pView, bFlEQ );   
 }

void TCnct::MoveTo( const CRect& positon, TDiplomView* pView )
 {
   TDrawShape::MoveTo( positon, pView );
 }


void TCnct::CreLineRgn( CRgn& rRgn, const CRect& fixed )
 {
   int x = (PenX() + 5) / 2;
   int y = (PenY() + 5) / 2;
   POINT points[4];
   points[0].x = fixed.left;
   points[0].y = fixed.top;
   points[1].x = fixed.left;
   points[1].y = fixed.top;
   points[2].x = fixed.right;
   points[2].y = fixed.bottom;
   points[3].x = fixed.right;
   points[3].y = fixed.bottom;

   if (fixed.left < fixed.right)
   {
	   points[0].x -= x;
	   points[1].x += x;
	   points[2].x += x;
	   points[3].x -= x;
   }
   else
   {
	   points[0].x += x;
	   points[1].x -= x;
	   points[2].x -= x;
	   points[3].x += x;
   }

   if (fixed.top < fixed.bottom)
   {
	   points[0].y -= y;
	   points[1].y += y;
	   points[2].y += y;
	   points[3].y -= y;
   }
   else
   {
	   points[0].y += y;
	   points[1].y -= y;
	   points[2].y -= y;
	   points[3].y += y;
   }
   //rRgn.CreatePolygonRgn( points, 4, ALTERNATE );
   rRgn.CreatePolygonRgn( points, 4, WINDING );     
 }

CRect TCnct::GetPosition( bool bFlWithTracker ) const
 {
   CRect r( m_position ); 
   ExtendRect( r, 1 );
   r.NormalizeRect();
   //r.InflateRect( 2*DeltaTracker() + PenX(), 2*DeltaTracker() + PenY() );
   r.InflateRect( PenX(), PenY() );

   //if( bFlWithTracker )
	 //r.InflateRect( DeltaTracker(), DeltaTracker() );   

   return r;
 }

inline void Swap( int& x1, int& x2 )
 {
   int t = x1; x1 = x2; x2 = t;
 }


// rect must be in logical coordinates
BOOL TCnct::Intersects( const CRect& rect )
{
	ASSERT_VALID(this);

	CRect rectT = rect;
	rectT.NormalizeRect();

    //CRect fixed = m_position;
	//fixed.NormalizeRect();
    CRect fixed( m_position );
	ExtendRect( fixed, 1 );
	fixed.NormalizeRect(); fixed.InflateRect( 1, 1 );
	if( (rectT & fixed).IsRectEmpty() )
	  return FALSE;

	BarSpec bar( rectT );

	InitPointsArray( m_position, 1 );
	CPoint *p1 = m_arcPoints(), *p2 = m_arcPoints() + 1;
	for( int i = m_sNPoints; i; --i, ++p1, ++p2 )
	 {
       Line lin( *p1, *p2 );	
	   if( ClipSozKoe( lin, bar ) == 1 ) return TRUE;
	 }	
			
	/*CRgn rgn;	
	CreLineRgn( rgn, fixed );
	return rgn.RectInRegion( fixed );*/

	return FALSE;
}

void TCnct::ExtendRect( CRect& r, double dSc ) const
 {
   if( m_typ == TO_Cnct_Cln )
	{
	  //int k = fabs( double(m_iKln) * dSc );
	  int iBnd = r.right - double(m_iKln) * dSc;
	  if( r.left < r.right ) 
	   {
	     if( iBnd < r.left ) r.left = iBnd;
		 else if( iBnd > r.right ) r.right = iBnd;
	   }
	  else 
	   {
	     if( iBnd < r.right ) r.right = iBnd;
		 else if( iBnd > r.left ) r.left = iBnd;
	   }
	  /*if( r.left <= r.right )
	    r.left -= k,
	    r.right += k;
	  else
	    r.left += k,
	    r.right -= k;*/
	}
 }

TDrawShape* TCnct::Clone( TDiplomDoc* pDoc )
 {
	ASSERT_VALID(this);

	TCnct* pClone = new TCnct( *this );	
	ASSERT_VALID(pClone);
    
	Invalidate();
	int iSelKey = m_iSelCount;
	Select( m_pDocument->RefLstDraw(), false );

    pClone->m_position.OffsetRect( 10, -10 );
	pClone->SetScaled( pClone->m_dSc );

	if( pDoc != NULL )
	 {
	   pClone->Select( m_pDocument->RefLstDraw(), true );
	   pDoc->Add( pClone );
	   pClone->m_iSelCount = iSelKey;
	   pClone->Invalidate();
	 }

	pDoc->SetModifiedFlag();

	ASSERT_VALID(pClone);
	return pClone;
}

void TCnct::AR_Store( CArchive& rA )
 { 
   TDrawShape::AR_Store( rA );
   rA << *this;
 }
void TCnct::AR_Load( CArchive& rA )
 {
   TDrawShape::AR_Load( rA );
   rA >> *this;
 }

CArchive& operator<<( CArchive& rA, TCnct& rCh )
 { 
   operator<<( rA, (TDrawShape&)rCh );
   rA << int(rCh.m_typ) << rCh.m_dKSq << rCh.m_iKln;   

   bool bKeyH = rCh.IsHighLighted();
   if( bKeyH ) rCh.UnHighLight( false );	      

   rCh.m_tpPen.AR_Store( rA );   

   if( bKeyH ) rCh.HighLight( false );

   return rA;
 }

CArchive& operator>>( CArchive& rA, TCnct& rCh )
 {
   operator>>( rA, (TDrawShape&)rCh );
   int iTmp; rA >> iTmp; rCh.m_typ = (TObjTypes)iTmp;
   rA >> rCh.m_dKSq >> rCh.m_iKln;

   rCh.m_tpPen.AR_Load( rA );   

   rCh.m_sNPoints = rCh.GetPointsForTyp( rCh.m_typ );
   rCh.m_arcPoints = new CPoint[ rCh.m_sNPoints ];

   return rA;
 }


//-----------------------------------------------

TDrToolItem* TDrawPoint::GetDrTool( TTypesDrawTool typ )
 {
   switch( typ )
	{
	  case TDR_Pen:	    
	    return NULL;			  
	  case TDR_Brush:	    
	    return &m_brBrush;			  
	  default:
	    return NULL;
	};
 }

void TDrawPoint::InitGDI()
 {         
   (*m_tpPen).m_iStyle = PS_SOLID;
   (*m_tpPen).m_iWidth = 1;
   (*m_tpPen).m_clr = RGB(0,0,0);
   m_tpPen.Create();      

   (*m_brBrush).m_clr = RGB(0,0,0);
   m_brBrush.Create();
 }

TDrawPoint::TDrawPoint():
  m_tpPen( *GDIMgr() ),
  m_brBrush( *GDIMgr() )
 {
   m_position = CRect( 0, 10, 10, 0 );	
   m_vecGlu.assign( 1 );
   m_vecGlu[ 0 ] = TAuto_Glue( new TGlue(TG_InOut) );   

   InitGDI();

   m_bHighLight = false;
 }

TDrawPoint::TDrawPoint( const CRect& position ): 
  TDrawShape( position ),
  m_tpPen( *GDIMgr() ),
  m_brBrush( *GDIMgr() )
 {   
   m_vecGlu.assign( 1 );
   m_vecGlu[ 0 ] = TAuto_Glue( new TGlue(TG_InOut) );   

   InitGDI();

   m_bHighLight = false;

   ASSERT_VALID( this );	
 }

TDrawPoint::TDrawPoint( const TDrawPoint& rPC ):
  TDrawShape( rPC ),
  m_tpPen( *GDIMgr() ),
  m_brBrush( *GDIMgr() )
 {      
   m_vecGlu.assign( 1 );
   m_vecGlu[ 0 ] = TAuto_Glue( new TGlue(TG_InOut) );   

   m_tpPen = rPC.m_tpPen;
   m_brBrush = rPC.m_brBrush;

   m_vecBiPolus = rPC.m_vecBiPolus;

   m_bHighLight = rPC.m_bHighLight;
   m_clrKey = rPC.m_clrKey;
 }

TDrawPoint::~TDrawPoint()
 {
 }

void TDrawPoint::HighLight( bool bRedraw )
 {
   if( m_bHighLight ) return;

   m_bHighLight = true;
   m_brBrush.Recreate();
   m_clrKey = (*m_brBrush).m_clr;
   (*m_brBrush).m_clr = RGB( 255, 0, 0 );
   m_brBrush.Create();

   if( bRedraw ) Invalidate();
 }
void TDrawPoint::UnHighLight( bool bRedraw )
 {
   if( !m_bHighLight ) return;

   m_bHighLight = false;
   m_brBrush.Recreate();
   (*m_brBrush).m_clr = m_clrKey;   
   m_brBrush.Create();

   if( bRedraw ) Invalidate();
 }

void TDrawPoint::Draw( CDC* pDC, double dSc )
 {
   ASSERT_VALID(this);

   //CBrush brush( RGB(0,0,0) );   
   //CPen pen( PS_SOLID, 1, RGB(0,0,0) );	

   CBrush* pOldBrush = pDC->SelectObject( m_brBrush() );
   CPen* pOldPen = (CPen*)pDC->SelectStockObject( NULL_PEN );
   

   if( m_brBrush.GetData().m_clrBk != -1 )
     pDC->SetBkColor( m_brBrush.GetData().m_clrBk ),
     pDC->SetBkMode( OPAQUE );
   else pDC->SetBkMode( TRANSPARENT );
   
      
   CRect rp; MulRect( rp, m_position, dSc );   
   pDC->Ellipse( rp );
   
   pDC->SelectObject( pOldBrush );
   pDC->SelectObject( pOldPen );
 }

int TDrawPoint::GetGlueCount()
 {
   ASSERT_VALID(this);
   return 1;
 }
CPoint TDrawPoint::GetGlue( int nHandle )
 {
   ASSERT_VALID(this);
   return m_position.CenterPoint();
 }

CPoint TDrawPoint::GetGlue_Muled( int nHandle )
 {
   ASSERT_VALID(this);
   //return TDrawShape::GetGlue_Muled( nHandle );
   CPoint pt( m_position.CenterPoint() );
   pt.x = double(pt.x) * m_dSc; pt.y = double(pt.y) * m_dSc;
   return  pt;
 }

int TDrawPoint::GetHandleCount()
 {
   ASSERT_VALID(this);
   return 1;
 }

// returns center of handle in logical coordinates
CPoint TDrawPoint::GetHandle( int nHandle )
 {
   ASSERT_VALID(this);
   return m_position.CenterPoint();
 }

HCURSOR TDrawPoint::GetHandleCursor( int nHandle, bool bFlDrag )
 {
   ASSERT_VALID(this);
   
   return AfxGetApp()->LoadStandardCursor( IDC_SIZEALL );
 }

// point is in logical coordinates
void TDrawPoint::MoveHandleTo( int nHandle, CPoint point, TDiplomView* pView, bool bFlEQ )
 {
   ASSERT_VALID(this);

   CRect pos( m_position );
   pos.OffsetRect( point - pos.CenterPoint() );
   TDrawShape::MoveTo( pos, pView );
 }

void TDrawPoint::MoveTo( const CRect& positon, TDiplomView* pView )
 {
   TDrawShape::MoveTo( positon, pView );
 }

// rect must be in logical coordinates
BOOL TDrawPoint::Intersects( const CRect& rect )
 {
	ASSERT_VALID(this);

	CRect rectT = rect;
	rectT.NormalizeRect();

	//CRect fixed = m_position;
	//fixed.NormalizeRect();
	CRect fixed( GetPosition() );
	if( (rectT & fixed).IsRectEmpty() )
	  return FALSE;

	CRgn rgn;
	
	rgn.CreateEllipticRgn( fixed.left, fixed.top, fixed.right, fixed.bottom );
		
	return rgn.RectInRegion( rectT );
 }

TDrawShape* TDrawPoint::Clone( TDiplomDoc* pDoc )
 {
	ASSERT_VALID(this);

	TDrawPoint* pClone = new TDrawPoint( *this );	
	ASSERT_VALID(pClone);
    
	Invalidate();
	int iSelKey = m_iSelCount;
	Select( m_pDocument->RefLstDraw(), false );

    pClone->m_position.OffsetRect( 10, -10 );
	pClone->SetScaled( pClone->m_dSc );

	if( pDoc != NULL )
	 {
	   pClone->Select( m_pDocument->RefLstDraw(), true );
	   pDoc->Add( pClone );
	   pClone->m_iSelCount = iSelKey;
	   pClone->Invalidate();
	 }

    pDoc->SetModifiedFlag();

	ASSERT_VALID(pClone);
	return pClone;
}

void TDrawPoint::SetScaled( double dSc )
 {
   TDrawShape::SetScaled( dSc );   
 }

void TDrawPoint::AR_Store( CArchive& rA )
 { 
   TDrawShape::AR_Store( rA );
   rA << *this;
 }
void TDrawPoint::AR_Load( CArchive& rA )
 {
   TDrawShape::AR_Load( rA );
   rA >> *this;
 }

CArchive& operator<<( CArchive& rA, TDrawPoint& rCh )
 {
   operator<<( rA, (TDrawShape&)rCh );

   bool bKeyH = rCh.IsHighLighted();
   if( bKeyH ) rCh.UnHighLight( false );	      
      
   rCh.m_tpPen.AR_Store( rA );
   rCh.m_brBrush.AR_Store( rA );

   if( bKeyH ) rCh.HighLight( false );

   return rA;
 }

CArchive& operator>>( CArchive& rA, TDrawPoint& rCh )
 {
   operator>>( rA, (TDrawShape&)rCh );

   rCh.m_tpPen.AR_Load( rA );
   rCh.m_brBrush.AR_Load( rA );

   return rA;
 }


//------------------------------------------

TDrToolItem* TDrawTxt::GetDrTool( TTypesDrawTool typ )
 {
   switch( typ )
	{
	  case TDR_Font:	    
	    return &m_fnFont;			  	  
	  default:
	    return NULL;
	};
 }

void TDrawTxt::InitGDI()
 {            
   m_fnFont.Create();
 }


TDrawTxt::TDrawTxt():
  m_fnFont( *GDIMgr() ),
  m_mtx( FALSE )
 {
   m_csTxt = "Текст";
   InitGDI();

   m_bMultiLine = m_bLock = false;
   m_uiAln = DT_CENTER;
 }

TDrawTxt::TDrawTxt( const CRect& position ): 
  TDrawShape( position ),
   m_fnFont( *GDIMgr() ),
   m_mtx( FALSE )
 {   
   m_csTxt = "Текст";
   InitGDI();
   m_bMultiLine = m_bLock = false;
   m_uiAln = DT_CENTER;
   ASSERT_VALID( this );	
 }

TDrawTxt::TDrawTxt( const TDrawTxt& rPC ):
  TDrawShape( rPC ),
  m_fnFont( *GDIMgr() ),
  m_mtx( FALSE )
 { 
   CSingleLock lock( &m_mtx, TRUE );
   CSingleLock lock2( (CMutex*)&rPC.m_mtx, TRUE );
   m_csTxt = rPC.m_csTxt;
   lock.Unlock();

   m_fnFont = rPC.m_fnFont;
   m_bMultiLine = rPC.m_bMultiLine;
   m_bLock = false;
   m_uiAln = rPC.m_uiAln;
 }

void TDrawTxt::SetTxt( LPCSTR lp )
 {
   CSingleLock lock( &m_mtx, TRUE );
   m_csTxt = lp;
   lock.Unlock();
   Invalidate();
 }

TDrawTxt::~TDrawTxt()
 {
 }


void TDrawTxt::Draw( CDC* pDC, double dSc )
 {   
   CSingleLock lock( &m_mtx, TRUE );

   ASSERT_VALID(this);

   CFont *pFOld = pDC->SelectObject( m_fnFont() );
   pDC->SetTextColor( m_fnFont.GetData().m_clr );
	//(CFont*)pDC->SelectStockObject( ANSI_VAR_FONT );     
     
   /*CRect rOb = m_position;

   rOb.left = double(rOb.left) * dSc, rOb.right = double(rOb.right) * dSc,
   rOb.top = double(rOb.top) * dSc, rOb.bottom = double(rOb.bottom) * dSc;
*/ 
   CRect rp; MulRect( rp, m_position, dSc );
   
   pDC->SetBkMode( TRANSPARENT );

   if( m_bMultiLine )
	{
      CRect rr( rp ); 
	  int iHT = pDC->DrawText( m_csTxt, rr, m_uiAln|DT_VCENTER|DT_NOPREFIX|DT_WORDBREAK|DT_EDITCONTROL|DT_CALCRECT );
	  int iShft = (abs(rp.Height()) - abs(iHT)) / 2;
	  if( iShft > 0 ) rp.OffsetRect( 0, -iShft );
      pDC->DrawText( m_csTxt, rp, m_uiAln|DT_VCENTER|DT_NOPREFIX|DT_WORDBREAK|DT_EDITCONTROL );
	}
   else
    pDC->DrawText( m_csTxt, &rp, 
	  m_uiAln|DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX|DT_WORDBREAK|DT_EDITCONTROL );   
  
   pDC->SelectObject( pFOld );
 }

int TDrawTxt::GetGlueCount()
 {
   ASSERT_VALID(this);
   return 0;
 }
CPoint TDrawTxt::GetGlue( int nHandle )
 {
   ASSERT_VALID(this);
   return TDrawShape::GetGlue(nHandle);
 }

CPoint TDrawTxt::GetGlue_Muled( int nHandle )
 {
   ASSERT_VALID(this);
   return TDrawShape::GetGlue_Muled( nHandle );
 }

int TDrawTxt::GetHandleCount()
 {
   ASSERT_VALID(this);
   return TDrawShape::GetHandleCount();
 }

// returns center of handle in logical coordinates
CPoint TDrawTxt::GetHandle( int nHandle )
 {
   ASSERT_VALID(this);
   return TDrawShape::GetHandle(nHandle);
 }

HCURSOR TDrawTxt::GetHandleCursor( int nHandle, bool bFlDrag )
 {
   ASSERT_VALID(this);
   
   return TDrawShape::GetHandleCursor( nHandle, bFlDrag );
 }

// point is in logical coordinates
void TDrawTxt::MoveHandleTo( int nHandle, CPoint point, TDiplomView* pView, bool bFlEQ )
 {
   ASSERT_VALID(this);

   CRect r = m_position;
   TDrawShape::MoveHandleTo( nHandle, point, pView, bFlEQ );
   if( m_position.Width() < 5 )
	 m_position.left = r.left, m_position.right = r.right;
   if( -m_position.Height() < 5 )
	 m_position.top = r.top, m_position.bottom = r.bottom;
 }

void TDrawTxt::MoveTo( const CRect& positon, TDiplomView* pView )
 {
   TDrawShape::MoveTo( positon, pView );
 }

// rect must be in logical coordinates
BOOL TDrawTxt::Intersects( const CRect& rect )
{
	ASSERT_VALID(this);

	CRect rectT = rect;
	rectT.NormalizeRect();

	//CRect fixed = m_position;
	//fixed.NormalizeRect();
	CRect fixed( GetPosition() );
	if( (rectT & fixed).IsRectEmpty() )
	  return FALSE;

	/*CRgn rgn;	
	rgn.CreateRectRgn( fixed.left, fixed.top, fixed.right, fixed.bottom );		
	return rgn.RectInRegion(fixed);*/

	return TRUE;
}

TDrawShape* TDrawTxt::Clone( TDiplomDoc* pDoc )
 {
	ASSERT_VALID(this);

	TDrawTxt* pClone = new TDrawTxt( *this );	
	ASSERT_VALID(pClone);
    
	Invalidate();
	int iSelKey = m_iSelCount;
	Select( m_pDocument->RefLstDraw(), false );

    pClone->m_position.OffsetRect( 10, -10 );
	pClone->SetScaled( pClone->m_dSc );

	if( pDoc != NULL )
	 {
	   pClone->Select( m_pDocument->RefLstDraw(), true );
	   pDoc->Add( pClone );
	   pClone->m_iSelCount = iSelKey;
	   pClone->Invalidate();
	 }

    pDoc->SetModifiedFlag();

	ASSERT_VALID(pClone);
	return pClone;
}

void TDrawTxt::BoundsRound()
 {
   CSingleLock lock( &m_mtx, TRUE );

   CWindowDC dc( NULL );
   CFont *pFOld = dc.SelectObject( m_fnFont() );
   CSize sz = dc.GetTextExtent( "A", 1 );

   CRect rp; MulRect( rp, m_position, m_dSc );   
   rp.NormalizeRect();
   dc.DrawText( m_csTxt, rp, m_uiAln|DT_VCENTER|DT_NOPREFIX|DT_WORDBREAK|DT_EDITCONTROL|DT_CALCRECT );   
   rp.InflateRect( sz.cx/3, sz.cy/3 );
   DivRect( rp, rp, m_dSc );

   m_position = rp;	
   m_position.top = rp.bottom, m_position.bottom = rp.top;
   //m_position.InflateRect( 2, 2 );

   dc.SelectObject( pFOld );
 }

void TDrawTxt::SetBounds( int iWCh, int iHCh )
 {
   CWindowDC dc( NULL );
   CFont *pFOld = dc.SelectObject( m_fnFont() );
   CSize sz = dc.GetTextExtent( "A", 1 );
   sz.cx *= iWCh;
   sz.cy *= iHCh;

   sz.cx = MulDiv( sz.cx, GetDeviceCaps(dc, LOGPIXELSX), 72 ) + 4;
   sz.cy = MulDiv( sz.cy, GetDeviceCaps(dc, LOGPIXELSY), 72 ) + 4;

   m_position = CRect( 0, sz.cy, sz.cx, 0 );	
   //m_position.InflateRect( 2, 2 );

   dc.SelectObject( pFOld );
 }

void TDrawTxt::SetBounds()
 {
   CWindowDC dc( NULL );
   CFont *pFOld = dc.SelectObject( m_fnFont() );
   CSize sz = dc.GetTextExtent( InitTxt(), strlen(InitTxt()) );

   sz.cx = MulDiv( sz.cx, GetDeviceCaps(dc, LOGPIXELSX), 72 ) + 4;
   sz.cy = MulDiv( sz.cy, GetDeviceCaps(dc, LOGPIXELSY), 72 ) + 4;

   m_position = CRect( 0, sz.cy, sz.cx, 0 );	
   //m_position.InflateRect( 2, 2 );

   dc.SelectObject( pFOld );
 }

void TDrawTxt::AR_Store( CArchive& rA )
 { 
   TDrawShape::AR_Store( rA );
   rA << *this;
 }
void TDrawTxt::AR_Load( CArchive& rA )
 {
   TDrawShape::AR_Load( rA );
   rA >> *this;
 }

CArchive& operator<<( CArchive& rA, TDrawTxt& rCh )
 {
   operator<<( rA, (TDrawShape&)rCh );

   rA << rCh.m_csTxt;
   rCh.m_fnFont.AR_Store( rA );   

   rA << rCh.m_uiAln << int(rCh.m_bMultiLine);

   return rA;
 }

CArchive& operator>>( CArchive& rA, TDrawTxt& rCh )
 {
   operator>>( rA, (TDrawShape&)rCh );

   rA >> rCh.m_csTxt;
   rCh.m_fnFont.AR_Load( rA );   

   int iRd;
   rA >> rCh.m_uiAln >> iRd;

   rCh.m_bMultiLine = iRd;

   return rA;
 }

void TDrawTxt::SetScaled( double dSc )
 {
   TDrawShape::SetScaled( dSc );   
 }


//------------------------------------------

void TDrawRuler::InitGDI()
 {         
   (*m_tpPen).m_iStyle = PS_SOLID;
   (*m_tpPen).m_iWidth = 0;
   (*m_tpPen).m_clr = RGB(0,0,255);   
   m_tpPen.Create();

   (*m_tpPenSel).m_iStyle = PS_SOLID;
   (*m_tpPenSel).m_iWidth = 0;
   (*m_tpPenSel).m_clr = RGB(0,255,0);   
   m_tpPenSel.Create();
 }

TDrawRuler::TDrawRuler():
  m_tpPen( *GDIMgr() ),
  m_tpPenSel( *GDIMgr() )
 {
   m_bFlVert = false;
   m_position = CRect( 0, 3, 3, 0 );	

   InitGDI();
 }

TDrawRuler::TDrawRuler( const CRect& position ): 
  TDrawShape( position ),
  m_tpPen( *GDIMgr() ),
  m_tpPenSel( *GDIMgr() )
 {   
   m_bFlVert = false;
   InitGDI();

   ASSERT_VALID( this );	
 }

TDrawRuler::TDrawRuler( const TDrawRuler& rPC ):
  TDrawShape( rPC ),
  m_tpPen( *GDIMgr() ),
  m_tpPenSel( *GDIMgr() )
 {   
   m_bFlVert = rPC.m_bFlVert;

   m_tpPen = rPC.m_tpPen;
   m_tpPenSel = rPC.m_tpPenSel;
 }

TDrawRuler::~TDrawRuler()
 {
 }


void TDrawRuler::Draw( CDC* pDC, double dSc )
 {
   ASSERT_VALID(this);

   CRect rp; MulRect( rp, m_position, dSc );
   rp.NormalizeRect();
   CPen *pOld;
      
   //CPen pen( PS_SOLID, 1, RGB(0,0,255) );
   //CPen pSel( PS_SOLID, 1, RGB(0,255,0) );

   pOld = pDC->SelectObject( m_tpPen() );

   if( m_bFlVert )
	{   
      pDC->MoveTo( rp.left + 1, SHRT_MIN );	  
	  pDC->LineTo( rp.left + 1, SHRT_MAX );	  
	  if( IsSelected() )
	   {
         pDC->SelectObject( m_tpPenSel() );
		 pDC->MoveTo( rp.left, SHRT_MIN );	  
	     pDC->LineTo( rp.left, SHRT_MAX );	  
		 pDC->MoveTo( rp.left+2, SHRT_MIN );	  
	     pDC->LineTo( rp.left+2, SHRT_MAX );	  
	   }
	}
   else
	{
	  pDC->MoveTo( SHRT_MIN, rp.top + 1 );	  
	  pDC->LineTo( SHRT_MAX, rp.top + 1 );	   
	  if( IsSelected() )
	   {
         pDC->SelectObject( m_tpPenSel() );
		 pDC->MoveTo( SHRT_MIN, rp.top );	  
	     pDC->LineTo( SHRT_MAX, rp.top );	   
		 pDC->MoveTo( SHRT_MIN, rp.top + 2 );	  
	     pDC->LineTo( SHRT_MAX, rp.top + 2 );	   
	   }
	}             

   pDC->SelectObject( pOld );
 }

int TDrawRuler::GetGlueCount()
 {
   ASSERT_VALID(this);
   return 0;
 }
CPoint TDrawRuler::GetGlue( int nHandle )
 {
   ASSERT_VALID(this);
   return TDrawShape::GetGlue(nHandle);
 }

CPoint TDrawRuler::GetGlue_Muled( int nHandle )
 {
   ASSERT_VALID(this);
   return TDrawShape::GetGlue_Muled( nHandle );
 }

int TDrawRuler::GetHandleCount()
 {
   ASSERT_VALID(this);
   return TDrawShape::GetHandleCount();
 }

// returns center of handle in logical coordinates
CPoint TDrawRuler::GetHandle( int nHandle )
 {
   ASSERT_VALID(this);
   return 0;
 }

HCURSOR TDrawRuler::GetHandleCursor( int nHandle, bool bFlDrag )
 {
   ASSERT_VALID(this);
   
   return TDrawShape::GetHandleCursor( nHandle, bFlDrag );
 }

// point is in logical coordinates
void TDrawRuler::MoveHandleTo( int nHandle, CPoint point, TDiplomView* pView, bool bFlEQ )
 {
   ASSERT_VALID(this);

   if( !IsRulesVisible(m_pDocument) ) return;
   
   TDrawShape::MoveHandleTo( nHandle, point, pView, bFlEQ );   
 }

void TDrawRuler::MoveTo( const CRect& positon, TDiplomView* pView )
 {
   if( !IsRulesVisible(m_pDocument) ) return;

   TDrawShape::MoveTo( positon, pView );
 }

// rect must be in logical coordinates
BOOL TDrawRuler::Intersects( const CRect& rect )
 {
	ASSERT_VALID(this);

    if( !IsRulesVisible(m_pDocument) ) return FALSE;

	CRect rectT = rect;
	rectT.NormalizeRect();

	//CRect fixed = m_position;
	//fixed.NormalizeRect();
	CRect fixed( GetPosition() );
	if( (rectT & fixed).IsRectEmpty() )
	  return FALSE;

	/*CRgn rgn;	
	rgn.CreateRectRgn( fixed.left, fixed.top, fixed.right, fixed.bottom );		
	return rgn.RectInRegion(fixed);*/

	return TRUE;
 }

TDrawShape* TDrawRuler::Clone( TDiplomDoc* pDoc )
 {
   ASSERT_VALID(this);

   TDrawRuler* pClone = new TDrawRuler( *this );	
   ASSERT_VALID(pClone);
   
   Invalidate();
   int iSelKey = m_iSelCount;
   Select( m_pDocument->RefLstDraw(), false );

   pClone->m_position.OffsetRect( 10, -10 );
   pClone->SetScaled( pClone->m_dSc );

   if( pDoc != NULL )
	{
	  pClone->Select( m_pDocument->RefLstDraw(), true );
	  pDoc->Add( pClone );
	  pClone->m_iSelCount = iSelKey;
	  pClone->Invalidate();
	}

   pDoc->SetModifiedFlag();

   ASSERT_VALID(pClone);
   return pClone;
 } 

void TDrawRuler::AR_Store( CArchive& rA )
 { 
   TDrawShape::AR_Store( rA );
   rA << *this;
 }
void TDrawRuler::AR_Load( CArchive& rA )
 {
   TDrawShape::AR_Load( rA );
   rA >> *this;
 }

void TDrawRuler::ChCursor( HCURSOR& rHc )
 {
   rHc = AfxGetApp()->LoadStandardCursor( m_bFlVert ? IDC_SIZEWE:IDC_SIZENS );
 }

CArchive& operator<<( CArchive& rA, TDrawRuler& rCh )
 {
   operator<<( rA, (TDrawShape&)rCh );
   rA << int(rCh.m_bFlVert);

   rCh.m_tpPen.AR_Store( rA );   
   rCh.m_tpPenSel.AR_Store( rA );      

   return rA;
 }

CArchive& operator>>( CArchive& rA, TDrawRuler& rCh )
 {
   operator>>( rA, (TDrawShape&)rCh );
   int iT; rA >> iT; rCh.m_bFlVert = bool(iT);

   rCh.m_tpPen.AR_Load( rA );   
   rCh.m_tpPenSel.AR_Load( rA );      

   return rA;
 }

void TDrawRuler::SetVert( bool bFl, const CPoint& rPt )
 {   
   m_bFlVert = bFl;
   if( bFl )
	 m_position = CRect( rPt.x, 10000, rPt.x + 3, -10000 );	
   else
	 m_position = CRect( -10000, rPt.y + 3, 10000, rPt.y );	
 }
