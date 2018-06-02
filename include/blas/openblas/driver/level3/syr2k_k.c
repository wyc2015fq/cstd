



#ifndef KERNEL_FUNC
#ifndef LOWER
#define KERNEL_FUNC SYR2K_KERNEL_U
#else
#define KERNEL_FUNC SYR2K_KERNEL_L
#endif
#endif

static __inline int syrk_beta(BLASLONG m_from, BLASLONG m_to, BLASLONG n_from, BLASLONG n_to, FLOAT *alpha, FLOAT *c, BLASLONG ldc) {

  BLASLONG i;

#ifndef LOWER
  if (m_from > n_from) n_from = m_from;
  if (m_to   > n_to  ) m_to   = n_to;
#else
  if (m_from < n_from) m_from = n_from;
  if (m_to   < n_to  ) n_to   = m_to;
#endif

  c += (m_from + n_from * ldc) * COMPSIZE;

  m_to -= m_from;
  n_to -= n_from;

  for (i = 0; i < n_to; i++){

#ifndef LOWER

    SCAL_K(MIN(i + n_from - m_from + 1, m_to), 0, 0, alpha[0],
#ifdef COMPLEX
	   alpha[1],
#endif
	   c, 1, NULL, 0, NULL, 0);

    c += ldc * COMPSIZE;

#else

    SCAL_K(MIN(m_to - i + m_from - n_from, m_to), 0, 0, alpha[0],
#ifdef COMPLEX
	 alpha[1],
#endif
	 c, 1, NULL, 0, NULL, 0);

    if (i < m_from - n_from) {
      c += ldc * COMPSIZE;
    } else {
      c += (1 + ldc) * COMPSIZE;
    }
#endif

  }

  return 0;
}

#ifdef THREADED_LEVEL3
#include "level3_syr2k_threaded.c"
#else
#include "level3_syr2k.c"
#endif
