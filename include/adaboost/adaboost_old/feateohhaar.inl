
STATIC int EOHHAAR_print(FILE* pf, const EOHHAARFEAT* eohhaar)
{
  int type = eohhaar[ 0 ].type;
  fprintf(pf, _D(d), type);

  if (FT_EOH == type || FT_EOHEX == type) {
    FPRINTF_EOH(pf, eohhaar[ 0 ]);
  }
  else if (FT_HAAR == type) {
    FPRINTF_HAAR(pf, eohhaar[ 0 ]);
  }

  return 0;
}
STATIC int EOHHAAR_scanf(FILE* pf, EOHHAARFEAT* eohhaar)
{
  int type;
  fscanf(pf, _D(d), &type);
  eohhaar[ 0 ].type = type;

  if (FT_EOH == type || FT_EOHEX == type) {
    FSCANF_EOH(pf, eohhaar[ 0 ]);
  }
  else if (FT_HAAR == type) {
    FSCANF_HAAR(pf, eohhaar[ 0 ]);
  }

  return 0;
}
int EOHHAAR_setoffset(EOHHAARFEAT* haar, int al, int ai, double cs)
{
  EOHHAAR_SETOFFSET(haar[ 0 ], al, ai, cs);
  return 0;
}
double EOHHAAR_feat_val(double* A, int al, EOHHAARFEAT* eohhaar, int i, double IV)
{
  return EOHHAAR_FEAT_VAL(A, eohhaar[ i ], IV);
}

int EOHHAAR_integral(int h, int w, const unsigned char* img, int al, double* mag, int ml, int mi)
{
  EOH_grads_integral(h, w, img, al, mag, ml, mi);
  SKIP_INTEGRAL_SQ(h, w, img, al, 1, mag + EOH_CN, ml, mi);
  return 0;
}
int EOHHAAR_save_feat(BOOST* hh, int id)
{
  if (id < 0) {
    hh->ca->weaklen++;
  }
  else {
    hh->ca->eohhaar[ hh->ca->weaklen++ ] = hh->eohhaar[ id ];
  }

  return 0;
}

int EOHHAAR_init(BOOST* hh, const char* inifile, const char* featsect)
{
  char buf[ 256 ];
  int mincx, mincy, rectxy;
  hh->postxtfile = MALLOC(char, 256);
  hh->negtxtfile = MALLOC(char, 256);
  IniGetString(inifile, featsect, "正样本txt文件", "", hh->postxtfile, 256);
  IniGetString(inifile, featsect, "负样本txt文件", "", hh->negtxtfile, 256);
  IniGetFloat(inifile, featsect, "缩放比", hh->zoom);
  hh->ca->w = IniGetInt(inifile, featsect, "样本宽", 0);
  hh->ca->h = IniGetInt(inifile, featsect, "样本高", 0);
  mincx = IniGetInt(inifile, featsect, "最小宽", 2);
  mincy = IniGetInt(inifile, featsect, "最小高", 2);
  rectxy = IniGetInt(inifile, featsect, "特征穷举步进", 2);
  hh->stepxy = IniGetInt(inifile, featsect, "样本检测步进", 2);
  hh->samp_step = (hh->ca->h + 1) * (hh->ca->w + 1) * EOHHAAR_CN;
  IniGetString(inifile, featsect, "特征矩形文件", "", buf, 256);

  if (!hh->samp) {
    int cx = hh->ca->w, cy = hh->ca->h;
    hh->samp_size = hh->samp_num * (cy + 1) * (cx + 1) * EOHHAAR_CN;
    hh->samp = MALLOCSET(real, hh->samp_size, 0);
  }

  {
    EOHFEAT* eoh = NULL;
    HAARFEAT* haar = NULL;
    int eoh_feat_num, haar_feat_num;
    eoh_feat_num = read_rect_from_file(buf, NULL, EOH_CN - 1);

    if (eoh_feat_num) {
      eoh = MALLOC(EOHFEAT, eoh_feat_num);
      read_rect_from_file(buf, eoh, EOH_CN - 1);
    }

    haar_feat_num = get_haar_feat(hh->ca->h, hh->ca->w, mincx, mincy, rectxy, rectxy, NULL);

    if (haar_feat_num) {
      haar = MALLOC(HAARFEAT, haar_feat_num);
      get_haar_feat(hh->ca->h, hh->ca->w, mincx, mincy, rectxy, rectxy, haar);
    }

    //eoh_feat_num = 0;
    hh->feat_num = eoh_feat_num + haar_feat_num;

    if (hh->feat_num) {
      int i;
      hh->eohhaar = MALLOC(EOHHAARFEAT, hh->feat_num);

      for (i = 0; i < eoh_feat_num; ++i) {
        hh->eohhaar[ i ].x = eoh[ i ].x;
        hh->eohhaar[ i ].y = eoh[ i ].y;
        hh->eohhaar[ i ].w = eoh[ i ].w;
        hh->eohhaar[ i ].h = eoh[ i ].h;
        hh->eohhaar[ i ].bi = eoh[ i ].bi;
        hh->eohhaar[ i ].type = FT_EOH;
      }

      for (i = 0; i < haar_feat_num; ++i) {
        hh->eohhaar[ i + eoh_feat_num ].ft0 = haar[ i ].ft0;
        hh->eohhaar[ i + eoh_feat_num ].ft1 = haar[ i ].ft1;
        hh->eohhaar[ i + eoh_feat_num ].type = FT_HAAR;
      }
    }

    SAFEFREE(haar);
    SAFEFREE(eoh);
  }

  return 0;
}

int EOHHAAR_uninit(BOOST* hh)
{
  SAFEFREE(hh->eohhaar);
  SAFEFREE(hh->postxtfile);
  SAFEFREE(hh->negtxtfile);
  return 0;
}

int EOHHAAR_feat(BOOST* hh, int i, real* tmpSamp)
{
  int j, cx = hh->ca->w;
  const real* samp = hh->samp;
  EOHHAARFEAT eohhaar = hh->eohhaar[ i ];
  int stp = (cx + 1) * EOHHAAR_CN;
  EOHHAAR_SETOFFSET(eohhaar, stp, EOHHAAR_CN, 1.f);

  for (j = 0; j < hh->samp_num; ++j, samp += hh->samp_step) {
    if (hh->wi[ j ] < 0.f) {
      continue;
    }

    tmpSamp[ j ] = EOHHAAR_FEAT_VAL(samp, eohhaar, 1.f);
  }

  //PRINT1D(SampNum, tmpSamp, 1, "%13.9g\n", "ttttt1.txt");
  return 0;
}

// EOHHAAR 样本
// 每张图片占空间 12*4*33*33=52272 字节
int EOHHAAR_find_samp(BOOST* hh, int type)
{
  int i = 0, j = 0, n = 0, cx = hh->ca->w, cy = hh->ca->h;
  char buf[ 256 ];
  int _step = (cx + 1) * EOHHAAR_CN;
  int _size = (cy + 1) * _step;
  int SampNum = (type > 0) ? (hh->pos_num) : (hh->samp_num - hh->pos_num);
  real* wi = hh->wi + (type > 0 ? 0 : hh->pos_num);
  real* vi = hh->vi + (type > 0 ? 0 : hh->pos_num);
  real zoom = hh->zoom;
  const char* piclist = type > 0 ? hh->postxtfile : hh->negtxtfile;
  int picnum = 0;
  FILE* pf = fopen(piclist, "r");
  hh->samp_step = _size;

  if (!pf) {
    error(ERR_FOPEN);
    return 0;
  }

  if (1 == type) {
    FEAT_SETOFFSET(hh->ca->eohhaar, hh->ca->weaklen, _step, EOHHAAR_CN, 1.f, EOHHAAR);
  }

  for (i = 0, j = 0; j < SampNum && fscanf(pf, "%s", buf) > 0; ++i) {
    if (wi[ j ] >= 0.f) {
      ++j;
      continue;
    }

    {
      int height, width, channels = 1;
      struct fmtimgreader* reader = new_reader(buf, &height, &width, 0);

      if (reader) {
        int step = width * channels;
        unsigned char* data = (unsigned char*) malloc(height * step);
        real invvar;
        imread(reader, data, step, 1);
        ++picnum;

        if (1 == type) {
          int k;
          real* Samp = hh->samp + j * _size;
          real* Samp0 = Samp + EOH_CN;
          real* Samp1 = Samp0 + 1;
          real invarea = 1.f / (height * width);
          int ispass = 0;
          real score;
          assert(cy == height && cx == width);
          EOH_grads_integral(height, width, data, step, Samp, _step, EOHHAAR_CN);
          SKIP_INTEGRAL_SQ(height, width, data, step, 1, Samp0, _step, EOHHAAR_CN);

          //FPRINT2D("a.txt", "%13.9f, ", height, width, Samp1, _step, EOHHAAR_CN);
          SKIP_INTEGRAL_VAR(Samp0, _step * cy, EOHHAAR_CN * cx, Samp1, _step * cy, EOHHAAR_CN * cx, invarea, invvar);
          invvar = (invvar <= 0.f) ? 1.f : 1.f / (real) sqrt(invvar);
          //CASCADE_PASS(hh->ca, Samp, hh->ca->eohhaar, EOHHAAR_FEAT_VAL, invvar, ispass, score);
          cascade_pass(hh->ca, Samp, invvar, &ispass, &score);

          if (ispass) {
            for (k = 0; k < _size; k += EOHHAAR_CN) {
              Samp0[ k ] *= invvar;
            }

            wi[ j ] = 1.f;
            vi[ j ] = score;
            ++j;
          }
        }

        if (-1 == type) {
          real ss = 1.0;
          int w = width, h = height;
          unsigned char* tmpimg = MALLOC(unsigned char, w * h);

          for (; j < SampNum && w >= cx && h >= cy; ss *= zoom, w = XFLOOR(width / ss), h = XFLOOR(height / ss)) {
            int ranx = w - cx + 1;
            int rany = h - cy + 1;
            int stp = (w + 1) * EOHHAAR_CN;
            int siz = (h + 1) * stp;
            int x, y, k = 0;
            int stepx = hh->stepxy, stepy = hh->stepxy;
            real* inter = MALLOC(real, siz);
            real invarea = 1.f / (h * w);
            FILL(siz, inter, 0);
            IMRESIZE(height, width, data, step, 1, h, w, tmpimg, w, 1, INTER_BILINEARITY);
            EOH_grads_integral(h, w, tmpimg, w, inter, stp, EOHHAAR_CN);
            SKIP_INTEGRAL_SQ(h, w, tmpimg, w, 1, inter + EOH_CN, stp, EOHHAAR_CN);
            FEAT_SETOFFSET(hh->ca->eohhaar, hh->ca->weaklen, stp, EOHHAAR_CN, 1.f, EOHHAAR);

            //for (i=0; i<times; ++i)
            for (x = 0; j < SampNum && x < ranx; x += stepx) {
              for (y = 0; j < SampNum && y < rany; y += stepy) {
                real* tmpSamp = inter + y * stp + x * EOHHAAR_CN;
                real* tmpSamp0 = tmpSamp + EOH_CN;
                real* tmpSamp1 = tmpSamp0 + 1;
                int ispass = 0;
                real score;
                SKIP_INTEGRAL_VAR(tmpSamp0, stp * cy, EOHHAAR_CN * cx, tmpSamp1, stp * cy, EOHHAAR_CN * cx, invarea, invvar);
                invvar = (invvar <= 0.f) ? 1.f : 1.f / (real) sqrt(invvar);
                //CASCADE_PASS(hh->ca, tmpSamp, hh->ca->eohhaar, EOHHAAR_FEAT_VAL, invvar, ispass, score);
                cascade_pass(hh->ca, tmpSamp, invvar, &ispass, &score);

                if (ispass) {
                  int cy1 = cy + 1, k;
                  real* Samp = hh->samp + (j + hh->pos_num) * _size;
                  real* Samp0 = Samp + EOH_CN;
                  SKIP_COPY(cy1, _step, tmpSamp, stp, 1, Samp, _step, 1);

                  for (k = 0; k < _size; k += EOHHAAR_CN) {
                    Samp0[ k ] *= invvar;
                  }

                  wi[ j ] = 1.f;
                  vi[ j ] = score;
                  ++j;
                  ++k;
                }
              }
            }

            SAFEFREE(inter);
          }

          SAFEFREE(tmpimg);
        }

        free(data);
        free_reader(reader);
        printf("找了%5d张图片，共找到%5d个%s样本！\r", picnum, j, type > 0 ? "正" : "负");
      }
    }
  }

  fclose(pf);
  logprintf("找了%5d张图片，共找到%5d个%s样本！\n", picnum, j, type > 0 ? "正" : "负");
  return n;
}

