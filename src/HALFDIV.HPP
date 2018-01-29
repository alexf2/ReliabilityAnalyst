#if !defined(_HALF_DIVISION_)
#define _HALF_DIVISION_

class TNoRoot {};

struct TExtraData
 {
 };

typedef double THalfFunction( double x, TExtraData* pExtDta );


double HalfDivizion( THalfFunction* pF, TExtraData* pD,
							double a, double b, double eps
						 );

double Newton( THalfFunction* pF, THalfFunction* pFp1, TExtraData* pD,
					double d0, double dEps
				 );

#endif

