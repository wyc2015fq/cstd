
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fmtgif.h"
#include "gif_lib.h"

typedef FILE* fi_handle;

#define IO_WRITE fwrite
#define IO_SEEK fseek
#define IO_READ fread
#define IO_TELL ftell

typedef unsigned char BYTE;
typedef unsigned short WORD;

#define GIF_PLUGIN_SAVE_SUPPORT

typedef unsigned int size_t;

typedef int BOOL;

#ifndef FALSE
#define FALSE 1
#define TRUE 0
#endif

// data structure created at GifPlugin_Open and destroyed at GifPlugin_Close
typedef struct TGifPluginData {
  FILE* m_Handle;
  GifFileType* m_pGifFileType;
  int m_nTransparentColorIndex;
#ifdef GIF_PLUGIN_SAVE_SUPPORT

  BOOL m_bRead;
#endif
}
TGifPluginData;

TGifPluginData* GifPlugin_Open(const TCHAR* giffilename, BOOL read);
void GifPlugin_Close(TGifPluginData* data);
int GifPlugin_Load(TGifPluginData* pData, int page, int flags, unsigned char* pBitmap, int step);
BOOL GifPlugin_Save(const TCHAR* filename, const unsigned char* pDib, int height, int width, int step, int channels);

BOOL GifPlugin_Save2(TGifPluginData* pData, int page, int flags,
    const unsigned char* pDib, int height, int width, int step, int channels);

#ifdef _WINDOWS_
#pragma pack(push, 1)
#else
#pragma pack(1)
#endif

typedef struct TGIFGraphicControlExtensionBlock {
  BYTE nFlags;
  WORD nDelay;
  BYTE nTransparentColorIndex;
}
TGIFGraphicControlExtensionBlock;

#ifdef _WINDOWS_
#pragma pack(pop)
#else
#pragma pack(4)
#endif

// interlace pattern constants for loading/saving interlaced gif files
static const int InterlacedOffset[] = {
  0, 4, 2, 1
}
; // The way Interlaced image should
static const int InterlacedJumps[] = {
  8, 8, 4, 2
}
;  // be read - offsets and jumps...

// adapter function for use by the gif-lib input logic
int InputFuncAdapt(GifFileType* pGifFileType, GifByteType* pBuf, int nBytesToRead)
{
  return IO_READ(pBuf, 1, nBytesToRead, ((TGifPluginData*) pGifFileType->UserData) ->m_Handle);
}

// adapter function for use by the gif-lib output logic
#ifdef GIF_PLUGIN_SAVE_SUPPORT
int OutputFuncAdapt(GifFileType* pGifFileType, const GifByteType* pBuf, int nBytesToWrite)
{
  return IO_WRITE(
      (unsigned char*) pBuf, 1, nBytesToWrite, ((TGifPluginData*) pGifFileType->UserData) ->m_Handle);
}
#endif

// this plugin open function
TGifPluginData* GifPlugin_Open(const TCHAR* giffilename, int read)
{
  FILE* handle = _tfopen(giffilename, read ? _T("rb") : _T("wb"));
  TGifPluginData* pData = pmalloc(sizeof(TGifPluginData));

  if (!pData || !handle) {
    return NULL;
  }

  pData->m_Handle = handle;
  pData->m_nTransparentColorIndex = -1; // set to no transparent color
#ifdef GIF_PLUGIN_SAVE_SUPPORT

  pData->m_bRead = read;
#endif

  if (read) {
    pData->m_pGifFileType = DGi_tfopen(pData, InputFuncAdapt);
  }
  else {
#ifdef GIF_PLUGIN_SAVE_SUPPORT
    pData->m_pGifFileType = EGi_tfopen(pData, OutputFuncAdapt);
#else

    pData->m_pGifFileType = NULL;
#endif

  }

  if (!pData->m_pGifFileType) {
    pfree(pData);
    return NULL;
  }

  return pData;
}

// this plugin close function
void GifPlugin_Close(TGifPluginData* pData)
{
  if (!pData) {
    return ;
  }

#ifdef GIF_PLUGIN_SAVE_SUPPORT

  if (pData->m_bRead) {
    DGifCloseFile(pData->m_pGifFileType);
  }
  else {
    EGifCloseFile(pData->m_pGifFileType);
  }

#else

  DGifCloseFile(pData->m_pGifFileType);
#endif

  fclose(pData->m_Handle);
  pfree(pData);
}

#define THROW(msg)  do { printf(msg); return 0; } while(0)
#define THROW_RETURN(msg, code)  do { printf(msg); return (code); } while(0)
#define FreeImage_GetScanLine(pBitmap, n, step)  (pBitmap + (n) * (step))

// this plugin load function
int GifPlugin_Load(TGifPluginData* pData, int page, int flags, unsigned char* pBitmap, int step)
{
  // FIBITMAP *pBitmap= NULL;
  // the freeimage bitmap. NOTE: This is needed here because of the exception logic
  // (if we THROW(an exeception we will pfree the bitmap in the catch block).
  GifFileType* pGifFile = pData->m_pGifFileType;
  GifRecordType RecordType;
  //int page_i = 0;
  //IO_SEEK(pData->m_Handle, 0, SEEK_SET);

  if (!pData || page < 0) {
    return FALSE;
  }

  {
    do {
      if (DGifGetRecordType(pGifFile, &RecordType) == GIF_ERROR) {
        THROW("io error or invalid gif format");
      }

      switch (RecordType) {

      case IMAGE_DESC_RECORD_TYPE: {
        unsigned char* pBitmapBuf = 0;
        GifColorType* pGifPalette;
        ColorMapObject* pImageColorMap;
        int nImagePosX, nImagePosY, nImageWidth, nImageHeight;

        /*if (8 != pGifFile->SColorResolution)
         THROW("only 8 bit color resolution gif are supported";*/
        if (DGifGetImageDesc(pGifFile) == GIF_ERROR) {
          THROW("io error or invalid gif format");
        }

        // Read the image line by line
        // NOTE: The global size of the image is given by,
        // GifFile->SWidth and GifFile->SHeight
        //
        // The following sizes/positions are for the current frame only!

        nImagePosX = pGifFile->Image.Left;
        nImagePosY = pGifFile->Image.Top;
        nImageWidth = pGifFile->Image.Width;
        nImageHeight = pGifFile->Image.Height;

        if (nImagePosX < 0 ||
            nImagePosX > pGifFile->SWidth ||
            nImagePosX + nImageWidth > pGifFile->SWidth ||
            nImagePosY < 0 ||
            nImagePosY > pGifFile->SHeight ||
            nImagePosY + nImageHeight > pGifFile->SHeight) {
          THROW_RETURN("invalid gif dimensions", TRUE);
        }

        // 1. Allocate a freeimage bitmap
        // pBitmap = FreeImage_Allocate(pGifFile->SWidth, pGifFile->SHeight, 8);

        if (!pBitmap) {
          THROW_RETURN("DIB allocation failed", TRUE);
        }

        // Set's the transparent color of the gif
        if (pData->m_nTransparentColorIndex >= 0) {
          BYTE TransparencyTable[ 256 ];

          memset(TransparencyTable, 0xff, pData->m_nTransparentColorIndex);
          TransparencyTable[ pData->m_nTransparentColorIndex ] = 0;

          //FreeImage_SetTransparencyTable(pBitmap,
          // TransparencyTable, pData->m_nTransparentColorIndex+1);
        }

        pBitmapBuf = (unsigned char*) pmalloc(pGifFile->SWidth * pGifFile->SHeight);
        // clear dib with GIF background color
        {
          //TODO: memset the all the bib at once and not line by line.
          int n;

          for (n = 0; n < pGifFile->SHeight; ++n) {
            BYTE* pScanLine = FreeImage_GetScanLine(pBitmapBuf, n, pGifFile->SWidth);
            memset(pScanLine, pGifFile->SBackGroundColor, pGifFile->SWidth);
          }
        }

        // 2. copy the palette from gif to freeimage palette
        // get the current image color map (the image map can be local to a frame or global to all the file)
        pImageColorMap = pGifFile->Image.ColorMap ? pGifFile->Image.ColorMap : pGifFile->SColorMap;

        if (pImageColorMap->ColorCount > 256) {
          THROW("invalid gif color count");
        }

        pGifPalette = pImageColorMap->Colors;
        //pPalette = FreeImage_GetPalette(pBitmap);

        // 3. copy gif scanlines to freeimage bitmap
        if (pGifFile->Image.Interlace) {
          int nPass, n;

          // Need to perform 4 passes on the image
          for (nPass = 0; nPass < 4; ++nPass) {
            for (n = InterlacedOffset[ nPass ]; n < nImageHeight; n += InterlacedJumps[ nPass ]) {
              //NOTE: The gif is an top-down image BUT freeimage dib are bottom-up
              BYTE* pScanLine = FreeImage_GetScanLine(pBitmapBuf, nImageHeight - n - 1 + nImagePosY, pGifFile->SWidth);

              if (DGifGetLine(pGifFile, pScanLine, nImageWidth) == GIF_ERROR) {
                THROW("io error or invalid gif format");
              }
            }
          }
        }
        else {
          int n;

          for (n = 0; n < nImageHeight; ++n) {
            //NOTE: The gif is an top-down image BUT freeimage dib are bottom-up
            BYTE* pScanLine = FreeImage_GetScanLine(pBitmapBuf, n + nImagePosY, pGifFile->SWidth);

            if (DGifGetLine(pGifFile, pScanLine + nImagePosX, nImageWidth) == GIF_ERROR) {
              THROW("io error or invalid gif format");
            }
          }
        }

        {
          int n, m;

          for (n = pGifFile->SHeight; n--;) {
            //NOTE: The gif is an top-down image BUT freeimage dib are bottom-up
            BYTE* pScanLine = FreeImage_GetScanLine(pBitmapBuf, n, pGifFile->SWidth);
            BYTE* pScanLine2 = FreeImage_GetScanLine(pBitmap, n, step);

            for (m = pGifFile->SWidth; m--;) {
              int idx = *pScanLine++;
              pScanLine2[ 0 ] = pGifPalette[ idx ].Blue;
              pScanLine2[ 1 ] = pGifPalette[ idx ].Green;
              pScanLine2[ 2 ] = pGifPalette[ idx ].Red;
              pScanLine2 += 3;
            }
          }
        }

        // pfree this image since we don't need it in the gif structure...
        // NOTE: If we don't pfree it and the gif as more than one image
        // that images all get appended to the SavedImages array.
        FreeSavedImages(pGifFile);
        pGifFile->SavedImages = NULL;
        pGifFile->ImageCount = 0;
        pfree(pBitmapBuf);
        return TRUE;
      }
      break;

      case EXTENSION_RECORD_TYPE: {
        int ExtCode;
        GifByteType* Extension;

        //
        // Skip any extension blocks in file
        //
        if (DGifGetExtension(pGifFile, &ExtCode, &Extension) == GIF_ERROR) {
          THROW("io error or invalid gif format");
        }

        if (ExtCode == GRAPHICS_EXT_FUNC_CODE  // is this a Graphic Control Extension block?
            && Extension[ 0 ] >= 4) {       // this block must have at least 4 bytes!
          TGIFGraphicControlExtensionBlock* pExt;
          pExt = (TGIFGraphicControlExtensionBlock*) & Extension[ 1 ];

          if (pExt->nFlags & 0x01) {      // this image has an transparent color?
            pData->m_nTransparentColorIndex = pExt->nTransparentColorIndex;
          }

          //in pExt->nDelay is the delay time in 1/100 of a second that the next frame is expected to wait
          //unsigned nDelay= (Extension[2] | (Extension[3]<<8)) * 10; // nDelay converted to ms
        }

        while (Extension) {
          if (DGifGetExtensionNext(pGifFile, &Extension) == GIF_ERROR) {
            THROW("io error or invalid gif format");
          }
        }
      }
      break;
      } // switch
    }
    while (RecordType != TERMINATE_RECORD_TYPE);
  }

  return FALSE;
}

// this plugin save function
#ifdef GIF_PLUGIN_SAVE_SUPPORT
int GifPlugin_Save1(TGifPluginData* pData, int page, int flags,
    const unsigned char* pDib, int height, int width, int step, int channels)
{

  GifFileType* pGifFile = pData->m_pGifFileType;
  ColorMapObject* pGifColorMap = NULL;

  if (!pData) {
    return FALSE;
  }

  {
    GifColorType* pGifPalette;
    int nImageWidth = pGifFile->SWidth;
    int nImageHeight = pGifFile->SHeight;
    int n;
    // if we have an transparency table make sure that the transparent
    // color is with 0 (full transparent alpha) value.
    // NOTE: This plugin sets the gif transparent color index to the
    // last transparency index of the freeimage dib.

    int nTransparentColorIndex = -1;
    // nTransparentColorIndex = FreeImage_GetTransparencyCount(pDib)-1;
    // if (nTransparentColorIndex >= 0 && FreeImage_GetTransparencyTable(pDib)[nTransparentColorIndex])
    // THROW("invalid transparent color in dib)";

    // copy color table from dib into gif color map
    if (!(pGifColorMap = MakeMapObject(256, NULL))) {
      THROW("No memory");
    }

    pGifPalette = pGifColorMap->Colors;

    // write gif screen description
    if (EGifPutScreenDesc(pGifFile, width, height, 8, 0, NULL) == GIF_ERROR) {
      THROW("io error");
    }

    // write transparent color information if the dib has one
    if (nTransparentColorIndex >= 0) {
      TGIFGraphicControlExtensionBlock Extension = { 0x01, 0, nTransparentColorIndex };

      if (EGifPutExtension(pGifFile, GRAPHICS_EXT_FUNC_CODE, 4, &Extension) == GIF_ERROR) {
        THROW("io error");
      }
    }

    // write image data
    //TODO: retreive the interlace option value from flags argument
    if (EGifPutImageDesc(pGifFile, 0, 0,
        nImageWidth, nImageHeight, FALSE, pGifColorMap) == GIF_ERROR) {
      THROW("io error");
    }

    FreeMapObject(pGifColorMap);
    pGifColorMap = NULL; //NOTE: need to place NULL here because the exception handling code frees it!

    for (n = nImageHeight; n--;) {
      //NOTE: The gif is an top-down image BUT freeimage dib are bottom-up
      BYTE* pScanLine = (BYTE*) FreeImage_GetScanLine(pDib, n, step);

      if (EGifPutLine(pGifFile, pScanLine, nImageWidth) == GIF_ERROR) {
        THROW("io error");
      }
    }

    return TRUE;
  }

  if (pGifColorMap) {
    FreeMapObject(pGifColorMap);
  }

  return FALSE;
}

// Close output file (if open), and exit.
static void QuitGifError(GifFileType* GifFile)
{
  PrintGifError();
  //if (GifFile != NULL) EGifCloseFile(GifFile);
  //exit(EXIT_FAILURE);
}

#include "fmtimg.h"

void SaveGif(GifFileType* GifFile, GifByteType* OutputBuffer,
    ColorMapObject* OutputColorMap,
    int ExpColorMapSize, int Width, int Height)
{
  int i;
  GifByteType* Ptr = OutputBuffer;

  if (EGifPutScreenDesc(GifFile, Width, Height, ExpColorMapSize, 0, OutputColorMap) == GIF_ERROR) {
    //QuitGifError(GifFile);
  }

  if (EGifPutImageDesc(GifFile, 0, 0, Width, Height, FALSE, OutputColorMap) == GIF_ERROR) {
    QuitGifError(GifFile);
  }

  for (i = 0; i < Height; i++) {
    if (EGifPutLine(GifFile, Ptr, Width) == GIF_ERROR) {
      QuitGifError(GifFile);
    }

    GifQprintf("\b\b\b\b%-4d", Height - i - 1);

    Ptr += Width;
  }
}

int GifPlugin_Save2(TGifPluginData* pData, int page, int flags,
    const unsigned char* pDib, int height, int width, int step, int channels)
{
  int ColorFlag = FALSE, ExpNumOfColors = 8, OneFileFlag = FALSE,
      HelpFlag = FALSE, ColorMapSize = 256;

  int Width = width, Height = height, sz = height * width, i, j;
  char** FileName = NULL;
  GifByteType* RedBuffer = NULL, *GreenBuffer = NULL,
               *BlueBuffer = NULL, *OutputBuffer = NULL;
  ColorMapObject* OutputColorMap = NULL;
  RedBuffer = (GifByteType*) pmalloc(sizeof(GifByteType) * sz);
  GreenBuffer = (GifByteType*) pmalloc(sizeof(GifByteType) * sz);
  BlueBuffer = (GifByteType*) pmalloc(sizeof(GifByteType) * sz);

  for (i = 0; i < height; ++i) {
    const unsigned char* scanline = pDib + i * step;
    int pos = i * width;

    if (3 == channels) {
      for (j = 0; j < width; ++j) {
        BlueBuffer[ pos + j ] = scanline[ 0 ];
        GreenBuffer[ pos + j ] = scanline[ 1 ];
        RedBuffer[ pos + j ] = scanline[ 2 ];
        scanline += 3;
      }
    }
    else if (1 == channels) {
      for (j = 0; j < width; ++j) {
        BlueBuffer[ pos + j ] = GreenBuffer[ pos + j ] = RedBuffer[ pos + j ] = scanline[ j ];
      }
    }
  }

  OutputColorMap = MakeMapObject(ColorMapSize, NULL);
  OutputBuffer = (GifByteType*) pmalloc(Width * Height * sizeof(GifByteType));

  if (OutputColorMap && OutputBuffer) {
    if (QuantizeBuffer(Width, Height, &ColorMapSize,
        RedBuffer, GreenBuffer, BlueBuffer,
        OutputBuffer, OutputColorMap->Colors) == GIF_ERROR) {
      QuitGifError(NULL);
    }

    pfree((char*) RedBuffer);
    pfree((char*) GreenBuffer);
    pfree((char*) BlueBuffer);
    SaveGif(pData->m_pGifFileType, OutputBuffer, OutputColorMap, ExpNumOfColors, Width, Height);
  }
  else {
    //printf("Failed to allocate memory required, aborted.");
  }

  if (OutputColorMap) {
    FreeMapObject(OutputColorMap);
  }

  if (OutputBuffer) {
    pfree(OutputBuffer);
  }

  return TRUE;
}

int GifPlugin_Save(const TCHAR* filename, const unsigned char* pDib, int height, int width, int step, int channels)
{
  TGifPluginData* pData = GifPlugin_Open(filename, 0);
  int ret = 0;

  if (pData) {
    ret = GifPlugin_Save2(pData, 0, 0, pDib, height, width, step, channels);
    GifPlugin_Close(pData);
  }

  return ret;
}

#endif //#ifdef GIF_PLUGIN_SAVE_SUPPORT

