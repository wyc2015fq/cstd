# 最简单的视频编码器：基于libx265（编码YUV为H.265） - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2014年12月23日 17:36:43[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：16338标签：[x265																[h265																[视频																[编码																[YUV](https://so.csdn.net/so/search/s.do?q=YUV&t=blog)
个人分类：[我的开源项目																[x265](https://blog.csdn.net/leixiaohua1020/article/category/2619509)](https://blog.csdn.net/leixiaohua1020/article/category/1843731)







=====================================================

最简单的视频编码器系列文章列表：
[最简单的视频编码器：编译](http://blog.csdn.net/leixiaohua1020/article/details/42069383)



[最简单的视频编码器：基于libx264（编码YUV为H.264）](http://blog.csdn.net/leixiaohua1020/article/details/42078645)


[最简单的视频编码器：基于libx265（编码YUV为H.265）](http://blog.csdn.net/leixiaohua1020/article/details/42079101)


[最简单的视频编码器：libvpx（编码YUV为VP8）](http://blog.csdn.net/leixiaohua1020/article/details/42079217)


=====================================================

本文记录一个最简单的基于libx265的H.265(HEVC)视频编码器。此前记录的编码器是通过FFmpeg调用libx265完成编码的，例如：

[《最简单的基于FFmpeg的视频编码器-更新版（YUV编码为HEVC(H.265)）》](http://blog.csdn.net/leixiaohua1020/article/details/39770947)

相比与上文中的编码器，本文记录的编码器属于“轻量级”的编码器。因为它不再包含FFmpeg的代码，直接调用libx265完成编码。因此项目的体积非常小巧。该编码器可以将输入的YUV数据编码为H.265码流文件。

![](https://img-blog.csdn.net/20150110201506476)


## 流程图

调用libx265进行视频编码的流程图如下所示。

![](https://img-blog.csdn.net/20141222135440812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




从流程图中可以看出x265的API和x264的API十分相似。它们在用法上只有微小的不同。
流程图中主要的函数如下所示。
x265_param_alloc()：为参数集结构体x265_param分配内存。
x265_param_default()：设置参数集结构体x265_param的缺省值。
x265_picture_alloc()：为图像结构体x265_picture分配内存。
x265_picture_init()：设置图像结构体x265_picture的缺省值。
x265_encoder_open()：打开编码器。
x265_encoder_encode()：编码一帧图像。
x265_encoder_close()：关闭编码器。
x265_picture_free()：释放x265_picture_alloc()申请的资源。
x265_param_free()：释放x265_param_alloc()申请的资源。

存储数据的结构体如下所示。
x265_picture：存储压缩编码前的像素数据。
x265_nal：存储压缩编码后的码流数据。

此外流程图中还包括一个“flush_encoder”模块，该模块使用的函数和编码模块是一样的。唯一的不同在于不再输入视频像素数据。它的作用是输出编码器中剩余的码流数据。


## 源代码

```cpp
/**
 * 最简单的基于X265的视频编码器
 * Simplest X265 Encoder
 *
 * 雷霄骅 Lei Xiaohua
 * leixiaohua1020@126.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * 本程序可以YUV格式的像素数据编码为H.265码流，是最简单的
 * 基于libx265的视频编码器
 *
 * This software encode YUV data to H.265 bitstream.
 * It's the simplest encoder example based on libx265.
 */
#include <stdio.h>
#include <stdlib.h>

#if defined ( __cplusplus)
extern "C"
{
#include "x265.h"
};
#else
#include "x265.h"
#endif

int main(int argc, char** argv){
	int i,j;
	FILE *fp_src=NULL;
	FILE *fp_dst=NULL;
	int y_size;
	int buff_size;
	char *buff=NULL;
	int ret;
	x265_nal *pNals=NULL;
	uint32_t iNal=0;

	x265_param* pParam=NULL;
	x265_encoder* pHandle=NULL;
	x265_picture *pPic_in=NULL;

	//Encode 50 frame
	//if set 0, encode all frame
	int frame_num=50;
	int csp=X265_CSP_I420;
	int width=640,height=360;

	fp_src=fopen("../cuc_ieschool_640x360_yuv420p.yuv","rb");
	//fp_src=fopen("../cuc_ieschool_640x360_yuv444p.yuv","rb");

	fp_dst=fopen("cuc_ieschool.h265","wb");
	//Check
	if(fp_src==NULL||fp_dst==NULL){
		return -1;
	}

	pParam=x265_param_alloc();
	x265_param_default(pParam);
	pParam->bRepeatHeaders=1;//write sps,pps before keyframe
	pParam->internalCsp=csp;
	pParam->sourceWidth=width;
	pParam->sourceHeight=height;
	pParam->fpsNum=25;
	pParam->fpsDenom=1;
	//Init
	pHandle=x265_encoder_open(pParam);
	if(pHandle==NULL){
		printf("x265_encoder_open err\n");
		return 0;
	}
	y_size = pParam->sourceWidth * pParam->sourceHeight;

	pPic_in = x265_picture_alloc();
	x265_picture_init(pParam,pPic_in);
	switch(csp){
	case X265_CSP_I444:{
		buff=(char *)malloc(y_size*3);
		pPic_in->planes[0]=buff;
		pPic_in->planes[1]=buff+y_size;
		pPic_in->planes[2]=buff+y_size*2;
		pPic_in->stride[0]=width;
		pPic_in->stride[1]=width;
		pPic_in->stride[2]=width;
		break;
					   }
	case X265_CSP_I420:{
		buff=(char *)malloc(y_size*3/2);
		pPic_in->planes[0]=buff;
		pPic_in->planes[1]=buff+y_size;
		pPic_in->planes[2]=buff+y_size*5/4;
		pPic_in->stride[0]=width;
		pPic_in->stride[1]=width/2;
		pPic_in->stride[2]=width/2;
		break;
					   }
	default:{
		printf("Colorspace Not Support.\n");
		return -1;
			}
	}
	
	//detect frame number
	if(frame_num==0){
		fseek(fp_src,0,SEEK_END);
		switch(csp){
		case X265_CSP_I444:frame_num=ftell(fp_src)/(y_size*3);break;
		case X265_CSP_I420:frame_num=ftell(fp_src)/(y_size*3/2);break;
		default:printf("Colorspace Not Support.\n");return -1;
		}
		fseek(fp_src,0,SEEK_SET);
	}

	//Loop to Encode
	for( i=0;i<frame_num;i++){
		switch(csp){
		case X265_CSP_I444:{
			fread(pPic_in->planes[0],1,y_size,fp_src);		//Y
			fread(pPic_in->planes[1],1,y_size,fp_src);		//U
			fread(pPic_in->planes[2],1,y_size,fp_src);		//V
			break;}
		case X265_CSP_I420:{
			fread(pPic_in->planes[0],1,y_size,fp_src);		//Y
			fread(pPic_in->planes[1],1,y_size/4,fp_src);	//U
			fread(pPic_in->planes[2],1,y_size/4,fp_src);	//V
			break;}
		default:{
			printf("Colorspace Not Support.\n");
			return -1;}
		}

		ret=x265_encoder_encode(pHandle,&pNals,&iNal,pPic_in,NULL);	
		printf("Succeed encode %5d frames\n",i);

		for(j=0;j<iNal;j++){
			fwrite(pNals[j].payload,1,pNals[j].sizeBytes,fp_dst);
		}	
	}
	//Flush Decoder
	while(1){
		ret=x265_encoder_encode(pHandle,&pNals,&iNal,NULL,NULL);
		if(ret==0){
			break;
		}
		printf("Flush 1 frame.\n");

		for(j=0;j<iNal;j++){
			fwrite(pNals[j].payload,1,pNals[j].sizeBytes,fp_dst);
		}
	}
	
	x265_encoder_close(pHandle);
	x265_picture_free(pPic_in);
	x265_param_free(pParam);
	free(buff);
	fclose(fp_src);
	fclose(fp_dst);
	
	return 0;
}
```


## 运行结果

程序的输入为一个YUV文件（已经测试过YUV444P和YUV420P两种格式）。

![](https://img-blog.csdn.net/20141222135737812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


输出为H.265码流文件。

![](https://img-blog.csdn.net/20141222135753832?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


H.265码流文件的信息如下所示。

![](https://img-blog.csdn.net/20141222135803535?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


## 下载

**Simplest Encoder**


**项目主页**

SourceForge：[https://sourceforge.net/projects/simplestencoder/](https://sourceforge.net/projects/simplestencoder/)

Github：[https://github.com/leixiaohua1020/simplest_encoder](https://github.com/leixiaohua1020/simplest_encoder)

开源中国：[http://git.oschina.net/leixiaohua1020/simplest_encoder](http://git.oschina.net/leixiaohua1020/simplest_encoder)




CDSN下载地址：[http://download.csdn.net/detail/leixiaohua1020/8284105](http://download.csdn.net/detail/leixiaohua1020/8284105)

该解决方案包含了几个常见的编码器的使用示例：
simplest_vpx_encoder：最简单的基于libvpx的视频编码器
simplest_x264_encoder：最简单的基于libx264的视频编码器
simplest_x265_encoder：最简单的基于libx265的视频编码器](https://so.csdn.net/so/search/s.do?q=编码&t=blog)](https://so.csdn.net/so/search/s.do?q=视频&t=blog)](https://so.csdn.net/so/search/s.do?q=h265&t=blog)](https://so.csdn.net/so/search/s.do?q=x265&t=blog)




