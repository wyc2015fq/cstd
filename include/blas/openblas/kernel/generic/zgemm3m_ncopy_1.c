



#ifndef USE_ALPHA
#define REAL_PART(a, b)  (a)
#define IMAGE_PART(a, b) (b)
#else
#define REAL_PART(a, b)  (alpha_r * (a) - alpha_i * (b))
#define IMAGE_PART(a, b) (alpha_i * (a) + alpha_r * (b))
#endif

#if defined(REAL_ONLY)
#define CMULT(a, b) (REAL_PART(a, b))
#elif defined(IMAGE_ONLY)
#define CMULT(a, b) (IMAGE_PART(a, b))
#else
#define CMULT(a, b) (REAL_PART(a, b) + IMAGE_PART(a, b))
#endif

int CNAME(BLASLONG m, BLASLONG n, FLOAT *a, BLASLONG lda,
#ifdef USE_ALPHA
	   FLOAT alpha_r, FLOAT alpha_i,
#endif
	   FLOAT *b){

  BLASLONG i;

  FLOAT *a_offset, a1, a2;

  lda *= 2;

  while (n > 0) {
    a_offset = a;
    a += lda;

    for (i = 0; i < m; i ++) {

      a1 = *(a_offset + 0);
      a2 = *(a_offset + 1);

      *(b +  0) = CMULT(a1, a2);

      a_offset += 2;

      b ++;
    }
    n --;
  }

  return 0;
}
