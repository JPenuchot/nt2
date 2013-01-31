/*							sinhl.c
 *
 *	Hyperbolic sine, long double precision
 *
 *
 *
 * SYNOPSIS:
 *
 * long double x, y, cephes_sinhl();
 *
 * y = cephes_sinhl( x );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns hyperbolic sine of argument in the range MINLOGL to
 * MAXLOGL.
 *
 * The range is partitioned into two segments.  If |x| <= 1, a
 * rational function of the form x + x**3 P(x)/Q(x) is employed.
 * Otherwise the calculation is sinh(x) = ( exp(x) - exp(-x) )/2.
 *
 *
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE       -2,2       10000       1.5e-19     3.9e-20
 *    IEEE     +-10000      30000       1.1e-19     2.8e-20
 *
 */

/*
Cephes Math Library Release 2.7:  January, 1998
Copyright 1984, 1991, 1998 by Stephen L. Moshier
*/

#include "mconf.h"

#ifdef UNK
static long double P[] = {
 1.7550769032975377032681E-6L,
 4.1680702175874268714539E-4L,
 3.0993532520425419002409E-2L,
 9.9999999999999999998002E-1L,
};
static long double Q[] = {
 1.7453965448620151484660E-8L,
-5.9116673682651952419571E-6L,
 1.0599252315677389339530E-3L,
-1.1403880487744749056675E-1L,
 6.0000000000000000000200E0L,
};
#endif

#ifdef IBMPC
static short P[] = {
0xec6a,0xd942,0xfbb3,0xeb8f,0x3feb, XPD
0x365e,0xb30a,0xe437,0xda86,0x3ff3, XPD
0x8890,0x01f6,0x2612,0xfde6,0x3ff9, XPD
0x0000,0x0000,0x0000,0x8000,0x3fff, XPD
};
static short Q[] = {
0x4edd,0x4c21,0xad09,0x95ed,0x3fe5, XPD
0x4376,0x9b70,0xd605,0xc65c,0xbfed, XPD
0xc8ad,0x5d21,0x3069,0x8aed,0x3ff5, XPD
0x9c32,0x6374,0x2d4b,0xe98d,0xbffb, XPD
0x0000,0x0000,0x0000,0xc000,0x4001, XPD
};
#endif

#ifdef MIEEE
static long P[] = {
0x3feb0000,0xeb8ffbb3,0xd942ec6a,
0x3ff30000,0xda86e437,0xb30a365e,
0x3ff90000,0xfde62612,0x01f68890,
0x3fff0000,0x80000000,0x00000000,
};
static long Q[] = {
0x3fe50000,0x95edad09,0x4c214edd,
0xbfed0000,0xc65cd605,0x9b704376,
0x3ff50000,0x8aed3069,0x5d21c8ad,
0xbffb0000,0xe98d2d4b,0x63749c32,
0x40010000,0xc0000000,0x00000000,
};
#endif

extern long double MAXNUML, MAXLOGL, MINLOGL, LOGE2L;
#ifdef ANSIPROT
extern long double cephes_fabsl ( long double );
extern long double cephes_expl ( long double );
extern long double cephes_polevll ( long double, long double *, int );
extern long double cephes_p1evll ( long double, long double *, int );
#else
long double cephes_fabsl(), cephes_expl(), cephes_polevll(), cephes_p1evll();
#endif
#ifdef INFINITIES
extern long double INFINITYL;
#endif
#ifdef NANS
extern long double NANL;
#endif

long double cephes_sinhl(x)
long double x;
{
long double a;

#ifdef MINUSZERO
if( x == 0.0 )
	return(x);
#endif
a = cephes_fabsl(x);
if( (x > (MAXLOGL + LOGE2L)) || (x > -(MINLOGL-LOGE2L) ) )
	{
	cephes_mtherr( "sinhl", DOMAIN );
#ifdef INFINITIES
	if( x > 0.0L )
		return( INFINITYL );
	else
		return( -INFINITYL );
#else
	if( x > 0.0L )
		return( MAXNUML );
	else
		return( -MAXNUML );
#endif
	}
if( a > 1.0L )
	{
	if( a >= (MAXLOGL - LOGE2L) )
		{
		a = cephes_expl(0.5L*a);
		a = (0.5L * a) * a;
		if( x < 0.0L )
			a = -a;
		return(a);
		}
	a = cephes_expl(a);
	a = 0.5L*a - (0.5L/a);
	if( x < 0.0L )
		a = -a;
	return(a);
	}

a *= a;
return( x + x * a * (cephes_polevll(a,(long double*)P,3)/cephes_polevll(a,(long double*)Q,4)) );
}
