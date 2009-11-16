

#include "xlines.h"


int lines_intersect(double x1, double y1,   /* First line segment */
                    double x2, double y2,

                    double x3, double y3,   /* Second line segment */
                    double x4, double y4,

                    double &x,
                    double &y )
{
    double a1, a2, b1, b2, c1, c2; /* Coefficients of line eqns. */
    double r1, r2, r3, r4;         /* 'Sign' values */
    double denom, offset, num;     /* Intermediate values */

    /* Compute a1, b1, c1, where line joining points 1 and 2
     * is "a1 x  +  b1 y  +  c1  =  0".
     */

    a1 = y2 - y1;
    b1 = x1 - x2;
    c1 = x2 * y1 - x1 * y2;

    /* Compute r3 and r4.
     */


    r3 = a1 * x3 + b1 * y3 + c1;
    r4 = a1 * x4 + b1 * y4 + c1;

    /* Check signs of r3 and r4.  If both point 3 and point 4 lie on
     * same side of line 1, the line segments do not intersect.
     */

    if ( r3 != 0 &&
         r4 != 0 &&
         SAME_SIGNS( r3, r4 ))
        return ( DONT_INTERSECT );

    /* Compute a2, b2, c2 */

    a2 = y4 - y3;
    b2 = x3 - x4;
    c2 = x4 * y3 - x3 * y4;

    /* Compute r1 and r2 */

    r1 = a2 * x1 + b2 * y1 + c2;
    r2 = a2 * x2 + b2 * y2 + c2;

    /* Check signs of r1 and r2.  If both point 1 and point 2 lie
     * on same side of second line segment, the line segments do
     * not intersect.
     */

    if ( r1 != 0 &&
         r2 != 0 &&
         SAME_SIGNS( r1, r2 ))
        return ( DONT_INTERSECT );

    /* Line segments intersect: compute intersection point. 
     */

    denom = a1 * b2 - a2 * b1;
    if ( denom == 0 )
        return ( COLLINEAR );
    offset = denom < 0 ? - denom / 2 : denom / 2;

    /* The denom/2 is to get rounding instead of truncating.  It
     * is added or subtracted to the numerator, depending upon the
     * sign of the numerator.
     */

    num = b1 * c2 - b2 * c1;
    x = ( num < 0 ? num - offset : num + offset ) / denom;

    num = a2 * c1 - a1 * c2;
    y = ( num < 0 ? num - offset : num + offset ) / denom;

    return ( DO_INTERSECT );

} /* lines_intersect */

/* A main program to test the function.

main()
{
    long x1, x2, x3, x4, y1, y2, y3, y4;
    long x, y;

    for (;;) {
        printf( "X1, Y1: " );
	scanf( "%ld %ld", &x1, &y1 );
        printf( "X2, Y2: " );
	scanf( "%ld %ld", &x2, &y2 );
        printf( "X3, Y3: " );
	scanf( "%ld %ld", &x3, &y3 );
        printf( "X4, Y4: " );
	scanf( "%ld %ld", &x4, &y4 );

        switch ( lines_intersect( x1, y1, x2, y2, x3, y3, x4, y4, &x, &y )) {
            case DONT_INTERSECT:
			 printf( "Lines don't intersect\n" );
			 break;
            case COLLINEAR:
                         printf( "Lines are collinear\n" );
                         break;
            case DO_INTERSECT:
			 printf( "Lines intersect at %ld,%ld\n", x, y );
                         break;
            }
        }
    }

 */

