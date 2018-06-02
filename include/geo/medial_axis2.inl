int medial_axis(HDDC hDC, int n, const DPOINT* pt, DPOINT* out)
{
  inter_info_t ii[1] = {0};
  int i, j, k, m = 0;
  DPOINT bi[1000]; // 平分线
  DPOINT bk[1000]; // 平分线交点
  DPOINT bpt;
  double dis[1000];
  double mindis = MAX_DOUBLE;
  BOOL cox[1000];
  int mini = -1;
  if (!issimple(n, pt)) {
    //return 0;
  }
  if (!isconvex(n, pt, cox)) {
    //return 0;
  }
  // 计算各顶点的角平分线
  for (i = 0; i < n; ++i) {
    DPOINT d;
    char buf[256];
    _snprintf(buf, 256, "%d", i);
    draw_textPt(hDC, buf, -1, (int)pt[i].x, (int)pt[i].y, 0, _RGB(0, 0, 0));
    j = (i + n - 1) % n;
    k = (i + 1) % n;
    pt_slope(&pt[k].x, &pt[i].x, &pt[j].x, &d.x);
    bi[i] = d;
    //draw_line(hDC, pt[i].x, pt[i].y, d.x, d.y, _RGB(0, 0, 255));
  }
  //求相邻平分线的交点，以及交点到边的距离
  for (i = 0; i < n; ++i) {
    k = (i + 1) % n;
    line_inter(&pt[i].x, &bi[i].x, &pt[k].x, &bi[k].x, ii);
    bk[i] = ii->pt;
    pt_pedal(&pt[i].x, &pt[k].x, &bk[i].x, 0, dis + i, 0);
    if (cox[i] && dis[i] < mindis) {
      mindis = dis[i];
      mini = i;
    }
  }
  bpt = bk[mini];
  out[m++] = bpt;
  //从最小距离处开始
  if (1) {
    for (j = mini, k = mini + 1; k != j;) {
      DPOINT d0, d1, dj, dk;
      inter_info_t jii[1] = {0};
      inter_info_t kii[1] = {0};
      int jj = (j + n - 1) % n;
      int kk = (k + 1) % n;
      if (kk == jj) {
        break;
      }
      pt_slope2(&pt[j].x, &pt[jj].x, &pt[k].x, &pt[kk].x, &d0.x, &d1.x);
      drawaa_ellipse(hDC, d0.x, d0.y, 5, 5, _RGBA(0, 0, 255, 150), 0, 0);
      line_inter(&bpt.x, &d0.x, &pt[jj].x, &bi[jj].x, jii);
      dj = jii->pt;
      line_inter(&bpt.x, &d0.x, &pt[kk].x, &bi[kk].x, kii);
      dk = jii->pt;
      if (fabs(jii->ts[0]) < fabs(kii->ts[0])) {
        draw_line(hDC, bpt.x, bpt.y, dj.x, dj.y, _RGB(0, 255, 0));
        //draw_line(hDC, pt[jj].x, pt[jj].y, dj.x, dj.y, _RGB(255, 0, 0));
        bpt = dj, j = jj;
      }
      else {
        draw_line(hDC, bpt.x, bpt.y, dk.x, dk.y, _RGB(0, 255, 0));
        //draw_line(hDC, pt[kk].x, pt[kk].y, dk.x, dk.y, _RGB(255, 0, 0));
        bpt = dk, k = kk;
      }
      out[m++] = bpt;
    }
  }
  else {
    for (j = mini, k = mini + 1; k != j;) {
      DPOINT d0, d1, dj, dk;
      DPOINT dj0, dj1;
      DPOINT dk0, dk1;
      inter_info_t jii[1] = {0};
      inter_info_t kii[1] = {0};
      int jj = (j + n - 1) % n;
      int jjj = (jj + n - 1) % n;
      int kk = (k + 1) % n;
      int kkk = (kk + 1) % n;
      if (kk == jj) {
        break;
      }
      pt_slope2(&pt[j].x, &pt[jj].x, &pt[k].x, &pt[kk].x, &d0.x, &d1.x);
      pt_slope2(&pt[j].x, &pt[jj].x, &pt[kk].x, &pt[kkk].x, &dk0.x, &dk1.x);
      pt_slope2(&pt[jj].x, &pt[jjj].x, &pt[k].x, &pt[kk].x, &dj0.x, &dj1.x);
      line_inter(&bpt.x, &d1.x, &dj0.x, &dj1.x, jii);
      dj = jii->pt;
      line_inter(&bpt.x, &d1.x, &dk0.x, &dk1.x, kii);
      dk = kii->pt;
      if (jii->ts[0] < kii->ts[0]) {
        bpt = dj, j = jj;
      }
      else {
        bpt = dk, k = kk;
      }
      out[m++] = bpt;
    }
  }
  for (i = 0; i < m; ++i) {
    k = (i + 1) % n;
    drawaa_ellipse(hDC, out[i].x, out[i].y, 5, 5, _RGBA(0, 255, 0, 150), 0, 0);
    if (i < m - 1) {
      //draw_line(hDC, out[i].x, out[i].y, out[k].x, out[k].y, _RGB(255, 0, 0));
    }
  }
  for (i = 0; i < n; ++i) {
    DPOINT pt0;
    j = (i + n - 1) % n;
    k = (i + 1) % n;
    // if ()
    pt0 = (dis[i] > dis[j]) ? bk[j] : bk[i];
    //drawaa_ellipse(hDC, pt0.x, pt0.y, 5, 5, _RGB(0, 255, 0), 0);
  }
  return m;
}

