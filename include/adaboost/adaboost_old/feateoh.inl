
typedef struct EOH_FEAT_T {
  char postxtfile[256];
  char negtxtfile[256];
  EOHFEAT* eoh;
  int allhaarlen;
  double* samp;
  int samp_size;
  int samp_step;
  int w, h;
  int stepxy;
  double zoom;
}
EOH_FEAT_T;

double EOH_feat_val(double* A, int al, EOHFEAT* eoh, int i, double IV)
{
  return EOH_FEAT_VAL(A, eoh[ i ], IV);
}

int EOH_setoffset(EOHFEAT* haar, int al, int ai, double cs)
{
  EOH_SETOFFSET(haar[ 0 ], al, ai, cs);
  return 0;
}
int EOH_save_feat(BOOST* hh, int id)
{
  EOH_FEAT_T* fs = (EOH_FEAT_T*)(hh->h_feat);

  if (id < 0) {
    hh->ca->weaklen++;
  }
  else {
    hh->ca->eoh[ hh->ca->weaklen++ ] = fs->eoh[ id ];
  }

  return 0;
}

int make_eoh_rect(int w, int h, int cn, int mincx, int sx, int scx, EOHFEAT* out)
{
  int i = 0, x, y, cx, cy, j;

  for (cx = mincx; cx <= w; cx += scx) {
    for (cy = mincx; cy <= h; cy += scx) {
      for (x = 0; x + cx <= w; x += sx) {
        for (y = 0; y + cy <= h; y += sx) {
          if (out) {
            out[i].x = x;
            out[i].y = y;
            out[i].w = cx;
            out[i].h = cy;
            out[i].bi = 0;

            for (j = 1; j < cn; ++j) {
              out[i + j] = out[i];
              out[i + j].bi = j;
            }
          }

          i += cn;
        }
      }
    }
  }

  return i;
}

int EOH_init(BOOST* hh)
{
  int mincx, mincy, rectxy;
  EOH_FEAT_T* fs = MALLOC(EOH_FEAT_T, 1);
  memset(fs, 0, sizeof(HAAR_FEAT_T));
  strcpy(fs->postxtfile, inigetstr("正样本txt文件"));
  strcpy(fs->negtxtfile, inigetstr("负样本txt文件"));

  fs->zoom = inigetfloat("缩放比");
  fs->w = hh->ca->w = inigetint("样本宽");
  fs->h = hh->ca->h = inigetint("样本高");
  mincx = inigetint("最小宽");
  mincy = inigetint("最小高");
  rectxy = inigetint("特征穷举步进");
  fs->stepxy = inigetint("样本检测步进");
  fs->samp_step = (fs->h + 1) * (fs->w + 1) * EOH_CN;
  hh->h_feat = fs;

  hh->feat_num = make_eoh_rect(fs->w, fs->h, EOH_CN - 1, mincx, rectxy, rectxy, NULL);
  fs->eoh = MALLOC(EOHFEAT, hh->feat_num);
  hh->feat_num = make_eoh_rect(fs->w, fs->h, EOH_CN - 1, mincx, rectxy, rectxy, fs->eoh);

  if (!fs->samp) {
    int cx = hh->ca->w, cy = hh->ca->h;
    fs->samp_size = hh->samp_num * fs->samp_step;
    fs->samp = MALLOCSET(double, fs->samp_size, 0);
  }

  return 0;
}

int EOH_uninit(BOOST* hh)
{
  EOH_FEAT_T* fs = (EOH_FEAT_T*)(hh->h_feat);
  FREE(fs->eoh);
  return 0;
}

int EOH_samp_data(BOOST* hh, char** buf, int* size)
{
  EOH_FEAT_T* fs = (EOH_FEAT_T*)(hh->h_feat);
  *buf = (char*)(fs->samp);
  *size = fs->samp_size;
  return 0;
}

double* EOH_feat(BOOST* hh, int i)
{
  EOH_FEAT_T* fs = (EOH_FEAT_T*)(hh->h_feat);
  int j, cx = hh->ca->w;
  const double* samp = fs->samp;
  EOHFEAT eoh = fs->eoh[ i ];
  int stp = (cx + 1) * EOH_CN;
  EOH_SETOFFSET(eoh, stp, EOH_CN, 1.f);

  for (j = 0; j < hh->samp_num; ++j, samp += fs->samp_step) {
    if (hh->wi[ j ] < 0.f) {
      hh->fi[ j ] = 0;
    }
    else {
      hh->fi[ j ] = EOH_FEAT_VAL(samp, eoh, 0);
    }
  }

  //FPRINT1D(SampNum, tmpSamp, 1, "%13.9g\n", "ttttt1.txt");
  return hh->fi;
}

// EOH 样本
// 每张图片占空间 10*4*33*33=43560 字节
int EOH_find_samp(BOOST* hh, int type)
{
  EOH_FEAT_T* fs = (EOH_FEAT_T*)(hh->h_feat);
  int i = 0, j = 0, n = 0, cx = hh->ca->w, cy = hh->ca->h;
  int sampcnt = 0;
  char buf[ 256 ];
  int _step = (cx + 1) * EOH_CN;
  int _size = (cy + 1) * _step;
  int sampnum = (type > 0) ? (hh->pos_num) : (hh->samp_num - hh->pos_num);
  double* wi = hh->wi + (type > 0 ? 0 : hh->pos_num);
  double* vi = hh->vi + (type > 0 ? 0 : hh->pos_num);
  const char* piclist = type > 0 ? fs->postxtfile : fs->negtxtfile;
  int picnum = 0;
  FILE* pf = fopen(piclist, "r");
  img_t im[2] = {0};
  img_t* im1 = im + 1;

  if (!pf) {
    return 0;
  }

  if (1 == type) {
    FEAT_SETOFFSET(hh->ca->eoh, hh->ca->weaklen, _step, EOH_CN, 1.f, EOH);
  }

  for (picnum = 0, j = 0; j < sampnum && fscanf(pf, "%s", buf) > 0;) {
    utime_start(time);

    if (wi[ j ] >= 0.f) {
      ++j;
      continue;
    }

    if (!imread(buf, 1, 1, im)) {
      continue;
    }

    if (1) {
      ++picnum;

      if (1 == type) {
        int ispass = 0;
        double score;
        double* tmpSamp = fs->samp + j * _size;
        ++sampcnt;
        imresize_1(im, fs->h, fs->w, im);
        EOH_grads_integral(im->h, im->w, im->tt.data, im->s, tmpSamp, _step, EOH_CN);
        cascade_pass(hh->ca, tmpSamp, _step, 0, &ispass, &score);

        if (ispass) {
          wi[ j ] = 1.f;
          vi[ j ] = score;
          ++j;
        }
      }

      if (-1 == type) {
        double ss = fs->zoom;

        for (; j < sampnum; ss *= 1.1) {
          int w = XFLOOR(im->w / ss), h = XFLOOR(im->h / ss);
          int ranx = w - cx + 1;
          int rany = h - cy + 1;
          int stp = (w + 1) * EOH_CN;
          int siz = (h + 1) * stp;
          int x, y, k = 0;
          int stepx = fs->stepxy, stepy = fs->stepxy;
          double* inter;

          if (w < cx || h < cy) {
            break;
          }

          inter = MALLOCSET(double, siz, 0);
          imresize_1(im, h, w, im1);
          EOH_grads_integral(h, w, im1->tt.data, w, inter, stp, EOH_CN);
          FEAT_SETOFFSET(hh->ca->eoh, hh->ca->weaklen, stp, EOH_CN, 1.f, EOH);

          //for (i=0; i<times; ++i)
          for (x = 0; j < sampnum && x < ranx; x += stepx) {
            for (y = 0; j < sampnum && y < rany; y += stepy) {
              double* tmpSamp = inter + y * stp + x * EOH_CN;
              int ispass = 0;
              double score;
              ++sampcnt;
              //CASCADE_PASS(hh->ca, tmpSamp, hh->ca->eoh, EOH_FEAT_VAL, 0, ispass, score);
              cascade_pass(hh->ca, tmpSamp, stp, 0, &ispass, &score);

              if (j < sampnum && ispass) {
                int cy1 = hh->ca->h + 1;
                double* Samp = fs->samp + (j + hh->pos_num) * _size;
                SKIP_COPY(cy1, _step, tmpSamp, stp, 1, Samp, _step, 1);

                //imwrite("aa.bmp", hh->h, cx, data+step*y+x, step, 1);
                wi[ j ] = 1.f;
                vi[ j ] = score;
                ++j;
                ++k;
              }
            }
          }

          FREE(inter);
        }
      }

      printf("找了%5d张图片，共%5d个样本, 共找到%5d个%s样本！\r", picnum, sampcnt, j, type > 0 ? "正" : "负");
    }
  }

  freeims(im, 2);
  fclose(pf);
  logprintf("找了%5d张图片，共%5d个样本, 共找到%5d个%s样本！\n", picnum, sampcnt, j, type > 0 ? "正" : "负");
  return n;
}

