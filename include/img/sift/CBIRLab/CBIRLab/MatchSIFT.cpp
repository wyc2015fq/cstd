#include "StdAfx.h"
#include "MatchSIFT.h"

extern "C"
{
	//SIFT库文件
	#include "sift.h"
	#include "imgfeatures.h"
	#include "kdtree.h"
	#include "utils.h"
	#include "xform.h"
};

CMatchSIFT::CMatchSIFT(void)
{
}

CMatchSIFT::~CMatchSIFT(void)
{
}

bool CMatchSIFT::initialize(CMySIFT* sift1, CMySIFT* sift2)
{
	this->imgSIFT1 = sift1;
	this->imgSIFT2 = sift2;
	n1 = imgSIFT1->num;
	n2 = imgSIFT2->num;

	return true;
}

bool CMatchSIFT::matchTwoImage(CRetrievalResult& retrievalRes)
{
	int k, i, m = 0, width1 = imgSIFT1->img->width, height1 = imgSIFT1->img->height;
	struct feature* feat;
	struct kd_node* kd_root;
	double d0, d1;
	struct feature** nbrs;
	kd_root = kdtree_build( imgSIFT2->feat, n2 );
	for( i = 0; i < n1; i++ )
	{
		feat = imgSIFT1->feat + i;
		k = kdtree_bbf_knn( kd_root, feat, 2, &nbrs, KDTREE_BBF_MAX_NN_CHKS );
		if( k == 2 )
		{
			d0 = descr_dist_sq( feat, nbrs[0] );
			d1 = descr_dist_sq( feat, nbrs[1] );
			if( d0 < d1 * NN_SQ_DIST_RATIO_THR )
			{
				//m表示匹配点数
				m++;
				imgSIFT1->feat[i].fwd_match = nbrs[0];
			}
		}
		pfree( nbrs );
	}

	/* 
	UNCOMMENT BELOW TO SEE HOW RANSAC FUNCTION WORKS

	Note that this line above:
	feat1[i].fwd_match = nbrs[0];

	is important for the RANSAC function to work.
	*/

	/************************************************************************/
	/* 
	cvWarpPerspective有个输入参数是透视矩阵H 如果你已知了这个H的话 每个点的坐标就很好算啦
	假设H是个3*3的矩阵（至于为什么是3*3，自己查阅一下有关透视矩阵的资料，这个矩阵涉及到图像的移动，旋转，仿射，缩放等变换）
	H=【a,b,c; d,e,f; g,h,i】
	a, b, c
	d, e, f
	g, h, i

	假设原始点（x，y），映射之后的坐标点为（x',y'）
	x'= (a*x + b*y + c) / (g*x + h*y + i);
	y'= (d*x + e*y + f) / (g*x + h*y + i);
	*/
	/************************************************************************/
	
	CvMat* H;
	H = ransac_xform( imgSIFT1->feat, n1, FEATURE_FWD_MATCH, lsq_homog, 4, 0.01,
		homog_xfer_err, 3.0, NULL, NULL );

	if( H )
	{
		//将匹配结果写入retrievalRes中
		RegionQuad region;
		double hData[3][3] = {
			cvmGet(H, 0, 0), cvmGet(H, 0, 1), cvmGet(H, 0, 2),
			cvmGet(H, 1, 0), cvmGet(H, 1, 1), cvmGet(H, 1, 2),
			cvmGet(H, 2, 0), cvmGet(H, 2, 1), cvmGet(H, 2, 2)
		};

		//(x, y) = (0, 0)进行变换
		region.points[0].x = cvRound((hData[0][0]*0 + hData[0][1]*0 + hData[0][2])
			/(hData[2][0]*0 + hData[2][1]*0 + hData[2][2]));
		region.points[0].y = cvRound((hData[1][0]*0 + hData[1][1]*0 + hData[1][2])
			/(hData[2][0]*0 + hData[2][1]*0 + hData[2][2]));

		//(x, y) = (0, height)进行变换
		region.points[1].x = cvRound((hData[0][0]*0 + hData[0][1]*height1 + hData[0][2])
			/(hData[2][0]*0 + hData[2][1]*height1 + hData[2][2]));
		region.points[1].y = cvRound((hData[1][0]*0 + hData[1][1]*height1 + hData[1][2])
			/(hData[2][0]*0 + hData[2][1]*height1 + hData[2][2]));

		//(x, y) = (width, height)进行变换
		region.points[2].x = cvRound((hData[0][0]*width1 + hData[0][1]*height1 + hData[0][2])
			/(hData[2][0]*width1 + hData[2][1]*height1 + hData[2][2]));
		region.points[2].y = cvRound((hData[1][0]*width1 + hData[1][1]*height1 + hData[1][2])
			/(hData[2][0]*width1 + hData[2][1]*height1 + hData[2][2]));

		//(x, y) = (width, 0)进行变换
		region.points[3].x = cvRound((hData[0][0]*width1 + hData[0][1]*0 + hData[0][2])
			/(hData[2][0]*width1 + hData[2][1]*0 + hData[2][2]));
		region.points[3].y = cvRound((hData[1][0]*width1 + hData[1][1]*0 + hData[1][2])
			/(hData[2][0]*width1 + hData[2][1]*0 + hData[2][2]));

		region.transformToRect(imgSIFT2->img->width, imgSIFT2->img->height);

		retrievalRes.regionCount++;
		retrievalRes.vpRegions.push_back(region);

		
		/*
		//框出矩形区域
		cvLine(imgSIFT2->img, region.points[0], region.points[1], CC_RGB(255,0,255), 2, 8, 0);
		cvLine(imgSIFT2->img, region.points[1], region.points[2], CC_RGB(255,0,255), 2, 8, 0);
		cvLine(imgSIFT2->img, region.points[2], region.points[3], CC_RGB(255,0,255), 1, 8, 0);
		cvLine(imgSIFT2->img, region.points[3], region.points[0], CC_RGB(255,0,255), 1, 8, 0);

		cvNamedWindow( "imgSIFT1->img", 1 );
		cvShowImage( "imgSIFT1->img", imgSIFT2->img );
		cvWaitKey( 0 );
		*/
		
		
		/*
		//透视变换后显示的示例图像
		IplImage* xformed;
		xformed = cvCreateImage( cvGetSize( imgSIFT2->img ), IMG_DEPTH_8U, 3 );

		//将图像1经过透视变换到对应的图像2的对应区域，其他区域用黑色填充
		cvWarpPerspective( imgSIFT1->img, xformed, H, 
		CC_INTER_LINEAR + CC_WARP_FILL_OUTLIERS,
		cvScalarAll( 0 ) );

		cvNamedWindow( "Xformed", 1 );
		cvShowImage( "Xformed", xformed );
		cvWaitKey( 0 );
		cvReleaseImage( &xformed );
		*/
		

		cvReleaseMat( &H );
		kdtree_release( kd_root );
		return true;
	}

	kdtree_release( kd_root );
	return false;
}