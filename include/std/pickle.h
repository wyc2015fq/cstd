
#include "types_c.h"

char* EncodeVarint32(char* dst, uint32_t v) {
  // Operate on characters as unsigneds unsigned
  char* ptr = dst;
  static const int B = 128;
  if (v < (1<<7)) {
   *(ptr++) = v;
  } else if (v < (1<<14)) {
   *(ptr++) = v | B;
    *(ptr++) = v>>7;
  } else if (v < (1<<21)) {
   *(ptr++) = v | B;
    *(ptr++) = (v>>7) | B;
    *(ptr++) = v>>14;
  } else if (v < (1<<28)) {
   *(ptr++) = v | B;
    *(ptr++) = (v>>7) | B;
    *(ptr++) = (v>>14) | B;
    *(ptr++) = v>>21;
  } else {
    *(ptr++) = v | B;
    *(ptr++) = (v>>7) | B;
    *(ptr++) = (v>>14) | B;
    *(ptr++) = (v>>21) | B;
    *(ptr++) = v>>28;
  }
  return (ptr);
}
static const char* GetVarint32Ptr(const char* p, const char* limit, uint32_t* value) {
  if (p < limit) {
    uchar result;
    result = *(p);
    if ((result & 128) == 0) {
      *value = result;
      return p+1;
    }
  }
  uint32_t result = 0;
  for (uint32_t shift = 0; shift <= 28 && p < limit; shift += 7) {
    uchar byte = *(p++);
    if (byte & 128) { // More bytes are present
      result |= ((byte & 127) << shift);
    }
    else {
      result |= (byte << shift);
      *value = result;
      return (p);
    }
  }
  return NULL;
}
char* EncodeVarint64(char* dst, uint64_t v) {
  static const int B = 128;
  uchar* ptr = (uchar*)(dst);
  while (v >= B) {
    *(ptr++) = (v & (B - 1)) | B;
    v >>= 7;
  }
  *(ptr++) = (char)(v);
  return (char*)(ptr);
}
const char* GetVarint64Ptr(const char* p, const char* limit, uint64_t* value) {
  uint64_t result = 0;
  for (uint32_t shift = 0; shift <= 63 && p < limit; shift += 7) {
    uint64_t byte = (uchar)*(p++);
    if (byte & 128) { // More bytes are present
      result |= ((byte & 127) << shift);
    }
    else {
      result |= (byte << shift);
      *value = result;
      return (p);
    }
  } return NULL;
}


#include "rand_c.h"

int hist_flaot32(int* hist, float f) {
  suf32_t ss;
  ss.f = f;
  int i;
  for (i = 0; i < 32; ++i) {
    int k = (1 << i) & ss.u;
    hist[i] += k ? 1 : -1;
  }
  return 0;
}

int test_flaot() {
  rng_t* r = mt_static();
  int i = 0;
  int hist[32] = {0};
  for (i = 0; i < 10000; ++i) {
    float f = rng_normal(r, 0, 100);
    hist_flaot32(hist, f);
  }
  return 0;
}

int test_pickle() {
  test_flaot();
  if (1) {
    uint32_t i, j;
    char buf[16];
    for (i = 1; i <= UINT_MAX / 3; i *= 3) {
      const char* p = EncodeVarint32(buf, i);
      const char* q = GetVarint32Ptr(buf, p, &j);
      printf("%d %d\n", i, p - buf);
      assert(q);
      assert(i == j);
    }
  }
  if (1) {
    uint64_t i, j;
    char buf[8];
    for (i = 3486784401; i <= ULLONG_MAX / 3; i *= 3) {
      const char* p = EncodeVarint64(buf, i);
      const char* q = GetVarint64Ptr(buf, p, &j);
      printf("%I64d %I64d %d\n", i, j, p - buf);
      assert(q);
      assert(i == j);
    }
  }
  return 0;
}