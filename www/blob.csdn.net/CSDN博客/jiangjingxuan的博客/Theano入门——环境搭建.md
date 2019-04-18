# Theano入门——环境搭建 - jiangjingxuan的博客 - CSDN博客





2019年02月22日 10:02:23[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：39








**本文为原创，转载请注明：[https://blog.csdn.net/jiangjingxuan/article/details/87872125](https://blog.csdn.net/jiangjingxuan/article/details/87872125)**



一、安装MinGW-W64

1、下载MinGW-W64

下载地址：[https://versaweb.dl.sourceforge.net/project/mingw-w64/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/installer/mingw-w64-install.exe](https://versaweb.dl.sourceforge.net/project/mingw-w64/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/installer/mingw-w64-install.exe)

2、运行mingw-w64-install.exe，安装MinGW-W64到C:\mingw64目录，其中配置参数如下

![](https://img-blog.csdnimg.cn/20190222092538558.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ppYW5namluZ3h1YW4=,size_16,color_FFFFFF,t_70)

 3、安装完成后，进入目录C:\mingw64\mingw64\bin，复制mingw32-make.exe文件，重命名为make.exe。



二、安装Git

1、下载Git for Windows

下载地址：[https://github.com/git-for-windows/git/releases/download/v2.20.1.windows.1/Git-2.20.1-64-bit.exe](https://github.com/git-for-windows/git/releases/download/v2.20.1.windows.1/Git-2.20.1-64-bit.exe)

2、运行Git-2.20.1-64-bit.exe，其中配置参数如下：

![](https://img-blog.csdnimg.cn/2019022209382424.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ppYW5namluZ3h1YW4=,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190222093858137.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ppYW5namluZ3h1YW4=,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190222093910696.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ppYW5namluZ3h1YW4=,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190222093928356.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ppYW5namluZ3h1YW4=,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190222093946234.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ppYW5namluZ3h1YW4=,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190222094000397.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ppYW5namluZ3h1YW4=,size_16,color_FFFFFF,t_70)





四、安装OpenBLAS

1、下载OpenBLAS 0.3.5 version.zip

下载地址：[https://managedway.dl.sourceforge.net/project/openblas/v0.3.5/OpenBLAS%200.3.5%20version.zip](https://managedway.dl.sourceforge.net/project/openblas/v0.3.5/OpenBLAS%200.3.5%20version.zip)

2、解压OpenBLAS 0.3.5 version.zip到C盘，重命名源码目录xianyi-OpenBLAS-eebc189为OpenBLAS-0.3.5。

3、进入OpenBLAS解压的目录C:\OpenBLAS-0.3.5，鼠标右键打开Git Bash，分别输入以下指令编译OpenBLAS

```bash
$ make
$ make install PREFIX=/c/OpenBLAS-0.3.5/dist
```

4、“计算机”-->“属性”-->"高级系统设置"-->"高级"-->“环境变量”，在"系统变量"的PATH后添加C:\OpenBLAS-0.3.5\dist\bin;

![](https://img-blog.csdnimg.cn/20190222094448322.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ppYW5namluZ3h1YW4=,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190222094643124.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ppYW5namluZ3h1YW4=,size_16,color_FFFFFF,t_70)

 5、在当前的用户目录下新增文件.theanorc.txt

位置：C:\Users\[用户名]\.theanorc.txt

文件内容：

```
[global]
openmp = False
device = cpu
floatX = float32
allow_input_downcast=True

[blas]
ldflags =-LC:\OpenBLAS-0.3.5\dist\bin -lopenblas
```



五、安装Miniconda Python 2.7 64-bit

1、安装Miniconda

下载地址：[https://repo.anaconda.com/miniconda/Miniconda2-latest-Windows-x86_64.exe](https://repo.anaconda.com/miniconda/Miniconda2-latest-Windows-x86_64.exe)

2、运行Miniconda2-latest-Windows-x86_64.exe，安装完成后设置为默认Python解释器并添加到环境变量。

![](https://img-blog.csdnimg.cn/20190222095413109.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ppYW5namluZ3h1YW4=,size_16,color_FFFFFF,t_70)



六、安装 Theano

1、以"管理员身份运行"打开"命令提示符"

![](https://img-blog.csdnimg.cn/20190222095534966.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ppYW5namluZ3h1YW4=,size_16,color_FFFFFF,t_70)

2、在"命令提示符"中分别运行以下指令，安装Theano

```bash
pip install numpy scipy
pip install parameterized
pip install Pillow pytest
conda install mingw libpython
conda install mkl mkl-service m2w64-toolchain nose sphinx pydot-ng
pip install theano
```

3、Theano安装完成后，新建文件D:\mlpj\project\test.py，文件内容：

 文件内容：

```python
import numpy as np
import time
import theano
 
print('blas.ldflags=', theano.config.blas.ldflags)
 
A = np.random.rand(1000, 10000).astype(theano.config.floatX)
B = np.random.rand(10000, 1000).astype(theano.config.floatX)
np_start = time.time()
AB = A.dot(B)
np_end = time.time()
X, Y = theano.tensor.matrices('XY')
mf = theano.function([X, Y], X.dot(Y))
t_start = time.time()
tAB = mf(A, B)
t_end = time.time()
print("NP time: %f[s], theano time: %f[s] (times should be close when run on CPU!)" % (
np_end - np_start, t_end - t_start))
print("Result difference: %f" % (np.abs(AB - tAB).max(), ))
```

3、在"命令提示符"中运行指令python  D:\mlpj\project\test.py，出现如下图所示，说明Theano已经成功安装！

![](https://img-blog.csdnimg.cn/20190222100215986.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ppYW5namluZ3h1YW4=,size_16,color_FFFFFF,t_70)















