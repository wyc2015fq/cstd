
#include ".\draw\unzip.inl"

#ifdef _UNICODE
#define GetZipItem GetZipItemW
#else
#define GetZipItem GetZipItemA
#endif

int test_zip()
{
  TUnzipHandleData uzh[1] = {0};
  HZIP hz = NULL;
  const char* sFile = "D:/code/c/GUI/bcgsoft/BCG/Bin/BCGPStyle2007Luna1030/BCGPStyle2007Luna1030.zip";
  ZIPENTRY ze;
  int i;
  const char* m_lpstr;
  m_lpstr = "BCGPStyle2007Luna1030_BLUE_IDB_OFFICE2007_STATUSBAR_VERT_BACK_1_PNG.png";
  m_lpstr = "BCGPStyle2007Luna1030_IDX_OFFICE2007_STYLE_STYLE_XML.xml";
  hz = OpenZipF(uzh, sFile, 0);

  if (hz) {
    int res, dwSize;
    BYTE* pData;

    if (FindZipItem(hz, m_lpstr, true, &i, &ze) != 0)  {
      return 0;
    }

    dwSize = ze.unc_size;

    if (dwSize == 0) {
      return 0;
    }

    pData = MALLOC(BYTE, dwSize);

    res = UnzipItemM(hz, i, pData, dwSize);

    if (res == 0) {
      pfree(pData);
      CloseZip(hz);
    }
  }

  return 0;
}