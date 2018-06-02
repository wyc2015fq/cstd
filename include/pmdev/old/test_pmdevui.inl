
#include "ui/test_ui.inl"
#include "img/imload.inl"
#include "img/color.inl"
//#include "fmtimg/fmtimg.inl"

typedef struct test_pmdevui_t {
  ctrl_t c[1];
  button_t btn_connect[1];
  button_t btn_CPM_DLAllUser[1];
  button_t btn_CPM_DLDevBaseInfo[1];
  button_t btn_CPM_DLAllIdentifyRecord[1];
  title_editctrl_t edt_ip[1];
  title_editctrl_t edt_user[1];
  title_editctrl_t edt_pass[1];
  layout_t lay0[1];
  listbox_t lb_reg[1];
  listbox_t lb_rec[1];
  listbox_t lb_log[1];
  null_t nul0[1];
  pmdev_t pv[1];
  vstr_t sv_log[1];
  DEV_BATCH_USER user[1];
  DEV_BATCH_RECORD record[1];
} test_pmdevui_t;

test_pmdevui_t* g_pdev = 0;

static int addlistprintf(const char* fmt, ...)
{
  test_pmdevui_t* s = g_pdev;

  if (s) {
    char buf[1024];
    va_list ARGLIST;
    va_start(ARGLIST, fmt);
    _vsnprintf(buf, 1024, fmt, ARGLIST);
    strv_ins_str(s->sv_log, -1, STR1(buf));
    listbox_set_item_count(s->lb_log, g_pdev->sv_log->n);
    sendmsg(s->c, s->lb_log->c, s->lb_log->c->hwnd, WM_VSCROLL, 0, SB_BOTTOM);
    va_end(ARGLIST);
  }

  return 0;
}

BOOL eventfunui(pmdev_t* pv, const char* sIP, const char* sClientIP, int eType, int eFlag, void* pvContent, int nSerialNo, int eResult, void* pvClientData)
{
  test_pmdevui_t* s = g_pdev;
  ctrl_t* c = (ctrl_t*)pv->userdata;
#define PRINTF addlistprintf
#include "eventfunui_impl.inl"
  return 0;
}

static LRESULT test_pmdevui_proc(ctrl_t* sender, ctrl_t* c, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  test_pmdevui_t* s = (test_pmdevui_t*)c->x;
  IRECT rc = c->rc;
  int x = LOWORD(lParam), y = HIWORD(lParam);
  ctrl_t* sub = 0;

  if (WM_CTRLINIT == uMsg) {
    const char *def_ip = "192.168.1.155";
    const char *def_user = "admin";
    const char *def_pass = "201031";
    ctrl_set(button_proc, s->btn_connect, s->btn_connect->c, c, c->sys);
    s->btn_connect->text = "连接";
    ctrl_set(button_proc, s->btn_CPM_DLAllUser, s->btn_CPM_DLAllUser->c, c, c->sys);
    s->btn_CPM_DLAllUser->text = "下载所有用户";
    ctrl_set(button_proc, s->btn_CPM_DLAllIdentifyRecord, s->btn_CPM_DLAllIdentifyRecord->c, c, c->sys);
    s->btn_CPM_DLAllIdentifyRecord->text = "下载所有识别记录";
    ctrl_set(button_proc, s->btn_CPM_DLDevBaseInfo, s->btn_CPM_DLDevBaseInfo->c, c, c->sys);
    s->btn_CPM_DLDevBaseInfo->text = "获取基本信息";
    ctrl_set(title_editctrl_proc, s->edt_ip, s->edt_ip->c, c, c->sys);
    title_editctrl_set(s->edt_ip, "ip", def_ip, TT_SINGLELINE);
    ctrl_set(title_editctrl_proc, s->edt_user, s->edt_user->c, c, c->sys);
    title_editctrl_set(s->edt_user, "user", def_user, TT_SINGLELINE);
    ctrl_set(title_editctrl_proc, s->edt_pass, s->edt_pass->c, c, c->sys);
    title_editctrl_set(s->edt_pass, "pass", def_pass, TT_SINGLELINE);
    ctrl_set(listbox_proc, s->lb_reg, s->lb_reg->c, c, c->sys);
    ctrl_set(listbox_proc, s->lb_rec, s->lb_rec->c, c, c->sys);
    ctrl_set(listbox_proc, s->lb_log, s->lb_log->c, c, c->sys);
    ctrl_set(null_proc, s->nul0, s->nul0->c, c, c->sys);

    ctrl_set(layout_proc, s->lay0, s->lay0->c, c, c->sys);
    layout_set(s->lay0, 4, 0,
        " xxxx;"
        "a%%%%;"//nul grp_ipcap
        "a% %%;"//
        "x%% %;"
        "x+  +;"
        "x+  +;"
        "x%---;"
        , s->edt_ip->c, s->edt_user, s->edt_pass, s->btn_connect
        , s->btn_CPM_DLAllUser, s->btn_CPM_DLDevBaseInfo, s->btn_CPM_DLAllIdentifyRecord, s->lb_reg, s->nul0, s->lb_rec, s->lb_log);
    g_pdev = s;

    if (0) {
      int i;

      for (i = 0; i < 100; ++i) {
        addlistprintf(" %d CPM_Connect\n", i);
      }
    }

    CPM_Connect(s->pv, def_ip, def_user, def_pass, eventfunui, c);
    return 0;
  }

  sub = s->lay0->c;

  if (sub->ctrlproc && WM_SIZE != uMsg && WM_PAINT != uMsg) {
    if (sub->ctrlproc(c, sub, hwnd, uMsg, wParam, lParam)) {
      return 1;
    }
  }

  if (WM_CTRLFREE == uMsg) {
    c->ctrlproc = 0;
    free(s);
    return 0;
  }

  switch (uMsg) {
  case WM_TIMER:
    if (1) {
      //force_redraw(c);
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
      force_redraw(c);
    }

    break;

  case WM_LBUTTONUP:
    if (sub) {
    }

    break;

  case WM_COMMAND:
    if (sub) {
      pmdev_t* pv = s->pv;

      if (sender == s->btn_connect->c) {
        char* ip = title_editctrl_gettext(s->edt_ip);
        char* user = title_editctrl_gettext(s->edt_user);
        char* pass = title_editctrl_gettext(s->edt_pass);
        CPM_Connect(pv, ip, user, pass, eventfunui, c);
      }

      if (sender == s->btn_CPM_DLAllUser->c) {
        CPM_DLAllUser(pv);
      }

      if (sender == s->btn_CPM_DLAllIdentifyRecord->c) {
        CPM_DLAllIdentifyRecord(pv);
      }

      if (sender == s->btn_CPM_DLDevBaseInfo->c) {
        CPM_DLDevBaseInfo(pv);
      }

      if (sender == s->lb_reg->c) {
        switch (wParam) {
        case LBN_SELCANCEL:
          force_redraw(c);
          break;
        }
      }

      force_redraw(c);
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

  case LBN_SELCANCEL:
    break;

  case WM_MEASUREITEM:
    if (sender == s->lb_log->c) {
      ISIZE* ds = (ISIZE*)lParam;
      ds->h = 14;
      ds->w = 100;
    }

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
    if (sender == s->lb_log->c) {
      drawitem_t* ds = (drawitem_t*)lParam;
      COLOR clrBk = rgb(1, 1, 1);
      COLOR clrText = rgb(0, 0, 0);
      IRECT rcTxt = ds->itemrc;

      if (ds->state & ODS_SELECTED) {
        clrBk = _RGB(0, 0, 255);
        clrText = rgb(1, 1, 1);
      }

      imdraw_rect(ds->im, ds->cliprc, ds->itemrc, clrBk, 0, 0, 0);
      imdraw_text(ds->im, ds->cliprc, rcTxt, s->sv_log->v[wParam].s, -1, c->fo, clrText, 0, TT_VCENTER);
    }

    if (sender == s->lb_reg->c) {
      drawitem_t* ds = (drawitem_t*)lParam;
      COLOR clrBk = rgb(1, 1, 1);
      COLOR clrText = rgb(0, 0, 0);
      IRECT rcTxt = ds->itemrc;
      IRECT rcImg;
      char buf[256];
      DEV_USER* dev_user = s->user->m_pUserArray + wParam;
      DEV_DATETIME* time1 = &dev_user->m_RegistTime;

      if (ds->state & ODS_SELECTED) {
        clrBk = _RGB(0, 0, 255);
        clrText = rgb(1, 1, 1);
      }

      iRectCutL(&rcTxt, 48, 0, &rcImg);
      {
        img_t imreg[1] = {0};
        //printf("lb_reg_sel%d\n", lb_reg_sel);
        imload_mem(dev_user->m_PicData, dev_user->m_PicLen, 0, imreg);
        //imdraw_image(im, cliprc, &rcImg, im2, 0);
        //{ int ii=0;          _snprintf(buf, 266, "adf%d.jpg", ii++);          imwrite2(buf, imreg);    }
        imcolorcvt(imreg, imreg, T_RGB, T_BGR);
        //imdraw_rect(ds->im, ds->cliprc, rcImg, clrBk, 0, 0, 0);
        imdraw_image(ds->im, ds->cliprc, &rcImg, imreg, 0);
        imfree(imreg);
      }
      imdraw_rect(ds->im, ds->cliprc, rcTxt, clrBk, 0, 0, 0);
      //iRectCutT(&rcTxt, RCH(&ds->itemrc)/2, 0, &rcTxt1);
      _snprintf(buf, 256, "用户ID=%s 用户名=%s 用户类型=%d\n注册时间%d-%d-%d", dev_user->m_UserID.m_ID, dev_user->m_UserName.m_ID, dev_user->m_UserType,
          time1->m_Date.m_Year, time1->m_Date.m_Month, time1->m_Date.m_Day//, time1->m_Time.m_Hour, time1->m_Time.m_Minute, time1->m_Time.m_Second, time1->m_Time.m_Minute
               );
      imdraw_text(ds->im, ds->cliprc, rcTxt, buf, -1, c->fo, clrText, 0, TT_VCENTER);
    }

    if (sender == s->lb_rec->c) {
      drawitem_t* ds = (drawitem_t*)lParam;
      COLOR clrBk = rgb(1, 1, 1);
      COLOR clrText = rgb(0, 0, 0);
      IRECT rcTxt = ds->itemrc;
      IRECT rcImg;
      char buf[256];
      DEV_RECORD* dev_record = s->record->m_pRecordArray + wParam;
      DEV_DATETIME* time1 = &dev_record->m_RecTime;

      if (ds->state & ODS_SELECTED) {
        clrBk = _RGB(0, 0, 255);
        clrText = rgb(1, 1, 1);
      }

      iRectCutL(&rcTxt, 48, 0, &rcImg);

      if (dev_record->m_PicLen > 0) {
        img_t imreg[1] = {0};
        //printf("lb_reg_sel%d\n", lb_reg_sel);
        imload_mem(dev_record->m_PicData, dev_record->m_PicLen, 0, imreg);
        //imdraw_image(im, cliprc, &rcImg, im2, 0);
        //{ int ii=0;          _snprintf(buf, 266, "adf%d.jpg", ii++);          imwrite2(buf, imreg);    }
        imcolorcvt(imreg, imreg, T_RGB, T_BGR);
        //imdraw_rect(ds->im, ds->cliprc, rcImg, clrBk, 0, 0, 0);
        imdraw_image(ds->im, ds->cliprc, &rcImg, imreg, 0);
        imfree(imreg);
      }
      else {
        imdraw_rect(ds->im, ds->cliprc, rcImg, clrBk, 0, 0, 0);
      }

      imdraw_rect(ds->im, ds->cliprc, rcTxt, clrBk, 0, 0, 0);
      //iRectCutT(&rcTxt, RCH(&ds->itemrc)/2, 0, &rcTxt1);
      _snprintf(buf, 256, "用户ID=%s 用户名=%s 记录类型=%d\n记录时间%d-%d-%d", dev_record->m_UserID.m_ID, dev_record->m_UserName.m_ID, dev_record->m_RecType,
          time1->m_Date.m_Year, time1->m_Date.m_Month, time1->m_Date.m_Day//, time1->m_Time.m_Hour, time1->m_Time.m_Minute, time1->m_Time.m_Second, time1->m_Time.m_Minute
               );
      imdraw_text(ds->im, ds->cliprc, rcTxt, buf, -1, c->fo, clrText, 0, TT_VCENTER);
    }

    break;

  case WM_PAINT:
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      int lb_reg_sel = listbox_getcursel(s->lb_reg->c);
      //{static int aaa=0;  printf("WM_PAINT %d\n", aaa++);}
      imdraw_fill(im, _GetSysColor(CLR_3DFACE));

      if (0 && lb_reg_sel >= 0) {
        img_t imreg[1] = {0};
        DEV_USER* dev_user = s->user->m_pUserArray + lb_reg_sel;
        //printf("lb_reg_sel%d\n", lb_reg_sel);
        imload_mem(dev_user->m_PicData, dev_user->m_PicLen, 0, imreg);
        //imdraw_image(im, cliprc, &rcImg, im2, 0);
        imdraw_image(im, &s->nul0->c->rc, &s->nul0->c->rc, imreg, 0);
        imfree(imreg);
      }

      c->redraw = 0;

      if (sub) {
        sub->redraw = 1;
      }

      //printf("WM_PAINT\n");
    }

    if (sub && sub->ctrlproc) {
      sub->ctrlproc(c, sub, hwnd, uMsg, wParam, lParam);
    }

    break;
  }

  return 0;
}

int test_pmdevui()
{
  sys_t sys[1] = {0};
  test_pmdevui_t test_pmdevui[1] = {0};
  win_set(sys, test_pmdevui_proc, test_pmdevui, test_pmdevui->c, 0, "test_pmdevui", WT_WINDOWS, 0, 0, 0);

  for (;;) {
    int ch = waitkey(sys, -1);

    //printf("--%08x\n", &ch);
    if (ch == -2) {
      break;
    }
  }

  return 0;
}



