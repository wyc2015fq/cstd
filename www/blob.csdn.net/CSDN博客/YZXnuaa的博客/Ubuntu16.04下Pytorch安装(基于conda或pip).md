# Ubuntu16.04下Pytorch安装(基于conda或pip) - YZXnuaa的博客 - CSDN博客
2018年03月27日 14:55:15[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：2890
# 问题描述
国内的我们在Ubuntu下用pip或者conda安装软件包都容易龟速最终导致失败,但是嗑盐的我们耽误不起时间,所以我们要找一个最好最快的安装的方式,以下我们介绍基于conda和pip的两种安装方式.
# 安装
## 基于conda安装
### 1. 安装conda命令(Anaconda或者Miniconda)
- 
Anaconda包括包管理器和很多常用package,因此如果很多库你都还没有安装,直接安装Anaconda是不错的办法.
- 
反之,如果很多依赖package你都安装好了,那么你安装miniconda 就好,也就是仅仅安装一个package管理器.
通常Anaconda的安装包都是.sh格式,假设是filename.sh,那么安装是在前面采用**sh**命令就可以进行安装了
`sh filename.sh`- 1
### 2.修改conda镜像源
在终端中运行以下命令修改镜像源,可以明显加速安装.
```
# 优先使用清华conda镜像
conda config --prepend channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/
# 也可选用科大conda镜像
conda config --prepend channels http://mirrors.ustc.edu.cn/anaconda/pkgs/free/
```
- 1
- 2
- 3
- 4
- 5
- 6
要查看镜像源是否安装成功的话,建议终端中运行以下命令:
`conda config --set show_channel_urls yes`- 1
会生成一个~/.condarc文件,运行cat命令查看文件内容
`cat ~/.condarc`- 1
显示内容为
```
channels:
  - https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/
  - http://mirrors.ustc.edu.cn/anaconda/pkgs/free/
  - defaults
show_channel_urls: true
```
- 1
- 2
- 3
- 4
- 5
这样每次用conda安装时,在package右边都会显示安装源的地址,那么我们对于安装的时间能有一个大致的估计. 
修改后,用conda安装镜像源中任何库都能明显加速,在我的电脑上安装速度级可以达到MB/s 
![安装速度](https://img-blog.csdn.net/20180118122537579?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveXVjaWNoZXVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### 3. 安装Pytorch
我安装的方法是conda+python2.7+CPU,所以安装命令如下:
`conda install pytorch-cpu torchvision -c pytorch`- 1
要是需要python 3或者GPU版本的安装,请参考[Pytorch官网](http://pytorch.org/),按如下方式选择你自己的安装方案,运行显示的安装命令. 
![寻找命令](https://img-blog.csdn.net/20180118123612928?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveXVjaWNoZXVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 基于pip文件安装
### 1. 配置镜像源
请参考博文[Ubuntu16.10下配置pip国内镜像源加速安装](http://blog.csdn.net/yucicheung/article/details/79095742). 
这一步是为加速安装torch vision进行的.
### 2. 下载.whl安装文件
**(1)速度慢但是版本最全：**
可以按pytorch安装指南的网址，如： 
[http://download.pytorch.org/whl/cpu/torch-0.3.1-cp35-cp35m-linux_x86_64.whl](http://download.pytorch.org/whl/cpu/torch-0.3.1-cp35-cp35m-linux_x86_64.whl)直接下载.whl文件，具体网址请在[pytorch官网](https://blog.csdn.net/yucicheung/article/details/pytorch.org)查询． 
**(2)速度快但是版本可能不全：**
从pypi官网 [https://pypi.python.org/packages](https://pypi.python.org/packages)下载需要的.whl文件 
也可以从以下的pypi国内的镜像源网站中下载相应的.whl文件: 
**豆瓣(douban) [http://pypi.douban.com/simple/](http://pypi.douban.com/simple/)** (推荐) 
阿里云 [http://mirrors.aliyun.com/pypi/simple/](http://mirrors.aliyun.com/pypi/simple/)
中国科技大学 [https://pypi.mirrors.ustc.edu.cn/simple/](https://pypi.mirrors.ustc.edu.cn/simple/)
清华大学 [https://pypi.tuna.tsinghua.edu.cn/simple/](https://pypi.tuna.tsinghua.edu.cn/simple/)
中国科学技术大学 [http://pypi.mirrors.ustc.edu.cn/simple/](http://pypi.mirrors.ustc.edu.cn/simple/)
比如我现在在cpu+python2.7上用的whl文件就是
> 
torch-0.3.0.post4-cp27-cp27mu-linux_x86_64.whl 
  在不清楚用哪个文件时,可以先按pytorch官网命令安装,命令行会有提示的.
### 3. 安装pytorch
用pip install .whl文件即可
```
pip install torch-0.3.0.post4-cp27-cp27mu-linux_x86_64.whl
pip install torch vision
```
- 1
- 2
有了镜像加持和.whl文件的本地化,安装很顺利 
torch vision安装非常迅速
# 测试
在终端中按以下顺序运行命令:
`python`- 1
在打开的python命令行中运行
```python
import torch
```
- 1
