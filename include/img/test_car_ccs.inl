
#include "../../include/cstd.h"


unsigned char pic[91 * 174] = {
#include "./test_car_ccs_pic.txt"
};
unsigned char* buf = (void*)0x8000000;
int buflen = 20000000;

int main()
{
  unsigned char* img;
  plate_algo pa = {0};
  int mode;
  mode = PLATE_DO_DETECT | PLATE_DO_CUTTER;
  mode = PLATE_DO_DETECT;

  BUFUSEBEGIN(buf, buflen);
  BUFMALLOC2(img, 3 * 576 * 720);
  BUFBOUNDCHECK();

  pa.h = 720, pa.w = 576, pa.bgr = img, pa.bgr_step = 720 * 3;
  pa.buf = GETBUFBEG(), pa.buflen = GETBUFLEN();
  pa.mode = mode;
  plate_algo_process(&pa);
  BUFUSEEND();
  return 0;
}