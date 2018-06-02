#include "StdAfx.h"
#include "MySIFT.h"
#include <pmalloc.h>

extern "C"
{
	//SIFT库文件
	#include "sift.h"
	#include "imgfeatures.h"
	#include "kdtree.h"
	#include "utils.h"
	#include "xform.h"
};

static struct feature* clone_feature( struct feature* );
static struct feature* new_feature( void );

static struct feature* new_feature( void )
{
	struct feature* feat;
	struct detection_data* ddata;

	feat = (struct feature *)pmalloc( sizeof( struct feature ) );
	memset( feat, 0, sizeof( struct feature ) );
	ddata = (struct detection_data *)pmalloc( sizeof( struct detection_data ) );
	memset( ddata, 0, sizeof( struct detection_data ) );
	feat->feature_data = ddata;
	feat->type = FEATURE_LOWE;

	return feat;
}

//注意：由于struct feature中有大量的指针类型必须进行深度拷贝
//目前还有很多问题。
static struct feature* clone_feature( struct feature* feat )
{
	struct feature* new_feat;
	struct detection_data* ddata;

	new_feat = new_feature();
	ddata = feat_detection_data( new_feat );
	memcpy( new_feat, feat, sizeof( struct feature ) );
	if (feat_detection_data(feat))
	{
		memcpy( ddata, feat_detection_data(feat), sizeof( struct detection_data ) );
	}
	else
	{
		ddata = NULL;
	}
	new_feat->feature_data = ddata;

	return new_feat;
}

CMySIFT::CMySIFT(void)
{
}

CMySIFT::~CMySIFT(void)
{
	cvReleaseImage( &img );
	pfree( feat );
}

bool CMySIFT::initialize(CString img_filePath, bool isBlur)
{
	img = cvLoadImage(img_filePath, 1);
	if( ! img )
		return false;

	if (isBlur)
	{
		IplImage *tmpblur = cvCloneImage(img);
		cvSmooth(tmpblur, img, CC_GAUSSIAN, 3, 0, 0, 0);
		cvReleaseImage(&tmpblur);
	}

	/*计算边缘
	IplImage* img_gray = cvCreateImage(cvSize(img->width, img->height), img->depth, 1);
	cvCvtColor(img, img_gray, CC_BGR2GRAY);
	IplImage* img_cny = cvCreateImage(cvGetSize(img_gray), img_gray->depth, 1);
	cvCanny(img_gray, img_cny, 50, 100, 3);
	cvReleaseImage(&img);
	img = img_cny;
	*/

	num = sift_features( img, &feat );
	return true;
}

bool CMySIFT::cloneSiftFeature(CMySIFT& sift)
{
	img = cvCreateImage( cvGetSize( sift.img ), IMG_DEPTH_8U, IMAGE_CHANNELS );
	cvCopyImage(sift.img, img);
	num = sift.num;
	feat = clone_feature(sift.feat);

	return true;
}
