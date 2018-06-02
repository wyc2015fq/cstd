
#include <stdio.h>
#include <stdlib.h>
#include "imglib.h"
#include "macro.h"


#define _H (512)
#define _W (512)

real A[ _H* _W ] = {0};
real B[ _H* _W ] = {0};
unsigned char uA[ _H* _W ] = {0};

#define _KH (5)
#define _KW (5)


#if 0
#define O (1.f/(_KH*_KW))
real sob[ _KH* _KH ] = {
  O, O, O, O, O,
  O, O, O, O, O,
  O, O, O, O, O,
  O, O, O, O, O,
  O, O, O, O, O,
};
#undef O
#else
real sob[ _KH* _KH ] = {
  1, 1, 2, 1, 1,
  1, 1, 2, 1, 1,
  0, 0, 0, 0, 0,
  -1, -1, -2, -1, -1,
  -1, -1, -2, -1, -1,
};
#endif


#if 0
int main(int argc, char* argv[])
{
  //int i;
  FILL(30 * _W, A + 1 * _W, 255);
  //COPY(_H*_W, A, uA);
  //imwrite("a.bmp", _H, _W, uA, _W, 1);
  //decompose_col_row(_KH, _KW, sob, _KW);
  {
    utime_start(_start_time);
    filter2d(_H, _W, A, _W, 1, B, _W, _KH, _KW, sob, _KW, _KH / 2, _KH / 2, 1);
    printf("%f\n", utime_elapsed(_start_time));
  }
  {
    utime_start(_start_time);
    filter2d_old(_H, _W, A, _W, 1, B, _W, _KH, _KW, sob, _KW, _KH / 2, _KH / 2, 1);
    printf("%f\n", utime_elapsed(_start_time));
  }
  //COPY(_H*_W, B, uA);
  //imwrite("b.bmp", _H, _W, uA, _W, 1);
  return 0;
}

#else
#include "cstd.h"
#include "findfile.h"

int copy_to_8u(int h, int w, const real* A, int al, unsigned char* B, int bl);

int main(int argc, char* argv[])
{
  char buf[ 256 ];
  int stepxy = 2;
  int allpic = 0;
  char* spec;
  int argi = 1;
  unsigned char* tmpimg = NULL;
  _mkdir(".\\out");
  spec = "*.*";
  begin_find(info, spec)
  char* picname = fileinfo_filename(info);
  struct fmtimgreader* reader = new_reader(picname);

  if (reader) {
    int height = reader->m_height, width = reader->m_width, cn = 1;
    int step = width * cn;
    unsigned char* data = (unsigned char*) pmalloc(height * step);
    real* fdata = (real*) pmalloc(sizeof(real) * height * step);
    int bodynum = 0;
    imread(reader, data, step, 0);
    COPY(height * step, data, fdata);
    free_reader(reader);
    {
      utime_start(_start_time);
      filter2d(height, width, fdata, step, 1, fdata, step, _KH, _KW, sob, _KW, _KH / 2, _KH / 2, 1);
      printf("%f %3d %3d %s\n", utime_elapsed(_start_time), height, width, picname);
    }
    copy_to_8u(height, width * cn, fdata, step, data, step);
    _snprintf(buf, 256, ".\\out\\%s_%s", "filter2d", picname);
    imwrite(buf, height, width, data, step, 1);
    pfree(data);
    pfree(fdata);
  }

  end_find(info);
}
#endif
