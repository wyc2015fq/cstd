#ifndef _CAPOPENNI_INL_
#define _CAPOPENNI_INL_
#include "OpenNI.h"
using namespace openni;
#pragma comment(lib, "OpenNI2.lib")
#include "cstd.h"
void CheckOpenNIError(Status result, const char* status)
{
  if (result != STATUS_OK) {
    printf(" %s: %s\n", status, OpenNI::getExtendedError());
  }
}
void openni_uninit(void)
{
  OpenNI::shutdown();
}
int openni_init()
{
  Status result = STATUS_OK;
  static int inited = 0;
  if (0 == inited) {
    result = OpenNI::initialize();
    inited = result == STATUS_OK;
    atexit(openni_uninit);
  }
  return result == STATUS_OK;
}
int openni_capnum()
{
  if (openni_init()) {
    Array<DeviceInfo> di;
    OpenNI::enumerateDevices(&di);
    return di.getSize();
  }
  return 0;
}
class capopenni_class
{
public:
  Device device;
  VideoFrameRef oniDepthImg;
  VideoStream oniDepthStream;
  VideoFrameRef oniColorImg;
  VideoStream oniColorStream;
};
#define _WW 640
#define _HH 480
int openni_open(capopenni* s0, int k)
{
  capopenni* s = NULL;
  if (NULL==s0) {
    return caperoad_capnum();
  }
  s = pmalloc(sizeof(capopenni));
  s0->x = s;
  if (openni_init()) {
    Status result = STATUS_OK;
    capopenni_class* x;
    Array<DeviceInfo> di;
    OpenNI::enumerateDevices(&di);
    char buf[256];
    if (k >= di.getSize()) {
      return 0;
    }
    x = new capopenni_class;
    s->x = x;
    result = x->device.open(di[k].getUri());
    // create depth stream
    result = x->oniDepthStream.create(x->device, openni::SENSOR_DEPTH);
    _snprintf(buf, 256, "oniDepthStream[%d].create", k);
    CheckOpenNIError(result, buf);
    VideoMode modeDepth;
    modeDepth.setResolution(_WW, _HH);
    modeDepth.setFps(30);
    modeDepth.setPixelFormat(PIXEL_FORMAT_DEPTH_1_MM);
    x->oniDepthStream.setVideoMode(modeDepth);
    // start depth stream
    result = x->oniDepthStream.start();
    _snprintf(buf, 256, "oniDepthStream[%d].start", k);
    CheckOpenNIError(result, buf);
    // create color stream
    result = x->oniColorStream.create(x->device, openni::SENSOR_COLOR);
    _snprintf(buf, 256, "oniColorStream[%d].create", k);
    CheckOpenNIError(result, buf);
    VideoMode modeColor;
    modeColor.setResolution(_WW, _HH);
    modeColor.setFps(30);
    modeColor.setPixelFormat(PIXEL_FORMAT_RGB888);
    x->oniColorStream.setVideoMode(modeColor);
    //¡¾4¡¿
    // set depth and color imge registration mode
    if (x->device.isImageRegistrationModeSupported(IMAGE_REGISTRATION_DEPTH_TO_COLOR)) {
      x->device.setImageRegistrationMode(IMAGE_REGISTRATION_DEPTH_TO_COLOR);
    }
    // start color stream
    result = x->oniColorStream.start();
    _snprintf(buf, 256, "oniColorStream[%d].start", k);
    CheckOpenNIError(result, buf);
    return result == STATUS_OK;
  }
  return false;
}
int openni_close(capopenni* s)
{
  capopenni_class* x = (capopenni_class*)s->x;
  x->oniDepthStream.destroy();
  x->oniColorStream.destroy();
  x->device.close();
  return 0;
}
int swaprb(int h, int w, unsigned char* A, int al, int ai)
{
  int i, j;
  for (i = 0; i < h; ++i) {
    unsigned char* A1 = A + i * al;
    for (j = 0; j < w; ++j) {
      int t = A1[0];
      A1[0] = A1[2], A1[2] = t;
      A1 += ai;
    }
  }
  return 0;
}
int imswaprb(img_t* im)
{
  return swaprb(im->height, im->width, im->data, im->step, im->cn);
}
int openni_getframe(capopenni* s, img_t* im)
{
  capopenni_class* x = (capopenni_class*)s->x;
  if (x->oniColorStream.readFrame(&x->oniColorImg) == STATUS_OK) {
    int h = x->oniColorImg.getHeight(), w = x->oniColorImg.getWidth();
    imsetsize(im, h, w, 3, 1);
    memcpy(im->data, x->oniColorImg.getData(), im->height * im->step);
    imswaprb(im);
    return true;
  }
  return false;
}
int openni_getframe_depth(capopenni* s, img_t* im)
{
  capopenni_class* x = (capopenni_class*)s->x;
  if (x->oniDepthStream.readFrame(&x->oniDepthImg) == STATUS_OK) {
    int i, h = x->oniDepthImg.getHeight(), w = x->oniDepthImg.getWidth();
    int mv = x->oniDepthStream.getMaxPixelValue();
    unsigned short* pDep = (unsigned short*)x->oniDepthImg.getData();
    imsetsize(im, h, w, 3, 1);
    //memcpy(im->data, x->oniDepthImg.getData(), im->height*im->step);
    for (i = 0; i < h * w; ++i) {
      im->data[3 * i] = (unsigned char)(pDep[i] * 255 / (mv));
      im->data[3 * i + 1] = im->data[3 * i];
      im->data[3 * i + 2] = im->data[3 * i];
    }
    return true;
  }
  return false;
}
int test_capopenni()
{
  capopenni s[1] = {0};
  int tt = openni_capnum();
  int t = openni_open(s, 0);
  return 0;
}
#endif // _CAPOPENNI_INL_

