#include <math.h>
#include "Halfdiv.hpp"

const double dHalfZero = 1.0E-7;

double HalfDivizion( THalfFunction* pF, TExtraData* pD,
							double a, double b, double eps
						 )
 {
	double x = (a + b) / 2.0;
	eps *= 2;

	while( fabs(a-b) >= eps )
	 {
		if( a > b )
		 {
			double c = a; a = b; b = c;
		 }

		double fA = pF(a, pD), fB = pF(b, pD), fX = pF(x, pD);

		//if( b - a < dHalfZero ) return (a + b) / 2.0;
		if( fA*fB > 0 )
		  throw TNoRoot();

		if( fA*fX <= 0 ) b = x;
		else a = x;
		x = (a + b) / 2.0;
	 }

	return x;
 }

double Newton( THalfFunction* pF, THalfFunction* pFp1, TExtraData* pD,
					double d0, double dEps
				 )
 {
	double dX;
	do
	 {

		dX = d0 - pF(d0, pD)/pFp1(d0, pD);
		if( fabs(d0 - dX) < dEps ) break;

		d0 = dX;

	 } while(1);

	return dX;
 }

