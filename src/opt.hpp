#if !defined(_TMYOPTIONS_)
#define _TMYOPTIONS_

#include "stdafx.h"

#include <fstream>
using namespace std;

void ThrowBadCfg();

template<class Tdata, class Targ> class TClassFunc
 {
public:
  TClassFunc( Tdata dta ):
    m_data( dta )
	 {
	 }

  void operator()( Targ arg )
   {
   }

protected:
  Tdata m_data;
 };

class TOptions
 {

friend class TMainFrame;

friend fstream& operator<<( fstream& rStrm, TOptions& );
friend fstream& operator>>( fstream& rStrm, TOptions& );

public:
  TOptions()
   {
   }
  TOptions( CWnd* );
  virtual ~TOptions();

  TOptions( const TOptions& rOpt )
   {
     this->operator=( rOpt );	 
   }

  TOptions& operator=( const TOptions& rOpt );
   

protected:
  WINDOWPLACEMENT  m_wndPlac;
 };

struct TMFData
 {
   int m_iSCX, m_iSCY;
   unsigned char m_bFlStatusOn, m_bFlToolOn, m_bFlLocToolOn,
	   m_bFlFactureOn, m_bSaveDesktopOnExit, m_bSaveOptOnExit;

   unsigned char m_bGridOn, m_bGuidsOn, m_bCnctPointsOn,
	 m_bStepGrid, m_bStepGuids;
 };

class TMFrOpt: public TOptions
 {

friend class TMainFrame;

friend fstream& operator<<( fstream& rStrm, TMFrOpt& );
friend fstream& operator>>( fstream& rStrm, TMFrOpt& );
friend class CMyDialogBar;
friend class TDiplomApp;
friend class TChildFrame;

public:
  TMFrOpt(): TOptions()
   {
     StdInit();
   }
  TMFrOpt( CWnd* pW ): TOptions( pW ) 
   {
     StdInit();
   }
  TMFrOpt( const TMFrOpt& rOpt )
   {
     this->operator=( rOpt );	 
   }

  TMFrOpt& operator=( const TMFrOpt& rOpt )
   {
     TOptions::operator=( rOpt );
     
	 memcpy( &d, &rOpt.d, sizeof(d) );

	 return *this;
   }

protected:
   TMFData d;

   void StdInit()
	{
      d.m_iSCX = GetSystemMetrics( SM_CXSCREEN );
      d.m_iSCY = GetSystemMetrics( SM_CYSCREEN );
	  d.m_bFlStatusOn = d.m_bFlToolOn = d.m_bFlLocToolOn =
	    d.m_bFlFactureOn = d.m_bSaveDesktopOnExit = 
		d.m_bSaveOptOnExit = 1;

	  d.m_bGridOn = d.m_bGuidsOn = d.m_bCnctPointsOn =
	  d.m_bStepGrid = d.m_bStepGuids = 1;
	}
 };

fstream& operator<<( fstream& rStrm, TMFrOpt& rO ); 
fstream& operator>>( fstream& rStrm, TMFrOpt& rO );

struct TCHData
 {
   /*double m_dStnslW;
   int m_iViewScale;
   unsigned char m_bFlStensilOn;*/
   int  m_reserved;
 };

class TMCildOpt: public TOptions
 {

friend class TMainFrame;

friend fstream& operator<<( fstream& rStrm, TMCildOpt& );
friend fstream& operator>>( fstream& rStrm, TMCildOpt& );
friend class CMyDialogBar;
friend class TDiplomApp;
friend class TChildFrame;
friend class TFindOptData;

public:
  TMCildOpt(): TOptions()
   {
     StdInit();
   }
  TMCildOpt( CWnd* pW ): TOptions( pW ) 
   {
     StdInit();
   }
  TMCildOpt( const TMCildOpt& rOpt )
   {
     this->operator=( rOpt );	 
   }

  TMCildOpt& operator=( const TMCildOpt& rOpt )
   {
     TOptions::operator=( rOpt );
     
	 m_csPath = rOpt.m_csPath;	 
	 memcpy( &d, &rOpt.d, sizeof(d) );

	 return *this;
   }

protected:  
  CString m_csPath;
  TCHData d;  
  
   void StdInit()
	{
	   d.m_reserved = 0;
       /*d.m_bFlStensilOn = 1;
	   d.m_dStnslW = 1.0/3.0;
	   d.m_iViewScale = 100;*/
	}
 };

fstream& operator<<( fstream& rStrm, TMCildOpt& rO ); 
fstream& operator>>( fstream& rStrm, TMCildOpt& rO );

#endif
