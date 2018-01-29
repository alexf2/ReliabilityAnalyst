#if !defined(_LOCK_UPDATE_)
#define _LOCK_UPDATE_

#include "stdafx.h"

class TLockUpdate
 {
public:
  TLockUpdate( CWnd* pW ):
    m_pW( pW )
	 {
	   pW->LockWindowUpdate();	   
	 }

  void Release()
   {
     if( m_pW )
	  {
        m_pW->UnlockWindowUpdate();
		m_pW = NULL;
	  }
   }

  ~TLockUpdate()
	{
	  Release();
	}

private:
  CWnd* m_pW;  
 };

class TLockRedraw
 {
public:
  TLockRedraw( CWnd* pW ):
    m_pW( pW )
	 {
	   pW->SetRedraw( FALSE );	   
	 }

  void Release()
   {
     if( m_pW )
	  {
        m_pW->SetRedraw( TRUE );
		m_pW = NULL;
	  }
   }

  ~TLockRedraw()
	{
	  Release();
	}

private:
  CWnd* m_pW;  
 };

#endif
