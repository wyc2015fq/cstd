#ifndef _CANVAS_INL_
#define _CANVAS_INL_
#include "cstd.h"
#include "geo.inl"
enum {
#define HTML_COLOR_DEF(NAME, CLR) HCLR_##NAME = 0xFF##CLR ,
#include "html_color.txt"
#undef HTML_COLOR_DEF
};
#define xypos_toint(x) ((int)(x))
#define RADIAN (PI/180.0)
// 线条末端线帽的样式。
enum {
  lineCap_butt, // 默认。向线条的每个末端添加平直的边缘。
  lineCap_round, // 向线条的每个末端添加圆形线帽。
  lineCap_square, // 向线条的每个末端添加正方形线帽。
};
// 当两条线交汇时边角的类型
enum {
  lineJoin_bevel, // 创建斜角。
  lineJoin_round, // 创建圆角
  lineJoin_miter, // 默认。创建尖角。值 "miter" 受 miterLimit 属性的影响。
};
BOOL color_parse(const char* s, int l, COLOR* pout)
{
  char str[16] = {0};
  const char* e;
  l = l > 0 ? l : strlen(s);
  e = s + l;
  for (; ' ' == *s || '\t' == *s; ++s);
  if ('#' == s[0]) {
    memset(str, 'f', 8);
    ++s;
    l = e - s;
    switch (l) {
    case 4:
      str[0] = str[1] = s[0], str[2] = str[3] = s[1], str[4] = str[5] = s[2], str[6] = str[7] = s[3];
      break;
    case 3:
      str[2] = str[3] = s[0], str[4] = str[5] = s[1], str[6] = str[7] = s[2];
      break;
    case 2:
      str[0] = str[1] = s[0], str[4] = str[5] = str[6] = str[7] = s[1];
      break;
    case 1:
      str[4] = str[5] = str[6] = str[7] = s[0];
      break;
    default:
      memcpy(str + (8 - l), s, l);
      break;
    }
    *pout = cstr_hex2int(str, 8);
    return TRUE;
  }
  else {
    char* p;
    static const char* sss = "|"
#define HTML_COLOR_DEF(NAME, CLR) #NAME"|"
#include "html_color.txt"
#undef HTML_COLOR_DEF
        ;
    static const COLOR ccc[] = {
#define HTML_COLOR_DEF(NAME, CLR) 0xFF##CLR,
#include "html_color.txt"
#undef HTML_COLOR_DEF
    };
    static int ssslen = strlen(sss);
    int k = cstr_splitfind(sss, ssslen, s, l, 1, NULL);
    if (k >= 0) {
      *pout = ccc[k];
    }
#define HTML_COLOR_DEF(NAME, CLR) if (0==cstr_icmp(s, l, #NAME , -1, 1)) { *pout = 0xFF##CLR; return TRUE; }
#include "html_color.txt"
#undef HTML_COLOR_DEF
    if (p = cstr_chr(s, l, '(')) {
      int ok = 1;
      double r = 0, g = 0, b = 0, a = 255;
      ++p;
      for (; p != e && *s != '('; ++s) {
        switch (*s) {
#define TTTT(ca, a) case ca: a = strtod(p, &p); a = a<1?(a*255+0.5):a; break
          TTTT('a', a);
          TTTT('r', r);
          TTTT('g', g);
          TTTT('b', b);
#undef TTTT
        default:
          ok = 0;
          break;
        }
        for (; ' ' == *p || ',' == *p || '\t' == *p; ++p);
      }
      if (ok) {
        *pout = _RGBA(r, g, b, a);
      }
      return ok;
    }
  }
  return FALSE;
}
typedef float xypos_t;
// Private so Surface objects can not be copied
typedef struct canvas_t canvas_t;
struct canvas_t {
  void* x;
#define canvas_release(s) s->release(s)
  int (*release)(canvas_t* s);
  // 颜色、样式
  // color|#xxxxxx
  // gradient x0 y0 x1 y1 [stop color]
  //x0 渐变开始点的 x 坐标
  //y0 渐变开始点的 y 坐标
  //x1 渐变结束点的 x 坐标
  //y1 渐变结束点的 y 坐标
  // RadialGradient
  //x0 渐变的开始圆的 x 坐标
  //y0 渐变的开始圆的 y 坐标
  //r0 开始圆的半径
  //x1 渐变的结束圆的 x 坐标
  //y1 渐变的结束圆的 y 坐标
  //r1 结束圆的半径
#define canvas_fillStyle(s, clr) s->fillStyle(s, clr)
  int (*fillStyle)(canvas_t* s, const char* str); // 填充的颜色，默认为”#000000”，与strokeStyle一样，值也可以设置为CSS颜色值、渐变对象或者模式
#define canvas_strokeStyle(s, clr) s->strokeStyle(s, clr)
  int (*strokeStyle)(canvas_t* s, const char* str); // 线条的颜色，默认为”#000000”，其值可以设置为CSS颜色值、渐变对象或者模式对象。对象。
  // 阴影
  int (*shadowColor)(canvas_t* s);// 设置或返回用于阴影的颜色
  int (*shadowBlur)(canvas_t* s);// 设置或返回用于阴影的模糊级别
  int (*shadowOffsetX)(canvas_t* s);// 设置或返回阴影距形状的水平距离
  int (*shadowOffsetY)(canvas_t* s);// 设置或返回阴影距形状的垂直距离
  // 线条样式
#define canvas_lineWidth(s, px) s->lineWidth(s, px)
  int (*lineWidth)(canvas_t* s, float px); // 线条的宽度，单位是像素（px），默认为1.0。
#define canvas_lineCap(s, type) s->lineCap(s, type)
  int (*lineCap)(canvas_t* s, int type); // 线条的端点样式，有butt（无）、round（圆头）、square（方头）三种类型可供选择，默认为butt。
#define canvas_lineJoin(s, type) s->lineJoin(s, type)
  int (*lineJoin)(canvas_t* s, int type); // 线条的转折处样式，有round（圆角）、bevel（平角）、miter（尖角）三种；类型可供选择，默认为miter。
#define canvas_miterLimit(s, x) s->miterLimit(s, x)
  int (*miterLimit)(canvas_t* s, float x); //线条尖角折角的锐利程序，默认为10。
  // 矩形
#define canvas_rect(s, left, top, width, height) s->rect(s, left, top, width, height)
  int (*rect)(canvas_t* s, xypos_t left, xypos_t top, xypos_t width, xypos_t height); // 用于描绘一个已知左上角顶点位置以及宽和高的矩形，描绘完成后Context的绘制起点会移动到该矩形的左上角顶点。参数表示矩形左上角顶点的x、y坐标以及矩形的宽和高。
#define canvas_fillRect(s, left, top, width, height) s->fillRect(s, left, top, width, height)
  int (*fillRect)(canvas_t* s, xypos_t left, xypos_t top, xypos_t width, xypos_t height); // 绘制“被填充”的矩形
#define canvas_strokeRect(s, left, top, width, height) s->strokeRect(s, left, top, width, height)
  int (*strokeRect)(canvas_t* s, xypos_t left, xypos_t top, xypos_t width, xypos_t height); // 绘制矩形（无填充）
#define canvas_clearRect(s, left, top, width, height) s->clearRect(s, left, top, width, height)
  int (*clearRect)(canvas_t* s, xypos_t left, xypos_t top, xypos_t width, xypos_t height); //在给定的矩形内清除指定的像素
  // 路径
#define canvas_beginPath(s) s->beginPath(s)
  int (*beginPath)(canvas_t* s); // 起始一条路径，或重置当前路径
#define canvas_closePath(s) s->closePath(s)
  int (*closePath)(canvas_t* s); // 创建从当前点回到起始点的路径
#define canvas_clip(s) s->clip(s)
  int (*clip)(canvas_t* s); // 用于按照已有的路线在画布中设置剪辑区域。调用clip()方法之后，图形绘制代码只对剪辑区域有效而不再影响区域外的画布。（默认剪辑区域为整个Canvas区域）
#define canvas_fill(s) s->fill(s)
  int (*fill)(canvas_t* s); // 用于使用当前的填充风格来填充路径的区域。
#define canvas_stroke(s) s->stroke(s)
  int (*stroke)(canvas_t* s); // 绘制已定义的路径。默认颜色是黑色。
#define canvas_moveTo(s, x, y) s->moveTo(s, x, y)
  int (*moveTo)(canvas_t* s, xypos_t x, xypos_t y); // 用于显式地指定路径的起点。不创建线条。默认状态下，第一条路径的起点是画布的(0, 0)点，之后的起点是上一条路径的终点。
#define canvas_lineTo(s, x, y) s->lineTo(s, x, y)
  int (*lineTo)(canvas_t* s, xypos_t x, xypos_t y); // 用于描绘一条从起点从指定位置的直线路径，描绘完成后绘制的起点会移动到该指定位置。
#define canvas_arcTo(s, x1, y1, x2, y2, radius) s->arcTo(s, x1, y1, x2, y2, radius)
  int (*arcTo)(canvas_t* s, xypos_t x1, xypos_t y1, xypos_t x2, xypos_t y2, xypos_t radius); // 用于描绘一个与两条线段相切的圆弧，两条线段分别以当前Context绘制起点和(x2, y2)点为起点，都以(x1, y1)点为终点，圆弧的半径为radius。描绘完成后绘制起点会移动到以(x2, y2)为起点的线段与圆弧的切点。
#define canvas_arc(s, x, y, radius, startAngle, endAngle, anticlockwise) s->arc(s, x, y, radius, startAngle, endAngle, anticlockwise)
  int (*arc)(canvas_t* s, xypos_t x, xypos_t y, xypos_t radius, xypos_t startAngle, xypos_t endAngle, BOOL anticlockwise); // 用于描绘一个以(x, y)点为圆心，radius为半径，startAngle为起始弧度，endAngle为终止弧度的圆弧。anticlockwise为布尔型的参数，true表示逆时针，false表示顺时针。参数中的两个弧度以0表示0°，位置在3点钟方向；Math.PI值表示180°，位置在9点钟方向。
#define canvas_quadraticCurveTo(s, cpx, cpy, x, y) s->quadraticCurveTo(s, cpx, cpy, x, y)
  int (*quadraticCurveTo)(canvas_t* s, xypos_t cpx, xypos_t cpy, xypos_t x, xypos_t y); // 创建二次贝塞尔曲线。以当前Context绘制起点为起点，(cpx,cpy)点为控制点，(x, y)点为终点的二次样条曲线路径。
#define canvas_bezierCurveTo(s, cpx1, cpy1, cpx2, cpy2, x, y) s->bezierCurveTo(s, cpx1, cpy1, cpx2, cpy2, x, y)
  int (*bezierCurveTo)(canvas_t* s, xypos_t cpx1, xypos_t cpy1, xypos_t cpx2, xypos_t cpy2, xypos_t x, xypos_t y); // 创建三次方贝塞尔曲线。以当前Context绘制起点为起点，(cpx1,cpy1)点和(cpx2, cpy2)点为两个控制点，(x, y)点为终点的贝塞尔曲线路径。 路径描绘完成后，需要调用Context对象的fill()和stroke()方法来填充路径和绘制路径线条，或者调用clip()方法来剪辑Canvas区域。以上三个方法的原型如下：
#define canvas_isPointInPath(s, x, y) s->isPointInPath(s, x, y)
  int (*isPointInPath)(canvas_t* s, xypos_t x, xypos_t y); // 如果指定的点位于当前路径中，则返回 true，否则返回 false
  // 转换
#define canvas_scale(s, scalewidth, scaleheight) s->scale(s, scalewidth, scaleheight)
  int (*scale)(canvas_t* s, float scalewidth, float scaleheight); // 缩放当前绘图至更大或更小
#define canvas_rotate(s, angle) s->rotate(s, angle)
  int (*rotate)(canvas_t* s, float angle); // 旋转当前绘图
#define canvas_translate(s, x, y) s->translate(s, x, y)
  int (*translate)(canvas_t* s, float x, float y); // 重新映射画布上的 (0,0) 位置
  // a c e
  // b d f
  // 0 0 1
  // a 水平旋转绘图
  // b 水平倾斜绘图
  // c 垂直倾斜绘图
  // d 垂直缩放绘图
  // e 水平移动绘图
  // f 垂直移动绘图
#define canvas_transform(s, a, b, c, d, e, f) s->transform(s, a, b, c, d, e, f)
  int (*transform)(canvas_t* s, float a, float b, float c, float d, float e, float f); // 替换绘图的当前转换矩阵
#define canvas_setTransform(s, a, b, c, d, e, f) s->setTransform(s, a, b, c, d, e, f)
  int (*setTransform)(canvas_t* s, float a, float b, float c, float d, float e, float f); // 将当前转换重置为单位矩阵。然后运行 transform()
  // 文本
  // [style][variant][weight][size][family]
  // font-style 规定字体样式。可能的?担簄ormal|italic|oblique
  // font-variant 规定字体变体。可能的值： normal|small-caps
  // font-weight 规定字体的粗细。可能的值： normal|bold|bolder|lighter|100|200|300|400|500|600|700|800|900
  // font-size / line-height 规定字号和行高，以像素计。
  // font-family 规定字体系列。
#define canvas_font(s, str) s->font(s, str)
  int (*font)(canvas_t* s, const char* str); // 设置或返回文本内容的当前字体属性
  //start 默认。文本在指定的位置开始。
  //end 文本在指定的位置结束。
  //center 文本的中心被放置在指定的位置。
  //left 文本左对齐。
  //right 文本右对齐。
#define canvas_textAlign(s, str) s->textAlign(s, str)
  int (*textAlign)(canvas_t* s, const char* str); // 设置或返回文本内容的当前对齐方式 "center|end|left|right|start";
  //alphabetic 默认。文本基线是普通的字母基线。
  //top 文本基线是 em 方框的顶端。。
  //hanging 文本基线是悬挂基线。
  //middle 文本基线是 em 方框的正中。
  //ideographic 文本基线是表意基线。
  //bottom 文本基线是 em 方框的底端。
#define canvas_textBaseline(s, str) s->textBaseline(s, str)
  int (*textBaseline)(canvas_t* s, const char* str); // 设置或返回在绘制文本时使用的当前文本基线 "alphabetic|top|hanging|middle|ideographic|bottom";
#define canvas_fillText(s, text, textlen, x, y, maxWidth) s->fillText(s, text, textlen, x, y, maxWidth)
  int (*fillText)(canvas_t* s, const char* text, int textlen, xypos_t x, xypos_t y, xypos_t maxWidth); // 在画布上绘制“被填充的”文本
#define canvas_strokeText(s, text, textlen, x, y, maxWidth) s->strokeText(s, text, textlen, x, y, maxWidth)
  int (*strokeText)(canvas_t* s, const char* text, int textlen, xypos_t x, xypos_t y, xypos_t maxWidth); // 在画布上绘制文本（无填充）
#define canvas_measureText(s, text, textlen) s->measureText(s, text, textlen)
  int (*measureText)(canvas_t* s, const char* text, int textlen); // 返回包含指定文本宽度的对象
  // 图像绘制
#define canvas_drawImage(s, img, f, rcimg, x, y, sz) s->drawImage(s, img, f, rcimg, x, y, sz)
  int (*drawImage)(canvas_t* s, const img_t* img, int frame, const IRECT* rcimg, xypos_t x, xypos_t y, const ISIZE* sz); // 向画布上绘制图像、画布或视频
  // 像素操作
  int (*width)(canvas_t* s); // 返回 ImageData 对象的宽度
  int (*height)(canvas_t* s); // 返回 ImageData 对象的高度
  int (*data)(canvas_t* s); // 返回一个对象，其包含指定的 ImageData 对象的图像数据
  int (*createImageData)(canvas_t* s); // 创建新的、空白的 ImageData 对象
  int (*getImageData)(canvas_t* s); // 返回 ImageData 对象，该对象为画布上指定的矩形复制像素数据
  int (*putImageData)(canvas_t* s); // 把图像数据（从指定的 ImageData 对象）放回画布上
  // 合成
  int (*globalAlpha)(canvas_t* s); // 设置或返回绘图的当前 alpha 或透明值
  int (*globalCompositeOperation)(canvas_t* s); // 设置或返回新图像如何绘制到已有的图像上
  // 其他
  int (*save)(canvas_t* s); // 保存当前环境的状态
  int (*restore)(canvas_t* s); // 返回之前保存过的路径状态和属性
  int (*createEvent)(canvas_t* s); //
  int (*getContext)(canvas_t* s); //
  int (*toDataURL)(canvas_t* s); //
};
#define CANVAS_FUN(TT) \
  TT(release) \
  TT(fillStyle) \
  TT(strokeStyle) \
  TT(shadowColor) \
  TT(shadowBlur) \
  TT(shadowOffsetX) \
  TT(shadowOffsetY) \
  TT(lineWidth) \
  TT(lineCap) \
  TT(lineJoin) \
  TT(miterLimit) \
  TT(rect) \
  TT(fillRect) \
  TT(strokeRect) \
  TT(clearRect) \
  TT(beginPath) \
  TT(closePath) \
  TT(clip) \
  TT(fill) \
  TT(stroke) \
  TT(moveTo) \
  TT(lineTo) \
  TT(arcTo) \
  TT(arc) \
  TT(quadraticCurveTo) \
  TT(bezierCurveTo) \
  TT(isPointInPath) \
  TT(scale) \
  TT(rotate) \
  TT(translate) \
  TT(transform) \
  TT(setTransform) \
  TT(font) \
  TT(textAlign) \
  TT(textBaseline) \
  TT(fillText) \
  TT(strokeText) \
  TT(measureText) \
  TT(drawImage) \
  TT(width) \
  TT(height) \
  TT(data) \
  TT(createImageData) \
  TT(getImageData) \
  TT(putImageData) \
  TT(globalAlpha) \
  TT(globalCompositeOperation) \
  TT(save) \
  TT(restore) \
  TT(createEvent) \
  TT(getContext) \
  TT(toDataURL)
#endif // _CANVAS_INL_

