
#include "cv/utils.inl"
#include "draw/imdraw.inl"

// 用 k - means 对高斯分布的随机样本进行聚类
int test_kmeans()
{
  enum { cluster_count = 5, sample_count = 1000};
  COLOR color_tab[cluster_count];
  img_t im[1] = {0};
  CvRNG rng = cvRNG(0xffffffff);

  imsetsize(im, 1000, 1000, 3, 1);

  color_tab[0] = CV_RGB(255, 0, 0);
  color_tab[1] = CV_RGB(0, 255, 0);
  color_tab[2] = CV_RGB(100, 100, 255);
  color_tab[3] = CV_RGB(255, 0, 255);
  color_tab[4] = CV_RGB(255, 255, 0);

  cvNamedWindow("clusters", 1);

  for (;;) {
    int i, k;
    IPOINT points[1000] = {0};
    int clusters[1000] = {0};

    /* generate random sample from multigaussian distribution */
    for (k = 0; k < cluster_count; k++) {
      IPOINT center, off = iPOINT(im->w / 10, im->h / 10);
      int start_row, rows;
      center.x = cvRandInt(&rng) % im->w;
      center.y = cvRandInt(&rng) % im->h;
      start_row = k * sample_count / cluster_count;
      rows = (k == cluster_count - 1 ? sample_count : (k + 1) * sample_count / cluster_count) - start_row;
      icvRandn_32s(&rng, &(points[start_row].x), 2, cvSize(2, rows), 2, &center.x, &off.x);
    }

    /* shuffle samples */
    for (i = 0; i < sample_count / 2; i++) {
      IPOINT* pt1 = points + cvRandInt(&rng) % sample_count;
      IPOINT* pt2 = points + cvRandInt(&rng) % sample_count;
      IPOINT temp;
      CV_SWAP(*pt1, *pt2, temp);
    }

    cvKMeans2_32s(&points->x, sample_count, 2, 2, cluster_count, clusters);

    imzero(im);

    for (i = 0; i < sample_count; i++) {
      IPOINT pt = points[i];
      int cluster_idx = clusters[i];
      imdraw_circle1(im, pt.x, pt.y, 2, color_tab[cluster_idx], 0);
    }

    imshow(im);

    if (cvWaitKey(0) == 27) { // 'ESC'
      break;
    }
  }

  return 0;
}

