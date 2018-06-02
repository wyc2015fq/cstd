#include "cstd.h"
#include "draw/imdraw.inl"
#include "Ribbon.inl"
#include "RibbonProfessionalRenderer.inl"
//#include "RibbonButton.inl"
#include "ui/window.inl"
#include "ui/test_ui.inl"
int test_ribbon()
{
  img_t im[1] = {0};
  int h = 600, w = 800;
  IRECT rc;
  Ribbon bib[1] = {0};
  RibbonProfessionalRenderer render[1];
  font_t fo[1] = {0};
  font_init_memraster(fo, "song", zimo_songti_12);
  bib->init();
  render->init();
  imsetsize(im, h, w, 3, 1);
  cvNamedWindow("im", 1);
  imdraw_fill(im, rgb(1, 1, 1));
  rc = iRECT2(0, 0, w, h);
  //bib->UpdateRegions(rc, 1, fo);
  int sMode = RibbonElementSizeMode_Large;
  if (0) {
    RibbonButton btn[1] = {0};
    btn->c->_text = "ÄãºÃ adsf";
    btn->c->type = RibbonItemClass_RibbonButton;
    btn->c->_enabled = 1;
    //btn->c->_selected = 1;
    //btn->c->_pressed = 1;
    btn->c->_checked = 1;
    //ISIZE sz = btn->MeasureSize(sMode, fo);
    btn->c->_bounds = iRECT2(10, 10, 100, 100);
    //render->PaintOn(im, &rc, rc, bib);
    IRECT ItemMargin = iRECT(0, 0, 0, 0);
    render->RibbonButton_OnPaint(im, &rc, btn, ItemMargin, sMode, fo);
  }
  if (0) {
    RibbonTab tab[1] = {0};
    IRECT ItemMargin = iRECT(0, 0, 0, 0);
    tab->_text = "asdf";
    tab->_active = 1;
    tab->_bounds = iRECT2(20, 10, 100, 40);
    tab->_tabContentBounds = iRECT2(10, 50, 200, 200);
    render->RibbonTab_OnPaint(im, &rc, tab, ItemMargin, fo);
  }
  if (1) {
    RibbonPanel pan[1] = {0};
    pan->_text = "asdf";
    pan->_enabled = 1;
    pan->_selected = 1;
    pan->_buttonMoreEnabled = 1;
    pan->_buttonMoreVisible = 1;
    //pan->_buttonMorePressed = 1;
    pan->_bounds = iRECT2(20, 10, 100, 100);
    pan->_contentBounds = iRECT2(22, 12, 96, 80);
    pan->_buttonMoreBounds = iRECT2(100, 92, 20, 20);
    iRectInflate2(&pan->_buttonMoreBounds, -2, -2);
    render->RibbonPanel_OnPaint(im, &rc, pan, sMode, fo);
  }
  imshow(im);
  cvWaitKey(-1);
  return 0;
}

