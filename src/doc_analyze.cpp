#include "doc_analyze.hpp"
#include "diplomDoc.h"
#include "diplomView.h"
#include "my_uniq.hpp"
#include "drawobj.h"
#include <math.h>

enum TGR_Mode { TG_UCS, TG_Rng };
template<> struct std::greater<class TDrawShape *>
 {      
   bool operator()( const TDrawShape*& a1, const TDrawShape*& a2 ) const
	{
	  if( m_grMode == TG_UCS )
	   {
		 CRect r1( a1->m_position ); 
		 a1->ExtendRect( r1, 1 );
		 CRect r2( a2->m_position ); 
		 a2->ExtendRect( r2, 1 );
		 if( m_iHV < 3 )
		   return min( r1.left, r1.right ) < min( r2.left, r2.right );
		 else
		   return min( r1.top, r1.bottom ) < min( r2.top, r2.bottom );
	   }
	  else
	   {
	     if( a1->isA() == TT_Point ) return false;
		 if( a2->isA() == TT_Point ) return true;
		 if( ((TDrawPC*)a1)->m_sRang == ((TDrawPC*)a2)->m_sRang )
		   return ((TDrawPC*)a1)->m_sNum > ((TDrawPC*)a2)->m_sNum;
		 else
		   return ((TDrawPC*)a1)->m_sRang > ((TDrawPC*)a2)->m_sRang;
	   }
	}

   TDistrHV m_iHV;
   TGR_Mode m_grMode;
 };


static LONG GRef( CRect& rR, TDistrHV iHV )
 {
   switch( iHV )
	{
	  case TD_1:
	    if( rR.left < rR.right ) return rR.left;
		else return rR.right;
	  case TD_3:
	    if( rR.left > rR.right ) return rR.left;
		else return rR.right;
	  case TD_2:
	    return (rR.left + rR.right) / 2;

	  case TD_4:
	    if( rR.top > rR.bottom ) return rR.top;
		else return rR.bottom;
	  case TD_6:
	    if( rR.top < rR.bottom ) return rR.top;
		else return rR.bottom;
	  case TD_5:
	    return (rR.top + rR.bottom) / 2;
	};
   return 0;
 }

void TDiplomDoc::DistribSelection( TDistrHV iHV )
 {
   TVarHolderVal<int> hold( TDrawShape::m_iGlueLevel, 0 );

   list<TDrawShape*> lstSel;
   TDrawShapeList::iterator itSta( m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_lstDrw.end() );   
   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->IsSelected() &&
	     !((*itSta)->isA() == TT_Cnct && ((TCnct*)((*itSta).get()))->IsHaveGlu()) &&
         (*itSta)->isA() != TT_Ruler
	   )	 
	  lstSel.push_back( (*itSta).get() );

   greater<class TDrawShape*> mgr; 
   mgr.m_iHV = iHV; mgr.m_grMode = TG_UCS;
   lstSel.sort( mgr );   

   int iA, iB;
   CRect r1( lstSel.front()->m_position ); 
   lstSel.front()->ExtendRect( r1, 1 );
   CRect r2( lstSel.back()->m_position ); 
   lstSel.back()->ExtendRect( r2, 1 );
   LONG ref1 = GRef( r1, iHV ),
	    ref2 = GRef( r2, iHV );
   double dMul = double(ref2 - ref1) / double(lstSel.size() - 1);

   TDiplomView *pV = GetMainViev();
   list<TDrawShape*>::iterator itSta2( lstSel.begin() );
   list<TDrawShape*>::iterator itEnd2( lstSel.end() );
   for( int i = 0; itSta2 != itEnd2; ++itSta2, ++i )
	{
      pV->InvalObj( *itSta2, true );

	  CRect r( (*itSta2)->m_position ); 
      (*itSta2)->ExtendRect( r, 1 );

	  if( iHV < 3 )
	    (*itSta2)->m_position.OffsetRect( 
		   CPoint(ref1 + double(i)*dMul - GRef(r, iHV), 0) ); 
	  else
	    (*itSta2)->m_position.OffsetRect( 
		   CPoint(0, ref1 + double(i)*dMul - GRef(r, iHV)) ); 

	  pV->InvalObj( *itSta2, true );
	}

   //CheckAllGlued();
   //RecalcAllGlued();
   RecalcAllGlued_Selected();
   SetModifiedFlag();
 }

void TDoublePolus::RemovePoints( list<TDrawShape*>& rL )
 {
   list<TDrawShape*>::iterator itSta( rL.begin() );
   list<TDrawShape*>::iterator itEnd( rL.end() );

   for( ; itSta != itEnd; )
	 if( (*itSta)->isA() == TT_Point ) rL.erase( itSta++ );
	 else ++itSta;	   
 }
void TDoublePolus::RemovePoints()
 {
   TDoublePolus::RemovePoints( m_lstPC );
 }

void TDoublePolus::MarkCncts( bool bFl )
 {
   list<TDrawShape*>::iterator itSta( m_lstPC.begin() );
   list<TDrawShape*>::iterator itEnd( m_lstPC.end() );      
   itEnd--;      
   
   for( ; itSta != itEnd; ++itSta )
	{
      list<TDrawShape*>::iterator itSta2( itSta );	  
      list<TDrawShape*>::iterator itEnd2( m_lstPC.end() );   
	  
	  itSta2++;	  

	  for( ; itSta2 != itEnd2; ++itSta2 )
	   {	     		 
		 TCnct *pCnct = (*itSta)->GetLinkTo( *itSta2 );
		 if( pCnct ) pCnct->m_bFlMark = bFl;
	   }	  
	}   
 }


void TDoublePolus::CreFlipLst( list<TCnct*>& rL, short sRng )
 {
   list<TDrawShape*>::iterator itSta( m_lstPC.begin() );
   list<TDrawShape*>::iterator itEnd( m_lstPC.end() );         
   
   for( ; itSta != itEnd; ++itSta )
	{
      TCnct *pCnct = NULL;   
	  bool  bFlNoEnter;
	  TDrawShape *p = (*itSta)->GoTop( pCnct, *itSta, false, bFlNoEnter, true );
				
	  if( p && pCnct && p->isA() == TT_PC && ((TDrawPC*)p)->m_sRang > sRng &&
	      !pCnct->m_bFlMark ) rL.push_back( pCnct );

	  for( ; pCnct; p = (*itSta)->GoTop(pCnct, *itSta, false, bFlNoEnter, true) )	
	    if( p && pCnct && p->isA() == TT_PC && ((TDrawPC*)p)->m_sRang > sRng &&
	      !pCnct->m_bFlMark ) rL.push_back( pCnct );
	}
 }

void TDoublePolus::Intersect2( list<TDrawShape*>& rL )
 {
   list<TDrawShape*>::iterator itSta( rL.begin() );
   list<TDrawShape*>::iterator itEnd( rL.end() );
   for( ; itSta != itEnd; )
	{
      list<TDrawShape*>::iterator it = 
	     find( m_lstPC.begin(), m_lstPC.end(), *itSta );
	  if( it == m_lstPC.end() ) rL.erase( itSta++ );
	  else ++itSta;
	}
 }

void TDoublePolus::Intersect( TDoublePolus& rDp, list<TDrawShape*>& rL )
 {
   list<TDrawShape*>::iterator itSta( m_lstPC.begin() );
   list<TDrawShape*>::iterator itEnd( m_lstPC.end() );
   //set_intersection( m_lstPC.begin(), m_lstPC.end(), rDp.m_lstPC.begin(), rDp.m_lstPC.end(), rL.begin() );
   for( ; itSta != itEnd; ++itSta )
	{
      list<TDrawShape*>::iterator it = find( rDp.m_lstPC.begin(), rDp.m_lstPC.end(), *itSta );
	  if( it != rDp.m_lstPC.end() )
	    rL.push_back( *it );
	}

   //rL.unique();
   My_unique( rL );
   
   std::greater<class TDrawShape*> mgr; 
   mgr.m_grMode = TG_Rng;
   rL.sort( mgr );
 }

int TDoublePolus::CountPC( list<TDrawShape*> &rL )
 {
   list<TDrawShape*>::iterator itSta( rL.begin() );
   list<TDrawShape*>::iterator itEnd( rL.end() );   
      
   int iCnt = 0;
   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->isA() == TT_PC ) iCnt++;

   return iCnt;
 }

TDoublePolus::TDoublePolus( TDrawPC *pSta, TDrawShapeList &lstDrw, int iNum, TDiplomDoc* pDoc, bool bMaxOut )
 {
   m_iNum = iNum;

   m_exc.m_exc = TE_None;

   m_lstSiz = TDiplomDoc::CalcPCAndPoints( lstDrw );
  
   ResetUsedFlag( lstDrw );   
   int iOut = -1;
   int iRet;
   list<int> lstOuts;
   try {
	  FindOutRng( pSta, iOut, lstOuts, NULL );
	}
   catch( TGrafExc exc )
	{
      m_exc = exc;
	  return;
	}

   list<int>::iterator it = bMaxOut ?
	max_element( lstOuts.begin(), lstOuts.end() ):
	min_element( lstOuts.begin(), lstOuts.end() );
   if( it == lstOuts.end() )
	{
      m_exc.m_exc = TE_NoTerm;
	  return;
	}   
   m_iOutRng = *it;   
   lstOuts.clear();
   ResetUsedFlag( lstDrw );
   m_pSta = FindSta( lstDrw );
   if( !m_pSta )
	{
      m_exc.m_exc = TE_NoStart;
	  return;
	}

   iOut = -1;
   bool bFlMark = false;
   try {
     MarkDP( m_pSta, iOut, NULL, bFlMark );
	}
   catch( TGrafExc exc )
	{
      m_exc = exc;
	  return;
	}

   //m_lstPC.unique();
   My_unique( m_lstPC );

   /*TDrawShapeList::iterator itSta( lstDrw.begin() );
   TDrawShapeList::iterator itEnd( lstDrw.end() );
   
   for( ; itSta != itEnd; ++itSta )*/

   list<TDrawPC*> lstSort;
   pDoc->CreRngs_Back( lstSort );
   std::greater<class TDrawShape*> mgr; 
   mgr.m_grMode = TG_Rng;
   m_lstPC.sort( mgr );

   list<TDrawShape*>::iterator itStaMy( m_lstPC.begin() );
   list<TDrawShape*>::iterator itEndMy( m_lstPC.end() );

   list<TDrawPC*>::iterator itSta( lstSort.begin() );
   list<TDrawPC*>::iterator itEnd( lstSort.end() );
 
   for( ; itStaMy != itEndMy; )
	{
      if( (*itStaMy)->isA() != TT_PC ) break;

	  short sRng = ((TDrawPC*)*itStaMy)->m_sRang;
	  for( ; itSta != itEnd; ++itSta )
	    if( (*itSta)->m_sRang == sRng ) break;

	  if( itSta == itEnd )
	   {
         m_exc.m_exc = TE_Assimetric;
		 m_exc.m_iData = sRng;
	     return;
	   }

      for( ; itStaMy != itEndMy && itSta != itEnd; ++itStaMy, ++itSta )
	   {
	     if( (*itStaMy)->isA() != TT_PC ) break;
		 if( ((TDrawPC*)*itStaMy)->m_sRang != sRng ) break;

		 if(
		     ((TDrawPC*)*itStaMy)->m_sRang != (*itSta)->m_sRang ||
		     ((TDrawPC*)*itStaMy)->m_sNum != (*itSta)->m_sNum
		   )
		  {
			 m_exc.m_exc = TE_Assimetric;
			 m_exc.m_iData = sRng;
			 return;
		  }
	   }
	  if( itSta != itEnd && (*itSta)->m_sRang == sRng )
	   {
		  m_exc.m_exc = TE_Assimetric;
		  m_exc.m_iData = sRng;
		  return;
	   }

	  if( itStaMy == itEndMy ) break;
	}
 }

TDrawShape* TDoublePolus::FindSta( TDrawShapeList &lstDrw )
 {
   TDrawShapeList::iterator itSta( lstDrw.begin() );
   TDrawShapeList::iterator itEnd( lstDrw.end() );
   
   for( ; itSta != itEnd; ++itSta )
     if( (*itSta)->isA() == TT_PC && 
	     ((TDrawPC*)(*itSta).get())->m_sRang == 1 &&
		 ((TDrawPC*)(*itSta).get())->m_sNum == 1 
	   )
	   return (*itSta).get();

   return NULL;
 }

void TDoublePolus::ResetUsedFlag( TDrawShapeList &lstDrw )
 {
   TDrawShapeList::iterator itSta( lstDrw.begin() );
   TDrawShapeList::iterator itEnd( lstDrw.end() );
   
   for( ; itSta != itEnd; ++itSta )
	  (*itSta)->ResUsedFlag();
 }

void TDoublePolus::MarkDP( TDrawShape *pPC, int& riOut, TDrawShape *pPCSrc, bool& rFlOK )
 {
   if( !pPC ) return;

   if( pPC->isA() == TT_PC )	 
	{
	  if( riOut >= ((TDrawPC*)pPC)->m_sRang ) 
	    throw TGrafExc( TE_HaveHLinks );

	  //if( ((TDrawPC*)pPC)->IsUsed() )
	    //throw TGrafExc( TE_HaveBadLinks );

	  riOut = ((TDrawPC*)pPC)->m_sRang;	 
	}

   pPC->SetUsedFlag( true );
   if( pPC->IsUsed() > m_lstSiz )
	 throw TGrafExc( TE_HaveCycles );

   TCnct *pCnct = NULL;
   bool bFlNoEnter;
   TDrawShape *p = pPC->GoTop( pCnct, pPCSrc, false, bFlNoEnter, true );
   if( !p ) 
	{   
	  if( !bFlNoEnter && riOut == m_iOutRng )  rFlOK = true;
	  return;
	}
   
   bool bFlKeyP = (p && p->isA() == TT_Point);
   for( ; pCnct; p = pPC->GoTop(pCnct, pPCSrc, false, bFlNoEnter, true) )	
	{
	  int iOut = riOut;
	  bool bFlThOK = false;
	  MarkDP( p, iOut, pPC, bFlThOK );	  
	  if( bFlThOK ) 
	   {
	     rFlOK = true;
	     MarkObj( p );
	   }
	}

   if( rFlOK ) MarkObj( pPC );

      
   if( bFlKeyP && !bFlNoEnter && riOut == m_iOutRng ) rFlOK = true;	 
 }

void TDoublePolus::MarkObj( TDrawShape *p )
 {
   bool bFlAdd;

   if( p->isA() == TT_PC )
	 bFlAdd = ((TDrawPC*)p)->MarkBiPolus( m_iNum );
   else if( p->isA() == TT_Point )
	 bFlAdd = ((TDrawPoint*)p)->MarkBiPolus( m_iNum );
   else return;

   if( bFlAdd ) m_lstPC.push_back( p );
 }

void TDoublePolus::FindOutRng( TDrawShape *pPC, int& riOut, list<int>& rlstOut, TDrawShape *pPCSrc )
 {       
   if( pPC->isA() == TT_PC )	 
	{
	  if( riOut >= ((TDrawPC*)pPC)->m_sRang ) 
	    throw TGrafExc( TE_HaveHLinks );

	  //if( ((TDrawPC*)pPC)->IsUsed() )
	    //throw TGrafExc( TE_HaveBadLinks );

	  riOut = ((TDrawPC*)pPC)->m_sRang;	 
	}

   pPC->SetUsedFlag( true );
   if( pPC->IsUsed() > m_lstSiz )
	 throw TGrafExc( TE_HaveCycles );
   //if( pPC->isA() == TT_PC && ((TDrawPC*)pPC)->IsUsed() > 2 )
	 //throw TGrafExc( TE_HaveCycles );

   TCnct *pCnct = NULL;
   bool bFlNoEnter;
   TDrawShape *p = pPC->GoTop( pCnct, pPCSrc, false, bFlNoEnter, true );
   if( !p ) 
	{
      if( !bFlNoEnter ) rlstOut.push_back( riOut );
	  return;
	}
       
   bool bFlKeyP = (p && p->isA() == TT_Point);
   for( ; p; p = pPC->GoTop(pCnct, pPCSrc, false, bFlNoEnter, true) )	
	{
	  int iOut = riOut;
	  FindOutRng( p, iOut, rlstOut, pPC );	  
	}

   if( bFlKeyP && !bFlNoEnter )
	 rlstOut.push_back( riOut );
 }

void TDoublePolus::AddNoDup( list<int>& lst, int iAdd )
 {
   list<int>::iterator it = find( lst.begin(), lst.end(), iAdd );
   if( it == lst.end() ) lst.push_back( iAdd );
 }

TDrawPC *TPCPath::FindIntersectMax( TPCPath& rP )
 {
   list<TDrawShape*>::iterator itSta( m_lst.begin() );
   list<TDrawShape*>::iterator itEnd( m_lst.end() );
   int iRng = -1;
   TDrawPC *pRet = NULL;
   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->isA() == TT_PC )
	  {
		list<TDrawShape*>::iterator it = find( rP.m_lst.begin(), rP.m_lst.end(), *itSta );
		if( it != rP.m_lst.end()  &&
			iRng < ((TDrawPC*)(*itSta))->m_sRang
		  )
		  iRng = ((TDrawPC*)(*itSta))->m_sRang,
		  pRet = (TDrawPC*)*itSta;
	  }

   return pRet;
 }

void TPCConnect::Create1( TDrawPC *pPC, TDrawPC *pEnd )
 {
   m_pPC = pPC;
   
   bool bFl = false;
   FindBackPath1( pPC, pEnd, NULL, bFl );
	//TDrawShape *pPC, int& riOut, TDrawShape *pPCSrc, bool& rFlOK )
   if( bFl ) m_pats.m_lst.push_back( pPC );
   //m_pats.m_lst.unique();
   My_unique( m_pats.m_lst );

   std::greater<class TDrawShape*> mgr; 
   mgr.m_grMode = TG_Rng;
   m_pats.m_lst.sort( mgr );   

   //m_pats.m_lst.remove( pPC );

   if( !m_pats.m_lst.size() && pPC->m_sRang == 1 && pPC->m_sNum == 1 )
	{
	  /*if( bFlPutRefs )
	   {
	     pPC->m_bCTypeValid = true;
         m_cType = TCT_11; 
	     pPC->m_pTPC = this;
	   }*/
	  return;
	}

   list<TDrawShape*>::iterator itSta( m_pats.m_lst.begin() );
   list<TDrawShape*>::iterator itEnd( m_pats.m_lst.end() );
   int iCnt = 0;
   for( ;itSta != itEnd; ++itSta )	
	 if( (*itSta)->isA() == TT_PC ) 
	  {
	    iCnt++;   
		if( ((TDrawPC*)(*itSta))->m_sRang > pPC->m_sRang )
		 {
		   TGrafExc exc( TE_BadInLink_LotoHi );
		   exc.m_iRng = pPC->m_sRang;
		   exc.m_iNumb = pPC->m_sNum;
		   throw exc;		   
		 }
	  }

   if( iCnt < 1 )
	{
	  TGrafExc exc( TE_CanntGoTo );
	  exc.m_iRng = pPC->m_sRang;
	  exc.m_iNumb = pPC->m_sNum;
	  if( pEnd )
	    exc.m_iRng2 = pEnd->m_sRang,
	    exc.m_iNumb2 = pEnd->m_sNum;
	  else
	    exc.m_iRng2 = -1,
	    exc.m_iNumb2 = -1;
	  throw exc;
	}
   /*if( !pEnd && iCnt > 2 )
	{
	  TGrafExc exc( TE_VeryManyIn );
	  exc.m_iRng = pPC->m_sRang;
	  exc.m_iNumb = pPC->m_sNum;
	  throw exc;	  
	}*/

   m_nIn = iCnt;
   //if( bFlPutRefs ) pPC->m_pTPC = this;
 }

void TPCConnect::Create( TDrawPC *pPC, TDrawPC *pEnd, bool bFlPutRefs, bool bFlEnblIsol )
 {
   if( bFlPutRefs )
     pPC->m_pTPC = NULL,
     pPC->m_bCTypeValid = false;

   m_pPC = pPC;

   int iOut = pEnd ? pEnd->m_sRang:-1;
   bool bFl = false;
   FindBackPath( pPC, iOut, NULL, bFl );
	//TDrawShape *pPC, int& riOut, TDrawShape *pPCSrc, bool& rFlOK )
   //m_pats.m_lst.unique();
   My_unique( m_pats.m_lst );

   std::greater<class TDrawShape*> mgr; 
   mgr.m_grMode = TG_Rng;
   m_pats.m_lst.sort( mgr );   

   m_pats.m_lst.remove( pPC );

   if( !m_pats.m_lst.size() && pPC->m_sRang == 1 && pPC->m_sNum == 1 )
	{
	  if( bFlPutRefs )
	   {
	     pPC->m_bCTypeValid = true;
         m_cType = TCT_11; 
	     pPC->m_pTPC = this;		 
	   }
	  return;
	}

   list<TDrawShape*>::iterator itSta( m_pats.m_lst.begin() );
   list<TDrawShape*>::iterator itEnd( m_pats.m_lst.end() );
   int iCnt = 0;
   for( ;itSta != itEnd; ++itSta )	
	 if( (*itSta)->isA() == TT_PC ) 
	  {
	    iCnt++;   
		if( ((TDrawPC*)(*itSta))->m_sRang >= pPC->m_sRang )
		 {
		   TGrafExc exc( TE_BadInLink_LotoHi );
		   exc.m_iRng = pPC->m_sRang;
		   exc.m_iNumb = pPC->m_sNum;
		   throw exc;		   
		 }
	  }

   if( iCnt < 1 && !bFlEnblIsol )
	{
	  TGrafExc exc( TE_CanntGoBottom );
	  exc.m_iRng = pPC->m_sRang;
	  exc.m_iNumb = pPC->m_sNum;
	  throw exc;
	}
   /*if( !pEnd && iCnt > 2 )
	{
	  TGrafExc exc( TE_VeryManyIn );
	  exc.m_iRng = pPC->m_sRang;
	  exc.m_iNumb = pPC->m_sNum;
	  throw exc;	  
	}*/

   m_nIn = iCnt;
   if( bFlPutRefs ) 
	 pPC->m_pTPC = this,
	 pPC->m_bCTypeValid = true;
 }

TDrawPC *TPCConnect::GetLink1()
 {
   if( !m_pats.m_lst.size() ) return NULL;
   TDrawShape *p = m_pats.m_lst.front();
   if( p->isA() == TT_PC ) return (TDrawPC*)p;
   return NULL;
 }
TDrawPC *TPCConnect::GetLink2()
 {
   if( m_pats.m_lst.size() < 2 ) return NULL;
   TDrawShape *p = *++m_pats.m_lst.begin();
   if( p->isA() == TT_PC ) return (TDrawPC*)p;
   return NULL;
 }

void TPCConnect::FindBackPath1( TDrawShape *pPC, TDrawShape *pPCEnd, TDrawShape *pPCSrc, bool& rFlOK )
 {
   if( !pPC ) return;

   if( pPCEnd == pPC )
	{
      rFlOK = true;    	  
	  return;
	}      

   TCnct *pCnct = NULL;   
   TDrawShape *p = pPC->GoBottom( pCnct, pPCSrc, false, true );   
		      
   for( ; pCnct; p = pPC->GoBottom(pCnct, pPCSrc, false, true) )	
	{	  
	  FindBackPath1( p, pPCEnd, pPC, rFlOK );	  
	  if( rFlOK ) 
	   {	     
	     m_pats.m_lst.push_back( p );
		 return;
	   }
	}

   //if( rFlOK ) m_pats.m_lst.push_back( pPC );
 }

void TPCConnect::FindBackPath( TDrawShape *pPC, int& riOut, TDrawShape *pPCSrc, bool& rFlOK )
 {         
   if( !pPC ) return;

   if( pPCSrc && pPC->isA() == TT_PC &&	
       (riOut == -1 || riOut == ((TDrawPC*)pPC)->m_sRang)
	 )
	{
      rFlOK = true;    
	  //m_pats.m_lst.push_back( p );
	  return;
	}   

   pPC->SetUsedFlag( true );
   if( pPC->IsUsed() > m_lstSiz )
	  throw TGrafExc( TE_HaveCycles );

   TCnct *pCnct = NULL;   
   TDrawShape *p = pPC->GoBottom( pCnct, pPCSrc, false, true );   
		      
   for( ; pCnct; p = pPC->GoBottom(pCnct, pPCSrc, false, true) )	
	{
	  //int iOut = riOut;
	  bool bFlThOK = false;
	  FindBackPath( p, riOut, pPC, bFlThOK );	  
	  if( bFlThOK ) 
	   {
	     rFlOK = true;
	     m_pats.m_lst.push_back( p );
	   }
	}

   if( rFlOK ) m_pats.m_lst.push_back( pPC );
 }

template<> struct std::greater<TAP_TPCConnect>
 {      
   bool operator()( const TAP_TPCConnect& a1, const TAP_TPCConnect& a2 ) const
	{	 	  
	  if( a1->m_pPC->m_sRang == a2->m_pPC->m_sRang )
		return a1->m_pPC->m_sNum > a2->m_pPC->m_sNum;
	  else
		return a1->m_pPC->m_sRang > a2->m_pPC->m_sRang;	 
	}   
 };

void TAnalyzer::RestoreSort()
 {
   std::greater<TAP_TPCConnect> mgr;
   m_lstPC_Cnct.sort( mgr );
 }


void TAnalyzer::Create()
 {
   InvalidateCtypes();
   UnlockAll();

   TDrawShapeList::iterator itSta( m_rlstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_rlstDrw.end() );   
      
   TDoublePolus::ResetUsedFlag( m_rlstDrw );

   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->isA() == TT_PC )
	  {	    	   
        TAP_TPCConnect ap( new TPCConnect(m_iListSz) );
		ap->Create( (TDrawPC*)(*itSta).get(), NULL, true );
		LockConnect( ap.get(), true, false );
		m_lstPC_Cnct.push_back( ap );
	  }   

   std::greater<TAP_TPCConnect> mgr;
   m_lstPC_Cnct.sort( mgr );


   /*TAP_TPCConnect_Lst::iterator itSta2( m_lstPC_Cnct.begin() );
   TAP_TPCConnect_Lst::iterator itEnd2( m_lstPC_Cnct.end() );
   for( ; itSta2 != itEnd2; ++itSta2 )
	  LockConnect( (*itSta2).get(), true, false );*/
   
   AnalyzeConnectType( false );
 }

void TAnalyzer::InvalidateCtype_What( TPCConnect* pC1, TPCConnect* pC2 )
 {
   /*TAP_TPCConnect_Lst::iterator itSta2( m_lstPC_Cnct.begin() );
   TAP_TPCConnect_Lst::iterator itEnd2( m_lstPC_Cnct.end() );
   for( ; itSta2 != itEnd2; ++itSta2 )
	 if( (*itSta2).get() != pC1 && (*itSta2).get() != pC2 )
	  {
	    TDrawPC *pL1 = (*itSta2)->GetLink1(),
		        *pL2 = (*itSta2)->GetLink2();				

		if( (pC1 && (pL1 == pC1->m_pPC || pL2 == pC1->m_pPC)) ||
		    (pC2 && (pL1 == pC2->m_pPC || pL2 == pC2->m_pPC))
		  )
		  InvalidateCtype( (*itSta2).get() ); 
	  }	   */

   TDrawPC *pPC1 = pC1 ? pC1->m_pPC:NULL;
   TDrawPC *pPC2 = pC2 ? pC2->m_pPC:NULL;

   TAP_TPCConnect_Lst::iterator itSta2( m_lstPC_Cnct.begin() );
   TAP_TPCConnect_Lst::iterator itEnd2( m_lstPC_Cnct.end() );
   for( ; itSta2 != itEnd2; ++itSta2 )	 
	{
	  if( (*itSta2).get() == pC1 || (*itSta2).get() == pC2 ) continue;

	  list<TDrawShape*>::iterator itS( (*itSta2)->m_pats.m_lst.begin() );
	  list<TDrawShape*>::iterator itE( (*itSta2)->m_pats.m_lst.end() );
	  for( ; itS != itE; ++itS )
	    if( (*itS)->isA() == TT_PC )
		 {
           TDrawPC *p = (TDrawPC *)*itS;
		   if( p == pPC1 || p == pPC2 )
			{
              InvalidateCtype( (*itSta2).get() );
			  break;
			}
		 }	  
	}	   
 }

void TAnalyzer::InvalidateRng_What( int iRHi, int iRLo )
 {
   TAP_TPCConnect_Lst::iterator itSta2( m_lstPC_Cnct.begin() );
   TAP_TPCConnect_Lst::iterator itEnd2( m_lstPC_Cnct.end() );
   for( ; itSta2 != itEnd2; ++itSta2 )	 
	{
	  list<TDrawShape*>::iterator itS( (*itSta2)->m_pats.m_lst.begin() );
	  list<TDrawShape*>::iterator itE( (*itSta2)->m_pats.m_lst.end() );
	  for( ; itS != itE; ++itS )
	    if( (*itS)->isA() == TT_PC )
		 {
           TDrawPC *p = (TDrawPC *)*itS;
		   if( p->m_sRang <= iRHi && p->m_sRang >= iRLo )
			{
              InvalidateCtype( (*itSta2).get() );
			  break;
			}
		 }	  
	}	   
 }

struct TCnctDta
 {
   TCnctDta( TCnct *pCnct, int nGl )
	{
      m_pCnct = pCnct; 
	  m_nGl = nGl;
	}
   TCnct *m_pCnct;
   int m_nGl;
 };

void TAnalyzer::FlipInCnctTo( TPCConnect *pCSrc, TPCConnect *pCDst )
 {   
   list<TCnctDta> lstCnct;

   list<TGlueDescr>::iterator itSta( pCSrc->m_pPC->m_vecGlu[1]->m_lstGlu.begin() );
   list<TGlueDescr>::iterator itEnd( pCSrc->m_pPC->m_vecGlu[1]->m_lstGlu.end() );
   for( ; itSta != itEnd; ++itSta ) 
	{
      TGlue *pGlue = ((TCnct*)(*itSta).m_pShp)->m_vecGlu[ ((TCnct*)(*itSta).m_pShp)->Another((*itSta).m_nGlu) - 1 ].get();	  

	  TCnctDta dta( (TCnct*)(*itSta).m_pShp, (*itSta).m_nGlu );
	  if( pCDst->m_pPC->IsConnectedTo(pGlue->m_lstGlu.size() && pGlue->m_lstGlu.front().IsGlued() ? pGlue->m_lstGlu.front().m_pShp:NULL, 2) )
	    dta.m_nGl = -1;

	  lstCnct.push_back( dta );
	  //se RemoveLink( (TCnct*)(*itSta).m_pShp );
	}
	

   list<TCnctDta>::iterator itSta2( lstCnct.begin() );
   list<TCnctDta>::iterator itEnd2( lstCnct.end() );
   for( ; itSta2 != itEnd2; ++itSta2 )
	{      	  
	  if( (*itSta2).m_nGl == -1 ) RemoveLink( (*itSta2).m_pCnct );
	  else
	   {
		 (*itSta2).m_pCnct->DeGlue( (*itSta2).m_nGl, m_bRedraw ? m_pDoc->GetMainViev():NULL );
		 bool bFl;
		 (*itSta2).m_pCnct->MakeGlue( (*itSta2).m_nGl, pCDst->m_pPC, 2, bFl );	   	  
	   }
	}

   //pCSrc->m_pPC->m_vecGlu[1]->m_lstGlu.clear();

   pCDst->m_pPC->RecalcGlued( m_bRedraw ? m_pDoc->GetMainViev():NULL, true );
 }

void TAnalyzer::HCenterInRect( TPCConnect *pC, CRect& rRect )
 {
   CPoint pt( rRect.CenterPoint() - pC->m_pPC->m_position.CenterPoint() );
   pt.y = 0;
   if( m_bRedraw ) pC->m_pPC->Invalidate();
   pC->m_pPC->m_position.OffsetRect( pt );
   if( m_bRedraw ) pC->m_pPC->Invalidate();
   pC->m_pPC->RecalcGlued( m_bRedraw ? m_pDoc->GetMainViev():NULL, true );
 }

void TAnalyzer::CenterInRect( TPCConnect *pC, CRect& rRect )
 {
   CPoint pt( rRect.CenterPoint() - pC->m_pPC->m_position.CenterPoint() );
   
   if( m_bRedraw ) pC->m_pPC->Invalidate();
   pC->m_pPC->m_position.OffsetRect( pt );
   if( m_bRedraw ) pC->m_pPC->Invalidate();
   pC->m_pPC->RecalcGlued( m_bRedraw ? m_pDoc->GetMainViev():NULL, true );
 }


void TAnalyzer::Simplify( TPCConnect* pC )
 {
   if( pC->m_cType != TCT_Parall &&
	   pC->m_cType != TCT_Seqv &&
	   pC->m_cType != TCT_FullAccess
	 ) return;

   TPCConnect *pSbl = GetSibling( pC );
   switch( pC->m_cType )
	{
	  case TCT_Seqv:
	   {
         LockConnect( pC, false, true );		 
		 InvalidateCtype_What( pC, pSbl );
		 FlipInCnctTo( pC, pSbl );
		 TParamCalc dta;
		 CalcSeqv( pC, pSbl, dta );
		 RemovePC( pC );
		 AssParamPE( pSbl, dta );
		 NextPE( pSbl );
	   };
	  break;
	  case TCT_Parall:
	  case TCT_FullAccess:
	   {
         TPCConnect *pC1, *pC2;
		 if( pC->m_pPC->m_sNum < pSbl->m_pPC->m_sNum )
		   pC1 = pC, pC2 = pSbl;
		 else pC2 = pC, pC1 = pSbl;

         TDrawShape *pAln = pC1->GetLink1();
		 TDrawShape *pAln2 = pC1->GetLink2();
		 CRect rCntr( pAln->m_position );
		 if( pC->m_cType == TCT_FullAccess )
		  {
		    rCntr.left = min( pAln->m_position.left, pAln2->m_position.left );
			rCntr.right = max( pAln->m_position.right, pAln2->m_position.right );
		  }

         LockConnect( pC2, false, true );		 
		 InvalidateCtype_What( pC1, pC2 );
		 InvalidateCtype_What( ((TDrawPC*)pAln)->m_pTPC, NULL );
		 FlipInCnctTo( pC2, pC1 );
		 TParamCalc dta;
		 CalcParall( pC1, pC2, dta );
		 RemovePC( pC2 );
		 AssParamPE( pC1, dta );
		 NextPE( pC1 );

		 HCenterInRect( pC1, rCntr );
	   };
	   break;
	};

   AnalyzeConnectType( true );
 }

void TAnalyzer::NextPE( TPCConnect* pC )
 {
   pC->m_pPC->m_csName.Format( "ПЭ#%u", (int)m_iPECnt++ );
   pC->m_pPC->GenerateTTY();
   if( m_bRedraw ) pC->m_pPC->Invalidate();
 }

void TAnalyzer::CalcSeqDec( TPCConnect *pC1, TPCConnect *pCpe, TParamCalc &rDta )
 {
   double kc1 = pC1->m_pPC->m_dKC,
	      kcPE = pCpe->m_pPC->m_dKC,
		  kg1 = pC1->m_pPC->m_dKReady,
		  kgPE = pCpe->m_pPC->m_dKReady,
		  l1 = pC1->m_pPC->m_dLambda,
		  lPE = pCpe->m_pPC->m_dLambda;

   rDta.m_dLambda = lPE - l1;
   rDta.m_dKReady = kgPE / kg1;
   
   double a = kg1 * (1 - kg1);
   double b = kc1 * (1 - kgPE) - kcPE * (1 - kg1);
   double c = kc1 * (kc1*rDta.m_dKReady - kcPE) * (1 - rDta.m_dKReady);

   rDta.m_dKC = (-b + sqrt(b*b - 4*a*c)) / (2*a);

   rDta.m_dM = (1 - rDta.m_dKReady) / (rDta.m_dLambda * rDta.m_dKReady);
   rDta.m_dD = 2*rDta.m_dKC / (rDta.m_dLambda*rDta.m_dKReady*rDta.m_dKReady) - rDta.m_dM*rDta.m_dM;
 }

void TAnalyzer::CalcParall( TPCConnect *pC1, TPCConnect *pC2, TParamCalc &rDta )
 {
   double kc1 = pC1->m_pPC->m_dKC,
	      kc2 = pC2->m_pPC->m_dKC,
		  kg1 = pC1->m_pPC->m_dKReady,
		  kg2 = pC2->m_pPC->m_dKReady,
		  l1 = pC1->m_pPC->m_dLambda,
		  l2 = pC2->m_pPC->m_dLambda;

   rDta.m_dKReady = kg1 + kg2 - kg1*kg2;

   rDta.m_dLambda = (kg1*(1 - kg2)*l1 + kg2*(1 - kg1)*l2) / 
	 rDta.m_dKReady;

   rDta.m_dKC = (kc1*kc1*(1 - kg2)*(1 - kg2) + kc1*kc2*(1 - rDta.m_dKReady) + kc2*kc2*(1 - kg1)*(1 - kg1)) /
	 (kc1*(1 - kg2) + kc2*(1 - kg1));

   rDta.m_dM = (1 - rDta.m_dKReady) / (rDta.m_dLambda * rDta.m_dKReady);
   rDta.m_dD = 2*rDta.m_dKC / (rDta.m_dLambda*rDta.m_dKReady*rDta.m_dKReady) - rDta.m_dM*rDta.m_dM;
 }

void TAnalyzer::CalcSeqv( TPCConnect *pC1, TPCConnect *pC2, TParamCalc &rDta )
 {
   rDta.m_dLambda = pC1->m_pPC->m_dLambda + pC2->m_pPC->m_dLambda;   
   double kc1 = pC1->m_pPC->m_dKC,
	      kc2 = pC2->m_pPC->m_dKC,
		  kg1 = pC1->m_pPC->m_dKReady,
		  kg2 = pC2->m_pPC->m_dKReady;
   rDta.m_dKReady = kg1 * kg2;
   rDta.m_dKC = (kc1*kc1 * kg2*(1 - kg2) + kc1*kc2*(1 - rDta.m_dKReady) + kc2*kc2*kg1*(1 - kg1)) /
	(kc1*(1 - kg2) + kc2*(1 - kg1));

   rDta.m_dM = (1 - rDta.m_dKReady) / (rDta.m_dLambda * rDta.m_dKReady);
   rDta.m_dD = 2*rDta.m_dKC / (rDta.m_dLambda*rDta.m_dKReady*rDta.m_dKReady) - rDta.m_dM*rDta.m_dM;
 }

void TAnalyzer::AssParamPE( TPCConnect *pC, TParamCalc &rDta )
 {
   pC->m_pPC->m_dLambda = rDta.m_dLambda;
   pC->m_pPC->m_dM = rDta.m_dM;
   pC->m_pPC->m_dD = rDta.m_dD;
   pC->m_pPC->m_dKReady = rDta.m_dKReady;
   pC->m_pPC->m_dKC	= rDta.m_dKC;
 }

class TRenovPCFnd
 {
public:
  TRenovPCFnd( TPCConnect *pC )
   {
     m_pC = pC;
   }

  bool operator()( const TAP_TPCConnect& rC )
   {
     return m_pC == rC.get();
   }

  TPCConnect *m_pC;
 };

void TAnalyzer::RemovePCs( int iRHi, int iRLo )
 {
   TAP_TPCConnect_Lst::iterator itSta2( m_lstPC_Cnct.begin() );
   TAP_TPCConnect_Lst::iterator itEnd2( m_lstPC_Cnct.end() );
   for( ; itSta2 != itEnd2; )	 
	 if( (*itSta2)->m_pPC->m_sRang <= iRHi &&
	     (*itSta2)->m_pPC->m_sRang >= iRLo
	   )
	  {
		TDrawShape *pObj = (*itSta2)->m_pPC;
		int iId = pObj->ID();

		TDrawShapeList::iterator itSta( m_rlstDrw.begin() );
		TDrawShapeList::iterator itEnd( m_rlstDrw.end() );
		TDrawShapeList::iterator itObj;
   
		for( ; itSta != itEnd; ++itSta )
		 {
		   //(*itSta)->PreRemoveRelation( pObj );
		   if( pObj == (*itSta).get() ) itObj = itSta;
		 }

		pObj->DeGlueAll( m_bRedraw ? m_pDoc->GetMainViev():NULL );

		if( m_bRedraw )  pObj->Invalidate();

		(*itObj).release(); m_rlstDrw.erase( itObj );
		pObj->Remove();   		

		m_lstPC_Cnct.erase( itSta2++ );
	  }
	 else ++itSta2;
 }

void TAnalyzer::RemovePC( TPCConnect *pC )
 {
   TDrawShape *pObj = pC->m_pPC;
   int iId = pObj->ID();

   TDrawShapeList::iterator itSta( m_rlstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_rlstDrw.end() );
   TDrawShapeList::iterator itObj;
   
   for( ; itSta != itEnd; ++itSta )
	{
	  //(*itSta)->PreRemoveRelation( pObj );
	  if( pObj == (*itSta).get() ) itObj = itSta;
	}

   pObj->DeGlueAll( m_bRedraw ? m_pDoc->GetMainViev():NULL );

   if( m_bRedraw )  pObj->Invalidate();

   (*itObj).release(); m_rlstDrw.erase( itObj );
   pObj->Remove();   

   TRenovPCFnd dta( pC );
   TAP_TPCConnect_Lst::iterator it = find_if( m_lstPC_Cnct.begin(), m_lstPC_Cnct.end(), dta );
   ASSERT( it != m_lstPC_Cnct.end() );

   m_lstPC_Cnct.erase( it );
 }

void TAnalyzer::InvalidateCtype( TPCConnect* pC )
 {   
   LockConnect( pC, false, false );     

   //m_pPC->m_pTPC = NULL,
   //m_pPC->m_bCTypeValid = false;

   pC->m_cType = TCT_NotInit;

   pC->m_pats.m_lst.clear();
   pC->m_nIn = 0;
 }

void TAnalyzer::InvalidateCtypes()
 {
   TDrawShapeList::iterator itSta( m_rlstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_rlstDrw.end() );   
   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->isA() == TT_PC )
	   ((TDrawPC*)(*itSta).get())->m_bCTypeValid = false,
	   ((TDrawPC*)(*itSta).get())->m_pTPC = NULL;
 }

class TDataFindSbl
 {
public:
   TDataFindSbl( TPCConnect *pExclude, TDrawShape *pPtrTo )
	{
      m_pExclude = pExclude; m_pPtrTo = pPtrTo;
	  m_iCntSameRng = 0;
	}

   bool operator()( const TAP_TPCConnect& rA )
	{
	  if( rA.get() == m_pExclude ) return false;
      
	  list<TDrawShape*>::iterator itSta3( rA->m_pats.m_lst.begin() );
	  list<TDrawShape*>::iterator itEnd3( rA->m_pats.m_lst.end() );
	  for( ; itSta3 != itEnd3; ++itSta3 )
		if( *itSta3 == m_pPtrTo ) 
		 {
           if( rA->m_pPC->m_sRang == m_pExclude->m_pPC->m_sRang )
			 m_iCntSameRng++;
		   else return true;
		 }

	  return false;
	}

   TPCConnect *m_pExclude;
   TDrawShape *m_pPtrTo;
   int m_iCntSameRng;
 };

bool TAnalyzer::AnalyzeConnectType( bool bFlCreate, bool bFlEnblIsol )
 {
   TAP_TPCConnect_Lst::iterator itSta( m_lstPC_Cnct.begin() );
   TAP_TPCConnect_Lst::iterator itEnd( m_lstPC_Cnct.end() );

   bool bFlHaveIsol = false;
   if( bFlCreate )
	{
	  for( ; itSta != itEnd; ++itSta )
	    if( (*itSta)->m_cType == TCT_NotInit || !(*itSta)->m_pPC->m_bCTypeValid )
		 {
		   (*itSta)->Create( (*itSta)->m_pPC, NULL, true, bFlEnblIsol );			   
		   LockConnect( (*itSta).get(), (*itSta)->m_nIn > 0, false );		
		   if( !(*itSta)->m_nIn ) 
			 bFlHaveIsol = true,
			 InvalidateCtype( (*itSta).get() );
		 }

	  itSta = m_lstPC_Cnct.begin();
      itEnd = m_lstPC_Cnct.end();
	}

   if( bFlHaveIsol ) return false;
   
   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->m_cType == TCT_NotInit || !(*itSta)->m_pPC->m_bCTypeValid )
	  {        
        TDrawPC *pLnk1 = (*itSta)->GetLink1();
		TDrawPC *pLnk2 = (*itSta)->GetLink2();
		if( pLnk1 && pLnk2 ) (*itSta)->m_nIn = 2;
		else if( pLnk1 ) (*itSta)->m_nIn = 1;
		else if( (*itSta)->m_pPC->m_sRang != 1 || (*itSta)->m_pPC->m_sNum != 1 )
		 {
		   TGrafExc exc( TE_CanntGoBottom );
		   exc.m_iRng = (*itSta)->m_pPC->m_sRang;
		   exc.m_iNumb = (*itSta)->m_pPC->m_sNum;
		   throw exc;		   
		 }
		else
		 {
           (*itSta)->m_nIn = 1;
		   (*itSta)->m_cType = TCT_11;
		   (*itSta)->m_pPC->m_bCTypeValid = true;
		   continue;
		 }

		short iRng = (*itSta)->m_pPC->m_sRang;
		if( (*itSta)->m_nIn == 1 )
		 {
           TAP_TPCConnect_Lst::iterator itSta2( m_lstPC_Cnct.begin() );
		   TAP_TPCConnect_Lst::iterator itEnd2( m_lstPC_Cnct.end() );
		   bool bFlHaveOtherRng = false;
		   bool bFlHaveThisRng = false;
		   bool bFlHaveThisRng_Bi = false;
		   for( ; itSta2 != itEnd2; ++itSta2 ) 
			 if( (*itSta2).get() != (*itSta).get() &&
			  
                 ((*itSta2)->GetLink1() == pLnk1 ||
				 (*itSta2)->GetLink2() == pLnk1)
			   )
			  {
                if( (*itSta2)->m_pPC->m_sRang == iRng )
				 {
                   if( (*itSta2)->m_nIn == 1 ) bFlHaveThisRng = true;
                   else bFlHaveThisRng_Bi = true;
				 }
				else bFlHaveOtherRng = true;
			  }			  

			if( bFlHaveOtherRng == false && bFlHaveThisRng == true 			    
			  )
			  (*itSta)->m_cType = TCT_Parall;
			else if( bFlHaveOtherRng == false && bFlHaveThisRng == false &&
			    bFlHaveThisRng_Bi == true
			  )
			  (*itSta)->m_cType = TCT_NotDeterm;
			else if( bFlHaveOtherRng == true )
			  (*itSta)->m_cType = TCT_NotDeterm;
			else 
			  (*itSta)->m_cType = TCT_Seqv;

			(*itSta)->m_pPC->m_bCTypeValid = true;
		 }
		else
		 {
		   list<TDrawShape*>::iterator itSta3( (*itSta)->m_pats.m_lst.begin() );
		   list<TDrawShape*>::iterator itEnd3( (*itSta)->m_pats.m_lst.end() );
		   bool bFlFound1 = false;
		   for( ; itSta3 != itEnd3; ++itSta3 )
			 if( (*itSta3)->isA() == TT_PC )
			  {
			    TDataFindSbl dta( (*itSta).get(), *itSta3 );
                TAP_TPCConnect_Lst::iterator it = find_if( m_lstPC_Cnct.begin(), m_lstPC_Cnct.end(), dta );
				if( it != m_lstPC_Cnct.end() ) 
				 {                   
                   (*itSta)->m_cType = TCT_NotDeterm;
				   (*itSta)->m_pPC->m_bCTypeValid = true;
				   bFlFound1 = true;
                   break;
				 }
			  }

		   if( !bFlFound1 )
			{
			   TAP_TPCConnect_Lst::iterator itSta2( m_lstPC_Cnct.begin() );
			   TAP_TPCConnect_Lst::iterator itEnd2( m_lstPC_Cnct.end() );
			   
			   bool bFlFound = false;
			   for( ; itSta2 != itEnd2; ++itSta2 ) 
				 if( (*itSta2).get() != (*itSta).get() &&
				  
					 (((*itSta2)->GetLink1() == pLnk1 &&
					  (*itSta2)->GetLink2() == pLnk2) ||
					  ((*itSta2)->GetLink1() == pLnk2 &&
					  (*itSta2)->GetLink2() == pLnk1))
				   )
				  {
					if( (*itSta2)->m_pPC->m_sRang == iRng )
					 {
					   bFlFound = true;
					   break;
					 }				
				  }			  

			   if( bFlFound )
				 (*itSta)->m_cType = TCT_FullAccess;
			   else 
				{ 
				  (*itSta)->m_cType = TCT_BiIn;
				  if( DetectTopSofist((*itSta).get()) )
					(*itSta)->m_cType = TCT_BiInSofist;
				}

			   (*itSta)->m_pPC->m_bCTypeValid = true;
			}
		 }
	  }

	return true;
 }

class TFindIntSibling
 {
public:
   TFindIntSibling( short sRng, short sNum )
	{
	  m_rng = sRng;
	  m_num = sNum;
	}

   bool operator()( const TAP_TPCConnect& rA )
	{
      return rA->m_pPC->isA() == TT_PC && rA->m_pPC->m_sRang == m_rng &&
	    rA->m_pPC->m_sNum == m_num;
	}

   short m_rng, m_num;
 };

bool TAnalyzer::DetectTopSofist( TPCConnect* pC )
 {
   TDoublePolus::ResetUsedFlag( m_rlstDrw );

   pC->m_lstInt.clear();

   TPCConnect cnctThis( m_iListSz );
   cnctThis.Create( pC->m_pPC, m_p11, false );
   int iRng = pC->m_pPC->m_sRang;

   TAP_TPCConnect_Lst lstPC_Cnct;

   TAP_TPCConnect_Lst::iterator itSta( m_lstPC_Cnct.begin() );
   TAP_TPCConnect_Lst::iterator itEnd( m_lstPC_Cnct.end() );
   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta).get() != pC && (*itSta)->m_pPC->m_sRang == iRng )
	  {
        TAP_TPCConnect ap( new TPCConnect(m_iListSz) );
		ap->Create( (*itSta)->m_pPC, m_p11, false );
		lstPC_Cnct.push_back( ap );
	  }

   pC->m_N = iRng;
   itSta = lstPC_Cnct.begin();
   itEnd = lstPC_Cnct.end();
   int iMax = -1;
   pC->m_pInt = NULL;
   for( ; itSta != itEnd; ++itSta )
	{
      TDrawPC *p = (*itSta)->m_pats.FindIntersectMax( cnctThis.m_pats );
	  if( p && p->m_sRang > iMax )
	    iMax = p->m_sRang,
		pC->m_pInt = p;
	}

   pC->m_M = iMax;

   bool bFlRes = pC->m_N > pC->m_M && pC->m_M != -1;
   if( bFlRes )
	{	  
	  TFindIntSibling dta( pC->m_pPC->m_sRang, pC->m_pPC->m_sNum - 1 );
	  TAP_TPCConnect_Lst::iterator it = find_if( m_lstPC_Cnct.begin(), m_lstPC_Cnct.end(), dta );
	  if( it == m_lstPC_Cnct.end() )
	   {
	     TFindIntSibling dta( pC->m_pPC->m_sRang, pC->m_pPC->m_sNum + 1 );
	     it = find_if( m_lstPC_Cnct.begin(), m_lstPC_Cnct.end(), dta );
	   }

      //TDrawPC *pInt = pC->m_pInt;
	  TAP_TPCConnect_Lst::iterator itInt = GetItRMaxN( pC->m_pInt->m_sRang );
	  for( short sNum = (*itInt)->m_pPC->m_sNum; sNum > 0; --sNum )
	   {
	     TFindIntSibling dta( pC->m_pInt->m_sRang, sNum );
         TAP_TPCConnect_Lst::iterator itVar = find_if( m_lstPC_Cnct.begin(), m_lstPC_Cnct.end(), dta );
		 if( itVar == m_lstPC_Cnct.end() ) continue;

		 TPCConnect cnctThisToInt( m_iListSz );
		 TPCConnect cnctThisToInt2( m_iListSz );
		 try {
		   cnctThisToInt.Create1( pC->m_pPC, (*itVar)->m_pPC );		 
		   cnctThisToInt2.Create1( (*it)->m_pPC, (*itVar)->m_pPC );
		  }
		 catch( TGrafExc ex )
		  {
		    if( ex.m_exc == TE_CanntGoTo ) continue;
			throw;
		  }

		 pC->m_lstInt.merge( cnctThisToInt.m_pats.m_lst );
		 pC->m_lstInt.merge( cnctThisToInt2.m_pats.m_lst );
		 My_unique( pC->m_lstInt );
		 return true;
	   }
	  
	  TGrafExc exc( TE_CanntCreInt );
	  exc.m_iRng = pC->m_pPC->m_sRang;
	  exc.m_iNumb = pC->m_pPC->m_sNum;
	  throw exc;		   
	}

   return bFlRes;
 }

class TCnctFnd_Dta_Parall
 {
public:
   TCnctFnd_Dta_Parall( TPCConnect* pC )
	{
	  m_pC = pC;
	  m_pLnk = pC->GetLink1();
	}
   bool operator()( const TAP_TPCConnect& rC )
	{
      return rC.get() != m_pC && 
	         rC->m_pPC->m_sRang == m_pC->m_pPC->m_sRang && 
			 rC->m_nIn == 1 &&
			 m_pLnk == rC->GetLink1();
	}
TPCConnect* m_pC;
TDrawPC *m_pLnk;
 };

class TCnctFnd_Dta_FAcc
 {
public:
   TCnctFnd_Dta_FAcc( TPCConnect* pC )
	{
	  m_pC = pC;
	  m_pLnk1 = pC->GetLink1();
	  m_pLnk2 = pC->GetLink2();
	}
   bool operator()( const TAP_TPCConnect& rC )
	{
      return rC.get() != m_pC && 
	         rC->m_pPC->m_sRang == m_pC->m_pPC->m_sRang && 
			 rC->m_nIn == 2 &&
			 (m_pLnk1 == rC->GetLink1() &&
			  m_pLnk2 == rC->GetLink2() ||

			  m_pLnk1 == rC->GetLink2() &&
			  m_pLnk2 == rC->GetLink1());
	}
TPCConnect* m_pC;
TDrawPC *m_pLnk1, *m_pLnk2;
 };

class TCnctFnd_Dta_NDAcc
 {
public:
   TCnctFnd_Dta_NDAcc( TPCConnect* pC )
	{
	  m_pC = pC;
	  m_pLnk = pC->GetLink1();
	}
   bool operator()( const TAP_TPCConnect& rC1, const TAP_TPCConnect& rC2 )
	{
      /*return rC.get() != m_pC && 
	         rC->m_pPC->m_sRang != m_pC->m_pPC->m_sRang && 
			 (rC->m_nIn == 1 &&
			  m_pLnk == rC->GetLink1() ||
			  (rC->m_nIn == 2 &&
			   (m_pLnk == rC->GetLink1() ||
			    m_pLnk == rC->GetLink2()
			  )));			  			  */
	  if( rC1.get() == m_pC || rC1->m_pPC->m_sRang == m_pC->m_pPC->m_sRang ||
	      (rC1->GetLink1() != m_pLnk && rC1->GetLink2() != m_pLnk)
		)
	    return true;
	  if( rC2.get() == m_pC || rC2->m_pPC->m_sRang == m_pC->m_pPC->m_sRang ||
	      (rC2->GetLink1() != m_pLnk && rC2->GetLink2() != m_pLnk)
		)
	    return false;

	  if( rC1->m_pPC->m_sRang == rC2->m_pPC->m_sRang ) 
	    return rC1->m_pPC->m_sNum < rC2->m_pPC->m_sNum;
	  return  rC1->m_pPC->m_sRang < rC2->m_pPC->m_sRang;
	}
TPCConnect* m_pC;
TDrawPC *m_pLnk;
 };


TPCConnect *TAnalyzer::GetSibling( TPCConnect* pC )
 {
   switch( pC->m_cType )
	{
	  case TCT_Parall:	  
	   {
	     TCnctFnd_Dta_Parall dta( pC );
	     TAP_TPCConnect_Lst::iterator it = find_if( m_lstPC_Cnct.begin(), m_lstPC_Cnct.end(), dta );
		 if( it == m_lstPC_Cnct.end() ) return NULL;
		 return (*it).get();
	   }
	  case TCT_FullAccess:
	   {
	     TCnctFnd_Dta_FAcc dta( pC );
	     TAP_TPCConnect_Lst::iterator it = find_if( m_lstPC_Cnct.begin(), m_lstPC_Cnct.end(), dta );
		 if( it == m_lstPC_Cnct.end() ) return NULL;
		 return (*it).get();
	   }
	  case TCT_Seqv:
	   {
	    //return pC->m_pPC->m_pTPC;	  
	     TDrawPC *p = pC->GetLink1();
		 return p ? p->m_pTPC:NULL;
	   }
	  case TCT_NotDeterm:
	   {
	     TCnctFnd_Dta_NDAcc dta( pC );
	     TAP_TPCConnect_Lst::iterator it = max_element( m_lstPC_Cnct.begin(), m_lstPC_Cnct.end(), dta );
		 if( it == m_lstPC_Cnct.end() ) return NULL;
		 return (*it).get();
	   }

	  case TCT_BiIn:
	  case TCT_BiInSofist:
	  case TCT_11:
	    return NULL;
	}

   return NULL;
 }

TAnalyzer::TAnalyzer( TDrawShapeList &rLst, TDiplomDoc *pDoc ):
  m_rlstDrw( rLst ),
  m_pDoc( pDoc )
 {
   m_iPECnt = 1;
   m_bRedraw = true;
   //m_pOwner = pOwner;
   m_iListSz = TDiplomDoc::CalcPCAndPoints( m_rlstDrw );
   m_p11 = (TDrawPC*)TDoublePolus::FindSta( m_rlstDrw );
 }

void TAnalyzer::UnlockAll()
 {
   TDrawShapeList::iterator itSta( m_rlstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_rlstDrw.end() );   
        
   for( ; itSta != itEnd; ++itSta )
	 (*itSta)->ResLockUsed();
 }

void TAnalyzer::RemoveLink( TDrawShape* pObj )
 {
   ASSERT( pObj->isA() == TT_Cnct || pObj->isA() == TT_Point );

   int iId = pObj->ID();

   TDrawShapeList::iterator itSta( m_rlstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_rlstDrw.end() );
   TDrawShapeList::iterator itObj;
   
   for( ; itSta != itEnd; ++itSta )
	{
	  //(*itSta)->PreRemoveRelation( pObj );
	  if( pObj == (*itSta).get() ) itObj = itSta;
	}

   pObj->DeGlueAll( m_bRedraw ? m_pDoc->GetMainViev():NULL );
   if( m_bRedraw ) pObj->Invalidate();

   (*itObj).release(); m_rlstDrw.erase( itObj );
   
   if( pObj->isA() == TT_Point )
	{
	  TAP_TPCConnect_Lst::iterator itSta( m_lstPC_Cnct.begin() );
	  TAP_TPCConnect_Lst::iterator itEnd( m_lstPC_Cnct.end() );
	  for( ; itSta != itEnd; ++itSta )
	   {
		 list<TDrawShape*>::iterator it = 
		   find( (*itSta)->m_pats.m_lst.begin(), (*itSta)->m_pats.m_lst.end(), pObj );
		 if( it != (*itSta)->m_pats.m_lst.end() )
		   (*itSta)->m_pats.m_lst.erase( it );
	   }
	}

   pObj->Remove();
 }

void TAnalyzer::LockConnect_For( short iHi, short iLo, bool bFl, bool bFlRemov )
 {
   TAP_TPCConnect_Lst::iterator itSta( m_lstPC_Cnct.begin() );
   TAP_TPCConnect_Lst::iterator itEnd( m_lstPC_Cnct.end() );
   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->m_pPC->m_sRang <= iHi && (*itSta)->m_pPC->m_sRang >= iLo )
	   LockConnect( (*itSta).get(), bFl, bFlRemov );
 }

void TAnalyzer::LockConnect( TPCConnect *pC, bool bFl, bool bFlRemov )
 {
   //pC->ResetUsed();
   pC->m_pats.m_lst.push_front( pC->m_pPC );

   list<TDrawShape*>::iterator itSta( pC->m_pats.m_lst.begin() );
   list<TDrawShape*>::iterator itEnd( pC->m_pats.m_lst.end() );      
   itEnd--;      
   
   for( ; itSta != itEnd; ++itSta )
	{
      list<TDrawShape*>::iterator itSta2( itSta );	  
      list<TDrawShape*>::iterator itEnd2( pC->m_pats.m_lst.end() );   	  

	  itSta2++;	  

	  for( ; itSta2 != itEnd2; ++itSta2 )
	   {	     		 
		 TCnct *pCnct = (*itSta)->GetLinkTo( *itSta2 );
		 if( pCnct ) 
		  {
            pCnct->LockUsed( bFl );
			if( bFlRemov && !bFl && !pCnct->GetLocked() ) RemoveLink( pCnct );
		  }
	   }	  
	}

   pC->m_pats.m_lst.erase( pC->m_pats.m_lst.begin() );

   itSta = pC->m_pats.m_lst.begin();
   itEnd = pC->m_pats.m_lst.end(); 
   for( ; itSta != itEnd; )
	 if( (*itSta)->isA() == TT_Point )
	  {
	    list<TDrawShape*>::iterator it = itSta++;
        (*it)->LockUsed( bFl );
		if( bFlRemov && !bFl && !(*it)->GetLocked() ) RemoveLink( *it );
	  }
	 else ++itSta;
 }

void TPCConnect::ResetUsed()
 {
   list<TDrawShape*>::iterator itSta( m_pats.m_lst.begin() );
   list<TDrawShape*>::iterator itEnd( m_pats.m_lst.end() );
   for( ;itSta != itEnd; ++itSta )
	 (*itSta)->ResUsedFlag();
 }

TPCConnect *TAnalyzer::FirstSimpl()
 {
   TAP_TPCConnect_Lst::iterator itSta( m_lstPC_Cnct.begin() );
   TAP_TPCConnect_Lst::iterator itEnd( m_lstPC_Cnct.end() );
   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->m_cType != TCT_BiIn ) return (*itSta).get();	 

   return NULL;
 }

void TAnalyzer::RemoveAllNotUsed()
 {
   TDrawShapeList::iterator itSta( m_rlstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_rlstDrw.end() );
   for( ; itSta != itEnd; )
	 if( !(*itSta)->GetLocked() )
	  {
        if( (*itSta)->isA() != TT_Point && (*itSta)->isA() != TT_Cnct )
		 {
		   ++itSta;
		   continue;
		 }
		RemoveLink( (*itSta++).get() );
	  }
	 else ++itSta;
 }

TDrawPC* TPCConnect::RootInt()
 {
   list<TDrawShape*>::iterator itSta( m_lstInt.begin() );
   list<TDrawShape*>::iterator itEnd( m_lstInt.end() );
   short sMinRng = SHRT_MAX;
   TDrawPC *p = NULL;
   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->isA() == TT_PC )
	  {
        if( ((TDrawPC*)*itSta)->m_sRang < sMinRng )
		  sMinRng = ((TDrawPC*)*itSta)->m_sRang,
		  p = (TDrawPC*)*itSta;
	  }

   return p;
 }

LPCSTR TPCConnect::CnctDescr()
 {
   switch( m_cType )
	{
	  case TCT_NotInit:
	    return "Не инициализированное";
	  case TCT_Parall:
	    return "Параллельное";
	  case TCT_Seqv:
	    return "Последовательное";
	  case TCT_FullAccess:
	    return "Полнодоступное";
	  case TCT_NotDeterm:
	    return "Неопределённое";
	  case TCT_BiIn:
	    return "Много-входовое";
	  case TCT_BiInSofist:
	    return "Много-входовое топологически сложное";
	  case TCT_11:
	    return "Корневое";

	  default:
	    return "Ошибка";
	};
 }


void TPCConnect::CreDPFromLst( list<TDrawShape*>& rL, TDrawShapeList &rOut, TDiplomDoc *pOwn )
 {
   list<TDrawShape*>::iterator itSta( rL.begin() );
   list<TDrawShape*>::iterator itEnd( rL.end() );
   int iMin = INT_MAX;
   for( ; itSta != itEnd; ++itSta )
	{
      TAuto_DrawShape ap( NULL );

	  if( (*itSta)->isA() == TT_PC )
	   {
		 ap = TAuto_DrawShape( new TDrawPC((TDrawPC&)**itSta) );
		 iMin = min( iMin, ((TDrawPC*)(*itSta))->m_sRang );
	   }
	  else if( (*itSta)->isA() == TT_Point )
		ap = TAuto_DrawShape(new TDrawPoint((TDrawPoint&)**itSta) );
	  else continue;

	  ap->m_pDocument = pOwn;
	  ap->CreateIndex( (TIndexedList&)rOut );
	  ap->SetScaled( pOwn->m_dSc );
	  rOut.push_back( ap );	  
	}	

   itSta = rL.begin();
   itEnd = rL.end();
   itEnd--;      
   TDrawShapeList::iterator itSta_( rOut.begin() );
   TDrawShapeList::iterator itEnd_( rOut.end() );
   itEnd_--;

   TDrawShapeList lstTmp;

   int iIndAdd = 1;
   
   for( ; itSta != itEnd; ++itSta, ++itSta_ )
	{
      list<TDrawShape*>::iterator itSta2( itSta );	  
      list<TDrawShape*>::iterator itEnd2( rL.end() );   
	  TDrawShapeList::iterator itSta2_( itSta_ );
      TDrawShapeList::iterator itEnd2_( rOut.end() );	  

	  itSta2++; itSta2_++;
	  
	  
	  for( ; itSta2 != itEnd2; ++itSta2, ++itSta2_ )
	   {	     		 
         TDrawPC *pp1 = (TDrawPC*)(*itSta);
		 TDrawPC *pp2 = (TDrawPC*)(*itSta2);
		 TDrawPC *pp1_ = (TDrawPC*)(*itSta_).get();
		 TDrawPC *pp2_ = (TDrawPC*)(*itSta2_).get();

		 TCnct *pCnct = (*itSta)->GetLinkTo( *itSta2 );
		 if( pCnct ) 
		  {
		    TAuto_DrawShape ap( new TCnct(TO_Cnct_Simple) );
			TCnct *pCC = (TCnct*)ap.get();
			ap->m_pDocument = pOwn;
			ap->CreateIndex( (TIndexedList&)rOut );
			ap->ShiftUnic( ++iIndAdd );
			ap->SetScaled( pOwn->m_dSc );
			//rOut.push_back( ap );	  
			lstTmp.push_back( ap );
			

			TGlue *pGl = pCnct->m_vecGlu[0].get();
			bool bFlM;
			TDrawShape* pShp = pGl->m_lstGlu.front().m_pShp == *itSta ? (*itSta_).get():(*itSta2_).get();
			pCC->MakeGlue( 1, 
			  pShp,
			  pGl->m_lstGlu.front().m_nGlu, bFlM );
			 //int nGlu, TDrawShape* pShp, int nGluForin, bool& rMade )

			pGl = pCnct->m_vecGlu[1].get();
			pCC->MakeGlue( 2, 
			  pGl->m_lstGlu.front().m_pShp == *itSta ? (*itSta_).get():(*itSta2_).get(),
			  pGl->m_lstGlu.front().m_nGlu, bFlM );						
		  }
	   }
	}

   rOut.splice( rOut.end(), lstTmp );

   itSta_ = rOut.begin();
   itEnd_ = rOut.end();
   --iMin;
   for( ; itSta_ != itEnd_; ++itSta_ )
     if( (*itSta_)->isA() == TT_PC )
	  {
	    (*itSta_)->RecalcGlued( NULL, true );
	    ((TDrawPC*)(*itSta_).get())->m_sRang -= iMin;
		if( ((TDrawPC*)(*itSta_).get())->m_sRang == 1 )
		  ((TDrawPC*)(*itSta_).get())->m_sNum = 1;
	  }
	 else if( (*itSta_)->isA() == TT_Point )
	   (*itSta_)->RecalcGlued( NULL, true );
 }

TPCConnect *TAnalyzer::GetRoot()
 {
   TAP_TPCConnect_Lst::iterator itSta( m_lstPC_Cnct.begin() );
   TAP_TPCConnect_Lst::iterator itEnd( m_lstPC_Cnct.end() );
   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->m_pPC->m_sRang == 1 )	     
	    return (*itSta).get();

   return NULL;
 }
TPCConnect *TAnalyzer::GetNoRoot()
 {
   TAP_TPCConnect_Lst::iterator itSta( m_lstPC_Cnct.begin() );
   TAP_TPCConnect_Lst::iterator itEnd( m_lstPC_Cnct.end() );
   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->m_pPC->m_sRang != 1 )	     
	    return (*itSta).get();

   return NULL;
 }

class TFndRngDta
 {
public:
   TFndRngDta( short sRng )
	{
	  m_sRng = sRng;
	}
   bool operator()( const TAP_TPCConnect& rA )
	{
      return rA->m_pPC->m_sRang == m_sRng;
	}

   short m_sRng;
 };

TAP_TPCConnect_Lst::iterator TAnalyzer::GetItRMaxN( short sRng )
 {
   TFndRngDta dta( sRng );
   return find_if( m_lstPC_Cnct.begin(), m_lstPC_Cnct.end(), dta );
 }

void TAnalyzer::MakeRect( TDrawPC *p1, TDrawPC *p2, CRect& rNew )
 {
   rNew.left = min( p1->m_position.left, p2->m_position.left );
   rNew.right = max( p1->m_position.right, p2->m_position.right );
   rNew.top = max( p1->m_position.top, p2->m_position.top );
   rNew.bottom = min( p1->m_position.bottom, p2->m_position.bottom );
 }

void TAnalyzer::MkConnectFromTo( TDrawPC *p1, TDrawPC *p2 )
 {
   TAuto_DrawShape ap( new TCnct(TO_Cnct_Simple) );
   TCnct *pCC = (TCnct*)ap.get();
   
   m_pDoc->Add( pCC, m_rlstDrw );
   ap.release();
   /*ap->m_pDocument = m_pDoc;
   ap->CreateIndex( (TIndexedList&)m_rlstDrw );   
   ap->SetScaled( m_pDoc->m_dSc );   
   lstTmp.push_back( ap );*/   

   bool bFl;
   p1->m_bEnableMultiIn = true;
   pCC->MakeGlue( 1, p1, 2, bFl );
   p1->m_bEnableMultiIn = false;
   p2->m_bEnableMultiIn = true;
   pCC->MakeGlue( 2, p2, 1, bFl );	 
   p2->m_bEnableMultiIn = false;
 }

void TAnalyzer::RecalcGluesFT( short rHi, short rLo )
 {
   TAP_TPCConnect_Lst::iterator itSta( m_lstPC_Cnct.begin() );
   TAP_TPCConnect_Lst::iterator itEnd( m_lstPC_Cnct.end() );
   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->m_pPC->m_sRang <= rHi && 
	     (*itSta)->m_pPC->m_sRang >= rLo
	   )	  	    
        (*itSta)->m_pPC->RecalcGlued( m_bRedraw ? m_pDoc->GetMainViev():NULL, true );
 }

short TAnalyzer::FindLoRng( TDoublePolus &rDP, short sRng )
 {
   list<TDrawShape*>::iterator itSta( rDP.m_lstPC.begin() );
   list<TDrawShape*>::iterator itEnd( rDP.m_lstPC.end() );   
   short sRes = SHRT_MAX;
   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->isA() == TT_PC && ((TDrawPC*)*itSta)->m_sRang > sRng )
	  {
	    if( ((TDrawPC*)*itSta)->m_sRang < sRes ) 
		  sRes = ((TDrawPC*)*itSta)->m_sRang;
	  }

   return sRes;
 }

short TAnalyzer::LockConnect_ForDP( TDoublePolus &rDP, short sRng )
 {
   list<TDrawShape*>::iterator itSta( rDP.m_lstPC.begin() );
   list<TDrawShape*>::iterator itEnd( rDP.m_lstPC.end() );   
   short sRes = SHRT_MAX;
   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->isA() == TT_PC && ((TDrawPC*)*itSta)->m_sRang > sRng )
	  {
        if( !((TDrawPC*)*itSta)->m_bCTypeValid || !((TDrawPC*)*itSta)->m_pTPC )
		 {
           TGrafExc exc( TE_IllegalInvalidPtr );
		   exc.m_iRng = ((TDrawPC*)*itSta)->m_sRang;
		   exc.m_iNumb = ((TDrawPC*)*itSta)->m_sNum;
		   throw exc;		   
		 }

		TPCConnect *p = ((TDrawPC*)*itSta)->m_pTPC;

		LockConnect( p, false, true );

		if( ((TDrawPC*)*itSta)->m_sRang < sRes ) 
		  sRes = ((TDrawPC*)*itSta)->m_sRang;
	  }

   return sRes;
 }

void TAnalyzer::InvalidateDP_WhatAndRemov( TDoublePolus &rDP, short sRng )
 {
   list<TDrawShape*>::iterator itSta( rDP.m_lstPC.begin() );
   list<TDrawShape*>::iterator itEnd( rDP.m_lstPC.end() );   
   
   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->isA() == TT_PC && ((TDrawPC*)*itSta)->m_sRang >= sRng )
	  {
        if( !((TDrawPC*)*itSta)->m_bCTypeValid || !((TDrawPC*)*itSta)->m_pTPC )
		 {
           TGrafExc exc( TE_IllegalInvalidPtr );
		   exc.m_iRng = ((TDrawPC*)*itSta)->m_sRang;
		   exc.m_iNumb = ((TDrawPC*)*itSta)->m_sNum;
		   throw exc;		   
		 }

		TPCConnect *p = ((TDrawPC*)*itSta)->m_pTPC;

        InvalidateCtype_What( p, NULL ); 		
		if( ((TDrawPC*)*itSta)->m_sRang > sRng ) RemovePC( p );
	  }
 }

void TAnalyzer::MakeRectForRang( list<TDrawShape*>& rL, short sRng, CRect& rR )
 {
   list<TDrawShape*>::iterator itSta( rL.begin() );
   list<TDrawShape*>::iterator itEnd( rL.end() );
   int iXMin = INT_MAX, iXMax = INT_MIN,
	   iYMin = INT_MAX, iYMax = INT_MIN;
   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->isA() == TT_PC && ((TDrawPC*)*itSta)->m_sRang == sRng )
	   iXMin = min( iXMin, (*itSta)->m_position.left ),
	   iXMax = max( iXMax, (*itSta)->m_position.right ),
	   iYMin = min( iYMin, (*itSta)->m_position.bottom ),
	   iYMax = max( iYMax, (*itSta)->m_position.top );

   rR.SetRect( iXMin, iYMax, iXMax, iYMin );
 }

TPCConnect *TAnalyzer::AttachNewToDP( list<TDrawShape*>& rL, short sAssRng, TParamCalc& rAssDta )
 {
   TDrawPC *p = new TDrawPC( (TDrawPC&)*rL.front() );
   p->m_iSelCount = 0;
   p->SetSel_Internal( false );
   p->m_sRang = sAssRng;
   p->m_sNum = 1;
   
   m_pDoc->Add( p, m_rlstDrw );

   CRect rNew; 
   TAnalyzer::MakeRectForRang( rL, ((TDrawPC*)rL.front())->m_sRang, rNew );
   CPoint ptCC = rNew.CenterPoint();
   ptCC.y = rNew.bottom - 2*abs( p->m_position.Height() );

   TAP_TPCConnect ap( new TPCConnect(m_iListSz) );
   //ap->Create( p, NULL, true );
   TPCConnect *pCC = ap.get();
   m_lstPC_Cnct.push_back( ap );			
   pCC->m_pPC = p;
   p->m_pTPC = pCC;
   TAnalyzer::AssParamPE( pCC, rAssDta );
   //pAnalyzer->CenterInRect( pCC, rNew );
   p->m_position.OffsetRect( ptCC - p->m_position.CenterPoint() );
   NextPE( pCC );
   p->m_pTPC = NULL;

   list<TDrawShape*>::iterator itSta( rL.begin() );
   list<TDrawShape*>::iterator itEnd( rL.end() );

   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->isA() == TT_PC && ((TDrawPC*)*itSta)->m_sRang == ((TDrawPC*)rL.front())->m_sRang )
	  {
	    MkConnectFromTo( (TDrawPC*)*itSta, p );
		((TDrawPC*)*itSta)->RecalcGlued( m_bRedraw ? m_pDoc->GetMainViev():NULL, true );
	  }

   p->RecalcGlued( m_bRedraw ? m_pDoc->GetMainViev():NULL, true );

   return pCC;
 }

TPCConnect* TAnalyzer::AttachTo( TPCConnect *pCC, short sAssRng, TParamCalc& rAssDta )
 {
   TDrawPC *p = new TDrawPC( *pCC->m_pPC );
   p->m_iSelCount = 0;
   p->SetSel_Internal( false );
   p->m_sRang = sAssRng;
   p->m_sNum = 1;
   
   m_pDoc->Add( p, m_rlstDrw );

   CRect rNew( pCC->m_pPC->m_position );   
   CPoint ptCC = rNew.CenterPoint();
   ptCC.y -= 2*abs( p->m_position.Height() );

   TAP_TPCConnect ap( new TPCConnect(m_iListSz) );
   //ap->Create( p, NULL, true );
   TPCConnect *pCCNew = ap.get();
   m_lstPC_Cnct.push_back( ap );			
   pCCNew->m_pPC = p;
   p->m_pTPC = pCCNew;
   TAnalyzer::AssParamPE( pCCNew, rAssDta );
   //pAnalyzer->CenterInRect( pCC, rNew );
   p->m_position.OffsetRect( ptCC - p->m_position.CenterPoint() );
   NextPE( pCCNew );
   p->m_pTPC = NULL;
   

   MkConnectFromTo( pCC->m_pPC, p );
   p->RecalcGlued( m_bRedraw ? m_pDoc->GetMainViev():NULL, true );
   pCC->m_pPC->RecalcGlued( m_bRedraw ? m_pDoc->GetMainViev():NULL, true );

   return pCCNew;
 }

void TAnalyzer::FlipTo( list<TCnct*>& rL, TPCConnect* pC )
 {
   list<TCnct*>::iterator itSta( rL.begin() );
   list<TCnct*>::iterator itEnd( rL.end() );

   bool bFl;
   for( ; itSta != itEnd; ++itSta )
	 if( !(*itSta)->m_vecGlu[0]->m_lstGlu.size() ||
	     !(*itSta)->m_vecGlu[0]->m_lstGlu.front().IsGlued()
	   )
	    (*itSta)->MakeGlue( 1, pC->m_pPC, 2, bFl );	   	  
	 else
	   if( !(*itSta)->m_vecGlu[1]->m_lstGlu.size() ||
	     !(*itSta)->m_vecGlu[1]->m_lstGlu.front().IsGlued()
	   )
	    (*itSta)->MakeGlue( 2, pC->m_pPC, 2, bFl );	   	  

   pC->m_pPC->RecalcGlued( m_bRedraw ? m_pDoc->GetMainViev():NULL, true );
 }

void TAnalyzer::CreFlipPCLst( TDoublePolus& rDP, list<TPCConnect*>& rL, short sRng )
 {
   TAP_TPCConnect_Lst::iterator itSta( m_lstPC_Cnct.begin() );
   TAP_TPCConnect_Lst::iterator itEnd( m_lstPC_Cnct.end() );           

   for( ; itSta != itEnd; ++itSta )   
	 //if( !(*itSta)->m_pPC->IsInBiPolus(rDP.m_iNum) )
	 if( !(*itSta)->m_pPC->m_vecBiPolus.size() )
	  {
	    TDrawPC *p1 = (*itSta)->GetLink1(),
		        *p2 = (*itSta)->GetLink2();
        if( p1 && p1->m_sRang > sRng && p1->IsInBiPolus(rDP.m_iNum) || 
		    p2 && p2->m_sRang > sRng && p2->IsInBiPolus(rDP.m_iNum)
		  )
	        rL.push_back( (*itSta).get() );	    
	  }
 }

void TAnalyzer::FlipPCTo( list<TPCConnect*>& rL, TPCConnect* pCC )
 {
   list<TPCConnect*>::iterator itSta( rL.begin() );
   list<TPCConnect*>::iterator itEnd( rL.end() );

   for( ; itSta != itEnd; ++itSta )
	{
	  MkConnectFromTo( pCC->m_pPC, (*itSta)->m_pPC );
	  (*itSta)->m_pPC->RecalcGlued( m_bRedraw ? m_pDoc->GetMainViev():NULL, true );
	}

   pCC->m_pPC->RecalcGlued( m_bRedraw ? m_pDoc->GetMainViev():NULL, true );
 }

void TMultiDPSimplfy::DetectIntersect( list<TDrawShape*>& rlOut, TAP_Ms_Lst& rL )
 {   
   rL.front()->m_apDP->Intersect( *(*++rL.begin())->m_apDP.get(), rlOut );
   if( rL.size() < 3 ) return;

   TAP_Ms_Lst::iterator itSta( rL.begin() ); 
   TAP_Ms_Lst::iterator itEnd( rL.end() );
   advance( itSta, 2 );
   
   for( ; itSta != itEnd; ++itSta )
	 (*itSta)->m_apDP->Intersect2( rlOut );
 }

template<> struct std::greater<TAP_Ms>
 {      
   bool operator()( const TAP_Ms& a1, const TAP_Ms& a2 ) const
	{	 	  
	  if( m_sSrt == 0 )
	   {
		 if( a1->m_sPRang == a2->m_sPRang )
		   return a1->m_sPNum > a2->m_sPNum;
		 else
		   return a1->m_sPRang > a2->m_sPRang;	 
	   }
	  else// if( m_sSrt == 1 )
	   {
         return a1->m_sMinRem < a2->m_sMinRem;
	   }
	}   

   short m_sSrt;
 };

void TMultiDPSimplfy::Srt( TAP_Ms_Lst& rL )
 {
   std::greater<TAP_Ms> mgr;    
   mgr.m_sSrt = 0;
   rL.sort( mgr );
 }

void TMultiDPSimplfy::Srt_Remainder( TAP_Ms_Lst& rL )
 {
   std::greater<TAP_Ms> mgr;    
   mgr.m_sSrt = 1;
   rL.sort( mgr );
 }
