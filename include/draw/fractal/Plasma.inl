
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "macro.h"
#include "xtypes.h"

typedef struct CScenePlasma {
  DEFINE_SCENE_FRACTAL_BASE(scene_fractal_base);
  int m_iWidth;
  int m_iHeight;
  int m_iAlpha;
  int m_icostbl[ 256 ];

  int m_iModifier1;
  int m_iModifier2;
  int m_iModifier3;
  int m_iModifier4;

  int m_iXModifier1;
  int m_iXModifier2;
  int m_iYModifier1;
  int m_iYModifier2;

  unsigned char m_a1, m_a2, m_a3, m_a4;
  unsigned char m_b1, m_b2, m_b3, m_b4;

  BYTE* m_pPlasmaBits;
  COLORREF m_pPalletteBuffer[ 256 ];
  COLORREF m_PlasmaColors[ 16 ]; // Yep 16 colors needed to generate our pallete...

}
CScenePlasma;




void CScenePlasmaInitCostBLTable(CScenePlasma* sw)
{
  int t;

  for (t = 0; t < 256; t++) {
    sw->m_icostbl[ t ] = (int)(30 * cos(t * (3.14159 / 64)));
  }
}
void CScenePlasmaSetRGB(CScenePlasma* sw, int iIndex, int R, int G, int B)
{
  COLORREF color = _RGB(R + 50, G + 50, B + 50);
  sw->m_pPalletteBuffer[ iIndex ] = color;

}

int
MulDiv1(
    int nNumber,
    int nNumerator,
    int nDenominator
)
{
  __int64 x = nNumber;
  x *= nNumerator;
  x /= nDenominator;
  return (int) x;
}

void CScenePlasmaCreateGradient(COLORREF clrStart, COLORREF clrEnd, long lSteps, COLORREF* pBuffer)
{
  // I created this routine to make real smooth gradients...
  // It may not be real optimized, but it works....and that is what matters to me right now...

  int r, g, b;              // First distance, then starting value
  int rTotal, gTotal, bTotal;
  int roffset, goffset, boffset;
  int scalerR;
  int scalerG;
  int scalerB;
  int i;


  roffset = goffset = boffset = 0;
  scalerR = scalerG = scalerB = 0;

  // Get the color differences and scalers
  rTotal = GetRV(clrEnd) - GetRV(clrStart);

  if (rTotal < 0) {
    scalerR = -1;
  }
  else if (rTotal > 0) {
    scalerR = 1;
  }

  gTotal = GetGV(clrEnd) - GetGV(clrStart);

  if (gTotal < 0) {
    scalerG = -1;
  }
  else if (gTotal > 0) {
    scalerG = 1;
  }

  bTotal = GetBV(clrEnd) - GetBV(clrStart);

  if (bTotal < 0) {
    scalerB = -1;
  }
  else if (bTotal > 0) {
    scalerB = 1;
  }

  // reset to positives
  rTotal = abs(rTotal);
  gTotal = abs(gTotal);
  bTotal = abs(bTotal);

  // Get the starting color values...
  r = GetRV(clrStart);
  g = GetGV(clrStart);
  b = GetBV(clrStart);

  for (i = 0; i < lSteps; i++) {
    roffset = MulDiv1(i, rTotal, lSteps);
    goffset = MulDiv1(i, gTotal, lSteps);
    boffset = MulDiv1(i, bTotal, lSteps);

    roffset *= scalerR;
    goffset *= scalerG;
    boffset *= scalerB;

    pBuffer[ i ] = _RGB((b + boffset), (g + goffset), (r + roffset));

  }

}

void CScenePlasmaInitPallette(CScenePlasma* sw)
{
  // Create a gradient between all the colors we have for our plasma
  CScenePlasmaCreateGradient(sw->m_PlasmaColors[ 0 ], sw->m_PlasmaColors[ 1 ], 32, &sw->m_pPalletteBuffer[ 0 ]);   // From black to Blue
  CScenePlasmaCreateGradient(sw->m_PlasmaColors[ 2 ], sw->m_PlasmaColors[ 3 ], 32, &sw->m_pPalletteBuffer[ 32 ]);   // From Green to Blue
  CScenePlasmaCreateGradient(sw->m_PlasmaColors[ 4 ], sw->m_PlasmaColors[ 5 ], 32, &sw->m_pPalletteBuffer[ 64 ]);   // From Green to White
  CScenePlasmaCreateGradient(sw->m_PlasmaColors[ 6 ], sw->m_PlasmaColors[ 6 ], 32, &sw->m_pPalletteBuffer[ 96 ]);   // From Cyan to Cyan
  CScenePlasmaCreateGradient(sw->m_PlasmaColors[ 8 ], sw->m_PlasmaColors[ 9 ], 32, &sw->m_pPalletteBuffer[ 128 ]);   // Cyan To Cyan
  CScenePlasmaCreateGradient(sw->m_PlasmaColors[ 10 ], sw->m_PlasmaColors[ 11 ], 32, &sw->m_pPalletteBuffer[ 160 ]);   // While to dark Green
  CScenePlasmaCreateGradient(sw->m_PlasmaColors[ 12 ], sw->m_PlasmaColors[ 13 ], 32, &sw->m_pPalletteBuffer[ 192 ]);   // Dark Blue to dark green
  CScenePlasmaCreateGradient(sw->m_PlasmaColors[ 14 ], sw->m_PlasmaColors[ 15 ], 32, &sw->m_pPalletteBuffer[ 224 ]);   // Dark Blue to black

}
void CScenePlasmaCalcPlasma(CScenePlasma* sw)
{
  int x, y;
  unsigned char* tscr;
  // Initialize with outer variables
  sw->m_a1 = sw->m_b1;
  sw->m_a2 = sw->m_b2;
  tscr = (unsigned char*) & sw->m_pPlasmaBits[ 0 ];

  for (y = 0; y < sw->m_iHeight; y++) {
    // Initialize with outer variables

    sw->m_a3 = sw->m_b3;
    sw->m_a4 = sw->m_b4;

    for (x = 0; x < sw->m_iWidth; x++) {
      *tscr++ = sw->m_icostbl[ sw->m_a1 ] +
          sw->m_icostbl[ sw->m_a2 ] +
          sw->m_icostbl[ sw->m_a3 ] +
          sw->m_icostbl[ sw->m_a4 ] ;

      // Higher values result in many slower plasmas

      sw->m_a3 += sw->m_iModifier1; //4;
      sw->m_a4 += sw->m_iModifier2; //1;
    }

    // Same as the previous comment

    sw->m_a1 += sw->m_iModifier3; //1;
    sw->m_a2 += sw->m_iModifier4; //4;

  }

  sw->m_b1 += sw->m_iYModifier1; //y modifier 1
  sw->m_b2 += sw->m_iYModifier2; //y modifier 2
  sw->m_b3 += sw->m_iXModifier1; //x modifier 1
  sw->m_b4 += sw->m_iXModifier2; //x modifier 2

}
void CScenePlasmaCreate(CScenePlasma* sw, int iHeight, int iWidth)
{
  if (iWidth == sw->m_iWidth && iHeight == sw->m_iHeight) {
    return ;
  }

  SAFEFREE(sw->m_pPlasmaBits) ;

  sw->m_pPlasmaBits = MALLOC(BYTE, (iWidth * iHeight));
  sw->m_iHeight = iHeight;
  sw->m_iWidth = iWidth;

  // zero out our plasma
  memset(sw->m_pPlasmaBits, 0, (iWidth * iHeight));
  // Init the Cost Table
  CScenePlasmaInitCostBLTable(sw);
  // Init our pallette
  CScenePlasmaInitPallette(sw);
  // Go ahead and calculate a plasma
  CScenePlasmaCalcPlasma(sw);
}

void CScenePlasmaDoDraw(CScenePlasma* sw, int iheight, int iwidth, unsigned char* pBits, int iLineLength, int cn)
{
  int i, x;
  // Right now Im just going to blit it right onto the video memory
  unsigned char* pSrcBitlin; // = sw->m_pFireBits+(sw->m_iWidth*3);// get rid of our fire source
  BYTE* dst; //=(BYTE*)Dib->pVideoMemory;
  COLORREF myColor;
  BYTE r;
  BYTE g;
  BYTE b;
  CScenePlasmaCreate(sw, iheight, iwidth);

  for (i = 0; i < sw->m_iHeight; i++) {
    dst = (BYTE*) & pBits[(iLineLength * i) ];
    pSrcBitlin = &sw->m_pPlasmaBits[ sw->m_iWidth * i ];

    for (x = 0; x < sw->m_iWidth; x++) {
      myColor = sw->m_pPalletteBuffer[ pSrcBitlin[ x ] ];

      r = GetRV(myColor);
      g = GetGV(myColor);
      b = GetBV(myColor);

      dst[ 0 ] = (BYTE)(((r - dst[ 0 ]) * sw->m_iAlpha + (dst[ 0 ] << 8)) >> 8);
      dst[ 1 ] = (BYTE)(((g - dst[ 1 ]) * sw->m_iAlpha + (dst[ 1 ] << 8)) >> 8);
      dst[ 2 ] = (BYTE)(((b - dst[ 2 ]) * sw->m_iAlpha + (dst[ 2 ] << 8)) >> 8);
      dst += cn;
    }
  }

  CScenePlasmaCalcPlasma(sw);
}

void CScenePlasmaSetDefaultValues(CScenePlasma* sw)
{
  sw->m_iAlpha = 255;

  sw->m_iModifier1 = 1;
  sw->m_iModifier2 = 2;
  sw->m_iModifier3 = 1;
  sw->m_iModifier4 = 2;

  sw->m_iXModifier1 = -1;
  sw->m_iXModifier2 = 3;

  sw->m_iYModifier1 = 1;
  sw->m_iYModifier2 = -2;

  // Set up our defalt plasma colors
  sw->m_PlasmaColors[ 0 ] = _RGB(0, 0, 0);   // From black
  sw->m_PlasmaColors[ 1 ] = _RGB(0, 0, 255);   // To Blue
  sw->m_PlasmaColors[ 2 ] = _RGB(0, 0, 255);   // From  Blue
  sw->m_PlasmaColors[ 3 ] = _RGB(0, 255, 0);   // To Green
  sw->m_PlasmaColors[ 4 ] = _RGB(0, 255, 0);   // From Green
  sw->m_PlasmaColors[ 5 ] = _RGB(0, 255, 255);   // To Cyan
  sw->m_PlasmaColors[ 6 ] = _RGB(0, 255, 255);   // Cyan
  sw->m_PlasmaColors[ 7 ] = _RGB(0, 255, 255);
  sw->m_PlasmaColors[ 8 ] = _RGB(0, 255, 255);   // Cyan
  sw->m_PlasmaColors[ 9 ] = _RGB(0, 255, 255);
  sw->m_PlasmaColors[ 10 ] = _RGB(0, 255, 255);   // From White
  sw->m_PlasmaColors[ 11 ] = _RGB(0, 255, 0);   // To dark green
  sw->m_PlasmaColors[ 12 ] = _RGB(0, 255, 0);   // From Dark Blue
  sw->m_PlasmaColors[ 13 ] = _RGB(0, 0, 255);   // To dark green
  sw->m_PlasmaColors[ 14 ] = _RGB(0, 0, 255);   // From Dark Blue
  sw->m_PlasmaColors[ 15 ] = _RGB(0, 0, 0);   // To Black

  CScenePlasmaInitPallette(sw);
}

void CScenePlasmaClear(CScenePlasma* sw)
{
  SAFEFREE(sw->m_pPlasmaBits);
  SAFEFREE(sw);
}
void CScenePlasmaUpdate(CScenePlasma* sw, unsigned long StepTime_ms)
{
  sw->m_iAlpha = StepTime_ms;
}

CScenePlasma* CScenePlasmaInti()
{
  CScenePlasma* sw = MALLOC(CScenePlasma, 1);
  MEMSET(sw, 0, 1);
  sw->m_iHeight = 0;
  sw->m_iWidth = 0;
  sw->m_pPlasmaBits = NULL;
  sw->m_iAlpha = 255;

  sw->m_a1 = 0;
  sw->m_a2 = 0;
  sw->m_a3 = 0;
  sw->m_a4 = 0;
  sw->m_b1 = 0;
  sw->m_b2 = 0;
  sw->m_b3 = 0;
  sw->m_b4 = 0;

  sw->m_iModifier1 = 1;
  sw->m_iModifier2 = 2;
  sw->m_iModifier3 = 1;
  sw->m_iModifier4 = 2;

  sw->m_iXModifier1 = -1;
  sw->m_iXModifier2 = 3;

  sw->m_iYModifier1 = 1;
  sw->m_iYModifier2 = -2;

  // Set up our defalt plasma colors
  sw->m_PlasmaColors[ 0 ] = _RGB(0, 0, 0);   // From black
  sw->m_PlasmaColors[ 1 ] = _RGB(0, 0, 255);   // To Blue

  sw->m_PlasmaColors[ 2 ] = _RGB(0, 0, 255);   // From  Blue
  sw->m_PlasmaColors[ 3 ] = _RGB(0, 255, 0);   // To Green

  sw->m_PlasmaColors[ 4 ] = _RGB(0, 255, 0);   // From Green
  sw->m_PlasmaColors[ 5 ] = _RGB(0, 255, 255);   // To Cyan

  sw->m_PlasmaColors[ 6 ] = _RGB(0, 255, 255);   // Cyan
  sw->m_PlasmaColors[ 7 ] = _RGB(0, 255, 255);

  sw->m_PlasmaColors[ 8 ] = _RGB(0, 255, 255);   // Cyan
  sw->m_PlasmaColors[ 9 ] = _RGB(0, 255, 255);

  sw->m_PlasmaColors[ 10 ] = _RGB(0, 255, 255);   // From White
  sw->m_PlasmaColors[ 11 ] = _RGB(0, 255, 0);   // To dark green

  sw->m_PlasmaColors[ 12 ] = _RGB(0, 255, 0);   // From Dark Blue
  sw->m_PlasmaColors[ 13 ] = _RGB(0, 0, 255);   // To dark green

  sw->m_PlasmaColors[ 14 ] = _RGB(0, 0, 255);   // From Dark Blue
  sw->m_PlasmaColors[ 15 ] = _RGB(0, 0, 0);   // To Black

  SCENE_FRACTAL_SET_CLASS(sw, CScenePlasma);
  return sw;
}

