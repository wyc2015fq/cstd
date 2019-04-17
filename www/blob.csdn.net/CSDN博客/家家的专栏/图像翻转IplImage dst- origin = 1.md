# 图像翻转IplImage *dst->origin = 1 - 家家的专栏 - CSDN博客





2010年08月25日 20:18:00[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：2167








   在缩放图像中的图像翻转问题：

  例如：



IplImage *dst = 0;   //目标图像指针
CvSize dst_cvsize;   //目标图像尺寸
dst_cvsize.width = frames[i]->width * 0.5;  //目标图像的宽为源图象宽的scale倍
dst_cvsize.height = frames[i]->height * 0.5; //目标图像的高为源图象高的scale倍
dst = cvCreateImage( dst_cvsize, frames[i]->depth, frames[i]->nChannels); //构造目标图象
cvResize(frames[i], dst, CV_INTER_AREA); //缩放源图像到目标图像


dst->origin=1;           //进行图像翻转



