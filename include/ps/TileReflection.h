
typedef struct POINT_FLOAT {
  double x;
  double y;
} POINT_FLOAT;

//=============================================================================
/**
 *  Tile reflection effect (>=24 bit).
@verbatim
    example:
        FCPixelTileReflection   aCmd (45, 20, 8);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
int FCPixelTileReflection(int aw, int ah, BYTE* a, int al, int ai, int nAngle, int nSquareSize, int nCurvature)
{
  int x, y, i;
  enum { aasamples = 17, };

  double   m_sin, m_cos;
  double   m_scale;
  double   m_curvature;
  POINT_FLOAT   m_aapt[aasamples];
  BYTE* ba = (BYTE*)memdup(a, ah * al);

  nAngle = FClamp(nAngle, -45, 45);
  m_sin = sin(AngleToRadian(nAngle));
  m_cos = cos(AngleToRadian(nAngle));

  nSquareSize = FClamp(nSquareSize, 2, 200);
  m_scale = LIB_PI / (double)nSquareSize;

  nCurvature = FClamp(nCurvature, -20, 20);

  if (nCurvature == 0) {
    nCurvature = 1;
  }

  m_curvature = nCurvature * nCurvature / 10.0 * (abs(nCurvature) / nCurvature);

  for (i = 0; i < aasamples; i++) {
    double x = (i * 4) / (double)aasamples, y = i / (double)aasamples;
    x = x - (int)x;
    m_aapt[i].x = m_cos * x + m_sin * y;
    m_aapt[i].y = m_cos * y - m_sin * x;
  }

  for (y = 0; y < ah; y++) {
    for (x = 0; x < aw; x++) {
      BYTE* pPixel = a + y * al + x * ai;
      double   hw = aw / 2.0, hh = ah / 2.0, i = x - hw, j = y - hh;
      int  k, mm, sum[4] = {0};

      for (mm = 0; mm < aasamples; mm++) {
        BYTE*    p;
        int   xSample, ySample;
        double   u = i + m_aapt[mm].x;
        double   v = j - m_aapt[mm].y;

        double   s =  m_cos * u + m_sin * v;
        double   t = -m_sin * u + m_cos * v;

        s += m_curvature * tan(s * m_scale);
        t += m_curvature * tan(t * m_scale);
        u = m_cos * s - m_sin * t;
        v = m_sin * s + m_cos * t;

        xSample = (int)(hw + u);
        ySample = (int)(hh + v);

        xSample = FClamp(xSample, 0, aw - 1);
        ySample = FClamp(ySample, 0, ah - 1);

        p = ba + (xSample) * ai + (ySample) * al;

        for (k = 0; k < ai; ++k) {
          sum[k] += p[k];
        }
      }

      for (k = 0; k < ai; ++k) {
        sum[k] /= aasamples;
        pPixel[k] = FClamp0255(sum[k]);
      }
    }
  }

  free(ba);
  return 0;
}

