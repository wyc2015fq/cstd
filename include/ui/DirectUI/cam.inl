
#include "imresize.h"
#include "..\imgproc\cap.inl"

#define UICam_SetPos CControlUI_SetPos

#define UICAM_TIMERID  1110
int UICam_Event( CWindowWnd* ww, CControlUI* ctr, TEventUI* event) {
  UICam* cama = (UICam*)ctr;
  if (UIEVENT_INIT==event->Type) {
    UISetTimer(ww, ctr, UICAM_TIMERID, 30);
  }
  if (UIEVENT_TIMER==event->Type) {
    //Invalidate(ww->m_hWnd);
    NeedUpdate(ww, ctr);
    //if (ctr->m_bUpdateNeeded) puts("asdf\n");
    if (cama->cam==NULL) {
      const camera_param_t cp = { 640, 480, cama->rate};
      cama->cam = cam_start(cama->index, &cp);
    }
    if (cama->cam!=NULL) {
      cam_GrabFrame(cama->cam);
    }
  }
  return CControlUI_Event(ww, ctr, event);
}
int UICam_CtrInit(CControlUI* ctr) {
  UICam* cama = (UICam*)ctr;
  CControlUI_CtrInit(ctr);
  cama->index = 0;
  cama->rate = 15;
  cama->cam=0;
  cama->isflip=0;
  return 0;
}
int UICam_CtrFree(CControlUI* ctr) {
  UICam* cama = (UICam*)ctr;
  if (cama->cam) {
    cam_stop( cama->cam );
  }
  CControlUI_CtrFree(ctr);
  return 0;
}
int UICam_SetAttribute( UIResource* res, CControlUI* ctr, LPCTSTR pstrName, LPCTSTR pstrValue ) {
  UICam* cama = (UICam*)ctr;
  if ( _tcscmp( pstrName, _T( "align" ) ) == 0 ) {
    if ( _tcsstr( pstrValue, _T( "left" ) ) != NULL ) {
      cama->m_uStyle = DT_LEFT;
    }
    if ( _tcsstr( pstrValue, _T( "center" ) ) != NULL ) {
      cama->m_uStyle = DT_CENTER;
    }
    if ( _tcsstr( pstrValue, _T( "right" ) ) != NULL ) {
      cama->m_uStyle = DT_RIGHT;
    }
  } else
  if ( _tcscmp( pstrName, _T( "index" ) ) == 0 ) {
    cama->index = _tcstol( pstrValue, 0, 10 );
  } else if ( _tcscmp( pstrName, _T( "rate" ) ) == 0 ) {
    cama->rate = _tcstol( pstrValue, 0, 10 );
  } else if ( _tcscmp( pstrName, _T( "isflip" ) ) == 0 ) {
    cama->isflip = ( _tcscmp( pstrValue, _T( "true" ) ) == 0 );
  } else
    CControlUI_SetAttribute( res, ctr, pstrName, pstrValue );
  return 0;
}
typedef struct _BITMAP_INFO {
  BITMAPINFOHEADER bmiHeader;
  RGBQUAD bmiColors[ 256 ];
}BITMAP_INFO;

int ImgDrawOnDC( HDC hdc, int iHeight, int iWidth, const unsigned char* src, int iPech, int ai, int x, int y, int cx, int cy, int isflip ) {
  BITMAP_INFO bmih;
  unsigned char* GetBits = NULL;
  int ret, iBicnt = ai * 8;
  int dWidth = cx;//iWidth > 600 ? 600 : iWidth;
  int dHeight = cy;//iWidth > 600 ? ( 600 * iHeight / iWidth ) : iHeight;
  int iPech2 = BMPMARK( ( dWidth * ai ) );
  int off=0, iPech1=iPech2;
  
  if ( !src ) {
    return 0;
  }
  make_bitmap_info_header( dWidth, dHeight, iBicnt, &( bmih.bmiHeader ) );
  make_bitmap_rgbquad( bmih.bmiColors, 256 );
  
  GetBits = ( unsigned char * ) malloc( dHeight * iPech2 );
  if (isflip) {
    off = ( dHeight - 1 ) * iPech2, iPech1 = -iPech2;
  } else {
    off = 0, iPech1 = iPech2;
  }
  IMRESIZE( iHeight, iWidth, src, iPech, ai, dHeight, dWidth, GetBits+off, iPech1, ai, INTER_BILINEARITY );
  ret = StretchDIBits( hdc, x, y, dWidth, dHeight, 0, 0, dWidth, dHeight, GetBits,
    ( BITMAPINFO* ) ( &( bmih.bmiHeader ) ), DIB_RGB_COLORS, SRCCOPY );
  free( GetBits );
  return ret;
}

int UICam_DoPaint( CWindowWnd* ww, CControlUI* ctr, HDC hDC, RECT rcPaint ) {
  UICam* cama = (UICam*)ctr;
  if (!ctr->m_bUpdateNeeded)
    return 0;
  if (cama->cam) {
    int h, w, al, ai;
    int x, y, cx, cy;
    unsigned char* a;
    cam_GetFrame( cama->cam, h, w, a, al, ai );
    x = ctr->m_rcItem.left;
    y = ctr->m_rcItem.top;
    cx = rcWidth(ctr->m_rcItem);
    cy = rcHeight(ctr->m_rcItem);
    if (cx*h>cy*w) {
      cx = cy*w/h;
    } else {
      cy = cx*h/w;
    }
    if (cama->m_uStyle==DT_CENTER) {
      x+=(rcWidth(ctr->m_rcItem)-cx)/2;
    } else if (cama->m_uStyle==DT_RIGHT) {
      x=rcWidth(ctr->m_rcItem)-cx;
    }

    ImgDrawOnDC(hDC, h, w, a, al, ai, x, y, cx, cy, cama->isflip);
  }
  return 0;
}
