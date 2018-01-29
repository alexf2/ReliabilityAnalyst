#include "stdafx.h"
#include "drawtool.hpp"
#include "diplomView.h"

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CDrawTool implementation

//CPtrList CDrawTool::c_tools;
list<TAuto_DrawTool> CDrawTool::m_lstTools;

CPoint CDrawTool::c_down;
UINT CDrawTool::c_nDownFlags;
CPoint CDrawTool::c_last;
DrawShape CDrawTool::c_drawShape = DS_selection;

#include <algorithm>
using namespace std;

CDrawTool::CDrawTool( DrawShape drawShape )
 {
   m_drawShape = drawShape;
   m_bHaveScrolled = m_bDragging = false;
   m_hcCurrent = AfxGetApp()->LoadStandardCursor( IDC_ARROW );
   //c_tools.AddTail(this);
   m_lstTools.push_back( TAuto_DrawTool(this) );

   m_pActionShp = NULL;   
 }

CDrawTool::~CDrawTool()
 {
   //OnCancel();
 }

void CDrawTool::PreScroll( TDiplomView* pView, CPoint ptOff )
 {
   m_bHaveScrolled = true;
 }

void CDrawTool::PostScroll( TDiplomView* pView )
 {
   m_bHaveScrolled = false;
 }

CDrawTool* CDrawTool::FindTool( DrawShape drawShape )
 {
/*
	POSITION pos = c_tools.GetHeadPosition();
	while (pos != NULL)
	{
		CDrawTool* pTool = (CDrawTool*)c_tools.GetNext(pos);
		if (pTool->m_drawShape == drawShape)
			return pTool;
	}

	return NULL;*/
  TFndTool_Dta dta( drawShape );
  list<TAuto_DrawTool>::iterator itFnd = 
    find_if( m_lstTools.begin(), m_lstTools.end(), dta );
  
  return itFnd == m_lstTools.end() ? NULL:(*itFnd).get();
 }

void CDrawTool::OnLButtonDown( TDiplomView* pView, UINT nFlags, const CPoint& point )
 {	
   pView->SetCapture();
   c_nDownFlags = nFlags;
   c_down = point;
   c_last = point;

   m_bDragging = true;
 }

void CDrawTool::OnLButtonDblClk( TDiplomView* /*pView*/, UINT /*nFlags*/, const CPoint& /*point*/ )
 {
 }

void CDrawTool::OnLButtonUp( TDiplomView* /*pView*/, UINT /*nFlags*/, const CPoint& point )
 {
	ReleaseCapture();

	if( point == c_down )
	  CDrawTool::c_drawShape = DS_selection;

	m_bDragging = false;
 }

HCURSOR CDrawTool::ToolGetCursor()
 {
   return m_hcCurrent;
 }

void CDrawTool::OnSetCursor()
 {   
   if( GetCursor() != m_hcCurrent )  SetCursor( m_hcCurrent );
 }

void CDrawTool::OnMouseMove( TDiplomView* /*pView*/, UINT /*nFlags*/, const CPoint& point )
 {
   c_last = point;
   //::SetCursor( ToolGetCursor() );
   OnSetCursor();
 }

void CDrawTool::OnEditProperties( TDiplomView* /*pView*/ )
 {
 }

void CDrawTool::OnCancel( TDiplomView* pView )
 {
	CDrawTool::c_drawShape = DS_selection;
	/*if( m_bDragging )
	 {
	   ReleaseCapture();
	   m_bDragging = false;
	 }*/
 }

////////////////////////////////////////////////////////////////////////////
// CResizeTool

enum SelectMode
{
	none,
	netSelect,
	move,
	size
};

SelectMode selectMode = none;
int nDragHandle, nGlue;

CPoint lastPoint;

CSelectTool::CSelectTool()
	: CDrawTool( DS_selection )
{
  m_drDir = TD_None;
  m_bAtObj = false;
}

CSelectTool::~CSelectTool()
 {
 }

void CSelectTool::OnLButtonDown( TDiplomView* pView, UINT nFlags, const CPoint& point )
 {
	CPoint local = point;
	pView->ClientToDoc_SC( &local );

	TDrawShape* pObj;
	selectMode = none;

	// Check for resizing (only allowed on single selections)
	
		/*pObj = pView->m_selection.GetHead();
		nDragHandle = pObj->HitTest(local, pView, TRUE);
		if (nDragHandle != 0)
			selectMode = size;*/
	m_pActionShp = pObj = pView->HitTest( local, nDragHandle, nGlue );
	if( pObj ) pObj->FixPos();
	if( nDragHandle != 0 ) 
	 {
	   selectMode = size;	
	   GL_SetMsgStr( "Для фиксации координатной оси удерживайте \"SHIFT\", для дублирования - перед началом операции \"CTRL\"" );
	 }
    else if( pObj != NULL )
	 {
       GL_SetMsgStr( "Для фиксации координатной оси удерживайте \"SHIFT\", для дублирования - перед началом операции \"CTRL\"" );      

	   selectMode = move;

	   //if( !pView->IsShpSelected(pObj) )
	   pView->SelectShp( pObj, (nFlags & MK_SHIFT) != 0 );

	   // Ctrl+Click clones the selection...

	   if( (nFlags & MK_CONTROL) != 0 )
		 pView->CloneShpSelection();
	 }

	if( selectMode == move )
	  TDrawShape::m_iGlueLevel = 
	    pView->CalcSelected(false) == 1 ? 2:0;
	else if( selectMode == size )
	  TDrawShape::m_iGlueLevel = 2;
	 
	// Click on background, start a net-selection
	if( selectMode == none )
	 {
	   if( (nFlags & MK_SHIFT) == 0 )
		  pView->SelectShp( NULL );

	   selectMode = netSelect;

	   GL_SetMsgStr( "Групповое выделение с включением границ" );

	   CClientDC dc( pView );
	   CRect rect( point.x, point.y, point.x, point.y );
	   rect.NormalizeRect();
	   dc.DrawFocusRect( rect );
	 }

	lastPoint = local;

	CDrawTool::OnLButtonDown( pView, nFlags, point );
 }

void CSelectTool::OnLButtonDblClk(TDiplomView* pView, UINT nFlags, const CPoint& point)
 {	
   CPoint local = point;
   pView->ClientToDoc_SC( &local );
   TDrawShape* pObj = pView->ObjectAt( local );
   if( pObj != NULL ) 
	{
	  if( (nFlags & MK_SHIFT) != 0 ) pView->Deselect( pObj );
	  else pView->EditText( pObj );
	}

	CDrawTool::OnLButtonDblClk( pView, nFlags, point );
 }

void CSelectTool::OnEditProperties( TDiplomView* pView )
 {
 }

void CSelectTool::OnLButtonUp( TDiplomView* pView, UINT nFlags, const CPoint& point )
 {
   TDrawShape::m_iGlueLevel = 0;

   if( pView->GetCapture() == pView )
	{
      GL_RestoreMsg();

	  if( selectMode == netSelect )
	   {
		 CClientDC dc( pView );
		 CRect rect( c_down.x, c_down.y, c_last.x, c_last.y );
		 rect.NormalizeRect();
		 dc.DrawFocusRect( rect );

		 pView->SelectWithinRect( rect, (nFlags & MK_SHIFT) != 0 );
	   }
	  else if( selectMode != none )
	   {
	     if( m_pActionShp ) m_pActionShp->SetInve( false );
		 pView->CastSelectionInve( false, true );
		 pView->GetDocument()->UpdateMainView( pView );
		 m_pActionShp = NULL;
		 
		 if( selectMode == move ) pView->GetDocument()->CheckAllGlued();
		 pView->GetDocument()->RecalcAllGlued();
	   }
	}

    m_drDir = TD_None;

	CDrawTool::OnLButtonUp( pView, nFlags, point );
 }

inline int sign( int iArg )
 {
   return iArg < 0 ? -1:1;
 }

void CSelectTool::OnMouseMove( TDiplomView* pView, UINT nFlags, const CPoint& point )
 {
   m_hcCurrent = AfxGetApp()->LoadStandardCursor( IDC_ARROW );

   if( pView->GetCapture() != pView )
	{
	  bool bFlHavSel = pView->HaveSelected();
	  CPoint local = point;
	  pView->ClientToDoc_SC( &local );
	  TDrawShape* pObj = NULL;
	  int nHandle;
	  pObj = pView->HitTest( local, nHandle, nGlue );
	  if( CDrawTool::c_drawShape == DS_selection && bFlHavSel )
	   {
		 //TDrawShape* pObj = pView->m_selection.GetHead();
		 
		 //int nHandle = pObj->HitTest( local, pView, TRUE );
		 		 		 
		 if( pObj && nHandle != 0 )
		  {
			//SetCursor(  );
            m_hcCurrent = pObj->GetHandleCursor( nHandle, false );
		    //OnSetCursor();
			CDrawTool::OnMouseMove( pView, nFlags, point );
			return; // bypass CDrawTool
		  }
	   }

	  if( pObj ) 
	   {
	     pObj->ChCursor( m_hcCurrent );
		 if( !m_bAtObj )
		  {            
			if( pObj->isA() == TT_PC ) 
			 {
			   m_bAtObj = true;
			   //CString str; ((TDrawPC*)pObj)->GetInfoStr( str );
			   GL_SetMsgStr( ((TDrawPC*)pObj)->GetInfoStr() );
			 }
		  }		 
	   }
	  else if( m_bAtObj )
	   {
         m_bAtObj = false; GL_RestoreMsg();
	   }

	  if( CDrawTool::c_drawShape == DS_selection )	   
	   {
	     if( pObj && nFlags&MK_CONTROL ) 
		   m_hcCurrent = AfxGetApp()->LoadCursor( IDC_CURSOR_ADD );
		 CDrawTool::OnMouseMove( pView, nFlags, point );
	   }
	   
	  return;
	}

	if( selectMode == netSelect ) 
	 {
	   CClientDC dc(pView);
	   CRect rect(c_down.x, c_down.y, c_last.x, c_last.y);
	   rect.NormalizeRect();
	   if( !m_bHaveScrolled ) dc.DrawFocusRect(rect);	   
	   rect.SetRect(c_down.x, c_down.y, point.x, point.y);
	   rect.NormalizeRect();
	   dc.DrawFocusRect(rect);

	   CDrawTool::OnMouseMove(pView, nFlags, point);
	   return;
	 }

	CPoint local = point;
	pView->ClientToDoc_SC( &local );
	CPoint delta = (CPoint)(local - lastPoint);
	
    if( selectMode == move )	
	 {
	   /*CRect position = pObj->m_position;
	   position += delta;*/
	   if( (nFlags&MK_SHIFT) )
		{
		  if( m_drDir == TD_None )
		   {
			 if( !delta.x ) m_drDir = TD_Vert;
			 else m_drDir = TD_Hor;
		   }

		  int k;
		  if( abs(delta.x) > abs(delta.y) ) k = delta.x;
		  else k = delta.y;
		  k = abs( k );

		  if( m_drDir == TD_Hor ) 
		    delta.x = sign(delta.x) * k, delta.y = 0;
		  else
		    delta.y = sign(delta.y) * k, delta.x = 0;		   
		}
	   else m_drDir = TD_None;

	   pView->CastOnSelection_MoveTo( delta );
	 }
    else if( nDragHandle != 0 )	 
	  pView->MoveHandleTo( nDragHandle, local, m_pActionShp, nFlags&MK_SHIFT );	 
	/*POSITION pos = pView->m_selection.GetHeadPosition();
	while( pos != NULL )	
	 {
	   TDrawShape* pObj = pView->m_selection.GetNext( pos );
	   CRect position = pObj->m_position;

	   if( selectMode == move )
		{
		  position += delta;
		  pObj->MoveTo( position, pView );
		}
	   else if( nDragHandle != 0 )
		{
		  pObj->MoveHandleTo( nDragHandle, local, pView );
		}
	 }*/

	lastPoint = local;

	if( selectMode == size && CDrawTool::c_drawShape == DS_selection )
	 {
	   c_last = point;
	   //SetCursor( pView->GetHeadSelected()->GetHandleCursor(nDragHandle) );
	   //m_hcCurrent = pView->GetHeadSelected()->GetHandleCursor( nDragHandle );
	   m_hcCurrent = m_pActionShp->GetHandleCursor( nDragHandle, true );
	   CDrawTool::OnMouseMove( pView, nFlags, point );
	   //OnSetCursor();
	   return; // bypass CDrawTool
	 }

	c_last = point;

	if( CDrawTool::c_drawShape == DS_selection )
	  CDrawTool::OnMouseMove( pView, nFlags, point );
 }

void CSelectTool::OnCancel( TDiplomView* pView )
 {   
   if( m_bDragging )
	{
	  TDrawShape::m_iGlueLevel = 0;
	  pView->GetDocument()->RecalcAllGlued();	  
	  CPoint pt; GetCursorPos( &pt ); pView->ScreenToClient( &pt );
	  BYTE b = 0;
	  //::GetKeyboardState( &b );
	  OnLButtonUp( pView, b, pt );
	}
   GL_RestoreMsg();
   CDrawTool::OnCancel( pView );
 }

int TFndTool_Dta::operator()( TAuto_DrawTool& arg )
 {
   return arg->m_drawShape == m_shp;
 }

void CDrawTool::OnKeyDown( int, CView* )
 {
   OnSetCursor();
 }
void CDrawTool::OnKeyUp( int, CView* )
 {
   OnSetCursor();
 }

void CSelectTool::OnKeyDown( int iKey, CView *pV_ )
 { 
   TDiplomView *pV = (TDiplomView*)pV_;

   if( pV->GetCapture() != pV && iKey == VK_CONTROL )
	{
	  CPoint local;
	  GetCursorPos( &local ); pV->ScreenToClient( &local );
	  pV->ClientToDoc_SC( &local );

	  TDrawShape* pObj;
	  int nDragHandle;
	  m_pActionShp = pObj = pV->HitTest( local, nDragHandle, nGlue );
	  if( !nDragHandle && pObj )
		m_hcCurrent = AfxGetApp()->LoadCursor( IDC_CURSOR_ADD );

	  CDrawTool::OnKeyDown( iKey, pV );
	}
 }
void CSelectTool::OnKeyUp( int iKey, CView *pV_ )
 {
   TDiplomView *pV = (TDiplomView*)pV_;

   if( pV->GetCapture() != pV && iKey == VK_CONTROL )
	{
	  CPoint local;
	  GetCursorPos( &local ); pV->ScreenToClient( &local );
	  pV->ClientToDoc_SC( &local );

	  TDrawShape* pObj;
	  int nDragHandle;
	  m_pActionShp = pObj = pV->HitTest( local, nDragHandle, nGlue );
	  if( !nDragHandle && pObj )
		m_hcCurrent = AfxGetApp()->LoadStandardCursor( IDC_ARROW );

	  CDrawTool::OnKeyUp( iKey, pV );
	}
 }

void CSelectTool::PreScroll( TDiplomView* pView, CPoint ptOff )
 {
   CDrawTool::PreScroll( pView, ptOff );

   if( selectMode == netSelect )
	{
	  CClientDC dc( pView );
	  CRect rect( c_down.x, c_down.y, c_last.x, c_last.y );
	  rect.NormalizeRect();
	  dc.DrawFocusRect( rect );   

	  c_down.Offset( ptOff );
	}
 }
