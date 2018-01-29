#if !defined(_CLIP_SZ_)
#define _CLIP_SZ_

#include "stdafx.h"

struct Line
  {
    Line( const CRect& r )
	 {
       x1 = r.left; y1 = r.top;
	   x2 = r.right; y2 = r.bottom;
	 }

	Line( const CPoint& p1, const CPoint& p2 )
	 {
       x1 = p1.x; y1 = p1.y;
	   x2 = p2.x; y2 = p2.y;
	 }

    int x1, y1,
        x2, y2;
  };

struct PicsF_
  {
    float x, y;
  };

struct BarSpec
  {
    BarSpec( const CRect& r )
	 {
       x1 = r.left; y1 = r.bottom;
	   x2 = r.right; y2 = r.top;
	 }

    int x1, y1,
        x2, y2;
  };

int ClipSozKoe( Line& pline, BarSpec& clipper );

#endif
