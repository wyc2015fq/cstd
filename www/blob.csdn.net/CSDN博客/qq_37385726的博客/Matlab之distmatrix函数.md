# Matlab之distmatrix函数 - qq_37385726的博客 - CSDN博客





2018年09月14日 09:45:21[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：275








# **目录**

[1. 自己写一个distmatrix函数放进去。](#1.%20%E8%87%AA%E5%B7%B1%E5%86%99%E4%B8%80%E4%B8%AAdistmatrix%E5%87%BD%E6%95%B0%E6%94%BE%E8%BF%9B%E5%8E%BB%E3%80%82)

[2. 将函数放到 C:\Program Files\MATLAB\R2017a\toolbox\images\images 路径下](#2.%C2%A0%E5%B0%86%E5%87%BD%E6%95%B0%E6%94%BE%E5%88%B0%C2%A0C%3A%5CProgram%20Files%5CMATLAB%5CR2017a%5Ctoolbox%5Cimages%5Cimages%C2%A0%E8%B7%AF%E5%BE%84%E4%B8%8B)

[3.更新cache](#3.%E6%9B%B4%E6%96%B0cache)

我觉得找过来的八成是看了《使用MATLAB图像和视频处理》一书，报错来的2333

没错，我也是报错了在网上找不到解决教程于是才写了这篇文章，帮大家解决下问题吧。



MATLAB中并没有内置distmatrix函数，所以你也用不了，解决方式是：

### 1. 自己写一个distmatrix函数放进去。

**源代码如下**：

```
function y = distmatrix(M,N)

u = 0:(M - 1);
v = 0:(N - 1);

ind_u = find(u > M/2);
u(ind_u) = u(ind_u) - M;
ind_v = find(v > N/2);
v(ind_v) = v(ind_v) - N;

[V, U] = meshgrid(v, u);

%calculate distance matrix
y = sqrt((U .^ 2) + (V .^ 2));
```

上面这个代码，你当然可以自己去加函数描述了，我就不放上了。



### 2. 将函数放到 C:\Program Files\MATLAB\R2017a\toolbox\images\images 路径下

我看到关于图像处理的很多函数都在这个路径下，于是我们就也放在这个路径下吧



### 3.更新cache

文件放在matlabroot/toolbox路径下了吧，此时如果运行which distmatrix是找不到文件的，这和Matlab为了提高启动和运行速度所采取的一个优化措施有关。Matlab把toolbox下的函数都cache了一下，所以只需要更新一下toolboxcache就可以了。

更新方式有两种，一种通过设置，一种跑代码就行。

①

更新缓存的步骤



要更新缓存和缓存文件，请执行以下操作：
- 
在**主页**选项卡上的**环境**部分中，点击 ![](https://ww2.mathworks.cn/help/matlab/matlab_env/help_browser_action_btn_zh_CN.png)**预设**。选择 **MATLAB** > **常规**。

此时将显示**常规预设项**窗格。

- 
点击**更新工具箱路径缓存**并点击**确定**。


②备用函数

要更新缓存，请使用 `rehash toolbox`。要同时更新缓存和缓存文件，请使用 `rehash toolboxcache`。有关详细信息，请参阅 [`rehash`](https://ww2.mathworks.cn/help/matlab/ref/rehash.html)。





