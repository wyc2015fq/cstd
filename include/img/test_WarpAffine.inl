
//OpenCV: 三对点计算仿射变换
// 仿射变换.cpp : 定义控制台应用程序的入口点。

#include "cv/thresh.inl"
#include "cv/imgwarp.inl"

int test_WarpAffine()
{
  DPOINT srcTri[3], dstTri[3];
  double rot_mat[2 * 3] = {0};
  double warp_mat[2 * 3] = {0};
  img_t src[2] = {0};
  img_t* dst = src + 1;
  CvSize size;

  _chdir("D:/bak/pub/pic");
  imread("lena512.jpg", 3, 1, src);
  imsetsamesize(dst, src);
  size = cvSize(src->w, src->h);

  if (0) {
    double a1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    double a[] = {
      0,      0,      1,      0,      0,      0,      0,      0,
      512,      0,      1,      0,      0,      0, -262144,      0,
      0,    512,      1,      0,      0,      0,      0,      0,
      512,    512,      1,      0,      0,      0, -262144, -262144,
      0,      0,      0,      0,      0,      1,      0,      0,
      0,      0,      0,    512,      0,      1,      0,      0,
      0,      0,      0,      0,    512,      1,      0, -262144,
      0,      0,      0,    512,    512,      1, -262144, -262144
    };
    double b[] = {    0,    512,      0,    512,      0,      0,    512,    512};
    enum {h = 8};
    double w[h] = {0};
    double x[h] = {0};
    double u[h * h] = {0};
    double v[h * h] = {0};
    cvSVD_64f(a, h, w, 1, u, h, v, h, CC_SVD_U_T + CC_SVD_V_T, h, h);
    cvSVBkSb_64f(w, 1, u, h, v, h, b, 1, x, 1, CC_SVD_U_T + CC_SVD_V_T, h, h, 1);
    PRINT2D("%6.4f, ", "\n", h, h, u, h, 1);
    PRINT2D("%6.4f, ", "\n", h, h, v, h, 1);
    PRINT2D("%6.4f, ", "\n", 1, h, w, 1, 1);
    PRINT2D("%6.4f, ", "\n", 1, h, x, 1, 1);
    PRINT2D("%6.4f, ", "\n", 1, h, w, 1, 1);
  }

  if (0) {
    //COMPUTE WARP MATRIX
    srcTri[0] = dPOINT(0, 0);          //src Top left
    srcTri[1] = dPOINT(src->w - 1, 0);    //src Top right
    srcTri[2] = dPOINT(0, src->h - 1);          //src Bottom left
    dstTri[0] = dPOINT(src->w * 0.0, src->h * 0.33);  //dst Top left
    dstTri[1] = dPOINT(src->w * 0.85, src->h * 0.25); //dst Top right
    dstTri[2] = dPOINT(src->w * 0.15, src->h * 0.7); //dst Bottom left
    cvGetAffineTransform(srcTri, dstTri, warp_mat); //由三对点计算仿射变换
    cvWarpAffine_8u(src->tt.data, src->s, size, dst->tt.data, dst->s, size, warp_mat, src->c, 0, 0);

    imshow(src);
    imshow(dst);
    cvWaitKey(-1);

  }

  //COMPUTE ROTATION MATRIX
  if (0) {
    DPOINT center = dPOINT(src->w / 2., src->h / 2.);
    double angle = -50.0;
    double scale = 0.6;
    cv2DRotationMatrix(center, angle, scale, rot_mat);
    cvWarpAffine_8u(src->tt.data, src->s, size, dst->tt.data, dst->s, size, rot_mat, src->c, 0, 0);

    imshow(src);
    imshow(dst);
    cvWaitKey(-1);
  }

  for (;;) {
    DPOINT srcQuad[4], dstQuad[4];
    double warp_matrix[9] = {0};
    double a = 0.2, b = 0.8;


    srcQuad[0] = dPOINT(0, 0);           //src Top left
    srcQuad[1] = dPOINT(src->w - 1, 0);  //src Top right
    srcQuad[2] = dPOINT(0, src->h - 1);           //src Bottom left
    srcQuad[3] = dPOINT(src->w - 1, src->h - 1);  //src Bot right

    srcQuad[0] = dPOINT(src->w * 0, src->h * 0); //dst Top left
    srcQuad[1] = dPOINT(src->w * 1, src->h * 0); //dst Top right
    srcQuad[2] = dPOINT(src->w * 0, src->h * 1); //dst Bottom left
    srcQuad[3] = dPOINT(src->w * 1, src->h * 1); //dst Bot right

    //- - - - - - - - - - - - - -//

    dstQuad[0] = dPOINT(src->w * a, src->h * a); //dst Top left
    dstQuad[1] = dPOINT(src->w * b, src->h * a); //dst Top right
    dstQuad[2] = dPOINT(src->w * a, src->h * b); //dst Bottom left
    dstQuad[3] = dPOINT(src->w * b, src->h * b); //dst Bot right

    dstQuad[0] = dPOINT(src->w * 0.05, src->h * 0.33); //dst Top left
    dstQuad[1] = dPOINT(src->w * 0.9, src->h * 0.25); //dst Top right
    dstQuad[2] = dPOINT(src->w * 0.2, src->h * 0.7); //dst Bottom left
    dstQuad[3] = dPOINT(src->w * 0.8, src->h * 0.9); //dst Bot right
    cvGetPerspectiveTransform(dstQuad, srcQuad, warp_matrix); //由四对点计算透射变换

    //PRINT2D("%6.4f, ", "\n", 3, 3, warp_matrix, 3, 1);
    //对图像进行透视变换
    {
      utime_start(_start_time);
      cvWarpPerspective_8u(src->tt.data, src->s, size, dst->tt.data, dst->s, size, warp_matrix, src->c, 0);
      printf("%f\n", utime_elapsed(_start_time));
    }
    imshow(src);
    imshow(dst);
    cvWaitKey(10);
  }

  freeims(src, 2);
  return 0;
}


#include "cv/rand.inl"
#include "cv/matmul2.inl"

#define MATMUL_ARGS(arrtype)  const arrtype* A, int ay, int ax, int ai, const arrtype* B, int by, int bx, int bi, arrtype* C, int cy, int cx, int crow, int ccol, int len
int matmul_impl(MATMUL_ARGS(double))
{
  int x, y, i;
  typedef double arrtype;
  typedef double worktype;

  for (y = 0; y < crow; ++y) {
    const arrtype* A0 = A + ay * y;
    const arrtype* B0 = A + by * y;
    arrtype* C0 = C + cy * y;

    for (x = 0; x < ccol; ++x) {
      C0[cx * x] = 0;
    }

    for (x = 0; x < ccol; ++x) {
      const arrtype* A1 = A0 + ax * x;
      const arrtype* B1 = A0 + bx * x;
      arrtype c = 0;
      i = 0;

      for (; i < len - 3; i += 4) {
        c += A1[ai * (i + 0)] * B1[bi * (i + 0)]
            + A1[ai * (i + 1)] * B1[bi * (i + 1)]
            + A1[ai * (i + 2)] * B1[bi * (i + 2)]
            + A1[ai * (i + 3)] * B1[bi * (i + 3)];
      }

      for (; i < len; ++i) {
        c += A1[ai * (i + 0)] * B1[bi * (i + 0)];
      }

      C0[cx * x] = c;
    }
  }

  return 0;
}
int matmul(int m, int n, int l, double* A, int al, double* B, int bl, double* C, int cl)
{
  int i, j, k;

  for (i = 0; i < m; ++i) {
    for (j = 0; j < n; ++j) {
      C[i * cl + j] = 0;
    }
  }

  for (j = 0; j < n; ++j) {
    for (i = 0; i < m; ++i) {
      for (k = 0; k < l; ++k) {
        C[i * cl + j] += A[i * al + k] * B[k * bl + j];
      }
    }
  }

  return 0;
}

int test_matmul()
{
  CvRNG rng = 1000;
  typedef double arrtype;
  arrtype pp[] = {1, 1};

  if (0) {
    enum {M = 2};
    arrtype A[M * M], B[M * M];
    arrtype C0[M * M] = {0};
    arrtype C1[M * M] = {0};
    CvSize size = cvSize(M, M);
    icvRandn_64f(&rng, A, M, size, 1, pp, pp);
    icvRandn_64f(&rng, B, M, size, 1, pp, pp);
    str_printf_mat("%3f ", M, M, A, M * sizeof(arrtype), M * M * 20);
    str_printf_mat("%3f ", M, M, B, M * sizeof(arrtype), M * M * 20);
    matmul_impl(A, M, 0, 1, B, 0, 1, M, C0, M, 1, M, M, M);
    str_printf_mat("%3f ", M, M, C0, M * sizeof(arrtype), M * M * 20);
  }

  if (1) {
    enum {M = 200};
    arrtype* A = MALLOC(arrtype, 3 * M * M);
    arrtype* B = A + M * M;
    arrtype* C = A + M * M * 2;
    int i;
    CvSize size = cvSize(M, M);
    icvRandn_64f(&rng, A, M, size, 1, pp, pp);
    icvRandn_64f(&rng, B, M, size, 1, pp, pp);

    for (i = 0; i < 10; ++i) {
      utime_start(_start_time);
      //matmul_impl(A, M, 0, 1, B, 0, 1, M, C, M, 1, M, M, M);
      //matmul(M, M, M, A, M, B, M, C, M);
      //GeneralMul(A, B, C, n);
      //BlockCacul(A, B, C, M, 4, 100);
      cvGEMM_64f(A, M, B, M, 1, 0, 0, 0, C, M, 0, M, M, M);
      printf("%f\n", utime_elapsed(_start_time));
    }

    FREE(A);
  }

  return 0;
}

#include "cv/geometry.inl"
//#include "cv/approx.inl"
#include "cv/contours.inl"
#include "draw/imdraw.inl"
#include "draw/imdrawaa.inl"
#include "img/color.inl"

//  Example 8-3. Finding and drawing contours on an input image
int test_FindContours()
{
  //Some defines we left out of the book
  img_t im[10] = {0};
  img_t* img_8uc1 = im;
  img_t* img_edge = im + 1;
  img_t* img_8uc3 = im + 2;
  CvSize size;
  IVC vc[1] = {0};
  IPOINT pt[100000] = {0};
  int len[1000] = {0};
  CONTOURINFO cc[1000] = {0};
  int i, j;
  iVC_init(vc, pt, countof(pt), cc, countof(cc));

  if (0) {
    _chdir("D:/pub/pic");
    imread("pic1.png", 1, 1, img_8uc1);
    //Changed this a little for safer image loading and help if not
  }
  else {
    int levels = 3;
    int w = 800;
    imsetsize(im, w, w, 1, 1);

    for (i = 0; i < 1; i++) {
      int dx = (i % 2) * 250 - 30; //0%2=0;
      int dy = (i / 2) * 150;
      COLOR white = rgb(1, 1, 1);
      COLOR black = rgb(0, 0, 0);

      if (i == 0) {
        for (j = 0; j <= 10; j++) {
          double angle = (j + 5) * CC_PI / 21;
          IPOINT pt1 = iPOINT(cvRound(dx + 100 + j * 10 - 80 * cos(angle)), cvRound(dy + 100 - 90 * sin(angle)));
          IPOINT pt2 = iPOINT(cvRound(dx + 100 + j * 10 - 30 * cos(angle)), cvRound(dy + 100 - 30 * sin(angle)));
          imdraw_line(im, pt1, pt2, white, 1);
        }
      }

      imdraw_ellipse1(im, dx + 150, dy + 100, 100, 70, 0, 0, 360, white, -1);
      imdraw_ellipse1(im, dx + 115, dy + 70, 30, 20, 0, 0, 360, black, -1);
      imdraw_ellipse1(im, dx + 185, dy + 70, 30, 20, 0, 0, 360, black, -1);
      imdraw_ellipse1(im, dx + 115, dy + 70, 15, 15, 0, 0, 360, white, -1);
      imdraw_ellipse1(im, dx + 185, dy + 70, 15, 15, 0, 0, 360, white, -1);
      imdraw_ellipse1(im, dx + 115, dy + 70, 5, 5, 0, 0, 360, black, -1);
      imdraw_ellipse1(im, dx + 185, dy + 70, 5, 5, 0, 0, 360, black, -1);
      imdraw_ellipse1(im, dx + 150, dy + 100, 10, 5, 0, 0, 360, black, -1);
      imdraw_ellipse1(im, dx + 150, dy + 150, 40, 10, 0, 0, 360, black, -1);
      imdraw_ellipse1(im, dx + 27, dy + 100, 20, 35, 0, 0, 360, white, -1);
      imdraw_ellipse1(im, dx + 273, dy + 100, 20, 35, 0, 0, 360, white, -1);
      //imshow(im);  cvWaitKey(-1);
    }

    // comment this out if you do not want approximation
    //contours = cvApproxPoly(contours, sizeof(CvContour), storage, CC_POLY_APPROX_DP, 3, 1);
    //cvApproxPoly: 逼近方法  精度 逼近曲线是否封闭
  }

  imsetsamesize(img_edge, img_8uc1);
  imsetsize(img_8uc3, img_8uc1->h, img_8uc1->w, 3, 1);
  size = cvSize(img_8uc1->w, img_8uc1->h);

  for (; 1;) {
    int n = 0;
    cvThreshold_8u(img_8uc1->tt.data, img_8uc1->s, img_edge->tt.data, img_edge->s, size, 128, 255, CC_THRESH_BINARY);  //对数组元素进行固定阈值操作

    //在二值图像中寻找轮廓
    // Try all four values and see what happens

    {
      utime_start(_start_time);
      cvFindContours(img_edge->tt.data, img_edge->s, size, vc);
      printf("%f\n", utime_elapsed(_start_time));
    }

    for (i = 0; i < vc->l; ++i) {
      len[i] = cc[i].npt;
    }

    for (i = 0; i < vc->l; ++i) {
      cvMerge(img_edge->tt.data, img_edge->tt.data, img_edge->tt.data, 0, img_edge->s, img_8uc3->tt.data, img_8uc3->s, size, 3, 1);
      imdrawaa_poly_solid_i(img_8uc3, 0, vc->pt + n, len + i, 1, rgba(1, 0, 0, 0.2), 0);
      imshow2("img_8uc1", img_edge);
      imshow2("img_8uc3", img_8uc3);
      cvWaitKey(-1);
      n += len[i];
    }

    cvMerge(img_edge->tt.data, img_edge->tt.data, img_edge->tt.data, 0, img_edge->s, img_8uc3->tt.data, img_8uc3->s, size, 3, 1);
    imdrawaa_poly_solid_i(img_8uc3, 0, vc->pt, len, vc->l, rgba(1, 0, 0, 0.2), 0);
    imshow2("img_8uc1", img_edge);
    imshow2("img_8uc3", img_8uc3);
    cvWaitKey(50);
  }


  freeims(im, 3);
  return 0;
}

#if 0
// Kalman（卡尔曼）滤波器的跟踪弹球模拟程序，大家在自己机器上运行下就知道这个程序的作用了。
// 白色球是目标球，蓝色球是观测球，绿色球是kalman估计球，白球在四壁上来回撞击，蓝球观测，
// 当白球撞壁反弹时，kalman绿球被“甩”出弹壁，因为它以之前的状态估计白球还会往前走，
// 但是它被甩出去后，它根据自己的模型和实际观测值重新更新模型，使自己重新较准确地跟踪白球。
int test_Kalman()
{
  cvNamedWindow("Kalman", 1);
  CvRandState random;//创建随机
  cvRandInit(&random, 0, 1, -1, CC_RAND_NORMAL);
  IplImage* image = cvCreateImage(cvSize(600, 450), 8, 3);
  CvKalman* kalman = cvCreateKalman(4, 2, 0); //状态变量4维，x、y坐标和在x、y方向上的速度，测量变量2维，x、y坐标

  CvMat* xK = cvCreateMat(4, 1, CC_32FC1); //初始化状态变量，坐标为（40,40），x、y方向初速度分别为10、10
  xK->data.fl[0] = 40.;
  xK->data.fl[1] = 40;
  xK->data.fl[2] = 10;
  xK->data.fl[3] = 10;

  const float F[] = {1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1}; //初始化传递矩阵 [1  0  1  0]
  //               [0  1  0  1]
  //               [0  0  1  0]
  //               [0  0  0  1]
  memcpy(kalman->transition_matrix->data.fl, F, sizeof(F));

  CvMat* wK = cvCreateMat(4, 1, CC_32FC1); //过程噪声
  cvZero(wK);

  CvMat* zK = cvCreateMat(2, 1, CC_32FC1); //测量矩阵2维，x、y坐标
  cvZero(zK);

  CvMat* vK = cvCreateMat(2, 1, CC_32FC1); //测量噪声
  cvZero(vK);

  cvSetIdentity(kalman->measurement_matrix, cvScalarAll(1));  //初始化测量矩阵H=[1  0  0  0]
  //                [0  1  0  0]
  cvSetIdentity(kalman->process_noise_cov, cvScalarAll(1e-1));/*过程噪声____设置适当数值，
                                                                    增大目标运动的随机性，
                                                                    但若设置的很大，则系统不能收敛，
                                                                    即速度越来越快*/
  cvSetIdentity(kalman->measurement_noise_cov, cvScalarAll(10));/*观测噪声____故意将观测噪声设置得很大，
                                                                    使之测量结果和预测结果同样存在误差*/
  cvSetIdentity(kalman->error_cov_post, cvRealScalar(1));  /*后验误差协方差*/
  cvRand(&random, kalman->state_post);

  CvMat* mK = cvCreateMat(1, 1, CC_32FC1); //反弹时外加的随机化矩阵


  while (1) {
    cvZero(image);
    cvRectangle(image, cvPoint(30, 30), cvPoint(570, 420), CC_RGB(255, 255, 255), 2); //绘制目标弹球的“撞击壁”
    const CvMat* yK = cvKalmanPredict(kalman, 0); //计算预测位置
    cvRandSetRange(&random, 0, sqrt(kalman->measurement_noise_cov->data.fl[0]), 0);
    cvRand(&random, vK);  //设置随机的测量误差
    cvMatMulAdd(kalman->measurement_matrix, xK, vK, zK);  //zK=H*xK+vK
    cvCircle(image, cvPoint(cvRound(CC_MAT_ELEM(*xK, float, 0, 0)), cvRound(CC_MAT_ELEM(*xK, float, 1, 0))),
        4, CC_RGB(255, 255, 255), 2); //白圈，真实位置
    cvCircle(image, cvPoint(cvRound(CC_MAT_ELEM(*yK, float, 0, 0)), cvRound(CC_MAT_ELEM(*yK, float, 1, 0))),
        4, CC_RGB(0, 255, 0), 2); //绿圈，预估位置
    cvCircle(image, cvPoint(cvRound(CC_MAT_ELEM(*zK, float, 0, 0)), cvRound(CC_MAT_ELEM(*zK, float, 1, 0))),
        4, CC_RGB(0, 0, 255), 2); //蓝圈，观测位置

    cvRandSetRange(&random, 0, sqrt(kalman->process_noise_cov->data.fl[0]), 0);
    cvRand(&random, wK); //设置随机的过程误差
    cvMatMulAdd(kalman->transition_matrix, xK, wK, xK); //xK=F*xK+wK

    if (cvRound(CC_MAT_ELEM(*xK, float, 0, 0)) < 30) { //当撞击到反弹壁时，对应轴方向取反外加随机化
      cvRandSetRange(&random, 0, sqrt(1e-1), 0);
      cvRand(&random, mK);
      xK->data.fl[2] = 10 + CC_MAT_ELEM(*mK, float, 0, 0);
    }

    if (cvRound(CC_MAT_ELEM(*xK, float, 0, 0)) > 570) {
      cvRandSetRange(&random, 0, sqrt(1e-2), 0);
      cvRand(&random, mK);
      xK->data.fl[2] = -(10 + CC_MAT_ELEM(*mK, float, 0, 0));
    }

    if (cvRound(CC_MAT_ELEM(*xK, float, 1, 0)) < 30) {
      cvRandSetRange(&random, 0, sqrt(1e-1), 0);
      cvRand(&random, mK);
      xK->data.fl[3] = 10 + CC_MAT_ELEM(*mK, float, 0, 0);
    }

    if (cvRound(CC_MAT_ELEM(*xK, float, 1, 0)) > 420) {
      cvRandSetRange(&random, 0, sqrt(1e-3), 0);
      cvRand(&random, mK);
      xK->data.fl[3] = -(10 + CC_MAT_ELEM(*mK, float, 0, 0));
    }

    printf("%f_____%f\n", xK->data.fl[2], xK->data.fl[3]);


    cvShowImage("Kalman", image);

    cvKalmanCorrect(kalman, zK);


    if (cvWaitKey(100) == 'e') {
      break;
    }
  }

  cvReleaseImage(&image);/*释放图像*/
  cvDestroyAllWindows();
}
#endif

int test_stitcher()
{
  img_t im[3] = {0};
  img_t* im1 = im;
  img_t* im2 = im + 1;
  img_t* im3 = im + 2;
  _chdir("D:/pub/pic/stitcher/");
  imread("1.jpg", 3, 1, im1);
  imread("2.jpg", 3, 1, im2);
  imread("3.jpg", 3, 1, im3);

#if 0
  Mat pano;
  Stitcher stitcher = stitcher::createDefault(try_use_gpu);
  Stitcher::Status status = stitcher.stitch(imgs, pano);

  if (status != Stitcher::OK) {
    cout << "Can't stitch images, error code = " << int(status) << endl;
    return -1;
  }

  imwrite(result_name, pano);
#endif
  freeims(im, 3);
  return 0;
}