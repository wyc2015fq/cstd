
static int str_getstr(char* s, char** ps, char* str, int len, const char* def) {
  int i=0, j=0;
  len--;
  for (; s[i] && !isalnum(s[i]); ++i) {}
  for (; j<len && isalnum(s[i]); ++i) {
    str[j++] = s[i];
  }
  str[j] = 0;
  if (0==j && def) {
    strncpy(str, def, len+1);
  }
  for (; isalnum(s[i]); ++i) {}
  *ps = s+i;
  return j>0;
}
static int str_getint(const char* s, char** ps, int* pint, int def) {
  int i=0, ret=0;
  char* s1 = (char*)s;
  for (; s[i] && !isdigit(s[i]); ++i) {}
  if (isdigit(s[i])) {
    def = strtol(s+i, &s1, 10);
    ret = 1;
  }
  *pint = def;
  *ps = s1;
  return ret;
}
static int str_getdouble(const char* s, char** ps, double* pd, double def) {
  int i=0, ret=0;
  char* s1 = (char*)s;
  for (; s[i] && !isdigit(s[i]); ++i) {}
  if (isdigit(s[i])) {
    def = strtod(s+i, &s1);
    ret = 1;
  }
  *pd = def;
  *ps = s1;
  return ret;
}
static int str_getfloat(const char* s, char** ps, float* pf, float def) {
  double d = *pf;
  int ret = str_getdouble(s, ps, &d, def);
  *pf = d;
  return ret;
}

#define MAX_BLOB 16
#define MAX_NAME 128

typedef struct Layer {
  // one input and one output blob
  bool one_blob_only;

  // support inplace inference
  bool support_inplace;

  // layer type name
  int index;
  // layer name
  char name[MAX_NAME];
  // blob index which this layer needs as input
  int bottoms[MAX_BLOB];
  int bottoms_size;
  // blob index which this layer produces as output
  int tops[MAX_BLOB];
  int tops_size;
  size_t x[256];
} Layer;

struct layer_registry_entry {
  // layer type name
  const char* name;
  // layer factory entry
  // layer factory function
  int (*load_param)(Layer* layer, const char* param);
  int (*forward)(buf_t* bf, const Layer* layer, const img_t* bottom_blobs, img_t* top_blobs);
  //int (*forward_inplace)(const Layer* layer, const img_t* bottom_blobs, img_t* top_blobs);
};

#include "layer/Input.inl"
#include "layer/absval.inl"
#include "layer/ArgMax.inl"
#include "layer/Convolution.inl"
#include "layer/pooling.inl"
#include "layer/innerproduct.inl"
#include "layer/relu.inl"
#include "layer/split.inl"
#include "layer/Concat.inl"
#include "layer/Dropout.inl"

#if 0
#include "layer/BatchNorm.inl"
#endif

#define LAYER_DEFDEF(DEF) \
LAYER_REGISTRY(Input)   \
LAYER_REGISTRY(AbsVal)  \
LAYER_REGISTRY(ArgMax)  \
LAYER_REGISTRY(Convolution)  \
LAYER_REGISTRY(Pooling)  \
LAYER_REGISTRY(ReLU)  \
LAYER_REGISTRY(Split)  \
LAYER_REGISTRY(Concat)  \
LAYER_REGISTRY(Dropout)  \

static const struct layer_registry_entry layer_registry[] = {
#define LAYER_REGISTRY(NAME)  { #NAME, NAME ## _load_param, NAME ## _forward },
  LAYER_DEFDEF(LAYER_REGISTRY)
#undef LAYER_REGISTRY
};
enum { layer_registry_entry_count = countof(layer_registry) };
