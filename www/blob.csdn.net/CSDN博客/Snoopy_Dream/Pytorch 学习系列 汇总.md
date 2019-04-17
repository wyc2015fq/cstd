# Pytorch 学习系列 汇总 - Snoopy_Dream - CSDN博客





置顶2018年11月15日 19:16:56[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：120
所属专栏：[faster-rcnn.pytorch](https://blog.csdn.net/column/details/25674.html)[pytorch yolov3](https://blog.csdn.net/column/details/30519.html)









**目录**

[首先是pytorch基础系列：](#%E9%A6%96%E5%85%88%E6%98%AFpytorch%E5%9F%BA%E7%A1%80%E7%B3%BB%E5%88%97%EF%BC%9A)

[1. pytorch常用的一些基础属性和函数，以及与所熟知的numpy的不同，矩阵思维。](#1.%20pytorch%E5%B8%B8%E7%94%A8%E7%9A%84%E4%B8%80%E4%BA%9B%E5%B1%9E%E6%80%A7%E5%92%8C%E5%87%BD%E6%95%B0%EF%BC%8C%E4%BB%A5%E5%8F%8A%E4%B8%8E%E6%89%80%E7%86%9F%E7%9F%A5%E7%9A%84numpy%E7%9A%84%E4%B8%8D%E5%90%8C%E3%80%82)

[2. pytorch写cnnn训练的一系列流程。](#2.%20pytorch%E5%86%99cnn%E7%9A%84%E4%B8%80%E7%B3%BB%E5%88%97%E6%B5%81%E7%A8%8B%E3%80%82)

[3. pytorch 搭建模型 经典](#3.%20pytorch%20%E6%90%AD%E5%BB%BA%E6%A8%A1%E5%9E%8B%20%E7%BB%8F%E5%85%B8) ResNet+yolo

[5. nn.moduleList 和Sequential由来、用法和实例 —— 写网络模型](https://blog.csdn.net/e01528/article/details/84397174)【高阶】

[4. 深入 学习神经网络中 模块的属性方法。](#4.%20%E6%B7%B1%E5%85%A5%20%E5%AD%A6%E4%B9%A0%E7%A5%9E%E7%BB%8F%E7%BD%91%E7%BB%9C%E4%B8%AD%20%E6%A8%A1%E5%9D%97%E7%9A%84%E5%B1%9E%E6%80%A7%E6%96%B9%E6%B3%95%E3%80%82)backward() autograd..

**6. 矩阵思维 repeat view *等函数的实际应用 yolo**

[7. PyTorch简明笔记[1]-Tensor的初始化和基本操作](https://www.jianshu.com/p/7dbfc7076e5a)

        踩过的坑

                    1. [什么情况下应该设置 cudnn.benchmark = True？](https://blog.csdn.net/e01528/article/details/85275616)

[2. PyTorch 更改训练好的模型 【继续训练](https://blog.csdn.net/e01528/article/details/85273172)】

[3. RuntimeError: Error(s) in loading state_dict for ***: Missing key(s) in state_dict:](https://blog.csdn.net/e01528/article/details/84981058)

                    4. [trian和val结果相差很大。](https://blog.csdn.net/e01528/article/details/84984478)

                    5. [pytorch 多gpu训练](https://blog.csdn.net/e01528/article/details/84862743)

       一些utils

                    1. [label smoothing pytorch版本](https://blog.csdn.net/e01528/article/details/85019274)

[2. 如何正确设置验证集 如何设置一个好的初始学习率](https://blog.csdn.net/e01528/article/details/85019237)

                     3. [多模型融合（闲人勿入）](https://blog.csdn.net/e01528/article/details/84593385)

                     4. [python numpy 三行代码打乱训练数据](https://blog.csdn.net/e01528/article/details/84798110)

[其次是实例faster rcnn 源码学习：](#%E5%85%B6%E6%AC%A1%E6%98%AF%E5%AE%9E%E4%BE%8Bfaster%20rcnn%20%E6%BA%90%E7%A0%81%E5%AD%A6%E4%B9%A0%EF%BC%9A)

[1. 首先需要必备的用到 的编程基础](#1.%20%E9%A6%96%E5%85%88%E9%9C%80%E8%A6%81%E5%BF%85%E5%A4%87%E7%9A%84%E7%94%A8%E5%88%B0%20%E7%9A%84%E7%BC%96%E7%A8%8B%E5%9F%BA%E7%A1%80)

[2. 从输入输出 的维度角度 理解整个流程（手写流程图）](#2.%20%E4%BB%8E%E8%BE%93%E5%85%A5%E8%BE%93%E5%87%BA%C2%A0%E7%9A%84%E7%BB%B4%E5%BA%A6%E8%A7%92%E5%BA%A6%20%E7%90%86%E8%A7%A3%E6%95%B4%E4%B8%AA%E6%B5%81%E7%A8%8B%EF%BC%88%E6%89%8B%E5%86%99%E6%B5%81%E7%A8%8B%E5%9B%BE%EF%BC%89)

[3. 深入了解损失函数的过程](#3.%20%E6%B7%B1%E5%85%A5%E4%BA%86%E8%A7%A3%E6%8D%9F%E5%A4%B1%E5%87%BD%E6%95%B0%E7%9A%84%E8%BF%87%E7%A8%8B)

[4. 关键源码学习](#4.%20%E5%85%B3%E9%94%AE%E6%BA%90%E7%A0%81%E5%AD%A6%E4%B9%A0)[（手写anchor_target_layer流程图）](#2.%20%E4%BB%8E%E8%BE%93%E5%85%A5%E8%BE%93%E5%87%BA%C2%A0%E7%9A%84%E7%BB%B4%E5%BA%A6%E8%A7%92%E5%BA%A6%20%E7%90%86%E8%A7%A3%E6%95%B4%E4%B8%AA%E6%B5%81%E7%A8%8B%EF%BC%88%E6%89%8B%E5%86%99%E6%B5%81%E7%A8%8B%E5%9B%BE%EF%BC%89)

[5. faster rcnn工程 文件函数结构 学习](#5.%20faster%20rcnn%E5%B7%A5%E7%A8%8B%20%E6%96%87%E4%BB%B6%E5%87%BD%E6%95%B0%E7%BB%93%E6%9E%84%20%E5%AD%A6%E4%B9%A0) init forward

之前写的内容有些杂乱，特此进行整理。

## 首先是pytorch基础系列：



### 1. pytorch常用的一些基础属性和函数，以及与所熟知的numpy的不同，矩阵思维。

[numpy：从多重for循环思维转换到矩阵思维](https://blog.csdn.net/e01528/article/details/83757602) 【广播】

[pyTorch基础系列（一）—— 基本语法](https://blog.csdn.net/e01528/article/details/83825515)

[RPN 预备编程知识（一） : Numpy VS Pytorch （ Anchor_target_layer ）](https://blog.csdn.net/e01528/article/details/83685031)【用心写的】

[numpy：从多重for循环思维转换到矩阵思维](https://blog.csdn.net/e01528/article/details/83757602)

### 2. pytorch写cnn的一系列流程。

[PyTorch基础系列（二）——cnn简单训练6个流程](https://blog.csdn.net/e01528/article/details/83894811)【用心写的】

[pytorch minst分类 莫凡](https://blog.csdn.net/e01528/article/details/84036369)

[pytorch 进度条+分类【闲人误入Alibaba Cloud German AI Challenge 2018 AI For Earth Observation主程序】](https://blog.csdn.net/e01528/article/details/86062452)

### 3. pytorch 搭建模型 经典

[pytorch 50行代码搭建ResNet-](https://blog.csdn.net/e01528/article/details/83339241)50

[yolo v3 网络结构](https://blog.csdn.net/e01528/article/details/84566868)

[pytorch yolov3 构建class Darknet 脑海中过一遍](https://blog.csdn.net/e01528/article/details/84570849)

### 4. 深入 学习神经网络中 模块的属性方法。

[PyTorch基础系列（三）——深入理解autograd：Variable属性方法【最新已经和tensor合并为一类】](https://blog.csdn.net/e01528/article/details/84069243)【用心写的】

[pytorch 基础系列（四）——nn.module](https://blog.csdn.net/e01528/article/details/84075090)

## 6. 矩阵思维 repeat view *等函数的实际应用

[pytorch yolov3 yolo层的构建 矩阵运算思维启蒙 损失函数要求公示里面的乘以相应的anchor](https://blog.csdn.net/e01528/article/details/84584904)

## 其次是实例faster rcnn 源码学习：



### 1. 首先需要必备的用到 的编程基础

[reshape和transpose的区别](https://blog.csdn.net/e01528/article/details/83786013)**【用心写的】**

[numpy：从多重for循环思维转换到矩阵思维](https://blog.csdn.net/e01528/article/details/83757602)

[RPN 预备编程知识（一） : Numpy VS Pytorch （ Anchor_target_layer ）](https://blog.csdn.net/e01528/article/details/83685031)

[RPN 预备编程知识（二） : 函数篇章 bbox_transform.py](https://blog.csdn.net/e01528/article/details/83662611)

### 2. 从输入输出 的维度角度 理解整个流程（手写流程图）

[pytoch faster rcnn复现系列（一） RPN层输入输出维度理解](https://blog.csdn.net/e01528/article/details/83618169)**【用心写的】**

### 3. 深入了解损失函数的过程

[faster rcnn中 损失函数（一）——softmax，softmax loss和cross entropy的讲解](https://blog.csdn.net/e01528/article/details/83795970) 【用心写的】

[faster rcnn中损失函数（二）—— Smoooh L1 Loss的讲解](https://blog.csdn.net/e01528/article/details/83856670) 【用心写的】

[faster rcnn中损失函数（三）——理解faster-rcnn中计算rpn_loss_cls&rpn_loss_box的过程](https://blog.csdn.net/e01528/article/details/84026513) 【用心写的】

### 4. 关键源码学习

[ROIPooling的意义？全连接层输入需要固定尺度？全连接层的实现？为什么需要两个全连接层？](https://blog.csdn.net/e01528/article/details/83616906)【附送 用心写的】

[faster rcnn pytorch 复现系列（二）：generate_anchors源码解析](https://blog.csdn.net/e01528/article/details/83576796)【用心写的】

[faster rcnn pytorch 复现系列（三）： Anchor_target_layer.py](https://blog.csdn.net/e01528/article/details/83650284)**【**手写流程图 **用心写的】**

### 5. faster rcnn工程 文件函数结构 学习

[继承nn.Module后的 init与forward函数【trian_val、vgg16、faster_rcnn、rpn】.py 学习 文件结构 大工程安排](https://blog.csdn.net/e01528/article/details/84025728)













