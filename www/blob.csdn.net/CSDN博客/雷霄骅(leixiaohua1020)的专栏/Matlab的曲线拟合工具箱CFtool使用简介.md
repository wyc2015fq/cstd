# Matlab的曲线拟合工具箱CFtool使用简介 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月22日 14:57:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：29
个人分类：[纯编程](https://blog.csdn.net/leixiaohua1020/article/category/1647545)










Matlab有一个功能强大的曲线拟合工具箱 cftool ，使用方便，能实现多种类型的线性、非线性曲线拟合。下面结合我使用的Matlab R2011b 来简单介绍如何使用这个工具箱。

假设我们要拟合的函数形式是 y=A*x*x + B*x, 且A>0,B>0 。

1、在命令行输入数据：
 x=[110.3323148.7328178.064202.8258033224.7105244.5711262.908280.0447296.204311.5475];
 y=[5101520253035404550];
 2、启动曲线拟合工具箱
 cftool
 3、进入曲线拟合工具箱界面“Curve Fitting tool”
 （1）利用X data和Y data的下拉菜单读入数据x,y，这时会自动画出数据集的曲线图，注意右侧的Auto fit选项；
 （2）通过下拉菜单“Type of fit”选择拟合曲线的类型，工具箱提供的拟合类型有：
 Custom Equations：用户自定义的函数类型
 Exponential：指数逼近，有2种类型， a*exp(b*x) 、 a*exp(b*x) + c*exp(d*x)
 Fourier：傅立叶逼近，有7种类型，基础型是 a0 + a1*cos(x*w) + b1*sin(x*w)
 Gaussian：高斯逼近，有8种类型，基础型是 a1*exp(-((x-b1)/c1)^2)
 Interpolant：插值逼近，有4种类型，linear、nearest neighbor、cubic spline、shape-preserving
 Polynomial：多形式逼近，有9种类型，linear ~、quadratic ~、cubic ~、4-9th degree ~
 Power：幂逼近，有2种类型，a*x^b 、a*x^b + c
 Rational：有理数逼近，分子、分母共有的类型是linear ~、quadratic ~、cubic ~、4-5th degree ~；此外，分子还包括constant型
 Smoothing Spline：平滑逼近（翻译的不大恰当，不好意思）
 Sum of Sin Functions：正弦曲线逼近，有8种类型，基础型是 a1*sin(b1*x + c1)
 Weibull：只有一种，a*b*x^(b-1)*exp(-a*x^b)

选择好所需的拟合曲线类型及其子类型，并进行相关设置：
 ——如果是非自定义的类型，根据实际需要点击“Fit options”按钮，设置拟合算法、修改待估计参数的上下限等参数；
 ——如果选Custom Equations，点击“New”按钮，弹出自定义函数等式窗口，有“Linear Equations线性等式”和“General Equations构造等式”两种标签。

在本例中选Custom Equations，点击“New”按钮，选择“General Equations”标签，输入函数类型y=a*x*x + b*x，设置参数a、b的上下限，然后点击OK。

（5）类型设置完成后，点击“Apply”按钮，就可以在Results框中得到拟合结果，如下例：
 general model:
 f(x) = a*x*x+b*x

Coefficients (with 95% confidence bounds):
 a = 0.009194 (0.009019, 0.00937)
 b = 1.78e-011 (fixed at bound)

Goodness of fit:
 SSE: 6.146
 R-square: 0.997
 Adjusted R-square: 0.997
 RMSE: 0.8263

同时，也会在工具箱窗口中显示拟合曲线。

这样，就完成一次曲线拟合啦，十分方便快捷。当然，如果你觉得拟合效果不好，还可以在“Fitting”窗口点击“New fit”按钮，按照步骤（4）~（5）进行一次新的拟合。

不过，需要注意的是，cftool 工具箱只能进行单个变量的曲线拟合，即待拟合的公式中，变量只能有一个。对于混合型的曲线，例如 y = a*x + b/x ，工具箱的拟合效果并不好。




