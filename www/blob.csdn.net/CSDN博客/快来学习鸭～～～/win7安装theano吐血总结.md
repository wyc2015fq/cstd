# win7安装theano吐血总结 - 快来学习鸭～～～ - CSDN博客





2017年01月05日 16:12:09[DivinerShi](https://me.csdn.net/sxf1061926959)阅读数：13235








[文章地址:](http://blog.csdn.net/sxf1061926959/article/details/54094356)[http://blog.csdn.net/sxf1061926959/article/details/54094356](http://blog.csdn.net/sxf1061926959/article/details/54094356)

作者：[XFShi](https://github.com/Shicoder)[https://github.com/Shicoder](https://github.com/Shicoder)

```
Theano是我安装时间最长的一个东西了。
我的环境：Win7 64位
```

## 1、安装Anaconda

Anaconda公认的方便，里面有安装theano需要的所有包。 

建议选择老版本的anaconda，新版的anaconda2也能装，但是里面没有MinGW，需要自己用命令下载，命令如下：conda install mingw libpython。但是学校的校园网实在是难以恭维，在cmd下下载东西国内的都很慢，况且国外的。 

Anaconda版本可以到这里自己选择，我用的是anaconda-2.1.0-windows-x86_64.exe，地址： 
[https://mirrors.tuna.tsinghua.edu.cn/anaconda/archive/](https://mirrors.tuna.tsinghua.edu.cn/anaconda/archive/)

我选择这个版本的原因是这个版本预先内置安装了MinGW，这样我们就不用再去自己安装mingw了，自己安装会出现很多问题，之前看有些博客说自己下载个mingw然后放到环境变量里面，这个官网也说了，行是可行的，但是不稳定，我也试了，会莫名其妙报错。当然也可以下载anaconda2新的版本，我试过最近的anaconda2，里面是没有mingw的，要自己下载，用 
`conda install mingw libpython`

但是校园网慢，很难下载下来。我也成功下载下来过，也成功装上了，确实是可以的，但是中间也报过莫名其妙的错误。所以网快的小伙伴可以下个新的anaconda2试试，反正不行删了再来嘛。 
**继续**，由于我安装的这个自带了MinGW，所以我就可以直接跳过这步，省去了好多出现错误的可能性。 

到现在为止，你已经下载好了anaconda（任何版本），在你的anaconda安装目录下，**例如C:\Anaconda文件下有一个MinGW文件夹了，那么可以继续了**，mingw自己下载安装在其他地方的，建议删了重来。
## 2、theano安装

要用到命令了，打开cmd，输入 
`pip install theano`

我们校内网实在是慢，根本下不下来，只有大晚上才能下载。当然最后还是让我成功下载下来了。推荐一个conda和pip加速的方法： 
**使用国内的源**，因为我们在cmd下下载慢，很大一部分原因是下载源都在国外，所以被墙了，很慢。可以参考下面的地址更改pip和conda 的源。（这步可有可无，只为加速下载）。 
[http://blog.csdn.net/sxf1061926959](http://blog.csdn.net/sxf1061926959)
**回到前面**，当命令执行完成后，会显示 successfully installed theano-XXXX，下图是我从网上截的，我自己现在都不敢去动，毕竟装了那么久，哭。我的和下图的还有点不一样，我运行后出现的第一句是 download/unpacking theano ，运行完成后是  successfully installed theano six 
![这里写图片描述](https://img-blog.csdn.net/20170105160930667?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**这里如果pip install theano下载不下来，可以试试这样**： 

去官网[https://github.com/Theano/Theano](https://github.com/Theano/Theano)把theano下载下来，**取出theano-master里面的子文件夹theano**，看清楚了是**子文件夹**，如下图，![这里写图片描述](https://img-blog.csdn.net/20170105160604853?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)只要划红线的那个文件夹。 

然后放到 anaconda\Lib\site-packages\ 目录下 
**3、环境变量配置**

这步很重要，我之前因为安装网上的各种版本都试了一遍，所以环境变量比较乱，这也可能是我很多安装失败的原因。这里我的**建议**是，如果你安装报错，问题找不到后要删了重装，那么一定要**清理干净环境变量**，网上有说要清理注册表什么的，也许人家是对的，但是我找了半天也没找到那个注册表，而且我也没删注册表，表示照样让我装上了？ 

好了，打开环境变量，我觉得既然你是一个要装theano的人，那么环境变量的位置肯定已经烂熟于心了。如下图： 
![这里写图片描述](https://img-blog.csdn.net/20170105160638194?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**下面所以的环境变量都配置在系统环境变量里面。**

1、在path后加： 
`D:\MySoft\Develop\Anaconda\MinGW\bin;D:\MySoft\Develop\Anaconda\MinGW\x86_64-mingw32\lib`; 

这里的 D:\MySoft\Develop\Anaconda 是我自己的 anaconda 路径，**根据自己的安装目录，改成自己的。**

2、新建环境变量PYTHONPATH，变量值为`D:\MySoft\Develop\Anaconda\Lib\site-packages\theano;`

注意自己的路径，注意分号，别打错字，建议直接去这 

复制，省的出错了，都不知道哪里错了。 

3、打开cmd，看看自己的home目录， 
![这里写图片描述](https://img-blog.csdn.net/20170105161017120?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

进入到这个目录，就是C盘下的用户下的xxx目录下。 

创建一个 .theanorc.txt  **注意注意前面的那个点号**，一定要有。 
**内容如下：**
```
[blas]
ldflags=

[gcc]
cxxflags=-ID:\MySoft\Develop\Anaconda\MinGW\x86_64-w64-mingw32\include
```

最后一行我看也有人用 

```
cxxflags=-ID:\MySoft\Develop\Anaconda\MinGW\include
```


我试了下好像都行。 

我配置了GPU，所以我的是全部是这样的，你们按上面这个就可以了。
```
[global] 
device = gpu
floatX = float32 

[nvcc]
flags=-LD:\MySoft\Develop\Anaconda\libs
compiler_bindir=C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin
fastmath = True

[blas]
ldflags=

[gcc]
cxxflags=-ID:\MySoft\Develop\Anaconda\MinGW\x86_64-w64-mingw32\include
```

正常情况下，到这里应该就好了，用 cpu的theano 就弄好了。 

打开cmd， 

输入：python 

再输入 

```
import theano  

theano.test()
```


 没有报错就是OK了，会有一个 warning：PyCUDA import failed ,这个是因为没有GPU。当然如果报错了，也不要怕，我之前在台式机上的就有错，什么nose什么的，具体忘了，但是具体使用的时候居然还能用。**所以可以再试试下面的测试demo**。**或者先重启下电脑**，我之前有一次报错，重启了一下就可以用了。
```
1.import numpy as np  
2.import time  
3.import theano  
4.A = np.random.rand(1000,10000).astype(theano.config.floatX)  
5.B = np.random.rand(10000,1000).astype(theano.config.floatX)  
6.np_start = time.time()  
7.AB = A.dot(B)  
8.np_end = time.time()  
9.X,Y = theano.tensor.matrices('XY')  
10.mf = theano.function([X,Y],X.dot(Y))  
11.t_start = time.time()  
12.tAB = mf(A,B)  
13.t_end = time.time()  
14.print "NP time: %f[s], theano time: %f[s] (times should be close when run on CPU!)" %(  
15.                                           np_end-np_start, t_end-t_start)  
16.print "Result difference: %f" % (np.abs(AB-tAB).max(), )
```

如果这个demo跑完还有错的话， 
**第一:**按照错误的提示去网上找答案; 
**第二:**删了重装，不要怕，删了没事的，先去卸载程序哪里把anaconda下的python卸载了，或者直接把anaconda文件夹删了。 

我之前遇到过一个错误。如下： 

WARNING (theano.gof.compilelock): Overriding existing lock by dead process ‘5216’ (I am process ‘5488’) 

网上的答案是这样的：调试之后发现是theano库中function_module.py代码出现了问题，于是用自己下载的theano-mater中的theano子文件夹（[【Theano-master下载】](https://github.com/Theano/Theano)）替换掉theano_installer_latest.msi自动安装的theano（Anaconda\Lib\site-packages\theano） 

有遇到这个的可以先试试这个方法，但是我自己试了下，发现没有用。最后发现是环境变量里有重复的mingw。删掉没有用的那个就可以了 

后来又报错xception: Compilation failed (return status=1):。。。。。。。。。。。 

发现是.theanorc.txt文件因为看了好多版本的这个，写串了。还有什么DLL load failed，好多错误。
## 总结一下：

**1、**一定要有MinGW，而且一定不能是单独安装的，而且环境变量里一定**只能有一个**。 

可以是anaconda自带的MinGW，也可以是通过命令 conda mingw libpython 下载的。 
**2、**安装完theano报错的，不要急，先看看环境变量对不对，有没有写错，有没有多个mingw，环境变量有没有漏写，看看.theanorc.txt里面写的对不对，路径对不对，文件名前面的那个点.有没有写。如果以上都对的，那么去github上clone一个theano（[https://github.com/Theano/Theano](https://github.com/Theano/Theano)），将里面的子文件夹theano放到anaconda\Lib\site-packages\下，再看看对不对。 

如果还是不行，那么删了换个版本的anaconda再装一次吧。 

当前面全部弄好后，cpu的theano已经可以用了，如果要装GPU，看我的参考文献，写的还可以。
## 参考文献：

[http://blog.csdn.net/hjimce/article/details/46654229](http://blog.csdn.net/hjimce/article/details/46654229)
[http://blog.sina.com.cn/s/blog_990865340101hvuq.html](http://blog.sina.com.cn/s/blog_990865340101hvuq.html)











