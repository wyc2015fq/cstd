# 什么是龙格现象(Runge phenomenon)？如何避免龙格现象？ - Machine Learning with Peppa - CSDN博客





2018年04月06日 18:04:45[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：3551








在《计算方法》和《计算机图形学基础》中讲到插值（线性插值、抛物线插值、高次lagrang插值）的拟合度,在三种自由曲线的图形中，是上升趋势，我们总以为次数越高精度越高，实际上，当点数n 增大（次数m=n-1 也增大）时，有时会在两端产生激烈的震荡，出现函数不收敛的现象，即所谓的龙格现象。在数值分析中,高次插值会产生龙格现象。即在两端处波动极大,产生明显的震荡。这种现象产生的原因是什么?是截断误差的增大还是舍入误差的增大?我们简单的来看一看。


通常在我们的计算方法中，有利用多项式对某一函数的近似逼近，利用多项式就可以计算相应的函数值。例如，在事先不知道某一函数的具体形式的情况下，只能测量得知某一些分散的函数值。利用已经测的数据，应用待定系数法便可以求得一个多项式函数f（x）。应用此函数就可以计算或者说预测其他日期的气温值。一般情况下，想象一下泰勒展开，多项式的次数越多，需要的阶数越多，而信息也就越完全。但是事情并不总是这样，1901年，Carl Runge发表了他关于高次多项式插值风险的研究结果，给出一个简单的函数：

![](https://img-blog.csdn.net/20180406180116577?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM5NTIxNTU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
该函数被称为龙格函数，该函数有这么一个性质，就是你使用多项式插值来逼近，居然在次数越大的时候误差越大，这和一般的“次数越多越好”的常识有冲突了。下面是演示插值的M文件：

```python
[plain] view plain copy
for i=3:2:11  
     x=linspace(-1,1,i);  
     y=1./(1+25*x.^2);  
     p=polyfit(x,y,i-1);  
     xx=-1:0.01:1;  
     yy=polyval(p,xx);  
    plot(xx,yy,'b');  
hold on;  
grid on;  
end;  
plot(x,1./(1+25*x.^2),'r');
```

运行效果如下：

![](https://img-blog.csdn.net/20180406180222421?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM5NTIxNTU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
图中红色的才是真正的函数图形。一般吧这种次数越高而插值结果越偏离原函数的现象称为龙格现象。所以在不熟悉曲线运动趋势的前提下，不要轻易使用高次插值。            


