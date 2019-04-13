
# 图像处理中卷积的实现（TensorFlow和OpenCV） - 郭云飞的专栏 - CSDN博客


2017年10月24日 10:30:07[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：1283


**一、用C解释原理**
假设图像（宽6高4），一个卷积核（宽3高3），如下：

```python
unsigned char src[24] ={
    1,2,3,4,5,6,
    1,1,1,1,1,1,
    2,1,2,1,2,1,
    4,5,6,1,2,3};
```
```python
float kernel[9] = {
    -1., 0., 1., 
    -2., 0., 2.,
    -3., 0., 3.};
```
卷积的步骤为：1、对kernel做逆时针180°旋转；2、遍历src，加权求和
C代码（亲测通过）：

```python
bool conv2d(unsigned char* src, float* dst, int nRows, int nCols, 
			float* kernel, int nKernelRows, int nKernelCols)
{
	if (nKernelRows % 2 == 0 || nKernelCols % 2 == 0) {
		cout << "The kernel size must be an odd" << endl;
		return false;
	}
	// we take the center as the anchor of kernel
	int nCenRow = nKernelRows / 2;
	int nCenCol = nKernelCols / 2;
	int nCenOff = nCenRow * nKernelCols + nCenCol;
	// turn rotate 180 degrees anti-clockwise to the kernel image.
	float* RotateKernel = (float*)malloc(sizeof(float) * nKernelCols * nKernelRows);
	for (int r = 0; r < nKernelRows; r++) {
		for (int c = 0; c < nKernelCols; c++) {
			RotateKernel[(nKernelRows - r) * nKernelCols - 1 - c] = kernel[c + r * nKernelRows];
		}
	}
	// test code - show rotate kernel
/*	for(int r = 0; r < nKernelRows; r++) {
		for (int c = 0; c < nKernelCols; c++) {
			cout << setw(8) << setiosflags(ios::fixed) << setprecision(2)
			<< RotateKernel[r * nKernelCols + c];
		}
		cout << endl;
	} */
	for (int r = 0; r < nRows; r++) {
		for (int c = 0; c < nCols; c++) {
			float fTemp = 0.f;
			for (int kr = -nCenRow; kr <= nCenRow; kr++) {
				for (int kc = -nCenCol; kc <= nCenCol; kc++) {
					// border type - constant 0
					if (r + kr >= 0 && r + kr < nRows && c + kc >= 0 && c + kc < nCols) {
						fTemp += src[(r + kr) * nCols + c + kc] * 1.f * 
						RotateKernel[kr * nKernelCols + kc + nCenOff];
/*						cout 
						<< setw(4) << r 
						<< setw(4) << c 
						<< setw(4) << kr 
						<< setw(4) << kc 
						<< setw(4) << src[(r + kr) * nCols + c + kc] * 1.f
						<< setw(4) << RotateKernel[kr * nKernelCols + kc + nCenOff]
						<< setw(4) << fTemp
						<< endl; */
					}
				}
			}
			dst[r * nCols + c] = fTemp;
		}
	}
	
	free(RotateKernel);
	return true;
}
// test
int main(int argc, char** argv)
{
	cout << ">> ----" << "\n" << endl;
	
	int nRows = 4;
	int nCols = 6;
	unsigned char src[24] ={1,2,3,4,5,6,
							1,1,1,1,1,1,
							2,1,2,1,2,1,
							4,5,6,1,2,3};
	float dst[24];
	memset(dst, 0, 24*sizeof(float));
	float kernel[9] = {-1., 0., 1., 
					   -2., 0., 2.,
					   -3., 0., 3.};
	int nKernelRows = 3;
	int nKernelCols = 3;
	conv2d(src, dst, nRows, nCols, kernel, nKernelRows, nKernelRows);
	// test code
	for(int r = 0; r < nRows; r++) {
		for (int c = 0; c < nCols; c++) {
			cout << setw(8) << setiosflags(ios::fixed) << setprecision(2) << dst[r * nCols + c];
		}
		cout << endl;
	}
	cout << "\n" << ">> ----" << endl;
	return 0;
}
```
**二、OpenCV卷积**
上边代码仅用来说明原理，实际中，这样的代码效率很渣哈！不想自己优化的，就用OpenCV的现成的！

```python
void filter2D(Mat src,
              Mat dst,
              int ddepth,
              Mat kernel,
              Point anchor,
              double delta,
              int borderType);
```
```python
src: (input) This is the image that you want to convolve.
dst: (input) This image stores the final result of the convolution. It should be the same size and have the same number of channels as src. This can be the same as src (in place operation is supported).
ddepth: (input) This is the desired bit depth of the final result (8, 16, 32, etc). It it is negative, the depth is the same as the source image.
kernel: (input) The convolution kernel used to convolve the source image. This has to be a single channel, floating point matrix. If you want to apply different kernels to different channels, you need to split the channels, and convolve each of them them individually.
anchor: (input) The relative position of the anchor in the kernel matrix. If this is set to (-1,-1), the center of the kernel is used as the anchor point.
delta: (input) A value that is added to all pixels after convolution.
borderType: (input) Possible values for this include:
BORDER_REPLICATE
BORDER_CONSTANT
BORDER_REFLECT_101
BORDER_WARP
BORDER_TRANSPARENT
BORDER_DEFAULT (same as reflect)
BORDER_ISOLATED
```
borderType的具体解释：

```python
enum BorderTypes {  
    BORDER_CONSTANT    = 0, //!< `iiiiii|abcdefgh|iiiiiii`  with some specified `i`  
    BORDER_REPLICATE   = 1, //!< `aaaaaa|abcdefgh|hhhhhhh`  
    BORDER_REFLECT     = 2, //!< `fedcba|abcdefgh|hgfedcb`  
    BORDER_WRAP        = 3, //!< `cdefgh|abcdefgh|abcdefg`  
    BORDER_REFLECT_101 = 4, //!< `gfedcb|abcdefgh|gfedcba`  
    BORDER_TRANSPARENT = 5, //!< `uvwxyz|absdefgh|ijklmno`  
  
    BORDER_REFLECT101  = BORDER_REFLECT_101, //!< same as BORDER_REFLECT_101  
    BORDER_DEFAULT     = BORDER_REFLECT_101, //!< same as BORDER_REFLECT_101  
    BORDER_ISOLATED    = 16 //!< do not look outside of ROI  
};
```
但是，用filter2D时，要时刻清醒一点：
这个算子实际就算的是相关（correlation），如果你的卷积核是对称（symmetrical）的，则在数学结果上，卷积核相关的值是相同的。但如果卷积核不对称，必须向上边那样，先做个180°旋转！
另外，如果卷积核很大的话，该函数会自动采用离散傅里叶变换算法来加速计算！
**三、TensorFlow中卷积和OpenCV卷积对比试验**
在Python环境下用TensorFlow实现了卷积（核心就tf.nn.conv2d怎么一个函数），代码如下：

```python
import numpy as np
import tensorflow as tf
def weight_variable(shape):
	initial = tf.truncated_normal(shape, mean = 0.0, stddev = 0.1, dtype = tf.float32)
	return tf.Variable(initial)
# Input testing sample (images) #################
InputShape = (6, 5, 2) # rows * columns * channels
aSample    = weight_variable([1, InputShape[0], InputShape[1], InputShape[2]])

# Layer 0 : convolution #########################
L0_KerSize = 3
L0_KerNum  = 4
L0_W       = weight_variable ([L0_KerSize, L0_KerSize, InputShape[2], L0_KerNum])
L0_Conv    = tf.nn.conv2d(aSample, L0_W, strides=[1,1,1,1], padding='SAME')
with tf.Session() as session:
	session.run(tf.initialize_all_variables())
	aa = session.run(aSample)
	for i in range(0, aa.shape[3]):
		print '----------------------------------- image ', i
		print aa[0, :, :, i]
	
	ww = session.run(L0_W)
	for i in range(0, ww.shape[2]):
		for j in range(0, ww.shape[3]):
			print '----------------------------------- W in_channels: ', i, ', kernel: ', j
			print ww[:, :, i, j]
	ke = session.run(L0_Conv)
	for i in range(0, ke.shape[3]):
		print '----------------------------------- result channel: ', i
		print ke[:, :, :, i]
	
	# save model
	np.savez('./tfModelWandB.npz', l0w = session.run(L0_W))
	# save the sample
	np.savez('./tfModelSample.npz',img = session.run(aSample))
```
上边代码中，输入图像在尺寸为6×5×2（宽×高×通道），图像在数据我们用随机直。卷积核在尺寸为3×3×2×4（卷积核大小为3×3, 由于输入图像有俩通道，所以共8个核）。
运行上述Python脚本，结果如下：

```python
----------------------------------- image  0
[[ 0.12625436 -0.11937501 -0.10891404 -0.02895968 -0.01037076]
 [ 0.10400867 -0.03660493 -0.06411878  0.03639115 -0.07101893]
 [ 0.08615526  0.02973037 -0.07166351  0.09142997 -0.09188165]
 [ 0.03162669  0.10494743  0.11962699  0.0277028   0.03729795]
 [-0.05309613 -0.08204257  0.05443277 -0.03429593 -0.07877157]
 [-0.01566831 -0.04015071 -0.09142643 -0.00112125  0.11420252]]
----------------------------------- image  1
[[-0.11111959 -0.16222687 -0.01761985  0.04520827  0.10118878]
 [-0.09962401 -0.08819477 -0.00178826  0.07190076  0.02236971]
 [ 0.11599306  0.01306704 -0.0909201   0.06941512 -0.08966626]
 [-0.1846966   0.01611226 -0.05774776  0.04476134  0.14001481]
 [ 0.02164672  0.07135325 -0.03206375  0.02136724 -0.05802757]
 [-0.09017277 -0.02534078 -0.0369864   0.02165738  0.08163627]]
----------------------------------- W in_channels:  0 , kernel:  0
[[-0.1011525   0.10236194 -0.00020576]
 [-0.00089763 -0.07612719  0.09727343]
 [ 0.0805968  -0.11845497 -0.05366211]]
----------------------------------- W in_channels:  0 , kernel:  1
[[-0.076056   -0.06423963 -0.04123518]
 [ 0.07255634  0.06006186 -0.18577591]
 [-0.05044123  0.12277362 -0.02045252]]
----------------------------------- W in_channels:  0 , kernel:  2
[[-0.01193008 -0.14593969 -0.09650309]
 [-0.13044347  0.11086304 -0.04400647]
 [ 0.01193826 -0.08462534  0.07637326]]
----------------------------------- W in_channels:  0 , kernel:  3
[[-0.13617061  0.01164558 -0.12195086]
 [ 0.07623294  0.0258304   0.13812433]
 [-0.03091484  0.10123607 -0.0035188 ]]
----------------------------------- W in_channels:  1 , kernel:  0
[[-0.07265482  0.01244575  0.00212537]
 [ 0.04495018  0.023914   -0.1128113 ]
 [-0.00164994  0.16139059  0.06106531]]
----------------------------------- W in_channels:  1 , kernel:  1
[[ 0.00165231  0.04590112 -0.06946135]
 [-0.15189163 -0.19788276 -0.09452266]
 [ 0.17610592 -0.13991795 -0.0667515 ]]
----------------------------------- W in_channels:  1 , kernel:  2
[[ 0.02323942  0.03866912  0.01701009]
 [-0.19660048  0.06464723  0.09740929]
 [-0.04083445  0.10340548 -0.01418425]]
----------------------------------- W in_channels:  1 , kernel:  3
[[ 0.12228265 -0.07337139 -0.09307034]
 [ 0.06473815  0.11169808 -0.16438608]
 [-0.09561314  0.03154143 -0.00246837]]
----------------------------------- result channel:  0
[[[-0.03739976 -0.00652579 -0.00029235 -0.00252694  0.02010462]
  [ 0.01502605 -0.02524716  0.00445372 -0.00619115  0.01265023]
  [-0.03134274 -0.0185694  -0.00523668  0.02338057  0.0123302 ]
  [ 0.03039362  0.0001417  -0.03971532  0.02063169 -0.02515039]
  [-0.02260888  0.04110029 -0.00449737 -0.00852947  0.00462775]
  [-0.00700756 -0.0101008   0.01076016 -0.00560605 -0.01263588]]]
----------------------------------- result channel:  1
[[[ 0.10042734  0.05935576 -0.0128195  -0.02626312 -0.03063701]
  [ 0.03694522  0.07160199  0.00495993 -0.00277422 -0.00050682]
  [-0.00165216 -0.01934666  0.0072494   0.0039469   0.00272913]
  [ 0.00245166  0.00575879  0.03114944 -0.0073841  -0.03136678]
  [-0.00172585 -0.05503821 -0.03527689 -0.01038689  0.00948465]
  [ 0.02958811  0.04824578  0.00224401 -0.02803131 -0.00762568]]]
----------------------------------- result channel:  2
[[[-0.02438386 -0.02085204  0.05009508  0.02561796  0.00610304]
  [-0.00906303  0.00712102  0.03920614  0.02884112 -0.02172693]
  [-0.01397291 -0.02041199 -0.02616402  0.0495156  -0.0192653 ]
  [-0.02276399  0.05459163 -0.01967556  0.01101356  0.0106208 ]
  [-0.02624287 -0.04140569 -0.01417977 -0.00138508 -0.01365112]
  [ 0.00944899  0.02417353 -0.00302217  0.03445129  0.02398454]]]
----------------------------------- result channel:  3
[[[ 0.00876225 -0.03086763 -0.03310447 -0.01490853 -0.00273006]
  [ 0.05252417 -0.03308056 -0.03416688  0.01424855 -0.01157384]
  [ 0.03530029  0.03657329 -0.01992576  0.01661721  0.00349931]
  [-0.02489512  0.03342621 -0.01179191  0.00639945  0.01237202]
  [-0.02661994 -0.01541688 -0.03310237 -0.03533022 -0.0057847 ]
  [-0.01069926 -0.01741536  0.01016802 -0.00291296  0.02400826]]]
```
为了便于和OpenCV结果对比，上边输出了图像数据、卷积核数据、以及卷积结果。
另外：为了OpenCV下能用到相同胡图像数据、和卷积核数据，我们利用numpy.savez()函数对数据进行存储。
好，接下来在C++ 环境下利用OpenCV的cv::filter2D函数进行卷积，代码如下：

```python
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <dirent.h>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <sys/io.h>
#include <sys/times.h>
#include <iomanip> // setw()
using namespace std;
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
#include "opencv2/ximgproc.hpp"
using namespace cv::ximgproc;
#include "cnpy.h";
bool loadImage(std::vector<cv::Mat> &Img);
void loadW(std::vector<cv::Mat> &W);
int main(int argc, char** argv)
{
	cout << ">> ----" << "\n" << endl;
	// load image data
	std::vector<cv::Mat> splitImg;
	loadImage(splitImg);
	// load W
	std::vector<cv::Mat> W;
	loadW(W);
	// output data opencv memeory
	int nKernelNumber = (int)W.size() / (int)splitImg.size();
	std::vector<cv::Mat> dst;
	for (int i = 0; i < nKernelNumber; i++) {
		cv::Mat tmp = cv::Mat(splitImg[0].rows, splitImg[0].cols, CV_32FC1, Scalar::all(0));
		dst.push_back(tmp);
	}
	// convolution ops
	for (int i = 0; i < (int)splitImg.size(); i++) {
		for (int j = 0; j < nKernelNumber; j++) {
			cv::Mat tmp;
			cv::filter2D(splitImg[i], tmp, CV_32F, W[i*nKernelNumber + j], 
				         Point(-1, -1), 0, BORDER_CONSTANT);
			cv::add(dst[j], tmp, dst[j]);
		}
	}
	// debug
	if (1) {
		for (int i = 0; i < (int)dst.size(); i++) {
			cout << "----------------------- result channel: " << i << endl;
			for (int r = 0; r < dst[i].rows; r++) { // y
				cout << ">> ";
				for (int c = 0; c < dst[i].cols; c++) { // x
					cout << setw(12) << dst[i].at<float>(r, c); 
				}
				cout << endl;
			}
		}
	}
	cout << "\n" << ">> ----" << endl;
	return 0;
}
/***************************
* Read a tensor from file. 
* The tensor format must be [batch, height, width, channel], and here the 
* batch mus be 1
* This means that the data is a single image sample with one or more channels.
*/
bool loadImage(std::vector<cv::Mat> &Img)
{
	string strFile = "./tfModelSample.npz";
	if (access(strFile.c_str(), 0) == -1) {  
        cout << ">> error. File not exists. Info = " << strFile.c_str() << endl;  
        return false;  
    }  
    cnpy::npz_t npzData = cnpy::npz_load(strFile);
    cnpy::NpyArray arr = npzData["img"];
    if (arr.shape.size() != 4) {
			cout << ">> error. shape.size() != 4" << endl;
			return false;
	}
	if (arr.shape[0] != 1) {
		cout << ">> error. the 1st dims must be 1. That is batch = 1 always" << endl;
		return false;
	}
	// Please attention: 
	float *mv1 = arr.data<float>();  
	int nOffset0 = arr.shape[1]*arr.shape[2]*arr.shape[3];  
	int nOffset1 = arr.shape[2]*arr.shape[3];  
	int nOffset2 = arr.shape[3];
	// opencv memeory
	for (int i = 0; i < arr.shape[3]; i++) {
		cv::Mat tmp = cv::Mat(arr.shape[1], arr.shape[2], CV_32FC1, Scalar::all(0));
		Img.push_back(tmp);
	}
	// copy data
	for (int c = 0; c < arr.shape[2]; c++) { // x
		for (int r = 0; r < arr.shape[1]; r++) { // y
			for (int k = 0; k < arr.shape[3]; k++) {
				Img[k].at<float>(r, c) = mv1[r*nOffset1 + c*nOffset2 + k];
			}
		}
	}
	// debug
	if (1) {
		for (int i = 0; i < (int)Img.size(); i++) {
			cout << "----------------------- image " << i << endl;
			for (int r = 0; r < Img[i].rows; r++) { 
				cout << ">> ";
				for (int c = 0; c < Img[i].cols; c++) { 
					cout << setw(12) << Img[i].at<float>(r, c); 
				}
				cout << endl;
			}
		}
	}
	return true;
}
/**************************
* Read a tensor from file.
* The tensor format is [kernel_height, kernel_width, in_channels, kernel_number]
* This means the tensor is a W used for convolution like the useage in 
* tf.nn.conv2d().
*/
void loadW(std::vector<cv::Mat> &W)
{
	string strFile = "./tfModelWandB.npz";
	if (access(strFile.c_str(), 0) == -1) {  
        cout << ">> error. File not exists. Info = " << strFile.c_str() << endl;  
        return false;  
    }  
    cnpy::npz_t npzData = cnpy::npz_load(strFile);  
    cnpy::NpyArray arr = npzData["l0w"];  
    // Please attention: if dtype = tf.float32 in tensorflow, here the data type  
    // must be float, if you use double, the data will be wrong.   
    float *mv1 = arr.data<float>();  
    int nOffset0 = arr.shape[1]*arr.shape[2]*arr.shape[3];  
    int nOffset1 = arr.shape[2]*arr.shape[3];  
    int nOffset2 = arr.shape[3];  
	// opencv memory
	for (int i = 0; i < arr.shape[2] * arr.shape[3]; i++) {
		cv::Mat curMat = cv::Mat(arr.shape[0], arr.shape[1], CV_32FC1, Scalar::all(0));
		W.push_back(curMat);
	}
	// copy data
    for (int r = 0; r < arr.shape[0]; r++) {  
        for (int c = 0; c < arr.shape[1]; c++) {  
            for (int chan = 0; chan < arr.shape[2]; chan++) {
                for (int k = 0; k < arr.shape[3]; k++) {  
 					W[chan*arr.shape[3] + k].at<float>(r, c) = 
 					mv1[r*nOffset0 + c*nOffset1 + chan*nOffset2 + k];
                }  
            }  
        }  
    } 
	// debug
	if (1) {
		for (int i = 0; i < arr.shape[2]; i++) {
			for (int j = 0; j < arr.shape[3]; j++) {
					cout << "----------------------- W in_channels: " 
					     << i << ", kernel: " << j << endl;
				for (int r = 0; r < W[i*arr.shape[3] + j].rows; r++) { // y
					cout << ">> ";
					for (int c = 0; c < W[i*arr.shape[3] + j].cols; c++) { // x
						cout << setw(12) << W[i*arr.shape[3] + j].at<float>(r, c); 
					}
					cout << endl;
				}
			}
		}
	}
}
```
首先读取了之前存好胡图像数据和卷积核数据。这部分用到了cnpy，一个外国小哥写的读取numpy.array在库。
运行结果如下：

```python
raintai@ml-high:~/z$ ./main.pio 
>> ----
----------------------- image 0
>>     0.126254   -0.119375   -0.108914  -0.0289597  -0.0103708
>>     0.104009  -0.0366049  -0.0641188   0.0363911  -0.0710189
>>    0.0861553   0.0297304  -0.0716635     0.09143  -0.0918816
>>    0.0316267    0.104947    0.119627   0.0277028   0.0372979
>>   -0.0530961  -0.0820426   0.0544328  -0.0342959  -0.0787716
>>   -0.0156683  -0.0401507  -0.0914264 -0.00112125    0.114203
----------------------- image 1
>>     -0.11112   -0.162227  -0.0176198   0.0452083    0.101189
>>    -0.099624  -0.0881948 -0.00178826   0.0719008   0.0223697
>>     0.115993    0.013067  -0.0909201   0.0694151  -0.0896663
>>    -0.184697   0.0161123  -0.0577478   0.0447613    0.140015
>>    0.0216467   0.0713532  -0.0320638   0.0213672  -0.0580276
>>   -0.0901728  -0.0253408  -0.0369864   0.0216574   0.0816363
----------------------- W in_channels: 0, kernel: 0
>>    -0.101153    0.102362-0.000205758
>> -0.000897632  -0.0761272   0.0972734
>>    0.0805968   -0.118455  -0.0536621
----------------------- W in_channels: 0, kernel: 1
>>    -0.076056  -0.0642396  -0.0412352
>>    0.0725563   0.0600619   -0.185776
>>   -0.0504412    0.122774  -0.0204525
----------------------- W in_channels: 0, kernel: 2
>>   -0.0119301    -0.14594  -0.0965031
>>    -0.130443    0.110863  -0.0440065
>>    0.0119383  -0.0846253   0.0763733
----------------------- W in_channels: 0, kernel: 3
>>    -0.136171   0.0116456   -0.121951
>>    0.0762329   0.0258304    0.138124
>>   -0.0309148    0.101236  -0.0035188
----------------------- W in_channels: 1, kernel: 0
>>   -0.0726548   0.0124457  0.00212537
>>    0.0449502    0.023914   -0.112811
>>  -0.00164994    0.161391   0.0610653
----------------------- W in_channels: 1, kernel: 1
>>   0.00165231   0.0459011  -0.0694614
>>    -0.151892   -0.197883  -0.0945227
>>     0.176106   -0.139918  -0.0667515
----------------------- W in_channels: 1, kernel: 2
>>    0.0232394   0.0386691   0.0170101
>>      -0.1966   0.0646472   0.0974093
>>   -0.0408345    0.103405  -0.0141843
----------------------- W in_channels: 1, kernel: 3
>>     0.122283  -0.0733714  -0.0930703
>>    0.0647381    0.111698   -0.164386
>>   -0.0956131   0.0315414 -0.00246837
----------------------- result channel: 0
>>   -0.0373998 -0.00652579-0.000292354 -0.00252695   0.0201046
>>     0.015026  -0.0252472  0.00445372 -0.00619115   0.0126502
>>   -0.0313427  -0.0185694 -0.00523668   0.0233806   0.0123302
>>    0.0303936  0.00014171  -0.0397153   0.0206317  -0.0251504
>>   -0.0226089   0.0411003 -0.00449737 -0.00852947  0.00462775
>>  -0.00700756  -0.0101008   0.0107602 -0.00560604  -0.0126359
----------------------- result channel: 1
>>     0.100427   0.0593558  -0.0128195  -0.0262631   -0.030637
>>    0.0369452    0.071602  0.00495992 -0.00277422-0.000506826
>>  -0.00165216  -0.0193467   0.0072494   0.0039469  0.00272913
>>   0.00245166  0.00575878   0.0311494  -0.0073841  -0.0313668
>>  -0.00172585  -0.0550382  -0.0352769  -0.0103869  0.00948465
>>    0.0295881   0.0482458  0.00224401  -0.0280313 -0.00762568
----------------------- result channel: 2
>>   -0.0243839   -0.020852   0.0500951    0.025618  0.00610305
>>  -0.00906303  0.00712102   0.0392061   0.0288411  -0.0217269
>>   -0.0139729   -0.020412   -0.026164   0.0495156  -0.0192653
>>    -0.022764   0.0545916  -0.0196756   0.0110136   0.0106208
>>   -0.0262429  -0.0414057  -0.0141798 -0.00138508  -0.0136511
>>   0.00944899   0.0241735 -0.00302218   0.0344513   0.0239845
----------------------- result channel: 3
>>   0.00876225  -0.0308676  -0.0331045  -0.0149085 -0.00273006
>>    0.0525242  -0.0330806  -0.0341669   0.0142486  -0.0115738
>>    0.0353003   0.0365733  -0.0199258   0.0166172  0.00349931
>>   -0.0248951   0.0334262  -0.0117919  0.00639945    0.012372
>>   -0.0266199  -0.0154169  -0.0331024  -0.0353302  -0.0057847
>>   -0.0106993  -0.0174154    0.010168 -0.00291295   0.0240083
>> ----
raintai@ml-high:~/z$
```
可以看出结果完全一直。通过分析C++代码可以了解TensorFlow中卷积的一些细节。例如：输出结果中，每个卷积核心的最终结果是输入各通道卷积结果的相加，等等。当然更详细的了解，可以自己实现openCV的filter2D函数。
上边代码的git地址：https://code.csdn.net/guoyunfei20/tensorflow_opencv_convolution_compare.git










