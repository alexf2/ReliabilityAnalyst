// diplomDoc.cpp : implementation of the TDiplomDoc class
//

#include "stdafx.h"
#include "diplom.h"

#include "diplomDoc.h"
#include "diplomView.h"
#include "childfrm.h"

#include "getgdimgr.hpp"

#include <wingdi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <algorithm>
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// TDiplomDoc

IMPLEMENT_DYNCREATE(TDiplomDoc, CDocument)

BEGIN_MESSAGE_MAP(TDiplomDoc, CDocument)
	//{{AFX_MSG_MAP(TDiplomDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TDiplomDoc construction/destruction

TDiplomDoc::TDiplomDoc():
  m_tpPen_GlueNorm( *GDIMgr() ),
  m_tpPen_GlueSel( *GDIMgr() ),
  m_tbBrush_GlueSel( *GDIMgr() ),
  m_tpBrush_Han1( *GDIMgr() ),
  m_tpBrush_Han2( *GDIMgr() ),
  m_bFlLoadObjOnly( false )
{
	// TODO: add one-time construction code here
  m_nMapMode = MM_ANISOTROPIC;
  m_size    = CSize( 850, 1100 );
  m_ptCenterPriv = m_ptCenter = CPoint( 425, 550 );
  m_Albom     = DMORIENT_LANDSCAPE;
  m_sPageType = DMPAPER_LETTER;
  ComputePageSize();

  m_iKeyChecked = -1;

  m_dSc= 1;

  m_bHaveSel_G = m_bHaveSel = false;
  m_iCntSel = 0;

  //CreateInstuments();

  (*m_tpPen_GlueNorm).m_iStyle = PS_SOLID;
  (*m_tpPen_GlueNorm).m_iWidth = 0;
  (*m_tpPen_GlueNorm).m_clr = RGB(0,0,255);
  m_tpPen_GlueNorm.Create();

  (*m_tpPen_GlueSel).m_iStyle = PS_SOLID;
  (*m_tpPen_GlueSel).m_iWidth = 3;
  (*m_tpPen_GlueSel).m_clr = RGB(0,0,0);
  m_tpPen_GlueSel.Create();
    
  (*m_tbBrush_GlueSel).m_clr = RGB(255,0,0);  	  	  
  m_tbBrush_GlueSel.Create();

  (*m_tpBrush_Han1).m_clr = RGB(0,128,0);  	  	  
  m_tpBrush_Han1.Create();

  (*m_tpBrush_Han2).m_clr = RGB(0,0,255);  	  	  
  m_tpBrush_Han2.Create();

  memset( &m_wndPlac, 0, sizeof(m_wndPlac) );

  m_pChFrame = NULL;

  m_bFlArrow = true;
  m_bFlSimpleView = false;
}

TDiplomDoc::~TDiplomDoc()
{
}

TChfOpt* TDiplomDoc::GetFrameOpt()
 {
   if( !m_pChFrame )
	{
      POSITION pos = GetFirstViewPosition();	  
	  CView *pV = GetNextView( pos ); 
	  ASSERT( pV );
      m_pChFrame = reinterpret_cast<TChildFrame*>(pV->GetParentFrame());
	}

   return &m_pChFrame->m_optX;
 }

struct TPapCode
 {
   short m_sCod;
   int m_iW, m_iH;
 };

CSize GetPaperFormat( LPDEVMODE pDev )
 {
   static TPapCode codes[] =
	{
	 { DMPAPER_LETTER, 850, 1110 },
	 { DMPAPER_LEGAL, 850, 1400 },
	 { DMPAPER_A4, 827, 1169 },
	 { DMPAPER_CSHEET, 1700, 2200 },
	 { DMPAPER_DSHEET, 2200, 3400 },
	 { DMPAPER_ESHEET, 3400, 4400 },
	 { DMPAPER_LETTERSMALL, 850, 1110 },
	 { DMPAPER_TABLOID, 1100, 1700 },
	 { DMPAPER_LEDGER, 1700, 1100 },
	 { DMPAPER_STATEMENT, 550, 850 },
	 { DMPAPER_EXECUTIVE, 725, 1050 },
	 { DMPAPER_A3, 1169, 1653 }, 
	 { DMPAPER_A4SMALL, 827, 1169 },
	 { DMPAPER_A5, 583, 827 }, 
	 { DMPAPER_B4, 984, 1394 }, 
	 { DMPAPER_B5, 716, 1012 }, 
	 { DMPAPER_FOLIO, 850, 1300 },
	 { DMPAPER_QUARTO, 846, 1083 },
	 { DMPAPER_10X14, 1000, 1400 },
	 { DMPAPER_11X17, 1100, 1700 },
	 { DMPAPER_NOTE, 850, 1100 },
	 { DMPAPER_ENV_9, 387, 887 },
	 { DMPAPER_ENV_10, 412, 950 },
	 { DMPAPER_ENV_11, 450, 1037 },
	 { DMPAPER_ENV_12, 475, 1100 },
	 { DMPAPER_ENV_14, 500, 1150 },
	 { DMPAPER_ENV_DL, 433, 866 },
	 { DMPAPER_ENV_C5, 638, 902 }, 
	 { DMPAPER_ENV_C3, 1276, 1803 },
	 { DMPAPER_ENV_C4, 902, 1276 }, 
	 { DMPAPER_ENV_C6, 449, 638 },
	 { DMPAPER_ENV_C65, 449, 902 }, 
	 { DMPAPER_ENV_B4, 984, 1390 },
	 { DMPAPER_ENV_B5, 693, 984 }, 
	 { DMPAPER_ENV_B6, 693, 492 }, 
	 { DMPAPER_ENV_ITALY, 433, 905 },
	 { DMPAPER_ENV_MONARCH, 387, 750 },
	 { DMPAPER_ENV_PERSONAL, 362, 650 },
	 { DMPAPER_FANFOLD_US, 1487, 1100 },
	 { DMPAPER_FANFOLD_STD_GERMAN, 850, 1200 },
	 { DMPAPER_FANFOLD_LGL_GERMAN, 850, 1300 },
	 { 0, 0, 0 }	 
	};

   if( !pDev->dmPaperSize ) return CSize( pDev->dmPaperWidth, pDev->dmPaperLength );

   TPapCode *pC = codes;
   for( ; pC->m_iW && pC->m_iH; ++pC )
	 if( pDev->dmPaperSize == pC->m_sCod ) 
	  {
	    CSize sz( pC->m_iW, pC->m_iH );
	    if( pDev->dmOrientation == DMORIENT_LANDSCAPE )
		 {
           int x = sz.cx; sz.cx = sz.cy; sz.cy = x;
		 }

		return sz;
	  }

	return CSize( 827, 1169 );
 }

void TDiplomDoc::ComputePageSize()
 {
	CSize new_size( 850, 1100 );  // 8.5" x 11" default

	CPrintDialog dlg( FALSE );
	if( AfxGetApp()->GetPrinterDeviceDefaults(&dlg.m_pd) )
	 {
	   // GetPrinterDC returns a HDC so attach it
	   /*CDC dc;
	   HDC hDC= dlg.CreatePrinterDC();
	   ASSERT( hDC != NULL );
	   dc.Attach( hDC );

	   // Get the size of the page in loenglish		
	   new_size.cx = MulDiv( dc.GetDeviceCaps(HORZSIZE), 1000, 254 );
	   new_size.cy = MulDiv( dc.GetDeviceCaps(VERTSIZE), 1000, 254 );
       */		
	   new_size    = GetPaperFormat( dlg.GetDevMode() );		 		
	   m_Albom     = dlg.GetDevMode()->dmOrientation;
       m_sPageType = dlg.GetDevMode()->dmPaperSize;
	 }
	else
	  AfxMessageBox( "Не найден стандартный принтер", MB_OK|MB_ICONEXCLAMATION );

	// if size changed then iterate over views and reset
	if( new_size != m_size )
	{
		m_size = new_size;
		POSITION pos = GetFirstViewPosition();
		while( pos != NULL )
		 {
		   CView *pV = GetNextView( pos );
		   if( typeid(*pV) == typeid(TDiplomView) ) 		   
		     ((TDiplomView*)pV)->SetPageSize( m_size );
		 }
	}
 }


BOOL TDiplomDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// TDiplomDoc serialization

inline CArchive& operator<<( CArchive& ar, bool b )
 {
   ar << int(b);
   return ar;
 }
inline CArchive& operator>>( CArchive& ar, bool& b )
 {
   int i;
   ar >> i; b = bool(i);
   return ar;
 }

void TDiplomDoc::ReadObjects( CArchive& ar )
 {
   POSITION ps = GetFirstViewPosition();
   CView *pV = GetNextView( ps );	   
   TChildFrame *pF = (TChildFrame*)pV->GetParentFrame(); 

   TIndexedUnic unic;
   unic.CreateIndex( (TIndexedList&)m_lstDrw );
   int iShft = m_lstDrw.size() ? unic.ID():0;

   int iSz; ar >> iSz;
   for( ; iSz > 0; --iSz )
	{
	  int iTyp;
	  ar >> iTyp;          

      TAuto_DrawShape ap;

	  switch( iTyp )
	   {		     
		 case TT_Shape:
		   ap = auto_ptr<TDrawShape>( new TDrawShape() );
		   break;
		 case TT_PC:			   
		   ap = auto_ptr<TDrawShape>( (TDrawShape*)new TDrawPC() );
		   break;			  
		 case TT_Cnct:			   
		   ap = auto_ptr<TDrawShape>( (TDrawShape*)new TCnct() );
		   break;
		 case TT_Point:
		   ap = auto_ptr<TDrawShape>( (TDrawShape*)new TDrawPoint() );
		   break;
		 case TT_Text:
		   ap = auto_ptr<TDrawShape>( (TDrawShape*)new TDrawTxt() );
		   break;
		 case TT_Ruler:
		   ap = auto_ptr<TDrawShape>( (TDrawShape*)new TDrawRuler() );
		   break;
	   };
	  ap->m_pDocument = this;
	  //ar >> *ap;
	  ap->AR_Load( ar );
	  ap->ShiftIDs( iShft );
	  ap->ShiftUnic( iShft );
	  ap->SetScaled( pF->m_optX.m_dScale );

	  m_lstDrw.push_back( ap );
	}
 }

int TDiplomDoc::GetObjCnt( bool bFlSelectionOnly )
 {
   if( bFlSelectionOnly == false ) return m_lstDrw.size();

   TDrawShapeList::iterator itSta( m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_lstDrw.end() );

   int iCnt = 0;
   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->isA() != TT_Ruler && (*itSta)->IsSelected() ) ++iCnt;

   return iCnt;
 }

void TDiplomDoc::WriteObjects( CArchive& ar, bool bFlSelectionOnly )
 {
   ar << GetObjCnt( bFlSelectionOnly );

   TShpStoreTool_Dta dta( ar, bFlSelectionOnly );
   for_each( m_lstDrw.begin(), m_lstDrw.end(), dta );
 }

void TDiplomDoc::LoadObjectsOnly( CString& rStr )
 {
   m_bFlLoadObjOnly = true;
   try {
     OnOpenDocument( rStr );
	}
   catch( CException *pE )
	{
	  m_bFlLoadObjOnly = false;
	  throw;
	}
   catch( exception ex )
	{
	  m_bFlLoadObjOnly = false;
	  throw;
	}
   m_bFlLoadObjOnly = false;
 }

void TDiplomDoc::Serialize( CArchive& ar )
 {
    CWaitCursor wc;

    POSITION ps = GetFirstViewPosition();
	CView *pV = GetNextView( ps );	   
    TChildFrame *pF = (TChildFrame*)pV->GetParentFrame(); 

	if( !m_bFlLoadObjOnly ) pF->Serialize( ar );
	else pF->SerializeEMM( ar );

	if( ar.IsStoring() )
	 {       	          
	   ar << m_size.cx << m_size.cy << m_Albom << m_sPageType <<
	     m_nMapMode	<< m_ptCenter.x << m_ptCenter.y <<
		 m_ptCenterPriv.x << m_ptCenterPriv.y << m_dSc;
	   CPoint ptScroll( pF->m_pMainView->GetScrollPosition() );
	   ar << ptScroll.x << ptScroll.y;

	   ar << m_bFlArrow << m_bFlSimpleView;

	   pF->GetWindowPlacement( &m_wndPlac );
	   ar.Write( &m_wndPlac, sizeof(m_wndPlac) );

	   CPoint ptScr( pF->m_pMainView->GetScrollPosition() );
	   //CPoint ptScr( pF->m_pMainView->GetDeviceScrollPosition() );	   
	   ar << ptScr.x << ptScr.y;

       RenumIDs();

	   WriteObjects( ar );	   

	   TIndexedUnic::RenumIDs( (TIndexedList&)m_lstBrig );
	   ar << m_lstBrig.size() << m_iKeyChecked;

	   TLST_Brigada::iterator itSta( m_lstBrig.begin() );
	   TLST_Brigada::iterator itEnd( m_lstBrig.end() );
	   for( ; itSta != itEnd; ++itSta )
		 (*itSta)->AR_Store( ar );
	 }
	else
	 {       
	   if( !m_bFlLoadObjOnly )
		{
		  ar >> m_size.cx >> m_size.cy >> m_Albom >> m_sPageType >>
		   m_nMapMode	>> m_ptCenter.x >> m_ptCenter.y >>
		   m_ptCenterPriv.x >> m_ptCenterPriv.y >> m_dSc;
		  ar >> m_pLoadScroll.x >> m_pLoadScroll.y;

		  ar >> m_bFlArrow >> m_bFlSimpleView;

		  ar.Read( &m_wndPlac, sizeof(m_wndPlac) );	   

		  CPoint ptScr;
		  ar >> ptScr.x >> ptScr.y;	   
		  //pF->m_pMainView->ScrollToPosition( ptScr );
		}
	   else
		{		  
		  CSize m_size_;
		  short m_Albom_;
		  short m_sPageType_;
		  int m_nMapMode_;

		  CPoint m_ptCenter_, m_ptCenterPriv_;
		  WINDOWPLACEMENT  m_wndPlac_;
		  CPoint m_pLoadScroll_;
		  bool m_bFlArrow_, m_bFlSimpleView_;  
		  CPoint ptScr_;

		  double m_dSc_;		  

		  ar >> m_size_.cx >> m_size_.cy >> m_Albom_ >> m_sPageType_ >>
		   m_nMapMode_	>> m_ptCenter_.x >> m_ptCenter_.y >>
		   m_ptCenterPriv_.x >> m_ptCenterPriv_.y >> m_dSc_;
		  ar >> m_pLoadScroll_.x >> m_pLoadScroll_.y;

		  ar >> m_bFlArrow_ >> m_bFlSimpleView_;

		  ar.Read( &m_wndPlac_, sizeof(m_wndPlac_) );	   
		  
		  ar >> ptScr_.x >> ptScr_.y;	   
		}

	   ReadObjects( ar );	   

	   RestoreGluPtrs();

	   bool bFlAllWired = true;
	   int iNumbBrig; ar >> iNumbBrig >> m_iKeyChecked;
	   for( ; iNumbBrig > 0; --iNumbBrig )
		{
          TAP_Brigada apBrig( new TBrigada() );
		  apBrig->AR_Load( ar );
		  TBrigada *pKey = apBrig.get();

		  m_lstBrig.push_back( apBrig );

		  if( !TBrigada::WirePCPtrs(pKey->m_lstPC, m_lstDrw) )
		   {
             bFlAllWired = false;
			 pKey->m_lstPC.clear();
			 pKey->m_iTotal = pKey->m_iLabel = 0;
		   }
		}
	   if( !bFlAllWired )
		{
          AfxMessageBox( "Не удалось построить списки устройств для некоторых бригад", MB_OK|MB_ICONINFORMATION );
		}
	 }
 }

void TDiplomDoc::RestoreGluPtrs()
 {
   TDrawShapeList::iterator itSta( m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_lstDrw.end() );

   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->IsActiveGlue() )	  
       for( int i = (*itSta)->GetGlueCount(); i > 0; --i ) 
		{
          if( !(*itSta)->m_vecGlu[ i - 1 ]->m_lstGlu.size() ) continue;
		  TGlueDescr &rDscr = (*itSta)->m_vecGlu[ i - 1 ]->m_lstGlu.front();
		  if( !rDscr.IsGlued() || rDscr.m_pShp ) continue;

		  TDrawShape *pShp = (TDrawShape*)TIndexedUnic::Find_ID( rDscr.m_iID, (TIndexedList&)m_lstDrw );
		  //ASSERT( pShp != NULL );
		  int nForin = rDscr.m_nGlu;
		  rDscr.m_nGlu = rDscr.m_iID = -1; rDscr.m_pShp = NULL;
		  
		  if( !pShp ) continue;
		  
		  bool bFlm;
		  (*itSta)->MakeGlue( i, pShp, nForin, bFlm );
		   //int nGlu, TDrawShape* pShp, int nGluForin, bool& rMade );	

		  /*TGlue &rGl = *(pShp->m_vecGlu[ rDscr.m_nGlu - 1 ].get());

		  list<TGlueDescr>::iterator itSta2( rGl.m_lstGlu.begin() );
		  list<TGlueDescr>::iterator itEnd2( rGl.m_lstGlu.end() );
		  for( ; itSta2 != itEnd2; ++itSta2 )
		    if( (*itSta2).m_iID == (*itSta)->ID() )
			 {
               (*itSta2).m_pShp = (*itSta).get();
			   break;
			 }*/
		}
 }

/////////////////////////////////////////////////////////////////////////////
// TDiplomDoc diagnostics

#ifdef _DEBUG
void TDiplomDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void TDiplomDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// TDiplomDoc commands

void TDiplomDoc::DeleteContents() 
 {
   // TODO: Add your specialized code here and/or call the base class
   
   m_lstDrw.clear();
   m_lstBrig.clear();

   CDocument::DeleteContents();
 }

void TDiplomDoc::Add( TDrawShape* pShp )
 {
   pShp->m_pDocument = this;
   pShp->CreateIndex( (TIndexedList&)m_lstDrw );
   m_lstDrw.push_back( TAuto_DrawShape(pShp) );
   pShp->SetScaled( m_dSc );
 }

void TDiplomDoc::Add( TDrawShape* pShp, TDrawShapeList& rL )
 {
   pShp->m_pDocument = this;
   pShp->CreateIndex( (TIndexedList&)rL );
   rL.push_back( TAuto_DrawShape(pShp) );
   pShp->SetScaled( m_dSc );
 }

TDrawShape* TDiplomDoc::GetMinID()
 {   
   TDrawShapeList::iterator itSta( m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_lstDrw.end() );
   int iMin = INT_MAX;
   TDrawShape *pObj = NULL;
   for( ; itSta != itEnd; ++itSta )
	 if( /*(*itSta)->IsSelected() &&*/ (*itSta)->m_iSelCount < iMin && (*itSta)->m_iSelCount > 0 )
	  {
		iMin = (*itSta)->m_iSelCount;
		pObj = (*itSta).get();
	  }

   return pObj;
 }

void TDiplomDoc::RemoveSelected( bool bFlFirst )
 {
   TDiplomView *pV = GetMainViev();

   if( bFlFirst )
	{      
	  TDrawShape *pObj = GetMinID();

	  if( pObj && !pObj->IsLocked() )
	   {
	     //pObj->Select( m_lstDrw, false );
		 pV->SelectShp( pObj, true );
	     //pV->InvalObj( pObj, true );
	     RemoveObj( pObj );		   
	   }

	  SetModifiedFlag();
	  return;
	}

   bool bFlRestart = true;
   while( bFlRestart )
	{
      bFlRestart = false;

	  TDrawShapeList::iterator itSta( m_lstDrw.begin() );
      TDrawShapeList::iterator itEnd( m_lstDrw.end() );
	  for( ; itSta != itEnd; )
	    if( (*itSta)->IsSelected() && !(*itSta)->IsLocked() )
		 {
		   TDrawShape *pShp = (*itSta++).get();
           //bFlRestart = true;
		   pV->InvalObj( pShp, true );
		   RemoveObj( pShp );		   
		   //break;
		 }
		else ++itSta;
	}
   SetModifiedFlag();
 }

void TDiplomDoc::RemoveObj( TDrawShape* pObj )
 {
   int iId = pObj->ID();

   TDrawShapeList::iterator itSta( m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_lstDrw.end() );
   TDrawShapeList::iterator itObj;
   
   for( ; itSta != itEnd; ++itSta )
	{
	  //(*itSta)->PreRemoveRelation( pObj );
	  if( pObj == (*itSta).get() ) itObj = itSta;
	}

   pObj->DeGlueAll( GetMainViev() );

   (*itObj).release(); m_lstDrw.erase( itObj );
   pObj->Remove();
 }

void TDiplomDoc::SelectInve()
 {
   TShpSelInveTool_Dta dta( RefLstDraw() );
   for_each( m_lstDrw.begin(), m_lstDrw.end(), dta );   

   RenumSelected();
 }

void TDiplomDoc::SelectHigh()
 {
   TDiplomView *pV = GetMainViev();

   SelectAll( false );

   TDrawShapeList::iterator itSta( m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_lstDrw.end() );      
   int iCnt = 1;
   for( ; itSta != itEnd; ++itSta )
    if( (*itSta)->IsHighLighted() )
	 {
	   (*itSta)->SetSel_Internal( true );
	   pV->InvalObj( (*itSta).get(), true );
	   (*itSta)->m_iSelCount = iCnt++;
	 }	   

   RenumSelected();
 }

void TDiplomDoc::SelectAll( bool bFl )
 {
   TShpSelTool_Dta dta( bFl, RefLstDraw(), GetMainViev() );
   for_each( m_lstDrw.begin(), m_lstDrw.end(), dta );

   RenumSelected();   
 }

void TDiplomDoc::RenumSelected()
 {
   TDrawShapeList::iterator itSta( m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_lstDrw.end() );      
   int iCnt = 1;
   for( ; itSta != itEnd; ++itSta )
	//if( (*itSta)->isA() != TT_Ruler )
	 //{
	   if( (*itSta)->IsSelected() ) (*itSta)->m_iSelCount = iCnt++;
	   else (*itSta)->m_iSelCount = 0;
	 //}
 }


/*void TDiplomDoc::UpdateMainView( CView* pSender, LPARAM lHint, CObject* pHint )
 {
   POSITION pos = GetFirstViewPosition();
   while( pos != NULL )
	{
	  CView *pV = GetNextView( pos );
	  if( typeid(*pV) == typeid(TDiplomView) ) 		   
		reinterpret_cast<TDiplomView*>( pV )->OnUpdate( pSender, lHint, pHint );
	}   
 }*/

void TDiplomDoc::ZSelection( bool bFlForward )
 {
   TDiplomView *pV = GetMainViev();

   TDrawShapeList::iterator itSta( m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_lstDrw.end() );

   TDrawShapeList lstTmp;

   for( ; itSta != itEnd; )
	 if( (*itSta)->IsSelected() )
	  {
	    pV->InvalObj( (*itSta).get(), true );
	    lstTmp.push_back( *itSta );
		m_lstDrw.erase( itSta++ );
	  }
	 else ++itSta;

   m_lstDrw.splice( bFlForward ? m_lstDrw.end():m_lstDrw.begin(), lstTmp );
 }

CPoint TDiplomDoc::GetSelectionCenter( bool bFlSel, CRect *pROut )
 {
   TDrawShapeList::iterator itSta( m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_lstDrw.end() );   

   int iMinX = INT_MAX, iMaxX = INT_MIN,
	   iMinY = INT_MAX, iMaxY = INT_MIN;
   for( ; itSta != itEnd; ++itSta )	
	 if( (*itSta)->isA() != TT_Ruler && (!bFlSel || (*itSta)->IsSelected()) )
	  {
		CRect r( (*itSta)->m_position ); (*itSta)->ExtendRect( r, 1 );
		iMinX = min( iMinX, min(r.left, r.right) ),
		iMaxX = max( iMaxX, max(r.left, r.right) ),
		iMinY = min( iMinY, min(r.top, r.bottom) ),
		iMaxY = max( iMaxY, max(r.top, r.bottom) );
	  }

   CRect rRes(  iMinX, iMinY, iMaxX, iMaxY );
   if( pROut ) *pROut = rRes;

   return rRes.CenterPoint();
 }

void TDiplomDoc::CenterDraw()
 {
   CPoint ptCent;
   //if( m_Albom == DMORIENT_LANDSCAPE )
	 //ptCent = CPoint( m_size.cy / 2, m_size.cx / 2 );
   //else
	ptCent = CPoint( m_size.cx / 2, m_size.cy / 2 );
   
         
   CPoint ptOff = ptCent - GetSelectionCenter(false);

   TDrawShapeList::iterator itSta( m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_lstDrw.end() );

   for( ; itSta != itEnd; ++itSta )
	if( (*itSta)->isA() != TT_Ruler )
	  (*itSta)->MakeOffset( ptOff );

   SetModifiedFlag();
 }

void TDiplomDoc::DrawSelected( CDC* pDrawDC )
 {
   if( m_pChFrame )	
	{
	  ::WaitForSingleObject( m_pChFrame->m_evDataAccEnable, INFINITE );
	  //m_pChFrame->m_evDataAccEnable.ResetEvent();
	}

   TDrawShapeList::iterator itSta( m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_lstDrw.end() );

   int iSave = pDrawDC->SaveDC();   

   try {
	 for( ; itSta != itEnd; ++itSta )
	   if( (*itSta)->IsSelected() ) (*itSta)->Draw( pDrawDC, 1 );
	 }
   catch( CException *pE )
	{
	  pDrawDC->RestoreDC( iSave );
	  if( m_pChFrame )	m_pChFrame->m_evDataAccEnable.SetEvent();
	  throw;
	}
   catch( exception ex )
	{
	  pDrawDC->RestoreDC( iSave );
	  if( m_pChFrame )	m_pChFrame->m_evDataAccEnable.SetEvent();
	  throw;
	}

   pDrawDC->RestoreDC( iSave );
   if( m_pChFrame )	m_pChFrame->m_evDataAccEnable.SetEvent();
 }

void TDiplomDoc::DrawRulers( CDC* pDrawDC, double dSc, const CRect& rClip_ )
 {
   if( pDrawDC->IsPrinting() || !m_pChFrame || !m_pChFrame->m_optX.m_bFlGuids ) return;

   TDrawShapeList::iterator itSta( m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_lstDrw.end() );

   m_bHaveSel_G = false;

   for( ; itSta != itEnd; ++itSta )	   
     if( (*itSta)->isA() == TT_Ruler ) 
	  {
	    (*itSta)->Draw( pDrawDC, dSc );
		if( (*itSta)->IsSelected() ) 
		  m_bHaveSel_G = true, m_bHaveSel = true;
	  }
 }

void TDiplomDoc::Draw( CDC* pDrawDC, double dSc, const CRect& rClip_ )
 {
   if( m_pChFrame )	
	{
	  ::WaitForSingleObject( m_pChFrame->m_evDataAccEnable, INFINITE );
	  //m_pChFrame->m_evDataAccEnable.ResetEvent();
	}
	

   TDrawShapeList::iterator itSta( m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_lstDrw.end() );

   CRect rClip( rClip_ ); rClip.NormalizeRect();
   CRect r;

   m_bHaveSel = false, m_iCntSel = 0;
   bool bFlHaveInve = false;
   list<TDrawShape*> lstInvePtr;
   
   CPen *pSavePen = pDrawDC->SelectObject( m_tpPen_GlueNorm() );
   CBrush *pSaveBr = pDrawDC->SelectObject( m_tbBrush_GlueSel() );

   //int iSave = pDrawDC->SaveDC();   

   try {
	  for( ; itSta != itEnd; ++itSta )
	   {         
         if( (*itSta)->isA() == TT_Ruler ) continue;

		 if( (*itSta)->IsSelected() 		     
		   ) 
		   m_bHaveSel = true, ++m_iCntSel;		 

		 if( pDrawDC->IsPrinting() )
		   (*itSta)->Draw( pDrawDC, 1 );
		 else
		  { 
		    if( !(*itSta)->IsInve() )
			 {
			   //CRect rOb( (*itSta)->m_position ); 
			   CRect rOb( (*itSta)->GetPosition((*itSta)->IsSelected()) );
			   MulRect( rOb, rOb, dSc );
			   rOb.NormalizeRect();
			   /*rOb.left = double(rOb.left) * dSc, rOb.right = double(rOb.right) * dSc,
			   rOb.top = double(rOb.top) * dSc, rOb.bottom = double(rOb.bottom) * dSc;
	  */
			   if( (*itSta)->IsSelected() ) rOb.InflateRect( 10, 10 );

			   if( r.IntersectRect(rClip, rOb) )
				 (*itSta)->Draw( pDrawDC, dSc );

			   //if( !(*itSta)->IsSelected() ) 
				 (*itSta)->DrawGlue( pDrawDC, TDrawShape::selected, m_tpPen_GlueNorm(), m_tpPen_GlueSel() );
			 }
			else 
			 {
			   bFlHaveInve = true;
			   lstInvePtr.push_back( (*itSta).get() );
			 }
		  }
	   } 

	  if( bFlHaveInve == true )
	   {
         int iKeyROP = pDrawDC->SetROP2( R2_MASKPEN );		 		 

	     list<TDrawShape*>::iterator itSta( lstInvePtr.begin() );
		 list<TDrawShape*>::iterator itEnd( lstInvePtr.end() );
		 for( ; itSta != itEnd; ++itSta )
		  {
            if( (*itSta)->isA() == TT_Ruler ) continue;

            //CRect rOb( (*itSta)->m_position ); 
		    CRect rOb( (*itSta)->GetPosition((*itSta)->IsSelected()) );
			MulRect( rOb, rOb, dSc );
			rOb.NormalizeRect();

			if( (*itSta)->IsSelected() ) rOb.InflateRect( 10, 10 );
			
			if( r.IntersectRect(rClip, rOb) )
			  (*itSta)->Draw( pDrawDC, dSc );			
			
		  }
		 pDrawDC->SetROP2( iKeyROP );
	   }

	  if( m_bHaveSel == true && !pDrawDC->IsPrinting() )
	   {
         itSta = m_lstDrw.begin();
         itEnd = m_lstDrw.end();
		 for( ; itSta != itEnd; ++itSta )
		  {
            if( (*itSta)->isA() == TT_Ruler ) continue;            

			if( (*itSta)->IsSelected() ) 
			 {			   			   
			   pDrawDC->SelectObject( 
				 (*itSta)->m_iSelCount == 1 ? m_tpBrush_Han1():m_tpBrush_Han2() );			   
			   (*itSta)->DrawTracker( pDrawDC, TDrawShape::selected );			   
			 }
		  }
	   }
	}
   catch( CException *pE )
	{
	  //pDrawDC->RestoreDC( iSave );
	  pDrawDC->SelectObject( pSaveBr );
	  pDrawDC->SelectObject( pSavePen );
	  if( m_pChFrame )	m_pChFrame->m_evDataAccEnable.SetEvent();
	  throw;
	}
   catch( exception ex )
	{
	  //pDrawDC->RestoreDC( iSave );
	  pDrawDC->SelectObject( pSaveBr );
	  pDrawDC->SelectObject( pSavePen );
	  if( m_pChFrame )	m_pChFrame->m_evDataAccEnable.SetEvent();
	  throw;
	}

   //pDrawDC->RestoreDC( iSave );
   pDrawDC->SelectObject( pSaveBr );
   pDrawDC->SelectObject( pSavePen );

   DrawRulers( pDrawDC, dSc, rClip_ );

   if( m_pChFrame )	m_pChFrame->m_evDataAccEnable.SetEvent();
 }

void TDiplomDoc::NewScaleAll( double dSc )
 {
   TDrawShapeList::iterator itSta( m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_lstDrw.end() );

   m_dSc = dSc;

   for( ; itSta != itEnd; ++itSta )
	 (*itSta)->SetScaled( dSc );
 }

/*void TDiplomDoc::CreateInstuments()
 {
   m_apBrSelectFirst = auto_ptr<CBrush>( new CBrush(RGB(0,128,0)) );
   m_apBrSelectSecond = auto_ptr<CBrush>( new CBrush(RGB(0,0,255)) );

   m_apPenGlueNorm = auto_ptr<CPen>( new CPen(PS_SOLID, 0, RGB(0,0,255)) );
   m_apPenGlueSel = auto_ptr<CPen>( new CPen(PS_SOLID, 3, RGB(0,0,0)) );
 }*/

/*TDiplomView* TDiplomDoc::GetMainViev()
 {
   POSITION pos = GetFirstViewPosition();
   while( pos != NULL )
	{
	  CView *pV = GetNextView( pos );
	  if( typeid(*pV) == typeid(TDiplomView) ) 		   
		return reinterpret_cast<TDiplomView*>( pV );
	}
   return NULL;
 }*/

void TDiplomDoc::UpdateMainView( CView* pSender, LPARAM lHint, CObject* pHint )
 {
   GetMainViev()->OnUpdate( pSender, lHint, pHint );
 }   

TDiplomView* TDiplomDoc::GetMainViev()
 {
   return m_pChFrame ? m_pChFrame->m_pMainView:NULL;
 }

void TShpSelTool_Dta::operator()( TAuto_DrawShape& arg )
 {
   //arg->Select( m_rLst, m_bFl );
   if( arg->m_bSelected != m_bFl ) 
	{
      arg->SetSel_Internal( m_bFl );
	  m_pMainView->InvalObj( arg.get(), true );
	}
   if( m_bFl ) arg->m_iSelCount = m_iCnt++;
 }

bool TDiplomDoc::HaveSelectedObj()
 {
   if( m_bHaveSel )
	{
	  TDrawShapeList::iterator itSta( m_lstDrw.begin() );
	  TDrawShapeList::iterator itEnd( m_lstDrw.end() );

	  for( ; itSta != itEnd; ++itSta )
	   if( (*itSta)->isA() != TT_Ruler ) return true;	   
	}
   return false;
 }

static int FSel1( TAuto_DrawShape& arg )
 {
   return arg->m_iSelCount == 1;
 }

void TDiplomDoc::AlignSelection( TAlnH iH, TAlnV iV )
 {
   TVarHolderVal<int> hold( TDrawShape::m_iGlueLevel, 0 );

   if( iH == TL_NoneH && iV == TL_NoneV ) return;

   TDrawShapeList::iterator it = find_if( m_lstDrw.begin(), m_lstDrw.end(), FSel1 );   
   if( it != m_lstDrw.end() )
	{
      if( (*it)->isA() == TT_Ruler )
	   {
         (*it)->m_iSelCount = -1;
		 TDrawShape *p = GetMinID();
		 (*it)->m_iSelCount = 1;
		 if( p )
		  {
            TShpAln_Tool dta( p, iH, iV, GetMainViev() );
	        for_each( m_lstDrw.begin(), m_lstDrw.end(), dta );
		  }
	   }
	  else
	   {
		 TShpAln_Tool dta( (*it).get(), iH, iV, GetMainViev() );
		 for_each( m_lstDrw.begin(), m_lstDrw.end(), dta );
	   }
	}

   //CheckAllGlued();
   //RecalcAllGlued();
   RecalcAllGlued_Selected();
   SetModifiedFlag();
 }
//typedef greater<_Ty> _Pr3;



/*enum TAlnH { TL_Top=0, TL_Cent, TL_Bottom, TL_NoneH };
enum TAlnV { TL_Left=0, TL_Cent, TL_Right, TL_NoneV };
enum TDistrHV { TD_1=0, TD_2, TD_3, TD_4, TD_5, TD_6 };*/

void TShpAln_Tool::operator()( TAuto_DrawShape& arg )
 {
   if( arg.get() == m_pShpBase || !arg->IsSelected() || 
	   arg->isA() == TT_Ruler  ) return;

   if( arg->isA() == TT_Cnct && ((TCnct*)(arg.get()))->IsHaveGlu() )
	 return;

   m_pV->InvalObj( arg.get(), true );

   CRect r( arg->m_position ); 
   arg->ExtendRect( r, 1 );

   switch( m_h )
	{
	  case TL_Top:
	    if( r.top > r.bottom ) 
		  arg->m_position.OffsetRect( 
		    CPoint(0, m_iParamH - r.top) ); 
		else
		  arg->m_position.OffsetRect( 
		    CPoint(0, m_iParamH - r.bottom) ); 		 
		break;
	  case TL_Cent:	   
	    arg->m_position.OffsetRect( 
		  CPoint(0, 
		    m_iParamH - (r.top+r.bottom)/2) );
		break;
	  case TL_Bottom:	   
		if( r.top < r.bottom ) 
		  arg->m_position.OffsetRect( 
		    CPoint(0, m_iParamH - r.top) ); 
		else
		  arg->m_position.OffsetRect( 
		    CPoint(0, m_iParamH - r.bottom) ); 		 
		break;
	}

   switch( m_v )
	{
	  case TL_Left:
	    if( r.left < r.right ) 
		  arg->m_position.OffsetRect( 
		    CPoint(m_iParamV - r.left, 0) ); 
		else
		  arg->m_position.OffsetRect( 
		    CPoint(m_iParamV - r.right, 0) ); 		 
		break;
	  case TL_CentV:	   
	    arg->m_position.OffsetRect( 
		  CPoint(m_iParamV - (r.left+r.right)/2, 0));
		break;
	  case TL_Right:	   
	    if( r.left > r.right ) 
		  arg->m_position.OffsetRect( 
		    CPoint(m_iParamV - r.left, 0) ); 
		else
		  arg->m_position.OffsetRect( 
		    CPoint(m_iParamV - r.right, 0) ); 		 
		break;
	}

   m_pV->InvalObj( arg.get(), true );
 }

TShpAln_Tool::TShpAln_Tool( TDrawShape *pShpBase, TAlnH h, TAlnV v, TDiplomView *pV ):
  m_h( h ), m_v( v ), m_pShpBase( pShpBase ),
  m_pV( pV )
 {
//m_iParamH, m_iParamV;
   CRect r( pShpBase->m_position ); 
   pShpBase->ExtendRect( r, 1 );

   switch( h )
	{
	  case TL_Top:
	    m_iParamH = max( r.top, r.bottom );
		break;
      case TL_Cent:	   
        m_iParamH = (r.top + r.bottom) / 2;
		break; 	   
	  case TL_Bottom:	   
	    m_iParamH = min( r.top, r.bottom );
		break;
	};

   switch( v )
	{
	  case TL_Left:
	    m_iParamV = min( r.left, r.right );
		break;
      case TL_CentV:	   
        m_iParamV = (r.left + r.right) / 2;
		break; 	   
	  case TL_Right:	   
	    m_iParamV = max( r.left, r.right );
		break;
	};
 }

void TDiplomDoc::SetViewMode()
 {
   TDrawShapeList::iterator itSta( m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_lstDrw.end() );

   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->isA() == TT_PC )
	   reinterpret_cast<TDrawPC*>((*itSta).get())->SwitchModes( m_bFlSimpleView );

   RecalcAllGlued();
 }

int TDiplomDoc::CalcSelectedAndGet( bool bFlWithGuids, TDrawShape*& rP )
 {
   TDrawShapeList::iterator itSta( m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_lstDrw.end() );

   int iCnt = 0;
   int iMinInd = INT_MAX; rP = NULL;
   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->IsSelected() && 
	     (bFlWithGuids || (*itSta)->isA() != TT_Ruler)
	   ) 
	  {
	    iCnt++;
		if( iMinInd > (*itSta)->m_iSelCount )
		  iMinInd = (*itSta)->m_iSelCount,
		  rP = (*itSta).get();
	  }

   return iCnt;
 }

int TDiplomDoc::CalcSelected( bool bFlWithGuids )
 {
   TDrawShapeList::iterator itSta( m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_lstDrw.end() );

   int iCnt = 0;
   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->IsSelected() && 
	     (bFlWithGuids || (*itSta)->isA() != TT_Ruler)
	   ) iCnt++;

   return iCnt;
 }

void TDiplomDoc::CheckAllGlued()
 {
   TDrawShapeList::iterator itSta( m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_lstDrw.end() );

   TDiplomView* pV = GetMainViev();
   
   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->isA() != TT_Ruler && (*itSta)->IsActiveGlue() )	     
	   (*itSta)->CheckGlued0( pV );
 }

void TDiplomDoc::RecalcAllGlued()
 {
   TDrawShapeList::iterator itSta( m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_lstDrw.end() );

   TDiplomView* pV = GetMainViev();
   
   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->isA() != TT_Ruler && !(*itSta)->IsActiveGlue() )	     
	   (*itSta)->RecalcGlued( pV, true );
 }

void TDiplomDoc::RecalcAllGlued_Selected()
 {
   TDrawShapeList::iterator itSta( m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_lstDrw.end() );

   TDiplomView* pV = GetMainViev();
   
   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->IsSelected() && (*itSta)->isA() != TT_Ruler && !(*itSta)->IsActiveGlue() )	     
	   (*itSta)->RecalcGlued( pV, true );
 }

TDrawShape* TDiplomDoc::FindInterect( const CPoint& pt )
 {
   TDrawShapeList::reverse_iterator itSta( m_lstDrw.rbegin() );
   TDrawShapeList::reverse_iterator itEnd( m_lstDrw.rend() );      
   
   CRect rDelta( pt.x - 2, pt.y - 2, pt.x + 2, pt.y + 2 );
   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->Intersects(rDelta) ) return (*itSta).get();

   return NULL;
 }

TDrawShape* TDiplomDoc::HitTest2( const CPoint& pt, int& nDragHandle, int& nGlue, TDrawShape *pExc )
 {
   TDrawShapeList::reverse_iterator itSta( m_lstDrw.rbegin() );
   TDrawShapeList::reverse_iterator itEnd( m_lstDrw.rend() );   

   TDiplomView* pV = GetMainViev();
   nDragHandle = nGlue = 0;
   
   for( ; itSta != itEnd; ++itSta )
	 if( !(*itSta)->IsActiveGlue() && (*itSta).get() != pExc )
	  {
		TDrawShape *p = (*itSta)->HitTest( pt, nDragHandle, nGlue, pV, true );
		if( p ) return p;
	  }

   return NULL;
 }

TDrawShape* TDiplomDoc::HitTest0( const CPoint& pt, int& nDragHandle, int& nGlue, TDrawShape *pExc )
 {
   TDrawShapeList::reverse_iterator itSta( m_lstDrw.rbegin() );
   TDrawShapeList::reverse_iterator itEnd( m_lstDrw.rend() );   

   TDiplomView* pV = GetMainViev();
   nDragHandle = nGlue = 0;
   
   for( ; itSta != itEnd; ++itSta )
	 if( !(*itSta)->IsActiveGlue() && (*itSta).get() != pExc )
	  {
		TDrawShape *p = (*itSta)->HitTest( pt, nDragHandle, nGlue, pV, true );
		if( p && nGlue ) return p;
	  }

   return NULL;
 }

void TDiplomDoc::RenumIDs()
 {
   TIndexedUnic::RenumIDs( (TIndexedList&)m_lstDrw );

   TDrawShapeList::iterator itSta( m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_lstDrw.end() );

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

	TLST_Brigada::iterator itSta2( m_lstBrig.begin() );
	TLST_Brigada::iterator itEnd2( m_lstBrig.end() );
	for( ; itSta2 != itEnd2; ++itSta2 )
	  (*itSta2)->RenumPCs();
 }

TPropEdit* TDiplomDoc::VecGetProp( TAP_PropEdit_Vec& rVec, TTypesDrawTool typ )
 {
   for( int i = rVec.size() - 1; i >= 0; --i )
	 if( rVec[ i ]->m_ttEdit.TypeIs() == typ && rVec[i]->m_bFlAvaliable )
	    return rVec[ i ].get();

   return NULL;
 }

void TDiplomDoc::CreateVectorProp( TAP_PropEdit_Vec& rVec )
 {
   TDrawShape *pObj = GetMinID();
   if( !pObj ) return;

   TDrToolItem *pTool = pObj->GetDrTool( TDR_Pen );
   if( pTool )
     rVec.push_back( TAP_PropEdit(new TPropEdit(pTool)) );

   pTool = pObj->GetDrTool( TDR_Brush );
   if( pTool )
     rVec.push_back( TAP_PropEdit(new TPropEdit(pTool)) );

   pTool = pObj->GetDrTool( TDR_Font );
   if( pTool )
     rVec.push_back( TAP_PropEdit(new TPropEdit(pTool)) );

   TDrawShapeList::iterator itSta( m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_lstDrw.end() );

   for( ; itSta != itEnd; ++itSta )	
    if( (*itSta)->IsSelected() ) 
	  for( int i = rVec.size() - 1; i >= 0; --i )
	    if( !(*itSta)->GetDrTool(rVec[ i ]->m_ttEdit.TypeIs()) )
		   rVec[ i ]->m_bFlAvaliable = false;
 }

void TDiplomDoc::AssNewProp( TAP_PropEdit_Vec& rVec, TTypesDrawTool prTyp )
 {
   TDrawShapeList::iterator itSta( m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_lstDrw.end() );

   TDiplomView *pV = GetMainViev();

   for( ; itSta != itEnd; ++itSta )	
    if( (*itSta)->IsSelected() )
	  for( int i = rVec.size() - 1; i >= 0; --i )
	   {
         if( rVec[ i ]->m_bFlAvaliable == false ) continue;
		 if( prTyp != TDR_All && rVec[ i ]->m_ttEdit.TypeIs() != prTyp )
		   continue;

         TDrToolItem *pTool =
		   (*itSta)->GetDrTool( rVec[i]->m_ttEdit.TypeIs() );
		 ASSERT( pTool != NULL && pTool->m_pTool != NULL );
		 if( *pTool->m_pTool == *(rVec[i]->m_ttEdit.m_ap.get()) )
		   continue;

		 *pTool = rVec[i]->m_ttEdit;
		 pV->InvalObj( (*itSta).get(), true );
	   }
 }

void TDiplomDoc::AddBrig( TAP_Brigada ap )
 {   
   ap->CreateIndex( (TIndexedList&)m_lstBrig );
   m_lstBrig.push_back( ap );

   SetModifiedFlag();
 }

void TDiplomDoc::RemoveBrig( DWORD dw )
 {
   TIndexedList::iterator it = 
	  TIndexedUnic::Find_ID_It( dw, (TIndexedList&)m_lstBrig );
   TIndexedList::iterator itEnd = ((TIndexedList&)m_lstBrig).end();

   ASSERT( it != itEnd );

   m_lstBrig.erase( (TLST_Brigada::iterator&)it );

   SetModifiedFlag();
 }

void TDiplomDoc::ServisSelected( int iServ )
 {
   TDrawShapeList::iterator itSta( m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_lstDrw.end() );

   TDiplomView *pV = GetMainViev();

   for( ; itSta != itEnd; ++itSta )	
     if( (*itSta)->IsSelected() && (*itSta)->isA() == TT_PC )
	  {
	    short key = ((TDrawPC*)(*itSta).get())->m_sServis_Brig;
	    ((TDrawPC*)(*itSta).get())->m_sServis_Brig = iServ;
		if( key != iServ )
		  pV->InvalObj( (*itSta).get(), true );
	  }
 }

BOOL TDiplomDoc::OnOpenDocument(LPCTSTR lpszPathName) 
 {
    CString str;
	str.Format( "Загрузка схемы: \"%s\"", lpszPathName );
    GL_SetMsgStr( str );

	BOOL bRes;
	try {
	  bRes = CDocument::OnOpenDocument( lpszPathName );		
	 }
	catch( CException *pE )
	 {
	   GL_RestoreMsg();
	   throw;
	 }
	catch( exception e )
	 {
       GL_RestoreMsg();
	   throw;
	 }

	GL_RestoreMsg();
	
	// TODO: Add your specialized creation code here
	
	return bRes;
 }

BOOL TDiplomDoc::OnSaveDocument(LPCTSTR lpszPathName) 
 {
	// TODO: Add your specialized code here and/or call the base class
    CString str;
	str.Format( "Сохранение схемы: \"%s\"", lpszPathName );
    GL_SetMsgStr( str );
	
	BOOL bRes;
	try {
	  bRes = CDocument::OnSaveDocument( lpszPathName );
	 }
	catch( CException *pE )
	 {
	   GL_RestoreMsg();
	   throw;
	 }
	catch( exception e )
	 {
       GL_RestoreMsg();
	   throw;
	 }

	GL_RestoreMsg();

	return bRes;
 }

BOOL TDiplomDoc::CanCloseFrame( CFrameWnd* pFrame ) 
 {
	// TODO: Add your specialized code here and/or call the base class
    if(	!CDocument::CanCloseFrame(pFrame) ) return FALSE;

	return reinterpret_cast<TChildFrame*>(pFrame)->MyCanClose();
 }

void TDiplomDoc::OnCloseDocument() 
 {
	// TODO: Add your specialized code here and/or call the base class
    if( m_pChFrame && !m_pChFrame->MyCanClose() ) return;
	 
	CDocument::OnCloseDocument();
 }

BOOL TDiplomDoc::My_CanClose()
 {
   if( m_pChFrame )
     return CanCloseFrame( m_pChFrame );      
   else return TRUE;
 }

void TDiplomDoc::Sel_Objs( TSEL_Select sel, int iCl )
 {
   SHORT shC = ::GetKeyState( VK_SHIFT )&0x8000;

   if( sel == TSEL_SelAll || sel == TSEL_SelHigh )
	{
	  TDrawShapeList::iterator itSta( m_lstDrw.begin() );
	  TDrawShapeList::iterator itEnd( m_lstDrw.end() );

	  TDiplomView *pV = GetMainViev();

	  int iCnt = 1;
	  for( ; itSta != itEnd; ++itSta )	
		if( (*itSta)->isA() == iCl && 
		    (sel == TSEL_SelAll || (*itSta)->IsHighLighted()) ) 
		 {
		   (*itSta)->SetSel_Internal( true );
		   (*itSta)->m_iSelCount = iCnt++;
		 }
		else
		 {
		   if( shC && (*itSta)->IsSelected() )
			 (*itSta)->m_iSelCount = iCnt++;
		   else
		     (*itSta)->SetSel_Internal( false );
		 }
	}
   else
	{
	  TDrawShapeList::iterator itSta( m_lstDrw.begin() );
	  TDrawShapeList::iterator itEnd( m_lstDrw.end() );   

	  TDrawShape *p = NULL;
	  for( ; itSta != itEnd; ++itSta )
		if( (*itSta)->IsSelected() && (*itSta)->isA() == TT_PC )
		 {
		   p = (*itSta).get(); break;
		 }

	  if( !p ) 
	   {
		 m_pChFrame->MessageBox( "Нужно выделить любое устройство принадлежащее двухполюснику", "Ошибка", 
			  MB_OK|MB_ICONINFORMATION );
		 return;
	   }

	  ClearDPolus( m_lstDrw );

	  TDoublePolus dbl( (TDrawPC*)p, m_lstDrw, 0, this, iCl == 5 );	  

	  if( dbl.m_exc.m_exc != TE_None )
	   {
		 ReportExcStat( dbl.m_exc );
		 return;
	   }

	  if( iCl == 5 ) iCl = 0;
	  SelectDP( dbl, iCl, shC != 0 );   
	  ClearDPolus( m_lstDrw );
	}

   RenumSelected();
 }

TDrawPC *TDiplomDoc::GetNoDPPc( TDrawShapeList& rL )
 {
   TDrawShapeList::iterator itSta( rL.begin() );
   TDrawShapeList::iterator itEnd( rL.end() );
   
   for( ; itSta != itEnd; ++itSta )	
	 if( (*itSta)->isA() == TT_PC &&  
	     !((TDrawPC*)(*itSta).get())->m_vecBiPolus.size()
	   ) return ((TDrawPC*)(*itSta).get());

   return NULL;
 }

template<> struct std::greater<TAP_TPDP>
 {   
   bool operator()( const TAP_TPDP& a1, const TAP_TPDP& a2 ) const
	{
      return a1->m_iOutRng < a2->m_iOutRng;
	}
 };

bool TDiplomDoc::CreAllDPs( TAP_TPDP_Lst& rL )
 {
   ClearDPolus( m_lstDrw );
   TDrawPC *pPC;
   for( int i = 0; pPC = GetNoDPPc(m_lstDrw); ++i )
	{
      TAP_TPDP ap( new TDoublePolus(pPC, m_lstDrw, i, this, false) );
	  if( ap->m_exc.m_exc != TE_None )
	   {
		 ReportExcStat( ap->m_exc );
		 return false;
	   }

	  rL.push_back( ap );
	}

   std::greater<TAP_TPDP> mgr;    
   rL.sort( mgr );   

   return true;
 }

void TDiplomDoc::ClearLbl()
 {
   TDrawShapeList::iterator itSta( m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_lstDrw.end() );
   
   for( ; itSta != itEnd; ++itSta )	
	 if( (*itSta)->isA() == TT_PC )
	   ((TDrawPC*)(*itSta).get())->m_bLbl = false;
 }

void TDiplomDoc::FillRanges()
 {
   list<TDrawPC*> rL;
   CreRngs( rL );

   typedef list<TDrawPC*>::iterator TIterTyp; 
   TIterTyp itSta = rL.begin();
   TIterTyp itEnd = rL.end();

   list<short> lstRang;

   ClearLbl();

   short sPrivRng = -1;
   TIterTyp itKey;
   for( ; itSta != itEnd; ++itSta )
	{
	  if( (*itSta)->m_sRang != sPrivRng )
	   {
         itKey = itSta; sPrivRng = (*itSta)->m_sRang;
	   }
	  if( (*itSta)->IsSelected() ) (*itKey)->m_bLbl = true;
	}

   TDiplomView *pV = GetMainViev();

   itSta = rL.begin();
   itEnd = rL.end();
   sPrivRng = -1;
   for( ; itSta != itEnd; ++itSta )
	{
	  if( (*itSta)->m_sRang != sPrivRng )
	   {
         itKey = itSta; sPrivRng = (*itSta)->m_sRang;
	   }
	  if( (*itKey)->m_bLbl )  
	   {
	     (*itSta)->AssSameMD( *itKey );
		 (*itSta)->GenerateTTY();
		 pV->InvalObj( *itSta );
	   }
	}

   SetModifiedFlag();
 }


void TDiplomDoc::CreRng()
 {
   list<TDrawPC*> rL;
   CreRngsXY( rL );

   typedef list<TDrawPC*>::iterator TIterTyp; 
   TIterTyp itSta = rL.begin();
   TIterTyp itEnd = rL.end();

   if( !rL.size() ) return;

   short shRng = 1, shNumb = 1;
   int iYbottom = rL.front()->m_position.bottom;
   for( ; itSta != itEnd; ++itSta )
	{
      if( (*itSta)->m_position.top < iYbottom )
	   {
         shRng++; shNumb = 1;
		 iYbottom = (*itSta)->m_position.bottom;
	   }

	  if( (*itSta)->m_sRang != shRng || (*itSta)->m_sNum != shNumb )
	   {
         (*itSta)->m_sRang = shRng;
		 (*itSta)->m_sNum = shNumb;
		 (*itSta)->GenerateTTY();
	   }
	  ++shNumb;
	}

   SetModifiedFlag();
 }

void TDiplomDoc::ScaleSel( double dSc, int iScFrom )
 {
   CPoint pt;
   if( iScFrom == 1 )
     pt = GetSelectionCenter( true );

   TDrawShapeList::iterator itSta( m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_lstDrw.end() );

   for( ; itSta != itEnd; ++itSta )		
      if( (*itSta)->IsSelected() //&&
	      //!((*itSta)->isA() == TT_Cnct && ((TCnct*)(*itSta).get())->IsHaveGlu())
		)
	   {
	     if( iScFrom == 0 ) pt = (*itSta)->m_position.CenterPoint();
	     (*itSta)->ScaleUser( dSc, pt );
		 TTItem_Font *p = (TTItem_Font*)(*itSta)->GetDrTool( TDR_Font );
		 if( p )
		  {
            p->Recreate();
			(**p).m_lf.lfHeight = double((**p).m_lf.lfHeight) * dSc;
            p->Create();
		  }
	   }

   //RecalcAllGlued();
   RecalcAllGlued_Selected();
   SetModifiedFlag();
 }

void TDiplomDoc::CountSelGuid( int& iCntV, int& iCntH, TDrawRuler*& pV, TDrawRuler*& pH )
 {
   iCntV = iCntH = 0;
   pV = pH = NULL;
   TDrawShapeList::iterator itSta( m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_lstDrw.end() );

   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->isA() == TT_Ruler && (*itSta)->IsSelected() )
	  {
        if( ((TDrawRuler*)(*itSta).get())->GetFlag() ) 
		 {
		   ++iCntV;
		   if( !pV ) pV = (TDrawRuler*)(*itSta).get();
		 }
		else 
		 {
		   ++iCntH;
		   if( !pH ) pH = (TDrawRuler*)(*itSta).get();
		 }
	  }
 }

void TDiplomDoc::AlignSelected_AtRuler( TDrawRuler*& pV, TDrawRuler*& pH )
 {   
   TDrawShapeList::iterator itSta( m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_lstDrw.end() );

   TDiplomView* pView = GetMainViev();
   int iV = pV ? pV->m_position.left + 1:0;
   int iH = pH ? pH->m_position.top - 1:0;

   for( ; itSta != itEnd; ++itSta )
    if( (*itSta)->IsSelected() && (*itSta)->isA() != TT_Ruler &&
	    !((*itSta)->isA() == TT_Cnct && ((TCnct*)(*itSta).get())->IsHaveGlu())
	  )
	 {
       int iHOffs = pV ? 
		(iV - min((*itSta)->m_position.left, (*itSta)->m_position.right)):0;
	   int iVOffs = pH ? 
		(iH - max((*itSta)->m_position.top, (*itSta)->m_position.bottom)):0;
	   
	   pView->InvalObj( (*itSta).get(), true );
	   (*itSta)->m_position.OffsetRect( iHOffs, iVOffs );
	   pView->InvalObj( (*itSta).get(), true );
	 }

	RecalcAllGlued_Selected();
 }



/*void TDiplomDoc::SelectDBL()
 {
   TDrawShapeList::iterator itSta( m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_lstDrw.end() );   

   TDrawShape *p = NULL;
   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->IsSelected() && (*itSta)->isA() == TT_PC )
	  {
        p = (*itSta).get(); break;
	  }

   if( !p ) 
	{
      m_pChFrame->MessageBox( "Нужно выделить любое устройство принадлежащее двухполюснику", "Ошибка", 
	       MB_OK|MB_ICONINFORMATION );
	  return;
	}

   ClearDPolus( m_lstDrw );

   TDoublePolus dbl( (TDrawPC*)p, m_lstDrw, 0, this );

   if( dbl.m_exc.m_exc != TE_None )
	{
      ReportExcStat( dbl.m_exc );
	  return;
	}

   SelectDP( dbl );   
   ClearDPolus( m_lstDrw );
 }*/

void TDiplomDoc::Get_GrafErr( TGrafExc& ex, CString& str )
 {   
   str = "";

   switch( ex.m_exc )
	{
	  case TE_HaveBadLinks:
	    str = "В схеме имеются некорректные связи. ";
		break;
	  case TE_HaveHLinks:
	    str = "В схеме имеются некорректные связи. Запрещена связь между элементами одного ранга и со старшего на младший ранг";
		break;
	  case TE_HaveCycles:
	    str = "В схеме имеются циклы";
		break;		
	  case TE_NoTerm:
	    str = "Ошибка в схеме - в двухполюснике не задан выход";
		break;		
	  case TE_NoStart:
	    str = "Не найден вход схемы - пропущены ранги или номера элементов (требуется элемент 1.1)";
		break;		
	  case TE_Assimetric:
	    str.Format( "На ранге [%u] нарушена симметричность схемы", ex.m_iData );
	    //lp = (LPSTR)(LPCSTR)str;
		break;		
	  case TE_IllegalEnter:
	    str = "Схема имеет лишние входы. Допустим только один вход - у вершины 1.1";
		break;		

	  case TE_BadInLink_LotoHi:
	    str.Format( "У элемента [%u.%u] связь с входа на старший ранг", (int)ex.m_iRng, (int)ex.m_iNumb );
	    //lp = (LPSTR)(LPCSTR)str;
		break;		
	  case TE_CanntGoBottom:
	    str.Format( "В элемент [%u.%u] нет входа с младших рангов (допустимо только для 1.1)", (int)ex.m_iRng, (int)ex.m_iNumb );
	    //lp = (LPSTR)(LPCSTR)str;
		break;		
	  case TE_VeryManyIn:
	    str.Format( "У элемента [%u.%u] слишком много входов (можно не более двух)", (int)ex.m_iRng, (int)ex.m_iNumb );
	    //lp = (LPSTR)(LPCSTR)str;
		break;		
	  case TE_11NoMoreSimplify:
	    str = "Достигнут исток схемы - больше нет возможных свёрток";
		break;	
	  case TE_OnlyMultiInElem:
	    str = "Нельзя свернуть - остались только многовходовые элементы";
		break;
      case TE_CanntGoTo:
	     if( ex.m_iRng2 != -1 )
	       str.Format( "Из элемента [%u.%u] нет восходящего пути в [%u.%u]", 
		      (int)ex.m_iRng, (int)ex.m_iNumb, (int)ex.m_iRng2, (int)ex.m_iNumb2 );
		 else
		   str.Format( "Из элемента [%u.%u] нет восходящего пути в младшие ранги", 
		      (int)ex.m_iRng, (int)ex.m_iNumb );
		break;
	  case TE_CanntCreInt:
	    str.Format( "Невозможно создать пересечение для устранения топ.сложности [%u.%u]", 
		      (int)ex.m_iRng, (int)ex.m_iNumb );
	    break;
	  case TE_NoMoreSimplifiedCnct:
	    str.Format( "Преобразование прекращено на [%u.%u] - больше нет сводимых связей", 
		      (int)ex.m_iRng, (int)ex.m_iNumb );
	    break;
	  case TE_IllegalInvalidPtr:
	   str.Format( "Элемент [%u.%u] не содержит ссылки на дескриптор связи", 
		      (int)ex.m_iRng, (int)ex.m_iNumb );
	    break;
	  default:
	    break;
	};
 }

void TDiplomDoc::ReportExcStat( TGrafExc exc )
 {   
   CString str;
   TDiplomDoc::Get_GrafErr( exc, str );   
   m_pChFrame->MessageBox( str, "Ошибка", MB_OK|MB_ICONSTOP );
 }

void TDiplomDoc::UnHiglight( TT_IndTypes typ )
 {
   TDrawShapeList::iterator itSta( m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_lstDrw.end() );   
   
   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->isA() == typ )
	   (*itSta)->UnHighLight();
 }
void TDiplomDoc::ClearDPolusAndUsedFlag( TDrawShapeList& lstDrw )
 {
   TDrawShapeList::iterator itSta( lstDrw.begin() );
   TDrawShapeList::iterator itEnd( lstDrw.end() );   
   
   for( ; itSta != itEnd; ++itSta )
	{
	  if( (*itSta)->isA() == TT_PC )
		((TDrawPC*)(*itSta).get())->ClearBiPolus();
	  else if( (*itSta)->isA() == TT_Point )
		((TDrawPoint*)(*itSta).get())->ClearBiPolus();

	  if( (*itSta)->isA() == TT_Cnct )
	    ((TCnct*)(*itSta).get())->m_bFlMark = false;

	  (*itSta)->ResUsedFlag();
	}
 }
void TDiplomDoc::ClearDPolus( TDrawShapeList& lstDrw )
 {
   TDrawShapeList::iterator itSta( lstDrw.begin() );
   TDrawShapeList::iterator itEnd( lstDrw.end() );   
   
   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->isA() == TT_PC )
	   ((TDrawPC*)(*itSta).get())->ClearBiPolus();
	 else if( (*itSta)->isA() == TT_Point )
	   ((TDrawPoint*)(*itSta).get())->ClearBiPolus();
 }
void TDiplomDoc::ClearUsedFlag( TDrawShapeList& lstDrw )
 {
   TDrawShapeList::iterator itSta( lstDrw.begin() );
   TDrawShapeList::iterator itEnd( lstDrw.end() );   
   
   for( ; itSta != itEnd; ++itSta )
	 (*itSta)->ResUsedFlag();
 }

void TDiplomDoc::SelectDP( TDoublePolus& rDP, int sel, bool bAdd ) //0 - all, 1 - PC, 2 - Links
 {
   list<TDrawShape*>::iterator itSta( rDP.m_lstPC.begin() );
   list<TDrawShape*>::iterator itEnd( rDP.m_lstPC.end() );      
   itEnd--;

   TDiplomView* pView = GetMainViev();
   if( !bAdd ) SelectAll( false );

   if( rDP.m_lstPC.size() == 1 && rDP.m_lstPC.front()->isA() == TT_PC && sel != 2 )
	 rDP.m_lstPC.front()->SetSel_Internal( true ),
	 rDP.m_lstPC.front()->m_iSelCount = 1;

   int iCnt = 1;
   for( ; itSta != itEnd; ++itSta )
	{
      list<TDrawShape*>::iterator itSta2( itSta );	  
      list<TDrawShape*>::iterator itEnd2( rDP.m_lstPC.end() );   

	  if( !(*itSta)->IsSelected() && sel != 2 )
	   {
	     if( !(sel == 1 && (*itSta)->isA() == TT_Point) )
		   (*itSta)->SetSel_Internal( true ),
		   (*itSta)->m_iSelCount = iCnt++;
	   }

	  itSta2++; 
	  /*if( !(*itSta)->IsSelected() )
	   {
		 (*itSta)->SetSel_Internal( true );	  
		 (*itSta)->m_iSelCount = iCnt++;
	   }*/

	  for( ; itSta2 != itEnd2; ++itSta2 )
	   {
	     if( !(*itSta2)->IsSelected() && sel != 2 )
		  {
		    if( !(sel == 1 && (*itSta2)->isA() == TT_Point) )
			  (*itSta2)->SetSel_Internal( true ),
			  (*itSta2)->m_iSelCount = iCnt++;
		  }
		 
		 TCnct *pCnct = (*itSta)->GetLinkTo( *itSta2 );
		 if( sel != 1 && pCnct && !pCnct->IsSelected() )
		  {
			pCnct->SetSel_Internal( true );	  
			pCnct->m_iSelCount = iCnt++;
		  }
	   }	  
	}

   if( sel != 1 )
	{
	  itSta = rDP.m_lstPC.begin();
	  itEnd = rDP.m_lstPC.end();      
	  for( ; itSta != itEnd; ++itSta )
		(*itSta)->SelectEmptyLink( iCnt );	  
	}

   //RenumSelected();
   pView->Invalidate( FALSE );
 }

short TDiplomDoc::CheckConnect()
 {
   TDoublePolus::ResetUsedFlag( m_lstDrw );

   TDrawShape *pSta = TDoublePolus::FindSta( m_lstDrw );
   if( !pSta )
	{
      TGrafExc exc( TE_NoStart );
	  m_pChFrame->m_evDataAccEnable.SetEvent();
	  ReportExcStat( exc );
	  return 2;
	}

   int iOut = -1;
   try {
	  m_b_FndPath_Lbl = true;
	  FindPath( pSta, NULL, NULL, iOut );
	}
   catch( TGrafExc exc )
	{
	  m_pChFrame->m_evDataAccEnable.SetEvent();
      ReportExcStat( exc );
	  return 2;
	}

   TDrawShapeList::iterator itSta( m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_lstDrw.end() );   
   
   short sRet = 1;
   for( ; itSta != itEnd; ++itSta )
	{     
	  bool bFl;
      if( (*itSta)->isA() == TT_PC )
	    bFl = ((TDrawPC*)(*itSta ).get())->IsUsed();
	  else if( (*itSta )->isA() == TT_Point )
	    bFl = ((TDrawPoint*)(*itSta ).get())->IsUsed();
	  //else if( (*itSta )->isA() == TT_Cnct )
	    //bFl = ((TCnct*)(*itSta ).get())->IsUsed();
	  else continue;

	  if( !bFl && ((*itSta)->isA() == TT_PC || (*itSta)->isA() == TT_Point) )
	   {
         iOut = -1;
		 bool bResFnd;
		 try {
			m_b_FndPath_Lbl = false;
			bResFnd = FindPath( (*itSta).get(), NULL, pSta, iOut );
		  }
		 catch( TGrafExc exc )
		  {
		    m_pChFrame->m_evDataAccEnable.SetEvent();
			ReportExcStat( exc );
			return 2;
		  }
		 if( bResFnd )
		  {
            m_b_FndPath_Lbl = true;
			FindPath( (*itSta).get(), NULL, pSta, iOut );
		  }

		 if( (*itSta)->isA() == TT_PC )
		   bFl = ((TDrawPC*)(*itSta ).get())->IsUsed();
		 else if( (*itSta )->isA() == TT_Point )
		   bFl = ((TDrawPoint*)(*itSta ).get())->IsUsed();
	   }

	  if( !bFl )
	   {
         sRet = 0;
		 (*itSta)->HighLight();
		 //GetMainViev()->UpdateWindow();
	   }
	}

   itSta = m_lstDrw.begin();
   itEnd = m_lstDrw.end();   
      
   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->isA() == TT_Cnct && 
	     !((TCnct*)(*itSta ).get())->IsUsed() 
	   )
	  {
        sRet = 0;
		(*itSta)->HighLight();
		//GetMainViev()->UpdateWindow();
	  }

   return sRet;
 }

bool TDiplomDoc::FindPath( TDrawShape *pPC, TDrawShape *pPCSrc, TDrawShape *pStopPC, int& riOut )
 {       
   if( pPC->isA() == TT_PC )	 
	{
	  if( riOut >= ((TDrawPC*)pPC)->m_sRang ) 
	    throw TGrafExc( TE_HaveHLinks );

	  //if( ((TDrawPC*)pPC)->IsUsed() )
	    //throw TGrafExc( TE_HaveBadLinks );

	  riOut = ((TDrawPC*)pPC)->m_sRang;	 
	}

   if( m_b_FndPath_Lbl ) pPC->SetUsedFlag( true );
   //if( pPC->isA() == TT_PC && ((TDrawPC*)pPC)->IsUsed() > 2 )
	 //throw TGrafExc( TE_HaveCycles );

   TCnct *pCnct = NULL;
   bool bFlNoEnter;
   TDrawShape *p = pPC->GoTop( pCnct, pPCSrc, false, bFlNoEnter, m_b_FndPath_Lbl );
   if( !p || p == pStopPC )  
	{
      if( p == pStopPC ) return true;
	  return false;
	}
          
   bool bRet = false;
   for( ; p && p!=pStopPC; p = pPC->GoTop(pCnct, pPCSrc, false, bFlNoEnter, m_b_FndPath_Lbl) )	
	{
	  int iOut = riOut;
	  bool bRes = FindPath( p, pPC, pStopPC, iOut );	  
	  if( bRes ) bRet = true;
	}   

   return bRet;
 }

void TDiplomDoc::UnHilite()
 {
   TDrawShapeList::iterator itSta( m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_lstDrw.end() );   
      
   for( ; itSta != itEnd; ++itSta )
	 (*itSta)->UnHighLight();
 }

void TDiplomDoc::ShowInt( TDrawShape *pShp )
 {
   TAnalyzer analyz( m_lstDrw, this );

   
   TAnalyzer *pA;

   CWaitCursor wc;
   if( !m_pChFrame->m_bFlRun )
	{
	  try {	  
		 analyz.Create();
	   }
	  catch( TGrafExc exc )
	   {
		 wc.Restore();
		 ReportExcStat( exc );
		 return;
	   }
	  pA = &analyz;
	}
   else pA = m_pChFrame->m_apAnalyzer.get();
   wc.Restore();

   TPCConnect *p = ((TDrawPC*)pShp)->m_pTPC;
   if( (p->m_cType != TCT_BiInSofist && p->m_cType != TCT_NotDeterm) )
	{
	  CString str;
      str.Format( "У элемента [%u.%u] нет сложной связи", (int)((TDrawPC*)pShp)->m_sRang, (int)((TDrawPC*)pShp)->m_sNum );

      m_pChFrame->m_evDataAccEnable.SetEvent();

      m_pChFrame->MessageBox( str, "Информация", 
	       MB_OK|MB_ICONINFORMATION );
	  return;
	}   

   if( p->m_cType == TCT_BiInSofist )
	{
      HiliteLstPCs( p->m_lstInt );

	  int iCnt = TDoublePolus::CountPC( p->m_lstInt );
	  CString str;
      str.Format( "Это пересечение путей (n = %u) используемое для устранения топологической сложности", iCnt );

      m_pChFrame->m_evDataAccEnable.SetEvent();

      m_pChFrame->MessageBox( str, "Информация", 
	       MB_OK|MB_ICONINFORMATION );
	}
   else
	{
      TPCConnect *pSibl = pA->GetSibling( p );
	  ClearDPolus( m_lstDrw );
      TDoublePolus dbl( p->m_pPC, m_lstDrw, 0, this, true );
	  TDoublePolus dbl1( pSibl->m_pPC, m_lstDrw, 1, this, true );
	  list<TDrawShape*> lstInt;
	  dbl.Intersect( dbl1, lstInt ); 

	  HiliteLstPCs( lstInt );

	  int iCnt = TDoublePolus::CountPC( lstInt );
	  CString str;
      str.Format( "Это пересечение двухполюсников (n = %u) используемое для устранения многополюсника", iCnt );

      m_pChFrame->m_evDataAccEnable.SetEvent(); 

      m_pChFrame->MessageBox( str, "Информация", 
	       MB_OK|MB_ICONINFORMATION );
	}
 }


void TDiplomDoc::ShowCnctType( TDrawShape *pShp )
 {
   TAnalyzer analyz( m_lstDrw, this );
   TAnalyzer *pA;

   CWaitCursor wc;
   if( !m_pChFrame->m_bFlRun )
	{	  
	  try {	  
		 analyz.Create();
	   }
	  catch( TGrafExc exc )
	   {
		 wc.Restore();
		 ReportExcStat( exc );
		 return;
	   }
	  pA = &analyz;
	}
   else pA = m_pChFrame->m_apAnalyzer.get();


   TPCConnect *p = ((TDrawPC*)pShp)->m_pTPC;
   ASSERT( p != NULL );

   CString str;
   str.Format( "У элемента [%u.%u] ", (int)((TDrawPC*)pShp)->m_sRang, (int)((TDrawPC*)pShp)->m_sNum );
   CString str2;
   switch( p->m_cType )
	{
	  case TCT_Parall:
	    str2.Format( "параллельная связь" );
		break;
	  case TCT_Seqv:
	    str2.Format( "последовательная связь" );
		break;
	  case TCT_FullAccess:
	    str2.Format( "полнодоступная связь" );
		break;
	  case TCT_NotDeterm:
	    str2.Format( "неопределённая связь" );
		break;
	  case TCT_BiIn:
	    str2.Format( "много-входовая связь" );
		break;
	  case TCT_BiInSofist:
	    str2.Format( "много-входовая топологически сложная связь" );
		break;
	  case TCT_11:
	    str2.Format( "нет связи - это вход схемы" );
		break;	   
	};

   str += str2; str2 = "";

   switch( p->m_cType )
	{	  
	  case TCT_Seqv:	    
	   {
	     TPCConnect *pSibl = pA->GetSibling( p );
	     str2.Format( " с [%u.%u]", (int)pSibl->m_pPC->m_sRang, (int)pSibl->m_pPC->m_sNum );
		 HiliteCnct( p ); 
		 break;
	   }
	  case TCT_Parall:
	  case TCT_FullAccess:
	  case TCT_NotDeterm:
	   {
	     TPCConnect *pSibl = pA->GetSibling( p );
	     str2.Format( " с [%u.%u]", (int)pSibl->m_pPC->m_sRang, (int)pSibl->m_pPC->m_sNum );
		 HiliteCnct( p ); HiliteCnct( pSibl );
		 break;
	   }	  	  
	  case TCT_BiIn:	    	  
	  case TCT_11:
	    str2.Format( "" );
		HiliteCnct( p );
		break;	   
	  case TCT_BiInSofist:	    
	    str2.Format( " M = %u, N = %u", (int)p->m_M, (int)p->m_N );
		HiliteCnct( p );
		break;	   
	};

   str += str2;

   wc.Restore();

   m_pChFrame->m_evDataAccEnable.SetEvent();

   m_pChFrame->MessageBox( str, "Информация", 
	       MB_OK|MB_ICONINFORMATION );
 }

void TDiplomDoc::HiliteLstPCs( list<TDrawShape*>& rLst, bool bFlHilite )
 {   
   list<TDrawShape*>::iterator itSta( rLst.begin() );
   list<TDrawShape*>::iterator itEnd( rLst.end() );      
   itEnd--;

   TDiplomView* pView = GetMainViev();   

   //list<TDrawShape*>& rLst = pCnct->m_pats.m_lst;

   if( rLst.size() == 1 )
	{
	  if( bFlHilite )
	    rLst.front()->HighLight();
	  else rLst.front()->UnHighLight();
	}
   
   for( ; itSta != itEnd; ++itSta )
	{
      list<TDrawShape*>::iterator itSta2( itSta );	  
      list<TDrawShape*>::iterator itEnd2( rLst.end() );   

	  if( bFlHilite ) (*itSta)->HighLight();	  
	  else (*itSta)->UnHighLight();

	  itSta2++;
	  /*if( !(*itSta)->IsSelected() )
	   {
		 (*itSta)->SetSel_Internal( true );	  
		 (*itSta)->m_iSelCount = iCnt++;
	   }*/

	  for( ; itSta2 != itEnd2; ++itSta2 )
	   {
	     if( bFlHilite ) (*itSta2)->HighLight();
		 else (*itSta2)->UnHighLight();
		 
		 TCnct *pCnct = (*itSta)->GetLinkTo( *itSta2 );
		 if( pCnct ) 
		  {
		    if( bFlHilite ) pCnct->HighLight();
			else pCnct->UnHighLight();
		  }
	   }
	}
 }

void TDiplomDoc::HiliteCnct( TPCConnect* pCnct, bool bFlHilite )
 {
   if( !pCnct ) return;

   pCnct->m_pats.m_lst.push_front( pCnct->m_pPC );

   HiliteLstPCs( pCnct->m_pats.m_lst, bFlHilite );
   
   pCnct->m_pats.m_lst.erase( pCnct->m_pats.m_lst.begin() );
   //pView->Invalidate( FALSE );   
 }

int TDiplomDoc::CalcPCAndPoints( TDrawShapeList& rLst )
 {
   TDrawShapeList::iterator itSta( rLst.begin() );
   TDrawShapeList::iterator itEnd( rLst.end() );   
     
   int iCnt = 0;
   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->isA() == TT_PC || (*itSta)->isA() == TT_Point )
	   ++iCnt;

   return iCnt;
 }

void TDiplomDoc::ShowNPaths( TDrawShape *pShp )
 {   
   TAnalyzer analyz( m_lstDrw, this );
   TAnalyzer *pA;

   if( !m_pChFrame->m_bFlRun )
	{
	  CWaitCursor wc;

	  try {	  
		 analyz.Create();
	   }
	  catch( TGrafExc exc )
	   {
		 wc.Restore();
		 ReportExcStat( exc );
		 return;
	   }
	  wc.Restore();
	  pA = &analyz;
	}
   else pA = m_pChFrame->m_apAnalyzer.get();

   CString str; 
   str.Format( "Параметр отношения восходящей непосредственной достижимости = %u",
	 pShp->GetLocked() );

   m_pChFrame->m_evDataAccEnable.SetEvent();
   m_pChFrame->MessageBox( str, "Информация", 
	       MB_OK|MB_ICONINFORMATION );

 }
void TDiplomDoc::SimplifyT( TDrawShape *pShp )
 {
   TAnalyzer analyz( m_lstDrw, this );

   CWaitCursor wc;

   try {	  
	  analyz.Create();
	}
   catch( TGrafExc exc )
	{
	  wc.Restore();
      ReportExcStat( exc );
	  return;
	}
   wc.Restore();

   TPCConnect *p = ((TDrawPC*)pShp)->m_pTPC;

   if( p->m_cType != TCT_Parall && p->m_cType != TCT_Seqv &&
	   p->m_cType != TCT_FullAccess
	 )
    {
      m_pChFrame->MessageBox( "Этот элемент тривиально не сводится", "Информация", 
	       MB_OK|MB_ICONINFORMATION );
	  return;
	}
   analyz.Simplify( p );
 }


void TDiplomDoc::FlipObjects()
 {
   TDrawShapeList::iterator itSta( m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_lstDrw.end() );   
     
   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->isA() != TT_Ruler )
	  {
	    (*itSta)->m_position.top *= -1;
		(*itSta)->m_position.bottom *= -1;
	  }
 }

void TDiplomDoc::ShiftObjects( CPoint cp )
 {
   TDrawShapeList::iterator itSta( m_lstDrw.begin() );
   TDrawShapeList::iterator itEnd( m_lstDrw.end() );   
     
   for( ; itSta != itEnd; ++itSta )
	 if( (*itSta)->isA() != TT_Ruler )
       (*itSta)->m_position.OffsetRect( cp );
 }

