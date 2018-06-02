int iris_detect_show(img_t* mask_iris, int* z_top, int* z_bottom, int* nc, int* wc)
{
  int ii, jj, ROI_row, ROI_col;
  int nx = nc[0] >> 8, ny = nc[1] >> 8, nr = (nc[2] >> 8) - 1;
  int wx = wc[0] >> 8, wy = wc[1] >> 8, wr = (wc[2] >> 8) + 1;
  // 显示最终的检查结果
  ROI_row = mask_iris->h, ROI_col = mask_iris->w;
  //FILL(ROI_row * ROI_col, mask_iris->tt.data, 255);  // 直角坐标系下的掩膜 0表示无效点，1为有效点

  for (ii = 0; ii < ROI_col; ++ii) {
    if ((round(z_top[ii]) <= 0) || (round(z_top[ii]) >= ROI_row)) {
      continue;
    }

    for (jj = 0; jj < z_top[ii]; ++jj) {
      mask_iris->tt.data[mask_iris->s * jj + ii] = 0;
    }
  }

  for (ii = 0; ii < ROI_col; ++ii) {
    if (round(z_bottom[ii]) <= 0 || (round(z_bottom[ii]) >= ROI_row)) {
      continue;
    }

    for (jj = z_bottom[ii]; jj < ROI_row; ++jj) {
      mask_iris->tt.data[mask_iris->s * jj + ii] = 0;
    }
  }

  for (ii = 0; ii < ROI_row; ++ii) {
    for (jj = 0; jj < ROI_col; ++jj) {
      if (((ii - ny) * (ii - ny) + (jj - nx) * (jj - nx)) <= nr * nr) {
        mask_iris->tt.data[mask_iris->s * ii + jj] = 0;
      }
      else if (((ii - wy) * (ii - wy) + (jj - wx) * (jj - wx)) >= wr * wr) {
        mask_iris->tt.data[mask_iris->s * ii + jj] = 0;
      }
    }
  }

  //imshow(mask_iris); cvWaitKey(-1);

  //v_dw = draw_yuan(nx, ny, nr, v_dw);
  //v_dw = draw_yuan(wx, wy, wr, v_dw);
  return 0;
}
