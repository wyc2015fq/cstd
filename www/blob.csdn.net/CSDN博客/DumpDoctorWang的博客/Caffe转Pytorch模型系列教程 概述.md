# Caffe转Pytorch模型系列教程 概述 - DumpDoctorWang的博客 - CSDN博客





置顶2019年03月22日 17:13:41[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：63
个人分类：[深度学习](https://blog.csdn.net/DumpDoctorWang/article/category/8305497)









尝试了github上提供得诸多Caffe转Pytorch模型的工具之后，我要使用的模型始终不能成功地转换为Pytorch模型。只得手动地把Caffe模型转为Pytorch模型。



### 目录
- [零、知识储备以及步骤说明](#_3)
- [1、知识储备](#1_4)
- [2、步骤说明](#2_8)
- [一、把Caffe的模型转换为Pytorch模型](#CaffePytorch_13)
- [1、可视化Caffe的网络模型](#1Caffe_14)
- [2、从.caffemodel提取参数](#2caffemodel_16)
- [3、调试转换后的Pytorch模型](#3Pytorch_18)




# 零、知识储备以及步骤说明

## 1、知识储备
- 知道如何搭建Pytorch网络。
- 知道如何阅读Caffe的网络。

## 2、步骤说明
- 首先把Caffe的网络模型转换为Pytorch模型。
- 提取Caffe的权重，以便在Pytorch模型中使用。
- 调试模型，一般这步的时间最长。

# 一、把Caffe的模型转换为Pytorch模型

## 1、可视化Caffe的网络模型

参见：[把Caffe的模型转换为Pytorch模型](https://blog.csdn.net/DumpDoctorWang/article/details/88718079)。

## 2、从.caffemodel提取参数

参见：[从.caffemodel/.caffemodel.h5提取Caffe模型的参数](https://blog.csdn.net/DumpDoctorWang/article/details/88732919)。

## 3、调试转换后的Pytorch模型

参见：[调试从Caffe模型转换过来的Pytorch模型](https://blog.csdn.net/DumpDoctorWang/article/details/88742149)。

本系列博客用到的关键资源地址代码：[https://github.com/Mannix1994/SfSNet-Pytorch](https://github.com/Mannix1994/SfSNet-Pytorch)

如果大家觉得这个系列的教程还阔以，麻烦大家在github上给我star！！！



