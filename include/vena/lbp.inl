
static const uchar lbp_mapping59[] = {
#include "lbp_mapping59.txt"
};

int imlbp_u8(const img_t* im, double radius, int neighbors, const uchar* mapping, int bin, int rowBlk, int colBlk, uchar* out) {
  int i, j, k;
  double spoints[8][2] = {0};
  double a = 2*CV_PI/neighbors;
  double miny=1000, maxy=0, minx=1000, maxx=0;
  int ysize = im->h, xsize = im->w;
  int bsizey, bsizex, origy, origx, dx, dy, wR, hR;
  //img_t im2[1] = {0};
  for (i=0; i<neighbors; ++i) {
    spoints[i][0] = -radius*sin((i)*a);
    spoints[i][1] = radius*cos((i)*a);
    miny=MIN(miny, spoints[i][0]);
    maxy=MAX(maxy, spoints[i][0]);
    minx=MIN(minx, spoints[i][1]);
    maxx=MAX(maxx, spoints[i][1]);
  }
  bsizey=(int)(ceil(MAX(maxy,0))-floor(MIN(miny,0))+1);
  bsizex=(int)(ceil(MAX(maxx,0))-floor(MIN(minx,0))+1);
  origy=-(int)floor(MIN(miny,0));
  origx=-(int)floor(MIN(minx,0));
  dx = xsize - bsizex+1;
  dy = ysize - bsizey+1;
  //imsetsize(im2, dy, dx, 1, 1);
  memset(out, 0, rowBlk*colBlk*bin);
  wR =(dx/colBlk);
  hR=(dy/rowBlk);
  if(xsize < bsizex || ysize < bsizey) {
    printf("Too small input image. Should be at least (2*radius+1) x (2*radius+1)");
  }
  for (i=0; i<dy; ++i) {
    const uchar* a = im->tt.data + im->s*(i+origy)+origx;
    //uchar* b = (uchar*)(im2->tt.data + im2->s*i);
    for (j=0; j<dx; ++j, ++a) {
      int result=0;
      for (k=0; k<neighbors; ++k) {
        double y = spoints[k][0];
        double x = spoints[k][1];
        int fy = (int)floor(y), cy = (int)ceil(y), ry = ROUND(y);
        int fx = (int)floor(x), cx = (int)ceil(x), rx = ROUND(x);
        double N;
        if ((fabs(x - rx) < 1e-6) && (fabs(y - ry) < 1e-6)) {
          N = a[ry*im->s + rx];
        } else {
          double ty = y - fy;
          double tx = x - fx;
          
          // Calculate the interpolation weights.
          double w1 = (1 - tx) * (1 - ty);
          double w2 =      tx  * (1 - ty);
          double w3 = (1 - tx) *      ty ;
          double w4 =      tx  *      ty ;
          
          N = w1*a[fy*im->s + fx] + w2*a[fy*im->s+cx] + w3*a[cy*im->s+fx] + w4*a[cy*im->s+cx];
        }
        if (N>=a[0]) {
          result |= (1<<k);
        }
      }
      result = mapping[result];
      //ASSERT(t<VENA_FEATLEN);
      if (i<hR*rowBlk && j<wR*colBlk) {
        int t = ((i/hR)*colBlk+(j/wR))*bin + result;
        out[t]++;
      }
      //b[j] = result;
    }
  }
  
  if (0) {
    //imshow(im);
    //imshow(im2);
    //imshowmat_u1(im2);
    //cvShowMat( "out" , "%I8u", rowBlk*colBlk, bin, out, bin, 1, -1);
    //cvWaitKey(-1);
  }
  //imfree(im2);
  return 0;
}

int abssub_u8(int n, const uchar* a, const uchar* b) {
  int i, s=0;
  for (i=0; i<n; ++i) {
    s += ABSSUB(a[i], b[i]);
  }
  return s;
}

int vena_feat(const uchar* imstd, uchar* feat) {
  img_t imst[1] = {0};
  IMINIT(imst, VENA_STDH, VENA_STDW, imstd, VENA_STDW, 1, 1);
  imlbp_u8(imst, 2, 8, lbp_mapping59, 59, 8, 8, feat);
  return VENA_FEATLEN;
}

#ifdef TEST
int test_lbp() {
  int i, j, err;
  img_t im[10] = {0};
  uchar* out = NULL;
  int* id = 0;
  int pics = 20;
  sys_chdir("E:/code/cstd/src/vena/识别测试/lbp特征提取和识别");
#define ISTRAIN(I)  ((I)%pics<2)
  if (1) {
  }
  if (1) {
    vstr_t sv[1] = {0};
    vstr_load("list.txt", sv);
    out = MALLOC(uchar, VENA_FEATLEN*sv->n);
    id = MALLOC(int, sv->n);
    for (i=0; i<sv->n; ++i) {
      id[i] = i/pics;
    }
    for (i=0; i<sv->n; ++i) {
      imread(sv->v[i].s, 1, 1, im);
      imlbp_u8(im, 2, 8, lbp_mapping59, 59, 8, 8, out+i*VENA_FEATLEN);
      if (0) {
        uchar* out1 = out+i*VENA_FEATLEN;
        //cvShowMat( "out" , "%I8u", 64, 59, out1, 59, 1, -1);    cvWaitKey(-1);
      }
    }
    if (1) {// 统计类内类间
      int jmin = 100000, nmax=0;
      for (i=0; i<sv->n; ++i) {
        uchar* a = out + i*VENA_FEATLEN;
        int kid = 0;
        for (j=i+1; j<sv->n; ++j) {
          uchar* b = out + j*VENA_FEATLEN;
          int d = abssub_u8(VENA_FEATLEN, a, b);
          if (id[i]!=id[j]) {
            jmin = MIN(jmin, d);
          } else {
            nmax = MAX(nmax, d);
          }
        }
      }
      printf("%d %d \n", jmin, nmax);
    }
    if (0) {// 识别率测试
      err=0;
      for (i=0; i<sv->n; ++i) {
        if (ISTRAIN(i)) {
          uchar* a = out + i*VENA_FEATLEN;
          int dmin = 100000;
          int kid = 0;
          for (j=0; j<sv->n; ++j) {
            if (!ISTRAIN(j)) {
              uchar* b = out + j*VENA_FEATLEN;
              int d = abssub_u8(VENA_FEATLEN, a, b);
              if (d<dmin) {
                dmin = d;
                kid = id[j];
              }
            }
          }
          if (kid!=id[i]) {
            err++;
          }
        }
      }
    }
    strv_free(sv);
  }
  imfrees(im, 10);
  return 0;
}

#endif //TEST
