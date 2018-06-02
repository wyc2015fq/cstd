

int BottomEyelidLocation(buf_t* bf, const img_t* img, int nr, int wr, int polar_nx, int polar_ny, int ss, double* canshu123)
{
  img_t BottomEyelidROI[1] = {0}, I5[1] = {0};
  int ii, jj, kk, length_point, num_point, ROI_row, ROI_col;
  int h = img->h, w = img->w;

  length_point = 0;           // 选取的在iris 内部的点的个数
  num_point = 0;              // 判定是否有遮挡的计数

  bf_imsetsize(bf, BottomEyelidROI, h, w, 1, 1);
  imclone(img, BottomEyelidROI);

  ROI_row = BottomEyelidROI->h, ROI_col = BottomEyelidROI->w;

  horizontal_filter(bf, img, 0, I5);

  // I4为double  I5为logical型
  // 6_1 进行判定，对I4图像的pupil外iris内内部进行判断是否与下眼睑有遮挡
  // 注意： 有遮挡时必定有很强的canny曲线检测到，也就是可以对I4进行(nr+10,wr+2)范围内进行I4==1统计计数
  for (jj = 5; jj <= ROI_col - 5; ++jj) {
    for (kk = round(2 * ROI_row / 3.); kk <= ROI_row; ++kk) {
      if (I5->tt.data[I5->s * (kk - 1) + jj - 1] > 128) {
        double diffdist = sqrt(SQR(kk - polar_ny) + SQR(jj - polar_nx));

        if ((diffdist > nr + 10) && (diffdist < wr + 2)) {
          num_point = num_point + 1;
        }
      }
    }
  }

  //imshow(I5);cvWaitKey(-1);
  if (num_point <= 70) {              // 这里有个重要的参数设定啊
    //z = 1;
    //a[0].x=1, a[0].y=1;
    canshu123[0] = canshu123[1] = canshu123[2] = 0;
  }
  else {
    IPOINT* a_pt = 0;
    BFMALLOC(bf, a_pt, 1000);

    // 每行选出一个边缘点   没有的边缘点 all_edgepoint为0
    for (jj = 5; jj <= ROI_col - 5; ++jj) {
      for (kk = (2 * ROI_row / 3); kk <= ROI_row; ++kk) {
        if (I5->tt.data[I5->s * (kk - 1) + jj - 1] > 128) {
          double diffdist = FastDistance2D((kk - polar_ny), (jj - polar_nx));

          if (length_point < 1000 && (diffdist > nr + 10) && (diffdist < wr + 5)) {
            // diffabs(1,jj) = kk;
            // all_edgepoint(1,jj) = kk;
            a_pt[length_point].x = jj;
            a_pt[length_point].y = kk;
            BottomEyelidROI->tt.data[BottomEyelidROI->s * (kk - 1) + jj - 1] = 255;
            length_point = length_point + 1;
            break;
          }
        }

        I5->tt.data[I5->s * (kk - 1) + jj - 1] = 0;
      }
    }

#if 0
    {
      int ii;
      imshow(BottomEyelidROI);
      cvWaitKey(-1);
      my_two_polyfit(a_pt, length_point, canshu123);

      for (ii = 0; ii < ROI_row; ++ii) {
        double x = ii + 1;
        int z = (int)(canshu123[0] * (x * x) + canshu123[1] * x + canshu123[2]);

        if (z > 0 && z < ROI_row) {
          //z[ii] = ROI_row;
          BottomEyelidROI->tt.data[BottomEyelidROI->s * z + ii] = 255;
        }
      }
    }
    imshow(BottomEyelidROI);
    cvWaitKey(-1);

#endif

    for (ii = 0; ii < jj; ++ii) {
      a_pt[ii].x = a_pt[ii].x * ss >> 8;
      a_pt[ii].y = a_pt[ii].y * ss >> 8;
    }

    my_two_polyfit(a_pt, length_point, canshu123);
    BFFREE(bf, a_pt, 1000);
  }


  //imshow(img); imshow(I5); //cvWaitKey(-1);
  bf_imfree(bf, I5);
  bf_imfree(bf, BottomEyelidROI);

  return 0;
}
