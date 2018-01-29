#if !defined(_ALNDLG_)
#define _ALNDLG_

#include "stdafx.h"
#include "resource.h"

class TAlnDlg: public CDialog
 {
public:
  TAlnDlg( CWnd *pPar, CPoint& rPt, int iCntSel ):
    CDialog( IDD_DLG_ALN, pPar ),
	m_rptIni( rPt ),
	m_iCntSel( iCntSel )
   {
     m_bAD = 0;
	 m_iAln_H = 1; m_iAln_V = 3;
     m_iDistr_HV = 0;
   }

  virtual BOOL OnInitDialog();
  virtual void DoDataExchange( CDataExchange* pDX );
  virtual void OnOK();

  afx_msg HBRUSH OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor );
    
  int m_bAD;
  int m_iAln_H, m_iAln_V;
  int m_iDistr_HV;

private:      
  CButton m_bA_Hor[4], m_bA_Ver[4];
  CBitmap m_bmpsA[8];  
  CButton m_bD_Hor[3], m_bD_Ver[3];
  CBitmap m_bmpsD[6];      

  CPoint& m_rptIni;

  int m_iCntSel;

  DECLARE_MESSAGE_MAP()
 };

#endif
