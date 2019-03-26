
#ifndef _FMTGIF_INL_
#define _FMTGIF_INL_

typedef short int code_int;

/* Various error codes used by decoder */
#define OUT_OF_MEMORY -10
#define BAD_CODE_SIZE -20
#define READ_ERROR -1
#define WRITE_ERROR -2
#define OPEN_ERROR -3
#define CREATE_ERROR -4
#define MAX_CODES 4095
#define GIFBUFTAM 16383
#define TRANSPARENCY_CODE 0xF9

//LZW GIF Image compression
#define MAXBITSCODES 12
#define HSIZE 5003 /* 80% occupancy */
#define MAXCODE(n_bits) (((code_int) 1 << (n_bits)) - 1)
#define HashTabOf(i) fmt->htab[i]
#define CodeTabOf(i) fmt->codetab[i]

#pragma pack(push, 1)

typedef struct tag_gifgce {
  uint8 flags; /*res:3|dispmeth:3|userinputflag:1|transpcolflag:1*/
  uint16 delaytime;
  uint8 transpcolindex;
} struct_gifgce;

typedef struct tag_dscgif { /* Logic Screen Descriptor */
  char header[6]; /* Firma and version */
  uint16 scrwidth;
  uint16 scrheight;
  char pflds;
  char bcindx; // 背景颜色(在全局颜色列表中的索引，如果没有全局颜色列表，该值没有意义)
  char pxasrat; // 像素宽高比(Pixel Aspect Radio)
} struct_dscgif;

typedef struct tag_image { /* Image Descriptor */
  uint16 l;
  uint16 t;
  uint16 w;
  uint16 h;
  uint8 pf;
} struct_image;

typedef struct rgb_color { /* Image Descriptor */
	uint8 r;
	uint8 g;
	uint8 b;
} rgb_color;

typedef struct tag_TabCol { /* Tabla de colores */
  short colres; /* color resolution */
  short sogct; /* size of global color table */
  rgb_color paleta[256]; /* paleta */
} struct_TabCol;

#pragma pack(pop)//恢复对齐状态


typedef struct gif_load_t {
  stream_t* m_strm;
  uint16 biBitCount;
  uint8 dispmeth;
  int m_loops;
  char m_comment[256];
  struct_gifgce gifgce;

  char szLastError[256]; ///< debugging
  int32 nBkgndIndex; ///< used for GIF, PNG, MNG
  int32 nFrame; ///< used for TIF, GIF, MNG : actual frame
  uint32 dwFrameDelay; ///< used for GIF, MNG
  //int32 xDPI1; ///< horizontal resolution
  //int32 yDPI1; ///< vertical resolution
  //RECT rSelectionBox1; ///< bounding rectangle
  //uint8 nAlphaMax1; ///< max opacity (fade)
  BOOL bAlphaPaletteEnabled1; ///< true if alpha values in the palette are enabled.
  BOOL bEnabled1; ///< enables the painting functions

  int interlaced, iypos, istep, iheight, ipass;
#if 1
  int ibf;
  int ibfmax;
  uint8 buf[GIFBUFTAM + 1];
#endif

  short curr_size;  /* The current code size */
  short clear;  /* Value for a clear code */
  short ending;  /* Value for a ending code */
  short newcodes;  /* First available code */
  short top_slot;  /* Highest code for current size */
  short slot;  /* Last read code */

  // The following static variables are used for seperating out codes
  short navail_bytes; /* # bytes left in block */
  short nbits_left; /* # bits left in current uint8 */
  uint8 b1;  /* Current uint8 */
  uint8 byte_buff[257]; /* Current block */
  uint8* pbytes;  /* Pointer to next uint8 in block */

  uint8 stack[MAX_CODES + 1]; /* Stack for storing pixels */
  uint8 suffix[MAX_CODES + 1]; /* Suffix table */
  uint16 prefix[MAX_CODES + 1]; /* Prefix linked list */

  struct_dscgif dscgif;
  struct_TabCol TabCol;
  int32 bTrueColor;
  int32 first_transparent_index;
} gif_load_t;

#define Read(b, c, d) (stream_read(fmt->m_strm, b, (c)*(d))/(c))
#define Write(b, c, d) (stream_write(fmt->m_strm, b, (c)*(d))/(c))

#define Seek(b, c) stream_seek(fmt->m_strm, (b), c)
#define PutC(b) stream_put8(fmt->m_strm, b)
#define Putword(w) {PutC((uint8)(w & 0xff));PutC((uint8)((w >> 8) & 0xff));}
#define Flush()
#define Tell() stream_tell(fmt->m_strm)


#ifndef bLittleEndianHost
static short g__test = 1;
#define bLittleEndianHost (*((char *) &g__test) == 1)
#endif // bLittleEndianHost

static short bit_ntohs(const short x)
{
  if (bLittleEndianHost) {
    return x;
  }

  return ((x & 0xff) << 8) | ((x >> 8) & 0xff);
}

static int32 bit_ntohl(const int32 x)
{
  if (bLittleEndianHost) {
    return x;
  }

  return ((x & 0xff) << 24) | ((x & 0xff00) << 8) | ((x >> 8) & 0xff00) | ((x >> 24) & 0xff);
}

static const uint32 code_mask[] = { //
  0x0000, 0x0001, 0x0003, 0x0007, 0x000F,
  0x001F, 0x003F, 0x007F, 0x00FF,
  0x01FF, 0x03FF, 0x07FF, 0x0FFF,
  0x1FFF, 0x3FFF, 0x7FFF, 0xFFFF
};

typedef struct image_iterator {
  int Ity, Itx;
  int h, w, s;
  uint8* IterImage;
} image_iterator;

#include "gif_load.inl"
#include "gif_save.inl"

#undef Flush
#undef Tell
#undef Putword
#undef PutC
#undef Seek
#undef Read
#undef Write

#undef OUT_OF_MEMORY
#undef BAD_CODE_SIZE
#undef READ_ERROR
#undef WRITE_ERROR
#undef OPEN_ERROR
#undef CREATE_ERROR
#undef MAX_CODES
#undef GIFBUFTAM
#undef TRANSPARENCY_CODE

#undef MAXBITSCODES
#undef HSIZE
#undef MAXCODE
#undef HashTabOf
#undef CodeTabOf

static int gif_load(stream_t* f, int req_comp, img_t* im)
{
  gif_load_t* fmt;
  int ret;

  if ((fmt = (gif_load_t*)pmalloc(sizeof(gif_load_t)))!=NULL) {
    memset(fmt, 0, sizeof(gif_load_t));
    stream_seek(f, 0, SEEK_SET);
    ret = gif_load_impl(fmt, f, req_comp, im);
    pfree(fmt);
  }

  return ret;
}
#define gif_load_filename(fname, req_comp, im) gif_load_file_close(fopen(fname, "rb"), req_comp, im)
static int gif_load_file_close(FILE* pf, int req_comp, img_t* im)
{
  int ret = 0;

  if (pf) {
    stream_t s[1] = {0};
    fstream_init(s, pf);
    ret = gif_load(s, req_comp, im);
    fclose(pf);
  }

  return ret;
}
#undef BAD_CODE_SIZE
#undef bLittleEndianHost
#undef CodeTabOf
#undef CREATE_ERROR
#undef Flush
#undef GIFBUFTAM
#undef HashTabOf
#undef HSIZE
#undef MAX_CODES
#undef MAXBITSCODES
#undef MAXCODE
#undef OPEN_ERROR
#undef OUT_OF_MEMORY
#undef PutC
#undef Putword
#undef Read
#undef READ_ERROR
#undef Seek
#undef Tell
#undef TRANSPARENCY_CODE
#undef Write
#undef WRITE_ERROR

#endif // _FMTGIF_INL_
