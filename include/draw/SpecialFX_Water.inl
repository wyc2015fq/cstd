
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "macro.h"
#include "xtypes.h"

typedef struct SpecialFX_Water_t {
  int m_iWidth;
  int m_iHeight;
  int m_iLightModifier;
  int m_iHpage; // The current heightfield
  int m_iDensity; // The water density - can change the density...
  BOOL m_bDrawWithLight;
  int m_iSrcStep;
  // the height fields
  int* m_iHeightField1;
  int* m_iHeightField2;
  int mode;
}
SpecialFX_Water_t;

#define random( min, max ) (( rand() % (int)((( max ) + 1 ) - ( min ))) + ( min ))

int SpecialFX_Water_Clear(SpecialFX_Water_t* sw)
{
  // Cleanup
  SAFEFREE(sw->m_iHeightField1);
  SAFEFREE(sw->m_iHeightField2);
  return 0;
}

int SpecialFX_Water_FlattenWater(SpecialFX_Water_t* sw)
{
  // Clear our height fields
  memset(sw->m_iHeightField1, 0, (sw->m_iWidth * sw->m_iHeight) * sizeof(int));
  memset(sw->m_iHeightField2, 0, (sw->m_iWidth * sw->m_iHeight) * sizeof(int));
  return 0;
}

int SpecialFX_Water_SmoothWater(SpecialFX_Water_t* sw, int npage)
{
  int newh;
  int count = sw->m_iWidth + 1;
  int x, y;

  int* newptr;
  int* oldptr;

  // Set up the pointers
  if (npage == 0) {
    newptr = &sw->m_iHeightField1[ 0 ];
    oldptr = &sw->m_iHeightField2[ 0 ];
  }
  else {
    newptr = &sw->m_iHeightField2[ 0 ];
    oldptr = &sw->m_iHeightField1[ 0 ];
  }

  // Sorry, this function might not be as readable as I'd like, because
  // I optimized it somewhat.  (enough to make me feel satisfied with it)

  for (y = 1; y < sw->m_iHeight - 1; y++) {
    for (x = 1; x < sw->m_iWidth - 1; x++) {
      // This does the eight-pixel method.  It looks much better.

      newh = ((oldptr[ count + sw->m_iWidth ]
          + oldptr[ count - sw->m_iWidth ]
          + oldptr[ count + 1 ]
          + oldptr[ count - 1 ]
          + oldptr[ count - sw->m_iWidth - 1 ]
          + oldptr[ count - sw->m_iWidth + 1 ]
          + oldptr[ count + sw->m_iWidth - 1 ]
          + oldptr[ count + sw->m_iWidth + 1 ]
              ) >> 3)
          + newptr[ count ];


      newptr[ count ] = newh >> 1;
      count++;
    }

    count += 2;
  }

  return 0;
}

int SpecialFX_Water_CalcWaterBigFilter(SpecialFX_Water_t* sw, int npage, int density)
{
  int x, y;
  int newh;
  int count = (2 * sw->m_iWidth) + 2;

  int* newptr;
  int* oldptr;

  // Set up the pointers
  if (npage == 0) {
    newptr = &sw->m_iHeightField1[ 0 ];
    oldptr = &sw->m_iHeightField2[ 0 ];
  }
  else {
    newptr = &sw->m_iHeightField2[ 0 ];
    oldptr = &sw->m_iHeightField1[ 0 ];
  }


  // Sorry, this function might not be as readable as I'd like, because
  // I optimized it somewhat.  (enough to make me feel satisfied with it)

  for (y = 2; y < sw->m_iHeight - 2; y++) {
    for (x = 2; x < sw->m_iWidth - 2; x++) {
      // This does the 25-pixel method.  It looks much okay.

      newh = (
          (
              (
                  (oldptr[ count + sw->m_iWidth ]
                      + oldptr[ count - sw->m_iWidth ]
                      + oldptr[ count + 1 ]
                      + oldptr[ count - 1 ]
                  ) << 1)
              + ((oldptr[ count - sw->m_iWidth - 1 ]
                  + oldptr[ count - sw->m_iWidth + 1 ]
                  + oldptr[ count + sw->m_iWidth - 1 ]
                  + oldptr[ count + sw->m_iWidth + 1 ]))
              + ((
                  oldptr[ count - (sw->m_iWidth * 2) ]
                  + oldptr[ count + (sw->m_iWidth * 2) ]
                  + oldptr[ count - 2 ]
                  + oldptr[ count + 2 ]
                  ) >> 1)
              + ((
                  oldptr[ count - (sw->m_iWidth * 2) - 1 ]
                  + oldptr[ count - (sw->m_iWidth * 2) + 1 ]
                  + oldptr[ count + (sw->m_iWidth * 2) - 1 ]
                  + oldptr[ count + (sw->m_iWidth * 2) + 1 ]
                  + oldptr[ count - 2 - sw->m_iWidth ]
                  + oldptr[ count - 2 + sw->m_iWidth ]
                  + oldptr[ count + 2 - sw->m_iWidth ]
                  + oldptr[ count + 2 + sw->m_iWidth ]
                  ) >> 2)
          )
          >> 3)
          - (newptr[ count ]);


      newptr[ count ] = newh - (newh >> density);
      count++;
    }

    count += 4;
  }

  return 0;
}

int SpecialFX_Water_HeightBlob(SpecialFX_Water_t* sw, int x, int y, int radius, int height, int page)
{
  int rquad;
  int cx, cy, cyq;
  int left, top, right, bottom;

  int* newptr;
  int* oldptr;

  // Set up the pointers
  if (page == 0) {
    newptr = &sw->m_iHeightField1[ 0 ];
    oldptr = &sw->m_iHeightField2[ 0 ];
  }
  else {
    newptr = &sw->m_iHeightField2[ 0 ];
    oldptr = &sw->m_iHeightField1[ 0 ];
  }

  rquad = radius * radius;

  // Make a randomly-placed blob...
  if (x < 0) {
    x = 1 + radius + rand() % (sw->m_iWidth - 2 * radius - 1);
  }

  if (y < 0) {
    y = 1 + radius + rand() % (sw->m_iHeight - 2 * radius - 1);
  }

  left = -radius;
  right = radius;
  top = -radius;
  bottom = radius;

  // Perform edge clipping...
  if (x - radius < 1) {
    left -= (x - radius - 1);
  }

  if (y - radius < 1) {
    top -= (y - radius - 1);
  }

  if (x + radius > sw->m_iWidth - 1) {
    right -= (x + radius - sw->m_iWidth + 1);
  }

  if (y + radius > sw->m_iHeight - 1) {
    bottom -= (y + radius - sw->m_iHeight + 1);
  }


  for (cy = top; cy < bottom; cy++) {
    cyq = cy * cy;

    for (cx = left; cx < right; cx++) {
      if (cx * cx + cyq < rquad) {
        newptr[ sw->m_iWidth * (cy + y) + (cx + x) ] += height;
      }
    }
  }

  return 0;
}

int SpecialFX_Water_HeightBox(SpecialFX_Water_t* sw, int x, int y, int radius, int height, int page)
{
  int cx, cy;
  int left, top, right, bottom;
  int* newptr;
  int* oldptr;

  // Set up the pointers
  if (page == 0) {
    newptr = &sw->m_iHeightField1[ 0 ];
    oldptr = &sw->m_iHeightField2[ 0 ];
  }
  else {
    newptr = &sw->m_iHeightField2[ 0 ];
    oldptr = &sw->m_iHeightField1[ 0 ];
  }

  if (x < 0) {
    x = 1 + radius + rand() % (sw->m_iWidth - 2 * radius - 1);
  }

  if (y < 0) {
    y = 1 + radius + rand() % (sw->m_iHeight - 2 * radius - 1);
  }

  left = -radius;
  right = radius;
  top = -radius;
  bottom = radius;

  // Perform edge clipping...
  if (x - radius < 1) {
    left -= (x - radius - 1);
  }

  if (y - radius < 1) {
    top -= (y - radius - 1);
  }

  if (x + radius > sw->m_iWidth - 1) {
    right -= (x + radius - sw->m_iWidth + 1);
  }

  if (y + radius > sw->m_iHeight - 1) {
    bottom -= (y + radius - sw->m_iHeight + 1);
  }

  for (cy = top; cy < bottom; cy++) {
    for (cx = left; cx < right; cx++) {
      newptr[ sw->m_iWidth * (cy + y) + (cx + x) ] = height;
    }
  }

  return 0;
}


int SpecialFX_Water_WarpBlob(SpecialFX_Water_t* sw, int x, int y, int radius, int height, int page)
{
  int cx, cy;
  int left, top, right, bottom;
  int square;
  int radsquare = radius * radius;
  int* newptr;
  int* oldptr;

  // Set up the pointers
  if (page == 0) {
    newptr = &sw->m_iHeightField1[ 0 ];
    oldptr = &sw->m_iHeightField2[ 0 ];
  }
  else {
    newptr = &sw->m_iHeightField2[ 0 ];
    oldptr = &sw->m_iHeightField1[ 0 ];
  }

  //  radsquare = (radius*radius) << 8;
  radsquare = (radius * radius);

  height /= 64;

  left = -radius;
  right = radius;
  top = -radius;
  bottom = radius;

  // Perform edge clipping...
  if (x - radius < 1) {
    left -= (x - radius - 1);
  }

  if (y - radius < 1) {
    top -= (y - radius - 1);
  }

  if (x + radius > sw->m_iWidth - 1) {
    right -= (x + radius - sw->m_iWidth + 1);
  }

  if (y + radius > sw->m_iHeight - 1) {
    bottom -= (y + radius - sw->m_iHeight + 1);
  }

  for (cy = top; cy < bottom; cy++) {
    for (cx = left; cx < right; cx++) {
      square = cy * cy + cx * cx;

      //      square <<= 8;
      if (square < radsquare) {
        //        Height[page][WATERWID*(cy+y) + cx+x]
        //          += (sqrt(radsquare)-sqrt(square))*height;
        newptr[ sw->m_iWidth * (cy + y) + cx + x ]
        += (int)((radius - sqrt(square)) * (float)(height));
      }
    }
  }

  return 0;
}

int SpecialFX_Water_SineBlob(SpecialFX_Water_t* sw, int x, int y, int radius, int height, int page)
{
  int cx, cy;
  int left, top, right, bottom;
  int square, dist;
  int radsquare = radius * radius;
  float length = (float)((1024.0 / (float) radius) * (1024.0 / (float) radius));
  int* newptr;
  int* oldptr;

  // Set up the pointers
  if (page == 0) {
    newptr = &sw->m_iHeightField1[ 0 ];
    oldptr = &sw->m_iHeightField2[ 0 ];
  }
  else {
    newptr = &sw->m_iHeightField2[ 0 ];
    oldptr = &sw->m_iHeightField1[ 0 ];
  }

  if (x < 0) {
    x = 1 + radius + rand() % (sw->m_iWidth - 2 * radius - 1);
  }

  if (y < 0) {
    y = 1 + radius + rand() % (sw->m_iHeight - 2 * radius - 1);
  }


  //  radsquare = (radius*radius) << 8;
  radsquare = (radius * radius);

  //  height /= 8;

  left = -radius;
  right = radius;
  top = -radius;
  bottom = radius;


  // Perform edge clipping...
  if (x - radius < 1) {
    left -= (x - radius - 1);
  }

  if (y - radius < 1) {
    top -= (y - radius - 1);
  }

  if (x + radius > sw->m_iWidth - 1) {
    right -= (x + radius - sw->m_iWidth + 1);
  }

  if (y + radius > sw->m_iHeight - 1) {
    bottom -= (y + radius - sw->m_iHeight + 1);
  }

  for (cy = top; cy < bottom; cy++) {
    for (cx = left; cx < right; cx++) {
      square = cy * cy + cx * cx;

      if (square < radsquare) {
        dist = (int)(sqrt(square * length));
        newptr[ sw->m_iWidth * (cy + y) + cx + x ]
        += (int)((cos(dist) + 0xffff) * (height)) >> 19;
      }
    }
  }

  return 0;
}

int SpecialFX_Water_Render(SpecialFX_Water_t* sw, const unsigned char* pSrcImage, int SrcStep, unsigned char* pTargetImage, int TargetStep, int cn)
{
  // Yes they have to be the same size...(for now)
  int dx, dy;
  int x, y, k;

  int* ptr = &sw->m_iHeightField1[ 0 ];

  if (sw->m_bDrawWithLight) {
    for (y = 0; y < sw->m_iHeight - 1; ++y) {
      for (x = 0; x < sw->m_iWidth - 1; ++x) {
        int offset = sw->m_iWidth * y + x;
        int tx = ((ptr[ offset ] - ptr[ offset + 1 ]) >> 3);
        dx = x + tx;
        dy = y + ((ptr[ offset ] - ptr[ offset + sw->m_iWidth ]) >> 3);

        if (dx < sw->m_iWidth && dx >= 0 && dy < sw->m_iHeight && dy >= 0) {
          const unsigned char* a = pSrcImage + SrcStep * dy + cn * dx;
          unsigned char* b = pTargetImage + TargetStep * y + cn * x;

          for (k = 0; k < cn; ++k) {
            long R = a[k] - tx;
            b[k] = (unsigned char)((R < 0) ? 0 : (R > 255) ? 255 : R);
          }
        }
      }
    }
  }
  else {
    for (y = 0; y < sw->m_iHeight - 1; ++y) {
      for (x = 0; x < sw->m_iWidth - 1; ++x) {
        int offset = sw->m_iWidth * y + x;
        dx = x + ((ptr[ offset ] - ptr[ offset + 1 ]) >> 3);
        dy = y + ((ptr[ offset ] - ptr[ offset + sw->m_iWidth ]) >> 3);
        memcpy(pTargetImage + TargetStep * y + cn * x, pSrcImage + SrcStep * dy + cn * dx, cn);
      }
    }
  }

  //SpecialFX_Water_CalcWater( sw, sw->m_iHpage, sw->m_iDensity );
  //int SpecialFX_Water_CalcWater( SpecialFX_Water_t* sw, int npage, int density )
  {
    int npage = sw->m_iHpage;
    int density = sw->m_iDensity;
    int newh;
    int count = sw->m_iWidth + 1;
    int* newptr;
    int* oldptr;
    int x, y;

    // Set up the pointers
    if (npage == 0) {
      newptr = &sw->m_iHeightField1[ 0 ];
      oldptr = &sw->m_iHeightField2[ 0 ];
    }
    else {
      newptr = &sw->m_iHeightField2[ 0 ];
      oldptr = &sw->m_iHeightField1[ 0 ];
    }


    // Sorry, this function might not be as readable as I'd like, because
    // I optimized it somewhat.  (enough to make me feel satisfied with it)

    for (y = (sw->m_iHeight - 1) * sw->m_iWidth; count < y; count += 2) {
      for (x = count + sw->m_iWidth - 2; count < x; count++) {

        // This does the eight-pixel method.  It looks much better.

        newh = ((oldptr[ count + sw->m_iWidth ]
            + oldptr[ count - sw->m_iWidth ]
            + oldptr[ count + 1 ]
            + oldptr[ count - 1 ]
            + oldptr[ count - sw->m_iWidth - 1 ]
            + oldptr[ count - sw->m_iWidth + 1 ]
            + oldptr[ count + sw->m_iWidth - 1 ]
            + oldptr[ count + sw->m_iWidth + 1 ]
                ) >> 2)
            - newptr[ count ];


        newptr[ count ] = newh - (newh >> density);
        /*
        // This is the "sludge" method...
        newh = (oldptr[count]<<2)
        +  oldptr[count-1-sw->m_iWidth]
        +  oldptr[count+1-sw->m_iWidth]
        +  oldptr[count-1+sw->m_iWidth]
        +  oldptr[count+1+sw->m_iWidth]
        + ((oldptr[count-1]
        +   oldptr[count+1]
        +   oldptr[count-sw->m_iWidth]
        +   oldptr[count+sw->m_iWidth])<<1);

          newptr[count] = (newh-(newh>>6)) >> density;
        */
      }
    }
  }
  sw->m_iHpage ^= 1;
  return 0;
}

int SpecialFX_Water_Update(SpecialFX_Water_t* sw, unsigned long StepTime_ms)
{
  int x, y;

  if (sw->m_iWidth && sw->m_iHeight) {
    x = rand() % sw->m_iWidth;
    y = rand() % sw->m_iHeight;
    SpecialFX_Water_HeightBlob(sw, x, y, 15, 150, sw->m_iHpage);
    SpecialFX_Water_SineBlob(sw, x, y, 15, 150, sw->m_iHpage);
    SpecialFX_Water_WarpBlob(sw, x, y, 150, 500, sw->m_iHpage);
  }

  return 0;
}

int SpecialFX_Water_Init(SpecialFX_Water_t* sw, int iHeight, int iWidth)
{
  if (iWidth == sw->m_iWidth && iHeight == sw->m_iHeight) {
    return 0;
  }

  sw->m_bDrawWithLight = TRUE;
  sw->m_iLightModifier = 1;
  sw->m_iHpage = 0;
  sw->m_iDensity = 5;

  SAFEFREE(sw->m_iHeightField1);
  SAFEFREE(sw->m_iHeightField2);

  // Create our height fields
  sw->m_iHeightField1 = MALLOC(int, (iWidth * iHeight));
  sw->m_iHeightField2 = MALLOC(int, (iWidth * iHeight));

  // Clear our height fields
  memset(sw->m_iHeightField1, 0, (iWidth * iHeight) * sizeof(int));
  memset(sw->m_iHeightField2, 0, (iWidth * iHeight) * sizeof(int));

  sw->m_iWidth = iWidth;
  sw->m_iHeight = iHeight;

  // Set our page to 0
  sw->m_iHpage = 0;

  sw->m_iSrcStep = iWidth * 3;

  return 0;
}

