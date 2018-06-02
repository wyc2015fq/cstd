

uchar GetMedianNum(uchar* bArray, int iFilterLen)
{
  int i, j;   // 循环变量
  uchar bTemp;

  // 用冒泡法对数组进行排序
  for (j = 0; j < iFilterLen - 1; j ++) {
    for (i = 0; i < iFilterLen - j - 1; i ++) {
      if (bArray[ i ] > bArray[ i + 1 ]) {
        // 互换
        bTemp = bArray[ i ];
        bArray[ i ] = bArray[ i + 1 ];
        bArray[ i + 1 ] = bTemp;
      }
    }
  }

  // 计算中值
  if ((iFilterLen & 1) > 0) {
    // 数组有奇数个元素，返回中间一个元素
    bTemp = bArray[(iFilterLen + 1) / 2 ];
  }
  else {
    // 数组有偶数个元素，返回中间两个元素平均值
    bTemp = (bArray[ iFilterLen / 2 ] + bArray[ iFilterLen / 2 + 1 ]) / 2;
  }

  return bTemp;
}

int edge_detection_waiyuan(buf_t* bf, const img_t* polarv, int pupil_radius, IPOINT* sample, int p_samplemaxlen)
{
  int i, j, n = 0, ROI_row, ROI_col, r_first, r_last;
  img_t polarv1[1] = {0}, polarv2[1] = {0}, I2[1] = {0}, orient_img[1] = {0};
  img_t I3[1] = {0}, I_edge[1] = {0}, v_edge_y[1] = {0}, edge[1] = {0};
  int h = polarv->h, w = polarv->w;
  int buflen = bf->len;

  if (pupil_radius < 5) {
    return 0;
  }

  //int num_up = 0, num_down = 0;
  {
    //img_t* t, *aa=imread2("004.bmp", 1);
    //CV_SWAP(polarv, aa, t);
    //bf_imfree(bf,aa);
  }
  // 用直方图统计的方法去除误点
  // 边缘检测
  ROI_row = polarv->h, ROI_col = polarv->w;
#if 0
  imclone(polarv, polarv1);

  //imwrite3(polarv);
  // 去除睫毛 对边缘检测的影响
  for (i = 1; i <= ROI_row; ++i) {
    for (j = 4; j <= ROI_col - 14; ++j) {
      uchar* b =  polarv->tt.data + polarv->s * (i - 1) + (j - 4);
      k = nth_element(7, b, 6);
      polarv1->tt.data[polarv1->s * (i - 1) + (j - 1)] = b[k];
    }
  }

  imclone(polarv1, polarv2);

  for (i = 2; i <= ROI_row - 2; ++i) {
    for (j = 2; j <= ROI_col - 2; ++j) {
      int l = polarv1->s;
      uchar dd[16];
      uchar* d = dd, *b =  polarv1->tt.data + polarv1->s * (i - 1) + (j - 1);
      *d++ = b[-l - 1], *d++ = b[-l], *d++ = b[-l + 1], //
      *d++ = b[-1], *d++ = b[0], *d++ = b[+1], //
      *d++ = b[l - 1], *d++ = b[l], *d++ = b[l + 1];
      k = GetMedianNum(dd, 9);
      polarv2->tt.data[polarv2->s * (i - 1) + (j - 1)] = k;
    }
  }

#else
  bf_imsetsize(bf, polarv2, h, w, 1, 1);
  imclone(polarv, polarv2);
#endif

  // 得到的polarv 假如这一行有等于255的点, 则这一行的边缘不在考虑的范围
  for (r_first = 1; r_first <= ROI_row; ++r_first) {
    if (polarv2->tt.data[polarv2->s * (r_first - 1) + ROI_col - 3] == 255) {
      break;
    }
  }

  for (r_last = ROI_row; r_last >= r_first; --r_last) {
    if (polarv2->tt.data[polarv2->s * (r_last - 1) + ROI_col - 3] == 255) {
      break;
    }
  }

  //{utime_start(_start_time);

#if 0
  {
    double scaling = 1.0,    sigma = 2,    vert = 1.0,    horz = 0.0;
    // generate the edge image
    canny(polarv2, sigma, scaling, vert, horz, I2, orient_img);    // edge detection 4-19
  }

  adjgamma(I2, 1.9, I3);       // 将I2 增强 并normalize to [0, 1]  4-19
  nonmaxsup(I3, orient_img, 1.5, I_edge);   // figure,imshow(I_edge,[])
  imsetsize(v_edge_y, I_edge->h, I_edge->w, 1, 1);
  num_up = 0;
  num_down = 0;
  r_first = MAX(r_first, 0);
  r_first = MIN(r_first, ROI_row);
  r_last = MAX(r_last, 0);
  r_last = MIN(r_last, ROI_row);

  for (i = 1; i <= ROI_row; ++i) {
    if (i <= r_first || i >= r_last) {
      int off = 10;
      int pupil_radius_off = pupil_radius + off;
      double* pos = I_edge->tt.f8 + I_edge->w * (i - 1) + pupil_radius_off - 1;
      int n = I_edge->w - off - pupil_radius_off, max_num;
      MAX_ELEMENT(n, pos, max_num);

      if (pos[max_num] > 0.1) { // && polarv(i,ind-2:ind+2) ~= 255
        int ind = max_num + pupil_radius_off;
        v_edge_y->tt.data[v_edge_y->s * (i - 1) + ind - 1] = 255;
      }
    }
  }

#else
  {
    double scaling = 1.0,    sigma = 2,    vert = 1.0,    horz = 0.0;
    // generate the edge image
    canny_fix(bf, polarv2, sigma, scaling, vert, horz, 1.9, 1.5, I_edge);    // edge detection 4-19
  }

  bf_imsetsize(bf, v_edge_y, I_edge->h, I_edge->w, 1, 1);
  // num_up = 0;
  // num_down = 0;
  r_first = MAX(r_first, 0);
  r_first = MIN(r_first, ROI_row);
  r_last = MAX(r_last, 0);
  r_last = MIN(r_last, ROI_row);

  for (i = 1; i <= ROI_row; ++i) {
    if (i <= r_first || i >= r_last) {
      int off = 10;
      int pupil_radius_off = pupil_radius + off + 5;
      uchar* pos = I_edge->tt.data + I_edge->w * (i - 1) + pupil_radius_off;
      int n = I_edge->w - off - pupil_radius_off, max_num;
      MAX_ELEMENT(n, pos, max_num);

      if (pos[max_num] > (0.1 * 255)) { // && polarv(i,ind-2:ind+2) ~= 255
        int ind = max_num + pupil_radius_off;
        v_edge_y->tt.data[v_edge_y->s * (i - 1) + ind - 1] = 255;
      }
    }
  }

#endif

  //printf(" %.4f", utime_elapsed(_start_time));}

  //imshow(I3);  imshow(v_edge_y);  imshow(I_edge);  imshow(polarv);  imshow(polarv2);  cvWaitKey(-1);

  {
    // RR 为累积几个像素和
    int ii, jj, RR = 3, max_up_num = 0, max_up_ind = 0, counts_up_leiji_len = 0;
    int max_down_num = 0, max_down_ind = 0, counts_down_leiji_len = 0;
    int* counts_up = 0;
    int* counts_up_leiji = 0;
    int* counts_down = 0;
    int* counts_down_leiji = 0;
    int clen = 200;
    BFMALLOC(bf, counts_up, 4 * clen);
    counts_up_leiji = counts_up + clen;
    counts_down = counts_up_leiji + clen;
    counts_down_leiji = counts_down + clen;

    for (i = 0; i < 200; ++i) {
      counts_up[i] = counts_up_leiji[i] = counts_down[i] = counts_down_leiji[i] = 0;
    }

    // 进行直方图统计，得到边缘点最多的几列，去除误点
    for (i = 0; i < r_first; ++i) {
      for (j = 0; j < v_edge_y->w; ++j) {
        if (v_edge_y->tt.data[v_edge_y->s * i + j]) {
          // 存储每一列中为1的点的个数
          counts_up[j]++;
        }
      }
    }

    for (j = 0; j < v_edge_y->w - RR; ++j) {
      // 存储每一列中为1的点的个数
      int sum = 0;

      for (i = 0; i < RR; ++i) {
        sum += counts_up[j + i];
      }

      counts_up_leiji[j] = sum;
    }

    counts_up_leiji_len = v_edge_y->w - RR;
    MAX_ELEMENT(counts_up_leiji_len, counts_up_leiji, max_up_ind);
    max_up_num = counts_up_leiji[max_up_ind];

    FILL(200, counts_down, 0);

    for (i = r_last; i < v_edge_y->h; ++i) {
      for (j = 0; j < v_edge_y->w; ++j) {
        if (v_edge_y->tt.data[v_edge_y->s * i + j]) {
          // 存储每一列中为1的点的个数
          counts_down[j]++;
        }
      }
    }

    for (j = 0; j < v_edge_y->w - RR; ++j) {
      // 存储每一列中为1的点的个数
      int sum = 0;

      for (i = 0; i < RR; ++i) {
        sum += counts_down[j + i];
      }

      counts_down_leiji[j] = sum;
    }

    counts_down_leiji_len = v_edge_y->w - RR;
    MAX_ELEMENT(counts_down_leiji_len, counts_down_leiji, max_down_ind);
    max_down_num = counts_down_leiji[max_down_ind];

    BFFREE(bf, counts_up, 4 * clen);

    // 取有效点 为 max_ind : max_ind+RR-1
    bf_imsetsize(bf, edge, ROI_row, ROI_col, 1, 1);

    for (jj = max_up_ind; jj <= MIN(ROI_col, max_up_ind + RR); ++jj) {
      for (ii = 1; ii <= r_first; ++ii) {
        edge->tt.data[edge->s * (ii - 1) + (jj - 1)] = v_edge_y->tt.data[v_edge_y->s * (ii - 1) + (jj - 1)];

        if (v_edge_y->tt.data[v_edge_y->s * (ii - 1) + (jj - 1)] > 128) {
          polarv->tt.data[polarv->s * (ii - 1) + (jj - 1)] = 255;
        }
      }
    }

    for (jj = max_down_ind + 1; jj <= MIN(ROI_col, max_down_ind + RR); ++jj) {
      for (ii = r_last; ii <= ROI_row; ++ii) {
        edge->tt.data[edge->s * (ii - 1) + (jj - 1)] = v_edge_y->tt.data[v_edge_y->s * (ii - 1) + (jj - 1)];

        if (v_edge_y->tt.data[v_edge_y->s * (ii - 1) + (jj - 1)] > 128) {
          polarv->tt.data[polarv->s * (ii - 1) + (jj - 1)] = 255;
        }
      }
    }

    n = 0;

    for (j = 0; j < edge->w; ++j) {
      for (i = 0; i < edge->h; ++i) {
        if (edge->tt.data[edge->s * i + j] && n < p_samplemaxlen) {
          sample[n].y = i;
          sample[n].x = j;
          ++n;
        }
      }
    }

    {
      int avg_x = 0;
      int t1 = MAX(1, (int)(max_up_num / 15));
      int t2 = MAX(1, (int)(max_down_num / 15));
      int n2 = n;
      n = 0;

      for (i = 0; i < n2 / 2; i += t1) {
        sample[n++] = sample[i];
        avg_x += sample[i].x;
      }

      for (; i < n2; i += t2) {
        sample[n++] = sample[i];
        avg_x += sample[i].x;
      }

#if 0

      if (n > 0) {
        avg_x /= n;
        n2 = n;
        n = 0;

        for (i = 0; i < n2; i++) {
          if (abs(sample[i].x - avg_x) < 10) {
            sample[n++] = sample[i];
          }
        }
      }

#endif
    }
  }

  //imshow(edge); imshow(polarv); imshow(v_edge_y); imshow(polarv2); cvWaitKey(-1);

  bf_imfree(bf, edge);
  bf_imfree(bf, v_edge_y);
  bf_imfree(bf, I_edge);
  bf_imfree(bf, polarv1);
  bf_imfree(bf, polarv2);
  bf_imfree(bf, I2);
  bf_imfree(bf, orient_img);
  bf_imfree(bf, I3);

  ASSERT(buflen == bf->len);
  return n;
}
