# 在Windows下正确安装Scipy与Numpy - Machine Learning with Peppa - CSDN博客





2018年04月13日 15:31:15[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：3493










开发环境：win10 x64   Python 2.7.x/3.5.1


直接使用Pip在线下载安装scipy，会产生各种报错

```python
pip install numpy
pip install scipy
```

后来查资料才发现需要在它之前需要安装一个numpy+mkl这样的包。而numpy可以正常安装成功，scipy失败原因是scipy要依赖于numpy和其他的很多库（如LAPACK/BLAS）


那么如何安装numpy和scipy才能成功的使用这两个库呢？

1. 下载numpy+mkl

使用http://www.lfd.uci.edu/~gohlke/pythonlibs/ 这个地址来下载，虽然这个网站是非官方的，但是可靠性可以放心。这个网页的内容特别多，加载速度特别慢，最好等网页加载完成之后再使用ctrl+f来查找内容，否则会发现，怎么没有我要的包？

![](https://images2015.cnblogs.com/blog/901353/201701/901353-20170118101739234-1606640825.png)



这么多包，到底哪个才是我们需要的呢？不要着急，我们看看这些包名称的组成，其中cp27-cp27m 代表python2.7+，同理cp-34-cp34m代表python3.4+，win32对应python32。win-amd64代表python64，但是我的处理器不是amd啊，放心intel的它也支持。（亲测通过）

在网页上我们看到这句话：This page is not a pip package index.也就是通过Pip我们下载不到的，所以只能手动下载。注意：下载之后，文件名称可以会与你点击下载的名称不一致（mkl这部分可能不见了），这时候需要改为一致。

2.安装numpy+mkl

下载成功之后，我们进入包的存放路径，使用 pip install numpy-1.12.0+mkl-cp27-cp27m-win_amd64.whl 命令进行安装（默认的安装路径为：C:\Python2.7.13-64\Lib\site-packages）

3. 下载安装scipy

(venv) pip install <路径名>\scipy-0.18.1-cp35-none-win_amd64.whl

使用pip在线安装的scipy的时候，不清楚为什么下载的是32位的，所有还是在这个非官方的网站下载了（我们这个时候也可以看到，它明确说明了需要先安装numpy+mkl）

![](https://images2015.cnblogs.com/blog/901353/201701/901353-20170118101815703-372983960.png)

关于包的名称的含义就不重复了，根据需要进行下载了。下载完成之后，我们进入包的存放路径，使用 pip install scipy-0.18.1-cp27-cp27m-win_amd64.whl 命令进行安装（默认的安装路径为：C:\Python2.7.13-64\Lib\site-packages）

![](https://images2015.cnblogs.com/blog/901353/201701/901353-20170118101834265-583963478.png)

此时在Python中执行


```python
import scipy as sc
import numpy as np
```

测试成功，可成功使用这两个常用库了




