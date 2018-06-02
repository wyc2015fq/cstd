
//=============================================================================
/**
 *  Soft glow effect (>=24 bit).
@verbatim
    example:
        FCPixelSoftGlow   aCmd (10, 60, 110);
        img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
#define INT_MULT(a,b,t)  ((t) = (a) * (b) + 0x80, ((((t) >> 8) + (t)) >> 8))
int FCPixelSoftGlow(int aw, int ah, BYTE* a, int al, int ai, int nRadius, int nBrightness, int nContrast)
{
  int m_nRadius = nRadius;
  int m_nBrightness = nBrightness;
  int m_nContrast = nContrast;
  int x, y, i;
  // make lightness image
  BYTE* b = (BYTE*)memdup(a, ah * al);

  // gauss blur
  FCPixelBlur_Gauss_IIR(aw, ah, b, al, ai, m_nRadius, m_nRadius);
  FCPixelBrightness(aw, ah, b, al, ai, m_nBrightness);
  FCPixelContrast(aw, ah, b, al, ai, m_nContrast);

  // blend
  for (y = 0; y < ah; y++) {
    for (x = 0; x < aw; x++) {
      BYTE*    pSrc = b + (x) * ai + al * (y);
      BYTE*    pDst = a + (x) * ai + al * (y);

      for (i = 0; i < 3; i++) {
        // screen op
        int   tmp;
        pDst[i] = 255 - INT_MULT((255 - pDst[i]), (255 - pSrc[i]), tmp);
      }
    }
  }

  free(b);
  return 0;
}
