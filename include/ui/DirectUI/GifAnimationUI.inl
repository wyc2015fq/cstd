
#include "winimage.inl"
typedef struct UIGifAnimation {
  CControlUI base;
  ImageSet AnimGif;
  int CurrentImage;   // Current Image being displayed
} UIGifAnimation;
#define UIGifAnimation_SetPos CControlUI_SetPos
#define UIGIFANIMATION_TIMERID 100
int UIGifAnimation_Event( CWindowWnd* ww, CControlUI* ctr, TEventUI* event) {
  UIGifAnimation* gifa = (UIGifAnimation*)ctr;
  if (UIEVENT_INIT==event->Type) {
    UISetTimer(ww, ctr, UIGIFANIMATION_TIMERID, 30);
  }
  if (UIEVENT_TIMER==event->Type) {
    gifa->CurrentImage=(gifa->CurrentImage+1)%gifa->AnimGif.nImages;
    //Invalidate(ww->m_hWnd);
    NeedUpdate(ww, ctr);
  }
  return CControlUI_Event(ww, ctr, event);
}
int UIGifAnimation_CtrInit(CControlUI* ctr) {
  UIGifAnimation* gifa = (UIGifAnimation*)ctr;
  CControlUI_CtrInit(ctr);
  gifa->CurrentImage = 0;
  return 0;
}
int UIGifAnimation_CtrFree(CControlUI* ctr) {
  UIGifAnimation* gifa = (UIGifAnimation*)ctr;
  ImageSet_Free(&gifa->AnimGif);
  CControlUI_CtrFree(ctr);
  return 0;
}
int UIGifAnimation_LoadAnimatedGif( CControlUI* ctr, LPCTSTR FileName ) {
  UIGifAnimation* gifa = (UIGifAnimation*)ctr;
  int Result = LoadGIF( &gifa->AnimGif, FileName );
  return Result;
}
int UIGifAnimation_SetAttribute( UIResource* res, CControlUI* ctr, LPCTSTR pstrName, LPCTSTR pstrValue ) {
  UIGifAnimation* gifa = (UIGifAnimation*)ctr;
  if ( _tcscmp( pstrName, _T( "gifimage" ) ) == 0 )
    UIGifAnimation_LoadAnimatedGif( ctr, pstrValue );
  else
    CControlUI_SetAttribute( res, ctr, pstrName, pstrValue );
  return 0;
}
int UIGifAnimation_DoPaint( CWindowWnd* ww, CControlUI* ctr, HDC hDC, RECT rcPaint ) {
  UIGifAnimation* gifa = (UIGifAnimation*)ctr;
  C_Image* current = gifa->AnimGif.img[ gifa->CurrentImage ];
  //CControlUI_PaintBkColor(ctr, hDC);
  GDIPaint( current, hDC, ctr->m_rcItem.left + current->xPos, ctr->m_rcItem.top + current->yPos );
  ctr->m_bUpdateNeeded=0;
  return 0;
}
