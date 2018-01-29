#if !defined(_CRow_ListView_)
#define _CRow_ListView_

// RListVw.h : interface of the CRowListView class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1997 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "listvwex.h"
#include "diplomDoc.h"

class CRowListView : public CListViewEx
{

friend class TChildFrame;

protected: // create from serialization only	
	DECLARE_DYNCREATE(CRowListView)

// Attributes
public:
    CRowListView();  

	TDiplomDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRowListView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRowListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
   afx_msg LRESULT OnCommandHelp( WPARAM, LPARAM lParam );
   afx_msg LRESULT OnCommandHelpF1( WPARAM, LPARAM lParam );

// Generated message map functions
protected:
	//{{AFX_MSG(CRowListView)
	afx_msg void OnViewSmallIcons();
	afx_msg void OnViewLargeIcons();
	afx_msg void OnViewList();
	afx_msg void OnViewDetails();
	afx_msg void OnViewFullRowDetails();
	afx_msg void OnUpdateViewSmallIcons(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewLargeIcons(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewList(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewDetails(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewFullRowDetails(CCmdUI* pCmdUI);
	afx_msg void OnViewStateIcons();
	afx_msg void OnUpdateViewStateIcons(CCmdUI* pCmdUI);
	afx_msg void OnViewClientWidthSel();
	afx_msg void OnUpdateViewClientWidthSel(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// list view image lists
protected:
	CImageList m_LargeImageList;
	CImageList m_SmallImageList;
	CImageList m_StateImageList;

// view type helpers
public:
	BOOL SetViewType(DWORD dwViewType);
	DWORD GetViewType();

// flags and states
protected:
	BOOL m_bStateIcons;
	int m_nCheckedItem;
	HICON m_hSmallIcon;
	HICON m_hLargeIcon;

	void CheckItem(int nNewCheckedItem);
};


/////////////////////////////////////////////////////////////////////////////
#endif

