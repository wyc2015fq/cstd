
#define NORMALBTN 1
#define OVERBTN 1
#define PRESSBTN 2

#define UPDATEWIN
enum StyleBtn {STL_NORMAL = 0, STL_FLAT = 1, STL_SEMIFLAT = 2, STL_XP = 3};
enum Type { NORMAL = 0, TOOLBUTTON };

class CToolButton
{
public:
  img_t* m_Icon;
  ISIZE m_SizeImage;
  ISIZE m_SizeText;
  ISIZE m_szButton;
  BOOL m_bMouserOver;
  BOOL m_bShowDark;
  BOOL m_ScrollButton;
  BOOL m_bSimple;
  BOOL m_bMousePress;
  BOOL bPress;
  int m_stlbtn;
  int m_iMilliseconds;
  IRECT rcArea;
  
  int m_nIcon;
  img_t* m_imgList;
  Type bTypeButton;
  BOOL m_bEnabled;
  
  COLOR m_clColor;
  
  void SetSimpleButton(BOOL bSimple = TRUE) {
    m_bSimple = bSimple;
  };
  void SetWait(int iMilliseconds = 50) {
    m_iMilliseconds = iMilliseconds;
  };
  
  
  ISIZE CToolButton::GetSizeButton()
  {
    RecalSize();
    return m_szButton;
  }

  void CToolButton_init()
  {
    m_bMouserOver = FALSE;
    m_SizeImage = iSIZE(0, 0);
    m_SizeText = iSIZE(0, 0);
    m_clColor = GetRGBColorFace();
    m_bShowDark = TRUE;
    m_ScrollButton = FALSE;
    m_bSimple = FALSE;
    m_Icon = NULL;
  }
  
  BOOL CToolButton_OnDrawItem(img_t* im, const IRECT* pclip, IRECT rcItem, int itemState, const char* m_szText, const font_t* fo)
  {
    IRECT rc = rcItem;
    UINT uState = itemState;
    COLOR cb;
    
    if (uState & ODS_SELECTED) { //the button is pressed
      cb = (GetRGBPressBXP());
    }
    else if (m_bMouserOver) {
      cb = (GetRGBFondoXP());
    }
    
    else {
      cb = (m_clColor);
    }
    
    
    if ((uState & ODS_SELECTED) || m_bMouserOver) {
      imdraw_Draw3dRect(im, pclip, &rc, GetRGBCaptionXP(), GetRGBCaptionXP(), 0);
      iRectDeflate2(&rc, 1, 1);
      
    }
    else if (m_ScrollButton || m_bSimple) {
      imdraw_Draw3dRect(im, pclip, &rc, GetRGBColorShadow(), GetRGBColorShadow(), 0);
      iRectDeflate2(&rc, 1, 1);
    }
    
    
    imdraw_FillRect(im, pclip, &rc, cb);
    int calculodify;
    calculodify = RCH(&rc) - (m_SizeImage.h);
    calculodify /= 2;
    int nHeigh = calculodify + (m_bShowDark ? 1 : 0);
    int nWidth = m_ScrollButton ? RCW(&rc) / 2 : 2;
    IPOINT m_point = iPOINT(nWidth, nHeigh);
    
    if (m_SizeImage.w > 2) {
      if (m_bMouserOver == 1 && !(uState & ODS_DISABLED) && !(uState & ODS_SELECTED) && m_bShowDark) {
        //IPOINT p = iPOINT(m_point.x + 1, m_point.y + 1);
        //pdc.DrawState(p,m_SizeImage,m_Icon,DSS_MONO,GetRGBColorShadow());
        m_point.x -= 1;
        m_point.y -= 1;
      }
      IRECT rc1;
      rc1 = iRECT2(m_point.x, m_point.y, m_Icon->w, m_Icon->h);
      imdraw_image(im, pclip, &rc1, m_Icon, 0);
      //pdc.DrawIconEx(m_point, m_Icon, m_SizeImage, (uState == ODS_DISABLED ? DSS_DISABLED : DSS_NORMAL));
    }
    
    if (m_SizeText.w > 2) {
      COLOR crTextColor = Color_Black;
      IRECT rectletra = rc;
      int nt = m_ScrollButton ? 0 : 8;
      rectletra.l += m_SizeImage.w + nt;
      
      if (uState & ODS_DISABLED) {
        //pdc.DrawState(pt, m_SizeText, m_szText, DSS_DISABLED, TRUE, 0, (COLOR*)NULL);
        imdraw_DrawText(im, pclip, m_szText, rectletra, DT_SINGLELINE | DT_LEFT | DT_VCENTER, fo, crTextColor);
      }
      else {
        if (m_bMouserOver != 1) {
        }
        
        imdraw_DrawText(im, pclip, m_szText, rectletra, DT_SINGLELINE | DT_LEFT | DT_VCENTER, fo, crTextColor);
      }
    }
    
    return 1;
  }
  
  void CToolButton::RecalSize()
  {
    int m_High = 0;
    //ISIZE SizeCad = iSIZE(0, 0);
    m_High = MAX(m_SizeImage.h, m_SizeText.h);
    m_szButton = iSIZE(m_SizeText.w + m_SizeImage.w + 7, m_High);
  }
  
  //---------------------------------------------------------------------------------
  
  void NormalButton_init(void)
  {
    m_stlbtn = STL_NORMAL;
    m_clColor = GetRGBColorFace();
  }
  
  void NormalButton_DrawItem(img_t* im, const IRECT* pclip, IRECT rcItem, int itemState, const char* m_szText, const font_t* fo)
  {
    IRECT rc = rcItem;
    UINT uState = itemState;
    COLOR cb;
    cb = (m_clColor);
    COLOR clrTL = GetRGBColorBTNHigh();
    COLOR clrBR = GetRGBColorShadow();
    imdraw_FillRect(im, pclip, &rc, cb);
    
    
    if (uState & ODS_SELECTED) { //the button is pressed
      switch (m_stlbtn) {
      case STL_FLAT:
      case STL_NORMAL:
      case STL_SEMIFLAT:
        imdraw_Draw3dRect(im, pclip, &rc, clrBR, clrTL, 0);
        iRectDeflate2(&rc, 1, 1);
        break;
        
      }
      
    }
    
    else if (m_bMouserOver) {
      switch (m_stlbtn) {
      case STL_FLAT:
      case STL_NORMAL:
      case STL_SEMIFLAT:
        imdraw_Draw3dRect(im, pclip, &rc, clrTL, clrBR, 0);
        iRectDeflate2(&rc, 1, 1);
        break;
        
      }
    }
    else {
      switch (m_stlbtn) {
      case STL_NORMAL:
        imdraw_Draw3dRect(im, pclip, &rc, clrTL, clrBR, 0);
        iRectDeflate2(&rc, 1, 1);
        break;
        
      case STL_SEMIFLAT:
        imdraw_Draw3dRect(im, pclip, &rc, clrTL, clrBR, 0);
        iRectDeflate2(&rc, 1, 1);
        break;
        
      case STL_FLAT:
        break;
      }
    }
    
    int calculodify;
    calculodify = RCH(&rc) - (m_SizeImage.h);
    calculodify /= 2;
    int nHeigh = calculodify + (m_bShowDark ? 1 : 0);
    int nWidth = RCW(&rc) / 2 ;
    IPOINT m_point = iPOINT(nWidth, nHeigh);
    
    if (m_SizeImage.w > 2) {
      IRECT rc1;
      //rc1 = iRECT2(m_point.x + 1, m_point.y + 1, m_Icon->w, m_Icon->h);
      if (m_bMouserOver == 1 && !(uState & ODS_DISABLED) && !(uState & ODS_SELECTED) && m_bShowDark) {
        //pdc.DrawState(p, m_SizeImage, m_Icon, DSS_MONO, (COLOR*)NULL);
        //imdraw_image(im, pclip, &rc1, m_Icon, 0);
        m_point.x -= 1;
        m_point.y -= 1;
      }
      rc1 = iRECT2(m_point.x + 1, m_point.y + 1, m_Icon->w, m_Icon->h);
      imdraw_image(im, pclip, &rc1, m_Icon, 0);
      //pdc.DrawState(m_point, m_SizeImage, m_Icon, DSS_MONO, (COLOR*)NULL);
    }
    
    if (m_SizeText.w > 2) {
      COLOR crText = Color_Black;
      IRECT rectletra = rc;
      rectletra.l += m_SizeImage.w;
      //IPOINT pt = iPOINT(rectletra.t, rectletra.l);
      
      if (uState & ODS_DISABLED) {
        //pdc.DrawState(pt, m_SizeText, m_szText, DSS_DISABLED, TRUE, 0, (COLOR*)NULL);
      }
      else {
        //pdc.DrawText(m_szText, rectletra, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
      }
      imdraw_DrawText(im, pclip, m_szText, rectletra, DT_SINGLELINE | DT_LEFT | DT_VCENTER, fo, crText);
    }
  }
  
  
  
  //-------------------------------------------------------------------
  
  void CTabbedButton_init()
  {
    bPress = FALSE;
    m_iMilliseconds = 50;
  }
  
  //----------------------------
  //CControlBarButton
  //----------------------------
  
  void CControlBarButton_init()
  {
    bTypeButton = (Type)NORMAL;
    m_bEnabled = TRUE;
  }
  
  void CControlBarButton_Paint(img_t* im, const IRECT* pclip, int st, IRECT rc, COLOR clrFondo, BOOL isBackDark)
  {
    COLOR cb;
    
    if (bTypeButton == NORMAL) {
      cb = (clrFondo);
    }
    else {
      if (m_bEnabled)
        if (st == OVERBTN) {
          cb = (GetRGBFondoXP());
        }
        else if (st == PRESSBTN) {
          cb = (GetRGBPressBXP());
        }
        else {
          cb = (clrFondo);
        }
    }
    
    if (m_bEnabled) {
      imdraw_FillRect(im, pclip, &rc, cb);
      
      if (st == OVERBTN)
        imdraw_Draw3dRect(im, pclip, &rc, bTypeButton == NORMAL ?_GetSysColor(CLR_BTNSHADOW) : GetRGBCaptionXP(),
        bTypeButton == NORMAL ?_GetSysColor(CLR_BTNSHADOW) : GetRGBCaptionXP(), 0);
      
      if (st == PRESSBTN)
        imdraw_Draw3dRect(im, pclip, &rc, bTypeButton == NORMAL ?_GetSysColor(CLR_BTNHIGHLIGHT) : GetRGBCaptionXP(),
        bTypeButton == NORMAL ?_GetSysColor(CLR_BTNHIGHLIGHT) : GetRGBCaptionXP(), 0);
    }
    
    if (bTypeButton == NORMAL) {
      IRECT rc1 = iRECT2(rc.l + 1, rc.t + 2, m_imgList->w, m_imgList->h);
      imdraw_imagelist(im, pclip, rc1, m_imgList, isBackDark ? m_nIcon + 1 : m_nIcon, 0, 0, 0);
      //m_imgList.Draw(isBackDark ? m_nIcon + 1 : m_nIcon, pDC->m_hDC, iPOINT(rc.l + 1, rc.t + 2), ILD_TRANSPARENT);
    } else {
      //if (m_bEnabled)
      IRECT rc1 = iRECT2(rc.l + 1, rc.t + 2, m_imgList->w, m_imgList->h);
      imdraw_imagelist(im, pclip, rc1, m_imgList, isBackDark ? m_nIcon + 1 : m_nIcon, 0, 0, 0);
    }
    
  }
  
};

