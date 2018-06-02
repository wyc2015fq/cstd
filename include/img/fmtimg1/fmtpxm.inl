
#ifndef SUPPORT_PXM
#define SUPPORT_PXM


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include "_fmtimg.h"

// P?M filter factory

typedef struct fmtpxm {
  int m_bpp;
  int m_offset;
  int m_binary;
  int m_maxval;
  PaletteEntry m_palette[ 256 ];
}
fmtpxm;

#define PXM_SIGN_LEN 3

int fmtpxm_checksign(ihand_t m_strm)
{
  int re = 0;
  int m_sign_len = PXM_SIGN_LEN;
  char* psign = (char*) pmalloc(m_sign_len * sizeof(char));

  if (psign) {
    re = getsign(m_strm, psign, m_sign_len);

    if (re) {
      re = psign[ 0 ] == 'P' && '1' <= psign[ 1 ] &&
          psign[ 1 ] <= '6' && isspace(psign[ 2 ]);
    }

    pfree(psign);
  }

  return re;
}

//////////////////////////////////////////////////////////////////////////////// P?M reader////////////////////////////////////////////////////////////////////////////////
static int ReadNumber(ihand_t m_strm, int maxdigits)
{
  int code;
  int val = 0;
  int digits = 0;
  unsigned char be;
  READ(be);
  code = be;

  if (!isdigit(code)) {
    do {
      if (code == '#') {
        do {
          READ(be);
          code = be;
        }
        while (code != '\n' && code != '\r');
      }

      READ(be);
      code = be;

      while (isspace(code)) {
        READ(be);
        code = be;
      }
    }
    while (!isdigit(code));
  }

  do {
    val = val * 10 + code - '0';

    if (++digits >= maxdigits) {
      break;
    }

    READ(be);
    code = be;
  }
  while (isdigit(code));

  return val;
}

int fmtpxm_free(struct fmtpxm* fmt)
{
  return 0;
}

// PNG Filter Factory
void fmtpxm_init(struct fmtpxm* fmt)
{
  int m_sign_len = PXM_SIGN_LEN;
  const char* m_signature = "";
  const char* m_description = _T("Portable image format (*.pbm;*.pgm;*.ppm)");
  fmt->m_offset = -1;
  fmt->m_bpp = 0;
  fmt->m_binary = fmt->m_maxval = 0;
}

int fmtpxm_read_header(ihand_t m_strm, img_t* im, fmtpxm* fmt)
{
  int result = FALSE;

  if (!fmtpxm_checksign(m_strm)) {
    return 0;
  }

  fmtpxm_init(fmt);

  if (!m_strm) {
    return FALSE;
  }

  SetPos(0);
  //if( setjmp( m_strm.JmpBuf()) == 0 )
  {
    int code;
    unsigned char be;
    READ(be);
    code = be;

    if (code != 'P') {
      BAD_HEADER_ERR();
    }

    READ(be);
    code = be;

    switch (code) {
    case '1':
    case '4':
      fmt->m_bpp = 1;
      break;

    case '2':
    case '5':
      fmt->m_bpp = 8;
      break;

    case '3':
    case '6':
      fmt->m_bpp = 24;
      break;

    default:
      BAD_HEADER_ERR();
    }

    fmt->m_binary = code >= '4';
    im->c = fmt->m_bpp / 8;

    im->w = ReadNumber(m_strm, INT_MAX);
    im->h = ReadNumber(m_strm, INT_MAX);
    im->f = 1;

    fmt->m_maxval = fmt->m_bpp == 1 ? 1 : ReadNumber(m_strm, INT_MAX);

    if (fmt->m_maxval > 255) {
      fmt->m_binary = FALSE;
    }

    if (im->w > 0 && im->h > 0 &&
        fmt->m_maxval > 0 && fmt->m_maxval < (1 << 16)) {
      fmt->m_offset = GetPos();
      result = TRUE;
    }

bad_header_exit:
    ;
  }

  if (!result) {
    fmt->m_offset = -1;
    im->w = im->h = -1;
  }

  return result;
}

int fmtpxm_read_data(ihand_t m_strm, img_t* im, fmtpxm* fmt)
{
  unsigned char* data = im->tt.data;
  int step = im->s, channels = im->c;
#define BUFFER_SIZE (1 << 12)
  unsigned char buffer[ BUFFER_SIZE ];
  unsigned char pal_buffer[ BUFFER_SIZE ];
  unsigned char bgr_buffer[ BUFFER_SIZE ];
  PaletteEntry palette[ 256 ];
  int result = FALSE;
  unsigned char* src = buffer;
  unsigned char* gray_palette = pal_buffer;
  unsigned char* bgr = bgr_buffer;
  int m_width = im->w;
  int m_height = im->h;
  int m_iscolor = im->c > 1;
  int m_offset = fmt->m_offset, m_maxval = fmt->m_maxval;
  int m_bpp = fmt->m_bpp, m_binary = fmt->m_binary;
  int src_pitch = (m_width * m_bpp + 7) / 8;
  int nch = m_iscolor ? 3 : 1;
  int width3 = m_width * nch;
  int color = channels > 1;
  int i, x, y;

  if (m_offset < 0 || !m_strm) {
    return FALSE;
  }

  if (src_pitch + 32 > BUFFER_SIZE) {
    src = (unsigned char*) pmalloc(sizeof(unsigned char) * (width3 + 32));
  }

  if (m_maxval + 1 > BUFFER_SIZE) {
    gray_palette = (unsigned char*) pmalloc(sizeof(unsigned char) * (m_maxval + 1));
  }

  if (m_width * 3 + 32 > BUFFER_SIZE) {
    bgr = (unsigned char*) pmalloc(sizeof(unsigned char) * (im->w * 3 + 32));
  }

  // create LUT for converting colors
  for (i = 0; i <= fmt->m_maxval; i++) {
    gray_palette[ i ] = (unsigned char)((i * 255 / m_maxval) ^ (m_bpp == 1 ? 255 : 0));
  }

  FillGrayPalette(palette, m_bpp == 1 ? 1 : 8 , m_bpp == 1);

  //if( setjmp( m_strm.JmpBuf()) == 0 )
  {
    SetPos(m_offset);

    switch (m_bpp) {
      //////////////////////////////////////////////////////////////////////////////// 1 BPP////////////////////////////////////////////////////////////////////////////////
    case 1:
      if (!m_binary) {
        for (y = 0; y < m_height; y++, data += step) {
          for (x = 0; x < m_width; x++) {
            src[ x ] = (unsigned char)(ReadNumber(m_strm, 1) != 0);
          }

          if (color) {
            FillColorRow8(data, src, m_width, palette);
          }
          else {
            FillGrayRow8(data, src, m_width, gray_palette);
          }
        }
      }
      else {
        for (y = 0; y < m_height; y++, data += step) {
          READ_NBYTE(src, src_pitch);

          if (color) {
            FillColorRow1(data, src, m_width, palette);
          }
          else {
            FillGrayRow1(data, src, m_width, gray_palette);
          }
        }
      }

      result = TRUE;
      break;

      //////////////////////////////////////////////////////////////////////////////// 8 BPP////////////////////////////////////////////////////////////////////////////////
    case 8:
    case 24:
      for (y = 0; y < m_height; y++, data += step) {
        if (!m_binary) {
          for (x = 0; x < width3; x++) {
            int code = ReadNumber(m_strm, INT_MAX);

            if ((unsigned) code > (unsigned) m_maxval) {
              code = m_maxval;
            }

            src[ x ] = gray_palette[ code ];
          }
        }
        else {
          READ_NBYTE(src, src_pitch);
        }

        if (m_bpp == 8) {
          if (color) {
            FillColorRow8(data, src, m_width, palette);
          }
          else {
            memcpy(data, src, m_width);
          }
        }
        else {
          if (color) {
            IMTRANS1D(m_width, src, 3, data, 3, RGB2BGR);
          }
          else {
            IMTRANS1D(m_width, src, 3, data, 1, BGR2GRAY);
          }
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

  if (gray_palette != pal_buffer) {
    pfree(gray_palette);
  }

#undef BUFFER_SIZE
  return result;
}

static char PxMLUT[ 256 ][ 5 ];
static int isPxMLUTInitialized = FALSE;

int fmtpxm_write(ihand_t m_strm, const img_t* im, const PaletteEntry* palette)
{
  int height = im->h, width = im->w, step = im->s, channels = im->c;
  const unsigned char* data = im->tt.data;
  int isBinary = FALSE;
  int result = FALSE;

  int channels0 = channels;
  int fileStep = width * channels0;
  int x, y;

  assert(data && width > 0 && height > 0 && step >= fileStep);

  if (m_strm) {
    int lineLength = ((isBinary ? 1 : 4) * channels0 +
        (channels0 > 1 ? 2 : 0)) * width + 32;
    int bufferSize = 128; // buffer that should fit a header
    char* buffer = 0;

    if (bufferSize < lineLength) {
      bufferSize = lineLength;
    }

    buffer = (char*) pmalloc(sizeof(char) * bufferSize);

    if (!buffer) {
      return FALSE;
    }

    if (!isPxMLUTInitialized) {
      int i;

      for (i = 0; i < 256; i++) {
        sprintf(PxMLUT[ i ], "%4d", i);
      }

      isPxMLUTInitialized = 1;
    }

    // write header;
    sprintf(buffer, "P%c\n%d %d\n255\n",
        '2' + (channels0 > 1 ? 1 : 0) + (isBinary ? 3 : 0),
        width, height);

    WRITE_NBYTE(buffer, (int) strlen(buffer));

    for (y = 0; y < height; y++, data += step) {
      if (isBinary) {
        if (channels == 3) {
          IMTRANS1D(width, data, 3, buffer, 3, BGR2RGB);
        }
        else if (channels == 4) {
          IMTRANS1D(width, data, 4, buffer, 3, BGRA2BGR);
        }

        WRITE_NBYTE(channels0 > 1 ? buffer : (char*) data, fileStep);
      }
      else {
        char* ptr = buffer;

        if (channels0 > 1) {
          for (x = 0; x < width * channels0; x += channels0) {
            strcpy(ptr, PxMLUT[ data[ x + 2 ] ]);
            ptr += 4;
            strcpy(ptr, PxMLUT[ data[ x + 1 ] ]);
            ptr += 4;
            strcpy(ptr, PxMLUT[ data[ x + 0 ] ]);
            ptr += 4;
            *ptr++ = ' ';
            *ptr++ = ' ';
          }
        } else {
          for (x = 0; x < width; x++) {
            strcpy(ptr, PxMLUT[ data[ x ] ]);
            ptr += 4;
          }
        }

        *ptr++ = '\n';

        WRITE_NBYTE(buffer, (int)(ptr - buffer));
      }
    }

    pfree(buffer);
    result = TRUE;
  }

  return result;
}


#endif // SUPPORT_PXM
