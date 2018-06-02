
#include <tchar.h>
#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cstd.h"
#include "cap.h"
#include "window.h"
#include "findfile.h"

//#include "fmtgif2.h"

int imOil(int height, int width, unsigned char* src, int step, int cn, int N);

int imWaterColor(int src_height, int src_width, unsigned char* src, int src_step, int N);

int imCartoon(int src_height, int src_width, unsigned char* src, int src_step);

unsigned char* immedian(int h, int w, const unsigned char* src, int sl,
    unsigned char* dst, int dl, int cn, int y, int x, int cy, int cx);

unsigned char* immax(int h, int w, const unsigned char* src, int sl,
    unsigned char* dst, int dl, int cn, int y, int x, int cy, int cx);

int test_PicEffect()
{
  TCHAR buf[256];
  _mkdir(".\\out");
  begin_find(info, "*.*")
  const char* picname = fileinfo_filename(info);
  struct fmtimgreader* reader = new_reader(picname);

  if (reader) {
    int height = reader->m_height;
    int width = reader->m_width;
    int cn = 3;//reader->m_iscolor ? 3 : 1;
    int step;
    unsigned char* data;
    unsigned char* data2;
    //cn = 1;
    step = width * cn;
    data = (unsigned char*)pmalloc(height * step);
    data2 = (unsigned char*)pmalloc(height * step);
    memset(data, 128, height * step);
    imread(reader, data2, step, cn > 1);
#define TEST(_FUN, _OUT)  \
  memcpy(data, data2, height * step); \
  { utime_start(_start_time); _FUN; \
    _snprintf(buf, 256, _OUT, picname); \
    printf("%12.9f %s\n", utime_elapsed(_start_time), buf); }\
  imwrite(buf, height, width, data, step, cn);

    //TEST(immedian(height, width, data, width*3, data, width*3, 3, 3, 3, 7, 7);, ".\\out\\%s_immedian.bmp");
    //TEST(imMedian(height, width, data, data, 7);, ".\\out\\%s_imMedian1.bmp");
    //TEST(imwatercolor(height, width, data, width*3, data, width*3, 3, 3);, ".\\out\\%s_WaterColor2.bmp");
    TEST(immax(height, width, data, width * 3, data, width * 3, 3, 2, 2, 5, 5); , ".\\out\\%s_OilPaint2.bmp");
    //TEST(imOil(height, width, data, width*3, 3, 2), ".\\out\\%s_OilPaint.bmp");
    //TEST(imOil(height, width, data, width*3, 3, 2), ".\\out\\%s_OilPaint.bmp");
    //TEST(imWaterColor(height, width, data, width*3, 3), ".\\out\\%s_WaterColor.bmp");
    //TEST(imCartoon(height, width, data, width*3), ".\\out\\%s_Cartoon.bmp");

    pfree(data);
    pfree(data2);
    free_reader(reader);
  }

  end_find(info);
  return 0;
}


int main()
{
  test_PicEffect();
  return 0;
}
