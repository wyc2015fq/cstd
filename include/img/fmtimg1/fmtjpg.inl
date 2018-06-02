#ifndef _FMTJPG_INL_
#define _FMTJPG_INL_

#include <setjmp.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "_fmtimg.h"
#include "lib/libjpeg-8b.inl"

#ifdef _MSC_VER
//#pragma comment (lib,"libjpeg.lib")
#endif
#ifdef __GNUC__
#pragma comment (lib,"libjpeg.a")
#endif

#ifdef _WINDOWS_

#define XMD_H // prevent redefinition of INT32
#undef FAR  // prevent FAR redefinition

#endif

#if defined _WINDOWS_ && defined __GNUC__
typedef unsigned char BOOLean;
#endif

#ifdef __cplusplus
extern "C" {
#endif
//#include "jpeglib.h"
#ifdef __cplusplus
}
#endif

//////////////////////////////////////////////////////////////////////////////// Error processing////////////////////////////////////////////////////////////////////////////////

typedef struct GrFmtJpegErrorMgr {
  struct jpeg_error_mgr pub;    /* "parent" structure */
  jmp_buf setjmp_buffer;        /* jump label */
}
GrFmtJpegErrorMgr;

METHODDEF(void)
error_exit2(j_common_ptr cinfo)
{
  GrFmtJpegErrorMgr* err_mgr = (GrFmtJpegErrorMgr*)(cinfo->err);

  /* Return control to the setjmp point */
  longjmp(err_mgr->setjmp_buffer, 1);
}

/***************************************************************************
 * following code is for supporting MJPEG image files
 * based on a message of Laurent Pinchart on the video4linux mailing list
 ***************************************************************************/

/* JPEG DHT Segment for YCrCb omitted from MJPEG data */
static
unsigned char my_jpeg_odml_dht[ 0x1a4 ] = {  //
  //
  0xff, 0xc4, 0x01, 0xa2,
  0x00, 0x00, 0x01, 0x05, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,
  0x01, 0x00, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,
  0x10, 0x00, 0x02, 0x01, 0x03, 0x03, 0x02, 0x04, 0x03, 0x05, 0x05, 0x04, 0x04, 0x00, 0x00, 0x01, 0x7d,
  0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12, 0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
  0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08, 0x23, 0x42, 0xb1, 0xc1, 0x15, 0x52, 0xd1, 0xf0,
  0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0a, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x25, 0x26, 0x27, 0x28,
  0x29, 0x2a, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
  0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
  0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
  0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
  0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4, 0xc5,
  0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2,
  0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa,
  0x11, 0x00, 0x02, 0x01, 0x02, 0x04, 0x04, 0x03, 0x04, 0x07, 0x05, 0x04, 0x04, 0x00, 0x01, 0x02, 0x77,
  0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21, 0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71,
  0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91, 0xa1, 0xb1, 0xc1, 0x09, 0x23, 0x33, 0x52, 0xf0,
  0x15, 0x62, 0x72, 0xd1, 0x0a, 0x16, 0x24, 0x34, 0xe1, 0x25, 0xf1, 0x17, 0x18, 0x19, 0x1a, 0x26,
  0x27, 0x28, 0x29, 0x2a, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
  0x49, 0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
  0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
  0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5,
  0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3,
  0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda,
  0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa
};

/*
 * Parse the DHT table.
 * This code comes from jpeg6b (jdmarker.c).
 */
static
int my_jpeg_load_dht(struct jpeg_decompress_struct* info, unsigned char* dht,
    JHUFF_TBL* ac_tables[], JHUFF_TBL* dc_tables[])
{
  unsigned int length = (dht[ 2 ] << 8) + dht[ 3 ] - 2;
  unsigned int pos = 4;
  unsigned int count, i;
  int index;

  JHUFF_TBL** hufftbl;
  unsigned char bits[ 17 ];
  unsigned char huffval[ 256 ];

  while (length > 16) {
    bits[ 0 ] = 0;
    index = dht[ pos++ ];
    count = 0;

    for (i = 1; i <= 16; ++i) {
      bits[ i ] = dht[ pos++ ];
      count += bits[ i ];
    }

    length -= 17;

    if (count > 256 || count > length) {
      return -1;
    }

    for (i = 0; i < count; ++i) {
      huffval[ i ] = dht[ pos++ ];
    }

    length -= count;

    if (index & 0x10) {
      index -= 0x10;
      hufftbl = &ac_tables[ index ];
    }
    else {
      hufftbl = &dc_tables[ index ];
    }

    if (index < 0 || index >= NUM_HUFF_TBLS) {
      return -1;
    }

    if (*hufftbl == NULL) {
      * hufftbl = jpeg_alloc_huff_table((j_common_ptr) info);
    }

    if (*hufftbl == NULL) {
      return -1;
    }

    memcpy((*hufftbl) ->bits, bits, sizeof(*hufftbl) ->bits);
    memcpy((*hufftbl) ->huffval, huffval, sizeof(*hufftbl) ->huffval);
  }

  if (length != 0) {
    return -1;
  }

  return 0;
}

/***************************************************************************
 * end of code for supportting MJPEG image files
 * based on a message of Laurent Pinchart on the video4linux mailing list
 ***************************************************************************/

// information about each component
typedef struct cmp_info {
  char h;  // horizontal sampling factor
  char v;  // vertical   sampling factor
  char tq; // quantization table index
  char td, ta; // DC & AC huffman tables
  int dc_pred; // DC predictor
}
cmp_info;

typedef struct fmtjpg {
  int m_offset; // offset of first scan
  int m_version; // JFIF version
  int m_planes; // 3 (YCrCb) or 1 (Gray)
  int m_precision; // 8 or 12-bit per sample
  int m_type; // SOF type
  int m_MCUs; // MCUs in restart interval
  int m_ss, m_se, m_ah, m_al; // progressive JPEG parameters

  struct cmp_info m_ci[ 3 ];

  int m_tq[ 4 ][ 64 ];
  int m_is_tq[ 4 ];

  struct jpeg_decompress_struct m_cinfo[1]; // pointer to IJG JPEG codec structure
  GrFmtJpegErrorMgr m_jerr[1]; // pointer to error processing manager state
}
fmtjpg;

int fmtjpg_free(fmtjpg* fmt)
{
  jpeg_destroy_decompress(fmt->m_cinfo);
  return 1;
}


#define fmtjpg_extA ".jpg|.jpe|.jpeg"
#define fmtjpg_extW L".jpg|.jpe|.jpeg"

int fmtjpg_checkext(const TCHAR* picname)
{
  const TCHAR* pext = _tcsrchr(picname, '.');

  if (!pext) {
    return 0;
  }

  return !_tcsicmp(pext, _T(".jpg"))        //
      || !_tcsicmp(pext, _T(".jpe"))        //
      || !_tcsicmp(pext, _T(".jpeg"))        //
      ;
}

int fmtjpg_read_header(ihand_t m_strm, img_t* im, fmtjpg* fmt)
{
  int result = FALSE;
  struct jpeg_decompress_struct* cinfo;
  GrFmtJpegErrorMgr* jerr;
  const char* m_signature = "\xFF\xD8\xFF";
  const char* m_description = _T("JPEG files (*.jpeg;*.jpg;*.jpe)");

  if (!checksign(m_strm, m_signature, 3)) {
    return 0;
  }

  memset(fmt, 0, sizeof(fmtjpg));

  SetPos(0);
  //clear();
  cinfo = fmt->m_cinfo;
  jerr = fmt->m_jerr;

  cinfo->err = jpeg_std_error(&jerr->pub);
  jerr->pub.error_exit = error_exit2;

  if (setjmp(jerr->setjmp_buffer) == 0) {
    jpeg_create_decompress(cinfo);

    if (m_strm) {
      if (xft_xio == xftype(m_strm)) {
        XFILE_IO* xio = (XFILE_IO*)(m_strm);
        jpeg_stdio_src(cinfo, xio->m_fp);
      }
      else {
        XFILE_MEM* mem = (XFILE_MEM*)(m_strm);
        jpeg_mem_src(cinfo, mem->m_pBuffer, mem->m_Size);
      }

      jpeg_read_header(cinfo, TRUE);

      im->w = cinfo->image_width;
      im->h = cinfo->image_height;
      im->c = cinfo->num_components;
      im->f = 1;
      result = TRUE;
    }
  }

  if (!result) {
    fmtjpg_free(fmt);
  }

  return result;
}

int fmtjpg_read_data(ihand_t m_strm, img_t* im, fmtjpg* fmt)
{
  unsigned char* data = im->tt.data;
  int step = im->s, channels = im->c;
  int result = FALSE;
  int color = channels > 1;
  int m_width = im->w, m_height = im->h;
  void* m_cinfo = fmt->m_cinfo;
  void* m_jerr = fmt->m_jerr;

  if (m_cinfo && m_jerr && m_width && m_height) {
    struct jpeg_decompress_struct* cinfo = (struct jpeg_decompress_struct*) m_cinfo;
    GrFmtJpegErrorMgr* jerr = (GrFmtJpegErrorMgr*) m_jerr;
    JSAMPARRAY buffer = 0;

    if (setjmp(jerr->setjmp_buffer) == 0) {
      /* check if this is a mjpeg image format */
      if (cinfo->ac_huff_tbl_ptrs[ 0 ] == NULL &&
          cinfo->ac_huff_tbl_ptrs[ 1 ] == NULL &&
          cinfo->dc_huff_tbl_ptrs[ 0 ] == NULL &&
          cinfo->dc_huff_tbl_ptrs[ 1 ] == NULL) {
        // yes, this is a mjpeg image format, so load the correct huffman table
        my_jpeg_load_dht(cinfo, my_jpeg_odml_dht, cinfo->ac_huff_tbl_ptrs, cinfo->dc_huff_tbl_ptrs);
      }

      if (color) {
        if (cinfo->num_components != 4) {
          cinfo->out_color_space = JCS_RGB;
          cinfo->out_color_components = 3;
        }
        else {
          cinfo->out_color_space = JCS_CMYK;
          cinfo->out_color_components = 4;
        }
      }
      else {
        if (cinfo->num_components != 4) {
          cinfo->out_color_space = JCS_GRAYSCALE;
          cinfo->out_color_components = 1;
        }
        else {
          cinfo->out_color_space = JCS_CMYK;
          cinfo->out_color_components = 4;
        }
      }

      jpeg_start_decompress(cinfo);

      buffer = (*cinfo->mem->alloc_sarray)((j_common_ptr) cinfo, JPOOL_IMAGE, m_width * 4, 1);

      for (; m_height--; data += step) {
        jpeg_read_scanlines(cinfo, buffer, 1); //读取一行jpg图像数据到buffer

        if (color) {
          if (cinfo->out_color_components == 3) {
            IMTRANS1D(m_width, ((unsigned char*) buffer[ 0 ]), 3, data, channels, RGB2BGR);
          }
          else {
            IMTRANS1D(m_width, ((unsigned char*) buffer[ 0 ]), 4, data, channels, CMYK2BGR);
          }
        }
        else {
          if (cinfo->out_color_components == 1) {
            memcpy(data, buffer[ 0 ], m_width);
          }
          else {
            IMTRANS1D(m_width, (unsigned char*) buffer[ 0 ], 4, data, 1, CMYK2GRAY);
          }
        }
      }

      result = TRUE;
      jpeg_finish_decompress(cinfo);
    }
  }

  return result;
}

int fmtjpg_write(ihand_t m_strm, const img_t* im, const PaletteEntry* palette)
{
  int height = im->h, width = im->w, step = im->s, channels = im->c;
  const unsigned char* data = im->tt.data;
  const int default_quality = 95;
  struct jpeg_compress_struct cinfo = {0};
  GrFmtJpegErrorMgr jerr;

  int m_width = width;

  int result = FALSE;
  int channels0 = channels > 1 ? 3 : 1;
  unsigned char* buffer = 0; // temporary buffer for row flipping

  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = error_exit2;

  if (setjmp(jerr.setjmp_buffer) == 0) {
    jpeg_create_compress(&cinfo);

    if (m_strm) {
      if (xft_xio == xftype(m_strm)) {
        jpeg_stdio_dest(&cinfo, m_strm->m_fp);
      }
      else {
        //对于JPEG格式，只需用 jpeg_mem_src 函数替代原来从文件读取的 jpeg_stdio_src(&cinfo, infile)即可。
        jpeg_mem_dest(&cinfo, &m_strm->m_pBuffer, &m_strm->m_Size);
      }

      cinfo.image_width = width;
      cinfo.image_height = height;
      cinfo.input_components = channels0;
      cinfo.in_color_space = channels0 > 1 ? JCS_RGB : JCS_GRAYSCALE;

      jpeg_set_defaults(&cinfo);
      jpeg_set_quality(&cinfo, default_quality, TRUE /* limit to baseline-JPEG values */);
      jpeg_start_compress(&cinfo, TRUE);

      if (channels0 > 1) {
        buffer = (unsigned char*)pmalloc(width * channels0);
      }

      for (; height--; data += step) {
        unsigned char* ptr = (unsigned char*) data;

        if (channels == 3) {
          IMTRANS1D(m_width, data, 3, buffer, 3, BGR2RGB);
          ptr = buffer;
        }
        else if (channels == 4) {
          IMTRANS1D(m_width, data, 4, buffer, 3, BGRA2BGR);
          ptr = buffer;
        }

        jpeg_write_scanlines(&cinfo, &ptr, 1);
      }

      jpeg_finish_compress(&cinfo);
      result = TRUE;
    }
  }

  jpeg_destroy_compress(&cinfo);

  if (buffer) {
    pfree(buffer);
  }

  return result;
}

#endif // _FMTJPG_INL_
