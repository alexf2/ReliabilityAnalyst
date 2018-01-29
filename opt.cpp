#include "opt.hpp"
#include "diplom.h"

void ThrowBadCfg()
 {
   CString str;
   str.Format( "Испорчен файл конфигурации [%s]", 
	 (LPCSTR)static_cast<TDiplomApp*>(AfxGetApp())->GetConfig() );

   throw exception( (LPCSTR)str );
 }

fstream& operator<<( fstream& rStrm, TOptions& rO )
 {
   rStrm.write( (char*)&rO.m_wndPlac, sizeof(WINDOWPLACEMENT) );
   return rStrm;
 }

fstream& operator>>( fstream& rStrm, TOptions& rO )
 {
   rStrm.read( (char*)&rO.m_wndPlac, sizeof(WINDOWPLACEMENT) );   
   streamsize szRes = rStrm.gcount();
   if( szRes != sizeof(WINDOWPLACEMENT) ) ThrowBadCfg();

   return rStrm;
 }

fstream& operator<<( fstream& rStrm, TMFrOpt& rO )
 {
   operator<<( rStrm, static_cast<TOptions&>(rO) );

   rStrm.write( (char*)&rO.d, sizeof(rO.d) );
         
   return rStrm;
 }

fstream& operator>>( fstream& rStrm, TMFrOpt& rO )
 {
   operator>>( rStrm, static_cast<TOptions&>(rO) );

   rStrm.read( (char*)&rO.d, sizeof(rO.d) );      
   if( rStrm.gcount() != sizeof(rO.d) ) ThrowBadCfg();

   return rStrm;
 }



TOptions::TOptions( CWnd* pW )
 {
   pW->GetWindowPlacement( &m_wndPlac );
 }

TOptions::~TOptions()
 {
 }
  

TOptions& TOptions::operator=( const TOptions& rOpt )
 {
   memcpy( &m_wndPlac, &rOpt.m_wndPlac, sizeof(WINDOWPLACEMENT) );
   return *this;
 }
   

fstream& operator<<( fstream& rStrm, TMCildOpt& rO )
 {
   operator<<( rStrm, static_cast<TOptions&>(rO) );

   rStrm.write( (char*)&rO.d, sizeof(rO.d) );
   
   int dw = rO.m_csPath.GetLength();

   rStrm.write( (char*)&dw, sizeof(dw) );
   rStrm.write( const_cast<char*>((LPCSTR)rO.m_csPath), dw );   

   return rStrm;
 }

fstream& operator>>( fstream& rStrm, TMCildOpt& rO )
 {
   operator>>( rStrm, static_cast<TOptions&>(rO) );

   //rStrm.read( (char*)&rO.m_iSCX, sizeof(rO.m_iSCX) );
   rStrm.read( (char*)&rO.d, sizeof(rO.d) );
   if( rStrm.gcount() != sizeof(rO.d) ) ThrowBadCfg();
   
   int dw = 0;

   rStrm.read( (char*)&dw, sizeof(dw) );
   LPSTR lp = rO.m_csPath.GetBuffer( dw + 1 );
   rStrm.read( lp, dw );  
   rO.m_csPath.ReleaseBuffer( dw );
   
   if( rStrm.gcount() != dw ) ThrowBadCfg();

   return rStrm;
 }


//protected:
//  WINDOWPLACEMENT  m_wndPlac;