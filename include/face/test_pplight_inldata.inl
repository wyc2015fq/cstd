
#include "cstd.h"
#include "img/filters.h"
#include "face/pplight_16s.inl"
uchar g_buf[1<<20];

static int loaddata_1(const char* fn, void* buf, size_t n)
{
  FILE* pf = fopen(fn, "rb");
  if (pf) {
    size_t rn = fread(buf, 1, n, pf);
    //ASSERT(rn==n);
    fclose(pf);
  }
  return 0;
}

static int savedata_11(const char* fn, const void* buf, size_t n, int off)
{
  FILE* pf = fopen(fn, "wb");
  if (pf) {
    size_t rn;
    fseek(pf, off, SEEK_SET);
    rn = fwrite(buf, 1, n, pf);
    ASSERT(rn == n);
    fclose(pf);
  }
  return 0;
}

uchar stdface_100x100[] = {
#include "C://stdface_100x100.inl"
};
int test_pplight_inldata() {
	img_t im[2] = {0};
	img_t* im1 = im+1;
	imsetsize(im, 100, 100, 1, 1);
  savedata_11("stdface_100x100.dat", stdface_100x100, 100*100, 0);
	memcpy(im->tt.data, stdface_100x100, 100*100);
	//savedata_inl("C:/stdface_100x100.inl", im->tt.data, 100*100);
	imsetsize(im1, 100, 100, 1, 1);
	pplight_16s(im->h, im->w, im->tt.data, im->s, 1, im1->tt.data, im1->s, 1, g_buf);
#if 0
	imshow(im);
	imshow(im1);
	cvWaitKey(0);
#endif
	imfrees(im, 2);
	return 0;
}

int test_pplight_datafile() {
  img_t im[2] = {0};
  img_t* im1 = im+1;
  imsetsize(im, 100, 100, 1, 1);
  loaddata_1("stdface_100x100.dat", im->tt.data, 100*100);
  //savedata_inl("C:/stdface_100x100.inl", im->tt.data, 100*100);
  imsetsize(im1, 100, 100, 1, 1);
  pplight_16s(im->h, im->w, im->tt.data, im->s, 1, im1->tt.data, im1->s, 1, g_buf);
	imfrees(im, 2);
  return 0;
}