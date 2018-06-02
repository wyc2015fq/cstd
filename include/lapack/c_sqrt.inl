

 double f__cabs(double, double);
static void c_sqrt(scomplex *r, scomplex* z)
{
        double mag, t;
        double zi = z->i, zr = z->r;

        if( (mag = f__cabs(zr, zi)) == 0.)
                r->r = r->i = 0.;
        else if(zr > 0)
        {
                t = sqrt(0.5 * (mag + zr) );
                r->r = (real)t;
                t = zi / t;
                r->i = (real)(0.5 * t);
                }
        else
                {
                t = sqrt(0.5 * (mag - zr) );
                if(zi < 0)
                        t = -t;
                r->i = (real)t;
                t = zi / t;
                r->r = (real)(0.5 * t);
                }
        }
