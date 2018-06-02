

int glassRemoval(buf_t* bf, const img_t* im, img_t* out)
{
  int hist[256] = {0};
  int i, j, h = im->h, w = im->w;
  int dd = MIN(h, w) / 4;
  const unsigned char* data = im->tt.data;
  int thm = (int)floor(0.05 * h * w), th = 255, cnt = 0;
  img_t R[1] = {0};
  img_t R2[1] = {0};

  bf_imsetsize(bf, out, h, w, 1, 1);

  for (i = 0; i < h - 0; ++i) {
    for (j = 0; j < w - 0; ++j) {
      hist[data[i * w + j]]++;
    }
  }

  for (i = 255; i >= 0; --i) {
    if (cnt > thm) {
      th = i + 1;
      break;
    }

    cnt += hist[i];
  }

  bf_imsetsize(bf, R, h, w, 1, 1);
  ASSERT(h > dd * 2 && w > dd * 2);

  for (i = dd; i < h - dd; ++i) {
    for (j = dd; j < w - dd; ++j) {
      int t = i * w + j;
      R->tt.data[t] = data[t] > th;
    }
  }

  bf_imsetsize(bf, R2, h, w, 1, 1);
  //imdilate_rect(R, R2, 3);
  //iminpaint2(im, R2, out);
  dilate_rect(bf, R->h, R->w, R->tt.data, R->s, R2->tt.data, R2->s, 3, 3, 1, 1, 1);
  inpaint2_min(im->h, im->w, im->tt.data, im->s, R2->tt.data, R2->s, out->tt.data, out->s);
  //imshow(im);  imshow(R2); imshow(R);  imshow(out);  cvWaitKey(-1);
  bf_imfree(bf, R2);
  bf_imfree(bf, R);
  return 0;
}
