
#ifndef _FMTGIF_INL_
#define _FMTGIF_INL_

#include "_fmtimg.h"

//#include "ximage.h"

typedef short int code_int;

/* Various error codes used by decoder */
#define OUT_OF_MEMORY -10
#define BAD_CODE_SIZE -20
#define READ_ERROR -1
#define WRITE_ERROR -2
#define OPEN_ERROR -3
#define CREATE_ERROR -4
#define MAX_CODES   4095
#define GIFBUFTAM 16383
#define TRANSPARENCY_CODE 0xF9

//LZW GIF Image compression
#define MAXBITSCODES    12
#define HSIZE  5003     /* 80% occupancy */
#define MAXCODE(n_bits) (((code_int) 1 << (n_bits)) - 1)
#define HashTabOf(i)    fmt->htab[i]
#define CodeTabOf(i)    fmt->codetab[i]


#pragma pack(push, 1)

typedef struct tag_gifgce {
  uint8 flags; /*res:3|dispmeth:3|userinputflag:1|transpcolflag:1*/
  uint16 delaytime;
  uint8 transpcolindex;
} struct_gifgce;

typedef struct tag_dscgif {   /* Logic Screen Descriptor  */
  char header[6];       /* Firma and version */
  uint16 scrwidth;
  uint16 scrheight;
  char pflds;
  char bcindx; // 背景颜色(在全局颜色列表中的索引，如果没有全局颜色列表，该值没有意义)
  char pxasrat; // 像素宽高比(Pixel Aspect Radio)
} struct_dscgif;

typedef struct tag_image {     /* Image Descriptor */
  uint16 l;
  uint16 t;
  uint16 w;
  uint16 h;
  uint8 pf;
} struct_image;

typedef struct tag_TabCol {   /* Tabla de colores */
  short colres;         /* color resolution */
  short sogct;          /* size of global color table */
  rgb_color paleta[256];    /* paleta */
} struct_TabCol;

#pragma pack(pop)//恢复对齐状态

typedef struct tag_RLE {
  int rl_pixel;
  int rl_basecode;
  int rl_count;
  int rl_table_pixel;
  int rl_table_max;
  int just_cleared;
  int out_bits;
  int out_bits_init;
  int out_count;
  int out_bump;
  int out_bump_init;
  int out_clear;
  int out_clear_init;
  int max_ocodes;
  int code_clear;
  int code_eof;
  unsigned int obuf;
  int obits;
  unsigned char oblock[256];
  int oblen;
} struct_RLE;

typedef struct fmtgifw {
  ihand_t m_strm;
  uint16       biPlanes;
  uint32      biClrUsed;
  uint32      biClrImportant;
  uint16       biBitCount;
  int32  nBkgndIndex;
  uint8  dispmeth;
  uint32  dwFrameDelay;
  const PaletteEntry* pal;
  int m_loops;
  int              curx, cury;
  int32             CountDown;
  uint32    cur_accum;
  int              cur_bits;
  int a_count;
  char accum[256];
  int n_bits;       /* number of bits/code */
  //LZW GIF Image compression routines
  int32 htab[HSIZE];
  unsigned short codetab[HSIZE];
  code_int maxcode;   /* maximum code, given n_bits */
  code_int free_ent;    /* first unused entry */
  int clear_flg;
  int g_init_bits;
  int ClearCode;
  int EOFCode;
  int32  xOffset;
  int32  yOffset;
  uint32  dwCodecOpt;  ///< for GIF, TIF : 0=def.1=unc,2=fax3,3=fax4,4=pack,5=jpg

  char m_comment[256];
} fmtgifw;

typedef struct tagCxImageInfo {
  ihand_t m_strm;
  uint16    biBitCount;
  uint8  dispmeth;
  int m_loops;
  char m_comment[256];
  struct_gifgce gifgce;

  char  szLastError[256]; ///< debugging
  int32  nBkgndIndex;      ///< used for GIF, PNG, MNG
  //RGBQUAD nBkgndColor1;    ///< used for RGB transparency
  //float  fQuality1;        ///< used for JPEG, JPEG2000 (0.0f ... 100.0f)
  //uint8  nJpegScale1;       ///< used for JPEG [ignacio]
  int32  nFrame;           ///< used for TIF, GIF, MNG : actual frame
  uint32  dwFrameDelay;    ///< used for GIF, MNG
  //int32  xDPI1;             ///< horizontal resolution
  //int32  yDPI1;             ///< vertical resolution
  //RECT  rSelectionBox1;    ///< bounding rectangle
  //uint8  nAlphaMax1;        ///< max opacity (fade)
  BOOL  bAlphaPaletteEnabled1; ///< true if alpha values in the palette are enabled.
  BOOL  bEnabled1;         ///< enables the painting functions

  //int32  nNumLayers1;
  //uint32  dwFlags1;      ///< 0x??00000 = reserved, 0x00??0000 = blend mode, 0x0000???? = layer id - user flags
  //BOOL  bGetAllFrames1;


  int interlaced, iypos, istep, iheight, ipass;
  int ibf;
  int ibfmax;
  uint8 buf[GIFBUFTAM + 1];

  short curr_size;                     /* The current code size */
  short clear;                         /* Value for a clear code */
  short ending;                        /* Value for a ending code */
  short newcodes;                      /* First available code */
  short top_slot;                      /* Highest code for current size */
  short slot;                          /* Last read code */

  /* The following static variables are used
  * for seperating out codes */
  short navail_bytes;              /* # bytes left in block */
  short nbits_left;                /* # bits left in current uint8 */
  uint8 b1;                           /* Current uint8 */
  uint8 byte_buff[257];               /* Current block */
  uint8* pbytes;                      /* Pointer to next uint8 in block */

  uint8 stack[MAX_CODES + 1];            /* Stack for storing pixels */
  uint8 suffix[MAX_CODES + 1];           /* Suffix table */
  uint16 prefix[MAX_CODES + 1];           /* Prefix linked list */

  struct_dscgif dscgif;
  struct_TabCol TabCol;
  int32 bTrueColor;
  int32 first_transparent_index;
} fmtgif;

//#include "ximaiter.h"

#define Read(b, c, d)   xfread(b, c, d, fmt->m_strm)
#define Write(b, c, d)  xfwrite(b, c, d, fmt->m_strm)

#define Seek(b, c)  xfseek(fmt->m_strm, (b), c)
#define PutC(b)  {uint8 by=b; xfwrite((&by), 1, 1, fmt->m_strm);}
#define Putword(w)  {PutC((uint8)(w & 0xff));PutC((uint8)((w >> 8) & 0xff));}
#define Flush()  xfflush(fmt->m_strm)
#define Tell()   xftell(fmt->m_strm)


#ifndef bLittleEndianHost
short g__test = 1;
#define bLittleEndianHost  (*((char *) &g__test) == 1)
#endif // bLittleEndianHost

short bit_ntohs(const short x)
{
  if (bLittleEndianHost) {
    return x;
  }

  return ((x & 0xff) << 8) | ((x >> 8) & 0xff);
}

int32 bit_ntohl(const int32 x)
{
  if (bLittleEndianHost) {
    return x;
  }

  return ((x & 0xff) << 24) | ((x & 0xff00) << 8) |
      ((x >> 8) & 0xff00) | ((x >> 24) & 0xff);
}

static const uint32 code_mask[] =  //
{
  0x0000, 0x0001, 0x0003, 0x0007, 0x000F,
    0x001F, 0x003F, 0x007F, 0x00FF,
    0x01FF, 0x03FF, 0x07FF, 0x0FFF,
    0x1FFF, 0x3FFF, 0x7FFF, 0xFFFF
};

#include "fmtgif_decode.inl"
#include "fmtgif_encode.inl"

#undef Flush
#undef Tell
#undef Putword
#undef PutC
#undef Seek
#undef Read
#undef Write


#endif // _FMTGIF_INL_
