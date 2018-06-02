
int pashan(img_t* I)
{
  int i, j, k, h = I->h, w = I->w;
  img_t h_eg[1] = {0};
  int pp[1000];
  imsetsize(h_eg, h, w, 1, 1);

  for (i = 0; i < h; ++i) {
    uchar* pos = I->tt.data + I->s * i;

    for (j = 1; j < w; ++j) {
      h_eg->tt.data[h_eg->s * i + j] = ABS(pos[j] - pos[j - 1]);
    }
  }

  //imshow(h_eg);

  for (i = 0; i < h; ++i) {
    pp[i] = 20;
  }

  for (k = 0; k < 1000000000; ++k) {
    pp[0] = pp[1];
    pp[h - 1] = pp[h - 2];

    for (i = 1; i < h - 1; ++i) {
      uchar* pos = h_eg->tt.data + h_eg->s * i;

      if (pp[i - 1] == pp[i + 1] && pp[i + 1] >= pp[i] && pp[i] < w) {
        if (pos[pp[i] + 1] >= pos[pp[i]] - 10) {
          pp[i]++;
        }
      }
    }

#if 0
    {
      img_t I2[1] = {0};
      imclone(I, I2);

      for (i = 0; i < h; ++i) {
        uchar* pos = I2->tt.data + I2->s * i;
        pos[pp[i]] = 255;
      }

      imshow(I2);
      cvWaitKey(10);
      imfree(I2);
    }
#endif
  }

  imfree(h_eg);
  return 0;
}
