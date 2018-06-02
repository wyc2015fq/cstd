
#include "dcmfmt.inl"

int test_dcmfmt()
{
  dcmfmt s[1] = {0};
  const char* fn = "D:/CTdata/CENOVIXuncompress/IM-0001-0001.dcm";
  dcmfmt_read(s, fn, WINDOW);
  {
    img_t im[1] = {0};
    IMINIT(im, s->Rows, s->Cols, s->m_pData, s->Cols, 1, 1);
    imshow(im);
    cvWaitKey(-1);
  }
  dcmfmt_free(s);
  return 0;
}
