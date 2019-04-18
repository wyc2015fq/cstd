# Windows下安装Pytorch - YZXnuaa的博客 - CSDN博客
2018年06月06日 16:45:53[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：2119
需要满足以下条件：
- Anaconda3 x64 (with Python 3.5/3.6)
- Windows 64位系统（Windows 7 或 Windows Server 2008 及以上）
- GPU版本还需要任意版本的 CUDA （包内置了CUDA 8 / 9 的部分主要二进制文件）
这几个条件个人感觉还算比较OK，如果不想放弃Anaconda2也可以创建虚拟环境来使用。
要安装的话，如果你不嫌弃anaconda cloud的网速的话，只需根据自己的系统键入下面的一条命令即可：（注：仅 0.3.1，以后不再更新）
```
# for CUDA 8, Windows 10 or Windows Server 2016
conda install -c peterjc123 pytorch
# for CUDA 9, Windows 10 or Windows Server 2016
conda install -c peterjc123 pytorch cuda90
# for CUDA 9.1, Windows 10 or Windows Server 2016
conda install -c peterjc123 pytorch cuda91
# for CUDA 8, Windows 7 or Windows Server 2008/2012
conda install -c peterjc123 pytorch cuda80
# for CUDA 9, Windows 7 or Windows Server 2008/2012
conda install -c peterjc123 pytorch cuda90
```
如果不能忍受conda那蜗牛爬般的网速的话，那么大家可以尝试以下两种途径：
1. 添加清华源，然后使用conda进行安装。（注：0.3.1 及以后）
```
### for those who don't use tsinghua mirror before
conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/
conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/main/
conda config --set show_channel_urls yes
### for 0.4.0 and later
conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/cloud/pytorch/
# for CPU only packages
conda install pytorch-cpu
# for Windows 7, Windows Server 2008 or up, CUDA 8
conda install pytorch
# for Windows 7, Windows Server 2008 or up, CUDA 9
conda install pytorch cuda90
# for Windows 7, Windows Server 2008 or up, CUDA 9.1
conda install pytorch cuda91
### for 0.3.1
conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/cloud/peterjc123/
# for CUDA 8, for Windows 7, Windows 10 or Windows Server 2016
conda install pytorch
# for CUDA 9, for Windows 7, Windows 10 or Windows Server 2016
conda install pytorch cuda90
# for CUDA 9.1, Windows 10 or Windows Server 2016
conda install pytorch cuda91
# for CUDA 8, Windows 7 or Windows Server 2008/2012
conda install pytorch_legacy
# for CUDA 9, Windows 7 or Windows Server 2008/2012
conda install pytorch_legacy cuda90
```
2. 百度云进行下载，大家下载之后，键入如下几条指令：（注：0.4.0及以后的不再存放）
下载链接：https://pan.baidu.com/s/1BK71G4naPDKTymvQVI9yYQ
```
cd "下载包的路径"
conda install numpy mkl cffi
conda install --offline pytorch????.tar.bz2
```
注：文件名说明：
一般为以下两种形式
*PACKAGENAME*-*VERSION*-*PYTHON*_*VERSION*cu*CUDA_VERSION*.tar.bz
或
*PACKAGENAME*-*VERSION*-*PYTHON*_*VERSION_*cuda*CUDA_VERSION_*cudnn*CUDNN_VERSIONHASH_REVISION*.tar.bz2
*PACKAGENAME* 分为 pytorch 和 pytorch_legacy， 分别为NT内核版本10和6的两类系统进行编译；*VERSION* 代表 pytorch 的版本；而*PYTHON*则代表python程序的版本，主要分为3.5和3.6；*CUDA_VERSION*和*CUDNN_VERSION*分别代表CUDA和cuDNN编译的版本；*REVISION*代表修订号。请自行选择合适的版本进行安装。
安装之后，也千万要注意，要在主代码的最外层包上
```python
if __name__ == '__main__':
```
这个判断，可以参照我昨天文章中的例子，因为PyTorch的多线程库在Windows下工作还不正常。
更新：经网友提醒，若import torch时发生如下错误：
```python
Traceback (most recent call last):
  File "test.py", line 2, in <module>
    import torch
  File "C:\Anaconda3\lib\site-packages\torch\__init__.py", line 41, in <module>
    from torch._C import *
ImportError: DLL load failed: The specified module could not be found.
```
- 请将Anaconda的Python版本升级至3.5.3/3.6.2及以上。
- 如果安装了CUDA编译的包，请确保你的电脑有Nvidia的显卡。
- 如还不行，试试看创建虚拟环境是否能解决。
附一段简单测试CUDA与cuDNN是否工作正常的代码：
```python
# CUDA TEST
import torch
x = torch.Tensor([1.0])
xx = x.cuda()
print(xx)
# CUDNN TEST
from torch.backends import cudnn
print(cudnn.is_acceptable(xx))
```
如果CUDA工作不正常，那就不能使用.cuda()将模型和数据通过GPU进行加速了。而如果cuDNN不能正常工作，那就使用如下代码关掉它：
```python
cudnn.enabled = False
```
以上，就是文章的全部内容啦，如果感觉还意犹未尽的话，可以给我的Github [主页](https://link.zhihu.com/?target=https%3A//github.com/peterjc123/)或者[项目](https://link.zhihu.com/?target=https%3A//github.com/peterjc123/pytorch/tree/windows)加个watch或者star之类的（滑稽），以后说不定还会再分享一些相关的经验。
