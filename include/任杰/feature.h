#ifndef _FEATURE_H_
#define _FEATURE_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _USRDLL
#define DECLSPEC __declspec(dllexport)
#else
#define DECLSPEC
#endif

typedef unsigned char uchar;

// 复数类型定义
typedef struct tag_complex_se {
	double re, im;
} complex_se;

/* 用于生成PP+LBP59BIN特征
 * ah:      该函数仅适用于100 x 100灰度图, 必须为100.
 * aw:      该函数仅适用于100 x 100灰度图, 必须为100.
 * A :      图像数据指针.
 * al:      该函数仅适用于100 x 100灰度图, 必须为100.
 * ai:      该函数仅适用于100 x 100灰度图, 必须为1.
 * type:    特征类型, 当前该值可指定任意值.
 * featptr: 特征指针, 该指针为空时函数返回所需要的float类型特征数组大小, 否则特征存入该数组.
*/
DECLSPEC int feature_gen(int ah, int aw, const uchar* A, int al, int ai, int type, float* featptr);

/* 用于生成PP+LGBP8BIN特征
 * ah:      该函数仅适用于80 x 80灰度图, 必须为80.
 * aw:      该函数仅适用于80 x 80灰度图, 必须为80.
 * A:       图像数据指针.
 * al:      该函数仅适用于80 x 80灰度图, 必须为80
 * ai:      该函数仅适用于80 x 80灰度图, 必须为1.
 * type:    特征类型, 当前该值可指定任意值.
 * featptr: 特征指针, 该指针为空时函数返回所需要的float类型特征数组大小, 否则特征存入该数组.
*/
DECLSPEC int feature_gen2(int ah, int aw, const unsigned char* A, int al, int ai, int type, float* featptr);

/*
 * n: 特征块个数, 此处仅适用于n为40的情况
 * a: 特征a首指针, a的长度为40 * 191
 * b: 特征b首指针, b的长度为40 * 191
*/
DECLSPEC float cos_dist(int n, const float *a, const float *b);

#ifdef __cplusplus
}
#endif

#endif