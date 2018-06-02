
#include "math3d.h"
typedef struct md3d_t {
  int nv, nf;
  float* v3;
  float* v2;
  int* f;
} md3d_t;

int md3d_setsize(md3d_t* m, int nv, int nf)
{
  m->v3 = (float*)my_realloc2(m->v3, m->nv * 3 * sizeof(float), nv * 3 * sizeof(float));
  m->v2 = (float*)my_realloc2(m->v2, m->nv * 2 * sizeof(float), nv * 2 * sizeof(float));
  m->f = (int*)my_realloc2(m->f, m->nf * sizeof(int), nf * sizeof(int));
  m->nf = nf;
  m->nv = nv;
  return 0;
}
int md3d_free(md3d_t* m)
{
  SAFEFREE(m->f);
  SAFEFREE(m->v3);
  SAFEFREE(m->v2);
  m->nv = 0, m->nf = 0;
  return 0;
}

void m4_scalef(float* s, float x, float y, float z)
{
  M00(s) *= x, M01(s) *= y, M02(s) *= z;
  M10(s) *= x, M11(s) *= y, M12(s) *= z;
  M20(s) *= x, M21(s) *= y, M22(s) *= z;
  M30(s) *= x, M31(s) *= y, M32(s) *= z;
}
void m4_translatef(float* s, float x, float y, float z)
{
  M03(s) = M00(s) * x + M01(s) * y + M02(s) * z + M03(s);
  M13(s) = M10(s) * x + M11(s) * y + M12(s) * z + M13(s);
  M23(s) = M20(s) * x + M21(s) * y + M22(s) * z + M23(s);
  M33(s) = M30(s) * x + M31(s) * y + M32(s) * z + M33(s);
}
void m4_rotatef(float* s, float angle, float x, float y, float z)
{
  float m1[16];
  float si, co, xy, yz, zx, xs, ys, zs, one_c, mag;
  si = (float)sin(RAD(angle));
  co = (float)cos(RAD(angle));
  mag = sqrtf(x * x + y * y + z * z);

  // no rotation, leave mat as-is
  if (mag <= 1.0e-4) {
    return;
  }

  x /= mag, y /= mag, z /= mag;
  xy = x * y;
  yz = y * z;
  zx = z * x;
  xs = x * si;
  ys = y * si;
  zs = z * si;
  one_c = 1.0F - co;
  M4SET(m1, (one_c * x * x) + co, (one_c * xy) - zs, (one_c * zx) + ys, 0.0f,
      (one_c * xy) + zs, (one_c * y * y) + co, (one_c * yz) - xs, 0.0f,
      (one_c * zx) - ys, (one_c * yz) + xs, (one_c * z * z) + co, 0.0F,
      0.0F, 0.0F, 0.0F, 1.0F);
  M4MULM4(s, s, m1);
}

typedef struct en3d_t {
  RECT rc;
  float Ze;
} en3d_t;
#define perspective_md3d(en, m, m0, scale)  perspective_v3(en, (m)->nv, (m)->v3, (m)->v2, m0, scale)
int perspective_v3(en3d_t* en, int n, const float* v3, float* v2, const float* m, float scale)
{
  int i;
  float t, o[4], offx = RCX(&en->rc) / 2.f, offy = RCY(&en->rc) / 2.f;

  //M4SETV(m1, m);
  //m4_scalef(m1, scale, -scale, scale);
  for (i = 0; i < n; ++i, v3 += 3, v2 += 2) {
    M4MULV3(m, v3, o);
    t = -scale * en->Ze / (scale * VZ(o) - en->Ze);
    VX(v2) = VX(o) * t + offx;
    VY(v2) = offy - VY(o) * t;
  }

  return 0;
}


int vcgen_box3d(md3d_t* m, float lx, float ly, float lz, float rx, float ry, float rz)
{
  float* v;
  int* f;
  //float l[2][3]={lx, ly, lz, rx, ry, rz};
  //int i, x, y, z;
  //for (i=0; i<8; ++i) {
  //  V3FSET(v,i,l[!!(i&1)][0],l[!!(i&2)][1],l[!!(i&4)][2]);
  //}
  md3d_setsize(m, 8, 6 * 5);
  v = m->v3;
  f = m->f;
  V3FSET(v, 0, lx, ly, lz); //n,n,n
  V3FSET(v, 1, rx, ly, lz); //p,n,n
  V3FSET(v, 2, rx, ry, lz); //p,p,n
  V3FSET(v, 3, lx, ry, lz); //n,p,n
  V3FSET(v, 4, lx, ly, rz); //n,n,p
  V3FSET(v, 5, rx, ly, rz); //p,n,p
  V3FSET(v, 6, rx, ry, rz); //p,p,p
  V3FSET(v, 7, lx, ry, rz); //n,p,p
  MV4PUSH(f, 0, 1, 2, 3); // 后
  MV4PUSH(f, 7, 6, 5, 4); // 前
  MV4PUSH(f, 0, 3, 7, 4); // 左
  MV4PUSH(f, 3, 2, 6, 7); // 上
  MV4PUSH(f, 2, 1, 5, 6); // 右
  MV4PUSH(f, 1, 0, 4, 5); // 下
  return 3 * 8;
}
int vcgen_axes(md3d_t* m, float cx, float cy, float cz, float rx, float ry, float rz)
{
  float* v;
  int* f;
  md3d_setsize(m, 4, 3 * 3);
  v = m->v3;
  f = m->f;
  V3FSET(v, 0, cx, cy, cz); //c,c,c
  V3FSET(v, 1, rx, cy, cz); //p,c,c
  V3FSET(v, 2, cx, ry, cz); //c,p,c
  V3FSET(v, 3, cx, cy, rz); //c,c,p
  MV2PUSH(f, 0, 1);
  MV2PUSH(f, 0, 2);
  MV2PUSH(f, 0, 3);
  return 4;
}
int vcgen_test(md3d_t* m)
{
  float* v;
  int* f;
  md3d_setsize(m, 7, 6 * 4);
  v = m->v3;
  f = m->f;

  V3FSET(v, 0, 200, 790, 100);
  V3FSET(v, 1, 370, 760, 110);
  V3FSET(v, 2, 60, 670, 115);
  V3FSET(v, 3, 360, 890, 92);
  V3FSET(v, 4, 280, 620, 125);
  V3FSET(v, 5, 30, 880, 95);
  V3FSET(v, 6, 230, 960, 110);
  MV3PUSH(f, 4, 2, 0);
  MV3PUSH(f, 4, 0, 1);
  MV3PUSH(f, 2, 5, 0);
  MV3PUSH(f, 1, 0, 3);
  MV3PUSH(f, 0, 6, 3);
  MV3PUSH(f, 0, 5, 6);
  return 0;
}

int vcgen_testfun(md3d_t* m, int cx, int cy)
{
  float* v;
  int* f;
  int x, y, k;
  float fWaves = 3.0f;
  float xFact = fWaves / (float)cx * 100.0f;
  float yFact = fWaves / (float)cy * 100.0f;
  int m_iCurrAngle = (clock() / 100) % 360;

  md3d_setsize(m, cx * cy, (cx - 1) * (cy - 1) * 5);
  v = m->v3;
  f = m->f;

  k = 0;

  for (y = 0; y < cy; ++y) {
    for (x = 0; x < cx; ++x, ++k) {
      // Data *should be* in range of (-1,-1,-1 / 1,1,1) - looks prettier!
      // Draw a *pretty* curve
      V3FSET(v, k,
          -1.0f + 2.0f * ((float)x / (float)cx),
          -1.0f + 2.0f * ((float)y / (float)cy),
          (float)(cos(RAD(((m_iCurrAngle + y) % 360)*yFact)) + sin(RAD(((m_iCurrAngle + x) % 360)*xFact))) / 2.0f);

      if (x > 0 && y > 0) {
        MV4PUSH(f, k, k - cx, k - cx - 1, k - 1);
      }
    }
  }

  return 0;
}

#define draw_md3d(hDC, m, clr)  draw_v3(hDC, (m)->v2, (m)->nv, (m)->f, (m)->nf, clr)
int draw_v3(HDDC hDC, const float* v2, int nv, int* f, int nf, COLORREF clr)
{
  int i, j, k = 0;
  char buf[32];
  RECT rc;
  COLORREF _clr;
  _clr = _RGB(255, 0, 0);

  for (i = 0, k = 0; i < nf; i += f[0] + 1, f += f[0] + 1, ++k) {
    float c[4] = {0};

    for (j = 0; j < f[0]; ++j) {
      int k = (j + 1) % f[0];
      const float* p0 = v2 + f[j + 1] * 2;
      V2ADD(c, c, p0);
      //if (f[j+1]<f[k+1])
      {
        const float* p1 = v2 + f[j + 1] * 2;
        const float* p2 = v2 + f[k + 1] * 2;
        drawaa_strokeline(hDC, VX(p1), VY(p1), VX(p2), VY(p2), clr, 1);
      }
    }

    rc = iRECT3(iPOINT((int)VX(c) / f[0] - 15, (int)VY(c) / f[0] - 10), iSIZE(30, 20));
    _snprintf(buf, 32, "%d", k);
    //draw_textR2(hDC, buf, rc, DT_CENTER|DT_VCENTER);
  }

  _clr = _RGB(0, 0, 0);

  for (i = 0; i < nv; ++i) {
    const float* p1 = v2 + i * 2;
    rc = iRECT3(iPOINT((int)VX(p1), (int)VY(p1)), iSIZE(30, 20));
    _snprintf(buf, 32, "%d", i);
    //draw_textR2(hDC, buf, rc, DT_CENTER|DT_VCENTER);
  }

  return 0;
}

// Draw the cube given as min/max pair of data-range
int DrawCube(HDDC hDC, RECT rc, const float* m)
{
  md3d_t m1[1] = {0};
  md3d_t m2[1] = {0};
  md3d_t m3[1] = {0};
  en3d_t en[1] = {0};

  COLORREF cPen = (_RGB(255, 255, 255));
  COLORREF cPenR = (_RGB(255, 64, 64));
  COLORREF cPenG = (_RGB(64, 255, 64));
  COLORREF cPenB = (_RGB(64, 64, 255));
  COLORREF _clr = cPen;
  en->rc = rc;
  en->Ze = -1000.f;
  vcgen_box3d(m1, -1, -1, -1, 1, 1, 1);
  vcgen_axes(m2, 0, 0, 0, 1, 1, 1);
  //vcgen_test(m3);
  vcgen_testfun(m3, 20, 20);

  perspective_md3d(en, m1, m, 200.f);
  perspective_md3d(en, m2, m, 200.f);
  perspective_md3d(en, m3, m, 200.f);

  draw_md3d(hDC, m1, _RGBA(0, 0, 0, 150));
  draw_md3d(hDC, m3, _RGBA(0, 0, 0, 150));
  //draw_md3d(hDC, m2, _RGB(0,0,0));

  md3d_free(m1);
  md3d_free(m2);
  md3d_free(m3);
  return 0;
}
