//软件方法操作颜色缓冲区
//查询函数
//设置背景颜色
//将指定的颜色设置为背景色
//单一颜色(r, g, b, a)
void SetBkColor2(HDDC pCB, DWORD clr)
{
  int i;
  DWORD dwScanLength;
  DWORD* pbyColor = pCB->data;

  for (i = 0; i < pCB->w; i++) {
    *pbyColor++ = clr;
  }

  //内存之间拷贝, 拷贝剩下的 (pCB->nHeight - 1)行.
  dwScanLength = 4 * (DWORD)pCB->w;

  for (i = 1; i < pCB->h; i++) {
    //指向一行开始的位置
    pbyColor = pCB->data + (i * dwScanLength);
    //从第一行拷贝至第 i 行:
    memcpy(pbyColor,  pCB->data,  dwScanLength);
  }
}
//设置为一个指定的背景图像
//背景图像的数据是显式表示的, 即并不包裹于图像
//使用时的警告:图像必须与颜色缓冲区尺寸完全相同!
void SetBkImage(HDDC pCB, const BYTE* pbyImage,  int nWidth,  int nHeight)
{
  ASSERT(pbyImage);

  if ((nWidth != pCB->w) || (nHeight != pCB->h)) {
    zb_error("图像尺寸与颜色缓冲区大小不匹配");
    return;
  }

  CopyMemory(pCB->data,  pbyImage,  pCB->w * pCB->h * 4);
}
//设置一个点的颜色
//这是z-buffer算法关于颜色缓冲区的主要操作
void CBSetPixel(HDDC pCB, int x,  int y,  float r,  float g,  float b , float a)
{
  if ((x < 0) || (y < 0) || (x >= pCB->w) || (y >= pCB->h)) {
    return;
  }

  {
    DWORD dwIndex = y * pCB->w * 4 + 4 * x;
    BYTE byRed = (BYTE)(r * 255.0f);
    BYTE byGreen = (BYTE)(g * 255.0f);
    BYTE byBlue = (BYTE)(b * 255.0f);
    BYTE byAlpha = (BYTE)(a * 255.0f);
    BYTE* pbyColor = (BYTE*)((BYTE*)pCB->data + dwIndex);
    *pbyColor++ = byBlue;
    *pbyColor++ = byGreen;
    *pbyColor++ = byRed;
    *pbyColor++ = byAlpha;
  }
}
void CBSetPixel1(HDDC pCB, int x,  int y,  BYTE r,  BYTE g,  BYTE b ,  BYTE a)
{
  if ((x < 0) || (y < 0) || (x >= pCB->w) || (y >= pCB->h)) {
    return;
  }

  {
    DWORD dwIndex = y * pCB->w * 4 + 4 * x;
    BYTE* pbyColor = (BYTE*)pCB->data + dwIndex;
    //蓝、绿、红、Alpha
    *pbyColor++ = b;
    *pbyColor++ = g;
    *pbyColor++ = r;
    *pbyColor++ = a;
  }
}
//获取颜色
PIXELCOLORRGBA CBGetPixel(HDDC pCB, int x,  int y)
{
  PIXELCOLORRGBA rgba = {0,  0,  0,  0};

  if ((x < 0) || (y < 0) || (x >= pCB->w) || (y >= pCB->h)) {
    return rgba;
  }

  {
    DWORD dwIndex = y * pCB->w * 4 + 4 * x;
    BYTE* pbyColor = (BYTE*)pCB->data + dwIndex;
    //蓝、绿、红、Alpha
    rgba.blue = *pbyColor++;
    rgba.green = *pbyColor++;
    rgba.red = *pbyColor++;
    rgba.alpha = *pbyColor++;
    return rgba;
  }
}
//由颜色缓冲区中的数据直接创建一个DDB对象, 即HBITMAP对象
HBITMAP CBCreateDdb(HDDC pCB, HDC hDC)
{
  //第一步, 创建一个BITMAPV4HEADER结构
  HBITMAP hBitmap;
  //指向像素数据的指针,
  BITMAPV4HEADER pbmih[1];
  pbmih->bV4Size = sizeof(BITMAPV4HEADER);  //108
  pbmih->bV4Width = (LONG)pCB->w;
  pbmih->bV4Height = (LONG)(-pCB->h);
  pbmih->bV4Planes = 1;
  pbmih->bV4BitCount = 32;
  pbmih->bV4V4Compression = BI_RGB;
  pbmih->bV4SizeImage = 0;
  pbmih->bV4XPelsPerMeter = 0;
  pbmih->bV4YPelsPerMeter = 0;
  pbmih->bV4ClrUsed = 0;
  pbmih->bV4ClrImportant = 0;
  //第二步, 创建一个DDB
  hBitmap = CreateDIBitmap(hDC, (BITMAPINFOHEADER*)pbmih, CBM_INIT, pCB->data, (BITMAPINFO*)pbmih, DIB_RGB_COLORS);
  return hBitmap;
}
//将颜色缓冲区的内容交换到前台(即输出颜色缓冲区)
void CBSwapBuffer(HDDC pCB, HDC hDC)
{
  HDC memDC = CreateCompatibleDC(hDC);
  HBITMAP hBitmap = CBCreateDdb(pCB, hDC);
  HBITMAP hOldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);
  BitBlt(hDC, 0, 0, pCB->w, pCB->h, memDC, 0, 0, SRCCOPY);
  DeleteObject(hBitmap);
  SelectObject(memDC, hOldBitmap);
  DeleteDC(memDC);
}//32位深度缓冲

#define newZBuffer(nWidth, nHeight) newMatC(nWidth, nHeight, T_FLOAT, sizeof(float))
//初始化深度
void initAllDepth(HMC pZB, float fDepth)
{
  int i, nSize = pZB->w * pZB->h;
  ASSERT(pZB->t == T_FLOAT);

  for (i = 0; i <  nSize; i++) {
    ((float*)(pZB->data))[i] = fDepth;
  }
}
//设置深度值
void SetDepth(HMC pZB, int x,  int y,  float fDepth)
{
  ASSERT(pZB->t == T_FLOAT);

  if ((x >= 0) && (y >= 0) && (x < pZB->w) && (y < pZB->h)) {
    ((float*)(pZB->data))[y * pZB->w + x] = fDepth;
  }
}
//获取深度值
#define GetDepth(pZB, x,  y)  (((x >= 0) && (y >= 0) && (x < pZB->w) && (y < pZB->h)) ? _PixelT(pZB, x, y, float) : -1.0f)

//将观察坐标系下的点变换到屏幕坐标系
//事实上, 这个变换适合于视点在Z坐标上的情形
//vertex----观察坐标系下的点
//在这里, 我们作了简化
//事实上, 应该考虑
//第一, 视点至世界坐标系原点的距离 与 视距 之间的关系;
//第二, 物体、屏幕、视点之间的位置关系
//我们规定:
//第一, 只能看见视点前方的物体, 这样 ze 应该大于 0.0f
//    比如, 视点位于场景的外接球之外, 即可满足这一要求
//第二, 屏幕在视点前方.
//我们并非开发一个图形系统!
//右手平面直角坐标系变为左手直角坐标系
VECTOR3D CViewFinder_PerspectiveH(VIEWFINDER* pVF, VECTOR3D vertex)
{
  VECTOR3D pt = vertex;
  float xe,  ye,  ze, w = vertex.w;

  //对于一个无穷远点
  if (ABS(w) <= 0.00001f) {
    xe = vertex.x;
    ye = vertex.y;
    ze = 1000000.0f;
  }
  else {
    xe = vertex.x / w;
    ye = vertex.y / w;
    ze = vertex.z / w;
  }

  //对于ze = 0.0f的情况, 这里只作简化处理:
  if (ABS(ze) < 0.00001f) {
    ze = 1.0f;
  }

  //屏幕上的点
  pt.x = ((xe * pVF->fDistView) / ze * pVF->xScale + pVF->ptCenter.x);
  pt.y = ((-ye * pVF->fDistView) / ze * pVF->yScale + pVF->ptCenter.y);
  return pt;
}
