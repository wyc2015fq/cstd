# matlab代码转exe可执行软件 - 心纯净，行致远 - CSDN博客





2018年03月19日 22:21:06[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：2166标签：[matlab																[exe软件																[脚本文件																[函数文件](https://so.csdn.net/so/search/s.do?q=函数文件&t=blog)
个人分类：[【MatLab/Simulink】](https://blog.csdn.net/zhanshen112/article/category/7184373)





matlab脚本文件或者函数文件可以转成exe脱机运行的软件。下面是一个实例：

在新建的脚本文件中输入

```
x=0:0.01:pi;
y=sin(x);
plot(x,y)
```

然后保存。然后在命令行键入：
`mcc -m test1   %test1是exe程序的名字`
即可生成test1.exe程序。

**亲测脚本文件转exe确实可行，函数m文件转exe也可以。但有一点需要注意的是exe文件名必须和要转的脚本文件或者函数文件的文件名相同。**

**在转之前，需要先安装mingw**，教程参考：[http://blog.csdn.net/zhanshen112/article/details/79484031。](http://blog.csdn.net/zhanshen112/article/details/79484031)](https://so.csdn.net/so/search/s.do?q=脚本文件&t=blog)](https://so.csdn.net/so/search/s.do?q=exe软件&t=blog)](https://so.csdn.net/so/search/s.do?q=matlab&t=blog)




