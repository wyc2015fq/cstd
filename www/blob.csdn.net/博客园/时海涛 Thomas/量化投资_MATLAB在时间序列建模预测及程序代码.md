# 量化投资_MATLAB在时间序列建模预测及程序代码 - 时海涛|Thomas - 博客园




- [博客园](https://www.cnblogs.com/)
- [首页](https://www.cnblogs.com/noah0532/)
- [新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
- [联系](https://msg.cnblogs.com/send/%E6%97%B6%E6%B5%B7%E6%B6%9B%7CThomas)
- [管理](https://i.cnblogs.com/)
- [订阅](https://www.cnblogs.com/noah0532/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





# [量化投资_MATLAB在时间序列建模预测及程序代码](https://www.cnblogs.com/noah0532/p/8724132.html)





 1　　ARMA时间序列机器特性

　　下面介绍一种重要的平稳时间序列——ARMA时间序列。

　　ARMA时间序列分为三种：

　　AR模型，auto regressiv model

　　MA模型，moving average model

　　ARMA模型，auto regressive moving average model

 　　可证ARMA时间序列具有遍历性，因此可以通过它的一个样本估计自协方差函数及自相关函数。

2　　ARMA、AR、MA模型的基础知识(略)

3　　例：随机模拟下列序列，样本容量10000，其中样本符合均值为零，方差为1的标准正太分布。**计算自相关值**

　　MATLAB代码如下：

%% DEMO1
% 利用模型数据研究随机模拟下序列。计算自相关函数
clc;clear;
rng('default'); % 初始化随机种子，保持随机种子一致
elps = randn(1,10000);  % 产生10000个服从正态分布的随机数
x(1) = 0;  % 赋初始值
for j = 2:10000
    x(j) = 0.8 * x(j-1) + elps(j) - 0.4 * elps(j-1);  % 产生样本点
end
y = (x - mean(x));  % 把数据中心化处理
gama0 = var(x);  % 求样本方差
for j = 1:10
    gama(j) = y(j+1:end)*y(1:end-j)'/10000;  %求自协方差函数
end
rho = gama/gama0;  %求自相关函数
rho2 = autocorr(x); % 直接利用MATLAB工具箱求自相关函数。
disp([rho(1),rho(2),rho(4),rho(4)]);
disp([rho2(2),rho2(3),rho2(4),rho2(5)])

% 其自相关函数的计算结果基本一致
%     0.5430    0.4296    0.2551    0.2551
%     0.5430    0.4297    0.3396    0.2552




4　　**例：利用MATLAB计算自相关值**

%% DEMO2
% 利用模型数据研究随机模拟下序列。计算偏自相关函数
clc;clear;
rng('default'); % 初始化随机种子，保持随机种子一致
elps = randn(1,10000);  % 产生10000个服从正态分布的随机数
x(1) = 0;  % 赋初始值
for j = 2:10000
    x(j) = 0.8 * x(j-1) + elps(j) - 0.4 * elps(j-1);  % 产生样本点
end
y = (x - mean(x));  % 把数据中心化处理
gama0 = var(x);  % 求样本方差
L = 10;
for j = 1:L
    gama(j) = y(j+1:end)*y(1:end-j)'/10000;  %求自协方差
end
rho = gama/gama0;   % 求自相关系数
f(1,1)=rho(1);
for k = 2:L
    s1 = rho(k);s2=1;  % 计算初始值
    for j = 1:k-1
        s1 = s1-rho(k-j)*f(k-1,j);
        s2 = s2-rho(j)*f(k-1,j);
        f(k,k)=s1/s2;
     end
     for j = 1:k-1
         f(k,j) = f(k-1,j)-f(k,k)*f(k-1,k-j);
     end
end
pcorr=diag(f)';  %提取偏相关函数
pcorr2=parcorr(x); %直接利用MATLAB工具箱计算偏相关函数
disp([pcorr(1),pcorr(2),pcorr(4),pcorr(4)]);
disp([pcorr2(2),pcorr2(3),pcorr2(4),pcorr2(5)])

% 利用公式和利用MATLAB工具箱计算的偏相关值基本一致
%     0.5430    0.1911    0.0057    0.0057
%     0.5431    0.1913    0.0694    0.0056



5　　**ARMA模型时间序列的建模与预报**

步骤：

　　第一：首先进行模型的识别与定价，即要判断是AR（p），MA(q)，还是ARMA（p，q）模型的类别，并估计阶数p和q。其实这都是归结到模型的定阶问题。

　　第二：当定阶后，就要对模型参数进行估计。

　　第三：定阶与参数估计完成后，还有对模型进行检验，即要检验残差是否为平稳白噪声。若检验获得通过，则ARMA模型建模完成。

　　第四：作为时间序列建模后的一个重要应用，还要讨论ARMA模型的时间序列的预报。



6　　MATLAB中的GARCH工具箱可以实现时间序列建模的功能。工具箱中的模型ARMAX（R，M，Nx）为ARMA模型函数。下面通过一些实例来进行建模的演示。



7　　MATLAB中arima模型和arimax模型的区别

**一元时间序列模型：**arma和arima，具有差分和不差分的区别

**多元时间序列模型：**arimax，时间序列一维以上的不同时间序列，在先生生活中预测往往受很多因素的影响。



7.1　　arima模型的参数说明：

　　7.1.1　　如果差分为0的话就是arma模型。

　　7.1.2　　创建

　　　　　　模式1：Md1 = arima　　% 创建一个0级别的ARIMA模型，也就是arima(0,0,0)

　　　　　　模式2：Md1 = arima(p,D,q)  % 创建一个非季节性的线性时间序列模型，其中p为AR模型的阶数，D为几阶差分，q为MA模型的阶数

　　　　　　模式3：Md1 = arima(Name,Value)  % 创建一个线性时间序列模型，采用增加额外选项、值的方式。Name是可选项，Value为选项的值。Name必须用(' ')这种内部引用方式。对应的格式可以是Name1,Value,Name2,Value2.....

　　7.1.3　　创建方法：

**方法一：**如果直接输入采用模式2的形式Md1 = arima(p,D,q)的方式；Md1 = arima(1,1,1) % 表示一阶差分的AR1,MA1模型。但是这个模型没有任何的参数。是属于首先建立整个模型的结构框架的形式：

　　　　　　因此：输出的也是一个对象引用的内容：

Md1 = 
    ARIMA(1,1,1) Model:
    --------------------
    Distribution: Name = 'Gaussian'
               P: 2
               D: 1
               Q: 1
        Constant: NaN
              AR: {NaN} at Lags [1]
             SAR: {}
              MA: {NaN} at Lags [1]
             SMA: {}
        Variance: NaN

**方法二**：对应Name-Value的变量输入方式。其实这些Name和Value的键和值也是这个对象里面的内容。因此这里把所有的键和值列出来。进行解释一下。

键：**'AR'**

解释：非季节自回归的参数。默认为空NaNs

键：**'ARLags'**

解释：与AR系数相关的正整数滞后向量。默认为1,2

键：**'Beta'**

解释：与AR系数相关的正整数滞后向量。默认[ ]；非回归系数回归内容

键：**'Constant'**

解释：线性时间序列中的标量常数，也就是**常数项**。默认NaN；

键：**'D'**

解释：非负整数表示的非季节性差分滞后算子多项式的次数（非季节性的一体化程度）在线性时间序列。默认0；

键：**'Distribution'**

解释：创新过程的条件概率分布。分布是“高斯”或“T”。或者，将其指定为具有字段名称的数据结构，以存储分布“高斯”或“T”。如果分布是“t”，则结构也需要字段自由度来存储自由度。默认高斯分布；

键：**'MA'**

解释：非季节性移动平均系数对应于可逆多项式细胞载体。当未指定malags，马是一种细胞载体的系数在滞后1，…对非季节性移动平均多项式的次数。当指定的MALags，马是一个相当长的细胞载体与MALags的滞后相关系数。默认NaN；

键：**'MALags'**

解释：与MA系数相关的正整数滞后向量。默认0；

键：**'SAR'**

解释：季节AR模型。

键：**'SARLags'**

解释：。

键：**'SMA'**

解释：。

键：**'SMALags'**

解释：。

键：**'Seasonality'**

解释：

线性时间序列模型中季节性差分滞后算子多项式的非负整数。默认为0；

键：**'Variance'**

解释：模型创新的正标量方差，或支持的条件方差模型对象（例如，GARCH模型对象）。

　　7.1.4  建模举例：

　　比如有这么一个模型ARIMA(2,1,1)　　

　　表达式如下：

![](https://images2018.cnblogs.com/blog/1328368/201804/1328368-20180406022740179-1480274954.png)

　　对应的MATLAB建模公式如下：
　　tdist = struct('Name','t','DoF',10);
　　model = arima('**Constant**',0.4,'**AR**',{0.8,-0.3},'MA',0.5,'**D**',1,'Distribution',tdist,'Variance',0.15)
　　对应对象里面的内容如下：
model = 

    ARIMA(2,1,1) Model:
    --------------------
    Distribution: Name = 't', DoF = 10
               P: 3
               D: 1
               Q: 1
        Constant: 0.4
              AR: {0.8 -0.3} at Lags [1 2]
             SAR: {}
              MA: {0.5} at Lags [1]
             SMA: {}
        Variance: 0.15
**　　解释：Constant：对应的是常数项：0.4**

**　　　　　　AR：对应的是AR模型的滞后项的参数：0.8 -0.3**

**　　　　　　MA：对应的是MA模型的参数：0.5**

**　　　　　　D：为一阶差分形式**

**　　　　　　Distribution：tdist，如果加上分布，这里用一个结构体来说明，这里指的是t分布，有10个自由度。也可以不加这个分布，默认为高斯分布。**

**　　　　　　Variance：指的是常数项的方差是0.5，在有些教材中一般都是在公式下面打一个小括号，对应每一个参数的表示方差(0.5)**







**8　　其他相关配套函数**
|[estimate](file:///E:/MATLAB/R2016b/help/econ/arima.estimate.html)|Estimate ARIMA or ARIMAX model parameters|
|----|----|
|[filter](file:///E:/MATLAB/R2016b/help/econ/arima.filter.html)|Filter disturbances using ARIMA or ARIMAX model|
|[forecast](file:///E:/MATLAB/R2016b/help/econ/arima.forecast.html)|Forecast ARIMA or ARIMAX process|
|[impulse](file:///E:/MATLAB/R2016b/help/econ/arima.impulse.html)|Impulse response function|
|[infer](file:///E:/MATLAB/R2016b/help/econ/arima.infer.html)|Infer ARIMA or ARIMAX model residuals or conditional variances|
|[print](file:///E:/MATLAB/R2016b/help/econ/arima.print.html)|Display parameter estimation results for ARIMA or ARIMAX models|
|[simulate](file:///E:/MATLAB/R2016b/help/econ/arima.simulate.html)|Monte Carlo simulation of ARIMA or ARIMAX models|



.

.

..................................














