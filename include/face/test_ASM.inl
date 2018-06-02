
//#include "ui\window.inl"
//#include "draw\imdraw1.inl"
#include "cstd.h"
#include "draw/imdraw.inl"
#include "img/color.inl"
#include "asmfit.inl"
#include "geo/Subdiv.inl"
// #include "face/face_recog.inl"
#include "facetrack.inl"
#include "img/imgio.inl"

int test_PyramidDown1() {
  double aa = sqrt( 0.1250 );
  double bb = sqrt( 3.1250 );
  double cc = sqrt( 8.0000 );
  cc /= aa;
  bb /= aa;
  aa /= aa;
  return 0;
}

//#include "cv1/morph.inl"

int _rgb_[4]={255, 0, 0, 50};

int starface(int h, int w, unsigned char* rgb, int step, int channels, char* picname ) {
  uchar* stdimg=0;
  int ret=0;
  delaunay_t dp = {0};
  
  img_t im[1]={0};
  real* ptx=0, *pty=0;
  imread(picname, 3, 1, im);
  //starface1(im->height, im->width, im->data, im->step, h, w, rgb, step, channels);
  imfree( im );
  return 1;
  
  return 0;
}

int test_GetImagePyramid() {
  const char * picname = ( "a.bmp" );
  img_t im[1]={0};
  if ( im->tt.data ) {
    int channels = 1;
    int i, h, w;
    int NumPyramidLevels = 6;
    unsigned char** Pyr = NULL;
    IPOINT leye = {0}, reye = {0};
    //channels = 1;
    imread( picname, channels, 1, im );
    GetImagePyramid( im->h, im->w, im->tt.data, im->s, NumPyramidLevels,0, 0, 0, 0, leye, reye, 0, 0, 0, 0 );
    h = im->h;
    w = im->w;
    for ( i = 0; i < NumPyramidLevels; ++i ) {
      char buf[ 256 ];
      img_t im1[1] = {0};
      _snprintf( buf, 256, "Pyr%d.bmp", i );
      IMINIT(im1, h, w, Pyr[ i ], w, 1, 1);
      imwrite( buf, im1 );
      h /= 2;
      w /= 2;
    }
    imfree( im );
  }
  return 0;
}

#define SOLVE_4X4(a0,a1,a2,a3,b0,b1,b2,b3,c0,c1,c2,c3,d0,d1,d2,d3) \
  -(+(b2)*(c0)*(a3)*(d1)-(a0)*(b1)*(c2)*(d3)+(a2)*(c1)*(b3)*(d0)+(b1)*(a2)*(c0)*(d3) \
    +(a0)*(b1)*(c3)*(d2)+(c2)*(b1)*(a3)*(d0)+(c2)*(a0)*(b3)*(d1)-(a0)*(b3)*(c1)*(d2) \
    -(b3)*(c0)*(a2)*(d1)-(a3)*(c1)*(b2)*(d0)-(b1)*(a3)*(c0)*(d2)+(a0)*(b2)*(c1)*(d3) \
    -(c3)*(b1)*(a2)*(d0)-(c3)*(a0)*(b2)*(d1)-(b0)*(c2)*(a3)*(d1)+(b0)*(a3)*(c1)*(d2) \
    -(b0)*(a2)*(c1)*(d3)+(b0)*(c3)*(a2)*(d1)+(a1)*(b0)*(c2)*(d3)-(a1)*(b0)*(c3)*(d2) \
    +(a1)*(b3)*(c0)*(d2)-(a1)*(b2)*(c0)*(d3)+(a1)*(c3)*(b2)*(d0)-(a1)*(c2)*(b3)*(d0))

int test_SOLVE_4X4() {
  int dd = SOLVE_4X4(
             1, 2, 3, 4,
             15, 26, 7, 8,
             29, 10, 11, 12,
             33, 114, 15, 16
           );
  return 0;
}

#undef SELECT_PT_COLOR
#define PT_COLOR CC_RGB(255, 0, 0)        // 点的颜色
#define SELECT_PT_COLOR CC_RGB(0, 255, 0) // 被选择点的颜色

#include "kouhong.inl"


int drawkonhon(int h, int w, unsigned char* img, int step, int cn, const float* pt0, int n1, const float* pt1, int n2, int clr) {
  FPOINT pts[256];
  int i, len[] = {n1, n2};
  img_t im[1] = {0};
  IMINIT(im, h, w, img, step, cn, 1);
  for ( i = 0; i < n1; ++i ) {
    const float* p = pt0 + i*2;
    pts[i].x = p[0];
    pts[i].y = p[1];
  }
  for ( i = 0; i < n2; ++i ) {
    const float* p = pt1 + (n2-1-i)*2;
    pts[n1+i].x = p[0];
    pts[n1+i].y = p[1];
  }
  //imdrawaa_poly_solid(im, false, NULL, pts, len, 2, clr);
  return 0;
}

int draw_face_pt( img_t* im, const IRECT* pclip, CvPoint2D32f* pt, int point_number,
                  int* _contours_ending_points, int contours_ending_points_len,
                  int radius, int draw_id ) {
  int i;
  if (0) {
    for ( i = 0; i < point_number; ++i ) {
      int Before, After;
      IPOINT pt1, pt2;
      GetBeforeAfterPts( i, _contours_ending_points, contours_ending_points_len, &Before, &After );
      pt1.x = ( int ) pt[ i ].x;
      pt1.y = ( int ) pt[ i ].y;
      pt2.x = ( int ) pt[ Before ].x;
      pt2.y = ( int ) pt[ Before ].y;
      //imdraw_circle(im, pclip, pt1.x, pt1.y, radius, PT_COLOR, 1);
      imdrawaa_circle(im, pclip, pt[ i ].x, pt[ i ].y, radius, 0, PT_COLOR, 1);
      //imdrawaa_circle( im, pclip, pt1.x, pt1.y, radius, PT_COLOR, 0, 0 );
      //imdraw_line( im, pclip, pt1, pt2, CC_RGB( 255, 255, 0 ), 1 );
      imdrawaa_line( im, pclip, pt[ i ].x, pt[ i ].y, pt[ Before ].x, pt[ Before ].y, CC_RGB( 255, 255, 0 ), 1 );
      if ( draw_id ) {
        char buf[ 25 ];
        _snprintf( buf, 25, "%d", i );
        //xDrawText( h, w, ptr, step, pix_size, pt1.y, pt1.x,
        //           "新宋体", MIN( MAX( radius, 5 ) + 5, 20 ), 0, 0, 1, buf, SELECT_PT_COLOR );
      }
    }
  }
  if (1) {
    int j=5;
    int i0, i1, i2;
    i0 = j>0 ? _contours_ending_points[j-1] : 0;
    i1 = _contours_ending_points[j];
    i2 = _contours_ending_points[j+1];
    if (0) {
      for ( i = i0; i < i1; ++i ) {
        int Before, After;
        IPOINT pt1, pt2;
        GetBeforeAfterPts( i, _contours_ending_points, contours_ending_points_len, &Before, &After );
        pt1.x = ( int ) pt[ i ].x;
        pt1.y = ( int ) pt[ i ].y;
        pt2.x = ( int ) pt[ Before ].x;
        pt2.y = ( int ) pt[ Before ].y;
      //imdraw_circle(im, pclip, pt1.x, pt1.y, radius, PT_COLOR, 1);
      imdrawaa_circle(im, pclip, pt[ i ].x, pt[ i ].y, radius, 0, PT_COLOR, 1);
      //imdrawaa_circle( im, pclip, pt1.x, pt1.y, radius, PT_COLOR, 0, 0 );
      //imdraw_line( im, pclip, pt1, pt2, CC_RGB( 255, 255, 0 ), 1 );
      imdrawaa_line( im, pclip, pt[ i ].x, pt[ i ].y, pt[ Before ].x, pt[ Before ].y, CC_RGB( 255, 255, 0 ), 1 );
        if ( draw_id ) {
          char buf[ 25 ];
          _snprintf( buf, 25, "%d", i );
          //xDrawText( h, w, ptr, step, pix_size, pt1.y, pt1.x,
          //           "新宋体", MIN( MAX( radius, 5 ) + 5, 20 ), 0, 0, 1, buf, SELECT_PT_COLOR );
        }
      }
    }
    
    FPOINT ipt[128];
    int len[2] = {i1-i0, i2-i1};
    int len2 = len[0] + len[1];
    IRECT bound = iRECT(pt[i0].x, pt[i0].y, pt[i0].x, pt[i0].y);
    for ( i = 0; i < i1-i0; ++i ) {
      ipt[i].x = pt[i0+i].x;
      ipt[i].y = pt[i0+i].y;
      iRectBoundUpdate(&bound, pt[i0+i].x, pt[i0+i].y);
    }
    for ( i = 0; i < i2-i1; ++i ) {
      ipt[i1-i0+i].x = pt[i2-1-i].x;
      ipt[i1-i0+i].y = pt[i2-1-i].y;
      iRectBoundUpdate(&bound, pt[i2-1-i].x, pt[i2-1-i].y);
    }
    bound = iRectInflate1(bound, 40);
    //imdrawaa_rect(im, 0, NULL, dRECT(bound.l, bound.t, bound.r, bound.b), 0, _rgb(0,1,1), 2);
    if (1) {
      img_t im1[10] = {0};
      int i=0;
      //img_t* hsv = im1;
      //img_t* hue = im1+i++;
      img_t* gry = im1+i++;
      img_t* palette = im1+i++;
      //imcolorcvt(im, hsv, T_RGB, T_HSV);
      //imsplit(hsv, hue, 0, 1);
      //imhisteq(hue, hue);
      imsetsize(gry, im->h, im->w, 1, 1);
      //imdrawaa_poly_solid(im, false, pclip, ipt, len, 2, _RGBA(255, 111, 111, 100));
      COLOR clr = _RGBA(_rgb_[0], _rgb_[1], _rgb_[2], _rgb_[3]);
      //imdrawaa_poly_solid(im, false, pclip, ipt, len, 2, clr);
      drawkonhon(im->h, im->h, im->tt.data, im->s, im->c, &pt[i0].x, i1-i0, &pt[i1].x, i2-i1, clr);
      imdrawaa_poly_solid(gry, false, pclip, ipt, len, 2, _RGB(255, 255, 255));
      if (0) {
        img_t im2[1] = {0};
        img_t gry2[1] = {0};
        sys_chdir("E:/code/cstd/matlab/facekohon");
        imsubref(im, bound, im2);
        imwrite("im.bmp", im2);
        imsubref(gry, bound, gry2);
        imwrite("mask.bmp", gry2);
      }
      if (1) {
        imgauss(gry, gry, 5);
        imshow(gry);
      }
      if (0) {
        double m1[3];
        double v1[3];
        img_t im2[1] = {0};
        img_t gry2[1] = {0};
        //imerode(gry, gry, cvSize(3, 3), MORPH_RECT, iPOINT(1,1), BD_CONSTANT);
        imsubref(im, bound, im2);
        imsubref(gry, bound, gry2);
        
        if (0) {
#if 0
          const int palette_size = 3;
          uchar input_palette[palette_size*4] = {0};
          uchar output_palette[palette_size*4] = {0};
          gridacc_kmeans(im2->h, im2->w, im2->tt.data, im2->s, im2->c, 2, palette_size, input_palette);
          palette2img(palette_size, input_palette, 40, palette);
#endif
        }
        
        if (0) {
          img_mean_var(im2->h, im2->w, im2->tt.data, im2->s, im2->c, gry2->tt.data, gry2->s, m1, v1);
          //img_pdf_mask(im2->h, im2->w, im2->tt.data, im2->s, im2->c, gry2->tt.data, gry2->s, m1, v1);
          //imgauss(gry, gry, 2);
          //img_mask_blender(im2->h, im2->w, im2->tt.data, im2->s, im2->c, gry2->tt.data, gry2->s, _RGB(_rgb_[0], _rgb_[1], _rgb_[2]));
        }
        if (0) {
#if 0
          uchar input_palette[8] = {0};
          uchar* output_palette = input_palette+4;
          img_mean_var(im2->h, im2->w, im2->tt.data, im2->s, im2->c, gry2->tt.data, gry2->s, m1, v1);
          input_palette[0] = BOUND(m1[0], 0, 255);
          input_palette[1] = BOUND(m1[1], 0, 255);
          input_palette[2] = BOUND(m1[2], 0, 255);
          input_palette[3] = 255;
          output_palette[0] = BOUND(_rgb_[2], 0, 255);
          output_palette[1] = BOUND(_rgb_[1], 0, 255);
          output_palette[2] = BOUND(_rgb_[0], 0, 255);
          output_palette[3] = 255;
          palette2img(2, input_palette, 40, palette);
          //imwrite("E:/code/c/Image/recolor/im2.png", im2);
          //memcpy(output_palette, input_palette, 4);
          drawRes(input_palette, output_palette, 1, 2, im2->h, im2->w,
            im2->tt.data, im2->s, im2->tt.data, im2->s, im->c, gry2->tt.data, gry2->s, 0.5);
#endif
        }
        imshow(im2);
        imshow(palette);
      }
      //imshow(gry);
      //imshow(hue);
      imfrees(im1, 10);
    }
  }
  return 0;
}

#ifndef _DEBUG
#undef DrawRectangle2
#define DrawRectangle2(h, w, A, al, ai, rt, color, lw)
#endif

int mat_copy( int h, int w, unsigned char* A, int al, int ai,
            unsigned char* B, int bl, int bi, int cn, int is_alpha ) {
  int i, j, k = 0;
      unsigned char* A1 = A;
      unsigned char* B1 = B;
  if ( is_alpha && 3 == cn ) {
    for ( i = 0; i < h; ++i, A += al, B += bl ) {
      unsigned char* A0 = A;
      unsigned char* B0 = B;
      
      for ( j = 0; j < w; ++j, A0 += ai, B0 += bi ) {
        unsigned char a = A0[ 3 ];
        if ( a == 255 ) {
          B0[ 0 ] = A0[ 0 ];
          B0[ 1 ] = A0[ 1 ];
          B0[ 2 ] = A0[ 2 ];
        } else {
          alpha_composite( B0[ 0 ], A0[ 0 ], a, B0[ 0 ] );
          alpha_composite( B0[ 1 ], A0[ 1 ], a, B0[ 1 ] );
          alpha_composite( B0[ 2 ], A0[ 2 ], a, B0[ 2 ] );
        }
      }
    }
  }
  return 0;
}

void CalcWarpParameters( real x, real y, real x1, real y1,
                        real x2, real y2, real x3, real y3,
                        real*alpha, real *beta, real *gamma ) {
  real c = ( + x2 * y3 - x2 * y1 - x1 * y3 - x3 * y2 + x3 * y1 + x1 * y2 );
  *alpha = ( y * x3 - y3 * x + x * y2 - x2 * y + x2 * y3 - x3 * y2 ) / c;
  *beta = ( -y * x3 + x1 * y + x3 * y1 + y3 * x - x1 * y3 - x * y1 ) / c;
  *gamma = 1 - *alpha - *beta;
}

int aam_wrap(int ah, int aw, const unsigned char* a, int al,
             int bh, int bw, unsigned char* b, int bl, int cn,
             const FPOINT* apt, const FPOINT* bpt, const int* tris, int nTriangles ) {
  int ll = 0;
  real alpha, belta, gamma;
  int ind1, ind2, ind3, i, j, k, len = 0;
  // real x, y, x1, y1, x2, y2, x3, y3, c;
  int left, right, top, bottom;
  // then we find the triangle that the point lies in
  for ( k = 0; k < nTriangles; k++ ) {
    ind1 = tris[ k * 3 + 0 ];
    ind2 = tris[ k * 3 + 1 ];
    ind3 = tris[ k * 3 + 2 ];
    left = (int)MIN3(bpt[ ind1 ].x, bpt[ ind2 ].x, bpt[ ind3 ].x);
    right = (int)MAX3(bpt[ ind1 ].x, bpt[ ind2 ].x, bpt[ ind3 ].x);
    top = (int)MIN3(bpt[ ind1 ].y, bpt[ ind2 ].y, bpt[ ind3 ].y);
    bottom = (int)MAX3(bpt[ ind1 ].y, bpt[ ind2 ].y, bpt[ ind3 ].y);
    left = MAX(left, 0);
    right = MIN(right, bw-1);
    top = MAX(top, 0);
    bottom = MIN(bottom, bh-1);
    for ( i = top; i <= bottom; ++i ) {
      for ( j = left; j <= right; ++j ) {
        // calculate alpha and belta for warp
        CalcWarpParameters( ( real ) j, ( real ) i, bpt[ ind1 ].x, bpt[ ind1 ].y,
          bpt[ ind2 ].x, bpt[ ind2 ].y, bpt[ ind3 ].x, bpt[ ind3 ].y, &alpha, &belta, &gamma );
        // the point(j,i) is located in the k-th triangle
        if ( alpha > 0.f && belta > 0.f && gamma > 0.f
          && alpha < 1.f && belta < 1.f && gamma < 1.f ) {
          float x, y;
          int x0, y0;
          int ix, iy;
          unsigned char* B1 = b + i * bl+j*3;
          x = alpha * apt[ ind1 ].x + belta * apt[ ind2 ].x + gamma * apt[ ind3 ].x;
          y = alpha * apt[ ind1 ].y + belta * apt[ ind2 ].y + gamma * apt[ ind3 ].y;
          x0 = (int)(x*SHIFT1);
          y0 = (int)(y*SHIFT1);
          ix = (int)(x);
          iy = (int)(y);
          B1[0] = a[iy*al+ix*3+0];
          B1[1] = a[iy*al+ix*3+1];
          B1[2] = a[iy*al+ix*3+2];
          //INTER_BILINEARITY(a, x0, y0, al, cn, B1);
          ++len;
          // make sure each point only located in only one triangle
          //break;
        }
      }
    }
  }
  return len;
}


#if 0
int starface1(int stdh, int stdw, const unsigned char* stdimg, int stdstep,
              int h, int w, unsigned char* rgb, int step, int channels ) {
  int ret=0;
  int i;
  delaunay_t dp = {0};
  {
    asmfit_param_t ap = {0};
    {
      uchar* stdimggry;
      real* ptx=0, *pty=0;
      stdimggry = MALLOC(uchar, stdh* stdw);
      colorcvt( stdh, stdw, stdimg, stdstep, stdimggry, stdw, T_BGR, T_GRAY );
      ap.height = stdh, ap.width = stdw, ap.data = stdimggry, ap.datastep = stdw;
      stdh = stdh, stdw = stdw;
      //ap.leye = leye, ap.reye = reye;
      ap.asm_mode = asmTrainData_all;
      if (NULL==ap.buf) {
        asmfit_memsize(&ap);
        ap.shape_result = MALLOC(FPOINT, ap.shape_result_len);
        ap.buf = MALLOC(char, ap.buflen);
      }
      
      //cvShowImage( "sadf", stdh, stdw, stdimg, stdstep, 3 );
      ret = asmfit_process2( &ap );
      free(stdimggry);
      if (ret) {
        ptx = MALLOC(real, ap.shape_result_len);
        pty = MALLOC(real, ap.shape_result_len);
        for (i=0; i<ap.shape_result_len; ++i) {
          ptx[i] = ap.shape_result[i].x;
          pty[i] = ap.shape_result[i].y;
        }
        dp.ptx = ptx, dp.pty = pty;
        dp.point_num = ap.shape_result_len;
        if (NULL==dp.buf) {
          delaunay_memsize(&dp);
          dp.buf = MALLOC(char, dp.buflen);
        }
        delaunay_process( &dp );
      }
    }
    FREE(ap.shape_result);
    FREE(ap.buf);
  }
  if (ret) {
    asmfit_param_t ap = {0};
    int NbOfPoints;
    unsigned char *gry = 0;
    gry = MALLOC( unsigned char, h * w );
    colorcvt( h, w, rgb, step, channels, gry, w, 1, T_BGR, T_GRAY );
    ap.height = h, ap.width = w, ap.data = gry, ap.datastep = w;
    //ap.leye = leye, ap.reye = reye;
    ap.asm_mode = asmTrainData_all;
    if (NULL==ap.buf) {
      asmfit_memsize(&ap);
      ap.shape_result = MALLOC(FPOINT, ap.shape_result_len);
      ap.buf = MALLOC(char, ap.buflen);
    }
    ret = asmfit_process2( &ap );
    if (ret) {
      real* ptx=0, *pty=0;
      ptx = MALLOC(real, ap.shape_result_len);
      pty = MALLOC(real, ap.shape_result_len);
      for (i=0; i<ap.shape_result_len; ++i) {
        ptx[i] = ap.shape_result[i].x;
        pty[i] = ap.shape_result[i].y;
      }
      aam_wrap(stdh, stdw, stdimg, stdstep, h, w, rgb, step, channels,dp.ptx, dp.pty, ptx, pty, dp.tris, dp.tri_num);
      NbOfPoints = ap.shape_result_len;
      if (0) {
        //draw_face_pt( h, w, rgb, step, channels, ap.shape_result, NbOfPoints, ap.contours_ending_points, ap.contours_ending_points_len, 3, 0, 0 );
        //draw_shape1(h, w, rgb, step, channels, dp.point_num, ptx, pty, dp.edges, dp.edge_num);
        //DrawCircle( h, w, rgb, step, channels, ap.leye, 3, CC_RGB( 0, 255, 0 ), -1, 0, 0 );
        //DrawCircle( h, w, rgb, step, channels, ap.reye, 3, CC_RGB( 0, 255, 0 ), -1, 0, 0 );
      }
      //DrawRectangle2( height, width, rgb, step, channels, *face, CC_RGB( 255, 0, 0 ), 1 );
      
      printf("%d, %d  ", ap.leye.x, ap.leye.y);
      FREE(ptx);
      FREE(pty);
    }
    FREE(gry);
    FREE(ap.shape_result);
    FREE(ap.buf);
  }
  FREE((char*)dp.ptx);
  FREE((char*)dp.pty);
  FREE(dp.buf);
  return 0;
}
#endif

int fpt_zoom(const FPOINT* pt, int n, double ss, FPOINT* pt2) {
  double x=0, y=0;
  int i;
  for (i=0; i<n; ++i) {
    x += pt[i].x;
    y += pt[i].y;
  }
  x/=n;
  y/=n;
  for (i=0; i<n; ++i) {
    pt2[i].x = (float)((pt[i].x - x)*ss + x);
    pt2[i].y = (float)((pt[i].y - y)*ss + y);
  }
  return 0;
}
// 方脸
int fpt_rectfix(FPOINT* pt, int n, double ss, FPOINT* pt2) {
  int i;
  double x=0, y=0;
  double minx, miny, maxx, maxy, h2, w2;
  if (n<2) {
    return 0;
  }
  minx = pt[0].x, miny = pt[0].y, maxx = pt[0].x, maxy = pt[0].y;
  for (i=1; i<n; ++i) {
    minx = MIN(minx, pt[i].x);
    miny = MIN(miny, pt[i].y);
    maxx = MAX(maxx, pt[i].x);
    maxy = MAX(maxy, pt[i].y);
  }
  h2 = (maxy-miny)*0.5;
  w2 = (maxx-minx)*0.5;
  for (i=0; i<n; ++i) {
    x += pt[i].x;
    y += pt[i].y;
  }
  x/=n;
  y/=n;
  //pt[0].x = x;
  //pt[0].y = y+1;
  for (i=0; i<n; ++i) {
    double dy = fabs(pt[i].y-y)/h2;
    double dx = fabs(pt[i].x-x)/w2;
    double dd2 = sqrt(dx*dx+dy*dy);
    double dd = atan2(dy, dx);
    double ss1;
    double d1 = M_PI/4 - fabs(dd-M_PI/4);
    dd = 1 + 0.4 * d1/(M_PI/4);
    //printf("%f\n", d1);
    ss1 = dd*ss;
    if (dd2<0.8 || pt[i].y<y) {
      ss1 = 1;
    }
    pt2[i].x = (float)((pt[i].x - x)*ss1 + x);
    pt2[i].y = (float)((pt[i].y - y)*ss1 + y);
  }
  return 0;
}


int smooth_pts(FPOINT* pts) {
  enum {NPTS = 101, MAXPTBUF = 3};
  static CvPoint2D32f ptbuf[MAXPTBUF][NPTS];
  static int nptbuf=0;
  CvPoint2D32f* p = ptbuf[nptbuf%MAXPTBUF];
  MEMCPY(p, pts, 101);
  ++nptbuf;
  int i, j;
  int n = MIN(nptbuf, MAXPTBUF);
  for (i=0; i<NPTS; ++i) {
    pts[i].x = pts[i].y = 0;
  }
  for (j=0; j<n; ++j) {
    p = ptbuf[j];
    for (i=0; i<NPTS; ++i) {
      pts[i].x += p[i].x;
      pts[i].y += p[i].y;
    }
  }
  for (i=0; i<NPTS; ++i) {
    pts[i].x /= n;
    pts[i].y /= n;
  }
  return 0;
}

int findfaceasm(img_t* im, const IRECT* pclip, int r ) {
  int ret=0;
  asmfit_param_t ap = {0};
  img_t im1[10] = {0};
  img_t* stdimggry = im1;
  real* ptx=0, *pty=0;
  imcolorcvt( im, stdimggry, T_BGR, T_GRAY );
  ap.height = im->h, ap.width = im->w, ap.data = stdimggry->tt.data, ap.datastep = stdimggry->s;
  //ap.leye = leye, ap.reye = reye;
  ap.asm_mode = asmTrainData_all;
  if (NULL==ap.buf) {
    asmfit_memsize(&ap);
    ap.shape_result = MALLOC(CvPoint2D32f, ap.shape_result_len);
    ap.buf = MALLOC(char, ap.buflen);
  }
  //cvShowImage( "sadf", stdh, stdw, stdimg, stdstep, 3 );
  ret = asmfit_process2( &ap );
  if (0) {
    int npt = ap.shape_result_len+4;
    FPOINT* pt = MALLOC(FPOINT, npt);
    FPOINT* pt2 = MALLOC(FPOINT, npt);
    int i, n = npt * 8 + 20;
    int* edges = MALLOC(int, n);
    int* tris = MALLOC(int, n);
    int edge_num, tri_num;
    img_t im1[10] = {0};
    npt-=4;
    MEMCPY(pt, ap.shape_result, npt);
    pt[npt++] = fPOINT(0, 0);
    pt[npt++] = fPOINT(0, im->h);
    pt[npt++] = fPOINT(im->w, 0);
    pt[npt++] = fPOINT(im->w, im->h);
    Delaunay(npt, pt, edges, &edge_num, tris, &tri_num);
    //fpt_zoom(pt, npt-4, 1.5, pt2);
    fpt_rectfix(pt, npt-4, 1., pt2);
    MEMCPY(pt2+npt-4, pt+npt-4, 4);
    for (i=0; i<edge_num; ++i) {
      IPOINT pt[2] = {pt[edges[2*i]].x, pt[edges[2*i]].y, pt[edges[2*i+1]].x, pt[edges[2*i+1]].y};
      //imdraw_line( im, pclip, pt[0], pt[1], CC_RGB( 255, 0, 0 ), 1 );
    }
    imclone2(im, im1);
    //aam_wrap(im->h, im->w, im->tt.data, im->s, im1->h, im1->w, im1->tt.data, im1->s, im->c, pt, pt2, tris, tri_num);
    imshow(im1);cvWaitKey(10);
    imfrees(im1, 10);
    FREE(edges);
    FREE(tris);
    FREE(pt);
    FREE(pt2);
  }
  if (0) {
    int npt = ap.shape_result_len;
    FPOINT* pt = ap.shape_result;
    //fpt_zoom(pt, npt, 1.5, pt);
    fpt_rectfix(pt, npt, 1., pt);
  }
  smooth_pts(ap.shape_result);
  draw_face_pt(im, pclip, ap.shape_result, ap.shape_result_len,
    ap.contours_ending_points, ap.contours_ending_points_len, r, 1);
  //imshow(im);
  FREE(ap.shape_result);
  FREE(ap.buf);
  imfrees(im1, 10);
  return 0;
}

int test_asm_fit() {
  char buf[ 256 ] = {0};
  char buf2[ 256 ] = {0};
  int i=0;
  FILE* pfout;
  unsigned char *rgb = 0, *gry = 0;
  int fram = 0;
  char ch = 0;
  int icoef_lim = 18;
  int radius = 3;
  int draw_id = 0;
  FILE* plisttxt;
  
  //sys_chdir("D:/doc/pic/");
  sys_chdir("C:/pic/yiwei5/0/");
  sys_chdir("E:/pub/pic");
  sys_chdir("E:/pub/bin/face/facestd/testcas");
  //sys_mkdir( "./debug" );
  sys_mkdir( "./fit" );
  pfout = fopen( "./fit.txt", "wb" );

  plisttxt = fopen("list.txt", "rb");
  for ( fram = 0; 'q' != ch && 'Q' != ch && fgets(buf, 256, plisttxt)>0; ++fram ) {
    img_t im[1]={0};
    strtrim(buf, "\n\r");
    imread(buf, 3, 1, im);
    if (1) {
      utime_start( _start_time );
      findfaceasm(im, 0, 2);
      
      printf( "%6.6f %d\n", utime_elapsed( _start_time ), fram );
      if ( pfout ) {
        fprintf( pfout, "\r\n" );
      }
      
      imshow( im ); ch = cvWaitKey( -1 );
      imfree(im);
      _snprintf(buf2, 256, "./fit/%s.bmp", buf);
      //imwrite2(buf2, im);
    }
    FREE( rgb );
    FREE( gry );
  }
  
  //asm_free( &asm );
  if ( pfout ) {
    fclose( pfout );
  }
  return 0;
}

#ifdef _WIN32
#define cap_open capdshow_open
#else
#define cap_open capv4l_open
#endif

#include "cap/capimgseq.inl"

int test_asm_fit2() {
  int aa = printf("211");
  char buf[ 256 ] = {0};
  cap_t cap[1] = {0};
  char wndname[] = "Edge";
  char tbarname[] = "Threshold";
  int i=0;
  unsigned char *rgb = 0, *gry = 0;
  int fram = 0;
  char ch = 0;
  int icoef_lim = 18;
  int radius = 3;
  int draw_id = 0;
  img_t im[10] = {0};
  int iscam = 1;

  cvNamedWindow( "cam", 0 );
  //cvCreateTrackbar( "nPelsSearch", "cam", &( asm->nPelsSearch ), 15, 0 );
  //cvCreateTrackbar( "max_iter", "cam", &( asm->max_iter ), 20, 0 );
  //cvCreateTrackbar( "coef_lim", "cam", &( icoef_lim ), 30, 0 );
  cvCreateTrackbar( "r", "cam", _rgb_+0, 255, 0 );
  cvCreateTrackbar( "g", "cam", _rgb_+1, 255, 0 );
  cvCreateTrackbar( "b", "cam", _rgb_+2, 255, 0 );
  cvCreateTrackbar( "a", "cam", _rgb_+3, 255, 0 );
  //cap_open( cap, 0, 640, 480 );
  if (iscam) {
    cap_open( cap, 0, 1280, 960 );
  } else {
    sys_chdir("F:/pub/bin/photo/yiwei");
    capimgseq_open(cap, "list.txt", 3 );
  }
  //cap = cap_open_imgseq( "F:/pub/bin/ASM/pic/1/list.txt", 3 );
  
  if ( NULL == cap->x ) {
    return 0;
  }
  
  for ( ; 'q' != ch && 'Q' != ch; ++fram ) {
    img_t* rgb = im+1;
    cap_getframe(cap, im, CAP_RGB);
    
    //asm->coef_lim = icoef_lim / 10.;
    //memcpy(rgb, img, step*height);
    imclone2(im, rgb);
    //FLIP2D( height, step, rgb, step, 1, t );
    // imwrite("bbb.bmp",height, width, rgb, step, 3);
    // IMTRANS(height, width, rgb, step, 3, gry, width, 1, BGR2GRAY);
    
    if (1) {
      utime_start( _start_time );
      findfaceasm(im, 0, 2);
      printf( "%6.6f %d\n", utime_elapsed( _start_time ), fram );
      imshow2( "cam", im );
      //_snprintf(buf, 256, "%sfit/_00_%d.bmp", TEST_PIC_PATH, fram);
      //imwrite(buf, height, width, rgb, step, channels);
    }
    if (0) {
      utime_start( _start_time );
      
      //starface(height, width, rgb, step, channels, buf);
      printf( "%6.6f %d\n", utime_elapsed( _start_time ), fram );
      //if ( pfout ) {
      //  fprintf( pfout, "\r\n" );
      //}
      
      imshow2( "cam", rgb );
      //_snprintf(buf, 256, "%sfit/_00_%d.bmp", TEST_PIC_PATH, fram);
      //imwrite(buf, height, width, rgb, step, channels);
    }
    ch = cvWaitKey( iscam ? 10 : -1);
  }
  cap_close( cap );
  
  //asm_free( &asm );
  imfrees(im, 10);
  return 0;
}
