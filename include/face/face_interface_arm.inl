
#include "face.inl"
#include "dsp_mem.inl"

void face_recog_close() {}
void face_recog_open() {}
int face_recog_process( void* p0) {
  facerecog_process(p0);
  return 0;
}
