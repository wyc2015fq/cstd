# 树莓派启用i2c设备 - xqhrs232的专栏 - CSDN博客
2017年03月03日 11:50:39[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1979
原文地址::[http://www.cnblogs.com/hangxin1940/archive/2013/04/02/2997077.html](http://www.cnblogs.com/hangxin1940/archive/2013/04/02/2997077.html)
## 修改配置文件
树莓派已经集成了i2c驱动,只需要启动就行. 修改配置文件
```
sudo nano /etc/modprobe.d/raspi-blacklist.conf
```
![配置](http://images.cnblogs.com/cnblogs_com/hangxin1940/466697/o_rasp_i2c1.png)将第三行注释掉, CTRL + X保存
接下来启动i2c模块
```
sudo nano /etc/modules
```
![配置](http://images.cnblogs.com/cnblogs_com/hangxin1940/466697/o_rasp_i2c2.png)增加一行 `i2c-dev`
## 安装工具
```
sudo apt-get install i2c-tools python-smbus
```
将`pi`用户添加至`i2c`组
```
sudo adduser pi i2c
```
重启
```
sudo shutdown -r now
```
## 测试i2c
连接i2c设备,并运行如下命令 树莓派A型:
```
sudo i2cdetect -y 0
```
树莓派B型:
```
sudo i2cdetect -y 1
```
将会看到i2c地址
![配置](http://images.cnblogs.com/cnblogs_com/hangxin1940/466697/o_rasp_i2c3.png)
*我的i2c接上了 GY-85 9轴模块, 共3个传感器 ITG3205+ADXL345+HMC5883L*
