# MNIST手写数字数据集格式，如何读取MNIST数据集？ - Koma Hub - CSDN博客
2018年05月15日 16:20:01[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：1240
数据集下载地址：[http://yann.lecun.com/exdb/mnist/](http://yann.lecun.com/exdb/mnist/)
```
TRAINING SET LABEL FILE (train-labels-idx1-ubyte):
[offset] [type]          [value]          [description] 
0000     32 bit integer  0x00000801(2049) magic number (MSB first) 
0004     32 bit integer  60000            number of items 
0008     unsigned byte   ??               label 
0009     unsigned byte   ??               label 
........ 
xxxx     unsigned byte   ??               label
The labels values are 0 to 9.
TRAINING SET IMAGE FILE (train-images-idx3-ubyte):
[offset] [type]          [value]          [description] 
0000     32 bit integer  0x00000803(2051) magic number 
0004     32 bit integer  60000            number of images 
0008     32 bit integer  28               number of rows 
0012     32 bit integer  28               number of columns 
0016     unsigned byte   ??               pixel 
0017     unsigned byte   ??               pixel 
........ 
xxxx     unsigned byte   ??               pixel
Pixels are organized row-wise. Pixel values are 0 to 255. 0 means background (white), 255 means foreground (black).
TEST SET LABEL FILE (t10k-labels-idx1-ubyte):
[offset] [type]          [value]          [description] 
0000     32 bit integer  0x00000801(2049) magic number (MSB first) 
0004     32 bit integer  10000            number of items 
0008     unsigned byte   ??               label 
0009     unsigned byte   ??               label 
........ 
xxxx     unsigned byte   ??               label
The labels values are 0 to 9.
TEST SET IMAGE FILE (t10k-images-idx3-ubyte):
[offset] [type]          [value]          [description] 
0000     32 bit integer  0x00000803(2051) magic number 
0004     32 bit integer  10000            number of images 
0008     32 bit integer  28               number of rows 
0012     32 bit integer  28               number of columns 
0016     unsigned byte   ??               pixel 
0017     unsigned byte   ??               pixel 
........ 
xxxx     unsigned byte   ??               pixel
Pixels are organized row-wise. Pixel values are 0 to 255. 0 means background (white), 255 means foreground (black).
```
上链接包含四个文件：两个train文件，两个测试文件，分别包含28*28的点阵和labels(也就是0~9)，以训练数据为例，给出images和labels的文件结构：
![](https://img-blog.csdn.net/20180515161327112)
训练数据中包括60000个28*28的手写图片，测试数据中包含10000个28*28的手写图片。下面给出GitHub上的一段源代码，其中我在代码中给出了完全的注释：
```cpp
#ifndef __MNIST_H__//定义mnist数据集的头文件宏，就像_omp_
#define __MNIST_H__
/* Copyright (c) 2014 Nuri Park
 * MNIST loader by Nuri Park - https://github.com/projectgalateia/mnist
 */
#ifdef USE_MNIST_LOADER /* Fundamental macro to make the code active 主程序调用该时需对其进行宏定义*/
#ifdef __cplusplus//满足C++编译条件
extern "C" {
#endif
/*
 * Make mnist_load function static.
 * Define when the header is included multiple time.
 */
#ifdef MNIST_STATIC
#define _STATIC static
#else
#define _STATIC 
#endif
/*
 * Make mnist loader to load image data as double type.
 * It divides unsigned char values by 255.0, so the results ranges from 0.0 to 1.0
 */
#ifdef MNIST_DOUBLE
#define MNIST_DATA_TYPE double //如果定义了MNIST_DOUBLE，那么28*28的矩阵中将保存double
#else
#define MNIST_DATA_TYPE unsigned char//否则28*28矩阵中保存的是unsigned char类型
#endif
typedef struct mnist_data {//一个mnist数据集image的结构体
	MNIST_DATA_TYPE data[28][28]; /* 28x28 data for the image */
	unsigned int label; /* label : 0 to 9 */
} mnist_data;
/*
 * If it's header inclusion, make only function prototype visible.
 */
#ifdef MNIST_HDR_ONLY
_STATIC int mnist_load(
	const char *image_filename,
	const char *label_filename,
	mnist_data **data,
	unsigned int *count);
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 * Load a unsigned int from raw data.
 * MSB first.
 *  也可以使用下面的函数：ReverseInt((int*)tmp)
 */
static unsigned int mnist_bin_to_int(char *v)
{
	int i;
	unsigned int ret = 0;
	for (i = 0; i < 4; ++i) {
		ret <<= 8;
		ret |= (unsigned char)v[i];
	}
	return ret;
}
static int ReverseInt(int *i)  
{  
    unsigned char ch1, ch2, ch3, ch4;  
    ch1 = *i & 255;  
    ch2 = (*i >> 8) & 255;  
    ch3 = (*i >> 16) & 255;  
    ch4 = (*i >> 24) & 255;  
    return((int)ch1 << 24) + ((int)ch2 << 16) + ((int)ch3 << 8) + ch4;  
} 
/*
 * MNIST dataset loader.
 *
 * Returns 0 if successed.
 * Check comments for the return codes.
 */
_STATIC int mnist_load(
	const char *image_filename,
	const char *label_filename,
	mnist_data **data,
	unsigned int *count)
{
	int return_code = 0;
	int i;
	char tmp[4];//一个int
	unsigned int image_cnt, label_cnt;
	unsigned int image_dim[2];//row and col
	FILE *ifp = fopen(image_filename, "rb");//数据集图片 28*28
	FILE *lfp = fopen(label_filename, "rb");//数据集label  0~9
	if (!ifp || !lfp) {//判断文件是否存在，不存在退出
		return_code = -1; /* No such files */
		goto cleanup;
	}
    /*Magic number一般是指硬写到代码里的整数常量，数值是编程者自己指定的，
      其他人不知道数值有什么具体意义，表示不明觉厉，就称作magic number。*/
	fread(tmp, 1, 4, ifp);//从images文件中读取magic number
	if (mnist_bin_to_int(tmp) != 2051) {
		return_code = -2; /* Not a valid image file */
		goto cleanup;
	}
    printf("magic number: %d (%s)\n", mnist_bin_to_int(tmp), tmp);
	fread(tmp, 1, 4, lfp);//从label文件中读取magic number
	if (mnist_bin_to_int(tmp) != 2049) {
		return_code = -3; /* Not a valid label file */
		goto cleanup;
	}
    printf("magic number: %d (%s)\n", mnist_bin_to_int(tmp), tmp);
	fread(tmp, 1, 4, ifp);//从images文件中读取图片个数：28*28
	image_cnt = mnist_bin_to_int(tmp);
    printf("number of images: %d (%s)\n", mnist_bin_to_int(tmp), tmp);
    
	fread(tmp, 1, 4, lfp);//从label文件中读取图片个数：28*28
	label_cnt = mnist_bin_to_int(tmp);
    printf("number of images: %d (%s)\n", mnist_bin_to_int(tmp), tmp);
	if (image_cnt != label_cnt) {//如果images和label的头文件中对应的图片个数不相同，则返回退出
		return_code = -4; /* Element counts of 2 files mismatch */
		goto cleanup;
	}
	for (i = 0; i < 2; ++i) {//读取图片的维度：row 和 col
		fread(tmp, 1, 4, ifp);
		image_dim[i] = mnist_bin_to_int(tmp);
	}
    printf("image dimension: %d * %d\n", image_dim[0], image_dim[1]);
	if (image_dim[0] != 28 || image_dim[1] != 28) {
		return_code = -2; /* Not a valid image file */
		goto cleanup;
	}
    /*返回图片个数60000*/
	*count = image_cnt;
    /*为指针（数组）开辟60000个mnist格式空间*/
	*data = (mnist_data *)malloc(sizeof(mnist_data) * image_cnt);
    /*读取60000个28*28的图片*/
	for (i = 0; i < image_cnt; ++i) {
		int j;
		unsigned char read_data[28 * 28];
		mnist_data *d = &(*data)[i];
		fread(read_data, 1, 28*28, ifp);//读取28*28的图片
#ifdef MNIST_DOUBLE//如果是double型，则进行此操作
        //printf("double.\n");
		for (j = 0; j < 28*28; ++j) {
			d->data[j/28][j%28] = read_data[j] / 255.0;
		}
#else//如果是char型则进行此操作
		memcpy(d->data, read_data, 28*28);//把读取的28*28图片赋值给结构体的data
#endif
		fread(tmp, 1, 1, lfp);//读取对应的label
		d->label = tmp[0];//给结构体的label赋值
	}
cleanup://对应上面的goto
	if (ifp) fclose(ifp);
	if (lfp) fclose(lfp);
	return return_code;
}
#endif /* MNIST_HDR_ONLY */
#ifdef __cplusplus//c++支持
}
#endif
#endif /* USE_MNIST_LOADER */
#endif /* __MNIST_H__ */
```
详细介绍一下magic number的计算过程：
![](https://img-blog.csdn.net/20180515162056209)
使用方法为：
```cpp
#define USE_MNIST_LOADER
#define MNIST_DOUBLE
#include "mnist.h"
#include <stdio.h>
int main(int argc, char **argv)
{
    mnist_data *mnist;
    unsigned int cnt;
    int ret;
    int i, j;
    if (ret = mnist_load("train-images-idx3-ubyte", "train-labels-idx1-ubyte", &mnist, &cnt)) 
    {
        printf("An error occured: %d\n", ret);
    } 
    else 
    {
        printf("image count: %d, %d\n", cnt, sizeof(mnist->data));
        
        if(1)
        for(j=0;j<cnt;j++)
        {
            printf("\n%d\n",mnist[j].label);
            for(i=0;i<28*28;i++)
            {
                if(mnist[j].data[i/28][i%28]==0)
                {
                    printf(" ");
                }
                else 
                {
                    printf("#",mnist[j].data[i/28][i%28]);
                }
                if(!(i%28))
                    printf("\n");
            }
        }
    }
    free(mnist);
    return 0;
}
```
我的编译环境为：
```
>gcc --version
gcc (MinGW.org GCC-6.3.0-1) 6.3.0
Copyright (C) 2016 Free Software Foundation, Inc.
```
屏幕输出为（简单给出几个例子）：
```
9
            #######
           ########
          #########
          ### #####
         ###  ####
         ###   ###
         #########
          #######
           ######
              ###
             ###
            ####
            ###
           ####
           ###
          ####
          ###
         ####
        ####
        ####
3
     ##########
     ############
      ###########
              ###
              ###
             ###
            ####
           ####
          ####
          #######
          #########
             ######
                ###
                ###
               ###
              ####
              ###
             ###
          ######
          ####
9
           #########
         ###########
        ###### #####
       ######  #####
      #####   #####
      ####   ######
      ####  #######
      ############
      ############
      ####### ###
             ####
             ####
             ###
            ####
            ###
           ####
           ###
         #####
         ####
         ####
8
           #######
            #######
         ##########
       #############
       ############
      ###     #####
      ####    ####
       #####  ###
       #########
          #####
           ####
         #######
         ### ####
        ####  ####
        ####   ###
        ###    ###
        ###    ###
        ###   ####
        #########
         #######
```
