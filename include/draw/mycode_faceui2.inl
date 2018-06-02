{
  SYSIO;
  static ctrl_t cc[10];
  IRECT rc = iRECT(0, 40, out_w, out_h - 80);
  int i = 0, j = 0;
  static int frame = 0;
  static iscappic = 1;
  static int savecnt = 0;
  static int loging = 0;
  texture_t tex[1] = {0};
  int gap = 0;
  GET_MSG();
  softgc_clear(sg, 0);
  //gcSolidRect2(g, iRECT2(100, 100, 100, 100), _rgb(1,0,0));
  //drv->SetTexture(drv, tex, cn2PixFmt(cn), h, w, img, step);
  {
    IRECT rcimg = rc, rc1;
    double sx, sy;
    BMPINIT(tex, h, w, (uchar*)img, step, cn * 8);
    gcRectImageR(g, rcimg, tex, NULL);
    sx = RCW(&rcimg) * 1. / tex->w;
    sy = RCH(&rcimg) * 1. / tex->h;
    for (j = 0; j < 3; ++j) {
      rc1 = iRECT2(rcimg.l + xywh[j * 4 + 0] * sx, rcimg.t + xywh[j * 4 + 1] * sy, xywh[j * 4 + 2] * sx, xywh[j * 4 + 3] * sy);
      gcEdgeRect1(g, rc1, 0, _rgba(0, 1, 0, 0.7), 2);
    }
  }
  i = 0;
  if (ui_recog == uitype) {
    ctrl_t* manage_btn = cc + i++;
    int thd = 460;
    BMPINIT(tex, 60, 60, (uchar*)manage_60x60x4, 60 * 4, 4 * 8);
    manage_btn->rc = iRECT(rc.l, rc.t + RCH(&rc) * 4 / 5, rc.r, rc.b);
    switch (io->msg) {
    case MSG_LBUTTONDOWN: {
      IRECT rc1 = iRECT(rc.l, 0, rc.r, rc.t + RCH(&rc) * 4 / 5);
      if (iPtInRect(&rc1, x, y)) {
        picmode = !picmode;
      }
    }
    break;
    }
    if (image_button_ctrl(manage_btn, "管理员登陆", _rgb(1, 0, 0), tex, flex_row | flex_center)) {
      printf("%s\n", "管理员登陆");
      loging = 1;
    }
    {
      IRECT rcTxt = iRECT2(rc.l, rc.t, RCW(&rc), 80);
      if (facedata.mennum > 0) {
        if (pid >= 0 && pdis < thd) {
          gcSolidRect2(g, rcTxt, _rgba(0, 1, 0, 0.5));
          snprintf(buf, 256, "识别%s id:%d(%s) 得分:%d", pdis < thd ? "成功" : "失败", pid, facedata.featlib[pid].userid, pdis);
          gcSolidText(g, rcTxt, buf, NULL, io->font, 0, TF_VCENTER | TF_CENTER, _rgba(1, 1, 1, 0.9));
        }
      }
      else {
        loging_ok = 1;
        gcSolidRect2(g, rcTxt, _rgba(0, 1, 0, 0.5));
        snprintf(buf, 256, "还没注册任何人脸!!!最多支持%d人", MAXMEN);
        gcSolidText(g, rcTxt, buf, NULL, io->font, 0, TF_VCENTER | TF_CENTER, _rgba(1, 1, 1, 0.9));
      }
    }
    if (pdis < thd) {
      if (loging) {// 登陆成功
        uitype = ui_menu;
        loging = 0;
      }
    }
  }
  else {
    switch (uitype) {
    case ui_menu: {
      IRECT prc[4];
      ui_type menu[] = {ui_record, ui_seting, ui_userreg, ui_manage};
      const char* btnstr[] = {"记录查询", "基本设置", "用户登记", "用户管理"};
      uchar* texdata[] = {record_60x60x4, seting_60x60x4, userreg_60x60x4, manage_60x60x4};
      COLOR clrs[4] = {_RGB(40, 200, 90), _RGB(180, 40, 210), _RGB(50, 160, 200), _RGB(10, 210, 270)};
      iRectCutB(&rc, -60, gap, &cc[4].rc);
      BMPINIT(tex, 45, 45, (uchar*)return_45x45x4, 45 * 4, 4 * 8);
      if (image_button_ctrl(cc + 4, "返回", _rgb(0.3, 0.3, 0.3), tex, flex_row | flex_center)) {
        printf("%s\n", "返回");
        loging_ok = 0;
        uitype = ui_recog;
      }
      iRectMatrix(rc, 2, 2, gap, gap, TF_CENTER | TF_VCENTER, 4, prc);
      for (i = 0; i < 4; ++i) {
        BMPINIT(tex, 60, 60, (uchar*)texdata[i], 60 * 4, 4 * 8);
        cc[i].rc = prc[i];
        if (image_button_ctrl(cc + i, btnstr[i], clrs[i], tex, flex_column | flex_center)) {
          printf("%s\n", btnstr[i]);
          uitype = menu[i];
        }
      }
    }
    break;
    case ui_record: { // 记录查询
      iRectCutB(&rc, -60, gap, &cc[4].rc);
      BMPINIT(tex, 45, 45, (uchar*)return_45x45x4, 45 * 4, 4 * 8);
      if (image_button_ctrl(cc + 4, "返回", _rgb(0.3, 0.3, 0.3), tex, flex_row | flex_center)) {
        printf("%s\n", "返回");
        uitype = ui_menu;
      }
    }
    break;
    case ui_seting: { // 基本设置
      iRectCutB(&rc, -60, gap, &cc[4].rc);
      BMPINIT(tex, 45, 45, (uchar*)return_45x45x4, 45 * 4, 4 * 8);
      if (image_button_ctrl(cc + 4, "返回", _rgb(0.3, 0.3, 0.3), tex, flex_row | flex_center)) {
        printf("%s\n", "返回");
        uitype = ui_menu;
      }
    }
    break;
    case ui_userreg: { //用户登记
      iRectCutB(&rc, -60, gap, &cc[4].rc);
      BMPINIT(tex, 45, 45, (uchar*)return_45x45x4, 45 * 4, 4 * 8);
      if (image_button_ctrl(cc + 4, "返回", _rgb(0.3, 0.3, 0.3), tex, flex_row | flex_center)) {
        printf("%s\n", "返回");
        if (uitype == ui_menu) {
          loging_ok = 0;
        }
        cur_pic_count = 0;
        mode = 2;
        uitype = ui_menu;
      }
      mode = 1;
      cc->rc = iRECT2(rc.l, rc.t, RCW(&rc), 30);
      snprintf(buf, 256, "%d", cur_pic_count);
      progress_ctrl(cc, buf, cur_pic_count * 1.f / MAXPIC);
    }
    break;
    case ui_manage: { //用户管理
      enum {item_per_page = 5};
      IRECT rcs[item_per_page] = {0}, rcTop;
      int i, j;
      static int iPage = 0;
      iRectCutB(&rc, -60, gap, &cc[4].rc);
      BMPINIT(tex, 45, 45, (uchar*)return_45x45x4, 45 * 4, 4 * 8);
      if (image_button_ctrl(cc + 4, "返回", _rgb(0.3, 0.3, 0.3), tex, flex_row | flex_center)) {
        printf("%s\n", "返回");
        uitype = ui_menu;
      }
      iPage = BOUND(iPage, 0, facedata.mennum / item_per_page);
      iRectCutT(&rc, 30, 0, &rcTop);
      gcSolidRect2(g, rcTop, _rgba(0, 0, 1, 0.5));
      snprintf(buf, 256, "用户列表(%d/%d)", iPage + 1, 1 + facedata.mennum / item_per_page);
      gcSolidText(g, rcTop, buf, NULL, io->font, 0, TF_VCENTER | TF_CENTER, _rgba(1, 1, 1, 0.9));
      iRectMatrix(rc, item_per_page, 1, 0, 0, 0, item_per_page, rcs);
      for (i = 0; i < item_per_page; ++i) {
        j = iPage * item_per_page + i;
        if (i >= 0 && j < facedata.mennum) {
          IRECT rc1 = rcs[i], rc2, rc3;
          gcEdgeRect1(g, rc1, _rgba(1, 1, 0, 0.5), _rgba(0, 1, 0, 0.7), 2);
          BMPINIT(tex, 100, 100, (uchar*)facedata.men[j].stdface, 100, 8);
          iRectCutL(&rc1, RCH(&rc1), 0, &rc2);
          iRectAlign(rc2, RCH(&rc1) - 4, RCH(&rc1) - 4, TF_VCENTER | TF_CENTER, &rc3);
          gcRectImageR(g, rc3, tex, NULL);
          snprintf(buf, 256, "%s", facedata.featlib[j * MAXPIC].userid);
          gcSolidText(g, rc1, buf, NULL, io->font, 0, TF_VCENTER | TF_RIGHT, _rgba(1, 1, 1, 0.9));
        }
      }
    }
    break;
    }
  }
  if (0) {
    IRECT rcTxt = iRECT2(rc.l, rc.t, RCW(&rc), 80);
    snprintf(buf, 256, "debug: rect[%d %d %d %d]\n%d id=%s 得分:%d\n已采集:%d个人\nmsg=%d x=%d y=%d\n采集=%d frame=%d time=%dms",
        xywh[0], xywh[1], xywh[2], xywh[3], pid, facedata.featlib[pid].userid,
        pdis, facedata.mennum, io->msg, x, y, savecnt, frame++, (int)((t2 - t1)));
    gcSolidRect2(g, rcTxt, _rgba(0, 0, 0, 0.5));
    gcSolidText(g, rcTxt, buf, NULL, io->font, 0, TF_VCENTER | TF_LEFT, _rgba(1, 1, 1, 0.9));
  }
}

