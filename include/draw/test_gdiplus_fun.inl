#include "img/imgio.inl"
typedef struct {
  wchar Codepoint;
  float XAdvance;
  float X0, Y0, X1, Y1;
  float U0, V0, U1, V1;// Texture coordinates
} ImFontGlyph;
typedef struct {
  // Members: Hot ~62/78 bytes
  float FontSize;// <user set> // Height of characters, set during loading (don't change after loading)
  float Scale;// = 1.f // Base font scale, multiplied by the per-window font scale which you can adjust with SetFontScale()
  ImVec2 DisplayOffset;// = (0.f,1.f) // Offset font rendering by xx pixels
  int Glyphs_Size;
  int IndexXAdvance_Size;
  int IndexLookup_Size;
  ImFontGlyph* Glyphs;// // All glyphs.
  float* IndexXAdvance;// // Sparse. Glyphs->XAdvance in a directly indexable way (more cache-friendly, for CalcTextSize functions which are often bottleneck in large UI).
  unsigned short* IndexLookup;// // Sparse. Index glyphs by Unicode code-point.
  ImFontGlyph* FallbackGlyph;// == FindGlyph(FontFallbackChar)
  float FallbackXAdvance;// == FallbackGlyph->XAdvance
  wchar FallbackChar;// = '?' // Replacement glyph if one isn't found. Only set via SetFallbackChar()
  // Members: Cold ~18/26 bytes
  short ConfigDataCount;// ~ 1 // Number of ImFontConfig involved in creating this font. Bigger than 1 when merging multiple font sources into one ImFont.
  //ImFontConfig* ConfigData;// // Pointer within ContainerAtlas->ConfigData
  //ImFontAtlas* ContainerAtlas;// // What we has been loaded into
  float Ascent, Descent;// // Ascent: distance from top to bottom of e.g. 'A' [0..FontSize]
  // Methods
  uchar* TexPixelsAlpha8;
  int TexWidth;
  int TexHeight;
  int TexDesiredWidth;
  ImVec2 TexUvWhitePixel;
  void* TexID;
} ImFont;
typedef struct {
  void* FontData;// // TTF data
  int FontDataSize;// // TTF data size
  bool FontDataOwnedByAtlas;// true // TTF data ownership taken by the container ImFontAtlas (will delete memory itself). Set to true
  int FontNo;// 0 // Index of font within TTF file
  float SizePixels;// // Size in pixels for rasterizer
  int OversampleH, OversampleV;// 3, 1 // Rasterize at higher quality for sub-pixel positioning. We don't use sub-pixel positions on the Y axis.
  bool PixelSnapH;// false // Align every glyph to pixel boundary. Useful e.g. if you are merging a non-pixel aligned font with the default font. If enabled, you can set OversampleH/V to 1.
  ImVec2 GlyphExtraSpacing;// 0, 0 // Extra spacing (in pixels) between glyphs
  const ushort* GlyphRanges;// // Pointer to a user-provided list of Unicode range (2 value per range, values are inclusive, zero-terminated list). THE ARRAY DATA NEEDS TO PERSIST AS LONG AS THE FONT IS ALIVE.
  bool MergeMode;// false // Merge into previous ImFont, so you can combine multiple inputs font into one ImFont (e.g. ASCII font + icons + Japanese glyphs).
  bool MergeGlyphCenterV;// false // When merging (multiple ImFontInput for one ImFont), vertically center new glyphs instead of aligning their baseline
  // [Internal]
  char Name[32];// Name (strictly for debugging)
  ImFont* DstFont;
} ImFontConfig;
//#include "font/ttffont.inl"
void test_gdiplus1(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  //softgc
  //sgClear(g, ColorWhite);
  //创建用于绘制图像所使用的画笔
  gcPenColor(g, ColorRed);
  //画矩形
  gcDrawRectangle(g, 20, 20, 200, 100);
  //画直线
  gcDrawLine(g, 20, 250, 200, 250);
  //画椭圆
  gcDrawEllipse(g, 250, 20, 150, 100);
}
void test_gdiplus2(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  //创建Graphics对象
  //gcClear(ColorWhite);
  //创建画笔，第一个参数为颜色，第二个画笔的宽度，可选参数，默认为1；
  gcPenColor(g, ColorBlack);
  gcLineWidth(g, 2);
  //绘制矩形
  //gcDrawRectangle(g, 50, 50, 200, 100);
  //重新设置画笔颜色
  gcPenColor(g, ColorGreen);
  //重新设置画笔宽度
  gcLineWidth(g, 10);
  //绘制椭圆
  //gcDrawEllipse(g, 50, 200, 200, 100);
  {
    Path path[1] = {0};
    PathAddArc(path, 100, 100, 100, 200, 10, 270);
    gcPen(g, ColorBlack, 1);
    gcDrawPath2(g, path);
    PathFree(path);
  }
}
void test_gdiplus3(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  float dashArray[] = {
    5.0f, //线长5个像素
      2.0f, //间断2个像素
      15.0f, //线长15个像素
      8.0f //间断4个像素
  };
  //gcClear(ColorWhite);
  //默认为实线
  gcPenColor(g, ColorRed);
  gcLineWidth(g, 3);
  gcDrawRectangle(g, 50, 50, 200, 100);
  //设置画笔线型
  gcDashStyle(g, DashStyleDashDotDot);
  gcDrawRectangle(g, 50, 200, 200, 100);
  //使用自定义线型
  gcLineDash(g, 0, dashArray, countof(dashArray));
  gcPenColor(g, ColorBlack);
  gcDrawLine(g, 280, 100, 500, 100);
  gcDashStyle(g, DashStyleSolid);
}
void test_gdiplus4(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  //gcClear(ColorWhite);
  //绘制正方形基线
  gcPen(g, ColorRed, 1);
  gcDrawRectangle(g, 50, 50, 200, 200);
  gcDrawRectangle(g, 300, 50, 200, 200);
  //画圆
  gcPenAlignment(g, PenAlignmentCenter);
  gcPen(g, ColorBlue, 16);
  gcDrawEllipse(g, 50, 50, 200, 200);
  gcPen(g, ColorGreen, 8);
  gcDrawEllipse(g, 50, 50, 200, 200);
  //设置画笔对齐方式为 Inset
  gcPenAlignment(g, PenAlignmentInset);
  //重新画圆
  gcPen(g, ColorBlue, 16);
  gcDrawEllipse(g, 300, 50, 200, 200);
  gcPen(g, ColorGreen, 8);
  gcDrawEllipse(g, 300, 50, 200, 200);
}
void test_gdiplus5(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  int i;
  //gcClear(ColorWhite);
  gcPen(g, ColorRed, 16);
  //设置起点和终点的线帽
  //p.StartCap = LineCapDiamondAnchor;
  //p.EndCap = LineCapRound;
  for (i = 0; i < LineCapCustom; ++i) {
    gcLineCap(g, (LineCap)i, (LineCap)i);
    gcDrawLine(g, 50, 50 + 30 * i, 350, 50 + 30 * i);
  }
}
void test_gdiplus6(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  //gcClear(ColorWhite);
  gcPen(g, ColorRed, 6);
  gcDrawRectangle(g, 50, 50, 200, 100);
  //画笔的缩放
  //将水平方向扩大3倍，垂直方向保持不变
  //gcScaleTransform(g, 3,1);
  //平移绘图平面
  //Path path[1] = {0};
  //PathAddRectangle(path, 50, 50, 200, 100);
  //PathTranslate(path, 300, 0);
  //使用缩放了的画笔绘制图形
  gcDrawRectangle(g, 50, 50, 200, 100);
  //60°旋转
  //PathRotate(g, 60, MatrixOrderAppend);
  //向下平移绘图平面
  //PathTranslate(g, 0,150);
  //使用旋转了的画笔绘制图形
  gcDrawRectangle(g, 50, 50, 200, 100);
}
void test_gdiplus7(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  //gcClear(ColorWhite);
  gcPenAlignment(g, PenAlignmentCenter);
  gcPen(g, ColorRed, 25);
  //设置画笔结合方式为剪切连接
  gcLineJoin(g, LineJoinMiterClipped);
  gcLineJoin(g, LineJoinRound);
  gcDrawRectangle(g, 50, 50, 200, 100);
  //设置画笔结合方式为圆角
  gcLineJoin(g, LineJoinRound);
  gcDrawRectangle(g, 350, 50, 200, 100);
  //设置画笔的结合方式为斜切
  gcLineJoin(g, LineJoinBevel);
  gcDrawRectangle(g, 50, 250, 200, 100);
  //设置画笔的结合方式为斜接
  gcLineJoin(g, LineJoinMiter);
  gcDrawRectangle(g, 350, 250, 200, 100);
}
void test_gdiplus8(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  //gcClear(ColorWhite);
  gcPen(g, ColorBlack, 3);
  //单色画刷(SolidBrush)
  gcSolidBrush(g, _RGB(255, 0, 0));
  //使用SolidBrush 画刷填充第一个矩形并绘制出rect1的边框
  gcFillRectangle(g, 50, 50, 200, 100);
  gcDrawRectangle(g, 50, 50, 200, 100);
  //=============================================================================
  //影线画刷（HatchBrush）
  gcHatchBrush2(g, HatchStyleBackwardDiagonal, _RGB(255, 0, 0), ColorGray);
  //使用 HatchBrush 画刷填充第二个矩形，并绘制出其边框
  gcFillRectangle(g, 300, 50, 200, 100);
  gcDrawRectangle(g, 300, 50, 200, 100);
#if 0
  //=============================================================================
  Rectangle rect3 = 50, 200, 200, 100);
  //纹理画刷(TextureBrush)
  Bitmap bmp = (Bitmap)Image.FromFile(@"C:\Users\ThinkPad\Desktop\GDIPuls Brush\Start.png");
  TextureBrush tb = new TextureBrush(bmp);
  //绘制原图
  gcDrawImage(bmp, 50, 320);
  //填充矩形3并绘制矩形边框
  gcFillRectangle(g, rect3);
  gcDrawRectangle(g, rect3);
#endif
  //===============================================================================
  //线性渐变画刷（LinearGradientBrush）
  gcGradientBrushRectS(g, 300, 200, 200, 100, ColorGreen, ColorYellow, LinearGradientModeForwardDiagonal);
  //填充矩形4并绘制其边框
  gcFillRectangle(g, 300, 200, 200, 100);
  gcDrawRectangle(g, 300, 200, 200, 100);
#if 0
  //===============================================================================
  Rectangle rect5 = 550, 200, 200, 100);
  //路径渐变画刷(PathGradientBrush)
  Point[] pts = {
    rect5.Left, rect5.Top,
    rect5.Right, rect5.Top,
    rect5.Right, rect5.Bottom,
    rect5.Left, rect5.Bottom
  };
  PathGradientBrush pb = new PathGradientBrush(pts);
  //设置中心的颜色
  pb.CenterColor = ColorGreen;
      //填充矩形5并绘制其边框
      gcFillRectangle(pb, rect5);
      gcDrawRectangle(p, rect5);
#endif
}
    void test_gdiplus9(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  //定义曲线、多边形端点坐标
  FPOINT closedCurvePts[] = {
    100.0f, 250.0f,
      200.0f, 200.0f,
      250.0f, 350.0f,
      50.0f, 300.0f,
      100.0f, 250.0f
  };
  //gcClear(ColorWhite);
  //构建绿色的单色画刷
  gcSolidBrush(g, ColorGreen);
  //填充椭圆
  gcFillEllipse(g, 50, 50, 200, 100);
  //使用 .NET 内置的单身画刷
  gcSolidBrush(g, ColorRed);
  gcFillRectangle(g, 300, 50, 200, 100);
  //定义曲线端点并填充
  //gcFillClosedCurve(g,closedCurvePts);
  gcSolidBrush(g, ColorGreen);
  gcFillPolygon(g, closedCurvePts, countof(closedCurvePts), 1, GC_FLOAT, sizeof(FPOINT));
  //定义多边形端点并填充
  //gcTranslate(g, 300,0);
  gcSolidBrush(g, ColorLightBlue);
  gcFillPolygon(g, closedCurvePts, countof(closedCurvePts), 1, GC_FLOAT, sizeof(FPOINT));
  //gcResetPath(g);
}
void test_gdiplus10(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  //gcClear(ColorWhite);
  //构建第一种影线画刷
  gcHatchBrush2(g, HatchStyleVertical, ColorRed, ColorWhite);
  gcFillRectangle(g, 10, 10, 200, 100);
  //构建第二种画刷
  gcHatchBrush2(g, HatchStyleHorizontal, ColorRed, ColorLightGray);
  gcFillRectangle(g, 250, 10, 200, 100);
  //构建第三种画刷
  gcHatchBrush2(g, HatchStyleBackwardDiagonal, ColorGreen, ColorLightYellow);
  gcFillRectangle(g, 10, 150, 200, 100);
  //构建第四种画刷
  gcHatchBrush2(g, HatchStyleForwardDiagonal, ColorBlack, ColorRed);
  gcFillRectangle(g, 250, 150, 200, 100);
}
void test_gdiplus11(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  //gcClear(ColorWhite);
  //Rectangle rect1 = 10,10,300,300);
  //Rectangle rect2 = 350,10,300,300);
  //导入图片
  
  static img_t im[1] = {0};
  texture_t img[1] = {0};
  if (!im->tt.data) {
    imread("E:/pub/pic/Nemo.jpg", 3, 1, im);
  }
  BMPINITIM(img, im);
  img->fmt = PixelFormat24bppRGB;
  
  gcPen(g, ColorGreen, 3);
  gcWrapMode(g, WrapModeTile, WrapModeTile);
  //使用图片的原始大小构建纹理画刷
  gcTextureBrush(g, img, NULL);
  //填充椭圆并绘制其边框
  gcFillEllipse(g, 10, 10, 300, 300);
  gcDrawEllipse(g, 10, 10, 300, 300);
  //使用图片的部分区域构建纹理画刷
  gcTextureBrushRect(g, img, 55, 35, 55, 35, NULL);
  //填充椭圆并绘制其边框
  gcFillEllipse(g, 350, 10, 300, 300);
  gcDrawEllipse(g, 350, 10, 300, 300);
}
void test_gdiplus12(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  //装入纹理图片
  static img_t im[1] = {0};
  texture_t bmp[1] = {0};
  const float matrix[] = {0.5f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f};
  int i = 0;
  if (!im->tt.data) {
    imread("E:/pub/pic/Nemo.jpg", 3, 1, im);
  }
  BMPINITIM(bmp, im);
  bmp->fmt = PixelFormat24bppRGB;

  //gcClear(ColorWhite);
  gcPen(g, ColorBlue, 3);
  gcSolidBrush(g, ColorBlack);
  //Font myFont = new Font("Arial", 13);
  //构建纹理画刷
  //对画刷进行缩放，以便查看效果
  gcTextureBrush(g, bmp, matrix);
  //对图片不使用排列方式
  gcWrapMode(g, WrapModeClamp, WrapModeClamp);
  gcFillRectangle(g, i * 150, 10, 150, 150);
  gcDrawRectangle(g, i * 150, 10, 150, 150);
  //gcDrawString("Clamp", myFont, sBrush, new PointF(0, 170));
  i += 1;
  //对图片使用平铺排列方式
  gcWrapMode(g, WrapModeTile, WrapModeTile);
  gcFillRectangle(g, i * 150 + 20, 10, 150, 150);
  gcDrawRectangle(g, i * 150 + 20, 10, 150, 150);
  //gcDrawString("Tile",myFont,sBrush,new PointF(180,170));
  i += 1;
  //对图片使用水平翻转排列方式
  gcWrapMode(g, WrapModeTileFlip, WrapModeTile);
  gcFillRectangle(g, i * 150 + 40, 10, 150, 150);
  gcDrawRectangle(g, i * 150 + 40, 10, 150, 150);
  //gcDrawString("TileFlipX", myFont, sBrush, new PointF(360, 170));
  //对图片使用垂直翻转排列方式
  gcWrapMode(g, WrapModeTile, WrapModeTileFlip);
  gcFillRectangle(g, 10, 200, 150, 150);
  gcDrawRectangle(g, 10, 200, 150, 150);
  //gcDrawString("TileFlipX", myFont, sBrush, new PointF(10, 360));
  //对图片使用水平、垂直同时翻转排列方式
  gcWrapMode(g, WrapModeTileFlip, WrapModeTileFlip);
  gcFillRectangle(g, 180, 200, 150, 150);
  gcDrawRectangle(g, 180, 200, 150, 150);
  //gcDrawString("TileFlipXY", myFont, sBrush, new PointF(180, 360));
}
void test_gdiplus13(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  //gcClear(ColorWhite);
  gcPen(g, ColorBlack, 3);
  gcWrapMode(g, WrapModeReflect, WrapModeReflect);
  //gcWrapMode(g, );
  //使用2个点定义渐变区域
  gcGradientBrushS(g, 10, 10, 10, 50, ColorRed, ColorBlue);//终点颜色
  //填充矩形区域并绘制其边框
  gcFillRectangle(g, 10, 10, 200, 100);
  gcDrawRectangle(g, 10, 10, 200, 100);
  //使用矩形来定义渐变区域
  gcGradientBrushRectS(g, 250, 10, 200, 100, ColorPink, ColorGreen, LinearGradientModeBackwardDiagonal);
  //填充矩形区域并绘制其边框
  gcFillRectangle(g, 250, 10, 200, 100);
  gcDrawRectangle(g, 250, 10, 200, 100);
  //使用指定的偏转角度来构建画刷
  gcGradientBrushRectAngle(g, 500, 10, 200, 100, ColorLightYellow, ColorRed, 60.0f, true);
  gcFillRectangle(g, 500, 10, 200, 100);
  gcDrawRectangle(g, 500, 10, 200, 100);
}
void test_gdiplus14(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  //gcClear(ColorWhite);
  //定义渐变颜色数组
  COLOR colors[] = { ColorRed, ColorGreen, ColorBlue };
  float positions[] = { 0.0f, 0.3f, 1.0f };
  //定义ColorBlend对象
  //定义线型渐变画刷
  gcGradientBrushColorsRect(g, 10, 10, 200, 100, LinearGradientModeHorizontal, positions, colors, countof(positions));
  //设置渐变画刷的多色渐变信息
  //lBrush.InterpolationColors = colorBlend;
  gcFillRectangle(g, 10, 10, 200, 100);
}
void test_gdiplus15(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  //gcClear(ColorWhite);
  //构造五星的10个边的端点坐标
  FPOINT points[] = {
    75, 0,
    100, 50,
    150, 50,
    112, 75,
    150, 150,
    75, 100,
    0, 150,
    37, 75,
    0, 50,
    50, 50,
  };
  COLOR colors[] = {
    ColorBlack, ColorGreen,
    ColorBlue, ColorWhite,
    ColorBlack, ColorGreen,
    ColorBlue, ColorWhite,
    ColorBlack, ColorGreen
  };
  //创建路径
  Path path[1] = {0};
  //在路径中添加直线
  PathAddPolygon(path, points, countof(points), true);
  PathTranslate(path, 100, 100);
  //创建路径渐变画刷
  //设置中心点颜色
  //设置每个端点的色彩
  //设置路径渐变画刷的边缘色
  //gcPathGradientBrush2(g, path, colors, countof(colors), NULL, ColorRed);
  //gcPathRadialBrush4(g, path, ColorRed, ColorBlue, NULL);
  gcPathRadialBrush3(g, path, countof(colors), colors, NULL, NULL);
  gcWrapMode2(g, WrapModeTileFlip);
  gcFillPath2(g, path);
  gcFillRect(g, 100, 100, 300, 300);
  PathFree(path);
}
void test_gdiplus16(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  //gcClear(ColorWhite);
  //消除锯齿
  //gcSmoothingMode = SmoothingModeAntiAlias;
#if 0
  FontFamily fontFamily = new FontFamily("宋体");
  Font myFont1 = new Font(fontFamily, 15, FontStyleRegular | UnitPixel);
  g.DrawString("字体系列为宋体，大小为15，字体风格为常规的黑色字体", myFont1, Brushes.Black, new PointF(10, 50));
  Font myFont2 = new Font("Arial", 25.5f, FontStyle.Italic, UnitPixel);
  g.DrawString("字体系列为Arial，大小为25.5，字体风格为倾斜的红色字体", myFont2, Brushes.Red, new PointF(10, 100));
  Font myFont3 = new Font("微软雅黑", 20, FontStyleUnderline, UnitPixel);
  TextRenderer.DrawText(g, "使用TextRenderer类绘制字符串", myFont3, new Point(10, 150), ColorGreen);
#endif
}
void test_gdiplus17(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
#if 0
  Graphics g = e.Graphics;
  g.Clear(ColorWhite);
  g.SmoothingMode = SmoothingMode.AntiAlias;
  //定义输出字体
  Font myFont = new Font("Arial", 15, FontStyle.Regular, GraphicsUnit.Pixel);
  //定义文本输出格式
  StringFormat strFmt = new StringFormat();
  strFmt.Alignment = StringAlignment.Near;
  strFmt.LineAlignment = StringAlignment.Near;
  string tempStr = string.Empty;
  //获取系统所有已安装的字体系列
  InstalledFontCollection installedCollection = new InstalledFontCollection();
  FontFamily[] fontFamilyArray = installedCollection.Families;
  int index = 0;
  //访问 fontFamilyArray 数组中的每一个成员
  foreach(FontFamily i in fontFamilyArray) {
    //获取当前字体系统的名称
    tempStr += i.Name + ",";
    index++;
  }
  //绘制所有字体
  Rectangle textOutRect = new Rectangle(0, 15, Width, Height);
  g.DrawString(tempStr, myFont, Brushes.Black, textOutRect, strFmt);
  tempStr = string.Format("系统中已经安装的字体有{0}种，其名称分别为：\n", index + 1);
  g.DrawString(tempStr, myFont, Brushes.Red, new PointF(0, 0));
#endif
}
void test_gdiplus18(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;// TODO: Add your command handler code here
  //创建Graphics对象
  //创建20号"楷体"字体
  font_t font[1] = {0};
#if 0
  gcFont("楷体_GB2312", 20, FontStyleRegular, UnitPoint);//定义输出UNICODE字符串
  char* string = "天极网的读者朋友，您好！";//以蓝色画刷和20号"楷体"显示字符串
  int len = strlen(string);
  gcSolidBrush(g, ColorBlue)
  gcDrawString(g, string, len, 30, 10);//定义字符串显示画刷
  gcGradientBrush(g, 30, 50, 100, 50, _RGBA(255, 255, 0, 0), _RGBA(255, 0, 0, 255)); //以线性渐变画刷和创建的20号"楷体"显示字符串
  gcDrawString(g, string, len, 30, 50);//创建20号"华文行楷"字体
  gcFont("华文行楷", 20, FontStyleRegular, UnitPoint);//以线性渐变画刷和20号"华文行楷"显示字符串
  gcDrawString(g, string, len, 30, 90);//以图像创建画刷
  Image image(L "d://3.jpg");
  gcTextureBrush(&image);//以图像画刷和20号"华文行楷"显示字符串
  gcDrawString(g, string, len, 30, 130);//创建25号"华文中宋"字体
  gcFont("华文中宋", 25, FontStyleRegular, UnitPoint);//以图像画刷和20号"华文行楷"显示字符串
  gcDrawString(g, string, len, 30, 170);
#endif
}
#if 0
//GdiPlus[16]: IGPLinearGradientBrush 之 SetBlendBellShape、SetBlendTriangularShape
//SetBlendBellShape 方法: 创建基于钟形曲线的渐变过渡过程;
//SetBlendTriangularShape 方法: 创建一个从中心色向两端单个颜色线性过渡的线性渐变过程.
//两个方法好像区别不大.
//SetBlendBellShape 测试效果图:
//SetBlendTriangularShape 测试效果图:
//SetBlendBellShape 测试代码:
void test_gdiplus19(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  IRECT Rect = iRECT2(10, 10, 120, 150);
  gcGradientBrushRectAngle2(g, Rect, 0xFFFF0000, 0xFF0000FF, 0);
  Brush.SetBlendBellShape(1);
  Graphics.FillEllipse(Brush, Rect);
  Brush.SetBlendBellShape(0.8);
  Graphics.TranslateTransform(Rect.X + Rect.Width, 0);
  Graphics.FillEllipse(Brush, Rect);
  Brush.SetBlendBellShape(0.5);
  Graphics.TranslateTransform(Rect.X + Rect.Width, 0);
  Graphics.FillEllipse(Brush, Rect);
  Brush.SetBlendBellShape(0.2);
  Graphics.TranslateTransform(Rect.X + Rect.Width, 0);
  Graphics.FillEllipse(Brush, Rect);
  Brush.SetBlendBellShape(0);
  Graphics.TranslateTransform(Rect.X + Rect.Width, 0);
  Graphics.FillEllipse(Brush, Rect);
  //
  Graphics.TranslateTransform(-Graphics.Transform.OffsetX, Rect.Y + Rect.Height);
  Brush.SetBlendBellShape(1, 1);
  Graphics.FillEllipse(Brush, Rect);
  Brush.SetBlendBellShape(1, 0.8);
  Graphics.TranslateTransform(Rect.X + Rect.Width, 0);
  Graphics.FillEllipse(Brush, Rect);
  Brush.SetBlendBellShape(1, 0.5);
  Graphics.TranslateTransform(Rect.X + Rect.Width, 0);
  Graphics.FillEllipse(Brush, Rect);
  Brush.SetBlendBellShape(1, 0.2);
  Graphics.TranslateTransform(Rect.X + Rect.Width, 0);
  Graphics.FillEllipse(Brush, Rect);
  Brush.SetBlendBellShape(1, 0);
  Graphics.TranslateTransform(Rect.X + Rect.Width, 0);
  Graphics.FillEllipse(Brush, Rect);
}
//SetBlendTriangularShape 测试代码:
void test_gdiplus20(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
Graphics :
    = TGPGraphics.Create(Canvas.Handle);
  Rect.Initialize(10, 10, 120, 150);
Brush :
    = TGPLinearGradientBrush.Create(Rect, 0xFFFF0000, 0xFF0000FF, 0);
  Brush.SetBlendBellShape(1);
  Graphics.FillEllipse(Brush, Rect);
  Brush.SetBlendTriangularShape(0.8);
  Graphics.TranslateTransform(Rect.X + Rect.Width, 0);
  Graphics.FillEllipse(Brush, Rect);
  Brush.SetBlendTriangularShape(0.5);
  Graphics.TranslateTransform(Rect.X + Rect.Width, 0);
  Graphics.FillEllipse(Brush, Rect);
  Brush.SetBlendTriangularShape(0.2);
  Graphics.TranslateTransform(Rect.X + Rect.Width, 0);
  Graphics.FillEllipse(Brush, Rect);
  Brush.SetBlendTriangularShape(0);
  Graphics.TranslateTransform(Rect.X + Rect.Width, 0);
  Graphics.FillEllipse(Brush, Rect);
  //
  Graphics.TranslateTransform(-Graphics.Transform.OffsetX, Rect.Y + Rect.Height);
  Brush.SetBlendTriangularShape(1, 1);
  Graphics.FillEllipse(Brush, Rect);
  Brush.SetBlendTriangularShape(1, 0.8);
  Graphics.TranslateTransform(Rect.X + Rect.Width, 0);
  Graphics.FillEllipse(Brush, Rect);
  Brush.SetBlendTriangularShape(1, 0.5);
  Graphics.TranslateTransform(Rect.X + Rect.Width, 0);
  Graphics.FillEllipse(Brush, Rect);
  Brush.SetBlendTriangularShape(1, 0.2);
  Graphics.TranslateTransform(Rect.X + Rect.Width, 0);
  Graphics.FillEllipse(Brush, Rect);
  Brush.SetBlendTriangularShape(1, 0);
  Graphics.TranslateTransform(Rect.X + Rect.Width, 0);
  Graphics.FillEllipse(Brush, Rect);
}
#endif
void test_gdiplus21(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  IRECT rc, Rect = iRECT2(30, 10, 150, 200);
  // 实心画刷
  rc = Rect;
  gcSolidBrush(g, 0xFFFF0000);
  gcFillEllipseRect(g, rc);
  // 阴影画刷
  rc = iRectOffset2(Rect, 250, 0);
  gcHatchBrush2(g, HatchStyleDiagonalCross, 0xFF0000FF, 0xFFCCCCCC);
  gcFillEllipseRect(g, rc);
  //rc = iRectOffset2(Rect, 250, 0);
  // gcTextureBrush('C:\GdiPlusImg\Texture.jpg');
  //FillEllipse(Brush, rc);
  rc = iRectOffset2(Rect, 0, 250);
  gcGradientBrushRectAngle2(g, rc, 0xFFFF0000, 0xFF0000FF, 90);
  gcFillEllipseRect(g, rc);
  rc = iRectOffset2(Rect, 250, 250);
  gcRadialBrushRectS2(g, rc, 0xFFFF0000, 0xFFFFFF00);
  gcFillEllipseRect(g, rc);
  return ;
}
#if 0
LineCapAnchorMask 指定用于检查线帽是否为锚头帽的掩码。
LineCapArrowAnchor 指定箭头状锚头帽。
LineCapCustom 指定自定义线帽。
LineCapDiamondAnchor 指定菱形锚头帽。
LineCapFlat 指定平线帽。
LineCapNoAnchor 指定没有锚。
LineCapRound 指定圆线帽。
LineCapRoundAnchor 指定圆锚头帽。
LineCapSquare 指定方线帽。
LineCapSquareAnchor 指定方锚头帽。
LineCapTriangle 指定三角线帽。
#endif
//画笔线帽 - Cap
void test_gdiplus22(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  gcPen(g, ColorBlue, 8);
  gcLineCap(g, LineCapArrowAnchor, LineCapRoundAnchor);//{起点线帽, 终点线帽}
  gcDrawLine(g, 20, 20, 300, 175);
}
// kCGPathStroke kCGPathFill
void test_gdiplus23(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  Path path[1] = {0};
  // An opaque type that represents a Quartz 2D drawing environment.
  //Graphics Context是图形上下文,可以将其理解为一块画布,我们可以在上面进行绘画操作,绘制完成后,将画布放到我们的view中显示即可,view看作是一个画框.
  //自己学习时实现的demo，希望对大家有帮助，具体的实现看代码，并有完美的注释解释，还有一些对我帮助的博文供大家参考。都在代码里面。
  // 覆盖drawRect方法，你可以在此自定义绘画和动画
  //An opaque type that represents a Quartz 2D drawing environment.
  //一个不透明类型的Quartz 2D绘画环境,相当于一个画布,你可以在上面任意绘画
  /*写文字*/
  COLOR crtext = _rgba(1, 0, 0, 1.0);//设置填充颜色
  COLOR colors[] = {ColorWhite, ColorGray, ColorBlack, ColorYellow, ColorBlue, ColorRed, ColorGreen, ColorOrange, ColorBrown,};
  IRECT rc;
  //UIFont *font = [UIFont boldSystemFontOfSize:15.0];//设置
  gcSolidRect(g, 0, 0, 500, 500, ColorBlack);
  gcSolidText(g, iRECT2(10, 20, 180, 20), "画圆： ", NULL, io->font, 0, TF_VCENTER, crtext);
  gcSolidText(g, iRECT2(10, 80, 180, 20), "画线及孤线： ", NULL, io->font, 0, TF_VCENTER, crtext);
  gcSolidText(g, iRECT2(10, 120, 180, 20), "画矩形： ", NULL, io->font, 0, TF_VCENTER, crtext);
  gcSolidText(g, iRECT2(10, 160, 180, 20), "画扇形和椭圆： ", NULL, io->font, 0, TF_VCENTER, crtext);
  gcSolidText(g, iRECT2(10, 220, 180, 20), "画三角形： ", NULL, io->font, 0, TF_VCENTER, crtext);
  gcSolidText(g, iRECT2(10, 260, 180, 20), "画圆角矩形： ", NULL, io->font, 0, TF_VCENTER, crtext);
  gcSolidText(g, iRECT2(10, 300, 180, 20), "画贝塞尔曲线： ", NULL, io->font, 0, TF_VCENTER, crtext);
  gcSolidText(g, iRECT2(10, 340, 180, 20), "图片： ", NULL, io->font, 0, TF_VCENTER, crtext);
  /*画圆*/
  //边框圆
  gcPen(g, _rgba(1, 1, 1, 1.0), 1); //画笔线的颜色 线的宽度
  //void PathAddArc(CGContextRef c,float x, float y,float radius,float startAngle,float endAngle, int clockwise)1弧度＝180°/π （≈57.3°） 度＝弧度×180°/π 360°＝360×π/180 ＝2π 弧度
  // x,y为圆点坐标，radius半径，startAngle为开始的弧度，endAngle为 结束的弧度，clockwise 0为顺时针，1为逆时针。
  PathAddArc2(path, 100, 20, 15, 15, 0, 360);//添加一个圆
  gcDrawPath2(g, path);//绘制路径
  //填充圆，无边框
  PathReset(path);
  PathAddArc2(path, 150, 30, 30, 30, 0, 360);//添加一个圆
  gcFillPath2(g, path);//绘制填充
  //画大圆并填充颜
  gcSolidBrush(g, _rgba(1, 0, 0, 1));//填充颜色
  gcLineWidth(g, 3.0);//线的宽度
  PathReset(path);
  PathAddArc2(path, 250, 40, 40, 40, 0, 360);//添加一个圆
  //kCGPathFill填充非零绕数规则,kCGPathEOFill表示用奇偶规则,kCGPathStroke路径,kCGPathFillStroke路径填充,kCGPathEOFillStroke表示描线，不是填充
  gcPath2(g, GcOptFillStroke, path);//绘制路径加填充
  /*画线及孤线*/
  //画线
  {
    FPOINT aPoints[2] = { 100, 80, 130, 80 };//坐标2
  //PathAddLines(CGContextRef c, const FPOINT points[],size_t count)
  //points[]坐标数组，和count大小
  PathReset(path);
  PathAddLines(path, aPoints, 2);//添加线
  }
  gcPath2(g, GcOptStroke, path);//根据坐标绘制路径
  //画笑脸弧线
  gcPenColor(g, _rgba(0, 0, 1, 1));//改变画笔颜色
  PathReset(path);
  //PathAddArcToPoint(CGContextRef c, float x1, float y1,float x2, float y2, float radius)
  //x1,y1跟p1形成一条线的坐标p2，x2,y2结束坐标跟p3形成一条线的p3,radius半径,注意, 需要算好半径的长度,
  //左眼
  PathMoveTo(path, 140, 80);
  PathAddArcToPoint(path, 140, 80, 156, 80, 10, false);
  //右眼
  PathMoveTo(path, 160, 80);
  PathAddArcToPoint(path, 160, 80, 176, 80, 10, false);
  //嘴巴
  PathMoveTo(path, 150, 90);
  PathAddArcToPoint(path, 150, 90, 166, 90, 10, true);
  gcStrokePath2(g, path);//绘画路径
  //注，如果还是没弄明白怎么回事，请参考：http://donbe.blog.163.com/blog/static/138048021201052093633776/
  //画矩形
  PathReset(path);
  gcStrokeRect(g, 100, 120, 10, 10); //画方框
  gcFillRect(g, 120, 120, 10, 10); //填充框
  //矩形，并填弃颜色
  gcLineWidth(g, 2.0);//线的宽度
  gcSolidBrush(g, ColorBlue);//填充蓝色
  gcStrokeColor(g, ColorYellow);//线框颜色
  PathAddRect(path, 140, 120, 60, 30);//画方框
  gcPath2(g, GcOptFillStroke, path);//绘画路径
  //矩形，并填弃渐变颜色
  //关于颜色参考http://blog.sina.com.cn/s/blog_6ec3c9ce01015v3c.html
  //http://blog.csdn.net/reylen/article/details/8622932
  //第一种填充方式，第一种方式必须导入类库quartcore并#import <QuartzCore/QuartzCore.h>，这个就不属于在g上画，而是将层插入到view层上面。那么这里就设计到Quartz Core 图层编程了。
  
  rc = iRECT2(240, 120, 60, 30);
  gcGradientBrushRect2(g, rc, NULL, colors, countof(colors), LinearGradientModeVertical);
  gcFillRect2(g, rc);
  
  //第二种填充方式
  {
    COLOR colors[] = {
      _rgba(1, 1, 1, 1),
      _rgba(1, 1, 0, 1),
      _rgba(1, 0, 0, 1),
      _rgba(1, 0, 1, 1),
      _rgba(0, 1, 1, 1),
      _rgba(0, 1, 0, 1),
      _rgba(0, 0, 1, 1),
      _rgba(0, 0, 0, 1),
    };
    //形成梯形，渐变的效果
  }
  //画线形成一个矩形
  //CGContextSaveGState与CGContextRestoreGState的作用
  //CGContextSaveGState函数的作用是将当前图形状态推入堆栈。之后，您对图形状态所做的修改会影响随后的描画操作，但不影响存储在堆栈中的拷贝。在修改完成后，您可以通过CGContextRestoreGState函数把堆栈顶部的状态弹出，返回到之前的图形状态。这种推入和弹出的方式是回到之前图形状态的快速方法，避免逐个撤消所有的状态修改；这也是将某些状态（比如裁剪路径）恢复到原有设置的唯一方式。
  //CGContextSaveGState(g);
  PathReset(path);
  PathMoveTo(path, 220, 90);
  PathLineTo(path, 240, 90);
  PathLineTo(path, 240, 110);
  PathLineTo(path, 220, 110);
  PathCloseFigure(path);
  //CGContextClip(g);//g裁剪路径,后续操作的路径
  //CGContextDrawLinearGradient(CGContextRef g,CGGradientRef gradient, FPOINT startPoint, FPOINT endPoint,CGGradientDrawingOptions options)
  //gradient渐变颜色,startPoint开始渐变的起始位置,endPoint结束坐标,options开始坐标之前or开始之后开始渐变
  gcGradientBrush(g, 220, 90, 240, 110, NULL, colors, countof(colors));
  gcFillPath2(g, path);
  //再写一个看看效果
  //CGContextSaveGState(g);
  PathReset(path);
  PathMoveTo(path, 260, 90);
  PathLineTo(path, 280, 90);
  PathLineTo(path, 280, 100);
  PathLineTo(path, 260, 100);
  PathCloseFigure(path);
  //gcClip(g);//裁剪路径
  //说白了，开始坐标和结束坐标是控制渐变的方向和形状
  gcGradientBrush(g, 260, 90 , 260, 100, NULL, colors, countof(colors));
  gcFillPath2(g, path);
  //下面再看一个颜色渐变的圆
  rc = iRECT2(300, 100, 20, 20);
  gcRadialBrushRect2(g, rc, NULL, colors, countof(colors));
  gcFillEllipseRect(g, rc);
  /*画扇形和椭圆*/
  //画扇形，也就画圆，只不过是设置角度的大小，形成一个扇形
  gcSolidBrush(g, _rgba(0, 1, 1, 1));//填充颜色
  //以10为半径围绕圆心画指定角度扇形
  PathReset(path);
  PathMoveTo(path, 160, 180);
  PathAddArc2(path, 160, 180, 30, 30, -60, -60);
  PathCloseFigure(path);
  gcPath2(g, GcOptFillStroke, path);//绘制路径
  //画椭圆
  PathReset(path);
  PathAddEllipse(path, 160, 180, 20, 8);//椭圆
  gcPath2(g, GcOptFillStroke, path);
  /*画三角形*/
  //只要三个点就行跟画一条线方式一样，把三点连接起来
  //坐标点
  {
    FPOINT sPoints[3] = {100, 220, 130, 220, 130, 160,};
  PathAddLines(path, sPoints, 3);//添加线
  }
  PathCloseFigure(path);//封起来
  gcPath2(g, GcOptFillStroke, path);//根据坐标绘制路径
  /*画圆角矩形*/
  PathReset(path);
  PathAddRoundRectangle(path, 120, 250, 60, 30, 10, 10, 10, 10);
  gcPath2(g, GcOptFillStroke, path);//根据坐标绘制路径
  //画贝塞尔曲线
  //二次曲线
  PathReset(path);
  PathMoveTo(path, 120, 300);//设置Path的起点
  PathQuadTo(path, 190, 310, 120, 390); //设置贝塞尔曲线的控制点坐标和终点坐标
  //三次曲线函数
  PathMoveTo(path, 200, 300);//设置Path的起点
  PathCubicTo(path, 250, 280, 250, 400, 280, 300);//设置贝塞尔曲线的控制点坐标和控制点坐标终点坐标
  gcPath2(g, GcOptStroke, path);
#if 0
  //图片
UIImage* image = [UIImage imageNamed: @"apple.jpg"];
[image drawInRect: CGRectMake(60, 340, 20, 20)]; //在坐标中画出图片
  // [image drawAtPoint:fPOINT(100, 340)];//保持图片大小在point点开始画图片，可以把注释去掉看看
  CGContextDrawImage(g, CGRectMake(100, 340, 20, 20), image.CGImage);//使用这个使图片上下颠倒了，参考http://blog.csdn.net/koupoo/article/details/8670024
  // CGContextDrawTiledImage(g, CGRectMake(0, 0, 20, 20), image.CGImage);//平铺图
#endif
  PathFree(path);
}
void test_gdiplus24(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  Path path[1] = {0};
  //用Ellipses和Arcs绘制曲线
  gcSolidRect(g, 0, 0, 500, 500, ColorBlack);
  // Drawing with a white stroke color
  gcPenColor(g, _rgba(1.0, 1.0, 1.0, 1.0));
  // And draw with a blue fill color
  gcSolidBrush(g, _rgba(0.0, 0.0, 1.0, 1.0));
  // Draw them with a 2.0 stroke width so they are a bit more visible.
  gcLineWidth(g, 2.0);
  // Add an ellipse circumscribed in the given rect to the current path, then stroke it
  PathAddEllipse(path, 30.0, 30.0, 60.0, 60.0);
  gcStrokePath2(g, path);
  // Stroke ellipse convenience that is equivalent to AddEllipseInRect(); StrokePath();
  gcDrawEllipse(g, 30.0, 120.0, 60.0, 60.0);
  // Fill rect convenience equivalent to AddEllipseInRect(); FillPath();
  gcFillEllipse(g, 30.0, 210.0, 60.0, 60.0);
  // Stroke 2 seperate arcs
  PathReset(path);
  PathAddArc2(path, 150.0, 60.0, 30.0, 30.0, 0.0, 90);
  gcStrokePath2(g, path);
  PathReset(path);
  PathAddArc2(path, 150.0, 60.0, 30.0, 30.0, 270, -90);
  gcStrokePath2(g, path);
  // Stroke 2 arcs together going opposite directions.
  PathReset(path);
  PathAddArc2(path, 150.0, 150.0, 30.0, 30.0, 0, 90);
  PathAddArc2(path, 150.0, 150.0, 30.0, 30.0, 270, -90);
  gcStrokePath2(g, path);
  // Stroke 2 arcs together going the same direction..
  PathReset(path);
  PathAddArc2(path, 150.0, 240.0, 30.0, 30.0, 0, 90);
  PathAddArc2(path, 150.0, 240.0, 30.0, 30.0, 180, 90);
  gcStrokePath2(g, path);
  // Stroke an arc using AddArcToPoint
  PathReset(path);
  PathAddArcToPoint(path, 210.0, 30.0, 240.0, 60.0, 30.0, true);
  gcStrokePath2(g, path);
  // Show the two segments that are used to determine the tangent lines to draw the arc.
  //gcPenColor(g, _rgba(1.0, 0.0, 0.0, 1.0));
  //PathReset(path);
  //PathAddLines(path, p, sizeof(p)/sizeof(p[0]));
  //gcStrokePath2(g, path);
  // As a bonus, we'll combine arcs to create a round rectangle!
  // Drawing with a white stroke color
  gcPenColor(g, _rgba(1.0, 1.0, 1.0, 1.0));
  // If you were making this as a routine, you would probably accept a rectangle
  // that defines its bounds, and a radius reflecting the "rounded-ness" of the rectangle.
  // NOTE: At this point you may want to verify that your radius is no more than half
  // the width and height of your rectangle, as this technique degenerates for those cases.
  // In order to draw a rounded rectangle, we will take advantage of the fact that
  // PathAddArcToPoint will draw straight lines past the start and end of the arc
  // in order to create the path from the current position and the destination position.
  // In order to create the 4 arcs correctly, we need to know the min, mid and max positions
  // on the x and y lengths of the given rectangle.
  // Next, we will go around the rectangle in the order given by the figure below.
  // minx midx maxx
  // miny 2 3 4
  // midy 1 9 5
  // maxy 8 7 6
  // Which gives us a coincident start and end point, which is incidental to this technique, but still doesn't
  // form a closed path, so we still need to close the path to connect the ends correctly.
  // Thus we start by moving to point 1, then adding arcs through each pair of points that follows.
  // You could use a similar tecgnique to create any shape with rounded corners.
  // Start at 1
  // Close the path
  PathReset(path);
  {
    float radius = 10.0;
  PathAddRoundRectangle(path, 210.0, 90.0, 60.0, 60.0, radius, radius, radius, radius);
  }
  // Fill & stroke the path
  gcPath2(g, GcOptFillStroke, path);
  PathFree(path);
}
//绘制Beziers和Quadratics曲线
void test_gdiplus25(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  FPOINT s = fPOINT(30.0, 120.0);
  FPOINT e = fPOINT(300.0, 120.0);
  FPOINT cp1 = fPOINT(120.0, 30.0);
  FPOINT cp2 = fPOINT(210.0, 210.0);
  Path path[1] = {0};

  //用Ellipses和Arcs绘制曲线
  gcSolidRect(g, 0, 0, 500, 500, ColorBlack);
  // Drawing with a white stroke color
  gcPenColor(g, _rgba(1.0, 1.0, 1.0, 1.0));
  // Draw them with a 2.0 stroke width so they are a bit more visible.
  gcLineWidth(g, 2.0);
  // Draw a bezier curve with end points s,e and control points cp1,cp2
  PathMoveTo(path, s.x, s.y);
  PathCurveTo(path, cp1.x, cp1.y, cp2.x, cp2.y, e.x, e.y);
  gcStrokePath2(g, path);
  // Show the control points.
  gcPenColor(g, _rgba(1.0, 0.0, 0.0, 1.0));
  PathReset(path);
  PathMoveTo(path, s.x, s.y);
  PathLineTo(path, cp1.x, cp1.y);
  PathMoveTo(path, e.x, e.y);
  PathLineTo(path, cp2.x, cp2.y);
  gcStrokePath2(g, path);
  // Draw a quad curve with end points s,e and control point cp1
  gcPenColor(g, _rgba(1.0, 1.0, 1.0, 1.0));
  s = fPOINT(30.0, 300.0);
  e = fPOINT(270.0, 300.0);
  cp1 = fPOINT(150.0, 180.0);
  PathReset(path);
  PathMoveTo(path, s.x, s.y);
  PathQuadTo(path, cp1.x, cp1.y, e.x, e.y);
  gcStrokePath2(g, path);
  // Show the control point.
  gcPenColor(g, _rgba(1.0, 0.0, 0.0, 1.0));
  PathReset(path);
  PathMoveTo(path, s.x, s.y);
  PathLineTo(path, cp1.x, cp1.y);
  PathLineTo(path, e.x, e.y);
  gcStrokePath2(g, path);
  PathFree(path);
}
// 绘制矩形
void test_gdiplus26(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  Path path[1] = {0};
  gcSolidRect(g, 0, 0, 500, 500, ColorBlack);
  // Drawing with a white stroke color
  gcStrokeColor(g, _rgba(1.0, 1.0, 1.0, 1.0));
  // And drawing with a blue fill color
  gcFillColor(g, _rgba(0.0, 0.0, 1.0, 1.0));
  // Draw them with a 2.0 stroke width so they are a bit more visible.
  gcLineWidth(g, 2.0);
  // Add Rect to the current path, then stroke it
  PathAddRect(path, 30.0, 30.0, 60.0, 60.0);
  gcStrokePath2(g, path);
  // Stroke Rect convenience that is equivalent to above
  gcStrokeRect(g, 30.0, 120.0, 60.0, 60.0);
  // Stroke rect convenience equivalent to the above, plus a call to gcLineWidth().
  gcLineWidth(g, 10);
  gcStrokeRect(g, 30.0, 210.0, 60.0, 60.0);
  // Demonstate the stroke is on both sides of the path.
  
  //red
  gcStrokeColor(g, _rgba(1.0, 0.0, 0.0, 1.0));
  gcLineWidth(g, 2);
  gcStrokeRect(g, 30.0, 210.0, 60.0, 60.0);

  gcStrokeColor(g, _rgba(1.0, 1.0, 1.0, 1.0));
  {
    FRECT rects[3];
  rects[0] = fRECT2(120.0, 30.0, 60.0, 60.0);
  rects[1] = fRECT2(120.0, 120.0, 60.0, 60.0);
  rects[2] = fRECT2(120.0, 210.0, 60.0, 60.0);
  PathReset(path);
  // Bulk call to add rects to the current path.
  PathAddRectangles(path, rects, countof(rects));
  }
  gcStrokePath2(g, path);
  // Create filled rectangles via two different paths.
  // Add/Fill path
  path->Count = 0;
  PathAddRect(path, 210.0, 30.0, 60.0, 60.0);
  gcFillPath2(g, path);
  // Fill convienience.
  gcFillRect(g, 210.0, 120.0, 60.0, 60.0);

  PathFree(path);
}
// 绘制多边形
void test_gdiplus27(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  int i, j=0, drawingMode[] = {GcOptFill, GcOptStroke, GcOptEOFill, GcOptFillStroke, GcOptEOFillStroke};
  Path path[1] = {0};
  double r = 50;
  gcSolidRect(g, 0, 0, 500, 800, ColorBlack);
  for (j=0; j<countof(drawingMode); ++j) {
    FPOINT center;
    float y;
    // Drawing with a white stroke color
    gcStrokeColor(g, _rgba(1.0, 1.0, 1.0, 1.0));
    // Drawing with a blue fill color
    gcFillColor(g, _rgba(0.0, 0.0, 1.0, 1.0));
    // Draw them with a 2.0 stroke width so they are a bit more visible.
    gcLineWidth(g, 2.0);
    y = j*(r*2+10) + r;
    // Add a star to the current path
    center = fPOINT(90.0, y);
    PathReset(path);
    PathMoveTo(path, center.x, center.y + r);
    for (i = 1; i < 5; ++i)
    {
      float x = r * sinf(i * 4.0 * M_PI / 5.0);
      float y = r * cosf(i * 4.0 * M_PI / 5.0);
      PathLineTo(path, center.x + x, center.y + y);
    }
    // And close the subpath.
    PathCloseFigure(path);
    // Now add the hexagon to the current path
    center = fPOINT(210.0, y);
    PathMoveTo(path, center.x, center.y + r);
    for (i = 1; i < 6; ++i)
    {
      float x = r * sinf(i * 2.0 * M_PI / 6.0);
      float y = r * cosf(i * 2.0 * M_PI / 6.0);
      PathLineTo(path, center.x + x, center.y + y);
    }
    // And close the subpath.
    PathCloseFigure(path);
    // Now draw the star & hexagon with the current drawing mode.
    gcPath2(g, drawingMode[j], path);
  }
  PathFree(path);
}
// 绘制line
void test_gdiplus28(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  FPOINT addLines[] ={10.0, 90.0,70.0, 60.0,130.0, 90.0,190.0, 60.0,250.0, 90.0,310.0, 60.0,};
  FPOINT strokeSegments[] ={10.0, 150.0,70.0, 120.0,130.0, 150.0,190.0, 120.0,250.0, 150.0,310.0, 120.0,};
  Path path[1] = {0};
  gcSolidRect(g, 0, 0, 500, 800, ColorBlack);
  // Drawing lines with a white stroke color
  gcStrokeColor(g, _rgba(1.0, 1.0, 1.0, 1.0));
  // Draw them with a 2.0 stroke width so they are a bit more visible.
  gcLineWidth(g, 2.0);
  // Draw a single line from left to right
  PathMoveTo(path, 10.0, 30.0);
  PathLineTo(path, 310.0, 30.0);
  gcStrokePath2(g, path);
  // Draw a connected sequence of line segments
  PathReset(path);
  // Bulk call to add lines to the current path.
  // Equivalent to MoveToPoint(points[0]); for(i=1; i<count; ++i) AddLineToPoint(points[i]);
  PathAddLines(path, addLines, countof(addLines));
  gcStrokePath2(g, path);
  // Draw a series of line segments. Each pair of points is a segment
  // Bulk call to stroke a sequence of line segments.
  // Equivalent to for(i=0; i<count; i+=2) { MoveToPoint(point[i]); AddLineToPoint(point[i+1]); StrokePath(); }
  PathAddLineSegments(path, strokeSegments, countof(strokeSegments));
  gcStrokePath2(g, path);
  PathFree(path);
}
// 画虚线
void test_gdiplus29(ctrl_t* c)
{
  SYSIO;
  SOFTGC_ARG;
  Path path[1] = {0};
  float dashPhase = 5;
  float dashPattern[] = {10, 10};
  int dashCount = countof(dashPattern);
  PathReset(path);
  gcSolidRect(g, 0, 0, 500, 800, ColorBlack);
  gcStrokeColor(g, _rgba(1.0, 1.0, 1.0, 1.0));
  // Each dash entry is a run-length in the current coordinate system
  // The concept is first you determine how many points in the current system you need to fill.
  // Then you start consuming that many pixels in the dash pattern for each element of the pattern.
  // So for example, if you have a dash pattern of {10, 10}, then you will draw 10 points, then skip 10 points, and repeat.
  // As another example if your dash pattern is {10, 20, 30}, then you draw 10 points, skip 20 points, draw 30 points,
  // skip 10 points, draw 20 points, skip 30 points, and repeat.
  // The dash phase factors into this by stating how many points into the dash pattern to skip.
  // So given a dash pattern of {10, 10} with a phase of 5, you would draw 5 points (since phase plus 5 yields 10 points),
  // then skip 10, draw 10, skip 10, draw 10, etc.
  gcLineDash(g, dashPhase, dashPattern, dashCount);
  PathReset(path);
  // Draw a horizontal line, vertical line, rectangle and circle for comparison
  PathMoveTo(path, 10.0, 20.0);
  PathLineTo(path, 310.0, 20.0);
  PathMoveTo(path, 160.0, 30.0);
  PathLineTo(path, 160.0, 130.0);
  PathAddRect(path, 10.0, 30.0, 100.0, 100.0);
  PathAddEllipse(path, 210.0, 30.0, 100.0, 100.0);
  // And width 2.0 so they are a bit more visible
  gcLineWidth(g, 2.0);
  gcStrokePath2(g, path);
  gcDashStyle(g, DashStyleSolid);
  PathFree(path);
}
#if 0
void test_gdiplus30(ctrl_t* c)
{
}
void test_gdiplus31(ctrl_t* c)
{
}
void test_gdiplus32(ctrl_t* c)
{
}
void test_gdiplus33(ctrl_t* c)
{
}
#endif