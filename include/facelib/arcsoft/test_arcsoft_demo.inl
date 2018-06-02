
#define ROOTPATH "./"
#define DATAPATH "./facedata.dat"
#include "cstd.h"
#include "cfile.h"
#include "cap.h"
#include "facelib/arcsoft/arcsoft_facerecong.inl"
#include "face/face_recog.inl"
#include "cap/capimgseq.inl"
#include "imgui/imgui.inl"
//#include "draw/imdrawaa.inl"
//#include "draw/imdrawbin.inl"
#include "face/A20/show_cnsun_logo.inl"

int test_facerecog_savedata(const facerecogapp_t* app, const char* arg) {
  //savefile(DATAPATH, &app->feat, sizeof(facedata));
  //_snprintf(buf, 256, "%s.dat", curfeat.m_PID.m_ID);
  //savefile(buf, &app->feat[featnum], app->maxpic*sizeof(curfeat));
  return 0;
}
int test_facerecog_loaddata(facerecogapp_t* app, const char* arg) {
  return 0;
}
static facerecogapp_t app[1] = {0};
static char oubbuf[256] = {0};
sys_serial_port_t g_ss[1] = {0};

int test_facerecog_msg_default(const facerecogapp_t* app, const char* arg) {
  static int i = 0;
  if (g_ss->x) {
    printf("%d : %s\n", i, arg);
    strncpy(oubbuf, arg, sizeof(oubbuf));
    char buf[1024];
    _snprintf(buf,  countof(buf), "%d : %s\n", i, arg);
    int len = strlen(buf);
    sys_serial_port_send(g_ss, buf, len);
    ++i;
  }
  return 0;
}

int my_recv_message(sys_serial_port_t* ss, char* buf, int len, UINT Msg) {
  buf[len] = 0;
  printf("%s\n", buf);
  facerecogapp_cmd(app, buf);
  return 0;
}

int test_facerecogapp_loadsave() {
  facerecogapp_t app[1] = {0};
  int i, j=0, menid, cur_feat_num;
  sys_chdir("D:/pub/bin/face");
  //remove("id4.dat");
  //remove("*.dat");
  facerecogapp_loadall(app);
  if (0) {
    char* ff = facerecogapp_getmenfeatstr(app, "yw002");
    facerecogapp_setmenfeatstr(app, "yw004", ff, false);
    //savefile("aaa.txt", ff, strlen(ff));
    char* feattxt = facerecogapp_loadfeattxt("D:/pub/bin/face/down/yw004.txt");
    facerecogapp_cmd(app, feattxt);
  }
  for (i=0; i<app->men_num; ++i) {
    char* name = app->men[i].m_PID.m_ID;
    int num = getmenfeatnum(app, name);
    printf("%s %d\n", name, num);
  }
  srand(time(0));
  for (i=0; i<100; ++i) {
  char name[256] = {0};
    _snprintf(name, 256, "id%d", i);
    _snprintf(app->cur_feat.m_PID.m_ID, ID_LEN, "%s", name);
    _snprintf(app->cur_men.m_PID.m_ID, ID_LEN, "%s", name);
    cur_feat_num = app->feat_num+10;
    if (cur_feat_num>app->max_feat_num) {
      int max_feat_num = cur_feat_num*3/2+20;
      MYREALLOC(app->feat, max_feat_num);
      app->max_feat_num = max_feat_num;
    }
    menid = find_men(app, app->cur_men.m_PID.m_ID);
    if (menid<0) {
      menid = app->men_num;
      ++app->men_num;
    }
    if (menid >= app->max_men_num) {
      int max_men_num = app->men_num*3/2 + 20;
      MYREALLOC(app->men, max_men_num);
      app->max_men_num = max_men_num;
    }
    app->men[menid] = app->cur_men;
    for (j=0; j<10; ++j) {
      int k=0;
      for (k=0; k<FEATURE_LEN; ++k) {
        app->cur_feat.m_Feature[k] = rand()%20;
      }
      app->feat[app->feat_num + j] = app->cur_feat;
    }
    app->feat_num = cur_feat_num;
    if (0) {
      char* list = facerecogapp_getlist(app, 0);
      printf("%s\n", list);
      FREE(list);
    }
    if (0) {
      char* str = facerecogapp_getmenfeatstr(app, name);
      char* str2 = cstr_vsprintf2("%s<%s>", name, str);
      printf("%s\n", str2);
      FREE(str2);
      FREE(str);
    }
    facerecogapp_savemen(app, name);
    if (1&rand()) {
      facerecogapp_delmen(app, name);
    }
  }
  return 0;
}

#define USE_SERIAL
int test_facerecogctrl(ctrl_t* c) {
  SYSIO;
  static vstr_t rec[1] = {0};
  static int curid = 0;
  char buf[256] = {0};
  int ret = 0;
  static int inited = 0;
  static cap_t cap[2] = {0};
  cap_t* caprgb =  cap + 1;
  static img_t im[10] = {0};
  img_t* imrgb = im + 1;
  static int64 _start_time = 0;
  static double time2=0;
  static double timedelims = 3; //认证时间间隔
  static int frame = 0;
  static char username[256] = "";
  if (!inited) {
#if 1
    inited = capdshow_open(cap, 0, 640, 480);
    //inited = capdshow_open(caprgb, 0, 640, 480);
#else
    inited = capimgseq_open(cap, "D:/facedata/list2.txt");
    inited = capimgseq_open(caprgb, "D:/facedata/list2.txt");
#endif
    facerecogapp_cmd(app, NULL);
    app->msg = test_facerecog_msg_default;
    app->mode = Recog1N;
    facerecogapp_loadall(app);
    sys_serial_port_open(g_ss, 2, 9600, 'N', 8, 1);
    g_ss->recv_message = my_recv_message;
    g_ss->user = app;
  }
  if (!inited) {
    return 0;
  }
  FORCE_REDRAW(c->rc);
  if (MSG_TIMER==io->msg) {
    int t = 0;
    const char* s_pixtype = "GRAY";
    ++frame;
    if (1) {
      cap_getframe(cap, im, T_BGR);
      s_pixtype = "RGB";
      //imswap_rb(imrgb);
      app->trans = 0;
      imtrans2(im, im, app->trans, 0);
      if (caprgb->x) {
        cap_getframe(caprgb, imrgb, T_BGR);
      } else {
        imclone2(im, imrgb);
      }
    }
    facerecogapp_frame(app, im->h, im->w, im->tt.data, im->s, im->c, s_pixtype, imrgb->tt.data, imrgb->s);
    //imdrawaa_text(im, 0, NULL, "adsf", c->rc, _rgb(1,0,0), 0, NULL);
  }
  
  //facerecogapp_cmd(app, "DEL_ID=yw\n \n");
  {
    static ctrl_t cc[10] = {0};
    int i = 0;
    static texture_t tex[1] = {0};
    static iscappic = 1;
    static int savecnt = 0;
    IRECT rcRight;
    IRECT rc;
    color_background_ctrl(c->rc, _RGB(222, 222, 222), _RGB(22, 22, 22), 1, 1);
    iRectCutR(&c->rc, -300, 2, &rcRight);
    rc = c->rc;
    flexboxlay(rc, countof(cc), cc, 0, flex_column | flex_wrap);
    //gcSolidRect2(g, iRECT2(100, 100, 100, 100), _rgb(1,0,0));
    if (1) {
      int i1 = 0;
      static ctrl_t cc1[40];
      int* xywh = app->xywh;
      flexboxlay(rcRight, countof(cc1), cc1, 0, flex_column | flex_wrap);
      //snprintf(buf, 256, "人脸框[x=%d y=%d w=%d h=%d]\n%d id=%s 得分:%d(%s)\n已登记:%d个人\nframe=%d time=%dms\n注意: 最多登记%d人, 认证时间间隔(%.1g秒)",
      //  xywh[0], xywh[1], xywh[2], xywh[3], pid, facedata.featlib[pid].m_PID.m_ID, pdis, reg_ok ? "认证成功" : "认证失败",
      //   facedata.men_num, frame, (int)(time2/frame), MAXMEN, timedelims);
      fmtlabel_ctrl(cc1 + i1++, "人脸框[x=%d y=%d w=%d h=%d]", xywh[0], xywh[1], xywh[2], xywh[3]);
      fmtlabel_ctrl(cc1 + i1++, "cur_pic_count=%d", app->cur_pic_count);
      fmtlabel_ctrl(cc1 + i1++, "max_cap_pic=%d", app->max_cap_pic);
      fmtlabel_ctrl(cc1 + i1++, "mode=%d", app->mode);
      fmtlabel_ctrl(cc1 + i1++, "feat_num=%d", app->feat_num);
      fmtlabel_ctrl(cc1 + i1++, "men_num=%d", app->men_num);
      fmtlabel_ctrl(cc1 + i1++, "采集累计时间=%g", app->cap_time_used);
      fmtlabel_ctrl(cc1 + i1++, "cap_timeout=%g", app->cap_timeout);
      fmtlabel_ctrl(cc1 + i1++, "oubbuf=%s", oubbuf);
      fmtlabel_ctrl(cc1 + i1++, "提示：注册图片保存在out目录");
      //fmtlabel_ctrl(cc1 + i1++, "id=%s 得分:%d(%s)", pid, facedata.featlib[pid].m_PID.m_ID, pdis);
    }
    //drv->SetTexture(drv, tex, cn2PixFmt(cn), h, w, img, step);
    
    if (1) {
      static ctrl_t cc1[10] = {0};
      int i1 = 0;
      static int saveid = 0;
      cc[i].sz = iSIZE(100, 40);
      flexboxlay(cc[i].rc, countof(cc1), cc1, 0, flex_row);
      if (button_ctrl(cc1 + i1++, "清空用户")) {
        facerecogapp_cmd(app, "");
      }
      if (button_ctrl(cc1 + i1++, "清空记录")) {
        rec->n = 0;
      }
      //if (button_ctrl(cc1 + i1++, "用户")) {        showmode = 1;      }
      //if (button_ctrl(cc1 + i1++, "记录")) {        showmode = 2;      }
      //if (button_ctrl(cc1 + i1++, picmode ? "视频" : "图片")) {        showmode = 0;        picmode = !picmode;      }
      static int curid = 0;
      if (*username) {
        snprintf(buf, countof(buf), "ADD_ID=%s", username);
      } else {
        snprintf(buf, countof(buf), "ADD_ID=id_%d", curid);
      }
      if (app->mode==2) {
        if (button_ctrl(cc1 + i1++, "停止认证")) {
          facerecogapp_cmd(app, "IDENT_OFF=1");
        }
      } else {
        if (button_ctrl(cc1 + i1++, "开始认证")) {
          facerecogapp_cmd(app, "IDENT_ON=1");
        }
      }
      if (app->mode==1) {
        if (app->cap_pause) {
          if (button_ctrl(cc1 + i1++, "恢复采集")) {
            facerecogapp_cmd(app, "CAP_PAUSE=0");
          }
        } else {
          if (button_ctrl(cc1 + i1++, "暂停采集")) {
            facerecogapp_cmd(app, "CAP_PAUSE=1");
          }
        }
      }
      ++i;
      if (0 && button_ctrl(cc1 + i1++, iscappic ? "停止" : "登记")) {
        iscappic = !iscappic;
        if (iscappic) {
          saveid++;
        }
      }
      if (0) {
        int picbuflen = im->h * im->s;
        uchar* picbuf = (uchar*)pmalloc(picbuflen);
        const char* ext = "jpg";
        int len = imsave_mem(picbuf, picbuflen, ext, im->h, im->w, im->tt.data, im->s, im->c);
        snprintf(buf, 256, "%s/pic_%d_%d.%s", ROOTPATH, saveid, savecnt++, ext);
        savefile(buf, picbuf, len);
        pfree(picbuf);
      }
      if (button_ctrl(cc1 + i1++, "  登记  ")) {
        facerecogapp_cmd(app, buf);
        ++curid;
      }
      edit_ctrl(cc1 + i1++, "username", username, countof(username));
    }
    
    // 登记
    if (1) {
      if (1) {
        vstr_t sv[1] = {0};
        int j;
        static int sel = 0, hotitem = 0;
        static IPOINT pos = {0};
        static const char* strs[] = {"用户列表"};
        static int width[1] = {310};
        static ctrl_t cc1[2] = {0};
        ctrl_t* cc1_ = cc1;
        ctrl_t* cc2_ = cc1+1;
        IRECT rc1 = cc[i].rc, rc2;
        static img_t regim[1] = {0};
        iRectCutL(&rc1, 100, 0, &rc2);
        //curmen.regpic, REGW*3
        vstr_setsize(sv, app->men_num);
        flexboxlay(rc1, countof(cc1), cc1, 0, flex_column);
        for (j = 0; j < app->men_num; ++j) {
          str_setstr(sv->v + j, app->men[j].m_PID.m_ID, -1);
        }
        listheader(cc1, 1, strs, width);
        //cc[i].rc.b = rc.b;
        cc1[1].rc.b = rc1.b;
        if (strlist_ctrl(cc1+1, sv, 1, width, &pos, &sel, &hotitem)) {
          if (sel>=0 && sel<app->men_num) {
            imload_mem(app->men[sel].picdata, app->men[sel].picdatalen, 3, regim);
          }
        }
        cc[i++].sz = iSIZE(cc1[1].sz.w,cc1[1].sz.h+30);
        if (sel>=0 && sel<app->men_num) {
          static ctrl_t cc2[1] = {0};
          cc2->rc = rc2;
          BMPINITIM(tex, regim);
          // BMPINIT(tex, REGH, REGW, app->men[sel].regpic, REGW*3, 3*8);
          image_ctrl(cc2, tex, NULL, NULL, NULL);
          gcRectImageR(g, rc2, tex, NULL);
        } else {
          gcSolidText(g, rc2, "注册照", NULL, io->font, 0, TF_VCENTER | TF_CENTER, ColorBlack);
        }
        vstr_free(sv);
      }
      if (1) {
        static int sel = 0, hotitem = 0;
        static IPOINT pos = {0};
        static const char* strs[] = {"记录列表"};
        static int width[1] = {310};
        listheader(cc + i++, 1, strs, width);
        strlist_ctrl(cc + i++, rec, 1, width, &pos, &sel, &hotitem);
      }
      progress_ctrl(cc + i++, buf, app->cur_pic_count * 1. / app->max_cap_pic);
      if (1) {
        IRECT rcimg, rcface, rc1;
        double ss;
        int j;
        IRECT rcs[2] = {0};
        COLOR rectcolor = app->rec_ok ? _rgb(0,1,0) : _rgb(1,0,0);
        int* xywh = app->xywh;
        cc[i].sz = iSIZE(120, 160);
        cc[i].rc.b = rc.b;
        BMPINITIM(tex, im);
        rc1 = cc[i].rc;
        int col=1;
        //cc[i].rc.b = rc.b;
        iRectMatrix(rc1, 1, col, 0, 0, 0, 2, rcs);
        ss = iRectScaling(rcs[0], tex->w, tex->h, &rcimg);
        //show_cnsun_logo(im);
        gcRectImageR(g, rcimg, tex, NULL);
        if (app->ret) {
          for (j = 0; j < 3; ++j) {
            rc1 = iRECT2(rcimg.l + xywh[j * 4 + 0] * ss, rcimg.t + xywh[j * 4 + 1] * ss, xywh[j * 4 + 2] * ss, xywh[j * 4 + 3] * ss);
            if (0==j) {
              rcface = rc1;
            }
            gcEdgeRect1(g, rc1, 0, rectcolor, 1);
          }
        }
        rc1 = iRECT(rcface.l, rcface.t, rcface.r, rcface.t+20);
        gcEdgeRect1(g, rc1, _rgba(0,0,0,0.5), 0, 0);
        if (app->rec_ok) {
          _snprintf(buf, countof(buf), "%s", app->curid.m_ID);
        } else {
          _snprintf(buf, countof(buf), "%s", "???");
        }
        gcSolidText(g, rc1, buf, NULL, io->font, 0, TF_VCENTER | TF_CENTER, ColorWhite);
        BMPINITIM(tex, imrgb);
        if (col>1) {
          ss = iRectScaling(rcs[1], tex->w, tex->h, &rcimg);
          gcRectImageR(g, rcimg, tex, NULL);
        }
        ++i;
      }
    }
  }
  
  return 0;
}

int test_arcsoft_demo() {
#ifdef _DEBUG
  sys_chdir("D:/pub/bin/face/arcsoft");
#endif // _DEBUG
  return host_run(test_facerecogctrl, 30);
}


