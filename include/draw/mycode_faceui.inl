{
  SYSIO;
  static ctrl_t cc[10];
  IRECT rc = iRECT(0, 0, out_w, out_h);
  int i = 0;
  static texture_t tex[1] = {0};
  static int frame = 0;
  static iscappic = 1;
  static int savecnt = 0;
  GET_MSG();
  flexboxlay(rc, countof(cc), cc, 0, flex_column | flex_wrap);
  softgc_clear(sg, 0);
  //gcSolidRect2(g, iRECT2(100, 100, 100, 100), _rgb(1,0,0));
  snprintf(buf, 256, "矩形框[%d %d %d %d]\n%d id=%s 得分:%d\n已采集:%d个人\nmsg=%d x=%d y=%d\n采集=%d frame=%d time=%dms",
      xywh[0], xywh[1], xywh[2], xywh[3], pid, featlib[pid].userid, pdis, mennum, io->msg, x, y, savecnt, frame++, (int)((t2 - t1)));
  label_ctrl(cc + i++, buf);
  //drv->SetTexture(drv, tex, cn2PixFmt(cn), h, w, img, step);
  BMPINIT(tex, h, w, (uchar*)img, step, cn * 8);
  {
    static ctrl_t cc1[5] = {0};
    int i1 = 0;
    static int saveid = 0;
    cc[i].sz = iSIZE(100, 40);
    flexboxlay(cc[i].rc, countof(cc1), cc1, 0, flex_row);
    if (button_ctrl(cc1 + i1++, "用户")) {
      showmode = 1;
    }
    if (button_ctrl(cc1 + i1++, "记录")) {
      showmode = 2;
    }
    if (button_ctrl(cc1 + i1++, picmode ? "视频" : "图片")) {
      showmode = 0;
      picmode = !picmode;
      //WT588D_cmd_Play(1);
    }
    snprintf(buf, 256, "注册id=%d", curid);
    if (button_ctrl(cc1 + i1++, buf)) {
      mode = 1;
    }
    ++i;
    if (button_ctrl(cc1 + i1++, iscappic ? "停止" : "采集")) {
      iscappic = !iscappic;
      if (iscappic) {
        saveid++;
      }
    }
    if (iscappic) {
      int picbuflen = h * w * cn;
      uchar* picbuf = (uchar*)pmalloc(picbuflen);
      const char* ext = "jpg";
      int len = imsave_mem(picbuf, picbuflen, ext, h, w, (uchar*)img, step, cn);
      snprintf(buf, 256, "%s/pic_%d_%d.%s", ROOTPATH, saveid, savecnt++, ext);
      am4373_savefile(buf, picbuf, len);
      pfree(picbuf);
    }
  }
#if 1
  // 注册
  progress_ctrl(cc + i++, cur_pic_count * 1. / MAXPIC);
  if (0 == showmode) {
    IRECT rcimg, rcface;
    double ss;
    int j;
    cc[i].sz = iSIZE(120, 160);
    cc[i].rc.b = rc.b;
    ss = iRectScaling(cc[i].rc, tex->w, tex->h, &rcimg);
    gcRectImageR(g, rcimg, tex, NULL);
    for (j = 0; j < 3; ++j) {
      rcface = iRECT2(rcimg.l + xywh[j * 4 + 0] * ss, rcimg.t + xywh[j * 4 + 1] * ss, xywh[j * 4 + 2] * ss, xywh[j * 4 + 3] * ss);
      gcEdgeRect1(g, rcface, 0, _rgb(0, 1, 0), 1);
    }
    ++i;
  }
  else if (1 == showmode) {
    vstr_t sv[1] = {0};
    int j;
    static int sel = 0, hotitem = 0;
    static IPOINT pos = {0};
    static const char* strs[] = {"用户列表"};
    static int width[1] = {310};
    strv_setsize(sv, mennum);
    for (j = 0; j < mennum; ++j) {
      str_setstr(sv->v + j, featlib[j * MAXPIC].userid, -1);
    }
    //listheader(cc + i++, 1, strs, width);
    cc[i].rc.b = rc.b;
    strlist_ctrl(cc + i++, sv, 1, width, &pos, &sel, &hotitem);
    strv_free(sv);
  }
  else if (2 == showmode) {
    static int sel = 0, hotitem = 0;
    static IPOINT pos = {0};
    static const char* strs[] = {"记录列表"};
    static int width[1] = {310};
    //listheader(cc + i++, 1, strs, width);
    cc[i].rc.b = rc.b;
    strlist_ctrl(cc + i++, rec, 1, width, &pos, &sel, &hotitem);
  }
  host_end(host, 45);
#endif
}

