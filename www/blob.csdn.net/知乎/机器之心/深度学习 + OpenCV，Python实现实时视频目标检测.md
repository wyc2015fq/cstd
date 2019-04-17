# 深度学习 + OpenCV，Python实现实时视频目标检测 - 知乎
# 



选自PyimageSearch

机器之心编译

参与：路雪、李泽南




> 使用 OpenCV 和 Python 对实时视频流进行深度学习目标检测是非常简单的，我们只需要组合一些合适的代码，接入实时视频，随后加入原有的目标检测功能。




在本文中我们将学习如何扩展原有的目标检测项目，使用深度学习和 OpenCV 将应用范围扩展到实时视频流和视频文件中。这个任务会通过 VideoStream 类来完成。
- 深度学习目标检测教程：[http://www.pyimagesearch.com/2017/09/11/object-detection-with-deep-learning-and-opencv/](https://link.zhihu.com/?target=http%3A//www.pyimagesearch.com/2017/09/11/object-detection-with-deep-learning-and-opencv/)

- VideoStream 类教程：[http://www.pyimagesearch.com/2016/01/04/unifying-picamera-and-cv2-videocapture-into-a-single-class-with-opencv/](https://link.zhihu.com/?target=http%3A//www.pyimagesearch.com/2016/01/04/unifying-picamera-and-cv2-videocapture-into-a-single-class-with-opencv/)

现在，我们将开始把深度学习+目标检测的代码应用于视频流中，同时测量 FPS 处理速度。




## 使用深度学习和 OpenCV 进行视频目标检测

为了构建基于 OpenCV 深度学习的实时目标检测器，我们需要有效地接入摄像头/视频流，并将目标检测应用到每一帧里。

首先，我们打开一个新文件，将其命名为 real_time_object_detection.py，随后加入以下代码：
![](https://pic1.zhimg.com/v2-409d290eaa6941c68116384e75372cac_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='150'></svg>)
我们从第 2-8 行开始导入封包。在此之前，你需要 imutils 和 OpenCV 3.3。在系统设置上，你只需要以默认设置安装 OpenCV 即可（同时确保你遵循了所有 Python 虚拟环境命令）。

Note：请确保自己下载和安装的是 OpenCV 3.3（或更新版本）和 OpenCV-contrib 版本（适用于 OpenCV 3.3），以保证其中包含有深度神经网络模块。

下面，我们将解析这些命令行参数：
![](https://pic2.zhimg.com/v2-b2363c698cacfef765e25bba112bbfed_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='168'></svg>)
与此前的目标检测项目相比，我们不需要图像参数，因为在这里我们处理的是视频流和视频——除了以下参数保持不变：
- --prototxt：Caffe prototxt 文件路径。
- --model：预训练模型的路径。
- --confidence：过滤弱检测的最小概率阈值，默认值为 20%。

随后，我们初始化类列表和颜色集：
![](https://pic3.zhimg.com/v2-f572a4c68a153ea7e43a50be2e0c31ce_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='132'></svg>)
在第 22-26 行，我们初始化 CLASS 标签，和相应的随机 COLORS。有关这些类的详细信息（以及网络的训练方式），请参考：[http://www.pyimagesearch.com/2017/09/11/object-detection-with-deep-learning-and-opencv/](https://link.zhihu.com/?target=http%3A//www.pyimagesearch.com/2017/09/11/object-detection-with-deep-learning-and-opencv/)

现在，我们加载自己的模型，并设置自己的视频流：
![](https://pic4.zhimg.com/v2-07fa3cac96f8e0e1027c6f9c8988e133_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='184'></svg>)
我们加载自己的序列化模型，提供对自己的 prototxt 和模型文件的引用（第 30 行），可以看到在 OpenCV 3.3 中，这非常简单。

下一步，我们初始化视频流（来源可以是视频文件或摄像头）。首先，我们启动 VideoStream（第 35 行），随后等待相机启动（第 36 行），最后开始每秒帧数计算（第 37 行）。VideoStream 和 FPS 类是 imutils 包的一部分。

现在，让我们遍历每一帧（如果你对速度要求很高，也可以跳过一些帧）：
![](https://pic4.zhimg.com/v2-1b857fb25bbdb082584fc33fdc997f6f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='256'></svg>)
首先，我们从视频流中读取一帧（第 43 行），随后调整它的大小（第 44 行）。由于我们随后会需要宽度和高度，所以我们在第 47 行上进行抓取。随后将 frame 转换为一个有 dnn 模块的 blob（第 48 行）。

现在，我们设置 blob 为神经网络的输入（第 52 行），通过 net 传递输入（第 53 行），这给我们提供了 detections。

这时，我们已经在输入帧中检测到了目标，现在是时候看看置信度的值，以判断我们能否在目标周围绘制边界框和标签了：
![](https://pic4.zhimg.com/v2-bde89fd9bdecd77abf34285c94fdf56f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='392'></svg>)
我们首先在 detections 内循环，记住一个图像中可以检测到多个目标。我们还需要检查每次检测的置信度（即概率）。如果置信度足够高（高于阈值），那么我们将在终端展示预测，并以文本和彩色边界框的形式对图像作出预测。让我们逐行来看一下：

在 detections 内循环，首先我们提取 confidence 值（第 59 行）。

如果 confidence 高于最低阈值（第 63 行），那么我们提取类标签索引（第 67 行），并计算检测到的目标的坐标（第 68 行）。

然后，我们提取边界框的 (x, y) 坐标（第 69 行），之后将用于绘制矩形和文本。

我们构建一个文本 label，包含 CLASS 名称和 confidence（第 72、73 行）。

我们还要使用类颜色和之前提取的 (x, y) 坐标在物体周围绘制彩色矩形（第 74、75 行）。

通常，我们希望标签出现在矩形上方，但是如果没有空间，我们将在矩形顶部稍下的位置展示标签（第 76 行）。

最后，我们使用刚才计算出的 y 值将彩色文本置于帧上（第 77、78 行）。

帧捕捉循环剩余的步骤还包括：（1）展示帧；（2）检查 quit 键；（3）更新 fps 计数器：
![](https://pic1.zhimg.com/v2-c0aa6f64eda4143440864b5ce13c0564_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='186'></svg>)
上述代码块简单明了，首先我们展示帧（第 81 行），然后找到特定按键（第 82 行），同时检查「q」键（代表「quit」）是否按下。如果已经按下，则我们退出帧捕捉循环（第 85、86 行）。最后更新 fps 计数器（第 89 行）。

如果我们退出了循环（「q」键或视频流结束），我们还要处理这些：
![](https://pic2.zhimg.com/v2-6140b4235c869ec0d086e43d7cde0909_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='154'></svg>)
当我们跳出（exit）循环，fps 计数器 停止（第 92 行)，每秒帧数的信息向终端输出（第 93、94 行)。

我们关闭窗口（第 97 行），然后停止视频流（第 98 行）。

如果你到了这一步，那就可以做好准备用自己的网络摄像头试试看它是如何工作的了。我们来看下一部分。




## 实时深度学习目标检测的结果

为了实时深度学习目标检测器正常运行，确保你使用本指南「Downloads」部分中的示例代码和预训练的卷积神经网络。（请打开原文链接，进入「Downloads」部分，输入自己的邮箱地址，获取所需代码和其他资料。）

打开终端，执行下列命令：
![](https://pic1.zhimg.com/v2-a343c9bf81d1950d01fca76b27d16584_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='136'></svg>)
如果 OpenCV 能够访问你的摄像头，你可以看到带有检测到的目标的输出视频帧。我对样本视频使用了深度学习目标检测，结果如下：
![](https://pic4.zhimg.com/v2-64cecb4992f975a0220d6ac685cdc42b_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='200' height='122'></svg>)



图 1：使用深度学习和 OpenCV + Python 进行实时目标检测的短片。

注意深度学习目标检测器不仅能够检测到人，还能检测到人坐着的沙发和旁边的椅子——所有都是实时检测到的！




## 总结

今天的博客中，我们学习了如何使用深度学习 + OpenCV + 视频流来执行实时目标检测。我们通过下列两个教程完成了这一目标：

1. 使用深度学习和 OpenCV 进行目标检测（[http://www.pyimagesearch.com/2017/09/11/object-detection-with-deep-learning-and-opencv/](https://link.zhihu.com/?target=http%3A//www.pyimagesearch.com/2017/09/11/object-detection-with-deep-learning-and-opencv/)）

2. 在 OpenCV 上进行高效、线程化的视频流（[http://www.pyimagesearch.com/2016/01/04/unifying-picamera-and-cv2-videocapture-into-a-single-class-with-opencv/](https://link.zhihu.com/?target=http%3A//www.pyimagesearch.com/2016/01/04/unifying-picamera-and-cv2-videocapture-into-a-single-class-with-opencv/)）

最终结果是基于深度学习的目标检测器可以处理 6-8 个 FPS 的视频（当然，这也取决于你的系统速度）。

你还可以通过以下途径进一步提升速度：

1. 跳过帧。

2. 使用 MobileNet 的不同变体（速度更快，但是准确率下降）。

3. 使用 SqueezeNet 的量子化变体（我还未对此进行测试，但是我想应该会更快，因为它的网络足迹更小）。

原文链接：[http://www.pyimagesearch.com/2017/09/18/real-time-object-detection-with-deep-learning-and-opencv/](https://link.zhihu.com/?target=http%3A//www.pyimagesearch.com/2017/09/18/real-time-object-detection-with-deep-learning-and-opencv/)




本文为机器之心编译，转载请联系本公众号获得授权。


