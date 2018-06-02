#include <math.h>
typedef enum GradientType {
  GRADIENT_HORIZONTAL, GRADIENT_VERTICAL, GRADIENT_RADIAL, GRADIENT_DIAGONAL, GRADIENT_FDIAGONAL, GRADIENT_BDIAGONAL
} GradientType;
typedef enum TransformationType {
  TRANSFORMATION_NONE, TRANSFORMATION_CHARICATURE, TRANSFORMATION_FISHEYE, TRANSFORMATION_SWIRLED, TRANSFORMATION_CYLINDER, TRANSFORMATION_SHIFT
} TransformationType;
void ApplyTransformation(unsigned char* m_Data, int bw, int width, int height, TransformationType transformationType)
{
  int X, Y, i, j;
  double r, fi;
  double R = sqrt(pow(width / 4, 2) + pow(height / 4, 2));
  double FI = atan((double)(height) / (double)(width));
  int size = height * width * 4;
  unsigned char* newBuffer = MALLOC(unsigned char, size);

  //memcpy(newBuffer, m_Data, size);
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      switch (transformationType) {
      case TRANSFORMATION_CHARICATURE: {
        fi = atan((double)(i - height / 2) / (double)(j - width / 2));
        r = sqrt(pow((i - height / 2), 2) + pow((j - width / 2), 2));
        X = (int)(sqrt(r * R * 2) * cos(fi));
        Y = (int)(fabs(sqrt(r * R * 2) * sin(fi)));
        newBuffer[i * width * 4 + j * 4 + 0] = m_Data[Y * bw + X * 4 + 0];
        newBuffer[i * width * 4 + j * 4 + 1] = m_Data[Y * bw + X * 4 + 1];
        newBuffer[i * width * 4 + j * 4 + 2] = m_Data[Y * bw + X * 4 + 2];
      }
      break;

      case TRANSFORMATION_FISHEYE: {
        fi = atan((double)(i - height / 2) / (double)(j - width / 2));
        r = sqrt(pow((i - height / 2), 2) + pow((j - width / 2), 2));
        X = (int)((pow(r, 2) / R) * cos(fi));
        Y = (int)(fabs((pow(r, 2) / R) * sin(fi))) % height;
        newBuffer[i * width * 4 + j * 4 + 0] = m_Data[Y * bw + X * 4 + 0];
        newBuffer[i * width * 4 + j * 4 + 1] = m_Data[Y * bw + X * 4 + 1];
        newBuffer[i * width * 4 + j * 4 + 2] = m_Data[Y * bw + X * 4 + 2];
      }
      break;

      case TRANSFORMATION_SWIRLED: {
        fi = atan((double)(i - height / 2) / (double)(j - width / 2));
        r = sqrt(pow((i - height / 2), 2) + pow((j - width / 2), 2));
        X = (int)(fabs(r * cos(fi + r / 8)));
        Y = (int)(fabs(r * sin(fi + r / 8)));
        newBuffer[i * width * 4 + j * 4 + 0] = m_Data[Y * bw + X * 4 + 0];
        newBuffer[i * width * 4 + j * 4 + 1] = m_Data[Y * bw + X * 4 + 1];
        newBuffer[i * width * 4 + j * 4 + 2] = m_Data[Y * bw + X * 4 + 2];
      }
      break;

      case TRANSFORMATION_CYLINDER: {
        fi = atan((double)(i - height / 2) / (double)(j - width / 2));
        r = sqrt(pow((i - height / 2), 2) + pow((j - width / 2), 2));
        X = abs((j - width / 2) + (int)(fi + r / 10) % 32 - 8);
        Y = i;
        newBuffer[i * width * 4 + j * 4 + 0] = m_Data[Y * bw + X * 4 + 0];
        newBuffer[i * width * 4 + j * 4 + 1] = m_Data[Y * bw + X * 4 + 1];
        newBuffer[i * width * 4 + j * 4 + 2] = m_Data[Y * bw + X * 4 + 2];
      }
      break;

      case TRANSFORMATION_SHIFT: {
        fi = atan((double)(i - height / 2) / (double)(j - width / 2));
        r = sqrt(pow((i - height / 2), 2) + pow((j - width / 2), 2));
        X = (int)(fabs(r * cos(fi)));
        Y = (int)(fabs(r * sin(fi)));
        newBuffer[i * width * 4 + j * 4 + 0] = m_Data[Y * bw + X * 4 + 0];
        newBuffer[i * width * 4 + j * 4 + 1] = m_Data[Y * bw + X * 4 + 1];
        newBuffer[i * width * 4 + j * 4 + 2] = m_Data[Y * bw + X * 4 + 2];
      }
      break;
      }
    }
  }

  memcpy2d(m_Data, bw, newBuffer, width * 4, height, width * 4);
  free(newBuffer);
}
//GradientType m_Type;
RECT cRect(int l, int t, int r, int b)
{
  RECT rc = iRECT(l, t, r, b);
  return rc;
}
RECT cRectClip(HDDC hDC, const RECT* lpRect)
{
  RECT rc = {
    0, 0, hDC->w, hDC->h
  };

  if (lpRect) {
    rc.left = BOUND(lpRect->left, 0, hDC->w);
    rc.right = BOUND(lpRect->right, 0, hDC->w);
    rc.top = BOUND(lpRect->top, 0, hDC->h);
    rc.bottom = BOUND(lpRect->bottom, 0, hDC->h);
  }

  return rc;
}
void DrawGradient_1(HDDC hDC, const RECT* lpRect, COLORREF startColor, COLORREF endColor, GradientType gradientType, TransformationType transformationType)
{
  int i, j, y, x1, y1, bw = hDC->bw;
  double r, d, n1;
  RECT rect = cRectClip(hDC, lpRect);
  // Calculate image size
  int width = rect.right - rect.left;
  int height = rect.bottom - rect.top;
  int size = height * width * 4;
  double radius = sqrt(pow(height / 2, 2) + pow(width / 2, 2));
  double k = (double)(rect.bottom - rect.top) / (double)(rect.right - rect.left);
  double n = 0.0;
  double k1 = - 1 / k;
  unsigned char* m_Data = (unsigned char*)(&_Pixel(hDC, rect.left, rect.top));

  if (gradientType == GRADIENT_BDIAGONAL) {
    k = (double)(rect.bottom - rect.top) / (double)(rect.left - rect.right);
  }

  if ((gradientType == GRADIENT_FDIAGONAL) || (gradientType == GRADIENT_BDIAGONAL)) {
    gradientType = GRADIENT_DIAGONAL;
  }

  // Create gradient
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      switch (gradientType) {
        // Horizontal gradient
      case GRADIENT_HORIZONTAL: {
        m_Data[i * bw + j * 4 + 0] = (unsigned char)(j * GetBV(endColor) / width + (width - j) * GetBV(startColor) / width);
        m_Data[i * bw + j * 4 + 1] = (unsigned char)(j * GetGV(endColor) / width + (width - j) * GetGV(startColor) / width);
        m_Data[i * bw + j * 4 + 2] = (unsigned char)(j * GetRV(endColor) / width + (width - j) * GetRV(startColor) / width);
      }
      break;

      // Vertical gradient
      case GRADIENT_VERTICAL: {
        m_Data[i * bw + j * 4 + 0] = (unsigned char)(i * GetBV(endColor) / height + (height - i) * GetBV(startColor) / height);
        m_Data[i * bw + j * 4 + 1] = (unsigned char)(i * GetGV(endColor) / height + (height - i) * GetGV(startColor) / height);
        m_Data[i * bw + j * 4 + 2] = (unsigned char)(i * GetRV(endColor) / height + (height - i) * GetRV(startColor) / height);
      }
      break;

      // Radial gradient
      case GRADIENT_RADIAL: {
        r = sqrt(pow((double)(i - height / 2) / radius, 2) + pow((double)(j - width / 2) / radius, 2));

        if (r <= 1.0) {
          m_Data[i * bw + j * 4 + 0] = (unsigned char)(r * GetBV(endColor) + (1 - r) * GetBV(startColor));
          m_Data[i * bw + j * 4 + 1] = (unsigned char)(r * GetGV(endColor) + (1 - r) * GetGV(startColor));
          m_Data[i * bw + j * 4 + 2] = (unsigned char)(r * GetRV(endColor) + (1 - r) * GetRV(startColor));
        }
        else {
          m_Data[i * bw + j * 4 + 0] = GetBV(endColor);
          m_Data[i * bw + j * 4 + 1] = GetGV(endColor);
          m_Data[i * bw + j * 4 + 2] = GetRV(endColor);
        }
      }
      break;

      // Diagonal gradient
      case GRADIENT_DIAGONAL: {
        y = (int)(k * (j - width / 2) + n);
        n1 = (int)((i - height / 2) - k1 * (j - width / 2));
        x1 = (int)((n1 - n) / (k - k1));
        y1 = (int)(k1 * x1 + n1);
        d = sqrt(pow((j - width / 2) - x1, 2) + pow((i - height / 2) - y1, 2)) / (2 * radius);

        if ((i - height / 2) > y) {
          m_Data[i * bw + j * 4 + 0] = (unsigned char)((0.5 - d) * GetBV(endColor) + (0.5 + d) * GetBV(startColor));
          m_Data[i * bw + j * 4 + 1] = (unsigned char)((0.5 - d) * GetGV(endColor) + (0.5 + d) * GetGV(startColor));
          m_Data[i * bw + j * 4 + 2] = (unsigned char)((0.5 - d) * GetRV(endColor) + (0.5 + d) * GetRV(startColor));
        }
        else {
          m_Data[i * bw + j * 4 + 0] = (unsigned char)((0.5 - d) * GetBV(startColor) + (0.5 + d) * GetBV(endColor));
          m_Data[i * bw + j * 4 + 1] = (unsigned char)((0.5 - d) * GetGV(startColor) + (0.5 + d) * GetGV(endColor));
          m_Data[i * bw + j * 4 + 2] = (unsigned char)((0.5 - d) * GetRV(startColor) + (0.5 + d) * GetRV(endColor));
        }
      }
      break;
      }
    }
  }

  if (transformationType != TRANSFORMATION_NONE) {
    ApplyTransformation(m_Data, bw, width, height, transformationType);
  }
}
int test_DrawGradient(HDDC hDC)
{
  COLORREF crTextColor = _RGB(255, 255, 255);
#define TESTDRAWGRADIENT(l,t,r,b, T1, T2) {RECT RC={l,t,r,b}; DrawGradient_1( hDC, &RC, _RGB(0,0,255), _RGB(255,0,0), T1, T2 ); }
  //if (1) TTextOut(hDC, RC.left, RC.top, #T1 "\n" #T2 , crTextColor );
  {
    utime_start(_start_time);
    TESTDRAWGRADIENT(0, 0, 200, 200, GRADIENT_HORIZONTAL, TRANSFORMATION_NONE);
    TESTDRAWGRADIENT(220, 0, 420, 200, GRADIENT_VERTICAL, TRANSFORMATION_NONE);
    TESTDRAWGRADIENT(440, 0, 640, 200, GRADIENT_RADIAL, TRANSFORMATION_NONE);
    TESTDRAWGRADIENT(660, 0, 860, 200, GRADIENT_FDIAGONAL, TRANSFORMATION_NONE);
    TESTDRAWGRADIENT(0, 220, 200, 420, GRADIENT_RADIAL, TRANSFORMATION_CHARICATURE);
    TESTDRAWGRADIENT(220, 220, 420, 420, GRADIENT_VERTICAL, TRANSFORMATION_FISHEYE);
    TESTDRAWGRADIENT(440, 220, 640, 420, GRADIENT_HORIZONTAL, TRANSFORMATION_SWIRLED);
    TESTDRAWGRADIENT(660, 220, 860, 420, GRADIENT_FDIAGONAL, TRANSFORMATION_CYLINDER);
    printf("%f\n", utime_elapsed(_start_time));
  }
  return 0;
}
#undef TESTDRAWGRADIENT

