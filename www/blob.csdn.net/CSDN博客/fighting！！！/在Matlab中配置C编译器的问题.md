# 在Matlab中配置C编译器的问题 - fighting！！！ - CSDN博客
2018年06月14日 22:31:26[dujiahei](https://me.csdn.net/dujiahei)阅读数：78

源地址：https://blog.csdn.net/beautiful_face/article/details/53453091
这段时间在做语音信号盲源分离的问题，在编写程序的过程中使用了现成的OMP和KSVD工具包，自然引入了在matlab中配置C编译器的问题。
    起初，我使用了VS2013和matlab2013搭建开发环境，遇到了一个常见的问题，那就是输入mex - setup，提示无法找到C编译器的错误。在这个过程中，我重装过好几次软件，大家知道，这两个软件的安装和卸载很耗费时间，我是来来回回折腾了好久。也尝试过论坛或博客上的解决办法例如修改配置文件（D:\Program Files\MATLAB\R2013a\bin\win64\mexopts\compilerComponents），事实证明这个方法是没起到什么作用![哭](http://static.blog.csdn.net/xheditor/xheditor_emot/default/cry.gif)；也尝试过，在matlab安装路径下引入matlab和VS2013的关联文件，结果依然是徒劳![快哭了](http://static.blog.csdn.net/xheditor/xheditor_emot/default/fastcry.gif)。
    有文章讲，VS的版本必须低于matlab的版本，万般无赖之下，我使用了VS2010+matlab2013和VS2013+matlab2016两种方式，结果简直是完美，软件下载、安装和配置加在一起也就小半天。
    写这篇文章的目的是希望大家在遇到这个问题时，直接采用最快、最直接的方法。
