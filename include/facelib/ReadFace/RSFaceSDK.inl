
#include "RSFaceSDK.h"
#pragma comment(lib,"libReadFace.lib")

#if 0

int RSFaceSDK_face_detection(const img_t* im, XRECT* out) {
  static RSHandle hEngine = NULL;
  static RSHandle handle = NULL;
  rs_face*	FaceRes = NULL;
  int i, nRet, nface = 0;
  if (NULL==hEngine) {
    char *APPID = "291e0e83138767cfbeadac20c2148ee4";
    char *SDKKey = "718b6abf62113aaf80bbcfdfc417dd8225ecf6c2";
    rsInitLicenseManager(&hEngine, APPID, SDKKey);
    rsInitFaceDetect(&handle, hEngine);
  }
  nRet = rsRunFaceDetect(handle, im->data, PIX_FORMAT_BGR888, im->w, im->h, im->s,
    RS_IMG_CLOCKWISE_ROTATE_0, &FaceRes, &nface);
  if (nRet>=0) {
    for (i = 0; i < nface; ++i) {
      out[i].x = FaceRes[i].rect.left;
      out[i].y = FaceRes[i].rect.top;
      out[i].w = FaceRes[i].rect.width;
      out[i].h = FaceRes[i].rect.height;
    }
  }
  return nface;
}

#endif