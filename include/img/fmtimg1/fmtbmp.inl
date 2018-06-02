#ifndef _FMTBMP_INL_
#define _FMTBMP_INL_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "_fmtimg.h"

typedef enum BmpCompression {
  BMP_RGB = 0,
  BMP_RLE8 = 1,
  BMP_RLE4 = 2,
  BMP_BITFIELDS = 3
} BmpCompression;

//当biCompression成员的值是BI_RGB时，它没有调色板。16位中，最低的5位表示蓝色分量，中间的5位表示绿色分量，高的5位表示红色分量，一共占用了15位，最高的一位保留，设为0。这种格式也被称作555   16位位图。
//当biCompression成员的值是 BI_BITFIELDS 时，原来调色板的位置被三个DWORD变量占据，称为红、绿、蓝掩码。
//分别用于描述红、绿、蓝分量在16位中所占的位置。在Windows 95（或98）中，系统可接受两种格式的位域：555和565；
//在555格式下，红、绿、蓝的掩码分别是：0x7C00、0x03E0、0x001F，
//而在565格式下，它们则分别为：0xF800、0x07E0、0x001F。

typedef struct fmtbmp {
  int m_bpp;
  int m_offset;
  PaletteEntry m_palette[ 256 ];
  BmpCompression m_rle_code;
}
fmtbmp;

int fmtbmp_read_header(ihand_t m_strm, img_t* im, fmtbmp* fmt)
{
  int result = FALSE;
  int m_bpp = 0, m_rle_code = 0, m_offset = 0, size;
  int m_width = 0, m_height = 0, m_iscolor = 0;
  PaletteEntry* m_palette;

  if (!checksign(m_strm, "BM", 2)) {
    return 0;
  }

  m_palette = fmt->m_palette;

  if (!m_strm) {
    return FALSE;
  }

  SetPos(10);
  READ(m_offset);
  READ(size);

  if (size >= 36) {
    int clrused;
    READ(m_width);
    READ(m_height);
    READ(m_bpp);
    m_bpp >>= 16;
    READ(m_rle_code);
    SetPos(6 * sizeof(int) + 22);     //seek(12, SEEK_CUR);
    READ(clrused);
    SetPos(7 * sizeof(int) + 22 + size - 36);     // seek(size - 36, SEEK_CUR);

    if (m_width > 0 && m_height > 0 &&
        (((m_bpp == 1 || m_bpp == 4 || m_bpp == 8 ||
            m_bpp == 24 || m_bpp == 32) && m_rle_code == BMP_RGB) ||
            (m_bpp == 16 && (m_rle_code == BMP_RGB || m_rle_code == BMP_BITFIELDS)) ||
            (m_bpp == 4 && m_rle_code == BMP_RLE4) ||
            (m_bpp == 8 && m_rle_code == BMP_RLE8))) {
      m_iscolor = TRUE;
      result = TRUE;

      if (m_bpp <= 8) {
        memset(m_palette, 0, sizeof(m_palette));
        READ_NBYTE((unsigned char*) m_palette, (clrused == 0 ? 1 << m_bpp : clrused) * 4);
        m_iscolor = IsColorPalette(m_palette, m_palette + (1 << m_bpp));
      }
      else if (m_bpp == 16 && m_rle_code == BMP_BITFIELDS) {
        int redmask, greenmask, bluemask;
        READ(redmask);
        READ(greenmask);
        READ(bluemask);

        if (bluemask == 0x1f && greenmask == 0x3e0 && redmask == 0x7c00) {
          m_bpp = 15;
        }
        else if (bluemask == 0x1f && greenmask == 0x7e0 && redmask == 0xf800) {}
        else {
          result = FALSE;
        }
      }
      else if (m_bpp == 16 && m_rle_code == BMP_RGB) {
        m_bpp = 15;
      }
    }
  }
  else if (size == 12) {
    READ(m_width);
    READ(m_height);
    READ(m_bpp);
    m_bpp >>= 16;

    if (m_width > 0 && m_height > 0 &&
        (m_bpp == 1 || m_bpp == 4 || m_bpp == 8 ||
            m_bpp == 24 || m_bpp == 32)) {
      if (m_bpp <= 8) {
        unsigned char buffer[ 256 * 3 ];
        int j, clrused = 1 << m_bpp;
        READ_NBYTE(buffer, clrused * 3);

        for (j = 0; j < clrused; j++) {
          m_palette[ j ].b = buffer[ 3 * j + 0 ];
          m_palette[ j ].g = buffer[ 3 * j + 1 ];
          m_palette[ j ].r = buffer[ 3 * j + 2 ];
        }
      }

      result = TRUE;
    }
  }

  if (!result) {
    m_offset = -1;
    m_width = m_height = -1;
  }
  else {
    im->w = m_width;
    im->h = m_height;
    im->c = m_iscolor ? 3 : 1;
    im->f = 1;
    fmt->m_bpp = m_bpp;
    fmt->m_offset = m_offset;
    fmt->m_rle_code = (BmpCompression) m_rle_code;
  }

  return result;
}

int fmtbmp_read_data(ihand_t m_strm, img_t* im, fmtbmp* fmt)
{
  unsigned char* data = im->tt.data;
  int step = im->s, nch = im->c;
  int result = FALSE;
  int m_width = im->w, m_height = im->h;
  int m_bpp = fmt->m_bpp, m_offset = fmt->m_offset;
  int m_rle_code = fmt->m_rle_code;
  enum { BUFFER_SIZE = (1 << 12) };

  PaletteEntry* m_palette = fmt->m_palette;

  unsigned char buffer[ BUFFER_SIZE ];
  unsigned char bgr_buffer[ BUFFER_SIZE ];
  unsigned char gray_palette[ 256 ];
  unsigned char* src = buffer;
  unsigned char* bgr = bgr_buffer;
  int src_pitch = (((m_width * (m_bpp != 15 ? m_bpp : 16) + 7) / 8 + 3) & -4);
  int color = nch > 1;
  int width3 = m_width * nch;
  int step1 = step;
  int y;
  short st;
  unsigned char bt;
  UNUSED(result);

  if (m_offset < 0 || !m_strm) {
    return FALSE;
  }

  data += (m_height - 1) * step;
  step = -step;

  if ((m_bpp != 24 || !color) && src_pitch + 32 > BUFFER_SIZE) {
    src = (unsigned char*)pmalloc(src_pitch + 32);
  }

  if (!color) {
    if (m_bpp <= 8) {
      IMTRANS1D((1 << m_bpp), (unsigned char*) m_palette, 4, gray_palette, 1, BGRA2GRAY);
    }

    if (m_width * 3 + 32 > BUFFER_SIZE) {
      bgr = (unsigned char*)pmalloc(m_width * 3 + 32);
    }
  }

  //if( 1 )
  {
    SetPos(m_offset);

    switch (m_bpp) {
      /************************* 1 BPP ************************/
    case 1:
      for (y = 0; y < m_height; y++, data += step) {
        READ_NBYTE(src, src_pitch);
        FillColorRow1(color ? data : bgr, src, m_width, m_palette);

        if (!color) {
          IMTRANS1D(m_width, (unsigned char*) bgr, 3, data, 1, BGR2GRAY);
        }
      }

      result = TRUE;
      break;

      /************************* 4 BPP ************************/
    case 4:
      if (m_rle_code == BMP_RGB) {
        for (y = 0; y < m_height; y++, data += step) {
          READ_NBYTE(src, src_pitch);

          if (color) {
            FillColorRow4(data, src, m_width, m_palette);
          }
          else {
            FillGrayRow4(data, src, m_width, gray_palette);
          }
        }

        result = TRUE;
      }
      else if (m_rle_code == BMP_RLE4) {       // rle4 compression
        unsigned char* line_end = data + width3;
        y = 0;

        for (;;) {
          unsigned char* end;
          int code, len;
          READ(st);
          code = st;
          len = code & 255;
          code >>= 8;

          if (len != 0) {     // encoded mode
            PaletteEntry clr[ 2 ];
            unsigned char gray_clr[ 2 ];
            int t = 0;

            clr[ 0 ] = m_palette[ code >> 4 ];
            clr[ 1 ] = m_palette[ code & 15 ];
            gray_clr[ 0 ] = gray_palette[ code >> 4 ];
            gray_clr[ 1 ] = gray_palette[ code & 15 ];

            end = data + len * nch;

            if (end > line_end) {
              goto decode_rle4_bad;
            }

            do {
              if (color) {
                WRITE_PIX(data, clr[ t ]);
              }
              else {
                *data = gray_clr[ t ];
              }

              t ^= 1;
            }
            while ((data += nch) < end);
          }
          else if (code > 2) {       // absolute mode
            if (data + code * nch > line_end) {
              goto decode_rle4_bad;
            }

            READ_NBYTE(src, (((code + 1) >> 1) + 1) & -2);

            if (color) {
              data = FillColorRow4(data, src, code, m_palette);
            }
            else {
              data = FillGrayRow4(data, src, code, gray_palette);
            }
          }
          else {
            int x_shift3 = (int)(line_end - data);
            int y_shift = m_height - y;

            if (code == 2) {
              READ(bt);
              x_shift3 = bt * nch;
              READ(bt);
              y_shift = bt;
            }

            len = x_shift3 + ((y_shift * width3) & ((code == 0) - 1));

            if (color)
              data = FillUniColor(data, &line_end, step, width3,
                  &y, m_height, x_shift3, m_palette[ 0 ]);
            else
              data = FillUniGray(data, &line_end, step, width3,
                  &y, m_height, x_shift3, gray_palette[ 0 ]);

            if (y >= m_height) {
              break;
            }
          }
        }

        result = TRUE;
decode_rle4_bad:
        ;
      }

      break;

      /************************* 8 BPP ************************/
    case 8:
      if (m_rle_code == BMP_RGB) {
        for (y = 0; y < m_height; y++, data += step) {
          READ_NBYTE(src, src_pitch);

          if (color) {
            FillColorRow8(data, src, m_width, m_palette);
          }
          else {
            FillGrayRow8(data, src, m_width, gray_palette);
          }
        }

        result = TRUE;
      }
      else if (m_rle_code == BMP_RLE8) {       // rle8 compression
        unsigned char* line_end = data + width3;
        int line_end_flag = 0;
        y = 0;

        for (;;) {
          int code, len;
          READ(st);
          code = st;
          len = code & 255;
          code >>= 8;

          if (len != 0) {     // encoded mode
            int prev_y = y;
            len *= nch;

            if (data + len > line_end) {
              goto decode_rle8_bad;
            }

            if (color)
              data = FillUniColor(data, &line_end, step, width3,
                  &y, m_height, len, m_palette[ code ]);
            else
              data = FillUniGray(data, &line_end, step, width3,
                  &y, m_height, len, gray_palette[ code ]);

            line_end_flag = y - prev_y;
          }
          else if (code > 2) {       // absolute mode
            int prev_y = y;
            int code3 = code * nch;

            if (data + code3 > line_end) {
              goto decode_rle8_bad;
            }

            READ_NBYTE(src, (code + 1) & -2);

            if (color) {
              data = FillColorRow8(data, src, code, m_palette);
            }
            else {
              data = FillGrayRow8(data, src, code, gray_palette);
            }

            line_end_flag = y - prev_y;
          }
          else {
            int x_shift3 = (int)(line_end - data);
            int y_shift = m_height - y;

            if (code || !line_end_flag || x_shift3 < width3) {
              if (code == 2) {
                READ(bt);
                x_shift3 = bt * nch;
                READ(bt);
                y_shift = bt;
              }

              x_shift3 += (y_shift * width3) & ((code == 0) - 1);

              if (y >= m_height) {
                break;
              }

              if (color)
                data = FillUniColor(data, &line_end, step, width3,
                    &y, m_height, x_shift3, m_palette[ 0 ]);
              else
                data = FillUniGray(data, &line_end, step, width3,
                    &y, m_height, x_shift3, gray_palette[ 0 ]);

              if (y >= m_height) {
                break;
              }
            }

            line_end_flag = 0;
          }
        }

        result = TRUE;
decode_rle8_bad:
        ;
      }

      break;

      /************************* 15 BPP ************************/
    case 15:
      for (y = 0; y < m_height; y++, data += step) {
        READ_NBYTE(src, src_pitch);

        if (!color) {
          IMTRANS1D(m_width, src, 2, data, 1, BGR5552GRAY);
        }
        else {
          IMTRANS1D(m_width, src, 2, data, 3, BGR5552BGR);
        }
      }

      result = TRUE;
      break;

      /************************* 16 BPP ************************/
    case 16:
      for (y = 0; y < m_height; y++, data += step) {
        READ_NBYTE(src, src_pitch);

        if (!color) {
          IMTRANS1D(m_width, src, 2, data, 1, BGR5652GRAY);
        }
        else {
          IMTRANS1D(m_width, src, 2, data, 3, BGR5652BGR);
        }
      }

      result = TRUE;
      break;

      /************************* 24 BPP ************************/
    case 24:
      for (y = 0; y < m_height; y++, data += step) {
        READ_NBYTE(src, src_pitch);

        if (!color) {
          IMTRANS1D(m_width, src, 3, data, 1, BGR2GRAY);
        }
        else {
          IMTRANS1D(m_width, src, 3, data, nch, BGRA2BGR);
        }
      }

      result = TRUE;
      break;

      /************************* 32 BPP ************************/
    case 32:
      for (y = 0; y < m_height; y++, data += step) {
        READ_NBYTE(src, src_pitch);

        if (!color) {
          IMTRANS1D(m_width, src, 4, data, 1, BGRA2GRAY);
        }
        else {
          IMTRANS1D(m_width, src, 4, data, 3, BGRA2BGR);
        }
      }

      result = TRUE;
      break;

    default:
      assert(0);
    }
  }

  if (src != buffer) {
    pfree(src);
  }

  if (bgr != bgr_buffer) {
    pfree(bgr);
  }

  return 1;
}

#if 0
static void* memcpy2d(void* dst, void* src, int h, int w, int dst_skip, int src_skip)
{
  if (0 == dst_skip && 0 == src_skip) {
    memcpy(dst, src, h * w);
  }
  else {
    int i;

    for (i = 0; i < h; ++i, dst += dst_skip, src += src_skip) {
      memcpy(dst, src, w);
    }
  }

  return dst;
}
#endif

int fmtbmp_write(ihand_t m_strm, const img_t* im, const PaletteEntry* palette)
{
  int result = FALSE;
  static const char* fmtSignBmp = "BM";
  int height = im->h, width = im->w, step = im->s, channels = im->c;
  const unsigned char* data = im->tt.data;
  //const Base& m_strm = *this;
  int fileStep = (width * channels + 3) & -4;
  const unsigned char zeropad[] = "\0\0\0\0";
  int bitmapHeaderSize = 40;
  int paletteSize = channels > 1 ? 0 : 1024;
  int headerSize = 14 /* fileheader */ + bitmapHeaderSize + paletteSize;
  int val;
  short st;

  PaletteEntry palette0[ 256 ];

  if (NULL == palette) {
    palette = palette0;
  }

  if (!m_strm) {
    return FALSE;
  }

  assert(data && width >= 0 && height >= 0 && step >= (width * channels));

  // BMP文件头（14字节）
  // write signature 'BM'
  WRITE_NBYTE(fmtSignBmp, (int) strlen(fmtSignBmp));

  // write file header
  val = fileStep * height + headerSize;
  WRITE(val);   // file size
  val = 0;
  WRITE(val);
  WRITE(headerSize);

  // write bitmap header
  WRITE(bitmapHeaderSize);
  WRITE(width);
  WRITE(height);
  st = 1;
  WRITE(st);
  st = (short)(channels << 3);
  WRITE(st);
  val = BMP_RGB;
  WRITE(val);
  val = 0;
  WRITE(val);
  WRITE(val);
  WRITE(val);
  WRITE(val);
  WRITE(val);

  if (channels == 1) {
    FillGrayPalette(palette0, 8, FALSE);
    WRITE_NBYTE(palette, 256 * sizeof(PaletteEntry));
  }

  width *= channels;
  data += step * (height - 1);

  for (; height--; data -= step) {
    WRITE_NBYTE(data, width);

    if (fileStep > width) {
      WRITE_NBYTE(zeropad, fileStep - width);
    }
  }

  result = TRUE;
  return result;
}

#define fmtbmp_free(x) (0)

#endif // _FMTBMP_INL_
