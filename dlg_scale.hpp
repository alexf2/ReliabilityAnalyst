#if !defined(_TSCALE_DLG_)
#define _TSCALE_DLG_


#include "stdafx.h"
#include "resource.h"

class TScaleDlg: public CDialog
{
// Construction
public:
	TScaleDlg( CWnd* pParent, CPoint& rp ):
	  CDialog( IDD_DLG_SCALE, pParent ),
	  m_rPt( rp )
	   { 
	     m_iSc = 2;
		 m_iUpDownd = 0;
		 m_dRes = 2;
		 m_iCenter =1;
	   }


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual void OnOK();
	virtual BOOL OnInitDialog();

	afx_msg void OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar );

public:    	
  int m_iSc;
  int m_iUpDownd;
  int m_iCenter;

  double m_dRes;

  CSliderCtrl m_sl;
  CEdit m_edSc;

  CPoint &m_rPt;

  DECLARE_MESSAGE_MAP()
 };

#endif
