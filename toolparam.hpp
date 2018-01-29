#if !defined(_TOOLPARAM_)
#define _TOOLPARAM_

#include "stdafx.h"
#include "drtool.hpp"
#include "drawobj.h"

#include <list>
#include <vector>
#include <memory>
using namespace std;


template<class T> class TEnumValues
 {

public:

  typedef vector<T> TVal_Lst; 
  typedef vector<T>::iterator TVal_Iterator;

  TEnumValues()
   {
     m_it = m_lst.begin();
   }

  TEnumValues& operator,( T arg )
   {
     m_lst.push_back( arg );
	 return *this;
   }

  T First()
   {
     m_it = m_lst.begin();
	 return *m_it;
   }
  T Next()
   {
     return m_it == m_lst.end() ? First():*m_it++;
   }
  void SetIter( T arg )
   {
     TVal_Iterator itSta( m_lst.begin() );
	 TVal_Iterator itEnd( m_lst.end() );
	 for( ; itSta != itEnd; ++itSta )
	   if( *itSta == arg )
		{
          m_it = ++itSta; return;
		}
	 
	 First();	  
   }

  int IndexOf( T arg )
   {
     TVal_Iterator itSta( m_lst.begin() );
	 TVal_Iterator itEnd( m_lst.end() );
	 for( int i = 0; itSta != itEnd; ++itSta, ++i )
	   if( *itSta == arg ) return i;

	 m_lst.push_back( arg );
	 return m_lst.size() - 1;
   }

  int GetSize() const
   {
     return m_lst.size();
   }

  T operator[]( int i )
   {
     return m_lst[ i ];
   }

  TVal_Lst& RefLst()
   {
     return m_lst;
   }

protected:
  TVal_Lst m_lst;
  TVal_Iterator m_it;
 };

typedef TEnumValues<int> EnumVal_Int;
typedef TEnumValues<COLORREF> EnumVal_Clr;


auto_ptr<EnumVal_Int> Line_EnumPattern();
auto_ptr<EnumVal_Int> Line_EnumThick();

auto_ptr<EnumVal_Int> Fill_EnumPattern();

auto_ptr<EnumVal_Clr> Common_EnumColor();
auto_ptr<EnumVal_Clr> BkGnd_EnumColor();

class TPropEdit
 {
public:
  TPropEdit( TDrToolItem *pPropInit ):
    m_pPropInit( pPropInit ),
	m_ttEdit( *pPropInit )
	 {
	   m_bFlAvaliable = true;
	   m_ttEdit.Recreate();
	 }

  TPropEdit( TPropEdit& rPE ):
    m_pPropInit( rPE.m_pPropInit ),
	m_ttEdit( *rPE.m_pPropInit )
	 {
	   m_bFlAvaliable = rPE.m_bFlAvaliable;
	   m_ttEdit.Recreate();
	 }

  TDrToolItem *m_pPropInit;
  bool m_bFlAvaliable;
  TDrToolItem m_ttEdit;
 };

typedef auto_ptr<TPropEdit> TAP_PropEdit;
typedef vector<TAP_PropEdit> TAP_PropEdit_Vec;

#endif
