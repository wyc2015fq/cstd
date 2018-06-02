
enum FlatStyle {
  BUTTONSTYLE_3D,
    BUTTONSTYLE_FLAT,
    BUTTONSTYLE_SEMIFLAT,
    BUTTONSTYLE_NOBORDERS
};

enum AlignStyle {
  ALIGN_LEFT,
    ALIGN_RIGHT,
    ALIGN_CENTER
};

struct button_drawer {
  int m_nFlatStyle;
  int m_nAlignStyle;
  
  img_t* m_Image;
  img_t* m_ImageHot;
  img_t* m_ImageDisabled;
  img_t* m_ImageChecked;
  img_t* m_ImageCheckedHot;
  img_t* m_ImageCheckedDisabled;
  
  ISIZE m_sizeImage;
  
  IMAGES_IDS m_nStdImageId;
  IMAGES_IDS m_nStdImageDisabledId;
  IMAGE_STATE m_StdImageState;
  
  CSize m_sizePushOffset;
  
  COLORREF m_clrRegular;
  COLORREF m_clrHover;
  COLORREF m_clrFace;
  COLORREF m_clrText;
  BOOL m_bTransparent;
  BOOL m_bRighImage; // Is image on the right side?
  BOOL m_bTopImage; // Is image on top?
  
  BOOL m_bOnGlass;
  BOOL m_bGrayDisabled;
  BOOL m_bDrawFocus;
  
  button_drawer() {
    m_bOnGlass = FALSE;
    m_bTransparent = 0;
    m_bTopImage = FALSE;
    m_bGrayDisabled = TRUE;
    m_bDrawFocus = TRUE;
    m_nFlatStyle = BUTTONSTYLE_3D;
    m_sizeImage = CSize(0, 0);
    m_nStdImageId = (IMAGES_IDS) - 1;
    m_nStdImageDisabledId = (IMAGES_IDS) - 1;
    m_StdImageState = ImageBlack;
    m_bRighImage = FALSE;
    m_sizePushOffset = CSize(2, 2);
    m_clrRegular = clrDefault;
    m_clrHover = clrDefault;
    m_clrText = clrDefault;
    m_clrFace = (COLORREF) - 1;
  }
  
  //
  void CleanUp()
  {
    m_nStdImageId = (IMAGES_IDS) - 1;
    m_nStdImageDisabledId = (IMAGES_IDS) - 1;
    m_sizeImage = CSize(0, 0);
  }
  void SetTextColor(COLORREF clrText) {
    m_clrRegular = clrText;
  }
  
  void SetTextHotColor(COLORREF clrTextHot) {
    m_clrHover = clrTextHot;
  }
  void SetImage(img_t* hBitmap, img_t* hBitmapHot = NULL, img_t* hBitmapDisabled = NULL) {
    m_Image = hBitmap;
    m_ImageHot = hBitmapHot;
    m_ImageDisabled = hBitmapDisabled;
  }
  void SetCheckedImage(img_t* hBitmap, img_t* hBitmapHot = NULL, img_t* hBitmapDisabled = NULL) {
    m_ImageChecked = hBitmap;
    m_ImageCheckedHot = hBitmapHot;
    m_ImageCheckedDisabled = hBitmapDisabled;
  }
  
  //
  void SetStdImage(IMAGES_IDS id, IMAGE_STATE state = ImageBlack, IMAGES_IDS idDisabled = (IMAGES_IDS) 0)
  {
    CleanUp();
    
    m_sizeImage = CBCGPMenuImages_Size();
    m_nStdImageId = id;
    m_nStdImageDisabledId = idDisabled;
    m_StdImageState = state;
  }
  
  //
  void DrawBorder(CDC* pDC, CRect& rectClient, UINT uiState, BOOL m_bPushed, BOOL m_bHighlighted, BOOL m_bChecked)
  {
    ASSERT_VALID(pDC);
    
    BOOL bBorderIsReady = FALSE;
    //printf("%d\n", m_bPushed);
    //----------------
    // Draw 3d border:
    //----------------
    if (m_nFlatStyle != BUTTONSTYLE_NOBORDERS) {
      if (m_bPushed && m_bHighlighted || (uiState & ODS_SELECTED) || m_bChecked) {
        if (!bBorderIsReady) {
          pDC->Draw3dRect(rectClient, globalData.clrBtnDkShadow, globalData.clrBtnHilite);
          rectClient.DeflateRect(1, 1);
          
          if (m_nFlatStyle != BUTTONSTYLE_FLAT) {
            pDC->Draw3dRect(rectClient, globalData.clrBtnShadow, globalData.clrBtnLight);
          }
          
          rectClient.DeflateRect(1, 1);
        }
        else {
          rectClient.DeflateRect(2, 2);
        }

        rectClient.l += m_sizePushOffset.w;
        rectClient.t += m_sizePushOffset.h;
      }
      else if (!bBorderIsReady && (m_nFlatStyle != BUTTONSTYLE_FLAT || m_bHighlighted)) {
        pDC->Draw3dRect(rectClient, globalData.clrBtnHilite, globalData.clrBtnDkShadow);
        rectClient.DeflateRect(1, 1);
        
        if (m_nFlatStyle == BUTTONSTYLE_3D || (m_nFlatStyle == BUTTONSTYLE_SEMIFLAT && m_bHighlighted)) {
          pDC->Draw3dRect(rectClient, globalData.clrBtnLight, globalData.clrBtnShadow);
        }
        
        rectClient.DeflateRect(1, 1);
      }
      else {
        rectClient.DeflateRect(2, 2);
      }
    }
    else {
      rectClient.DeflateRect(2, 2);
    }
  }
  
  //
  void SetFaceColor(COLORREF crFace)
  {
    m_clrFace = crFace;
  }
  
  //
  int GetImageHorzMargin() const
  {
    return nImageHorzMargin;
  }
  //
  int GetVertMargin() const
  {
    return nVertMargin;
  }
  
  //
  CSize SizeToContent(font_t* fo, BOOL bCalcOnly, const char* text)
  {
    CString strText = text;
    //GetWindowText(strText);
    
    CSize sizeText(0, 0);
    
    sizeText = GetTextExtent(fo, strText);
    if (m_Image) {
      m_sizeImage.w = m_Image->w;
      m_sizeImage.h = m_Image->h;
    }
    int cx = 0;
    int cy = 0;
    
    if (m_bTopImage) {
      cx = max(sizeText.w, m_sizeImage.w) + GetImageHorzMargin();
      
      if (sizeText.w > 0) {
        cx += GetImageHorzMargin();
      }
      
      cy = sizeText.h + m_sizeImage.h + 2 * GetVertMargin();
      
      if (sizeText.h > 0) {
        cy += GetVertMargin();
      }
    }
    else {
      cx = sizeText.w + m_sizeImage.w + GetImageHorzMargin();
      
      if (sizeText.w > 0) {
        cx += GetImageHorzMargin();
      }
      
      cy = max(sizeText.h, m_sizeImage.h) + GetVertMargin() * 2;
    }
    
    if (!bCalcOnly) {
      //SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
    }
    
    return CSize(cx, cy);
  }
  
  //
  void DoDrawItem(CDC* pDC, CRect rectClient, UINT itemState, CBCGPButtonBase* pButton, const char* text, font_t* fo)
  {
    DWORD dwStyle = pButton->GetStyle();
    BOOL m_bPushed = pButton->IsPushed();
    BOOL m_bHighlighted = pButton->IsHighlighted();
    BOOL m_bChecked = pButton->IsChecked();
    
    m_clrText = clrDefault;
    BOOL m_bRadioButton = FALSE;
    BOOL m_bCheckButton = FALSE;
    BOOL m_bAutoCheck = FALSE;
    BOOL m_b3State = FALSE;
    BOOL m_bDefaultButton = FALSE;
    BOOL m_bIsLeftText = FALSE;
    const char* strText = text;
    
    m_nAlignStyle = ALIGN_CENTER;
    
    switch (dwStyle & 0x0F) {
    case BS_CHECKBOX:
      m_bCheckButton = TRUE;
      break;
      
    case BS_AUTOCHECKBOX:
      m_bCheckButton = m_bAutoCheck = TRUE;
      break;
      
    case BS_3STATE:
      m_bCheckButton = m_b3State = TRUE;
      break;
      
    case BS_AUTO3STATE:
      m_bCheckButton = m_bAutoCheck = m_b3State = TRUE;
      break;
      
    case BS_RADIOBUTTON:
      m_bRadioButton = TRUE;
      break;
      
    case BS_AUTORADIOBUTTON:
      m_bRadioButton = m_bAutoCheck = TRUE;
      break;
      
    case BS_DEFPUSHBUTTON:
      m_bDefaultButton = TRUE;
      break;
    }
    
    BOOL bDefaultDraw = TRUE;
    m_bTransparent = 0;
    if (!m_bTransparent) {
      if (globalData.OnDrawPushButton(pDC, rectClient, pButton, m_clrText)) {
        rectClient.DeflateRect(2, 2);
        bDefaultDraw = FALSE;
      }
    }
    
    if (bDefaultDraw) {
      if (m_bTransparent) {
        // Copy background from the parent window
        globalData.DrawParentBackground(pDC);
      }
      else {
        if (m_clrFace == (COLORREF) - 1) {
          FillRect(pDC, &rectClient, globalData.brBtnFace);
        }
        else {
          imdraw_FillSolidRect(pDC->im, 0, rectClient, m_clrFace);
        }
      }
      
      if (pButton->IsChecked() && pButton->IsHighlightChecked() && !(pButton->IsPushed() && pButton->IsHighlighted())) {
        HighlightRect(pDC->im, 0, rectClient);
      }
      
      if (m_bPushed)
        DrawBorder(pDC, rectClient, itemState, m_bPushed, m_bHighlighted, m_bCheckButton && m_bChecked);
    }
    
    //---------------------
    // Draw button content:
    //---------------------
    {
      UINT uiState = itemState;
      const CRect& rect = rectClient;
      CRect rectText = rect;
      CRect rectImage = rect;
      
      if (m_bCheckButton || m_bRadioButton) {
        if (dwStyle & BS_LEFTTEXT) {
          m_bIsLeftText = TRUE;
        }
        
        switch (dwStyle & BS_CENTER) {
        case BS_LEFT:
        default:
          m_nAlignStyle = ALIGN_LEFT;
          break;
          
        case BS_RIGHT:
          m_nAlignStyle = ALIGN_RIGHT;
          break;
          
        case BS_CENTER:
          m_nAlignStyle = ALIGN_CENTER;
          break;
        }
      }
      
      if (m_sizeImage.w != 0) {
        if (strText && strlen(strText)>0) {
          if (m_bTopImage) {
            rectImage.b = rectImage.t + m_sizeImage.h + GetVertMargin();
            rectText.t = rectImage.b;
            rectText.b -= GetVertMargin();
          }
          else if (m_bRighImage) {
            rectText.r -= m_sizeImage.w + GetImageHorzMargin() / 2;
            rectImage.l = rectText.r;
            rectImage.r -= GetImageHorzMargin() / 2;
          }
          else {
            rectText.l += m_sizeImage.w + GetImageHorzMargin() / 2;
            rectImage.l += GetImageHorzMargin() / 2;
            rectImage.r = rectText.l;
          }
        }
        
        // Center image:
        rectImage.DeflateRect((rectImage.Width() - m_sizeImage.w) / 2,
          max(0, (rectImage.Height() - m_sizeImage.h) / 2));
      }
      else {
        rectImage.SetRectEmpty();
      }
      
      //-----------
      // Draw text:
      //-----------
      CFont* pOldFont = pDC->SelectObject(fo);
      //ASSERT(pOldFont != NULL);
      pDC->SelectObject(fo);
      
      pDC->SetBkMode(TRANSPARENT);
      
      UINT uiDTFlags = DT_END_ELLIPSIS;
      BOOL bIsSingleLine = FALSE;
      
      BOOL bDefaultCheckRadio = (m_bCheckButton || m_bRadioButton) && (dwStyle & BS_PUSHLIKE) == 0;
      BOOL bMultiLine = (dwStyle & BS_MULTILINE) != 0;
      
      COLORREF clrTextDefault = bDefaultCheckRadio ? globalData.clrBarText : globalData.clrBtnText;
      COLORREF clrText = m_clrRegular == clrDefault ? clrTextDefault : m_clrRegular;
      
      if (m_bHighlighted && m_clrHover != clrDefault) {
        clrText = m_clrHover;
      }
      
      if (bDefaultCheckRadio && !bMultiLine) {
        uiDTFlags |= DT_VCENTER | DT_SINGLELINE;
        bIsSingleLine = TRUE;
      }
      else {
        if (bMultiLine) {
          uiDTFlags |= DT_WORDBREAK;
          
          if (!bDefaultCheckRadio) {
            rectText.DeflateRect(GetVertMargin() / 2, GetVertMargin() / 2);
            
            CRect rectCalc = rectText;
            pDC->DrawText(strText, rectCalc, DT_CALCRECT | DT_WORDBREAK);
            
            rectText.t = rectText.CenterPoint().y - rectCalc.Height() / 2;
            rectText.b = rectText.t + rectCalc.Height();
          }
        }
        else if (strchr(strText, _T('\n'))==NULL) {
          uiDTFlags |= DT_VCENTER | DT_SINGLELINE;
          bIsSingleLine = TRUE;
        }
        else {
          rectText.DeflateRect(0, GetVertMargin() / 2);
        }
        
        switch (m_nAlignStyle) {
        case ALIGN_LEFT:
          uiDTFlags |= DT_LEFT;
          rectText.l += GetImageHorzMargin() / 2;
          break;
          
        case ALIGN_RIGHT:
          uiDTFlags |= DT_RIGHT;
          rectText.r -= GetImageHorzMargin() / 2;
          break;
          
        case ALIGN_CENTER:
          uiDTFlags |= DT_CENTER;
          break;
        }
      }
      
      if ((uiState & ODS_DISABLED) && m_bGrayDisabled) {
#ifndef _BCGSUITE_
        
        if (!m_bOnGlass && globalData.IsEmbossDisabledBtnText())
#else
          if (!m_bOnGlass)
#endif
          {
            pDC->SetTextColor(globalData.clrBtnHilite);
            
            CRect rectShft = rectText;
            rectShft.OffsetRect(1, 1);
            OnDrawText(pDC, rectShft, strText, uiDTFlags, uiState);
          }
          
          pDC->SetTextColor(globalData.clrGrayedText);
      }
      else {
        if (m_clrText == clrDefault) {
          pDC->SetTextColor(clrText);
        }
        else {
          pDC->SetTextColor(m_clrText);
        }
      }
      
      OnDrawText(pDC, rectText, strText, uiDTFlags, uiState);
      
      //------------
      // Draw image:
      //------------
      if (!rectImage.IsRectEmpty()) {
        if (m_nStdImageId != (IMAGES_IDS) - 1) {
          IMAGES_IDS id = m_nStdImageId;
          
          if ((uiState & ODS_DISABLED) && m_bGrayDisabled &&
            m_nStdImageDisabledId != 0) {
            id = m_nStdImageDisabledId;
          }
          
          CBCGPMenuImages_Draw(pDC, id, rectImage.TopLeft(), m_StdImageState);
        }
        else {
          BOOL bIsDisabled = (uiState & ODS_DISABLED) && m_bGrayDisabled;
          img_t* imageChecked = (bIsDisabled && m_ImageCheckedDisabled) ? m_ImageCheckedDisabled : (m_bHighlighted && m_ImageCheckedHot) ? m_ImageCheckedHot : m_ImageChecked;
          img_t* image = (bIsDisabled && m_ImageDisabled) ? m_ImageDisabled : (m_bHighlighted && m_ImageHot) ? m_ImageHot : m_Image;
          
          if (m_bChecked && imageChecked) {
            ::DrawImage(pDC, imageChecked, rectImage.l, rectImage.t, 0, FALSE, bIsDisabled && m_ImageCheckedDisabled);
          }
          else if (image) {
            ::DrawImage(pDC, image, rectImage.l, rectImage.t, 0, FALSE, bIsDisabled && m_ImageDisabled);
          }
        }
      }
      
      pDC->SelectObject(pOldFont);
 }
 
 if ((itemState & ODS_FOCUS) && m_bDrawFocus) {
   if (m_bOnGlass) {
     return;
   }
   
   ASSERT_VALID(pDC);
   
   BOOL bDefaultCheckRadio = (m_bCheckButton || m_bRadioButton) && (dwStyle & BS_PUSHLIKE) == 0;
   BOOL bMultiLineCheckRadio = bDefaultCheckRadio && (dwStyle & BS_MULTILINE) != 0;
   
   CRect rectFocus = rectClient;
   
   if (bDefaultCheckRadio && !bMultiLineCheckRadio) {
     CString strText = text;
     
     strText.Remove(_T('&'));
     
     CFont* pOldFont = pDC->SelectObject(fo);
     ASSERT(pOldFont != NULL);
     
     rectFocus.r = rectFocus.l + pDC->GetTextExtent(strText).w;
     rectFocus.t = rectFocus.CenterPoint().y - pDC->GetTextExtent(strText).h / 2;
     rectFocus.b = rectFocus.t + pDC->GetTextExtent(strText).h;
     rectFocus.InflateRect(1, 1);
     
     pDC->DrawFocusRect(rectFocus);
     
     pDC->SelectObject(pOldFont);
     return;
   }
   
   
   rectFocus.DeflateRect(1, 1);
   
   COLORREF clrBckgr = (m_clrFace == (COLORREF) - 1) ? globalData.clrBtnFace : m_clrFace;
   
   {
     rectFocus.DeflateRect(1, 1);
     pDC->Draw3dRect(rectFocus, clrBckgr, clrBckgr);
   }
   
   pDC->DrawFocusRect(rectFocus);
   
 }
}

//
void OnDrawText(CDC* pDC, const CRect& rect, const char* strText, UINT uiDTFlags, UINT uiState)
{
  ASSERT_VALID(pDC);
  
  CRect rectText = rect;
  
  if (m_bOnGlass) {
    globalData.DrawTextOnGlass(pDC, strText, rectText, uiDTFlags, 6,  !(uiState & ODS_DISABLED) ? globalData.clrBarText : globalData.clrGrayedText);
  }
  else {
    pDC->DrawText(strText, -1, rectText, uiDTFlags);
  }
}


};