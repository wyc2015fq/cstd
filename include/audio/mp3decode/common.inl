// Global Include Files
#ifdef MACINTOSH
#include <SANE.h>
#include <pascal.h>
#endif
#include <ctype.h>
#if defined(MSC60)
#ifndef MS_DOS
#define MS_DOS
#endif
#ifndef PROTO_ARGS
#define PROTO_ARGS
#endif
#endif
#ifdef UNIX
#define TABLES_PATH "tables" /* to find data files */
/* name of environment variable holding path of table files */
#define MPEGTABENV "MPEGTABLES"
#define PATH_SEPARATOR "/" /* how to build paths */
#endif /* UNIX */
#ifdef MACINTOSH
/* #define TABLES_PATH ":tables:" /* where to find data files */
#endif /* MACINTOSH */
/*
 * Don't define FAR to far unless you're willing to clean up the
 * prototypes
 */
#define FAR /*far*/
#ifdef __STDC__
#ifndef PROTO_ARGS
#define PROTO_ARGS
#endif
#endif
#ifdef CONVEX
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#endif
/* MS_DOS and VMS do not define TABLES_PATH, so OpenTableFile will default
 to finding the data files in the default directory */
// Global Include Files
/* #define UNIX /* Unix conditional compile switch */
/* #define MACINTOSH /* Macintosh conditional compile switch */
/* #define MS_DOS /* IBM PC conditional compile switch */
/* #define MSC60 /* Compiled for MS_DOS with MSC v6.0 */
/* #define AIX /* AIX conditional compile switch */
/* #define CONVEX /* CONVEX conditional compile switch */
#include <stdio.h>
#include <string.h>
#include <math.h>
#ifdef UNIX
#include <unistd.h>
#endif /* UNIX */
#ifdef MACINTOSH
#include <stdlib.h>
#include <console.h>
#endif /* MACINTOSH */
#ifdef MS_DOS
#include <stdlib.h>
#ifdef MSC60
#include <memory.h>
#else
#include <alloc.h>
#include <mem.h>
#endif /* MSC60 */
#endif /* MS_DOS */
/***********************************************************************
*
* Global Definitions
*
***********************************************************************/
/* General Definitions */
#ifdef MS_DOS
#define FLOAT double
#else
#define FLOAT float
#endif
#define FALSE 0
#define TRUE 1
#define NULL_CHAR '\0'
#define MAX_U_32_NUM 0xFFFFFFFF
#define PI 3.14159265358979
#define PI4 PI/4
#define PI64 PI/64
#define LN_TO_LOG10 0.2302585093
#define VOL_REF_NUM 0
#define MPEG_AUDIO_ID 1
#define MAC_WINDOW_SIZE 24
#define MONO 1
#define STEREO 2
#define BITS_IN_A_BYTE 8
#define WORD 16
#define MAX_NAME_SIZE 81
#define SBLIMIT 32
#define SSLIMIT 18
#define FFT_SIZE 1024
#define HAN_SIZE 512
#define SCALE_BLOCK 12
#define SCALE_RANGE 64
#define SCALE 32768
#define CRC16_POLYNOMIAL 0x8005
/* MPEG Header Definitions - Mode Values */
#define MPG_MD_STEREO 0
#define MPG_MD_JOINT_STEREO 1
#define MPG_MD_DUAL_CHANNEL 2
#define MPG_MD_MONO 3
/* Mode Extention */
#define MPG_MD_LR_LR 0
#define MPG_MD_LR_I 1
#define MPG_MD_MS_LR 2
#define MPG_MD_MS_I 3
/* AIFF Definitions */
// Note: The value of a multi-character constant is implementation-defined.
#if !defined(MS_DOS) && !defined(AIX) && !defined(__hpux) && !defined(sgi)
#define IFF_LONG
#define IFF_ID_FORM 'FORM'
#define IFF_ID_AIFF 'AIFF'
#define IFF_ID_COMM 'COMM'
#define IFF_ID_SSND 'SSND'
#define IFF_ID_MPEG 'MPEG'
#else
#define IFF_ID_FORM "FORM"
#define IFF_ID_AIFF "AIFF"
#define IFF_ID_COMM "COMM"
#define IFF_ID_SSND "SSND"
#define IFF_ID_MPEG "MPEG"
#endif
/* "bit_stream.h" Definitions */
#define MINIMUM 4 /* Minimum size of the buffer in bytes */
#define MAX_LENGTH 32 /* Maximum length of word written or
read from bit stream */
#define READ_MODE 0
#define WRITE_MODE 1
#define ALIGNING 8
#define BINARY 0
#define ASCII 1
#define BS_FORMAT ASCII /* BINARY or ASCII = 2x bytes */
#define BUFFER_SIZE 4096
#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define MAX(A, B) ((A) > (B) ? (A) : (B))
// Global Type Definitions
/* Structure for Reading Layer II Allocation Tables from File */
typedef struct {
  unsigned int steps;
  unsigned int bits;
  unsigned int group;
  unsigned int quant;
} sb_alloc, *alloc_ptr;
typedef sb_alloc al_table[SBLIMIT][16];
/* Header Information Structure */
typedef struct {
  int version;
  int lay;
  int error_protection;
  int bitrate_index;
  int sampling_frequency;
  int padding;
  int extension;
  int mode;
  int mode_ext;
  int copyright;
  int original;
  int emphasis;
} layer, *the_layer;
/* Parent Structure Interpreting some Frame Parameters in Header */
typedef struct {
  layer* header; /* raw header information */
  int actual_mode; /* when writing IS, may forget if 0 chs */
  al_table* alloc; /* bit allocation table read in */
  int tab_num; /* number of table as loaded */
  int stereo; /* 1 for mono, 2 for stereo */
  int jsbound; /* first band of joint stereo coding */
  int sblimit; /* total number of sub bands */
} frame_params;
/* Double and SANE Floating Point Type Definitions */
typedef struct IEEE_DBL_struct {
  unsigned long hi;
  unsigned long lo;
} IEEE_DBL;
typedef struct SANE_EXT_struct {
  unsigned long l1;
  unsigned long l2;
  unsigned short s1;
} SANE_EXT;
/* AIFF Type Definitions */
typedef char ID[4];
typedef struct ChunkHeader_struct {
  ID ckID;
  long ckSize;
} ChunkHeader;
typedef struct Chunk_struct {
  ID ckID;
  long ckSize;
  ID formType;
} Chunk;
typedef struct CommonChunk_struct {
  ID ckID;
  long ckSize;
  short numChannels;
  unsigned long numSampleFrames;
  short sampleSize;
  char sampleRate[10];
} CommonChunk;
typedef struct SoundDataChunk_struct {
  ID ckID;
  long ckSize;
  unsigned long offset;
  unsigned long blockSize;
} SoundDataChunk;
typedef struct blockAlign_struct {
  unsigned long offset;
  unsigned long blockSize;
} blockAlign;
typedef struct IFF_AIFF_struct {
  short numChannels;
  unsigned long numSampleFrames;
  short sampleSize;
  double sampleRate;
  unsigned long sampleType;
  blockAlign blkAlgn;
} IFF_AIFF;
/* "bit_stream.h" Type Definitions */
typedef struct bit_stream_struc {
  FILE* pt; /* pointer to bit stream device */
  unsigned char* buf; /* bit stream buffer */
  int buf_size; /* size of buffer (in number of bytes) */
  long totbit; /* bit counter of bit stream */
  int buf_byte_idx; /* pointer to top byte in buffer */
  int buf_bit_idx; /* pointer to top bit of top byte in buffer */
  int mode; /* bit stream open in read or write mode */
  int eob; /* end of buffer index */
  int eobs; /* end of bit stream flag */
  char format; /* format of file in rd mode (BINARY/ASCII) */
} Bit_stream_struc;
/* Layer III side information. */
typedef struct {
  unsigned main_data_begin;
  unsigned private_bits;
  struct {
    unsigned scfsi[4];
    struct gr_info_s {
      unsigned part2_3_length;
      unsigned big_values;
      unsigned global_gain;
      unsigned scalefac_compress;
      unsigned window_switching_flag;
      unsigned block_type;
      unsigned mixed_block_flag;
      unsigned table_select[3];
      unsigned subblock_gain[3];
      unsigned region0_count;
      unsigned region1_count;
      unsigned preflag;
      unsigned scalefac_scale;
      unsigned count1table_select;
    } gr[2];
  } ch[2];
} III_side_info_t;
/* Layer III scale factors. */
typedef struct {
  int l[23]; /* [cb] */
  int s[3][13]; /* [window][cb] */
} III_scalefac_t[2]; /* [ch] */
/* Wave Header Definitions */
typedef struct {
  char szRiffID[4];//"RIFF"
  unsigned int dwRiffSize;
  char szRiffFormat[4];//"WAVE"
} RIFF_HEADER;
typedef struct {
  unsigned short wFormatTag; /* format type WORD*/
  unsigned short wChannels; /* number of channels (i.e. mono, stereo, etc.) WORD*/
  unsigned int dwSamplesPerSec; /* sample rate */
  unsigned int dwAvgBytesPerSec; /* for buffer estimation WORD*/
  unsigned short wBlockAlign; /* block size of data WORD*/
  unsigned short wBitsPerSample;
} WAVE_FORMAT;
typedef struct {
  char szFmtID[4];//"fmt"
  unsigned int dwFmtSize;
  WAVE_FORMAT wavFormat;
} FMT_BLOCK;
typedef struct {
  char szDataID[4];//"data"
  unsigned int dwDataSize;
} DATA_BLOCK;
// Global Variable Definitions
char* mode_names[4] = { "stereo", "j-stereo", "dual-ch", "single-ch" };
char* layer_names[3] = { "I", "II", "III" };
double s_freq[4] = {44.1, 48, 32, 0};
int bitrate[3][15] = {
  {0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448},
  {0, 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384},
  {0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320}
};
double FAR multiple[64] = {
  2.00000000000000, 1.58740105196820, 1.25992104989487,
  1.00000000000000, 0.79370052598410, 0.62996052494744, 0.50000000000000,
  0.39685026299205, 0.31498026247372, 0.25000000000000, 0.19842513149602,
  0.15749013123686, 0.12500000000000, 0.09921256574801, 0.07874506561843,
  0.06250000000000, 0.04960628287401, 0.03937253280921, 0.03125000000000,
  0.02480314143700, 0.01968626640461, 0.01562500000000, 0.01240157071850,
  0.00984313320230, 0.00781250000000, 0.00620078535925, 0.00492156660115,
  0.00390625000000, 0.00310039267963, 0.00246078330058, 0.00195312500000,
  0.00155019633981, 0.00123039165029, 0.00097656250000, 0.00077509816991,
  0.00061519582514, 0.00048828125000, 0.00038754908495, 0.00030759791257,
  0.00024414062500, 0.00019377454248, 0.00015379895629, 0.00012207031250,
  0.00009688727124, 0.00007689947814, 0.00006103515625, 0.00004844363562,
  0.00003844973907, 0.00003051757813, 0.00002422181781, 0.00001922486954,
  0.00001525878906, 0.00001211090890, 0.00000961243477, 0.00000762939453,
  0.00000605545445, 0.00000480621738, 0.00000381469727, 0.00000302772723,
  0.00000240310869, 0.00000190734863, 0.00000151386361, 0.00000120155435,
  1E-20
};
/*******************************************************************************
*
* Allocate number of bytes of memory equal to "block".
*
*******************************************************************************/
void FAR* mem_alloc(unsigned long block, char* item)
{
  void* ptr;
#ifdef MACINTOSH
  ptr = NewPtr(block);
#endif
#ifdef MSC60
  /*ptr = (void FAR *) _fmalloc((unsigned int)block);*/ /* far memory, 92-07-08 sr */
  ptr = (void FAR*) malloc((unsigned int)block); /* far memory, 93-08-24 ss */
#endif
#if ! defined (MACINTOSH) && ! defined (MSC60)
  ptr = (void FAR*) malloc(block);
#endif
  if (ptr != NULL) {
#ifdef MSC60
    _fmemset(ptr, 0, (unsigned int)block); /* far memory, 92-07-08 sr */
#else
    memset(ptr, 0, block);
#endif
  }
  else {
    printf("Unable to allocate %s\n", item);
    exit(0);
  }
  return(ptr);
}
// Free memory pointed to by "*ptr_addr".
void mem_free(void** ptr_addr)
{
  if (*ptr_addr != NULL) {
#ifdef MACINTOSH
    DisposPtr(*ptr_addr);
#else
    free(*ptr_addr);
#endif
    *ptr_addr = NULL;
  }
}
// Check block of memory all equal to a single byte, else return FALSE
// test - but only tested as a char (bottom 8 bits)
int memcheck(char* array, int test, int num)
{
  int i = 0;
  while (array[i] == test && i < num) {
    i++;
  }
  if (i == num) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}
/*open and initialize the buffer; */
void alloc_buffer(Bit_stream_struc* bs, int size)
{
  bs->buf = (unsigned char FAR*) mem_alloc(size * sizeof(unsigned char), "buffer");
  bs->buf_size = size;
}
/*empty and close the buffer */
void desalloc_buffer(Bit_stream_struc* bs)
{
  free(bs->buf);
}
/***********************************************************************
*
* Global Function Definitions
*
***********************************************************************/
/* The system uses a variety of data files. By opening them via this
function, we can accommodate various locations. */
FILE* OpenTableFile(char* name)
{
  char fulname[80];
  char* envdir;
  FILE* f;
  fulname[0] = '\0';
#ifdef TABLES_PATH
  strcpy(fulname, TABLES_PATH); /* default relative path for tables */
#endif /* TABLES_PATH */ /* (includes terminal path seperator */
#ifdef UNIX /* envir. variables for UNIX only */
  {
    char* getenv();
    envdir = getenv(MPEGTABENV); /* check for environment */
    if (envdir != NULL) {
      strcpy(fulname, envdir);
    }
    strcat(fulname, PATH_SEPARATOR); /* add a "/" on the end */
  }
#endif /* UNIX */
  strcat(fulname, name);
  if ((f = fopen(fulname, "r")) == NULL) {
    fprintf(stderr, "OpenTable: could not find %s\n", fulname);
#ifdef UNIX
    if (envdir != NULL) {
      fprintf(stderr, "Check %s directory '%s'\n", MPEGTABENV, envdir);
    }
    else
      fprintf(stderr, "Check local directory './%s' or setenv %s\n",
          TABLES_PATH, MPEGTABENV);
#else /* not unix : no environment variables */
#ifdef TABLES_PATH
    fprintf(stderr, "Check local directory './%s'\n", TABLES_PATH);
#endif /* TABLES_PATH */
#endif /* UNIX */
  }
  return f;
}
/***********************************************************************
/*
/* Read one of the data files ("alloc_*") specifying the bit allocation/
/* quatization parameters for each subband in layer II encoding
/*
/**********************************************************************/
int read_bit_alloc(table, alloc) /* read in table, return # subbands */
int table;
al_table* alloc;
{
  unsigned int a, b, c, d, i, j;
  FILE* fp;
  char name[16], t[80];
  int sblim;
  strcpy(name, "alloc_0");
  switch (table) {
  case 0 :
    name[6] = '0';
    break;
  case 1 :
    name[6] = '1';
    break;
  case 2 :
    name[6] = '2';
    break;
  case 3 :
    name[6] = '3';
    break;
  default :
    name[6] = '0';
  }
  if (!(fp = OpenTableFile(name))) {
    printf("Please check bit allocation table %s\n", name);
    exit(1);
  }
  printf("using bit allocation table %s\n", name);
  fgets(t, 80, fp);
  sscanf(t, "%d\n", &sblim);
  while (!feof(fp)) {
    fgets(t, 80, fp);
    sscanf(t, "%d %d %d %d %d %d\n", &i, &j, &a, &b, &c, &d);
    (*alloc)[i][j].steps = a;
    (*alloc)[i][j].bits = b;
    (*alloc)[i][j].group = c;
    (*alloc)[i][j].quant = d;
  }
  fclose(fp);
  return sblim;
}
/***********************************************************************
/*
/* Using the decoded info the appropriate possible quantization per
/* subband table is loaded
/*
/**********************************************************************/
int pick_table(fr_ps) /* choose table, load if necess, return # sb's */
frame_params* fr_ps;
{
  int table, lay, ws, bsp, br_per_ch, sfrq;
  int sblim = fr_ps->sblimit; /* return current value if no load */
  lay = fr_ps->header->lay - 1;
  bsp = fr_ps->header->bitrate_index;
  br_per_ch = bitrate[lay][bsp] / fr_ps->stereo;
  ws = fr_ps->header->sampling_frequency;
  sfrq = s_freq[ws];
  /* decision rules refer to per-channel bitrates (kbits/sec/chan) */
  if ((sfrq == 48 && br_per_ch >= 56) ||
      (br_per_ch >= 56 && br_per_ch <= 80)) {
    table = 0;
  }
  else if (sfrq != 48 && br_per_ch >= 96) {
    table = 1;
  }
  else if (sfrq != 32 && br_per_ch <= 48) {
    table = 2;
  }
  else {
    table = 3;
  }
  if (fr_ps->tab_num != table) {
    if (fr_ps->tab_num >= 0) {
      mem_free((void**) & (fr_ps->alloc));
    }
    fr_ps->alloc = (al_table FAR*) mem_alloc(sizeof(al_table),
        "alloc");
    sblim = read_bit_alloc(fr_ps->tab_num = table, fr_ps->alloc);
  }
  return sblim;
}
int js_bound(lay, m_ext)
int lay, m_ext;
{
  static int jsb_table[3][4] = { { 4, 8, 12, 16 }, { 4, 8, 12, 16},
    { 0, 4, 8, 16}
  }; /* lay+m_e -> jsbound */
  if (lay < 1 || lay > 3 || m_ext < 0 || m_ext > 3) {
    fprintf(stderr, "js_bound bad layer/modext (%d/%d)\n", lay, m_ext);
    exit(1);
  }
  return(jsb_table[lay - 1][m_ext]);
}
void hdr_to_frps(fr_ps) /* interpret data in hdr str to fields in fr_ps */
frame_params* fr_ps;
{
  layer* hdr = fr_ps->header; /* (or pass in as arg?) */
  fr_ps->actual_mode = hdr->mode;
  fr_ps->stereo = (hdr->mode == MPG_MD_MONO) ? 1 : 2;
  if (hdr->lay == 2) {
    fr_ps->sblimit = pick_table(fr_ps);
  }
  else {
    fr_ps->sblimit = SBLIMIT;
  }
  if (hdr->mode == MPG_MD_JOINT_STEREO) {
    fr_ps->jsbound = js_bound(hdr->lay, hdr->mode_ext);
  }
  else {
    fr_ps->jsbound = fr_ps->sblimit;
  }
  /* alloc, tab_num set in pick_table */
}
void WriteHdr(fr_ps, s)
frame_params* fr_ps;
FILE* s;
{
  layer* info = fr_ps->header;
  fprintf(s, "HDR: s=FFF, id=%X, l=%X, ep=%X, br=%X, sf=%X, pd=%X, ",
      info->version, info->lay, !info->error_protection,
      info->bitrate_index, info->sampling_frequency, info->padding);
  fprintf(s, "pr=%X, m=%X, js=%X, c=%X, o=%X, e=%X\n",
      info->extension, info->mode, info->mode_ext,
      info->copyright, info->original, info->emphasis);
  fprintf(s, "layer=%s, tot bitrate=%d, sfrq=%.1f, mode=%s, ",
      layer_names[info->lay - 1], bitrate[info->lay - 1][info->bitrate_index],
      s_freq[info->sampling_frequency], mode_names[info->mode]);
  fprintf(s, "sblim=%d, jsbd=%d, ch=%d\n",
      fr_ps->sblimit, fr_ps->jsbound, fr_ps->stereo);
  fflush(s);
}
void WriteBitAlloc(bit_alloc, f_p, s)
unsigned int bit_alloc[2][SBLIMIT];
frame_params* f_p;
FILE* s;
{
  int i, j;
  int st = f_p->stereo;
  int sbl = f_p->sblimit;
  int jsb = f_p->jsbound;
  fprintf(s, "BITA ");
  for (i = 0; i < sbl; ++i) {
    if (i == jsb) {
      fprintf(s, "-");
    }
    for (j = 0; j < st; ++j) {
      fprintf(s, "%1x", bit_alloc[j][i]);
    }
  }
  fprintf(s, "\n");
  fflush(s);
}
void WriteScale(bit_alloc, scfsi, scalar, fr_ps, s)
unsigned int bit_alloc[2][SBLIMIT], scfsi[2][SBLIMIT], scalar[2][3][SBLIMIT];
frame_params* fr_ps;
FILE* s;
{
  int stereo = fr_ps->stereo;
  int sblimit = fr_ps->sblimit;
  int lay = fr_ps->header->lay;
  int i, j, k;
  if (lay == 2) {
    fprintf(s, "SFSI ");
    for (i = 0; i < sblimit; i++) for (k = 0; k < stereo; k++)
        if (bit_alloc[k][i]) {
          fprintf(s, "%d", scfsi[k][i]);
        }
    fprintf(s, "\nSCFs ");
    for (k = 0; k < stereo; k++) {
      for (i = 0; i < sblimit; i++)
        if (bit_alloc[k][i])
          switch (scfsi[k][i]) {
          case 0:
            for (j = 0; j < 3; j++)
              fprintf(s, "%2d%c", scalar[k][j][i],
                  (j == 2) ? ';' : '-');
            break;
          case 1:
          case 3:
            fprintf(s, "%2d-", scalar[k][0][i]);
            fprintf(s, "%2d;", scalar[k][2][i]);
            break;
          case 2:
            fprintf(s, "%2d;", scalar[k][0][i]);
          }
      fprintf(s, "\n");
    }
  }
  else { /* lay == 1 */
    fprintf(s, "SCFs ");
    for (i = 0; i < sblimit; i++) for (k = 0; k < stereo; k++)
        if (bit_alloc[k][i]) {
          fprintf(s, "%2d;", scalar[k][0][i]);
        }
    fprintf(s, "\n");
  }
}
void WriteSamples(ch, sample, bit_alloc, fr_ps, s)
int ch;
unsigned int FAR sample[SBLIMIT];
unsigned int bit_alloc[SBLIMIT];
frame_params* fr_ps;
FILE* s;
{
  int i;
  int stereo = fr_ps->stereo;
  int sblimit = fr_ps->sblimit;
  fprintf(s, "SMPL ");
  for (i = 0; i < sblimit; i++)
    if (bit_alloc[i] != 0) {
      fprintf(s, "%d:", sample[i]);
    }
  if (ch == (stereo - 1)) {
    fprintf(s, "\n");
  }
  else {
    fprintf(s, "\t");
  }
}
int NumericQ(s) /* see if a string lookd like a numeric argument */
char* s;
{
  char c;
  while ((c = *s++) != '\0' && isspace((int)c)) /* strip leading ws */
    ;
  if (c == '+' || c == '-') {
    c = *s++; /* perhaps skip leading + or - */
  }
  return isdigit((int)c);
}
int BitrateIndex(layr, bRate) /* convert bitrate in kbps to index */
int layr; /* 1 or 2 */
int bRate; /* legal rates from 32 to 448 */
{
  int index = 0;
  int found = 0;
  while (!found && index < 15) {
    if (bitrate[layr - 1][index] == bRate) {
      found = 1;
    }
    else {
      ++index;
    }
  }
  if (found) {
    return(index);
  }
  else {
    fprintf(stderr, "BitrateIndex: %d (layer %d) is not a legal bitrate\n",
        bRate, layr);
    return(-1); /* Error! */
  }
}
int SmpFrqIndex(sRate) /* convert samp frq in Hz to index */
long sRate; /* legal rates 32000, 44100, 48000 */
{
  if (sRate == 44100L) {
    return(0);
  }
  else if (sRate == 48000L) {
    return(1);
  }
  else if (sRate == 32000L) {
    return(2);
  }
  else {
    fprintf(stderr, "SmpFrqIndex: %ld is not a legal sample rate\n", sRate);
    return(-1); /* Error! */
  }
}
/****************************************************************************
*
* Routines to convert between the Apple SANE extended floating point format
* and the IEEE double precision floating point format. These routines are
* called from within the Audio Interchange File Format (AIFF) routines.
*
*****************************************************************************/
/*
*** Apple's 80-bit SANE extended has the following format:
 1 15 1 63
 +-+-------------+-+-----------------------------+
 |s| e |i| f |
 +-+-------------+-+-----------------------------+
 msb lsb msb lsb
 The value v of the number is determined by these fields as follows:
 If 0 <= e < 32767, then v = (-1)^s * 2^(e-16383) * (i.f).
 If e == 32767 and f == 0, then v = (-1)^s * (infinity), regardless of i.
 If e == 32767 and f != 0, then v is a NaN, regardless of i.
 *** IEEE Draft Standard 754 Double Precision has the following format:
 MSB
 +-+---------+-----------------------------+
 |1| 11 Bits | 52 Bits |
 +-+---------+-----------------------------+
 ^ ^ ^
 | | |
 Sign Exponent Mantissa
*/
/*****************************************************************************
*
* double_to_extended()
*
* Purpose: Convert from IEEE double precision format to SANE extended
* format.
*
* Passed: Pointer to the double precision number and a pointer to what
* will hold the Apple SANE extended format value.
*
* Outputs: The SANE extended format pointer will be filled with the
* converted value.
*
* Returned: Nothing.
*
*****************************************************************************/
void double_to_extended(pd, ps)
double* pd;
char ps[10];
{
#ifdef MACINTOSH
  x96tox80(pd, (extended*) ps);
#else
  register unsigned long top2bits;
  register unsigned short* ps2;
  register IEEE_DBL* p_dbl;
  register SANE_EXT* p_ext;
  p_dbl = (IEEE_DBL*) pd;
  p_ext = (SANE_EXT*) ps;
  top2bits = p_dbl->hi & 0xc0000000;
  p_ext->l1 = ((p_dbl->hi >> 4) & 0x3ff0000) | top2bits;
  p_ext->l1 |= ((p_dbl->hi >> 5) & 0x7fff) | 0x8000;
  p_ext->l2 = (p_dbl->hi << 27) & 0xf8000000;
  p_ext->l2 |= ((p_dbl->lo >> 5) & 0x07ffffff);
  ps2 = (unsigned short*) & (p_dbl->lo);
  ps2++;
  p_ext->s1 = (*ps2 << 11) & 0xf800;
#endif
}
/*****************************************************************************
*
* extended_to_double()
*
* Purpose: Convert from SANE extended format to IEEE double precision
* format.
*
* Passed: Pointer to the Apple SANE extended format value and a pointer
* to what will hold the the IEEE double precision number.
*
* Outputs: The IEEE double precision format pointer will be filled with
* the converted value.
*
* Returned: Nothing.
*
*****************************************************************************/
void extended_to_double(ps, pd)
char ps[10];
double* pd;
{
#ifdef MACINTOSH
  x80tox96((extended*) ps, pd);
#else
  register unsigned long top2bits;
  register IEEE_DBL* p_dbl;
  register SANE_EXT* p_ext;
  p_dbl = (IEEE_DBL*) pd;
  p_ext = (SANE_EXT*) ps;
  top2bits = p_ext->l1 & 0xc0000000;
  p_dbl->hi = ((p_ext->l1 << 4) & 0x3ff00000) | top2bits;
  p_dbl->hi |= (p_ext->l1 << 5) & 0xffff0;
  p_dbl->hi |= (p_ext->l2 >> 27) & 0x1f;
  p_dbl->lo = (p_ext->l2 << 5) & 0xffffffe0;
  p_dbl->lo |= (unsigned long)((p_ext->s1 >> 11) & 0x1f);
#endif
}
/*****************************************************************************
*
* Read Audio Interchange File Format (AIFF) headers.
*
*****************************************************************************/
int aiff_read_headers(file_ptr, aiff_ptr)
FILE* file_ptr;
IFF_AIFF* aiff_ptr;
{
  register char i;
  register long seek_offset;
  register long sound_position;
  char temp_sampleRate[10];
  ChunkHeader Header;
  Chunk FormChunk;
  CommonChunk CommChunk;
  SoundDataChunk SndDChunk;
  if (fseek(file_ptr, 0, SEEK_SET) != 0) {
    return(-1);
  }
  if (fread(&FormChunk, sizeof(Chunk), 1, file_ptr) != 1) {
    return(-1);
  }
#ifdef IFF_LONG
  if (*(unsigned long*) FormChunk.ckID != IFF_ID_FORM ||
      *(unsigned long*) FormChunk.formType != IFF_ID_AIFF) {
    return(-1);
  }
#else
  if (strncmp(FormChunk.ckID, IFF_ID_FORM, 4) ||
      strncmp(FormChunk.formType, IFF_ID_AIFF, 4)) {
    return(-1);
  }
#endif
  /*
  * chunks need not be in any particular order
  */
  while (fread(&Header, sizeof(ChunkHeader), 1, file_ptr) == 1) {
#ifdef IFF_LONG
    if (*(unsigned long*)Header.ckID == IFF_ID_COMM) {
#else
    if (strncmp(Header.ckID, IFF_ID_COMM, 4) == 0) {
#endif
      /*
      * read comm chunk
      */
      if (fread(&CommChunk.numChannels, sizeof(short), 1, file_ptr) != 1) {
        return(-1);
      }
      if (fread(&CommChunk.numSampleFrames, sizeof(unsigned long), 1,
          file_ptr) != 1) {
        return(-1);
      }
      if (fread(&CommChunk.sampleSize, sizeof(short), 1, file_ptr) != 1) {
        return(-1);
      }
      if (fread(CommChunk.sampleRate, sizeof(char[10]), 1, file_ptr) != 1) {
        return(-1);
      }
      for (i = 0; i < sizeof(char[10]); i++) {
        temp_sampleRate[i] = CommChunk.sampleRate[i];
      }
      extended_to_double(temp_sampleRate, &aiff_ptr->sampleRate);
      aiff_ptr->numChannels = CommChunk.numChannels;
      aiff_ptr->numSampleFrames = CommChunk.numSampleFrames;
      aiff_ptr->sampleSize = CommChunk.sampleSize;
#ifdef IFF_LONG
    }
    else if (*(unsigned long*)Header.ckID == IFF_ID_SSND) {
#else
    }
    else if (strncmp(Header.ckID, IFF_ID_SSND, 4) == 0) {
#endif
      /*
      * read ssnd chunk
      */
      if (fread(&SndDChunk.offset, sizeof(long), 1, file_ptr) != 1) {
        return(-1);
      }
      if (fread(&SndDChunk.blockSize, sizeof(long), 1, file_ptr) != 1) {
        return(-1);
      }
      aiff_ptr->blkAlgn.offset = SndDChunk.offset;
      aiff_ptr->blkAlgn.blockSize = SndDChunk.blockSize;
      aiff_ptr->sampleType = *(unsigned long*)Header.ckID;
      /*
      * record position of sound data
      */
      sound_position = ftell(file_ptr);
      /*
      * skip over sound data to look at remaining chunks
      */
      seek_offset = Header.ckSize - sizeof(SoundDataChunk) +
          sizeof(ChunkHeader);
      if (fseek(file_ptr, seek_offset, SEEK_CUR) != 0) {
        return(-1);
      }
    }
    else {
      /*
      * skip unknown chunk
      */
      seek_offset = Header.ckSize;
      if (fseek(file_ptr, seek_offset, SEEK_CUR) != 0) {
        return(-1);
      }
    }
  }
  return(sound_position);
}
// Seek past some Audio Interchange File Format (AIFF) headers to sound data.
int aiff_seek_to_sound_data(file_ptr)
FILE* file_ptr;
{
  if (fseek(file_ptr, sizeof(Chunk) + sizeof(SoundDataChunk), SEEK_SET) != 0) { //12 16
    return(-1);
  }
  return(0);
}
// Write Audio Interchange File Format (AIFF) headers.
int aiff_write_headers(FILE* file_ptr, IFF_AIFF* aiff_ptr)
{
  register char i;
  register long seek_offset;
  char temp_sampleRate[10];
  Chunk FormChunk;
  CommonChunk CommChunk;
  SoundDataChunk SndDChunk;
#ifdef IFF_LONG
  *(unsigned long*) FormChunk.ckID = IFF_ID_FORM;
  *(unsigned long*) FormChunk.formType = IFF_ID_AIFF;
  *(unsigned long*) CommChunk.ckID = IFF_ID_COMM;
#else
  strncpy(FormChunk.ckID, IFF_ID_FORM, 4);
  strncpy(FormChunk.formType, IFF_ID_AIFF, 4);
  strncpy(CommChunk.ckID, IFF_ID_COMM, 4);
#endif
  double_to_extended(&aiff_ptr->sampleRate, temp_sampleRate);
  for (i = 0; i < sizeof(char[10]); i++) {
    CommChunk.sampleRate[i] = temp_sampleRate[i];
  }
  CommChunk.numChannels = aiff_ptr->numChannels;
  CommChunk.numSampleFrames = aiff_ptr->numSampleFrames;
  CommChunk.sampleSize = aiff_ptr->sampleSize;
  SndDChunk.offset = aiff_ptr->blkAlgn.offset;
  SndDChunk.blockSize = aiff_ptr->blkAlgn.blockSize;
  *(unsigned long*) SndDChunk.ckID = aiff_ptr->sampleType;
  CommChunk.ckSize = sizeof(CommChunk.numChannels) +
      sizeof(CommChunk.numSampleFrames) + sizeof(CommChunk.sampleSize) +
      sizeof(CommChunk.sampleRate);
  SndDChunk.ckSize = sizeof(SoundDataChunk) - sizeof(ChunkHeader) +
      (CommChunk.sampleSize + BITS_IN_A_BYTE - 1) / BITS_IN_A_BYTE *
      CommChunk.numChannels * CommChunk.numSampleFrames;
  FormChunk.ckSize = sizeof(Chunk) + SndDChunk.ckSize + sizeof(ChunkHeader) +
      CommChunk.ckSize;
  if (fseek(file_ptr, 0, SEEK_SET) != 0) {
    return(-1);
  }
  if (fwrite(&FormChunk, sizeof(Chunk), 1, file_ptr) != 1) {
    return(-1);
  }
  if (fwrite(&SndDChunk, sizeof(SoundDataChunk), 1, file_ptr) != 1) {
    return(-1);
  }
  seek_offset = SndDChunk.ckSize - sizeof(SoundDataChunk) +
      sizeof(ChunkHeader);
  if (fseek(file_ptr, seek_offset, SEEK_CUR) != 0) {
    return(-1);
  }
  if (fwrite(CommChunk.ckID, sizeof(ID), 1, file_ptr) != 1) {
    return(-1);
  }
  if (fwrite(&CommChunk.ckSize, sizeof(long), 1, file_ptr) != 1) {
    return(-1);
  }
  if (fwrite(&CommChunk.numChannels, sizeof(short), 1, file_ptr) != 1) {
    return(-1);
  }
  if (fwrite(&CommChunk.numSampleFrames, sizeof(unsigned long), 1,
      file_ptr) != 1) {
    return(-1);
  }
  if (fwrite(&CommChunk.sampleSize, sizeof(short), 1, file_ptr) != 1) {
    return(-1);
  }
  if (fwrite(CommChunk.sampleRate, sizeof(char[10]), 1, file_ptr) != 1) {
    return(-1);
  }
  return(0);
}
/*****************************************************************************
*
* bit_stream.c package
* Author: Jean-Georges Fritsch, C-Cube Microsystems
*
*****************************************************************************/
/********************************************************************
This package provides functions to write (exclusive or read)
information from (exclusive or to) the bit stream.
 If the bit stream is opened in read mode only the get functions are
 available. If the bit stream is opened in write mode only the put
 functions are available.
********************************************************************/
/*open_bit_stream_w(); open the device to write the bit stream into it */
/*open_bit_stream_r(); open the device to read the bit stream from it */
/*close_bit_stream(); close the device containing the bit stream */
/*alloc_buffer(); open and initialize the buffer; */
/*desalloc_buffer(); empty and close the buffer */
/*back_track_buffer(); goes back N bits in the buffer */
/*unsigned int get1bit(); read 1 bit from the bit stream */
/*unsigned long getbits(); read N bits from the bit stream */
/*unsigned long byte_ali_getbits(); read the next byte aligned N bits from*/
/* the bit stream */
/*unsigned long look_ahead(); grep the next N bits in the bit stream without*/
/* changing the buffer pointer */
/*put1bit(); write 1 bit from the bit stream */
/*put1bit(); write 1 bit from the bit stream */
/*putbits(); write N bits from the bit stream */
/*byte_ali_putbits(); write byte aligned the next N bits into the bit stream*/
/*unsigned long sstell(); return the current bit stream length (in bits) */
/*int end_bs(); return 1 if the end of bit stream reached otherwise 0 */
/*int seek_sync(); return 1 if a sync word was found in the bit stream */
/* otherwise returns 0 */
/* refill the buffer from the input device when the buffer becomes empty */
int refill_buffer(bs)
Bit_stream_struc* bs; /* bit stream structure */
{
  register int i = bs->buf_size - 2 - bs->buf_byte_idx;
  register unsigned long n = 1;
  register int index = 0;
  char val[2];
  while ((i >= 0) && (!bs->eob)) {
    if (bs->format == BINARY) {
      n = fread(&bs->buf[i--], sizeof(unsigned char), 1, bs->pt);
    }
    else {
      while ((index < 2) && n) {
        n = fread(&val[index], sizeof(char), 1, bs->pt);
        switch (val[index]) {
        case 0x30:
        case 0x31:
        case 0x32:
        case 0x33:
        case 0x34:
        case 0x35:
        case 0x36:
        case 0x37:
        case 0x38:
        case 0x39:
        case 0x41:
        case 0x42:
        case 0x43:
        case 0x44:
        case 0x45:
        case 0x46:
          index++;
          break;
        default:
          break;
        }
      }
      if (val[0] <= 0x39) {
        bs->buf[i] = (val[0] - 0x30) << 4;
      }
      else {
        bs->buf[i] = (val[0] - 0x37) << 4;
      }
      if (val[1] <= 0x39) {
        bs->buf[i--] |= (val[1] - 0x30);
      }
      else {
        bs->buf[i--] |= (val[1] - 0x37);
      }
      index = 0;
    }
    if (!n) {
      bs->eob = i + 1;
    }
  }
}
static char* he = "0123456789ABCDEF";
/* empty the buffer to the output device when the buffer becomes full */
void empty_buffer(bs, minimum)
Bit_stream_struc* bs; /* bit stream structure */
int minimum; /* end of the buffer to empty */
{
  register int i;
#if BS_FORMAT == BINARY
  for (i = bs->buf_size - 1; i >= minimum; i--) {
    fwrite(&bs->buf[i], sizeof(unsigned char), 1, bs->pt);
  }
#else
  for (i = bs->buf_size - 1; i >= minimum; i--) {
    char val[2];
    val[0] = he[((bs->buf[i] >> 4) & 0x0F)];
    val[1] = he[(bs->buf[i] & 0x0F)];
    fwrite(val, sizeof(char), 2, bs->pt);
  }
#endif
  for (i = minimum - 1; i >= 0; i--) {
    bs->buf[bs->buf_size - minimum + i] = bs->buf[i];
  }
  bs->buf_byte_idx = bs->buf_size - 1 - minimum;
  bs->buf_bit_idx = 8;
}
/* open the device to write the bit stream into it */
void open_bit_stream_w(bs, bs_filenam, size)
Bit_stream_struc* bs; /* bit stream structure */
char* bs_filenam; /* name of the bit stream file */
int size; /* size of the buffer */
{
  if ((bs->pt = fopen(bs_filenam, "wb")) == NULL) {
    printf("Could not create \"%s\".\n", bs_filenam);
    exit(1);
  }
  alloc_buffer(bs, size);
  bs->buf_byte_idx = size - 1;
  bs->buf_bit_idx = 8;
  bs->totbit = 0;
  bs->mode = WRITE_MODE;
  bs->eob = FALSE;
  bs->eobs = FALSE;
}
/* open the device to read the bit stream from it */
void open_bit_stream_r(bs, bs_filenam, size)
Bit_stream_struc* bs; /* bit stream structure */
char* bs_filenam; /* name of the bit stream file */
int size; /* size of the buffer */
{
  register unsigned long n;
  register unsigned char flag = 1;
  unsigned char val;
  if ((bs->pt = fopen(bs_filenam, "rb")) == NULL) {
    printf("Could not find \"%s\".\n", bs_filenam);
    exit(1);
  }
  //检查文件为ASCII还是BINARY, 这里为BINARY
  do {
    n = fread(&val, sizeof(unsigned char), 1, bs->pt);
    switch (val) {
    case 0x30:
    case 0x31:
    case 0x32:
    case 0x33:
    case 0x34:
    case 0x35:
    case 0x36:
    case 0x37:
    case 0x38:
    case 0x39:
    case 0x41:
    case 0x42:
    case 0x43:
    case 0x44:
    case 0x45:
    case 0x46:
    case 0xa: /* \n */
    case 0xd: /* cr */
    case 0x1a: /* sub */
      break;
    default: /* detection of an binary character */
      flag--;
      break;
    }
  }
  while (flag & n);
  if (flag) {
    printf("the bit stream file %s is an ASCII file\n", bs_filenam);
    bs->format = ASCII;
  }
  else {
    bs->format = BINARY;
    printf("the bit stream file %s is a BINARY file\n", bs_filenam);
  }
  fclose(bs->pt);
  if ((bs->pt = fopen(bs_filenam, "rb")) == NULL) {
    printf("Could not find \"%s\".\n", bs_filenam);
    exit(1);
  }
  //为bs->buf分配空间(malloc), buf_size:4M,
  alloc_buffer(bs, size);
  bs->buf_byte_idx = 0;
  bs->buf_bit_idx = 0;
  bs->totbit = 0;
  bs->mode = READ_MODE;
  bs->eob = FALSE;
  bs->eobs = FALSE;
}
/*close the device containing the bit stream after a read process*/
void close_bit_stream_r(bs)
Bit_stream_struc* bs; /* bit stream structure */
{
  fclose(bs->pt);
  desalloc_buffer(bs);
}
/*close the device containing the bit stream after a write process*/
void close_bit_stream_w(bs)
Bit_stream_struc* bs; /* bit stream structure */
{
  empty_buffer(bs, bs->buf_byte_idx);
  fclose(bs->pt);
  desalloc_buffer(bs);
}
int putmask[9] = {0x0, 0x1, 0x3, 0x7, 0xf, 0x1f, 0x3f, 0x7f, 0xff};
int clearmask[9] = {0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x80, 0x0};
void back_track_buffer(bs, N) /* goes back N bits in the buffer */
Bit_stream_struc* bs; /* bit stream structure */
int N;
{
  int tmp = N - (N / 8) * 8;
  register int i;
  bs->totbit -= N;
  for (i = bs->buf_byte_idx; i < bs->buf_byte_idx + N / 8 - 1; i++) {
    bs->buf[i] = 0;
  }
  bs->buf_byte_idx += N / 8;
  if ((tmp + bs->buf_bit_idx) <= 8) {
    bs->buf_bit_idx += tmp;
  }
  else {
    bs->buf_byte_idx ++;
    bs->buf_bit_idx += (tmp - 8);
  }
  bs->buf[bs->buf_byte_idx] &= clearmask[bs->buf_bit_idx];
}
int mask[8] = {0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80};
/*read 1 bit from the bit stream */
unsigned int get1bit(bs)
Bit_stream_struc* bs; /* bit stream structure */
{
  unsigned int bit;
  register int i;
  bs->totbit++;
  if (!bs->buf_bit_idx) {
    bs->buf_bit_idx = 8;
    bs->buf_byte_idx--;
    if ((bs->buf_byte_idx < MINIMUM) || (bs->buf_byte_idx < bs->eob)) {
      if (bs->eob) {
        bs->eobs = TRUE;
      }
      else {
        for (i = bs->buf_byte_idx; i >= 0; i--) {
          bs->buf[bs->buf_size - 1 - bs->buf_byte_idx + i] = bs->buf[i];
        }
        refill_buffer(bs);
        bs->buf_byte_idx = bs->buf_size - 1;
      }
    }
  }
  bit = bs->buf[bs->buf_byte_idx] & mask[bs->buf_bit_idx - 1];
  bit = bit >> (bs->buf_bit_idx - 1);
  bs->buf_bit_idx--;
  return(bit);
}
/*write 1 bit from the bit stream */
void put1bit(bs, bit)
Bit_stream_struc* bs; /* bit stream structure */
int bit; /* bit to write into the buffer */
{
  bs->totbit++;
  bs->buf[bs->buf_byte_idx] |= (bit & 0x1) << (bs->buf_bit_idx - 1);
  bs->buf_bit_idx--;
  if (!bs->buf_bit_idx) {
    bs->buf_bit_idx = 8;
    bs->buf_byte_idx--;
    if (bs->buf_byte_idx < 0) {
      empty_buffer(bs, MINIMUM);
    }
    bs->buf[bs->buf_byte_idx] = 0;
  }
}
/*look ahead for the next N bits from the bit stream */
unsigned long look_ahead(bs, N)
Bit_stream_struc* bs; /* bit stream structure */
int N; /* number of bits to read from the bit stream */
{
  unsigned long val = 0;
  register int j = N;
  register int k, tmp;
  register int bit_idx = bs->buf_bit_idx;
  register int byte_idx = bs->buf_byte_idx;
  if (N > MAX_LENGTH) {
    printf("Cannot read or write more than %d bits at a time.\n", MAX_LENGTH);
  }
  while (j > 0) {
    if (!bit_idx) {
      bit_idx = 8;
      byte_idx--;
    }
    k = MIN(j, bit_idx);
    tmp = bs->buf[byte_idx] & putmask[bit_idx];
    tmp = tmp >> (bit_idx - k);
    val |= tmp << (j - k);
    bit_idx -= k;
    j -= k;
  }
  return(val);
}
/*read N bit from the bit stream */
unsigned long getbits(bs, N)
Bit_stream_struc* bs; /* bit stream structure */
int N; /* number of bits to read from the bit stream */
{
  unsigned long val = 0;
  register int i;
  register int j = N;
  register int k, tmp;
  if (N > MAX_LENGTH) {
    printf("Cannot read or write more than %d bits at a time.\n", MAX_LENGTH);
  }
  bs->totbit += N;
  while (j > 0) {
    if (!bs->buf_bit_idx) {
      bs->buf_bit_idx = 8;
      bs->buf_byte_idx--;
      if ((bs->buf_byte_idx < MINIMUM) || (bs->buf_byte_idx < bs->eob)) {
        if (bs->eob) {
          bs->eobs = TRUE;
        }
        else {
          for (i = bs->buf_byte_idx; i >= 0; i--) {
            bs->buf[bs->buf_size - 1 - bs->buf_byte_idx + i] = bs->buf[i];
          }
          refill_buffer(bs);
          bs->buf_byte_idx = bs->buf_size - 1;
        }
      }
    }
    k = MIN(j, bs->buf_bit_idx);
    tmp = bs->buf[bs->buf_byte_idx] & putmask[bs->buf_bit_idx];
    tmp = tmp >> (bs->buf_bit_idx - k);
    val |= tmp << (j - k);
    bs->buf_bit_idx -= k;
    j -= k;
  }
  return(val);
}
/*write N bits into the bit stream */
void putbits(bs, val, N)
Bit_stream_struc* bs; /* bit stream structure */
unsigned int val; /* val to write into the buffer */
int N; /* number of bits of val */
{
  register int j = N;
  register int k, tmp;
  if (N > MAX_LENGTH) {
    printf("Cannot read or write more than %d bits at a time.\n", MAX_LENGTH);
  }
  bs->totbit += N;
  while (j > 0) {
    k = MIN(j, bs->buf_bit_idx);
    tmp = val >> (j - k);
    bs->buf[bs->buf_byte_idx] |= (tmp & putmask[k]) << (bs->buf_bit_idx - k);
    bs->buf_bit_idx -= k;
    if (!bs->buf_bit_idx) {
      bs->buf_bit_idx = 8;
      bs->buf_byte_idx--;
      if (bs->buf_byte_idx < 0) {
        empty_buffer(bs, MINIMUM);
      }
      bs->buf[bs->buf_byte_idx] = 0;
    }
    j -= k;
  }
}
/*write N bits byte aligned into the bit stream */
void byte_ali_putbits(bs, val, N)
Bit_stream_struc* bs; /* bit stream structure */
unsigned int val; /* val to write into the buffer */
int N; /* number of bits of val */
{
  unsigned long aligning, sstell();
  if (N > MAX_LENGTH) {
    printf("Cannot read or write more than %d bits at a time.\n", MAX_LENGTH);
  }
  aligning = sstell(bs) % 8;
  if (aligning) {
    putbits(bs, (unsigned int)0, (int)(8 - aligning));
  }
  putbits(bs, val, N);
}
/*read the next bute aligned N bits from the bit stream */
unsigned long byte_ali_getbits(bs, N)
Bit_stream_struc* bs; /* bit stream structure */
int N; /* number of bits of val */
{
  unsigned long aligning, sstell();
  if (N > MAX_LENGTH) {
    printf("Cannot read or write more than %d bits at a time.\n", MAX_LENGTH);
  }
  aligning = sstell(bs) % 8;
  if (aligning) {
    getbits(bs, (int)(8 - aligning));
  }
  return(getbits(bs, N));
}
/*return the current bit stream length (in bits)*/
unsigned long sstell(bs)
Bit_stream_struc* bs; /* bit stream structure */
{
  return(bs->totbit);
}
/*return the status of the bit stream*/
/* returns 1 if end of bit stream was reached */
/* returns 0 if end of bit stream was not reached */
int end_bs(bs)
Bit_stream_struc* bs; /* bit stream structure */
{
  return(bs->eobs);
}
/*this function seeks for a byte aligned sync word in the bit stream and
places the bit stream pointer right after the sync.
This function returns 1 if the sync was found otherwise it returns 0 */
int seek_sync(Bit_stream_struc* bs, long sync, int N)
{
  unsigned long aligning;
  unsigned long val;
  long maxi = (int)pow(2.0, (FLOAT)N) - 1;
  aligning = sstell(bs) % ALIGNING;
  if (aligning) {
    getbits(bs, (int)(ALIGNING - aligning));
  }
  val = getbits(bs, N);
  while (((val & maxi) != sync) && (!end_bs(bs))) {
    val <<= ALIGNING;
    val |= getbits(bs, ALIGNING);
  }
  if (end_bs(bs)) {
    return(0);
  }
  else {
    return(1);
  }
}
/*****************************************************************************
*
* End of bit_stream.c package
*
*****************************************************************************/
void update_CRC(data, length, crc)
unsigned int data, length, *crc;
{
  unsigned int masking, carry;
  masking = 1 << length;
  while ((masking >>= 1)) {
    carry = *crc & 0x8000;
    *crc <<= 1;
    if (!carry ^ !(data & masking)) {
      *crc ^= CRC16_POLYNOMIAL;
    }
  }
  *crc &= 0xffff;
}
/*****************************************************************************
*
* CRC error protection package
*
*****************************************************************************/
void I_CRC_calc(fr_ps, bit_alloc, crc)
frame_params* fr_ps;
unsigned int bit_alloc[2][SBLIMIT];
unsigned int* crc;
{
  int i, k;
  layer* info = fr_ps->header;
  int stereo = fr_ps->stereo;
  int jsbound = fr_ps->jsbound;
  *crc = 0xffff; /* changed from '0' 92-08-11 shn */
  update_CRC(info->bitrate_index, 4, crc);
  update_CRC(info->sampling_frequency, 2, crc);
  update_CRC(info->padding, 1, crc);
  update_CRC(info->extension, 1, crc);
  update_CRC(info->mode, 2, crc);
  update_CRC(info->mode_ext, 2, crc);
  update_CRC(info->copyright, 1, crc);
  update_CRC(info->original, 1, crc);
  update_CRC(info->emphasis, 2, crc);
  for (i = 0; i < SBLIMIT; i++)
    for (k = 0; k < ((i < jsbound) ? stereo : 1); k++) {
      update_CRC(bit_alloc[k][i], 4, crc);
    }
}
void II_CRC_calc(fr_ps, bit_alloc, scfsi, crc)
frame_params* fr_ps;
unsigned int bit_alloc[2][SBLIMIT], scfsi[2][SBLIMIT];
unsigned int* crc;
{
  int i, k;
  layer* info = fr_ps->header;
  int stereo = fr_ps->stereo;
  int sblimit = fr_ps->sblimit;
  int jsbound = fr_ps->jsbound;
  al_table* alloc = fr_ps->alloc;
  *crc = 0xffff; /* changed from '0' 92-08-11 shn */
  update_CRC(info->bitrate_index, 4, crc);
  update_CRC(info->sampling_frequency, 2, crc);
  update_CRC(info->padding, 1, crc);
  update_CRC(info->extension, 1, crc);
  update_CRC(info->mode, 2, crc);
  update_CRC(info->mode_ext, 2, crc);
  update_CRC(info->copyright, 1, crc);
  update_CRC(info->original, 1, crc);
  update_CRC(info->emphasis, 2, crc);
  for (i = 0; i < sblimit; i++)
    for (k = 0; k < ((i < jsbound) ? stereo : 1); k++) {
      update_CRC(bit_alloc[k][i], (*alloc)[i][0].bits, crc);
    }
  for (i = 0; i < sblimit; i++)
    for (k = 0; k < stereo; k++)
      if (bit_alloc[k][i]) {
        update_CRC(scfsi[k][i], 2, crc);
      }
}
/*****************************************************************************
*
* End of CRC error protection package
*
*****************************************************************************/
#ifdef MACINTOSH
/*****************************************************************************
*
* Set Macintosh file attributes.
*
*****************************************************************************/
void set_mac_file_attr(fileName, vRefNum, creator, fileType)
char fileName[MAX_NAME_SIZE];
short vRefNum;
OsType creator;
OsType fileType;
{
  /*
  short theFile;
  char pascal_fileName[MAX_NAME_SIZE];
  FInfo fndrInfo;
  CtoPstr(strcpy(pascal_fileName, fileName));
  FSOpen(pascal_fileName, vRefNum, &theFile);
  GetFInfo(pascal_fileName, vRefNum, &fndrInfo);
  fndrInfo.fdCreator = creator;
  fndrInfo.fdType = fileType;
  SetFInfo(pascal_fileName, vRefNum, &fndrInfo);
  FSClose(theFile);
  */
}
#endif
#ifdef MS_DOS
/* ------------------------------------------------------------------------
new_ext.
Puts a new extension name on a file name <filename>.
Removes the last extension name, if any.
92-08-19 shn
------------------------------------------------------------------------ */
char* new_ext(char* filename, char* extname)
{
  int found, dotpos;
  char newname[80];
  /* First, strip the extension */
  dotpos = strlen(filename);
  found = 0;
  do {
    switch (filename[dotpos]) {
    case '.' :
      found = 1;
      break;
    case '\\': /* used by MS-DOS */
    case '/' : /* used by UNIX */
    case ':' :
      found = -1;
      break; /* used by MS-DOS in drive designation */
    default :
      dotpos--;
      if (dotpos < 0) {
        found = -1;
      }
      break;
    }
  }
  while (found == 0);
  if (found == -1) {
    strcpy(newname, filename);
  }
  if (found == 1) {
    strncpy(newname, filename, dotpos);
  }
  newname[dotpos] = '\0';
  strcat(newname, extname);
  return(newname);
}
#endif
#define BUFSIZE 4096
static unsigned long offset, totbit = 0, buf_byte_idx = 0;
static unsigned int buf[BUFSIZE];
static unsigned int buf_bit_idx = 8;
/*return the current bit stream length (in bits)*/
unsigned long hsstell()
{
  return(totbit);
}
/* int putmask[9]={0x0, 0x1, 0x3, 0x7, 0xf, 0x1f, 0x3f, 0x7f, 0xff}; */
extern int putmask[9];
/*read N bit from the bit stream */
unsigned long hgetbits(N)
int N; /* number of bits to read from the bit stream */
{
  unsigned long val = 0;
  register int j = N;
  register int k, tmp;
  /*
  if (N > MAX_LENGTH)
  printf("Cannot read or write more than %d bits at a time.\n", MAX_LENGTH);
  */
  totbit += N;
  while (j > 0) {
    if (!buf_bit_idx) {
      buf_bit_idx = 8;
      buf_byte_idx++;
      if (buf_byte_idx > offset) {
        printf("Buffer overflow !!\n");
        exit(3);
      }
    }
    k = MIN(j, buf_bit_idx);
    tmp = buf[buf_byte_idx % BUFSIZE] & putmask[buf_bit_idx];
    tmp = tmp >> (buf_bit_idx - k);
    val |= tmp << (j - k);
    buf_bit_idx -= k;
    j -= k;
  }
  return(val);
}
unsigned int hget1bit()
{
  return(hgetbits(1));
}
/*write N bits into the bit stream */
void hputbuf(val, N)
unsigned int val; /* val to write into the buffer */
int N; /* number of bits of val */
{
  if (N != 8) {
    printf("Not Supported yet!!\n");
    exit(-3);
  }
  buf[offset % BUFSIZE] = val;
  offset++;
}
void rewindNbits(N)
int N;
{
  totbit -= N;
  buf_bit_idx += N;
  while (buf_bit_idx >= 8) {
    buf_bit_idx -= 8;
    buf_byte_idx--;
  }
}
void rewindNbytes(N)
int N;
{
  totbit -= N * 8;
  buf_byte_idx -= N;
}

