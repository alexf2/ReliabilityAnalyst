#include "indexeditem.hpp"

void TIndexedUnic::CreateIndex( TIndexedList& rL )
 {   
   TIndexedList::iterator it( rL.begin() );
   TIndexedList::iterator itEnd( rL.end() );

   int iMax = 0;
   for( ; it != itEnd; ++it )
	 iMax = max( iMax, (*it)->m_iID );

   m_iID = iMax + 1;
 }

void TIndexedUnic::AR_Store( CArchive& rA )
 { 
   rA << *this;
 }
void TIndexedUnic::AR_Load( CArchive& rA )
 {
   rA >> *this;
 }

TIndexedUnic::~TIndexedUnic()
 {
 }
