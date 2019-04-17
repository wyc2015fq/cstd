# 【特征匹配】RANSAC算法原理与源码解析 - 勿在浮砂筑高台 - CSDN博客





置顶2015年12月08日 20:34:33[勿在浮砂筑高台](https://me.csdn.net/luoshixian099)阅读数：37730标签：[RANSAC																[误匹配检测](https://so.csdn.net/so/search/s.do?q=误匹配检测&t=blog)](https://so.csdn.net/so/search/s.do?q=RANSAC&t=blog)
个人分类：[Feature Matching																[Image Processing](https://blog.csdn.net/luoshixian099/article/category/2734445)](https://blog.csdn.net/luoshixian099/article/category/5732053)

所属专栏：[特征匹配](https://blog.csdn.net/column/details/featuresdetect.html)







转载请注明出处：http://blog.csdn.net/luoshixian099/article/details/50217655

勿在浮沙筑高台

  随机抽样一致性（RANSAC）算法，可以在一组包含“外点”的数据集中，采用不断迭代的方法，寻找最优参数模型，不符合最优模型的点，被定义为“外点”。在图像配准以及拼接上得到广泛的应用，本文将对RANSAC算法在OpenCV中角点误匹配对的检测中进行解析。

![](https://img-blog.csdn.net/20151208144705062)![](https://img-blog.csdn.net/20151208144641115)





1.RANSAC原理

  OpenCV中滤除误匹配对采用RANSAC算法寻找一个最佳单应性矩阵H，矩阵大小为3×3。RANSAC目的是找到最优的参数矩阵使得满足该矩阵的数据点个数最多，通常令h33=1来归一化矩阵。由于单应性矩阵有8个未知参数，至少需要8个线性方程求解，对应到点位置信息上，一组点对可以列出两个方程，则至少包含4组匹配点对。

![](https://img-blog.csdn.net/20151208160537989)


其中(x,y)表示目标图像角点位置,(x',y')为场景图像角点位置，s为尺度参数。

  RANSAC算法从匹配数据集中随机抽出4个样本并保证这4个样本之间不共线，计算出单应性矩阵，然后利用这个模型测试所有数据，并计算满足这个模型数据点的个数与投影误差(即代价函数)，若此模型为最优模型，则对应的代价函数最小。

![](https://img-blog.csdn.net/20151208160552841)


-----------------------------------------------------------------------------------------------------------------

RANSAC算法步骤：

          1. 随机从数据集中随机抽出4个样本数据 (此4个样本之间不能共线)，计算出变换矩阵H，记为模型M；

          2. 计算数据集中所有数据与模型M的投影误差，若误差小于阈值，加入内点集 I ；

          3. 如果当前内点集 I 元素个数大于最优内点集 I_best , 则更新 I_best = I，同时更新迭代次数k ;

          4. 如果迭代次数大于k,则退出 ; 否则迭代次数加1，并重复上述步骤；

  注：迭代次数k在不大于最大迭代次数的情况下，是在不断更新而不是固定的；

![](https://img-blog.csdn.net/20151208162540847)其中，p为置信度，一般取0.995；w为"内点"的比例 ; m为计算模型所需要的最少样本数=4；

-----------------------------------------------------------------------------------------------------------------




2.例程

OpenCV中此功能通过调用findHomography函数调用，下面是个例程：



```cpp
#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;
using namespace std;
int main(int argc, char** argv)
{
	Mat obj=imread("F:\\Picture\\obj.jpg");   //载入目标图像
	Mat scene=imread("F:\\Picture\\scene.jpg"); //载入场景图像
	if (obj.empty() || scene.empty() )
	{
		cout<<"Can't open the picture!\n";
		return 0;
	}
	vector<KeyPoint> obj_keypoints,scene_keypoints;
	Mat obj_descriptors,scene_descriptors;
   	ORB detector;     //采用ORB算法提取特征点
	detector.detect(obj,obj_keypoints);
	detector.detect(scene,scene_keypoints);
	detector.compute(obj,obj_keypoints,obj_descriptors);
	detector.compute(scene,scene_keypoints,scene_descriptors);
	BFMatcher matcher(NORM_HAMMING,true); //汉明距离做为相似度度量
	vector<DMatch> matches;
	matcher.match(obj_descriptors, scene_descriptors, matches);
	Mat match_img;
	drawMatches(obj,obj_keypoints,scene,scene_keypoints,matches,match_img);
	imshow("滤除误匹配前",match_img);

	//保存匹配对序号
	vector<int> queryIdxs( matches.size() ), trainIdxs( matches.size() );
	for( size_t i = 0; i < matches.size(); i++ )
	{
		queryIdxs[i] = matches[i].queryIdx;
		trainIdxs[i] = matches[i].trainIdx;
	}	

	Mat H12;   //变换矩阵

	vector<Point2f> points1; KeyPoint::convert(obj_keypoints, points1, queryIdxs);
	vector<Point2f> points2; KeyPoint::convert(scene_keypoints, points2, trainIdxs);
    int ransacReprojThreshold = 5;  //拒绝阈值


    H12 = findHomography( Mat(points1), Mat(points2), CV_RANSAC, ransacReprojThreshold );
    vector<char> matchesMask( matches.size(), 0 );  
	Mat points1t;
	perspectiveTransform(Mat(points1), points1t, H12);
	for( size_t i1 = 0; i1 < points1.size(); i1++ )  //保存‘内点’
	{
		if( norm(points2[i1] - points1t.at<Point2f>((int)i1,0)) <= ransacReprojThreshold ) //给内点做标记
		{
			matchesMask[i1] = 1;
		}	
	}
	Mat match_img2;   //滤除‘外点’后
	drawMatches(obj,obj_keypoints,scene,scene_keypoints,matches,match_img2,Scalar(0,0,255),Scalar::all(-1),matchesMask);

	//画出目标位置
	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = cvPoint(0,0); obj_corners[1] = cvPoint( obj.cols, 0 );
	obj_corners[2] = cvPoint( obj.cols, obj.rows ); obj_corners[3] = cvPoint( 0, obj.rows );
	std::vector<Point2f> scene_corners(4);
	perspectiveTransform( obj_corners, scene_corners, H12);
    line( match_img2, scene_corners[0] + Point2f(static_cast<float>(obj.cols), 0), 
		scene_corners[1] + Point2f(static_cast<float>(obj.cols), 0),Scalar(0,0,255),2);
	line( match_img2, scene_corners[1] + Point2f(static_cast<float>(obj.cols), 0), 
		scene_corners[2] + Point2f(static_cast<float>(obj.cols), 0),Scalar(0,0,255),2);
	line( match_img2, scene_corners[2] + Point2f(static_cast<float>(obj.cols), 0), 
		scene_corners[3] + Point2f(static_cast<float>(obj.cols), 0),Scalar(0,0,255),2);
	line( match_img2, scene_corners[3] + Point2f(static_cast<float>(obj.cols), 0),
		scene_corners[0] + Point2f(static_cast<float>(obj.cols), 0),Scalar(0,0,255),2);

    imshow("滤除误匹配后",match_img2);
	waitKey(0);
	
    return 0;
}
```

3. RANSAC源码解析


(1)findHomography内部调用cvFindHomography函数

srcPoints:目标图像点集

dstPoints:场景图像点集

method: 最小中值法、RANSAC方法、最小二乘法

ransacReprojTheshold:投影误差阈值

mask:掩码



```cpp
cvFindHomography( const CvMat* objectPoints, const CvMat* imagePoints,
                  CvMat* __H, int method, double ransacReprojThreshold,
                  CvMat* mask )
{
    const double confidence = 0.995;  //置信度
    const int maxIters = 2000;    //迭代最大次数
    const double defaultRANSACReprojThreshold = 3; //默认拒绝阈值
    bool result = false;
    Ptr<CvMat> m, M, tempMask;

    double H[9];
    CvMat matH = cvMat( 3, 3, CV_64FC1, H );  //变换矩阵
    int count;

    CV_Assert( CV_IS_MAT(imagePoints) && CV_IS_MAT(objectPoints) );

    count = MAX(imagePoints->cols, imagePoints->rows);
    CV_Assert( count >= 4 );           //至少有4个样本
    if( ransacReprojThreshold <= 0 )
        ransacReprojThreshold = defaultRANSACReprojThreshold;

    m = cvCreateMat( 1, count, CV_64FC2 );  //转换为齐次坐标
    cvConvertPointsHomogeneous( imagePoints, m );

    M = cvCreateMat( 1, count, CV_64FC2 );//转换为齐次坐标
    cvConvertPointsHomogeneous( objectPoints, M );

    if( mask )
    {
        CV_Assert( CV_IS_MASK_ARR(mask) && CV_IS_MAT_CONT(mask->type) &&
            (mask->rows == 1 || mask->cols == 1) &&
            mask->rows*mask->cols == count );
    }
    if( mask || count > 4 )
        tempMask = cvCreateMat( 1, count, CV_8U );
    if( !tempMask.empty() )
        cvSet( tempMask, cvScalarAll(1.) );

    CvHomographyEstimator estimator(4);
    if( count == 4 )
        method = 0;
    if( method == CV_LMEDS )  //最小中值法
        result = estimator.runLMeDS( M, m, &matH, tempMask, confidence, maxIters );
    else if( method == CV_RANSAC )    //采用RANSAC算法
        result = estimator.runRANSAC( M, m, &matH, tempMask, ransacReprojThreshold, confidence, maxIters);//(2)解析
    else
        result = estimator.runKernel( M, m, &matH ) > 0; //最小二乘法

    if( result && count > 4 )
    {
        icvCompressPoints( (CvPoint2D64f*)M->data.ptr, tempMask->data.ptr, 1, count );  //保存内点集
        count = icvCompressPoints( (CvPoint2D64f*)m->data.ptr, tempMask->data.ptr, 1, count );
        M->cols = m->cols = count;
        if( method == CV_RANSAC )  //
            estimator.runKernel( M, m, &matH );  //内点集上采用最小二乘法重新估算变换矩阵
        estimator.refine( M, m, &matH, 10 );// 
    }

    if( result )
        cvConvert( &matH, __H );  //保存变换矩阵

    if( mask && tempMask )
    {
        if( CV_ARE_SIZES_EQ(mask, tempMask) )
           cvCopy( tempMask, mask );
        else
           cvTranspose( tempMask, mask );
    }

    return (int)result;
}
```



(2) runRANSAC

maxIters:最大迭代次数

m1,m2 :数据样本

model:变换矩阵

reprojThreshold:投影误差阈值

confidence:置信度  0.995



```cpp
bool CvModelEstimator2::runRANSAC( const CvMat* m1, const CvMat* m2, CvMat* model,
                                    CvMat* mask0, double reprojThreshold,
                                    double confidence, int maxIters )
{
    bool result = false;
    cv::Ptr<CvMat> mask = cvCloneMat(mask0);
    cv::Ptr<CvMat> models, err, tmask;
    cv::Ptr<CvMat> ms1, ms2;

    int iter, niters = maxIters;
    int count = m1->rows*m1->cols, maxGoodCount = 0;
    CV_Assert( CV_ARE_SIZES_EQ(m1, m2) && CV_ARE_SIZES_EQ(m1, mask) );

    if( count < modelPoints )
        return false;

    models = cvCreateMat( modelSize.height*maxBasicSolutions, modelSize.width, CV_64FC1 );
    err = cvCreateMat( 1, count, CV_32FC1 );//保存每组点对应的投影误差
    tmask = cvCreateMat( 1, count, CV_8UC1 );

    if( count > modelPoints )  //多于4个点
    {
        ms1 = cvCreateMat( 1, modelPoints, m1->type );
        ms2 = cvCreateMat( 1, modelPoints, m2->type );
    }
    else  //等于4个点
    {
        niters = 1; //迭代一次
        ms1 = cvCloneMat(m1);  //保存每次随机找到的样本点
        ms2 = cvCloneMat(m2);
    }

    for( iter = 0; iter < niters; iter++ )  //不断迭代
    {
        int i, goodCount, nmodels;
        if( count > modelPoints )
        {
           //在(3)解析getSubset
            bool found = getSubset( m1, m2, ms1, ms2, 300 ); //随机选择4组点，且三点之间不共线，(3)解析
            if( !found )
            {
                if( iter == 0 )
                    return false;
                break;
            }
        }

        nmodels = runKernel( ms1, ms2, models );  //估算近似变换矩阵，返回1
        if( nmodels <= 0 )
            continue;
        for( i = 0; i < nmodels; i++ )//执行一次 
        {
            CvMat model_i;
            cvGetRows( models, &model_i, i*modelSize.height, (i+1)*modelSize.height );//获取3×3矩阵元素
            goodCount = findInliers( m1, m2, &model_i, err, tmask, reprojThreshold );  //找出内点，(4)解析

            if( goodCount > MAX(maxGoodCount, modelPoints-1) )  //当前内点集元素个数大于最优内点集元素个数
            {
                std::swap(tmask, mask);
                cvCopy( &model_i, model );  //更新最优模型
                maxGoodCount = goodCount;
				//confidence 为置信度默认0.995,modelPoints为最少所需样本数=4,niters迭代次数
                niters = cvRANSACUpdateNumIters( confidence,  //更新迭代次数，(5)解析
                    (double)(count - goodCount)/count, modelPoints, niters );
            }
        }
    }

    if( maxGoodCount > 0 )
    {
        if( mask != mask0 )
            cvCopy( mask, mask0 );
        result = true;
    }

    return result;
}
```



(3)getSubset

ms1,ms2:保存随机找到的4个样本

maxAttempts:最大迭代次数，300



```cpp
bool CvModelEstimator2::getSubset( const CvMat* m1, const CvMat* m2,
                                   CvMat* ms1, CvMat* ms2, int maxAttempts )
{
    cv::AutoBuffer<int> _idx(modelPoints); //modelPoints 所需要最少的样本点个数
    int* idx = _idx;
    int i = 0, j, k, idx_i, iters = 0;
    int type = CV_MAT_TYPE(m1->type), elemSize = CV_ELEM_SIZE(type);
    const int *m1ptr = m1->data.i, *m2ptr = m2->data.i;
    int *ms1ptr = ms1->data.i, *ms2ptr = ms2->data.i;
    int count = m1->cols*m1->rows;

    assert( CV_IS_MAT_CONT(m1->type & m2->type) && (elemSize % sizeof(int) == 0) );
    elemSize /= sizeof(int); //每个数据占用字节数

    for(; iters < maxAttempts; iters++)
    {
        for( i = 0; i < modelPoints && iters < maxAttempts; )
        {
            idx[i] = idx_i = cvRandInt(&rng) % count;  // 随机选取1组点
            for( j = 0; j < i; j++ )  // 检测是否重复选择
                if( idx_i == idx[j] )
                    break;
            if( j < i )
                continue;  //重新选择
            for( k = 0; k < elemSize; k++ )  //拷贝点数据
            {
                ms1ptr[i*elemSize + k] = m1ptr[idx_i*elemSize + k];
                ms2ptr[i*elemSize + k] = m2ptr[idx_i*elemSize + k];
            }
            if( checkPartialSubsets && (!checkSubset( ms1, i+1 ) || !checkSubset( ms2, i+1 )))//检测点之间是否共线
            {
                iters++;  //若共线，重新选择一组
                continue;
            }
            i++;
        }
        if( !checkPartialSubsets && i == modelPoints &&
            (!checkSubset( ms1, i ) || !checkSubset( ms2, i )))
            continue;
        break;
    }

    return i == modelPoints && iters < maxAttempts;  //返回找到的样本点个数
}
```



(4) findInliers & computerReprojError



```cpp
int CvModelEstimator2::findInliers( const CvMat* m1, const CvMat* m2,
                                    const CvMat* model, CvMat* _err,
                                    CvMat* _mask, double threshold )
{
    int i, count = _err->rows*_err->cols, goodCount = 0;
    const float* err = _err->data.fl;
    uchar* mask = _mask->data.ptr;

    computeReprojError( m1, m2, model, _err );  //计算每组点的投影误差
    threshold *= threshold;
    for( i = 0; i < count; i++ )
        goodCount += mask[i] = err[i] <= threshold;//误差在限定范围内，加入‘内点集’
    return goodCount;
}
//--------------------------------------
void CvHomographyEstimator::computeReprojError( const CvMat* m1, const CvMat* m2,const CvMat* model, CvMat* _err )
{
	int i, count = m1->rows*m1->cols;
	const CvPoint2D64f* M = (const CvPoint2D64f*)m1->data.ptr;
	const CvPoint2D64f* m = (const CvPoint2D64f*)m2->data.ptr;
	const double* H = model->data.db;
	float* err = _err->data.fl;

	for( i = 0; i < count; i++ )        //保存每组点的投影误差，对应上述变换公式
	{
		double ww = 1./(H[6]*M[i].x + H[7]*M[i].y + 1.);    
		double dx = (H[0]*M[i].x + H[1]*M[i].y + H[2])*ww - m[i].x;
		double dy = (H[3]*M[i].x + H[4]*M[i].y + H[5])*ww - m[i].y;
		err[i] = (float)(dx*dx + dy*dy);
	}
}
```
(5)cvRANSACUpdateNumIters


对应上述k的计算公式

p：置信度

ep:外点比例



```cpp
cvRANSACUpdateNumIters( double p, double ep,
                        int model_points, int max_iters )
{
    if( model_points <= 0 )
        CV_Error( CV_StsOutOfRange, "the number of model points should be positive" );

    p = MAX(p, 0.);
    p = MIN(p, 1.);
    ep = MAX(ep, 0.);
    ep = MIN(ep, 1.);

    // avoid inf's & nan's
    double num = MAX(1. - p, DBL_MIN);  //num=1-p,做分子
    double denom = 1. - pow(1. - ep,model_points);//做分母
    if( denom < DBL_MIN )
        return 0;

    num = log(num);
    denom = log(denom);

    return denom >= 0 || -num >= max_iters*(-denom) ?
        max_iters : cvRound(num/denom);
}
```








