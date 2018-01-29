#if !defined(_MY_UNIQUE_)
#define _MY_UNIQUE_

//#include <algorithm>
#include <list>
using namespace std;

template<class T>  void My_unique( list<T>& rLst )
 {
   list<T>::iterator itSta( rLst.begin() );
   list<T>::iterator itEnd( rLst.end() );
   for( ; itSta != itEnd; )
	{
	  list<T>::iterator itS = itSta; ++itS;
      list<T>::iterator it = find( itS, rLst.end(), *itSta );
	  if( it != rLst.end() )	   
	     rLst.erase( it ); 
	  else ++itSta;
	}
 }

#endif
