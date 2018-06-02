



#if defined(SANDYBRIDGE)
#include "sger_microk_sandy-2.c"
#endif

int CNAME(BLASLONG m, BLASLONG n, BLASLONG dummy1, FLOAT alpha,
	 FLOAT *x, BLASLONG incx,
	 FLOAT *y, BLASLONG incy,
	 FLOAT *a, BLASLONG lda, FLOAT *buffer){

	FLOAT *X    = x;

	if (incx != 1) {
    		X = buffer;
    		COPY_K(m, x, incx, X, 1);
	}

	BLASLONG m1 = m & -16;

	while (n > 0) 
	{
   		FLOAT y0 = alpha * *y;
		if ( m1 > 0 )
		{
			#ifdef HAVE_KERNEL_16
				sger_kernel_16(m1, X, a, &y0);
			#else
    				AXPYU_K(m1, 0, 0, y0, X, 1, a, 1, NULL, 0);
			#endif
		}
	
		if ( m > m1 )
		{
    			AXPYU_K(m-m1, 0, 0, y0, X+m1 , 1, a+m1, 1, NULL, 0);
		}
	
    		a += lda;
    		y += incy;
    		n --;
	}

	return 0;
}

