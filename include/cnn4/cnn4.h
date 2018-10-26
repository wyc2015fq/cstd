
#define MAXOBJ 10

enum nnObjType {
  NNT_VAR,
  NNT_FUN,
};

enum nnDataType {
  NNVT_NUL,
  NNVT_INT8,
  NNVT_UINT8,
  NNVT_INT16,
  NNVT_UINT16,
  NNVT_INT32,
  NNVT_UINT32,
  NNVT_FLOAT,
  NNVT_DOUBLE,
};

typedef int(*nnfunc_t)(struct nnNet* net, struct nnLayer* obj);

struct nnBlob {
  int dim[4];
  char* data;
  nnDataType type;
  char name[32];
};

struct nnLayer {
  int i[MAXOBJ];
  int o[MAXOBJ];
  int in;
  int on;
  char name[32];
  nnfunc_t run;
};

struct nnNet {
  struct nnLayer* layers;
  int layers_size;
};
