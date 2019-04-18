# 基于 OpenCv 和 Python 的手指识别及追踪 - 算法与数学之美 - CSDN博客
2018年08月19日 21:46:34[算法与数学之美](https://me.csdn.net/FnqTyr45)阅读数：1598
> 
***本文为 AI 研习社编译的技术博客，原标题 Finger Detection and Tracking using OpenCV and Python，作者为Jason Brownlee***
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkzU1zLK5GQafC05u6xDAuj4UJWXXND0ab2kvKgCiayAp3YuVPqibgBvXKxGPaGvEdcxrqIgmaCDtYicQ/640?wx_fmt=jpeg)
*详细代码参考：https://github.com/amarlearning/opencv*
手指追踪是许多计算机视觉应用的重要特征。在该应用中，使用基于直方图的方法将手与背景帧分离。 使用阈值处理和滤波技术来进行背景消除以获得最佳结果。
我在手指识别时遇到的挑战之一是将手与背景区分开并识别手指的尖端。我将向您展示我用于手指跟踪的技术，我在此项目中使用了该技术。如果想要查看手指识别和跟踪的实际操作，请观看我上传的视频。
在要跟踪用户手部移动的应用程序中，肤色在要跟踪用户手部移动的应用程序中，肤色直方图将非常有用。使用直方图从图像中剔除背景，仅留下包含肤色的图像部分。
检测皮肤的一种更简单的方法是找到特定 RGB 或 HSV 范围内的像素。如果您想了解更多有关此方法的信息，请点击此处（*https://docs.opencv.org/3.4.2/df/d9d/tutorial_py_colorspaces.html*）。
上述方法的问题在于改变光线条件和肤色可能会使皮肤检测的结果很糟糕。另一方面，直方图往往更准确，并且直方图能够考虑到当前的光照条件。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkzU1zLK5GQafC05u6xDAuj40cbSEY7SiaufYuoFP3VXtZSiaDND26D8sbm3OrDcvoGOAlpu2ZOWtXSw/640?wx_fmt=jpeg)
在框架中绘制一个绿色的长方形并且用户将他们的手放置在长方形中。应用程序提取用户手掌的肤色，然后绘制成一个直方图。
绘制长方形的过程遵循以下函数：
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkzU1zLK5GQafC05u6xDAuj4kRBOMfzs0KAL3DC7h8pgbzjzZTQ5asqxbbezibl4XdCq7ugf5MRUteQ/640?wx_fmt=jpeg)
这个过程非常简单——我创建了四行代码来表示每个长方形的坐标。
> 
**hand_rect_one_x**
**hand_rect_one_y**
**hand_rect_two_x**
**hand_rect_two_y**
这四行代码不断迭代以在框架内使用：cv2.rectangle 绘制生成长方形；这里的total_rectangle表示行列的长度9。
现在用户理解了在哪里放置他们的手掌，接下来成功的关键步骤是从这些长方形中提取像素，然后基于像素生成 HSV 直方图
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkzU1zLK5GQafC05u6xDAuj4E6oelJ2YFyo8XqxZ85ev6q2FeOiajErnic7AQl7qyiabbJrhaE9xCQbfw/640?wx_fmt=jpeg)
函数把输入框架转换成 HSV 直方图。使用 Numpy 库生成一张图像。图像大小为[90*10]，颜色通道数为 3。我们将其命名为 ROI （Region of Interest）. 它从绿色的长方形中提取了 900 个像素值，然后将它们放入 ROI 矩阵中。cv2.calcHist 基于 ROI 矩阵给肤色创建了一个直方图并且 cv2.normalize 使用 norm 类型对矩阵进行归一化 cv2.NORM_MINMAX 。现在我们可以用直方图来检测框架中的皮肤区域。
现在用户理解了将他们的手掌放在哪里，下一步就是从这些长方形中提取像素然后用它们生成HSV直方图。
现在基于肤色直方图我们可以找到包含皮肤的框架区域，OpenCV 提供了一个简便的方法，cv2.calvBackProject，该方法使用直方图来分离图像中的特征。我通过这个函数来把肤色直方图应用到框架中。如果你想获取更多关于 back project 的信息，可以通过链接1*（https://docs.opencv.org/master/dc/df6/tutorial_py_histogram_backprojection.html）*和链接2*（https://docs.opencv.org/2.4/doc/tutorials/imgproc/histograms/back_projection/back_projection.html）*来获取。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkzU1zLK5GQafC05u6xDAuj44DFxJNqdpMO9kibCcnc1WAF1kgMgwIgTpwsXXCoNC3EEicyhFHTAq13w/640?wx_fmt=png)
......
翻译 | 余杭  Lamaric
来源 | 雷锋网
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabky7x6u1VxMVMia4MLibNzC2nrumY3zDflTsCeoM04M1BrkvPny8tsw6hYkIicUr42iarLmadL2x6JwV6A/640?wx_fmt=png)
算法数学之美微信公众号欢迎赐稿
稿件涉及数学、物理、算法、计算机、编程等相关领域，经采用我们将奉上稿酬。
投稿邮箱：math_alg@163.com
