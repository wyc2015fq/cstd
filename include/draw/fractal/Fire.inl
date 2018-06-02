
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "macro.h"
#include "xtypes.h"

typedef struct CSceneFire {
  DEFINE_SCENE_FRACTAL_BASE(scene_fractal_base);
  int m_iFlameHeight;
  int m_iWidth;
  int m_iHeight;
  int m_iFireSource; //The y position for the lit spots
  int m_iFireChance;
  int m_iAvgFlameWidth;
  int m_iAlpha;
  COLORREF m_FireColors[ 4 ];
  COLORREF m_pPalletteBuffer[ 256 ];
  BYTE* m_pFireBits;
  long* m_pYIndexes;
}
CSceneFire;


#define random( min, max ) (( rand() % (int)((( max ) + 1 ) - ( min ))) + ( min ))

void CSceneFireClrHotSpots(CSceneFire* sw)
{
  // clear the fire line
  memset(&sw->m_pFireBits[ sw->m_pYIndexes[ sw->m_iFireSource ] ], 0, sw->m_iWidth);
}

void CSceneFireInitPallette(CSceneFire* sw)
{
  // Create a gradient between all the colors we have for our fire...
  int iColor;
  long iCount = 0;
  COLORREF clrStart;
  COLORREF clrEnd;
  int r, g, b;              // First distance, then starting value
  float rStep, gStep, bStep;    // Step size for each color
  int nSteps, iOnBand;
  float fStep;

  for (iColor = 1; iColor < 4; iColor++) {

    clrStart = sw->m_FireColors[ iColor - 1 ];
    clrEnd = sw->m_FireColors[ iColor ];

    // Get the color differences
    r = (GetRV(clrEnd) - GetRV(clrStart));
    g = (GetGV(clrEnd) - GetGV(clrStart));
    b = (GetBV(clrEnd) - GetBV(clrStart));

    nSteps = MAX(abs(r), MAX(abs(g), abs(b)));
    fStep = (float)(255 / 3) / (float) nSteps;
    // Calculate the step size for each color
    rStep = r / (float) nSteps;
    gStep = g / (float) nSteps;
    bStep = b / (float) nSteps;

    // Reset the colors to the starting position
    r = GetRV(clrStart);
    g = GetGV(clrStart);
    b = GetBV(clrStart);

    for (iOnBand = 0; iOnBand < nSteps; iOnBand++) {
      //COLORREF color = CC_RGB(r+rStep*iOnBand, g + gStep*iOnBand, b + bStep *iOnBand);
      COLORREF color = _RGB(b + bStep * iOnBand, g + gStep * iOnBand, r + rStep * iOnBand);

      long lIndex = (int)(iOnBand * fStep);

      if (lIndex + ((iColor - 1) * 85) < 255) {
        sw->m_pPalletteBuffer[ lIndex + ((iColor - 1) * 85) ] = color;
      }
    }
  }

  // Step on the second color a little bit...
  clrStart = sw->m_FireColors[ 0 ];
  clrEnd = sw->m_FireColors[ 1 ];

  MEMSET(sw->m_pPalletteBuffer, 0, sw->m_iFlameHeight);

  // Get the color differences
  r = (GetRV(clrEnd) - GetRV(clrStart));
  g = (GetGV(clrEnd) - GetGV(clrStart));
  b = (GetBV(clrEnd) - GetBV(clrStart));

  nSteps = MAX(abs(r), MAX(abs(g), abs(b)));
  fStep = (float)(85 - sw->m_iFlameHeight) / (float) nSteps;
  // Calculate the step size for each color
  rStep = r / (float) nSteps;
  gStep = g / (float) nSteps;
  bStep = b / (float) nSteps;

  // Reset the colors to the starting position
  r = GetRV(clrStart);
  g = GetGV(clrStart);
  b = GetBV(clrStart);

  for (iOnBand = 0; iOnBand < nSteps; iOnBand++) {
    //COLORREF color = CC_RGB(r+rStep*iOnBand, g + gStep*iOnBand, b + bStep *iOnBand);
    COLORREF color = _RGB(b + bStep * iOnBand, g + gStep * iOnBand, r + rStep * iOnBand);

    long lIndex = (int)(iOnBand * fStep);

    sw->m_pPalletteBuffer[ lIndex + (85 - sw->m_iFlameHeight) ] = color;
  }

}

// Macro to get a random integer within a specified range */
#define getrandom( min, max ) (( rand() % (int)((( max ) + 1 ) - ( min ))) + ( min ))

void CSceneFireSetHotSpots(CSceneFire* sw)
{
  int i;
  long lPosition = 0;
  CSceneFireClrHotSpots(sw);
  //sw->m_iAvgFlameWidth

  while (lPosition < sw->m_iWidth) {
    // see if we should do a flame
    if (getrandom(0, 100) < sw->m_iFireChance) {
      // get a flame width
      long lFlameWidth = getrandom(1, sw->m_iAvgFlameWidth);

      for (i = 0; i < lFlameWidth; i++) {
        if (lPosition < sw->m_iWidth) {
          sw->m_pFireBits[ sw->m_pYIndexes[ sw->m_iFireSource ] + lPosition ] = 254; // set a hot spot!
          lPosition++;
        }
      }
    }

    lPosition++;
  }

  //  for (x = 0; x < sw->m_iWidth; x++)
  //  {
  //    if (getrandom(0,100) < sw->m_iFireChance)
  //    {

  //    }
  //  }
}

unsigned char CSceneFireAverage(CSceneFire* sw, int x, int y)
{

  unsigned char ave_color;
  unsigned char ave1, ave2, ave3, ave4, ave5, ave6, ave7;

  // Make sure we are not at the last line...
  if (y == sw->m_iHeight) {
    ave1 = sw->m_pFireBits[ sw->m_pYIndexes[ y - 1 ] + x ];
  }
  else {
    ave1 = sw->m_pFireBits[ sw->m_pYIndexes[ y + 1 ] + x ];
  }

  ave2 = sw->m_pFireBits[ sw->m_pYIndexes[ y - 1 ] + x ];
  ave3 = sw->m_pFireBits[ sw->m_pYIndexes[ y ] + x + 1 ];
  ave4 = sw->m_pFireBits[ sw->m_pYIndexes[ y ] + x - 1 ];
  ave5 = sw->m_pFireBits[ sw->m_pYIndexes[ y ] + x + 2 ];
  ave6 = sw->m_pFireBits[ sw->m_pYIndexes[ y ] + x - 2 ];
  ave7 = sw->m_pFireBits[ sw->m_pYIndexes[ y ] + x ];

  ave_color = (ave1 + ave2 + ave3 + ave4 + ave5 + ave6 + ave7) / 7;

  return (ave_color);
}
void CSceneFireMakeLines(CSceneFire* sw)
{
  int x, y;

  for (x = 0; x < sw->m_iWidth; x++) {
    for (y = sw->m_iFireSource; y < sw->m_iHeight - 1; y++)
      //       for (y = sw->m_iHeight; y > sw->m_iFireSource; y--)
    {
      //sw->m_pFireBits[sw->m_pYIndexes[y-1]+x] =Average(x,y);
      sw->m_pFireBits[ sw->m_pYIndexes[ y + 1 ] + x ] = CSceneFireAverage(sw, x, y);
    }
  }
}

void CSceneFireCreate(CSceneFire* sw, int iHeight, int iWidth)
{
  int y;

  if (iWidth == sw->m_iWidth && iHeight == (sw->m_iHeight - 3)) {
    return ;
  }

  // Add three to the height
  sw->m_iWidth = iWidth;
  sw->m_iHeight = iHeight;
  sw->m_iHeight += 3;
  sw->m_pYIndexes = MALLOC(long, sw->m_iHeight + 222);
  sw->m_pFireBits = MALLOC(BYTE, sw->m_iWidth * sw->m_iHeight);

  // Clear the Fire bits
  memset(sw->m_pFireBits, 0, (sw->m_iWidth * sw->m_iHeight));

  // do all the y index pre-calc..
  for (y = sw->m_iHeight; y > 0; y--) {
    sw->m_pYIndexes[ y ] = y * sw->m_iWidth;
  }

  // Create our pallete

  CSceneFireInitPallette(sw);
  CSceneFireClrHotSpots(sw);
}

void CSceneFireDoDraw(CSceneFire* sw, int iheight, int iwidth, unsigned char* pVideoMemory, int dst_step, int cn)
{
  int i, x;
  // Right now Im just going to blit it right onto the video memory
  unsigned char* pSrcBitlin; // = sw->m_pFireBits+(sw->m_iWidth*3);// get rid of our fire source
  BYTE* dst; //=(BYTE*)Dib->pVideoMemory;
  BYTE r, g, b;

  CSceneFireCreate(sw, iheight, iwidth);
  CSceneFireSetHotSpots(sw);     // generate random hotspots
  CSceneFireMakeLines(sw);    // do all the math and screen updating

  for (i = 0; i < sw->m_iHeight - 3; i++) {
    dst = (BYTE*) & pVideoMemory[(dst_step * (sw->m_iHeight - 4 - i)) ];
    pSrcBitlin = &sw->m_pFireBits[ sw->m_pYIndexes[ i + 3 ] ];

    for (x = 0; x < sw->m_iWidth; x++) {
      r = GetRV(sw->m_pPalletteBuffer[ pSrcBitlin[ x ] ]);
      g = GetGV(sw->m_pPalletteBuffer[ pSrcBitlin[ x ] ]);
      b = GetBV(sw->m_pPalletteBuffer[ pSrcBitlin[ x ] ]);

      dst[ 0 ] = (BYTE)(((r - dst[ 0 ]) * sw->m_iAlpha + (dst[ 0 ] << 8)) >> 8);
      dst[ 1 ] = (BYTE)(((g - dst[ 1 ]) * sw->m_iAlpha + (dst[ 1 ] << 8)) >> 8);
      dst[ 2 ] = (BYTE)(((b - dst[ 2 ]) * sw->m_iAlpha + (dst[ 2 ] << 8)) >> 8);
      dst += cn;
    }
  }
}

void CSceneFireClear(CSceneFire* sw)
{
  // Cleanup
  SAFEFREE(sw->m_pFireBits);
  SAFEFREE(sw->m_pYIndexes);
  SAFEFREE(sw);
}

void CSceneFireUpdate(CSceneFire* sw, unsigned long StepTime_ms)
{
  sw->m_iAlpha = StepTime_ms;
}

CSceneFire* CSceneFireInti()
{
  CSceneFire* sw = MALLOC(CSceneFire, 1);
  MEMSET(sw, 0, 1);
  sw->m_iWidth = 0;
  sw->m_iHeight = 0;
  sw->m_pFireBits = NULL;
  sw->m_pYIndexes = NULL;
  sw->m_iFireSource = 2;
  sw->m_iFireChance = 10;
  sw->m_iFlameHeight = 50;
  sw->m_iAlpha = 30;

  sw->m_FireColors[ 0 ] = _RGB(0, 0, 0);   // Black
  sw->m_FireColors[ 1 ] = _RGB(255, 0, 0);   // Red
  sw->m_FireColors[ 2 ] = _RGB(255, 255, 0);   // Yellow
  sw->m_FireColors[ 3 ] = _RGB(255, 255, 255);   // White
  sw->m_iAvgFlameWidth = 35;
  // Get Rid of anything already there

  SCENE_FRACTAL_SET_CLASS(sw, CSceneFire);
  return sw;
}

