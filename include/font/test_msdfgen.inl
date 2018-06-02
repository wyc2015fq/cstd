//#pragma warning(disable: 4244)
#pragma warning(disable: 4541)
#include "img/imgio.inl"
#include ".\msdfgen.inl"
#define LARGE_VALUE 1e240
#define ABORT(msg) { puts(msg); return 0; }
int savePng(const img_t* bitmap, img_t* pixels)
{
  int x, y, w = bitmap->w, h = bitmap->h;
  int cn = bitmap->c / sizeof(float);
  uchar* it;
  imsetsize(pixels, h, w, cn, 1);
  it = pixels->tt.data;
  for (y = h - 1; y >= 0; --y) {
    float* bitmap1 = (float*)(bitmap->tt.data + bitmap->s * y);
    for (x = 0; x < w * cn; ++x) {
      int v = (int)(bitmap1[x] * 0x100);
      *it++ = BOUND(v, 0, 0xff);
    }
  }
  return 0;
}
#include "ui/window.inl"
#if 1
#define STB_TRUETYPE_IMPLEMENTATION // force following include to generate implementation
#include "stb/stb_truetype.h"
int ttf_toShape(Shape* shape, void* stbVertex, int verCount)
{
  int nIdx;
  DPOINT current;
  DPOINT previous;
  DPOINT contrl;
  Contour* contour = NULL;
  EdgeColor color = WHITE;
  for (nIdx = 0; nIdx < verCount; nIdx++) {
    stbtt_vertex* stbVer = ((stbtt_vertex*)stbVertex + nIdx);
    switch (stbVer->type) {
    case STBTT_vmove:
      current.x = stbVer->x;
      current.y = stbVer->y;
      contour = (Contour*)myAllocPush(shape->contours, shape->len, 1);
      break;
    case STBTT_vline:
      previous = current;
      current.x = stbVer->x;
      current.y = stbVer->y;
      ASSERT(contour);
      Contour_addLine(contour, previous, current, color);
      break;
    case STBTT_vcurve:
      previous = current;
      contrl.x = stbVer->cx;
      contrl.y = stbVer->cy;
      current.x = stbVer->x;
      current.y = stbVer->y;
      //insert the middle point
      Contour_addQuadratic(contour, previous, contrl, current, color);
      break;
    default:
      break;
    }
  }
  return shape->len;
}
#endif
int test_msdfgen()
{
  bool skipColoring = false;
  enum {
    RANGE_UNIT,
    RANGE_PX
  } rangeMode = RANGE_PX;
  Shape shape[1] = {0};
  DPOINT scale = dPOINT(1, 1);
  DPOINT translate = dPOINT(0, 0);
  bool autoFrame = true;
  int width = 32, height = width;
  int outcx = 128;
  int testWidthM = outcx, testHeightM = outcx;
  int testWidth = outcx, testHeight = outcx;
  bool scaleSpecified = false;
  double range = 1;
  double pxRange = 2;
  double avgScale;
  DRECT bounds = {LARGE_VALUE, -LARGE_VALUE, -LARGE_VALUE, LARGE_VALUE};
  if (0) {
    char* defineshape = "{ 1471,0; 1149,0; 1021,333; 435,333; 314,0; 0,0; 571,1466; 884,1466; # }{ 926,580; 724,1124; 526,580; # }";
    if (!readShapeDescription(defineshape, shape, &skipColoring)) {
      printf("Parse error in shape description.");
      return 0;
    }
  }
  if (1) {
    stbtt_fontinfo font[1] = {0};
    const char* fontfile = 0;
    unsigned char* ttf_buffer = NULL;
    int len = 0;
    fontfile = "c:/windows/fonts/simfang.ttf";
    len = loaddata(fontfile, &ttf_buffer);
    stbtt_InitFont(font, ttf_buffer, stbtt_GetFontOffsetForIndex(ttf_buffer, 0));
    if (1) {
      stbtt_vertex* stbVertex = NULL;
      int verCount = 0;
      ushort c;
      c = *L"ý“";
      c = *L"Îª";
      c = *L"ÎÒ";
      verCount = stbtt_GetCodepointShape(font, c, &stbVertex);
      ttf_toShape(shape, stbVertex, verCount);
      STBTT_free(stbVertex, 0);
    }
  }
  avgScale = .5 * (scale.x + scale.y);
  Shape_bounds(shape, &bounds);
  // Auto-frame
  if (autoFrame) {
    double l = bounds.l, b = bounds.b, r = bounds.r, t = bounds.t;
    DPOINT dims, frame = dPOINT(width, height);
    if (rangeMode == RANGE_UNIT) {
      l -= range, b -= range, r += range, t += range;
    }
    else if (!scaleSpecified) {
      frame.x -= 2 * pxRange;
      frame.y -= 2 * pxRange;
    }
    if (l >= r || b >= t) {
      l = 0, b = 0, r = 1, t = 1;
    }
    if (frame.x <= 0 || frame.y <= 0) {
      ABORT("Cannot fit the specified pixel range.");
    }
    dims = dPOINT(r - l, t - b);
    if (scaleSpecified) {
      translate = dPOINT(.5 * (frame.x / scale.x - dims.x) - l, .5 * (frame.y / scale.y - dims.y) - b);
    }
    else {
      if (dims.x * frame.y < dims.y * frame.x) {
        translate = dPOINT(.5 * (frame.x / frame.y * dims.y - dims.x) - l, -b);
        scale.x = scale.y = avgScale = frame.y / dims.y;
      }
      else {
        translate = dPOINT(-l, .5 * (frame.y / frame.x * dims.x - dims.y) - b);
        scale.x = scale.y = avgScale = frame.x / dims.x;
      }
    }
    if (!scaleSpecified) {
      translate.x += pxRange / scale.x, translate.y += pxRange / scale.y;
    }
  }
  if (rangeMode == RANGE_PX) {
    range = pxRange / min(scale.x, scale.y);
  }
  if (1) {
    int i = 0;
    img_t im[10] = {0};
    img_t* sdf = im + i++;
    img_t* msdf = im + i++;
    img_t* render = im + i++;
    img_t* renderMulti = im + i++;
    img_t* output = im + i++;
    img_t* testRenderMulti = im + i++;
    img_t* testRender = im + i++;
    imsetsize(sdf, height, width, sizeof(float), 1);
    imsetsize(msdf, height, width, sizeof(FloatRGB), 1);
    imsetsize(render, testHeight, testWidth, sizeof(float), 1);
    imsetsize(renderMulti, testHeightM, testWidthM, sizeof(FloatRGB), 1);
    if (1) {
      generateSDF(sdf, shape, range, scale, translate, true);
      savePng(sdf, output);
      simulate8bit(sdf);
      renderSDFRGB(renderMulti, sdf, avgScale * range);
      savePng(renderMulti, testRenderMulti);
      renderSDF(render, sdf, avgScale * range);
      savePng(render, testRender);
      imshowfmt(sdf, PF_32bppGrayFloat);
      imshowfmt(renderMulti, PF_32bppGrayFloat);
      imshow(output);
      imshow(testRender);
      imshow(testRenderMulti);
      cvWaitKey(-1);
    }
    if (1) {
      double angleThreshold = 3;
      uint64 coloringSeed = 0;
      double edgeThreshold = 1.00000001;
      const char* edgeAssignment = NULL;
      if (!skipColoring) {
        edgeColoringSimple(shape, angleThreshold, coloringSeed);
      }
      //if (edgeAssignment) parseColoring(shape, edgeAssignment);
      generateMSDF(msdf, shape, range, scale, translate, edgeThreshold, true);
      savePng(msdf, output);
      simulate8bitRGB(msdf);
      renderMSDFRGB(renderMulti, msdf, avgScale * range);
      savePng(renderMulti, testRenderMulti);
      renderMSDF(render, msdf, avgScale * range);
      savePng(render, testRender);
      cvNamedWindow("output", 0);
      imshow(output);
      imshow(testRender);
      imshow(testRenderMulti);
      cvWaitKey(-1);
    }
  }
  return 0;
}

