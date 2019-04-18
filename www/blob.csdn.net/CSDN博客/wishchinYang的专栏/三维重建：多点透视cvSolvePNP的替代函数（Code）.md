# 三维重建：多点透视cvSolvePNP的替代函数（Code） - wishchinYang的专栏 - CSDN博客
2016年08月05日 14:33:09[wishchin](https://me.csdn.net/wishchin)阅读数：1998
﻿﻿       在调试JNI程序时，所有的Shell都已经加载完成，而唯一真正核心的cv::SolvePnP却不能在JNI里面获得通行证，经过反复测试都不能运行，因此只能忍痛舍弃，自行编写一个具有相似功能的函数对其进行替代。
        原函数是这样的：
```cpp
void rec3D::reconstruct3D(const vector<Point2f>& image_points, Mat& rvecs, Mat& tvecs)
{
 print_Matf( cam.intrinsic_Mat );
 print_Matf( cam.disCoeff );
 print_VecPointf(image_points);
 print_VecPointf(cam.object_points );
 cv::solvePnP(cam.object_points , image_points , cam.intrinsic_Mat , cam.disCoeff , rvecs , tvecs , false , CV_P3P);
 print_Matf( cam.intrinsic_Mat );
 print_Matf( cam.disCoeff );
 print_VecPointf(image_points);
 print_VecPointf(cam.object_points );
 print_Matf( rvecs );
 print_Matf( tvecs );
}
```
         当然，函数后面的测试时没有机会调用的，而函数前面的测试则是准确无误的....
1.   关于多点投射的函数cv::solvePnP
参考链接：[http://blog.csdn.net/abc20002929/article/details/8520063](http://blog.csdn.net/abc20002929/article/details/8520063)
Wiki链接：CV照相机 标定和三维重建 详细资料
计算多点透视问题，有离散的方法和解方程的方法，分别为cvPOSIT 和SolePnP。
应用场景：给定物体3D点集与对应的图像2D点集，之后进行姿态计算(即求旋转与位移矩阵)。
输入输出：
1.输入都是3D点集和对应的2D点集，其中cvPOSIT的3D点包含在posit_object结构中
2.输出均包括旋转矩阵和位移向量
形式不同：
solvePnP有摄像机的一些内参
语义不同：
除了一堆数据类型检查和转化外，其实solvePnP调用的是cvFindExtrinsicCameraParams2通过已知的内参进行未知外参求解，是一个精确解；而cvPOSIT是用仿射投影模型近似透视投影模型下，不断迭代计算出来的估计值(在物体深度变化相对于物体到摄像机的距离比较大的时候,这种算法可能不收敛)。
2. 函数原型
Finds an object pose from 3D-2D point correspondences.
solvePnP([http://docs.opencv.org/modules/calib3d](http://docs.opencv.org/modules/calib3d) /doc/camera_calibration_and_3d_reconstruction.html#solvepnp)
bool solvePnP(
InputArray objectPoints, InputArray imagePoints, InputArray cameraMatrix, InputArray distCoeffs,
 OutputArray rvec, OutputArray tvec,
bool useExtrinsicGuess=false, int flags=ITERATIVE )
cvPOSIT([http://www.opencv.org.cn/index.php/Cv%E7%85%A7%E7%9B%B8%E6%9C%BA%E5%AE%9A%E6%A0%87%E5%92%8C%E4%B8%89%E7%BB%B4%E9%87%8D%E5%BB%BA#POSIT](http://www.opencv.org.cn/index.php/Cv%E7%85%A7%E7%9B%B8%E6%9C%BA%E5%AE%9A%E6%A0%87%E5%92%8C%E4%B8%89%E7%BB%B4%E9%87%8D%E5%BB%BA#POSIT))
执行POSIT算法
void cvPOSIT(
 CvPOSITObject* posit_object, CvPoint2D32f* image_points, 
>double focal_length, 
CvTermCriteria criteria, CvMatr32f rotation_matrix,  CvVect32f translation_vector );
**3.Cv2.410的源代码**
OpenCV参考手册第392页
```cpp
bool cv::solvePnP( InputArray _opoints, InputArray _ipoints,
                  InputArray _cameraMatrix, InputArray _distCoeffs,
                  OutputArray _rvec, OutputArray _tvec, bool useExtrinsicGuess, int flags )
{
    Mat opoints = _opoints.getMat(), ipoints = _ipoints.getMat();
    int npoints = std::max(opoints.checkVector(3, CV_32F), opoints.checkVector(3, CV_64F));
    CV_Assert( npoints >= 0 && npoints == std::max(ipoints.checkVector(2, CV_32F), ipoints.checkVector(2, CV_64F)) );
    _rvec.create(3, 1, CV_64F);
    _tvec.create(3, 1, CV_64F);
    Mat cameraMatrix = _cameraMatrix.getMat(), distCoeffs = _distCoeffs.getMat();
    if (flags == CV_EPNP)
    {
        cv::Mat undistortedPoints;
        cv::undistortPoints(ipoints, undistortedPoints, cameraMatrix, distCoeffs);
        epnp PnP(cameraMatrix, opoints, undistortedPoints);
        cv::Mat R, rvec = _rvec.getMat(), tvec = _tvec.getMat();
        PnP.compute_pose(R, tvec);
        cv::Rodrigues(R, rvec);
        return true;
    }
    else if (flags == CV_P3P)
    {
        CV_Assert( npoints == 4);
        cv::Mat undistortedPoints;
        cv::undistortPoints(ipoints, undistortedPoints, cameraMatrix, distCoeffs);
        p3p P3Psolver(cameraMatrix);
        cv::Mat R, rvec = _rvec.getMat(), tvec = _tvec.getMat();
        bool result = P3Psolver.solve(R, tvec, opoints, undistortedPoints);
        if (result)
            cv::Rodrigues(R, rvec);
        return result;
    }
    else if (flags == CV_ITERATIVE)
    {
        CvMat c_objectPoints = opoints, c_imagePoints = ipoints;
        CvMat c_cameraMatrix = cameraMatrix, c_distCoeffs = distCoeffs;
        CvMat c_rvec = _rvec.getMat(), c_tvec = _tvec.getMat();
        cvFindExtrinsicCameraParams2(&c_objectPoints, &c_imagePoints, &c_cameraMatrix,
                                     c_distCoeffs.rows*c_distCoeffs.cols ? &c_distCoeffs : 0,
                                     &c_rvec, &c_tvec, useExtrinsicGuess );
        return true;
    }
    else
        CV_Error(CV_StsBadArg, "The flags argument must be one of CV_ITERATIVE or CV_EPNP");
    return false;
}
```
**有效代码：**
```cpp
else if (flags == CV_P3P)
    {
        CV_Assert( npoints == 4);
        cv::Mat undistortedPoints;
        cv::undistortPoints(ipoints, undistortedPoints, cameraMatrix, distCoeffs);
        p3p P3Psolver(cameraMatrix);
        cv::Mat R, rvec = _rvec.getMat(), tvec = _tvec.getMat();
        bool result = P3Psolver.solve(R, tvec, opoints, undistortedPoints);
        if (result)
            cv::Rodrigues(R, rvec);
        return result;
    }
```
4. 函数mySolvePnP()
```cpp
int rec3D::mySolvePnP(
 //vector<cv::Point3f>  &_opoints , vector<cv::Point2f>  &_ipoints ,
 InputArray  &_opoints , InputArray  &_ipoints ,
 //cv::Mat  &intrinsic_Mat , cv::Mat  &disCoeff ,
 InputArray &_cameraMatrix , InputArray  &_distCoeffs ,
 //cv::Mat  &rvecs , cv::Mat  &tvecs ,
 OutputArray  &_rvec , OutputArray  &_tvec ,
 bool useExtrinsicGuess, int flags)
{
 cv::Mat opoints = _opoints.getMat(), ipoints = _ipoints.getMat();
 int npoints = std::max(opoints.checkVector(3, CV_32F), opoints.checkVector(3, CV_64F));
 CV_Assert( npoints >= 0 && npoints == std::max(ipoints.checkVector(2, CV_32F), ipoints.checkVector(2, CV_64F)) );
 _rvec.create(3, 1, CV_64F);
 _tvec.create(3, 1, CV_64F);
 cv::Mat cameraMatrix = _cameraMatrix.getMat(), distCoeffs = _distCoeffs.getMat();
 {
  CV_Assert( npoints == 4);
  cv::Mat undistortedPoints;
  cv::undistortPoints(ipoints, undistortedPoints, cameraMatrix, distCoeffs);
  p3p P3Psolver(cameraMatrix);
  cv::Mat R, rvec = _rvec.getMat(), tvec = _tvec.getMat();
  bool result = P3Psolver.solve(R, tvec, opoints, undistortedPoints);
  if (result)
   cv::Rodrigues(R, rvec);
  return result;
 }
 return true;
}
暂时未完成，待修改...................
```
真正运行的JNI函数：
```cpp
真正可以运行的JNI函数：
int rec3D::mySolvePnP(
 vector<cv::Point3f>  &_opoints ,const  vector<cv::Point2f>  &_ipoints ,
 //InputArray  &_opoints , InputArray  &_ipoints ,
 cv::Mat  &intrinsic_Mat , cv::Mat  &disCoeff ,
 //InputArray &_cameraMatrix , InputArray  &_distCoeffs ,
 cv::Mat  &_rvecs , cv::Mat  &_tvecs ,
 //OutputArray  &_rvec , OutputArray  &_tvec ,
 bool useExtrinsicGuess, int flags)
{
 cv::Mat opoints = Mat(_opoints, true);// _opoints.getMat();
 cv::Mat ipoints = Mat(_ipoints , true);///_ipoints.getMat();
 int npoints = std::max(opoints.checkVector(3, CV_32F), opoints.checkVector(3, CV_64F));//The error source! wishchin !
 CV_Assert( npoints >= 0 && npoints == std::max(ipoints.checkVector(2, CV_32F), ipoints.checkVector(2, CV_64F)) );
 //_rvecs= new Mat(3, 1, CV_64F);//_rvec.create(3, 1, CV_64F);
 //_tvecs= new Mat(3, 1, CV_64F);//.create(3, 1, CV_64F);
 //cv::Mat cameraMatrix = _cameraMatrix.getMat(), distCoeffs = _distCoeffs.getMat();
 cv::Mat cameraMatrix = intrinsic_Mat.clone();
 cv::Mat distCoeffs = disCoeff.clone();
 //{
 CV_Assert( npoints == 4);
 cv::Mat undistortedPoints(ipoints.rows,ipoints.cols, ipoints.type() );
 //undistortedPoints 为输出参数！
 undistortPointsMy(ipoints, undistortedPoints, cameraMatrix, distCoeffs);//undistortPoints(ipoints, undistortedPoints, cameraMatrix, distCoeffs);
 //1.修改函数
 //p3pMy P3Psolver= new p3pMy(cameraMatrix);
 p3pMy P3Psolver(cameraMatrix);
 //cv::Mat R, rvec = _rvec.getMat(), tvec = _tvec.getMat();
 //cv::Mat     R = _rvecs.clone();
 //cv::Mat  rvec = _rvecs.clone();
 //cv::Mat  tvec = _tvecs.clone();
 ////2.修改函数
 //bool result = P3Psolver.solve(R, tvec, opoints, undistortedPoints);
 cv::Mat     R;// = _rvecs.clone();
 //cv::Mat  rvec = _rvecs.clone();
 //cv::Mat  tvec = _tvecs.clone();
 //2.修改函数
 bool result = P3Psolver.solve(_rvecs, _tvecs, opoints, undistortedPoints);
 if (result) cv::Rodrigues(_rvecs, R);//cv::Rodrigues(R, rvec);
 //return result;
 //}
 return true;
}
//void rec3D::undistortPointsMy( InputArray _src, OutputArray _dst,
// InputArray _cameraMatrix,
// InputArray _distCoeffs,
// InputArray _Rmat,
// InputArray _Pmat )
void rec3D::undistortPointsMy(
 cv::Mat &_src,  cv::Mat &_dst,
 cv::Mat &_cameraMatrix,
 cv::Mat &_distCoeffs)
{
 cv::Mat src =_src;// _src.getMat();
 cv::Mat cameraMatrix = _cameraMatrix;//_cameraMatrix.getMat();
 cv::Mat distCoeffs = _distCoeffs;//_distCoeffs.getMat();//, R = _Rmat.getMat(), P = _Pmat.getMat();
 cv::Mat R;
 cv::Mat P;
 CV_Assert( src.isContinuous() && (src.depth() == CV_32F || src.depth() == CV_64F) &&
  ((src.rows == 1 && src.channels() == 2) || src.cols*src.channels() == 2));
 //_dst.create(src.size(), src.type(), -1, true);
 Mat dst = _dst;//_dst.getMat();
 //Fuck！引用传来传去的！ dst矩阵！
 CvMat _csrc = src, _cdst = dst, _ccameraMatrix = cameraMatrix;
 CvMat matR, matP, _cdistCoeffs, *pR=0, *pP=0, *pD=0;
 if( R.data )
  pR = &(matR = R);
 if( P.data )
  pP = &(matP = P);
 if( distCoeffs.data )
  pD = &(_cdistCoeffs = distCoeffs);
 //Fuck！引用传来传去的！ dst矩阵！
 //cvUndistortPoints(&_csrc, &_cdst, &_ccameraMatrix, pD, pR, pP);
 cvUndistortPointsMyRf(&_csrc, &_cdst, &_ccameraMatrix, pD, pR, pP);
}
//逻辑混乱！！！
void rec3D::cvUndistortPointsMy(
 const CvMat* _src, CvMat* _dst, const CvMat* _cameraMatrix,const CvMat* _distCoeffs,
 const CvMat* matR, const CvMat* matP )
{
 double A[3][3], RR[3][3], k[8]={0,0,0,0,0,0,0,0}, fx, fy, ifx, ify, cx, cy;
 CvMat matA=cvMat(3, 3, CV_64F, A), _Dk;
 CvMat _RR=cvMat(3, 3, CV_64F, RR);
 const CvPoint2D32f* srcf;
 const CvPoint2D64f* srcd;
 CvPoint2D32f* dstf;
 CvPoint2D64f* dstd;
 int stype, dtype;
 int sstep, dstep;
 int i, j, n, iters = 1;
 CV_Assert( CV_IS_MAT(_src) && CV_IS_MAT(_dst) &&
  (_src->rows == 1 || _src->cols == 1) &&
  (_dst->rows == 1 || _dst->cols == 1) &&
  _src->cols + _src->rows - 1 == _dst->rows + _dst->cols - 1 &&
  (CV_MAT_TYPE(_src->type) == CV_32FC2 || CV_MAT_TYPE(_src->type) == CV_64FC2) &&
  (CV_MAT_TYPE(_dst->type) == CV_32FC2 || CV_MAT_TYPE(_dst->type) == CV_64FC2));
 CV_Assert( CV_IS_MAT(_cameraMatrix) &&
  _cameraMatrix->rows == 3 && _cameraMatrix->cols == 3 );
 cvConvert( _cameraMatrix, &matA );//这一句关系到画三维坐标系，但看起来毫无用处！
 if( _distCoeffs )
 {
  CV_Assert( CV_IS_MAT(_distCoeffs) &&
   (_distCoeffs->rows == 1 || _distCoeffs->cols == 1) &&
   (_distCoeffs->rows*_distCoeffs->cols == 4 ||
   _distCoeffs->rows*_distCoeffs->cols == 5 ||
   _distCoeffs->rows*_distCoeffs->cols == 8));
  _Dk = cvMat( _distCoeffs->rows, _distCoeffs->cols,
   CV_MAKETYPE(CV_64F,CV_MAT_CN(_distCoeffs->type)), k);
  cvConvert( _distCoeffs, &_Dk );
  iters = 5;
 }
 if( matR )
 {
  CV_Assert( CV_IS_MAT(matR) && matR->rows == 3 && matR->cols == 3 );
  cvConvert( matR, &_RR );
 }
 else
  cvSetIdentity(&_RR);
 if( matP )
 {
  double PP[3][3];
  CvMat _P3x3, _PP=cvMat(3, 3, CV_64F, PP);
  CV_Assert( CV_IS_MAT(matP) && matP->rows == 3 && (matP->cols == 3 || matP->cols == 4));
  cvConvert( cvGetCols(matP, &_P3x3, 0, 3), &_PP );
  cvMatMul( &_PP, &_RR, &_RR );
 }
 srcf = (const CvPoint2D32f*)_src->data.ptr;
 srcd = (const CvPoint2D64f*)_src->data.ptr;
 dstf = (CvPoint2D32f*)_dst->data.ptr;
 dstd = (CvPoint2D64f*)_dst->data.ptr;
 stype = CV_MAT_TYPE(_src->type);
 dtype = CV_MAT_TYPE(_dst->type);
 sstep = _src->rows == 1 ? 1 : _src->step/CV_ELEM_SIZE(stype);
 dstep = _dst->rows == 1 ? 1 : _dst->step/CV_ELEM_SIZE(dtype);
 n = _src->rows + _src->cols - 1;
 fx = A[0][0];
 fy = A[1][1];
 ifx = 1./fx;
 ify = 1./fy;
 cx = A[0][2];
 cy = A[1][2];
 for( i = 0; i < n; i++ )
 {
  double x, y, x0, y0;
  if( stype == CV_32FC2 )
  {
   x = srcf[i*sstep].x;
   y = srcf[i*sstep].y;
  }
  else
  {
   x = srcd[i*sstep].x;
   y = srcd[i*sstep].y;
  }
  x0 = x = (x - cx)*ifx;
  y0 = y = (y - cy)*ify;
  // compensate distortion iteratively
  for( j = 0; j < iters; j++ )
  {
   double r2 = x*x + y*y;
   double icdist = (1 + ((k[7]*r2 + k[6])*r2 + k[5])*r2)/(1 + ((k[4]*r2 + k[1])*r2 + k[0])*r2);
   double deltaX = 2*k[2]*x*y + k[3]*(r2 + 2*x*x);
   double deltaY = k[2]*(r2 + 2*y*y) + 2*k[3]*x*y;
   x = (x0 - deltaX)*icdist;
   y = (y0 - deltaY)*icdist;
  }
  double xx = RR[0][0]*x + RR[0][1]*y + RR[0][2];
  double yy = RR[1][0]*x + RR[1][1]*y + RR[1][2];
  double ww = 1./(RR[2][0]*x + RR[2][1]*y + RR[2][2]);
  x = xx*ww;
  y = yy*ww;
  if( dtype == CV_32FC2 )
  {
   dstf[i*dstep].x = (float)x;
   dstf[i*dstep].y = (float)y;
  }
  else
  {
   dstd[i*dstep].x = x;
   dstd[i*dstep].y = y;
  }
 }
}
void rec3D::cvUndistortPointsMyRf(
 const CvMat* _src, CvMat* _dst,
 const CvMat* _cameraMatrix,
 const CvMat* _distCoeffs,
 const CvMat* matR,
 const CvMat* matP )
{
 double A[3][3], RR[3][3];
 double k[8]={0,0,0,0,0,0,0,0};
 double fx, fy, ifx, ify, cx, cy;
 CvMat matA=cvMat(3, 3, CV_64F, A);
 CvMat _Dk;
 CvMat _RR=cvMat(3, 3, CV_64F, RR);
 const CvPoint2D32f* srcf;
 const CvPoint2D64f* srcd;
 CvPoint2D32f* dstf;
 CvPoint2D64f* dstd;
 int stype, dtype;
 int sstep, dstep;
 int i, j, n, iters = 1;
 CV_Assert(
  CV_IS_MAT(_src) && CV_IS_MAT(_dst) &&
  (_src->rows == 1 || _src->cols == 1) &&
  (_dst->rows == 1 || _dst->cols == 1) &&
  _src->cols + _src->rows - 1 == _dst->rows + _dst->cols - 1 &&
  (CV_MAT_TYPE(_src->type) == CV_32FC2 || CV_MAT_TYPE(_src->type) == CV_64FC2) &&
  (CV_MAT_TYPE(_dst->type) == CV_32FC2 || CV_MAT_TYPE(_dst->type) == CV_64FC2));
 CV_Assert( CV_IS_MAT(_cameraMatrix) &&
  _cameraMatrix->rows == 3 && _cameraMatrix->cols == 3 );
 //图像到矩阵的转换:cvConvert( src, dst );
 //cvConvert( _cameraMatrix, &matA );//这一句关系到画三维坐标系，但看起来毫无用处！
 cvConvertMy( _cameraMatrix, &matA );
 if( _distCoeffs )
 {
  CV_Assert( CV_IS_MAT(_distCoeffs) &&
   (_distCoeffs->rows == 1 || _distCoeffs->cols == 1) &&
   (_distCoeffs->rows*_distCoeffs->cols == 4 ||
   _distCoeffs->rows*_distCoeffs->cols == 5 ||
   _distCoeffs->rows*_distCoeffs->cols == 8));
  _Dk = cvMat( _distCoeffs->rows, _distCoeffs->cols,
   CV_MAKETYPE(CV_64F,CV_MAT_CN(_distCoeffs->type)), k);
  //cvConvert( _distCoeffs, &_Dk );
  cvConvertMy(_distCoeffs, &_Dk );
  iters = 5;
 }
 if( matR )
 {
  CV_Assert( CV_IS_MAT(matR) && matR->rows == 3 && matR->cols == 3 );
  //cvConvert( matR, &_RR );
  cvConvertMy(matR, &_RR );
 }
 else
  cvSetIdentity(&_RR);
 if( matP )
 {
  double PP[3][3];
  CvMat _P3x3, _PP=cvMat(3, 3, CV_64F, PP);
  CV_Assert( CV_IS_MAT(matP) && matP->rows == 3 && (matP->cols == 3 || matP->cols == 4));
  //cvConvert( cvGetCols(matP, &_P3x3, 0, 3), &_PP );
  cvConvertMy( cvGetCols(matP, &_P3x3, 0, 3), &_PP );
  cvMatMul( &_PP, &_RR, &_RR );
 }
 srcf = (const CvPoint2D32f*)_src->data.ptr;
 srcd = (const CvPoint2D64f*)_src->data.ptr;
 //dst的指针引用！
 dstf = (CvPoint2D32f*)_dst->data.ptr;
 dstd = (CvPoint2D64f*)_dst->data.ptr;
 stype = CV_MAT_TYPE(_src->type);
 dtype = CV_MAT_TYPE(_dst->type);
 sstep = _src->rows == 1 ? 1 : _src->step/CV_ELEM_SIZE(stype);
 dstep = _dst->rows == 1 ? 1 : _dst->step/CV_ELEM_SIZE(dtype);
 n = _src->rows + _src->cols - 1;
 fx = A[0][0];
 fy = A[1][1];
 ifx = 1./fx;
 ify = 1./fy;
 cx = A[0][2];
 cy = A[1][2];
 for( i = 0; i < n; i++ )
 {
  double x, y, x0, y0;
  if( stype == CV_32FC2 )
  {
   x = srcf[i*sstep].x;
   y = srcf[i*sstep].y;
  }
  else
  {
   x = srcd[i*sstep].x;
   y = srcd[i*sstep].y;
  }
  x0 = x = (x - cx)*ifx;
  y0 = y = (y - cy)*ify;
  // compensate distortion iteratively
  for( j = 0; j < iters; j++ )
  {
   double r2 = x*x + y*y;
   double icdist = (1 + ((k[7]*r2 + k[6])*r2 + k[5])*r2)/(1 + ((k[4]*r2 + k[1])*r2 + k[0])*r2);
   double deltaX = 2*k[2]*x*y + k[3]*(r2 + 2*x*x);
   double deltaY = k[2]*(r2 + 2*y*y) + 2*k[3]*x*y;
   x = (x0 - deltaX)*icdist;
   y = (y0 - deltaY)*icdist;
  }
  double xx = RR[0][0]*x + RR[0][1]*y + RR[0][2];
  double yy = RR[1][0]*x + RR[1][1]*y + RR[1][2];
  double ww = 1./(RR[2][0]*x + RR[2][1]*y + RR[2][2]);
  x = xx*ww;
  y = yy*ww;
  if( dtype == CV_32FC2 )
  {
   dstf[i*dstep].x = (float)x;
   dstf[i*dstep].y = (float)y;
  }
  else
  {
   dstd[i*dstep].x = x;
   dstd[i*dstep].y = y;
  }
 }
}
//bool rec3D::cvConvertMy(const CvMat* _cameraMatrix,double matA[][3] )
//{
//
// return true;
//}
bool rec3D::cvConvertMy(const CvMat* Mat, CvMat* MatA )
{
 int C = Mat->step/ Mat->cols/sizeof(Mat->type);
 for (int row = 0; row < Mat->rows && row < MatA->rows; row++){
  float* ptr  = (float*)(Mat->data.ptr + row * Mat->step);//第row行数据的起始指针
  float* ptrD = (float*)(MatA->data.ptr + row * MatA->step);
  for (int col = 0; col < Mat->cols && col < MatA->cols; col++)
  {
   for (int i=0;i<C;++i)
   {
    *(ptrD+C*col+i)   = *(ptr+C*col+i);
   }
   //*(ptrD+3*col)   = *(ptr+3*col);//cout<<*(ptr+3*col)<<endl;
   //*(ptrD+3*col+1)   = *(ptr+3*col+1);//cout<<*(ptr+3*col+1)<<endl;
   //*(ptrD+3*col+2)   = *(ptr+3*col+2);//cout<<*(ptr+3*col+2)<<endl;
  }
 }
 return true;
}
```
完成！
