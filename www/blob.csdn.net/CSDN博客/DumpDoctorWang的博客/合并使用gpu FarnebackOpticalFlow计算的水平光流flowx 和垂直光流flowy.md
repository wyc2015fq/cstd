# 合并使用gpu::FarnebackOpticalFlow计算的水平光流flowx 和垂直光流flowy - DumpDoctorWang的博客 - CSDN博客





2017年11月29日 17:51:26[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：879








## 一、问题来源

近期在学习GPU加速。无意间看到OpenCV库中有用GPU来计算光流的类gpu::FarnebackOpticalFlow，CPU上的光流函数是calcOpticalFlowFarneback。经过测试，用GPU上的光流函数计光流类gpu::FarnebackOpticalFlow计算的速度远远比CPU上的函数calcOpticalFlowFarneback快，但是，遇到了一个问题，gpu::FarnebackOpticalFlow输出的结果是两个GpuMat:flowx和flowy，calcOpticalFlowFarneback函数输出的结果是一个Mat类型的flow,包含水平和垂直方向上的光流。到这里，我就不知道该怎么合并flowx和flowy。

## 二、问题的解决

百度了很久，没有找到资料，国外也去看了，也没有找到。于是打算自己测试，首先测试了flow,flowx和flowy的维数，发现三个得到维数都相同，然后我又查看了这三者的通道数，发现flow的通道是2，flowx和flowy的通道数是1；然后我拿flow的通道1的数据和flowx比对、拿flow的通道二的数据和flowy比较，发现两组数据完全相同，也就是说，将flowx和flowy合并，就可以得到flow！

接下来直接上代码。

```
//头文件
	#include <opencv2/gpu/gpu.hpp>
	using namespace cv;
	
	///用GPU来完成光流计算
	gpu::GpuMat preGrayGpu(preGray);    //当前帧的灰度Mat
	gpu::GpuMat nextGrayGpu(nextGray);  //下一帧的灰度Mat
	gpu::GpuMat opfGpuX;                //水平方向的光流Mat
	gpu::GpuMat opfGpuY;                //垂直方向的光流Mat
	gpu::FarnebackOpticalFlow mOpticalFlow; 
	mOpticalFlow.winSize = 15;          //mOpticalFlow的应该和calcOpticalFlowFarneback对应的参数相同
	mOpticalFlow.numLevels = 3;
	mOpticalFlow.numIters = 3;
	mOpticalFlow(preGrayGpu,nextGrayGpu,opfGpuX,opfGpuY); //计算光流
	Mat flowX,flowY;
	opfGpuX.download(flowX);
	opfGpuY.download(flowY);

	///合并x,y方向的光流
	Mat flow;             //存储合并后的光流
	vector<Mat> srcOpfMat;
	srcOpfMat.push_back(flowX);
	srcOpfMat.push_back(flowY);
	merge(srcOpfMat,flow); //flow就是合并后的光流
```



