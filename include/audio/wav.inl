#include "cstd.h"
#include "cfile.h"
//#include "img/imgio.inl"
typedef struct wav_t {
  short* data;
  int nSamples; /* No of samples in data */
  int c;
  int fs;
  double sampPeriod; /* Sample period in 100ns units 采样周期 */
  long nAvail; /* Num samples allocated for data */
  //int frSize; /* Num samples per frame */
  //int frRate; /* Frame rate */
  //int frIdx; /* Start of next frame */
  //int hdrSize;
} wav_t;
enum {imSum, imLeft, imRight, imAll};
int wavfree(wav_t* w)
{
  FREE(w->data);
  w->data = NULL;
  w->nSamples = w->c = w->fs = 0;
  return 0;
}
int wavresize(wav_t* w, int n, int c)
{
  MYREALLOC(w->data, n * c);
  w->nSamples = n;
  w->c = c;
  return 0;
}
#define WAV8_16(x) (((x) * 256) - 32768)
int wavsetdata(wav_t* w, int mode, int n, int chans, int fs, int bit, void* y0)
{
  int i, c = 1;
  chans = (chans < 1) ? 1 : chans;
  w->fs = fs;
  mode = (chans < 2) ? imLeft : mode;
  if (imAll == mode) {
    n *= chans;
    c = chans;
  }
  wavresize(w, n, c);
#define WAVSETDATA_CASEMODE(M, OP) case M: for (i = 0; i < n; ++i) { w->data[i] = OP; } break
  if (8 == bit) {
    uchar* y = (uchar*)y0;
    switch (mode) {
      WAVSETDATA_CASEMODE(imLeft, WAV8_16(y[i * chans]));
      WAVSETDATA_CASEMODE(imRight, WAV8_16(y[i * chans + 1]));
      WAVSETDATA_CASEMODE(imSum, (WAV8_16(y[i * chans]) + WAV8_16(y[i * chans + 1])) / 2);
    default:
      break;
    }
  }
  else {
    short* y = (short*)y0;
    switch (mode) {
      WAVSETDATA_CASEMODE(imLeft, (y[i * chans]));
      WAVSETDATA_CASEMODE(imRight, (y[i * chans + 1]));
      WAVSETDATA_CASEMODE(imSum, ((y[i * chans]) + (y[i * chans + 1])) / 2);
    default:
      break;
    }
  }
#undef WAVSETDATA_CASEMODE
  return 0;
}
#define big
#define little
#define log_error printf
#define WAW_ISTAG(tag, p) (*(DWORD*)(tag) == *(DWORD*)(p))
#define WAW_TAG(tag) (*(DWORD*)(tag))
#define WAV_FORMAT_PCM (0x0001)
#define WAV_FORMAT_ALAW (0x0006)
#define WAV_FORMAT_MULAW (0x0007)
typedef enum {//
  DoCVT = 1, /* input conversion needed */
  DoBSWAP = 2, /* byte swap needed */
  DoSPACK = 4, /* SHORT PACK decompression needed */
  DoSHORT = 8, /* SHORTEN decompression needed */
  DoMULAW = 16, /* 8 bit Mu-Law expansion needed */
  DoALAW = 32, /* 8 bit A-Law expansion needed */
  Do8_16 = 64, /* 8 bit PCM expansion needed */
  DoSTEREO = 128 /* Convert stereo to mono*/
} InputAction;
/* NIST mu-to-linear conversion, also valid for wav (I think?)*/
static short int NISTmutab[256] = {
  -32124, -31100, -30076, -29052,
  -28028, -27004, -25980, -24956, -23932, -22908, -21884, -20860,
  -19836, -18812, -17788, -16764, -15996, -15484, -14972, -14460,
  -13948, -13436, -12924, -12412, -11900, -11388, -10876, -10364,
  -9852, -9340, -8828, -8316, -7932, -7676, -7420, -7164, -6908,
  -6652, -6396, -6140, -5884, -5628, -5372, -5116, -4860, -4604,
  -4348, -4092, -3900, -3772, -3644, -3516, -3388, -3260, -3132,
  -3004, -2876, -2748, -2620, -2492, -2364, -2236, -2108, -1980,
  -1884, -1820, -1756, -1692, -1628, -1564, -1500, -1436, -1372,
  -1308, -1244, -1180, -1116, -1052, -988, -924, -876, -844, -812,
  -780, -748, -716, -684, -652, -620, -588, -556, -524, -492, -460,
  -428, -396, -372, -356, -340, -324, -308, -292, -276, -260, -244,
  -228, -212, -196, -180, -164, -148, -132, -120, -112, -104, -96,
  -88, -80, -72, -64, -56, -48, -40, -32, -24, -16, -8, 0, 32124,
  31100, 30076, 29052, 28028, 27004, 25980, 24956, 23932, 22908,
  21884, 20860, 19836, 18812, 17788, 16764, 15996, 15484, 14972,
  14460, 13948, 13436, 12924, 12412, 11900, 11388, 10876, 10364,
  9852, 9340, 8828, 8316, 7932, 7676, 7420, 7164, 6908, 6652, 6396,
  6140, 5884, 5628, 5372, 5116, 4860, 4604, 4348, 4092, 3900, 3772,
  3644, 3516, 3388, 3260, 3132, 3004, 2876, 2748, 2620, 2492, 2364,
  2236, 2108, 1980, 1884, 1820, 1756, 1692, 1628, 1564, 1500, 1436,
  1372, 1308, 1244, 1180, 1116, 1052, 988, 924, 876, 844, 812,
  780, 748, 716, 684, 652, 620, 588, 556, 524, 492, 460, 428, 396,
  372, 356, 340, 324, 308, 292, 276, 260, 244, 228, 212, 196, 180,
  164, 148, 132, 120, 112, 104, 96, 88, 80, 72, 64, 56, 48, 40,
  32, 24, 16, 8, 0
};
static short a2l[] = {
  -5504, -5248, -6016, -5760, -4480, -4224, -4992, -4736,
  -7552, -7296, -8064, -7808, -6528, -6272, -7040, -6784,
  -2752, -2624, -3008, -2880, -2240, -2112, -2496, -2368,
  -3776, -3648, -4032, -3904, -3264, -3136, -3520, -3392,
  -22016, -20992, -24064, -23040, -17920, -16896, -19968, -18944,
  -30208, -29184, -32256, -31232, -26112, -25088, -28160, -27136,
  -11008, -10496, -12032, -11520, -8960, -8448, -9984, -9472,
  -15104, -14592, -16128, -15616, -13056, -12544, -14080, -13568,
  -344, -328, -376, -360, -280, -264, -312, -296,
  -472, -456, -504, -488, -408, -392, -440, -424,
  -88, -72, -120, -104, -24, -8, -56, -40,
  -216, -200, -248, -232, -152, -136, -184, -168,
  -1376, -1312, -1504, -1440, -1120, -1056, -1248, -1184,
  -1888, -1824, -2016, -1952, -1632, -1568, -1760, -1696,
  -688, -656, -752, -720, -560, -528, -624, -592,
  -944, -912, -1008, -976, -816, -784, -880, -848,
  5504, 5248, 6016, 5760, 4480, 4224, 4992, 4736,
  7552, 7296, 8064, 7808, 6528, 6272, 7040, 6784,
  2752, 2624, 3008, 2880, 2240, 2112, 2496, 2368,
  3776, 3648, 4032, 3904, 3264, 3136, 3520, 3392,
  22016, 20992, 24064, 23040, 17920, 16896, 19968, 18944,
  30208, 29184, 32256, 31232, 26112, 25088, 28160, 27136,
  11008, 10496, 12032, 11520, 8960, 8448, 9984, 9472,
  15104, 14592, 16128, 15616, 13056, 12544, 14080, 13568,
  344, 328, 376, 360, 280, 264, 312, 296,
  472, 456, 504, 488, 408, 392, 440, 424,
  88, 72, 120, 104, 24, 8, 56, 40,
  216, 200, 248, 232, 152, 136, 184, 168,
  1376, 1312, 1504, 1440, 1120, 1056, 1248, 1184,
  1888, 1824, 2016, 1952, 1632, 1568, 1760, 1696,
  688, 656, 752, 720, 560, 528, 624, 592,
  944, 912, 1008, 976, 816, 784, 880, 848
};
int stream_gets(stream_t* f, char* buf, int maxlen)
{
  char ch;
  int i = 0;
  for (; (ch = stream_get8(f)) && '\n' != ch && i < maxlen; ++i) {
    buf[i] = ch;
  }
  buf[i] = 0;
  return i;
}
int cstr_gettok(const char* w, int l, int i, char* tok, int len)
{
  int j = 0;
  if (w[i] == ' ') {
    for (; i < l && w[++i] == ' ';);
  }
  for (; j < len && i < l && w[i] != ' ';) {
    tok[j++] = w[i++];
  }
  tok[j] = 0;
  return i + 1;
}
/* get the NIST Header info */
static int wavread_NIST_stream(stream_t* f, wav_t* w, int c)
{
  char buf[256], token[100], type[10], value[100], byteFormat[100] = {0}, sampCoding[100] = {0};
  BOOL interleaved = FALSE;
  int nS, sR, sS, cC, bit, ia = 0;
  int dataBytes, nSamples, hdrSize, sampPeriod, n, i;
  nS = sR = sS = -1;
  stream_seek(f, 0, SEEK_SET);
  stream_gets(f, buf, 256);
  if (!WAW_ISTAG("NIST", buf)) {
    //log_error("NIST header label missing");
    return 0;
  }
  stream_gets(f, buf, 256);
  hdrSize = atoi(buf); /* header #bytes */
  stream_gets(f, buf, 256);
  while ((n = stream_gets(f, buf, 256)) > 0 && strcmp(buf, "end_head") != 0) {
    i = 0;
    i = cstr_gettok(buf, n, i, token, 100);
    i = cstr_gettok(buf, n, i, type, 10);
    i = cstr_gettok(buf, n, i, value, 100);
    if (strcmp(token, "sample_count") == 0) {
      nS = atoi(value);
    }
    else if (strcmp(token, "sample_rate") == 0) {
      sR = atoi(value);
    }
    else if (strcmp(token, "sample_n_bytes") == 0) {
      sS = atoi(value);
    }
    else if (strcmp(token, "sample_sig_bits") == 0) {
      bit = atoi(value);
    }
    else if (strcmp(token, "sample_byte_format") == 0) {
      strcpy(byteFormat, value);
    }
    else if (strcmp(token, "sample_coding") == 0) {
      strcpy(sampCoding, value);
    }
    else if (strcmp(token, "channels_interleaved") == 0) {
      //GetNISTSVal(f,buf);
      if (strcmp(value, "TRUE") == 0) {
        interleaved = TRUE;
      }
    }
    else if (strcmp(token, "channel_count") == 0) {
      cC = atoi(value);
      if (cC == 2) {
        interleaved = TRUE;
      }
      else if (cC != 1) {
        log_error("channel count = %d in NIST header", cC);
      }
    }
  }
  if (sS < 1 || sS > 2) {
    log_error("Sample size = %d in NIST header", sS);
    return 0;
  }
  if (nS == -1) {
    log_error("Num samples undefined in NIST header");
    return 0;
  }
  if (sR == -1) {
    log_error("Sample Rate undefined in NIST header");
    return 0;
  }
  if (*byteFormat == 0) {
    log_error("Byte Format undefined in NIST header");
  }
  if (strcmp(sampCoding, "ulaw") == 0) {
    strcpy(sampCoding, "mu-law");
  }
  if (interleaved) {
    strcat(sampCoding, "-interleaved");
  }
  nSamples = nS;
  sampPeriod = 1.0E7 / (float)sR;
  dataBytes = nSamples * 2;
  /* Fix for bug in original WSJ0 shortpack headers */
  if (strcmp(byteFormat, "shortpack-v0") == 0) {
    strcpy(sampCoding, byteFormat);
    strcpy(byteFormat, "01");
  }
  /* standard 16 bit linear formats */
  if (strlen(sampCoding) == 0 || strcmp(sampCoding, "pcm") == 0) {
    if (strcmp(byteFormat, "01") == 0) {
    }
    else if (strcmp(byteFormat, "10") == 0) {
    }
  }
  /* ShortPack compression format */
  else if (DoMatch(sampCoding, "*shortpack*")) {
    ia = (InputAction)(ia | DoSPACK);
    ia = (InputAction)(ia | DoCVT);
  }
  /* Shorten compression format */
  else if (DoMatch(sampCoding, "*embedded-shorten*")) {
    ia = (InputAction)(ia | DoSHORT);
    ia = (InputAction)(ia | DoCVT);
  }
  /* Interleaved Mu-Law */
  else if (DoMatch(sampCoding, "*mu-law-interleaved*")) {
    /* nSamples /= 2; */
    ia = (InputAction)(ia | DoMULAW);
    ia = (InputAction)(ia | DoCVT);
  }
  else {
    log_error("unknown byte format in NIST header");
    return 0;
  }
  if (w) {
    short* y;
    MYREALLOC(y, sizeof(short)*nSamples);
    stream_seek(f, hdrSize, SEEK_SET);
    stream_read(f, y, sizeof(short)*nSamples);
    wavsetdata(w, c, nSamples, 1, sR, bit, y);
    FREE(y);
  }
  return nSamples;
}
static int wavread_stream(stream_t* f, wav_t* w, int c)
{
  char buf[32];
  int i;
  enum { WAVHDLEN = 44 };
  int32 len, lng, nSamples, samp, avgbytes;
  ushort type, chans, datlen, bit;
  stream_seek(f, 0, SEEK_SET);
  stream_read(f, buf, 12);
  if (!WAW_ISTAG("RIFF", buf)) {
    log_error("Input file is not in RIFF format");
    return 0;
  }
  i = 4;
  MEM_GET32L(buf, i, lng);
  if (!WAW_ISTAG("WAVE", buf + 8)) {
    //log_error("Input file is not in WAVE format");
    return 0;
  }
  while (1) {
    if (stream_eof(f)) {
      log_error("No data portion in WAVE file");
      return 0;
    }
    stream_read(f, buf, 8);
    i = 4;
    MEM_GET32L(buf, i, len);
    /* Check for data chunk */
    if (WAW_ISTAG("data", buf)) {
      break;
    }
    else if (WAW_ISTAG("fact", buf)) {
      ASSERT(len < 32);
      stream_read(f, buf, len);
    }
    else if (WAW_ISTAG("fmt ", buf)) {
      ASSERT(len < 32);
      stream_read(f, buf, len);
      i = 0;
      MEM_GET16L(buf, i, type);
      MEM_GET16L(buf, i, chans);
      MEM_GET32L(buf, i, samp);
      MEM_GET32L(buf, i, avgbytes);
      MEM_GET16L(buf, i, datlen);
      MEM_GET16L(buf, i, bit);
      if (type != WAV_FORMAT_PCM && type != WAV_FORMAT_MULAW && type != WAV_FORMAT_ALAW) {
        log_error("Only standard PCM, mu-law & a-law supported");
        return 0;
      }
      if (chans != 1 && chans != 2) {
        log_error("Neither mono nor stereo!");
        return 0;
      }
      if (bit != 16 && bit != 8) {
        log_error("Only 8/16 bit audio supported");
        return 0;
      }
      if ((type == WAV_FORMAT_MULAW || type == WAV_FORMAT_ALAW) && bit != 8) {
        log_error("Only 8-bit mu-law/a-law supported");
        return 0;
      }
    }
    else {
      for (i = 0; i < len; ++i) {
        stream_read(f, buf, 1);
      }
    }
  }
  nSamples = len / (bit / 8);
  if (w) {
    short* y = NULL;
    MYREALLOC(y, len);
    stream_read(f, y, len);
    wavsetdata(w, c, nSamples, chans, samp, bit, y);
    FREE(y);
  }
  /*If stereo: w->nSamplesSamples is the stereo value; changed in convertWAV*/
  return nSamples;
}
#define wavread_filename(fname, w, c) wavread_file_close(fopen(fname, "rb"), w, c)
int wavread_file_close(FILE* pf, wav_t* w, int c)
{
  int ret = 0;
  double winDur = 250000;
  double frPeriod = 100000;
  if (pf) {
    stream_t f[1] = {0};
    fstream_init(f, pf);
    (ret = wavread_stream(f, w, c)) > 0 || (ret = wavread_NIST_stream(f, w, c)) > 0;
    w->sampPeriod = 1E7 / w->fs;
    fclose(pf);
  }
  return ret;
}
int wavwrite_stream(stream_t* s, const wav_t* w)
{
  int i = 0;
  char buf[54];
  DWORD dw, datasize = w->nSamples * 2, fmtlen = 16;
  WORD word;
  //RIFF 12
  MEM_SETMEM(buf, i, "RIFF", 4);
  dw = 28 + fmtlen + datasize; // dwRiffSize
  MEM_SET32L(buf, i, dw);
  MEM_SETMEM(buf, i, "WAVE", 4);
  //fmt 8
  MEM_SETMEM(buf, i, "fmt ", 4);
  MEM_SET32L(buf, i, fmtlen);
  // 16
  word = WAV_FORMAT_PCM;
  MEM_SET16L(buf, i, word);
  word = w->c;
  MEM_SET16L(buf, i, word);
  dw = w->fs;
  MEM_SET32L(buf, i, dw);
  dw = (w->fs * w->c * 16) >> 3;
  MEM_SET32L(buf, i, dw);
  word = (16 * w->c) >> 3;
  MEM_SET16L(buf, i, word);
  word = 16;
  MEM_SET16L(buf, i, word);
  //MEM_SET16L(buf, i, 0);
  //Data 8
  MEM_SETMEM(buf, i, "data", 4);
  MEM_SET32L(buf, i, datasize);
  stream_write(s, buf, i);
  stream_write(s, w->data, datasize);
  return 0;
}
#define wavwrite_filename(fname, w) wavwrite_file_close(fopen(fname, "wb"), w)
int wavwrite_file_close(FILE* pf, const wav_t* w)
{
  int ret = 0;
  if (pf) {
    stream_t f[1] = {0};
    fstream_init(f, pf);
    ret = wavwrite_stream(f, w);
    fclose(pf);
  }
  return ret;
}

