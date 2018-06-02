#include "Grphcs.inl"
#include "Math3d.inl"
#include "Object3d.inl"
#include "Buffer.inl"
#include "Light.inl"
#include "Sphere.inl"
typedef struct _POINTV {
  union {
    POINT p;
    struct {
      int x, y;
    };
  };
  POINT t;
  IRGBA ic;
  float z;
} POINTV;
#define GRADIENTIRGBSET(_V, _N, _I1, _V1, _V2)    (_V.r=GRADIENT(_N, _I1, (_V1.r), (_V2.r)),_V.g=GRADIENT(_N, _I1, (_V1.g), (_V2.g)),_V.b=GRADIENT(_N, _I1, (_V1.b), (_V2.b)))

// 渐变填充梯形，用四种颜色差值
int DrawGouraudTrapezoid(HDDC hDC, HMC hZB, POINTV* p, HDDC hS, MATRIX2D* pm, BOOL bSmooth)
{
  int x, y, n, _j;
  int _r, _g, _b;
  POINTV p13, p24, t;
  MATRIX2Di im;
  int x1 = p[0].x, x2 = p[1].x, x3 = p[2].x, x4 = p[3].x, y1 = p[0].y, y2 = p[3].y;

  if (pm) {
    M2DF2FIX(im, (*pm));
  }

  assert((p[1].y == p[0].y) && (p[3].y == p[2].y));
  assert((x2 >= x1) && (x4 >= x3));
  assert((y2 >= y1));
  y = y1, n = y2 - y1;
  logtime(0);
  logecho(0);

  if (!bSmooth) {
    t.ic = p[0].ic;

    for (; y != y2; y++) {
      int i1 = y - y1;
      p13.x = GET_LINE_X(x1, y1, x3, y2, y);
      p24.x = GET_LINE_X(x2, y1, x4, y2, y);
      p13.z = GRADIENT(n, i1, p[2].z, p[0].z);
      p24.z = GRADIENT(n, i1, p[3].z, p[1].z);

      //DRAWHLINE_GRADIENTZB(hDC, hZB, p13.x, x12, y, p13.c, clr24, p13.z, z24);
      if (y < hDC->h && y >= 0) {
        int _X1 = MAX(MIN(p13.x, p24.x), 0), _X2 = MIN(MAX(p13.x, p24.x), hDC->w), _W = 1 + _X2 - _X1;
        DWORD* _A = &_Pixel(hDC, _X1, y);
        float* _Z = &_PixelT(hZB, _X1, y, float);
        float _z1 = p13.z, dz = (p24.z - p13.z) / _W;

        for (_j = 0; _j < _W; ++_j, _z1 += dz) {
          if (_Z[_j] > _z1) {
            _Z[_j] = _z1;
            _r = FIX2I(t.ic.r * 255), _g = FIX2I(t.ic.g * 255), _b = FIX2I(t.ic.b * 255);

            if (hS && hS->data) {
              int px, py, pz;
              x = _j + _X1;
              M2TRANSFORM_3X3_FIX(im, x, y, 1, px, py, pz);

              if (px >= 0 && px < hS->w && py >= 0 && py < hS->h) {
                DWORD c = _Pixel(hS, px, py), n = 50;
                _r = (_r * GetRV(c)) / n;
                _g = (_g * GetGV(c)) / n;
                _b = (_b * GetBV(c)) / n;
                _A[_j] = MINRGB(_r, _g, _b);
                //_A[_j]=_Pixel(hS, px, py);
              }
            }
            else {
              _A[_j] = MINRGB(_r, _g, _b);
            }
          }
        }
      }
    }

    return 0;
  }
  else {
    for (; y != y2; y++) {
      int i1 = y - y1;
      p13.x = GET_LINE_X(x1, y1, x3, y2, y);
      p24.x = GET_LINE_X(x2, y1, x4, y2, y);
      GRADIENTIRGBSET(p13.ic, n, i1, p[2].ic, p[0].ic);
      GRADIENTIRGBSET(p24.ic, n, i1, p[3].ic, p[1].ic);
      p13.z = GRADIENT(n, i1, p[2].z, p[0].z);
      p24.z = GRADIENT(n, i1, p[3].z, p[1].z);

      //DRAWHLINE_GRADIENTZB(hDC, hZB, p13.x, x12, y, p13.c, clr24, p13.z, z24);
      if (y < hDC->h && y >= 0) {
        int _X1 = MAX(MIN(p13.x, p24.x), 0), _X2 = MIN(MAX(p13.x, p24.x), hDC->w), _W = 1 + _X2 - _X1;
        DWORD* _A = &_Pixel(hDC, _X1, y);
        float* _Z = &_PixelT(hZB, _X1, y, float);
        float _z1 = p13.z, dz = (p24.z - p13.z) / _W;

        for (_j = 0; _j < _W; ++_j, _z1 += dz) {
          if (_Z[_j] > _z1) {
            int _r, _g, _b;
            _Z[_j] = _z1;
            GRADIENTIRGBSET(t.ic, _W, _j, p24.ic, p13.ic);
            _r = FIX2I(t.ic.r << 8), _g = FIX2I(t.ic.g << 8), _b = FIX2I(t.ic.b << 8);

            if (hS && hS->data) {
              int px, py, pz;
              x = _j + _X1;
              M2TRANSFORM_3X3_FIX(im, x, y, 1, px, py, pz);

              if (px >= 0 && px < hS->w && py >= 0 && py < hS->h) {
                DWORD c = _Pixel(hS, px, py), n = 250;
                _r = (_r * GetRV(c)) / n;
                _g = (_g * GetGV(c)) / n;
                _b = (_b * GetBV(c)) / n;
                _A[_j] = MINRGB(_r, _g, _b);
                //_A[_j]=_Pixel(hS, px, py);
              }
            }
            else {
              _A[_j] = MINRGB(_r, _g, _b);
              //logprintf("%3d ", _r);
            }
          }
        }

        //logprintf("\n================\n");
      }
    }
  }

  return 0;
}
int DrawGouraudTriangle(HDDC hDC, HMC hZB, POINTV* pt3, HDDC hS, BOOL bSmooth)
{
  int n, i1;
  POINTV pt4[4], t;
  MATRIX2D m;
  M2Slove(&m,
      pt3[0].x, pt3[0].y, 1.f, pt3[0].t.x, pt3[0].t.y, 1.f,
      pt3[1].x, pt3[1].y, 1.f, pt3[1].t.x, pt3[1].t.y, 1.f,
      pt3[2].x, pt3[2].y, 1.f, pt3[2].t.x, pt3[2].t.y, 1.f);
  TriSort(pt3, t);

  if (!bSmooth) {
    IRGBA t;
    //一般地, 采用第一个顶点的颜色来表示小面的颜色
    //这对模型的顶点安排要求比较高
    //因而, 我们采用三个顶点颜色的平均值来表示小面的颜色
    //在光照条件下, 这样处理也是可以接受的
    //顶点颜色归一化应该在外部进行
    //透明度, 这里也简单地处理成同一值
    t.r = (pt3[0].ic.r + pt3[1].ic.r + pt3[2].ic.r) / 3;
    t.g = (pt3[0].ic.g + pt3[1].ic.g + pt3[2].ic.g) / 3;
    t.b = (pt3[0].ic.b + pt3[1].ic.b + pt3[2].ic.b) / 3;
    t.a = (pt3[0].ic.a + pt3[1].ic.a + pt3[2].ic.a) / 3;
    pt3[0].ic = pt3[1].ic = pt3[2].ic = t;
  }

  if (pt3[1].y == pt3[0].y) {
    pt4[0] = pt3[0], pt4[1] = pt3[1], pt4[2] = pt3[2], pt4[3] = pt3[2];

    if (pt4[0].x > pt4[1].x) {
      SWAP(pt4[0], pt4[1], t);
    }

    DrawGouraudTrapezoid(hDC, hZB, pt4, hS, &m, bSmooth);
    return 0;
  }

  if (pt3[1].y == pt3[2].y) {
    pt4[0] = pt3[0], pt4[1] = pt3[0], pt4[2] = pt3[1], pt4[3] = pt3[2];

    if (pt4[2].x > pt4[3].x) {
      SWAP(pt4[2], pt4[3], t);
    }

    DrawGouraudTrapezoid(hDC, hZB, pt4, hS, &m, bSmooth);
    return 0;
  }

  t = pt3[1];
  t.x = GET_LINE_X(pt3[0].x, pt3[0].y, pt3[2].x, pt3[2].y, pt3[1].y);
  n = pt3[2].y - pt3[0].y;
  i1 = pt3[1].y - pt3[0].y;
  GRADIENTIRGBSET(t.ic, n, i1, pt3[2].ic, pt3[0].ic);
  t.z = GRADIENT(n, i1, pt3[2].z, pt3[0].z);
  pt4[0] = pt3[0], pt4[1] = pt3[0], pt4[2] = t, pt4[3] = pt3[1];

  if (pt4[2].x > pt4[3].x) {
    SWAP(pt4[2], pt4[3], t);
  }

  DrawGouraudTrapezoid(hDC, hZB, pt4, hS, &m, bSmooth);
  pt4[0] = pt4[2], pt4[1] = pt4[3], pt4[2] = pt3[2], pt4[3] = pt3[2];
  DrawGouraudTrapezoid(hDC, hZB, pt4, hS, &m, bSmooth);
  return 0;
}
int DrawGouraudWirize(HDDC hDC, HMC hZB, POINTV* p, DWORD dwRop)
{
  int x1 = p[0].x, y1 = p[0].y, x2 = p[1].x, y2 = p[1].y;
  float z1 = p[0].z, z2 = p[1].z;
  IRGBA clr1 = p[0].ic, clr2 = p[1].ic;
  int i, x, y, increx, increy, nx = 0, ny = 0, step = hDC->bw >> 2, steps, zstep = hZB->bw >> 2;
  DWORD* ptr;
  float* zb;
  POINT p1, p2;
  int _r, _g, _b;
  p1.x = x1, p1.y = y1, p2.x = x2, p2.y = y2;
  i = TClipLine(0, 0, hDC->h, hDC->w, &p1.x, &p1.y, &p2.x, &p2.y);

  if (!i) {
    return 0;
  }

  x1 = p1.x, y1 = p1.y, x2 = p2.x, y2 = p2.y;

  if (abs(x2 - x1) > abs(y2 - y1)) {
    steps = abs(x2 - x1);
  }
  else {
    steps = abs(y2 - y1);
  }

  if (0 == steps) {
    return 0;
  }

  increx = ((x2 - x1) << 16) / steps;
  increy = ((y2 - y1) << 16) / steps;
  x = 0;
  y = 0;
  ptr = &_Pixel(hDC, x1, y1);
  zb = &_PixelT(hZB, x1, y1, float);

  for (i = 0; i <= steps; ++i) {
    float _z1;
    int n = steps + 1;
    //在(x,y)处,以clr色画点
    int ix = x >> 16, iy = y >> 16;
    _z1 = GRADIENT(n, i, z2, z1);

    if (zb[ix + iy * zstep] > _z1) {
      IRGBA t;

      if (G3D_FLAT & dwRop) {
        t = clr1;
        _r = FIX2I(t.r * 255), _g = FIX2I(t.g * 255), _b = FIX2I(t.b * 255);
      }
      else {
        GRADIENTIRGBSET(t, n, i, clr2, clr1);
        _r = FIX2I(t.r * 255), _g = FIX2I(t.g * 255), _b = FIX2I(t.b * 255);
      }

      ptr[ix + iy * step] = MINRGB(_r, _g, _b);
      zb[ix + iy * zstep] = _z1;
    }

    x += increx;
    y += increy;
  }

  return 0;
}

//计算面的法线和顶点的平均法线
//顶点的平均法线为与该顶点相连的面的法线的平均值
//变换法线, m为已经的变换矩阵, 只适用于刚体运动
int CalcAllNoraml(int nV, Vertex3d* pV, HMC hF, MATRIX3D* m)
{
  int i, iv[3];

  for (i = 0; i < nV; ++i) {
    V3SET(pV[i].normal, 0, 0, 0);
  }

  if (hF) {
    for (i = 0; i < hF->h; ++i) {
      VECTOR3D n;
      GetMatElemV(iv, hF, i, int);
      ASSERT(iv[0] < nV);
      ASSERT(iv[1] < nV);
      ASSERT(iv[2] < nV);
      n = Normal(&pV[iv[0]].coord, &pV[iv[1]].coord, &pV[iv[2]].coord);
      V3ADD1(pV[iv[0]].normal, n);
      V3ADD1(pV[iv[1]].normal, n);
      V3ADD1(pV[iv[2]].normal, n);
      //pO->sList[i].normal=M3TransformV(m, n);
    }
  }

  for (i = 0; i < nV; ++i) {
    UNITIZE(pV[i].normal);
    pV[i].normal = M3TransformV(m, pV[i].normal);
  }

  return 0;
}
//开始渲染
Vertex3d* newVertex3d(int nV)
{
  Vertex3d* pV = (Vertex3d*)MALLOC(Vertex3d, nV);
  memset(pV, 0, sizeof(Vertex3d)*nV);
  initVertex3d(pV, nV);
  return pV;
}

//开始渲染
Vertex3d* BeginRender(HMC hV, HMC hC, HMC hF, MATRIX3D* m)
{
  int i, nV = hV->h;
  Vertex3d* pV;
  FRGBA clr = {1, 1, 1, 1};
  float mr = MaxRadius(hV);
  pV = newVertex3d(nV);
  assert(hF->w >= 3);

  for (i = 0; i < nV; ++i) {
    V3SET(pV[i].coord, 0, 0, 0);
    //pV[i].coord = _PixelT(hV, 0, i, VECTOR3D);
    GetMatElemV(pV[i].coord, hV, i, float);
    V3OPTv(pV[i].coord, /= mr);
    pV[i].clr = hC ? _PixelT(hC, 0, i, FRGBA) : clr;
    //printf("%f %f %f\n", pV[i].coord.x, pV[i].coord.y, pV[i].coord.z);
  }

  //物体的顶点法线
  CalcAllNoraml(nV, pV, hF, m);
  return pV;
}
#define ADDVERTEX3DF(pV, nV, X, Y, Z) (V3SET(pV[nV].coord, X, Y, Z), ++nV)
#define g3dBegin(rDC, _t, nV) { int _nV=0; Vertex3d* _pV=newVertex3d(nV); FRGBA _clr; rDC->dwRop=_t;
#define g3dColor3f(R,G,B)    FRGBSET(_clr, R,G,B)
#define g3dVertex3f(X,Y,Z)   V3SET(_pV[_nV].coord, X,Y,Z), _pV[_nV].clr = _clr, ++_nV;
#define g3dEnd()  CalcAllNoraml(_nV, _pV, 0, rDC->m); RenderObj(rDC, _nV, _pV, 0, 0, 0);free(_pV);}

//光照渲染，利用深度进行消隐
//pO        被光照的单个物体
int RenderObj(RenderDC* rDC, int nV, Vertex3d* pV, HMC hF, HMC hT, HMC hFT)
{
  HDDC hDC = rDC->hDC;
  MATRIX3D* m = rDC->m;
  int i, j, n1, n2, n3;
  TDrawDC hTex1[1] = {0};
  POINTV pt3[3];
  //观察矩阵，初始化为物体世界矩阵，存储将世界坐标变换为观察坐标后的结果
  MATRIX3D mWatch = *m;
  // 完成从世界坐标到观察坐标的变换
  //观察矩阵(初始化为单位矩阵)
  //M3IDENTITY(mWatch);
  //获取观察矩阵
  Watch(&mWatch, rDC->viewer.x, rDC->viewer.y, rDC->viewer.z);

  // i 按物体顶点个数计算循环
  for (i = 0; i < nV; i++) {
    VECTOR3D vInView; // 观察坐标系中的坐标
    // 几何变换，将世界坐标系的点变换为观察坐标系的点
    //获取一个顶点的齐次坐标，将顶点变换至到获取观察坐标
    vInView = M3TransformH(&mWatch, pV[i].coord);
    // 完成从观察坐标到屏幕坐标的变换
    //中间省略了如下几个步骤
    //其一, 透视剪切;其二, 设备坐标规格化
    //保存变换之结果，设置视口变换工具
    //执行缩减的透视变换和视口变换
    pV[i].scrn = CViewFinder_PerspectiveH(rDC->pVF, vInView);

    if (hF) {
      LightingVertex(pV + i, rDC);
    }
  }

  //绘制
  switch (G3D_RENDER_ELEM_MARK & rDC->dwRop) {
  case G3D_VERTICES: {
    //绘制点模型
    for (i = 0; i < nV; i++) {
      //颜色(R, G, B, A)
      float fRed = pV[i].clr.r;
      float fGreen = pV[i].clr.g;
      float fBlue = pV[i].clr.b;
      float fAlpha = pV[i].clr.a;
      //坐标
      int x = (int)pV[i].scrn.x;
      int y = (int)pV[i].scrn.y;
      //深度
      float z = pV[i].scrn.z;
      //缓冲区中的深度
      float zInBuffer = GetDepth(rDC->hZB, x, y);

      //如果当前点的z坐标小于深度缓冲区中的相同位置z的坐标
      if (z < zInBuffer) {
        //在颜色缓冲区中绘制第一个点
        CBSetPixel(hDC, x, y, fRed, fGreen, fBlue, fAlpha);
        //重置深度缓冲区的值
        SetDepth(rDC->hZB, x, y, z);
      }
    }
  }
  break;

  case G3D_LINES: {
    //绘制线框模型
    for (i = 0; i < nV - 1; i += 2) {
      //获取边的顶点索引
      int n1 = i;
      int n2 = n1 + 1;
      pt3[0].z = pV[n1].scrn.z, pt3[0].x = (int)pV[n1].scrn.x, pt3[0].y = (int)pV[n1].scrn.y, pt3[0].ic = IRGBA_FiI(pV[n1].clr);
      pt3[1].z = pV[n2].scrn.z, pt3[1].x = (int)pV[n2].scrn.x, pt3[1].y = (int)pV[n2].scrn.y, pt3[1].ic = IRGBA_FiI(pV[n2].clr);
      DrawGouraudWirize(hDC, rDC->hZB, pt3, rDC->dwRop);
    }
  }
  break;

  case G3D_TRIANGLES: {
    //绘制线框模型
    for (i = 0; i < nV - 2; i += 3) {
      //获取边的顶点索引
      int n1 = i;
      int n2 = n1 + 1;
      int n3 = n1 + 2;
      pt3[0].z = pV[n1].scrn.z, pt3[0].x = (int)pV[n1].scrn.x, pt3[0].y = (int)pV[n1].scrn.y, pt3[0].ic = IRGBA_FiI(pV[n1].clr);
      pt3[1].z = pV[n2].scrn.z, pt3[1].x = (int)pV[n2].scrn.x, pt3[1].y = (int)pV[n2].scrn.y, pt3[1].ic = IRGBA_FiI(pV[n2].clr);
      pt3[2].z = pV[n3].scrn.z, pt3[2].x = (int)pV[n3].scrn.x, pt3[2].y = (int)pV[n3].scrn.y, pt3[2].ic = IRGBA_FiI(pV[n3].clr);
      DrawGouraudTriangle(hDC, rDC->hZB, pt3, 0, 1);
    }
  }
  break;

  case G3D_WIRE: {
    //绘制线框模型
    for (i = 0; i < hF->h; i++) {
      //获取边的顶点索引
      for (j = 0; j < 3; ++j) {
        int j1 = (j == 2) ? 0 : (j + 1);
        GetMatElem(n1, hF, j, i, int);
        GetMatElem(n2, hF, j1, i, int);

        if (n1 > n2) {
          continue;
        }

        pt3[0].z = pV[n1].scrn.z, pt3[0].x = (int)pV[n1].scrn.x, pt3[0].y = (int)pV[n1].scrn.y, pt3[0].ic = IRGBA_FiI(pV[n1].clr);
        pt3[1].z = pV[n2].scrn.z, pt3[1].x = (int)pV[n2].scrn.x, pt3[1].y = (int)pV[n2].scrn.y, pt3[1].ic = IRGBA_FiI(pV[n2].clr);
        DrawGouraudWirize(hDC, rDC->hZB, pt3, rDC->dwRop);
      }
    }
  }
  break;

  case G3D_FACE:
  case G3D_TEX: {
    //绘制实体模型, flat 或 Gouraud模型
    HDDC hTex = 0;
    int k = 0;

    if (rDC->tex && hT && hFT && hFT->w >= 3 && hT->h > 0) {
      hTex = hTex1;
      HDDCSET_IMAGE(hTex, rDC->tex);
    }

    for (i = 0; i < hF->h; i++) {
      VECTOR3D tt;
      VECTOR3D n;
      float fCos;
      //获取顶点索引号
      GetMatElem(n1, hF, 0, i, int);
      GetMatElem(n2, hF, 1, i, int);
      GetMatElem(n3, hF, 2, i, int);
      V3SUB(tt, rDC->viewer, pV[n1].coord);
      n = tt;
#if 1
      n = Normal(&pV[n1].coord, &pV[n2].coord, &pV[n3].coord);
      // 背面消除
      n = M3TransformV(m, n);
      fCos = V3DOT(tt, n);
#else
      fCos = V3DOT(tt, pO->sList[i].normal);
#endif

      if (fCos < 0.f) {
        continue;
      }

      pt3[0].z = pV[n1].scrn.z, pt3[0].x = (int)pV[n1].scrn.x, pt3[0].y = (int)pV[n1].scrn.y, pt3[0].ic = IRGBA_FiI(pV[n1].clr);
      pt3[1].z = pV[n2].scrn.z, pt3[1].x = (int)pV[n2].scrn.x, pt3[1].y = (int)pV[n2].scrn.y, pt3[1].ic = IRGBA_FiI(pV[n2].clr);
      pt3[2].z = pV[n3].scrn.z, pt3[2].x = (int)pV[n3].scrn.x, pt3[2].y = (int)pV[n3].scrn.y, pt3[2].ic = IRGBA_FiI(pV[n3].clr);
#define PTINDC(PT, _DC)  (PT.x>=0 && PT.x<_DC->w && PT.y>=0 && PT.y<_DC->h)

      if (!PTINDC(pt3[0], hDC) && !PTINDC(pt3[1], hDC) && !PTINDC(pt3[2], hDC)) {
        continue;
      }

      //获取顶点颜色
      if (hTex && hT) {
        GetMatElem(n1, hFT, 0, i, int);
        GetMatElem(n2, hFT, 1, i, int);
        GetMatElem(n3, hFT, 2, i, int);
        GetMatElem(pt3[0].t.x, hT, 0, n1, int);
        GetMatElem(pt3[0].t.y, hT, 1, n1, int);
        GetMatElem(pt3[1].t.x, hT, 0, n2, int);
        GetMatElem(pt3[1].t.y, hT, 1, n2, int);
        GetMatElem(pt3[2].t.x, hT, 0, n3, int);
        GetMatElem(pt3[2].t.y, hT, 1, n3, int);
      }

      ++k;
      DrawGouraudTriangle(hDC, rDC->hZB, pt3, hTex, rDC->dwRop & G3D_GOURAUD);
    }

    //printf("%d  %d\n", k, nF);
  }
  break;
  }

  return 0;
}
int RenderObject3d(RenderDC* rDC, Object3d* pObj)
{
  int i = 0;
  Vertex3d* pV;
  MatC hV[10], hC[10], hF[10], hT[10], hFT[10];
  SETHMC(hV + i, 4, pObj[i].vListLen, sizeof(pObj[i].vList[0]), T_FLOAT, &(pObj[i].vList[0].coord));
  SETHMC(hC + i, 4, pObj[i].vListLen, sizeof(pObj[i].vList[0]), T_FLOAT, &(pObj[i].vList[0].clr));
  SETHMC(hF + i, 3, pObj[i].sListLen, sizeof(pObj[i].sList[0]), T_INT, (pObj[i].sList[0].vIndex));
  SETHMC(hFT + i, 3, pObj[i].sListLen, sizeof(pObj[i].sList[0]), T_INT, (pObj[i].sList[0].tIndex));
  SETHMC(hT + i, 3, pObj[i].tListLen, sizeof(pObj[i].tList[0]), T_INT, (pObj[i].tList));
  //光照渲染
  pV = BeginRender(hV + i, hC + i, hF + i, rDC->m);
  RenderObj(rDC, (hV + i)->h, pV, hF + i, hT + i, hFT + i);
  free(pV);
  return 0;
}

int Render3DS(RenderDC* rDC, t3DModel* pM)
{
  int i = 0, j;
  MatC hV[10], hF[10], hT[10], hFT[10];
  t3DObject* pObj = pM->pObject;

  for (i = 0; i < pM->numOfObjects; ++i) {
    float* t = MALLOC(float, pObj[i].numTexVertex * 2);
    rDC->tex = pM->pMaterials[pObj[i].materialID].tex;
    SETHMC(hV, 3, pObj[i].numOfVerts, sizeof(pObj[i].pVerts[0]), T_FLOAT, &(pObj[i].pVerts[0]));
    //SETHMC(hC, 4, pObj[i].vListLen, sizeof(pObj[i].vList[0]), T_FLOAT, &(pObj[i].vList[0].clr));
    SETHMC(hF, 3, pObj[i].numOfFaces, sizeof(pObj[i].pFaces[0]), T_USHORT, (pObj[i].pFaces[0].vertIndex));
    SETHMC(hFT, 3, pObj[i].numOfFaces, sizeof(pObj[i].pFaces[0]), T_USHORT, (pObj[i].pFaces[0].vertIndex));
    SETHMC(hT, 2, pObj[i].numTexVertex, sizeof(pObj[i].pTexVerts[0]), T_FLOAT, (pObj[i].pTexVerts));

    if (rDC->tex) {
      for (j = 0; j < pObj[i].numTexVertex; ++j) {
        t[j * 2] = (pObj[i].pTexVerts[j].x) * rDC->tex->width, t[j * 2 + 1] = (1.f - pObj[i].pTexVerts[j].y) * rDC->tex->height;
      }
    }

    SETHMC(hT, 2, pObj[i].numTexVertex, sizeof(float) * 2, T_FLOAT, (t));

    //光照渲染
    if (NULL == pObj[i].pV) {
      pObj[i].pV = BeginRender(hV, 0, hF, rDC->m);
    }

    RenderObj(rDC, (hV)->h, pObj[i].pV, hF, hT, hFT);
    //free(pV);
    free(t);
  }

  return 0;
}
