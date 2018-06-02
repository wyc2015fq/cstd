#ifndef SUPPORT_TIF
#define SUPPORT_TIF

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#include "_fmtimg.h"

#define HAVE_TIFF
#ifdef HAVE_TIFF

#ifdef _MSC_VER
//#pragma comment (lib,"libtiff.lib")
#endif
#ifdef __GNUC__
#pragma comment (lib,"libtiff.a")
#endif

#include ".\lib\include\tiff.h"
#include ".\lib\include\tiffio.h"

// native simple TIFF codec
enum TiffCompression {
  TIFF_UNCOMP = 1,
  TIFF_HUFFMAN = 2,
  TIFF_PACKBITS = 32773
};

enum TiffByteOrder {
  TIFF_ORDER_II = 0x4949,
  TIFF_ORDER_MM = 0x4d4d
};

enum TiffTag {
  TIFF_TAG_WIDTH = 256,
  TIFF_TAG_HEIGHT = 257,
  TIFF_TAG_BITS_PER_SAMPLE = 258,
  TIFF_TAG_COMPRESSION = 259,
  TIFF_TAG_PHOTOMETRIC = 262,
  TIFF_TAG_STRIP_OFFSETS = 273,
  TIFF_TAG_STRIP_COUNTS = 279,
  TIFF_TAG_SAMPLES_PER_PIXEL = 277,
  TIFF_TAG_ROWS_PER_STRIP = 278,
  TIFF_TAG_PLANAR_CONFIG = 284,
  TIFF_TAG_COLOR_MAP = 320
};

enum TiffFieldType {
  TIFF_TYPE_BYTE = 1,
  TIFF_TYPE_SHORT = 3,
  TIFF_TYPE_LONG = 4
};

typedef struct fmttif {
  FMTIMGBASEFEILD;
  void* m_tif;
}
fmttif;

static const char fmtSignTiffII[] = "II\x2a\x00";
static const char fmtSignTiffMM[] = "MM\x00\x2a";

#define TIF_SIGN_LEN 4

int fmttif_checksign(ihand_t m_strm)
{
  int re = 0;
  int m_sign_len = TIF_SIGN_LEN;
  char* psign = (char*) pmalloc(m_sign_len * sizeof(char));

  if (psign) {
    re = getsign(m_strm, psign, m_sign_len);

    if (re) {
      re = memcmp(psign, fmtSignTiffII, TIF_SIGN_LEN) == 0 ||
          memcmp(psign, fmtSignTiffMM, TIF_SIGN_LEN) == 0;
    }

    pfree(psign);
  }

  return re;
}

int fmttif_free(struct fmtimgreader* reader)
{
  fmttif* fmt = (fmttif*) reader;

  if (fmt->m_tif) {
    TIFF* tif = (TIFF*) fmt->m_tif;
    TIFFClose(tif);
    fmt->m_tif = 0;
  }

  pfree(reader);
  return 0;
}

static int grfmt_tiff_err_handler_init = 0;

//static void GrFmtSilentTIFFErrorHandler( const TCHAR* a, const TCHAR* b, va_list ) {}

void* GrFmtSilentTIFFErrorHandler = 0;

int fmttif_read(const struct fmtimgreader* reader, unsigned char* data, int step, int color);

#define fmttif_extA ".tiff|.tif"
#define fmttif_extW L".tiff|.tif"

// PNG Filter Factory
void fmttif_init(struct fmtimgreader* reader)
{
  fmttif* fmt = (fmttif*) reader;
  reader->m_sign_len = TIF_SIGN_LEN;
  reader->m_signature = "";
  reader->m_description = _T("TIFF Files (*.tiff;*.tif)");
  fmt->m_tif = 0;

  if (!grfmt_tiff_err_handler_init) {
    grfmt_tiff_err_handler_init = 1;

    TIFFSetErrorHandler(GrFmtSilentTIFFErrorHandler);
    TIFFSetWarningHandler(GrFmtSilentTIFFErrorHandler);
  }

  reader->freereader = fmttif_free;
  reader->read = fmttif_read;
}

tsize_t myTIFFRead(thandle_t m_strm, tdata_t d, tsize_t s)
{
  int aaa;
  aaa = READ_NBYTE(d, s);
  return aaa;
}

tsize_t myTIFFWrite(thandle_t m_strm, tdata_t d, tsize_t s)
{
  return WRITE_NBYTE(d, s);
}

toff_t myTIFFSeek(thandle_t m_strm, toff_t o, int l)
{
  int aa;
  aa = SEEK(o, l);
  aa = GetPos();
  return aa;
}

// "fileio.h"
int myTIFFClose(thandle_t m_strm)
{
  ihand_t file = (ihand_t)m_strm;
  xfclose(file);
  return 0;
}
toff_t myTIFFSize(thandle_t m_strm)
{
  ihand_t file = (ihand_t)m_strm;
  int old = xftell(file), len = 0;
  xfseek(file, 0, SEEK_END);
  len = xftell(file);
  xfseek(file, old, SEEK_SET);
  return len;
  return GetLength((FILE*)m_strm);
}

int myTIFFMapFile(thandle_t m_strm, tdata_t* d, toff_t* o)
{
  return 0;
}
void myTIFFUnmapFile(thandle_t m_strm, tdata_t d, toff_t o) {}

int fmttif_read(const struct fmtimgreader* reader, unsigned char* data, int step, int color);

struct fmtimgreader* fmttif_read_header(ihand_t m_strm) {
  int result = FALSE;
  struct fmtimgreader* reader = (struct fmtimgreader*) pmalloc(sizeof(fmttif));
  struct fmttif* fmt = (struct fmttif*) reader;
  char errmsg[ 1024 ];
  TIFF* tif;
  memset(reader, 0, sizeof(fmttif));

  SetPos(0);
  tif = TIFFClientOpen("mem", "r", (thandle_t) m_strm, myTIFFRead, myTIFFWrite,
      myTIFFSeek, myTIFFClose, myTIFFSize, 0, 0);

  if (tif) {
    int width = 0, height = 0, photometric = 0, compression = 0;
    fmt->m_tif = tif;

    if (TIFFRGBAImageOK(tif, errmsg) &&
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width) &&
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height) &&
        TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric) &&
        (!TIFFGetField(tif, TIFFTAG_COMPRESSION, &compression) ||
            (compression != COMPRESSION_LZW &&
                compression != COMPRESSION_OJPEG))) {
      reader->m_width = width;
      reader->m_height = height;
      //reader->m_iscolor = photometric > 1;

      result = TRUE;
    }
  }

  reader->freereader = (free_reader_func) fmttif_free;
  reader->read = fmttif_read;

  if (!result) {
    fmttif_free(reader);
    reader = NULL;
  }

  return reader;
}

int fmttif_read(const struct fmtimgreader* reader, unsigned char* data, int step, int channels)
{
  int result = FALSE;
  unsigned char* buffer = 0;
  struct fmttif* fmt = (struct fmttif*) reader;
  int m_height = reader->m_height;
  int m_width = reader->m_width;
  int m_iscolor = reader->m_channels > 1;
  int color = channels > 1;

  color = color > 0 || (color < 0 && m_iscolor);

  if (fmt->m_tif && m_width && m_height) {
    TIFF* tif = (TIFF*) fmt->m_tif;
    int tile_width0 = m_width, tile_height0 = 0;
    int x, y, i;
    int is_tiled = TIFFIsTiled(tif);

    if (!is_tiled &&
        TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &tile_height0) ||
        is_tiled &&
        TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width0) &&
        TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_height0)) {
      if (tile_width0 <= 0) {
        tile_width0 = m_width;
      }

      if (tile_height0 <= 0) {
        tile_height0 = m_height;
      }

      buffer = (unsigned char*) pmalloc(sizeof(unsigned char) * (tile_height0 * tile_width0 * 4 * 2));

      for (y = 0; y < m_height; y += tile_height0, data += step * tile_height0) {
        int tile_height = tile_height0;

        if (y + tile_height > m_height) {
          tile_height = m_height - y;
        }

        for (x = 0; x < m_width; x += tile_width0) {
          int tile_width = tile_width0, ok;

          if (x + tile_width > m_width) {
            tile_width = m_width - x;
          }

          if (!is_tiled) {
            ok = TIFFReadRGBAStrip(tif, y, (uint32*) buffer);
          }
          else {
            ok = TIFFReadRGBATile(tif, x, y, (uint32*) buffer);
          }

          if (!ok) {
            goto exit_func;
          }

          for (i = 0; i < tile_height; i++) {
            unsigned char* buffer2 = buffer + i * tile_width * 4;

            if (color) {
              unsigned char* data2 = data + x * 3 + step * (tile_height - i - 1);
              IMTRANS1D(tile_width, buffer2, 3, data2, 3, RGBA2BGR);
            }
            else {
              unsigned char* data2 = data + x + step * (tile_height - i - 1);
              IMTRANS1D(tile_width, buffer2, 4, data2, 1, RGB2GRAY);
            }
          }
        }
      }

      result = TRUE;
    }
  }

exit_func:

  pfree(buffer);

  return result;
}

#endif

////////////////////////////////////////////////////////////////////////////////

//typedef unsigned short WORD;

#define WriteTag( tag, fieldType, count, value )                              \
  do {                                                                          \
    WORD wd;                                                                    \
    DWORD dw;                                                                   \
    wd = tag; WRITE(wd);                                                        \
    wd = fieldType; WRITE(wd);                                                  \
    dw = count; WRITE(dw);                                                      \
    dw = value; WRITE(dw);                                                      \
  } while(0)

int fmttif_write(ihand_t m_strm, const img_t* im, const PaletteEntry* palette)
{
  int result = FALSE;
  int fileStep = width * channels;
  assert(data && width > 0 && height > 0 && step >= fileStep);

  if (m_strm) {
    int i, stripCount, directoryOffset, y = 0;
    int stripOffsetsOffset = 0, stripCountsOffset = 0, bitsPerSample = 8; // TODO support 16 bit
    int rowsPerStrip = (1 << 13) / fileStep;
    DWORD dw;
    WORD wd;
    int* stripOffsets;
    short* stripCounts;
    unsigned char* buffer;

    if (rowsPerStrip < 1) {
      rowsPerStrip = 1;
    }

    if (rowsPerStrip > height) {
      rowsPerStrip = height;
    }

    stripCount = (height + rowsPerStrip - 1) / rowsPerStrip;
    /*#if defined _DEBUG || !defined _WINDOWS_
            int uncompressedRowSize = rowsPerStrip * fileStep;
    #endif*/
    directoryOffset = 0;

    stripOffsets = (int*) pmalloc(sizeof(int) * stripCount);
    stripCounts = (short*) pmalloc(sizeof(short) * stripCount);
    buffer = (unsigned char*) pmalloc(sizeof(short) * (fileStep + 32));

    PutBytes(fmtSignTiffII, 4);
    dw = directoryOffset;
    WRITE(dw);

    // write an image data first (the most reasonable way
    // for compressed images)
    for (i = 0; i < stripCount; i++) {
      int limit = y + rowsPerStrip;

      if (limit > height) {
        limit = height;
      }

      stripOffsets[ i ] = GetPos();

      for (; y < limit; y++, data += step) {
        if (channels == 3) {
          IMTRANS1D(width, data, 3, buffer, 3, BGR2RGB);
        }
        else if (channels == 4) {
          IMTRANS1D(width, data, 4, buffer, 3, BGRA2BGR);
        }

        PutBytes(channels > 1 ? buffer : data, fileStep);
      }

      stripCounts[ i ] = (short)(GetPos() - stripOffsets[ i ]);
      /*assert( stripCounts[i] == uncompressedRowSize ||
              stripCounts[i] < uncompressedRowSize &&
              i == stripCount - 1);*/
    }

    if (stripCount > 2) {
      stripOffsetsOffset = GetPos();

      for (i = 0; i < stripCount; i++) {
        dw = stripOffsets[ i ];
        WRITE(dw);
      }

      stripCountsOffset = GetPos();

      for (i = 0; i < stripCount; i++) {
        wd = stripCounts[ i ];
        WRITE(dw);
      }
    }
    else if (stripCount == 2) {
      stripOffsetsOffset = GetPos();

      for (i = 0; i < stripCount; i++) {
        dw = stripOffsets[ i ];
        WRITE(dw);
      }

      stripCountsOffset = stripCounts [ 0 ] + (stripCounts [ 1 ] << 16);
    }
    else {
      stripOffsetsOffset = stripOffsets[ 0 ];
      stripCountsOffset = stripCounts[ 0 ];
    }

    if (channels > 1) {
      bitsPerSample = GetPos();
      wd = 8;
      WRITE(dw);
      WRITE(dw);
      WRITE(dw);

      if (channels == 4) {
        WRITE(dw);
      }
    }

    directoryOffset = GetPos();

    // write header
    wd = 9;
    WRITE(dw);

    /* warning: specification 5.0 of Tiff want to have tags in
       ascending order. This is a non-fatal error, but this cause
       warning with some tools. So, keep this in ascending order */

    WriteTag(TIFF_TAG_WIDTH, TIFF_TYPE_LONG, 1, width);
    WriteTag(TIFF_TAG_HEIGHT, TIFF_TYPE_LONG, 1, height);
    WriteTag(TIFF_TAG_BITS_PER_SAMPLE,
        TIFF_TYPE_SHORT, channels, bitsPerSample);
    WriteTag(TIFF_TAG_COMPRESSION, TIFF_TYPE_LONG, 1, TIFF_UNCOMP);
    WriteTag(TIFF_TAG_PHOTOMETRIC, TIFF_TYPE_SHORT, 1, channels > 1 ? 2 : 1);

    WriteTag(TIFF_TAG_STRIP_OFFSETS, TIFF_TYPE_LONG,
        stripCount, stripOffsetsOffset);

    WriteTag(TIFF_TAG_SAMPLES_PER_PIXEL, TIFF_TYPE_SHORT, 1, channels);
    WriteTag(TIFF_TAG_ROWS_PER_STRIP, TIFF_TYPE_LONG, 1, rowsPerStrip);

    WriteTag(TIFF_TAG_STRIP_COUNTS,
        stripCount > 1 ? TIFF_TYPE_SHORT : TIFF_TYPE_LONG,
        stripCount, stripCountsOffset);

    dw = 0;
    WRITE(dw);

    // write directory offset
    {
      //ihand_t f = _tfopen( picname, _T("r+b") );
      buffer[ 0 ] = (unsigned char) directoryOffset;
      buffer[ 1 ] = (unsigned char)(directoryOffset >> 8);
      buffer[ 2 ] = (unsigned char)(directoryOffset >> 16);
      buffer[ 3 ] = (unsigned char)(directoryOffset >> 24);

      //fseek( f, 4, SEEK_SET );
      //fwrite( buffer, 1, 4, f );
    }

    pfree(stripOffsets);
    pfree(stripCounts);
    pfree(buffer);

    result = TRUE;
  }

  return result;
}

#endif // SUPPORT_TIF
