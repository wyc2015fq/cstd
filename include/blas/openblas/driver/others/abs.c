


double fabs(double);
double sqrt(double);

#ifdef NEED_F2CCONV
double
#else
FLOAT
#endif
 CNAME(FLOAT *z){

  FLOAT real = z[0];
  FLOAT imag = z[1];
  double temp;

 real = fabs(real);
 imag = fabs(imag);

if(imag > real){
	temp = real;
	real = imag;
	imag = temp;
}

 if (imag == 0.) return real;

 temp = imag/real;
 temp = real * sqrt(1.0 + temp*temp);

 return temp;

}
