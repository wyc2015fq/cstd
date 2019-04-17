# （论文+代码）R-FCN：基于区域的全卷积神经网络进行物体检测 - 战斗蜗牛的专栏 - CSDN博客





2016年08月09日 10:02:05[vbskj](https://me.csdn.net/vbskj)阅读数：3548









何恺明课题组目标检测最近工作《R-FCN: Object Detection via Region-based Fully Convolutional Networks》：


We present region-based, fully convolutional networks for accurate and efficient object detection. In contrast to previous region-based detectors such as Fast/Faster R-CNN that apply a costly per-region subnetwork hundreds of times, our region-based detector
 is fully convolutional with almost all computation shared on the entire image. To achieve this goal, we propose position-sensitive score maps to address a dilemma between translation-invariance in image classification and translation-variance in object detection.
 Our method can thus naturally adopt fully convolutional image classifier backbones, such as the latest Residual Networks (ResNets), for object detection. We show competitive results on the PASCAL VOC datasets (e.g., 83.6% mAP on the 2007 set) with the 101-layer
 ResNet. Meanwhile, our result is achieved at a test-time speed of 170ms per image, 2.5-20x faster than the Faster R-CNN counterpart. 





论文链接：


https://arxiv.org/abs/1605.06409





代码链接：


https://github.com/daijifeng001/r-fcn





原文链接：


http://weibo.com/5501429448/E2rppq970?from=page_1005055501429448_profile&wvr=6&mod=weibotime&type=comment#_rnd1470568573930



