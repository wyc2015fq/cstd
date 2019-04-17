# OpenCV 光流算法加速---使用GPU来计算光流 - DumpDoctorWang的博客 - CSDN博客





2018年09月02日 19:35:06[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：1009








# 一、依赖项
- OpenCV 2.4.13.x + CUDA 8.0
- OpenCV 3.2.0及以上 + CUDA 8.0
- OpenCV 3.4.x + CUDA 9.1

OpenCV编译时，需要添加CUDA 支持。安装CUDA 以及OpenCV，可参考我的另外两篇博客
- [Ubuntu 安装CUDA与OpenCV](https://blog.csdn.net/DumpDoctorWang/article/details/82317235)
- [Ubuntu 编译OpenCV](https://blog.csdn.net/DumpDoctorWang/article/details/82259357)

# 二、代码

计算稠密光流时，一般使用”calcOpticalFlowFarneback“这个函数来计算，比如

```
Mat pre = imread(...);
Mat next = imread(...);

Mat preGray, nextGray;
cvtColor(pre, preGray, CV_BGR2GRAY);
cvtColor(next, nextGray, CV_BGR2GRAY);

Mat opf;//光流
calcOpticalFlowFarneback(preGray, nextGray, opf, 0.5, 3, 15, 3, 5, 1.1, 0);
```

那么使用GPU来加速时，代码如下

OpenCV 3.2.0及以上版本

```
Mat pre = imread(...);
Mat next = imread(...);

Mat preGray, nextGray;
cvtColor(pre, preGray, CV_BGR2GRAY);
cvtColor(next, nextGray, CV_BGR2GRAY);

cuda::GpuMat g_pg(preGray);     //当前帧的灰度GpuMat
cuda::GpuMat g_ng(nextGray);    //下一帧的灰度GpuMat
cuda::GpuMat opfGpu;            //光流GpuMat
auto opf_tool = cv::cuda::FarnebackOpticalFlow::create(3,0.5,false,15,3,5,1.1,0);
Mat opf; // 光流
opf_tool->calc(g_pg,g_ng,opfGpu);
opfGpu.download(opf);
```

OpenCV 2.4.13.* 版本，参见另外一篇博客

[合并使用gpu::FarnebackOpticalFlow计算的水平光流flowx 和垂直光流flowy](https://blog.csdn.net/DumpDoctorWang/article/details/78668154)



