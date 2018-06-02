
int TopEyelidLocation(buf_t* bf, const img_t* img, int nr, int nx, int ny, int ss, double* y)
{
  // z 为上眼睑的函数，每列对应ROI当列的行坐标
  // TopEyelidROI为感兴趣区域 ROI
  // a 为选中用于ployfit的最终上眼睑边缘点
  // I5 为ROI的横向边缘检测结果
  // TopEyelidROI_rgb 为ROI的最开始选择的上眼睑边缘点
  // length_genpoint 为选中的真正边缘点个数
  // all_edge_length 为最开始的选中的上眼睑边缘点个数
  const double TopEyelidTrainM1[] = {0.00167517747954918, -0.381046697246712, 77.6197490865995};
  const double TopEyelidTrainM2[] = {0.00211012981182002, -0.580928670674821, 93.2584231067020};
  const double TopEyelidTrainM3[] = {0.00171663194782703, -0.277037760721026, 66.8043107512525};
  const img_t* TopEyelidROI = img;
  img_t I5_2[1] = {0};
  img_t I5[1] = {0};
  img_t newdiffabs1[1] = {0};
  img_t newdiffabs2[1] = {0};
  img_t newdiffabs3[1] = {0};
  int ROI_row, ROI_col;
  int ii, jj, kk, all_edge_length, xx3, ccol, KKK, row_col;
  double* zz1 = 0;
  double* zz2 = 0;
  double* zz3 = 0;
  int* diffabs = 0;
  int* all_edgepoint = 0;
  int* new_h_sum1 = 0;
  int* new_h_sum2 = 0;
  int* new_h_sum3 = 0;
  int diffabs_indx1, diffabs_indx2, diffabs_indx3, M_indx;
  int H_SUM[3];
  int buflen = bf->len;
  int zzlen = 200;
  //nr = ceil(nr);
  // 进行去除睫毛 水平滤波 得到边缘检测的结果
  //{utime_start(_start_time);
  horizontal_filter(bf, TopEyelidROI, 1, I5_2); // figure,imshow(I5);

  //printf(" %.4f", utime_elapsed(_start_time));}
  bf_imsetsize(bf, I5, I5_2->h * 2, I5_2->w * 2, 1, 1);
  imresize_bili(I5_2, I5);
  ROI_row = I5->h, ROI_col = I5->w;

  //imshow(TopEyelidROI); imshow(I5);cvWaitKey(-1);
  //diffabs = zeros(1, ROI_col);
  //all_edgepoint = zeros(1, ROI_col);
  //     TopEyelidROI_rgb(:,:,1) = TopEyelidROI;
  //     TopEyelidROI_rgb(:,:,2) = TopEyelidROI;
  //     TopEyelidROI_rgb(:,:,3) = TopEyelidROI;
  all_edge_length = 0;

  BFMALLOC(bf, zz1, 3 * zzlen);
  zz2 = zz1 + zzlen;
  zz3 = zz2 + zzlen;
  BFMALLOC(bf, diffabs, 5 * zzlen);
  all_edgepoint = diffabs + zzlen;
  new_h_sum1 = all_edgepoint + zzlen;
  new_h_sum2 = new_h_sum1 + zzlen;
  new_h_sum3 = new_h_sum2 + zzlen;

  for (jj = 0; jj < zzlen; ++jj) {
    diffabs[jj] = all_edgepoint[jj] = new_h_sum1[jj] = new_h_sum2[jj] = new_h_sum3[jj] = 0;
  }

  // 5_31 将pupil直径上方 与 pupil直径外进行分别处理
  for (jj = 5; jj <= MIN(zzlen, ny - nr); ++jj) {
    for (kk = (3 * ROI_row / 5); kk >= 10; --kk) {
      int diffdist = FastDistance2D((kk -  nx), (jj - ny));

      if ((diffdist > (nr + 5)) && (I5->tt.data[I5->s * (kk - 1) + jj - 1] > 128)) {
        diffabs[jj - 1] = kk + 1;
        all_edgepoint[jj - 1] = kk + 1;
        all_edge_length = all_edge_length + 1;
        // TopEyelidROI_rgb(kk+1, jj, 1) = 255;
        break;
      }
    }
  }

  for (jj = ny + nr; jj <= MIN(zzlen, ROI_col - 5); ++jj) {
    for (kk = (3 * ROI_row / 5); kk >= 10; --kk) {
      int diffdist = FastDistance2D((kk -  nx), (jj - ny));

      if ((diffdist > (nr + 5)) && (I5->tt.data[I5->s * (kk - 1) + jj - 1] > 12)) {
        diffabs[jj - 1] = kk + 1;
        all_edgepoint[jj - 1] = kk + 1;
        all_edge_length = all_edge_length + 1;
        //                 TopEyelidROI_rgb(kk+1, jj, 1) = 255;
        break;
      }
    }
  }

  for (jj = ny - nr + 1; jj <= MIN(zzlen, ny + nr - 1); ++jj) {
    for (kk = (3 * ROI_row / 5); kk >= 10; --kk) {
      int diffdist = FastDistance2D((kk -  nx), (jj - ny));

      if ((diffdist < (nr - 5)) && (I5->tt.data[I5->s * (kk - 1) + jj - 1] > 128)) {
        diffabs[jj - 1] = kk + 1;
        all_edgepoint[jj - 1] = kk + 1;
        all_edge_length = all_edge_length + 1;
        //                 TopEyelidROI_rgb(kk+1, jj, 1) = 255;
        break;
      }
      else if (((diffdist > (nr + 5)) && (I5->tt.data[I5->s * (kk - 1) + jj - 1] > 128))) {
        diffabs[jj - 1] = kk + 1;
        all_edgepoint[jj - 1] = kk + 1;
        all_edge_length = all_edge_length + 1;
        //                 TopEyelidROI_rgb(kk+1, jj, 1) = 255;
        break;
      }
    }
  }

  row_col = MIN(zzlen, ROI_col);

  for (xx3 = 1; xx3 <= row_col; ++xx3) {
    zz1[xx3 - 1] = TopEyelidTrainM1[0] * (xx3 * xx3) + TopEyelidTrainM1[1] * xx3 + TopEyelidTrainM1[2];
    zz2[xx3 - 1] = TopEyelidTrainM2[0] * (xx3 * xx3) + TopEyelidTrainM2[1] * xx3 + TopEyelidTrainM2[2];
    zz3[xx3 - 1] = TopEyelidTrainM3[0] * (xx3 * xx3) + TopEyelidTrainM3[1] * xx3 + TopEyelidTrainM3[2];
  }

  bf_imsetsize(bf, newdiffabs1, row_col, row_col, sizeof(double), 1);
  bf_imsetsize(bf, newdiffabs2, row_col, row_col, sizeof(double), 1);
  bf_imsetsize(bf, newdiffabs3, row_col, row_col, sizeof(double), 1);

  ccol = row_col;

  //logclear();
  for (KKK = 0; KKK < row_col; ++KKK) {
    int i, iii;
    double* yyy, *yyyy, *yyyyy;
    new_h_sum1[KKK] = 0;
    new_h_sum2[KKK] = 0;
    new_h_sum3[KKK] = 0;

    for (i = 0; i < row_col; ++i) {
      newdiffabs1->tt.f8[newdiffabs1->w * KKK + i] = diffabs[i] + KKK + 1 - (row_col / 2) - zz1[i];   // 6_1修改
      newdiffabs2->tt.f8[newdiffabs2->w * KKK + i] = diffabs[i] + KKK + 1 - (row_col / 2) - zz2[i];
      newdiffabs3->tt.f8[newdiffabs3->w * KKK + i] = diffabs[i] + KKK + 1 - (row_col / 2) - zz3[i];
      //if (KKK==188) logprintf("%4.4f\n", newdiffabs1->tt.f8[newdiffabs1->w*KKK + i]);
    }

    yyy = newdiffabs1->tt.f8 + newdiffabs1->w * KKK;
    yyyy = newdiffabs2->tt.f8 + newdiffabs2->w * KKK;
    yyyyy = newdiffabs3->tt.f8 + newdiffabs3->w * KKK;

    for (iii = 0; iii < ccol; ++iii) {
      if (yyy[iii] >= -2 && yyy[iii] < 6) {
        new_h_sum1[KKK]++;
      }

      if (yyyy[iii] >= -2 && yyyy[iii] < 6) {
        new_h_sum2[KKK]++;
      }

      if (yyyyy[iii] >= -2 && yyyyy[iii] < 6) {
        new_h_sum3[KKK]++;
      }
    }
  }

  if (1) {
    MAX_ELEMENT(row_col, new_h_sum1, diffabs_indx1);
    MAX_ELEMENT(row_col, new_h_sum2, diffabs_indx2);
    MAX_ELEMENT(row_col, new_h_sum3, diffabs_indx3);
    H_SUM[0] = new_h_sum1[diffabs_indx1];
    H_SUM[1] = new_h_sum2[diffabs_indx2];
    H_SUM[2] = new_h_sum3[diffabs_indx3];

    {
      double* diffabs_tot[3];
      IPOINT* a_pt = 0;
      BFMALLOC(bf, a_pt, zzlen);
      //double* canshu123;
      diffabs_tot[0] = newdiffabs1->tt.f8 + newdiffabs1->w * diffabs_indx1;
      diffabs_tot[1] = newdiffabs2->tt.f8 + newdiffabs2->w * diffabs_indx2;
      diffabs_tot[2] = newdiffabs3->tt.f8 + newdiffabs3->w * diffabs_indx3;

      MAX_ELEMENT(3, H_SUM, M_indx);
      jj = 0;

      for (ii = 0; ii < row_col; ++ii) {
        if (fabs(diffabs_tot[M_indx][ii]) <= 10) {
          a_pt[jj].x = ii;
          a_pt[jj].y = all_edgepoint[ii];
          ++jj;
        }
      }

#if 0
      {
        IPOINT aa[] = {
#include "F:/pub/bin/iris/poly.txt"
        };
        my_two_polyfit(aa, sizeof(aa) / sizeof(IPOINT), y);
        DrawPoints(ROI_row, ROI_col, img->tt.data, ROI_col, 1, aa, jj, CV_RGB(255, 255, 255), 1, 8, 0);
        DrawPloy(ROI_row, ROI_col, img->tt.data, ROI_col, 1, y, 3, CV_RGB(255, 255, 255), 1, 8, 0);
        imshow(img);
        cvWaitKey(-1);
      }
#endif

      for (ii = 0; ii < jj; ++ii) {
        a_pt[ii].x = a_pt[ii].x * ss >> 8;
        a_pt[ii].y = a_pt[ii].y * ss >> 8;
      }

      my_two_polyfit(a_pt, jj, y);
      BFFREE(bf, a_pt, zzlen);
#if 0
      DrawPoints(ROI_row, ROI_col, I5->tt.data, ROI_col, 1, a, jj, CV_RGB(255, 255, 255), 1, 8, 0);
      DrawPloy(ROI_row, ROI_col, I5->tt.data, ROI_col, 1, 3, y, 0, 0, CV_RGB(255, 255, 255), 1, 8, 0);
      imshow(I5);
      cvWaitKey(-1);
#endif
    }
  }

  bf_imfree(bf, newdiffabs3);
  bf_imfree(bf, newdiffabs2);
  bf_imfree(bf, newdiffabs1);

  BFFREE(bf, diffabs, 5 * zzlen);
  BFFREE(bf, zz1, 3 * zzlen);

  bf_imfree(bf, I5);
  bf_imfree(bf, I5_2);
  ASSERT(buflen == bf->len);
  return 0;
}
