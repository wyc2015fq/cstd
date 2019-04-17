# 【Pytorch】Windows10下配置Pytorch环境 - CD's Coding - CSDN博客





2017年09月18日 19:24:54[糖果天王](https://me.csdn.net/okcd00)阅读数：4647








## 0x00 前言

前言什么的也懒得说了……  

总之：  

听说你Pytorch很牛， 

不乐意让我Windows用，  

而我又听说pytorch用来训练模型超好用， 

不仅没头脑而且不高兴！我要在我的windows上配一个！
**[NEW]** PyTorch在0.4.0版已完美适配Windows，详情请见[这篇文章](https://blog.csdn.net/okcd00/article/details/80077799)
[https://blog.csdn.net/okcd00/article/details/80077799](https://blog.csdn.net/okcd00/article/details/80077799)

## 0x01 准备工作
- Win10 64位操作系统
- Anaconda3：本人用的是3，2应该也可以（吧？）
- Pytorch 的 conda离线安装包 (Thx to 知乎@[蒲嘉宸](https://www.zhihu.com/people/pu-jia-chen)) 

链接：[http://pan.baidu.com/s/1jH9F7Si](http://pan.baidu.com/s/1jH9F7Si) 密码：8s5a

## 0x02 配置开始

首先，我们需要一个比较配pytorch的环境，此处使用python3.6.1，因为我把离线安装包下载到了D盘的Downloads目录下，所以我们就在`D:\Downloads\`这里操作吧，根据你们想放在哪里自行安排。

```
$ conda create -n torch python=3.6.1
Fetching package metadata ...........
Solving package specifications: .

Package plan for installation in environment C:\ProgramData\Anaconda3\envs\torch:

The following NEW packages will be INSTALLED:

    certifi:        2016.2.28-py36_0
    pip:            9.0.1-py36_1
    python:         3.6.1-2
    setuptools:     36.4.0-py36_0
    vs2015_runtime: 14.0.25420-0
    wheel:          0.29.0-py36_0
    wincertstore:   0.2-py36_0

Proceed ([y]/n)? y

vs2015_runtime 100% |###############################| Time: 0:01:28  23.07 kB/s
python-3.6.1-2 100% |###############################| Time: 0:28:32  19.29 kB/s
certifi-2016.2 100% |###############################| Time: 0:00:10  21.24 kB/s
wincertstore-0 100% |###############################| Time: 0:00:00  45.54 kB/s
setuptools-36. 100% |###############################| Time: 0:00:21  28.14 kB/s
#
# To activate this environment, use:
# > activate torch
#
# To deactivate an active environment, use:
# > deactivate
#
# * for power-users using bash, you must source
#
```

然后我们进入这个只有默认包的conda环境中，至少把numpy啥的pytorch必备组件给他装上：

```
(torch) D:\Downloads>conda install numpy mkl cffi
Fetching package metadata ...........
Solving package specifications: .

Package plan for installation in environment C:\ProgramData\Anaconda3\envs\torch:

The following NEW packages will be INSTALLED:

    cffi:      1.10.0-py36_0
    mkl:       2017.0.3-0
    numpy:     1.13.1-py36_0
    pycparser: 2.18-py36_0

Proceed ([y]/n)?

pycparser-2.18 100% |###############################| Time: 0:00:06  27.88 kB/s
```

然后就是激动人心的装这个离线包了！

```
(torch) D:\Downloads>conda install --offline pytorch-0.2.1-py36he6bf560_0.2.1cu80.tar.bz2

CondaError: EOFError('Compressed file ended before the end-of-stream marker was reached',)
CondaError: EOFError('Compressed file ended before the end-of-stream marker was reached',)
CondaError: EOFError('Compressed file ended before the end-of-stream marker was reached',)
```

我很想大声的喊一句“不可能只有我一个人，不会只有我遇到这种情况！” 

在check了一下下载的压缩包并没有损坏之后，查到一个解决方案：（Thx to @[李新鹏](https://www.zhihu.com/people/li.xin.peng)）:

> 
解决‘CondaError: EOFError(‘Compressed file ended before the end-of-stream marker was reached’,)’问题，在`anaconda/pkgs`文件夹删除缓存的pytorch安装包再安装比较好，不会删去其余完好的安装包


然后在anaconda目录（比如我的在`C:\ProgramData\Anaconda3\pkgs\`）下，找到先前缓存的pytorch包（比如我的是`C:\ProgramData\Anaconda3\pkgs\pytorch-0.2.1-py36he6bf560_0.2.1cu80.tar.bz2`），直接把这个文件删掉，然后回到CMD中，再次输入

```python
(torch) D:\Downloads>conda install --offline pytorch-0.2.1-py36he6bf560_0.2.1cu80.tar.bz2

(torch) D:\Downloads>python
Python 3.6.1 |Continuum Analytics, Inc.| (default, May 11 2017, 13:25:24) [MSC v.1900 64 bit (AMD64)] on win32
Type "help", "copyright", "credits" or "license" for more information.
>>> import pytorch
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
ModuleNotFoundError: No module named 'pytorch'
>>> import torch
>>> x = torch.Tensor([1.0])
>>> xx = x.cuda()
>>> print(xx)

 1
[torch.cuda.FloatTensor of size 1 (GPU 0)]
```

可喜可贺，可喜可贺~

![啦啦啦](https://img-blog.csdn.net/20170918192335796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

哦对了最后还可以放进jupyter notebook的kernel里用

```
sudo pip install -U ipykernel
# source activate torch
python -m ipykernel install --user
```




