

#include "ui/window.inl"
#include "draw/imdraw.inl"



#if 0
#include "test_kdtree.inl"
#include "img/color.inl"
#include "math/imath.inl"
#include "ml/boost.inl"

#include "test_KMeans.inl"
#include "luvhog.inl"

//#include "test_mem_fill_border.inl"
//#include "test_luvhog_feat.inl"

//#include "test_adaboost.inl"

//#include "test_boost.inl"
#include "test_hog_boost.inl"
#include "test_haar_boost.inl"
#endif

#include "cmath.h"
#include "cv/rand.inl"
#include "ui/draw.h"

#include "slp.inl"

int test_slp() {
  enum { cluster_count = 5, sample_count = 100};
  COLOR color_tab[cluster_count];
  img_t im[1] = {0};
  CvRNG rng = -1;
  
  test_slp1();
  return 0;
  imsetsize(im, 512, 512, 3, 1);
  
  color_tab[0] = CV_RGB(255, 0, 0);
  color_tab[1] = CV_RGB(0, 255, 0);
  color_tab[2] = CV_RGB(100, 100, 255);
  color_tab[3] = CV_RGB(255, 0, 255);
  color_tab[4] = CV_RGB(255, 255, 0);
  
  cvNamedWindow("clusters", 1);
  
  for (;;) {
    int i, j, k;
    IPOINT points[sample_count] = {0};
    int clusters[sample_count] = {0};
    
    /* generate random sample from multigaussian distribution */
    for (k = 0; k < cluster_count; k++) {
      IPOINT center, off = iPOINT(im->w / 20, im->h / 20);
      int start_row, rows;
      center.x = cvRandInt() % im->w;
      center.y = cvRandInt() % im->h;
      start_row = k * sample_count / cluster_count;
      rows = (k == cluster_count - 1 ? sample_count : (k + 1) * sample_count / cluster_count) - start_row;
      icvRandn_32s(&rng, &(points[start_row].x), 2, cvSize(2, rows), 2, &center.x, &off.x);
      for (j=0; j<rows; ++j) {
        clusters[start_row+j] = k;
      }
    }
    
    imzero(im);
    
    for (i = 0; i < sample_count; i++) {
      IPOINT pt = points[i];
      int cluster_idx = clusters[i];
      imdrawaa_ellipse(im, 0, pt.x, pt.y, 2, 2, color_tab[cluster_idx], 0, 0);
    }
    
    imshow(im);
    
    if (cvWaitKey(0) == 27) { // 'ESC'
      break;
    }
  }
  return 0;
}

int test_ml()
{
#if 1
  test_slp();
#else
  test_kmeans();
  test_dist();
  test_kdtree();
  haar_boost_train();
  test_haar_boost();
  hog_boost_train();
  test_hog_boost();
  test_boost_train();
  double t = 255.*(1 << 8) / sqrt(255 * 255 * 2);
  test_mem_fill_border();
  test_luvhog_feat();
#endif
  return 0;
}
