#include "brigad.hpp"
#include "diplomView.h"

void TBrigada::CalcStatistic( TStatistic& rSt )
 {  
   TSPC_List_It itSta( m_lstPC.begin() );
   TSPC_List_It itEnd( m_lstPC.end() );

   memset( &rSt, 0, sizeof(TStatistic) );
   rSt.m_iTotalPCs = m_lstPC.size();

   if( !rSt.m_iTotalPCs ) return;
 
   list<int> lstRng;
   
   for( ;itSta != itEnd; ++itSta )
	{
	  list<int>::iterator it = find( lstRng.begin(), lstRng.end(), (*itSta).m_pPC->m_sRang );
      if( it == lstRng.end() ) 
	     lstRng.push_back( (*itSta).m_pPC->m_sRang );

	  if( (*itSta).m_pPC->m_sServis_Brig == 2 )
	    rSt.m_iLabeled++;
	}

   rSt.m_iCntRanges = lstRng.size();
 }

void TBrigada::AR_Store( CArchive& rA )
 {
   TIndexedUnic::AR_Store( rA );

   rA << m_iOA << int(m_tdsDIS) << m_lstPC.size() <<
	 m_iTotal << m_iLabel;

   TSPC_List_It itSta( m_lstPC.begin() );
   TSPC_List_It itEnd( m_lstPC.end() );
   for( ;itSta != itEnd; ++itSta )
	 rA.Write( &*itSta, sizeof(TServisPC) );
 }
void TBrigada::AR_Load( CArchive& rA )
 {
   TIndexedUnic::AR_Load( rA );

   int iDS, iSz;
   rA >> m_iOA >> iDS >> iSz >> m_iTotal >> m_iLabel;
   m_tdsDIS = (TDisOfServis)iDS;

   if( !iSz ) return;

   m_lstPC.assign( iSz );
   TSPC_List_It itSta( m_lstPC.begin() );   
   for( ; iSz > 0; --iSz, ++itSta )
	{
	  rA.Read( &*itSta, sizeof(TServisPC) ); 
	  (*itSta).m_pPC = NULL;
	}
 }

bool TBrigada::WirePCPtrs( TSPC_List& rRem, TDrawShapeList& rPCs )
 {
   TSPC_List_It itSta( rRem.begin() );
   TSPC_List_It itEnd( rRem.end() );
   for( ;itSta != itEnd; ++itSta )
	{
	  TDrawShape* pFnd = 
	    (TDrawShape*)TIndexedUnic::Find_ID( (*itSta).m_iID, (TIndexedList&)rPCs );
	  if( !pFnd || pFnd->isA() != TT_PC ) return false;
	  (*itSta).m_pPC = (TDrawPC*)pFnd; 
	}

   return true;
 }

void TBrigada::ClearServisedPCs( TDrawShapeList& rPCs )
 {
   TDrawShapeList::iterator itSta( rPCs.begin() );
   TDrawShapeList::iterator itEnd( rPCs.end() );
   for( ;itSta != itEnd; ++itSta )
	 if( (*itSta)->isA() == TT_PC ) 
	    ((TDrawPC*)(*itSta).get())->m_sServis_Brig = 0;
 }

void TBrigada::ReadServisedPCs( TDrawShapeList& rPCs )
 {
   m_lstPC.clear();

  m_iTotal = m_iLabel = 0;

   TDrawShapeList::iterator itSta( rPCs.begin() );
   TDrawShapeList::iterator itEnd( rPCs.end() );
   for( ;itSta != itEnd; ++itSta )
	 if( (*itSta)->isA() == TT_PC && 
	     ((TDrawPC*)(*itSta).get())->m_sServis_Brig 
	   )
	  {
	    m_lstPC.push_back( TServisPC((TDrawPC*)(*itSta).get()) );
		m_iTotal++;
		if( ((TDrawPC*)(*itSta).get())->m_sServis_Brig == 2 )
		  m_iLabel++;
	  }
 }

void TBrigada::ShowServisedPCs( TDrawShapeList& rPCs, TDiplomView *pV )
 {
   TDrawShapeList::iterator itSta( rPCs.begin() );
   TDrawShapeList::iterator itEnd( rPCs.end() );
   for( ;itSta != itEnd; ++itSta )
	 if( (*itSta)->isA() == TT_PC )
	  {
	    short key = ((TDrawPC*)(*itSta).get())->m_sServis_Brig;
	    ((TDrawPC*)(*itSta).get())->m_sServis_Brig = 0;
		if( key != 0 )
		  pV->InvalObj( (*itSta).get() );
	  }

	TSPC_List_It itSta2( m_lstPC.begin() );
	TSPC_List_It itEnd2( m_lstPC.end() );
	for( ; itSta2 != itEnd2; ++itSta2 )
	 {
	   short key = (*itSta2).m_pPC->m_sServis_Brig;
	   (*itSta2).m_pPC->m_sServis_Brig = (*itSta2).m_iLbl_OR_Prior;
	   if( key != (*itSta2).m_iLbl_OR_Prior )
		 pV->InvalObj( (*itSta2).m_pPC );
	 }
 }

void TBrigada::CalcStatistic( TStatistic& rSt, TDrawShapeList& rPCs )
 {
   TDrawShapeList::iterator itSta( rPCs.begin() );
   TDrawShapeList::iterator itEnd( rPCs.end() );

   memset( &rSt, 0, sizeof(TStatistic) );
       
   list<int> lstRng;
   
   for( ;itSta != itEnd; ++itSta )
	if( (*itSta)->isA() == TT_PC && ((TDrawPC*)(*itSta).get())->m_sServis_Brig )
	 {
       rSt.m_iTotalPCs++;

	   list<int>::iterator it = 
		 find( lstRng.begin(), lstRng.end(), ((TDrawPC*)(*itSta).get())->m_sRang );
	   if( it == lstRng.end() ) 
		  lstRng.push_back( ((TDrawPC*)(*itSta).get())->m_sRang );

	   if( ((TDrawPC*)(*itSta).get())->m_sServis_Brig == 2 )
		 rSt.m_iLabeled++;
	 }

   rSt.m_iCntRanges = lstRng.size();
 }

bool TBrigada::CheckLM( TDrawShapeList& rPCs, short sLbl )
 {
   TDrawShapeList::iterator itSta( rPCs.begin() );
   TDrawShapeList::iterator itEnd( rPCs.end() );
     
   double dM, dD, dL; 
   bool bFlSta = true;
   for( ;itSta != itEnd; ++itSta )
	 if( (*itSta)->isA() == TT_PC && 
	     ((TDrawPC*)(*itSta).get())->m_sServis_Brig &&
	     (sLbl == -1 ||
	     ((TDrawPC*)(*itSta).get())->m_sServis_Brig == sLbl) )
	  {
        if( bFlSta )
		 {
           bFlSta = false;
		   dM = ((TDrawPC*)(*itSta).get())->m_dM;
		   dD = ((TDrawPC*)(*itSta).get())->m_dD;
		   dL = ((TDrawPC*)(*itSta).get())->m_dLambda;
		 }
		else if( dM != ((TDrawPC*)(*itSta).get())->m_dM ||
		         dD != ((TDrawPC*)(*itSta).get())->m_dD || 
				 dL != ((TDrawPC*)(*itSta).get())->m_dLambda
			   ) return false;
	  }

   return true;
 }

bool TBrigada::CheckLM( short sLbl )
 {
   TSPC_List::iterator itSta( m_lstPC.begin() );
   TSPC_List::iterator itEnd( m_lstPC.end() );
     
   double dM, dD, dL; 
   bool bFlSta = true;
   for( ;itSta != itEnd; ++itSta )
	 if( (*itSta).m_pPC->m_sServis_Brig &&
	    (sLbl == -1 || (*itSta).m_pPC->m_sServis_Brig == sLbl) )
	  {
        if( bFlSta )
		 {
           bFlSta = false;
		   dM = (*itSta).m_pPC->m_dM;
		   dD = (*itSta).m_pPC->m_dD;
		   dL = (*itSta).m_pPC->m_dLambda;
		 }
		else if( dM != (*itSta).m_pPC->m_dM ||
		         dD != (*itSta).m_pPC->m_dD || 
				 dL != (*itSta).m_pPC->m_dLambda
			   ) return false;
	  }

   return true;
 }


bool TBrigada::CheckingDIS( TDrawShapeList& rPCs, CString& rcsErr )
 {
   rcsErr = "";

   TStatistic st;
   TBrigada::CalcStatistic( st, rPCs );

   switch( m_tdsDIS )
	{
	  case  TDS_Priorety:
		{
		  if( st.m_iCntRanges > 2 )
		   {
			 rcsErr = "ѕри дисциплине \"ѕриоритетное обслуживание\"  должно быть не более двух типов устройств.";
			 return false;
		   }
		  if( !CheckLM(rPCs, 1) || !CheckLM(rPCs, 2) )
		   {
             rcsErr = "”стройства одного уровн€ проритета должны иметь одинаковые надЄжностные характеристики";
			 return false;
		   }
		};
	   break;
	  case TDS_NoPriorety:
		{
		  if( st.m_iCntRanges > 1 )
		   {
			 rcsErr = "ѕри бесприоритетном обслуживании устройства должны быть однотипными (одного ранга).";
			 return false;
		   }
		  /*if( st.m_iLabeled > 0 )
		   {
			 rcsErr = "ѕри бесприоритетном обслуживании не требуютс€ меченые устройства.";
			 return false;
		   }*/
		  if( !CheckLM(rPCs, 1) )
		   {
             rcsErr = "”стройства должны иметь одинаковые надЄжностные характеристики";
			 return false;
		   }
		}
	   break;
	  case TDS_LabeledPC:
		{
		  if( st.m_iCntRanges > 1 )
		   {
			 rcsErr = "ѕри дисциплине \"ћеченые устройства\"  требуетс€ однотипность (одинаковый ранг).";
			 return false;
		   }
		  if( !CheckLM(rPCs, -1) )
		   {
             rcsErr = "”стройства должны иметь одинаковые надЄжностные характеристики";
			 return false;
		   }
		}
	   break;
	};

   return true;
 }

LPCSTR GetDISDescribe_Brig( TDisOfServis m_tdsDIS )
 {
   switch( m_tdsDIS )
	{
	  case TDS_Priorety:
	    return "ѕриоритетна€";
	  case TDS_NoPriorety:
	    return "Ѕесприоритетна€";
	  case TDS_LabeledPC:
	    return "— мечеными устр.";
	  default:
	    return "Ќеизвестна€";
	}
 }

LPCSTR TBrigada::GetDISDescribe() const
 {
   return GetDISDescribe_Brig( m_tdsDIS );   
 }

void TBrigada::RenumPCs()
 {
   TSPC_List_It itSta( m_lstPC.begin() );
   TSPC_List_It itEnd( m_lstPC.end() );
   
   for( ;itSta != itEnd; ++itSta )
	 (*itSta).m_iID = (*itSta).m_pPC->ID();
 }

void TBrigada::GetML( double& m_M, double& d_L, double& m_M2, double& d_L2 )
 {
   m_M = m_M2 = 1;
   d_L = d_L2 = 0.5;   

   TSPC_List_It itSta( m_lstPC.begin() );
   TSPC_List_It itEnd( m_lstPC.end() );

   for( ;itSta != itEnd; ++itSta )
	if( (*itSta).m_iLbl_OR_Prior == 0 || (*itSta).m_iLbl_OR_Prior == 1 )
	 {
       m_M = (*itSta).m_pPC->m_dM;
       d_L = (*itSta).m_pPC->m_dLambda; 
	   if( m_tdsDIS != TDS_Priorety )
		 m_M2 = m_M, d_L2 = d_L;
	   break;
	 }
   
   itSta = m_lstPC.begin();
   itEnd = m_lstPC.end();

   for( ;itSta != itEnd; ++itSta )
	if( (*itSta).m_iLbl_OR_Prior == 2 )
	 {
       m_M2 = (*itSta).m_pPC->m_dM;
       d_L2 = (*itSta).m_pPC->m_dLambda; 
	   if( m_tdsDIS != TDS_Priorety )
		 m_M = m_M2, d_L = d_L2;
	   break;
	 }
 }

bool TBrigada::CheckAllData( CString& str )
 {
   if( !m_iTotal || !m_lstPC.size() )
	{
      str.Format( "Ѕригаде #%u не назначены устройства", (int)ID() );
	  return false;
	}

   if( !m_iLabel && m_tdsDIS != TDS_NoPriorety )
	{
	  if( m_tdsDIS != TDS_NoPriorety )
        str.Format( "Ѕригаде #%u не назначены меченые/приоритетные устройства", (int)ID() );
	  else str.Format( "Ѕригаде #%u не назначены устройства, требуемые дл€ исправности системы", (int)ID() );
	  return false;
	}

   TStatistic st;
   CalcStatistic( st );

   switch( m_tdsDIS )
	{
	  case  TDS_Priorety:
		{
		  if( st.m_iCntRanges > 2 )
		   {
			 str = "ѕри дисциплине \"ѕриоритетное обслуживание\"  должно быть не более двух типов устройств.";
			 return false;
		   }
		  if( !CheckLM(1) || !CheckLM(2) )
		   {
             str = "”стройства одного уровн€ проритета должны иметь одинаковые надЄжностные характеристики";
			 return false;
		   }
		};
	   break;
	  case TDS_NoPriorety:
		{
		  if( st.m_iCntRanges > 1 )
		   {
			 str = "ѕри бесприоритетном обслуживании устройства должны быть однотипными (одного ранга).";
			 return false;
		   }
		  /*if( st.m_iLabeled > 0 )
		   {
			 str = "ѕри бесприоритетном обслуживании не требуютс€ меченые устройства.";
			 return false;
		   }*/
		  if( !CheckLM(1) )
		   {
             str = "”стройства должны иметь одинаковые надЄжностные характеристики";
			 return false;
		   }
		}
	   break;
	  case TDS_LabeledPC:
		{
		  if( st.m_iCntRanges > 1 )
		   {
			 str = "ѕри дисциплине \"ћеченые устройства\"  требуетс€ однотипность (одинаковый ранг).";
			 return false;
		   }
		  if( !CheckLM(-1) )
		   {
             str = "”стройства должны иметь одинаковые надЄжностные характеристики";
			 return false;
		   }
		}
	   break;
	};

   return true;
 }
