# 白话压缩感知（含Matlab代码） - xiahouzuoxin - CSDN博客





2014年08月25日 15:16:46[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：34708
个人分类：[DSP](https://blog.csdn.net/xiahouzuoxin/article/category/1520295)

所属专栏：[DSP算法研究与实现](https://blog.csdn.net/column/details/zuoxin-dsp.html)













## 压缩感知介绍

压缩感知（Compressive Sensing，CS），有时也叫成Compressive Sampling。相对于传统的奈奎斯特采样定理——要求采样频率必须是信号最高频率的两倍或两倍以上（这就要求信号是带限信号，通常在采样前使用低通滤波器使信号带限），压缩感知则利用数据的冗余特性，只采集少量的样本还原原始数据。

这所谓的冗余特性，借助MLSS2014马毅老师的课件上的例子来说明，

![data_property](https://github.com/xiahouzuoxin/notes/raw/master/images/%E7%99%BD%E8%AF%9D%E5%8E%8B%E7%BC%A9%E6%84%9F%E7%9F%A5/data_property.png)

因为自然界的数据都存在局部低维结构、周期性、对称性等，因此，传统的固定采样率的采样方法必然存在信息冗余。由于信息冗余的存在，我们就知道有数据压缩那么一门学科。既然这样，为什么要把冗余的数据也采进来，再进行压缩呢，能不能不把冗余的数据采集进来？

压缩感知的思路就是：在采集的过程中就对数据进行了压缩，而且这种压缩能保证不失真（低失真）的恢复原始数据，这与传统的先2倍频率采集信号→存储→再压缩的方式不同，可以降低采集信号的存储空间和计算量。

好了，那么就来了解一下压缩感知的具体模型。

### [](https://github.com/xiahouzuoxin/notes/blob/master/essays/%E7%99%BD%E8%AF%9D%E5%8E%8B%E7%BC%A9%E6%84%9F%E7%9F%A5.md#1-%E7%A8%80%E7%96%8F%E8%A1%A8%E7%A4%BA)1. 稀疏表示

使用压缩感知理论首先要求信号能表示为稀疏信号，如x=[1 0 0 0 1 0]，其中只有2个1，可认为是稀疏的。我们将信号通过一个矩阵映射到稀疏空间，

![](https://camo.githubusercontent.com/91106bd15e2300d92ad12d6eb4a8b85692c9bc97/687474703a2f2f7777772e666f726b6f73682e636f6d2f6d6174687465782e6367693f2532302535434c61726765253230783d25354350736925374273253744)

设信号x为N维，即![](https://camo.githubusercontent.com/35cf11c7034dc85640713e0890c84d5b0877abd2/687474703a2f2f7777772e666f726b6f73682e636f6d2f6d6174687465782e6367693f253230253543736d616c6c253230783d253542785f312c785f322c2e2e2e2c785f4e25354425354554)，则![](https://camo.githubusercontent.com/2f241f526fb4710a5a6424a7bff6b7a58350ca15/687474703a2f2f7777772e666f726b6f73682e636f6d2f6d6174687465782e6367693f253230253543736d616c6c253230253543507369)为NxN维稀疏表达矩阵，s即是将x进行稀疏表示后的Nx1维向量，其中大部分元素值为0。稀疏表示的原理就是通过线性空间映射，将信号在稀疏空间进行表示。

比如，信号

![](https://camo.githubusercontent.com/fe80517c3126069ff83201e81d39b85737dc53c1/687474703a2f2f7777772e666f726b6f73682e636f6d2f6d6174687465782e6367693f253230253543736d616c6c253230783d253543636f73282535436672616325374232253543706925374425374232353625374474292b25354373696e28253543667261632537423225354370692537442537423132382537447429)

在时域是非稀疏的，但做傅里叶变换表示成频域后，只有少数几个点为非0（如下图）。则该信号的时域空间为非稀疏，频域空间为稀疏空间，![](https://camo.githubusercontent.com/2b9bc56e2fb4b64883d5120b0b1b556abeeb41c6/687474703a2f2f7777772e666f726b6f73682e636f6d2f6d6174687465782e6367693f253230253543736d616c6c2532302535435073693d652535452537426a7774253744)组成的矩阵。![](https://camo.githubusercontent.com/2f241f526fb4710a5a6424a7bff6b7a58350ca15/687474703a2f2f7777772e666f726b6f73682e636f6d2f6d6174687465782e6367693f253230253543736d616c6c253230253543507369)一般为正交矩阵。

![example1](https://github.com/xiahouzuoxin/notes/raw/master/images/%E7%99%BD%E8%AF%9D%E5%8E%8B%E7%BC%A9%E6%84%9F%E7%9F%A5/example1.png)

若稀疏表示后的结果s中只有k个值不为0，则称x的稀疏表示为k-Sparse。上图对x的频域稀疏表示就是2-Sparse。

### [](https://github.com/xiahouzuoxin/notes/blob/master/essays/%E7%99%BD%E8%AF%9D%E5%8E%8B%E7%BC%A9%E6%84%9F%E7%9F%A5.md#2-%E6%84%9F%E7%9F%A5%E6%B5%8B%E9%87%8F)2. 感知测量

压缩感知的目的是在采集信号时就对数据进行压缩，大牛们的思路集中到了数据采集上——既然要压缩，还不如就从大量的传感器中只使用其中很少的一部分传感器，采集少量的冗余度低的数据。这就是感知测量的通俗的说法，用表达式表示

![](https://camo.githubusercontent.com/839434ccb60b093f769e39d828d0d2a9b15a71dd/687474703a2f2f7777772e666f726b6f73682e636f6d2f6d6174687465782e6367693f2532302535434c61726765253230793d25354350686925374278253744)

其中的x就是稀疏表示中的信号，![](https://camo.githubusercontent.com/cf0264198b86222173bf5f8410e0f409f81a9f4a/687474703a2f2f7777772e666f726b6f73682e636f6d2f6d6174687465782e6367693f253230253543506869)为MxN维的感知矩阵（M表示测量信号的维度），y则表示M（M远小于N才有意义）个传感器的直接测量，因此维度为Mx1。

将稀疏表示过程和感知测量过程综合起来：

![Sparse](https://github.com/xiahouzuoxin/notes/raw/master/images/%E7%99%BD%E8%AF%9D%E5%8E%8B%E7%BC%A9%E6%84%9F%E7%9F%A5/Sparse.png)

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/%E7%99%BD%E8%AF%9D%E5%8E%8B%E7%BC%A9%E6%84%9F%E7%9F%A5.md#%E6%95%B0%E5%AD%A6%E6%8F%8F%E8%BF%B0)数学描述

对于压缩感知模型，其中每个量的维度一定要了解（通过维度的变化来理解压缩感知很有效）：

![yax](https://github.com/xiahouzuoxin/notes/raw/master/images/%E7%99%BD%E8%AF%9D%E5%8E%8B%E7%BC%A9%E6%84%9F%E7%9F%A5/yax.png)

从感知测量中知道：M就是测量的维度（M远小于N）。

压缩感知的原信号恢复问题描述为：

> 
由已知条件：

(1) 测量值y，且![](https://camo.githubusercontent.com/a9b1199f536be41cbc74116034409e35e49f23f8/687474703a2f2f7777772e666f726b6f73682e636f6d2f6d6174687465782e6367693f253230253543536d616c6c253230793d253543506869253543507369253742732537442b653d2535435468657461253742732537442b65)，其中e为噪声引入

(2) s为k-Sparse信号（k未知）

求解目标：k尽可能小的稀疏表示信号s及对应的![](https://camo.githubusercontent.com/6714238ec871f01870bed3c5d7a0bebae8a7ef64/687474703a2f2f7777772e666f726b6f73682e636f6d2f6d6174687465782e6367693f253230253543536d616c6c253230253543507369)。


用数学形式描述为：

![](https://camo.githubusercontent.com/49446517c1907642d62a798e7de9f086a16b06c7/687474703a2f2f7777772e666f726b6f73682e636f6d2f6d6174687465782e6367693f2532302535434c617267652532302535436d696e253743253743732537432537435f30253543253230253543253230253543253230253543253230732e742e253230793d2535435468657461253742732537442b65)

![](https://camo.githubusercontent.com/1e664b770761b67d005b46495f1891893aa19d7d/687474703a2f2f7777772e666f726b6f73682e636f6d2f6d6174687465782e6367693f2532302535434c61726765253230733a3d6172676d696e5f253742253543253742733a793d2535435468657461253742732537442b65253543253744253744253743253743732537432537435f30)

e可以看成告诉随机噪声，e~N(0,δ2)。

即是要求s使s的0范数（非0值的个数）最小，但0范数优化问题是很难求解的，于是一帮大牛就证明求解1范数也能逼近和上面相同的效果，而求解2范数及其更高的范数则结果相差越来越大（有些人在研究介于0范数与1范数之间的范数求解方法）。因此可转化为1范数求解：

![](https://camo.githubusercontent.com/4713111aaed0a45d17049b2c07229ffba1f63c2f/687474703a2f2f7777772e666f726b6f73682e636f6d2f6d6174687465782e6367693f2532302535434c617267652532302535436d696e253743253743732537432537435f31253543253230253543253230253543253230253543253230732e742e253230793d2535435468657461253742732537442b65)

由拉格朗日乘子法，上面的最优问题可转化成：

![](https://camo.githubusercontent.com/160d70b34f3c366900bfa59109639dba2bb3ca0c/687474703a2f2f7777772e666f726b6f73682e636f6d2f6d6174687465782e6367693f2532302535434c617267652532302535436d696e5f73253543253742253743253743732537432537435f312b2535436c616d6461253743253743792d25354354686574612537427325374425374325374325354532253543253744)

上面的最小值求解问题就可以直接通过凸优化解得结果了。

## [](https://github.com/xiahouzuoxin/notes/blob/master/essays/%E7%99%BD%E8%AF%9D%E5%8E%8B%E7%BC%A9%E6%84%9F%E7%9F%A5.md#%E7%A8%8B%E5%BA%8F%E5%88%86%E6%9E%90)程序分析

先下载CVX或spams工具箱，下面的matlab程序分别使用了时域稀疏信号和频域稀疏信号进行测试，两种不同在于时域稀疏信号不用稀疏表达矩阵（因此稀疏表达矩阵使用单位阵即可），而频域稀疏信号则需要先通过稀疏表达矩阵将信号在频域进行稀疏表示，再压缩感知后进行恢复时也要进行反FFT变换到时域。

关于M的选取：测量数M>=K*log(N/K),K是稀疏度,N信号长度,可以近乎完全重构

clcclearallcloseall%% 产生原始信号及相关参数n=;% 信号长度s=;% 稀疏度（从下面知道，分时域和频域的情况）m=*s;% 测量长度 M>=S*log(N/S)freq_sparse=;% 信号频域稀疏则为1iffreq_sparset=[:n-]';f=cos(*pi/*t)+sin(*pi/*t);% 产生频域稀疏的信号elsetmp=randperm(n);f=zeros(n,);f(tmp(:s))=randn(s,);% 产生时域稀疏的信号end%% 产生感知矩阵和稀疏表示矩阵Phi=sqrt(/m)*randn(m,n);% 感知矩阵（测量矩阵）% A = get_A_fourier(n, m);y=Phi*f;% 通过感知矩阵获得测量值% Psi 将信号变换到稀疏域iffreq_sparse% 信号频域可以稀疏表示Psi=inv(fft(eye(n,n)));% 傅里叶正变换，频域稀疏正交基（稀疏表示矩阵）else% 信号时域可以稀疏表示Psi=eye(n,n);% 时域稀疏正交基endA=Phi*Psi;% 恢复矩阵 A = Phi * Psi%% 优化方法1：使用CVX工具进行凸优化addpath('../../cvx-w64/cvx/');cvx_startup;% 设置cvx环境cvx_beginvariablexp(n)complex;% 如果xp是复数，则添加complex，否则不加minimize(norm(xp,));subjecttoA*xp==y;cvx_end%% 优化方法2：使用spams工具箱进行优化% addpath('spams-matlab/build');% param.L = 100;% param.eps = 0.001;% param.numThreads = -1;% % A=A./repmat(sqrt(sum(A.^2)),[size(A,1) 1]);% xp = mexOMP(y, A, param);       % 正交匹配追踪法Orthogonal Matching Pursuit%% 对比原信号和iffreq_sparsexp=real(ifft(full(xp)));% 傅里叶逆变换elseendplot(f+noise);holdonplot(real(xp),'r.');legend('Original','Recovered');
- 
设程序中的freq_sparse = 0时，观察时域稀疏信号的恢复结果为

![time](https://github.com/xiahouzuoxin/notes/raw/master/images/%E7%99%BD%E8%AF%9D%E5%8E%8B%E7%BC%A9%E6%84%9F%E7%9F%A5/time.png)

在恢复时域稀疏信号时，所使用的测量矩阵Phi为初始化的随机阵，因为本身时域就稀疏，而算法直接在时域进行恢复，所以稀疏表达矩阵就是单位阵Psi=E。上面的时域稀疏恢复结果显得没有误差是因为没有给原始信号添加噪声。

- 
设程序中的freq_sparse = 1时，观察频域稀疏信号的恢复结果为

![freq](https://github.com/xiahouzuoxin/notes/raw/master/images/%E7%99%BD%E8%AF%9D%E5%8E%8B%E7%BC%A9%E6%84%9F%E7%9F%A5/freq.png)

在恢复时域稀疏信号时，所使用的测量矩阵Phi为初始化的随机阵，因为信号只在频域稀疏，所以稀疏变换矩阵为傅里叶变换基，所以稀疏表达矩阵就是Psi = inv(fft(eye(n,n)))。同理，上面的频域稀疏恢复结果显得没有误差是因为没有给原始信号添加噪声。

- 
上面都是没有添加噪声的算法结果，我们给信号添加一些噪声，以时域信号为例，

iffreq_sparset=[:n-]';f=cos(*pi/*t)+sin(*pi/*t);% 产生频域稀疏的信号elsetmp=randperm(n);f=zeros(n,);f(tmp(:s))=randn(s,);% 产生时域稀疏的信号endnoise=random('norm',,0.01,[n]);f=f+noise;% 添加噪声%% Remaining code not changed

从下图的恢复结果看，已经能看到一些恢复误差了，

![err](https://github.com/xiahouzuoxin/notes/raw/master/images/%E7%99%BD%E8%AF%9D%E5%8E%8B%E7%BC%A9%E6%84%9F%E7%9F%A5/err.png)





