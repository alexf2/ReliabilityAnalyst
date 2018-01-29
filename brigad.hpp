#if !defined(_TBRIG_)
#define _TBRIG_

#include "stdafx.h"
#include "indexeditem.hpp"
#include "drawobj.h"

#include <list>
#include <memory>
using namespace std;

class TDrawPC;
class TDiplomView;

struct TServisPC
 {
   TServisPC()
	{
	  m_iID = -1; m_iLbl_OR_Prior = 0;
	  m_pPC = NULL;
	}

   TServisPC( TDrawPC *p )
	{
      m_iID = p->ID();
	  m_iLbl_OR_Prior = p->m_sServis_Brig;
	  m_pPC = p;
	}

   int m_iID;
   short m_iLbl_OR_Prior; //0-нет; 1-низкий; 2-высокий/меченое.

   TDrawPC *m_pPC; //need to wire
 };

typedef list<TServisPC> TSPC_List;
typedef TSPC_List::iterator TSPC_List_It;

struct TStatistic
 {
   unsigned int m_iTotalPCs;
   unsigned int m_iLabeled;
   unsigned int m_iCntRanges;   
 };

enum TDisOfServis { TDS_Priorety, TDS_NoPriorety, TDS_LabeledPC };

class TBrigada;
typedef auto_ptr<TBrigada> TAP_Brigada;
typedef list<TAP_Brigada> TLST_Brigada;

class TBrigada: public TIndexedUnic
 {
public:
  TBrigada(): TIndexedUnic()
   {
     m_iOA = 0; m_tdsDIS = TDS_LabeledPC;
	 m_iTotal = m_iLabel = 0;
   }

  TBrigada( int iOA, TDisOfServis tdsDIS ):
    TIndexedUnic(),
    m_iOA( iOA ), m_tdsDIS( tdsDIS )
	 {
	   m_iTotal = m_iLabel = 0;
	 }

  bool CheckingDIS( TDrawShapeList&, CString& rcsErr );//need wired list  
  void ShowServisedPCs( TDrawShapeList&, TDiplomView* );//need wired list  
  void ReadServisedPCs( TDrawShapeList& );
  static void ClearServisedPCs( TDrawShapeList& );

  static bool WirePCPtrs( TSPC_List&, TDrawShapeList& );

  void AR_Store( CArchive& rA );
  void AR_Load( CArchive& rA );

  void CalcStatistic( TStatistic& );//need wired list  
  static void CalcStatistic( TStatistic&, TDrawShapeList& );
 
  LPCSTR GetDISDescribe() const;

  int m_iOA; //ремонтники
  TDisOfServis m_tdsDIS;

  TSPC_List m_lstPC;
  void RenumPCs();

  void GetML( double& m_M, double& d_L, double& m_M2, double& d_L2 );
  bool CheckLM( TDrawShapeList&, short sLbl );
  bool CheckLM( short sLbl );
  bool CheckAllData( CString& );

  int m_iTotal, m_iLabel;
 };

LPCSTR GetDISDescribe_Brig( TDisOfServis );

#endif

