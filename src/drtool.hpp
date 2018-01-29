#if !defined(_DRAWTOOLS_)
#define _DRAWTOOLS_

#include "indexeditem.hpp"
#include "stdafx.h"
#include <memory>
#include <list>
#include <vector>
using namespace std;

#include <afxmt.h>

struct TDrTool_Data
 {
 };
typedef auto_ptr<TDrTool_Data> TAP_DrToolData;

#include "en_prop.hpp"

class TBaseDrTool;
typedef auto_ptr<TBaseDrTool> TAP_BaseDrTool;
typedef list<TAP_BaseDrTool> TAP_BaseDrTool_List;

class TBaseDrTool: public TIndexedUnic
 {
public:
  TBaseDrTool():
	TIndexedUnic()
	 {
	   m_d = NULL; m_iRefCnt = 0;
	   ++TBaseDrTool::m_iCntInst;
	 }

   virtual ~TBaseDrTool();

   int AddRef()
	{
	  return ++m_iRefCnt;
	}
   int ReleaseRef()
	{
      if( m_iRefCnt > 0 ) m_iRefCnt--;
	  return m_iRefCnt;
	}
   bool CanRemove() const
	{
      return m_iRefCnt == 0;
	}

   virtual void AR_Store( CArchive& rA );
   virtual void AR_Load( CArchive& rA );

   virtual void Create() = 0;
   virtual TBaseDrTool* CreateCopy() = 0;
   virtual TBaseDrTool* Copy_Content() = 0;
   virtual int operator==( const TBaseDrTool& );
   virtual int GetSizeData() const = 0;

   CGdiObject* Get() const { return m_apObj.get(); }  
   TDrTool_Data* Data() { return m_d; }

   virtual int isA() const
	{
      return TDR_Base;
	}

   typedef auto_ptr<CGdiObject> TAP_GdiObj;

   TAP_GdiObj m_apObj;

   static int m_iCntInst;
   	
protected:
   TDrTool_Data *m_d;
   int m_iRefCnt;
 };


struct TPen_Data: public TDrTool_Data
 {
   TPen_Data()
	{
      m_iStyle = PS_SOLID; m_iWidth = 1;
	  m_clr = RGB(0,0,0);	  
	}

   int m_iStyle, m_iWidth;
   COLORREF m_clr;
 };

class TPenDrTool: public TBaseDrTool
 {
public:
  TPenDrTool():
	TBaseDrTool()
	 {
	   m_d = new TPen_Data();
	 }

   virtual ~TPenDrTool();
   
   virtual void Create();
   virtual TBaseDrTool* CreateCopy();
   virtual TBaseDrTool* Copy_Content();
   virtual int GetSizeData() const;

   CPen* Get() const { return reinterpret_cast<CPen*>(m_apObj.get()); }  
   TPen_Data* Data() { return (TPen_Data*)m_d; }

   virtual int isA() const
	{
      return TDR_Pen;
	}
 };


struct TBr_Data: public TDrTool_Data
 {
   TBr_Data()
	{
      m_iIndex = HS_BDIAGONAL;
	  m_clrBk = m_clr = RGB(255,255,255);	  
	  m_solid = BS_SOLID;
	}

   int m_iIndex;
   COLORREF m_clr, m_clrBk;
   UINT m_solid;
 };

class TBrushDrTool: public TBaseDrTool
 {
public:
  TBrushDrTool():
	TBaseDrTool()
	 {
	   m_d = new TBr_Data();
	 }

   virtual ~TBrushDrTool();
   
   virtual void Create();
   virtual TBaseDrTool* CreateCopy();
   virtual TBaseDrTool* Copy_Content();
   virtual int GetSizeData() const;

   CBrush* Get() const { return reinterpret_cast<CBrush*>(m_apObj.get()); }  
   TBr_Data* Data() { return (TBr_Data*)m_d; }

   virtual int isA() const
	{
      return TDR_Brush;
	}
 };


struct TFont_Data: public TDrTool_Data
 {
   TFont_Data()
	{      
	  m_clr = RGB(0,0,0);
	  memcpy( &m_lf, &TFont_Data::lp_Init, sizeof(LOGFONT) ); 
	  m_dSc = 1;
	}

   int Weight()
	{
      return double(m_lf.lfHeight) * m_dSc;
	}
   
   COLORREF m_clr;
   LOGFONT m_lf;
   double m_dSc;

   static LOGFONT lp_Init;
 };

class TFontDrTool: public TBaseDrTool
 {
public:
  TFontDrTool():
	TBaseDrTool()
	 {
	   m_d = new TFont_Data();
	 }

   virtual ~TFontDrTool();
   
   virtual void Create();
   virtual TBaseDrTool* CreateCopy();
   virtual TBaseDrTool* Copy_Content();
   virtual int GetSizeData() const;

   CFont* Get() const { return reinterpret_cast<CFont*>(m_apObj.get()); }  
   TFont_Data* Data() { return (TFont_Data*)m_d; }

   virtual int isA() const
	{
      return TDR_Font;
	}
 };


class TDrToolsManeger
 {
public:
  TDrToolsManeger():
	 m_mtxLock( FALSE )
   {
   }

  ~TDrToolsManeger();

  TBaseDrTool* GetTool( int iInd )
   {
     CSingleLock lock( &m_mtxLock, TRUE );

     TBaseDrTool *pTool = (TBaseDrTool*)TIndexedUnic::Find_ID( iInd, (TIndexedList&)m_lst );
	 if( pTool ) pTool->AddRef();
	 return pTool;
   }
  bool ReleaseTool( int iInd );

  TBaseDrTool* GetTool( TBaseDrTool*, int& rIndex );

  void RecreateTools( bool bPen, bool bBr, bool bFont );

protected:
  CMutex m_mtxLock;
  TAP_BaseDrTool_List m_lst;
 };

class TDrToolItem;
typedef auto_ptr<TDrToolItem> TAP_DrItem;
typedef vector<TAP_DrItem> TDrItem_vec;

class TDiplomDoc;
class TChildFrame;

class TDrToolItem
 {

friend class TDiplomDoc;
friend class TChildFrame;

public:
  TDrToolItem( TDrToolsManeger& rMgr ):
	m_ap( NULL ),   
	m_rMgr( rMgr )
	 {
	   m_pTool = NULL; m_ind = -1;
	 }
   TDrToolItem( const TDrToolItem& rI ):
	 m_rMgr( rI.m_rMgr ),
	 m_ap( NULL )
	{
	  m_pTool = NULL; m_ind = -1;
	  operator=( rI );
	}
   TDrToolItem& operator=( const TDrToolItem& rI )	 
	{
      //m_ap = rI.m_ap;
	  //m_pTool = rI.m_pTool;
	  //m_ind = rI.m_ind;
	  Kill();
	  //ASSERT( rI.m_ap.get() != NULL );
	  m_ap = TAP_BaseDrTool( rI.m_ap.get() ? rI.m_ap->Copy_Content():
	   rI.m_pTool->Copy_Content() );
	  Create();
	  return *this;
	}

   ~TDrToolItem()
	{
      Kill(); 
	}

   void AR_Store( CArchive& rA );
   void AR_Load( CArchive& rA );

   void Create()
	{
	  ASSERT( m_pTool == NULL && m_ind == -1 );
      m_pTool = m_rMgr.GetTool( m_ap.get(), m_ind );
	  delete m_ap.release(); m_ap = TAP_BaseDrTool( NULL );
	}

   void Kill()
	{
      if( m_pTool && m_ind != -1 )
	   {
         m_rMgr.ReleaseTool( m_ind );
		 m_pTool = NULL; m_ind = -1;
	   }
	}

   void Recreate()
	{
	  ASSERT( m_pTool && m_ind != -1 );
      m_ap = TAP_BaseDrTool( m_pTool->Copy_Content() );
	  Kill();
	}

   TTypesDrawTool TypeIs()
	{
	  return (TTypesDrawTool)(m_pTool ? m_pTool->isA():m_ap->isA());
	}

   bool CmpProp( TDrToolItem& rI )
	{
      return *m_ap == *rI.m_ap;
	}

protected:
  TAP_BaseDrTool m_ap;

  TBaseDrTool *m_pTool;
  int m_ind;

  TDrToolsManeger& m_rMgr;
 };

template<class TDrObj_cl, class TDrData, class TGdiObjTyp> class TTItem: public TDrToolItem
 {
public:
   TTItem( TDrToolsManeger& rMgr ):
	  TDrToolItem( rMgr )
	   {
	     m_ap = TAP_BaseDrTool( (TBaseDrTool*)new TDrObj_cl() );
	   }
   TTItem( const TTItem& rI ):
     TDrToolItem( rI )
	{
	}

   TGdiObjTyp* operator()()
	{
      return (TGdiObjTyp*)(((TDrObj_cl*)m_pTool)->Get());
	}

   TDrData& operator*()
	{
	  ASSERT( m_ap.get() != NULL );
      return *(TDrData*)(((TDrObj_cl*)m_ap.get())->Data());
	}

   TDrData& GetData()
	{
      ASSERT( m_pTool != NULL );
	  return *(TDrData*)(((TDrObj_cl*)m_pTool)->Data());
	}

   TTItem& operator=( const TTItem& rI )
	{
	  TDrToolItem::operator=( (const TDrToolItem&)rI );
      return *this;
	}
 };

typedef TTItem<TPenDrTool, TPen_Data, CPen>  TTItem_Pen;
typedef TTItem<TBrushDrTool, TBr_Data, CBrush>  TTItem_Brush;
typedef TTItem<TFontDrTool, TFont_Data, CFont>  TTItem_Font;

#endif
