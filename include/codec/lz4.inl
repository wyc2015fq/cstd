
#include "lz4/liblz4.inl"

static int test_lz4()
{
  char szString[] = "2013-01-07 00:00:04,0.98644,0.98676 2013-01-07 00:01:19,0.98654,0.98676 2013-01-07 00:01:38,0.98644,0.98696";
  const char* pchSource = szString;
  int nInputSize = sizeof(szString);
  
  printf("- pchSource -\n%s\n", pchSource);
  printf("nbytes = %d\n\n", nInputSize);
  
  int nbytesPassed = 0;
  int nMaxCompressedSize = LZ4_compressBound(nInputSize);
  
  char *pszDest = (char*)malloc(nMaxCompressedSize);
  nbytesPassed = LZ4_compress(pchSource, pszDest, nInputSize);
  printf("- pszDest Compressed-\n");
  printf("nbytesPassed = %d\n %s\n\n", nbytesPassed, pszDest);
  // pszDest garbage ?
  
  char *pszDestUnCompressed = (char*)malloc(nInputSize);
  LZ4_uncompress(pszDest, pszDestUnCompressed, nInputSize);
  printf("- pszDestUnCompressed -\n");
  printf("nbytesPassed = %d\n %s\n\n", nbytesPassed, pszDestUnCompressed);
  //pszDestUnCompressed is correct ?
  
  free(pszDestUnCompressed);
  pszDestUnCompressed = 0;
  
  // ok lets write compressed pszDest to pszDest.dat
  
  free(pszDest);
  pszDest = 0;
  return 0;
}

int lz4_compress(const buf_t* src, buf_t* dst) {
  int nMaxCompressedSize = LZ4_compressBound(src->len);
  buf_t bf[1] = {0};
  if (src==dst) {
    bfsetsize(bf, src->len);
    memcpy(bf->data, src->data, src->len);
    src = bf;
  }
  bfsetsize(dst, nMaxCompressedSize);
  dst->len = LZ4_compress_default((const char*)src->data, (char*)dst->data, src->len, src->len);
  bffree(bf);
  return 0;
}

int lz4_uncompress(const buf_t* src, buf_t* dst) {
  buf_t bf[1] = {0};
  if (src==dst) {
    bfsetsize(bf, src->len);
    memcpy(bf->data, src->data, src->len);
    src = bf;
  }
  int i=0, nMaxSize = src->len;
  int len = -1;
  while(len<0) {
    nMaxSize*=2;
    bfsetsize(dst, nMaxSize);
    len = LZ4_decompress_safe((const char*)src->data, (char*)dst->data, src->len, dst->len);
  }
  dst->len = len;
  bffree(bf);
  return 0;
}
