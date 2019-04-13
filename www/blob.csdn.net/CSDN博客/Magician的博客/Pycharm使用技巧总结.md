
# Pycharm使用技巧总结 - Magician的博客 - CSDN博客


2018年11月25日 15:05:36[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：121



### 文章目录
[1、PyCharm如何自动生成函数注释](#1PyCharm_2)
[2、pycharm运行程序时在Python console窗口中运行](#2pycharmPython_console_13)
[3、Pycharm在创建py文件时,如何自动添加文件头注释](#3Pycharmpy_19)
[4、Pycharm配置远程调试](#4Pycharm_36)

## 1、PyCharm如何自动生成函数注释
一般在函数def()行下敲三次回车，然后手动添加说明
`def xxx(self, x1, x2, x3):
        """           
        :param x1: 
        :param x2: 
        :param x3: 
        :return: 
        """`
## 2、pycharm运行程序时在Python console窗口中运行
问题：在pycharm中点击run运行程序，发现没有打开run窗口，而是打开的Python console窗口。
解决方法：打开菜单栏run->edit configurations，把下图中的复选框取消就可以了
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018112515004611.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1,size_16,color_FFFFFF,t_70)
## 3、Pycharm在创建py文件时,如何自动添加文件头注释
打开PyCharm程序，根据菜单栏中按照如下进入设置：
`File -> settings -> Editor -> File and Code Templates -> Python Script`然后将一下内容添加进文本框即可：
`#!/usr/bin/env python 
# -*- coding: utf-8 -*- 
# @Time : ${DATE} ${TIME} 
# @Author : Aries 
# @Site : ${SITE} 
# @File : ${NAME}.py 
# @Software: ${PRODUCT_NAME}`![在这里插入图片描述](https://img-blog.csdnimg.cn/20181125150307368.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1,size_16,color_FFFFFF,t_70)
## 4、Pycharm配置远程调试
懒得写了，详情参考：[https://blog.csdn.net/five3/article/details/78615589](https://blog.csdn.net/five3/article/details/78615589)

