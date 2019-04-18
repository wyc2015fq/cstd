# Ubuntu17.10 安装 TensorFlow 1.4的GPU版本，包含CUDA,cuDNN - YZXnuaa的博客 - CSDN博客
2018年01月08日 13:39:52[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：756
1.查看当前配置
ubuntu@ubuntu-Z87P-D3:~/下载$ uname -m && cat /etc/*release
![](https://img-blog.csdn.net/20180108140153737?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvWVpYbnVhYQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2.下载
【CUDA官方下载】[https://developer.nvidia.com/cuda-downloads](https://developer.nvidia.com/cuda-downloads)
[大多数人会用到的链接：](https://developer.nvidia.com/cuda-downloads)
[https://developer.nvidia.com/cuda-downloads?target_os=Linux&target_arch=x86_64&target_distro=Ubuntu&target_version=1704&target_type=deblocal](https://developer.nvidia.com/cuda-downloads)
![](https://img-blog.csdn.net/20180108140226586?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvWVpYbnVhYQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
dpkg是执行本地的安装文件，所以要先进入当前路径
dpkg命令：
-i：安装软件包；
-r：删除软件包；
-P：删除软件包的同时删除其配置文件；
-L：显示于软件包关联的文件；
-l：显示已安装软件包列表；
--unpack：解开软件包；
-c：显示软件包内文件列表；
--confiugre：配置软件包。
得先定位到放deb包的位置才行。
比如下载在/home/username/download的话，得先用cd /home/username/downlaod定位到相应的目录下。若Terminal位置都不在放文件的目录里面自然会提示找不到文件。然后DPKG
获取文件路径：可视化界面查看，进入文件中，输入 Ctrl+L
安装key时照着nvidia说明书无限出错，按照界面给你的要求来写：
![](https://img-blog.csdn.net/20180108140603130?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvWVpYbnVhYQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
剩下两步按照nvidia说明书照旧。
3.cuDNN
https://developer.nvidia.com/rdp/cudnn-download
必须先注册，找到指定目标下载。因为很好下，更新速度也很快，所以貌似也没人分享资料
![](https://img-blog.csdn.net/20180108143854675?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvWVpYbnVhYQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
命令1： tar -zxf cudnn-9.1-linux-x64-v7.tgz && cd cuda   #解压缩文件包并且直接进入
命令2：ubuntu@ubuntu-Z87P-D3:~/下载/cuda$ sudo cp include/cudnn.h /usr/local/cuda-9.1/include/
![](https://img-blog.csdn.net/20180108144539307?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvWVpYbnVhYQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
命令3：ubuntu@ubuntu-Z87P-D3:~/下载/cuda$ sudo cp lib64/libcudnn* /usr/local/cuda-9.1/lib64
将目录下四个文件拷贝过去，手动操作也是可行的
4.安装libcupti-dev安装包
ubuntu@ubuntu-Z87P-D3:~$ sudo apt-get install libcupti-dev
一个小小的依赖文件
5.变量设置
## 1.5.变量设置
```bash
#!/bin/sh
# Author:wangxiaolei 王小雷
# Blog: http://blog.csdn.net/dream_an
# Github: https://github.com/wangxiaoleiai
# Date: 201707
# Organization: https://github.com/whaleai
export CUDA_HOME=/usr/local/cuda-8.0
export PATH=$CUDA_HOME/bin:$PATH
export LD_LIBRARY_PATH=$CUDA_HOME/lib64
```
- 1
- 2
- 
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 1
# 2.安装tensorflow-gpu
可以通过python2或者python3的安装，博主使用的是python3。
## 2.1.安装依赖
```
xiaolei@wang:~$ sudo apt-get install python-pip python-dev   # for Python 2.7
xiaolei@wang:~$ sudo apt-get install python3-pip python3-dev # for Python 3.n
```
- 1
- 2
## 2.2.安装tensorflow-gpu版本
```
xiaolei@wang:~$ pip install tensorflow-gpu  # Python 2.7;  GPU support
xiaolei@wang:~$ spip3 install tensorflow-gpu # Python 3.n; GPU support
```
- 1
- 2
![这里写图片描述](https://img-blog.csdn.net/20170711185240362?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHJlYW1fYW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 3.测试tensorflow-gpu
`xiaolei@wang:~$ python3`- 1
```python
import tensorflow as tf
hello = tf.constant('Hello, TensorFlow!')
sess = tf.Session()
print(sess.run(hello))
```
- 1
- 2
- 3
- 4
![这里写图片描述](https://img-blog.csdn.net/20170711185256401?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHJlYW1fYW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 
**可以看到已经使用GPU了！完结-成功**
### 1.彩蛋-卸载方法
> 
```
xiaolei@wang:~$ sudo pip uninstall tensorflow  # for Python 2.7
xiaolei@wang:~$ sudo pip3 uninstall tensorflow # for Python 3.n
```
- 1
- 2
### 2.彩蛋-官网给出的常见错误处理方法
> 
[https://www.tensorflow.org/install/install_sources#common_installation_problems](https://www.tensorflow.org/install/install_sources#common_installation_problems)
