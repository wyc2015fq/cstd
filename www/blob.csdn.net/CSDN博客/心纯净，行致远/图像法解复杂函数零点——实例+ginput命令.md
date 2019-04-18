# 图像法解复杂函数零点——实例+ginput命令 - 心纯净，行致远 - CSDN博客





2018年01月01日 11:24:36[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：393








复杂函数或者不易求解的函数有时通过代数方法不易直接求出答案，这时可以利用计算机的优点，通过图像法得出满足精度要求的答案。

题目：采用图解法求解（x+2）.^x=2


代码如下：



```
>> clear
>> %采用图解法求解（x+2）.^x=2
>> x=0:0.001:1;
>> y=(x+2).^x-2;
>> plot(x,y) 
>> grid on 
>> %需要先把图像放大，再运行以下命令，以便用鼠标获取交点坐标的数值
>> [x,y]=ginput(1);
>> format long

>> [x,y]=ginput(1)

x =

   0.698299365097217


y =

    -1.784653693682011e-07
```
![](https://img-blog.csdn.net/20180101112312939?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbnNoZW4xMTI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
该实例主要采用ginput命令，该命令的主要作用是用鼠标在图上获取数据。




——20180101,11:24




