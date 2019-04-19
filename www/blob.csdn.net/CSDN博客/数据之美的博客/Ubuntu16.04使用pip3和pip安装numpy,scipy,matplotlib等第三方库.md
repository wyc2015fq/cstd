# Ubuntu16.04使用pip3和pip安装numpy,scipy,matplotlib等第三方库 - 数据之美的博客 - CSDN博客
2017年12月18日 16:31:31[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：2725
**安装Python3第三方库numpy,scipy,matplotlib：**
```
sudo apt install python3-pip
pip3 install numpy
pip3 install scipy
pip3 install matplotlib
```
报错，安装matplotlib库需要安装python3-tk库:
```
sudo apt-get install python3-tk
pip3 install matplotlib
```
报错，安装matplotlib库需要安装nose库:
```
pip3 install nose
```
 安装图像处理库：
```
pip3 install pillow
```
通过 `from PIL import Image` 测试是否安装成功。
**安装Python2第三方库numpy,scipy,matplotlib：**
```
sudo pip install numpy
sudo pip install scipy
sudo pip install matplotlib
sudo pip install nose
```
  安装图像处理库：
```
sudo pip install pillow
```
通过 `from PIL import Image` 测试是否安装成功。
