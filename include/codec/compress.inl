#ifndef __COMPRESS_INL__
#define __COMPRESS_INL__

// ImGui - binary_to_compressed_c.cpp
// Helper tool to turn a file into a C array.
// The data is first compressed with stb_compress() to reduce source code size.
// Then encoded in Base85 to fit in a string so we can fit roughly 4 bytes of compressed data into 5 bytes of source code (suggested by @mmalex)
// (If we used 32-bits constants it would require take 11 bytes of source code to encode 4 bytes.)
// Useful if you want to embed fonts into your code.
// Note that even with compression, the output array is likely to be bigger than the binary file..
// Load compressed TTF fonts with ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF()

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

// stb_compress* from stb.h - declaration
typedef unsigned int stb_uint;
typedef unsigned char stb_uchar;

char Encode85Byte(unsigned int x) 
{
    x = (x % 85) + 35;
    return (x>='\\') ? x+1 : x;
}

// stb_compress* from stb.h - definition

////////////////////           compressor         ///////////////////////

static unsigned int stb_adler32a(unsigned int adler32, unsigned char* buffer, unsigned int buflen)
{
  const unsigned long ADLER_MOD = 65521;
  unsigned long s1 = adler32 & 0xffff, s2 = adler32 >> 16;
  unsigned long blocklen, i;
  blocklen = buflen % 5552;
  while (buflen) {
    for (i = 0; i + 7 < blocklen; i += 8) {
      s1 += buffer[0], s2 += s1;
      s1 += buffer[1], s2 += s1;
      s1 += buffer[2], s2 += s1;
      s1 += buffer[3], s2 += s1;
      s1 += buffer[4], s2 += s1;
      s1 += buffer[5], s2 += s1;
      s1 += buffer[6], s2 += s1;
      s1 += buffer[7], s2 += s1;
      buffer += 8;
    }
    for (; i < blocklen; ++i) {
      s1 += *buffer++, s2 += s1;
    }
    s1 %= ADLER_MOD, s2 %= ADLER_MOD;
    buflen -= blocklen;
    blocklen = 5552;
  }
  return (unsigned int)(s2 << 16) + (unsigned int)s1;
}
static stb_uint stb_adler32(stb_uint adler32, const stb_uchar *buffer, stb_uint buflen)
{
  const unsigned long ADLER_MOD = 65521;
  unsigned long s1 = adler32 & 0xffff, s2 = adler32 >> 16;
  unsigned long blocklen, i;
  
  blocklen = buflen % 5552;
  while (buflen) {
    for (i=0; i + 7 < blocklen; i += 8) {
      s1 += buffer[0], s2 += s1;
      s1 += buffer[1], s2 += s1;
      s1 += buffer[2], s2 += s1;
      s1 += buffer[3], s2 += s1;
      s1 += buffer[4], s2 += s1;
      s1 += buffer[5], s2 += s1;
      s1 += buffer[6], s2 += s1;
      s1 += buffer[7], s2 += s1;
      
      buffer += 8;
    }
    
    for (; i < blocklen; ++i)
      s1 += *buffer++, s2 += s1;
    
    s1 %= ADLER_MOD, s2 %= ADLER_MOD;
    buflen -= blocklen;
    blocklen = 5552;
  }
  return (s2 << 16) + s1;
}

static unsigned int stb_matchlen(const stb_uchar *m1, const stb_uchar *m2, stb_uint maxlen)
{
    stb_uint i;
    for (i=0; i < maxlen; ++i)
        if (m1[i] != m2[i]) return i;
    return i;
}

// simple implementation that just takes the source data in a big block

typedef struct stb_out_t {
stb_uchar *stb__out;
FILE      *stb__outfile;
stb_uint   stb__outbytes;
} stb_out_t;

static int stb__write(stb_out_t* so, unsigned char v)
{
    fputc(v, so->stb__outfile);
    ++so->stb__outbytes;
    return 0;
}

#define stb_out(v)    (so->stb__out ? *so->stb__out++ = (stb_uchar) (v) : stb__write(so, (stb_uchar) (v)))

static void stb_out2(stb_out_t* so, stb_uint v)
{
    stb_out(v >> 8);
    stb_out(v);
}

static void stb_out3(stb_out_t* so, stb_uint v) { stb_out(v >> 16); stb_out(v >> 8); stb_out(v); }
static void stb_out4(stb_out_t* so, stb_uint v) { stb_out(v >> 24); stb_out(v >> 16);
stb_out(v >> 8 ); stb_out(v);                  }

static void outliterals(stb_out_t* so, const stb_uchar *in, int numlit)
{
    while (numlit > 65536) {
        outliterals(so, in,65536);
        in     += 65536;
        numlit -= 65536;
    }

    if      (numlit ==     0)    ;
    else if (numlit <=    32)    stb_out (0x000020 + numlit-1);
    else if (numlit <=  2048)    stb_out2(so, 0x000800 + numlit-1);
    else /*  numlit <= 65536) */ stb_out3(so, 0x070000 + numlit-1);

    if (so->stb__out) {
        memcpy(so->stb__out,in,numlit);
        so->stb__out += numlit;
    } else
        fwrite(in, 1, numlit, so->stb__outfile);
}

static int stb__window = 0x40000; // 256K

static int stb_not_crap(int best, int dist)
{
    return   ((best > 2  &&  dist <= 0x00100)     
        || (best > 5  &&  dist <= 0x04000)
        || (best > 7  &&  dist <= 0x80000));
}

static  stb_uint stb__hashsize = 32768;

// note that you can play with the hashing functions all you
// want without needing to change the decompressor
#define stb__hc(q,h,c)      (((h) << 7) + ((h) >> 25) + q[c])
#define stb__hc2(q,h,c,d)   (((h) << 14) + ((h) >> 18) + (q[c] << 7) + q[d])
#define stb__hc3(q,c,d,e)   ((q[c] << 14) + (q[d] << 7) + q[e])

static unsigned int stb__running_adler;

static int stb_compress_chunk(stb_out_t* so, const stb_uchar *history,
    const stb_uchar *start,
    const stb_uchar *end,
    int length,
    int *pending_literals,
    stb_uchar **chash,
    stb_uint mask)
{
    int window = stb__window;
    stb_uint match_max;
    const stb_uchar *lit_start = start - *pending_literals;
    const stb_uchar *q = start;
    (void)history;

#define STB__SCRAMBLE(h)   (((h) + ((h) >> 16)) & mask)

    // stop short of the end so we don't scan off the end doing
    // the hashing; this means we won't compress the last few bytes
    // unless they were part of something longer
    while (q < start+length && q+12 < end) {
        int m;
        stb_uint h1,h2,h3,h4, h;
        const stb_uchar *t;
        int best = 2, dist=0;

        if (q+65536 > end)
            match_max = end-q;
        else
            match_max = 65536;

#define stb__nc(b,d)  ((d) <= window && ((b) > 9 || stb_not_crap(b,d)))

#define STB__TRY(t,p)  /* avoid retrying a match we already tried */ \
    if (p ? dist != q-t : 1)                             \
    if ((m = stb_matchlen(t, q, match_max)) > best)     \
    if (stb__nc(m,q-(t)))                                \
    best = m, dist = q - (t)

        // rather than search for all matches, only try 4 candidate locations,
        // chosen based on 4 different hash functions of different lengths.
        // this strategy is inspired by LZO; hashing is unrolled here using the
        // 'hc' macro
        h = stb__hc3(q,0, 1, 2); h1 = STB__SCRAMBLE(h);
        t = chash[h1]; if (t) STB__TRY(t,0);
        h = stb__hc2(q,h, 3, 4); h2 = STB__SCRAMBLE(h);
        h = stb__hc2(q,h, 5, 6);        t = chash[h2]; if (t) STB__TRY(t,1);
        h = stb__hc2(q,h, 7, 8); h3 = STB__SCRAMBLE(h);
        h = stb__hc2(q,h, 9,10);        t = chash[h3]; if (t) STB__TRY(t,1);
        h = stb__hc2(q,h,11,12); h4 = STB__SCRAMBLE(h);
        t = chash[h4]; if (t) STB__TRY(t,1);

        // because we use a shared hash table, can only update it
        // _after_ we've probed all of them
        chash[h1] = chash[h2] = chash[h3] = chash[h4] = (uchar*)q;

        if (best > 2)
            assert(dist > 0);

        // see if our best match qualifies
        if (best < 3) { // fast path literals
            ++q;
        } else if (best > 2  &&  best <= 0x80    &&  dist <= 0x100) {
            outliterals(so, lit_start, q-lit_start); lit_start = (q += best);
            stb_out(0x80 + best-1);
            stb_out(dist-1);
        } else if (best > 5  &&  best <= 0x100   &&  dist <= 0x4000) {
            outliterals(so, lit_start, q-lit_start); lit_start = (q += best);
            stb_out2(so, 0x4000 + dist-1);       
            stb_out(best-1);
        } else if (best > 7  &&  best <= 0x100   &&  dist <= 0x80000) {
            outliterals(so, lit_start, q-lit_start); lit_start = (q += best);
            stb_out3(so, 0x180000 + dist-1);     
            stb_out(best-1);
        } else if (best > 8  &&  best <= 0x10000 &&  dist <= 0x80000) {
            outliterals(so, lit_start, q-lit_start); lit_start = (q += best);
            stb_out3(so, 0x100000 + dist-1);     
            stb_out2(so, best-1);
        } else if (best > 9                      &&  dist <= 0x1000000) {
            if (best > 65536) best = 65536;
            outliterals(so, lit_start, q-lit_start); lit_start = (q += best);
            if (best <= 0x100) {
                stb_out(0x06);
                stb_out3(so, dist-1);
                stb_out(best-1);
            } else {
                stb_out(0x04);
                stb_out3(so, dist-1);
                stb_out2(so, best-1);
            }
        } else {  // fallback literals if no match was a balanced tradeoff
            ++q;
        }
    }

    // if we didn't get all the way, add the rest to literals
    if (q-start < length)
        q = start+length;

    // the literals are everything from lit_start to q
    *pending_literals = (q - lit_start);

    stb__running_adler = stb_adler32(stb__running_adler, start, q - start);
    return q - start;
}

static int stb_compress_inner(stb_out_t* so, const stb_uchar *input, stb_uint length)
{
    int literals = 0;
    stb_uint len,i;

    stb_uchar **chash;
    chash = (stb_uchar**) malloc(stb__hashsize * sizeof(stb_uchar*));
    if (chash == NULL) return 0; // failure
    for (i=0; i < stb__hashsize; ++i)
        chash[i] = NULL;

    // stream signature
    stb_out(0x57); stb_out(0xbc);
    stb_out2(so, 0);
    stb_out4(so, 0);       // 64-bit length requires 32-bit leading 0
    stb_out4(so, length);
    stb_out4(so, stb__window);

    stb__running_adler = 1;

    len = stb_compress_chunk(so, input, input, input+length, length, &literals, chash, stb__hashsize-1);
    assert(len == length);
    outliterals(so, input+length - literals, literals);
    free(chash);
    stb_out2(so, 0x05fa); // end opcode
    stb_out4(so, stb__running_adler);
    return 1; // success
}

stb_uint stb_compress(void *out, const void* input, int length)
{
  stb_out_t so[1] = {0};
  uchar* uout = (uchar*)out;
  so->stb__out = uout;
  so->stb__outfile = NULL;
  stb_compress_inner(so, (uchar*)input, length);
  return so->stb__out - uout;
}

bool binary_to_compressed_c(const char* filename, FILE* out, const char* symbol, bool use_base85_encoding, bool use_compression)
{
  char* compressed = NULL;
  char* data;
  const char* compressed_str = NULL;
  int data_sz, maxlen, compressed_sz;
  // Read file
  FILE* f = fopen(filename, "rb");
  if (!f) return false;
  if (fseek(f, 0, SEEK_END) || (data_sz = (int)ftell(f)) == -1 || fseek(f, 0, SEEK_SET)) { fclose(f); return false; }
  data = (char*)pmalloc(data_sz+4);
  if (fread(data, 1, data_sz, f) != (size_t)data_sz) {
    fclose(f); pfree(data);
    return false;
  }
  memset((void *)(((char*)data) + data_sz), 0, 4);
  fclose(f);
  
  // Compress
  maxlen = data_sz + 512 + (data_sz >> 2) + sizeof(int); // total guess
  compressed = use_compression ? (char*)pmalloc(maxlen) : data;
  compressed_sz = use_compression ? stb_compress((stb_uchar*)compressed, (stb_uchar*)data, data_sz) : data_sz;
  if (use_compression)
    memset(compressed + compressed_sz, 0, maxlen - compressed_sz);
  
  // Output as Base85 encoded
  fprintf(out, "// File: '%s' (%d bytes)\n", filename, (int)data_sz);
  fprintf(out, "// Exported using binary_to_compressed_c.cpp\n");
  compressed_str = use_compression ? "compressed_" : "";
  if (use_base85_encoding)
  {
    int src_i;
    char prev_c = 0;
    fprintf(out, "static const char %s_%sdata_base85[%d+1] =\n    \"", symbol, compressed_str, (int)((compressed_sz+3)/4)*5);
    for (src_i = 0; src_i < compressed_sz; src_i += 4)
    {
      // This is made a little more complicated by the fact that ??X sequences are interpreted as trigraphs by old C/C++ compilers. So we need to escape pairs of ??.
      unsigned int n5, d = *(unsigned int*)(compressed + src_i);
      for (n5 = 0; n5 < 5; n5++, d /= 85)
      {
        char c = Encode85Byte(d);
        fprintf(out, (c == '?' && prev_c == '?') ? "\\%c" : "%c", c);
        prev_c = c;
      }
      if ((src_i % 112) == 112-4)
        fprintf(out, "\"\n    \"");
    }
    fprintf(out, "\";\n\n");
  }
  else
  {
    int i, column = 0;
    fprintf(out, "static const unsigned int %s_%ssize = %d;\n", symbol, compressed_str, (int)compressed_sz);
    fprintf(out, "static const unsigned int %s_%sdata[%d/4] =\n{", symbol, compressed_str, (int)((compressed_sz+3)/4)*4);
    for (i = 0; i < compressed_sz; i += 4)
    {
      unsigned int d = *(unsigned int*)(compressed + i);
      if ((column++ % 12) == 0)
        fprintf(out, "\n    0x%08x, ", d);
      else
        fprintf(out, "0x%08x, ", d);
    }
    fprintf(out, "\n};\n\n");
  }
  
  // Cleanup
  pfree(data);
  if (use_compression)
    pfree(compressed);
  fflush(out);
  return true;
}
static unsigned int stb_decompress_length(void* in0)
{
  unsigned char* input = (unsigned char*)in0;
  return (input[8] << 24) + (input[9] << 16) + (input[10] << 8) + input[11];
}
static unsigned char* stb__barrier, *stb__barrier2, *stb__barrier3, *stb__barrier4;
static unsigned char* stb__dout;
static void stb__match(unsigned char* data, unsigned int length)
{
  // INVERSE of memmove... write each byte before copying the next...
  ASSERT(stb__dout + length <= stb__barrier);
  if (stb__dout + length > stb__barrier) {
    stb__dout += length;
    return;
  }
  if (data < stb__barrier4) {
    stb__dout = stb__barrier + 1;
    return;
  }
  while (length--) {
    *stb__dout++ = *data++;
  }
}
static void stb__lit(unsigned char* data, unsigned int length)
{
  ASSERT(stb__dout + length <= stb__barrier);
  if (stb__dout + length > stb__barrier) {
    stb__dout += length;
    return;
  }
  if (data < stb__barrier2) {
    stb__dout = stb__barrier + 1;
    return;
  }
  memcpy(stb__dout, data, length);
  stb__dout += length;
}
#define stb__in2(x) ((i[x] << 8) + i[(x)+1])
#define stb__in3(x) ((i[x] << 16) + stb__in2((x)+1))
#define stb__in4(x) ((i[x] << 24) + stb__in3((x)+1))
static unsigned char* stb_decompress_token(unsigned char* i)
{
  if (*i >= 0x20) { // use fewer if's for cases that expand small
    if (*i >= 0x80) {
      stb__match(stb__dout - i[1] - 1, i[0] - 0x80 + 1), i += 2;
    }
    else if (*i >= 0x40) {
      stb__match(stb__dout - (stb__in2(0) - 0x4000 + 1), i[2] + 1), i += 3;
    }
    else { /* *i >= 0x20 */
      stb__lit(i + 1, i[0] - 0x20 + 1), i += 1 + (i[0] - 0x20 + 1);
    }
  }
  else { // more ifs for cases that expand large, since overhead is amortized
    if (*i >= 0x18) {
      stb__match(stb__dout - (stb__in3(0) - 0x180000 + 1), i[3] + 1), i += 4;
    }
    else if (*i >= 0x10) {
      stb__match(stb__dout - (stb__in3(0) - 0x100000 + 1), stb__in2(3) + 1), i += 5;
    }
    else if (*i >= 0x08) {
      stb__lit(i + 2, stb__in2(0) - 0x0800 + 1), i += 2 + (stb__in2(0) - 0x0800 + 1);
    }
    else if (*i == 0x07) {
      stb__lit(i + 3, stb__in2(1) + 1), i += 3 + (stb__in2(1) + 1);
    }
    else if (*i == 0x06) {
      stb__match(stb__dout - (stb__in3(1) + 1), i[4] + 1), i += 5;
    }
    else if (*i == 0x04) {
      stb__match(stb__dout - (stb__in3(1) + 1), stb__in2(4) + 1), i += 6;
    }
  }
  return i;
}
static unsigned int stb_decompress(void* out0, const void* in0, unsigned int length)
{
  unsigned char* output = (unsigned char*)out0;
  unsigned char* i = (unsigned char*)in0;
  unsigned int olen;
  if (stb__in4(0) != 0x57bC0000) {
    return 0;
  }
  if (stb__in4(4) != 0) {
    return 0; // error! stream is > 4GB
  }
  olen = stb_decompress_length(i);
  stb__barrier2 = i;
  stb__barrier3 = i + length;
  stb__barrier = output + olen;
  stb__barrier4 = output;
  i += 16;
  stb__dout = output;
  for (;;) {
    unsigned char* old_i = i;
    i = stb_decompress_token(i);
    if (i == old_i) {
      if (*i == 0x05 && i[1] == 0xfa) {
        ASSERT(stb__dout == output + olen);
        if (stb__dout != output + olen) {
          return 0;
        }
        if (stb_adler32(1, output, olen) != (unsigned int) stb__in4(2)) {
          return 0;
        }
        return olen;
      }
      else {
        //ASSERT(0); /* NOTREACHED */
        return 0;
      }
    }
    ASSERT(stb__dout <= output + olen);
    if (stb__dout > output + olen) {
      return 0;
    }
  }
}

int test_binary_to_compressed_c(int argc, char** argv)
{
  int argn = 1;
  bool use_base85_encoding = false;
  bool use_compression = true;
  if (argc < 3)
  {
    printf("Syntax: %s [-base85] [-nocompress] <inputfile> <symbolname>\n", argv[0]);
    return 0;
  }
  
  if (argv[argn][0] == '-')
  {
    if (strcmp(argv[argn], "-base85") == 0) { use_base85_encoding = true; argn++; }
    else if (strcmp(argv[argn], "-nocompress") == 0) { use_compression = false; argn++; }
    else
    {
      printf("Unknown argument: '%s'\n", argv[argn]);
      return 1;
    }
  }
  
  binary_to_compressed_c(argv[argn], stdout, argv[argn+1], use_base85_encoding, use_compression);
  return 1;
}

#include "cfile.h"
static const char compress_header[16] = "comp01";

int compress_load(const char* fname, buf_t* bf) {
  buf_t bf1[1] = {0};
  if (buf_load(fname, bf1)) {
    //stb_compress
    if (0==memcmp(bf1->data, compress_header, sizeof(compress_header))) {
      int len = stb_decompress_length(bf1->data+sizeof(compress_header));
      bfsetsize(bf, len+10);
      bf->len = stb_decompress(bf->data, bf1->data+sizeof(compress_header), bf1->len-sizeof(compress_header));
      bffree(bf1);
      return bf->len;
    } else {
      printf("compress_load: file %s is not %s format!\n", fname, compress_header);
    }
  }
  return 0;
}
int compress_save(const char* fname, const void* data, int len) {
  char* p = NULL;
  MYREALLOC(p, len*2);
  if (p) {
    int len1 = stb_compress(p+sizeof(compress_header), data, len);
    memcpy(p, compress_header, sizeof(compress_header));
    savedata_11(fname, p, len1+sizeof(compress_header), 0);
    FREE(p);
  }
  return 0;
}

#endif // __COMPRESS_INL__
