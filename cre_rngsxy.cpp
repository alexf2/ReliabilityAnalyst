#include "diplomDoc.h"


template<> struct std::greater<class TDrawPC*>
 {   
   bool operator()( const TDrawPC*& a1, const TDrawPC*& a2 ) const
	{	  
	  switch( m_shClass )
	   {
		 case 1:
		   return Cmp1( a1, a2 );
		 case 2:
		   return Cmp2( a1, a2 );
		 case 3:
		   return Cmp3( a1, a2 );
	   }
	  return true;
	}   
   
   bool Cmp1( const TDrawPC*& a1, const TDrawPC*& a2 ) const
	{
	  if( (a1->m_position.top >= a2->m_position.top && a1->m_position.bottom <= a2->m_position.top) ||
	      (a1->m_position.top > a2->m_position.bottom && a1->m_position.bottom < a2->m_position.bottom)
		)
	    return  a1->m_position.left < a2->m_position.left;
	  else return a1->m_position.top > a2->m_position.top;
	}

   bool Cmp2( const TDrawPC*& a1, const TDrawPC*& a2 ) const
	{
	  if( a1->m_sRang == a2->m_sRang ) 
		return  a1->m_sNum < a2->m_sNum;
	  else return a1->m_sRang < a2->m_sRang;
	}

   bool Cmp3( const TDrawPC*& a1, const TDrawPC*& a2 ) const
	{
	  if( a1->m_sRang == a2->m_sRang ) 
		return  a1->m_sNum > a2->m_sNum;
	  else return a1->m_sRang > a2->m_sRang;
	}


   short m_shClass; //
 };


void TDiplomDoc::CreRngsXY( list<TDrawPC*>& rL )
 {
   TDrawShapeList::iterator itSta( m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_lstDrw.end() );

   rL.clear(); 

   for( ; itSta != itEnd; ++itSta )	
	 if( (*itSta)->isA() == TT_PC ) 
	   rL.push_back( (TDrawPC*)(*itSta).get() );

   greater<class TDrawPC*> mgr; 
   mgr.m_shClass = 1;
   rL.sort( mgr );   
 }

void TDiplomDoc::CreRngs( list<TDrawPC*>& rL )
 {
   TDrawShapeList::iterator itSta( m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_lstDrw.end() );

   rL.clear(); 

   for( ; itSta != itEnd; ++itSta )	
	 if( (*itSta)->isA() == TT_PC ) 
	   rL.push_back( (TDrawPC*)(*itSta).get() );

   greater<class TDrawPC*> mgr;    
   mgr.m_shClass = 2;
   rL.sort( mgr );   
 }

void TDiplomDoc::CreRngs_Back( list<TDrawPC*>& rL )
 {
   TDrawShapeList::iterator itSta( m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_lstDrw.end() );

   rL.clear(); 

   for( ; itSta != itEnd; ++itSta )	
	 if( (*itSta)->isA() == TT_PC ) 
	   rL.push_back( (TDrawPC*)(*itSta).get() );

   greater<class TDrawPC*> mgr;    
   mgr.m_shClass = 3;
   rL.sort( mgr );   
 }
