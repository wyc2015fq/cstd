#ifndef _FMTIMG_INL_
#define _FMTIMG_INL_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "str.h"
#include "_fmtimg.h"

#include "fmtbmp.inl"
#include "fmtjpg.inl"
#include "lib/libzlib.inl"
#ifdef _WIN
#include "fmtpng.inl"
#endif
//#include "fmttif.inl"
#include "fmtgif.inl"
#include "fmtgif1.inl"
#include "fmtpxm.inl"
#include "fmtima.inl"
#include "fmtpcx.inl"

#undef PutBytes
#undef GetBytes

#if 0
#ifdef _MSC_VER
//#pragma comment (lib,"zlib.lib")
#endif
#ifdef __GNUC__
#pragma comment (lib,"zlib.a")
#endif
#endif

int imread_xfile(XFILE* xf, int m_channels, int m_frames, img_t* im)
{
#define FMTIMG_DEF(xxx, ext)  {fmt##xxx fmt[1]={0};img_t im2[1]={0};if (fmt##xxx##_read_header( xf, im2, fmt )) { imsetsize( im, im2->h, im2->w, m_channels, 0==m_frames ? im2->f: m_frames ); fmt##xxx##_read_data( xf, im, fmt ); fmt##xxx##_free(fmt); return 1;}}
#include "fmtimg_list.txt"
#undef FMTIMG_DEF
  return 0;
}

int imread_file(FILE* pf, int m_channels, int m_frames, img_t* im)
{
  XFILE xf[1] = {0};

  if (NULL == pf) {
    return 0;
  }

  xfinit_file(xf, pf);
  return imread_xfile(xf, m_channels, m_frames, im);
}

int imread_mem(void* ptr, int size, int m_channels, int m_frames, img_t* im)
{
  XFILE xf[1] = {0};

  if (NULL == ptr) {
    return 0;
  }

  xfinit_mem(xf, (uchar *)ptr, size);
  return imread_xfile(xf, m_channels, m_frames, im);
}

int imwrite_mem(uchar* pBuffer, unsigned int size, int id, int height, int width, const uchar* data, int step, int channels)
{
  if (pBuffer && size) {
    XFILE xf;
    xfinit_mem(&xf, pBuffer, size);
    //imwrite_1(&xf, id, height, width, data, step, channels);
    //ASSERT(xf.m_Size<size);
    //memcpy(pBuffer, xf.m_pBuffer, xf.m_Size);
  }

  return 0;
}

// "tchar.h"
int imwriteA(const char* picname, const img_t* im, const PaletteEntry* palette)
{
  char* pext = strdup(strrchr(picname, '.'));
  FILE* fp = fopen(picname, "wb");

  strtolower(pext);

  if (fp) {
    XFILE m_strm[1] = {0};
    xfinit_file(m_strm, fp);
#define FMTIMG_DEF(xxx, ext)  if ( strstr(ext, pext ) ) { fmt##xxx##_write( m_strm, im, palette ); fclose(fp); return 0; }
#include "fmtimg_list.txt"
#undef FMTIMG_DEF
    fclose(fp);
  }

  pfree(pext);
  return 0;
}

#define WRAP_FOPEN(_wfopen, picname, mode, _DO) FILE* fp = _wfopen(picname, mode);  if (fp) {_DO; fclose(fp); }

int imreadA(const char* picname, int m_channels, int m_frames, img_t* im)
{
  int ret = 0;
  FILE* fp;

  if (picname[0] && (fp = fopen(picname, "rb"))) {
    ret = imread_file(fp, m_channels, m_frames, im);
    fclose(fp);
  }

  return ret;
}
#ifdef _WINDOWS_
int imwriteW(const wchar_t* picname, const img_t* im, const PaletteEntry* palette)
{
  const wchar_t* pext = wcsrchr(picname, L'.');
  FILE* fp = _wfopen(picname, L"wb");

  if (fp) {
    XFILE m_strm[1] = {0};
    xfinit_file(m_strm, fp);
#define FMTIMG_DEF(xxx, ext)  if ( wcsstr(L##ext, pext ) ) { fmt##xxx##_write( m_strm, im, palette ); fclose(fp); return 0; }
#include "fmtimg_list.txt"
#undef FMTIMG_DEF
    fclose(fp);
  }

  return 0;
}

int imreadW(const wchar_t* picname, int m_channels, int m_frames, img_t* im)
{
  int ret = 0;
  FILE* fp;

  if (picname[0] && (fp = _wfopen(picname, L"rb"))) {
    ret = imread_file(fp, m_channels, m_frames, im);
    fclose(fp);
  }

  return ret;
}
#endif




/* 根据文件名，写入图片到文件 */

#ifdef UNICODE
#define imwrite( _FN, _IM ) imwriteW(_FN, _IM, 0)
#define imwrite2( picname, HEIGHT, WIDTH, DATA, STEP, CHANNELS )  {img_t _IM[1]; _IM->h=HEIGHT, _IM->w=WIDTH, _IM->s=STEP, _IM->c=CHANNELS, _IM->tt.data=DATA; imwriteW( picname, _IM, 0 );}
#else
#define imwrite( _FN, _IM ) imwriteA(_FN, _IM, 0)
#define imwrite2( picname, HEIGHT, WIDTH, DATA, STEP, CHANNELS )  {img_t _IM[1]; _IM->h=HEIGHT, _IM->w=WIDTH, _IM->s=STEP, _IM->c=CHANNELS, _IM->tt.data=DATA; imwriteA( picname, _IM, 0 );}
#endif // !UNICODE

int imreadA(const char* picname, int m_channels, int m_frames, img_t* im);
int imreadW(const wchar_t* picname, int m_channels, int m_frames, img_t* im);
#ifdef UNICODE
#define imread  imreadW
#else
#define imread  imreadA
#endif // !UNICODE

#endif // _FMTIMG_INL_
