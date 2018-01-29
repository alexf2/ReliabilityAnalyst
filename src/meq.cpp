#include "ChildFrm.h"
#include "doc_analyze.hpp"
#include "diplomDoc.h"
#include "diplomView.h"
#include "getgdimgr.hpp"
#include "Globlmsg.hpp"
#include "my_uniq.hpp"

#include "Halfdiv.hpp"

#include <math.h>

struct TVCnct
 {
   double m_dLmds, m_dM;
 };

static double Fac( double n )
 {
   double dRes = 1;
   for( __int64 i = n; i > 0; --i )
	 dRes *= double(i);

   return dRes;
 }

static double Fac( double k, int iCnt )
 {
   double dRes = k;
   for( __int64 i = k - 1; iCnt > 0; --iCnt, --i )
	 dRes *= double(i);

   return dRes;
 }

class TLDGraf
 {
public:
  TLDGraf( TChildFrame *pOwner ):
	 m_pOwner( pOwner )  
   {
   };

  void Init_Labeled( int in, int iK, double dM, double dL ); //с мечеными устройствами
   //M - интенсивность потока восстановления	
  double TLDGraf::Calc_Cln( double l, double n )
   {
     return Fac(n) /(Fac(l) * Fac(n-l));
   }
  double Calc_Pln( int l, int n, double dM, double dL, int in );

  double Calc_Labeled( double dL );

  vector<TVCnct> m_cnct;
  TChildFrame *m_pOwner;
 };

double TLDGraf::Calc_Labeled( double dL )
 {
   int k = m_cnct.size();
   double dP0 = 1, dPM = 1;
   for( int i = 1; i <= k; ++i )
	{
	  dPM *= m_cnct[ i - 1 ].m_dM;
	  dP0 += (Fac(k, i-1)*pow(dL, double(i)) / dPM);	  

	  m_pOwner->CheckCancel();
	}

   dP0 = pow( dP0, -1 );

   double dRes = 0; dPM = 1;
   for( i = 1; i <= k; ++i )
	{
	  dPM *= m_cnct[ i - 1 ].m_dM;
	  dRes += (dP0 * Fac(k, i-1)*pow(dL, double(i)) / dPM);

	  m_pOwner->CheckCancel();
	}

   return dRes;
 }

void TLDGraf::Init_Labeled( int in, int iK, double dM, double dL )
 {
   ASSERT( in >= iK );

   m_cnct.assign( iK );
   for( int i = 1; i <= iK; ++i )
	{
      m_cnct[i-1].m_dLmds = double(iK - i + 1) * dL;
	  //m_cnct[i].m_dM =
	  double dMei = 0;
	  double dPi = 0;
	  for( int j = 0; j <= in - iK; ++j )
	   {
         double dMeij = dM / double(i + j); //i от 1 до k		 
		 double dPij = Calc_Pln( i+j, in, dM, dL, in )/Calc_Cln(i+j, in)*Calc_Cln(i, iK)*Calc_Cln(j, in-iK);
		 dMei += (dPij * dMeij);
		 dPi += dPij;

		 m_pOwner->CheckCancel();
	   }

	  m_cnct[i-1].m_dM = dMei / dPi;
	}
 }

double TLDGraf::Calc_Pln( int l, int n, double dM, double dL, int in )
 {
   double dRi = 1;
   for( int i = 1; i <= l; ++i )
	 dRi *= (dL*double(n - i + 1)/dM);

   double dSumm = 0;
   for( int j = 1; j <= n; ++j )
	{
      double dPi = 1;
	  for( i = 0; i <= j; ++i )
	    dPi *= (dL*double(n - i + 1)/dM);

	  dSumm += dPi;
	}

   return dRi / dSumm;
 }

inline double RI( double dL, int iN, int i, double dMy )
 {
   return i == 0 ? 1:(dL * double(iN - i + 1)) / (double(i) * dMy);
 }

inline double RT( double dL, int iN, int i, double dMy, int iR )
 {
   return (dL * double(iN - iR - i)) / (double(iR) * dMy);
 }

double Calc_P0( int iN, int iR, double dL, double dMy )
 {
   double dS1 = 0;
   for( int j = 0; j <= iR; ++j )
	{
      double dP = 1;
	  for( int i = 0; i <= j; ++i )
	    dP *= RI( dL, iN, i, dMy );

	  dS1 += dP;
	}

   double dP1 = 1;
   for( int i = 0; i <= iR; ++i )
	{
       double dSumm = 0;
	   for( j = 0; j <= iN - iR - 1; ++j )
		{
          double dPQ = 1;
		  for( int k = 0; k <= j; ++k )
			dPQ *= RT( dL, iN, k, dMy, iR );

		  dSumm += dPQ;
		}

	   dP1 *= RI( dL, iN, i, dMy ) * dSumm;
	}

   return dS1 + dP1;
 }

inline double RoI( double dL, int iN, int i, double dMy )
 {
   return (dL * double(iN - i + 1)) / (double(i) * dMy);
 }
inline double RoG( double dL, int iN, int g, double dMy, int iR )
 {
   return (dL * double(iN - g + 1)) / (double(iR) * dMy);
 }

double Calc_Pi( int l, int iN, int iR, double dL, double dMy, double dP0 )
 {
   if( l == 0 ) return Calc_P0( iN, iR, dL, dMy );

   if( iR < l && l <= iN )
	{
      double dP = 1;
	  for( int i = 1; i <= iR; ++i )
	    dP *= RoI( dL, iN, i, dMy );

	  double dP2 = 1;
	  for( i = iR + 1; i <= l; ++i )
	    dP2 *= RoG( dL, iN, i, dMy, iR );

	  return 1.0 / dP0 * dP * dP2;
	}
   else
	{
	  double dP = 1;
	  for( int i = 1; i <= l; ++i )
	    dP *= RoI( dL, iN, i, dMy );

	  return 1.0 / dP0 * dP;
	}
 }

/*double Calc_NoProrety( TCalc_EQRestore *p )
 {
   double dMy = 1.0 / p->m_M;
   double dP0 = Calc_P0( p->m_iTotal, p->m_iOA, p->d_L, dMy );

   double dS1 = 0;
   for( int i = 1; i <= p->m_iOA; ++i )
	 dS1 += double(i) * Calc_Pi( i, p->m_iTotal, p->m_iOA, p->d_L, dMy, dP0 );

   double dS2 = 0;
   for( i = p->m_iOA + 1; i <= p->m_iTotal; ++i )
	 dS2 += Calc_Pi( i, p->m_iTotal, p->m_iOA, p->d_L, dMy, dP0 );

   dS2 *= double( p->m_iOA );

   double dS3 = 0;
   if( p->m_iLabel >= p->m_iTotal ) dS3 = dP0;
   else
	 for( i = 1; i <= p->m_iTotal - p->m_iLabel; ++i )
	   dS3 += Calc_Pi( i, p->m_iTotal, p->m_iOA, p->d_L, dMy, dP0 );

   return dMy * (dS1 - dS2) / dS3;
 }*/

inline double Lambda( int i, TCalc_EQRestore *p )
 {
   return p->d_L * double(p->m_iTotal + 1 - i);
 }

inline double My_no( int i, TCalc_EQRestore *p )
 {
   return 1.0/p->m_M * double(i);
 }

inline double RI_no( TCalc_EQRestore *p, int i )
 {
   return i == 0 ? 1:Lambda(i, p) / My_no(i, p);
 }

inline double My_o( int i, TCalc_EQRestore *p )
 {
   return (1 <= i && i <= p->m_iOA) ? 1.0/p->m_M * double(i):1.0/p->m_M * double(p->m_iOA);
 }

inline double RI_o( TCalc_EQRestore *p, int i )
 {
   return i == 0 ? 1:Lambda(i, p) / My_o(i, p);
 }

double CalcP0_0( TCalc_EQRestore *p )
 {
   double dSumm = 0;
   for( int j = 1; j <= p->m_iOA; ++j )
	{
	  double dMul = 1;
	  for( int i = 1; i <= j; ++i )
	    dMul *= RI_o( p, i );

	  dSumm += dMul;
	}

   double dSumm2 = 0;
   for( j = p->m_iOA + 1; j <= p->m_iTotal; ++j )
	{
	  double dMul = 1;
	  for( int i = 1; i <= p->m_iOA; ++i )
	    dMul *= RI_o( p, i );

	  double dMul2 = 1;
	  for( i = p->m_iOA + 1; i <= j; ++i )
	    dMul2 *= RI_o( p, i );

	  dSumm2 += dMul * dMul2;
	}

   return 1.0 / (1.0 + dSumm + dSumm2);
 }
double CalcP0_n0( TCalc_EQRestore *p )
 {
   double dSumm = 0;
   for( int j = 1; j <= p->m_iTotal; ++j )
	{
	  double dMul = 1;
	  for( int i = 1; i <= j; ++i )
	    dMul *= RI_no( p, i );

	  dSumm += dMul;
	}

   return 1.0 / (1.0 + dSumm);
 }

double CalcPj_n0( TCalc_EQRestore *p, int j )
 {
   double dMul = 1;
   for( int i = 1; i <= j; ++i )
	 dMul *= RI_no( p, i );

   return CalcP0_n0(p) * dMul;
 }

double CalcPj_0( TCalc_EQRestore *p, int j )
 {
   if( 1 <= j  && j <= p->m_iOA )
	{
	  double dMul = 1;
	  for( int i = 1; i <= j; ++i )
		dMul *= RI_o( p, i );

	  return CalcP0_0(p) * dMul;
	}
   
   double dMul = 1;
   for( int i = 1; i <= p->m_iOA; ++i )
	 dMul *= RI_o( p, i );

   double dMul2 = 1;
   for( i = p->m_iOA + 1; i <= j; ++i )
	 dMul2 *= RI_o( p, i );

   return CalcP0_0(p) * dMul * dMul2;   
 }

double CalcTo( TCalc_EQRestore *p )
 {
   double dSumm = 0;
   for( int i = 1; i <= p->m_iTotal; ++i )
	 dSumm += My_o( i, p ) * CalcPj_0( p, i );

   double dMyE = 1.0 / dSumm;

   double dP0 = CalcP0_0( p );

   double dSumm1 = 0;
   for( int j = 1; j <= p->m_iTotal - p->m_iLabel; ++j )
	{
	  double dMul = 1;
	  for( int i = 0; i <= j; ++i )
	    dMul *= RI_o( p, i );

	  dSumm1 += dMul;
	}

   double dSumm2 = 0;
   for( j = 1; j <= p->m_iTotal - p->m_iLabel; ++j )
	{
	  double dMul = 1;
	  for( int i = 0; i <= j; ++i )
	    dMul *= RI_o( p, i );

	  dSumm2 += dMul;
	}

   return dMyE * (dP0 * (1 + dSumm1) / (1 - dP0*(1 + dSumm2)));
 }

double CalcTno( TCalc_EQRestore *p )
 {
   double dSumm = 0;
   for( int i = 1; i <= p->m_iTotal; ++i )
	 dSumm += My_no( i, p ) * CalcPj_n0( p, i );

   double dMyE = 1.0 / dSumm;
   //double dMyE = 1.0 / p->d_L;

   double dP0 = CalcP0_n0( p );

   double dSumm1 = 0;
   for( int j = 1; j <= p->m_iTotal - p->m_iLabel; ++j )
	{
	  double dMul = 1;
	  for( int i = 0; i <= j; ++i )
	    dMul *= RI_no( p, i );

	  dSumm1 += dMul;
	}

   double dSumm2 = 0;
   for( j = 1; j <= p->m_iTotal - p->m_iLabel; ++j )
	{
	  double dMul = 1;
	  for( int i = 0; i <= j; ++i )
	    dMul *= RI_no( p, i );

	  dSumm2 += dMul;
	}

   return dMyE * (dP0 * (1 + dSumm1) / (1 - dP0*(1 + dSumm2)));
 }

class TCalc_NoProretyData: public TExtraData
 {
public:
   TCalc_EQRestore *m_p;
   double m_dTo;
 };

double DeltaToTno( double x, TExtraData* pExtDta )
 {
   TCalc_NoProretyData *p = (TCalc_NoProretyData*)pExtDta;

   p->m_p->m_M = 1.0 / x;

   return p->m_dTo - CalcTno( p->m_p );
 }

double Calc_NoProrety( TCalc_EQRestore *p )
 {
   const double dEps = 0.0001;

   TCalc_NoProretyData dta; dta.m_p = p;

   dta.m_dTo = CalcTo( p );
   //double dTno = CalcTno( p );

   //return (dTo + dTno) / 2.0;
   
   return HalfDivizion( DeltaToTno, &dta, dEps, 1.0 / p->m_M + 10*dEps, dEps );
	//THalfFunction* pF, TExtraData* pD,
	//						double a, double b, double eps
	//					 )
 }

void TChildFrame::CalcEQRest()
 {
   TCalc_EQRestore *p = (TCalc_EQRestore *)m_apCD.get();

   TLDGraf graf( this );
   //if( p->m_iLabel > p->m_iOA )
	//{
	  if( p->m_tdsDIS == TDS_LabeledPC )
	   {
		 graf.Init_Labeled( p->m_iTotal, p->m_iLabel, 1.0/p->m_M, p->d_L );
		 p->m_dLamda = graf.Calc_Labeled( p->d_L );
	   }	 
	  else if( p->m_tdsDIS == TDS_NoPriorety )
	   {
		 double dMKey = p->m_M;
		 try {
		   p->m_dLamda = Calc_NoProrety( p );
		  }
		 catch( TNoRoot )
		  {	     
		  }

		 p->m_M = dMKey;
	   }
	//}
   //else p->m_dLamda = 1.0 / p->m_M;

   CString str;   
   
   if( p->m_tdsDIS == TDS_NoPriorety )
	 str.Format( "Бригада #%u - дисциплина \"%s\"\r\n"
				 "Число устройств: %u\r\n"
				 "Число устройств нужных для исправности: %u\r\n" 
				 "Интенсивность восстановления: %f\r\n"
				 "Экв. интенсивность восстановления: %f\r\n",
				 p->m_id,
				 GetDISDescribe_Brig( p->m_tdsDIS ),
				 p->m_iTotal,
				 p->m_iLabel,
				 1.0/p->m_M,
				 p->m_dLamda
			   );   
   else if( p->m_tdsDIS == TDS_LabeledPC )
	str.Format( "Бригада #%u - дисциплина \"%s\"\r\n"
				 "Число устройств: %u\r\n"
				 "Число меченых устройств: %u\r\n"
				 "Интенсивность восстановления: %f\r\n"
				 "Экв. интенсивность восстановления: %f\r\n",
				 p->m_id,
				 GetDISDescribe_Brig( p->m_tdsDIS ),
				 p->m_iTotal,
				 p->m_iLabel,
				 1.0/p->m_M,
				 p->m_dLamda
			   );   
   else
	str.Format( "Бригада #%u - дисциплина \"%s\"\r\n"
				 "Число устройств: %u\n"
				 "Число приоритетных устройств: %u\r\n"
				 "Интенсивность восстановления 1: %f\r\n"
				 "Интенсивность восстановления 2: %f\r\n"
				 "Экв. интенсивность восстановления: %f\r\n",
				 p->m_id,
				 GetDISDescribe_Brig( p->m_tdsDIS ),
				 p->m_iTotal,
				 p->m_iLabel,
				 1.0/p->m_M,
				 1.0/p->m_M2,
				 p->m_dLamda
			   );      
   
   if( m_apCD->m_pTxt )
	{
      m_apCD->m_pTxt->SetAln( DT_LEFT );
      m_apCD->m_pTxt->SetTxt( str );   
	}
 }


void TChildFrame::CalcRun()
 {
   /*bool bFl = true;
   for( int i = 0; i < 25; ++i )
	{
	  CSyncObject* sync[3] = { &m_evCalcStop, &m_evSingleStep, &m_evSingleStepMode };
	  CMultiLock lock( sync, 3 );

	  DWORD dwRes = lock.Lock( INFINITE, FALSE );
	  if( dwRes >= WAIT_OBJECT_0 && dwRes <= WAIT_OBJECT_0 + 2 )
		dwRes -= WAIT_OBJECT_0;
	  else if( dwRes >= WAIT_ABANDONED_0 && dwRes <= WAIT_ABANDONED_0 + 2 )
	    dwRes = -1;

	  if( dwRes == 0 ) CheckCancel();	  

	  MessageBeep( -1 );
	  

	  TAP_TPCConnect_Lst::iterator itSta( m_apAnalyzer->m_lstPC_Cnct.begin() );
	  TAP_TPCConnect_Lst::iterator itEnd( m_apAnalyzer->m_lstPC_Cnct.end() );
      for( ; itSta != itEnd; ++itSta )
	   {
	     m_evDataAccEnable.ResetEvent();
	     if( bFl ) (*itSta)->m_pPC->HighLight( true );
		 else (*itSta)->m_pPC->UnHighLight( true );
		 m_evDataAccEnable.SetEvent();
		 Sleep( 500 );
	   }
	  bFl = !bFl;	  
	}*/

   ::WaitForSingleObject( m_evDataAccEnable, INFINITE );
   //m_evDataAccEnable.ResetEvent();

   short res = m_pMainView->GetDocument()->CheckConnect();

   if( res == 0 )
	{
	  m_apCD->m_shErr = TCA_Common;		 
	  m_apCD->m_csErr = "Имеются оторванные объекты";
	  m_evDataAccEnable.SetEvent();
	  return;
	 }
     
   Quant();

   TLST_Brigada::iterator itSta( m_pMainView->GetDocument()->m_lstBrig.begin() );
   TLST_Brigada::iterator itEnd( m_pMainView->GetDocument()->m_lstBrig.end() );
   for( ; itSta != itEnd; ++itSta )
	{
	  TBrigada *p = (*itSta).get();

	  if( !TBrigada::WirePCPtrs(p->m_lstPC, m_apAnalyzer->m_rlstDrw) )
	   {
         m_apCD->m_csErr.Format( "У бригады #%u нарушен список обслуживаемых устройств", (int)p->ID() );
		 m_apCD->m_shErr = TCA_Common;		 
		 m_evDataAccEnable.SetEvent();
		 return;
	   }
	  
	  if( !p->CheckAllData(m_apCD->m_csErr) )
	   {
		 m_apCD->m_shErr = TCA_Common;		 
		 //m_apCD->m_csErr
		 m_evDataAccEnable.SetEvent();
		 return;
	   }
      m_apCD = auto_ptr<TCalcData>( (TCalcData*)new TCalc_EQRestore(p) );
	  m_apCD->m_pTxt = NULL;      

      Quant();
	  CalcEQRest();
	  Quant();
	  AssignEQRest( p, (TCalc_EQRestore*)m_apCD.get() );
	}

   m_evDataAccEnable.SetEvent();
   GlobalSimplify( m_apAnalyzer.get(), false, false, NULL );
 }

void TChildFrame::Quant()
 {
   CheckCancel();
   m_evDataAccEnable.SetEvent();
   ::WaitForSingleObject( m_evDataAccEnable, INFINITE );
   //m_evDataAccEnable.ResetEvent();
   CheckCancel();
 }

void TChildFrame::AssignEQRest( TBrigada *pBrig, TCalc_EQRestore *pP )
 {
   TSPC_List::iterator itSta( pBrig->m_lstPC.begin() );
   TSPC_List::iterator itEnd( pBrig->m_lstPC.end() );
   for( ; itSta != itEnd; ++itSta )
	{
	  //if( (*itSta).m_iLbl_OR_Prior == 2 )
	  (*itSta).m_pPC->m_dM = 1.0 / pP->m_dLamda;
	  (*itSta).m_pPC->GenerateTTY();
	  if( !(*itSta).m_pPC->ShortView() ) (*itSta).m_pPC->Invalidate();
	}
 }

static void ShiftShps( TDrawShapeList& rL, int shft )
 {
   TDrawShapeList::iterator itSta_( rL.begin() );
   TDrawShapeList::iterator itEnd_( rL.end() );
      
   for( ; itSta_ != itEnd_; ++itSta_ )
	 (*itSta_)->m_position.OffsetRect( shft, shft );
 }

static void AddTo( TDrawShapeList& lstInt, TDiplomDoc *pDoc )
 {
   TDrawShapeList::iterator itSta_( lstInt.begin() );
   TDrawShapeList::iterator itEnd_( lstInt.end() );
      
   for( ; itSta_ != itEnd_; ++itSta_ )
	{
	  TDrawShape *p = (*itSta_).release();
      pDoc->Add( p );
	  pDoc->GetMainViev()->InvalObj( p );
	}

   TDrawShapeList::iterator itSta( lstInt.begin() );
   TDrawShapeList::iterator itEnd( lstInt.end() );

   for( ; itSta != itEnd; ++itSta )	
	 for( int i = (*itSta)->GetGlueCount(); i > 0; --i ) 
	  {
		if( !(*itSta)->m_vecGlu[ i - 1 ]->m_lstGlu.size() ) continue;
		
		list<TGlueDescr>::iterator itSta2( (*itSta)->m_vecGlu[ i - 1 ]->m_lstGlu.begin() );
		list<TGlueDescr>::iterator itEnd2( (*itSta)->m_vecGlu[ i - 1 ]->m_lstGlu.end() );
		for( ; itSta2 != itEnd2; ++itSta2 )
		  if( (*itSta2).IsGlued() )
			(*itSta2).m_iID = (*itSta2).m_pShp->ID();
	  }	
 }

DWORD TChildFrame::WaitInRun( TAnalyzer *pAnalyzer )
 {
   m_evDataAccEnable.SetEvent();

   CSyncObject* sync[3] = { &m_evCalcStop, &m_evSingleStep, &m_evSingleStepMode };
   CMultiLock lock( sync, 3 );

   DWORD dwRes = lock.Lock( INFINITE, FALSE );
   if( dwRes >= WAIT_OBJECT_0 && dwRes <= WAIT_OBJECT_0 + 2 )
	 dwRes -= WAIT_OBJECT_0;
   else if( dwRes >= WAIT_ABANDONED_0 && dwRes <= WAIT_ABANDONED_0 + 2 )
	 dwRes = -1;

   if( dwRes == 2 ) pAnalyzer->m_bRedraw = false;

   ::WaitForSingleObject( m_evDataAccEnable, INFINITE );
   //m_evDataAccEnable.ResetEvent();

   return dwRes;
 }

void TChildFrame::AsyncMsgStr( LPCSTR lp )
 {
   CSingleLock lockM( &m_mtxMsg, TRUE );
   strncpy( strMsgSimpl.get(), lp, 1023 );
   strMsgSimpl.get()[ 1023 ] = 0;
   lockM.Unlock();
   PostMessage( globl_iMsgNewStatusStr, 0, 0 );
 }

class TMSFndDPWith
 {
public:
  TMSFndDPWith( TDrawPC *p )
   {
     m_p = p;
   }
  bool operator()( const TAP_Ms& rA )
   {
     return m_p->m_sRang == rA->m_sPRang;
   }
  TDrawPC *m_p;
 };

void TChildFrame::GlobalSimplify( TAnalyzer *pAnalyzer, bool bFlSingleStep, bool bFNoRoot, TPCConnect *pExt )
 {
   /*TAP_TPCConnect_Lst::iterator itSta( m_apAnalyzer->m_lstPC_Cnct.begin() );
   TAP_TPCConnect_Lst::iterator itEnd( m_apAnalyzer->m_lstPC_Cnct.end() );
   for( ; itSta != itEnd; ++itSta )
	{
      if( (*itSta)->m_cType == TCT_Parall
	}*/
    CheckCancel();

   ::WaitForSingleObject( m_evDataAccEnable, INFINITE );
	//m_evDataAccEnable.ResetEvent();
    pAnalyzer->RemoveAllNotUsed(); 
	m_evDataAccEnable.SetEvent();

	CheckCancel();

   while( pAnalyzer->m_lstPC_Cnct.size() > 1 )
	{
	  ::WaitForSingleObject( m_evDataAccEnable, INFINITE );
	  //m_evDataAccEnable.ResetEvent();
      TPCConnect *pCnct = pExt ? pExt:pAnalyzer->FirstSimpl();
	  m_evDataAccEnable.SetEvent();

	  if( !pCnct )
	    throw TGrafExc( TE_OnlyMultiInElem );
	  if( pCnct->m_cType == TCT_11 )
	    throw TGrafExc( TE_11NoMoreSimplify );

	  if( !bFlSingleStep && ::WaitForSingleObject(m_evSingleStepMode, 0)==WAIT_TIMEOUT )
	   {         
	     ::WaitForSingleObject( m_evDataAccEnable, INFINITE );
	     //m_evDataAccEnable.ResetEvent();

         if( pAnalyzer->m_bRedraw == false )
		  {
            pAnalyzer->m_bRedraw = true;
			((CView*)m_pMainView)->Invalidate( FALSE );
		  }

		 TPCConnect *pSibl = NULL;
		 char cBuf[ 1024 ];
		 switch( pCnct->m_cType )
		  {	  		
			case TCT_Parall:
			case TCT_FullAccess:
			case TCT_NotDeterm:
			 {
			   pSibl = pAnalyzer->GetSibling( pCnct );			   
			   pAnalyzer->m_pDoc->HiliteCnct( pCnct ); 
			   pAnalyzer->m_pDoc->HiliteCnct( pSibl );

			   sprintf( cBuf, "%s сведение элементов [%u.%u] и [%u.%u]", 
				 pCnct->CnctDescr(), 
				 (int)pCnct->m_pPC->m_sRang, (int)pCnct->m_pPC->m_sNum,
				 (int)pSibl->m_pPC->m_sRang, (int)pSibl->m_pPC->m_sNum );			   
			 }	  	  
			 break;
			case TCT_Seqv:
			case TCT_BiIn:	    	  
			case TCT_BiInSofist:
			case TCT_11:			  
			 {			   
			   pAnalyzer->m_pDoc->HiliteCnct( pCnct );

			   TPCConnect *pSibl2 = pAnalyzer->GetSibling( pCnct );
			   if( pSibl )
				 sprintf( cBuf, "%s сведение элементов [%u.%u] и [%u.%u]", 
				   pCnct->CnctDescr(), 
				   (int)pCnct->m_pPC->m_sRang, (int)pCnct->m_pPC->m_sNum,
				   (int)pSibl->m_pPC->m_sRang, (int)pSibl->m_pPC->m_sNum );
			   else
				 sprintf( cBuf, "%s сведение элемента [%u.%u]", 
				   pCnct->CnctDescr(), 
				   (int)pCnct->m_pPC->m_sRang, (int)pCnct->m_pPC->m_sNum );
			 }
			  break;	   
		  };		 
		 AsyncMsgStr( cBuf );
		 //m_evDataAccEnable.SetEvent();

		 DWORD dwRes = WaitInRun( pAnalyzer );
		 
		 //::WaitForSingleObject( m_evDataAccEnable, INFINITE );
		 //m_evDataAccEnable.ResetEvent();
         pAnalyzer->m_pDoc->HiliteCnct( pCnct, false ); 
		 pAnalyzer->m_pDoc->HiliteCnct( pSibl, false );
		 m_evDataAccEnable.SetEvent();

		 //if( dwRes == 2 ) pAnalyzer->m_bRedraw = false;
	   }

	  CheckCancel();	
	  
	  ::WaitForSingleObject( m_evDataAccEnable, INFINITE );
	  //m_evDataAccEnable.ResetEvent();

	  if( pCnct->m_cType == TCT_Parall || pCnct->m_cType == TCT_Seqv ||
	      pCnct->m_cType == TCT_FullAccess
		)
	   {
	     if( bFNoRoot && pAnalyzer->m_lstPC_Cnct.size() == 2 )
		  {
            m_evDataAccEnable.SetEvent();
			return;
		  }
	     pAnalyzer->Simplify( pCnct );
	   }
	  else if( pCnct->m_cType == TCT_BiInSofist )
	   {
	     TDiplomDoc::ClearUsedFlag( pAnalyzer->m_rlstDrw );

	     TDrawShapeList lstInt;
		 if( !bFlSingleStep && ::WaitForSingleObject(m_evSingleStepMode, 0)==WAIT_TIMEOUT )
		  {
			if( pAnalyzer->m_bRedraw == false )
			 {
			   pAnalyzer->m_bRedraw = true;
			   ((CView*)m_pMainView)->Invalidate( FALSE );
			 }
		    pAnalyzer->m_pDoc->HiliteLstPCs( pCnct->m_lstInt );

			char cBuf[ 1024 ];			
		    sprintf( cBuf, "Пересечение путей, используемое для устранения топ.сложности" );
			AsyncMsgStr( cBuf );

			//m_evDataAccEnable.SetEvent();
			DWORD dwRes = WaitInRun( pAnalyzer );
			
		    //::WaitForSingleObject( m_evDataAccEnable, INFINITE );
		    //m_evDataAccEnable.ResetEvent();
			pAnalyzer->m_pDoc->HiliteLstPCs( pCnct->m_lstInt, false );

			CheckCancel();
		  }
		 TPCConnect::CreDPFromLst( pCnct->m_lstInt, lstInt, pAnalyzer->m_pDoc );
		 //ShiftShps( lstInt, 200 );
		 //AddTo( lstInt,  pAnalyzer->m_pDoc );
		 TAnalyzer an( lstInt, pAnalyzer->m_pDoc );
		 an.m_bRedraw = false;
		 an.Create();
		 m_evDataAccEnable.SetEvent();
		 
		 GlobalSimplify( &an, true, true, NULL );

		 CheckCancel();
		 ::WaitForSingleObject( m_evDataAccEnable, INFINITE );
		 //m_evDataAccEnable.ResetEvent();
		 CheckCancel();

		 TPCConnect *pSibl = pAnalyzer->GetSibling( pCnct );
		 TPCConnect *pRoot = an.GetRoot(),
		            *pPE1 = an.GetNoRoot();
		 TParamCalc dta;
		 TAnalyzer::CalcParall( pCnct, pCnct, dta );
		 TAnalyzer::AssParamPE( pRoot, dta );
		 TAnalyzer::CalcSeqDec( pRoot, pPE1, dta );

		 int iRngRoot = pCnct->RootInt()->m_sRang;
		 int iRngTop = pCnct->m_pPC->m_sRang;
		 pAnalyzer->LockConnect_For( iRngTop - 1, iRngRoot + 1, false, true );
		 pAnalyzer->InvalidateRng_What( iRngTop - 1, iRngRoot );		 
		 pAnalyzer->RemovePCs( iRngTop - 1, iRngRoot + 1 );

		 TAP_TPCConnect_Lst::iterator itTop = pAnalyzer->GetItRMaxN( iRngTop );
		 TAP_TPCConnect_Lst::iterator itRoot = pAnalyzer->GetItRMaxN( iRngRoot );
		 TAP_TPCConnect_Lst::iterator itRoot2 = itRoot;
		 TAP_TPCConnect_Lst::iterator itTop_Key = itTop;
		 CRect rNew; TAnalyzer::MakeRect( (*itTop)->m_pPC, (*itRoot)->m_pPC, rNew );
		 for( ; (*itTop)->m_pPC->m_sRang == iRngTop; ++itTop )
		  {
		    TDrawPC *p = new TDrawPC( *((*itTop)->m_pPC) );
			p->m_iSelCount = 0;
			p->SetSel_Internal( false );
			p->m_sRang--;// p->m_sNum = (*itTop)->m_pPC->m_sNum;

			
			pAnalyzer->m_pDoc->Add( p, pAnalyzer->m_rlstDrw );

			TAP_TPCConnect ap( new TPCConnect((*itTop)->m_lstSiz) );
			//ap->Create( p, NULL, true );
			TPCConnect *pCC = ap.get();
			pAnalyzer->m_lstPC_Cnct.insert( itRoot, ap );			
			pCC->m_pPC = p;
			p->m_pTPC = pCC;
			TAnalyzer::AssParamPE( pCC, dta );
			pAnalyzer->CenterInRect( pCC, rNew );
			pAnalyzer->NextPE( pCC );
			p->m_pTPC = NULL;

			pAnalyzer->MkConnectFromTo( (*itRoot2)->m_pPC, p );
			pAnalyzer->MkConnectFromTo( p, (*itTop)->m_pPC );
			
			TAP_TPCConnect_Lst::iterator it = itTop; ++it;

			TAP_TPCConnect_Lst::iterator itKey = itRoot2++;
			if( itRoot2 == pAnalyzer->m_lstPC_Cnct.end() ||
			    (*itRoot2)->m_pPC->m_sRang != iRngRoot
			  )
			 {
               itRoot2 = itKey; rNew.OffsetRect( -p->m_position.Width()*1.5, 0 );
			 }
			else 
			  TAnalyzer::MakeRect( (*it)->m_pPC, (*itRoot2)->m_pPC, rNew );			               

			if( (*it)->m_pPC->m_sRang == iRngTop && (*it)->m_pPC != (*itTop)->m_pPC ) 			 			
			  pAnalyzer->MkConnectFromTo( p, (*it)->m_pPC );
			if( itTop != itTop_Key )
			 {			   
			   it = itTop; --it;
			   if( (*it)->m_pPC != (*itTop)->m_pPC )
			     pAnalyzer->MkConnectFromTo( p, (*it)->m_pPC );
			 }
			//pCC->m_pPC = NULL;
		  }//llllll

		 pAnalyzer->RecalcGluesFT( iRngTop, iRngRoot );

		 pAnalyzer->AnalyzeConnectType( true );
		 pAnalyzer->RemoveAllNotUsed();		 
	   }
	  else if( pCnct->m_cType == TCT_NotDeterm )
	   {
         TDiplomDoc::ClearDPolusAndUsedFlag( pAnalyzer->m_rlstDrw );

		 TAP_Ms_Lst lstMS;

		 TDrawPC *pPCLnk = pCnct->GetLink1();
		 TAP_TPCConnect_Lst::iterator itSta( pAnalyzer->m_lstPC_Cnct.begin() );
		 TAP_TPCConnect_Lst::iterator itEnd( pAnalyzer->m_lstPC_Cnct.end() );
		 int iDPCnct = 0;
		 for( ; itSta != itEnd; ++itSta )
		   if( (*itSta).get() != pPCLnk->m_pTPC && 
			   ((*itSta)->GetLink1() == pPCLnk || (*itSta)->GetLink2() == pPCLnk)
			 )			 
			{
			  TMSFndDPWith dta( (*itSta)->m_pPC );
			  TAP_Ms_Lst::iterator it = find_if( lstMS.begin(), lstMS.end(), dta );
			  if( it != lstMS.end() ) continue; 

              TAP_Ms ap( new TMultiDPSimplfy(iDPCnct++, pAnalyzer) );			  
			  ap->Create( (*itSta)->m_pPC );
			  lstMS.push_back( ap );
			}

		 TMultiDPSimplfy::Srt( lstMS );

	     /*TPCConnect *pSibl = pAnalyzer->GetSibling( pCnct );
		 TDoublePolus dp1( pCnct->m_pPC, pAnalyzer->m_rlstDrw, 1, pAnalyzer->m_pDoc, true ),
		              dp2( pSibl->m_pPC, pAnalyzer->m_rlstDrw, 2, pAnalyzer->m_pDoc, true );*/
		 list<TDrawShape*> lstInt;
		 TMultiDPSimplfy::DetectIntersect( lstInt, lstMS );		 		 
		 //dp1.Intersect( dp2, lstInt );

		 if( !bFlSingleStep && ::WaitForSingleObject(m_evSingleStepMode, 0)==WAIT_TIMEOUT )
		  {
			if( pAnalyzer->m_bRedraw == false )
			 {
			   pAnalyzer->m_bRedraw = true;
			   ((CView*)m_pMainView)->Invalidate( FALSE );
			 }

			TAP_Ms_Lst::iterator itSta( lstMS.begin() );
			TAP_Ms_Lst::iterator itEnd( lstMS.end() );			
			char cBuf[ 1024 ];
			for( int iCnctDPP = 1; itSta != itEnd; ++itSta, ++iCnctDPP )
			 {
			   pAnalyzer->m_pDoc->HiliteLstPCs( (*itSta)->m_apDP->m_lstPC );
			   			
			   sprintf( cBuf, "Максимальный двухполюсник #%u", iCnctDPP );
			   AsyncMsgStr( cBuf );

			   //m_evDataAccEnable.SetEvent();
			   DWORD dwRes = WaitInRun( pAnalyzer );
			   
			   //::WaitForSingleObject( m_evDataAccEnable, INFINITE );
			   //m_evDataAccEnable.ResetEvent();
			   pAnalyzer->m_pDoc->HiliteLstPCs( (*itSta)->m_apDP->m_lstPC, false );

			   CheckCancel();
			 }			

			pAnalyzer->m_pDoc->HiliteLstPCs( lstInt );

			sprintf( cBuf, "Пересечение двухполюсников (останется после свёртки)" );
			AsyncMsgStr( cBuf );

			//m_evDataAccEnable.SetEvent();
			DWORD dwRes = WaitInRun( pAnalyzer );
			
		    //::WaitForSingleObject( m_evDataAccEnable, INFINITE );
		    //m_evDataAccEnable.ResetEvent();
			pAnalyzer->m_pDoc->HiliteLstPCs( lstInt, false );

			CheckCancel();
		  }
		 

		 TDrawShapeList lstIntDP;
		 TPCConnect::CreDPFromLst( lstInt, lstIntDP, pAnalyzer->m_pDoc );
		 CheckCancel();

		 TAnalyzer anInt( lstIntDP, pAnalyzer->m_pDoc );
		 anInt.m_bRedraw = false;
		 anInt.Create();
		 m_evDataAccEnable.SetEvent();
		 
		 GlobalSimplify( &anInt, true, false, NULL );

		 CheckCancel();
		 ::WaitForSingleObject( m_evDataAccEnable, INFINITE );
		 //m_evDataAccEnable.ResetEvent();
		 CheckCancel();

		 TPCConnect *pRootInt = anInt.GetRoot();

		 short sRng = ((TDrawPC*)lstInt.front())->m_sRang;

		 TAP_Ms_Lst::iterator itSta2( lstMS.begin() );
		 TAP_Ms_Lst::iterator itEnd2( lstMS.end() );					 
		 for( ; itSta2 != itEnd2; ++itSta2 )
		  {
            TDrawShapeList lstDP;
			TPCConnect::CreDPFromLst( (*itSta2)->m_apDP->m_lstPC, lstDP, pAnalyzer->m_pDoc );

			Quant();

			TAnalyzer an( lstDP, pAnalyzer->m_pDoc );
			an.m_bRedraw = false;
			an.Create();
			m_evDataAccEnable.SetEvent();
			
			GlobalSimplify( &an, true, false, NULL );

			CheckCancel();
			::WaitForSingleObject( m_evDataAccEnable, INFINITE );
			//m_evDataAccEnable.ResetEvent();
			CheckCancel();

			TPCConnect *pRoot = an.GetRoot();		            			
			TAnalyzer::CalcSeqDec( pRootInt, pRoot, (*itSta2)->m_dta );

			(*itSta2)->m_sMinRem = pAnalyzer->FindLoRng( *((*itSta2)->m_apDP), sRng );
			pAnalyzer->CreFlipPCLst( *((*itSta2)->m_apDP), (*itSta2)->m_lstOutIn, sRng );
		  }

		 TMultiDPSimplfy::Srt_Remainder( lstMS );
		 CheckCancel();

		 /*TDrawShapeList lstDP1, lstDP2, lstIntDP; 
		 TPCConnect::CreDPFromLst( dp1.m_lstPC, lstDP1, pAnalyzer->m_pDoc );
		 CheckCancel();
		 TPCConnect::CreDPFromLst( dp2.m_lstPC, lstDP2, pAnalyzer->m_pDoc );
		 CheckCancel();
		 TPCConnect::CreDPFromLst( lstInt, lstIntDP, pAnalyzer->m_pDoc );
		 CheckCancel();
		 

		 TAnalyzer an1( lstDP1, pAnalyzer->m_pDoc );
		 an1.m_bRedraw = false;
		 an1.Create();
		 m_evDataAccEnable.SetEvent();
		 
		 GlobalSimplify( &an1, true, false, NULL );

		 CheckCancel();
		 ::WaitForSingleObject( m_evDataAccEnable, INFINITE );
		 m_evDataAccEnable.ResetEvent();
		 CheckCancel();

		 TAnalyzer an2( lstDP2, pAnalyzer->m_pDoc );
		 an2.m_bRedraw = false;
		 an2.Create();
		 m_evDataAccEnable.SetEvent();
		 
		 GlobalSimplify( &an2, true, false, NULL );

		 CheckCancel();
		 ::WaitForSingleObject( m_evDataAccEnable, INFINITE );
		 m_evDataAccEnable.ResetEvent();
		 CheckCancel();

		 TAnalyzer anInt( lstIntDP, pAnalyzer->m_pDoc );
		 anInt.m_bRedraw = false;
		 anInt.Create();
		 m_evDataAccEnable.SetEvent();
		 
		 GlobalSimplify( &anInt, true, false, NULL );

		 CheckCancel();
		 ::WaitForSingleObject( m_evDataAccEnable, INFINITE );
		 m_evDataAccEnable.ResetEvent();
		 CheckCancel();

		 TPCConnect *pRoot1 = an1.GetRoot(),
		            *pRoot2 = an2.GetRoot(),
					*pRootInt = anInt.GetRoot();
		 TParamCalc dta1;		 
		 TAnalyzer::CalcSeqDec( pRootInt, pRoot1, dta1 );
		 TParamCalc dta2;		 
		 TAnalyzer::CalcSeqDec( pRootInt, pRoot2, dta2 );

         short sRng = ((TDrawPC*)lstInt.front())->m_sRang;
		 short iMinRem1 = pAnalyzer->FindLoRng( dp1, sRng );
		 short iMinRem2 = pAnalyzer->FindLoRng( dp2, sRng );

		 //dp1.MarkCncts( true ), dp2.MarkCncts( true );
		 //list<TCnct*> lstFlip1, lstFlip2;
		 //dp1.CreFlipLst( lstFlip1, sRng ), dp2.CreFlipLst( lstFlip2, sRng );
		 list<TPCConnect*> lstFlipPC1, lstFlipPC2;
		 pAnalyzer->CreFlipPCLst( dp1, lstFlipPC1, sRng ), 
		 pAnalyzer->CreFlipPCLst( dp2, lstFlipPC2, sRng );*/

		 itSta2 = lstMS.begin();
		 TDoublePolus *p1 = (*itSta2)->m_apDP.get();
		 itEnd2 = lstMS.end(); itSta2++;
		 for( ; itSta2 != itEnd2; ++itSta2 )
		   p1->m_lstPC.splice( p1->m_lstPC.begin(), (*itSta2)->m_apDP->m_lstPC ),
		   Quant();

		 My_unique( p1->m_lstPC );
		 Quant();
		 p1->RemovePoints();
		 Quant();
		 TDoublePolus::RemovePoints( lstInt );

		 pAnalyzer->LockConnect_ForDP( *p1, sRng );		 
		 Quant();
		 pAnalyzer->InvalidateDP_WhatAndRemov( *p1, sRng );
		 Quant();		 

		 /*dp1.m_lstPC.splice( dp1.m_lstPC.begin(), dp2.m_lstPC );		 
		 My_unique( dp1.m_lstPC );		 

		 dp1.RemovePoints();

		 pAnalyzer->LockConnect_ForDP( dp1, sRng );
		 //short iMinRem2 = pAnalyzer->LockConnect_ForDP( dp2, ((TDrawPC*)lstInt.front())->m_sRang );
		 pAnalyzer->InvalidateDP_WhatAndRemov( dp1, sRng );
		 //pAnalyzer->InvalidateDP_WhatAndRemov( dp2, ((TDrawPC*)lstInt.front())->m_sRang );
		 */

		 TPCConnect *pPEC = 
		   pAnalyzer->AttachNewToDP( lstInt, lstMS.front()->m_sMinRem, lstMS.front()->m_dta );
		 pAnalyzer->FlipPCTo( lstMS.front()->m_lstOutIn, pPEC );

		 short sRngAss = lstMS.front()->m_sMinRem + 1;

		 itSta2 = lstMS.begin();
		 itEnd2 = lstMS.end(); itSta2++;
		 for( ; itSta2 != itEnd2; ++itSta2, ++sRngAss )
		  {
		    if( (*itSta2)->m_sMinRem > sRngAss )			  
			  sRngAss = (*itSta2)->m_sMinRem;

			//CheckCancel();
			/*m_evDataAccEnable.SetEvent();
			::WaitForSingleObject( m_evDataAccEnable, INFINITE );
			m_evDataAccEnable.ResetEvent();
			CheckCancel();*/
			Quant();

            TPCConnect *pPEC2 = 
		      pAnalyzer->AttachTo( pPEC, sRngAss, (*itSta2)->m_dta );
			pAnalyzer->FlipPCTo( (*itSta2)->m_lstOutIn, pPEC2 );

			pPEC = pPEC2;
		  }
		 //CheckCancel();

		 /*TPCConnect *p;
		 TPCConnect *pPEC1, *pPEC2;
		 if( iMinRem1 < iMinRem2 )
		  {
		    pPEC1 = p = pAnalyzer->AttachNewToDP( lstInt, iMinRem1, dta1 );
			pPEC2 = pAnalyzer->AttachTo( p, iMinRem2, dta2 );
		  }
		 else
		  {
            if( iMinRem1 == iMinRem2 ) ++iMinRem1;
			pPEC2 = p = pAnalyzer->AttachNewToDP( lstInt, iMinRem2, dta2 );
			pPEC1 = pAnalyzer->AttachTo( p, iMinRem1, dta1 );
		  }		 
		 //pAnalyzer->FlipTo( lstFlip1, pPEC1 );
		 //pAnalyzer->FlipTo( lstFlip2, pPEC2 );
		 pAnalyzer->FlipPCTo( lstFlipPC1, pPEC1 );
		 pAnalyzer->FlipPCTo( lstFlipPC2, pPEC2 );*/

		 pAnalyzer->RestoreSort();
		 Quant();
		 

		 if( !pAnalyzer->AnalyzeConnectType(true, true) )
		  {
		    pAnalyzer->RemoveAllNotUsed();
			pAnalyzer->AnalyzeConnectType( true, false );
			Quant();
		  }
		 pAnalyzer->RemoveAllNotUsed();
		 //Sleep( 500 );
		 //return;
	   }
	   else 
		{
		  TGrafExc exc( TE_NoMoreSimplifiedCnct );
		  if( pCnct )
		    exc.m_iRng = pCnct->m_pPC->m_sRang,
		    exc.m_iNumb = pCnct->m_pPC->m_sNum;
		  else
		    exc.m_iRng = -1,
		    exc.m_iNumb = -1;
		  m_evDataAccEnable.SetEvent();
		  throw exc;		   
		}

	  m_evDataAccEnable.SetEvent();
	  if( pExt ) break;
	}   
 }

