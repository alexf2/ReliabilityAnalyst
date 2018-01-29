#ifndef __INDEXED_ITEM__
#define __INDEXED_ITEM__


#include "stdafx.h"
#include <list>
#include <algorithm>
using namespace std;


class TIndexedUnic;

enum TT_IndTypes { TT_IndUnic, TT_Shape, TT_PC, TT_Cnct, TT_Point, TT_Text, TT_Ruler };


typedef auto_ptr<TIndexedUnic> TAuto_Indexed;
typedef list<TAuto_Indexed> TIndexedList;


class TFndID_Data
 {
public:
   TFndID_Data( int ID ): m_ID( ID ) {};
 
   bool operator()( const TAuto_Indexed& rI );	

   int m_ID;
 };

class TRenum_Data
 {
public:
   TRenum_Data( int IDnewSta ): m_IDnew( IDnewSta ) {};
 
   void operator()( const TAuto_Indexed& rI );	

   int m_IDnew;
 };

class TIndexedUnic: public CObject
 {

friend CArchive& operator<<( CArchive& rA, TIndexedUnic& rCh );
friend CArchive& operator>>( CArchive& rA, TIndexedUnic& rCh );
friend class TRenum_Data;

public:
  TIndexedUnic(): m_iID( 0 )
   {
   }
  virtual ~TIndexedUnic();

  void CreateIndex( TIndexedList& rL );

  virtual void AR_Store( CArchive& rA );
  virtual void AR_Load( CArchive& rA );

  virtual int isA() const
   {
     return TT_IndUnic;
   }

  int ID() const { return m_iID; }

  void ShiftUnic( int iShft )
   {
     m_iID += iShft;
   }

  static TIndexedUnic* Find_ID( int iID, TIndexedList& rL )
   {
     TFndID_Data dta( iID );
	 TIndexedList::iterator it = find_if( rL.begin(), rL.end(), dta );

     return it == rL.end() ? NULL:(*it).get();
   }

  static TIndexedList::iterator Find_ID_It( int iID, TIndexedList& rL )
   {
     TFndID_Data dta( iID );
	 TIndexedList::iterator it = find_if( rL.begin(), rL.end(), dta );

     return it;
   }

  static void RenumIDs( TIndexedList& rL )
   {
     TRenum_Data dta( 1 );
	 for_each( rL.begin(), rL.end(), dta );
   }
  
protected:
 int m_iID;
 //int m_iNew;
 };

inline void TRenum_Data::operator()( const TAuto_Indexed& rI )
 {
   rI->m_iID = m_IDnew++;
 }

inline bool TFndID_Data::operator()( const TAuto_Indexed& rI )
 {
   return rI->ID() == m_ID;
 }

inline CArchive& operator<<( CArchive& rA, TIndexedUnic& rCh )
 {
   rA << rCh.m_iID;
   return rA;
 }
inline CArchive& operator>>( CArchive& rA, TIndexedUnic& rCh )
 {
   rA >> rCh.m_iID;
   return rA;
 }

#endif
