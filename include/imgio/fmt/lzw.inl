
#ifndef _LZW_INL_
#define _LZW_INL_

//#include "cstd.h"
/*
 * TIFF Library.
 * Rev 5.0 Lempel-Ziv & Welch Compression Support
 *
 * This code is derived from the compress program whose code is
 * derived from software contributed to Berkeley by James A. Woods,
 * derived from original work by Spencer Thomas and Joseph Orost.
 *
 * The original Berkeley copyright notice appears below in its entirety.
 */

/*
 * NB: The 5.0 spec describes a different algorithm than Aldus
 *     implements.  Specifically, Aldus does code length transitions
 *     one code earlier than should be done (for real LZW).
 *     Earlier versions of this library implemented the correct
 *     LZW algorithm, but emitted codes in a bit order opposite
 *     to the TIFF spec.  Thus, to maintain compatibility w/ Aldus
 *     we interpret MSB-LSB ordered codes to be images written w/
 *     old versions of this library, but otherwise adhere to the
 *     Aldus "off by one" algorithm.
 *
 * Future revisions to the TIFF spec are expected to "clarify this issue".
 */
#define LZW_COMPAT    /* include backwards compatibility code */
/*
 * Each strip of data is supposed to be terminated by a CODE_EOI.
 * If the following #define is included, the decoder will also
 * check for end-of-strip w/o seeing this code.  This makes the
 * library more robust, but also slower.
 */
#define LZW_CHECKEOS    /* include checks for strips w/o EOI code */

#define MAXCODE(n)  ((1L<<(n))-1)
/*
 * The TIFF spec specifies that encoded bit
 * strings range from 9 to 12 bits.
 */
#define BITS_MIN  9   /* start with 9 bits */
#define BITS_MAX  12    /* max of 12 bit strings */
/* predefined codes */
#define CODE_CLEAR  256   /* code to clear string table */
#define CODE_EOI  257   /* end-of-information code */
#define CODE_FIRST  258   /* first free code entry */
#define CODE_MAX  MAXCODE(BITS_MAX)
#define HSIZE   9001L   /* 91% occupancy */
#define HSHIFT    (13-8)
#ifdef LZW_COMPAT
/* NB: +1024 is for compatibility with old files */
#define CSIZE   (MAXCODE(BITS_MAX)+1024L)
#else
#define CSIZE   (MAXCODE(BITS_MAX)+1L)
#endif

#define lzw_error printf
#define lzw_warning printf

/*
 * State block for each open TIFF file using LZW
 * compression/decompression.  Note that the predictor
 * state block must be first in this data structure.
 */

/*
 * Encoding-specific state.
 */
typedef uint16 hcode_t;     /* codes fit in 16 bits */

typedef struct {
  long  hash;
  hcode_t code;
} hash_t;

/*
 * Decoding-specific state.
 */
typedef struct code_ent {
  struct code_ent* next;
  unsigned short  length;   /* string len, including this token */
  uchar value;    /* data value */
  uchar firstchar;  /* first token of string */
} code_t;

typedef struct {
  unsigned short  lzw_nbits;    /* # of bits/code */
  unsigned short  lzw_maxcode;  /* maximum code for lzw_nbits */
  unsigned short  lzw_free_ent; /* next free entry in hash table */
  long    lzw_nextdata; /* next bits of i/o */
  long    lzw_nextbits; /* # of valid bits in lzw_nextdata */
  int     lzw_rw_mode;        /* preserve rw_mode from init */

  /* Decoding specific data */
  long  dec_nbitsmask;    /* lzw_nbits 1 bits, right adjusted */
  long  dec_restart;    /* restart count */
#ifdef LZW_CHECKEOS
  long  dec_bitsleft;   /* available bits in raw data */
#endif
  //decodeFunc dec_decode;    /* regular or backwards compatible */
  code_t* dec_codep;    /* current recognized code */
  code_t* dec_oldcodep;   /* previously recognized code */
  code_t* dec_free_entp;    /* next free entry */
  code_t* dec_maxcodep;   /* max available entry */
  code_t* dec_codetab;    /* kept separate for small machines */

  /* Encoding specific data */
  int enc_oldcode;    /* last code encountered */
  long  enc_checkpoint;   /* point at which to clear table */
#define CHECK_GAP 10000   /* enc_ratio check interval */
  long  enc_ratio;    /* current compression ratio */
  long  enc_incount;    /* (input) data bytes encoded */
  long  enc_outcount;   /* encoded (output) bytes */
  uchar* enc_rawlimit;    /* bound on sp_rawdata buffer */
  hash_t* enc_hashtab;    /* kept separate for small machines */
  int sp_row;
  int sp_curstrip;
  uchar* sp_rawdata;
  int sp_rawdatasize;
} LZWCodecState;

/*
 * LZW Decoder.
 */

#ifdef LZW_CHECKEOS
/*
 * This check shouldn't be necessary because each
 * strip is suppose to be terminated with CODE_EOI.
 */
#define NextCode(_sp1, _sp, _bp, _code, _get) {       \
    if ((_sp)->dec_bitsleft < nbits) {        \
      lzw_warning("LZWDecode: Strip %d not terminated with EOI code", _sp->sp_curstrip);        \
      _code = CODE_EOI;         \
    } else {              \
      _get(_sp,_bp,_code);  \
      (_sp)->dec_bitsleft -= nbits;  \
    }               \
  }
#else
#define NextCode(sp, sp, bp, code, get) get(sp, bp, code)
#endif


/*
 * Decode a "hunk of data".
 */
#define GetNextCode(sp, bp, code) {       \
    nextdata = (nextdata<<8) | *(bp)++;     \
    nextbits += 8;            \
    if (nextbits < nbits) {         \
      nextdata = (nextdata<<8) | *(bp)++;   \
      nextbits += 8;          \
    }             \
    code = (hcode_t)((nextdata >> (nextbits-nbits)) & nbitsmask); \
    nextbits -= nbits;          \
  }

static void codeLoop(LZWCodecState* sp)
{
  lzw_error("LZWDecode: Bogus encoding, loop in the code table; scanline %d", sp->sp_row);
}

static int LZWDecode_impl(LZWCodecState* sp, char* op0, int32 occ0, const uchar* bp)
{
  char* op = (char*) op0;
  char* op1 = op;
  long occ = (long) occ0;
  char* tp;
  hcode_t code;
  int len;
  long nbits, nextbits, nextdata, nbitsmask;
  code_t* codep, *free_entp, *maxcodep, *oldcodep;

  /*
   * Restart interrupted output operation.
   */
  if (sp->dec_restart) {
    long residue;

    codep = sp->dec_codep;
    residue = codep->length - sp->dec_restart;

    if (residue > occ) {
      /*
       * Residue from previous decode is sufficient
       * to satisfy decode request.  Skip to the
       * start of the decoded string, place decoded
       * values in the output buffer, and return.
       */
      sp->dec_restart += occ;

      do {
        codep = codep->next;
      }
      while (--residue > occ && codep);

      if (codep) {
        tp = op + occ;

        do {
          *--tp = codep->value;
          codep = codep->next;
        }
        while (--occ && codep);
      }

      return (1);
    }

    /*
     * Residue satisfies only part of the decode request.
     */
    op += residue, occ -= residue;
    tp = op;

    do {
      int t;
      --tp;
      t = codep->value;
      codep = codep->next;
      *tp = t;
    }
    while (--residue && codep);

    sp->dec_restart = 0;
  }

  nbits = sp->lzw_nbits;
  nextdata = sp->lzw_nextdata;
  nextbits = sp->lzw_nextbits;
  nbitsmask = sp->dec_nbitsmask;
  oldcodep = sp->dec_oldcodep;
  free_entp = sp->dec_free_entp;
  maxcodep = sp->dec_maxcodep;

  while (occ > 0) {
    NextCode(sp, sp, bp, code, GetNextCode);

    if (code == CODE_EOI) {
      break;
    }

    if (code == CODE_CLEAR) {
      free_entp = sp->dec_codetab + CODE_FIRST;
      nbits = BITS_MIN;
      nbitsmask = MAXCODE(BITS_MIN);
      maxcodep = sp->dec_codetab + nbitsmask - 1;
      NextCode(sp, sp, bp, code, GetNextCode);

      if (code == CODE_EOI) {
        break;
      }

      *op++ = (char)code, occ--;
      oldcodep = sp->dec_codetab + code;
      continue;
    }

    codep = sp->dec_codetab + code;

    // Add the new entry to the code table.
    if (free_entp < &sp->dec_codetab[0] || free_entp >= &sp->dec_codetab[CSIZE]) {
      lzw_error("LZWDecode: Corrupted LZW table at scanline %d", sp->sp_row);
      return (0);
    }

    free_entp->next = oldcodep;

    if (free_entp->next < &sp->dec_codetab[0] || free_entp->next >= &sp->dec_codetab[CSIZE]) {
      lzw_error("LZWDecode: Corrupted LZW table at scanline %d", sp->sp_row);
      return (0);
    }

    free_entp->firstchar = free_entp->next->firstchar;
    free_entp->length = free_entp->next->length + 1;
    free_entp->value = (codep < free_entp) ? codep->firstchar : free_entp->firstchar;

    if (++free_entp > maxcodep) {
      if (++nbits > BITS_MAX) { // should not happen
        nbits = BITS_MAX;
      }

      nbitsmask = MAXCODE(nbits);
      maxcodep = sp->dec_codetab + nbitsmask - 1;
    }

    oldcodep = codep;

    if (code >= 256) {
      //Code maps to a string, copy string value to output (written in reverse).
      if (codep->length == 0) {
        lzw_error("LZWDecode: Wrong length of decoded string: data probably corrupted at scanline %d", sp->sp_row);
        return (0);
      }

      if (codep->length > occ) {
        /*
         * String is too long for decode buffer,
         * locate portion that will fit, copy to
         * the decode buffer, and setup restart
         * logic for the next decoding call.
         */
        sp->dec_codep = codep;

        do {
          codep = codep->next;
        }
        while (codep && codep->length > occ);

        if (codep) {
          sp->dec_restart = occ;
          tp = op + occ;

          do  {
            *--tp = codep->value;
            codep = codep->next;
          }
          while (--occ && codep);

          if (codep) {
            codeLoop(sp);
          }
        }

        break;
      }

      len = codep->length;
      tp = op + len;

      do {
        int t;
        --tp;
        t = codep->value;
        codep = codep->next;
        *tp = t;
      }
      while (codep && tp > op);

      if (codep) {
        codeLoop(sp);
        break;
      }

      op += len, occ -= len;
    }
    else {
      *op++ = (char)code, occ--;
    }
  }

  sp->lzw_nbits = (unsigned short) nbits;
  sp->lzw_nextdata = nextdata;
  sp->lzw_nextbits = nextbits;
  sp->dec_nbitsmask = nbitsmask;
  sp->dec_oldcodep = oldcodep;
  sp->dec_free_entp = free_entp;
  sp->dec_maxcodep = maxcodep;

  if (occ > 0) {
    lzw_error("LZWDecode: Not enough data at scanline %d (short %d bytes)", (int)sp->sp_row, (int)occ);
    //return (0);
  }

  return op - op1;
}

#ifdef LZW_COMPAT

// Decode a "hunk of data" for old images.
#define GetNextCodeCompat(sp, bp, code) {     \
    nextdata |= (unsigned long) *(bp)++ << nextbits;  \
    nextbits += 8;            \
    if (nextbits < nbits) {         \
      nextdata |= (unsigned long) *(bp)++ << nextbits;\
      nextbits += 8;          \
    }             \
    code = (hcode_t)(nextdata & nbitsmask);     \
    nextdata >>= nbits;         \
    nextbits -= nbits;          \
  }

static int LZWDecodeCompat(LZWCodecState* sp, uchar* op0, int32 occ0)
{
  char* op = (char*) op0;
  long occ = (long) occ0;
  char* tp;
  uchar* bp;
  int code, nbits;
  long nextbits, nextdata, nbitsmask;
  code_t* codep, *free_entp, *maxcodep, *oldcodep;

  assert(sp != NULL);

  // Restart interrupted output operation.
  if (sp->dec_restart) {
    long residue;

    codep = sp->dec_codep;
    residue = codep->length - sp->dec_restart;

    if (residue > occ) {
      /*
       * Residue from previous decode is sufficient
       * to satisfy decode request.  Skip to the
       * start of the decoded string, place decoded
       * values in the output buffer, and return.
       */
      sp->dec_restart += occ;

      do {
        codep = codep->next;
      }
      while (--residue > occ);

      tp = op + occ;

      do {
        *--tp = codep->value;
        codep = codep->next;
      }
      while (--occ);

      return (1);
    }

    /*
     * Residue satisfies only part of the decode request.
     */
    op += residue, occ -= residue;
    tp = op;

    do {
      *--tp = codep->value;
      codep = codep->next;
    }
    while (--residue);

    sp->dec_restart = 0;
  }

  nbits = sp->lzw_nbits;
  nextdata = sp->lzw_nextdata;
  nextbits = sp->lzw_nextbits;
  nbitsmask = sp->dec_nbitsmask;
  oldcodep = sp->dec_oldcodep;
  free_entp = sp->dec_free_entp;
  maxcodep = sp->dec_maxcodep;

  while (occ > 0) {
    NextCode(sp, sp, bp, code, GetNextCodeCompat);

    if (code == CODE_EOI) {
      break;
    }

    if (code == CODE_CLEAR) {
      free_entp = sp->dec_codetab + CODE_FIRST;
      nbits = BITS_MIN;
      nbitsmask = MAXCODE(BITS_MIN);
      maxcodep = sp->dec_codetab + nbitsmask;
      NextCode(sp, sp, bp, code, GetNextCodeCompat);

      if (code == CODE_EOI) {
        break;
      }

      *op++ = code, occ--;
      oldcodep = sp->dec_codetab + code;
      continue;
    }

    codep = sp->dec_codetab + code;

    /*
     * Add the new entry to the code table.
     */
    if (free_entp < &sp->dec_codetab[0] ||
        free_entp >= &sp->dec_codetab[CSIZE]) {
      lzw_error("LZWDecodeCompat: Corrupted LZW table at scanline %d", sp->sp_row);
      return (0);
    }

    free_entp->next = oldcodep;

    if (free_entp->next < &sp->dec_codetab[0] ||
        free_entp->next >= &sp->dec_codetab[CSIZE]) {
      lzw_error("LZWDecodeCompat: Corrupted LZW table at scanline %d", sp->sp_row);
      return (0);
    }

    free_entp->firstchar = free_entp->next->firstchar;
    free_entp->length = free_entp->next->length + 1;
    free_entp->value = (codep < free_entp) ?
        codep->firstchar : free_entp->firstchar;

    if (++free_entp > maxcodep) {
      if (++nbits > BITS_MAX) { /* should not happen */
        nbits = BITS_MAX;
      }

      nbitsmask = MAXCODE(nbits);
      maxcodep = sp->dec_codetab + nbitsmask;
    }

    oldcodep = codep;

    if (code >= 256) {
      /*
       * Code maps to a string, copy string
       * value to output (written in reverse).
       */
      if (codep->length == 0) {
        lzw_error("LZWDecodeCompat: Wrong length of decoded string: data probably corrupted at scanline %d", sp->sp_row);
        return (0);
      }

      if (codep->length > occ) {
        /*
         * String is too long for decode buffer,
         * locate portion that will fit, copy to
         * the decode buffer, and setup restart
         * logic for the next decoding call.
         */
        sp->dec_codep = codep;

        do {
          codep = codep->next;
        }
        while (codep->length > occ);

        sp->dec_restart = occ;
        tp = op + occ;

        do  {
          *--tp = codep->value;
          codep = codep->next;
        }
        while (--occ);

        break;
      }

      op += codep->length, occ -= codep->length;
      tp = op;

      do {
        *--tp = codep->value;
      }
      while ((codep = codep->next) != NULL);
    }
    else {
      *op++ = code, occ--;
    }
  }

  sp->lzw_nbits = nbits;
  sp->lzw_nextdata = nextdata;
  sp->lzw_nextbits = nextbits;
  sp->dec_nbitsmask = nbitsmask;
  sp->dec_oldcodep = oldcodep;
  sp->dec_free_entp = free_entp;
  sp->dec_maxcodep = maxcodep;

  if (occ > 0) {
    lzw_error("LZWDecodeCompat: Not enough data at scanline %d (short %d bytes)", (int)sp->sp_row, (int)occ);
    return (0);
  }

  return (1);
}
#endif /* LZW_COMPAT */

/*
 * Reset encoding hash table.
 */
static void cl_hash1(LZWCodecState* sp)
{
  register hash_t* hp = &sp->enc_hashtab[HSIZE - 1];
  register long i = HSIZE - 8;

  do {
    i -= 8;
    hp[-7].hash = -1;
    hp[-6].hash = -1;
    hp[-5].hash = -1;
    hp[-4].hash = -1;
    hp[-3].hash = -1;
    hp[-2].hash = -1;
    hp[-1].hash = -1;
    hp[ 0].hash = -1;
    hp -= 8;
  }
  while (i >= 0);

  for (i += 8; i > 0; i--, hp--) {
    hp->hash = -1;
  }
}

#define CALCRATIO(sp, rat) {          \
    if (incount > 0x007fffff) { /* NB: shift will overflow */\
      rat = outcount >> 8;        \
      rat = (rat == 0 ? 0x7fffffff : incount/rat);  \
    } else              \
      rat = (incount<<8) / outcount;      \
  }
#define PutNextCode(op, c) {          \
    nextdata = (nextdata << nbits) | c;     \
    nextbits += nbits;          \
    *op++ = (uchar)(nextdata >> (nextbits-8));    \
    nextbits -= 8;            \
    if (nextbits >= 8) {          \
      *op++ = (uchar)(nextdata >> (nextbits-8));  \
      nextbits -= 8;          \
    }             \
    outcount += nbits;          \
  }

/*
 * Encode a chunk of pixels.
 *
 * Uses an open addressing double hashing (no chaining) on the
 * prefix code/next character combination.  We do a variant of
 * Knuth's algorithm D (vol. 3, sec. 6.4) along with G. Knott's
 * relatively-prime secondary probe.  Here, the modular division
 * first probe is gives way to a faster exclusive-or manipulation.
 * Also do block compression with an adaptive reset, whereby the
 * code table is cleared when the compression ratio decreases,
 * but after the table fills.  The variable-length output codes
 * are re-sized at this point, and a CODE_CLEAR is generated
 * for the decoder.
 */
static int LZWEncode_impl(LZWCodecState* sp, const uchar* bp, int32 cc, uchar* op)
{
  register long fcode;
  register hash_t* hp;
  register int h, c;
  hcode_t ent;
  long disp;
  long incount, outcount, checkpoint;
  long nextdata, nextbits;
  int free_ent, maxcode, nbits;
  uchar* limit;
  uchar* op0 = op;

  if (sp == NULL) {
    return (0);
  }

  /*
   * Load local state.
   */
  incount = sp->enc_incount;
  outcount = sp->enc_outcount;
  checkpoint = sp->enc_checkpoint;
  nextdata = sp->lzw_nextdata;
  nextbits = sp->lzw_nextbits;
  free_ent = sp->lzw_free_ent;
  maxcode = sp->lzw_maxcode;
  nbits = sp->lzw_nbits;
  limit = sp->enc_rawlimit;
  ent = sp->enc_oldcode;

  if (ent == (hcode_t) - 1 && cc > 0) {
    /*
     * NB: This is safe because it can only happen
     *     at the start of a strip where we know there
     *     is space in the data buffer.
     */
    PutNextCode(op, CODE_CLEAR);
    ent = *bp++;
    cc--;
    incount++;
  }

  while (cc > 0) {
    c = *bp++;
    cc--;
    incount++;
    fcode = ((long)c << BITS_MAX) + ent;
    h = (c << HSHIFT) ^ ent;  /* xor hashing */
#ifdef _WINDOWS

    /*
     * Check hash index for an overflow.
     */
    if (h >= HSIZE) {
      h -= HSIZE;
    }

#endif
    hp = &sp->enc_hashtab[h];

    if (hp->hash == fcode) {
      ent = hp->code;
      continue;
    }

    if (hp->hash >= 0) {
      /*
       * Primary hash failed, check secondary hash.
       */
      disp = HSIZE - h;

      if (h == 0) {
        disp = 1;
      }

      do {
        /*
         * Avoid pointer arithmetic 'cuz of
         * wraparound problems with segments.
         */
        if ((h -= disp) < 0) {
          h += HSIZE;
        }

        hp = &sp->enc_hashtab[h];

        if (hp->hash == fcode) {
          ent = hp->code;
          goto hit;
        }
      }
      while (hp->hash >= 0);
    }

    /*
     * New entry, emit code and add to table.
     */
    /*
     * Verify there is space in the buffer for the code
     * and any potential Clear code that might be emitted
     * below.  The value of limit is setup so that there
     * are at least 4 bytes free--room for 2 codes.
     */
    if (op > limit) {
      //sp->sp_rawcc = (int32)(op - sp->sp_rawdata);
      //TIFFFlushData1(sp);
      op = sp->sp_rawdata;
    }

    PutNextCode(op, ent);
    ent = c;
    hp->code = free_ent++;
    hp->hash = fcode;

    if (free_ent == CODE_MAX - 1) {
      /* table is full, emit clear code and reset */
      cl_hash1(sp);
      sp->enc_ratio = 0;
      incount = 0;
      outcount = 0;
      free_ent = CODE_FIRST;
      PutNextCode(op, CODE_CLEAR);
      nbits = BITS_MIN;
      maxcode = MAXCODE(BITS_MIN);
    }
    else {
      /*
       * If the next entry is going to be too big for
       * the code size, then increase it, if possible.
       */
      if (free_ent > maxcode) {
        nbits++;
        assert(nbits <= BITS_MAX);
        maxcode = (int) MAXCODE(nbits);
      }
      else if (incount >= checkpoint) {
        long rat;
        /*
         * Check compression ratio and, if things seem
         * to be slipping, clear the hash table and
         * reset state.  The compression ratio is a
         * 24+8-bit fractional number.
         */
        checkpoint = incount + CHECK_GAP;
        CALCRATIO(sp, rat);

        if (rat <= sp->enc_ratio) {
          cl_hash1(sp);
          sp->enc_ratio = 0;
          incount = 0;
          outcount = 0;
          free_ent = CODE_FIRST;
          PutNextCode(op, CODE_CLEAR);
          nbits = BITS_MIN;
          maxcode = MAXCODE(BITS_MIN);
        }
        else {
          sp->enc_ratio = rat;
        }
      }
    }

hit:
    ;
  }

  /*
   * Restore global state.
   */
  sp->enc_incount = incount;
  sp->enc_outcount = outcount;
  sp->enc_checkpoint = checkpoint;
  sp->enc_oldcode = ent;
  sp->lzw_nextdata = nextdata;
  sp->lzw_nextbits = nextbits;
  sp->lzw_free_ent = free_ent;
  sp->lzw_maxcode = maxcode;
  sp->lzw_nbits = nbits;
  return op - op0;
}

/*
 * Finish off an encoded strip by flushing the last
 * string and tacking on an End Of Information code.
 */
static int LZWPostEncode(LZWCodecState* sp, uchar* op, int n)
{
  long nextbits = sp->lzw_nextbits;
  long nextdata = sp->lzw_nextdata;
  long outcount = sp->enc_outcount;
  int nbits = sp->lzw_nbits;
  uchar* op0 = op;
  op += n;

  if (sp->enc_oldcode != (hcode_t) - 1) {
    PutNextCode(op, sp->enc_oldcode);
    sp->enc_oldcode = (hcode_t) - 1;
  }

  PutNextCode(op, CODE_EOI);

  if (nextbits > 0) {
    *op++ = (uchar)(nextdata << (8 - nextbits));
  }

  //sp->sp_rawcc = (int32)(op - sp->sp_rawdata);
  return op - op0;
}

/*
 * LZW Encoding.
 */

static int LZWEncode(const uchar* bp, int32 cc, uchar* op)
{
  LZWCodecState sp[1] = {0};
  int n = 0;
  assert(sp != NULL);
  sp->enc_hashtab = (hash_t*) malloc(HSIZE * sizeof(hash_t));

  if (sp->enc_hashtab == NULL) {
    lzw_error("No space for LZW hash table");
    return (0);
  }

  // Reset encoding state at the start of a strip.
  assert(sp != NULL);
  sp->lzw_nbits = BITS_MIN;
  sp->lzw_maxcode = MAXCODE(BITS_MIN);
  sp->lzw_free_ent = CODE_FIRST;
  sp->lzw_nextbits = 0;
  sp->lzw_nextdata = 0;
  sp->enc_checkpoint = CHECK_GAP;
  sp->enc_ratio = 0;
  sp->enc_incount = 0;
  sp->enc_outcount = 0;

  // The 4 here insures there is space for 2 max-sized codes in LZWEncode_impl and LZWPostDecode.
  sp->enc_rawlimit = sp->sp_rawdata + sp->sp_rawdatasize - 1 - 4;
  cl_hash1(sp);    // clear hash table
  sp->enc_oldcode = (hcode_t) - 1; // generates CODE_CLEAR in LZWEncode_impl

  n = LZWEncode_impl(sp, bp, cc, op);
  n = LZWPostEncode(sp, op, n);
  free(sp->enc_hashtab);
  return n;
}

static int LZWDecode(uchar* op0, int32 occ0, const uchar* bp, int sp_rawcc)
{
  LZWCodecState sp[1] = {0};
  int n, code;
  //int sp_rawcc = occ0;

  if (sp->dec_codetab == NULL) {
    sp->dec_codetab = (code_t*)malloc(CSIZE * sizeof(code_t));

    if (sp->dec_codetab == NULL) {
      lzw_error("No space for LZW code table");
      return (0);
    }

    /*
     * Pre-load the table.
     */
    code = 255;

    do {
      sp->dec_codetab[code].value = code;
      sp->dec_codetab[code].firstchar = code;
      sp->dec_codetab[code].length = 1;
      sp->dec_codetab[code].next = NULL;
    }
    while (code--);
  }


  /*
   * Setup state for decoding a strip.
   */
  //static int LZWPreDecode(LZWCodecState* sp)

  assert(sp != NULL);
#if 0

  /*
   * Check for old bit-reversed codes.
   */
  if (sp->sp_rawdata[0] == 0 && (sp->sp_rawdata[1] & 0x1)) {
#ifdef LZW_COMPAT

    if (!sp->dec_decode) {
      lzw_warning("Old-style LZW codes, convert file");
      /*
       * Override default decoding methods with
       * ones that deal with the old coding.
       * Otherwise the predictor versions set
       * above will call the compatibility routines
       * through the dec_decode method.
       */
      sp->sp_decoderow = LZWDecodeCompat;
      sp->sp_decodestrip = LZWDecodeCompat;
      sp->sp_decodetile = LZWDecodeCompat;
      /*
       * If doing horizontal differencing, must
       * re-setup the predictor logic since we
       * switched the basic decoder methods...
       */
      (*sp->sp_setupdecode)(sp);
      sp->dec_decode = LZWDecodeCompat;
    }

    sp->lzw_maxcode = MAXCODE(BITS_MIN);
#else /* !LZW_COMPAT */

    if (!sp->dec_decode) {
      lzw_error(
          "Old-style LZW codes not supported");
      sp->dec_decode = LZWDecode_impl;
    }

    return (0);
#endif/* !LZW_COMPAT */
  }
  else {
    sp->lzw_maxcode = MAXCODE(BITS_MIN) - 1;
    sp->dec_decode = LZWDecode_impl;
  }

#endif

  sp->lzw_nbits = BITS_MIN;
  sp->lzw_nextbits = 0;
  sp->lzw_nextdata = 0;

  sp->dec_restart = 0;
  sp->dec_nbitsmask = MAXCODE(BITS_MIN);
#ifdef LZW_CHECKEOS
  sp->dec_bitsleft = sp_rawcc << 3;
#endif
  sp->dec_free_entp = sp->dec_codetab + CODE_FIRST;
  /*
   * Zero entries that are not yet filled in.  We do
   * this to guard against bogus input data that causes
   * us to index into undefined entries.  If you can
   * come up with a way to safely bounds-check input codes
   * while decoding then you can remove this operation.
  */
  memset(sp->dec_free_entp, 0, (CSIZE - CODE_FIRST)*sizeof(code_t));
  sp->dec_oldcodep = &sp->dec_codetab[-1];
  sp->dec_maxcodep = &sp->dec_codetab[sp->dec_nbitsmask - 1];

  n = LZWDecode_impl(sp, (char*)op0, occ0, bp);
  FREE(sp->dec_codetab);
  return n;
}
#ifdef TEST
static int test_lzw() {
  int i, m, l, n = 1 << 12;
  uchar* p = MALLOC(uchar, n);
  uchar* o = MALLOC(uchar, n);
  uchar* s = MALLOC(uchar, n);
  memset(p, 0, n);
  memset(o, 0, n);
  memset(s, 0, n);

  for (i = 0; i < n; ++i) {
    p[i] = (uchar)(13 * i * i);
    //p[i] = (uchar)(i);
  }

  m = LZWEncode(p, n, o);
  l = LZWDecode(s, n, o, m);

  for (i = 0; i < n; ++i) {
    uchar a;
    a = (uchar)(13 * i * i);
    //a = (uchar)(i);
    ASSERT(a == p[i]);

    if (a != s[i]) {
      int adsf = 0;
    }

    ASSERT(a == s[i]);
  }

  FREE(p);
  FREE(o);
  return 0;
}
#endif // TEST

#endif //_LZW_INL_
