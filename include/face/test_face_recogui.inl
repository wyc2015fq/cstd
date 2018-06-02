#include "ui/uiwin.inl"
#include "test_face_recog.inl"
typedef editctrl_t title_editctrl_t;
typedef struct test_face_recog_t {
  //win_t win[1];
  button_t btn_del[1];
  slider_t sli0[1];
  layout_t lay0[1];
  layout_t lay_reg[1];
  button_t btn_reg[1];
  title_editctrl_t edt_id[1];
  layout_t lay_ipcap[1];
  title_editctrl_t edt_ip[1];
  title_editctrl_t edt_user[1];
  title_editctrl_t edt_pass[1];
  button_t btn_opencap[1];
  layout_t lay_rec[1];
  button_t btn_rec_clear[1];
  null_t nul0[1];
  listbox_t lb_reg[1];
  listbox_t lb_rec[1];
  capdev cap[1];
  img_t im[1];
  faceapp_t fa[1];
  facerecog_param_t fp[1];
} test_face_recog_t;
static LRESULT test_face_recog_proc(ctrl_t* sender, ctrl_t* c, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  test_face_recog_t* s = (test_face_recog_t*)c->x;
  IRECT rc = c->rc;
  int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
  faceapp_t* g = 0;
  if (WM_CTRLINIT == uMsg) {
    ctrl_set(button_proc, s->btn_reg, s->btn_reg->c, c, "text:s", "×¢²á");
    ctrl_set(button_proc, s->btn_del, s->btn_del->c, c, "text:s", "É¾³ý");
    ctrl_set(button_proc, s->btn_rec_clear, s->btn_rec_clear->c, c, "text:s", "Çå¿Õ¼ÇÂ¼");
    ctrl_set(button_proc, s->btn_opencap, s->btn_opencap->c, c, "text:s", "´ò¿ªipÉãÏñÍ·");
    ctrl_set(slider_proc, s->sli0, s->sli0->c, c, "text:s;min:i;max:i;value:i", "slider %6.3f", 0, 100, 10);
    ctrl_set(listbox_proc, s->lb_reg, s->lb_reg->c, c, 0);
    ctrl_set(listbox_proc, s->lb_rec, s->lb_rec->c, c, 0);
    ctrl_set(title_editctrl_proc, s->edt_id, s->edt_id->c, c, "fmt:i;text:s;title:s", TT_SINGLELINE, "", "id");
    ctrl_set(title_editctrl_proc, s->edt_ip, s->edt_ip->c, c, "fmt:i;text:s;title:s", TT_SINGLELINE, "192.168.1.46", "ip");
    ctrl_set(title_editctrl_proc, s->edt_user, s->edt_user->c, c, "fmt:i;text:s;title:s", TT_SINGLELINE, "admin", "user");
    ctrl_set(title_editctrl_proc, s->edt_pass, s->edt_pass->c, c, "fmt:i;text:s;title:s", TT_SINGLELINE, "123456", "pass");
    ctrl_set(null_proc, s->nul0, s->nul0->c, c, 0);
    ctrl_set(layout_proc, s->lay_ipcap, s->lay_ipcap->c, c, "text:s;gap:i;lay:s", "ipÉãÏñÍ·", 4,
        " x;"
        "a%;"
        "a%;"
        "a%;"
        "a%;"
        , s->edt_ip, s->edt_user, s->edt_pass, s->btn_opencap);
    ctrl_set(layout_proc, s->lay_reg, s->lay_reg->c, c, "text:s;gap:i;lay:s", "×¢²á", 4,
        " x;"
        "a%;"
        "a%;"
        "a%;"
        "x%;"
        , s->edt_id, s->btn_reg, s->btn_del, s->lb_reg);
    ctrl_set(layout_proc, s->lay_rec, s->lay_rec->c, c, "text:s;gap:i;lay:s", "¼ÇÂ¼", 4,
        " x;"
        "a%;"
        "x%;"
        , s->btn_rec_clear, s->lb_rec);
    ctrl_set(layout_proc, s->lay0, s->lay0->c, c, "gap:i;lay:s", 4,
        " xxxx;"
        "a%--%;"//nul grp_ipcap
        "x+ %;"
        "x+ %;"
        , s->nul0, s->lay_ipcap, s->lay_reg, s->lay_rec);
    s->fa->mode = FR_FACE_REC;
    buf_load(FACELIB, s->fa->m_fl);
    buf_load(RECPATH, s->fa->m_rl);
    listbox_set_item_count(s->lb_reg, facelib_get_feat_num(s->fa->m_fl));
    listbox_set_item_count(s->lb_rec, s->fa->m_rl->len / sizeof(SPFaceRecInfo));
    return 0;
  }
  if (!s->cap->x) {
    static int ttt = 0;
    if (WM_TIMER == uMsg) {
      if (0 == ttt) {
        //hcnet_open(s->cap, "192.168.1.45", 8000, "admin", "123456");
        //capvfw_open(capvfw* s, int wIndex)
      }
      ++ttt;
    }
  }
  g = s->fa;
  sub = s->lay0->c;
  sub = s->edt_id->c;
  sub = s->lay0->c;
  if (sub && sub->ctrlproc && WM_SET != uMsg && WM_SIZE != uMsg && WM_PAINT != uMsg) {
    if (sub->ctrlproc(c, sub, hwnd, uMsg, wParam, lParam)) {
      return 1;
    }
  }
  if (WM_CTRLFREE == uMsg) {
    imfree(s->im);
    cap_close(s->cap);
    c->ctrlproc = 0;
    free(s);
    return 0;
  }
  switch (uMsg) {
  case WM_SET:
    if (wParam && lParam) {
      return 0;
    }
    break;
  case WM_TIMER:
    if (s->cap->x) {
      //printf("%d\n", s->str->l);
      cap_getframe(s->cap, s->im, 0);
      if (s->im->h > 0) {
        editctrl_t* edt_id0 = s->edt_id->edit;
        faceapp_run(s->fa, s->fp, s->im, edt_id0->str->s);
        listbox_set_item_count(s->lb_reg, facelib_get_feat_num(s->fa->m_fl));
        listbox_set_item_count(s->lb_rec, s->fa->m_rl->len / sizeof(SPFaceRecInfo));
      }
      force_redraw(c);
    }
    break;
  case WM_GETMINMAXINFO: {
    MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
    mminfo->ptMinTrackSize.x = 600;
    mminfo->ptMinTrackSize.y = 400;
    //printf("WM_MOUSEMOVE\n");
  }
  break;
  case WM_SIZE:
    if (1) {
      sub->rc = c->rc;
      sub->redraw = 1;
      sub->ctrlproc(c, sub, hwnd, uMsg, wParam, lParam);
    }
    break;
  case WM_LBUTTONUP:
    if (sub) {
    }
    break;
  case WM_COMMAND:
    if (sub) {
      if (sender == s->btn_reg->c) {
        s->fa->mode = FR_FACE_REG;
      }
      if (sender == s->btn_rec_clear->c) {
        s->fa->m_rl->len = 0;
        buf_save(RECPATH, s->fa->m_rl);
      }
      if (sender == s->btn_del->c) {
        facelib_del_byid(s->fa->m_fl, s->lb_reg->hit);
        listbox_set_item_count(s->lb_reg, facelib_get_feat_num(s->fa->m_fl));
        buf_save(FACELIB, s->fa->m_fl);
      }
      if (sender == s->btn_opencap->c) {
        if (s->cap->x) {
          cap_close(s->cap);
        }
        //hcnet_open(s->cap, s->edt_ip->edit->str->s, 8000, s->edt_user->edit->str->s, s->edt_pass->edit->str->s);
      }
    }
    break;
  case WM_LBUTTONDBLCLK:
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
    }
    break;
  case WM_MOUSEMOVE:
    if (s) {
    }
    break;
  case WM_MEASUREITEM:
    if (sender == s->lb_reg->c) {
      ISIZE* ds = (ISIZE*)lParam;
      ds->h = 64;
      ds->w = 100;
    }
    if (sender == s->lb_rec->c) {
      ISIZE* ds = (ISIZE*)lParam;
      ds->h = 64;
      ds->w = 100;
    }
    break;
  case WM_DRAWITEM:
    if (sender == s->lb_reg->c) {
      drawitem_t* ds = (drawitem_t*)lParam;
      char buf[256];
      COLOR clrBk = rgb(1, 1, 1);
      COLOR clrText = rgb(0, 0, 0);
      _snprintf(buf, 256, "item %d", wParam);
      if (ds->state & ODS_SELECTED) {
        clrBk = _RGB(0, 0, 255);
        clrText = rgb(1, 1, 1);
      }
      {
        img_t im2[1] = {0};
        int k = wParam;
        SPInfo* sp = facelib_get_feat(g->m_fl, k);
        SPFaceRegInfo* pr = facelib_get_facereginfo(g->m_fl, k);
        IRECT rcImg = ds->itemrc;
        IRECT rcTxt = ds->itemrc;
        rcTxt.l = rcImg.r = ds->itemrc.l + RCH(&ds->itemrc) * STDW / STDH;
        _snprintf(buf, 256, "%s", SPInfoID(sp, 0));
        imdraw_rect(ds->im, ds->cliprc, ds->itemrc, clrBk, 0, 0, 0);
        imdraw_text(ds->im, ds->cliprc, rcTxt, buf, -1, g_scr->font, clrText, 0, TT_VCENTER);
        IMINIT(im2, STDH, STDW, pr->m_data, STDSTEP, STDCN, 1);
        imdraw_image(ds->im, ds->cliprc, &rcImg, im2, 0);
        if (ds->state & ODS_SELECTED) {
          imdraw_rect(ds->im, ds->cliprc, rcImg, rgba(0, 0, 1, 0.1), 0, 0, 0);
        }
      }
    }
    if (sender == s->lb_rec->c) {
      drawitem_t* ds = (drawitem_t*)lParam;
      char buf[256];
      COLOR clrBk = rgb(1, 1, 1);
      COLOR clrText = rgb(0, 0, 0);
      int nfrc = g->m_rl->len / sizeof(SPFaceRecInfo);
      int k = wParam;
      _snprintf(buf, 256, "item %d", wParam);
      if (ds->state & ODS_SELECTED) {
        clrBk = _RGB(0, 0, 255);
        clrText = rgb(1, 1, 1);
      }
      if (k >= 0 && k < nfrc) {
        img_t im2[1] = {0};
        SPFaceRecInfo* frc = ((SPFaceRecInfo*)g->m_rl->data) + k;
        IRECT rcImg = ds->itemrc;
        IRECT rcTxt = ds->itemrc;
        rcTxt.l = rcImg.r = ds->itemrc.l + RCH(&ds->itemrc) * STDW / STDH;
        _snprintf(buf, 256, "%s %5.3f %d-%d-%d %d:%d:%d", frc->m_PID.m_ID, frc->score,
            frc->time.wYear, frc->time.wMonth, frc->time.wDay, frc->time.wHour, frc->time.wMinute, frc->time.wSecond);
        imdraw_rect(ds->im, ds->cliprc, ds->itemrc, clrBk, 0, 0, 0);
        imdraw_text(ds->im, ds->cliprc, rcTxt, buf, -1, g_scr->font, clrText, 0, TT_VCENTER);
        IMINIT(im2, STDH, STDW, frc->m_data, STDSTEP, STDCN, 1);
        imdraw_image(ds->im, ds->cliprc, &rcImg, im2, 0);
        if (ds->state & ODS_SELECTED) {
          imdraw_rect(ds->im, ds->cliprc, rcImg, rgba(0, 0, 1, 0.1), 0, 0, 0);
        }
      }
    }
    break;
  case WM_PAINT:
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      //imdraw_fill(im, rgb(1,1,1));
      imdraw_fill(im, _GetSysColor(CLR_3DFACE));
      imdraw_image(im, &s->nul0->c->rc, &s->nul0->c->rc, s->im, 0);
      faceapp_draw(im, &s->nul0->c->rc, s->im, s->fa, s->fp, g_scr->font);
      sub->redraw = 1;
      c->redraw = 0;
      printf("WM_PAINT\n");
    }
    if (sub && sub->ctrlproc) {
      sub->ctrlproc(c, sub, hwnd, uMsg, wParam, lParam);
    }
    break;
  }
  return 0;
}

