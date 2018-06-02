//#include "ui/window.inl"
//#include "window.inl"
#if 0
//#define CANVAS_SETHDC canvas_gdi_set
#define CANVAS_SETHDC canvas_gdiplus_set
#else
#define CANVAS_SETHDC canvas_img_set
#endif
#include "cstd.h"
#include "canvas.inl"
#include "canvas_nul.inl"
#include "canvas_gdi.inl"
#include "canvas_gdiplus.inl"
#include "canvas_img.inl"
#include "wincv.inl"
#include "img/imgio.inl"
//#include "test_console.inl"
int test_on_mouse(void* param, int event, int x, int y, int flags)
{
  switch (event) {
  case CV_EVENT_MOUSEWHEELUP:
    {
      int asdf=0;
    }
    break;
  }
  return 0;
}
enum {
  tt_1,
  tt_fillStyle,
  tt_strokeStyle,
  tt_lineWidth,
  tt_lineCap,
  tt_lineJoin,
  tt_miterLimit,
  tt_rect,
  tt_fillRect,
  tt_strokeRect,
  tt_clearRect,
  tt_fill,
  tt_stroke,
  tt_beginPath,
  tt_moveTo,
  tt_closePath,
  tt_lineTo,
  tt_clip,
  tt_quadraticCurveTo,
  tt_bezierCurveTo,
  tt_arc,
  tt_arcTo,
  tt_isPointInPath,
  tt_scale,
  tt_rotate,
  tt_translate,
  tt_transform,
  tt_setTransform,
  tt_font,
  tt_textAlign,
  tt_textBaseline,
  tt_fillText,
  tt_strokeText,
  tt_measureText,
  tt_drawImage,
  tt_draw21,
  tt_draw22,
  tt_draw1,
  tt_draw2,
  tt_draw25,
  tt_draw26,
  tt_draw3,
  tt_draw4,
  tt_max,
};
#include "test_data.txt"

int tt = tt_draw4;
//#include "test_cstr_splitfind.inl"
int test_on_paint(void* param, canvas_t* ctx)
{
  int rr = 0;
  //test_BezierFit();
  switch (tt) {
  case tt_draw4:
    {
    }
    break;
  case tt_draw3: {
    utime_start(_start_time);
    canvas_rotate(ctx, rr * PI / 180);
    //canvas_font(ctx, "20px 宋体");
    canvas_font(ctx, "40px Georgia");
    canvas_font(ctx, "15px Arial");
    canvas_moveTo(ctx, 10, 10);
    canvas_fillStyle(ctx, "gradient 0 0 570 0 0 black 0.5 red 1 white");
    //canvas_fillStyle(ctx, "black");
    canvas_lineTo(ctx, 50, 30);
    canvas_fillRect(ctx, 20, 50, 500, 5);
    canvas_fillText(ctx, text_en, -1, 20, 50, 500);
    canvas_lineTo(ctx, 100, 10);
    canvas_rotate(ctx, -rr * PI / 180);
    canvas_lineTo(ctx, 100, 10);
    canvas_stroke(ctx);
    printf("%f\n", utime_elapsed(_start_time));
  }
  break;
  case tt_draw26:
    canvas_fillStyle(ctx, "RadialGradient 100 150 0 100 150 150 0.1 rgb(255,0,0) 0.5 rgb(0,255,0) 1 rgb(0,0,255)");
    canvas_fillRect(ctx, 0, 0, 400, 300);
    break;
  case tt_draw25:
    //可以把lg对象理解成GDI中线性brush
    canvas_fillStyle(ctx, "gradient 0 0 0 300 0 rgb(255,0,0) 0.5 rgb(0,255,0) 1 rgb(0,0,255)");
    //再用这个brush来画正方形
    canvas_fillRect(ctx, 0, 0, 400, 300);
    break;
  case tt_draw2: {
    int i, n = 0, dx = 150, dy = 150, s = 100;
    double x, y, dig;
    canvas_fillStyle(ctx, "#EEEFF");
    canvas_fillRect(ctx, 0, 0, 400, 300);
    canvas_beginPath(ctx);
    // canvas_globalCompositeOperation = 'and';
    canvas_fillStyle(ctx, "rgb(100,255,100)");
    dig = PI / 15 * 11;
    canvas_moveTo(ctx, dx, dy);
    for (i = 0; i < 30; i++) {
      x = sin(i * dig);
      y = cos(i * dig);
      canvas_bezierCurveTo(ctx, dx + x * s, dy + y * s - 100, dx + x * s + 100, dy + y * s, dx + x * s, dy + y * s);
    }
    canvas_closePath(ctx);
    canvas_fill(ctx);
    canvas_stroke(ctx);
  }
  break;
  case tt_draw1: {
    int i, n = 0, dx = 150, dy = 150, s = 100;
    double x, y, dig;
    canvas_fillStyle(ctx, "#EEEEFF");
    canvas_fillRect(ctx, 0, 0, 400, 300);
    canvas_beginPath(ctx);
    canvas_fillStyle(ctx, "rgb(100,255,100)");
    canvas_strokeStyle(ctx, "rgb(0,0,100)");
    dig = PI / 15 * 11;
    for (i = 0; i < 30; i++) {
      x = sin(i * dig);
      y = cos(i * dig);
      canvas_lineTo(ctx, dx + x * s, dy + y * s);
    }
    canvas_closePath(ctx);
    canvas_fill(ctx);
    canvas_stroke(ctx);
  }
  break;
  case tt_draw21:
    //实践表明在不设施fillStyle下的默认fillStyle=black
    canvas_fillRect(ctx, 0, 0, 100, 100);
    //实践表明在不设施strokeStyle下的默认strokeStyle=black
    canvas_strokeRect(ctx, 120, 0, 100, 100);
    //设置纯色
    canvas_fillStyle(ctx, "red");
    canvas_strokeStyle(ctx, "blue");
    canvas_fillRect(ctx, 0, 120, 100, 100);
    canvas_strokeRect(ctx, 120, 120, 100, 100);
    //设置透明度实践证明透明度值>0,<1值越低，越透明，值>=1时为纯色，值<=0时为完全透明
    canvas_fillStyle(ctx, "rgba(255,0,0,0.2)");
    canvas_strokeStyle(ctx, "rgba(255,0,0,0.2)");
    canvas_fillRect(ctx, 240, 0 , 100, 100);
    canvas_strokeRect(ctx, 240, 120, 100, 100);
    break;
  case tt_draw22:
    //实践表明在不设施fillStyle下的默认fillStyle=black
    canvas_fillRect(ctx, 0, 0, 100, 100);
    //实践表明在不设施strokeStyle下的默认strokeStyle=black
    canvas_strokeRect(ctx, 120, 0, 100, 100);
    //设置纯色
    canvas_fillStyle(ctx, "red");
    canvas_strokeStyle(ctx, "blue");
    canvas_fillRect(ctx, 0, 120, 100, 100);
    canvas_strokeRect(ctx, 120, 120, 100, 100);
    //设置透明度实践证明透明度值>0,<1值越低，越透明，值>=1时为纯色，值<=0时为完全透明
    canvas_fillStyle(ctx, "rgba(255,0,0,0.2)");
    canvas_strokeStyle(ctx, "rgba(255,0,0,0.2)");
    canvas_fillRect(ctx, 240, 0, 100, 100);
    canvas_strokeRect(ctx, 240, 120, 100, 100);
    canvas_clearRect(ctx, 50, 50, 240, 120);
    break;
  case tt_1:
    //开始一个新的绘制路径
    canvas_beginPath(ctx);
    //设置弧线的颜色为蓝色
    canvas_strokeStyle(ctx, "blue");
    canvas_strokeStyle(ctx, "rgba(0.9, 0.1, 0.1, 0.2)");
    //以canvas中的坐标点(100,100)为圆心，绘制一个半径为50px的圆形
    canvas_arc(ctx, 100, 100, 50, 0, 2 * PI, 0);
    //按照指定的路径绘制弧线
    canvas_stroke(ctx);
    break;
  case tt_fillStyle:
    canvas_fillStyle(ctx, "#0000ff");
    canvas_fillRect(ctx, 20, 20, 150, 100);
    canvas_translate(ctx, 160, 0);
    canvas_fillStyle(ctx, "gradient 0 0 0 170 0 black 1 white");
    canvas_fillRect(ctx, 20, 20, 150, 100);
    canvas_translate(ctx, 160, 0);
    canvas_fillStyle(ctx, "gradient 0 0 170 0 0 black 1 white");
    canvas_fillRect(ctx, 20, 20, 150, 100);
    canvas_translate(ctx, 160, 0);
    canvas_fillStyle(ctx, "gradient 0 0 170 0 0 black 0.5 red 1 white");
    canvas_fillRect(ctx, 20, 20, 150, 100);
    canvas_translate(ctx, 160, 0);
    canvas_fillStyle(ctx, "gradient 0 0 170 0 0 black 0.3 magenta 0.5 blue 0.6 green 0.8 yellow 1 red");
    canvas_fillRect(ctx, 20, 20, 150, 100);
    canvas_translate(ctx, 160, 0);
    break;
  case tt_strokeStyle:
    canvas_lineWidth(ctx, 5);
    canvas_strokeStyle(ctx, "#0000ff");
    canvas_strokeRect(ctx, 20, 20, 150, 100);
    canvas_translate(ctx, 160, 0);
    canvas_strokeStyle(ctx, "gradient 0 0 170 0 0 magenta 0.5 blue 1 red");
    canvas_strokeRect(ctx, 20, 20, 150, 100);
    canvas_translate(ctx, -160, 110);
    canvas_font(ctx, "30px Verdana");
    canvas_lineWidth(ctx, 1);
    // 用渐变进行填充
    canvas_strokeStyle(ctx, "gradient 0 0 370 0 0 magenta 0.5 blue 1 red");
    canvas_strokeText(ctx, "w3school.com.cn", -1, 10, 50, -1);
    //canvas_fillText(ctx, "w3school.com.cn", -1, 10, 50, -1);
    break;
  case tt_lineWidth:
    canvas_lineWidth(ctx, 10);
    canvas_strokeRect(ctx, 20, 20, 80, 100);
    break;
  case tt_lineCap:
    canvas_beginPath(ctx);
    canvas_lineWidth(ctx, 10);
    canvas_lineCap(ctx, lineCap_butt);
    canvas_moveTo(ctx, 20, 20);
    canvas_lineTo(ctx, 200, 20);
    canvas_stroke(ctx);
    canvas_beginPath(ctx);
    canvas_lineCap(ctx, lineCap_round);
    canvas_moveTo(ctx, 20, 40);
    canvas_lineTo(ctx, 200, 40);
    canvas_stroke(ctx);
    canvas_beginPath(ctx);
    canvas_lineCap(ctx, lineCap_square);
    canvas_moveTo(ctx, 20, 60);
    canvas_lineTo(ctx, 200, 60);
    canvas_stroke(ctx);
    break;
  case tt_lineJoin:
    canvas_beginPath(ctx);
    canvas_lineWidth(ctx, 10);
    canvas_lineJoin(ctx, lineJoin_round);
    canvas_moveTo(ctx, 20, 20);
    canvas_lineTo(ctx, 100, 50);
    canvas_lineTo(ctx, 20, 100);
    canvas_stroke(ctx);
    break;
  case tt_miterLimit:
    canvas_lineWidth(ctx, 10);
    canvas_lineJoin(ctx, lineJoin_miter);
    canvas_miterLimit(ctx, 5);
    canvas_moveTo(ctx, 20, 20);
    canvas_lineTo(ctx, 50, 27);
    canvas_lineTo(ctx, 20, 34);
    canvas_stroke(ctx);
    break;
  case tt_rect:
    canvas_rect(ctx, 20, 20, 150, 100);
    canvas_stroke(ctx);
    break;
  case tt_fillRect:
    canvas_fillRect(ctx, 20, 20, 150, 100);
    break;
  case tt_strokeRect:
    canvas_strokeRect(ctx, 20, 20, 150, 100);
    break;
  case tt_clearRect:
    canvas_fillStyle(ctx, "red");
    canvas_fillRect(ctx, 0, 0, 300, 150);
    canvas_clearRect(ctx, 20, 20, 100, 50);
    break;
  case tt_fill:
    canvas_rect(ctx, 20, 20, 150, 100);
    canvas_fillStyle(ctx, "green");
    canvas_fill(ctx);
    canvas_rect(ctx, 80, 80, 150, 100);
    canvas_fillStyle(ctx, "red");
    canvas_fill(ctx);
    break;
  case tt_stroke:
    canvas_beginPath(ctx);
    canvas_moveTo(ctx, 20, 20);
    canvas_moveTo(ctx, 20, 20);
    canvas_lineTo(ctx, 20, 100);
    canvas_lineTo(ctx, 70, 100);
    canvas_strokeStyle(ctx, "green");
    canvas_stroke(ctx);
    break;
  case tt_beginPath:
    canvas_beginPath(ctx);
    canvas_lineWidth(ctx, 5);
    canvas_strokeStyle(ctx, "red"); // 红色路径
    canvas_moveTo(ctx, 0, 75);
    canvas_lineTo(ctx, 250, 75);
    canvas_stroke(ctx); // 进行绘制
    canvas_beginPath(ctx);
    canvas_strokeStyle(ctx, "blue"); // 蓝色路径
    canvas_moveTo(ctx, 50, 0);
    canvas_lineTo(ctx, 150, 130);
    canvas_stroke(ctx); // 进行绘制
    break;
  case tt_moveTo:
    canvas_beginPath(ctx);
    canvas_moveTo(ctx, 0, 0);
    canvas_lineTo(ctx, 300, 150);
    canvas_stroke(ctx);
    break;
  case tt_closePath:
    canvas_beginPath(ctx);
    canvas_moveTo(ctx, 20, 20);
    canvas_lineTo(ctx, 20, 100);
    canvas_lineTo(ctx, 70, 100);
    canvas_closePath(ctx);
    canvas_stroke(ctx);
    break;
  case tt_lineTo:
    canvas_beginPath(ctx);
    canvas_moveTo(ctx, 0, 0);
    canvas_lineTo(ctx, 300, 150);
    canvas_stroke(ctx);
    break;
  case tt_clip:
    // 剪切矩形区域
    canvas_rect(ctx, 50, 20, 200, 120);
    canvas_stroke(ctx);
    canvas_clip(ctx);
    // 在 clip() 之后绘制绿色矩形
    canvas_fillStyle(ctx, "green");
    canvas_fillRect(ctx, 0, 0, 150, 100);
    break;
  case tt_quadraticCurveTo:
    canvas_beginPath(ctx);
    canvas_moveTo(ctx, 20, 20);
    canvas_quadraticCurveTo(ctx, 20, 100, 200, 20);
    canvas_stroke(ctx);
    break;
  case tt_bezierCurveTo:
    canvas_beginPath(ctx);
    canvas_moveTo(ctx, 20, 20);
    canvas_bezierCurveTo(ctx, 20, 100, 200, 100, 200, 20);
    canvas_stroke(ctx);
    break;
  case tt_arc:
    canvas_beginPath(ctx);
    canvas_arc(ctx, 100, 75, 50, 0, 2 * M_PI, FALSE);
    canvas_stroke(ctx);
    break;
  case tt_arcTo:
    canvas_beginPath(ctx);
    canvas_beginPath(ctx);
    canvas_moveTo(ctx, 20, 20); // 创建开始点
    canvas_lineTo(ctx, 100, 20); // 创建水平线
    canvas_arcTo(ctx, 100, 20, 150, 70, 50); // 创建弧
    canvas_lineTo(ctx, 150, 120); // 创建垂直线
    canvas_stroke(ctx); // 进行绘制
    break;
  case tt_isPointInPath:
    canvas_rect(ctx, 20, 20, 150, 100);
    if (canvas_isPointInPath(ctx, 20, 50)) {
      canvas_stroke(ctx);
    }
    break;
  case tt_scale:
    canvas_strokeRect(ctx, 5, 5, 25, 15);
    canvas_scale(ctx, 2, 2);
    canvas_strokeRect(ctx, 5, 5, 25, 15);
    break;
  case tt_rotate:
    canvas_rotate(ctx, 20 * PI / 180);
    canvas_fillRect(ctx, 50, 20, 100, 50);
    break;
  case tt_translate:
    canvas_fillRect(ctx, 10, 10, 100, 50);
    canvas_translate(ctx, 70, 70);
    canvas_fillRect(ctx, 10, 10, 100, 50);
    break;
  case tt_transform:
    canvas_fillStyle(ctx, "yellow");
    canvas_fillRect(ctx, 0, 0, 250, 100);
    canvas_transform(ctx, 1, 0.5, -0.5, 1, 30, 10);
    canvas_fillStyle(ctx, "red");
    canvas_fillRect(ctx, 0, 0, 250, 100);
    canvas_transform(ctx, 1, 0.5, -0.5, 1, 30, 10);
    canvas_fillStyle(ctx, "blue");
    canvas_fillRect(ctx, 0, 0, 250, 100);
    break;
  case tt_setTransform:
    canvas_fillStyle(ctx, "yellow");
    canvas_fillRect(ctx, 0, 0, 250, 100);
    canvas_setTransform(ctx, 1, 0.5, -0.5, 1, 30, 10);
    canvas_fillStyle(ctx, "red");
    canvas_fillRect(ctx, 0, 0, 250, 100);
    canvas_setTransform(ctx, 1, 0.5, -0.5, 1, 30, 10);
    canvas_fillStyle(ctx, "blue");
    canvas_fillRect(ctx, 0, 0, 250, 100);
    break;
  case tt_font:
    canvas_font(ctx, "40px Arial");
    canvas_fillText(ctx, "Hello World", -1, 10, 50, -1);
    break;
  case tt_textAlign:
    // 在位置 150 创建蓝线
    canvas_strokeStyle(ctx, "blue");
    canvas_moveTo(ctx, 150, 20);
    canvas_lineTo(ctx, 150, 170);
    canvas_stroke(ctx);
    canvas_font(ctx, "15px Arial");
    // 显示不同的 textAlign 值
    canvas_textAlign(ctx, "start");
    canvas_fillText(ctx, "textAlign=start", -1, 150, 60, -1);
    canvas_textAlign(ctx, "end");
    canvas_fillText(ctx, "textAlign=end", -1, 150, 80, -1);
    canvas_textAlign(ctx, "left");
    canvas_fillText(ctx, "textAlign=left", -1, 150, 100, -1);
    canvas_textAlign(ctx, "center");
    canvas_fillText(ctx, "textAlign=center", -1, 150, 120, -1);
    canvas_textAlign(ctx, "right");
    canvas_fillText(ctx, "textAlign=right", -1, 150, 140, -1);
    break;
  case tt_textBaseline://在位置 y=100 绘制蓝色线条
    canvas_strokeStyle(ctx, "blue");
    canvas_moveTo(ctx, 5, 100);
    canvas_lineTo(ctx, 395, 100);
    canvas_stroke(ctx);
    canvas_font(ctx, "20px Arial");
    //在 y=200 以不同的 textBaseline 值放置每个单词
    canvas_textBaseline(ctx, "top");
    canvas_fillText(ctx, "Top", -1, 5, 100, -1);
    canvas_textBaseline(ctx, "bottom");
    canvas_fillText(ctx, "Bottom", -1, 50, 100, -1);
    canvas_textBaseline(ctx, "middle");
    canvas_fillText(ctx, "Middle", -1, 120, 100, -1);
    canvas_textBaseline(ctx, "alphabetic");
    canvas_fillText(ctx, "Alphabetic", -1, 190, 100, -1);
    canvas_textBaseline(ctx, "hanging");
    canvas_fillText(ctx, "Hanging", -1, 290, 100, -1);
    break;
  case tt_fillText:
    canvas_font(ctx, "20px Georgia");
    canvas_fillText(ctx, "Hello World!", -1, 10, 50, -1);
    canvas_font(ctx, "30px Verdana");
    // 用渐变填色
    canvas_fillStyle(ctx, "gradient 0, 0, 200, 0 0 magenta 0.5 blue 1 red");
    canvas_fillText(ctx, "w3school.com.cn", -1, 10, 90, -1);
    canvas_fillStyle(ctx, "black");
    canvas_font(ctx, "15px 宋体");
    canvas_fillText(ctx, "字体系列为宋体，大小为15，字体风格为常规的黑色字体", -1, 10, 150, -1);
    canvas_fillStyle(ctx, "Red");
    canvas_font(ctx, "Italic 25.5px Arial");
    canvas_fillText(ctx, "字体系列为Arial，大小为25.5，字体风格为倾斜的红色字体", -1, 10, 200, -1);
    canvas_fillStyle(ctx, "green");
    canvas_font(ctx, "Underline 20px 微软雅黑");
    canvas_fillText(ctx, "使用TextRenderer类绘制字符串", -1, 10, 250, -1);
    break;
  case tt_strokeText:
    canvas_font(ctx, "20px Georgia");
    canvas_strokeText(ctx, "Hello World!", -1, 10, 50, -1);
    canvas_font(ctx, "30px Verdana");
    // 用渐变填色
    canvas_strokeStyle(ctx, "gradient 0, 0, 200, 0 0 magenta 0.5 blue 1 red");
    canvas_strokeText(ctx, "w3school.com.cn", -1, 10, 90, -1);
    break;
  case tt_measureText:
    if (1) {
      const char* txt = "Hello World";
      char buf[256];
      int width;
      canvas_font(ctx, "30px Arial");
      width = canvas_measureText(ctx, txt, -1);
      _snprintf(buf, 256, "width:%d", width);
      canvas_fillStyle(ctx, "#440000ff");
      canvas_fillRect(ctx, 10, 100, width, 30);
      canvas_fillStyle(ctx, "#000");
      canvas_fillText(ctx, buf, -1, 10, 50, -1);
      canvas_fillText(ctx, txt, -1, 10, 100, -1);
    }
    break;
  case tt_drawImage:
    if (1) {
      img_t img[1] = {0};
      ISIZE sz = {240, 160};
      ISIZE sz1 = {90, 80};
      IRECT rc1 = iRECT2(90, 130, 90, 80);
      imread("E:/pub/pic/tulip.jpg", 3, 1, img);
      // 向画布上面绘制图片
      canvas_drawImage(ctx, img, 0, NULL, 10, 10, NULL);
      canvas_translate(ctx, img->w + 10, 0);
      // 在画布上对图像进行定位，然后规定图像的宽度和高度
      canvas_drawImage(ctx, img, 0, NULL, 10, 10, &sz);
      canvas_translate(ctx, 0, img->h + 10);
      // 剪切图片，并在画布上对被剪切的部分进行定位
      canvas_drawImage(ctx, img, 0, &rc1, 20, 20, &sz1);
      imfree(img);
    }
    break;
  default:
    break;
  }
  return 0;
}
int test_canvas()
{
  const char* winname = "asdfasdf";
  //test_cstr_splitfind();
  //gradient_t aa = {1,2,3,4, 0.0, 0x11111111, 0.0, 0x11111111, 0.0, 0x11111111, 0.0, 0x11111111};
  cvNamedWindow(winname, 1);
  cvSetCallback(winname, test_on_mouse, test_on_paint, NULL);
  for (;;) {
    int ch = cvWaitKey(-1);
    if (-2 == ch) {
      break;
    }
    if (ch > 0) {
      tt = (tt + 1) % tt_max;
      cvInvalidateRect(winname, NULL);
    }
    //cvSetWidgetsXml("asdf", "test001.xml");
    //cvSetImage(winname, "image1", _W, _W, buf, _W*3, 3, 0);
    //printf("%d\n", ch);
  }
  return 0;
}

