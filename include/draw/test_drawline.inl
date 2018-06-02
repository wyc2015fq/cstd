typedef TDrawDC* PGpGraphics;
typedef struct PGpPen {
  COLORREF color;
  double wline;
} PGpPen;
typedef HDDC PGpGraphics;
void DrawAlignmentRect(PGpGraphics g, PGpPen pen, PGpPen basePen, pen_alignment_e alignment, double x, double y)
{
  DPOINT pt[10];
  int n;
  n = vcgen_rect(pt, x + 0, y + 0, x + 100, y + 100);
  drawaa_strokepoly_impl(g, pt, n, alignment, pen.color, pen.wline, round_cap, miter_join, inner_round);
  drawaa_strokepoly_impl(g, pt, n, alignment, basePen.color, basePen.wline, round_cap, miter_join, inner_round);
  //PenSetAlignment(pen, alignment);
  //GraphicsDrawRectangle(g, pen, 0, 0, 100, 100);
  //GraphicsDrawRectangle(g, basePen, 0, 0, 100, 100);
}
void DrawDashStyleLine(PGpGraphics g, PGpPen pen, double x, double y, int cap)
{
  // 自定义线条式样数组，数据含义为线段，空白，线段，空白...，
  // 实际线段长度为数据乘画笔宽度
  int ds;
  DPOINT pt[2] = {0, 0, 300, 0};
  double dashes[][2] = {
    1, 0,
    20, 10,
  };
  poly_offset(pt, 2, x, y);

  for (ds = 0; ds < 2; ds ++) {
    drawaa_dashpoly_impl(g, pt, 2, 0, pen.color, pen.wline, 0, dashes[ds], 2, cap, round_join, inner_round);
    //GraphicsDrawLine(g, pen, 0, 0, 300, 0);
    poly_offset(pt, 2, 0, 20);
  }
}
void test_drawline(PGpGraphics g)
{
  // 定义复合线比例数组，2个数据为1组，组之间的大小为空白，共3条线
  double cp[6] = {0.0f, 0.15f, 0.3f, 0.45f, 0.7f, 1.0f};
  PGpPen redPen = {rgb(1, 0, 0), 1}; // 红色笔
  PGpPen greenPen = {_RGB(2, 127, 0), 20}; // 绿色笔宽度20
  // 设置消除锯齿作图方式（牺牲速度），可注释该句后比较作图效果
  //GraphicsSetSmoothingMode(g, SmoothingModeAntiAlias);
  // 画布平移，方便作图时使用相对坐标
  int x = 20, y = 20;
  // 按不同的对齐方式画同样的矩形，redPen为比较基准线
  DrawAlignmentRect(g, greenPen, redPen, AlignmentCenter, x, y); // 居中（缺省）
  DrawAlignmentRect(g, greenPen, redPen, AlignmentInset, x, y + 130); // 线内
  // 复位图形变换
  x = 150, y = 20;
  // 用宽度6绿色笔画各种线条式样，线段两端为平头帽（缺省值）
  greenPen.wline = 6;
  DrawDashStyleLine(g, greenPen, x, y, butt_cap);
#if 0
  // 用宽度6绿色笔画各种线条式样，线段两端为圆头帽
  PenSetDashCap(greenPen, DashCapRound);
  GraphicsTranslate(g, 0, 10, MatrixOrderPrepend);
  DrawDashStyleLine(g, greenPen);
  // 用宽度30红色笔画复合线条
  GraphicsTranslate(g, -140, 20, MatrixOrderPrepend);
  PenSetWidth(redPen, 30);
  PenSetCompoundArray(redPen, cp, 6); // 设置复合线比例数组
  GraphicsDrawLine(g, redPen, 0, 0, 440, 0);
#endif
}
// 演示画刷
int test_brush(HDDC g)
{
  path_point path[] = {
    m_path_move_to(40, 140),
    m_path_curve4(275, 200, 105, 225, 190, 300),
    m_path_line_to(50, 350),
    m_path_line_to(20, 180),
  };
  DWORD ac[] = {Green, Yellow, Red, Blue, Orange, White};
  RECT clientRect = {0, 0, g->w, g->h};
  // 建立一个全局图像画刷，用来填充窗口背景
  //PGpImage backgroundImage = ImageCreate(L"..//..//Media//colorbars.jpg");
  //backgroundBrush = TextureBrushCreate(backgroundImage);
  DPOINT pt[1000];
  COLORREF col[256];
  int n;
  int lens[10];
  // 用图像画刷填充背景，同时用白色半透明实色画刷遮照背景图案
  //draw_fillrectR(g, backgroundBrush, 0, 0, clientRect.right, clientRect.bottom);
  draw_fillrectR(g, &clientRect, _SetAlpha(White, 180));
  // 用红色实色画刷画矩形，同时用半透明黄色实色刷错开画同样大小矩形在其上
  draw_fillrect(g, 20, 20, 50, 50, Red);
  draw_fillrect(g, 40, 40, 50, 50, _SetAlpha(Yellow, 180));
  // 用图案画刷画一个圆，图案前景色为绿色，背景色则为大半透明黄色
  //hb = HatchBrushCreate(HatchStyleForwardDiagonal, Green, _SetAlpha(Yellow, 100));
  drawaa_ellipse(g, 300, 80, 50, 50, _SetAlpha(Yellow, 100), 0);
  // 用渐变色画刷从右上至左下画一红、黄色渐变矩形
  fill_color_array(256, col, Red, Yellow);
  n = vcgen_rect(pt, 300, 250, 400, 350);
  drawaa_fillpoly_lut(g, pt, &n, 1, 400, 250, 300, 350, col);
  // 用６种颜色路径画刷匹配６坐标点路径画一图形（注意这不是多色填充，
  // 而是匹配路径坐标点渐变填充，颜色可以少于但不能大于路径坐标点；
  // 多色路径刷填充是沿路径中心点向边缘按规定的比例填充）。应该很漂亮！
  // 但遗憾的是，我还没见过有谁画成功过这个图形，只能画单色，也很漂亮的。
  n = vcgen_path(path, countof(path), pt, lens);
  drawaa_fillpoly_solid(g, pt, lens, n, Green);
  // 用紫色实色画刷画一个逆时针旋转30度的矩形
  n = vcgen_rect(pt, 100, 250, 175, 350);
  {
    TRANS_AFFINE_DEF(ta);
    trans_affine_rotation(ta, -30 * (PI / 180));
    trans_affine_transform_pt(ta, pt, n);
  }
  drawaa_fillpoly_solid(g, pt, &n, 1, SlateBlue);
  return 0;
}
#if 0
WCHAR flowedText1[] =
    L"I went down to the St James Infirmary,/n /
    Saw my baby there,/n /
    Stretched out on a long white table,/n /
    So sweet,so cold,so fair,/n /
    Let her go,let her go,God bless her,/n /
    Wherever she may be,/n /
    She can look this wide world over,/n /
    But she'll never find a sweet man like me,/n /
    When I die want you to dress me in straight lace shoes,/n /
    I wanna a boxback coat and a Stetson hat,/n /
    Put a twenty dollar gold piece on my watch chain,/n /
    So the boys'll know that I died standing up.";
WCHAR flowedText2[] =
    L"the sky seems full when you're in the cradle/n /
    the rain will fall and wash your dreams/n /
    stars are stars and they shine so hard.../n /
    now spit out the sky because its empty/n /
    and hollow and all your dreams are hanging out to dry/n /
    stars are stars and they shine so cold.../n /
    once i like crying twice i like laughter/n /
    come tell me what i'm after.";
WCHAR japaneseText[] = {
  31169, 12398, 21517, 21069, 12399, 12463,
  12522, 12473, 12391, 12377, 12290, 0
};
PGpBrush backgroundBrush;
PGpBrush textTextureBrush;
PGpBrush titleShadowBrush;
PGpBrush linearGradBrush;
PGpFont titleFont;
PGpFont textFont;
PGpFont thisFont;
PGpFont japaneseFont;
void OnCreate(void)
{
  Point ps[] = {{0, 0}, {0, 45}};
  PGpFontFamily serifFontFamily = FontFamilyGenericSerif();
  //Load the image to be used for the background from the exe's resource fork
  PGpImage image = ImageCreate(L"..//..//Media//colorbars.jpg");
  //Now create the brush we are going to use to paint the background
  backgroundBrush = TextureBrushCreate(image);
  ImageDelete(image);
  //Load the image to be used for the textured text from the exe's resource fork
  image = ImageCreate(L"..//..//Media//marble.jpg");
  textTextureBrush = TextureBrushCreate(image);
  ImageDelete(image);
  //Load the fonts we want to use
  thisFont = FontFromFamily(serifFontFamily, 20, 0, UnitPoint);
  titleFont = FontFromFamily(serifFontFamily, 60, 0, UnitPoint);
  textFont = FontFromFamily(serifFontFamily, 11, 0, UnitPoint);
  //Set up shadow brush - make it translucent
  titleShadowBrush = SolidBrushCreate(_SetAlpha(Black, 70));
  //Set up fonts and brushes for printing japanese text
  japaneseFont = FontCreate(L"MS Mincho", 36, 0);
  linearGradBrush = LineBrushFromPoint(&ps[0], &ps[1], Blue, Red);
}
void OnPaint(HDC DC)
{
  WCHAR s[40] = L"Hello Symetrical World";
  RECT clientRect;
  RectF r;
  REAL center;
  INT characters, lines;
  PGpStrFormat format = StrFormatCreate();
  PGpSolidBrush sb = SolidBrushCreate(_SetAlpha(White, 180));
  PGpGraphics g = GraphicsCreate(DC);
  GraphicsSetTextRenderingHint(g, TextRenderingHintAntiAlias);
  //Fill the background use the texture brush
  //and then apply a white wash
  GetClientRect(Handle, &clientRect);
  GraphicsFillRectangle(g, backgroundBrush, clientRect.left, clientRect.top, clientRect.right, clientRect.bottom);
  GraphicsFillRectangle(g, sb, clientRect.left, clientRect.top, clientRect.right, clientRect.bottom);
  //Simple draw hello world
  SolidBrushSetColor(sb, Black);
  GraphicsDrawStringXY(g, L"Hello World", thisFont, sb, 10, 10);
  //Draw a textured string
  GraphicsDrawStringXY(g, L"Graphics Samples", titleFont, titleShadowBrush, 15, 25);
  GraphicsDrawStringXY(g, L"Graphics Samples", titleFont, textTextureBrush, 10, 20);
  //Use Measure string to display a string at the center of the window
  SolidBrushSetColor(sb, Red);
  center = clientRect.right / 2.0f;
  GraphicsMeasureString(g, s, textFont, &r);
  GraphicsDrawStringXY(g, s, textFont, sb, center - r.Width / 2.0f, 10.0f);
  //Now draw a string flowed into a rectangle
  r = MakeRectF(20.0f, 150.0f, 250.0f, 300.0f);
  SolidBrushSetColor(sb, Gainsboro);
  GraphicsFillRectangleF(g, sb, r.X, r.Y, r.Width, r.Height);
  SolidBrushSetColor(sb, Blue);
  GraphicsDrawString(g, flowedText1, textFont, sb, &r, NULL);
  //Draw more flowed text but this time center it
  r = MakeRectF(420.0f, 150.0f, 250.0f, 300.0f);
  SolidBrushSetColor(sb, Gainsboro);
  GraphicsFillRectangleF(g, sb, r.X, r.Y, r.Width, r.Height);
  StrFormatSetAlignment(format, StringAlignmentCenter);
  SolidBrushSetColor(sb, Blue);
  GraphicsDrawString(g, flowedText2, textFont, sb, &r, format);
  //Work out how many lines and characters we printed just now
  characters = lines = 0;
  GraphicsMeasureStringSize(g, flowedText2, textFont,
      250.0f, 300.0f, format, &r, &characters, &lines);
  wsprintfW(s, L"We printed %d characters and %d lines", characters, lines);
  SolidBrushSetColor(sb, Black);
  GraphicsDrawStringXY(g, s, textFont, sb, 390.0f, 440.0f);
  //If we have the Japanese language pack draw some text in Japanese
  //Rotate it to make life truly exciting
  GraphicsRotate(g, -30, MatrixOrderPrepend);
  GraphicsTranslate(g, -180, 300, MatrixOrderPrepend);
  GraphicsDrawStringXY(g, japaneseText, japaneseFont, linearGradBrush, 175, 125);
  GraphicsResetTransform(g);
  StrFormatDelete(format);
  BrushDelete(sb);
  GraphicsDelete(g);
}
WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  /* 这里使用了双缓冲画窗口，可将第3个参数改为FALSE比较一下效果 */
  InitApplication(hInstance, nCmdShow, TRUE);
  CreateProc = OnCreate;
  DestroyProc = OnDestroy;
  PaintProc = OnPaint;
  return RunApplication(TEXT("C语言Gdiplus演示例子 -- 文字(仿c#例子)"), 700, 500);
}

#pragma hdrstop
#define ID_REASE 100
#define ID_ALPHA 101
#define ID_BRIGHT 102
#define ID_GRAY 103
#define ID_TWO 104
#define ID_INVERT 105
PGpBitmap bitmap;
PGpImageAttr attr;
Rect dRect;
HFONT hFont = NULL;
void CreateButton(LPTSTR caption, INT top, INT itemID, HFONT hFont)
{
  HWND hButton = CreateWindow(TEXT("Button"), caption, WS_CHILD | WS_VISIBLE,
      488, top, 80, 25, Handle, (HMENU)itemID, hInstance, NULL);
  SendMessage(hButton, WM_SETFONT, (WPARAM)(hFont), 0);
}
void OnCreate(void)
{
  hFont = CreateFont(14, 0, 0, 0, FW_NORMAL,
      FALSE, FALSE, FALSE, GB2312_CHARSET, OUT_DEFAULT_PRECIS,
      CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DECORATIVE, TEXT("宋体"));
  CreateButton(TEXT("半透明"), 30, ID_ALPHA, hFont);
  CreateButton(TEXT("亮度20%"), 65, ID_BRIGHT, hFont);
  CreateButton(TEXT("灰度化"), 100, ID_GRAY, hFont);
  CreateButton(TEXT("二值化"), 135, ID_TWO, hFont);
  CreateButton(TEXT("反 色"), 170, ID_INVERT, hFont);
  CreateButton(TEXT("重 置"), 205, ID_REASE, hFont);
  bitmap = BitmapFromFile(L"..//..//Media//Source.jpg", FALSE);
  attr = ImageAttrCreate();
  dRect = MakeRect(0, 0, ImageGetWidth(bitmap), ImageGetHeight(bitmap));
}
void OnDestroy(void)
{
  if (hFont) {
    DeleteObject(hFont);
  }

  ImageDelete(bitmap);
  ImageAttrDelete(attr);
}
void OnCommand(WORD NotifyCode, WORD itemID, HWND ctlHandle)
{
  ColorMatrix matrix;
  INT i;
  memset(&matrix, 0, sizeof(ColorMatrix));

  for (i = 0; i < 5; i ++) {
    matrix.m[i][i] = 1.0f;
  }

  ImageAttrReset(attr, ColorAdjustTypeDefault);

  switch (itemID) {
  case ID_REASE:
    InvalidateRect(Handle, &dRect, FALSE);
    return;

  case ID_ALPHA:
    matrix.m[3][3] = 0.5f;
    break;

  case ID_BRIGHT:
    matrix.m[4][0] = matrix.m[4][1] = matrix.m[4][2] = 0.2f;
    break;

  case ID_GRAY:
  case ID_TWO:
    matrix.m[0][0] = matrix.m[0][1] = matrix.m[0][2] = 0.30f;
    matrix.m[1][0] = matrix.m[1][1] = matrix.m[1][2] = 0.59f;
    matrix.m[2][0] = matrix.m[2][1] = matrix.m[2][2] = 0.11f;
    break;

  case ID_INVERT:
    matrix.m[0][0] = matrix.m[1][1] = matrix.m[2][2] = -1.0f;
    break;
  }

  ImageAttrSetColorMatrix(attr, &matrix, ColorMatrixFlagsDefault, ColorAdjustTypeDefault);

  if (itemID == ID_TWO) {
    ImageAttrSetThreshold(attr, 0.5, ColorAdjustTypeDefault);
  }

  InvalidateRect(Handle, &dRect, FALSE);
}
void OnPaint(HDC DC)
{
  PGpGraphics g = GraphicsCreate(DC); // GDI+ 画布
  GraphicsDrawImageRectRect(g, bitmap, &dRect, 0, 0, dRect.Width, dRect.Height, UnitPixel, attr);
  GraphicsDelete(g);
}
WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  // 这里使用了双缓冲画窗口 */
  InitApplication(hInstance, nCmdShow, TRUE);
  CreateProc = OnCreate;
  DestroyProc = OnDestroy;
  PaintProc = OnPaint;
  CommandProc = OnCommand;
  return RunApplication(TEXT("C语言Gdiplus演示例子 -- ColorMatrix"), 582, 392);
}

PGpBitmap bitmap;
PGpBitmap photo;
PGpBitmap watermark;
INT wmWidth, wmHeight;
INT phWidth, phHeight;
HFONT hFont = NULL;
ColorMatrix colorMatrix = {1.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.3, 0.0,
            0.0, 0.0, 0.0, 0.0, 1.0
                          };
void OnCreate(void)
{
  PGpImageAttr attr;
  PGpFont font;
  PGpBrush brush;
  PGpStrFormat format;
  PGpGraphics g;
  float x, y;
  Rect dRect;
  RectF fRect;
  HWND hButton;
  hFont = CreateFont(14, 0, 0, 0, FW_NORMAL,
      FALSE, FALSE, FALSE, GB2312_CHARSET, OUT_DEFAULT_PRECIS,
      CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DECORATIVE, TEXT("宋体"));
  hButton = CreateWindow(TEXT("Button"), TEXT("保存"), WS_CHILD | WS_VISIBLE,
      400, 204, 80, 25, Handle, (HMENU)ID_SAVE, hInstance, NULL);
  SendMessage(hButton, WM_SETFONT, (WPARAM)(hFont), 0);
  // 读取原始图片
  photo = BitmapFromFile(L"..//..//Media//100_0349.jpg", FALSE);
  phWidth = ImageGetWidth(photo);
  phHeight = ImageGetHeight(photo);
  // 读取水印图片
  watermark = BitmapFromFile(L"..//..//Media//Watermark.bmp", FALSE);
  wmWidth = ImageGetWidth(watermark);
  wmHeight = ImageGetHeight(watermark);
  // 建立一个新的位图，分辨率为72
  bitmap = BitmapCreate(phWidth, phHeight, PixelFormat32bppARGB);
  BitmapSetResolution(bitmap, 72, 72);
  // 建立新位图的画布，并设置图像显示质量和文本显示质量
  g = GraphicsFromImage(bitmap);
  GraphicsSetSmoothingMode(g, SmoothingModeAntiAlias);
  GraphicsSetTextRenderingHint(g, TextRenderingHintAntiAlias);
  /* 在画布上画原始图像 */
  GraphicsDrawImage(g, photo, 0, 0, phWidth, phHeight);
  /* 画水印原始图像 */
  // 建立一个图像显示属性对象
  attr = ImageAttrCreate();
  // 设置透明颜色为水印图片四角的底色，水印图显示为圆角图片
  ImageAttrSetColorKey(attr, 0xff00ff00, 0xff00ff00, ColorAdjustTypeBitmap);
  // 设置水印图片不透明度为0.3
  ImageAttrSetColorMatrix(attr, &colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);
  // 在画布左上角画原始水印图
  dRect = MakeRect(10, 10, wmWidth, wmHeight);
  GraphicsDrawImageRectRect(g, watermark, &dRect, 0, 0, wmWidth, wmHeight, UnitPixel, attr);
  /* 在画布下方居中画文本 */
  // 建立字体对象
  font = FontCreate(L"arial", 16, FontStyleBold);
  // 建立画刷对象
  brush = SolidBrushCreate(0x99000000);
  // 建立文本格式对象
  format = StrFormatCreate();
  // 设置文本居中显示
  StrFormatSetAlignment(format, StringAlignmentCenter);
  // 画文本阴影
  fRect = MakeRectF(phWidth / 2 + 1, phHeight - 26 + 1, 0, 0);
  GraphicsDrawString(g, L"Copyright ? 2010 - Maozefa", font, brush, &fRect, format);
  // 画文本
  fRect = MakeRectF(phWidth / 2, phHeight - 26, 0, 0);
  SolidBrushSetColor(brush, 0x99ffffff);
  GraphicsDrawString(g, L"Copyright ? 2010 - Maozefa", font, brush, &fRect, format);
  ImageAttrDelete(attr);
  StrFormatDelete(format);
  FontDelete(font);
  BrushDelete(brush);
  GraphicsDelete(g);
}
void OnDestroy(void)
{
  if (hFont) {
    DeleteObject(hFont);
  }

  ImageDelete(bitmap);
  ImageDelete(photo);
  ImageDelete(watermark);
}
void OnCommand(WORD NotifyCode, WORD itemID, HWND ctlHandle)
{
  EncoderParameters parameters;
  UINT quality = 100;
  GUID clsid;

  if (itemID != ID_SAVE) {
    return;
  }

  // 设置图像品质编码参数
  parameters.Count = 1;
  parameters.Parameter[0].Guid = EncoderQuality;
  parameters.Parameter[0].Type = EncoderParameterValueTypeLong;
  parameters.Parameter[0].NumberOfValues = 1;
  // 设置参数的值：品质等级，最高为100，图像文件大小与品质成正比
  parameters.Parameter[0].Value = &quality;

  // 保存水印图片
  if (GetEncoderClsid(L"image/jpeg", &clsid)) {
    ImageSaveToFile(bitmap, L"d://WatermarkPhoto.jpg", &clsid, ? meters);
  }
}
void OnPaint(HDC DC)
{
  PGpGraphics g = GraphicsCreate(DC); // GDI+ 画布
  // 显示原始图片
  GraphicsDrawImage(g, photo, 0, 0, phWidth, phHeight);
  // 显示水印原始图片
  GraphicsTranslate(g, 0, phHeight + 5, MatrixOrderPrepend);
  GraphicsDrawImage(g, watermark, 0, 0, wmWidth, wmHeight);
  // 显示带水印和文本的图像
  GraphicsTranslate(g, phWidth, -(phHeight + 5), MatrixOrderPrepend);
  GraphicsDrawImage(g, bitmap, 0, 0, phWidth, phHeight);
  GraphicsDelete(g);
}
WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  // 这里使用了双缓冲画窗口 */
  InitApplication(hInstance, nCmdShow, TRUE);
  CreateProc = OnCreate;
  DestroyProc = OnDestroy;
  PaintProc = OnPaint;
  CommandProc = OnCommand;
  return RunApplication(TEXT("C语言Gdiplus演示例子 -- 水印图片"), 518, 280);
}

本例子制作水印图片的步骤：
1、打开原始图片和原始水印图片；
2、建立一个新的图像，即要制作的水印图片，其大小等同于原始图片；
3、画原始图片到新图像；
4、利用图像显示属性对象设置原始水印图片关键颜色，将其四个角的绿色像素置为透明色，这样显示的图像为圆角形状；
5、利用图像显示属性对象设置原始水印图片颜色矩阵，使其按30 % 的不透明度显示；
6、根据设置好的图像显示属性对象显示原始水印图片到新图像左上角；
7、设置文本显示格式对象为文本居中显示；
8、以版权文本显示中心坐标偏右下1像素，用0x99000000颜色在新图像显示版权文本阴影，颜色中的0x99为文本阴影颜色不透明度；
9、以版权文本显示中心坐标，用0x99ffffff颜色在新图像显示版权文本，颜色中的0x99为文本颜色不透明度；
10、制作完毕。
本例子综合运用了GDI + 常用的图像操作过程和文本显示过程，包括图像显示属性对象和文本显示格式对象的运用。
原C++ GDI + 版本的Graphics.DrawString函数有个以x, y坐标为参数的重载方法，C版本将它删去了，只保留了以布局矩形为参数的方法，例子中的代码fRect = MakeRectF(phWidth / 2 , phHeight - 26, 0, 0);
是建立布局矩形，其中的x，y参数为文本显示的中心点，而非文本显示的左上角坐标，这是因为例子中文本格式设置为居中显示（StrFormatSetAlignment(format, StringAlignmentCenter);
），所以文本显示起始坐标点必须为文本中心点；而Width和Height参数分别设置为0，意思是不限制文本显示的右边和下边的边界，相当于原C++版本以x, y坐标为参数的重载方法，当文本长度超出时会截断，反之，如果给出具体的Width和Height，当文本长度超出时会换行。
例子中也演示了如何保存图像，并给出了设置保存图像质量编码参数的代码（仅对jpeg格式图像有效）。保存图像时用到的GetEncoderClsid函数是原GDI + 中没有的，为方便编写保存图像代码，我将该函数写在了GDI + 头文件GdipImageCode_c.h中。如果没有设置保存图像编码参数，ImageSavexxxx系列函数中的encoderParams参数设置为NULL即可，但其中的clsidEncoder参数是必须的。
例子代码使用的窗口框架代码和GDI + C语言版本下载地址见《在C语言Windows应用程序中使用GDI + 》。
下面是例子运行界面图，其中左上为原始图片，左下为原始水印图片，右边是制作好的带版权印记的水印图片：
FORCEINLINE
INT CheckValue(INT value)
{
  return (value & ~0xff) == 0 ? value : value > 255 ? 255 : 0;
}

// 线性亮度/对比度调整
VOID LineBrightAndContrast(BitmapData* data, INT bright, INT contrast, BYTE threshold)
{
  if (bright == 0 && contrast == 0) {
    return;
  }

  FLOAT bv = bright <= -255 ? -1.0f : bright / 255.0f;

  if (bright > 0 && bright < 255) {
    bv = 1.0f / (1.0f - bv) - 1.0f;
  }

  FLOAT cv = contrast <= -255 ? -1.0f : contrast / 255.0f;

  if (contrast > 0 && contrast < 255) {
    cv = 1.0f / (1.0f - cv) - 1.0f;
  }

  BYTE values[256];

  for (INT i = 0; i < 256; i ++) {
    INT v = contrast > 0 ? CheckValue(i + (INT)(i * bv + 0.5f)) : i;

    if (contrast >= 255) {
      v = v >= threshold ? 255 : 0;
    }
    else {
      v = CheckValue(v + (INT)((v - threshold) * cv + 0.5f));
    }

    values[i] = contrast <= 0 ? CheckValue(v + (INT)(v * bv + 0.5f)) : v;
  }

  PARGBQuad p = (PARGBQuad)data->Scan0;
  INT offset = data->Stride - data->Width * sizeof(ARGBQuad);

  for (UINT y = 0; y < data->Height; y ++, (BYTE*)p += offset) {
    for (UINT x = 0; x < data->Width; x ++, p ++) {
      p->Blue = values[p->Blue];
      p->Green = values[p->Green];
      p->Red = values[p->Red];
    }
  }
}

void __fastcall TForm1::Button2Click(TObject* Sender)
{
  Gdiplus::Bitmap* bmp = new Gdiplus::Bitmap(L"..\\..\\media\\source1.jpg");
  Gdiplus::Graphics* g = new Gdiplus::Graphics(Canvas->Handle);
  g->DrawImage(bmp, 0, 0);
  BitmapData data;
  LockBitmap(bmp, &data);
  LineBrightAndContrast(&data, 20, 0, 121);
  UnlockBitmap(bmp, &data);
  g->DrawImage(bmp, data.Width, 0);
  delete g;
  delete bmp;
}
C++图像处理 -- 图像合成
分类： C / C++ GDI + (VCL)应用 C / C++图形图像 2012 - 01 - 04 21: 20 2603人阅读 评论(3) 收藏 举报
ARGBcCc++C++gdi + GDI + mixer
阅读提示：
《C++图像处理》系列以代码清晰，可读性为主，全部使用C++代码。
《Delphi图像处理》系列以效率为侧重点，一般代码为PASCAL，核心代码采用BASM。
尽可能保持二者内容一致，可相互对照。
本文代码必须包括《C++图像处理 -- 数据类型及公用函数》文章中的BmpData.h头文件。
在图像处理过程中，图像的合成操作是使用频率最高的，如图像显示、图像拷贝、图像拼接以及的图层拼合叠加等。
图像合成，其实也就是图像像素颜色的混合，在Photoshop中，颜色混合是个很复杂的东西，不同的混合模式，将产生不同的合成效果，如果将之全部研究透彻，估计就得写一本书。因此，本文只谈谈最基本的图像合成，也就是Photoshop中的正常混合模式。
只要接触过图像处理的，都知道有个图像像素混合公式：
1）dstRGB = srcRGB * alpha + dstRGB * (1 - alpha)
    其中，dstRGB为目标图像素值；srcRGB为源图像素值；alpha为源图像素值混合比例（不透明度，范围0 - 1）。
    其实，这个像素混合公式有很大局限性，只适合不含Alpha信息的图像。
    要处理包括带Alpha通道图像（层）的混合，其完整的公式应该是：
    2 - 1）srcRGB = srcRGB * srcAlpha * alpha / 255 （源图像素预乘转换为PARGB）
        2 - 2）dstRGB = dstRGB * dstAlpha / 255 （目标图像素预乘转换为PARGB）
            2 - 3）dstRGB = dstRGB + srcRGB - dstRGB * srcAlpha * alpha / 255 （源图像素值与目标图像素值混合）
                2 - 4）dstAlpha = dstAlpha + srcAlpha * alpha - dstAlpha * srcAlpha * alpha / 255 （混合后的目标图Alpha通道值）
                    2 - 5）dstRGB = dstRGB * 255 / dstAlpha （混合后的目标图像素转换为ARGB）
                        其中，dstRGB为目标图像素值；srcRGB为源图像素值；dstAlpha为目标图Alpha通道值；srcAlpha为源图Alpha通道值；dstARGB为含Alpha目标图像素值；alpha为源图像素值混合比例（不透明度，范围0 - 1）。
                        将公式2中的2 - 1式代入2 - 3式，简化可得：
                        3 - 1）dstRGB = dstRGB * dstAlpha / 255
                            3 - 2）dstRGB = dstRGB + (srcRGB - dstRGB) * srcAlpha * alpha / 255
                                3 - 3）dstAlpha = dstAlpha + srcAlpha * alpha - dstAlpha * srcAlpha * alpha / 255
                                    3 - 4）dstRGB = dstRGB * 255 / dstAlpha
                                        当dstAlpha = srcAlpha = 255时，公式3中3 - 1式、3 - 3式和3 - 4式没有意义，3 - 2式也变化为：
                                            4）dstRGB = dstRGB + (srcRGB - dstRGB) * alpha
                                                不难看出，公式4是公式1的变形。因此，公式1只是公式3（或者公式2）在目标图和源图都不含Alpha信息（或者Alpha = 255）情况下的一个特例而已。
                                                    当公式4中的alpha = 1时，目标图像素等于源图像素，所以，本文前面说图像拷贝其实也是图像合成的范畴。
                                                        通过上面较详细的分析，可以看出，即使是最基本正常图像混合模式也是很复杂的。其实，上面还不是完整的分析，因为按照目标图Alpha信息、源图Alpha信息以及源图合成比例等三个要素的完全的排列组合，最多可以派生8个公式。
                                                        下面就按正常混合模式的全部8种情况（有2项重合，实际为7种情况）来分别进行代码实现，也可完善和补充上面的文字叙述：
                                                        [cpp] view plaincopyprint ?

                                                        FORCEINLINE
                                                        static VOID ARGBMixer(PARGBQuad pd, CONST PARGBQuad ps, INT alpha)
{
  pd->Blue += (((ps->Blue - pd->Blue) * alpha + 127) / 255);
  pd->Green += (((ps->Green - pd->Green) * alpha + 127) / 255);
  pd->Red += (((ps->Red - pd->Red) * alpha + 127) / 255);
}

FORCEINLINE
static VOID PARGBMixer(PARGBQuad pd, CONST PARGBQuad ps, INT alpha)
{
  pd->Blue = (pd->Blue * pd->Alpha + 127) / 255;
  pd->Green = (pd->Green * pd->Alpha + 127) / 255;
  pd->Red = (pd->Red * pd->Alpha + 127) / 255;
  pd->Blue += (((ps->Blue - pd->Blue) * alpha + 127) / 255);
  pd->Green += (((ps->Green - pd->Green) * alpha + 127) / 255);
  pd->Red += (((ps->Red - pd->Red) * alpha + 127) / 255);
  pd->Alpha += (alpha - (pd->Alpha * alpha + 127) / 255);
  pd->Blue = pd->Blue * 255 / pd->Alpha;
  pd->Green = pd->Green * 255 / pd->Alpha;
  pd->Red = pd->Red * 255 / pd->Alpha;
}

// source alpha = FALSE, dest alpha = FALSE, alpha < 255
static VOID Mixer0(BitmapData* dest, CONST BitmapData* source, INT alpha)
{
  PARGBQuad pd, ps;
  UINT width, height;
  INT dstOffset, srcOffset;
  GetDataCopyParams(dest, source, width, height, pd, ps, dstOffset, srcOffset);

  for (UINT y = 0; y < height; y ++, pd += dstOffset, ps += srcOffset) {
    for (UINT x = 0; x < width; x ++, pd ++, ps ++) {
      ARGBMixer(pd, ps, alpha);
    }
  }
}

// source alpha = FALSE, dest alpha = FALSE, alpha = 255
// source alpha = FALSE, dest alpha = TRUE, alpha = 255
static VOID Mixer1(BitmapData* dest, CONST BitmapData* source, INT alpha)
{
  PARGBQuad pd, ps;
  UINT width, height;
  INT dstOffset, srcOffset;
  GetDataCopyParams(dest, source, width, height, pd, ps, dstOffset, srcOffset);

  for (UINT y = 0; y < height; y ++, pd += dstOffset, ps += srcOffset) {
    for (UINT x = 0; x < width; x ++, *pd ++ = *ps ++);
  }
}

// source alpha = FALSE, dest alpha = TRUE, alpha < 255
static VOID Mixer2(BitmapData* dest, CONST BitmapData* source, INT alpha)
{
  PARGBQuad pd, ps;
  UINT width, height;
  INT dstOffset, srcOffset;
  GetDataCopyParams(dest, source, width, height, pd, ps, dstOffset, srcOffset);

  for (UINT y = 0; y < height; y ++, pd += dstOffset, ps += srcOffset) {
    for (UINT x = 0; x < width; x ++, pd ++, ps ++) {
      PARGBMixer(pd, ps, alpha);
    }
  }
}

// source alpha = TRUE, dest alpha = FALSE, alpha < 255
static VOID Mixer4(BitmapData* dest, CONST BitmapData* source, INT alpha)
{
  PARGBQuad pd, ps;
  UINT width, height;
  INT dstOffset, srcOffset;
  GetDataCopyParams(dest, source, width, height, pd, ps, dstOffset, srcOffset);

  for (UINT y = 0; y < height; y ++, pd += dstOffset, ps += srcOffset) {
    for (UINT x = 0; x < width; x ++, pd ++, ps ++) {
      ARGBMixer(pd, ps, (alpha * ps->Alpha + 127) / 255);
    }
  }
}

// source alpha = TRUE, dest alpha = FALSE, alpha = 255
static VOID Mixer5(BitmapData* dest, CONST BitmapData* source, INT alpha)
{
  PARGBQuad pd, ps;
  UINT width, height;
  INT dstOffset, srcOffset;
  GetDataCopyParams(dest, source, width, height, pd, ps, dstOffset, srcOffset);

  for (UINT y = 0; y < height; y ++, pd += dstOffset, ps += srcOffset) {
    for (UINT x = 0; x < width; x ++, pd ++, ps ++) {
      ARGBMixer(pd, ps, ps->Alpha);
    }
  }
}

// source alpha = TRUE, dest alpha = TRUE, alpha < 255
static VOID Mixer6(BitmapData* dest, CONST BitmapData* source, INT alpha)
{
  PARGBQuad pd, ps;
  UINT width, height;
  INT dstOffset, srcOffset;
  GetDataCopyParams(dest, source, width, height, pd, ps, dstOffset, srcOffset);

  for (UINT y = 0; y < height; y ++, pd += dstOffset, ps += srcOffset) {
    for (UINT x = 0; x < width; x ++, pd ++, ps ++) {
      INT alpha0 = (alpha * ps->Alpha + 127) / 255;

      if (alpha0) {
        PARGBMixer(pd, ps, alpha0);
      }
    }
  }
}

// source alpha = TRUE, dest alpha = TRUE, alpha = 255
static VOID Mixer7(BitmapData* dest, CONST BitmapData* source, INT alpha)
{
  PARGBQuad pd, ps;
  UINT width, height;
  INT dstOffset, srcOffset;
  GetDataCopyParams(dest, source, width, height, pd, ps, dstOffset, srcOffset);

  for (UINT y = 0; y < height; y ++, pd += dstOffset, ps += srcOffset) {
    for (UINT x = 0; x < width; x ++, pd ++, ps ++) {
      if (ps->Alpha) {
        PARGBMixer(pd, ps, ps->Alpha);
      }
    }
  }
}

typedef VOID (*MixerProc)(BitmapData*, CONST BitmapData*, INT);
// 图像合成。参数：32位目标数据，32位源数据，不透明度(0 - 1.0)
VOID ImageMixer(BitmapData* dest, CONST BitmapData* source, FLOAT alpha)
{
  INT alphaI = (INT)(alpha * 255);

  if (alphaI <= 0) {
    return;
  }

  if (alphaI > 255) {
    alphaI = 255;
  }

  MixerProc proc[] = {Mixer0, Mixer1, Mixer2, Mixer1, Mixer4, Mixer5, Mixer6, Mixer7};
  INT index = (alphaI / 255) |
      (HasAlphaFlag(dest) << 1) |
      (HasAlphaFlag(source) << 2);
  proc[index](dest, source, alphaI);
}

函数ImageMixer有三个参数，分别为目标图数据结构（借用GDI + 的BitmapData结构）指针、源图数据结构指针和源图像素混合比例（不透明度，取值范围为0 - 1）。函数体中的proc数组包括了图像混合的全部8种情况的子函数，而index则按混合比例、目标图Alpha信息和源图Alpha信息组合成子函数调用下标值（Alpha信息在BitmapData结构的保留字段中）。
当然，在实际的运用中，全部8种情况似乎是多了点，可根据情况进行适当合并取舍，以兼顾代码的复杂度和执行效率。下面是我认为比较合理的精简版ImageMixer函数：
[cpp] view plaincopyprint ?
// 图像合成。参数：32位目标数据，32位源数据，不透明度(0 - 1.0)
VOID ImageMixer(BitmapData* dest, CONST BitmapData* source, FLOAT alpha)
{
  INT alphaI = (INT)(alpha * 255);

  if (alphaI <= 0) {
    return;
  }

  if (alphaI > 255) {
    alphaI = 255;
  }

  if (alphaI == 255 && !HasAlphaFlag(source)) {
    Mixer1(dest, source, alphaI); // 拷贝合成
  }
  else if (HasAlphaFlag(dest)) {
    Mixer6(dest, source, alphaI); // PARGB合成
  }
  else {
    Mixer4(dest, source, alphaI); // ARGB合成
  }
}

这个ImageMixer函数只保留了3个调用子函数，其中，Mixer6是完全的正常混合模式，即前面公式3的实现；Mixer4为对不含Alpha信息目标图的混合，即在公式4基础上稍稍扩充了的情况；而Mixer1则为拷贝模式。
下面是采用BCB2010和GDI + 调用ImageMixer函数的例子：
[cpp] view plaincopyprint ?

void __fastcall TForm1::Button1Click(TObject* Sender)
{
  Gdiplus::Bitmap* dest = new Gdiplus::Bitmap(L"d:\\xmas_011.png");
  Gdiplus::Bitmap* source = new Gdiplus::Bitmap(L"d:\\Apple.png");
  Gdiplus::Graphics* g = new Gdiplus::Graphics(Canvas->Handle);
  g->DrawImage(dest, 0, 0);
  g->DrawImage(source, dest->GetWidth(), 0);
  BitmapData dst, src;
  LockBitmap(dest, &dst);
  LockBitmap(source, &src);
  ImageMixer(&dst, &src, 0.75);
  UnlockBitmap(source, &src);
  UnlockBitmap(dest, &dst);
  g->DrawImage(dest, dest->GetWidth() << 1, 0);
  delete g;
  delete source;
  delete dest;
}
typedef union {
  FLOAT Elements[6];
  struct {
    FLOAT m11;
    FLOAT m12;
    FLOAT m21;
    FLOAT m22;
    FLOAT dx;
    FLOAT dy;
  };
} MatrixElements, *PMatrixElements;

struct TransformMatrix {
private:
  MatrixElements elements;
  VOID ElementsInit(MatrixElements& e) {
    e.m11 = e.m22 = 1.0f;
    e.m12 = e.m21 = e.dx = e.dy = 0.0f;
  }
  VOID ElementsMultiply(MatrixElements& e) {
    FLOAT m11 = elements.m11;
    FLOAT m12 = elements.m12;
    elements.m11 = e.m11 * m11 + e.m12 * elements.m21;
    elements.m12 = e.m11 * m12 + e.m12 * elements.m22;
    elements.m21 = e.m21 * m11 + e.m22 * elements.m21;
    elements.m22 = e.m21 * m12 + e.m22 * elements.m22;
  }
public:
  // 建立一个新实例，并初始化为单位矩阵 Elements = 1,0,0,1,0,0
  TransformMatrix(VOID) {
    Reset();
  }
  // 建立一个新实例，并复制matrix的元素
  TransformMatrix(TransformMatrix* matrix) {
    SetElements(matrix->elements);
  }
  TransformMatrix(TransformMatrix& matrix) {
    SetElements(matrix.elements);
  }
  // 建立一个按指定的元素初始化的新实例
  TransformMatrix(FLOAT m11, FLOAT m12, FLOAT m21, FLOAT m22, FLOAT dx, FLOAT dy) {
    SetElements(m11, m12, m21, m22, dx, dy);
  }
  // 重置对象为单位矩阵
  VOID Reset(VOID) {
    ElementsInit(elements);
  }
  // 将对象与matrix相乘
  VOID Multiply(TransformMatrix* matrix) {
    elements.dx += (matrix->elements.dx * elements.m11 + matrix->elements.dy * elements.m21);
    elements.dy += (matrix->elements.dx * elements.m12 + matrix->elements.dy * elements.m22);
    ElementsMultiply(matrix->elements);
  }
  VOID Multiply(TransformMatrix& matrix) {
    Multiply(&matrix);
  }
  // 设置平移
  VOID Translate(FLOAT offsetX, FLOAT offsetY) {
    elements.dx += (offsetX * elements.m11 + offsetY * elements.m21);
    elements.dy += (offsetX * elements.m12 + offsetY * elements.m22);
  }
  // 设置缩放
  VOID Scale(FLOAT scaleX, FLOAT scaleY) {
    MatrixElements e;
    ElementsInit(e);
    e.m11 = scaleX;
    e.m22 = scaleY;
    ElementsMultiply(e);
  }
  // 设置剪切，注意不要将shearX, shearY同时设置为1
  VOID Shear(FLOAT shearX, FLOAT shearY) {
    MatrixElements e;
    ElementsInit(e);
    e.m21 = shearX;
    e.m12 = shearY;
    ElementsMultiply(e);
  }
  // 设置按角度angle沿原点旋转
  VOID Rotate(FLOAT angle) {
    MatrixElements e;
    angle = angle * M_PI / 180.0f;
    e.m11 = e.m22 = cos(angle);
    e.m12 = sin(angle);
    e.m21 = -e.m12;
    e.dx = e.dy = 0.0f;
    ElementsMultiply(e);
  }
  // 设置按角度angle沿中心点centerX, centerY旋转
  VOID RotateAt(FLOAT angle, FLOAT centerX, FLOAT centerY) {
    Translate(centerX, centerY);
    Rotate(angle);
    Translate(-centerX, -centerY);
  }
  // 如果此对象是可逆转的，则逆转该对象，返回TRUE；否则返回FALSE
  BOOL Invert(VOID) {
    double tmp = elements.m11 * elements.m22 - elements.m12 * elements.m21;

    if ((INT)(tmp * 1000.0f) == 0) {
      return FALSE;
    }

    tmp = 1.0f / tmp;
    FLOAT m11 = elements.m11;
    FLOAT dx = -elements.dx;
    elements.m11 = tmp * elements.m22;
    elements.m12 = tmp * -elements.m12;
    elements.m21 = tmp * -elements.m21;
    elements.m22 = tmp * m11;
    elements.dx = dx * elements.m11 - elements.dy * elements.m21;
    elements.dy = dx * elements.m12 - elements.dy * elements.m22;
    return TRUE;
  }
  // 按给定的大小计算并返回实施变换后的尺寸
  VOID GetTransformSize(INT width, INT height, FLOAT& fx, FLOAT& fy, FLOAT& fwidth, FLOAT& fheight) {
    FLOAT fxs[3], fys[3], v;
    fxs[1] = fys[0] = 0.0f;
    fxs[0] = fxs[2] = width;
    fys[1] = fys[2] = height;
    fx = fy = fwidth = fheight = 0.0f;

    for (INT i = 0; i < 3; i ++) {
      v = fxs[i] * elements.m11 + fys[i] * elements.m21;

      if (v < fx) {
        fx = v;
      }
      else if (v > fwidth) {
        fwidth = v;
      }

      v = fxs[i] * elements.m12 + fys[i] * elements.m22;

      if (v < fy) {
        fy = v;
      }
      else if (v > fheight) {
        fheight = v;
      }
    }

    fwidth -= fx;
    fheight -= fy;
    fx += elements.dx;
    fy += elements.dy;
  }
  // 按给定的大小计算并返回实施变换后整型数矩形
  VOID GetTransformRect(INT width, INT height, RECT& r) {
    FLOAT fx, fy, fwidth, fheight;
    GetTransformSize(width, height, fx, fy, fwidth, fheight);
    r.left = (INT)fx;
    r.top = (INT)fy;
    r.right = (INT)(fwidth + fx + 0.999999f);
    r.bottom = (INT)(fheight + fy + 0.999999f);
  }
  // 判断此对象是否是单位矩阵
  BOOL GetIdentity(VOID) {
    return (elements.m11 == 1.0f &&
        elements.m22 == 1.0f &&
        elements.m12 == 0.0f &&
        elements.m21 == 0.0f &&
        elements.dx == 0.0f &&
        elements.dy == 0.0f);
  }
  // 获取对象的x偏移量
  FLOAT GetOffsetX(VOID) {
    return elements.dx;
  }
  // 获取对象的y偏移量
  FLOAT GetOffsetY(VOID) {
    return elements.dy;
  }
  // 判断对象是否是可逆转的。
  BOOL GetInvertible(VOID) {
    return (INT)(1000.0f * (elements.m11 * elements.m22 - elements.m12 * elements.m21)) != 0;
  }
  // 获取对象元素
  MatrixElements& GetElements(VOID) {
    return elements;
  }
  // 设置对象元素。注：设置元素是覆盖形式的
  VOID SetElements(CONST MatrixElements& value) {
    SetElements(value.m11, value.m12, value.m21, value.m22, value.dx, value.dy);
  }
  VOID SetElements(FLOAT m11, FLOAT m12, FLOAT m21, FLOAT m22, FLOAT dx, FLOAT dy) {
    elements.m11 = m11;
    elements.m12 = m12;
    elements.m21 = m21;
    elements.m22 = m22;
    elements.dx = dx;
    elements.dy = dy;
  }
};

上面代码中定义了一个几何变换矩阵成员类型MatrixElements，便于实际编程时获取或设置几何变换矩阵成员，TransformMatrix只是简单的对其进行了封装，并通过计算实现有关的几何变换。
TransformMatrix的核心代码是Multiply函数（或ElementsMultiply函数）和Invert函数。
Multiply函数可完成各种复杂的几何变换计算，所有能够实现的具体几何变换都是可以通过其完成的（代码中的平移函数Translate也可以通过其完成的，当然多了一些不必要的计算）。虽说本文标题是《实现完整的图像平面几何变换》，但TransformMatrix中的几种基础的变换函数并不代表全部的几何变换，如对称几何变换（镜像），更不用说复杂的组合变换。这倒不是本人要做“标题党”，我所说的“实现完整的图像几何变换”，是指可以通过Multiply函数或者更直接的变换矩阵成员设置去实现“完整的”图像几何变换，除非其不能使用平面几何变换矩阵进行描述（如梯形变换我就没想到怎么实现，也许其超出了平面几何变换矩阵范畴？），或者不能进行实际的几何变换（不可逆）；“实现完整的图像几何变换”的另一层含义是下面的图像变换执行函数可实现TransformMatrix所能表示的任意图像几何变换，而不必去写一个个具体的，如缩放、旋转变换函数等。
Invert函数实现了变换矩阵的逆矩阵，通过这个几何变换逆矩阵，可以很方便地实现图形图像几何变换的实际操作。为什么要靠几何变换矩阵的逆矩阵，而不是直接依据变换矩阵来实现图形图像几何变换的实际操作呢？因为几何变换矩阵表示的意思是，把源图像的任意座标点通过几何变换后投影到目标图像。因为源图像像素通过几何变换后与目标图像上的像素点有可能不能一一对应，如图像缩放变换后，不是多个源图像像素点对应同一个目标像素点（缩小），就是源图像像素点不足以填充全部的目标像素点（放大），这就有可能造成目标图像像素点被重复绘制或者被遗漏的现象发生；而几何变换逆矩阵所表示的意思是，对于目标图像任意一个像素点，如果在几何变换前有源图像像素点与其对应，则进行复制。遍历目标图像像素点就能保证目标图像像素点既不重复、也不遗漏的被复制。
下面是一个图像几何变换函数代码：
[cpp] view plaincopyprint ?
// 执行图像数据几何变换
VOID Transform(BitmapData* dest, INT x, INT y, CONST BitmapData* source, TransformMatrix* matrix)
{
  // 复制几何变换矩阵对象
  TransformMatrix m(matrix);
  // 几何变换矩阵绝对增加平移量x, y
  m.GetElements().dx += x;
  m.GetElements().dy += y;
  // 按几何变换矩阵计算并获取目标图像数据子数据
  float fx, fy, fwidth, fheight;
  m.GetTransformSize(source->Width, source->Height, fx, fy, fwidth, fheight);
  BitmapData dst;

  if (!GetBitmapData(dest, (INT)fx, (INT)fy,
      (INT)(fwidth + 0.999999f), (INT)(fheight + 0.999999f), &dst)) {
    return;
  }

  // 获取几何变换逆矩阵
  if (!m.Invert()) {
    return;
  }

  // 如果子图数据与目标图像原点不一致，几何变换矩阵相对增加平移量fx, fy
  if (fx > 0.0f || fy > 0.0f) {
    if (fx < 0.0f) {
      fx = 0.0f;
    }
    else if (fy < 0.0f) {
      fy = 0.0f;
    }

    m.Translate(fx, fy);
  }

  // 设置子图扫描线指针及行偏移宽度
  UINT* pix = (UINT*)dst.Scan0;
  INT dstOffset = (dst.Stride >> 2) - dst.Width;
  // 几何变换逆矩阵的平移量为与子图原点对应的源图起始坐标点
  MatrixElements e = m.GetElements();
  float xs = e.dx;
  float ys = e.dy;

  // 逐点计算并复制源图几何变换后的数据到目标子图
  for (y = 0; y < (INT)dst.Height; y ++, pix += dstOffset, xs += e.m21, ys += e.m22) {
    float xs0 = xs;
    float ys0 = ys;

    for (x = 0; x < (INT)dst.Width; x ++, pix ++, xs0 += e.m11, ys0 += e.m12) {
      INT x0 = xs0 < 0.0f ? (INT)(xs0 - 0.5f) : (INT)(xs0 + 0.5f);
      INT y0 = ys0 < 0.0f ? (INT)(ys0 - 0.5f) : (INT)(ys0 + 0.5f);

      if (y0 >= 0 && y0 < (INT)source->Height && x0 >= 0 && x0 < (INT)source->Width) {
        *pix = *(UINT*)((CHAR*)source->Scan0 + y0 * source->Stride + (x0 << 2));
      }
    }
  }
}
上面图像几何变换函数的几个特点：
1、可以实现任意的图像几何变换（只要TransformMatrix能正确表达的，即变换矩阵可逆）；
2、采用了GDI + 的BitmapData结构（转换为32位ARGB像素格式），而并非任何具体的图像格式，保证了其通用性；
3、函数使用浮点数运算，但在计算像素点位置时避免了通常的浮点数乘除运算，既提高了一定的运算速度，也为以后修改为定点数运算奠定了基础；
4、函数采用临近像素插值，且没有边界像素处理代码，像素复制质量较差。
可以看出，Transform函数的着重点在于特点（1），在实际的实现代码中，可以把它作为一个框架进行扩充和修改。
下面是一个利用Transform函数对GDI + 位图进行旋转变换的例子（使用BCB2007）：
[cpp] view plaincopyprint ?
void __fastcall TForm1::Button1Click(TObject* Sender)
{
  // 获取源图像扫描线数据
  Gdiplus::Bitmap* bmp = new Gdiplus::Bitmap(L"..\\..\\media\\IMG_9440_mf.jpg");
  BitmapData source, dest;
  LockBitmap(bmp, &source);
  // 设置几何变换
  TransformMatrix matrix;
  matrix.Rotate(45);
  // 建立目标位图并获取其扫描线数据
  RECT r;
  matrix.GetTransformRect(source.Width, source.Height, r);
  Gdiplus::Bitmap* newBmp = new Gdiplus::Bitmap(
      r.right - r.left, r.bottom - r.top, PixelFormat32bppARGB);
  LockBitmap(newBmp, &dest);
  // 执行图像几何变换
  Transform(&dest, 0, 0, &source, &matrix);
  // 释放图像扫描线数据(位图解锁)
  UnlockBitmap(newBmp, &dest);
  UnlockBitmap(bmp, &source);
  // 画几何变换后的图像
  Gdiplus::Graphics* g = new Gdiplus::Graphics(Canvas->Handle);
  g->DrawImage(newBmp, 0, 0);
  delete g;
  delete newBmp;
  delete bmp;
}

下面是图像旋转变换例子运行界面截图：
旋转45度
由于图像几何变换是以源图原点（0，0）为变换原点，所以界面上只能看到原点右下边的图像。还有些几何变换，如旋转90度、180度等，可能会导致几何变换后的图像完全不可见，为了直观的看到各种几何变换后的完整图像，可以修改一下例子代码，将TransformMatrix::GetTransformRect函数返回矩形的左上边部分也包括进来：
[cpp] view plaincopyprint ?
void __fastcall TForm1::Button1Click(TObject* Sender)
{
  // 获取源图像扫描线数据
  Gdiplus::Bitmap* bmp = new Gdiplus::Bitmap(L"..\\..\\media\\IMG_9440_mf.jpg");
  BitmapData source, dest;
  LockBitmap(bmp, &source);
  // 设置几何变换
  TransformMatrix matrix;
  matrix.Rotate(45);
  // 建立目标位图并获取其扫描线数据
  RECT r;
  matrix.GetTransformRect(source.Width, source.Height, r);
  Gdiplus::Bitmap* newBmp = new Gdiplus::Bitmap(
      r.right - r.left, r.bottom - r.top, PixelFormat32bppARGB);
  LockBitmap(newBmp, &dest);
  // 执行图像几何变换。
  // 注意这里使用-r.left, -r.top为坐标，使得变换后的图像完全可见
  Transform(&dest, -r.left, -r.top, &source, &matrix);
  // 释放图像扫描线数据(位图解锁)
  UnlockBitmap(newBmp, &dest);
  UnlockBitmap(bmp, &source);
  // 画几何变换后的图像
  Gdiplus::Graphics* g = new Gdiplus::Graphics(Canvas->Handle);
  g->DrawImage(newBmp, 0, 0);
  delete g;
  delete newBmp;
  delete bmp;
}

C++图像处理 -- 图像黑白调整应用
分类： C / C++ C / C++图形图像 2011 - 07 - 10 15: 05 4505人阅读 评论(18) 收藏 举报
cCc++C++gdi + GDI + 图像着色图像黑白调整
阅读提示：
《C++图像处理》系列以代码清晰，可读性为主，全部使用C++代码。
《Delphi图像处理》系列以效率为侧重点，一般代码为PASCAL，核心代码采用BASM。
尽可能保持二者内容一致，可相互对照。
本文代码必须包括《C++图像处理 -- 数据类型及公用函数》文章中的BmpData.h头文件。
Photoshop CS的图像黑白调整功能，是通过对红、黄、绿、青、蓝和洋红等6种颜色的比例调节来完成的。能更精细地将彩色图片转换为高质量的黑白照片。
Photoshop CS图像黑白调整功能的计算公式为：
gray = (max - min) * ratio_max + (mid - min) * ratio_max_mid + min
    公式中：gray为像素灰度值，max、mid和min分别为图像像素R、G、B分量颜色的最大值、中间值和最小值，ratio_max为max所代表的分量颜色（单色）比率，ratio_max_mid则为max与mid两种分量颜色所形成的复色比率。
    用上面公式计算的灰度值，与我们通常所用的灰度计算方法有很大不同，通常所用的灰度公式为，是直接将颜色各分量乘以相应的比率相加而成，如：gray = 0.3R + 0.59G + 0.11B，而上面公式则是在最小值代表的颜色分量基础上，用最大值与最小值之差表示单色部分（红、绿、蓝），用中间值与最小值之差表示复色部分（黄、青、洋红），将单色和复色部分分别乘以与之对应的比率后相加，再加上最小值而得到灰度值。对于每个单独的像素来说，计算灰度值只需要用到上述6种颜色比率中的2种即可。在计算过程中可根据像素RGB相互关系选择对应的单色和复色比率，如像素RGB的大小关系为R > G > B，单色比率选最大值R红色，复色比率则为最大值R与中间值G所形成的复色黄色。
        用程序代码实现上面的灰度计算公式并不复杂，难点还是前面所说的根据像素RGB相互关系选择对应的单色和复色比率。在前天我写的《C++图像处理 -- 图像颜色混合（上）》文章中，已经实现了这项功能，同时，Photoshop图像黑白调整功能中附加的着色功能，也在文章中实现。本文的在上面文章代码基础上，编写一个相对简单的图像黑白调整界面，来实现图像动态黑白调整。
        下面是用BCB2007写的一个界面程序代码：
        程序头文件部分：
        [cpp] view plaincopyprint ?

#ifndef bwMainH
#define bwMainH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#define USE_GDIPLUS
#include "BmpData.h"

        enum TLockType {ltEdit, ltTrack};
typedef Set<TLockType, ltEdit, ltTrack> TLockTypes;
struct TForm1 : public TForm {
__published: // IDE-managed Components
  TPaintBox* PaintBox1;
  TLabel* Label1;
  TLabel* Label2;
  TLabel* Label3;
  TLabel* Label4;
  TLabel* Label5;
  TLabel* Label6;
  TLabel* Label7;
  TLabel* Label8;
  TLabel* Label9;
  TLabel* Label10;
  TLabel* Label11;
  TLabel* Label12;
  TLabel* Label13;
  TLabel* Label18;
  TComboBox* ComboBox1;
  TEdit* Edit1;
  TTrackBar* TrackBar1;
  TEdit* Edit2;
  TTrackBar* TrackBar2;
  TEdit* Edit3;
  TTrackBar* TrackBar3;
  TEdit* Edit4;
  TTrackBar* TrackBar4;
  TEdit* Edit5;
  TTrackBar* TrackBar5;
  TEdit* Edit6;
  TTrackBar* TrackBar6;
  TCheckBox* CheckBox1;
  TGroupBox* GroupBox1;
  TLabel* Label14;
  TLabel* Label15;
  TLabel* Label16;
  TLabel* Label17;
  TPaintBox* PaintBox2;
  TEdit* Edit7;
  TTrackBar* TrackBar7;
  TEdit* Edit8;
  TTrackBar* TrackBar8;
  TColorDialog* ColorDialog1;
  void __fastcall FormCreate(TObject* Sender);
  void __fastcall FormDestroy(TObject* Sender);
  void __fastcall ComboBox1Change(TObject* Sender);
  void __fastcall TrackBar1Change(TObject* Sender);
  void __fastcall Edit1Change(TObject* Sender);
  void __fastcall Edit1KeyPress(TObject* Sender, char& Key);
  void __fastcall Edit1Exit(TObject* Sender);
  void __fastcall CheckBox1Click(TObject* Sender);
  void __fastcall TrackBar7Change(TObject* Sender);
  void __fastcall Edit7Change(TObject* Sender);
  void __fastcall Edit7KeyPress(TObject* Sender, char& Key);
  void __fastcall PaintBox2Click(TObject* Sender);
  void __fastcall PaintBox1Paint(TObject* Sender);
  void __fastcall PaintBox2Paint(TObject* Sender);
  void __fastcall TrackBar8Change(TObject* Sender);
private: // User declarations
  Bitmap* Source; // 源图像
  Bitmap* Dest; // 调整后的图像
  BitmapData srcData;
  BitmapData dstData;
  float bwColors[6]; // 灰度选项数组
  int Bright; // 亮度
  TTrackBar* TrackBars[6]; // 灰度选项条元件数组
  TEdit* Edits[6]; // 灰度选项编辑框数组
  TLockTypes Lock;
  Gdiplus::Rect rect;
  ARGBQuad MixColor; // 混合颜色
  int __fastcall GetHue(void);
  int __fastcall GetSat(void);
  void __fastcall SetHue(int hue);
  void __fastcall SetSat(int sat);
  void __fastcall MixColorToHSV(void);
  void __fastcall HSVToMixColor(void);
  void __fastcall Execute(void);
  void __fastcall MixColorChange(void);
public: // User declarations
  __fastcall TForm1(TComponent* Owner);
  __property int Hue = {read = GetHue, write = SetHue}; // 色相
  __property int Sat = {read = GetSat, write = SetSat}; // 饱和度
};

const CustomIndex = 11; // 自定义选项索引
const DefaultTint = 0xe1d3b3; // 缺省混合颜色
const int DefOptions[][6] = { // 预定义灰度选项
  {40, 60, 40, 60, 20, 80},
  {128, 128, 100, 100, 128, 100},
  {100, 100, 100, 100, 100, 100},
  {0, 0, 0, 0, 0, 0},
  { -40, 235, 144, -68, -3, -107},
  {120, 110, -10, -50, 0, 120},
  {50, 120, 90, 50, 0, 0},
  {0, 0, 0, 110, 110, 110},
  {120, 120, -10, -50, -50, 120},
  { -50, -50, -50, 150, 150, 150},
  {120, 110, 40, -30, 0, 70}
};
extern PACKAGE TForm1* Form1;

#endif
代码文件部分：
[cpp] view plaincopyprint ?

#include <vcl.h>
#pragma hdrstop
#include "bwMain.h"

#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1* Form1;

__fastcall TForm1::TForm1(TComponent* Owner)
  : TForm(Owner)
{
}

ULONG gdiplusToken;
typedef FLOAT BWParams, *PBWParams;
// 黑白调整缺省参数：红，黄，绿，洋红，蓝，青
CONST INT _BWDefault[] = {410, 614, 410, 819, 205, 614};
enum {
  BWIndexBlue = 0x40000,
  BWIndexGreen = 0x20000,
  BWIndexRed = 0x00000
};
enum {
  IndexBlue = 0x00000,
  IndexGreen = 0x10000,
  IndexRed = 0x20000
};
typedef union { // 颜色分量交换结构
  INT tmp; // 交换时用的临时变量
  struct {
    SHORT value; // 颜色分量值
    SHORT index; // 颜色分量索引
  };
} RGBIndex;

// 交换像素分量
FORCEINLINE
VOID SwapRgb(RGBIndex& a, RGBIndex& b)
{
  a.tmp ^= b.tmp;
  b.tmp ^= a.tmp;
  a.tmp ^= b.tmp;
}

// 获取黑白灰度
FORCEINLINE iNT GetBWGray(CONST PARGBQuad pixel, CONST PINT bwParams)
{
  RGBIndex max, mid, min;
  min.tmp = pixel->Blue | BWIndexBlue;
  mid.tmp = pixel->Green | BWIndexGreen;
  max.tmp = pixel->Red | BWIndexRed;

  if (max.value < mid.value) {
    SwapRgb(max, mid);
  }

  if (max.value < min.value) {
    SwapRgb(max, min);
  }

  if (min.value > mid.value) {
    SwapRgb(min, mid);
  }

  return (((max.value - mid.value) * bwParams[max.index] +
      (mid.value - min.value) * bwParams[max.index + mid.index - 1] +
      512) >> 10) + min.value;
}

VOID ColorMix(PARGBQuad pd, CONST PARGBQuad ps, INT gray)
{
  // 灰度计算常数：蓝，绿、红
  CONST INT ys[3] = {113, 604, 307};
  RGBIndex max, mid, min;
  min.tmp = ps->Blue | IndexBlue;
  mid.tmp = ps->Green | IndexGreen;
  max.tmp = ps->Red | IndexRed;

  if (max.value < mid.value) {
    SwapRgb(max, mid);
  }

  if (max.value < min.value) {
    SwapRgb(max, min);
  }

  if (min.value > mid.value) {
    SwapRgb(min, mid);
  }

  INT max_min = max.value - min.value;

  // 饱和度为0，返回灰度
  if (max_min == 0) {
    pd->Blue = pd->Green = pd->Red = gray;
    return;
  }

  INT mid_min = mid.value - min.value;
  INT newMax, newMid, newMin;
  gray <<= 10;
  newMax = (gray + (max_min - mid_min) * ys[mid.index] + max_min * ys[min.index] + 512) >> 10;
  newMin = newMax - max_min;

  if (newMax > 255) {
    INT hueCoef = (mid_min << 10) / max_min;
    INT v0 = (ys[mid.index] * hueCoef) >> 10;
    INT v1 = ys[min.index] + ys[mid.index] - v0;
    newMin = (gray - (ys[max.index] + v0) * 255 + (v1 >> 1)) / v1;
    newMid = newMin + (((255 ^ newMin) * hueCoef + 512) >> 10);
    newMax = 255;
  }
  else if (newMin < 0) {
    INT hueCoef = (mid_min << 10) / max_min;
    INT tmp = ys[max.index] + ((ys[mid.index] * hueCoef + 512) >> 10);
    newMax = (gray + (tmp >> 1)) / tmp;
    newMid = (newMax * hueCoef + 512) >> 10;
    newMin = 1;
  }
  else {
    newMid = newMin + mid_min;
  }

  ((LPBYTE)pd)[max.index] = newMax;
  ((LPBYTE)pd)[mid.index] = newMid;
  ((LPBYTE)pd)[min.index] = newMin;
}

// 图像黑白调整。
// 调整参数bwParams为元素数等于6的数组指针，分别为红，黄，绿，青，蓝，洋红
VOID ImageBWCopy(BitmapData* dest, CONST BitmapData* source, CONST PBWParams bwParams = NULL)
{
  // 拷贝像素灰度参数，并交换青色和洋红色
  INT params[6], *pparams;

  if (bwParams) {
    for (INT i = 0; i < 6; i ++) {
      params[i] = (INT)(bwParams[i] * 1024 + 0.5);
    }

    params[3] ^= params[5];
    params[5] ^= params[3];
    params[3] ^= params[5];
    pparams = params;
  }
  else {
    pparams = (INT*)_BWDefault;
  }

  PARGBQuad pd, ps;
  UINT width, height;
  INT dstOffset, srcOffset;
  GetDataCopyParams(dest, source, width, height, pd, ps, dstOffset, srcOffset);

  for (UINT y = 0; y < height; y ++, pd += dstOffset, ps += srcOffset) {
    for (UINT x = 0; x < width; x ++, pd ++, ps ++) {
      INT gray = GetBWGray(ps, pparams);
      pd->Blue = pd->Green = pd->Red =
          (gray & ~0xff) == 0 ? gray : gray > 255 ? 255 : 0;
    }
  }
}

// 灰度图像染色。
VOID ImageTint(BitmapData* grayData, ARGB color)
{
  ARGBQuad colorTable[256];
  PARGBQuad p = colorTable;

  for (INT i = 0; i < 256; i ++, p ++) {
    ColorMix(p, (PARGBQuad)&color, i);
    p->Alpha = 255;
  }

  p = (PARGBQuad)grayData->Scan0;
  INT dataOffset = (grayData->Stride >> 2) - (INT)grayData->Width;

  for (UINT y = 0; y < grayData->Height; y ++, p += dataOffset) {
    for (UINT x = 0; x < grayData->Width; x ++, p ++) {
      p->Color = colorTable[p->Blue].Color;
    }
  }
}

void __fastcall TForm1::FormCreate(TObject* Sender)
{
  Gdiplus::GdiplusStartupInput gdiplusStartupInput;
  GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
  TrackBars[0] = TrackBar1;
  TrackBars[1] = TrackBar2;
  TrackBars[2] = TrackBar3;
  TrackBars[3] = TrackBar4;
  TrackBars[4] = TrackBar5;
  TrackBars[5] = TrackBar6;
  Edits[0] = Edit1;
  Edits[1] = Edit2;
  Edits[2] = Edit3;
  Edits[3] = Edit4;
  Edits[4] = Edit5;
  Edits[5] = Edit6;
  // 从文件装入图像到tmp
  Bitmap* tmp = new Bitmap(L"source1.jpg");
  rect.Width = tmp->GetWidth();
  rect.Height = tmp->GetHeight();
  // 分别建立新的源和目标图像数据到srcData和dstData
  GetBitmapData(rect.Width, rect.Height, &srcData);
  GetBitmapData(rect.Width, rect.Height, &dstData);
  // 将tmp图像数据分别锁定拷贝到srcData和dstData
  tmp->LockBits(&rect,
      ImageLockModeRead | ImageLockModeWrite | ImageLockModeUserInputBuf,
      PixelFormat32bppARGB, &srcData);
  tmp->UnlockBits(&srcData);
  tmp->LockBits(&rect,
      ImageLockModeRead | ImageLockModeWrite | ImageLockModeUserInputBuf,
      PixelFormat32bppARGB, &dstData);
  tmp->UnlockBits(&dstData);
  delete tmp;
  // 分别用图像数据srcData和dstData建立位图Source和Dest
  // 注：图像数据结构用于数据处理，位图用于显示，这样即可绑定数据结构和位图，
  // 又能避免每次处理图像数据时的锁定和解锁操作
  Source = new Bitmap(srcData.Width, srcData.Height, srcData.Stride,
      PixelFormat32bppARGB, (BYTE*)srcData.Scan0);
  Dest = new Bitmap(dstData.Width, dstData.Height, dstData.Stride,
      PixelFormat32bppARGB, (BYTE*)dstData.Scan0);
  ComboBox1Change(NULL);
}

void __fastcall TForm1::FormDestroy(TObject* Sender)
{
  delete Dest;
  delete Source;
  FreeBitmapData(&dstData);
  FreeBitmapData(&srcData);
  GdiplusShutdown(gdiplusToken);
}

// 执行图像黑白调整
void __fastcall TForm1::Execute(void)
{
  for (int i = 0; i < 6; i ++) { // 获取灰度选项条数据
    bwColors[i] = TrackBars[i]->Position / 100.0;
  }

  ImageBWCopy(&dstData, &srcData, bwColors); // 源图黑白调整到目标图

  if (CheckBox1->Checked && Sat) { // 如果色调选项被选，着色
    ImageTint(&dstData, MixColor.Color);
  }

  PaintBox1Paint(NULL); // 显示图像
}

// 预设黑白调整选项改变
void __fastcall TForm1::ComboBox1Change(TObject* Sender)
{
  if (ComboBox1->ItemIndex == CustomIndex) {
    return;
  }

  MixColor.Color = DefaultTint; // 设置缺省混合颜色
  MixColorToHSV(); // 计算并设置缺省色相、饱和度控件
  Lock = TLockTypes() << ltEdit << ltTrack;

  try {
    for (int i = 0; i < 6; i ++) { // 装入预设的选项数据到相应的控件
      TrackBars[i]->Position = DefOptions[ComboBox1->ItemIndex][i];
      Edits[i]->Text = DefOptions[ComboBox1->ItemIndex][i];
    }

    if (CheckBox1->Checked) {
      CheckBox1->Checked = FALSE; // 取消色调选项
    }
    else {
      Execute();
    }
  }

  __finally {
    Lock.Clear();
  }
}

// 黑白调整数据选项条改变
void __fastcall TForm1::TrackBar1Change(TObject* Sender)
{
  if (Lock.Contains(ltTrack)) {
    return;
  }

  Lock = TLockTypes() << ltEdit;

  try {
    TTrackBar* bar = (TTrackBar*)Sender;
    Edits[bar->Tag]->Text = bar->Position;
    ComboBox1->ItemIndex = CustomIndex; // 预设下拉框设置为自定义
    Execute();
  }

  __finally {
    Lock.Clear();
  }
}

// 黑白调整数据编辑框改变
void __fastcall TForm1::Edit1Change(TObject* Sender)
{
  if (Lock.Contains(ltEdit)) {
    return;
  }

  TEdit* edit = (TEdit*)Sender;

  if (edit->Text != "" && edit->Text != "-") {
    TrackBars[edit->Tag]->Position = StrToInt(edit->Text);
  }
}

void __fastcall TForm1::Edit1KeyPress(TObject* Sender, char& Key)
{
  if (Key >= ' ' && Key != '-' && (Key < '0' || Key > '9')) {
    Key = 0;
  }
}

void __fastcall TForm1::Edit1Exit(TObject* Sender)
{
  TEdit* edit = (TEdit*)Sender;

  if (edit->Text == "") {
    edit->Text = TrackBars[edit->Tag]->Position;
  }
}

// 混合颜色改变，画混合颜色，显示其RGB值
void __fastcall TForm1::MixColorChange(void)
{
  PaintBox2Paint(NULL);
  Label18->Caption = "R: " + IntToStr(MixColor.Red) +
      ", G: " + MixColor.Green +
      ", B: " + MixColor.Blue;
  Execute();
}

inline void RgbSwap(int& a, int& b)
{
  a ^= b;
  b ^= a;
  a ^= b;
}
// 按混合颜色计算并改变HSV
void __fastcall TForm1::MixColorToHSV(void)
{
  int max, mid, min;
  max = MixColor.Red;
  mid = MixColor.Green;
  min = MixColor.Blue;

  if (max < mid) {
    RgbSwap(max, mid);
  }

  if (max < min) {
    RgbSwap(max, min);
  }

  if (min > mid) {
    RgbSwap(min, mid);
  }

  int max_min = max - min;

  if (max_min == 0) {
    Hue = 0;
    Sat = 0;
  }
  else {
    int H;

    if (max == MixColor.Red) {
      H = ((MixColor.Green - MixColor.Blue) * 60 + 30) / max_min;
    }
    else if (max == MixColor.Green) {
      H = ((MixColor.Blue - MixColor.Red) * 60 + 30) / max_min + 120;
    }
    else {
      H = ((MixColor.Red - MixColor.Green) * 60 + 30) / max_min + 240;
    }

    Hue = H < 0 ? H + 360 : H;
    Sat = (max_min * 100) / max;
  }

  Bright = max;
}

inline ARGB RgbToColor(int r, int g, int b)
{
  return (r << 16) | (g << 8) | b;
}
// 按HSV计算并改变混合颜色
void __fastcall TForm1::HSVToMixColor(void)
{
  if (Sat == 0) {
    MixColor.Blue = MixColor.Green = MixColor.Red = Bright;
  }
  else {
    int index = Hue / 60;
    int f = Hue % 60;

    if ((index & 1) == 0) {
      f = 60 - f;
    }

    int a = Bright;
    int b = (Bright * (6000 - Sat * f)) / 6000;
    int c = (Bright * (100 - Sat)) / 100;

    switch (index) {
    case 0:
      MixColor.Color = RgbToColor(a, b, c);
      break;

    case 1:
      MixColor.Color = RgbToColor(b, a, c);
      break;

    case 2:
      MixColor.Color = RgbToColor(c, a, b);
      break;

    case 3:
      MixColor.Color = RgbToColor(c, b, a);
      break;

    case 4:
      MixColor.Color = RgbToColor(b, c, a);
      break;

    case 5:
      MixColor.Color = RgbToColor(a, c, b);
    }
  }

  MixColorChange();
}

int __fastcall TForm1::GetHue(void)
{
  return TrackBar7->Position;
}

int __fastcall TForm1::GetSat(void)
{
  return TrackBar8->Position;
}

void __fastcall TForm1::SetHue(int hue)
{
  if (Hue == hue) {
    return;
  }

  Lock = TLockTypes() << ltEdit << ltTrack;

  try {
    TrackBar7->Position = hue;
    Edit7->Text = hue;
  }

  __finally {
    Lock.Clear();
  }
}

void __fastcall TForm1::SetSat(int sat)
{
  if (Sat == sat) {
    return;
  }

  Lock = TLockTypes() << ltEdit << ltTrack;

  try {
    TrackBar8->Position = sat;
    Edit8->Text = sat;
  }

  __finally {
    Lock.Clear();
  }
}

// 色调选盒改变
void __fastcall TForm1::CheckBox1Click(TObject* Sender)
{
  Label14->Enabled = CheckBox1->Checked;
  Label15->Enabled = CheckBox1->Checked;
  Label16->Enabled = CheckBox1->Checked;
  Label17->Enabled = CheckBox1->Checked;
  Label18->Visible = CheckBox1->Checked;
  Edit7->Enabled = CheckBox1->Checked;
  Edit8->Enabled = CheckBox1->Checked;
  TrackBar7->SliderVisible = CheckBox1->Checked;
  TrackBar8->SliderVisible = CheckBox1->Checked;

  if (CheckBox1->Checked) {
    ComboBox1->ItemIndex = CustomIndex;
  }

  MixColorChange();
}

// 色相选项条改变
void __fastcall TForm1::TrackBar7Change(TObject* Sender)
{
  if (!Lock.Contains(ltTrack)) {
    Edit7->Text = TrackBar7->Position;
  }
}

// 饱和度选项条改变
void __fastcall TForm1::TrackBar8Change(TObject* Sender)
{
  if (!Lock.Contains(ltTrack)) {
    Edit8->Text = TrackBar8->Position;
  }
}

// 色相或者饱和度编辑框改变
void __fastcall TForm1::Edit7Change(TObject* Sender)
{
  TEdit* edit = (TEdit*)Sender;

  if (Lock.Contains(ltEdit) || edit->Text == "") {
    return;
  }

  Lock = TLockTypes() << ltTrack;

  try {
    int val = StrToInt(edit->Text);
    TTrackBar* bar = edit->Tag == 0 ? TrackBar7 : TrackBar8;

    if (bar->Position != val) {
      bar->Position = val;
    }

    HSVToMixColor();
  }

  __finally {
    Lock.Clear();
  }
}

void __fastcall TForm1::Edit7KeyPress(TObject* Sender, char& Key)
{
  if (Key >= ' ' && (Key < '0' || Key > '9')) {
    Key = 0;
  }
}

// 调用颜色对话框选择混合颜色
void __fastcall TForm1::PaintBox2Click(TObject* Sender)
{
  if (CheckBox1->Checked && ColorDialog1->Execute(Handle)) {
    MixColor.Color = (ARGB)ColorDialog1->Color;
    MixColor.Blue = MixColor.Red;
    MixColor.Red = (BYTE)ColorDialog1->Color;
    MixColorToHSV();
    MixColorChange();
  }
}

// 画黑白调整图像和源图像
void __fastcall TForm1::PaintBox1Paint(TObject* Sender)
{
  Gdiplus::Graphics* g = new Gdiplus::Graphics(PaintBox1->Canvas->Handle);

  try {
    g->DrawImage(Dest, rect);

    if (Sender != NULL) {
      g->TranslateTransform(0, rect.Height);
      g->DrawImage(Source, rect);
    }
  }

  __finally {
    delete g;
  }
}

// 画混合颜色
void __fastcall TForm1::PaintBox2Paint(TObject* Sender)
{
  if (CheckBox1->Checked)
    PaintBox2->Canvas->Brush->Color =
        (MixColor.Blue << 16) | (MixColor.Green << 8) | MixColor.Red;
  else {
    PaintBox2->Canvas->Brush->Color = Color;
  }

  PaintBox2->Canvas->Pen->Color = Color;
  PaintBox2->Canvas->Rectangle(PaintBox2->ClientRect);
}

界面程序中，实现图像黑白调整功能主要靠Execute函数完成。
下面是几张程序运行界面图：
1、缺省黑白调整参数运行界面，其中右上边的下拉编辑框显示的是一些预设黑白效果选项：
2、选择红外线效果黑白调整参数运行界面：
3、使用缺省参数进行黑白调整后，再用所选颜色进行着色的界面：
4、在上面界面基础上，色调不变，加大黄色调参数，使图中人物衣服颜色明亮一些，同时减少蓝色调参数，使人物的围脖颜色变暗一些：
因水平有限，错误在所难免，欢迎指正和指导。邮箱地址：maozefa@hotmail.com
这里可访问《C++图像处理 -- 文章索引》。
#endif

