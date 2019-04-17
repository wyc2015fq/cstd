# Python查看已安装库的版本 - DumpDoctorWang的博客 - CSDN博客





2019年04月11日 21:10:20[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：16











### 目录
- [一、Linux系统](#Linux_3)
- [1、Python2](#1Python2_4)
- [2、Python3](#2Python3_15)
- [二、Windows系统](#Windows_21)
- [1、Python2](#1Python2_22)
- [2、Python3](#2Python3_32)




以查看numpy的版本为例。

# 一、Linux系统

## 1、Python2

```bash
pip2 list | grep numpy
```

运行上面的命令输出：

```bash
numpy (1.15.4)
```

`1.15.4`就是库的版本号。`pip list`会列出所有安装的库；`|`是重定向输出符号，把`pip list`的输出重定为`grep`命令的输入；`grep`命令是从输入中找到包含`numpy`那一行并打印出来，如果没有找到，则没有输出。

## 2、Python3

```bash
pip3 list | grep numpy
```

# 二、Windows系统

## 1、Python2

```bash
pip2 list | findstr numpy
```

运行上面的命令输出：

```bash
numpy      1.16.2
```

`1.16.2`就是版本号。`findstr`类似于linux系统的`grep`命令，如果没有找到，则没有输出。

## 2、Python3

```bash
pip3 list | findstr numpy
```



