#include "drtool.hpp"
#include <typeinfo.h>

int TBaseDrTool::m_iCntInst = 0;

TBaseDrTool::~TBaseDrTool()
 {
   if( m_d )
	{
      delete m_d; m_d = NULL;
	  --TBaseDrTool::m_iCntInst;
	}
 }

void TBaseDrTool::AR_Store( CArchive& rA )
 {   
   TIndexedUnic::AR_Store( rA );

   ASSERT( m_d != NULL );
   rA.Write( m_d, GetSizeData() );
 }
void TBaseDrTool::AR_Load( CArchive& rA )
 {  
   TIndexedUnic::AR_Load( rA );

   ASSERT( m_d != NULL );
   rA.Read( m_d, GetSizeData() );
 }
int TBaseDrTool::operator==( const TBaseDrTool& rI )
 {
   ASSERT( m_d != NULL && rI.m_d != NULL );
   if( typeid(*m_d) != typeid(*rI.m_d) ) return FALSE;
   return !memcmp( m_d, rI.m_d, GetSizeData() );
 }


TPenDrTool::~TPenDrTool()
 {
 }   
void TPenDrTool::Create()
 {
   m_apObj = 
	 TAP_GdiObj( 
	   new CPen(Data()->m_iStyle, Data()->m_iWidth, Data()->m_clr) );
 }
TBaseDrTool* TPenDrTool::CreateCopy()
 {
   return new TPenDrTool();
 }
TBaseDrTool* TPenDrTool::Copy_Content()
 {
   TPenDrTool *p = new TPenDrTool();
   memcpy( p->m_d, m_d, sizeof(TPen_Data) );
   return p;
 }
int TPenDrTool::GetSizeData() const
 {
   return sizeof( TPen_Data );
 }

TBrushDrTool::~TBrushDrTool()
 {
 } 
void TBrushDrTool::Create()
 {   
   m_apObj = TAP_GdiObj( new CBrush() );
   LOGBRUSH lgBr =
	{
      Data()->m_solid,
	  Data()->m_clr,
	  Data()->m_solid == BS_HATCHED ? Data()->m_iIndex:0
	};
   ((CBrush*)m_apObj.get())->CreateBrushIndirect( &lgBr );
 }
TBaseDrTool* TBrushDrTool::CreateCopy()
 {
   return new TBrushDrTool();
 }
TBaseDrTool* TBrushDrTool::Copy_Content()
 {
   TBrushDrTool *p = new TBrushDrTool();
   memcpy( p->m_d, m_d, sizeof(TBr_Data) );
   return p;
 }
int TBrushDrTool::GetSizeData() const
 {
   return sizeof( TBr_Data );
 }

LOGFONT TFont_Data::lp_Init =
 { 
   20, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET,
   OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
   VARIABLE_PITCH | FF_DONTCARE, "Arial"
 };

TFontDrTool::~TFontDrTool()
 {
 }
void TFontDrTool::Create()
 {   
   m_apObj = TAP_GdiObj( new CFont() );   
   
   LOGFONT lp; memcpy( &lp, &Data()->m_lf, sizeof(LOGFONT) );
   lp.lfHeight = Data()->Weight();
   Get()->CreateFontIndirect( &lp );
 }
TBaseDrTool* TFontDrTool::CreateCopy()
 {
   return new TFontDrTool();
 }
TBaseDrTool* TFontDrTool::Copy_Content()
 {
   TFontDrTool *p = new TFontDrTool();
   memcpy( p->m_d, m_d, sizeof(TFont_Data) );
   return p;
 }
int TFontDrTool::GetSizeData() const
 {
   return sizeof( TFont_Data );
 }

TDrToolsManeger::~TDrToolsManeger()
 {
 }
//TBaseDrTool::m_iCntInst

bool TDrToolsManeger::ReleaseTool( int iInd )
 {
   CSingleLock lock( &m_mtxLock, TRUE );

   TIndexedList::iterator it = 
	  TIndexedUnic::Find_ID_It( iInd, (TIndexedList&)m_lst );
   
   TIndexedList::iterator itEnd = ((TIndexedList&)m_lst).end();
   ASSERT( it != itEnd );
   if( it == itEnd ) return false;

   ((TBaseDrTool*)((*it).get()))->ReleaseRef();
   
   if( ((TBaseDrTool*)((*it).get()))->CanRemove() ) 
	m_lst.erase( (TAP_BaseDrTool_List::iterator&)it );

   return true;
 }
TBaseDrTool* TDrToolsManeger::GetTool( TBaseDrTool* pTool_, int& rIndex )
 {
   CSingleLock lock( &m_mtxLock, TRUE );

   rIndex = -1;

   TAP_BaseDrTool_List::iterator itSta( m_lst.begin() );
   TAP_BaseDrTool_List::iterator itEnd( m_lst.end() );
   TBaseDrTool *pTool = NULL;
   for( ; itSta != itEnd; ++itSta )
	 if( *(*itSta) == *pTool_ )
	  {
        pTool = (*itSta).get(); break;
	  }

   if( !pTool )
	{
	  TAP_BaseDrTool ap( pTool_->Copy_Content() );
	  ap->Create(); 
      ap->CreateIndex( (TIndexedList&)m_lst );
	  pTool = ap.get();
	  m_lst.push_back( ap );      
	}

   if( pTool ) rIndex = pTool->ID(), pTool->AddRef();
   return pTool;
 }

void TDrToolsManeger::RecreateTools( bool bPen, bool bBr, bool bFont )
 {
   CSingleLock lock( &m_mtxLock, TRUE );

   TAP_BaseDrTool_List::iterator itSta( m_lst.begin() );
   TAP_BaseDrTool_List::iterator itEnd( m_lst.end() );
   
   for( ; itSta != itEnd; ++itSta )
    if( 
		(bPen && typeid(*(*itSta)) == typeid(TPenDrTool)) ||
		(bBr && typeid(*(*itSta)) == typeid(TBrushDrTool)) ||
		(bFont && typeid(*(*itSta)) == typeid(TFontDrTool)) 
	  )
	  (*itSta)->Create();
 }

void TDrToolItem::AR_Store( CArchive& rA )
 {
   TBaseDrTool *pT;
   if( m_ap.get() != NULL ) pT = m_ap.get();
   else pT = m_pTool;   

   rA << int(pT->isA());
   pT->AR_Store( rA );
 }
void TDrToolItem::AR_Load( CArchive& rA )
 {
   Kill();
   int iTyp; rA >> iTyp;
   switch( (TTypesDrawTool)iTyp )
	{
	  case TDR_Pen:
	    m_ap = TAP_BaseDrTool( (TBaseDrTool*)new TPenDrTool() );
		break;
	  case TDR_Brush:
	   m_ap = TAP_BaseDrTool( (TBaseDrTool*)new TBrushDrTool() );
	   break;
	  case TDR_Font:
	   m_ap = TAP_BaseDrTool( (TBaseDrTool*)new TFontDrTool() );
	   break;
	  default:
	    ASSERT( FALSE );
		return;
	};

   m_ap->AR_Load( rA );
   Create();
 }

