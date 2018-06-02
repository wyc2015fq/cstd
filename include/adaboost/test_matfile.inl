
#include "macro.h"
#include "cstd.h"
#include "filemat.h"

int test_matfile()
{
  typedef short T;
  int rows = 1000, cols = 1000 * sizeof(T);
  int i, j;
  {
    matbase* mf = create_matfile(rows, cols, sizeof(T), ".\\test.mat", "wb");

    if (mf) {
      for (i = 0; i < mf->rows; ++i) {
        T* rrr = (T*) mat_setrow(mf, i);

        for (j = 0; j < (int)(mf->cols); ++j) {
          rrr[ j ] = i + j;
        }

        mat_saverow(mf);
      }

      //compress_matfile(mf, ".\\test_comp.mat");
      mat_delete(mf);
    }
  }
  {
    matbase* mf = open_matfile(".\\test.mat");

    if (mf) {
      ASSERT(mf->rows == rows && mf->cols == cols);

      for (i = 0; i < mf->rows; ++i) {
        T* rrr = (T*) mat_setrow(mf, i);
        mat_loadrow(mf);

        for (j = 0; j < (int)(mf->cols); ++j) {
          ASSERT(rrr[ j ] == (T) i);
          //printf("%d\n", rrr[j]);
        }
      }

      if (1) {
        matbase* mf2 = create_matfile(cols, rows, sizeof(T), ".\\test_trans.mat", "wb");
        mat_transport(mf, mf2);
        mat_delete(mf2);
      }

      mat_delete(mf);
    }
  }
  {
    matbase* mf = open_matfile(".\\test_trans.mat");

    if (mf) {
      ASSERT(mf->rows == (cols) && mf->cols == rows);

      for (i = 0; i < mf->rows; ++i) {
        T* rrr = (T*) mat_setrow(mf, i);
        mat_loadrow(mf);

        for (j = 0; j < (int)(mf->cols / sizeof(T)); ++j) {
          ASSERT(rrr[ j ] == (T) j);
          //printf("%d\n", rrr[j]);
        }
      }

      mat_delete(mf);
    }
  }
  return 0;
}

int main()
{
  test_matfile();
  return 0;
}
