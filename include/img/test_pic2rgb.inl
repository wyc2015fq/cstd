
//#include "cstd.h"
//#include "imgio.inl"

#include <stdio.h>
#include <stdlib.h>

typedef unsigned char uchar;

#define stream_eof(stream)                    feof(stream)
#define stream_close(stream)                  fclose(stream)
#define stream_tell(stream)                   ftell(stream)
#define stream_get8(stream)                   fget8(stream)
#define stream_unget8(ch, stream)             funget8(ch, stream)
#define stream_put8(stream, x)                fput8(stream, x)
#define stream_seek(stream, offset, origin)   fseek((stream), offset, origin)
#define stream_read(stream, buffer, len)      fread(buffer, 1, len, (stream))
#define stream_write(stream, buffer, len)     fwrite(buffer, 1, len, (stream))

#define stream_getu1(stream) stream_get8(stream)
#define stream_getu2(stream)  ((stream_getu1(stream))|(stream_getu1(stream)<<8))
#define stream_getu4(stream)  ((stream_getu1(stream))|(stream_getu1(stream)<<8)|(stream_getu1(stream)<<16)|(stream_getu1(stream)<<24))

#define MEM_GET16B(p, i, x) x = (i+=2, (((uchar*)p)[i-2]<<8) | (((uchar*)p)[i-1]))
#define MEM_GET16L(p, i, x) x = (i+=2, (((uchar*)p)[i-1]<<8) | (((uchar*)p)[i-2]))
#define MEM_GET32B(p, i, x) x = (i+=4, (((uchar*)p)[i-4]<<24) | (((uchar*)p)[i-3]<<16) | (((uchar*)p)[i-2]<<8) | (((uchar*)p)[i-1]))
#define MEM_GET32L(p, i, x) x = (i+=4, (((uchar*)p)[i-1]<<24) | (((uchar*)p)[i-2]<<16) | (((uchar*)p)[i-3]<<8) | (((uchar*)p)[i-4]))

#define imio_err(a, b)  printf("%s, %s", a, b)

int bmp_load(FILE* s, int* ph, int* pw, int* pcn, char** pdata)
{
  uchar buf[1024];
  int i = 0;
  int m_bpp = 0, m_rle_code = 0, m_offset = 0, hsz, biPlanes;
  int m_width = 0, m_height = 0, m_cn, clr_used = 0;
  int y, src_pitch;
  uchar* m_palette = buf;

  if (!s) {
    return 0;
  }

  stream_seek(s, 0, SEEK_SET);
  stream_read(s, buf, 50);

  if (buf[0] != 'B' || buf[1] != 'M') {
    return 0;
  }

  i = 10;
  MEM_GET32L(buf, i, m_offset);
  MEM_GET32L(buf, i, hsz);

  if (hsz != 12 && hsz != 40 && hsz != 56 && hsz != 108 && hsz != 124) {
    return imio_err("unknown BMP", "BMP type not supported: unknown");
  }

  if (hsz >= 36) {
    MEM_GET32L(buf, i, m_width);
    MEM_GET32L(buf, i, m_height);
    MEM_GET16L(buf, i, biPlanes);

    if (biPlanes != 1) {
      return imio_err("bad BMP", "bad BMP");
    }

    MEM_GET16L(buf, i, m_bpp);
    MEM_GET32L(buf, i, m_rle_code);
    i = 46;
    MEM_GET32L(buf, i, clr_used);
    i = 7 * sizeof(int) + 22 + hsz - 36;

    if (m_rle_code == 1 || m_rle_code == 2) {
      return imio_err("BMP RLE", "BMP type not supported: RLE");
    }

    clr_used = (clr_used == 0 ? 1 << m_bpp : clr_used);
    m_cn = m_bpp > 24 ? 4 : 3;

    if (m_bpp <= 8) {
      return imio_err("BMP RLE", "不支持(bpp<=8)格式");
    }
    else if (m_bpp == 16 && m_rle_code == 3) {
      return imio_err("BMP RLE", "不支持(bpp<=16)格式");
    }
  }
  else {
    MEM_GET16L(buf, i, m_width);
    MEM_GET16L(buf, i, m_height);
    MEM_GET16L(buf, i, biPlanes);
    MEM_GET16L(buf, i, m_bpp);
  }

  if (pcn) {
    *pcn = m_cn;
  }
  
  if (pw) {
    *pw = m_width;
  }
  
  if (ph) {
    *ph = m_height;
  }

  if (pdata) {
    uchar* data = NULL;
    int step = m_width*m_cn;
    uchar aa[4];
    int aalen = ((step+3)&~3) - step;
    *pdata = (char*)malloc(m_height*step);
    data = *pdata + step * (m_height-1);
    stream_seek(s, m_offset, SEEK_SET);
    src_pitch = (((m_width * (m_bpp != 15 ? m_bpp : 16) + 7) / 8 + 3) & -4);

    for (y = 0; y < m_height; y++, data -= step) {
      stream_read(s, data, src_pitch);
      if (aalen) {
        stream_read(s, aa, aalen);
      }
    }

  }

  return 1;
}

int test_pic2rgb(int argn, char** argc) {
  char* data = NULL;
  int h=0, w=0, cn=0;
  if (argn<2) {
    printf("usage: pic2rgb picfilaname\n");
  }
  {
    FILE* pf = fopen(argc[1], "rb");
    if (pf) {
      if (bmp_load(pf, &h, &w, &cn, &data)) {
        char buf[256];
        int len = _snprintf(buf, 256, "%s_%dx%dx%d.dat", argc[1], h, w, cn);
        FILE* pf2 = fopen(buf, "wb");
        fwrite(data, h*w*cn, 1, pf2);
        fclose(pf2);
        free(data);
      } else {
        printf("文件错误\n");
      }
      fclose(pf);
    }
  }
  return 0;
}
