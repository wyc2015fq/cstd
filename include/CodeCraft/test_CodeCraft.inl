
#include "cstd.h"

typedef struct topo {
int LinkID,SourceID,DestinationID,Cost;
} topo;

int load_topo(const char* fn, topo* top, int maxtop) {
  FILE* pf = fopen(fn, "rb");
  int n=0;
  if (pf) {
    char buf[256];
    for (;fgets(buf, 256, pf)>0;) {
      ++n;
      if (top) {
        sscanf(buf, "%d,%d,%d,%d", &top->LinkID,&top->SourceID,&top->DestinationID,&top->Cost);
        ++top;
        if (n>=maxtop) {
          break;
        }
      }
    }
  }
  return n;
}

int topomat_mul(int n, const int* A, const int* B, int* C, int* D) {
  return 0;
}

int test_CodeCraft() {
  const char* path = NULL;
  const char* topofn;
  const char* demandfn;
  topo* top = NULL;
  int ntop=0;

  topofn = "topo0.csv";
  demandfn = "demand0.csv";
  path = "E:\\code\\proj\\HUAWEI Code Craft 2016 复赛赛题包\\case0";
  _chdir(path);

  ntop = load_topo(topofn, NULL, 0);
  MYREALLOC(top,ntop);
  load_topo(topofn, top, ntop);
  return 0;
}
