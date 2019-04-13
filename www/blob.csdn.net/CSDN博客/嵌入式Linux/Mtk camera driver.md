
# Mtk camera driver - 嵌入式Linux - CSDN博客

2016年02月22日 11:11:44[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：1557


引用：
[http://blog.chinaunix.net/uid-26009923-id-3999723.html](http://blog.chinaunix.net/uid-26009923-id-3999723.html)
## １
```python
kd_imgsensor
```
```python
.h
```
这个文件定义了camera节点的名字
```python
/* CAMERA DRIVER NAME */
```
```python
#
```
```python
define
```
```python
CAMERA_HW_DEVNAME            "kd_camera_hw"
```
里面还有很多ioctl的幻数，摄像头的名字等等，这个头文件算是给摄像头驱动的一个总结了
## ２
然后有两个目录
mediatek/platform/mt8127/hardware/ 这个是平台的hardware层
```python
./mediatek/
```
```python
platform
```
```python
/mt8127/hardware/mtkcam/core/drv/imgsensor/imgsensor_drv.cpp
```
这里属于mtk平台的hardware层，这里面有打开节点的地方，整个平台创建节点和打开节点都引用了一个宏就是CAMERA_HW_DEVNAME
```python
mediatek
```
```python
/hardware/mtkcam/
```
这里又有一个hardware层，mtk的hardware层不只是在一个地方。
## 3
./mediatek/custom/common/kernel/imgsensor/src/kd_sensorlist.c
```python
#
```
```python
define
```
```python
SUPPORT_I2C_BUS_NUM1          0
```
```python
//#define SUPPORT_I2C_BUS_NUM2        2  这里定义了I2C是哪一路的
```

