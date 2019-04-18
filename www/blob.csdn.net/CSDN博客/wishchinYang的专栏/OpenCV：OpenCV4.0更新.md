# OpenCV：OpenCV4.0更新 - wishchinYang的专栏 - CSDN博客
2019年03月05日 10:59:12[wishchin](https://me.csdn.net/wishchin)阅读数：145
个人分类：[场景处理/RgbD累积																[OpenCV](https://blog.csdn.net/wishchin/article/category/5778933)](https://blog.csdn.net/wishchin/article/category/1524721)
OpenCV4.0已经发布一段时间，更新在官方提示中，重要的是添加了ONNX接口和KinectFusion算法；
Release highlights:
- OpenCV is now C++11 library and requires C++11-compliant compiler. Minimum required CMake version has been raised to 3.5.1.
- A lot of C API from OpenCV 1.x has been removed.
- Persistence (storing and loading structured data to/from XML, YAML or JSON) in the *core* module has been completely reimplemented in C++ and lost the C API as well.
- New module G-API has been added, it acts as an engine for very efficient graph-based image procesing pipelines.
- *dnn* module was updated with [Deep Learning Deployment Toolkit](https://github.com/opencv/dldt) from the OpenVINO™ toolkit R4. See [the guide](https://github.com/opencv/opencv/wiki/Intel' rel=) how to build and use OpenCV with DLDT support.
- *dnn* module now includes experimental Vulkan backend and supports networks in ONNX format.
- The popular Kinect Fusion algorithm has been implemented and optimized for CPU and GPU (OpenCL)
- QR code detector and decoder have been added to the *objdetect* module
- Very efficient and yet high-quality DIS dense optical flow algorithm has been moved from opencv_contrib to the *video* module.
- More details can be found in previous announces: [4.0-alpha](https://opencv.org/opencv-4-0-0-alpha.html), [4.0-beta](https://opencv.org/opencv-4-0-0-beta.html), [4.0-rc](https://opencv.org/opencv-4-0-0-rc.html) and in the [changelog](https://github.com/opencv/opencv/wiki/ChangeLog#version400)
Branch 3.4 will be switched to maintanence mode: only bugfixes and light features will be accepted. **BTW, release 3.4.4 is ready too!**
重要更新：
        OpenCV 4.0 现在是一个 C++11 库，要求 C++11 兼容的编译器。所需的 CMake 至少是 3.5.1 版本。
        移除 OpenCV 1.x 中的大量 C API。core 模块中的 Persistence（用于存储和加载 XML、YAML 或 JSON 格式的结构化数据）可以完全使用 C++ 来重新实现，因此这里的 C API 也被移除。
        添加了新模块 G-API，它可作为基于图的高效图像处理流程。
        dnn 模块包括实验用 Vulkan 后端，且支持 ONNX 格式的网络。
        实现了流行的 Kinect Fusion 算法，且为 CPU 和 GPU (OpenCL) 进行优化。
        objdetect 模块中添加了二维码检测器和解码器。将高效、高质量的 DIS dense optical flow 算法从 opencv_contrib 移到 video 模块。
        此外，OpenCV 4.0 支持 Mask-RCNN 模型，性能也有所提升，图像处理操作可实现 15%-30% 的速度提升。
