

typedef struct test_evalayout_t {
  uictrl base;
  uiwids uw[1];
#define CTRLDEF(type, id, name, arg)  type##_ctrl name[1];
#include "test_evalayout_ctrldef.txt"
#undef CTRLDEF
} test_evalayout_t;
int test_evalayout_paint_proc(const test_evalayout_t* s, const event* e0)
{
  UIHDC(e0);
  UITOER(s, s->uw->wid);
  RECT rc = s->base.rc;

  //draw_evalayout_ctrl(s->eva, e, prc);
  if (e->wid) {
    DrawRect(hDC, s->base.rc, res->clrBtnFace, 0);
    draw_tab_ctrl(s->tab1, e);
  }

  RectIn(rc, 100, &rc);
  //TDrawEllipse(hDC, rc, _RGB(255, 0, 0), res->clrBtnText);
  //draw_uimdi(hDC, s->mdi, prc);
  //skin_win2000_botton_ctrl(hDC, s->btn, s->rcsub);
  //draw_css_border(hDC, 100, 100, 110, 100, _RGB(255, 0, 0), 3, 10, 1, 1);

  if (0) {
    //CCoolControlsManager ccm;
    //RECT rc = iRECT(100, 100, 250, 130);
    //ccm.m_nState = dsHoverMask;
    //ccm.DrawPushButton(hDC, rc, 0);
    //ccm.DrawRadioButton(hDC, rc);
    //ccm.DrawUpDown(hDC, rc);
    //ccm.DrawComboBox(hDC, rc);
    //} else {
    //test_draw_css_rect(hDC);
  }

  return 0;
}
int test_evalayout_frame_proc(test_evalayout_t* s, const event* e0)
{
  UIEVT(e0);
  UITOER(s, s->uw->wid);

  switch (e->msg) {
  case EVENT_LCLICK:
    if (e->sender == s->rbox1) {
      if (0 == s->rbox1->m_cur_item) {
        ui->res->dm_draw_f = dm_win2000;
      }
      else {
        ui->res->dm_draw_f = dm_agg;
      }
    }

    if (e->sender == s->btn1) {
      printf("%s\n", s->btn1->m_text->s);
    }

    if (e->sender == s->btn5) {
      printf("%s\n", s->btn5->m_text->s);
    }

    break;

  case WM_SIZE:
    if (1) {
      s->tab1->base.rc = s->base.rc;
    }

    break;

  case EVENT_EXIT:
    if (1) {
      ui_delctrls(s->uw, e);

      if (e->sender == s) {
        PostQuitMessage(0);
      }
    }

    break;

  case EVENT_INIT:
    if (s) {
#define CTRLDEF(struct, id, name, arg)  ui_initctrl(#struct, s->name, #arg), ui_setctrl(s->uw, id, s->name);
#include "test_evalayout_ctrldef.txt"
#undef CTRLDEF
      //Win_MoveCenter(s->ui->hwnd, 800, 600);
    }

    break;
  }

  if (e->wid) {
    tab_ctrl_event(s->tab1, e);
  }

  return 0;
}

int test_evalayout()
{
  UI_engine ui[1] = {0};
  test_evalayout_t s[1] = {0};

  UISETCALL(s, test_evalayout_paint_proc, test_evalayout_frame_proc);
  //button_ctrl_set(s->eva, "new", 0, 0);
  //ui_loadxml(s->ui, "test001.xml");
  UI_set(ui, "test_evalayout", s, 0);
  waitkey(ui, -1);
  res_free();
  return 0;
}

