
#include "xtypes.h"

typedef real (*dist_t)(void* a, void* b, void* userdata);

typedef struct findnn_param_t {
  unsigned int datalen;
  unsigned int datastep; // 非0表示数据是1d
  void* data; // 1d或2d数据
  void* sample;
  dist_t distfun;
  void* userdata;
  real mindist;
  int minid;
} findnn_param_t;

int findnn_process(findnn_param_t* p0) {
  findnn_param_t* p = (findnn_param_t*)p0;
  unsigned int i;
  real d;
  p->mindist=1<<30;
  p->minid = -1;
  if (p->datastep) {
    unsigned char* data = (unsigned char*)(p->data);
    for (i=0; i<p->datalen; ++i, data+=p->datastep) {
      real d;
      d = p->distfun(p->sample, data, p->userdata);
      if (d < p->mindist) {
        p->mindist = d;
        p->minid = i;
      }
    }
  } else {
    void** data = (void**)(p->data);
    for (i=0; i<p->datalen; ++i) {
      d = p->distfun(p->sample, data[i], p->userdata);
      if (d < p->mindist) {
        p->mindist = d;
        p->minid = i;
      }
    }
  }
  return 0;
}

int main() {
  return 0;
}
