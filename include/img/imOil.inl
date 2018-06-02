
#include <stdlib.h>
#include <string.h>

#define BINS  256



void* memcpy2d(void* dst, int dl, const void* src, int sl, int h, int w);

int imOilImpl(int height, int width, unsigned char* src, int src_step,
    unsigned char* dst, int dst_step, int cn, int N)
{
  int x, y, z, dx, dy, mfp;
  int histo[BINS];

  for (z = 0; z < cn; z++) {
    for (y = N; y < height - N; ++y) {
      for (x = N; x < width - N; ++x) {
        memset(histo, 0, BINS * sizeof(int));

        for (dy = y - N; dy <= y + N; dy++) {
          for (dx = x - N; dx <= x + N; dx++) {
            unsigned char t = *(src + dy * src_step + dx * cn + z);
            histo[t]++;
          }
        }

        for (dx = dy = 0; dx < BINS; dx++) {
          if (histo[dx] > dy) {
            dy = histo[dx];
            mfp = dx;
          }
        }

        dst[y * dst_step + x * cn + z] = (unsigned char) mfp;
      }
    }
  }

  return 1;
}

int imOil(int height, int width, unsigned char* src, int src_step, int cn, int N)
{
  unsigned char* data_cpy;

  if (2 * N + 1 > width || 2 * N + 1 > height) {
    return 0;
  }

  data_cpy = (unsigned char*) pmalloc(cn * width * height * sizeof(unsigned char));

  if (data_cpy == NULL) {
    return 0;
  }

  memcpy2d(data_cpy, width * 3, src, src_step, height, width * 3);
  imOilImpl(height, width, data_cpy, width * 3, src, src_step, cn, N);
  pfree(data_cpy);
  return 1;
}
