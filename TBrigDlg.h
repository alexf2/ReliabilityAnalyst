#if !defined(AFX_TBRIGDLG_H__352224C2_EBF5_11D1_9887_00504E02C39D__INCLUDED_)
#define AFX_TBRIGDLG_H__352224C2_EBF5_11D1_9887_00504E02C39D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TBrigDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// TBrigDlg dialog

#include "stdafx.h"
#include "brigad.hpp"

class TBrigDlg: public CDialog
{
// Construction
public:
	TBrigDlg( CWnd* pParent, TBrigada* pBr, CPoint& rp ):
	  CDialog( IDD_DLG_BRIG, pParent ),
	  m_rPt( rp )
	   {
         m_iOA = pBr->m_iOA;
		 m_tdsDIS = pBr->m_tdsDIS;
		 m_pBr = pBr;
		 m_iDISId = IDtoInd( pBr->m_tdsDIS );
	   }

// Dialog Data
	//{{AFX_DATA(TBrigDlg)
	enum { IDD = IDD_DLG_BRIG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TBrigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	virtual void OnOK();
	virtual BOOL OnInitDialog();

    int m_iOA; //ремонтники
    TDisOfServis m_tdsDIS;

	int IDtoInd( TDisOfServis );
    TDisOfServis IndtoID( int );

// Implementation
protected:
    CPoint &m_rPt;
	TBrigada *m_pBr;

	CSpinButtonCtrl m_spNumb;

	int m_iDISId;
	// Generated message map functions
	//{{AFX_MSG(TBrigDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TBRIGDLG_H__352224C2_EBF5_11D1_9887_00504E02C39D__INCLUDED_)
