
#if 0

//-----------------------------------------------------------------------------
int __ConvertToTrueColor(int iColor)
{
  if (!IsValidImage() || (ColorBits() == iColor)) {
    return ;
  }

  if ((iColor != 24) && (iColor != 32)) {
    assert(false);
    return;
  }

  // backup image
  const FCObjImage     OldPic(*this) ;

  if (!Create(OldPic.Width(), OldPic.Height(), iColor)) {
    return ;
  }

  // get palette
  RGBQUAD     pPal[256] ;

  if (OldPic.ColorBits() <= 8) {
    OldPic.GetColorTable(0, 1 << OldPic.ColorBits(), pPal) ;
  }

  // start color convert
  const int   nNewSpan = this->ColorBits() / 8, // 3 or 4
              nOldSpan = OldPic.ColorBits() / 8 ;

  for (int y = 0 ; y < Height() ; y++) {
    const BYTE*    pOld = OldPic.GetBits(y) ;
    BYTE*    pNew = this->GetBits(y) ;

    for (int x = 0 ; x < Width() ; x++, pNew += nNewSpan, pOld += nOldSpan) {
      switch (OldPic.ColorBits()) {
      case 1 :
      case 4 :
      case 8 : // 1,4,8 ==> 24,32
        CopyPixel(pNew, &pPal[OldPic.GetPixelData(x, y)], 3) ;
        break ;

      case 16 : { // 16 ==> 24,32
        RGBQUAD     crTrans ;

        if (OldPic.m_dwBitFields[1] == MASK16_GREEN_555) {
          crTrans = __Split16Bit_555(*(WORD*)pOld) ;
        }
        else if (OldPic.m_dwBitFields[1] == MASK16_GREEN_565) {
          crTrans = __Split16Bit_565(*(WORD*)pOld) ;
        }

        CopyPixel(pNew, &crTrans, 3) ;
      }
      break ;

      case 24 :
      case 32 : // 24,32 ==> 32,24
        CopyPixel(pNew, pOld, 3) ;
        break ;
      }
    }
  }

  if (iColor == 32) {
    SetAlphaChannelValue(0xFF) ;  // set alpha to 0xFF
  }
}

//-----------------------------------------------------------------------------
int GetAlphaChannel(FCObjImage* imgAlpha) const
{
  // create alpha-channel image, it's a 8-bit color image
  if (!imgAlpha || !IsValidImage() || (ColorBits() != 32) || (imgAlpha == this)
      || !imgAlpha->Create(Width(), Height(), 8)) {
    assert(false) ;
    return ;
  }

  // get alpha channel
  for (int y = 0 ; y < Height() ; y++)
    for (int x = 0 ; x < Width() ; x++) {
      *imgAlpha->GetBits(x, y) = PCL_A(GetBits(x, y)) ;
    }
}
//-----------------------------------------------------------------------------
int AppendAlphaChannel(const FCObjImage& alpha)
{
  if (!IsValidImage() || !alpha.IsValidImage() || (ColorBits() != 32) || (alpha.ColorBits() != 8) ||
      (Width() != alpha.Width()) || (Height() != alpha.Height())) {
    assert(false) ;
    return ;
  }

  // append alpha channel
  for (int y = 0 ; y < Height() ; y++)
    for (int x = 0 ; x < Width() ; x++) {
      PCL_A(GetBits(x, y)) = *alpha.GetBits(x, y) ;
    }
}
//-----------------------------------------------------------------------------
int SetAlphaChannelValue(int nValue)
{
  if (!IsValidImage() || (ColorBits() != 32)) {
    assert(false) ;
    return ;
  }

  for (int y = 0 ; y < Height() ; y++)
    for (int x = 0 ; x < Width() ; x++) {
      PCL_A(GetBits(x, y)) = nValue ;
    }
}
//-----------------------------------------------------------------------------
FCObjImage& operator= (const FCObjImage& img)
{
  if (!img.IsValidImage() || (&img == this)) {
    return (*this) ;
  }

  PCL_array<BITMAPINFOHEADER>   bmfh(img.NewImgInfoWithPalette()) ;

  if (Create(bmfh.get())) {
    // copy the pixels
    memcpy(GetMemStart(), img.GetMemStart(), img.GetPitch()*img.Height()) ;

    // copy the palette
    if (img.ColorBits() <= 8) {
      CopyPalette(img) ;
    }

    // copy position
    FCObjGraph::operator=(img) ;
  }

  return *this ;
}
//-----------------------------------------------------------------------------
bool GetSubBlock(FCObjImage* SubImg, RECT rcBlock) const
{
  if (!IsValidImage() || !SubImg || (SubImg == this) || (ColorBits() < 8)) {
    assert(false);
    return false;
  }

  const RECT   rcImage = {0, 0, Width(), Height()} ;
  RECT   rcD ;
  assert(IsRectInRect(rcImage, rcBlock)) ;

  if (::IntersectRect(&rcD, &rcImage, &rcBlock) == 0) {
    assert(false);
    return false; // rect of destination is empty
  }

  if (!SubImg->Create(RECTWIDTH(rcD), RECTHEIGHT(rcD), ColorBits())) {
    return false ;
  }

  // copy pixel
  const int     nSubPitch = SubImg->Width() * ColorBits() / 8 ;

  for (int i = 0 ; i < SubImg->Height() ; i++) {
    memcpy(SubImg->GetBits(i), GetBits(rcD.left, rcD.top + i), nSubPitch) ;
  }

  // copy palette
  if (ColorBits() <= 8) {
    SubImg->CopyPalette(*this) ;
  }

  // set relative position
  SubImg->SetGraphObjPos(rcD.left, rcD.top) ;
  return true ;
}
//-----------------------------------------------------------------------------
bool CoverBlock(const FCObjImage& img, int x, int y)
{
  if (!IsValidImage() || !img.IsValidImage() || (ColorBits() != img.ColorBits()) || (ColorBits() < 8)) {
    assert(false);
    return false;
  }

  // calculate covered RECT
  const RECT   rcImage = {0, 0, Width(), Height()},
               rcCover = {x, y, x + img.Width(), y + img.Height()} ;
  RECT         rcD ;

  if (::IntersectRect(&rcD, &rcImage, &rcCover) == 0) {
    return false ;  // rect of destination is empty
  }

  // copy pixel
  const int     nSubPitch = RECTWIDTH(rcD) * img.ColorBits() / 8 ;

  for (int cy = rcD.top ; cy < rcD.bottom ; cy++) { // copy
    const BYTE*    pS = img.GetBits(rcD.left - x, cy - y) ; // calculate edge
    BYTE*    pD = this->GetBits(rcD.left, cy) ;
    memcpy(pD, pS, nSubPitch) ;
  }

  return true ;
}
//-----------------------------------------------------------------------------
int TileBlock(const FCObjImage& img, int x, int y)
{
  int   nYStart = y ;

  while (nYStart < Height()) {
    int    nXStart = x ;

    while (nXStart < Width()) {
      CoverBlock(img, nXStart, nYStart) ;  // security ensured by CoverBlock
      nXStart += img.Width() ;
    }

    nYStart += img.Height() ;
  }
}
//-----------------------------------------------------------------------------
int CombineImage(const FCObjImage& Img32, int x, int y, int nAlphaPercent)
{
  RECT   rcD ;
  {
    RECT   rcImg = {0, 0, Width(), Height()},
           rcMask = {x, y, x + Img32.Width(), y + Img32.Height()} ;
    ::IntersectRect(&rcD, &rcImg, &rcMask) ;
  }

  if ((Img32.ColorBits() != 32) || (ColorBits() != 32) || IsRectEmpty(&rcD)) {
    assert(false);
    return;
  }

  nAlphaPercent = FClamp(nAlphaPercent, 0, 100) ;

  for (int cy = rcD.top ; cy < rcD.bottom ; cy++) {
    RGBQUAD*    pDest = (RGBQUAD*)this->GetBits(rcD.left, cy),
                * pSrc = (RGBQUAD*)Img32.GetBits(rcD.left - x, cy - y) ; // calculate edge

    for (int cx = rcD.left ; cx < rcD.right ; cx++, pDest++, pSrc++)
      CombineAlphaPixel(pDest, *pDest, pSrc,
          (nAlphaPercent == 100) ? PCL_A(pSrc)
          : (PCL_A(pSrc)*nAlphaPercent / 100)) ;
  }
}
//-----------------------------------------------------------------------------
int AlphaBlend(const FCObjImage& Img32, RECT rcDest, RECT rcSrc, int nAlphaPercent)
{
  if (nAlphaPercent == 0) {
    return ;
  }

  // parameter check, rcSrc must inside of Img32
  RECT     rcT ;
  {
    RECT   rcMask = {0, 0, Img32.Width(), Img32.Height()},
           rcImg = {0, 0, Width(), Height()} ;
    IntersectRect(&rcT, &rcDest, &rcImg) ;

    if (!IsValidImage() || (ColorBits() < 24) || !Img32.IsValidImage() || (Img32.ColorBits() != 32) ||
        !IsRectInRect(rcMask, rcSrc) || IsRectEmpty(&rcT)) {
      assert(false);
      return;
    }
  }

  nAlphaPercent = FClamp(nAlphaPercent, 0, 100) ;
  const int   nSpan = ColorBits() / 8 ; // 3 or 4

  if ((RECTWIDTH(rcDest) == RECTWIDTH(rcSrc)) && (RECTHEIGHT(rcDest) == RECTHEIGHT(rcSrc))) {
    const int   nSrcX = rcSrc.left + rcT.left - rcDest.left ;

    for (int y = rcT.top ; y < rcT.bottom ; y++) {
      const BYTE*   pSrc = Img32.GetBits(nSrcX, rcSrc.top + y - rcDest.top) ;  // notice edge
      BYTE*         pDest = this->GetBits(rcT.left, y) ;

      for (int x = rcT.left ; x < rcT.right ; x++, pDest += nSpan, pSrc += 4)
        AlphaBlendPixel(pDest, pSrc, (nAlphaPercent == 100) ? PCL_A(pSrc)
            : PCL_A(pSrc) * nAlphaPercent / 100) ;
    }
  }
  else {
    PCL_array<int>   pX(RECTWIDTH(rcT)),
              pY(RECTHEIGHT(rcT)) ;

    {
      // get coordinate on source image
      for (int y = rcT.top ; y < rcT.bottom ; y++) {
        int   n = rcSrc.top + (y - rcDest.top) * RECTHEIGHT(rcSrc) / RECTHEIGHT(rcDest) ;

        // get max which same scaled
        while (true) {
          int   yn1 = rcDest.top + (n + 1 - rcSrc.top) * RECTHEIGHT(rcDest) / RECTHEIGHT(rcSrc) ;

          if (yn1 != y) {
            break ;
          }

          n++ ;
        }

        pY[y - rcT.top] = FClamp(n, 0, Img32.Height() - 1) ;
      }

      for (int x = rcT.left ; x < rcT.right ; x++) {
        int   n = rcSrc.left + (x - rcDest.left) * RECTWIDTH(rcSrc) / RECTWIDTH(rcDest) ;

        // get max which same scaled
        while (true) {
          int   xn1 = rcDest.left + (n + 1 - rcSrc.left) * RECTWIDTH(rcDest) / RECTWIDTH(rcSrc) ;

          if (xn1 != x) {
            break ;
          }

          n++ ;
        }

        pX[x - rcT.left] = FClamp(n, 0, Img32.Width() - 1) ;
      }
    }

    for (int y = rcT.top ; y < rcT.bottom ; y++) {
      BYTE*    pDest = this->GetBits(rcT.left, y) ;

      for (int x = rcT.left ; x < rcT.right ; x++, pDest += nSpan) {
        const BYTE*    pSrc = Img32.GetBits(pX[x - rcT.left], pY[y - rcT.top]) ;
        AlphaBlendPixel(pDest, pSrc, (nAlphaPercent == 100) ? PCL_A(pSrc)
            : PCL_A(pSrc)*nAlphaPercent / 100) ;
      }
    }
  }
}
//-----------------------------------------------------------------------------
int LogicalBlend(const FCObjImage& MaskImg, LOGICAL_OP LogOP, int x, int y)
{
  if (!IsValidImage() || !MaskImg.IsValidImage() || (ColorBits() != MaskImg.ColorBits()) || (this == &MaskImg) || (ColorBits() != 8)) {
    assert(false) ;
    return ;
  }

  // inner rect
  RECT   rc ;
  {
    RECT   rc1 = {0, 0, Width(), Height()},
           rc2 = {x, y, x + MaskImg.Width(), y + MaskImg.Height()} ;

    if (::IntersectRect(&rc, &rc1, &rc2) == 0) {
      return ;  // no intersected rect
    }
  }

  for (int cy = rc.top ; cy < rc.bottom ; cy++)
    for (int cx = rc.left ; cx < rc.right ; cx++) {
      BYTE*    pSrc = MaskImg.GetBits(cx - x, cy - y) ; // notice edge
      BYTE*    pDest = this->GetBits(cx, cy) ;

      switch (LogOP) {
      case LOGI_OR  :
        *pDest |= *pSrc ;
        break ;

      case LOGI_AND :
        *pDest &= *pSrc ;
        break ;

      case LOGI_XOR :
        *pDest ^= *pSrc ;
        break ;

      case LOGI_SEL_ADD :
        if (*pSrc == 0xFF) {
          *pDest = 0xFF ;
        }

        break ;

      case LOGI_SEL_SUB :
        if (*pSrc == 0xFF) {
          *pDest = 0 ;
        }

        break ;

      default :
        assert(false);
      }
    }
}
//-----------------------------------------------------------------------------
int Serialize(bool bSave, BYTE* pSave)
{
  const BYTE*    pBak = pSave ;

  if (bSave) { // save
    assert(ColorBits() > 8) ;  // must true color image
    memcpy(pSave, &m_DibInfo, sizeof(m_DibInfo)) ;
    pSave += sizeof(m_DibInfo) ;
    memcpy(pSave, m_dwBitFields, 12) ;
    pSave += 12 ;

    int     nWrite = GetPitch() * Height() ;
    memcpy(pSave, GetMemStart(), nWrite) ;
    pSave += nWrite ;
  }
  else { // load
    Create((BITMAPINFOHEADER*)pSave) ;
    pSave += sizeof(m_DibInfo) + 12 ;

    int     nWrite = GetPitch() * Height() ;
    memcpy(GetMemStart(), pSave, nWrite) ;
    pSave += nWrite ;
  }

  pSave += FCObjGraph::Serialize(bSave, pSave) ;
  return (int)(pSave - pBak) ;
}
//-----------------------------------------------------------------------------
bool Load(const char* szFileName, FCImageProperty* pProperty)
{
  IMAGE_TYPE     imgType = GetImageHandleFactory()->QueryImageFileType(szFileName) ;
  std::auto_ptr<FCImageHandleBase>  pHandler(GetImageHandleFactory()->CreateImageHandle(imgType)) ;

  if (!pHandler.get()) {
    return false ;
  }

  PCL_Interface_Composite<FCObjImage>   listImage ;
  std::auto_ptr<FCImageProperty>        pImgProp ;
  bool     bRet = pHandler->LoadImageFile(szFileName, listImage, pImgProp) ;

  if (bRet) {
    if (listImage.PCL_GetObjectCount()) {
      *this = *listImage.PCL_GetObject(0) ;
    }

    if (pImgProp.get() && pProperty) {
      *pProperty = *pImgProp ;
    }
  }

  assert(bRet) ;
  return bRet ;
}
//-----------------------------------------------------------------------------
bool Load(BYTE* pStart, int nMemSize, IMAGE_TYPE imgType, FCImageProperty* pProperty)
{
  std::auto_ptr<FCImageHandleBase>  pHandler(GetImageHandleFactory()->CreateImageHandle(imgType)) ;

  if (!pHandler.get()) {
    return false ;
  }

  PCL_Interface_Composite<FCObjImage>   listImage ;
  std::auto_ptr<FCImageProperty>        pImgProp ;
  bool     bRet = pHandler->LoadImageMemory(pStart, nMemSize, listImage, pImgProp) ;

  if (bRet) {
    if (listImage.PCL_GetObjectCount()) {
      *this = *listImage.PCL_GetObject(0) ;
    }

    if (pImgProp.get() && pProperty) {
      *pProperty = *pImgProp ;
    }
  }

  assert(bRet) ;
  return bRet ;
}
//-----------------------------------------------------------------------------
bool LoadDIBStream(const int* pDIB, int nBufferSize)
{
  const BITMAPINFOHEADER*    pBmif = (const BITMAPINFOHEADER*)pDIB ;

  if (!Create(pBmif)) {
    assert(false);
    return false;
  }

  const BYTE*    p = (const BYTE*)pDIB + pBmif->biSize ;

  if (ColorBits() <= 8) {
    int   n = 1 << ColorBits() ;
    SetColorTable(0, n, (const RGBQUAD*)p) ;
    p += (4 * n) ;
  }
  else if (pBmif->biCompression == BI_BITFIELDS) {
    p += 12 ;
  }

  // copy pixel
  int   nLeave = nBufferSize - (int)(p - (BYTE*)pDIB) ;
  assert(nLeave >= GetPitch()*Height()) ;
  memcpy(GetMemStart(), p, FMin(nLeave, GetPitch()*Height())) ;
  return true ;
}
//-----------------------------------------------------------------------------
bool Save(const char* szFileName, const FCImageProperty& rProp) const
{
  if (!IsValidImage() || !szFileName) {
    return false ;
  }

  IMAGE_TYPE     imgType = GetImageHandleFactory()->QueryImageFileType(szFileName) ;
  std::auto_ptr<FCImageHandleBase>  pHandler(GetImageHandleFactory()->CreateImageHandle(imgType)) ;

  if (!pHandler.get()) {
    return false ;
  }

  // save list
  std::deque<const FCObjImage*>   saveList ;
  saveList.push_back(this) ;

  return pHandler->SaveImageFile(szFileName, saveList, rProp) ;
}
//-----------------------------------------------------------------------------
int __FillImageRect(const FCObjImage& img, const RECT& rcBlock, const int* pSrc)
{
  RECT     rc = {0, 0, img.Width(), img.Height()} ;
  IntersectRect(&rc, &rc, &rcBlock) ;

  if (IsRectEmpty(&rc)) {
    return  ;
  }

  const int   nSpan = img.ColorBits() / 8 ; // 1, 2, 3, 4

  for (int y = rc.top ; y < rc.bottom ; y++) {
    BYTE*    pPixel = img.GetBits(rc.left, y) ;

    for (int x = rc.left ; x < rc.right ; x++, pPixel += nSpan) {
      CopyPixel(pPixel, pSrc, nSpan) ;
    }
  }
}
// add frame
// bCopyEdge: duplicate edge during copying
int ExpandFrame(bool bCopyEdge, int iLeft, int iTop, int iRight, int iBottom)
{
  if ((ColorBits() < 8) || (iLeft < 0) || (iTop < 0) || (iRight < 0) || (iBottom < 0)) {
    assert(false) ;
    return ;
  }

  if ((iLeft == 0) && (iTop == 0) && (iRight == 0) && (iBottom == 0)) {
    return ;
  }

  // backup image then create expanded image
  const FCObjImage     imgOld(*this) ;

  if (!Create(imgOld.Width() + iLeft + iRight, imgOld.Height() + iTop + iBottom, imgOld.ColorBits())) {
    assert(false) ;
    return ;
  }

  // adjust image's position
  SetGraphObjPos(imgOld.GetGraphObjPos().x - iLeft, imgOld.GetGraphObjPos().y - iTop) ;

  // duplicate source image
  CoverBlock(imgOld, iLeft, iTop) ;

  // edge disposal
  if (!bCopyEdge) {
    return ;
  }

  // duplicate corner
  const RECT   rcUL = {0, 0, iTop, iLeft},
               rcUR = {Width() - iRight, 0, Width(), iTop},
               rcDL = {0, Height() - iBottom, iLeft, Height()},
               rcDR = {Width() - iRight, Height() - iBottom, Width(), Height()} ;
  __FillImageRect(*this, rcUL, imgOld.GetBits(0, 0)) ;
  __FillImageRect(*this, rcUR, imgOld.GetBits(imgOld.Width() - 1, 0)) ;
  __FillImageRect(*this, rcDL, imgOld.GetBits(0, imgOld.Height() - 1)) ;
  __FillImageRect(*this, rcDR, imgOld.GetBits(imgOld.Width() - 1, imgOld.Height() - 1)) ;

  // duplicate four-edge
  const int   dwPitch = GetPitch(),
              nSpan = ColorBits() / 8,
              nOldLineBytes = imgOld.Width() * nSpan ;
  int         m ;
  BYTE*         pSrc, * pDest ;

  // up
  pSrc = GetBits(iLeft, iTop) ;
  pDest = pSrc + dwPitch ;

  for (m = 0 ; m < iTop ; m++, pDest += dwPitch) {
    memcpy(pDest, pSrc, nOldLineBytes) ;
  }

  // bottom
  pSrc = GetBits(iLeft, imgOld.Height() + iTop - 1) ;
  pDest = pSrc - dwPitch ;

  for (m = 0 ; m < iBottom ; m++, pDest -= dwPitch) {
    memcpy(pDest, pSrc, nOldLineBytes) ;
  }

  // left
  pSrc = GetBits(iLeft, iTop) ;
  pDest = GetBits(0, iTop) ;

  for (m = 0 ; m < imgOld.Height() ; m++, pDest -= dwPitch, pSrc -= dwPitch) {
    BYTE*      pTemp = pDest ;

    for (int i = 0 ; i < iLeft ; i++, pTemp += nSpan) {
      CopyPixel(pTemp, pSrc, nSpan) ;
    }
  }

  // right
  pSrc = GetBits(iLeft + imgOld.Width() - 1, iTop) ;
  pDest = pSrc + nSpan ;

  for (m = 0 ; m < imgOld.Height() ; m++, pDest -= dwPitch, pSrc -= dwPitch) {
    BYTE*      pTemp = pDest ;

    for (int i = 0 ; i < iRight ; i++, pTemp += nSpan) {
      CopyPixel(pTemp, pSrc, nSpan) ;
    }
  }
}
//-----------------------------------------------------------------------------
// stretch (>=8 bit)
int Stretch(int nNewWidth, int nNewHeight)
{
  // parameter check
  if (!IsValidImage() || (nNewWidth <= 0) || (nNewHeight <= 0) || (ColorBits() < 8)) {
    //        assert(false) ;
    return ;
  }

  if ((nNewWidth == Width()) && (nNewHeight == Height())) {
    return ;
  }

  // first backup image
  const FCObjImage     imgOld(*this) ;

  if (!Create(nNewWidth, nNewHeight, imgOld.ColorBits())) {
    assert(false) ;
    return ;
  }

  // duplicate palette
  if (ColorBits() <= 8) {
    CopyPalette(imgOld) ;
  }

  // initialize index table
  const int        nSpan = ColorBits() / 8 ;
  PCL_array<int>   pTabX(Width()) ;

  for (int xx = 0 ; xx < Width() ; xx++) {
    pTabX[xx] = xx * imgOld.Width() / Width() ; // force to omit float
    assert(pTabX[xx] < imgOld.Width()) ;
  }

  for (int mm = 0 ; mm < (Width() - 1) ; mm++) {
    pTabX[mm] = (pTabX[mm + 1] - pTabX[mm]) * nSpan ;
  }

  // pTabX[i] put X byte span

  for (int y = 0 ; y < Height() ; y++) {
    const BYTE*    pOld = imgOld.GetBits(y * imgOld.Height() / Height()) ;
    BYTE*    pPixel = GetBits(y) ;

    for (int x = 0 ; x < Width() ; x++) {
      CopyPixel(pPixel, pOld, nSpan) ;
      pOld += pTabX[x] ;
      pPixel += nSpan ;
    }
  }
}
//-----------------------------------------------------------------------------
int SinglePixelProcessProc(FCInterface_PixelProcess& rProcessor, FCObjProgress* pProgress)
{
  if (!rProcessor.ValidateColorBits(this)) {
    assert(false);
    return;
  }

  // before
  rProcessor.OnEnterProcess(this) ;

  if (pProgress) {
    pProgress->ResetProgress() ;  // reset to 0
  }

  switch (rProcessor.QueryProcessType()) {
  case FCInterface_PixelProcess::PROCESS_TYPE_PIXEL : {
    for (int y = 0 ; y < Height() ; y++) {
      for (int x = 0 ; x < Width() ; x++) {
        rProcessor.ProcessPixel(this, x, y, GetBits(x, y)) ;
      }

      if (pProgress) {
        pProgress->SetProgress((y + 1) * 100 / Height()) ;
      }
    }
  }
  break ;

  case FCInterface_PixelProcess::PROCESS_TYPE_WHOLE :
    rProcessor.ProcessWholeImage(this, pProgress) ;
    break ;
  }

  // after
  rProcessor.OnLeaveProcess(this) ;
}

#endif
