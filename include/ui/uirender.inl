// sLinks
int imdraw_htmltext(img_t* im, sys_t* sys, const IRECT* pclip, IRECT rc, const char* pstrText, COLOR crText, UINT uStyle)
{
  // 考虑到在xml编辑器中使用<>符号不方便，可以使用{}符号代替
  // The string formatter supports a kind of "mini-html" that consists of various short tags:
  //
  // Bold: <b>text</b>
  // Color: <c #xxxxxx>text</c> where x = RGB in hex
  // Change font: <f x>text</f> where x = font id
  // Image: <i x y> where x = image name and y(optional) = imagelist id
  // NewLine <n>
  // Paragraph: <p x>text</p> where x = row spacing
  // X Indent: <x i> where i = hor indent in pixels
  // Y Indent: <y i> where i = ver indent in pixels
  COLOR TextColor = crText;
  BOOL bDraw = (uStyle & TT_CALCRECT) == 0;
  // If the drawstyle include a alignment, we'll need to first determine the text-size so
  // we can draw it at the correct position...
  if ((uStyle & TT_SINGLELINE) != 0 && (uStyle & TT_VCENTER) != 0 && (uStyle & TT_CALCRECT) == 0) {
    IRECT rcText = { 0, 0, 9999, 100 };
    imdraw_htmltext(im, sys, pclip, rcText, pstrText, crText, uStyle | TT_CALCRECT);
    rc.t = rc.t + ((rc.b - rc.t) / 2) - ((rcText.b - rcText.t) / 2);
    rc.b = rc.t + (rcText.b - rcText.t);
  }
  if ((uStyle & TT_SINGLELINE) != 0 && (uStyle & TT_CENTER) != 0 && (uStyle & TT_CALCRECT) == 0) {
    IRECT rcText = { 0, 0, 9999, 100 };
    int nLinks = 0;
    imdraw_htmltext(im, sys, pclip, rcText, pstrText, crText, uStyle | TT_CALCRECT);
    rc.l = rc.l + ((rc.r - rc.l) / 2) - ((rcText.r - rcText.l) / 2);
    rc.r = rc.l + (rcText.r - rcText.l);
  }
  if ((uStyle & TT_SINGLELINE) != 0 && (uStyle & TT_RIGHT) != 0 && (uStyle & TT_CALCRECT) == 0) {
    IRECT rcText = { 0, 0, 9999, 100 };
    int nLinks = 0;
    imdraw_htmltext(im, sys, pclip, rcText, pstrText, crText, uStyle | TT_CALCRECT);
    rc.l = rc.r - (rcText.r - rcText.l);
  }
  // Determine if we're hovering over a link, because we would like to
  // indicate it by coloring the link text.
  // BUG: This assumes that the prcLink has already been filled once with
  // link coordinates! That is usually not the case at first repaint. We'll clear
  // the remanining entries at exit.
  BOOL bHoverLink = FALSE;
  IPOINT ptMouse = sys->ptLastMousePos;
  IPOINT pt = { rc.l, rc.t };
  int iLinkIndex = 0;
  int cyLine = sys->fo->h;
  int cyMinHeight = 0;
  int cxMaxWidth = 0;
  IPOINT ptLinkStart = { 0 };
  BOOL bInLink = FALSE;
  font_t* fo = sys->fo;
  while (*pstrText != _T('\0')) {
    if (pt.x >= rc.r || *pstrText == _T('\n')) {
      // A new link was detected/requested. We'll adjust the line height
      // for the next line and expand the link hitbox (if any)
      if ((uStyle & TT_SINGLELINE) != 0) {
        break;
      }
      if (*pstrText == _T('\n')) {
        pstrText++;
      }
      pt.x = rc.l;
      pt.y += cyLine/* - tm.tmDescent*/;
      if (pt.y > rc.b && bDraw) {
        break;
      }
      ptLinkStart = pt;
      cyLine = fo->h;
      if (pt.x >= rc.r) {
        break;
      }
      while (*pstrText == _T(' ')) {
        pstrText++;
      }
    }
    else if ((*pstrText == _T('<') || *pstrText == _T('{'))
        && (pstrText[1] >= _T('a') && pstrText[1] <= _T('z'))
        && (pstrText[2] == _T(' ') || pstrText[2] == _T('>') || pstrText[2] == _T('}'))) {
      pstrText++;
      switch (*pstrText) {
      case _T('f'): { // Font
        pstrText++;
        fo = sys_font_load_win32(sys, pstrText);
        cyLine = MAX(cyLine, fo->h);
      }
      break;
      case _T('x'): { // Indent
        pstrText++;
        int iWidth = (int) _tcstol(pstrText, const_cast<LPTSTR*>(&pstrText), 10);
        pt.x += iWidth;
        cxMaxWidth = MAX(cxMaxWidth, pt.x);
      }
      break;
      case _T('n'): { // Newline
        pstrText++;
        if ((uStyle & TT_SINGLELINE) != 0) {
          break;
        }
        pt.x = rc.l;
        pt.y += cyLine/* - tm.tmDescent*/;
        if (pt.y > rc.b && bDraw) {
          break;
        }
        ptLinkStart = pt;
        cyLine = MAX(cyLine, fo->h);
        if (pt.x >= rc.r) {
          break;
        }
      }
      break;
      case _T('y'): {
        pstrText++;
        cyLine = (int) _tcstol(pstrText, const_cast<LPTSTR*>(&pstrText), 10);
      }
      break;
      case _T('i'): { // Image
        pstrText++;
        int iWidth = 0;
        int iHeight = 0;
        if (*pstrText == _T(' ')) {
          pstrText++;
        }
        img_t* pImageInfo = NULL;
        str_t sName[1] = {0};
        int imageListIndex = -1;
        while (*pstrText != _T('\0') && *pstrText != _T('>') && *pstrText != _T('}') && *pstrText != _T(' ')) {
          LPCTSTR pstrTemp = ::CharNext(pstrText);
          while (pstrText < pstrTemp) {
            str_catc(sName, *pstrText++);
          }
        }
        if (*pstrText == _T(' ')) {
          pstrText++;
        }
        if (isdigit(*pstrText)) {
          imageListIndex = (int) _tcstol(pstrText, const_cast<LPTSTR*>(&pstrText), 10);
        }
        if (!sName.IsEmpty()) {
          pImageInfo = sys_load_image_file(sys, (LPCTSTR)sName);
        }
        if (pImageInfo) {
          iWidth = pImageInfo->nX;
          iHeight = pImageInfo->nY;
          if (imageListIndex != -1) {
            if (imageListIndex >= pImageInfo->nX / pImageInfo->nY) {
              imageListIndex = 0;
            }
            iWidth = iHeight;
          }
          if (pt.x + iWidth >= rc.r && iWidth <= rc.r - rc.l && (uStyle & TT_SINGLELINE) == 0) {
            if (bInLink && iLinkIndex < nLinkRects) {
              ::SetRect(&prcLinks[iLinkIndex++], ptLinkStart.x, ptLinkStart.y, MIN(pt.x, rc.r), pt.y + tm.tmHeight);
            }
            if (bInLink && iLinkIndex < nLinkRects) {
              CStdString* pStr1 = (CStdString*)(sLinks + iLinkIndex - 1);
              CStdString* pStr2 = (CStdString*)(sLinks + iLinkIndex);
              *pStr2 = *pStr1;
            }
            pt.x = rc.l;
            pt.y += cyLine/* - tm.tmDescent*/;
            if (pt.y > rc.b && bDraw) {
              break;
            }
            ptLinkStart = pt;
          }
          if (bDraw) {
            CRect rcImage(pt.x, pt.y, pt.x + iWidth, pt.y + iHeight);
            CRect rcBmpPart(0, 0, iWidth, iHeight);
            if (imageListIndex != -1) {
              rcBmpPart.l = iWidth * imageListIndex;
              rcBmpPart.r = iWidth * (imageListIndex + 1);
            }
            CRect rcCorner(0, 0, 0, 0);
            DrawImage(im, pImageInfo->hBitmap, rcImage, rcImage, rcBmpPart, rcCorner, \
                pImageInfo->alphaChannel, 255);
          }
          cyLine = MAX(iHeight, cyLine);
          pt.x += iWidth;
          cyMinHeight = pt.y + iHeight;
          cxMaxWidth = MAX(cxMaxWidth, pt.x);
        }
      }
      break;
      case _T('c'): { // Color
        pstrText++;
        if (*pstrText == _T(' ')) {
          pstrText++;
        }
        if (*pstrText == _T('#')) {
          pstrText++;
        }
        DWORD clrColor = _tcstol(pstrText, const_cast<LPTSTR*>(&pstrText), 16);
        ::SetTextColor(im, RGB(GetBValue(clrColor), GetGValue(clrColor), GetRValue(clrColor)));
      }
      break;
      }
      while (*pstrText != _T('\0') && *pstrText != _T('>') && *pstrText != _T('}')) {
        pstrText = ::CharNext(pstrText);
      }
      pstrText = ::CharNext(pstrText);
    }
    else if ((*pstrText == _T('<') || *pstrText == _T('{')) && pstrText[1] == _T('/')) {
      pstrText++;
      pstrText++;
      switch (*pstrText) {
      case _T('a'):
        pstrText++;
        if (iLinkIndex < nLinkRects) {
          ::SetRect(&prcLinks[iLinkIndex++], ptLinkStart.x, ptLinkStart.y, MIN(pt.x, rc.r), pt.y + fo->h);
        }
        TextColor = crText;
        ::SelectObject(im, sys->GetDefaultFont());
        tm = sys->GetDefaultFontInfo();
        bInLink = FALSE;
        break;
      case _T('f'):
      case _T('b'):
        pstrText++;
        ::SelectObject(im, sys->GetDefaultFont());
        tm = sys->GetDefaultFontInfo();
        break;
      case _T('c'):
        pstrText++;
        ::SetTextColor(im, RGB(GetBValue(crText), GetGValue(crText), GetRValue(crText)));
        break;
      case _T('p'):
        pstrText++;
        cyLine = fo->h;
        break;
      }
      while (*pstrText != _T('\0') && *pstrText != _T('>') && *pstrText != _T('}')) {
        pstrText = ::CharNext(pstrText);
      }
      pstrText = ::CharNext(pstrText);
    }
    else if (*pstrText == _T('<') && pstrText[2] == _T('>') && (pstrText[1] == _T('{') || pstrText[1] == _T('}'))) {
      SIZE szSpace = { 0 };
      ::GetTextExtentPoint32(im, &pstrText[1], 1, &szSpace);
      if (bDraw) {
        ::TextOut(im, pt.x, pt.y, &pstrText[1], 1);
      }
      pt.x += szSpace.cx;
      cxMaxWidth = MAX(cxMaxWidth, pt.x);
      pstrText++;
      pstrText++;
      pstrText++;
    }
    else if (*pstrText == _T('{') && pstrText[2] == _T('}') && (pstrText[1] == _T('<') || pstrText[1] == _T('>'))) {
      SIZE szSpace = { 0 };
      ::GetTextExtentPoint32(im, &pstrText[1], 1, &szSpace);
      if (bDraw) {
        ::TextOut(im, pt.x, pt.y, &pstrText[1], 1);
      }
      pt.x += szSpace.cx;
      cxMaxWidth = MAX(cxMaxWidth, pt.x);
      pstrText++;
      pstrText++;
      pstrText++;
    }
    else if (*pstrText == _T(' ')) {
      SIZE szSpace = { 0 };
      ::GetTextExtentPoint32(im, _T(" "), 1, &szSpace);
      // Still need to paint the space because the font might have
      // underline formatting.
      if (bDraw) {
        ::TextOut(im, pt.x, pt.y, _T(" "), 1);
      }
      pt.x += szSpace.cx;
      cxMaxWidth = MAX(cxMaxWidth, pt.x);
      pstrText++;
    }
    else {
      IPOINT ptPos = pt;
      int cchChars = 0;
      int cchSize = 0;
      int cchLastGoodWord = 0;
      int cchLastGoodSize = 0;
      LPCTSTR p = pstrText;
      SIZE szText = { 0 };
      if (*p == _T('<') || *p == _T('{')) {
        p++, cchChars++, cchSize++;
      }
      while (*p != _T('\0') && *p != _T('<') && *p != _T('{') && *p != _T('\n')) {
        // This part makes sure that we're word-wrapping if needed or providing support
        // for TT_END_ELLIPSIS. Unfortunately the GetTextExtentPoint32() call is pretty
        // slow when repeated so often.
        // TODO: Rewrite and use GetTextExtentExPoint() instead!
        LPCTSTR pstrTemp = ::CharNext(p);
        cchChars++;
        cchSize += (int)(pstrTemp - p);
        szText.cx = cchChars * tm.tmMaxCharWidth;
        if (pt.x + szText.cx >= rc.r) {
          ::GetTextExtentPoint32(im, pstrText, cchSize, &szText);
        }
        if (pt.x + szText.cx > rc.r) {
          if (pt.x + szText.cx > rc.r && pt.x != rc.l) {
            cchChars--;
            cchSize -= (int)(pstrTemp - p);
          }
          if ((uStyle & TT_WORDBREAK) != 0 && cchLastGoodWord > 0) {
            cchChars = cchLastGoodWord;
            cchSize = cchLastGoodSize;
          }
          if ((uStyle & TT_END_ELLIPSIS) != 0 && cchChars > 2) {
            cchChars = cchLastGoodWord;
            cchSize = cchLastGoodSize;
          }
          pt.x = rc.r;
          cxMaxWidth = MAX(cxMaxWidth, pt.x);
          break;
        }
        if (!((p[0] >= _T('a') && p[0] <= _T('z')) || (p[0] >= _T('A') && p[0] <= _T('Z')))) {
          cchLastGoodWord = cchChars;
          cchLastGoodSize = cchSize;
        }
        if (*p == _T(' ')) {
          cchLastGoodWord = cchChars;
          cchLastGoodSize = cchSize;
        }
        p = ::CharNext(p);
      }
      if (cchChars > 0) {
        ::GetTextExtentPoint32(im, pstrText, cchSize, &szText);
        if (bDraw) {
          ::TextOut(im, ptPos.x, ptPos.y, pstrText, cchSize);
          if (pt.x >= rc.r && (uStyle & TT_END_ELLIPSIS) != 0) {
            ::TextOut(im, rc.r - 10, ptPos.y, _T("..."), 3);
          }
        }
        pt.x += szText.cx;
        cxMaxWidth = MAX(cxMaxWidth, pt.x);
        pstrText += cchSize;
      }
    }
    ASSERT(iLinkIndex <= nLinkRects);
  }
  // Clear remaining link rects and return number of used rects
  for (int i = iLinkIndex; i < nLinkRects; i++) {
    ::ZeroMemory(prcLinks + i, sizeof(IRECT));
  }
  nLinkRects = iLinkIndex;
  // Return size of text when requested
  if ((uStyle & TT_CALCRECT) != 0) {
    rc.b = MAX(cyMinHeight, pt.y + cyLine);
    rc.r = MIN(rc.r, cxMaxWidth);
  }
  if (bDraw) {
    ::SelectClipRgn(im, hOldRgn);
  }
  ::DeleteObject(hOldRgn);
  ::DeleteObject(hRgn);
  ::SelectObject(im, hOldFont);
  return 0;
}

