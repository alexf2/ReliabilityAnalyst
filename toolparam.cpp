#include "toolparam.hpp"

auto_ptr<EnumVal_Int> Line_EnumPattern()
 {
   auto_ptr<EnumVal_Int> ap( new TEnumValues<int>() );

   *ap, PS_SOLID, PS_DASH, PS_DOT, PS_DASHDOT, PS_DASHDOTDOT, PS_NULL;

   return ap;
 }
auto_ptr<EnumVal_Int> Line_EnumThick()
 {
   auto_ptr<EnumVal_Int> ap( new TEnumValues<int>() );

   *ap, 1, 3, 5, 7, 9;

   return ap;
 }

auto_ptr<EnumVal_Int> Fill_EnumPattern()
 {
   auto_ptr<EnumVal_Int> ap( new TEnumValues<int>() );

   *ap, -2, -1, HS_BDIAGONAL, HS_CROSS, HS_DIAGCROSS, HS_FDIAGONAL,
	 HS_HORIZONTAL, HS_VERTICAL;

   return ap;
 }

auto_ptr<EnumVal_Clr> Common_EnumColor()
 {
   auto_ptr<EnumVal_Clr> ap( new TEnumValues<COLORREF>() );

   *ap, 
	 RGB(0,0,0), RGB(255,255,255), RGB(255,0,0), RGB(0,255,0),
	 RGB(0,0,255), RGB(255,255,0), RGB(255,0,255), RGB(0,255,255),
	 RGB(128,0,0), RGB(0,128,0), RGB(0,0,128), RGB(128,128,0),
	 RGB(128,0,128), 
	 
	 RGB(192,192,192), RGB(230,230,230),
	 RGB(154,154,154), RGB(102,102,102);

   return ap;
 }

auto_ptr<EnumVal_Clr> BkGnd_EnumColor()
 {
   auto_ptr<EnumVal_Clr> ap( new TEnumValues<COLORREF>() );

   *ap, -1,
	 RGB(0,0,0), RGB(255,255,255), RGB(255,0,0), RGB(0,255,0),
	 RGB(0,0,255), RGB(255,255,0), RGB(255,0,255), RGB(0,255,255),
	 RGB(128,0,0), RGB(0,128,0), RGB(0,0,128), RGB(128,128,0),
	 RGB(128,0,128), 
	 
	 RGB(192,192,192), RGB(230,230,230),
	 RGB(154,154,154), RGB(102,102,102);

   return ap;
 }
