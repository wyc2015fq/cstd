/* File: HWave.c: Speech wav_t File Input/Output */
/* EXPORT->Str2ParmKind: Convert string representation to ParmKind */
static char* pmkmap[] = {"WAVEFORM", "LPC", "LPREFC", "LPCEPSTRA",
    "LPDELCEP", "IREFC",
    "MFCC", "FBANK", "MELSPEC",
    "USER", "DISCRETE", "PLP",
    "ANON"
                        };
ParmKind BaseParmKind(ParmKind kind)
{
  return kind & BASEMASK;
}
/* EXPORT->HasXXXX: returns true if XXXX included in ParmKind */
#define HasEnergy(kind) ( (kind & HASENERGY) != 0 )
#define HasDelta(kind) ( (kind & HASDELTA) != 0 )
#define HasAccs(kind) ( (kind & HASACCS) != 0 )
#define HasThird(kind) ( (kind & HASTHIRD) != 0 )
#define HasNulle(kind) ( (kind & HASNULLE) != 0 )
#define HasCompx(kind) ( (kind & HASCOMPX) != 0 )
#define HasCrcc(kind) ( (kind & HASCRCC) != 0 )
#define HasZerom(kind) ( (kind & HASZEROM) != 0 )
#define HasZeroc(kind) ( (kind & HASZEROC) != 0 )
#define HasVQ(kind) ( (kind & HASVQ) != 0 )
/* EXPORT-> ParmKind2Str: convert given parm kind to string */
char* ParmKind2Str(ParmKind kind, char* buf)
{
  strcpy(buf, pmkmap[BaseParmKind(kind)]);
  if (HasEnergy(kind)) {
    strcat(buf, "_E");
  }
  if (HasDelta(kind)) {
    strcat(buf, "_D");
  }
  if (HasNulle(kind)) {
    strcat(buf, "_N");
  }
  if (HasAccs(kind)) {
    strcat(buf, "_A");
  }
  if (HasThird(kind)) {
    strcat(buf, "_T");
  }
  if (HasCompx(kind)) {
    strcat(buf, "_C");
  }
  if (HasCrcc(kind)) {
    strcat(buf, "_K");
  }
  if (HasZerom(kind)) {
    strcat(buf, "_Z");
  }
  if (HasZeroc(kind)) {
    strcat(buf, "_0");
  }
  if (HasVQ(kind)) {
    strcat(buf, "_V");
  }
  return buf;
}
static void SetCodeStyle(IOConfigRec* cf)
{
  ParmKind tgt = cf->tgtPK & BASEMASK;
  char buf[MAXSTRLEN];
  switch (tgt) {
  case LPC:
  case LPREFC:
  case LPCEPSTRA:
    cf->style = LPCbased;
    break;
  case MELSPEC:
  case FBANK:
  case MFCC:
  case PLP:
    cf->style = FFTbased;
    break;
  case DISCRETE:
    cf->style = VQbased;
    break;
  default:
    HError("SetCodeStyle: Unknown style %s", ParmKind2Str(tgt, buf));
  }
}
static BOOL highDiff = FALSE;
/* TotalComps: return the total number of components in a parameter vector
 with nStatic components and ParmKind pk */
static int TotalComps(int nStatic, ParmKind pk)
{
  int n, x;
  n = nStatic;
  if (pk & HASENERGY) {
    ++n;
  }
  if (pk & HASZEROC) {
    ++n;
  }
  x = n;
  if (pk & HASDELTA) {
    n += x;
    if (pk & HASACCS) {
      n += x;
      if (pk & HASTHIRD) {
        n += x;
        if (highDiff == TRUE) {
          n += x;
        }
      }
    }
  }
  return n;
}
/* FindSpans: Finds the positions of the subcomponents of a parameter
 vector with size components as follows (span values are -ve if
 subcomponent does not exist):
 span: [0] .. [1] [2] [3] [4] .. [5] [6] .. [7] [8] .. [9]
 statics cep0 energy deltas accs third */
static void FindSpans(short span[12], ParmKind k, int size)
{
  int i, stat, en = 0, del = 0, acc = 0, c0 = 0, third = 0, fourth = 0;
  for (i = 0; i < 10; i++) {
    span[i] = -1;
  }
  /*
  if having higher order differentials and the precondition
  is there is third oder differentials
  */
  if (k & HASTHIRD && highDiff == TRUE) {
    fourth = third = acc = del = size / 5;
  }
  else if (k & HASTHIRD) {
    third = acc = del = size / 4;
  }
  else if (k & HASACCS) {
    acc = del = size / 3;
  }
  else if (k & HASDELTA) {
    del = size / 2;
  }
  if (k & HASENERGY) {
    ++en;
  }
  if (k & HASZEROC) {
    ++c0;
  }
  stat = size - c0 - en - del - acc - third - fourth;
  if (stat > 0) {
    span[0] = 0;
    span[1] = stat - 1;
  }
  if (c0 > 0) {
    span[2] = stat;
  }
  if (en > 0) {
    span[3] = stat + c0;
  }
  if (del > 0) {
    span[4] = stat + c0 + en;
    span[5] = stat + c0 + en + del - 1;
  }
  if (acc > 0) {
    span[6] = stat + c0 + en + del;
    span[7] = stat + c0 + en + del + acc - 1;
  }
  if (third > 0) {
    span[8] = stat + c0 + en + del + acc;
    span[9] = stat + c0 + en + del + acc + third - 1;
  }
  if (fourth > 0) {
    span[10] = stat + c0 + en + del + acc + third;
    span[11] = stat + c0 + en + del + acc + third + fourth - 1;
  }
}
/* NumEnergy: return the number of energy components in a parameter vector
 with nTotal components and ParmKind pk */
static int NumEnergy(ParmKind pk)
{
  int e;
  if (!(pk & (HASENERGY | HASZEROC))) {
    return 0;
  }
  e = 1;
  if (pk & HASDELTA) {
    ++e;
    if (pk & HASACCS) {
      ++e;
      if (pk & HASTHIRD) {
        ++e;
        if (highDiff == TRUE) {
          ++e;
        }
      }
    }
  }
  return e;
}
/* NumStatic: return the number of static components in a parameter vector
 with nTotal components and ParmKind pk */
static int NumStatic(int nTotal, ParmKind pk)
{
  short span[12];
  FindSpans(span, pk, nTotal);
  return span[1] - span[0] + 1;
}
int IOConfigRec_reinit(IOConfigRec* cf, double sampPeriod)
{
  char buf[50];
  int frSize = (int)(cf->winDur / sampPeriod);
  int frRate = (int)(cf->tgtSampRate / sampPeriod);
  int btgt = cf->tgtPK & BASEMASK;
  cf->srcSampRate = sampPeriod;
  if (cf->useHam && frSize != cf->frSize) {
    MYREALLOC(cf->winCoef, frSize);
    GenHamWindow(frSize, cf->winCoef);
  }
  cf->frSize = frSize;
  cf->frRate = frRate;
  cf->takeLogs = (btgt == PLP) ? FALSE : btgt != MELSPEC;
  SetCodeStyle(cf);
  if (cf->cepLifter > 0) {
    MYREALLOC(cf->cepWin, cf->numCepCoef);
    GenCepWin(cf->cepLifter, cf->numCepCoef, cf->cepWin);
  }
  MYREALLOC(cf->s, frSize);
  MYREALLOC(cf->r, frSize);
  cf->curPK = btgt = cf->tgtPK & BASEMASK;
  cf->a = cf->k = cf->c = cf->fbank = NULL;
  SetCodeStyle(cf);
  switch (cf->style) {
  case LPCbased:
    cf->nUsed = (btgt == LPCEPSTRA) ? cf->numCepCoef : cf->lpcOrder;
    if (btgt == LPREFC) {
      MYREALLOC(cf->k, cf->lpcOrder);
    }
    else {
      MYREALLOC(cf->a, cf->lpcOrder);
    }
    if (btgt == LPCEPSTRA) {
      MYREALLOC(cf->c, cf->numCepCoef);
    }
    break;
  case FFTbased:
    cf->nUsed = (btgt == MFCC || btgt == PLP) ? cf->numCepCoef : cf->numChans;
    MYREALLOC(cf->fbank, cf->numChans);
    InitFBank(cf);
    if (btgt != PLP) {
      if (btgt == MFCC) {
        MYREALLOC(cf->c, cf->numCepCoef);
      }
    }
    else { /* initialisation for PLP */
      MYREALLOC(cf->c, cf->numCepCoef + 1);
      MYREALLOC(cf->as, cf->numChans + 2);
      MYREALLOC(cf->eql, cf->numChans);
      MYREALLOC(cf->ac, cf->lpcOrder + 1);
      MYREALLOC(cf->lp, cf->lpcOrder + 1);
      MYREALLOC(cf->cm, (cf->lpcOrder + 1) * (cf->numChans + 2));
      InitPLP(cf, cf->lpcOrder, cf->eql, cf->cm);
    }
    break;
  default:
    HError("SetUpForCoding: target %s is not a parameterised form",
        ParmKind2Str(cf->tgtPK, buf));
  }
  if (cf->tgtPK & HASENERGY) {
    cf->curPK |= HASENERGY;
    ++cf->nUsed;
  }
  if (cf->tgtPK & HASZEROC) {
    cf->curPK |= HASZEROC;
    ++cf->nUsed;
  }
#if 0
  if (!ValidConversion(cf->curPK, cf->tgtPK)) {
    HError(6322, "SetUpForCoding: cannot convert to %s", ParmKind2Str(cf->tgtPK, buf));
  }
  if (cf->MatTranFN == NULL) {
    cf->tgtUsed = TotalComps(NumStatic(cf->nUsed, cf->curPK), cf->tgtPK);
  }
  else {
    if (cf->preQual) {
      cf->tgtUsed = NumRows(cf->MatTran) * (1 + HasDelta(cf->tgtPK) + HasAccs(cf->tgtPK) + HasThird(cf->tgtPK));
    }
    else {
      cf->tgtUsed = NumRows(cf->MatTran);
    }
  }
#endif
  cf->nCols = TotalComps(NumStatic(cf->nUsed, cf->curPK), cf->tgtPK);
  cf->nCols = (cf->nCols > cf->tgtUsed) ? cf->nCols : cf->tgtUsed;
  cf->nCvrt = cf->nUsed;
  return 0;
}
int IOConfigRec_free(IOConfigRec* cf)
{
  FREE(cf->winCoef);
  FREE(cf->fbank);
  FREE(cf->cf);
  FREE(cf->cf);
  FREE(cf->loChan);
  FREE(cf->loWt);
  FREE(cf->x);
  return 0;
}
#define SMAX 5 /* max num data streams + 1 */
typedef struct {
  BOOL eSep; /* Energy is in separate stream */
  short swidth[SMAX]; /* [0]=num streams,[i]=width of stream i */
  ParmKind bk; /* parm kind of the parm buffer */
  ParmKind pk; /* parm kind of this obs (bk or DISCRETE) */
  short vq[SMAX]; /* array[1..swidth[0]] of VQ index */
  float* fv[SMAX]; /* array[1..swidth[0]] of Vector */
} Observation;
/*
 A ParmBuf holds either a static table of parameter frames
 loaded from a file or a potentially infinite sequence
 of frames from an audio source. The key information relating
 to the speech data in a buffer or table can be obtained via
 a BufferInfo Record. A static table behaves like a stopped
 buffer.
*/
typedef enum {
  PB_INIT, /* Buffer is initialised and empty */
  PB_WAITING, /* Buffer is waiting for speech */
  PB_STOPPING, /* Buffer is waiting for silence */
  PB_FILLING, /* Buffer is filling */
  PB_STOPPED, /* Buffer has stopped but not yet empty */
  PB_CLEARED /* Buffer has been emptied */
} PBStatus;
/* Added for Microsoft WAVE format */
#ifndef WAVE_FORMAT_PCM
#define WAVE_FORMAT_PCM (0x0001)
#endif
#define WAVE_FORMAT_ALAW (0x0006)
#define WAVE_FORMAT_MULAW (0x0007)
typedef enum _SrcOrder {
  VAXSO, /* little-endian ie low byte first */
  SUNSO, /* big-endian ie hi byte first */
  UNKNOWNSO /* unknown source byte order */
} SrcOrder;
typedef enum {
  NOHEAD, /* Headerless File */
  HAUDIO, /* Direct Audio Input */
  HTK, /* used for both wave and parm files */
  TIMIT, /* Prototype TIMIT database */
  NIST, /* NIST databases eg RM1,TIMIT */
  SCRIBE, /* UK Scribe databases */
  AIFF, /* Apple Audio Interchange format */
  SDES1, /* Sound Designer I format */
  SUNAU8, /* Sun 8 bit MuLaw .au format */
  OGI, /* Oregon Institute format (similar to TIMIT) */
  ESPS, /* used for both wave and parm files */
  ESIG, /* used for both wave and parm files */
  WAV, /* Microsoft WAVE format */
  UNUSED,
  ALIEN, /* Unknown */
  UNDEFF
} FileFormat;
/*
This module provides an interface to speech wave files in a variety of
formats. It is assumed that all files consist of a (possibly null)
header followed by a sequence of 2 byte speech samples.
The type FileFormat lists the currently supported input formats
which are:
 NOHEAD - no head at all
 ALIEN - alien ie unknown
 HTK - the preferred HMM Toolkit format
 TIMIT - the TIMIT CD-ROM database
 NIST - the NIST Resource Management database
 SCRIBE - the UK SCRIBE CD-ROM database
 AIFF - Apple audio interchange format
 SDES1 - Sound Designer I format
 SUNAU8 - Sun 8 bit MU law .au format
 OGI - Oregon Institute format (similar to TIMIT)
 WAV - Microsoft WAV format
 ESPS - Entropic format
 ESIG - Entropic format
If the format is ALIEN, the configuration parameter HEADERSIZE
must be set to specify the size of the header in bytes.
The header of an alien file is simply skipped. The number of
samples is computed from the file size or if input is via a pipe
then the number of samples must be specified by the configuration
parameter NSAMPLES.
The NOHEAD format is treated the same as ALIEN except that a header
size of zero is assumed.
When a data file is opened with OpenWaveInput, the entire contents of the
file are read into memory, byte swapped if necessary, and the file
itself is then closed. Byte swapping may be forced by setting
the configuration parameter BYTEORDER to VAX to indicate that the source
data is in VAX order or any other non-null string to indicate non-VAX
order.
In systems which support it, input can be read from a pipe instead
of a file by setting the environment variable HWAVEFILTER - see the
definition of FOpen in HShell for details.
*/
/* ------------------------ Trace Flags --------------------- */
#define T_OPEN 0002 /* Report wav_t Open Operations */
/* ShowWaveInfo: print info for given wave - for tracing */
static void ShowWaveInfo(wav_t* w)
{
  printf(" sampPeriod: %f\n", w->sampPeriod);
  printf(" nSamples : %ld\n", w->nSamples);
  printf(" nAvail : %ld\n", w->nAvail);
  //printf(" frSize : %d\n", w->frSize);
  //printf(" frRate : %d\n", w->frRate);
  //printf(" frIdx : %d\n", w->frIdx);
}
/* ConsumeHeader: read rest of the header. This call avoids the
 use of stream_seek to allow input from a pipe */
static void ConsumeHeader(stream_t* f, int bytesRead, int headSize)
{
  int i;
  for (i = bytesRead; i < headSize; i++) {
    if (stream_get8(f) == EOF) {
      HError("ConsumeHeader: reading %d of header size %d", i, headSize);
    }
  }
}
/* --------------- ALIEN/NOHEAD Format Interface Routines -------------- */
/* MakeHeaderInfo: set up the header info in supplied wav_t
 with nSamples computed from file size or from confparm (NSAMPLES)
 if input is a pipe */
static CvStatus MakeHeaderInfo(stream_t* f, wav_t* w)
{
  w->sampPeriod = 0.0;
  w->nSamples = stream_filesize(f) / 2;
  return(CV_OK);
}
int MustSwap(SrcOrder x)
{
  SrcOrder x1 = ENDIANNESS == 'l' ? VAXSO : SUNSO;
  return x == x1;
}
/* IsVAXOrder: returns true if machine has VAX ordered bytes */
static BOOL IsVAXOrder(void)
{
  short x, *px;
  unsigned char* pc;
  px = &x;
  pc = (unsigned char*) px;
  *pc = 1;
  *(pc + 1) = 0; /* store bytes 1 0 */
  return x == 1; /* does it read back as 1? */
}
/* SwapInt32: swap byte order of int32 data value *p */
void SwapInt32(int32* p)
{
  char temp, *q;
  q = (char*) p;
  temp = *q;
  *q = *(q + 3);
  *(q + 3) = temp;
  temp = *(q + 1);
  *(q + 1) = *(q + 2);
  *(q + 2) = temp;
}
/* SwapShort: swap byte order of short data value *p */
void SwapShort(short* p)
{
  char temp, *q;
  q = (char*) p;
  temp = *q;
  *q = *(q + 1);
  *(q + 1) = temp;
}
/* GetALIENHeaderInfo: get header of alien file. In addition to info
 required by MakeHeaderInfo, header size must be given via conf-
 iguration parameter (HEADERSIZE) */
static long GetALIENHeaderInfo(stream_t* f, wav_t* w, int mode)
{
  InputAction ia = (InputAction)0;
  int hdSize = 0;
  if (MakeHeaderInfo(f, w) < CV_OK) {
    return -1;
  }
  if (MustSwap(UNKNOWNSO)) {
    ia = (InputAction)(ia | DoBSWAP);
  }
  ConsumeHeader(f, 0, hdSize);
  return w->nSamples * 2;
}
/* GetNOHEADHeaderInfo: create header for headerless file */
static long GetNOHEADHeaderInfo(stream_t* f, wav_t* w, int mode)
{
  InputAction ia = (InputAction)0;
  if (MakeHeaderInfo(f, w) < CV_OK) {
    return -1;
  }
  if (MustSwap(UNKNOWNSO)) {
    ia = (InputAction)(ia | DoBSWAP);
  }
  return w->nSamples * 2;
}
/* LoadData2: Loads samples from stream into Wave Record */
static CvStatus LoadData2(stream_t* f, wav_t* w, long fBytes)
{
  long bufSize;
  long nRead;
  if (fBytes == INT_MAX || fBytes < 0) {
    HError("LoadData2: File size appears to be infinite");
    return(CV_FAIL);
  }
  bufSize = w->nSamples * 2;
  if (bufSize < fBytes) {
    bufSize = fBytes;
  }
  w->nAvail = bufSize / 2;
  wavresize(w, w->nSamples, 1);
  if ((nRead = stream_read(f, w->data, fBytes)) != fBytes) {
    HError("LoadData2: Cannot read data into memory");
    return(CV_FAIL);
  }
  if (nRead != fBytes) {
    w->nSamples = nRead / 2;
  }
  return(CV_OK);
}
/* ---------------------- TIMIT Format Interface Routines --------------------- */
typedef struct { /* TIMIT File Header */
  short hdrSize;
  short version;
  short numChannels;
  short sampRate;
  int32 nSamples;
} TIMIThdr;
/* GetTIMITHeaderInfo: get fixed size binary 12 byte TIMIT header */
static long GetTIMITHeaderInfo(stream_t* f, wav_t* w, int mode)
{
  InputAction ia = (InputAction)0;
  TIMIThdr hdr;
  int fBytes, hdrSize, n = sizeof(hdr);
  BOOL bSwap;
  if ((bSwap = MustSwap(VAXSO))) { /* TIMIT is VAX ordered */
    ia = (InputAction)(ia | DoBSWAP);
  }
  if (stream_read(f, &hdr, n) != n) {
    HError("Cannot read TIMIT format header");
    return -1;
  }
  if (bSwap) {
    SwapShort(&hdr.hdrSize);
    SwapShort(&hdr.version);
    SwapShort(&hdr.numChannels);
    SwapShort(&hdr.sampRate);
    SwapInt32(&hdr.nSamples);
  }
  if (hdr.nSamples < 0 || hdr.sampRate < 0 || hdr.numChannels < 0 ||
      hdr.numChannels > 8) {
    HError("Bad Numbers in TIMIT format header");
    return -1;
  }
  w->nSamples = hdr.nSamples;
  w->sampPeriod = hdr.sampRate * 2.5;
  hdrSize = n;
  fBytes = w->nSamples * 2;
  if (LoadData2(f, w, fBytes) < CV_OK) {
    HError("OpenWaveInput: LoadData2 failed");
    return(0);
  }
  return fBytes;
}
/* ---------------------- OGI Format Interface Routines --------------------- */
typedef struct { /* OGI File Header */
  short hdrSize;
  short version;
  short numChannels;
  short sampRate;
  int32 nSamples;
  int32 lendian;
} OGIhdr;
/* GetOGIHeaderInfo: get fixed size binary 16 byte OGI header */
static long GetOGIHeaderInfo(stream_t* f, wav_t* w, int mode)
{
  InputAction ia = (InputAction)0;
  OGIhdr hdr;
  int fBytes, hdrSize, n = sizeof(hdr);
  BOOL bSwap;
  if ((bSwap = MustSwap(SUNSO))) { /* OGI is SUN ordered */
    ia = (InputAction)(ia | DoBSWAP);
  }
  if (stream_read(f, &hdr, n) != n) {
    HError("Cannot read OGI format header");
    return -1;
  }
  if (bSwap) {
    SwapShort(&hdr.hdrSize);
    SwapShort(&hdr.version);
    SwapShort(&hdr.numChannels);
    SwapShort(&hdr.sampRate);
    SwapInt32(&hdr.nSamples);
    SwapInt32(&hdr.lendian);
  }
  if (hdr.nSamples < 0 || hdr.sampRate < 0 || hdr.numChannels < 0 ||
      hdr.numChannels > 8) {
    HError("Bad Numbers in OGI format header");
    return -1;
  }
  w->nSamples = hdr.nSamples;
  w->sampPeriod = hdr.sampRate * 2.5;
  hdrSize = n;
  fBytes = w->nSamples * 2;
  if (LoadData2(f, w, fBytes) < CV_OK) {
    HError("OpenWaveInput: LoadData2 failed");
    return(0);
  }
  return fBytes;
}
/* ---------------------- NIST Format Interface Routines --------------------- */
static int cNIST; /* current input char */
static int cCount; /* num bytes read */
enum _CompressType {
  SHORTPACK, /* MIT shortpack-v0 */
  SHORTEN, /* CUED Shorten */
  IMULAW /* Interleaved 8 bit u-law */
};
typedef enum _CompressType CompressType;
/* GetNISTToken: get next token delimited by white space from f */
static char* GetNISTToken(stream_t* f, char* buf)
{
  int i = 0;
  while (isspace(cNIST)) {
    cNIST = stream_get8(f);
    ++cCount;
  }
  do {
    if (cNIST == EOF) {
      HError("GetNISTToken: Unexpected end of file");
    }
    buf[i++] = cNIST;
    cNIST = stream_get8(f);
    ++cCount;
  }
  while (!isspace(cNIST) && i < 99);
  buf[i] = '\0';
  return buf;
}
/* NISTSkipLine: skip to next input line of f */
static void NISTSkipLine(stream_t* f)
{
  while (cNIST != '\012') { /* new line is line feed on NIST ROM */
    cNIST = stream_get8(f);
    ++cCount;
    if (cNIST == EOF) {
      HError("NISTSkipLine: Unexpected end of file");
    }
  }
  cNIST = stream_get8(f);
  ++cCount;
}
/* GetNISTIVal: get int val from f (indicated by -i) */
static int GetNISTIVal(stream_t* f)
{
  char buf[100];
  if (strcmp(GetNISTToken(f, buf), "-i") != 0) {
    HError("GetNISTIVal: NIST type indicator -i expected");
  }
  return atoi(GetNISTToken(f, buf));
}
/* GetNISTSVal: get string of lenth n into s (indicated by -sn) */
static void GetNISTSVal(stream_t* f, char* s)
{
  char buf[100];
  GetNISTToken(f, buf);
  if (buf[0] != '-' || buf[1] != 's') {
    HError("GetNISTSVal: NIST type indicator -s expected");
  }
  GetNISTToken(f, s);
  if (atoi(buf + 2) != strlen(s)) {
    HError("GetNISTSVal: bad string length");
  }
}
/* for use in bit-twiddling operations in GetShortPackBlock */
static const unsigned char bitValue[8] = {1, 2, 4, 8, 16, 32, 64, 128};
/* GetShortPackBlock: decode the block of data starting at *inData and
 store at *outData return number of samples in block and update
 *inData and *outData. Data format defined by Mike Phillips at MIT */
static int GetShortPackBlock(char** inData, short** outData)
{
  char* in; /* dereferenced forms of the arguments */
  short* out;
  unsigned char nSamp, nBits;
  unsigned char buf = '\0';
  int i, k;
  BOOL negative;
  int charBits = 0;
  int numChar = 0;
  in = *inData;
  out = *outData;
  nSamp = *(in++); /* number of samples in block */
  nBits = *(in++); /* number of bits / sample excluding sign bit */
  if (nBits > 15) {
    HError("GetShortPackBlock: Incorrect number of bits/sample");
  }
  for (i = 0; i < nSamp; i++, out++) {
    *out = 0;
    if (charBits == 0) {
      buf = *(in++);
      numChar++;
    }
    negative = buf & bitValue[7 - charBits];
    charBits = (charBits + 1) % 8;
    k = nBits;
    while (k > 0) {
      if (charBits == 0) {
        buf = *(in++);
        numChar++;
      }
      if (charBits == 0 && k >= 8) { /* do the whole byte at once */
        *out |= buf << (k - 8);
        k -= 8;
      }
      else if (charBits == 0 && k >= 4) { /* most significant nibble */
        *out |= ((buf & '\360') >> 4) << (k - 4);
        k -= 4;
        charBits = 4;
      }
      else if (charBits == 4 && k >= 4) { /* least significant nibble */
        *out |= (buf & '\017') << (k - 4);
        k -= 4;
        charBits = 0;
      }
      else { /* do a single bit */
        *out |= ((buf & bitValue[7 - charBits]) >> (7 - charBits)) << (k - 1);
        k--;
        charBits = (charBits + 1) % 8;
      }
    }
    if (negative) { /* shortpack data is signed binary */
      *out = (*out == 0) ? -32768 : -(*out);
    }
  }
  if ((numChar % 2) != 0) {
    in++; /* MIT code uses shorts to read/write */
  }
  *outData = out;
  *inData = in;
  return (int)nSamp;
}
/* DecompressShortPack: decode short packed data into new data array */
void DecompressShortPack(wav_t* w)
{
  short* decomp = NULL; /* the decompressed data */
  long outSampCount = 0;
  char* inData;
  short* outData;
  MYREALLOC(decomp, w->nSamples * 2);
  inData = (char*)w->data;
  outData = decomp;
  while (outSampCount < w->nSamples) {
    outSampCount += GetShortPackBlock(&inData, &outData);
  }
  if (outSampCount != w->nSamples) {
    HError("DecompressShortPack: Incorrect number of decompressed samples");
  }
  wavsetdata(w, 1, outSampCount, 1, 0, 16, decomp);
  FREE(decomp);
}
/* DecodeIMuLaw: convert interleaved 8bit MU law to 16 bit linear */
static void DecodeIMuLaw(wav_t* w, int mode)
{
  unsigned char* src;
  short* tgt;
  int i, sample, lchan, rchan;
  //char smode[MAXSTRLEN];
  /* Set Conversion Mode - left channel, right channel or sum */
  //mode = imSum;
  /* Convert data */
  src = (unsigned char*) w->data;
  tgt = w->data;
  sample = 0;
  for (i = 1; i <= w->nSamples; i++, tgt++) {
    lchan = NISTmutab[*src++];
    rchan = NISTmutab[*src++];
    switch (mode) {
    case imLeft:
      sample = lchan;
      break;
    case imRight:
      sample = rchan;
      break;
    case imSum:
      sample = (lchan + rchan) / 2;
      break;
    }
    *tgt = sample;
  }
}
/* ConvertNISTData: decompress the wav_t */
CvStatus ConvertNISTData(wav_t* w, InputAction ia, int mode)
{
  if (ia & DoSPACK) {
    DecompressShortPack(w);
  }
  else if (ia & DoSHORT) {
    HError("ConvertNISTData: NIST Shorten Compression not implemented");
    return(CV_FAIL);
  }
  else if (ia & DoMULAW) {
    DecodeIMuLaw(w, mode);
  }
  return(CV_OK);
}
/* GetNISTHeaderInfo: get the NIST Header info */
static long GetNISTHeaderInfo(stream_t* f, wav_t* w, int mode)
{
  char token[100], *lab, byteFormat[100], sampCoding[100], buf[100];
  BOOL interleaved = FALSE;
  long nS, sR, sS, cC;
  long dataBytes;
  int fBytes, hdrSize;
  InputAction ia = (InputAction)0;
  cNIST = ' ';
  cCount = 0;
  nS = sR = sS = -1;
  byteFormat[0] = '\0';
  sampCoding[0] = '\0';
  lab = GetNISTToken(f, token); /* Check NIST label */
  if (strlen(lab) > 4) {
    *(lab + 4) = '\0';
  }
  if (strcmp(lab, "NIST") != 0) {
    HError("GetNISTHeaderInfo: NIST header label missing");
    return -1;
  }
  NISTSkipLine(f);
  hdrSize = atoi(GetNISTToken(f, token)); /* header #bytes */
  NISTSkipLine(f);
  while (strcmp(GetNISTToken(f, token), "end_head") != 0) {
    if (strcmp(token, "sample_count") == 0) { /* objects */
      nS = GetNISTIVal(f);
    }
    else if (strcmp(token, "sample_rate") == 0) {
      sR = GetNISTIVal(f);
    }
    else if (strcmp(token, "sample_n_bytes") == 0) {
      sS = GetNISTIVal(f);
    }
    else if (strcmp(token, "sample_byte_format") == 0) {
      GetNISTSVal(f, byteFormat);
    }
    else if (strcmp(token, "sample_coding") == 0) {
      GetNISTSVal(f, sampCoding);
    }
    else if (strcmp(token, "channels_interleaved") == 0) {
      GetNISTSVal(f, buf);
      if (strcmp(buf, "TRUE") == 0) {
        interleaved = TRUE;
      }
    }
    else if (strcmp(token, "channel_count") == 0) {
      cC = GetNISTIVal(f);
      if (cC == 2) {
        interleaved = TRUE;
      }
      else if (cC != 1) {
        HError("GetNISTHeaderInfo: channel count = %d in NIST header", cC);
      }
    }
    NISTSkipLine(f);
  }
  if (sS < 1 || sS > 2) {
    HError("GetNISTHeaderInfo: Sample size = %d in NIST header", sS);
    return -1;
  }
  if (nS == -1) {
    HError("GetNISTHeaderInfo: Num samples undefined in NIST header");
    return -1;
  }
  if (sR == -1) {
    HError("GetNISTHeaderInfo: Sample Rate undefined in NIST header");
    return -1;
  }
  if (strcmp(byteFormat, "") == 0) {
    HError("GetNISTHeaderInfo: Byte Format undefined in NIST header");
  }
  if (strcmp(sampCoding, "ulaw") == 0) {
    strcpy(sampCoding, "mu-law");
  }
  if (interleaved) {
    strcat(sampCoding, "-interleaved");
  }
  w->nSamples = nS;
  w->sampPeriod = 1.0E7 / (float)sR;
  dataBytes = w->nSamples * 2;
  /* Fix for bug in original WSJ0 shortpack headers */
  if (strcmp(byteFormat, "shortpack-v0") == 0) {
    strcpy(sampCoding, byteFormat);
    strcpy(byteFormat, "01");
  }
  /* standard 16 bit linear formats */
  if (strlen(sampCoding) == 0 || strcmp(sampCoding, "pcm") == 0) {
    if (strcmp(byteFormat, "01") == 0) {
      if (MustSwap(VAXSO)) {
        ia = (InputAction)(ia | DoBSWAP);
      }
    }
    else if (strcmp(byteFormat, "10") == 0)
      if (MustSwap(SUNSO)) {
        ia = (InputAction)(ia | DoBSWAP);
      }
  }
  /* ShortPack compression format */
  else if (DoMatch(sampCoding, "*shortpack*")) {
    ia = (InputAction)(ia | DoSPACK);
    ia = (InputAction)(ia | DoCVT);
    dataBytes = stream_filesize(f);
  }
  /* Shorten compression format */
  else if (DoMatch(sampCoding, "*embedded-shorten*")) {
    ia = (InputAction)(ia | DoSHORT);
    ia = (InputAction)(ia | DoCVT);
    dataBytes = stream_filesize(f);
  }
  /* Interleaved Mu-Law */
  else if (DoMatch(sampCoding, "*mu-law-interleaved*")) {
    /* w->nSamples /= 2; */
    ia = (InputAction)(ia | DoMULAW);
    ia = (InputAction)(ia | DoCVT);
    dataBytes = stream_filesize(f);
  }
  else {
    HError("GetNISTHeaderInfo: unknown byte format in NIST header");
    return -1;
  }
  ConsumeHeader(f, cCount, hdrSize);
  fBytes = dataBytes;
  /* Copy Data from file into memory */
  if (LoadData2(f, w, fBytes) < CV_OK) {
    HError("OpenWaveInput: LoadData2 failed");
    return(0);
  }
  if (ConvertNISTData(w, ia, mode) < CV_OK) {
    HError("OpenWaveInput: No Convertor for format ");
    return(0);
  }
  return dataBytes;
}
/* ---------------------- SDES1 Format Interface Routines --------------------- */
typedef struct { /* skeleton SDES1 format header */
  short hdrSize; /* should be 1336 */
  char fill1[182];
  int32 fileSize; /* num samples * 2 */
  char fill[832];
  int32 sampRate; /* sample rate in Hertz */
  int32 sampPeriod; /* sample period in usecs */
  short sampSize; /* sample size in bits (16) */
} SDes1Header;
/* GetSDES1HeaderInfo: get header for Sound Designer 1 format file */
static long GetSDES1HeaderInfo(stream_t* f, wav_t* w, int mode)
{
  InputAction ia = (InputAction)0;
  SDes1Header hdr;
  int fBytes, hdrSize, n = sizeof(hdr);
  BOOL bSwap;
  if ((bSwap = MustSwap(UNKNOWNSO))) { /* User might know byte order */
    ia = (InputAction)(ia | DoBSWAP);
  }
  if (stream_read(f, &hdr, n) != n) {
    HError("GetSDES1HeaderInfo: Cannot read SDES1 format header");
    return -1;
  }
  if (hdr.hdrSize != 1336) {
    HError("GetSDES1HeaderInfo: Bad HeaderSize in SDES1 format header");
    return -1;
  }
  if (bSwap) {
    SwapShort(&hdr.hdrSize);
    SwapInt32(&hdr.fileSize);
    SwapInt32(&hdr.sampRate);
    SwapInt32(&hdr.sampPeriod);
    SwapShort(&hdr.sampSize);
  }
  w->nSamples = hdr.fileSize / 2;
  w->sampPeriod = 1.0E7 / (float)hdr.sampRate;
  hdrSize = 1336;
  fBytes = w->nSamples * 2;
  return fBytes;
}
/* ---------------------- SCRIBE Format Interface Routines --------------------- */
/* GetSCRIBEHeaderInfo: create a header for a SCRIBE waveform file */
static long GetSCRIBEHeaderInfo(stream_t* f, wav_t* w, int mode)
{
  int hdrSize = 0;
  InputAction ia = (InputAction)0;
  if (MustSwap(UNKNOWNSO)) { /* User might know byte order */
    ia = (InputAction)(ia | DoBSWAP);
  }
  w->sampPeriod = 0.0; /* User must specify this */
  w->nSamples = stream_filesize(f) / 2;
  return w->nSamples * 2;
}
/* ---------------------- SUNAU8 Format Interface Routines --------------------- */
/* Note this is a special case of the NeXt SNDSoundStruct header */
typedef struct { /* SUNAU8 format header */
  int32 magic; /* magic number 0x2e736e64 */
  int32 dataLocation; /* offset to start of data */
  int32 dataSize; /* number of bytes of data */
  int32 dataFormat; /* format code */
  int32 sampRate; /* sample rate code */
  int32 numChan; /* number of channels */
  char info[4];
} SunAU8Header;
/* GetSUNAU8HeaderInfo: get header for SUN 8bit mulaw sound file */
static long GetSUNAU8HeaderInfo(stream_t* f, wav_t* w, int mode)
{
  InputAction ia = (InputAction)0;
  int n, hdrSize = 0;
  BOOL bSwap;
  SunAU8Header hdr;
  n = sizeof(SunAU8Header);
  if (stream_read(f, &hdr, n) != n) {
    HError("GetSunAU8HeaderInfo: Cannot read SunAU8 format header");
    return -1;
  }
  if ((bSwap = MustSwap(UNKNOWNSO))) { /* User might know byte order */
    SwapInt32(&hdr.magic);
    SwapInt32(&hdr.dataLocation);
    SwapInt32(&hdr.dataSize);
    SwapInt32(&hdr.dataFormat);
    SwapInt32(&hdr.sampRate);
    SwapInt32(&hdr.numChan);
  }
  if (hdr.magic != 0x2e736e64 || hdr.dataFormat != 1 /* 8 bit mulaw */
      || hdr.numChan != 1 /* mono */) {
    HError("GetSUNAU8HeaderInfo: Bad Numbers in SUNAU8 format header");
    return -1;
  }
  w->nSamples = hdr.dataSize;
  w->sampPeriod = 1248; /* 8012.821 Hz codec input rate */
  hdrSize = hdr.dataLocation;
  ConsumeHeader(f, n, hdrSize);
  ia = (InputAction)(ia | DoCVT); /* convert 8bit mulaw to 16bit linear */
  return w->nSamples; /* data size = 2 * file size */
}
/* ConvertSUNAU8Data: convert 8bit MU law to 16 bit linear */
static CvStatus ConvertSUNAU8Data(wav_t* w)
{
  unsigned char* src, ulawbyte;
  short* tgt;
  static int exp_lut[8] = {0, 132, 396, 924, 1980, 4092, 8316, 16764};
  int i, sign, exponent, mantissa, sample;
  /* Convert data */
  src = ((unsigned char*) w->data) + w->nSamples - 1;
  tgt = ((short*) w->data) + w->nSamples - 1;
  for (i = 1; i <= w->nSamples; i++, src--, tgt--) {
    ulawbyte = ~(*src);
    sign = (ulawbyte & 0x80);
    exponent = (ulawbyte >> 4) & 0x07;
    mantissa = ulawbyte & 0x0F;
    sample = exp_lut[exponent] + (mantissa << (exponent + 3));
    *tgt = (sign != 0) ? -sample : sample;
  }
  return(CV_OK);
}
/* ---------------------- AIFF Format Interface Routines --------------------- */
typedef struct { /* skeleton chunk record */
  int32 id; /* must be 'FORM' */
  int32 size; /* size of rest of chunk */
  int32 formType; /* must be 'AIFF' */
} FormChunk;
typedef struct {
  int32 id;
  int32 size;
  short numChannels;
} CommonChunk1;
typedef struct {
  unsigned int nSamples;
  short sampSize;
  /* extended sampRate; Apples non-standard fp format!! */
} CommonChunk2;
/* GetAIFFHeaderInfo: get AIFF format header */
static long GetAIFFHeaderInfo(stream_t* f, wav_t* w, int mode)
{
  InputAction ia = (InputAction)0;
  FormChunk fchunk;
  int fn = sizeof fchunk;
  int hdrSize = 0;
  long sndStart = 0; /* start of sound chunk */
  long fPtr;
  CommonChunk1 ch1, commchunk1;
  CommonChunk2 ch2, commchunk2;
  int cn1 = 10; /* sizeof(long) + sizeof(long) + sizeof(short); */
  int cn2 = 6; /* sizeof(long) + sizeof(short); */
  BOOL hasCC = FALSE, hasSC = FALSE;
  const long fcid = 0x464f524d; /* 'FORM' */
  const long ccid = 0x434f4d4d; /* 'COMM' */
  const long scid = 0x53534e44; /* 'SSND' */
  if (MustSwap(SUNSO)) {
    HError("GetAIFFHeaderInfo: Cannot byte swap AIFF format");
    return -1;
  }
  //rewind(f);
  if (stream_read(f, &fchunk, fn) != fn) {
    HError("GetAIFFHeaderInfo: Cannot read AIFF form chunk");
    return -1;
  }
  if (fchunk.id != fcid) {
    HError("GetAIFFHeaderInfo: Not an AIFF file!");
    return -1;
  }
  fPtr = 12;
  while (!(hasCC && hasSC)) {
    if (stream_seek(f, fPtr, SEEK_SET) != 0) {
      HError("GetAIFFHeaderInfo: Seek error searching for AIFF chunks");
      return -1;
    }
    if ((stream_read(f, &ch1, cn1) != cn1) || (stream_read(f, &ch2, cn2) != cn2)) {
      HError("GetAIFFHeaderInfo: Cannot read AIFF common chunk");
      return -1;
    }
    if (ch1.id == ccid) { /* common chunk found */
      hasCC = TRUE;
      commchunk1 = ch1;
      commchunk2 = ch2;
    }
    if (ch1.id == scid) { /* sound chunk found */
      hasSC = TRUE;
      sndStart = fPtr;
    }
    fPtr += ch1.size + 8;
  }
  w->nSamples = commchunk2.nSamples;
  w->sampPeriod = 625; /* fudge to avoid decoding Apples
 10 byte floating point format - assume 16 kHz */
  hdrSize = sndStart + 16;
  return w->nSamples * 2;
}
/* --------------------- WAV Format Interface Routines --------------------- */
/* GetWAVHeaderInfo: get header for Microsoft WAVE format sound file */
static long GetWAVHeaderInfo(stream_t* f, wav_t* w, int mode)
{
  InputAction ia = (InputAction)0;
  char magic[4];
  int32 len, lng, numBytes, fBytes;
  char c;
  short sht, sampSize, type, chans;
  if (MustSwap(VAXSO)) {
    ia = (InputAction)(ia | DoBSWAP);
  }
  stream_read(f, magic, 4);
  if (strncmp("RIFF", magic, 4)) {
    HError("Input file is not in RIFF format");
    return -1;
  }
  stream_read(f, &lng, 4);
  stream_read(f, magic, 4);
  if (strncmp("WAVE", magic, 4)) {
    HError("Input file is not in WAVE format");
    return -1;
  }
  /* Look for "fmt " before end of file */
  while (1) {
    if (stream_eof(f)) {
      HError("No data portion in WAVE file");
      return -1;
    }
    stream_read(f, magic, 4);
    stream_read(f, &len, 4);
    if (ia & DoBSWAP) {
      SwapInt32(&len);
    }
    /* Check for data chunk */
    if (strncmp("data", magic, 4) == 0) {
      break;
    }
    if (strncmp("fmt ", magic, 4) == 0) {
      stream_read(f, &type, 2);
      if (ia & DoBSWAP) {
        SwapShort(&type);
      }
      if (type != WAVE_FORMAT_PCM && type != WAVE_FORMAT_MULAW && type != WAVE_FORMAT_ALAW) {
        HError("Only standard PCM, mu-law & a-law supported");
        return -1;
      }
      if (type == WAVE_FORMAT_MULAW) {
        ia = (InputAction)(ia | (DoMULAW | DoCVT));
      }
      else if (type == WAVE_FORMAT_ALAW) {
        ia = (InputAction)(ia | (DoALAW | DoCVT));
      }
      stream_read(f, &chans, 2); /* Number of Channels */
      if (ia & DoBSWAP) {
        SwapShort(&chans);
      }
      if (chans != 1 && chans != 2) {
        HError("Neither mono nor stereo!");
        return -1;
      }
      if (chans == 2) {
        ia = (InputAction)(ia | (DoSTEREO | DoCVT));
      }
      stream_read(f, &lng, 4); /* Sample Rate */
      if (ia & DoBSWAP) {
        SwapInt32(&lng);
      }
      w->sampPeriod = 1.0E7 / (float)lng;
      stream_read(f, &lng, 4); /* Average bytes/second */
      stream_read(f, &sht, 2); /* Block align */
      stream_read(f, &sampSize, 2); /* Data size */
      if (ia & DoBSWAP) {
        SwapShort(&sampSize);
      }
      if (sampSize != 16 && sampSize != 8) {
        HError("Only 8/16 bit audio supported");
        return -1;
      }
      if ((type == WAVE_FORMAT_MULAW || type == WAVE_FORMAT_ALAW) && sampSize != 8) {
        HError("Only 8-bit mu-law/a-law supported");
        return -1;
      }
      if (type == WAVE_FORMAT_PCM && sampSize == 8) {
        ia = (InputAction)(ia | (Do8_16 | DoCVT));
      }
      len -= 16;
    }
    /* Skip chunk */
    for (; len > 0; len--) {
      stream_read(f, &c, 1);
    }
  }
  numBytes = len;
  w->nSamples = numBytes / (sampSize / 8);
  /*If stereo: w->nSamples is the stereo value; changed in convertWAV*/
  fBytes = numBytes;
  /* Copy Data from file into memory */
  if (LoadData2(f, w, fBytes) < CV_OK) {
    HError("OpenWaveInput: LoadData2 failed");
    return(0);
  }
  {
    unsigned char* srcc;
    short* tgt, *srcs;
    int i, sample, lchan, rchan;
    if (ia & DoMULAW) {
      srcc = ((unsigned char*) w->data) + w->nSamples - 1;
      tgt = ((short*) w->data) + w->nSamples - 1;
      for (i = 1; i <= w->nSamples; i++, srcc--, tgt--) {
        *tgt = NISTmutab[*srcc];
      }
      ia = (InputAction)(ia & (~DoBSWAP)); /* Must not byte-swap now */
    }
    if (ia & DoALAW) {
      srcc = ((unsigned char*) w->data) + w->nSamples - 1;
      tgt = ((short*) w->data) + w->nSamples - 1;
      for (i = 1; i <= w->nSamples; i++, srcc--, tgt--) {
        *tgt = a2l[*srcc];
      }
      ia = (InputAction)(ia & (~DoBSWAP)); /* Must not byte-swap now */
    }
    if (ia & Do8_16) {
      srcc = ((unsigned char*) w->data) + w->nSamples - 1;
      tgt = ((short*) w->data) + w->nSamples - 1;
      for (i = 1; i <= w->nSamples; i++, srcc--, tgt--) {
        *tgt = (*srcc * 256) - 32768;
      }
      ia = (InputAction)(ia & (~DoBSWAP));
    }
    if (ia & DoSTEREO) {
      w->nSamples /= 2; /*Final mono number*/
      //mode = imSum;
      srcs = w->data;
      tgt = w->data;
      sample = 0;
      for (i = 1; i <= w->nSamples; i++, tgt++) {
        lchan = *srcs++;
        rchan = *srcs++;
        switch (mode) {
        case imLeft:
          sample = lchan;
          break;
        case imRight:
          sample = rchan;
          break;
        case imSum:
          sample = (lchan + rchan) / 2;
          break;
        }
        *tgt = sample;
      }
    }
    return(CV_OK);
  }
  return numBytes;
}
#if 0
/* EXPORT->ReadEsignalHeader: Get header from Esignal file; return FALSE in case of failure. */
BOOL ReadEsignalHeader(stream_t* f, long* nSamp, long* sampP, short* sampS,
    short* kind, BOOL* bSwap, long* hdrS)
{
  FieldList list, list1;
  FieldSpec* field;
  char* version;
  int inarch = UNKNOWN; /* format variant in input */
  long pre_size = -9999; /* arbitrary invalid value for
 * uninitialized variable */
  long hdr_size = -9999;
  long rec_size = -9999;
  if (!(list = ReadHeader(&version, &inarch, &pre_size, &hdr_size, &rec_size))) {
    HError("ReadEsignalHeader: cannot read Esignal Header");
    return FALSE;
  }
  StoreESIGFieldList(list);
  if ((field = FindField(list, "parmKind"))) {
    *kind = Str2ParmKind((char*) field->data);
  }
  else {
    list1 = FieldOrder(list); /* obtain list with record fields only */
    if (list1 == NULL || list1[0] == NULL) {
      HError("ReadEsignalHeader: "
          "no record fields defined in Esignal file");
      return FALSE;
    }
    if (list1[1] != NULL) {
      HError("ReadEsignalHeader: "
          "extraneous field or missing item parmKind in Esignal file");
      return FALSE;
    }
    field = list1[0];
    if (field->occurrence != REQUIRED) {
      HError("ReadEsignalHeader: "
          "field %s in Esignal file is OPTIONAL; should be REQUIRED",
          field->nSamplesame);
      return FALSE;
    }
    if (FieldLength(field) != 1) {
      HError("ReadEsignalHeader: "
          "field %s in Esignal file has length %ld; 1 expected",
          field->nSamplesame, FieldLength(field));
      return FALSE;
    }
    *kind = WAVEFORM;
  }
  if ((field = FindField(list, "recordFreq"))) {
    *sampP = (long)(1.0E7 / ((double*)field->data)[0]);
  }
  else {
    HError(6252,
        "ReadEsignalHeader: item recordFreq missing from Esignal file");
    return FALSE;
  }
  *sampS = rec_size;
  if (isPipe) {
    *nSamp = INT_MAX;
  }
  else {
    *nSamp = NumberBytes(f, hdr_size, isPipe) / rec_size;
  }
  *hdrS = hdr_size;
  switch (inarch) {
  case EDR1: /* fall through */
  case EDR2:
    *bSwap = vaxOrder;
    break;
  case NATIVE:
    *bSwap = FALSE;
    break;
  case ASCII:
    HError("ReadEsignalHeader: "
        "Esignal ASCII format not supported");
    return FALSE;
  default:
    HError("ReadEsignalHeader: "
        "unrecognized architecture type in Esignal file");
    return FALSE;
  }
  return TRUE;
}
/* GetESIGHeaderInfo: get header info and check format is 1 2-byte item per record */
static long GetESIGHeaderInfo(stream_t* f, wav_t* w, int mode)
{
  int nsamp;
  long nSamp, sampP;
  short sampSize, kind;
  long hdrS;
  BOOL bSwap;
  if (!ReadEsignalHeader(f, &nSamp, &sampP, &sampSize,
      &kind, &bSwap, &hdrS, w->isPipe)) {
    HError("GetESIGHeaderInfo: cannot read Esignal Header");
  }
  if (kind != WAVEFORM) {
    HError("GetESIGHeaderInfo: sample kind is not WAVEFORM");
  }
  if (sampSize != 2) {
    HError("GetESIGHeaderInfo: samples are not shorts");
  }
  w->sampPeriod = sampP;
  w->hdrSize = hdrS;
  w->nSamples = nSamp; /*Will be INT_MAX if pipe*/
  if (bSwap) {
    ia = (InputAction)(ia | DoBSWAP);
  }
  if (w->isPipe) {
    if (!GetConfInt(cParm22, numParm22, "NSAMPLES", &nsamp)) {
      HError("GetESIGHeaderInfo: NSAMPLES not set in config");
      return -1;
    }
    w->nSamples = nsamp;
  }
  return w->nSamples * 2;
}
/* EXPORT->PutESIGHeader: Write header info to ESIG file f */
void PutESIGHeaderInfo(stream_t* f, wav_t* w)
{
  FieldSpec* field, *field1;
  FieldList inList, outList = NULL;
  int len, i;
  /* Create header items first */
  field = NewFieldSpec(CHAR, 1);
  field->occurrence = GLOBAL;
  field->nSamplesame = "commandLine";
  field->dim[0] = strlen(RetrieveCommandLine()) + 1;
  field->data = malloc(field->dim[0]);
  strcpy((char*) field->data, RetrieveCommandLine());
  AddField(&outList, field);
  field = NewFieldSpec(DOUBLE, 0);
  field->occurrence = GLOBAL;
  field->nSamplesame = "recordFreq";
  field->data = malloc(sizeof(double));
  *((double*)field->data) = 1.0E07 / w->sampPeriod;
  AddField(&outList, field);
  field = NewFieldSpec(DOUBLE, 0);
  field->occurrence = GLOBAL;
  field->nSamplesame = "startTime";
  field->data = malloc(sizeof(double));
  if (w->fmt == ESIG) {
    RetrieveESIGFieldList(&inList);
    field1 = FindField(inList, "startTime");
    *((double*)field->data) = *((double*)field1->data);
  }
  else {
    *((double*)field->data) = 0.0;
  }
  AddField(&outList, field);
  if (w->fmt == ESIG) {
    field = NewFieldSpec(NO_TYPE, 0);
    field->occurrence = VIRTUAL;
    field->nSamplesame = "source_1";
    len = FieldListLength(inList);
    for (i = 0; i < len; i++) {
      AddSubfield(field, inList[i]);
    }
    AddField(&outList, field);
  }
  /* Then create actual record descriptor */
  field = NewFieldSpec(SHORT, 1);
  field->dim[0] = 1;
  field->occurrence = REQUIRED;
  field->nSamplesame = "samples";
  AddField(&outList, field);
  WriteHeader(outList,
      (natWriteOrder22 && vaxOrder) ? NATIVE : EDR1,
      f, NULL);
}
/* ---------------------- HTK Format Interface Routines --------------------- */
#define WAVEFORM 0 /* HTK sample kind */
typedef struct { /* HTK File Header */
  int32 nSamples;
  int32 sampPeriod;
  short sampSize;
  short sampKind;
} HTKhdr;
/* EXPORT ReadHTKHeader: get header from HTK file, return false not HTK */
BOOL ReadHTKHeader(stream_t* f, long* nSamp, long* sampP, short* sampS,
    short* kind, BOOL* bSwap)
{
  HTKhdr hdr;
  int fBytes, hdrSize, n = sizeof(hdr);
  if (stream_read(f, &hdr, n) != n) {
    return FALSE;
  }
  if (!natReadOrder22 && vaxOrder) {
    *bSwap = TRUE;
  }
  else {
    *bSwap = MustSwap(UNKNOWNSO);
  }
  if (*bSwap) {
    SwapInt32(&hdr.nSamples);
    SwapInt32(&hdr.sampPeriod);
    SwapShort(&hdr.sampSize);
    SwapShort(&hdr.sampKind);
  }
  if (hdr.sampSize <= 0 || hdr.sampSize > 5000 || hdr.nSamples <= 0 ||
      hdr.sampPeriod <= 0 || hdr.sampPeriod > 1000000) {
    return FALSE;
  }
  *nSamp = hdr.nSamples;
  *sampP = hdr.sampPeriod;
  *sampS = hdr.sampSize;
  *kind = hdr.sampKind;
  return TRUE;
}
/* GetHTKHeaderInfo: get HTK format header and check its a WAVEFORM file */
static long GetHTKHeaderInfo(stream_t* f, wav_t* w, int mode)
{
  BOOL bSwap;
  short kind, size;
  long sp;
  if (!ReadHTKHeader(f, &(w->nSamples), &sp, &size, &kind, &bSwap)) {
    HError("GetHTKHeaderInfo: cannot read HTK Header");
    return -1;
  }
  w->sampPeriod = sp;
  if (bSwap) {
    ia = (InputAction)(ia | DoBSWAP);
  }
  if (kind != WAVEFORM) {
    HError("GetHTKHeaderInfo: sample kind is not WAVEFORM");
    return -1;
  }
  if (size != 2) {
    HError("GetHTKHeaderInfo: samples are not shorts");
    return -1;
  }
  w->hdrSize = sizeof(HTKhdr);
  return w->nSamples * 2;
}
/* EXPORT->WriteHTKHeader: Write header info to HTK file f */
void WriteHTKHeader(stream_t* f, long nSamp, long sampP, short sampS,
    short kind, BOOL* bSwap)
{
  HTKhdr hdr;
  int fBytes, hdrSize, n = sizeof(hdr);
  hdr.nSamples = nSamp;
  hdr.sampSize = sampS;
  hdr.sampKind = kind;
  hdr.sampPeriod = sampP;
  if (!natWriteOrder22 && vaxOrder) {
    SwapInt32(&hdr.nSamples);
    SwapInt32(&hdr.sampPeriod);
    SwapShort(&hdr.sampSize);
    SwapShort(&hdr.sampKind);
    if (bSwap != NULL) {
      *bSwap = TRUE;
    }
  }
  else if (bSwap != NULL) {
    *bSwap = FALSE;
  }
  if (fwrite(&hdr, n) != n) {
    HError("WriteHTKHeader: Cannot write HTK format header");
  }
}
/* PutHTKHeaderInfo: write a HTK (WAVEFORM) file header */
static void PutHTKHeaderInfo(stream_t* f, wav_t* w)
{
  WriteHTKHeader(f, w->nSamples, (long) w->sampPeriod, 2, WAVEFORM, NULL);
}
#endif
void ByteSwap(wav_t* w)
{
  short* p;
  long i;
  p = w->data;
  for (i = 0; i < w->nSamples; i++) {
    SwapShort(p++);
  }
}
/* ---------------------- wav_t Interface Routines --------------------- */
/* EXPORT-> OpenWaveInput: open input waveform file */
int OpenWaveInput(wav_t* w, stream_t* f, int fmt)
{
  InputAction ia = (InputAction)0; /* flags to enable conversions etc */
  long fBytes = 0; /* Num data bytes in file to load */
  //long stindex, enindex; /* segment indices */
  int sampSize = 2;
  int mode = imSum;
  /* Get Header */
  switch (fmt) {
  case ALIEN:
    fBytes = GetALIENHeaderInfo(f, w, mode);
    break;
  case NOHEAD:
    fBytes = GetNOHEADHeaderInfo(f, w, mode);
    break;
  case TIMIT:
    fBytes = GetTIMITHeaderInfo(f, w, mode);
    break;
  case OGI:
    fBytes = GetOGIHeaderInfo(f, w, mode);
    break;
  case NIST:
    fBytes = GetNISTHeaderInfo(f, w, mode);
    break;
  case SCRIBE:
    fBytes = GetSCRIBEHeaderInfo(f, w, mode);
    break;
  case SDES1:
    fBytes = GetSDES1HeaderInfo(f, w, mode);
    break;
  case AIFF:
    fBytes = GetAIFFHeaderInfo(f, w, mode);
    break;
  case SUNAU8:
    fBytes = GetSUNAU8HeaderInfo(f, w, mode);
    sampSize = 1;
    break;
  case WAV:
    fBytes = GetWAVHeaderInfo(f, w, mode);
    break;
#if 0
  case HTK:
    fBytes = GetHTKHeaderInfo(f, w, mode);
    break;
  case ESIG:
    fBytes = GetESIGHeaderInfo(f, w, mode);
    break;
#endif
  default:
    HError("OpenWaveInput: Unknown format %d", fmt);
    return(0);
    break;
  }
  /* If extended indexed file, modify header and skip to segment start */
  if (fBytes < 0) {
    HError("OpenWaveInput: Get[format]HeaderInfo failed");
    return(0);
  }
  /* Check for user override of sample period and set frame size/rate */
  //w->sampPeriod = t;
  if (w->sampPeriod <= 0.0) {
    HError("OpenWaveInput: cannot determine sample period for file");
    return(0);
  }
  //w->frSize = (int)(winDur / w->sampPeriod);
  //w->frRate = (int)(frPeriod / w->sampPeriod);
  /* Copy Data from file into memory */
  if (LoadData2(f, w, fBytes) < CV_OK) {
    HError("OpenWaveInput: LoadData2 failed");
    return(0);
  }
  /* If necessary call format dependent conversion routine */
  if (ia & DoCVT)
    switch (fmt) {
    case NIST:
      break;
    case SUNAU8:
      /* Copy Data from file into memory */
      if (LoadData2(f, w, fBytes) < CV_OK) {
        HError("OpenWaveInput: LoadData2 failed");
        return(0);
      }
      if (ConvertSUNAU8Data(w) < CV_OK) {
        HError("OpenWaveInput: No Convertor for format ");
        return(0);
      }
      break;
    case WAV:
      break;
    default:
      HError("OpenWaveInput: No Convertor for format ");
      return(0);
      break;
    }
  /* If necessary byte swap the waveform data */
  if (ia & DoBSWAP) {
    ByteSwap(w);
  }
  if (0) {
    printf("HWave: Input wav_t Opened\n");
    ShowWaveInfo(w);
  }
  return 1;
}
/* EXPORT->ZeroMeanWave: remove mean of wave w */
void ZeroMeanWave(wav_t* w)
{
  int i;
  short* x;
  double sum = 0.0, y, mean;
  x = (short*)w->data;
  for (i = 0; i < w->nSamples; i++, x++) {
    sum += *x;
  }
  mean = sum / (float)w->nSamples;
  x = (short*)w->data;
  for (i = 0; i < w->nSamples; i++, x++) {
    y = (float) * x - mean;
    if (y < -32767) {
      y = -32767;
      HError("ZeroMeanWave: sample too -ve\n");
    }
    if (y > 32767) {
      y = 32767;
      HError("ZeroMeanWave: sample too +ve\n");
    }
    *x = (short)((y > 0.0) ? y + 0.5 : y - 0.5);
  }
}
/* EXPORT->PutWaveSample: Append given nSamples in buf to wave w */
void PutWaveSample(wav_t* w, long nSamples, const short* buf)
{
  mem_push_back(&w->data, &w->nSamples, buf, nSamples, sizeof(short));
}
/* EXPORT->CloseWaveOutput: Output w to fname and free mem */
CvStatus CloseWaveOutput(const wav_t* w, FileFormat fmt, stream_t* f)
{
  if (f == NULL) { /* Open file */
    HError("CloseWaveOutput: Cannot create file ");
    return(CV_FAIL);
  }
  /* Output Header in appropriate format */
  if (fmt == UNDEFF) {
    fmt = WAV;
  }
  switch (fmt) {
#if 0
  case HTK:
    PutHTKHeaderInfo(f, w);
    break;
  case ESIG:
    PutESIGHeaderInfo(f, w);
    break;
#endif
  case NOHEAD: /* do nothing */
    break;
  default:
    HError("CloseWaveOutput: Cannot save data as");
    return(CV_FAIL);
    break;
  }
  /* Write Body of file */
  if (stream_write(f, w->data, w->nSamples * 2) != w->nSamples * 2) {
    HError("CloseWaveOutput: Cannot write data to ");
    return(CV_FAIL);
  }
  return(CV_OK);
}
ParmKind Str2ParmKind(const char* str)
{
  ParmKind i = -1;
  char* s, buf[255];
  BOOL hasE, hasD, hasN, hasA, hasT, hasF, hasC, hasK, hasZ, has0, hasV, found;
  int len;
  hasV = hasE = hasD = hasN = hasA = hasT = hasF = hasC = hasK = hasZ = has0 = FALSE;
  strcpy(buf, str);
  len = strlen(buf);
  s = buf + len - 2;
  while (len > 2 && *s == '_') {
    switch (*(s + 1)) {
    case 'E':
      hasE = TRUE;
      break;
    case 'D':
      hasD = TRUE;
      break;
    case 'N':
      hasN = TRUE;
      break;
    case 'A':
      hasA = TRUE;
      break;
    case 'C':
      hasC = TRUE;
      break;
    case 'T':
      hasT = TRUE;
      break;
    case 'F':
      hasF = TRUE;
      break;
    case 'K':
      hasK = TRUE;
      break;
    case 'Z':
      hasZ = TRUE;
      break;
    case '0':
      has0 = TRUE;
      break;
    case 'V':
      hasV = TRUE;
      break;
    default:
      HError("unknown ParmKind qualifier %s", str);
    }
    *s = '\0';
    len -= 2;
    s -= 2;
  }
  found = FALSE;
  do {
    s = pmkmap[++i];
    if (strcmp(buf, s) == 0) {
      found = TRUE;
      break;
    }
  }
  while (strcmp("ANON", s) != 0);
  if (!found) {
    return ANON;
  }
  if (i == LPDELCEP) { /* for backward compatibility with V1.2 */
    i = LPCEPSTRA | HASDELTA;
  }
  if (hasE) {
    i |= HASENERGY;
  }
  if (hasD) {
    i |= HASDELTA;
  }
  if (hasN) {
    i |= HASNULLE;
  }
  if (hasA) {
    i |= HASACCS;
  }
  if (hasT) {
    i |= HASTHIRD;
  }
  if (hasK) {
    i |= HASCRCC;
  }
  if (hasC) {
    i |= HASCOMPX;
  }
  if (hasZ) {
    i |= HASZEROM;
  }
  if (has0) {
    i |= HASZEROC;
  }
  if (hasV) {
    i |= HASVQ;
  }
  if (0) {
    printf("ParmKind is %d\n", i);
  }
  return i;
}

