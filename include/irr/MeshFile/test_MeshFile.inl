
#include "MFMD2.h"
#include "MF3DS.h"

int test_MeshFile() {
  stream_t s[1];
  CAnimatedMeshMD2 m[1] = {0};
  _chdir("D:/media");
  
  if (1) {
    fstream_init(s, fopen("room.3ds", "rb"));
    MFMD2_loadFile(s, m);
  }
  if (0) {
    fstream_init(s, fopen("sydney.md2", "rb"));
    MFMD2_loadFile(s, m);
  }
  return 0;
}
