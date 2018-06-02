
 double f__cabs(double, double);

static double c_abs(scomplex *z)
{
return( f__cabs( z->r, z->i ) );
}
