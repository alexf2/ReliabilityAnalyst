#if !defined(_DLG_PROPERTYS_)
#define _DLG_PROPERTYS_

#include "stdafx.h"
#include "toolparam.hpp"
#include "drtool.hpp"
#include "hint_typ.hpp"

#include "resource.h"

#include "..\\RUDAKOW\\SERVIS\\borctl.hpp"

enum TCC_Cbns { TCC_LinePattern, TCC_LineWeight, TCC_Color,
                TCC_FillPattern 
 };

class TColorCombo: public CComboBox
 {
public:
  TColorCombo( TCC_Cbns typ ): CComboBox()
   {
     m_typ = typ; m_dwExtra2 = m_dwExtra1 = 0;
   }

  virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
  virtual void MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct );

  void DrawContent( CDC& dc, CRect& r, LPDRAWITEMSTRUCT lpD );

  COLORREF m_dwExtra1, m_dwExtra2;

protected:
  TCC_Cbns m_typ;
 };


class TPRopPage_PC: public CPropPageTooltips
 {
public:  
  TPRopPage_PC( TDrawPC* pPC );
	
  virtual ~TPRopPage_PC();

  virtual BOOL OnInitDialog();  
  virtual void DoDataExchange( CDataExchange* pDX );    

  virtual void OnOK();
  
protected:  
  int m_sRang, m_sNum;
  double m_dLambda, m_dM, m_dD;
  CString m_csName;

  CSpinButtonCtrl m_sp1, m_sp2;  

  TDrawPC* m_pPC;
 };

class TPRopPage_Text: public CPropPageTooltips
 {
public:  
  TPRopPage_Text( TDrawTxt* pPC );
	
  virtual ~TPRopPage_Text();

  virtual BOOL OnInitDialog();  
  virtual void DoDataExchange( CDataExchange* pDX );    

  virtual void OnOK();
  
protected:    
  CString m_csTxt;
  int m_iMulti;

  TDrawTxt* m_pPC;
 };


class TPRopPage_Line: public CPropPageTooltips
 {
public:  
  TPRopPage_Line( TPropEdit* pPC );
	
  virtual ~TPRopPage_Line();

  virtual BOOL OnInitDialog();  
  virtual void DoDataExchange( CDataExchange* pDX );    

  virtual void OnOK();
  
protected:  
  int m_patt, m_w;
  COLORREF m_clr;

  int m_iInd_patt, m_iInd_w, m_iInd_clr;

  auto_ptr<EnumVal_Int> apPattern, apThick;
  auto_ptr<EnumVal_Clr> apClr;

  TColorCombo m_cbnPat, m_cbnWeight, m_cbnClr;

  TPropEdit* m_pPC;

  void OnSelClr();
  void OnClrExt();

  DECLARE_MESSAGE_MAP()
 };

class TPRopPage_Fill: public CPropPageTooltips
 {
public:  
  TPRopPage_Fill( TPropEdit* pPC );
	
  virtual ~TPRopPage_Fill();

  virtual BOOL OnInitDialog();  
  virtual void DoDataExchange( CDataExchange* pDX );    

  virtual void OnOK();
  
protected:  
  int m_iIndex;
  COLORREF m_clr, m_clrBk;
     
   //short m_solid;

  int m_iInd_ind, m_iInd_clr, m_iInd_clrBk;

  auto_ptr<EnumVal_Int> apIndex;
  auto_ptr<EnumVal_Clr> apClr, apClrBk;

  TColorCombo m_cbnInd, m_cbnClr, m_cbnClrBk;

  TPropEdit* m_pPC;

  void OnSelClr();
  void OnSelClrBk();
  void OnClrExt();
  void OnClrExtBk();

  DECLARE_MESSAGE_MAP()
 };

class TPropSheet: public TBWCCPropertySheet
 {   
public:  
  TPropSheet( CPoint& rPt, TH_HintPage hnt, LPCTSTR pszCaption, CWnd *pParentWnd = NULL );
    
  virtual ~TPropSheet();    

  virtual BOOL DestroyWindow();
  //afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
  virtual BOOL OnInitDialog();

protected:
  CPoint& m_rptIni;
  TH_HintPage m_hnt;

  int FindPage( TH_HintPage hp );

  DECLARE_MESSAGE_MAP()
 };


#endif
