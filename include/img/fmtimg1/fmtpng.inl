#ifndef _FMTPNG_INL_
#define _FMTPNG_INL_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "_fmtimg.h"
#include "lib/libzlib.inl"
#include "lib/libpng.inl"
//#include "lib/libpng.inl"

#ifdef _MSC_VER
//#pragma comment (lib,"libpng.lib")
//#pragma comment (lib,"zlib.lib")
#endif
#ifdef __GNUC__
#pragma comment (lib,"libjpeg.a")
#endif

#define HAVE_PNG
#ifdef HAVE_PNG

typedef struct fmtpng {
  void* m_png_ptr;  // pointer to decompression structure
  void* m_info_ptr; // pointer to image information structure
  void* m_end_info; // pointer to one more image information structure
  int m_color_type;
  int m_bit_depth;
}
fmtpng;

int fmtpng_checksign(ihand_t m_strm)
{
  int re = 0;
  int m_sign_len = 8;
  char psign[8];
  re = getsign(m_strm, psign, m_sign_len);

  if (re) {
    re = png_check_sig((unsigned char*) psign, m_sign_len) != 0;
  }

  return re;
}

int fmtpng_free(fmtpng* fmt)
{
  if (fmt->m_png_ptr) {
    png_structp png_ptr = (png_structp) fmt->m_png_ptr;
    png_infop info_ptr = (png_infop) fmt->m_info_ptr;
    png_infop end_info = (png_infop) fmt->m_end_info;
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    fmt->m_png_ptr = fmt->m_info_ptr = fmt->m_end_info = 0;
  }

  return 0;
}

// PNG Filter Factory

typedef struct {
  unsigned char* data;
  int size;
  int offset;
} ImageSource;

//从内存读取要重设读取回调函数，然后用png_set_read_fn（）函数指明读函数。
//从内存读取PNG图片的回调函数
static void pngReadCallback(png_structp png_ptr, png_bytep data, png_size_t length)
{
  ImageSource* isource = (ImageSource*)png_get_io_ptr(png_ptr);

  if (isource->offset + (int)length <= (int)isource->size) {
    memcpy(data, isource->data + isource->offset, length);
    isource->offset += length;
  }
  else {
    png_error(png_ptr, "pngReaderCallback failed");
  }
}

int fmtpng_read_header(ihand_t m_strm, img_t* im, fmtpng* fmt)
{
  int result = FALSE;
  png_structp png_ptr;
  int m_sign_len = 8;
  const char* m_signature = "\x89\x50\x4e\x47\xd\xa\x1a\xa";
  const char* m_description = _T("Portable Network Graphics files (*.png)");
  fmt->m_color_type = 0;
  fmt->m_png_ptr = 0;
  fmt->m_info_ptr = fmt->m_end_info = 0;

  if (!fmtpng_checksign(m_strm)) {
    return 0;
  }

  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);

  SetPos(0);

  if (png_ptr) {
    png_infop info_ptr = png_create_info_struct(png_ptr);
    png_infop end_info = png_create_info_struct(png_ptr);

    fmt->m_png_ptr = png_ptr;
    fmt->m_info_ptr = info_ptr;
    fmt->m_end_info = end_info;

    if (info_ptr && end_info) {
      if (setjmp(png_ptr->jmpbuf) == 0) {
        if (m_strm) {
          png_uint_32 width, height;
          int bit_depth, color_type;
          ImageSource imgsource;

          if (xft_xio == xftype(m_strm)) {
            png_init_io(png_ptr, (png_FILE_p)(m_strm->m_fp));
          }
          else {
            imgsource.data = m_strm->m_pBuffer;
            imgsource.size = m_strm->m_Size;
            imgsource.offset = 0;
            png_set_read_fn(png_ptr, &imgsource, pngReadCallback);
          }

          png_read_info(png_ptr, info_ptr);

          png_get_IHDR(png_ptr, info_ptr, &width, &height,
              &bit_depth, &color_type, 0, 0, 0);

          im->c = color_type == PNG_COLOR_TYPE_RGB ? 3 :
              color_type == PNG_COLOR_TYPE_RGB_ALPHA ? 4 :
              color_type == PNG_COLOR_TYPE_PALETTE ? 3 : 1;

          im->w = (int) width;
          im->h = (int) height;
          im->f = 1;
          fmt->m_color_type = color_type;
          fmt->m_bit_depth = bit_depth;

          result = TRUE;
        }
      }
    }
  }

  return result;
}

#define  isBigEndian() ((((const int*)"\0\x1\x2\x3\x4\x5\x6\x7")[0] & 255) != 0)

int fmtpng_read_data(ihand_t m_strm, img_t* im, fmtpng* fmt)
{
  unsigned char* data = im->tt.data;
  int step = im->s, cn = im->c;
  int result = FALSE;
  int color = cn > 1;
  unsigned char** buffer = 0;
  double display_exponent = 2.2;
  double gamma = 0;
  int m_iscolor = im->c > 1;

  if (fmt->m_png_ptr && fmt->m_info_ptr &&
      fmt->m_end_info && im->w &&
      im->h) {
    png_structp png_ptr = (png_structp) fmt->m_png_ptr;
    png_infop info_ptr = (png_infop) fmt->m_info_ptr;
    png_infop end_info = (png_infop) fmt->m_end_info;

    if (setjmp(png_jmpbuf(png_ptr)) == 0) {
      int y;

      if (fmt->m_bit_depth > 8) {
        png_set_strip_16(png_ptr);
      }
      else if (!isBigEndian()) {
        png_set_swap(png_ptr);
      }

      /* observation: png_read_image() writes 400 bytes beyond
       * end of data when reading a 400x118 color png
       * "mpplus_sand.png".  OpenCV crashes even with demo
       * programs.  Looking at the loaded image I'd say we get 4
       * bytes per pixel instead of 3 bytes per pixel.  Test
       * indicate that it is a good idea to always ask for
       * stripping alpha..  18.11.2004 Axel Walthelm
       */
      //png_set_strip_alpha( png_ptr );

      if (fmt->m_color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png_ptr);
      }

      if (fmt->m_color_type == PNG_COLOR_TYPE_GRAY && fmt->m_bit_depth < 8) {
        png_set_gray_1_2_4_to_8(png_ptr);
      }

      if (m_iscolor && color) {
        png_set_bgr(png_ptr);  // convert RGB to BGR
      }
      else if (color) {
        png_set_gray_to_rgb(png_ptr);  // Gray->RGB
      }
      else {
        png_set_rgb_to_gray(png_ptr, 1, -1, -1);  // RGB->Gray
      }

      /* unlike the example in the libpng documentation, we have *no* idea where
       * this file may have come from--so if it doesn't have a file gamma, don't
       * do any correction ("do no harm") */

      //if (png_get_gAMA(png_ptr, info_ptr, &gamma))
      //  png_set_gamma(png_ptr, display_exponent, gamma);

      png_read_update_info(png_ptr, info_ptr);
      {
        int Channels, Rowbytes;
        unsigned char* tmpbuf = NULL;
        Channels = (int) png_get_channels(png_ptr, info_ptr);
        buffer = (unsigned char**) pmalloc(sizeof(unsigned char*) * im->h);

        if (Channels == cn) {
          for (y = 0; y < im->h; y++) {
            buffer[ y ] = data + y * step;
          }

          png_read_image(png_ptr, buffer);
        }
        else if (2 == Channels && 3 == cn) {
          Rowbytes = png_get_rowbytes(png_ptr, info_ptr);
          tmpbuf = (unsigned char*)pmalloc(Rowbytes * im->h);

          for (y = 0; y < im->h; y++) {
            buffer[ y ] = tmpbuf + y * Rowbytes;
          }

          png_read_image(png_ptr, buffer);

          for (y = 0; y < im->h; y++) {
            int x;
            unsigned char* src, *dst;
            unsigned char r, a;
            src = tmpbuf + y * Rowbytes;
            dst = data + y * step;

            for (x = 0; x < im->w; ++x, src += 2, dst += 3) {
              r = src[ 0 ];
              a = src[ 1 ];

              if (a == 255) {
                dst[ 0 ] = r;
                dst[ 1 ] = r;
                dst[ 2 ] = r;
              }
              else {
                alpha_composite(dst[ 0 ], r, a, dst[ 0 ]);
                alpha_composite(dst[ 1 ], r, a, dst[ 1 ]);
                alpha_composite(dst[ 2 ], r, a, dst[ 2 ]);
              }
            }

            buffer[ y ] = data + y * Rowbytes;
          }

          pfree(tmpbuf);
        }
        else if (2 == Channels && 1 == cn) {
          Rowbytes = png_get_rowbytes(png_ptr, info_ptr);
          tmpbuf = (unsigned char*)pmalloc(Rowbytes * im->h);

          for (y = 0; y < im->h; y++) {
            buffer[ y ] = tmpbuf + y * Rowbytes;
          }

          png_read_image(png_ptr, buffer);

          for (y = 0; y < im->h; y++) {
            int x;
            unsigned char* src, *dst;
            unsigned char r, a;
            src = tmpbuf + y * Rowbytes;
            dst = data + y * step;

            for (x = 0; x < im->w; ++x, src += 2, dst += 1) {
              r = src[ 0 ];
              a = src[ 1 ];

              if (a == 255) {
                dst[ 0 ] = r;
              }
              else {
                alpha_composite(dst[ 0 ], r, a, dst[ 0 ]);
              }
            }

            buffer[ y ] = data + y * Rowbytes;
          }

          pfree(tmpbuf);
        }
        else if (4 == Channels && 3 == cn) {
          Rowbytes = png_get_rowbytes(png_ptr, info_ptr);
          tmpbuf = (unsigned char*)pmalloc(Rowbytes * im->h);

          for (y = 0; y < im->h; y++) {
            buffer[ y ] = tmpbuf + y * Rowbytes;
          }

          png_read_image(png_ptr, buffer);

          for (y = 0; y < im->h; y++) {
            int x;
            unsigned char* src, *dst;
            unsigned char r, g, b, a;
            src = tmpbuf + y * Rowbytes;
            dst = data + y * step;

            for (x = 0; x < im->w; ++x, src += 4, dst += 3) {
              r = src[ 0 ];
              g = src[ 1 ];
              b = src[ 2 ];
              a = src[ 3 ];

              if (a == 255) {
                dst[ 0 ] = r;
                dst[ 1 ] = g;
                dst[ 2 ] = b;
              }
              else {
                /* this macro (copied from png.h) composites the
                * foreground and background values and puts the
                * result into the first argument; there are no
                  * side effects with the first argument */
                alpha_composite(dst[ 0 ], r, a, dst[ 0 ]);
                alpha_composite(dst[ 1 ], g, a, dst[ 1 ]);
                alpha_composite(dst[ 2 ], b, a, dst[ 2 ]);
              }
            }

            buffer[ y ] = data + y * Rowbytes;
          }

          pfree(tmpbuf);
        }
        else if (3 == Channels && 4 == cn) {
          Rowbytes = png_get_rowbytes(png_ptr, info_ptr);
          tmpbuf = (unsigned char*)pmalloc(Rowbytes * im->h);

          for (y = 0; y < im->h; y++) {
            buffer[ y ] = tmpbuf + y * Rowbytes;
          }

          png_read_image(png_ptr, buffer);

          for (y = 0; y < im->h; y++) {
            int x;
            unsigned char* src, *dst;
            unsigned char r, g, b, a;
            src = tmpbuf + y * Rowbytes;
            dst = data + y * step;

            for (x = 0; x < im->w; ++x, src += 3, dst += 4) {
              r = src[ 0 ];
              g = src[ 1 ];
              b = src[ 2 ];
              a = (src[ 0 ] + src[ 1 ] + src[ 1 ] + src[ 2 ]) / 4;
              dst[ 3 ] = a;

              if (a != 255) {
                dst[ 0 ] = r;
                dst[ 1 ] = g;
                dst[ 2 ] = b;
              }
            }

            buffer[ y ] = data + y * Rowbytes;
          }

          pfree(tmpbuf);
        }

      }
      png_read_end(png_ptr, end_info);

      result = TRUE;
    }
  }

  if (buffer) {
    pfree(buffer);
  }

  return result;
}

//////////////////////////////////////////////////////////////////////////////// fmtpng_write////////////////////////////////////////////////////////////////////////////////

#define fmtpng_extA ".png"
#define fmtpng_extW L".png"

#define IMG_DEPTH_8U     8
#define IMG_DEPTH_16U   16
int fmtpng_write(ihand_t m_strm, const img_t* im, const PaletteEntry* palette)
{
  int height = im->h, width = im->w, step = im->s, channels = im->c;
  const unsigned char* data = im->tt.data;
  png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
  png_infop info_ptr = 0;
  unsigned char** buffer = 0;
  int y;
  int depth = IMG_DEPTH_8U;
  int result = FALSE;

  if (depth != IMG_DEPTH_8U && depth != IMG_DEPTH_16U) {
    return FALSE;
  }

  if (png_ptr) {
    info_ptr = png_create_info_struct(png_ptr);

    if (info_ptr) {
      if (setjmp(png_ptr->jmpbuf) == 0) {

        if (m_strm) {
          png_init_io(png_ptr, (png_FILE_p)(m_strm->m_fp));

          png_set_compression_mem_level(png_ptr, MAX_MEM_LEVEL);

          png_set_IHDR(png_ptr, info_ptr, width, height, depth,
              channels == 1 ? PNG_COLOR_TYPE_GRAY :
              channels == 3 ? PNG_COLOR_TYPE_RGB : PNG_COLOR_TYPE_RGBA,
              PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
              PNG_FILTER_TYPE_DEFAULT);

          png_write_info(png_ptr, info_ptr);

          png_set_bgr(png_ptr);

          if (!isBigEndian()) {
            png_set_swap(png_ptr);
          }

          buffer = (unsigned char**)pmalloc(sizeof(unsigned char*) * height);

          for (y = 0; y < height; y++) {
            buffer[ y ] = (unsigned char*)(data + y * step);
          }

          png_write_image(png_ptr, buffer);
          png_write_end(png_ptr, info_ptr);

          pfree(buffer);

          result = TRUE;
        }
      }
    }
  }

  png_destroy_write_struct(&png_ptr, &info_ptr);

  return result;
}

int fmtpng_write16(FILE* m_fp, int height, int width, const unsigned char* data, int step, int channels)
{
  png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
  png_infop info_ptr = 0;
  unsigned char** buffer = 0;
  int y;
  int depth = IMG_DEPTH_16U;
  int result = FALSE;

  if (depth != IMG_DEPTH_8U && depth != IMG_DEPTH_16U) {
    return FALSE;
  }

  if (png_ptr) {
    info_ptr = png_create_info_struct(png_ptr);

    if (info_ptr) {
      if (setjmp(png_ptr->jmpbuf) == 0) {

        if (m_fp) {
          png_init_io(png_ptr, (png_FILE_p)(m_fp));

          png_set_compression_mem_level(png_ptr, MAX_MEM_LEVEL);

          png_set_IHDR(png_ptr, info_ptr, width, height, depth,
              channels == 1 ? PNG_COLOR_TYPE_GRAY :
              channels == 3 ? PNG_COLOR_TYPE_RGB : PNG_COLOR_TYPE_RGBA,
              PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
              PNG_FILTER_TYPE_DEFAULT);

          png_write_info(png_ptr, info_ptr);

          png_set_bgr(png_ptr);

          if (!isBigEndian()) {
            png_set_swap(png_ptr);
          }

          buffer = (unsigned char**)pmalloc(sizeof(unsigned char*) * height);

          for (y = 0; y < height; y++) {
            buffer[ y ] = (unsigned char*)(data + y * step);
          }

          png_write_image(png_ptr, buffer);
          png_write_end(png_ptr, info_ptr);

          pfree(buffer);

          result = TRUE;
        }
      }
    }
  }

  png_destroy_write_struct(&png_ptr, &info_ptr);

  return result;
}

#endif

#endif // endif
