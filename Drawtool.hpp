#ifndef __DRAWTOOL_H__
#define __DRAWTOOL_H__

#include "drawobj.h"

#include <list>
#include <memory>
using namespace std;

#include "getgdimgr.hpp"

class CDrawTool;
class TDrawShape;
typedef auto_ptr<CDrawTool> TAuto_DrawTool;

enum DrawShape
 {
   DS_selection	
 };


class TFndTool_Dta
 {
public:
  TFndTool_Dta( DrawShape shp ):
    m_shp( shp )
	 {
	 }

  int operator()( TAuto_DrawTool& arg );
   

protected:
  DrawShape m_shp;
 };


class TDiplomView;

class CDrawTool
 {
// Constructors
public:
	CDrawTool( DrawShape nDrawShape );
	virtual ~CDrawTool();

// Overridables
	virtual void OnLButtonDown(TDiplomView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDblClk(TDiplomView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(TDiplomView* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(TDiplomView* pView, UINT nFlags, const CPoint& point);
	virtual void OnEditProperties(TDiplomView* pView);

	virtual void OnCancel( TDiplomView* pView );
	virtual HCURSOR ToolGetCursor();

	virtual void OnKeyDown( int, CView* );
	virtual void OnKeyUp( int, CView* );
    
	virtual void OnSetCursor();
	HCURSOR m_hcCurrent;

	virtual void PreScroll( TDiplomView* pView, CPoint ptOff );
	virtual void PostScroll( TDiplomView* pView );

// Attributes
	DrawShape m_drawShape;	    

	static CDrawTool* FindTool( DrawShape drawShape );
	static list<TAuto_DrawTool> m_lstTools;
	static CPoint c_down;
	static UINT c_nDownFlags;
	static CPoint c_last;	

	static DrawShape c_drawShape;

	bool IsDragging() const { return m_bDragging; }

protected:
  TDrawShape *m_pActionShp;
  bool m_bDragging;  
  bool m_bHaveScrolled;
  
};

class CSelectTool : public CDrawTool
 {
// Constructors
public:
    enum TDragDirect { TD_None, TD_Hor, TD_Vert };

	CSelectTool();
	virtual ~CSelectTool();

// Implementation
	virtual void OnLButtonDown(TDiplomView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDblClk(TDiplomView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(TDiplomView* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(TDiplomView* pView, UINT nFlags, const CPoint& point);
	virtual void OnEditProperties(TDiplomView* pView);

	virtual void OnCancel( TDiplomView* pView );
	//virtual HCURSOR ToolGetCursor();

	virtual void OnKeyDown( int, CView* );
	virtual void OnKeyUp( int, CView* );
 

	virtual void PreScroll( TDiplomView* pView, CPoint ptOff );

protected:
  TDragDirect m_drDir;

  bool m_bAtObj;
 };


////////////////////////////////////////////////////////////////////////////

#endif // __DRAWTOOL_H__
