
//=============================================================================
/**
 * Box blur effect (>=24 bit).
@verbatim
 example:
 FCPixelBlur_Box aCmd (5, true);
 img.SinglePixelProcessProc (aCmd);
@endverbatim
*/
/**
 * Constructor.
 * @param bCopyEdge true : blur border with alpha==0 pixel
*/
// if m_iBlock>200, the sum maybe exceed int32
void in_UpdateSum(int* R, int* G, int* B, int* A, const void* pAdd, const void* pSub, int ai)
{
  int nAddA = (ai == 4) ? PCL_A(pAdd) : 0xFF,
      nSubA = (ai == 4) ? PCL_A(pSub) : 0xFF;
  *B += PCL_B(pAdd) * nAddA - PCL_B(pSub) * nSubA;
  *G += PCL_G(pAdd) * nAddA - PCL_G(pSub) * nSubA;
  *R += PCL_R(pAdd) * nAddA - PCL_R(pSub) * nSubA;
  *A += nAddA - nSubA;
}
void in_SetPixel(int R, int G, int B, int A, void* pPixel, int ai, int m_iBlock)
{
  PCL_B(pPixel) = A ? (B / A) : 0;
  PCL_G(pPixel) = A ? (G / A) : 0;
  PCL_R(pPixel) = A ? (R / A) : 0;

  if (ai == 4) {
    PCL_A(pPixel) = A / FSquare(m_iBlock); // pixel number of block
  }
}
int FCPixelBlur_Box(int aw, int ah, BYTE* a, int al, int ai, int iBlockLen)
{
  int nLeftTop;
  int m_iBlock;
  BYTE* b;
  int bw, bl;
  int i, x, y, nx, ny;
  // RGBA sum of every scanline start
  int iFirstR = 0, iFirstG = 0, iFirstB = 0, iFirstA = 0;
  int iCurrR, iCurrG, iCurrB, iCurrA;

  m_iBlock = FMax(2, iBlockLen);

  // expand edge
  nLeftTop = m_iBlock / 2;
  b = ExpandFrame(aw, ah, a, al, ai, nLeftTop);
  bw = aw + nLeftTop * 2;
  bl = bw * ai;

  for (y = 0; y < ah; y++) {
    if (y == 0) { // first line
      iFirstR = 0, iFirstG = 0, iFirstB = 0, iFirstA = 0;

      for (ny = 0; ny < m_iBlock; ny++) {
        for (nx = 0; nx < m_iBlock; nx++) {
          BYTE* pPixel = b + (nx) * ai + bl * (y + ny);
          int nA = (ai == 4) ? PCL_A(pPixel) : 0xFF;
          iFirstB += PCL_B(pPixel) * nA;
          iFirstG += PCL_G(pPixel) * nA;
          iFirstR += PCL_R(pPixel) * nA;
          iFirstA += nA;
        }
      }
    }
    else { // Y move down
      // sub up line & add down line
      for (i = 0; i < m_iBlock; i++) {
        in_UpdateSum(&iFirstR, &iFirstG, &iFirstB, &iFirstA,
            b + (i)*ai + bl * (y - 1 + m_iBlock),
            b + (i)*ai + bl * (y - 1), ai);
      }
    }

    // set first pixel per scanline
    in_SetPixel(iFirstR, iFirstG, iFirstB, iFirstA, a + al * (y), ai, m_iBlock);

    // X move
    iCurrR = iFirstR, iCurrG = iFirstG, iCurrB = iFirstB, iCurrA = iFirstA;

    for (x = 1; x < aw; x++) {
      // sub left pixel & add right pixel
      for (i = 0; i < m_iBlock; i++) {
        in_UpdateSum(&iCurrR, &iCurrG, &iCurrB, &iCurrA,
            b + (x - 1 + m_iBlock)*ai + bl * (y + i),
            b + (x - 1)*ai + bl * (y + i), ai);
      }

      in_SetPixel(iCurrR, iCurrG, iCurrB, iCurrA, a + x * ai + al * y, ai, m_iBlock);
    }
  } // end of for(Y)

  free(b);
  return 0;
}
