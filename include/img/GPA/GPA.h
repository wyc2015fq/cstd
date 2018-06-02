
#ifndef _GPA_H_
#define _GPA_H_

#ifdef __cplusplus
extern "C"{;
#endif
	
typedef struct GPA GPA;

/************************************************************************
* 函数名称: GPA_new
* 功能描述: 创建一个GPA防抖动算法对象，用一帧图像做参考帧
* 参数列表: height 视频中一帧图像的高
* 参数列表: width  视频中一帧图像的宽
* 参数列表: reference_frame_bgr 视频中一帧图像（BGR格式）的地址，指向图像的左上角
* 参数列表: step  视频中一帧图像的一行的字节个数，不能小于 width*3
* 返回值: 返回指向GPA防抖动算法对象的指针
* 说  明: 
*************************************************************************/
GPA* GPA_new(int height, int width, const unsigned char* reference_frame_bgr, int step);

/************************************************************************
* 函数名称: GPA_update
* 功能描述: 去除当前帧的抖动。
* 参数列表: g 用GPA_new函数返回的防抖动算法对象
* 参数列表: current_frame_bgr 视频中一帧图像（BGR格式）的地址，指向图像的左上角
* 参数列表: step  视频中一帧图像的一行的字节个数，不能小于 width*3
* 参数列表: out   输出的已经去除了抖动的图像
* 参数列表: step  out_step输出图像的一行的字节个数，不能小于 width*3
* 返回值: 1 表示成功，0 表示内存分配失败
* 说  明: 
*************************************************************************/
int GPA_update(GPA* g, const unsigned char* current_frame_bgr, int step, unsigned char* out, int out_step);

/************************************************************************
* 函数名称: GPA_free
* 功能描述: 释放GPA防抖动算法对象
* 参数列表: g 用GPA_new函数返回的防抖动算法对象
* 返回值: 无
* 说  明: 
*************************************************************************/
void GPA_free(GPA* g);

#ifdef __cplusplus
}
#endif

#endif // _GPA_H_
