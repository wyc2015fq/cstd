# ffmpeg做fft - qq229873466的博客 - CSDN博客

2017年09月26日 11:59:38[qq229873466](https://me.csdn.net/qq229873466)阅读数：200


```cpp
#define inline _inline

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libavcodec/fft.h"

int main()
{
    float a[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };

    FFTComplex *tab;
    FFTContext s;

    int i;
    int do_inverse = 0;
    int fft_nbits = 3, fft_size;

    fft_size = 1 << fft_nbits;
    tab = av_malloc_array(fft_size, sizeof(FFTComplex));

    if (!(tab))
        goto cleanup;

    ff_fft_init(&s, fft_nbits, do_inverse);

    for (i = 0; i < fft_size; i++) {
        tab[i].re = a[i];
        tab[i].im = 0;
    }

    s.fft_permute(&s, tab);
    s.fft_calc(&s, tab);
    ff_fft_end(&s);

    for (i = 0; i < fft_size; i++)
    {
        printf("%f %fi\n", tab[i].re, tab[i].im);
    }
    
cleanup:
    av_free(tab);

    return 0;
}
```

