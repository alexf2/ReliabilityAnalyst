#if !defined(_DOC_ANALYZE_)
#define _DOC_ANALYZE_

#include <memory>
#include <list>
#include <vector>
using namespace std;

#include "toolparam.hpp"
//#include "brigad.hpp"

#include "en_prop.hpp"


class TDrawPC;
class TDrawShape;

enum TExcStat { TE_None, TE_HaveBadLinks, TE_HaveHLinks, TE_HaveCycles, 
                TE_NoTerm, TE_NoStart, TE_Assimetric,
				TE_IllegalEnter, TE_BadInLink_LotoHi, TE_CanntGoBottom, TE_VeryManyIn,
				TE_11NoMoreSimplify, TE_OnlyMultiInElem, TE_CanntGoTo, TE_CanntCreInt,
				TE_NoMoreSimplifiedCnct, TE_IllegalInvalidPtr };


class TGrafExc
 {
public:
  TGrafExc()
   {
     m_exc = TE_None;
   }
  TGrafExc( TExcStat exc ):
	m_exc( exc )
	 {
	 }

  TExcStat m_exc;
  int m_iData;
  short m_iRng, m_iNumb;
  short m_iRng2, m_iNumb2;
 };

class TDiplomDoc;

class TDoublePolus;
typedef auto_ptr<TDoublePolus> TAP_TPDP;
typedef list<TAP_TPDP> TAP_TPDP_Lst;

class TDoublePolus
 {
public:
  TDoublePolus( TDrawPC *pSta, TDrawShapeList &lstDrw, int iNum, TDiplomDoc*, bool bMaxOut );
  
  static void ResetUsedFlag( TDrawShapeList &lstDrw );
  void FindOutRng( TDrawShape*, int& riOut, list<int>& rlstOut, TDrawShape* );  
  void MarkDP( TDrawShape*, int& riOut, TDrawShape*, bool& rFlOK );  
  void MarkObj( TDrawShape *p );
  void AddNoDup( list<int>& lst, int );

  void Intersect( TDoublePolus& rDp, list<TDrawShape*>& );
  void Intersect2( list<TDrawShape*>& );

  static TDrawShape* FindSta( TDrawShapeList &lstDrw );  
  static int CountPC( list<TDrawShape*>& );  

  void RemovePoints();
  static void RemovePoints( list<TDrawShape*>& );

  void MarkCncts( bool );
  void CreFlipLst( list<TCnct*>&, short );  

  list<TDrawShape*> m_lstPC;
  TGrafExc m_exc;  

  int m_iOutRng;
  TDrawShape *m_pSta;

  int m_lstSiz;

  int m_iNum;
 };

class TPCPath
 {
public:

   TDrawPC *FindIntersectMax( TPCPath& );
   list<TDrawShape*> m_lst; //множество вершин и узлов принадлежащих всем путям удовлетворяющим цели
 };


enum TCnct_Types { TCT_NotInit, TCT_Parall, TCT_Seqv, TCT_FullAccess, 
                   TCT_NotDeterm, TCT_BiIn, TCT_BiInSofist, TCT_11
 };

class TPCConnect
 {
public:
   TPCConnect( int lstSiz )
    {
	  m_lstSiz = lstSiz;
      m_nIn = 0; m_pPC = NULL;
	  m_cType = TCT_NotInit;
    }
   void Create( TDrawPC *pPC, TDrawPC *pEnd, bool bFlPutRefs, bool bFlEnblIsol = false );
   void Create1( TDrawPC *pPC, TDrawPC *pEnd );
   void FindBackPath( TDrawShape *pPC, int& riOut, TDrawShape *pPCSrc, bool& rFlOK );
   void FindBackPath1( TDrawShape *pPC, TDrawShape *pPCEnd, TDrawShape *pPCSrc, bool& rFlOK );
   TDrawPC *GetLink1();
   TDrawPC *GetLink2();

   TDrawPC* RootInt();

   static void CreDPFromLst( list<TDrawShape*>& rL, TDrawShapeList &rOut, TDiplomDoc *pOwn );

   LPCSTR CnctDescr();

   void ResetUsed();

   TDrawPC *m_pPC;
   short m_nIn;//число входов
   TPCPath m_pats;
   TCnct_Types m_cType;

   short m_M, m_N; //при топ. сложности
   TDrawPC *m_pInt;
   list<TDrawShape*> m_lstInt;

   int m_lstSiz;
 };

typedef auto_ptr<TPCConnect> TAP_TPCConnect;
typedef list<TAP_TPCConnect> TAP_TPCConnect_Lst;

class TAnalyzer
 {
public:
   TAnalyzer( TDrawShapeList &rLst, TDiplomDoc *pDoc );
	

   void Create(); //-1 - for all PCs
      
   void InvalidateCtypes();
   void InvalidateCtype( TPCConnect* );
   void InvalidateCtype_What( TPCConnect*, TPCConnect* );
   bool AnalyzeConnectType( bool bFlCreate, bool bFlEnblIsol = false );
   TPCConnect *GetSibling( TPCConnect* );

   bool DetectTopSofist( TPCConnect* );

   void UnlockAll();
   void LockConnect( TPCConnect*, bool, bool );
   void LockConnect_For( short, short, bool, bool );
   void RemoveLink( TDrawShape* );

   void FlipInCnctTo( TPCConnect*, TPCConnect* );

   void Simplify( TPCConnect* );
   static void CalcSeqv( TPCConnect *pC1, TPCConnect *pC2, TParamCalc &rDta );
   static void CalcParall( TPCConnect *pC1, TPCConnect *pC2, TParamCalc &rDta );
   static void CalcSeqDec( TPCConnect *pC1, TPCConnect *pCpe, TParamCalc &rDta );
   static void AssParamPE( TPCConnect *pC, TParamCalc &rDta );
   void NextPE( TPCConnect* );
   void RemovePC( TPCConnect *pC );
   void RemovePCs( int iRHi, int iRLo );

   void InvalidateRng_What( int iRHi, int iRLo );

   static void MakeRect( TDrawPC*, TDrawPC*, CRect& rNew );

   void HCenterInRect( TPCConnect*, CRect& );
   void CenterInRect( TPCConnect*, CRect& );

   void MkConnectFromTo( TDrawPC*, TDrawPC* );

   TAP_TPCConnect_Lst::iterator GetItRMaxN( short );

   void RemoveAllNotUsed(); 
   void RecalcGluesFT( short, short );

   short LockConnect_ForDP( TDoublePolus&, short sRng );
   void InvalidateDP_WhatAndRemov( TDoublePolus&, short sRng );
   TPCConnect *AttachNewToDP( list<TDrawShape*>&, short sAssRng, TParamCalc& rAssDta );
   TPCConnect * AttachTo( TPCConnect*,  short sAssRng, TParamCalc& rAssDta );

   short FindLoRng( TDoublePolus&, short sRng );

   static void MakeRectForRang( list<TDrawShape*>& rL, short, CRect& );

   TPCConnect *FirstSimpl();
   void RestoreSort();

   void FlipTo( list<TCnct*>&, TPCConnect* );
        
   void CreFlipPCLst( TDoublePolus&, list<TPCConnect*>&, short );
   void FlipPCTo( list<TPCConnect*>&, TPCConnect* );

   TPCConnect *GetRoot();
   TPCConnect *GetNoRoot();

   TDrawShapeList &m_rlstDrw;
   TDiplomDoc *m_pDoc;
   TAP_TPCConnect_Lst m_lstPC_Cnct;
   bool m_bRedraw;

   int m_iListSz;
   TDrawPC *m_p11;

   int m_iPECnt;
 };

typedef list<TPCConnect*> TPCConnect_Lst_;

class TMultiDPSimplfy;
typedef auto_ptr<TMultiDPSimplfy> TAP_Ms;
typedef list<TAP_Ms> TAP_Ms_Lst;

class TMultiDPSimplfy
 {
public:
   TMultiDPSimplfy( int iCnct, TAnalyzer* pA )
	{
	  m_sMinRem = -1;
	  m_iCnct = iCnct;
	  m_pA = pA;
	}

   void Create( TDrawPC* pPc )
	{
	  m_apDP = TAP_TPDP( 
	     new TDoublePolus(pPc, m_pA->m_rlstDrw, m_iCnct, m_pA->m_pDoc, true)
		);
	  if( m_apDP->m_exc.m_exc != TE_None )	   
		throw m_apDP->m_exc;	

	  m_sPRang = pPc->m_sRang;
	  m_sPNum = pPc->m_sNum;
	}

   static void DetectIntersect( list<TDrawShape*>&, TAP_Ms_Lst& );
   static void Srt( TAP_Ms_Lst& );
   static void Srt_Remainder( TAP_Ms_Lst& );

   TPCConnect_Lst_ m_lstOutIn;
   TAP_TPDP m_apDP;
   short m_sMinRem;
   TParamCalc m_dta;

   int m_iCnct;
   TAnalyzer* m_pA;
   short m_sPRang, m_sPNum;
 };



#endif
